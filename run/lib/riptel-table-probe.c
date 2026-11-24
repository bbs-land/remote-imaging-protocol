/* Original live table routines; controlled memory/GDI/host services, no GUI. */
#include "riptel-probe-image.h"
#include <ctype.h>
#include <stdarg.h>
#include <strings.h>
static unsigned char ctx[512], palette[0x900], defaults_system[0x300],
    variables[0x700];
static struct {
  uint32_t before;
  unsigned char data[36 * 0x302];
  uint32_t after;
} records;
static unsigned char defaults[0x300], before[sizeof records.data],
    expected[sizeof records.data];
static char params[21 * 16], result[8192];
static uint16_t character_types[256];
static unsigned errors, uploads;
static void report(void *c, unsigned severity, const char *where,
                   const char *message) {
  (void)c;
  (void)severity;
  (void)where;
  (void)message;
  ++errors;
}
static void formatted(char *dest, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsprintf(dest, fmt, ap);
  va_end(ap);
}
static void upload(void *c) {
  require(c == ctx, "palette upload context");
  ++uploads;
}
static unsigned char *entry(unsigned n) { return records.data + n * 0x302; }

static unsigned char style_system[16], style_records[36 * 0x61],
    button_system[0x5c], button_records[36 * 0x1c], tw_system[128],
    tw_records[36 * 0xf1], env_system[128], env_records[36 * 16],
    port_system[128], port_records[36 * 0x78], mouse_system[128],
    pen_system[32];
