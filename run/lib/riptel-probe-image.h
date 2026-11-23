/* Shared preferred-base PE mapping and observation helpers for native i386
 * original-instruction probes. Entry-point wrappers verify SHA-256 first. */
#ifndef RIPTEL_PROBE_IMAGE_H
#define RIPTEL_PROBE_IMAGE_H
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
static unsigned char *image;
static unsigned checks;
static inline uint16_t u16(const void *p) {
  uint16_t v;
  memcpy(&v, p, 2);
  return v;
}
static inline uint32_t u32(const void *p) {
  uint32_t v;
  memcpy(&v, p, 4);
  return v;
}
static inline void put16(void *p, uint16_t v) { memcpy(p, &v, 2); }
static inline void put32(void *p, uint32_t v) { memcpy(p, &v, 4); }
static inline void pointer(void *p, void *v) {
  put32(p, (uint32_t)(uintptr_t)v);
}
static inline void fail(const char *s) {
  fprintf(stderr, "%s: %s\n", s, strerror(errno));
  exit(1);
}
static inline void require(int ok, const char *label) {
  ++checks;
  if (!ok) {
    fprintf(stderr, "FAIL: %s\n", label);
    exit(1);
  }
}
static inline void noop(void) {}
static inline void hook(uint32_t rva, void (*fn)(void)) {
  image[rva] = 0xe9;
  put32(image + rva + 1,
        (uint32_t)(uintptr_t)fn - (uint32_t)(uintptr_t)(image + rva + 5));
}
static inline void load_image(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f)
    fail("open DLL");
  if (fseek(f, 0, SEEK_END))
    fail("seek DLL");
  long size = ftell(f);
  rewind(f);
  require(size == 592896, "DLL size");
  unsigned char *raw = malloc((size_t)size);
  if (!raw)
    fail("allocate file");
  require(fread(raw, 1, (size_t)size, f) == (size_t)size, "read DLL");
  fclose(f);
  uint32_t pe = u32(raw + 0x3c), opt = pe + 24;
  require(u16(raw + pe + 4) == 0x14c && u32(raw + opt + 28) == 0x10000000,
          "PE32 image identity");
  int flags = MAP_PRIVATE | MAP_ANON;
#ifdef __FreeBSD__
  flags |= MAP_FIXED | MAP_EXCL;
#elif defined(MAP_FIXED_NOREPLACE)
  flags |= MAP_FIXED_NOREPLACE;
#endif
  image = mmap((void *)0x10000000, 0xa1000, PROT_READ | PROT_WRITE | PROT_EXEC,
               flags, -1, 0);
  if (image == MAP_FAILED)
    fail("map driver");
  require(image == (void *)0x10000000, "preferred image base available");
  for (unsigned i = 0; i < u16(raw + pe + 6); ++i) {
    unsigned char *s = raw + opt + u16(raw + pe + 20) + 40 * i;
    uint32_t rva = u32(s + 12), length = u32(s + 16), offset = u32(s + 20);
    require((uint64_t)offset + length <= (unsigned long)size &&
                (uint64_t)rva + length <= 0xa1000,
            "section bounds");
    memcpy(image + rva, raw + offset, length);
  }
  free(raw);
}
#endif
