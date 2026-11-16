/* Original button initialization/definition/reset and consumers.
 * Host services are controlled; no GUI or Windows raster-parity claim. */
#include "riptel-probe-image.h"
#include <ctype.h>
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

static unsigned char ctor_ports[128], ctor_port_entries[36 * 0x78], mouse[128],
    mouse_entries[128 * 0xa3], graphics[16], graphics_entry[0x61],
    env_entries[36 * 16], captured_field[0xa3];
static uint16_t character_types[256];
static char scratch_text[4096];
static unsigned drawn;
static unsigned char original_draw[5], original_effects[5][5];
static const unsigned effect_rvas[] = {0x104e0, 0x10a50, 0x106e0, 0x103c0,
                                       0x10c00};
static void *scratch(void *c) {
  require(c == ctx, "button owned scratch context");
  return scratch_text;
}
static void scratch_release(void *c, void *p) {
  require(c == ctx && p == scratch_text, "button scratch release");
}
static unsigned capture_draw(void *c, void *field, unsigned a, unsigned b,
                             unsigned d) {
  require(c == ctx && (a & 0xffff) == 1 && (b & 0xffff) == 0 &&
              (d & 0xffff) == 1,
          "original first draw dispatch arguments");
  memcpy(captured_field, field, sizeof captured_field);
  ++drawn;
  return 0;
}
static void creation_prepare(unsigned scale) {
  prepare(scale);
  memset(ctor_ports, 0, sizeof ctor_ports);
  memset(ctor_port_entries, 0, sizeof ctor_port_entries);
  memset(mouse, 0, sizeof mouse);
  memset(mouse_entries, 0, sizeof mouse_entries);
  memset(graphics, 0, sizeof graphics);
  memset(graphics_entry, 0, sizeof graphics_entry);
  memset(env_entries, 0, sizeof env_entries);
  memset(captured_field, 0, sizeof captured_field);
  pointer(ctx + 0x22, ctor_ports);
  pointer(ctor_ports, ctor_port_entries);
  put16(ctor_ports + 0xa, 2);
  ctor_port_entries[0] = 1;
  memcpy(ctor_port_entries + 0x3a, (int[]){5, 7, 645, 487}, 16);
  pointer(ctx + 0x1e, mouse);
  pointer(mouse, mouse_entries);
  pointer(ctx + 0xa, graphics);
  pointer(graphics, graphics_entry);
  graphics_entry[5] = 0;
  put16(graphics_entry + 6, 1);
  pointer(world, env_entries);
  put16(env_entries + 1, 640 * scale);
  put16(env_entries + 3, 480 * scale);
  drawn = 0;
}
static void create(uint32_t *args, const char *text) {
  ((void (*)(void *, unsigned, void *, const char *))(image + 0xa952))(
      ctx, 7, args, text);
}
static void creation_checks(void) {
  hook(0x15396, (void (*)(void))scratch);
  hook(0x153fe, (void (*)(void))scratch_release);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x654a8, (void (*)(void))strlen);
  hook(0x68b20, (void (*)(void))toupper);
  memcpy(original_draw, image + 0x2a4d3, 5);
  hook(0x2a4d3, (void (*)(void))capture_draw);
  const unsigned host[] = {0x18cf6, 0x18bf7, 0x343e4, 0x342ad, 0x34319};
  for (unsigned i = 0; i < sizeof host / sizeof host[0]; ++i)
    hook(host[i], noop);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isdigit(i) ? 4 : 0) | (isspace(i) ? 8 : 0) |
                         (isupper(i) ? 1 : 0) | (islower(i) ? 2 : 0);
  pointer(image + 0x87908, character_types);
  for (unsigned scale = 1; scale <= 4; scale *= 2)
    for (unsigned w = 0; w < 2; ++w)
      for (unsigned h = 0; h < 2; ++h) {
        creation_prepare(scale);
        uint32_t style[16];
        fixture_args(style);
        style[0] = w ? 64 : 0;
        style[1] = h ? 32 : 0;
        define(style);
        uint32_t xy[] = {20, 40, 180, 160, 0, 0, 0};
        create(xy, "<><>");
        unsigned x0 = 5 + 20 / scale, y0 = 7 + 40 / scale;
        unsigned x1 = w && h ? x0 + 64 / scale : 5 + 180 / scale;
        unsigned y1 = w && h ? y0 + 32 / scale : 7 + 160 / scale;
        require(!errors && drawn == 1 && u32(captured_field) == x0 &&
                    u32(captured_field + 4) == y0 &&
                    u32(captured_field + 8) == x1 &&
                    u32(captured_field + 0xc) == y1 &&
                    captured_field[0x2d] == 8 / scale,
                "fixed dimensions require both nonzero; mixed zero uses "
                "complete dynamic rectangle");
        require(u16(captured_field + 0x3a) == 640 * scale &&
                    u16(captured_field + 0x3c) == 480 * scale,
                "button field captures creation world dimensions independently "
                "of device geometry");
        creation_prepare(scale);
        fixture_args(style);
        style[0] = w ? 64 : 0;
        style[1] = h ? 32 : 0;
        define(style);
        uint32_t no_far[] = {20, 40, 0, 0, 0, 0, 0};
        create(no_far, "<><>");
        require(!errors && drawn == (w && h),
                "zero dynamic end with one/both style sizes zero skips empty "
                "nonmouse plain button");
      }
}