static void *query_allocations[16];
static unsigned query_count, allocation_failure;
static unsigned allocate_string(void **target, unsigned size) {
  require(query_count < 16 && size < 1024, "resident query allocation bound");
  if (allocation_failure) {
    *target = NULL;
    return 0;
  }
  *target = malloc(size);
  require(*target != NULL, "fixture query allocation");
  query_allocations[query_count++] = *target;
  return 1;
}
static void clear_queries(void) {
  for (unsigned i = 0; i < query_count; ++i)
    free(query_allocations[i]);
  query_count = 0;
}
static void prepare_families(void) {
  clear_queries();
  allocation_failure = 0;
  memset(style_system, 0, sizeof style_system);
  memset(style_records, 0, sizeof style_records);
  memset(button_system, 0, sizeof button_system);
  memset(button_records, 0, sizeof button_records);
  memset(tw_system, 0, sizeof tw_system);
  memset(tw_records, 0, sizeof tw_records);
  memset(env_system, 0, sizeof env_system);
  memset(env_records, 0, sizeof env_records);
  memset(port_system, 0, sizeof port_system);
  memset(port_records, 0, sizeof port_records);
  memset(mouse_system, 0, sizeof mouse_system);
  memset(pen_system, 0, sizeof pen_system);
  pointer(ctx + 0xa, style_system);
  pointer(style_system, style_records);
  put16(style_system + 8, 7);
  pointer(ctx + 0x16, button_system);
  pointer(button_system, button_records);
  put16(button_system + 0xa, 7);
  pointer(ctx + 0x12, tw_system);
  pointer(tw_system, tw_records);
  put16(tw_system + 0xa, 7);
  pointer(ctx + 0xe, env_system);
  pointer(env_system, env_records);
  put16(env_system + 0xa, 7);
  pointer(ctx + 0x22, port_system);
  pointer(port_system, port_records);
  pointer(ctx + 0x1e, mouse_system);
  pointer(ctx + 0x3a, pen_system);
  style_records[0x21] = 2;
  button_system[0xc] = 1;
  tw_records[0x27] = 0x80;
  env_records[0] = 1;
}
static void prepare(void) {
  memset(ctx, 0, sizeof ctx);
  memset(palette, 0, sizeof palette);
  memset(defaults_system, 0, sizeof defaults_system);
  memset(variables, 0, sizeof variables);
  pointer(ctx + 0x1a, palette);
  pointer(palette + 5, records.data);
  pointer(ctx + 0x72, defaults_system);
  pointer(defaults_system + 0x216, defaults);
  pointer(ctx + 0x26, variables);
  pointer(variables + 0x432, result);
  records.before = records.after = 0x1234abcd;
  for (unsigned i = 0; i < sizeof defaults; ++i)
    defaults[i] = (i * 7 + 3) % 64;
  for (unsigned n = 0; n < 36; ++n) {
    for (unsigned j = 0; j < 0x300; ++j)
      entry(n)[j] = (n * 5 + j) % 64;
    put16(entry(n) + 0x300, n ? 0 : 2);
  }
  prepare_families();
  palette[4] = 7;
  errors = uploads = 0;
  memset(params, 0, sizeof params);
  result[0] = 0;
}
static unsigned call_slot(unsigned rva, int slot) {
  return ((unsigned (*)(void *, int))(image + rva))(ctx, slot) & 0xffff;
}
static unsigned copy(unsigned source, unsigned dest) {
  return ((unsigned (*)(void *, unsigned, unsigned))(image + 0x446eb))(
             ctx, source, dest) &
         0xffff;
}
static void guards(void) {
  require(records.before == 0x1234abcd && records.after == 0x1234abcd,
          "table array guards");
}
static const char *macro(unsigned rva, unsigned n, const char *const *args) {
  memset(params, 0, sizeof params);
  put16(variables + 0x436, 0);
  for (unsigned i = 0; i < n; ++i) {
    require(strlen(args[i]) < 21, "macro fixture field width");
    strcpy(params + 21 * i, args[i]);
  }
  return ((const char *(*)(void *, unsigned, void *))(image + rva))(ctx, n,
                                                                    params);
}
static void reset_checks(void) {
  for (unsigned current = 0; current < 36; ++current)
    for (unsigned slot = 0; slot < 36; ++slot)
      for (unsigned flags = 0; flags < 4; ++flags) {
        if (!slot && flags != 2)
          continue;
        prepare();
        palette[4] = current;
        put16(entry(slot) + 0x300, flags);
        memcpy(expected, records.data, sizeof expected);
        if (!(flags & 1)) {
          memcpy(expected + slot * 0x302, defaults, sizeof defaults);
          put16(expected + slot * 0x302 + 0x300,
                slot ? (flags & ~2u) : (flags | 2));
        }
        unsigned ok = call_slot(0x440f0, slot);
        require(ok == !(flags & 1) && errors == !!(flags & 1),
                "reset respects protection");
        require(!memcmp(records.data, expected, sizeof expected) &&
                    palette[4] == current,
                "reset defaults RGB, clears nonzero definition, preserves "
                "current index");
        guards();
      }
  for (unsigned flags = 0; flags < 4; ++flags) {
    prepare();
    put16(entry(7) + 0x300, flags);
    call_slot(0x440f0, -1);
    require(u16(entry(7) + 0x300) == (flags & 1 ? flags : flags & ~2u),
            "CUR reset uses active slot");
  }
  prepare();
  for (unsigned i = 1; i < 36; ++i)
    put16(entry(i) + 0x300, i % 4);
  memcpy(expected, records.data, sizeof expected);
  for (unsigned i = 0; i < 36; ++i)
    if (!(u16(entry(i) + 0x300) & 1)) {
      memcpy(expected + i * 0x302, defaults, sizeof defaults);
      put16(expected + i * 0x302 + 0x300, i ? 0 : 2);
    }
  require(call_slot(0x440f0, -2) == 1 &&
              !memcmp(expected, records.data, sizeof expected),
          "table reset skips protected entries and clears other nonzero "
          "definitions");
  for (unsigned slot = 0; slot < 36; ++slot) {
    prepare();
    call_slot(0x442c3, slot);
    require(palette[4] == slot && call_slot(0x4460b, slot) == 1 &&
                (slot == 0 || !memcmp(entry(slot), defaults, sizeof defaults)),
            "selecting undefined palette defaults and defines it");
  }
}
static void copy_checks(void) {
  for (unsigned src = 0; src < 36; ++src)
    for (unsigned dst = 0; dst < 36; ++dst)
      for (unsigned sf = 0; sf < 4; ++sf)
        for (unsigned df = 0; df < 4; ++df) {
          if ((!src && sf != 2) || (!dst && df != 2))
            continue;
          prepare();
          put16(entry(src) + 0x300, sf);
          put16(entry(dst) + 0x300, df);
          memcpy(expected, records.data, sizeof expected);
          unsigned actual_sf = u16(entry(src) + 0x300),
                   actual_df = u16(entry(dst) + 0x300);
          unsigned ok =
              src != dst && (src == 0 || (actual_sf & 2)) && !(actual_df == 3);
          if (ok) {
            memcpy(expected + dst * 0x302, entry(src), 0x302);
            if (!dst)
              put16(expected + 0x300, actual_sf & ~1u);
          }
          require(copy(src, dst) == ok,
                  "direct palette copy definition/protection predicate");
          require(!memcmp(expected, records.data, sizeof expected) &&
                      palette[4] == 7,
                  "copy preserves full source record/flags except "
                  "destination-zero protection");
          guards();
        }
}
static void macro_checks(void) {
  const char *const reset[] = {"PAL", "5"};
  for (unsigned flags = 0; flags < 4; ++flags) {
    prepare();
    put16(entry(5) + 0x300, flags);
    macro(0x43d38, 2, reset);
    require(
        !u16(variables + 0x436) &&
            u16(entry(5) + 0x300) == (flags & 1 ? flags : 0),
        "RESET PAL macro retains reset helper definition/protection semantics");
  }
  const char *const tbl[] = {"PAL", "TBL"};
  prepare();
  for (unsigned i = 1; i < 36; ++i)
    put16(entry(i) + 0x300, 2 + (i % 2));
  macro(0x43d38, 2, tbl);
  for (unsigned i = 1; i < 36; ++i)
    require(u16(entry(i) + 0x300) == (i % 2 ? 3 : 0),
            "RESET PAL TBL definition/protection state");
  /* Protection anywhere in the destination list is checked before any write. */
  const char *const cases[][5] = {{"PAL", "5", "6", "7", "8"},
                                  {"PAL", "5", "8", "7", "6"},
                                  {"PAL", "5", "6", "NO", "8"},
                                  {"PAL", "5", "6", "5", "8"}};
  for (unsigned protected = 0; protected < 4; ++protected)
    for (unsigned n = 0; n < 4; ++n) {
      prepare();
      put16(entry(5) + 0x300, 3);
      put16(entry(7) + 0x300, protected);
      memcpy(before, records.data, sizeof before);
      macro(0x194f5, 5, cases[n]);
      unsigned reject = n >= 2 || (protected & 1);
      require(!!u16(variables + 0x436) == reject,
              "COPY prevalidates destination syntax/self/protection");
      if (reject)
        require(!memcmp(before, records.data, sizeof before),
                "rejected COPY updates no destinations");
      else
        for (unsigned d = 6; d <= 8; ++d)
          require(
              !memcmp(entry(5), entry(d), 0x302),
              "valid COPY duplicates protected source into each destination");
    }
  const char *const zero[] = {"PAL", "5", "0"};
  prepare();
  put16(entry(5) + 0x300, 3);
  macro(0x194f5, 3, zero);
  require(!u16(variables + 0x436) && u16(entry(0) + 0x300) == 2 &&
              !memcmp(entry(5), entry(0), 0x300),
          "COPY to zero clears copied protection");
  const char *const missing[] = {"PAL", "5", "6", "8"};
  prepare();
  memcpy(before, records.data, sizeof before);
  macro(0x194f5, 4, missing);
  require(u16(variables + 0x436) &&
              !memcmp(before, records.data, sizeof before),
          "undefined COPY source rejects before modifying any destination");
  const char *const port[] = {"PORT", "1", "2"};
  prepare();
  macro(0x194f5, 3, port);
  require(u16(variables + 0x436),
          "COPY port live entries rejects syntactically");
}

