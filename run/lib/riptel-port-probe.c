/* Original port allocation and copy geometry, with controlled OS services.
 * Captures final rectangles without emulating Windows rasterization. */
#include "riptel-probe-image.h"
#include <stdarg.h>
#include <strings.h>
static unsigned char ctx[512], ports[128], entries[36 * 0x78], world[128],
    palette[0x900], options[0x400], vars[0x700];
static unsigned errors, allocated_w, allocated_h, blits, stretches;
static int captured[10];
static char result[64];
static unsigned zero(void) { return 0; }
static void report(void *c, unsigned level, const char *where,
                   const char *why) {
  (void)c;
  (void)level;
  fprintf(stderr, "captured error: %s: %s\n", where, why);
  ++errors;
}
static unsigned area(unsigned w, unsigned h) {
  return (unsigned short)w * (unsigned short)h;
}
static void switch_port(void *c, unsigned n) {
  (void)c;
  put16(ports + 8, n);
}
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
static unsigned __attribute__((stdcall)) bitmap(unsigned dc, unsigned w,
                                                unsigned h) {
  (void)dc;
  allocated_w = w;
  allocated_h = h;
  return 7;
}
static unsigned __attribute__((stdcall)) set_rect(int *r, int x, int y,
                                                  int right, int bottom) {
  r[0] = x;
  r[1] = y;
  r[2] = right;
  r[3] = bottom;
  return 1;
}
static unsigned __attribute__((stdcall)) offset_rect(int *r, int x, int y) {
  r[0] += x;
  r[2] += x;
  r[1] += y;
  r[3] += y;
  return 1;
}
static unsigned __attribute__((stdcall)) empty(const int *r) {
  return r[0] >= r[2] || r[1] >= r[3];
}
static unsigned __attribute__((stdcall)) bitblt(unsigned dst, int x, int y,
                                                int w, int h, unsigned src,
                                                int sx, int sy, unsigned op) {
  (void)dst;
  (void)src;
  (void)op;
  ++blits;
  memcpy(captured, (int[]){x, y, w, h, sx, sy, w, h}, 8 * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall)) stretchblt(unsigned dst, int x, int y,
                                                    int w, int h, unsigned src,
                                                    int sx, int sy, int sw,
                                                    int sh, unsigned op) {
  (void)dst;
  (void)src;
  (void)op;
  ++stretches;
  memcpy(captured, (int[]){x, y, w, h, sx, sy, sw, sh}, 8 * sizeof(int));
  return 1;
}
static void formatted(char *dst, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsnprintf(dst, 64, fmt, args);
  va_end(args);
}
static void invoke(unsigned rva, unsigned *args) {
  ((void (*)(void *, unsigned, void *, void *))(image + rva))(ctx, 0, args,
                                                              NULL);
}
static void viewport(unsigned n, int w, int h) {
  unsigned char *p = entries + n * 0x78;
  p[0] = 4;
  put16(p + 1, n);
  put32(p + 0xb, w);
  put32(p + 0xf, h);
  put16(p + 0x13, w);
  put16(p + 0x15, h);
  put32(p + 0x42, w);
  put32(p + 0x46, h);
  put32(p + 0x6c, n + 1);
  put32(p + 0x74, n + 7);
}
static void prepare(unsigned ww, unsigned wh, unsigned dw, unsigned dh) {
  memset(ctx, 0, sizeof ctx);
  memset(ports, 0, sizeof ports);
  memset(entries, 0, sizeof entries);
  memset(world, 0, sizeof world);
  memset(palette, 0, sizeof palette);
  memset(options, 0, sizeof options);
  memset(vars, 0, sizeof vars);
  pointer(ctx + 0x22, ports);
  pointer(ports, entries);
  pointer(ctx + 0xe, world);
  pointer(ctx + 0x1a, palette);
  pointer(ctx + 0x36, options);
  pointer(ctx + 0x26, vars);
  pointer(vars + 0x432, result);
  put32(world + 0x24, ww);
  put32(world + 0x28, wh);
  put32(world + 0x2c, dw);
  put32(world + 0x30, dh);
  put32(ports + 0x22, 0x10000000);
  viewport(0, dw, dh);
  errors = allocated_w = allocated_h = blits = stretches = 0;
  memset(captured, 0, sizeof captured);
}
static void size_queries(void) {
  char params[3 * 21] = {0};
  strcpy(params, "CUR");
  strcpy(params + 21, "PORT");
  strcpy(params + 42, "DEVICE");
  unsigned routines[] = {0x2f863, 0x2f642};
  for (unsigned n = 0; n < 2; ++n)
    for (unsigned count = 0; count <= 3; ++count) {
      unsigned char decoded[12] = {0};
      unsigned status = ((unsigned (*)(void *, unsigned, void *, void *))(
                            image + 0x2f743))(ctx, count, params, decoded) &
                        0xffff;
      require(status == 1 && u16(decoded) == 1,
              "original valid size-query parser returns AX one");
      put16(vars + 0x436, 0);
      const char *r = ((const char *(*)(void *, unsigned, void *))(
          image + routines[n]))(ctx, count, params);
      require(r == NULL && u16(vars + 0x436) == 1,
              "original width/height callback rejects valid parser success");
    }
}

