/* Original RIPtel font-engine rotation oracle. Run through the isolated
 * wrapper. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
static unsigned char context[512], config[2048];
static void ptr(unsigned char *p, void *value) { memcpy(p, &value, 4); }
int main(int argc, char **argv) {
  if (argc != 3 || strlen(argv[2]) > 400)
    return 2;
  HMODULE dll = LoadLibraryA(argv[1]);
  printf("loaded %p error %lu\n", dll, GetLastError());
  fflush(stdout);
  if (!dll)
    return 1;
  ptr(context + 0x72, config);
  strcpy((char *)config + 0x16, argv[2]);
  int (*init)(void *) = (void *)((char *)dll + 0x3ff90);
  int result = init(context);
  printf("init %d engine %p vector %p\n", result, *(void **)(context + 0x5a),
         *(void **)(context + 0x4e));
  fflush(stdout);
  if (!result)
    return 1;
  unsigned char *base = (unsigned char *)dll;
  unsigned char *v = *(unsigned char **)(context + 0x4e);
  unsigned char *engine = *(unsigned char **)(context + 0x5a);
  int16_t (*lookup)(void *, const char *) = (void *)(base + 0x3fb50);
  int16_t (*realize)(void *, unsigned, unsigned, unsigned) =
      (void *)(base + 0x40170);
  int16_t (*glyph)(void *, void *, unsigned, unsigned, unsigned, void *,
                   unsigned, unsigned, int, int) = (void *)(base + 0x523f0);
  unsigned index = (uint16_t)lookup(context, "Marin");
  printf("font index %u\n", index);
  fflush(stdout);
  const uint16_t orientation[] = {0, 3, 2, 1};
  for (unsigned i = 0; i < 4; ++i) {
    *(uint16_t *)(v + 0x82) = orientation[i];
    *(uint16_t *)(v + 0x1cc4) = 0;
    int bytes = realize(context, index, 900, 0);
    int x0 = *(int16_t *)(v + 0xd0), y0 = *(int16_t *)(v + 0xd2),
        x1 = *(int16_t *)(v + 0xd4), y1 = *(int16_t *)(v + 0xd6);
    unsigned stride = (x1 - x0 + 7) / 8;
    if (stride & 1)
      ++stride;
    unsigned height = y1 - y0 + 1;
    printf("orientation %u engine %u bytes %d bounds %d,%d,%d,%d stride %u "
           "height %u\n",
           i, orientation[i], bytes, x0, y0, x1, y1, stride, height);
    fflush(stdout);
    if (bytes <= 0 || bytes > 65535 || stride * height > (unsigned)bytes)
      return 3;
    unsigned char *bitmap = malloc(bytes);
    if (!bitmap)
      return 3;
    memset(bitmap, 255, bytes);
    int ok = glyph(engine, *(void **)(v + 0x86), 'F', 0, 0, bitmap, stride,
                   height, -x0, y1);
    int16_t (*width)(void *, void *, unsigned, unsigned, unsigned) =
        (void *)(base + 0x4efd0);
    printf("glyph %d advance %d\n", ok,
           width(engine, *(void **)(v + 0x86), 'F', 0, 0));
    fflush(stdout);
    if (!ok)
      return 4;
    char filename[64];
    snprintf(filename, sizeof filename, "glyph-%u.pbm", i);
    FILE *f = fopen(filename, "wb");
    if (!f)
      return 4;
    fprintf(f, "P4\n%u %u\n", stride * 8, height);
    for (int at = 0; at < bytes; ++at)
      bitmap[at] ^= 255;
    fwrite(bitmap, 1, stride * height, f);
    fclose(f);
    free(bitmap);
  }
  return 0;
}
