/* Execute selected original i386 driver routines, with explicitly listed host
 * hooks. Build with a 32-bit C compiler. The wrapper verifies the complete DLL
 * SHA-256. This is an isolated routine probe, not an end-to-end RIPtel
 * renderer.
 */
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

static void require(int ok, const char *label);
static unsigned char context[256], table[64], style[0x61], viewport[128];
static char temporary[8192];
static unsigned char vector_state[0x2300], graphics_state[64], port[128];
struct render_args {
  uint32_t value[26];
};
static struct render_args captured;
static unsigned render_calls;
static void capture_render(struct render_args args) {
  captured = args;
  ++render_calls;
}
static unsigned font_load_calls, font_lookup_missing;
static unsigned loaded_size, loaded_face, render_success;
static unsigned char bitmap_memory[65536];
struct glyph_call {
  uint32_t arg[9];
};
static struct glyph_call glyph_calls[64];
static unsigned glyph_count;
static void glyph_capture(struct glyph_call call) {
  require(glyph_count < 64, "glyph capture bound");
  glyph_calls[glyph_count++] = call;
}
static unsigned allocate_bitmap(void *target, unsigned size) {
  require(size <= sizeof bitmap_memory, "controlled bitmap allocation bound");
  memset(bitmap_memory, 0, sizeof bitmap_memory);
  memcpy(target, &(void *){bitmap_memory}, 4);
  return 1;
}
static unsigned rasterize(void) { return 1; }
static unsigned capture_codes, width_count, raster_count;
static unsigned width_codes[512], raster_codes[512];
static unsigned glyph_width(void *engine, void *font, unsigned code,
                            unsigned face, unsigned flags) {
  (void)engine;
  (void)font;
  (void)face;
  (void)flags;
  if (capture_codes) {
    require(width_count < 512, "bounded captured character metrics");
    width_codes[width_count++] = code & 0xffff;
  }
  return 160;
}
static unsigned raster_code(void *engine, void *font, unsigned code,
                            unsigned face, unsigned flags, void *bits,
                            unsigned stride, unsigned height, int x, int y) {
  (void)engine;
  (void)font;
  (void)face;
  (void)flags;
  (void)bits;
  (void)stride;
  (void)height;
  (void)x;
  (void)y;
  require(raster_count < 512, "bounded captured character rasterization");
  raster_codes[raster_count++] = code & 0xffff;
  return 1;
}

static const char *renderer_message;
static unsigned font_lookup(void *ctx, const char *name) {
  (void)ctx;
  (void)name;
  return font_lookup_missing ? 0xffffffffu : 1;
}
static void renderer_log(const char *message) { renderer_message = message; }
static unsigned font_load(void *ctx, unsigned index, unsigned size,
                          unsigned face) {
  (void)ctx;
  require((index & 0xffff) == 1, "resolved font index reaches loader");
  ++font_load_calls;
  loaded_size = size & 0xffff;
  loaded_face = face & 0xffff;
  if (!render_success)
    return 0;
  /* Synthetic realized font: 160x240 units, at 1440 units/inch. */
  uint16_t value = 160;
  memcpy(vector_state + 0xd4, &value, 2);
  value = 240;
  memcpy(vector_state + 0xd6, &value, 2);
  value = 180;
  memcpy(vector_state + 0xc8, &value, 2);
  value = 240;
  memcpy(vector_state + 0xca, &value, 2);
  return 512;
}
static unsigned __attribute__((stdcall)) gdi_attribute(unsigned dc,
                                                       unsigned value) {
  (void)dc;
  (void)value;
  return 0;
}
static unsigned __attribute__((stdcall)) client_rect(unsigned hwnd,
                                                     int32_t *rect) {
  (void)hwnd;
  rect[0] = rect[1] = 0;
  rect[2] = 640;
  rect[3] = 480;
  return 1;
}