static const char *family_names[] = {"BUT", "STYLE", "TW", "ENV"};
static const unsigned family_numbers[] = {0, 1, 3, 7};
static const unsigned family_sizes[] = {0x1c, 0x61, 0xf1, 16};
static unsigned char *family_entry(unsigned f, unsigned n) {
  unsigned char *tables[] = {button_records, style_records, tw_records,
                             env_records};
  return tables[f] + family_sizes[f] * n;
}
static void set_family_flags(unsigned f, unsigned n, unsigned flags) {
  unsigned char *p = family_entry(f, n);
  if (f == 0) {
    button_system[0xc + n] = !!(flags & 2);
    button_system[0x30 + n] = flags & 1;
  } else if (f == 1)
    p[0x21] = flags;
  else if (f == 2)
    p[0x27] = ((flags & 2) ? 0x80 : 0) | ((flags & 1) ? 2 : 0);
  else
    p[0] = ((flags & 2) ? 1 : 0) | ((flags & 1) ? 2 : 0);
}
static unsigned family_flags(unsigned f, unsigned n) {
  unsigned char *p = family_entry(f, n);
  if (f == 0)
    return (button_system[0xc + n] ? 2 : 0) | (button_system[0x30 + n] ? 1 : 0);
  if (f == 1)
    return p[0x21] & 3;
  if (f == 2)
    return ((p[0x27] & 0x80) ? 2 : 0) | ((p[0x27] & 2) ? 1 : 0);
  return ((p[0] & 1) ? 2 : 0) | ((p[0] & 2) ? 1 : 0);
}
static unsigned family_copy(unsigned f, unsigned src, unsigned dst) {
  return ((unsigned (*)(void *, unsigned, unsigned, unsigned))(
             image + 0x18f8c))(ctx, family_numbers[f], src, dst) &
         0xffff;
}
static void family_checks(void) {
  const unsigned sources[] = {0, 5, 35}, destinations[] = {0, 6, 7, 35};
  for (unsigned f = 0; f < 4; ++f)
    for (unsigned si = 0; si < 3; ++si)
      for (unsigned di = 0; di < 4; ++di)
        for (unsigned sf = 0; sf < 4; ++sf)
          for (unsigned df = 0; df < 4; ++df) {
            unsigned src = sources[si], dst = destinations[di];
            if ((!src && sf != 2) || (!dst && df != 2))
              continue;
            prepare();
            set_family_flags(f, src, sf);
            set_family_flags(f, dst, df);
            family_entry(f, src)[4] = 73;
            unsigned source_flags = family_flags(f, src),
                     dest_flags = family_flags(f, dst);
            unsigned ok = src != dst && (src == 0 || (source_flags & 2)) &&
                          dest_flags != 3;
            unsigned actual = family_copy(f, src, dst);
            require(actual == ok,
                    "original per-family copy acceptance predicate");
            if (ok)
              require(family_entry(f, dst)[4] == 73 &&
                          family_flags(f, dst) == (f == 0 || dst == 0
                                                       ? source_flags & ~1u
                                                       : source_flags),
                      "per-family source-protection copy, button clearing, "
                      "zero exception");
          }
  for (unsigned f = 0; f < 4; ++f)
    for (unsigned df = 0; df < 4; ++df) {
      prepare();
      set_family_flags(f, 5, 3);
      set_family_flags(f, 7, df);
      family_entry(f, 5)[4] = 73;
      const char *args[] = {family_names[f], "5", "6", "7", "8"};
      macro(0x194f5, 5, args);
      require(!!u16(variables + 0x436) == !!(df & 1),
              "all copyable tables prevalidate destination protection");
      for (unsigned dst = 6; dst <= 8; ++dst)
        require(family_entry(f, dst)[4] == (df & 1 ? 0 : 73),
                "protected destination prevents all earlier/later writes");
      if (!(df & 1))
        for (unsigned dst = 6; dst <= 8; ++dst)
          require(family_flags(f, dst) == (f == 0 ? 2u : 3u),
                  "macro preserves family-specific copied protection");
      prepare();
      set_family_flags(f, 7, df);
      macro(0x194f5, 5, args);
      require(u16(variables + 0x436) && family_entry(f, 6)[4] == 0 &&
                  family_entry(f, 8)[4] == 0,
              "undefined source rejects each table before any write");
    }
  static char query[] = "$WIDTH$";
  for (unsigned fail_allocation = 0; fail_allocation < 2; ++fail_allocation) {
    prepare();
    set_family_flags(2, 5, 3);
    pointer(family_entry(2, 5) + 0x3f, query);
    allocation_failure = fail_allocation;
    require(
        family_copy(2, 5, 6) == 1,
        "text-window record copy can succeed despite query allocation failure");
    char *copy_query = (char *)(uintptr_t)u32(family_entry(2, 6) + 0x3f);
    require(fail_allocation ? !copy_query
                            : copy_query != query && !strcmp(copy_query, query),
            "individual text-window copy duplicates resident query, or loses "
            "it on allocation failure");
    clear_queries();
  }
}

static unsigned char upload_original[5];
static unsigned one(void) { return 1; }
static unsigned __attribute__((stdcall)) one_arg(unsigned a) {
  (void)a;
  return 1;
}
static unsigned __attribute__((stdcall)) two_args(unsigned a, unsigned b) {
  (void)a;
  (void)b;
  return 1;
}
static unsigned __attribute__((stdcall)) three_args(unsigned a, unsigned b,
                                                    unsigned c) {
  (void)a;
  (void)b;
  (void)c;
  return 1;
}
static unsigned __attribute__((stdcall)) set_rect(int *r, int x, int y,
                                                  int right, int bottom) {
  memcpy(r, (int[]){x, y, right, bottom}, 4 * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall))
