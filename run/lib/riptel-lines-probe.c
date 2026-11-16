/* Original pixel/point placement, clipping setup and pen-width decisions.
 * Controlled GDI services capture arguments; no GUI or Windows rasterizer. */
#include "riptel-probe-image.h"
static unsigned char ctx[512], world[128], ports[128], entries[36 * 0x78],
    graphics[64], style[0x61], objects[64], options[0x400];
static unsigned errors, pixels, fills, pen_calls, pen_width, drawn_dc;
static int pixel_x, pixel_y, filled[4], draw_clip[4], region[4], clips[3][4];
static unsigned visible;
static unsigned zero(void) { return 0; }
static unsigned color(void *c, unsigned index) {
  (void)c;
  (void)index;
  return 0x563412;
}
static void report(void *c, unsigned level, const char *where,
                   const char *why) {
  (void)c;
  (void)level;
  fprintf(stderr, "captured error: %s: %s\n", where, why);
  ++errors;
}
static unsigned __attribute__((stdcall)) discard(unsigned obj) {
  (void)obj;
  return 1;
}
static unsigned __attribute__((stdcall)) select_object(unsigned dc,
                                                       unsigned object) {
  (void)dc;
  (void)object;
  return 1;
}
static unsigned __attribute__((stdcall)) set_rop(unsigned dc, unsigned mode) {
  (void)dc;
  (void)mode;
  return 13;
}
static unsigned __attribute__((stdcall))
create_pen(unsigned kind, unsigned width, unsigned rgb) {
  (void)kind;
  (void)rgb;
  ++pen_calls;
  pen_width = width;
  return 2;
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
static unsigned __attribute__((stdcall)) inflate_rect(int *r, int x, int y) {
  r[0] -= x;
  r[2] += x;
  r[1] -= y;
  r[3] += y;
  return 1;
}
static unsigned __attribute__((stdcall)) create_region(int x, int y, int right,
                                                       int bottom) {
  memcpy(region, (int[]){x, y, right, bottom}, sizeof region);
  return 7;
}
static unsigned __attribute__((stdcall)) select_clip(unsigned dc,
                                                     unsigned handle) {
  require(dc == 1 || dc == 2, "original selected screen/offscreen DC");
  require(handle == 7, "original installs rectangle region");
  memcpy(clips[dc], region, sizeof region);
  return 1;
}
static unsigned __attribute__((stdcall)) set_pixel(unsigned dc, int x, int y,
                                                   unsigned rgb) {
  (void)rgb;
  ++pixels;
  drawn_dc = dc;
  pixel_x = x;
  pixel_y = y;
  memcpy(draw_clip, clips[dc], sizeof draw_clip);
  visible = x >= draw_clip[0] && x < draw_clip[2] && y >= draw_clip[1] &&
            y < draw_clip[3];
  return visible ? 0x563412 : ~0u;
}
static unsigned __attribute__((stdcall)) fill_rect(unsigned dc, const int *r,
                                                   unsigned brush) {
  (void)brush;
  ++fills;
  drawn_dc = dc;
  memcpy(filled, r, sizeof filled);
  memcpy(draw_clip, clips[dc], sizeof draw_clip);
  visible = r[0] < draw_clip[2] && r[2] > draw_clip[0] && r[1] < draw_clip[3] &&
            r[3] > draw_clip[1];
  return 1;
}
static void prepare(unsigned ww, unsigned wh, unsigned dw, unsigned dh,
                    unsigned type, int ox, int oy) {
  memset(ctx, 0, sizeof ctx);
  memset(world, 0, sizeof world);
  memset(ports, 0, sizeof ports);
  memset(entries, 0, sizeof entries);
  memset(graphics, 0, sizeof graphics);
  memset(style, 0, sizeof style);
  memset(objects, 0, sizeof objects);
  memset(clips, 0, sizeof clips);
  pointer(ctx + 0x36, options);
  pointer(ctx + 0xe, world);
  pointer(ctx + 0x22, ports);
  pointer(ports, entries);
  pointer(ctx + 0xa, graphics);
  pointer(graphics, style);
  pointer(ctx + 0x3a, objects);
  put32(world + 0x24, ww);
  put32(world + 0x28, wh);
  put32(world + 0x2c, dw);
  put32(world + 0x30, dh);
  put16(ports + 8, 1);
  entries[0] = 1;
  put32(entries + 0x6c, 1);
  unsigned char *p = entries + 0x78;
  p[0] = type;
  put32(p + 0x6c, 2);
  put32(p + 3, type == 1 ? 25 : 0);
  put32(p + 7, type == 1 ? 25 : 0);
  put32(p + 0xb, dw);
  put32(p + 0xf, dh);
  put32(p + 0x3a, ox);
  put32(p + 0x3e, oy);
  put32(p + 0x42, ox + 20);
  put32(p + 0x46, oy + 20);
  style[0x1d] = 1;
  errors = pixels = fills = pen_calls = pen_width = drawn_dc = visible = 0;
}
static void invoke(unsigned rva, unsigned *args) {
  ((void (*)(void *, unsigned, void *, void *))(image + rva))(ctx, 0, args,
                                                              NULL);
}
static void pen(unsigned flags, unsigned thickness) {
  unsigned args[] = {flags, 0, 0, thickness};
  invoke(0x1cd15, args);
}
static void point_checks(void) {
  unsigned frames[][4] = {
      {1280, 960, 640, 480}, {640, 350, 640, 480}, {640, 480, 320, 480}};
  unsigned widths[] = {1, 2, 3, 4, 255, 256};
  for (unsigned f = 0; f < 3; ++f)
    for (unsigned type = 1; type <= 4; type += 3)
      for (unsigned origins = 0; origins < 2; ++origins)
        for (unsigned w = 0; w < 6; ++w) {
          int ox = origins ? (type == 1 ? 65 : 40) : 0, oy = ox;
          prepare(frames[f][0], frames[f][1], frames[f][2], frames[f][3], type,
                  ox, oy);
          pen(0, widths[w]);
          require(!errors && pen_calls == 1 && pen_width == (widths[w] & 255),
                  "original pen stores low byte without ordinary coordinate "
                  "scaling");
          unsigned args[] = {2, 2};
          int x = 2 * frames[f][2] / frames[f][0],
              y = 2 * frames[f][3] / frames[f][1];
          unsigned dc = type == 1 ? 1 : 2;
          invoke(0x1e1d1, args);
          require(!errors && pixels == 1 && !fills && drawn_dc == dc,
                  "original PIXEL uses selected screen/offscreen DC");
          require(pixel_x == x && pixel_y == y,
                  "original PIXEL passes converted coordinates without origin "
                  "offset");
          require(!memcmp(draw_clip, (int[]){ox, oy, ox + 20, oy + 20},
                          sizeof draw_clip),
                  "PIXEL uses original viewport clipping setup");
          require(visible == !origins,
                  "unoffset PIXEL outside shifted viewport is clipped");
          invoke(0x1e2f8, args);
          require(!errors && fills == 1 && drawn_dc == dc,
                  "original POINT fills selected screen/offscreen DC");
          unsigned radius = (widths[w] & 255) / 2;
          require(!memcmp(filled,
                          (int[]){ox + x - (int)radius, oy + y - (int)radius,
                                  ox + x + 1 + (int)radius,
                                  oy + y + 1 + (int)radius},
                          sizeof filled),
                  "POINT applies viewport offset and expands to odd square "
                  "from stored thickness");
          require(visible, "POINT intersects viewport at shifted origin");
        }
}
static unsigned __attribute__((stdcall)) empty(const int *r) {
  return r[0] >= r[2] || r[1] >= r[3];
}
static void viewport_command_checks(void) {
  prepare(1280, 960, 640, 480, 1, 25, 25);
  unsigned viewport_args[] = {80, 80, 118, 118};
  invoke(0x20f87, viewport_args);
  require(!errors && !memcmp(entries + 0x78 + 0x3a, (int[]){40, 40, 60, 60},
                             sizeof filled),
          "original viewport command stores converted rectangle without adding "
          "screen-port origin");
  unsigned xy[] = {2, 2};
  invoke(0x1e2f8, xy);
  require(!errors && !memcmp(filled, (int[]){41, 41, 42, 42}, sizeof filled),
          "POINT uses stored viewport origin, not an extra port-origin sum");
  invoke(0x1e1d1, xy);
  require(pixel_x == 1 && pixel_y == 1 && !visible,
          "PIXEL ignores newly defined viewport origin and is clipped");
}
static void thickness_checks(void) {
  unsigned frames[][4] = {{1280, 960, 640, 480},
                          {640, 350, 640, 480},
                          {1280, 350, 640, 480},
                          {640, 960, 640, 480},
                          {640, 480, 1280, 480}};
  for (unsigned f = 0; f < 5; ++f)
    for (unsigned flags = 0; flags < 4; ++flags)
      for (unsigned thick = 0; thick < 4096; ++thick) {
        prepare(frames[f][0], frames[f][1], frames[f][2], frames[f][3], 4, 0,
                0);
        pen(flags, thick);
        unsigned effective = thick ? thick : 1;
        if (flags & 2) {
          effective = effective * frames[f][2] / frames[f][0];
          if (!effective)
            effective = 1;
        }
        require(
            !errors && pen_calls == 1 && pen_width == (effective & 255) &&
                style[0x1d] == (effective & 255),
            "pen width: clamp, optional X scale, clamp, then byte truncation");
        require(!!(style[0x21] & 4) == (flags == 1),
                "background-off-pixel state is set only for first field "
                "exactly one");
      }
  prepare(1280, 960, 640, 480, 4, 0, 0);
  pen(4, 3);
  require(errors == 1 && !pen_calls, "first field above three is rejected");
}
static void descriptor_checks(void) {
  unsigned char parser[256] = {0};
  pointer(parser + 0x30, image + 0x80820 + 14 * 40);
  for (unsigned xy = 1; xy <= 4; ++xy) {
    parser[0x39] = xy;
    require(((unsigned (*)(void *, unsigned))(image + 0x39de0))(parser, 3) == 2,
            "line thickness descriptor always consumes two characters");
    for (unsigned base = 36; base <= 64; base += 28) {
      parser[0x38] = base;
      require(((unsigned (*)(void *))(image + 0x39d70))(parser) == (base == 36),
              "line style numeric radix follows current base math");
    }
  }
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  unsigned hooks[] = {0x45038, 0x18cf6, 0x18bf7, 0x3988f, 0x398ee, 0x13c69};
  for (unsigned n = 0; n < sizeof hooks / sizeof hooks[0]; ++n)
    hook(hooks[n], noop);
  hook(0x33821, (void (*)(void))zero);
  hook(0x3935b, (void (*)(void))zero);
  hook(0x3dce0, (void (*)(void))color);
  hook(0x38e47, (void (*)(void))report);
  pointer(image + 0x963d0, (void *)discard);
  pointer(image + 0x963c0, (void *)select_object);
  pointer(image + 0x96408, (void *)set_rop);
  pointer(image + 0x963d4, (void *)create_pen);
  pointer(image + 0x96618, (void *)set_rect);
  pointer(image + 0x96610, (void *)offset_rect);
  pointer(image + 0x96698, (void *)inflate_rect);
  pointer(image + 0x963e8, (void *)create_region);
  pointer(image + 0x963d8, (void *)select_clip);
  pointer(image + 0x963c8, (void *)set_pixel);
  pointer(image + 0x9667c, (void *)fill_rect);
  pointer(image + 0x96418, (void *)discard);
  pointer(image + 0x96690, (void *)empty);
  descriptor_checks();
  point_checks();
  viewport_command_checks();
  thickness_checks();
  printf("PASS: %u original pixel/point/pen checks\n", checks);
  return 0;
}