static unsigned fill_count, fill_mode, pattern_count, clip_count, clip_ok;
static unsigned paste_count, bitmap_count, bitmap_fail, effect_count[4];
static int last_fill[4], last_clip[4], label_x, label_y;
static unsigned label_count;
static char bitmap_names[2][128];
static unsigned __attribute__((stdcall)) set_rect(int *r, int x, int y, int x1,
                                                  int y1) {
  memcpy(r, (int[]){x, y, x1, y1}, 16);
  return 1;
}
static unsigned __attribute__((stdcall)) inflate_rect(int *r, int x, int y) {
  r[0] -= x;
  r[1] -= y;
  r[2] += x;
  r[3] += y;
  return 1;
}
static unsigned __attribute__((stdcall)) gdi_attribute(unsigned dc,
                                                       unsigned n) {
  (void)dc;
  (void)n;
  return 13;
}
static void fill_button(void *c, int *r, unsigned color, unsigned mode) {
  require(c == ctx && (color & 0xffff) == 7, "plain fill context/color");
  memcpy(last_fill, r, 16);
  ++fill_count;
  fill_mode = mode & 0xffff;
}
static void pattern_button(void *c) {
  require(c == ctx, "pattern context");
  ++pattern_count;
}
static unsigned clip_button(void *c, int *r) {
  require(c == ctx, "auto clip context");
  memcpy(last_clip, r, 16);
  ++clip_count;
  return clip_ok;
}
static unsigned paste_button(void *c, unsigned x, unsigned y, unsigned x1,
                             unsigned y1, unsigned flag) {
  (void)x;
  (void)y;
  (void)x1;
  (void)y1;
  (void)flag;
  require(c == ctx, "paste context");
  ++paste_count;
  return 1;
}
static unsigned load_button(void *c, unsigned dc, const char *name, unsigned x,
                            unsigned y, unsigned x1, unsigned y1,
                            unsigned dither, unsigned mode, unsigned stretch,
                            int key, unsigned pal, int *r, unsigned rop,
                            unsigned unknown, unsigned noisy) {
  (void)dc;
  (void)x;
  (void)y;
  (void)x1;
  (void)y1;
  require(c == ctx && !dither && mode == 2 && stretch == 1 &&
              (int16_t)key == -1 && pal == 0x12345678 && rop == 0xcc0020 &&
              !unknown,
          "icon always unkeyed current palette COPY");
  require(bitmap_count < 2 &&
              noisy == (bitmap_count == 0 && strstr(name, ".BMH") ? 0u : 1u),
          "normal/hot icon noise mode");
  strcpy(bitmap_names[bitmap_count], name);
  ++bitmap_count;
  memcpy(r, (int[]){20, 40, 84, 72}, 16);
  return bitmap_fail == 2 || (bitmap_fail == 1 && bitmap_count == 1);
}
static void chisel_button(void *c, int *r, unsigned bright, unsigned dark) {
  (void)c;
  (void)r;
  (void)bright;
  (void)dark;
  ++effect_count[0];
}
static void sunken_button(void *c, int *r, unsigned corner, unsigned bright,
                          unsigned dark) {
  (void)c;
  (void)r;
  (void)corner;
  (void)bright;
  (void)dark;
  ++effect_count[1];
}
static void bevel_button(void *c, int *r, unsigned corner, unsigned bright,
                         unsigned dark, unsigned size) {
  (void)c;
  (void)r;
  (void)corner;
  (void)bright;
  (void)dark;
  require((size & 0xffff) == captured_field[0x2d],
          "bevel uses copied device byte");
  ++effect_count[2];
}
static void recess_button(void *c, int *r, unsigned corner, unsigned bright,
                          unsigned dark, unsigned size) {
  (void)c;
  (void)r;
  (void)corner;
  (void)bright;
  (void)dark;
  require((size & 0xffff) == captured_field[0x2d],
          "recess uses copied device byte");
  ++effect_count[3];
}
static unsigned resolve_button(void *c, char *name, unsigned mode,
                               unsigned flag) {
  (void)name;
  (void)mode;
  (void)flag;
  require(c == ctx, "icon resolver context");
  return 0;
}
static char *identity(char *s) { return s; }
static unsigned metric_height(void *c) {
  require(c == ctx, "metric context");
  return 8;
}
static unsigned metric_width(void *c, const char *s) {
  (void)s;
  require(c == ctx, "width context");
  return 32;
}
static unsigned label_button(void *c, unsigned x, unsigned y, void *f,
                             const char *text) {
  require(c == ctx && f == captured_field, "label context/field");
  if (text) {
    label_x = (int16_t)x;
    label_y = (int16_t)y;
    ++label_count;
  }
  return 0;
}
static void renderer_fixture(unsigned scale, unsigned flags, unsigned flags2,
                             const char *text) {
  creation_prepare(scale);
  memcpy(entries, image + 0x828e8, 0x1c);
  pointer(system_state, entries);
  put16(system_state + 8,
        1); /* Constructor/interaction caller holds the table. */
  memcpy(captured_field, (int[]){20, 40, 180, 160, 20, 40, 180, 160}, 32);
  captured_field[0x21] = 15;
  captured_field[0x22] = 0;
  captured_field[0x23] = 15;
  captured_field[0x24] = 15;
  captured_field[0x25] = 8;
  captured_field[0x26] = 7;
  captured_field[0x27] = 7;
  put16(captured_field + 0x28, flags);
  captured_field[0x2a] = flags2;
  captured_field[0x2d] = 8 / scale;
  put32(captured_field + 0x2e, 0x200);
  strcpy(scratch_text, text);
  pointer(captured_field + 0x32, scratch_text);
  put16(captured_field + 0x3a, 640 * scale);
  put16(captured_field + 0x3c, 480 * scale);
  put32(palettes + 0x82d, 0x12345678);
  fill_count = pattern_count = clip_count = paste_count = bitmap_count =
      label_count = 0;
  clip_ok = 1;
  bitmap_fail = 0;
  memset(effect_count, 0, sizeof effect_count);
}
static unsigned draw_button(unsigned initial) {
  return ((unsigned (*)(void *, void *, unsigned, unsigned, unsigned))(
      image + 0x2a4d3))(ctx, captured_field, initial, 0, 1);
}
static void renderer_checks(void) {
  memcpy(image + 0x2a4d3, original_draw, 5);
  pointer(image + 0x96618, (void *)set_rect);
  pointer(image + 0x96698, (void *)inflate_rect);
  pointer(image + 0x966dc, (void *)gdi_attribute);
  hook(0x13c69, noop);
  pointer(image + 0x96400, (void *)gdi_attribute);
  pointer(image + 0x96408, (void *)gdi_attribute);
  hook(0x65965, (void (*)(void))strstr);
  hook(0x65453, (void (*)(void))strchr);
  hook(0x69e40, (void (*)(void))strchr);
  hook(0x65a60, (void (*)(void))strstr);
  hook(0x68dd0, (void (*)(void))strlen);
  hook(0x68918, (void (*)(void))strcat);
  hook(0x3f71a, (void (*)(void))identity);
  hook(0x3f80e, (void (*)(void))resolve_button);
  hook(0x10440, (void (*)(void))fill_button);
  hook(0x2a1fc, (void (*)(void))pattern_button);
  hook(0x2866, (void (*)(void))clip_button);
  hook(0x2a00, (void (*)(void))paste_button);
  hook(0x4a410, (void (*)(void))load_button);
  hook(0x104e0, (void (*)(void))chisel_button);
  hook(0x10a50, (void (*)(void))sunken_button);
  hook(0x106e0, (void (*)(void))bevel_button);
  hook(0x10c00, (void (*)(void))recess_button);
  hook(0x3fc30, (void (*)(void))metric_height);
  hook(0x3fc40, (void (*)(void))metric_height);
  hook(0x3fcd0, (void (*)(void))metric_width);
  hook(0x2a22b, (void (*)(void))label_button);
  const unsigned hosts[] = {0x4b0e4, 0x383b0, 0x103c0, 0x3fb30};
  for (unsigned i = 0; i < sizeof hosts / sizeof hosts[0]; ++i)
    hook(hosts[i], noop);
  for (unsigned scale = 1; scale <= 4; scale *= 2)
    for (unsigned bits = 0; bits < 256; ++bits) {
      renderer_fixture(scale, 0x100, bits, "<><>");
      require(draw_button(1) == 0 && fill_count == !(bits & 32) &&
                  pattern_count == (!(bits & 32) && !!(bits & 64)) &&
                  (!fill_count || fill_mode == !(bits & 64)),
              "no-fill precedes pattern for every flags2 byte");
    }
  for (unsigned protect = 0; protect < 2; ++protect)
    for (unsigned success = 0; success < 2; ++success) {
      renderer_fixture(1, 0x8348, 0, "<><>");
      put16(system_state + 0xa, 5);
      system_state[0xc + 5] = 1;
      system_state[0x30 + 5] = protect;
      memset(entries + 5 * 0x1c, 0x5a, 0x1c);
      clip_ok = success;
      unsigned char expected[0x1c];
      memcpy(expected, entries + 5 * 0x1c, 0x1c);
      if (success)
        put16(expected + 6, (0x8348 & 0x7c37) | 1);
      require(draw_button(1) == !success && clip_count == 1 && !errors &&
                  !memcmp(expected, entries + 5 * 0x1c, 0x1c) &&
                  system_state[0x30 + 5] == protect,
              "successful auto clip mutates even protected current flags; "
              "failed capture leaves record");
    }
  for (unsigned bits = 0; bits < 2; ++bits)
    for (unsigned hot = 0; hot < 2; ++hot)
      for (unsigned fail = 0; fail < 3; ++fail) {
        renderer_fixture(1, 0x1080, bits * 128, "TEST.BMP<><>");
        if (hot)
          captured_field[0x20] = 8;
        bitmap_fail = fail;
        unsigned result = draw_button(1);
        require(result == (fail == 2 || (!hot && fail == 1)) &&
                    bitmap_count == (hot && fail ? 2u : 1u),
                "hot failure retries normal; normal failure rejects");
        require(strstr(bitmap_names[0], hot ? ".BMH" : ".BMP") != NULL &&
                    (!(hot && fail) || strstr(bitmap_names[1], ".BMP")),
                "normal/hot/fallback file names");
      }
  for (unsigned types = 1; types < 8; ++types) {
    unsigned flags =
        (types & 1 ? 1 : 0) | (types & 2 ? 128 : 0) | (types & 4 ? 256 : 0);
    renderer_fixture(1, flags, 0, "TEST.BMP<><>");
    require(draw_button(1) == 0 && bitmap_count == !!(types & 2) &&
                paste_count == (!(types & 2) && !!(types & 1)) &&
                fill_count == (types == 4),
            "type precedence is icon, clipboard, plain");
  }
  for (unsigned scale = 1; scale <= 4; scale *= 2)
    for (unsigned chisel = 0; chisel < 2; ++chisel)
      for (unsigned flags2 = 8; flags2 <= 24; flags2 += 8) {
        renderer_fixture(scale, 256 | (chisel ? 8 : 0), flags2, "<>LABEL<>");
        require(draw_button(1) == 0 && label_count == 1, "nonempty label draw");
        int x =
            (flags2 & 8)
                ? 20 + (chisel ? 20 : 10) / (int)scale
                : 180 - 32 - ((chisel ? 19 : 9) + (int)scale - 1) / (int)scale;
        require(label_x == x, "left wins simultaneous justification; "
                              "indentation in creation world units");
      }
}

