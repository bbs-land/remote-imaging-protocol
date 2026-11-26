/* Original PORT_WRITE handler/capture/writer; RAM GDI and CRT file services.
 * No original artifact changes, GUI, or real output files. */
#include "riptel-probe-image.h"
#include <strings.h>
static unsigned char ctx[512], ports[128], entries[36 * 0x78], palette[0x900],
    options[512];
static unsigned char dib[4096], file_data[8192];
static uint32_t source_pixels[256], captured_pixels[256];
static unsigned bitmap_w, bitmap_h, file_size, reports, opens, captures;
static unsigned source_w = 8, source_h = 4, selected;
static const char *last_error;
static void report(void *c, unsigned level, const char *where, const char *s) {
  (void)c;
  (void)level;
  (void)where;
  ++reports;
  last_error = s;
}
static void path(void *c, char *s) {
  (void)c;
  (void)s;
}
static void split(const char *s, char *drive, char *dir, char *name,
                  char *ext) {
  require(!strcmp(s, "OUT.BMP"), "owned export filename");
  *drive = *dir = 0;
  strcpy(name, "OUT");
  strcpy(ext, ".BMP");
}
static void make(char *out, const char *drive, const char *dir,
                 const char *name, const char *ext) {
  require(!*drive && !*dir && !strcmp(name, "OUT") && !strcmp(ext, ".BMP"),
          "original path reconstruction");
  strcpy(out, "OUT.BMP");
}
static void *open_file(const char *name, const char *mode) {
  require(!strcmp(name, "OUT.BMP") && !strcmp(mode, "w+b"),
          "original export opens binary replacement");
  ++opens;
  file_size = 0;
  return file_data;
}
static unsigned write_file(const void *p, unsigned size, unsigned count,
                           void *f) {
  require(f == file_data && file_size + size * count <= sizeof file_data,
          "bounded RAM output");
  memcpy(file_data + file_size, p, size * count);
  file_size += size * count;
  return count;
}
static unsigned close_file(void *f) {
  require(f == file_data, "close owned output");
  return 0;
}
static unsigned __attribute__((stdcall)) set_rect(int *r, int x, int y,
                                                  int right, int bottom) {
  memcpy(r, (int[]){x, y, right, bottom}, 16);
  return 1;
}
static unsigned __attribute__((stdcall)) client_rect(unsigned hwnd, int *r) {
  (void)hwnd;
  return set_rect(r, 0, 0, 8, 4);
}
static unsigned __attribute__((stdcall)) create_dc(unsigned ref) {
  require(ref == 101, "capture compatible DC reference");
  selected = 1;
  return 200;
}
static unsigned __attribute__((stdcall)) create_bitmap(unsigned ref, int w,
                                                       int h) {
  require(ref == 100, "capture bitmap device reference");
  if (w <= 0 || h <= 0 || w * h > 256)
    return 0;
  bitmap_w = w;
  bitmap_h = h;
  memset(captured_pixels, 0, sizeof captured_pixels);
  return 300;
}
static unsigned __attribute__((stdcall)) select_object(unsigned dc,
                                                       unsigned object) {
  require(dc == 200 && (object == 300 || object == 1),
          "owned bitmap selection");
  unsigned old = selected;
  selected = object;
  return old;
}
static unsigned __attribute__((stdcall))
select_palette(unsigned dc, unsigned pal, unsigned force) {
  require(dc == 200 && pal == 7 && force == 0, "controlled capture palette");
  return 1;
}
static unsigned __attribute__((stdcall)) bitblt(unsigned dc, int x, int y,
                                                int w, int h, unsigned source,
                                                int sx, int sy, unsigned op) {
  require(dc == 200 && source == 100 && selected == 300 && x == 0 && y == 0 &&
              op == 0xcc0020 && (unsigned)w == bitmap_w &&
              (unsigned)h == bitmap_h,
          "original screen writer requests exact COPY rectangle");
  ++captures;
  for (int j = 0; j < h; ++j)
    for (int i = 0; i < w; ++i) {
      require(sx + i >= 0 && sy + j >= 0 && sx + i < (int)source_w &&
                  sy + j < (int)source_h,
              "screen capture sample within owned source");
      captured_pixels[j * w + i] = source_pixels[(sy + j) * source_w + sx + i];
    }
  return 1;
}
static void *to_dib(unsigned bitmap, unsigned pal) {
  require((bitmap == 300 || bitmap == 400) && pal == 7,
          "owned HBITMAP to DIB service");
  unsigned w = bitmap == 400 ? source_w : bitmap_w,
           h = bitmap == 400 ? source_h : bitmap_h;
  uint32_t *pixels = bitmap == 400 ? source_pixels : captured_pixels;
  unsigned stride = (w * 3 + 3) & ~3u;
  memset(dib, 0, sizeof dib);
  put32(dib, 40);
  put32(dib + 4, w);
  put32(dib + 8, h);
  put16(dib + 12, 1);
  put16(dib + 14, 24);
  put32(dib + 20, stride * h);
  for (unsigned y = 0; y < h; ++y)
    for (unsigned x = 0; x < w; ++x) {
      unsigned char *p = dib + 40 + (h - 1 - y) * stride + 3 * x;
      uint32_t v = pixels[y * w + x];
      p[0] = v;
      p[1] = v >> 8;
      p[2] = v >> 16;
    }
  return dib;
}
static void *__attribute__((stdcall)) lock(void *p) {
  require(p == dib, "lock owned DIB");
  return p;
}
static unsigned __attribute__((stdcall)) unlock(void *p) {
  require(p == dib, "unlock owned DIB");
  return 1;
}
static unsigned __attribute__((stdcall)) size(void *p) {
  require(p == dib, "size owned DIB");
  return 40 + u32(dib + 20);
}
static void *__attribute__((stdcall)) free_dib(void *p) {
  require(p == dib, "release owned DIB");
  return NULL;
}
static unsigned __attribute__((stdcall)) destroy(unsigned object) {
  require(object == 200 || object == 300, "delete owned capture resources");
  return 1;
}
static unsigned __attribute__((stdcall)) dialog(unsigned a, const char *b,
                                                const char *c, unsigned d) {
  (void)a;
  (void)b;
  (void)c;
  (void)d;
  require(0, "unexpected trapped UI");
  return 0;
}
static unsigned zero(void) { return 0; }
static void prepare(unsigned type, unsigned flags) {
  memset(ctx, 0, sizeof ctx);
  memset(ports, 0, sizeof ports);
  memset(entries, 0, sizeof entries);
  memset(palette, 0, sizeof palette);
  memset(options, 0, sizeof options);
  pointer(ctx + 0x22, ports);
  pointer(ports, entries);
  pointer(ctx + 0x36, options);
  pointer(ctx + 0x1a, palette);
  put32(palette + 0x82d, 7);
  put32(ctx + 0x62, 101);
  put32(ctx + 0x6a, 100);
  unsigned char *p = entries + 0x78;
  p[0] = type;
  p[0x17] = flags;
  memcpy(p + 0x3a, (int[]){0, 0, 8, 4}, 16);
  put32(p + 0x74, 400);
  source_w = 8;
  source_h = 4;
  for (unsigned y = 0; y < source_h; ++y)
    for (unsigned x = 0; x < source_w; ++x)
      source_pixels[y * source_w + x] = 0x010000 + y * 16 + x;
  memcpy(file_data, "UNCHANGED", 9);
  file_size = 9;
  reports = opens = captures = 0;
  last_error = NULL;
}
static void invoke(unsigned x, unsigned y, unsigned right, unsigned bottom) {
  unsigned char old_ctx[sizeof ctx], old_entries[sizeof entries];
  memcpy(old_ctx, ctx, sizeof ctx);
  memcpy(old_entries, entries, sizeof entries);
  uint32_t args[] = {1, x, y, right, bottom, 0, 0};
  ((void (*)(void *, unsigned, void *, const char *))(image + 0x4699a))(
      ctx, 7, args, "OUT.BMP");
  require(!memcmp(old_ctx, ctx, sizeof ctx) &&
              !memcmp(old_entries, entries, sizeof entries),
          "export leaves current port, stored geometry, protection and cursor "
          "unchanged");
}
static void verify(unsigned x, unsigned y, unsigned w, unsigned h,
                   unsigned offscreen) {
  unsigned stride = (w * 3 + 3) & ~3u;
  require(!reports && opens == 1 && u16(file_data) == 0x4d42 &&
              u32(file_data + 18) == w && u32(file_data + 22) == h &&
              file_size == 54 + stride * h && u32(file_data + 10) == 54,
          "original serializer writes expected BMP dimensions and headers");
  require(captures == !offscreen,
          "screen crops through capture; offscreen writes its entire bitmap");
  for (unsigned j = 0; j < h; ++j)
    for (unsigned i = 0; i < w; ++i) {
      unsigned char *p = file_data + 54 + (h - 1 - j) * stride + i * 3;
      uint32_t value = p[0] | (p[1] << 8) | (p[2] << 16);
      require(value == 0x010000 + (y + j) * 16 + x + i,
              "exact output pixel labels and bottom-up rows");
    }
}
int main(int argc, char **argv) {
  require(argc == 2, "original DLL argument");
  load_image(argv[1]);
  hook(0x45038, noop);
  hook(0x3f776, (void (*)(void))path);
  hook(0x38e47, (void (*)(void))report);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x65442, (void (*)(void))strcat);
  hook(0x655e2, (void (*)(void))strcasecmp);
  hook(0x68dd0, (void (*)(void))strlen);
  hook(0x69e40, (void (*)(void))strrchr);
  hook(0x32cc5, (void (*)(void))zero);
  hook(0x4de5f, (void (*)(void))to_dib);
  hook(0x6cc30, (void (*)(void))split);
  hook(0x6cb90, (void (*)(void))make);
  hook(0x68f90, (void (*)(void))open_file);
  hook(0x69090, (void (*)(void))write_file);
  hook(0x68ec0, (void (*)(void))close_file);
  pointer(image + 0x96618, (void *)set_rect);
  pointer(image + 0x96644, (void *)client_rect);
  pointer(image + 0x964a0, (void *)create_dc);
  pointer(image + 0x96460, (void *)create_bitmap);
  pointer(image + 0x963c0, (void *)select_object);
  pointer(image + 0x963f4, (void *)select_palette);
  pointer(image + 0x963e4, (void *)bitblt);
  pointer(image + 0x964ac, (void *)destroy);
  pointer(image + 0x963d0, (void *)destroy);
  pointer(image + 0x964e4, (void *)lock);
  pointer(image + 0x964f4, (void *)unlock);
  pointer(image + 0x964ec, (void *)free_dib);
  pointer(image + 0x964b8, (void *)size);
  pointer(image + 0x966f4, (void *)dialog);
  for (unsigned off = 0; off < 2; ++off)
    for (unsigned flags = 0; flags < 4; ++flags)
      for (unsigned x = 0; x < 8; ++x)
        for (unsigned y = 0; y < 4; ++y) {
          prepare(off ? 4 : 1, flags);
          invoke(x, y, 0, 0);
          verify(off ? 0 : x, off ? 0 : y, off ? 8 : 8 - x, off ? 4 : 4 - y,
                 off);
          for (unsigned right = x + 1; right <= 10; ++right)
            for (unsigned bottom = y + 1; bottom <= 6; ++bottom) {
              prepare(off ? 4 : 1, flags);
              invoke(x, y, right, bottom);
              verify(off ? 0 : x, off ? 0 : y,
                     off ? 8 : (right > 8 ? 8 : right) - x,
                     off ? 4 : (bottom > 4 ? 4 : bottom) - y, off);
            }
        }
  prepare(4, 2);
  source_w = 10;
  source_h = 6;
  for (unsigned y = 0; y < source_h; ++y)
    for (unsigned x = 0; x < source_w; ++x)
      source_pixels[y * source_w + x] = 0x010000 + y * 16 + x;
  invoke(1, 1, 9, 2);
  verify(0, 0, 10, 6, 1);
  prepare(0, 0);
  invoke(0, 0, 0, 0);
  require(reports == 1 && !strcmp(last_error, "Drawing port is not in use") &&
              !opens && file_size == 9 && !memcmp(file_data, "UNCHANGED", 9),
          "undefined source diagnoses and preserves file");
  prepare(1, 2);
  invoke(1, 1, 1, 2);
  require(reports == 1 && !opens && file_size == 9,
          "explicit empty rectangle preserves file before capture");
  printf("PASS: %u PORT_WRITE geometry/serialized pixel checks\n", checks);
  return 0;
}