palette_entries(unsigned handle, unsigned start, unsigned count,
                const unsigned char *p) {
  (void)handle;
  require(start == 0 && count <= 256, "original palette upload range");
  ++uploads;
  for (unsigned i = 0; i < count; ++i)
    for (unsigned a = 0; a < 3; ++a) {
      unsigned value = entry(palette[4])[3 * i + a];
      unsigned expected_value = value == 63 ? 255 : ((value * 4) & 255);
      require(p[4 * i + a] == expected_value,
              "original current palette RGB upload");
    }
  return count;
}
static void port_current_checks(void) {
  pointer(image + 0x96618, (void *)set_rect);
  for (unsigned slot = 0; slot < 36; ++slot) {
    prepare();
    port_records[0] = 1;
    put32(port_records + 0x6c, 1);
    put32(port_system + 0x3a, 640);
    put32(port_system + 0x3e, 480);
    const char *query[] = {"PORT"};
    char slot_text[16];
    snprintf(slot_text, sizeof slot_text, "%u", slot);
    const char *select[] = {"PORT", slot_text};
    const char *r = macro(0x34f7d, 1, query);
    require(r && !strcmp(r, "0"), "CUR PORT queries logical current selection");
    macro(0x34f7d, 2, select);
    require(!u16(variables + 0x436) && u16(port_system + 8) == slot &&
                call_slot(0x338bc, slot) == 1,
            "CUR PORT selects and defines undefined screen port");
    if (slot) {
      unsigned char *p = port_records + slot * 0x78;
      require(
          p[0] == 1 && u32(p + 3) == 0 && u32(p + 7) == 0 &&
              u32(p + 0xb) == 640 && u32(p + 0xf) == 480 &&
              u32(p + 0x42) == 640 && u32(p + 0x46) == 480,
          "undefined selected port receives full-screen geometry and viewport");
    }
    r = macro(0x34f7d, 1, query);
    require(r && !strcmp(r, slot_text),
            "CUR PORT returns selected slot in decimal");
  }
}
static void header_upload_checks(void) {
  memcpy(image + 0x3e2e7, upload_original, 5);
  pointer(image + 0x96474, (void *)palette_entries);
  pointer(image + 0x963f4, (void *)three_args);
  pointer(image + 0x963e0, (void *)one_arg);
  hook(0x32cc5, (void (*)(void))one);
  for (unsigned flag = 64; flag <= 128; flag += 64)
    for (unsigned status = 0; status < 4; ++status)
      for (unsigned guard_bit = 0; guard_bit < 2; ++guard_bit) {
        prepare();
        put16(palette + 0x82b, 16);
        put16(entry(7) + 0x300, status);
        records.before = guard_bit ? 0x1236abcd : 0x1234abcd;
        memcpy(before, records.data, sizeof before);
        ((void (*)(void *, unsigned, unsigned))(image + 0x172c5))(ctx, flag, 5);
        require(!errors &&
                    uploads == (flag == 64 ? status == 2
                                           : guard_bit && !(status & 1)) &&
                    !memcmp(before, records.data, sizeof before),
                "header uploads without defaulting; bit128 guard reads "
                "preceding allocation flags");
      }
}
static unsigned char ansi_system[128];
static void whole_reset_checks(void) {
  /* Keep original reset orchestration and all palette operations; other
   * subsystems are controlled and their reset results are not asserted. */
  const unsigned hooks[] = {
      0x27f8f, 0x3e41a, 0x3c1cf, 0x3393c, 0x33112, 0x3326f, 0x13c69, 0x35320,
      0x394fe, 0x11210, 0x3dce0, 0x10f0,  0x13d0,  0x33cc6, 0xe0dc,  0x3170d,
      0x8577,  0x3d7d1, 0x3468a, 0x16b97, 0x33d82, 0x3e8eb, 0x3e943, 0x3e989,
      0x3e9d4, 0x31052, 0x3dc72, 0x3e126, 0x3e254, 0x18e33, 0x37e41, 0x23f20,
      0x24660, 0x454c4, 0x4576f, 0x390e4, 0x3378a, 0x2753a, 0x2fcf6, 0x3d8d0};
  for (unsigned i = 0; i < sizeof hooks / sizeof hooks[0]; ++i)
    hook(hooks[i], (void (*)(void))one);
  pointer(image + 0x96418, (void *)one_arg);
  pointer(image + 0x9667c, (void *)three_args);
  pointer(image + 0x963d0, (void *)one_arg);
  pointer(image + 0x963d4, (void *)three_args);
  pointer(image + 0x963c0, (void *)two_args);
  pointer(image + 0x963f0, (void *)two_args);
  pointer(image + 0x96400, (void *)two_args);
  pointer(image + 0x963f8, (void *)two_args);
  hook(0x32c1f, (void (*)(void))one);
  for (unsigned hard = 0; hard < 2; ++hard)
    for (unsigned flags = 0; flags < 4; ++flags) {
      prepare();
      pointer(ctx + 0x46, ansi_system);
      put16(palette + 0x82b, 16);
      for (unsigned i = 1; i < 36; ++i)
        put16(entry(i) + 0x300, flags);
      memcpy(before, records.data, sizeof before);
      const char *args[] = {hard ? "HARD" : "SOFT"};
      macro(0x43d38, 1, args);
      require(!errors && !u16(variables + 0x436),
              "original SOFT/HARD macro and outer reset execute");
      require(u16(style_records) == 15 && u16(style_records + 2) == 0 &&
                  style_records[5] == 0 && u16(style_records + 6) == 1 &&
                  style_records[9] == 1 && u16(style_records + 0x1a) == 15 &&
                  !style_records[0x24] && !u16(style_records + 0x25),
              "original SOFT/HARD style reset uses fixed classic defaults");
      for (unsigned i = 1; i < 36; ++i) {
        require(u16(entry(i) + 0x300) == (hard ? 0u : (flags & 1 ? flags : 0u)),
                "soft preserves protected palettes; hard removes protection "
                "and all nonzero definitions");
        require(
            !memcmp(entry(i),
                    hard || !(flags & 1) ? defaults : before + i * 0x302,
                    0x300),
            "SOFT/HARD palette RGB defaults respect their protection policy");
      }
      require(call_slot(0x4460b, 0) == 1 && palette[4] == (hard ? 0 : 7),
              "hard selects palette zero; soft retains now-possibly-undefined "
              "palette selection");
      guards();
    }
}