static unsigned rectangle_count, polygon_count, move_count, frame_count;
static int effect_rect[4], first_move[2], frame_rect[4], polygon_bounds[4];
static unsigned __attribute__((stdcall)) gdi_one(unsigned a) {
  (void)a;
  return 1;
}
static unsigned __attribute__((stdcall)) pen(unsigned style, unsigned width,
                                             unsigned color) {
  (void)color;
  require(style == 0 && width == 1, "effect pen is one device pixel");
  return 1;
}
static unsigned color_value(void *c, unsigned n) {
  require(c == ctx, "effect color context");
  return n & 0xffff;
}
static unsigned __attribute__((stdcall))
effect_rectangle(unsigned dc, int x, int y, int x1, int y1) {
  (void)dc;
  memcpy(effect_rect, (int[]){x, y, x1, y1}, 16);
  ++rectangle_count;
  return 1;
}
static unsigned __attribute__((stdcall)) effect_move(unsigned dc, int x, int y,
                                                     void *old) {
  (void)dc;
  (void)old;
  if (!move_count) {
    first_move[0] = x;
    first_move[1] = y;
  }
  ++move_count;
  return 1;
}
static unsigned __attribute__((stdcall)) effect_line(unsigned dc, int x,
                                                     int y) {
  (void)dc;
  (void)x;
  (void)y;
  return 1;
}
static unsigned __attribute__((stdcall)) effect_pixel(unsigned dc, int x, int y,
                                                      unsigned color) {
  (void)dc;
  (void)x;
  (void)y;
  (void)color;
  return 1;
}
static unsigned __attribute__((stdcall))
effect_polygon(unsigned dc, const int *p, unsigned n) {
  (void)dc;
  require(n == 4, "bevel face quadrilateral");
  for (unsigned i = 0; i < n; ++i) {
    if (!polygon_count && !i)
      memcpy(polygon_bounds, (int[]){p[0], p[1], p[0], p[1]}, 16);
    if (p[2 * i] < polygon_bounds[0])
      polygon_bounds[0] = p[2 * i];
    if (p[2 * i + 1] < polygon_bounds[1])
      polygon_bounds[1] = p[2 * i + 1];
    if (p[2 * i] > polygon_bounds[2])
      polygon_bounds[2] = p[2 * i];
    if (p[2 * i + 1] > polygon_bounds[3])
      polygon_bounds[3] = p[2 * i + 1];
  }
  ++polygon_count;
  return 1;
}
static unsigned __attribute__((stdcall)) effect_frame(unsigned dc, const int *r,
                                                      unsigned brush) {
  (void)dc;
  (void)brush;
  memcpy(frame_rect, r, 16);
  ++frame_count;
  return 1;
}
static void effect_checks(void) {
  for (unsigned n = 0; n < 5; ++n)
    memcpy(image + effect_rvas[n], original_effects[n], 5);
  hook(0x3dce0, (void (*)(void))color_value);
  pointer(image + 0x9640c, (void *)gdi_one);
  pointer(image + 0x96418, (void *)gdi_one);
  pointer(image + 0x963d0, (void *)gdi_one);
  pointer(image + 0x963c0, (void *)gdi_attribute);
  pointer(image + 0x963d4, (void *)pen);
  pointer(image + 0x96468, (void *)effect_rectangle);
  pointer(image + 0x963dc, (void *)effect_move);
  pointer(image + 0x963cc, (void *)effect_line);
  pointer(image + 0x963c8, (void *)effect_pixel);
  pointer(image + 0x9641c, (void *)effect_polygon);
  pointer(image + 0x96688, (void *)effect_frame);
  const unsigned cut[] = {11, 24, 39, 74, 149, 199, 249, 299, 32767};
  const unsigned xs[] = {1, 3, 4, 6, 7, 8, 10, 11, 13},
                 ys[] = {1, 3, 4, 6, 5, 6, 7, 8, 9};
  for (unsigned h = 0; h < 32768; ++h) {
    uint16_t x = 0, y = 0;
    unsigned row = 0;
    while (h > cut[row])
      ++row;
    ((void (*)(unsigned, void *, void *))(image + 0x102b0))(h, &x, &y);
    require(x == xs[row] && y == ys[row],
            "complete nonnegative signed-height chisel inset table");
  }
  for (unsigned scale = 1; scale <= 4; scale *= 2)
    for (unsigned h = 11; h <= 301; ++h) {
      renderer_fixture(scale, 256, 0, "<><>");
      int r[] = {20, 40, 180, 40 + (int)h};
      unsigned row = 0;
      while (h > cut[row])
        ++row;
      rectangle_count = move_count = 0;
      ((void (*)(void *, void *, unsigned, unsigned))(image + 0x104e0))(ctx, r,
                                                                        15, 8);
      require(rectangle_count == 1 && effect_rect[0] == 20 + (int)xs[row] &&
                  effect_rect[1] == 40 + (int)ys[row] &&
                  effect_rect[2] == 180 - (int)xs[row] &&
                  effect_rect[3] == 40 + (int)h - (int)ys[row],
              "original chisel uses device height and direct device insets");
    }
  for (unsigned scale = 1; scale <= 4; scale *= 2)
    for (unsigned size = 0; size <= 8; ++size) {
      renderer_fixture(scale, 256, 0, "<><>");
      int r[] = {20, 40, 180, 160};
      polygon_count = move_count = frame_count = 0;
      ((void (*)(void *, void *, unsigned, unsigned, unsigned, unsigned))(
          image + 0x106e0))(ctx, r, 7, 15, 8, size);
      require(polygon_count == 4 &&
                  !memcmp(polygon_bounds,
                          (int[]){20 - (int)size, 40 - (int)size,
                                  180 + (int)size, 160 + (int)size},
                          16),
              "original bevel expands in direct device units");
      ((void (*)(void *, void *, unsigned))(image + 0x103c0))(ctx, r, size);
      require(frame_count == 1 &&
                  !memcmp(frame_rect,
                          (int[]){19 - (int)size, 39 - (int)size,
                                  181 + (int)size, 161 + (int)size},
                          16),
              "black recess frame one device unit outside bevel extent");
      move_count = 0;
      ((void (*)(void *, void *, unsigned, unsigned, unsigned, unsigned))(
          image + 0x10c00))(ctx, r, 7, 15, 8, size);
      require(first_move[0] == 18 - (int)size &&
                  first_move[1] == 38 - (int)size,
              "recess outer pen two device units beyond bevel extent");
      move_count = 0;
      ((void (*)(void *, void *, unsigned, unsigned, unsigned))(
          image + 0x10a50))(ctx, r, 7, 15, 8);
      require(first_move[0] == 21 && first_move[1] == 41,
              "sunken pen inset is one device pixel");
    }
}

int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  for (unsigned n = 0; n < 5; ++n)
    memcpy(original_effects[n], image + effect_rvas[n], 5);
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
  creation_checks();
  renderer_checks();
  effect_checks();
  printf("PASS: %u button-style checks\n", checks);
  return 0;
}
