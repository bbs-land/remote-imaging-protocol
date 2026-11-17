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
static const char *last_error_message;
static char lookup_name[256], displayed_name[256];
static unsigned char color_entry[5], validator_entry[5], wallpaper_entry[5];
static unsigned render_dither, render_palette, render_stretch, render_result;
static int render_geometry[4], returned_rect[4] = {2, 3, 6, 5};
static unsigned wallpaper_calls, clipboard_calls, duplicate_calls, copy_calls;
static unsigned sequence, draw_sequence, duplicate_sequence, wallpaper_sequence;
static unsigned wallpaper_mode, wallpaper_current, duplicate_target;
static unsigned fail_delete, fail_create, retarget_clipboard;
static int wallpaper_source[4], wallpaper_bounds[4];
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
  last_error_message = message;
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
  memcpy(render_geometry, (int[]){(short)x0, (short)y0, (short)x1, (short)y1},
         16);
  render_dither = dither;
  render_palette = pal_mode;
  render_stretch = stretch;
  draw_sequence = ++sequence;
  (void)pal;
  (void)unknown;
  (void)noisy;
  ++display_calls;
  rop = op;
  transparent = (short)key;
  snprintf(displayed_name, sizeof displayed_name, "%s", name);
  memcpy(rect, returned_rect, 16);
  return render_result;
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
  wallpaper_calls = clipboard_calls = duplicate_calls = copy_calls = 0;
  sequence = draw_sequence = duplicate_sequence = wallpaper_sequence = 0;
  render_result = fail_delete = fail_create = retarget_clipboard = 0;
  last_error_message = NULL;
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
  const char *legacy[] = {"TEST.ICN", "TEST.HIC", "TEST.MSK",
                          "TEST.icn", "TEST.hic", "TEST.msk"};
  const char *normalized[] = {"TEST.BMP", "TEST.BMH", "TEST.BMM"};
  for (unsigned icon = 0; icon <= 1; ++icon)
    for (unsigned n = 0; n < 6; ++n)
      for (unsigned absent = 0; absent < 2; ++absent) {
        prepare();
        missing = absent;
        unsigned args[9] = {0};
        invoke(icon ? 0xcb38 : 0xc569, args, legacy[n]);
        require(
            lookup_calls == 1 && !strcmp(lookup_name, normalized[n % 3]),
            "legacy load names normalize before the only controlled lookup");
        require(
            display_calls == !absent,
            "missing normalized bitmap skips display without legacy lookup");
        if (!absent)
          require(!strcmp(displayed_name, normalized[n % 3]),
                  "legacy reference displays normalized BMP/BMH/BMM name");
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
/* Capture post-render requests, leaving the original flag/branch order intact.
 */
static void wallpaper_capture(void *c, const int *source, const int *bounds,
                              unsigned mode) {
  require(c == context, "wallpaper request context");
  ++wallpaper_calls;
  wallpaper_sequence = ++sequence;
  wallpaper_mode = mode & 0xffff;
  wallpaper_current = u16(ports + 8);
  memcpy(wallpaper_source, source, 16);
  memcpy(wallpaper_bounds, bounds, 16);
}
static unsigned clipboard_capture(void *c, const int *rect) {
  require(c == context && !memcmp(rect, returned_rect, 16),
          "clipboard receives rendered rectangle before wallpaper");
  ++clipboard_calls;
  duplicate_sequence = ++sequence;
  if (retarget_clipboard)
    put16(ports + 8, 0);
  return 1;
}
static unsigned duplicate_create(void *c, unsigned n, unsigned x, unsigned y,
                                 unsigned w, unsigned h, unsigned kind,
                                 unsigned enabled) {
  require(c == context && !x && !y && (short)w == 4 && (short)h == 2 &&
              kind == 1 && enabled == 1,
          "duplicate uses initial rendered size, not wallpaper viewport");
  ++duplicate_calls;
  duplicate_target = n & 0xffff;
  if (!fail_create)
    entries[duplicate_target * 0x78] = 4;
  return !fail_create;
}
static unsigned duplicate_delete(void *c, unsigned n, unsigned force) {
  require(c == context && n == 7 && force == 0,
          "explicit duplicate deletion arguments");
  return !fail_delete;
}
static unsigned duplicate_copy(void *c, void *source, const int *rect,
                               void *dest, void *dest_rect, unsigned mode) {
  (void)dest_rect;
  require(c == context && source == entries + 2 * 0x78 &&
              dest == entries + duplicate_target * 0x78 && mode == 0 &&
              !memcmp(rect, returned_rect, 16),
          "duplicate copies composited initial rectangle with COPY");
  ++copy_calls;
  duplicate_sequence = ++sequence;
  return 1;
}
static unsigned __attribute__((stdcall)) set_rect(int *, int, int, int, int);
static void flag_checks(void) {
  pointer(image + 0x96618, (void *)set_rect);
  hook(0x3ea2e, (void (*)(void))wallpaper_capture);
  hook(0x2866, (void (*)(void))clipboard_capture);
  hook(0x3326f, (void (*)(void))duplicate_create);
  hook(0x33112, (void (*)(void))duplicate_delete);
  hook(0x134d0, (void (*)(void))duplicate_copy);
  const unsigned duplicates[] = {0, 2, 7, 35, 36};
  const unsigned operations[] = {0xcc0020, 0x660046, 0xee0086, 0x8800c6,
                                 0x330008};
  for (unsigned flags = 0; flags < 64; ++flags)
    for (unsigned mode = 0; mode < 5; ++mode)
      for (unsigned sized = 0; sized < 2; ++sized)
        for (unsigned di = 0; di < sizeof duplicates / sizeof *duplicates;
             ++di) {
          prepare();
          put16(ports + 8, 2);
          memcpy(entries + 2 * 0x78 + 0x3a, (int[]){1, 2, 9, 10}, 16);
          unsigned args[] = {
              2, 3, sized ? 6 : 0, sized ? 5 : 0, duplicates[di], mode, flags,
              7, 0};
          invoke(0xc569, args, "SOURCE.BMP");
          require(!errors && lookup_calls == 1 && display_calls == 1 &&
                      rop == operations[mode] &&
                      transparent == (flags & 8 ? 7 : -1) &&
                      render_dither == !!(flags & 4) &&
                      render_palette == (flags & 2 ? 1u : 2u) &&
                      render_stretch == sized,
                  "all flags preserve independent initial "
                  "ROP/key/palette/dither/size requests");
          require(!memcmp(render_geometry,
                          (int[]){3, 5, sized ? 7 : 0, sized ? 7 : 0}, 16),
                  "flag sixteen does not alter initial converted destination "
                  "geometry");
          unsigned dup = flags & 1;
          require(clipboard_calls == (dup && !duplicates[di]) &&
                      duplicate_calls ==
                          (dup && duplicates[di] != 0 && duplicates[di] != 2 &&
                           duplicates[di] < 36) &&
                      copy_calls == duplicate_calls,
                  "duplication gates are independent of wallpaper and source "
                  "equality");
          require(
              wallpaper_calls == !!(flags & 16),
              "bit sixteen requests wallpaper, bit thirty-two alone does not");
          if (wallpaper_calls)
            require(wallpaper_mode == (flags & 32) && wallpaper_current == 2 &&
                        !memcmp(wallpaper_source, returned_rect, 16) &&
                        !memcmp(wallpaper_bounds, (int[]){1, 2, 9, 10}, 16) &&
                        wallpaper_sequence > draw_sequence &&
                        wallpaper_sequence > duplicate_sequence,
                    "wallpaper takes final rendered source/current viewport "
                    "after duplication");
        }
  for (unsigned flags = 64; flags < 4096; ++flags) {
    prepare();
    unsigned args[] = {0, 0, 0, 0, 0, 0, flags, 0, 0};
    invoke(0xc569, args, "SOURCE.BMP");
    require(errors == 1 && !lookup_calls && !display_calls &&
                !wallpaper_calls && !clipboard_calls && !duplicate_calls,
            "unknown high flag bits reject before "
            "lookup/draw/duplication/wallpaper");
  }
  prepare();
  fail_delete = 1;
  put16(ports + 8, 2);
  entries[7 * 0x78] = 4;
  unsigned args[] = {0, 0, 6, 5, 7, 0, 17, 0, 0};
  invoke(0xc569, args, "SOURCE.BMP");
  require(display_calls == 1 && !duplicate_calls && !wallpaper_calls,
          "failed explicit duplicate deletion returns before wallpaper");
  prepare();
  fail_create = 1;
  put16(ports + 8, 2);
  invoke(0xc569, args, "SOURCE.BMP");
  require(duplicate_calls == 1 && !copy_calls && wallpaper_calls == 1,
          "failed explicit duplicate creation still reaches wallpaper");
  prepare();
  retarget_clipboard = 1;
  put16(ports + 8, 2);
  memcpy(entries + 0x3a, (int[]){0, 0, 12, 10}, 16);
  args[4] = 0;
  invoke(0xc569, args, "SOURCE.BMP");
  require(clipboard_calls == 1 && wallpaper_calls == 1 &&
              wallpaper_current == 0 &&
              !memcmp(wallpaper_bounds, (int[]){0, 0, 12, 10}, 16),
          "wallpaper re-reads current viewport if clipboard helper changes "
          "current");
  prepare();
  render_result = 1;
  args[6] = 16;
  invoke(0xc569, args, "SOURCE.BMP");
  require(display_calls == 1 && !wallpaper_calls && !clipboard_calls,
          "renderer failure returns before duplication and wallpaper");
  memcpy(image + 0x3ea2e, wallpaper_entry, 5);
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
  memcpy(wallpaper_entry, image + 0x3ea2e, 5);
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
  flag_checks();
  color_key_checks();
  classify_checks();
  mask_and_merge_checks();
  renderer_checks();
  wallpaper_checks();
  rendered_wallpaper_checks();
  printf("PASS: %u bitmap routing/renderer/mask/merge checks\n", checks);
  return 0;
}
