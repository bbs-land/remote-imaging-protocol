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
  hook(0x3e2e7, (void (*)(void))upload);
  pointer(image + 0x87908, character_types);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isdigit(i) ? 4 : 0) | (isspace(i) ? 8 : 0) |
                         (isupper(i) ? 1 : 0) | (islower(i) ? 2 : 0);
  reset_checks();
  copy_checks();
  macro_checks();
  printf("PASS: %u table lifecycle checks\n", checks);
  return 0;
}
