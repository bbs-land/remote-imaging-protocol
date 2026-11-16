/* Original button initialization/definition/reset and consumers.
 * Host services are controlled; no GUI or Windows raster-parity claim. */
#include "riptel-probe-image.h"
static unsigned char ctx[512], system_state[0x5c], entries[36 * 0x1c],
    active[128], world[128], palettes[0x900], options[512];
static unsigned errors;
static const char *last_error;
static void report(void *c, unsigned level, const char *name,
                   const char *message) {
  (void)level;
  (void)name;
  require(c == ctx, "button error context");
  ++errors;
  last_error = message;
}
static unsigned allocate(void *target, unsigned size) {
  require(size == sizeof system_state,
          "original button-system allocation size");
  pointer(target, system_state);
  return 1;
}
static unsigned __attribute__((stdcall)) global_alloc(unsigned flags,
                                                      unsigned size) {
  require(flags == 0x42 && size == sizeof entries,
          "original button-table allocation size/flags");
  memset(entries, 0, sizeof entries);
  return (uintptr_t)entries;
}
static void *__attribute__((stdcall)) global_lock(void *handle) {
  require(handle == entries, "owned button table handle");
  return handle;
}
static unsigned __attribute__((stdcall)) global_unlock(void *handle) {
  require(handle == entries, "owned button table unlock");
  return 1;
}
static void prepare(unsigned scale) {
  memset(ctx, 0, sizeof ctx);
  memset(system_state, 0, sizeof system_state);
  memset(entries, 0, sizeof entries);
  memset(active, 0, sizeof active);
  memset(world, 0, sizeof world);
  memset(palettes, 0, sizeof palettes);
  memset(options, 0, sizeof options);
  pointer(ctx + 0x16, system_state);
  pointer(system_state + 4, entries);
  pointer(ctx + 2, active);
  pointer(ctx + 0xe, world);
  pointer(ctx + 0x1a, palettes);
  pointer(ctx + 0x36, options);
  put16(palettes + 0x82b, 256);
  put32(world + 0x24, 640 * scale);
  put32(world + 0x28, 480 * scale);
  put32(world + 0x2c, 640);
  put32(world + 0x30, 480);
  system_state[0xc] = 1;
  errors = 0;
  last_error = NULL;
}
static unsigned call_slot(unsigned rva, int slot) {
  return ((unsigned (*)(void *, int))(image + rva))(ctx, slot) & 0xffff;
}
static void define(uint32_t *args) {
  ((void (*)(void *, unsigned, void *, const char *))(image + 0xb325))(
      ctx, 16, args, NULL);
}
static void fixture_args(uint32_t *args) {
  memcpy(args,
         (uint32_t[]){64, 32, 2, 0x300, 8, 15, 0, 15, 8, 7, 0, 0, 15, 7, 0, 0},
         64);
}
static void defaults_checks(void) {
  prepare(1);
  require(
      (((unsigned (*)(void *))(image + 0x4588c))(ctx) & 0xffff) == 1 &&
          !errors && u16(system_state + 0xa) == 0 && !u16(system_state + 8),
      "original initialization selects button slot zero with balanced locks");
  for (unsigned n = 0; n < 36; ++n)
    require(
        !memcmp(entries + n * 0x1c, image + 0x828e8, 0x1c) &&
            system_state[0xc + n] == !n && system_state[0x30 + n] == 0,
        "original bootstrap defaults all entries, only zero initially in use");
  require(u16(entries) == 0 && u16(entries + 2) == 0 && u16(entries + 4) == 2 &&
              u16(entries + 6) == 0x300 && u16(entries + 8) == 3 &&
              u16(entries + 0xa) == 15 && u16(entries + 0xc) == 0 &&
              u16(entries + 0xe) == 15 && u16(entries + 0x10) == 8 &&
              u16(entries + 0x12) == 7 && u16(entries + 0x14) == 0 &&
              u16(entries + 0x16) == 0 && u16(entries + 0x18) == 15 &&
              u16(entries + 0x1a) == 7,
          "complete default button field values, including plain/bevel flags");
  for (unsigned n = 0; n < 36; ++n) {
    prepare(1);
    if (n)
      memset(entries + n * 0x1c, 0xa5, 0x1c);
    else
      memcpy(entries, image + 0x828e8, 0x1c);
    require(call_slot(0x4576f, n) == 1 && u16(system_state + 0xa) == n &&
                !memcmp(entries + n * 0x1c, image + 0x828e8, 0x1c) &&
                system_state[0xc + n] == 1,
            "undefined selection initializes the fixed default record");
    require(call_slot(0x4558d, n) == 1 && system_state[0xc + n] == !n &&
                !memcmp(entries + n * 0x1c, image + 0x828e8, 0x1c),
            "specific reset defaults fields but clears nonzero definition");
  }
  for (unsigned current = 0; current < 36; ++current)
    for (unsigned protect = 0; protect < 2; ++protect) {
      if (!current && protect)
        continue;
      prepare(1);
      memset(entries, 0x5a, sizeof entries);
      put16(system_state + 0xa, current);
      system_state[0xc + current] = 1;
      system_state[0x30 + current] = protect;
      unsigned char old_entries[sizeof entries], expected[sizeof system_state];
      memcpy(old_entries, entries, sizeof entries);
      memcpy(expected, system_state, sizeof expected);
      if (!protect) {
        memcpy(expected + 0xa, image + 0x828e8, 0x1c);
        expected[0xc] = 1;
      }
      require(call_slot(0x4558d, -1) == !protect && !errors &&
                  !memcmp(entries, old_entries, sizeof entries) &&
                  !memcmp(system_state, expected, sizeof expected),
              "current reset copies defaults into selection/in-use metadata, "
              "not a record; protection rejects");
    }
  prepare(1);
  memset(entries, 0x5a, sizeof entries);
  system_state[0x30 + 5] = 1;
  require(call_slot(0x4558d, -2) == 1 && !errors, "table reset executes");
  for (unsigned n = 0; n < 36; ++n)
    require(
        (n == 5 ? entries[n * 0x1c] == 0x5a
                : !memcmp(entries + n * 0x1c, image + 0x828e8, 0x1c)) &&
            system_state[0xc + n] == !n,
        "table reset defaults unprotected entries, preserves protected fields");
}
static void definition_checks(void) {
  uint32_t args[16];
  for (unsigned flags = 0; flags < 65536; ++flags) {
    prepare(1);
    fixture_args(args);
    args[3] = flags;
    memset(entries, 0x5a, 0x1c);
    define(args);
    unsigned valid = !!(flags & 0x181);
    require(!!errors == !valid,
            "style definition requires one or more type bits only");
    if (valid)
      require(u16(entries + 6) == flags &&
                  u16(entries + 8) == ((flags & 0x200) ? 8 : 0) &&
                  u16(entries) == 64 && u16(entries + 2) == 32,
              "all flags combinations retained, except unused bevel size "
              "normalized");
    else
      require(entries[0] == 0x5a, "missing type rejects without record writes");
  }
  for (unsigned scale = 1; scale <= 4; scale *= 2)
    for (unsigned bits = 0; bits <= 256; ++bits)
      for (unsigned protected = 0; protected < 2; ++protected) {
        prepare(scale);
        fixture_args(args);
        args[11] = bits;
        put16(system_state + 0xa, 5);
        system_state[0xc + 5] = 1;
        system_state[0x30 + 5] = protected;
        memset(entries + 5 * 0x1c, 0x5a, 0x1c);
        define(args);
        unsigned valid = bits < 256 && !protected;
        require(!!errors == !valid,
                "flags2 byte gate and explicit current-style protection");
        if (valid) {
          unsigned char *p = entries + 5 * 0x1c;
          require(u16(p) == 64 / scale && u16(p + 2) == 32 / scale &&
                      u16(p + 8) == 8 / scale && u16(p + 0x16) == bits && !p[5],
                  "style width/height/bevel converted at definition; flags2 "
                  "retained");
        } else
          require(entries[5 * 0x1c] == 0x5a,
                  "invalid/protected style definition unchanged");
      }
  unsigned char baseline[0x1c];
  for (unsigned other = 0; other < 36; ++other) {
    prepare(1);
    fixture_args(args);
    args[14] = other;
    define(args);
    if (!other)
      memcpy(baseline, entries, sizeof baseline);
    require(!errors && !memcmp(entries, baseline, sizeof baseline) &&
                !entries[5],
            "wire other_port field is unused by style definition");
  }
  for (unsigned orientation = 0; orientation < 6; ++orientation)
    for (unsigned group = 0; group < 38; ++group) {
      prepare(1);
      fixture_args(args);
      args[2] = orientation;
      args[10] = group;
      define(args);
      require(!!errors == !(orientation <= 4 && group < 36),
              "orientation/group validation boundaries");
    }
  prepare(1);
  fixture_args(args);
  args[3] = 0x10001;
  define(args);
  require(!errors && u16(entries + 6) == 1,
          "style flags truncate to stored low word");
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  hook(0x38e47, (void (*)(void))report);
  hook(0x45038, noop);
  hook(0x3988f, noop);
  hook(0x398ee, noop);
  hook(0x447c0, noop);
  hook(0x44802, noop);
  hook(0x69e60, (void (*)(void))memcpy);
  hook(0x68d70, (void (*)(void))memset);
  hook(0x65ca3, (void (*)(void))allocate);
  pointer(image + 0x964dc, (void *)global_alloc);
  pointer(image + 0x964e4, (void *)global_lock);
  pointer(image + 0x964f4, (void *)global_unlock);
  defaults_checks();
  definition_checks();
  printf("PASS: %u button-style checks\n", checks);
  return 0;
}