static unsigned char saved_stream[8192], save_record[0x87];
static unsigned stream_length, stream_position;
static unsigned stream_open(void *c, void *record, unsigned mode) {
  require(c == ctx && record == save_record && (mode == 2 || mode == 3),
          "controlled backup stream mode");
  stream_position = 0;
  if (mode == 2)
    stream_length = 0;
  return 1;
}
static unsigned stream_write(void *c, void *record, const void *data,
                             unsigned size) {
  size &= 0xffff; /* Original stream helper reads only the low word. */
  require(c == ctx && record == save_record &&
              stream_length + size <= sizeof saved_stream,
          "backup write bound");
  memcpy(saved_stream + stream_length, data, size);
  stream_length += size;
  return 1;
}
static unsigned stream_read(void *c, void *record, void *data, unsigned size) {
  size &= 0xffff;
  require(c == ctx && record == save_record &&
              stream_position + size <= stream_length,
          "backup read bound");
  memcpy(data, saved_stream + stream_position, size);
  stream_position += size;
  return 1;
}
static unsigned stream_close(void *c, void *record, unsigned mode) {
  (void)c;
  (void)record;
  (void)mode;
  return 1;
}
static void backup_query_checks(void) {
  hook(0x2148a, (void (*)(void))stream_open);
  hook(0x215a5, (void (*)(void))stream_write);
  hook(0x21777, (void (*)(void))stream_read);
  hook(0x21543, (void (*)(void))stream_close);
  hook(0x343e4, noop);
  prepare();
  put16(port_system + 8, 5);
  static char global_query[] = "$WIDTH$", local_query[] = "$HEIGHT$";
  pointer(port_system + 0x2a, global_query);
  unsigned char *port = port_records + 5 * 0x78;
  port[0] = 1;
  put16(port + 1, 5);
  port[0x17] = 1;
  pointer(port + 0x64, local_query);
  const uint32_t saved_image[] = {11, 22, 333, 444};
  memcpy(port + 0x4a, saved_image, sizeof saved_image);
  put16(port + 0x5a, 43);
  put32(port + 0x5c, 515);
  put32(port + 0x60, 616);
  unsigned char old_styles[sizeof style_records], old_env[sizeof env_records];
  memcpy(old_styles, style_records, sizeof style_records);
  memcpy(old_env, env_records, sizeof env_records);
  require(((unsigned (*)(void *, void *))(image + 0x219a9))(ctx, save_record) ==
              1,
          "original whole-port serializer saves resident queries");
  memset(port_records, 0, sizeof port_records);
  put16(port_system + 8, 0);
  pointer(port_system + 0x2a, NULL);
  port[0] = 1;
  port[0x17] = 1; /* existing protected live target */
  put32(port + 0x5c, 717);
  put32(port + 0x60, 818);
  require(((unsigned (*)(void *, void *))(image + 0x22226))(ctx, save_record) ==
                  1 &&
              stream_position == stream_length,
          "original whole-port restore consumes complete stream");
  char *restored_global = (char *)(uintptr_t)u32(port_system + 0x2a);
  char *restored_local = (char *)(uintptr_t)u32(port + 0x64);
  require(restored_global && restored_global != global_query &&
              !strcmp(restored_global, global_query) && restored_local &&
              restored_local != local_query &&
              !strcmp(restored_local, local_query),
          "whole-table port backup restores independent global/per-port query "
          "strings");
  require(u16(port_system + 8) == 5 && port[0] == 1 && port[0x17] == 1,
          "whole-port backup restores selection and saved protection over "
          "protected live entry");
  require(!memcmp(port + 0x4a, saved_image, sizeof saved_image) &&
              u16(port + 0x5a) == 43 && u32(port + 0x5c) == 717 &&
              u32(port + 0x60) == 818 &&
              !memcmp(old_styles, style_records, sizeof style_records) &&
              !memcmp(old_env, env_records, sizeof env_records),
          "port restore recovers image, discards saved cursor, changes no "
          "style/environment");
  clear_queries();
}
static void observable_checks(void) {
  const char *slot[] = {"PAL", "5"};
  for (unsigned flags = 0; flags < 4; ++flags) {
    prepare();
    put16(entry(5) + 0x300, flags);
    const char *value = macro(0x12fbd, 2, slot);
    require(value && !strcmp(value, flags & 2 ? "1" : "0"),
            "INUSE PAL exposes exact definition flag");
    value = macro(0x2daaa, 2, slot);
    require(value && !strcmp(value, !(flags & 2) ? "-1"
                                    : flags & 1  ? "1"
                                                 : "0"),
            "ISPROT PAL numeric slot returns -1 when undefined");
    palette[4] = 5;
    const char *current[] = {"PAL", "CUR"};
    value = macro(0x2daaa, 2, current);
    require(
        value && !strcmp(value, flags & 1 ? "1" : "0"),
        "ISPROT CUR tests current protection without numeric-slot INUSE check");
  }
}

static void palette_sequence_checks(void) {
  prepare();
  const char *which[] = {"PAL", "5"}, *current[] = {"PAL"};
  macro(0x34f7d, 2, which);
  entry(5)[0] = 43;
  macro(0x43d38, 2, which);
  const char *value = macro(0x12fbd, 2, which);
  require(value && !strcmp(value, "0"),
          "select modify reset leaves palette five undefined");
  value = macro(0x2daaa, 2, which);
  require(value && !strcmp(value, "-1"),
          "reset palette five ISPROT numeric query returns -1");
  value = macro(0x34f7d, 1, current);
  require(value && !strcmp(value, "5"),
          "reset preserves logical current palette even when undefined");
  macro(0x34f7d, 2, which);
  value = macro(0x12fbd, 2, which);
  require(value && !strcmp(value, "1") &&
              !memcmp(entry(5), defaults, sizeof defaults),
          "explicit reselection defines reset palette with defaults again");
}

/* Issue 24: original selection/state paths, with only OS presentation and
 * brush realization controlled. Run before whole_reset_checks installs its
 * broader hooks. */