static unsigned protection;
static const char *error_message;
#include "riptel-probe-image.h"
static unsigned protected_style(void *ctx, int slot) {
  (void)ctx;
  (void)slot;
  return protection;
}
static void report_error(void *ctx, unsigned level, const char *name,
                         const char *message) {
  (void)ctx;
  (void)level;
  (void)name;
  error_message = message;
}
static void *scratch(void *ctx) {
  (void)ctx;
  return temporary;
}
static void load(const char *path) {
  load_image(path);
  /* UI/context synchronization, style protection, error sink, scratch
   * allocation, macro expansion and scratch release. The tested numeric handler
   * is untouched.
   */
  hook(0x45038, noop);
  hook(0x3935b, (void (*)(void))protected_style);
  hook(0x38e47, (void (*)(void))report_error);
  hook(0x15396, (void (*)(void))scratch);
  hook(0x153fe, noop);
  hook(0x3988f, noop);
  hook(0x398ee, noop);
  hook(0x4b0e4, noop);
}
static void prepare(void) {
  memset(context, 0, sizeof context);
  memset(table, 0, sizeof table);
  memset(style, 0xa5, sizeof style);
  memset(viewport, 0, sizeof viewport);
  pointer(context + 0xa, table);
  pointer(table, style);
  pointer(context + 0xe, viewport);
  put32(viewport + 0x24, 1280);
  put32(viewport + 0x28, 960);
  put32(viewport + 0x2c, 640);
  put32(viewport + 0x30, 480);
  protection = 0;
  error_message = NULL;
}
static void apply(uint32_t *args, const char *name) {
  typedef void (*handler)(void *, unsigned, uint32_t *, const char *);
  ((handler)(image + 0x1adc0))(context, 11, args, name);
}
static void decode(const char *wire, uint32_t *args) {
  static const unsigned widths[] = {1, 1, 4, 2, 2, 2, 2, 2, 2, 2, 6};
  typedef uint32_t (*decoder)(const char *);
  for (unsigned i = 0, pos = 0; i < 11; ++i) {
    char value[8];
    memcpy(value, wire + pos, widths[i]);
    value[widths[i]] = 0;
    args[i] = ((decoder)(image + 0x211d6))(value);
    pos += widths[i];
  }
}
static unsigned native_copies, copy_mask;
static unsigned char copy_canvas[16], lookup_engine[0x900], lookup_font[0x8d],
    lookup_meta[128];