int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  unsigned hooks[] = {0x45038, 0x343e4, 0x342ad, 0x34319, 0x18cf6,
                      0x18bf7, 0x3988f, 0x398ee, 0x13c69};
  for (unsigned i = 0; i < sizeof hooks / sizeof hooks[0]; ++i)
    hook(hooks[i], noop);
  hook(0x33821, (void (*)(void))zero);
  hook(0x3393c, (void (*)(void))switch_port);
  hook(0x339e4, (void (*)(void))area);
  hook(0x38e47, (void (*)(void))report);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x655e2, (void (*)(void))strcasecmp);
  hook(0x68440, (void (*)(void))formatted);
  pointer(image + 0x96618, (void *)set_rect);
  pointer(image + 0x96610, (void *)offset_rect);
  pointer(image + 0x96690, (void *)empty);
  unsigned singles[] = {0x964a0, 0x96490, 0x9648c, 0x96494, 0x9640c, 0x963e0};
  for (unsigned i = 0; i < 6; ++i)
    pointer(image + singles[i], (void *)one_arg);
  unsigned doubles[] = {0x963f0, 0x96400, 0x963f8, 0x963c0};
  for (unsigned i = 0; i < 4; ++i)
    pointer(image + doubles[i], (void *)two_args);
  pointer(image + 0x9667c, (void *)three_args);
  pointer(image + 0x96460, (void *)bitmap);
  pointer(image + 0x963e4, (void *)bitblt);
  pointer(image + 0x96488, (void *)stretchblt);
  prepare(1280, 960, 640, 480);
  unsigned define[] = {1, 0, 0, 1280, 960, 3, 0};
  invoke(0x466ec, define);
  require(!errors && allocated_w == 640 && allocated_h == 480,
          "full-world offscreen allocation is exactly device extent");
  require(u16(entries + 0x78 + 0x13) == 640 &&
              u16(entries + 0x78 + 0x15) == 480,
          "stored port extents exclude extra edge");
  require(u16(ports + 8) == 1, "define flag 2 switches to new port");
  size_queries();
  prepare(1280, 960, 640, 480);
  unsigned larger[] = {1, 0, 0, 2560, 1920, 3, 0};
  invoke(0x466ec, larger);
  require(!errors && allocated_w == 1280 && allocated_h == 960,
          "offscreen allocation is not clamped to display extents");
  for (unsigned mode = 0; mode < 5; ++mode) {
    prepare(4, 1, 4, 1);
    viewport(1, 4, 1);
    viewport(0, 2, 1);
    unsigned copy[] = {1, 0, 0, 4, 1, 0, 1, 0, 0, 0, mode, 0};
    invoke(0x46372, copy);
    require(!errors && blits == 1 && !stretches,
            "zero-endpoint verbatim copy stays unscaled after clipping");
    require(!memcmp(captured, (int[]){1, 0, 1, 1, 0, 0, 1, 1}, 8 * sizeof(int)),
            "4x1 to 2x1 at x1 copies only first source pixel");
    blits = stretches = 0;
    copy[6] = 0;
    invoke(0x46372, copy);
    require(!errors && !blits && stretches == 1,
            "all-zero destination takes full viewport precedence");
    require(!memcmp(captured, (int[]){0, 0, 2, 1, 0, 0, 4, 1}, 8 * sizeof(int)),
            "all-zero destination fits whole source to viewport");
  }
  /* Equal explicit rectangles keep no-scale semantics; an explicit smaller
   * target uses the fit path. Only final geometry, not filter output, matters.
   */
  for (unsigned sw = 1; sw <= 8; ++sw)
    for (unsigned dw = 1; dw <= 8; ++dw)
      for (unsigned x = 0; x < dw; ++x)
        for (unsigned fit = 0; fit < 2; ++fit) {
          prepare(16, 16, 16, 16);
          viewport(1, sw, 1);
          viewport(0, dw, 1);
          unsigned end = fit ? dw : x + sw;
          unsigned copy[] = {1, 0, 0, sw, 1, 0, x, 0, end, 1, 0, 0};
          invoke(0x46372, copy);
          unsigned final_w = end > dw ? dw - x : end - x;
          unsigned scaled = (end - x) != sw;
          unsigned source_w = scaled ? sw : final_w;
          require(!errors && blits == !scaled && stretches == scaled,
                  "copy chooses scale before trim across width matrix");
          require(
              !memcmp(
                  captured,
                  (int[]){(int)x, 0, (int)final_w, 1, 0, 0, (int)source_w, 1},
                  8 * sizeof(int)),
              "equal-size trim adjusts counterpart; unequal-size trim fits");
        }
  prepare(4, 4, 4, 4);
  viewport(1, 1, 4);
  viewport(0, 1, 2);
  unsigned vertical[] = {1, 0, 0, 1, 4, 0, 0, 1, 0, 0, 0, 0};
  invoke(0x46372, vertical);
  require(
      !errors && blits == 1 && !stretches &&
          !memcmp(captured, (int[]){0, 1, 1, 1, 0, 0, 1, 1}, 8 * sizeof(int)),
      "vertical verbatim clipping also preserves one-to-one dimensions");
  for (unsigned fit = 0; fit < 2; ++fit) {
    prepare(8, 1, 8, 1);
    viewport(1, 2, 1);
    viewport(0, 4, 1);
    unsigned copy[] = {1, 0, 0, 4, 1, 0, 1, 0, fit ? 4 : 0, fit ? 1 : 0, 0, 0};
    invoke(0x46372, copy);
    require(!errors && blits == !fit && stretches == fit,
            "source clipping preserves initial scale choice");
    require(!memcmp(captured, (int[]){1, 0, fit ? 3 : 2, 1, 0, 0, 2, 1},
                    8 * sizeof(int)),
            "source clipping shortens paired destination only without scaling");
  }
  printf("PASS: %u port allocation/query/copy geometry checks\n",
         checks);
  return 0;
}