static unsigned char active_config[128], state_options[512];
static unsigned selected_cursor;
static unsigned __attribute__((stdcall)) cursor_capture(unsigned cursor) {
  selected_cursor = cursor;
  return 1;
}
static void wire_state(unsigned rva, unsigned a, unsigned b) {
  uint32_t args[] = {a, b};
  ((void (*)(void *, unsigned, void *, const char *))(image + rva))(ctx, 2,
                                                                    args, NULL);
}
static void state_prepare(void) {
  prepare();
  memset(active_config, 0, sizeof active_config);
  memset(state_options, 0, sizeof state_options);
  pointer(ctx + 2, active_config);
  pointer(ctx + 0x36, state_options);
  put16(env_system + 0xa, 0);
  put16(style_system + 8, 0);
  put32(env_system + 0x24, 640);
  put32(env_system + 0x28, 480);
  put32(env_system + 0x2c, 640);
  put32(env_system + 0x30, 480);
  for (unsigned n = 0; n < 36; ++n) {
    memcpy(style_records + n * 0x61, image + 0x762e8, 0x61);
    style_records[n * 0x61 + 0x21] = 2;
    unsigned char *port = port_records + n * 0x78;
    port[0] = 1;
    put16(port + 1, n);
    put32(port + 0x6c, 1);
    put32(port + 0x4a, 100 + n);
    put32(port + 0x4e, 200 + n);
    put32(port + 0x52, 300 + n);
    put32(port + 0x56, 400 + n);
    put16(port + 0x5a, n);
    put32(port + 0x5c, 500 + n);
    put32(port + 0x60, 600 + n);
  }
  for (unsigned n = 0; n < 8; ++n)
    put32(mouse_system + 0x24 + 4 * n, 1000 + n);
  selected_cursor = 0;
}
/* Issue #54: original wire switch, protection, selection and screen creation.
 */