static unsigned char slot_sentinels[224];
static void *glyph_slots[224];
static unsigned __attribute__((stdcall))
set_bitmap(unsigned bitmap, unsigned bytes, const unsigned char *bits) {
  require(bitmap == 99 && bytes == 1 && bits, "captured owned glyph mask");
  copy_mask = *bits;
  return bytes;
}
static unsigned __attribute__((stdcall)) glyph_rect(int *r, int x, int y,
                                                    int right, int bottom) {
  memcpy(r, (int[]){x, y, right, bottom}, 16);
  return 1;
}
static unsigned __attribute__((stdcall)) glyph_blt(unsigned dc, int x, int y,
                                                   int w, int h,
                                                   unsigned source, int sx,
                                                   int sy, unsigned rop) {
  require(dc == 1 && source == 2 && y == 0 && w == 8 && h == 1 && sx == 0 &&
              sy == 0,
          "original glyph copy retains controlled mask geometry");
  require(rop == 0xb8074a && x >= 0 && x < 2,
          "original glyph BitBlt always uses fixed masked replacement ROP");
  for (unsigned n = 0; n < 8; ++n)
    if (!(copy_mask & (0x80u >> n)))
      copy_canvas[x + n] = 15;
  ++native_copies;
  return 1;
}
static void *lookup_realized(void *engine, void *descriptor) {
  (void)descriptor;
  require(engine == lookup_engine,
          "bounded original glyph-cache lookup context");
  return lookup_font;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: probe DLL\n");
    return 2;
  }
  load(argv[1]);
  unsigned char saved_presenter[5];
  memcpy(saved_presenter, image + 0x40660, 5);
  uint32_t args[11];
  decode("0000BW1Q080000001a1a000000", args);
  const uint32_t expected[] = {0, 0, 736, 90, 8, 0, 0, 0, 100, 100, 0};
  require(!memcmp(args, expected, sizeof args),
          "original decoder: requested example");
  prepare();
  apply(args, "Marin");
  require(!error_message && style[0x24] == 1 &&
              !strcmp((char *)style + 0x27, "Marin"),
          "example installs extended style");
  require(u32(style + 0x53) == 736 && u16(style + 0x4f) == 90 &&
              u16(style + 0x57) == 8 && u16(style + 0x5f) == 100,
          "example stored field values");
  unsigned char baseline[sizeof style];
  memcpy(baseline, style, sizeof style);
  args[9] = 4095;
  args[10] = 0xffffffff;
  prepare();
  apply(args, "Marin");
  require(!error_message && !memcmp(style, baseline, sizeof style),
          "WW and six reserved characters have no handler effect");
  decode("0000BW1Q080000001a1a000000", args);
  for (unsigned field = 5; field <= 6; ++field) {
    for (unsigned value = 0; value < 4096; ++value) {
      args[field] = value;
      prepare();
      apply(args, "Marin");
      int valid = value == 0 || value == 900 || value == 1800 || value == 2700;
      require((error_message == NULL) == valid, "exhaustive rotation domain");
      if (!valid)
        require(style[0x24] == 0xa5, "rejected rotation leaves style intact");
    }
    args[field] = 0;
  }
  args[8] = 0;
  prepare();
  apply(args, "Marin");
  require(error_message &&
              !strcmp(error_message,
                      "Character spacing percentage cannot be zero!"),
          "zero spacing diagnostic");
  args[8] = 100;
  prepare();
  apply(args, "");
  require(error_message &&
              !strcmp(error_message, "No typeface name specified!"),
          "empty typeface diagnostic");
  prepare();
  protection = 1;
  apply(args, "Marin");
  require(error_message && style[0x24] == 0xa5,
          "protected style rejected before mutation");
  for (unsigned value = 0; value < 64; ++value) {
    args[0] = value;
    args[1] = 63 - value;
    prepare();
    apply(args, "Marin");
    require(!error_message && style[0x51] == value && style[0x52] == 63 - value,
            "all single-digit values stored without rejection");
  }
  args[0] = args[1] = args[2] = 0;
  for (unsigned value = 0; value < 4096; ++value) {
    args[3] = value;
    args[4] = value;
    args[7] = value;
    args[8] = value ? value : 1;
    prepare();
    apply(args, "Marin");
    require(!error_message && u16(style + 0x4f) == value &&
                u16(style + 0x57) == value && u16(style + 0x5d) == value &&
                u16(style + 0x5f) == args[8],
            "size shadow baseline spacing full wire domains");
  }
  args[3] = 25;
  args[4] = 1;
  args[7] = 0;
  args[8] = 100;
  for (unsigned value = 0; value < 2048; ++value) {
    args[2] = value;
    prepare();
    apply(args, "Marin");
    require(!error_message && u32(style + 0x53) == value,
            "low eleven flag bits accepted");
  }
  args[2] = 0xffffff;
  prepare();
  apply(args, "Marin");
  require(!error_message && u32(style + 0x53) == 0xffffff,
          "maximum flag value accepted");
  /* The radix predicate reads only this synthetic parser state and the original
   * dispatch record. No parser or decoder code is hooked.
   */
  typedef unsigned (*predicate)(void *);
  unsigned char parser[256] = {0};
  pointer(parser + 0x30, image + 0x80820 + 75 * 40);
  parser[0x38] = 36;
  require(((predicate)(image + 0x39d70))(parser) == 0,
          "y selects base64 with global base36");
  parser[0x38] = 64;
  require(((predicate)(image + 0x39d70))(parser) == 0,
          "y selects base64 with global base64");
  /* Exercise the original ordinary graphics-text consumer. Intercept only its
   * final 26-argument renderer call, after its original state interpretation.
   */
  unsigned char saved_renderer[5];
  memcpy(saved_renderer, image + 0x40ed0, 5);
  hook(0x40ed0, (void (*)(void))capture_render);
  typedef void (*placer)(void *, unsigned, unsigned, const char *);
  for (unsigned sr = 0; sr < 4; ++sr)
    for (unsigned cr = 0; cr < 4; ++cr) {
      args[2] = 0;
      args[3] = 25;
      args[4] = 0;
      args[5] = sr * 900;
      args[6] = cr * 900;
      args[7] = 0;
      args[8] = 100;
      prepare();
      apply(args, "Marin");
      render_calls = 0;
      pointer(viewport, viewport + 64);
      put16(viewport + 65, 1280);
      put16(viewport + 67, 960);
      pointer(context + 0x4e, vector_state);
      ((placer)(image + 0x42870))(context, 200, 100, "AB");
      const unsigned sdir[] = {0, 3, 2, 1};
      require(render_calls == 1 && (captured.value[16] & 0xffff) == sdir[sr] &&
                  (captured.value[17] & 0xffff) == cr,
              "all 16 rotation pairs reach renderer");
    }
  for (unsigned flags = 0; flags < 2048; ++flags) {
    args[2] = flags;
    args[4] = 8;
    args[5] = args[6] = 0;
    args[7] = 20;
    args[8] = 125;
    prepare();
    apply(args, "Marin");
    render_calls = 0;
    pointer(viewport, viewport + 64);
    put16(viewport + 65, 1280);
    put16(viewport + 67, 960);
    pointer(context + 0x4e, vector_state);
    ((placer)(image + 0x42870))(context, 200, 100, "AB");
    const uint32_t *a = captured.value;
    require(render_calls == 1 && (a[11] & 0xffff) == (flags & 3) &&
                (a[12] & 0xffff) == (flags & 12),
            "face and decoration flags reach renderer");
    require((a[14] & 0xffff) == ((flags >> 6) & 3) + 1 &&
                (a[15] & 0xffff) == 8 && (a[21] & 0xffff) == ((flags >> 4) & 3),
            "shadow mode distance and horizontal justification reach renderer");
    int baseline = (flags & 512) ? 10 : 20;
    if (flags & 256)
      baseline = -baseline;
    require((int16_t)a[19] == baseline &&
                (int16_t)a[20] == ((flags & 1024) ? -125 : 125),
            "baseline conversion sign and spacing sign reach renderer");
    require((a[18] & 0xffff) == 0,
            "y has no vertical-alignment input to renderer");
  }
  memcpy(image + 0x40ed0, saved_renderer, 5);
  hook(0x3fb50, (void (*)(void))font_lookup);
  hook(0x3fdf0, (void (*)(void))renderer_log);
  hook(0x4f230, noop);
  hook(0x40170, (void (*)(void))font_load);
  pointer(image + 0x96400, (void *)gdi_attribute);
  pointer(image + 0x963f0, (void *)gdi_attribute);
  pointer(image + 0x963f8, (void *)gdi_attribute);
  pointer(image + 0x96644, (void *)client_rect);
  prepare();
  memset(vector_state, 0, sizeof vector_state);
  memset(port, 0, sizeof port);
  pointer(context + 0x4e, vector_state);
  pointer(context + 0x3a, graphics_state);
  pointer(context + 0x22, port);
  put16(vector_state + 6, 1280);
  put16(vector_state + 8, 960);
  put32(port + 0x3a, 640);
  typedef void (*renderer)(struct render_args);
  captured.value[0] = (uint32_t)(uintptr_t)context;
  captured.value[10] = (uint32_t)(uintptr_t)"Marin";
  captured.value[13] = 90;
  captured.value[14] = 4;
  captured.value[15] = 8;
  captured.value[17] = 0;
  captured.value[11] = 3;
  captured.value[20] = 100;
  captured.value[25] = 0;
  font_lookup_missing = 1;
  font_load_calls = 0;
  ((renderer)(image + 0x40ed0))(captured);
  require(font_load_calls == 0,
          "unknown typeface causes no fallback font load");
  font_lookup_missing = 0;
  captured.value[13] = 0;
  error_message = NULL;
  ((renderer)(image + 0x40ed0))(captured);
  require(error_message && !strcmp(error_message, "Font size cannot be 0"),
          "zero font size is deferred renderer error");
  captured.value[13] = 90;
  renderer_message = NULL;
  ((renderer)(image + 0x40ed0))(captured);
  require(font_load_calls == 1 && loaded_size == 900 && loaded_face == 3,
          "font size multiplied by ten and bold italic passed to loader");
  require(renderer_message &&
              !strcmp(renderer_message, "Failed to load vector font"),
          "font load failure diagnostic");
  require(u16(vector_state + 0x6e) == 36 && u16(vector_state + 0x70) == 36,
          "font DPI derived from client dimensions and world frame");
  require(u16(vector_state + 0x1d24) == 4,
          "shadow distance scales by canvas width over world width");
  require(u16(vector_state + 0x1d22) == 4, "shadow direction retained");
  typedef int16_t (*spacing_delta)(void *, int);
  put16(vector_state + 0xd0, 0);
  put16(vector_state + 0xd4, 12);
  put16(vector_state + 4, 125);
  require(((spacing_delta)(image + 0x3fd90))(context, 8) == 2,
          "proportional 125 percent spacing adds two to width eight");
  put16(vector_state + 4, (uint16_t)-125);
  require(((spacing_delta)(image + 0x3fd90))(context, 8) == 3,
          "negative 125 percent uses font bounding-box width twelve for extra "
          "spacing");
  put16(vector_state + 4, 50);
  require(((spacing_delta)(image + 0x3fd90))(context, 7) == -3,
          "spacing reduction truncates toward zero");
  /* Continue through the ORIGINAL renderer's geometry with synthetic font
   * metrics and intercepted bitmap output. This checks placement, not fonts.
   */
  hook(0x65ca3, (void (*)(void))allocate_bitmap);
  hook(0x65cbf, noop);
  hook(0x40380, (void (*)(void))rasterize);
  hook(0x405b0, noop);
  hook(0x523f0, (void (*)(void))rasterize);
  hook(0x4efd0, (void (*)(void))glyph_width);
  hook(0x40660, (void (*)(void))glyph_capture);
  hook(0x40a90, noop);
  hook(0x40d70, noop);
  render_success = 1;
  memset(vector_state, 0, sizeof vector_state);
  put16(vector_state + 6, 640);
  put16(vector_state + 8, 480);
  put32(port + 0x3a, 640);
  for (unsigned i = 0; i < 26; ++i)
    captured.value[i] = 0;
  captured.value[0] = (uint32_t)(uintptr_t)context;
  captured.value[1] = 200;
  captured.value[2] = 100;
  captured.value[3] = 15;
  captured.value[4] = 2;
  captured.value[10] = (uint32_t)(uintptr_t)"Marin";
  captured.value[13] = 25;
  captured.value[20] = 100;
  captured.value[25] = (uint32_t)(uintptr_t)"AB";
  glyph_count = 0;
  ((renderer)(image + 0x40ed0))(captured);
  require(glyph_count == 2, "two controlled glyphs rendered");
  require((int16_t)glyph_calls[1].arg[1] - (int16_t)glyph_calls[0].arg[1] == 8,
          "controlled glyph advances eight pixels");
  struct glyph_call upright[2];
  memcpy(upright, glyph_calls, sizeof upright);
  for (unsigned mode = 1; mode <= 4; ++mode) {
    captured.value[14] = mode;
    captured.value[15] = 8;
    glyph_count = 0;
    ((renderer)(image + 0x40ed0))(captured);
    require(glyph_count == 4, "each glyph has one shadow copy");
    int dx = (int16_t)glyph_calls[0].arg[1] - (int16_t)glyph_calls[1].arg[1];
    int dy = (int16_t)glyph_calls[0].arg[2] - (int16_t)glyph_calls[1].arg[2];
    require(dx == ((mode == 1 || mode == 3) ? -8 : 8) &&
                dy == ((mode == 1 || mode == 2) ? -8 : 8),
            "four device-axis shadow directions");
    require((glyph_calls[0].arg[8] & 0xffff) == 2 &&
                (glyph_calls[1].arg[8] & 0xffff) == 15,
            "background shadow then foreground glyph");
  }
  captured.value[14] = captured.value[15] = 0;
  for (unsigned direction = 0; direction < 4; ++direction)
    for (unsigned orientation = 0; orientation < 4; ++orientation)
      for (unsigned justification = 0; justification < 3; ++justification) {
        captured.value[21] = justification;
        captured.value[16] = direction;
        captured.value[17] = orientation;
        captured.value[19] = 0;
        glyph_count = 0;
        ((renderer)(image + 0x40ed0))(captured);
        require(glyph_count == 2, "direction test renders two glyphs");
        int x = (int16_t)glyph_calls[0].arg[1],
            y = (int16_t)glyph_calls[0].arg[2];
        int ax = (int16_t)glyph_calls[1].arg[1] - x,
            ay = (int16_t)glyph_calls[1].arg[2] - y;
        captured.value[19] = 10;
        glyph_count = 0;
        ((renderer)(image + 0x40ed0))(captured);
        int dx = (int16_t)glyph_calls[0].arg[1] - x,
            dy = (int16_t)glyph_calls[0].arg[2] - y;
        const int dx_table[4][4] = {
            {0, 10, 0, 0}, {0, 0, 0, 0}, {0, 10, 0, 0}, {10, 10, 0, 0}};
        const int dy_table[4][4] = {
            {10, 0, 0, 0}, {10, 0, 0, 0}, {0, 0, -10, 0}, {0, 0, 0, 0}};
        require(dx == dx_table[direction][orientation] &&
                    dy == dy_table[direction][orientation],
                "all sixteen shipped baseline displacement branches");
        require((direction == 0   ? ax > 0
                 : direction == 1 ? ay > 0
                 : direction == 2 ? ax < 0
                                  : ay < 0),
                "physical advance direction");
      }
  captured.value[16] = captured.value[17] = captured.value[19] =
      captured.value[21] = 0;
  for (unsigned value = 0; value < 64; ++value) {
    captured.value[5] = value;
    captured.value[9] = 63 - value;
    glyph_count = 0;
    ((renderer)(image + 0x40ed0))(captured);
    require(glyph_count == 2 && !memcmp(glyph_calls, upright, sizeof upright),
            "s and f have no effect on controlled rendering");
  }
  /* Complete original byte-to-engine interface, including high bytes. */
  capture_codes = 1;
  hook(0x523f0, (void (*)(void))raster_code);
  const char *test_faces[] = {"Marin", "SYMBOL"};
  for (unsigned face = 0; face < 2; ++face)
    for (unsigned code = 1; code < 256; ++code) {
      char input[] = {(char)code, (char)code, 0};
      captured.value[10] = (uint32_t)(uintptr_t)test_faces[face];
      captured.value[25] = (uint32_t)(uintptr_t)input;
      glyph_count = raster_count = width_count = 0;
      ((renderer)(image + 0x40ed0))(captured);
      require(glyph_count == 2 && raster_count == 2 && width_count > 0,
              "each byte produces separate glyph and metric calls");
      for (unsigned n = 0; n < raster_count; ++n)
        require(raster_codes[n] == code,
                "original renderer passes zero-extended byte to glyph engine");
      require(width_count == 6 && width_codes[0] == 'W',
              "renderer first requests reference W metric");
      for (unsigned n = 1; n < width_count; ++n)
        require(width_codes[n] == code,
                "original renderer passes same raw byte to text metrics");
    }
  /* Original glyph presentation after successful rasterization; APIs capture
   * masks and interpret only the fixed ROP boundary, not Windows rasterization.
   */
  memcpy(image + 0x40660, saved_presenter, 5);
  hook(0x13c69, noop);
  hook(0xe6e9, noop);
  pointer(image + 0x963bc, (void *)set_bitmap);
  pointer(image + 0x96618, (void *)glyph_rect);
  pointer(image + 0x963e4, (void *)glyph_blt);
  put32(context + 0x62, 1);
  put16(vector_state + 0x1d0a, 15);
  put16(vector_state + 0x1d0c, 2);
  put32(vector_state + 0x1d0e, 1);
  put32(vector_state + 0x1cf2, 99);
  put32(vector_state + 0x1ce8, 1);
  put16(vector_state + 0x1cee, 8);
  put16(vector_state + 0x1cf0, 1);
  put32(vector_state + 0x1d12, 2);
  for (unsigned mode = 0; mode < 5; ++mode) {
    unsigned command[] = {mode};
    ((void (*)(void *, unsigned, const void *, const char *))(image + 0x2102c))(
        context, 1, command, NULL);
    require(style[4] == mode,
            "original write-mode command stores requested mode");
    for (unsigned mask = 0; mask < 256; ++mask) {
      unsigned char bits = mask;
      memset(copy_canvas, 3, sizeof copy_canvas);
      native_copies = 0;
      struct glyph_call call = {{(uint32_t)(uintptr_t)context, 0, 0,
                                 (uint32_t)(uintptr_t)&bits, 8, 1, 0, 0, 15}};
      ((void (*)(struct glyph_call))(image + 0x40660))(call);
      call.arg[1] = 1;
      ((void (*)(struct glyph_call))(image + 0x40660))(call);
      require(native_copies == 2,
              "overlapping glyphs each cause a bitmap copy");
      for (unsigned x = 0; x < 9; ++x) {
        unsigned ink = (x < 8 && !(mask & (0x80u >> x))) ||
                       (x > 0 && !(mask & (0x80u >> (x - 1))));
        require(
            copy_canvas[x] == (ink ? 15 : 3),
            "overlapping glyph COPY coverage does not cancel under XOR mode");
      }
    }
  }
  /* Original cached and uncached slot arithmetic. Both are font-neutral. */
  hook(0x52340, (void (*)(void))lookup_realized);
  hook(0x523c0, noop);
  pointer(lookup_font + 0x39, lookup_meta);
  put16(lookup_meta + 0x3c, 224);
  put16(lookup_meta + 0x3e, 32);
  pointer(lookup_font + 0x4d, glyph_slots);
  for (unsigned n = 0; n < 224; ++n)
    glyph_slots[n] = slot_sentinels + n;
  for (unsigned code = 0; code < 65536; ++code) {
    void *p = ((void *(*)(void *, void *, unsigned, unsigned, unsigned))(
        image + 0x55a30))(lookup_engine, lookup_font, code, 0, 0);
    require(p == (code >= 32 && code < 256 ? slot_sentinels + code - 32 : NULL),
            "original glyph cache maps raw code minus first character, no "
            "codepage translation");
  }
  unsigned char offset_font[32] = {0}, offset_header[128] = {0};
  unsigned offsets[224];
  pointer(offset_font + 0x10, offset_header);
  pointer(offset_header, offsets);
  put32(offset_font + 2, 0x212);
  for (unsigned n = 0; n < 224; ++n)
    offsets[n] = 0x1000 + n * 37;
  for (unsigned n = 0; n < 224; ++n) {
    unsigned offset = ((unsigned (*)(void *, void *, void *, unsigned))(
        image + 0x505e0))(lookup_engine, offset_font, NULL, n);
    require(
        offset == 0x212 + offsets[n],
        "original outline file lookup uses direct relative glyph offset slot");
  }
  printf("PASS: %u checks against original decoder, radix selector, handler "
         "and text consumer\n",
         checks);
  return 0;
}
