/* Original RIPtel explicit bitmap/icon load routing and transparent-color
 * classifier probes. File lookup, final GDI display and palette services are
 * controlled. Original renderer, decoder, classifiers and compositor execute;
 * the native RGB surfaces are not an original Windows GDI implementation.
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
static unsigned char color_entry[5], validator_entry[5];
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
static void *__attribute__((stdcall)) lock_memory(void *handle) {
  return handle;
}
static unsigned __attribute__((stdcall)) unlock_memory(void *handle) {
  (void)handle;
  return 0;
}
static unsigned __attribute__((stdcall)) caps(unsigned dc, unsigned which) {
  (void)dc;
  return which == 12 ? 24 : 1;
}
static unsigned __attribute__((stdcall))
dialog(unsigned owner, const char *text, const char *title, unsigned flags) {
  (void)owner;
  (void)text;
  (void)title;
  (void)flags;
  ++errors;
  return 1;
}
static unsigned allocate(void **target, unsigned length) {
  *target = calloc(1, length);
  return *target != NULL;
}
static void release(void **target) {
  free(*target);
  *target = NULL;
}
static void color_key_checks(void) {
  unsigned char logical[770] = {0};
  memcpy(image + 0x16140, color_entry, 5);
  memcpy(image + 0x44385, validator_entry, 5);
  hook(0x447c0, noop);
  hook(0x44802, noop);
  prepare();
  pointer(palette + 5, logical);
  for (unsigned i = 0; i < 256; ++i) {
    logical[3 * i] = i & 63;
    logical[3 * i + 1] = i >> 6;
  }
  memcpy(logical + 200 * 3, logical + 7 * 3, 3);
  unsigned limits[] = {2, 16, 236};
  for (unsigned n = 0; n < 3; ++n) {
    put16(palette + 0x82b, limits[n]);
    for (unsigned i = 0; i < 256; ++i) {
      unsigned result =
          ((unsigned (*)(void *, unsigned, unsigned))(image + 0x16140))(context,
                                                                        i, 0) &
          0xffff;
      require(result == (i == 200 && i >= limits[n] ? 7 : i),
              "key translation preserves low indices; maps later drawing "
              "colors to first exact logical-palette match");
    }
  }
  unsigned values[] = {0, 1, 255, 256, 1295};
  for (unsigned n = 0; n < 5; ++n) {
    unsigned result = ((unsigned (*)(void *, unsigned, unsigned))(
                          image + 0x44385))(context, values[n], 0) &
                      0xffff;
    require(result == (values[n] <= 255),
            "bitmap key validator accepts only 0 through 255");
  }
}
static void mask_and_merge_checks(void) {
  pointer(image + 0x964e4, (void *)lock_memory);
  pointer(image + 0x964f4, (void *)unlock_memory);
  pointer(image + 0x96484, (void *)caps);
  pointer(image + 0x966f4, (void *)dialog);
  hook(0x65ca3, (void (*)(void))allocate);
  hook(0x65cbf, (void (*)(void))release);
  header(16, 4, 1);
  errors = 0;
  unsigned mask_handle =
      ((unsigned (*)(void *, unsigned, void *, unsigned, unsigned, unsigned,
                     unsigned))(image + 0x493c0))(context, 1, dib, 4, 1, 0, 0);
  require(mask_handle == 0 && errors == 1,
          "original transparent-mask creation rejects 16-bit source");
  unsigned char source[2048], target[2048], selection[4] = {0, 1, 0, 1};
  unsigned depths[] = {1, 4, 8, 16, 24, 32};
  for (unsigned n = 0; n < sizeof depths / sizeof depths[0]; ++n) {
    unsigned depth = depths[n];
    header(depth, 4, 1);
    memcpy(source, dib, sizeof dib);
    memcpy(target, dib, sizeof dib);
    unsigned char *src =
        ((unsigned char *(*)(void *))(image + 0x4e1ec))(source);
    unsigned char *dst =
        ((unsigned char *(*)(void *))(image + 0x4e1ec))(target);
    unsigned bytes = (4 * depth + 7) / 8;
    memset(src, 0, bytes);
    memset(dst, 255, bytes);
    unsigned ok = ((unsigned (*)(void *, unsigned, unsigned, void *, void *))(
        image + 0x49f90))(source, 4, 1, target, selection);
    require(ok == 1, "original merge accepts supported pixel depth");
    if (depth == 1)
      require((dst[0] & 0xf0) == 0x50,
              "1-bit merge retains selection-one pixels");
    else if (depth == 4)
      require(dst[0] == 0x0f && dst[1] == 0x0f,
              "4-bit merge retains selection-one pixels");
    else {
      unsigned step = depth / 8;
      for (unsigned x = 0; x < 4; ++x)
        for (unsigned byte = 0; byte < step; ++byte)
          require(
              dst[x * step + byte] == (selection[x] ? 255 : 0),
              "merge keeps destination for selection one and source for zero");
    }
  }
  header(32, 2, 1);
  unsigned char pixels[8] = {255, 255, 255, 255, 0, 0, 0, 0}, selected[4];
  unsigned keys[][3] = {{0, 0, 0}, {255, 255, 255}, {17, 34, 51}};
  for (unsigned n = 0; n < 3; ++n) {
    memset(selected, 0, sizeof selected);
    ((void (*)(void *, void *, void *, unsigned, unsigned, unsigned, unsigned))(
        image + 0x49ea0))(dib, pixels, selected, 4, keys[n][0], keys[n][1],
                          keys[n][2]);
    require(selected[0] == 0 && selected[1] == 0,
            "32-bit classifier does not mark matching black/white RGB as "
            "transparent");
  }
}
#include "riptel-bitmap-services.h"
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  memcpy(renderer_entry, image + 0x4a410, 5);
  memcpy(color_entry, image + 0x16140, 5);
  memcpy(validator_entry, image + 0x44385, 5);
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
  color_key_checks();
  classify_checks();
  mask_and_merge_checks();
  renderer_checks();
  printf("PASS: %u bitmap routing/renderer/mask/merge checks\n", checks);
  return 0;
}
