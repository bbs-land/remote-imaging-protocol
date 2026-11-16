/* Original fill-style/brush construction and direct drawing arguments.
 * GDI services are controlled: bitmap bits and color/DC/rectangle arguments
 * are captured; Windows rasterization and its brush-origin implementation
 * are not executed. */
#include "riptel-probe-image.h"
static unsigned char ctx[512], parser[256], graphics[64], style[0x61],
    objects[64], palette[0x900], world[128], ports[128], entries[36 * 0x78];
static unsigned errors, brush_calls, solids, rectangles, drawing_dc, active_dc;
static unsigned compatible_calls;
static unsigned selected[4][3], text_color[4], back_color[4];
static int clip[4][4], region[4], drawn[8][4];
static unsigned char bitmap_bits[8], brush_bits[8];
static const unsigned char masks[12][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {255, 255, 255, 255, 255, 255, 255, 255},
    {255, 255, 0, 0, 255, 255, 0, 0},
    {1, 2, 4, 8, 16, 32, 64, 128},
    {0xe0, 0xc1, 0x83, 7, 14, 0x1c, 0x38, 0x70},
    {0xf0, 0x78, 0x3c, 0x1e, 15, 0x87, 0xc3, 0xe1},
    {0xa5, 0xd2, 0x69, 0xb4, 0x5a, 0x2d, 0x96, 0x4b},
    {255, 0x88, 0x88, 0x88, 255, 0x88, 0x88, 0x88},
    {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81},
    {0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33},
    {0x80, 0, 8, 0, 0x80, 0, 8, 0},
    {0x88, 0, 0x22, 0, 0x88, 0, 0x22, 0}};