static unsigned protection_result(unsigned old, unsigned pair) {
  if (pair & 2)
    return 0;
  return pair & 1 ? 1 : old;
}
static void switch_port_checks(void) {
  state_prepare();
  hook(0x45038, noop); /* unrelated keyboard/mouse pending-state cleanup */
  pointer(image + 0x96618, (void *)set_rect);
  put32(port_system + 0x3a, 640);
  put32(port_system + 0x3e, 480);
  unsigned char baseline[sizeof port_records];
  for (unsigned n = 0; n < 36; ++n) {
    unsigned char *p = port_records + n * 0x78;
    p[0] = n ? 4 : 1;
    p[0x17] = 0xa4;
    put32(p + 0x6c, 100 + n);
  }
  memcpy(baseline, port_records, sizeof baseline);
  for (unsigned source = 0; source < 36; ++source)
    for (unsigned target = 0; target < 36; ++target)
      for (unsigned kind = 0; kind < 4; ++kind) {
        /* kind 0=defined screen, 1=defined offscreen, 2=absent,
         * 3=absent with stale protection (construction must fail). */
        if ((kind >= 2 && (!target || source == target)))
          continue;
        for (unsigned initial = 0; initial < 4; ++initial)
          for (unsigned flags = 0; flags < 16; ++flags) {
            memcpy(port_records, baseline, sizeof port_records);
            unsigned char *src = port_records + source * 0x78;
            unsigned char *dst = port_records + target * 0x78;
            if (kind >= 2)
              memset(dst, 0, 0x78);
            else
              dst[0] = kind ? 4 : 1;
            if (source)
              src[0x17] |= initial & 1;
            if (target) {
              if (source != target)
                dst[0x17] |= (initial >> 1) & 1;
              if (kind >= 2)
                dst[0x17] = kind == 3;
            }
            unsigned old_src = src[0x17] & 1, old_dst = dst[0x17] & 1;
            unsigned char prior[sizeof port_records];
            memcpy(prior, port_records, sizeof prior);
            put16(port_system + 8, source);
            put32(ctx + 0x62, u32(src + 0x6c));
            errors = 0;
            wire_state(0x468eb, target, flags);
            unsigned current = kind == 3 ? 0 : target;
            unsigned wanted_src =
                source ? protection_result(old_src, flags >> 2) : 0;
            unsigned wanted_dst = kind == 2 ? 0 : old_dst;
            if (source == target)
              wanted_dst = wanted_src;
            if (current)
              wanted_dst = protection_result(wanted_dst, flags & 3);
            if (source == target)
              wanted_src = wanted_dst;
            require(u16(port_system + 8) == current && errors == (kind == 3),
                    "wire selects/creates target or falls back after "
                    "construction failure");
            require((src[0x17] & 1) == wanted_src &&
                        (dst[0x17] & 1) == wanted_dst &&
                        !(port_records[0x17] & 1),
                    "source first, destination last, unprotect wins, port-zero "
                    "immune");
            unsigned device = current && kind == 1 ? u32(dst + 0x6c)
                                                   : u32(port_records + 0x6c);
            require(u32(ctx + 0x62) == device,
                    "original selector and activation bind correct "
                    "screen/offscreen DC");
            for (unsigned n = 0; n < 36; ++n) {
              if (n == source || n == target)
                continue;
              require(
                  !memcmp(prior + n * 0x78, port_records + n * 0x78, 0x78),
                  "unrelated ports unchanged by conflicting flags or failure");
            }
            if (kind == 2)
              require(dst[0] == 1 && u16(dst + 1) == target &&
                          u32(dst + 3) == 0 && u32(dst + 7) == 0 &&
                          u32(dst + 0xb) == 640 && u32(dst + 0xf) == 480 &&
                          u32(dst + 0x42) == 640 && u32(dst + 0x46) == 480 &&
                          u32(dst + 0x6c) == u32(port_records + 0x6c),
                      "ordinary missing destination becomes a full-screen "
                      "screen port");
            if (kind == 3)
              require(!memcmp(dst, prior + target * 0x78, 0x78),
                      "failed protected-undefined target retained; destination "
                      "flags affect fallback only");
          }
      }
  /* The two-character MegaNum field can carry 0..1295 at base 36;
   * bits above the four documented controls are not validation errors. */
  for (unsigned flags = 0; flags < 1296; ++flags) {
    memcpy(port_records, baseline, sizeof port_records);
    put16(port_system + 8, 1);
    errors = 0;
    wire_state(0x468eb, 7, flags);
    require(
        !errors && u16(port_system + 8) == 7 &&
            (port_records[0x78 + 0x17] & 1) ==
                protection_result(0, (flags >> 2) & 3) &&
            (port_records[7 * 0x78 + 0x17] & 1) ==
                protection_result(0, flags & 3),
        "all base36 two-digit flags use only four low bits without rejection");
  }
  for (unsigned blocked = 0; blocked < 2; ++blocked)
    for (unsigned target = 0; target <= 36; ++target)
      for (unsigned flags = 0; flags < 16; ++flags) {
        if (!blocked && target < 36)
          continue;
        memcpy(port_records, baseline, sizeof port_records);
        put16(port_system + 8, 1);
        errors = 0;
        put16(state_options + 0x104, blocked);
        wire_state(0x468eb, target, flags);
        require(u16(port_system + 8) == 1 &&
                    !memcmp(port_records, baseline, sizeof baseline) &&
                    errors == !blocked,
                "disabled graphics or out-of-range target stops before all "
                "protection writes");
      }
  put16(state_options + 0x104, 0);
}
static void state_ownership_checks(void) {
  hook(0x11210, noop); /* GDI brush construction, traced separately in #12. */
  hook(0x45038, noop);
  pointer(image + 0x963d4, (void *)three_args);
  pointer(image + 0x963d0, (void *)one_arg);
  pointer(image + 0x96670, (void *)cursor_capture);
  pointer(image + 0x966e4, (void *)one_arg);
  hook(0x2148a, (void (*)(void))stream_open);
  hook(0x215a5, (void (*)(void))stream_write);
  hook(0x21777, (void (*)(void))stream_read);
  hook(0x21543, (void (*)(void))stream_close);
  state_prepare();
  require(u16(style_records) == 15 && u16(style_records + 2) == 0 &&
              style_records[4] == 0 && style_records[5] == 0 &&
              u16(style_records + 6) == 1 && style_records[8] == 0 &&
              style_records[9] == 1 && u16(style_records + 0x1a) == 15 &&
              style_records[0x1c] == 0 && style_records[0x1d] == 1 &&
              u16(style_records + 0x1e) == 0 && style_records[0x20] == 0 &&
              u16(style_records + 0x22) == 1 && style_records[0x24] == 0 &&
              u16(style_records + 0x25) == 0 && !style_records[0x27] &&
              u16(style_records + 0x4f) == 0 &&
              u16(style_records + 0x5f) == 100,
          "original fixed style defaults, including exact palette indices");
  for (unsigned n = 1; n < 36; ++n) {
    state_prepare();
    put16(style_records, 23);
    style_records[n * 0x61 + 0x21] = 0;
    wire_state(0x46e41, n, 0);
    require(!errors && u16(style_system + 8) == n &&
                u16(style_records + n * 0x61) == 15 &&
                style_records[n * 0x61 + 0x21] == 2,
            "wire first-use SWITCH_STYLE uses fixed defaults, not old style");
    require(call_slot(0x391fd, n) == 1 && u16(style_records + n * 0x61) == 15,
            "original style initializer uses same fixed record");
  }
  for (unsigned n = 0; n < 36; ++n)
    for (unsigned m = 0; m < 36; ++m) {
      state_prepare();
      unsigned char original_ports[sizeof port_records];
      unsigned char original_env[sizeof env_records];
      memcpy(original_ports, port_records, sizeof port_records);
      memcpy(original_env, env_records, sizeof env_records);
      put16(port_system + 8, n);
      wire_state(0x46e41, m, 0);
      require(u16(port_system + 8) == n &&
                  !memcmp(port_records, original_ports, sizeof port_records) &&
                  !memcmp(env_records, original_env, sizeof env_records),
              "style selection changes no port cursor/image or environment");
      unsigned char original_styles[sizeof style_records];
      memcpy(original_styles, style_records, sizeof style_records);
      require(call_slot(0x3393c, m) == 1 && u16(style_system + 8) == m &&
                  !memcmp(style_records, original_styles, sizeof style_records),
              "port selection changes no style selection or record");
      require(!memcmp(port_records, original_ports, sizeof port_records),
              "port selection retains all independent cursor/image values");
    }
  for (unsigned style_protected = 0; style_protected < 2; ++style_protected)
    for (unsigned env_protected = 0; env_protected < 2; ++env_protected) {
      state_prepare();
      put16(style_system + 8, 5);
      style_records[5 * 0x61 + 0x21] = 2 | style_protected;
      env_records[0] = 1 | (env_protected << 1);
      unsigned char old_styles[sizeof style_records];
      memcpy(old_styles, style_records, sizeof style_records);
      wire_state(0x1f3fd, 1, 8);
      wire_state(0xdc96, 6, 0);
      require(
          env_records[7] == (env_protected ? 0 : 8) &&
              active_config[0x3a] == (env_protected ? 0 : 8) &&
              env_records[8] == (env_protected ? 0 : 6) &&
              selected_cursor == (env_protected ? 0u : 1006u) &&
              !memcmp(old_styles, style_records, sizeof style_records),
          "wire color/mouse owned by environment; style protection irrelevant");
      errors = 0;
      const char *mouse[] = {"4"};
      macro(0x34750, 1, mouse);
      require(!errors && env_records[8] == 4 && selected_cursor == 1004,
              "MCURSOR side effect bypasses environment protection");
      const char *current_color[] = {"CUR", "RGB", "7"};
      macro(0x17eac, 3, current_color);
      require(!errors && env_records[7] == 7 && active_config[0x3a] == 7 &&
                  !memcmp(old_styles, style_records, sizeof style_records),
              "current COLORMODE macro also bypasses environment protection");
    }
  for (unsigned target_flags = 0; target_flags < 4; ++target_flags) {
    state_prepare();
    env_records[5 * 16] = target_flags;
    const char *color[] = {"5", "RGB", "7"};
    macro(0x17eac, 3, color);
    require(!!u16(variables + 0x436) == !(target_flags & 1) &&
                env_records[5 * 16 + 7] == (target_flags & 1 ? 7 : 0) &&
                env_records[5 * 16] == target_flags && !active_config[0x3a] &&
                !env_records[7],
            "noncurrent color macro requires definition, bypasses protection, "
            "edits only target");
  }
  for (unsigned protected_old = 0; protected_old < 2; ++protected_old) {
    state_prepare();
    env_records[0] = 1 | (protected_old << 1);
    env_records[5] = 36;
    env_records[6] = 2;
    env_records[7] = 1;
    env_records[8] = 2;
    unsigned char *target = env_records + 5 * 16;
    target[0] = 3;
    target[5] = 64;
    target[6] = 4;
    target[7] = 8;
    target[8] = 6;
    put16(target + 1, 1280);
    put16(target + 3, 960);
    unsigned char old_styles[sizeof style_records],
        old_ports[sizeof port_records];
    memcpy(old_styles, style_records, sizeof style_records);
    memcpy(old_ports, port_records, sizeof port_records);
    wire_state(0x46da0, 5, 0);
    require(!errors && u16(env_system + 0xa) == 5 &&
                active_config[0x38] == 64 && active_config[0x39] == 4 &&
                active_config[0x3a] == 8 && u32(env_system + 0x24) == 1280 &&
                u32(env_system + 0x28) == 960,
            "environment selection activates target math/color/world despite "
            "protection");
    require(
        env_records[5] == 64 && env_records[6] == 4 && env_records[7] == 8 &&
            env_records[8] == 2 && target[8] == 6,
        "switch setters overwrite departing math/color fields, retain mouse");
    require(
        u16(style_system + 8) == 0 && u16(port_system + 8) == 0 &&
            palette[4] == 7 && u16(button_system + 0xa) == 7 &&
            u16(tw_system + 0xa) == 7 &&
            !memcmp(old_styles, style_records, sizeof style_records) &&
            !memcmp(old_ports, port_records, sizeof port_records),
        "environment switch does not swap other table selections or states");
    ((void (*)(void *))(image + 0x34635))(ctx);
    require(selected_cursor == 1006 && u16(env_system + 0x1e) == 1,
            "target environment supplies cursor at next presentation refresh");
    wire_state(0x46da0, 0, 0);
    require(active_config[0x3a] == 8 && active_config[0x38] == 64 &&
                active_config[0x39] == 4,
            "switching back exposes overwritten departing fields");
  }
  /* Save scope and original style-restore index destination defect. */
  state_prepare();
  put16(style_system + 8, 5);
  put16(style_records + 7 * 0x61, 73);
  require(((unsigned (*)(void *, void *))(image + 0x217f7))(ctx, save_record) ==
              1,
          "original whole-style serializer runs");
  require(stream_length == 2 + sizeof style_records,
          "style backup contains current index and exactly 36 style records");
  put16(style_system + 8, 7);
  memset(style_records, 0, sizeof style_records);
  unsigned char old_ports[sizeof port_records], old_env[sizeof env_records];
  memcpy(old_ports, port_records, sizeof port_records);
  memcpy(old_env, env_records, sizeof env_records);
  unsigned restore_result =
      ((unsigned (*)(void *, void *))(image + 0x218bf))(ctx, save_record);
  require(
      (restore_result & 0xffff) == 1 && stream_position == stream_length &&
          u16(style_system + 8) == 7 && u16(style_records + 7 * 0x61) == 73,
      "style restore overwrites transient foreground, not saved current index");
  require(!memcmp(old_ports, port_records, sizeof port_records) &&
              !memcmp(old_env, env_records, sizeof env_records),
          "style save/restore captures no port or environment state");
}

