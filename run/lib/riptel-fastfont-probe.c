/* Original FastFont rasterizer with explicitly synthetic copied A-H records.
 * Run only through the private-display/private-prefix wrapper. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
static unsigned char context[512], config[2048];
static void ptr(unsigned char *p, void *v) { memcpy(p, &v, 4); }
static int16_t word(const void *p) {
  int16_t v;
  memcpy(&v, p, 2);
  return v;
}
static unsigned pixel(const unsigned char *b, unsigned stride, unsigned height,
                      int x, int y) {
  if (x < 0 || y < 0 || (unsigned)x >= stride * 8 || (unsigned)y >= height)
    return 9;
  return !(b[(unsigned)y * stride + (unsigned)x / 8] & (0x80 >> (x & 7)));
}
int main(int argc, char **argv) {
  if (argc != 3 || strlen(argv[2]) > 400)
    return 2;
  setvbuf(stdout, NULL, _IONBF, 0);
  HMODULE dll = LoadLibraryA(argv[1]);
  printf("loaded %p error %lu\n", dll, GetLastError());
  if (!dll)
    return 1;
  unsigned char *base = (unsigned char *)dll;
  ptr(context + 0x72, config);
  strcpy((char *)config + 0x16, argv[2]);
  if (!((int (*)(void *))(base + 0x3ff90))(context))
    return 3;
  unsigned char *v = *(unsigned char **)(context + 0x4e);
  void *engine = *(void **)(context + 0x5a);
  int16_t (*lookup)(void *, const char *) = (void *)(base + 0x3fb50);
  int16_t (*realize)(void *, unsigned, unsigned, unsigned) =
      (void *)(base + 0x40170);
  int16_t (*glyph)(void *, void *, unsigned, unsigned, unsigned, void *,
                   unsigned, unsigned, int, int) = (void *)(base + 0x523f0);
  const char *styles[] = {"Marin",    "Marin Ho", "Marin HT",
                          "Marin HC", "Marin HW", "Marin HE"};
  const unsigned sizes[] = {250, 500, 900, 1200, 2000, 2500};
  for (unsigned style = 0; style < 6; ++style) {
    int index = lookup(context, styles[style]);
    if (index < 0) {
      printf("missing %s\n", styles[style]);
      return 4;
    }
    for (unsigned z = 0; z < 6; ++z) {
      unsigned size = sizes[z];
      *(uint16_t *)(v + 0x82) = 0;
      *(uint16_t *)(v + 0x1cc4) = 0;
      int length = realize(context, index, size, 0);
      int x0 = word(v + 0xd0), y0 = word(v + 0xd2), x1 = word(v + 0xd4),
          y1 = word(v + 0xd6);
      unsigned stride = (x1 - x0 + 7) / 8;
      if (stride & 1)
        ++stride;
      unsigned height = y1 - y0 + 1;
      if (length <= 0 || length > 65535 || stride * height > (unsigned)length)
        return 5;
      unsigned char *bits = malloc(length);
      if (!bits)
        return 5;
      unsigned char *reference_a = malloc(length),
                    *reference_c = malloc(length);
      if (!reference_a || !reference_c)
        return 5;
      unsigned char *rf = ((void *(*)(void *, void *))(base + 0x52340))(
          engine, *(void **)(v + 0x86));
      if (!rf)
        return 7;
      unsigned char *representation =
          ((void *(*)(void *, void *, unsigned, unsigned))(base + 0x56950))(
              engine, rf, 0, 0);
      if (!representation)
        return 7;
      unsigned expected_filter = style ? (unsigned)word(rf + 4) / 50 : 0;
      if (expected_filter > 4)
        expected_filter = 4;
      if (representation[3] != expected_filter) {
        printf("FAIL: original hollow filter index is size-based and "
               "percent-independent\n");
        return 9;
      }
      printf("realized style=%s p=%d percent=%d hollow=%u filter=%u display=",
             styles[style], word(rf + 4), word(rf + 0x2a), rf[0x2c],
             representation[3]);
      unsigned char *display =
          (unsigned char *)engine + 0x41a + (int8_t)rf[1] * 16;
      for (unsigned i = 0; i < 8; ++i)
        printf("%d,", word(display + 2 * i));
      putchar(10);
      ((void (*)(void *, void *))(base + 0x523c0))(engine, rf);

      for (unsigned c = 'A'; c <= 'H'; ++c) {
        memset(bits, 255, length);
        int ok = glyph(engine, *(void **)(v + 0x86), c, 0, 0, bits, stride,
                       height, -x0, y1);
        if (!ok) {
          printf("failed glyph %u\n", c);
          return 6;
        }
        if (c == 'A')
          memcpy(reference_a, bits, length);
        if (c == 'C')
          memcpy(reference_c, bits, length);
        if (style == 0 && c == 'E' &&
            (memcmp(reference_a, bits, length) ||
             memcmp(reference_c, bits, length))) {
          printf(
              "FAIL: nonzero same-direction nested/duplicate contour oracle\n");
          return 8;
        }
        if (style == 0 && (c == 'F' || c == 'G') &&
            memcmp(reference_a, bits, length)) {
          printf("FAIL: negative winding is filled equally\n");
          return 8;
        }
        unsigned ink = 0;
        for (unsigned y = 0; y < height; ++y)
          for (unsigned x = 0; x < stride * 8; ++x)
            ink += pixel(bits, stride, height, x, y);
        int percent[] = {100, 100, 85, 75, 125, 150};
        int midx = -x0 + (300 * (int)size * percent[style] + 500000) / 1000000;
        int midy = y1 - (300 * (int)size + 5000) / 10000;
        if (style == 0 && c == 'H' && pixel(bits, stride, height, midx, midy)) {
          printf(
              "FAIL: opposite duplicate winding cancels interior coverage\n");
          return 8;
        }

        printf("style=%s size=%u glyph=%c bounds=%d,%d,%d,%d ink=%u center=%u "
               "row=",
               styles[style], size, c, x0, y0, x1, y1, ink,
               pixel(bits, stride, height, midx, midy));
        for (unsigned x = 0; x < stride * 8; ++x)
          putchar(pixel(bits, stride, height, x, midy) == 1 ? '#' : '.');
        printf(" col=");
        for (unsigned y = 0; y < height; ++y)
          putchar(pixel(bits, stride, height, midx, y) == 1 ? '#' : '.');
        putchar(10);
      }
      free(reference_c);
      free(reference_a);
      free(bits);
    }
  }
  printf("PASS: 288 original-engine glyph measurements; solid nested/duplicate "
         "oracle\n");
  return 0;
}