static unsigned zero(void) { return 0; }
static unsigned color(void *c, unsigned index) {
  (void)c;
  return index;
}
static void report(void *c, unsigned level, const char *where,
                   const char *why) {
  (void)c;
  (void)level;
  fprintf(stderr, "captured error: %s: %s\n", where, why);
  ++errors;
}
static unsigned __attribute__((stdcall)) discard(unsigned h) {
  (void)h;
  return 1;
}
static unsigned __attribute__((stdcall)) stock(unsigned n) { return 10 + n; }
static unsigned __attribute__((stdcall)) compatible_dc(unsigned h) {
  (void)h;
  ++compatible_calls;
  return 3;
}
static unsigned __attribute__((stdcall))
select_palette(unsigned dc, unsigned pal, unsigned force) {
  (void)dc;
  (void)pal;
  (void)force;
  return 1;
}
static unsigned __attribute__((stdcall)) select_object(unsigned dc,
                                                       unsigned obj) {
  require(dc >= 1 && dc <= 3, "controlled DC range");
  /* GDI keeps independent selections for brushes, pens and bitmaps.
   * Distinct original objects stand in for the otherwise opaque defaults. */
  unsigned kind = obj == 5 || obj == 22 ? 2 : obj == 18 || obj == 21 ? 1 : 0;
  unsigned old = selected[dc][kind];
  selected[dc][kind] = obj;
  return old ? old : 20 + kind;
}
static unsigned __attribute__((stdcall)) create_bitmap(unsigned w, unsigned h,
                                                       unsigned planes,
                                                       unsigned bits,
                                                       const void *initial) {
  require(w == 8 && h == 8 && planes == 1 && bits == 1 && !initial,
          "original creates unscaled 8x8 monochrome bitmap");
  memset(bitmap_bits, 0, sizeof bitmap_bits);
  return 5;
}
static unsigned __attribute__((stdcall)) set_pixel(unsigned dc, int x, int y,
                                                   unsigned rgb) {
  require(dc == 3 && x >= 0 && x < 8 && y >= 0 && y < 8 && rgb == 0,
          "original brush paints only black mask pixels");
  bitmap_bits[y] &= ~(0x80u >> x);
  return 0;
}
static unsigned __attribute__((stdcall)) fill_rect(unsigned dc, const int *r,
                                                   unsigned brush) {
  require(dc == 3 && brush == 10 &&
              !memcmp(r, (int[]){0, 0, 8, 8}, sizeof region),
          "original initializes monochrome brush white");
  memset(bitmap_bits, 255, sizeof bitmap_bits);
  return 1;
}
static unsigned __attribute__((stdcall)) pattern_brush(unsigned bitmap) {
  require(bitmap == 5, "original pattern brush receives constructed bitmap");
  ++brush_calls;
  memcpy(brush_bits, bitmap_bits, 8);
  return 6;
}
static unsigned __attribute__((stdcall)) solid_brush(unsigned rgb) {
  require(rgb == 1, "solid fill uses fill color");
  ++solids;
  return 7;
}
static unsigned __attribute__((stdcall)) set_text(unsigned dc, unsigned rgb) {
  unsigned old = text_color[dc];
  text_color[dc] = rgb;
  return old;
}
static unsigned __attribute__((stdcall)) set_back(unsigned dc, unsigned rgb) {
  unsigned old = back_color[dc];
  back_color[dc] = rgb;
  return old;
}
static unsigned __attribute__((stdcall)) set_background_mode(unsigned dc,
                                                             unsigned mode) {
  require(dc == active_dc && mode == 2,
          "filled rectangle requests opaque background");
  return 2;
}
static unsigned __attribute__((stdcall)) set_rop(unsigned dc, unsigned mode) {
  (void)dc;
  (void)mode;
  return 13;
}
static unsigned __attribute__((stdcall)) set_rect(int *r, int x, int y,
                                                  int right, int bottom) {
  memcpy(r, (int[]){x, y, right, bottom}, sizeof region);
  return 1;
}
static unsigned __attribute__((stdcall)) offset_rect(int *r, int x, int y) {
  r[0] += x;
  r[2] += x;
  r[1] += y;
  r[3] += y;
  return 1;
}
static unsigned __attribute__((stdcall)) create_region(int x, int y, int right,
                                                       int bottom) {
  memcpy(region, (int[]){x, y, right, bottom}, sizeof region);
  return 8;
}
static unsigned __attribute__((stdcall)) select_clip(unsigned dc,
                                                     unsigned handle) {
  require(dc == 1 || dc == 2, "original clips screen or bitmap DC");
  require(handle == 8, "original clip region handle");
  memcpy(clip[dc], region, sizeof region);
  return 1;
}
static unsigned __attribute__((stdcall)) rectangle(unsigned dc, int x, int y,
                                                   int right, int bottom) {
  require(rectangles < 8 && dc == active_dc,
          "original rectangle uses active port DC");
  require(selected[dc][0] == 6 && text_color[dc] == 1 && back_color[dc] == 2,
          "original rectangle uses mask brush and fill/background colors");
  drawing_dc = dc;
  memcpy(drawn[rectangles++], (int[]){x, y, right, bottom}, sizeof region);
  return 1;
}
static void prepare(unsigned scale, unsigned type) {
  memset(ctx, 0, sizeof ctx);
  memset(parser, 0, sizeof parser);
  memset(graphics, 0, sizeof graphics);
  memset(style, 0, sizeof style);
  memset(objects, 0, sizeof objects);
  memset(palette, 0, sizeof palette);
  memset(world, 0, sizeof world);
  memset(ports, 0, sizeof ports);
  memset(entries, 0, sizeof entries);
  memset(selected, 0, sizeof selected);
  memset(text_color, 0, sizeof text_color);
  memset(back_color, 0, sizeof back_color);
  pointer(ctx + 2, parser);
  pointer(ctx + 0xa, graphics);
  pointer(graphics, style);
  pointer(ctx + 0x3a, objects);
  pointer(ctx + 0x1a, palette);
  put16(palette + 0x82b, 236);
  put32(palette + 0x82d, 1);
  pointer(ctx + 0xe, world);
  put32(world + 0x24, 640 * scale);
  put32(world + 0x28, 480 * scale);
  put32(world + 0x2c, 640);
  put32(world + 0x30, 480);
  pointer(ctx + 0x22, ports);
  pointer(ports, entries);
  put16(ports + 8, 1);
  entries[0] = 1;
  put32(entries + 0x6c, 1);
  unsigned char *p = entries + 0x78;
  p[0] = type;
  put32(p + 0x6c, 2);
  put32(p + 3, type == 1 ? 2 : 0);
  put32(p + 7, type == 1 ? 1 : 0);
  put32(p + 0xb, 640);
  put32(p + 0xf, 480);
  memcpy(p + 0x3a, (int[]){5, 3, 29, 19}, sizeof region);
  active_dc = type == 1 ? 1 : 2;
  put32(ctx + 0x62, active_dc);
  put32(ctx + 0x6a, 1);
  put16(style + 2, 2);
  errors = brush_calls = solids = rectangles = drawing_dc = compatible_calls =
      0;
  memset(bitmap_bits, 0, sizeof bitmap_bits);
  memset(brush_bits, 0, sizeof brush_bits);
}
static void invoke(unsigned rva, unsigned *args) {
  ((void (*)(void *, unsigned, void *, void *))(image + rva))(ctx, 0, args,
                                                              NULL);
}
static void verify_mask(const unsigned char *expected) {
  require(!errors && brush_calls == 1 && !solids,
          "original constructs one patterned brush");
  for (unsigned y = 0; y < 8; ++y) {
    require(u16(style + 0xa + 2 * y) == expected[y],
            "original stores canonical row word");
    require(brush_bits[y] == (unsigned char)~expected[y],
            "original inversion preserves foreground-one mask semantics");
  }
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  unsigned hooks[] = {0x45038, 0x18cf6, 0x18bf7, 0x3988f,
                      0x398ee, 0x447c0, 0x44802, 0x13c69};
  for (unsigned n = 0; n < sizeof hooks / sizeof hooks[0]; ++n)
    hook(hooks[n], noop);
  hook(0x3935b, (void (*)(void))zero);
  hook(0x3dce0, (void (*)(void))color);
  hook(0x38e47, (void (*)(void))report);
  pointer(image + 0x9640c, (void *)stock);
  pointer(image + 0x963c0, (void *)select_object);
  pointer(image + 0x963d0, (void *)discard);
  pointer(image + 0x964a0, (void *)compatible_dc);
  pointer(image + 0x963f4, (void *)select_palette);
  pointer(image + 0x964a4, (void *)create_bitmap);
  pointer(image + 0x963c8, (void *)set_pixel);
  pointer(image + 0x9667c, (void *)fill_rect);
  pointer(image + 0x964a8, (void *)pattern_brush);
  pointer(image + 0x96418, (void *)solid_brush);
  pointer(image + 0x964ac, (void *)discard);
  pointer(image + 0x963f8, (void *)set_text);
  pointer(image + 0x96400, (void *)set_back);
  pointer(image + 0x96408, (void *)set_rop);
  pointer(image + 0x96618, (void *)set_rect);
  pointer(image + 0x96610, (void *)offset_rect);
  pointer(image + 0x963e8, (void *)create_region);
  pointer(image + 0x963d8, (void *)select_clip);
  pointer(image + 0x96468, (void *)rectangle);
  pointer(image + 0x963f0, (void *)set_background_mode);
  const char *alphabet =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz#&";
  for (unsigned n = 0; n < 256; ++n) {
    char mega[] = {alphabet[n / 36], alphabet[n % 36], 0};
    char ultra[] = {alphabet[n / 64], alphabet[n % 64], 0};
    require(((unsigned (*)(const char *))(image + 0x211a9))(mega) == n,
            "original MegaNum decoder agrees with canonical byte encoding");
    require(((unsigned (*)(const char *))(image + 0x211d6))(ultra) == n,
            "original UltraNum decoder agrees with canonical byte encoding");
  }
  for (unsigned n = 0; n < 12; ++n) {
    prepare(1, 1);
    unsigned args[] = {n, 1};
    invoke(0x1c679, args);
    if (n == 1)
      require(!errors && solids == 1 && !brush_calls,
              "preset one uses solid fill brush");
    else
      verify_mask(masks[n]);
  }
  for (unsigned row = 0; row < 8; ++row)
    for (unsigned bit = 0; bit < 8; ++bit) {
      prepare(1, 1);
      unsigned args[9] = {0};
      unsigned char expected[8] = {0};
      args[row] = expected[row] = 0x80u >> bit;
      args[8] = 1;
      invoke(0x1c55f, args);
      verify_mask(expected);
    }
  for (unsigned scale = 1; scale <= 2; ++scale)
    for (unsigned type = 1; type <= 4; type += 3) {
      prepare(scale, type);
      unsigned args[9] = {0x80, 0, 0, 0, 0, 0, 0, 0, 1};
      invoke(0x1c55f, args);
      unsigned a[] = {0, 0, 8 * scale, 8 * scale},
               b[] = {8 * scale, 0, 16 * scale, 8 * scale};
      invoke(0x19d9e, a);
      invoke(0x19d9e, b);
      require(!errors && rectangles == 2 && drawing_dc == active_dc,
              "adjacent bars retain direct screen/offscreen drawing context");
      require(!memcmp(drawn[0], (int[]){5, 3, 15, 13}, sizeof region) &&
                  !memcmp(drawn[1], (int[]){13, 3, 23, 13}, sizeof region),
              "original world scaling changes object geometry, not 8x8 brush "
              "construction");
      require(brush_calls == 1 && compatible_calls == 1 &&
                  brush_bits[0] == 0x7f,
              "adjacent objects reuse one unchanged pattern brush without an "
              "intermediate DC");
      /* Color setters are outside this second consumer's phase question.
       * Supply the same drawing colors explicitly as controlled DC state. */
      text_color[active_dc] = 1;
      back_color[active_dc] = 2;
      invoke(0x1bee5, a);
      invoke(0x1bee5, b);
      require(
          !errors && rectangles == 4 && drawing_dc == active_dc &&
              brush_calls == 1 && compatible_calls == 1,
          "filled rectangles reuse the same direct context and pattern brush");
      require(!memcmp(drawn[2], (int[]){5, 3, 14, 12}, sizeof region) &&
                  !memcmp(drawn[3], (int[]){13, 3, 22, 12}, sizeof region),
              "filled rectangle world conversion and stored viewport offset");
    }
  printf("PASS: %u original fill-mask/brush/consumer checks\n", checks);
  return 0;
}