int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  hook(0x38e47, (void (*)(void))report);
  hook(0x69e60, (void (*)(void))memcpy);
  hook(0x68d70, (void (*)(void))memset);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x65442, (void (*)(void))strcat);
  hook(0x654a8, (void (*)(void))strlen);
  hook(0x655e2, (void (*)(void))strcasecmp);
  hook(0x65604, (void (*)(void))atoi);
  hook(0x68440, (void (*)(void))formatted);
  hook(0x447c0, noop);
  hook(0x44802, noop);
  memcpy(upload_original, image + 0x3e2e7, 5);
  hook(0x3e2e7, (void (*)(void))upload);
  pointer(image + 0x87908, character_types);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isdigit(i) ? 4 : 0) | (isspace(i) ? 8 : 0) |
                         (isupper(i) ? 1 : 0) | (islower(i) ? 2 : 0);
  unsigned char original_delete[5], original_env_switch[5];
  memcpy(original_delete, image + 0x391fd, 5);
  memcpy(original_env_switch, image + 0x3dac0, 5);
  const unsigned host_hooks[] = {0x3988f, 0x398ee, 0x2badd, 0x2bb24,
                                 0x391fd, 0x4558d, 0x39ac8, 0x3c37e,
                                 0xe6e9,  0x18cf6, 0x18bf7, 0x3dac0};
  for (unsigned i = 0; i < sizeof host_hooks / sizeof host_hooks[0]; ++i)
    hook(host_hooks[i], noop);
  hook(0x65ca3, (void (*)(void))allocate_string);
  reset_checks();
  copy_checks();
  macro_checks();
  family_checks();
  port_current_checks();
  header_upload_checks();
  memcpy(image + 0x391fd, original_delete, 5);
  memcpy(image + 0x3dac0, original_env_switch, 5);
  state_ownership_checks();
  switch_port_checks();
  hook(0x3dac0, noop);
  whole_reset_checks();
  backup_query_checks();
  observable_checks();
  palette_sequence_checks();
  clear_queries();
  printf("PASS: %u table lifecycle checks\n", checks);
  return 0;
}
