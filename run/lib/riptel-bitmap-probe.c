/* Original RIPtel explicit bitmap/icon load routing and transparent-color
 * classifier probes. File lookup, final GDI display and palette services are
 * controlled. This does not claim complete bitmap rendering or BMM semantics.
 */
#include "riptel-probe-image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
static unsigned char context[512], ports[128], entries[36 * 0x78],
    palette[0x900], options[0x400], scratch_buffer[4096], dib[2048];
static unsigned lookup_calls, display_calls, missing, errors, rop;
static int transparent;
static char lookup_name[256], displayed_name[256];
static unsigned zero(void) { return 0; }
static unsigned one(void) { return 1; }
static void *scratch(void *ctx) {
  (void)ctx;
  return scratch_buffer;
}
static char *filename(char *s) { return s; }
static unsigned lookup(void *ctx, char *name, unsigned mode, unsigned flag) {
  (void)ctx;
  (void)mode;
  (void)flag;
  ++lookup_calls;
  snprintf(lookup_name, sizeof lookup_name, "%s", name);
  return missing;
}
static void report(void *ctx, unsigned level, const char *where,
                   const char *message) {
  (void)ctx;
  (void)level;
  (void)where;
  (void)message;
  ++errors;
}
static unsigned color(void *ctx, unsigned value, unsigned ignored) {
  (void)ctx;
  (void)ignored;
  return value;
}
static unsigned display(void *ctx, unsigned dc, const char *name, int x0,
                        int y0, int x1, int y1, unsigned dither,
                        unsigned pal_mode, unsigned stretch, int key,
                        unsigned pal, int *rect, unsigned op, unsigned unknown,
                        unsigned noisy) {
  (void)ctx;
  (void)dc;
  (void)x0;
  (void)y0;
  (void)x1;
  (void)y1;
  (void)dither;
  (void)pal_mode;
  (void)stretch;
  (void)pal;
  (void)unknown;
  (void)noisy;
  ++display_calls;
  rop = op;
  transparent = (short)key;
  snprintf(displayed_name, sizeof displayed_name, "%s", name);
  memset(rect, 0, 16);
  return 0;
}
static void prepare(void) {
  memset(context, 0, sizeof context);
  memset(ports, 0, sizeof ports);
  memset(entries, 0, sizeof entries);
  memset(palette, 0, sizeof palette);
  memset(options, 0, sizeof options);
  pointer(context + 0x22, ports);
  pointer(ports, entries);
  pointer(context + 0x1a, palette);
  pointer(context + 0x36, options);
  lookup_calls = display_calls = missing = errors = 0;
  lookup_name[0] = displayed_name[0] = 0;
}
static void invoke(unsigned rva, unsigned *args, const char *name) {
  ((void (*)(void *, unsigned, void *, const char *))(image + rva))(context, 9,
                                                                    args, name);
}
static void load_checks(void) {
  unsigned ops[] = {0xcc0020, 0x660046, 0xee0086, 0x8800c6, 0x330008};
  for (unsigned icon = 0; icon <= 1; ++icon)
    for (unsigned mode = 0; mode <= 4; ++mode) {
      prepare();
      unsigned args[9] = {0};
      args[icon ? 3 : 5] = mode;
      invoke(icon ? 0xcb38 : 0xc569, args, "MASK.BMM");
      require(!errors && lookup_calls == 1 && display_calls == 1,
              "explicit BMM load does one normal lookup and display");
      require(!strcmp(lookup_name, "MASK.BMM") &&
                  !strcmp(displayed_name, "MASK.BMM"),
              "BMM filename is passed unchanged to ordinary display");
      require(rop == ops[mode] && transparent == -1,
              "explicit BMM uses requested ordinary ROP with no color key");
    }
  prepare();
  unsigned args[9] = {0};
  invoke(0xc569, args, "MASK.MSK");
  require(!strcmp(lookup_name, "MASK.BMM"),
          "legacy MSK references normalize to BMM");
  prepare();
  args[6] = 8;
  args[7] = 7;
  invoke(0xc569, args, "MASK.BMM");
  require(!errors && display_calls == 1 && transparent == 7,
          "transparent-color flag forwards key independently of BMM extension");
  prepare();
  missing = 1;
  args[6] = 0;
  /* The missing-file placeholder is captured to avoid any drawing. */
  invoke(0xc569, args, "MASK.BMM");
  require(lookup_calls == 1 && display_calls == 0,
          "missing explicit BMM does not cause a second paired image lookup");
}
static void header(unsigned depth, unsigned width, unsigned height) {
  memset(dib, 0, sizeof dib);
  put32(dib, 40);
  put32(dib + 4, width);
  put32(dib + 8, height);
  put16(dib + 12, 1);
  put16(dib + 14, depth);
}
static void classify_checks(void) {
  unsigned char bits[64] = {0}, mask[64];
  header(1, 4, 1);
  bits[0] = 0x50; /* 0,1,0,1, high-bit first. */
  memset(mask, 0, sizeof mask);
  ((void (*)(void *, void *, void *, unsigned, unsigned))(image + 0x49ba0))(
      dib, bits, mask, 4, 0);
  require(!memcmp(mask, (unsigned char[]){1, 0, 1, 0}, 4),
          "1-bit transparent classifier matches index zero");
  memset(mask, 0, sizeof mask);
  ((void (*)(void *, void *, void *, unsigned, unsigned))(image + 0x49ba0))(
      dib, bits, mask, 4, 1);
  require(!memcmp(mask, (unsigned char[]){0, 1, 0, 1}, 4),
          "1-bit classifier matches index one without reading palette RGB");
  header(4, 4, 1);
  bits[0] = 0x17;
  bits[1] = 0x71;
  memset(mask, 0, sizeof mask);
  ((void (*)(void *, void *, void *, unsigned, unsigned))(image + 0x49c90))(
      dib, bits, mask, 4, 7);
  require(!memcmp(mask, (unsigned char[]){0, 1, 1, 0}, 4),
          "4-bit classifier compares nibble index to supplied key");
  header(8, 4, 1);
  memcpy(bits, (unsigned char[]){1, 7, 7, 1}, 4);
  memset(mask, 0, sizeof mask);
  ((void (*)(void *, void *, void *, unsigned, unsigned))(image + 0x49d60))(
      dib, bits, mask, 4, 7);
  require(!memcmp(mask, (unsigned char[]){0, 1, 1, 0}, 4),
          "8-bit classifier compares index, not generic nonzero pixels");
  header(24, 2, 1);
  memcpy(bits, (unsigned char[]){0x33, 0x22, 0x11, 0x33, 0x22, 0x12}, 6);
  memset(mask, 0, sizeof mask);
  ((void (*)(void *, void *, void *, unsigned, unsigned, unsigned, unsigned))(
      image + 0x49df0))(dib, bits, mask, 4, 0x11, 0x22, 0x33);
  require(mask[0] == 1 && mask[1] == 0,
          "24-bit classifier matches exact BGR components");
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  hook(0x45038, noop);
  hook(0x3445b, (void (*)(void))zero);
  hook(0x44385, (void (*)(void))one);
  hook(0x16140, (void (*)(void))color);
  hook(0x15396, (void (*)(void))scratch);
  hook(0x153fe, noop);
  hook(0x4b0e4, noop);
  hook(0x3f71a, (void (*)(void))filename);
  hook(0x3f80e, (void (*)(void))lookup);
  hook(0x31084, noop);
  hook(0x3112e, noop);
  hook(0x18cf6, noop);
  hook(0x18bf7, noop);
  hook(0x343e4, noop);
  hook(0x342ad, noop);
  hook(0x34319, noop);
  hook(0x3988f, noop);
  hook(0x398ee, noop);
  hook(0x383b0, noop);
  hook(0x38e47, (void (*)(void))report);
  hook(0x4a410, (void (*)(void))display);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x65442, (void (*)(void))strcat);
  hook(0x65453, (void (*)(void))strchr);
  hook(0x69e40, (void (*)(void))strrchr);
  hook(0x655e2, (void (*)(void))strcasecmp);
  load_checks();
  classify_checks();
  printf("PASS: %u preliminary bitmap routing/classifier checks\n", checks);
  return 0;
}
