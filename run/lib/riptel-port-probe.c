/* Original port allocation and copy geometry, with controlled OS services.
 * Captures final rectangles without emulating Windows rasterization. */
#include "riptel-probe-image.h"
#include <ctype.h>
#include <stdarg.h>
#include <strings.h>
static unsigned char ctx[512], ports[128], entries[36 * 0x78], world[128],
    palette[0x900], options[0x400], vars[0x700];
static unsigned errors, allocated_w, allocated_h, blits, stretches,
    quiet_errors;
static int captured[10];
static char result[64];
static unsigned char original_protection[5];
static unsigned zero(void) { return 0; }
static void report(void *c, unsigned level, const char *where,
                   const char *why) {
  (void)c;
  (void)level;
  if (!quiet_errors)
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
static unsigned query_parse(unsigned count, char *params,
                            unsigned char *decoded) {
  return ((unsigned (*)(void *, unsigned, void *, void *))(image + 0x2f743))(
             ctx, count, params, decoded) &
         0xffff;
}
static void size_queries(void) {
  static uint16_t character_types[256];
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = isdigit(i) ? 4 : 0;
  pointer(image + 0x87908, character_types);
  hook(0x65604, (void (*)(void))atoi);
  char params[4 * 21] = {0};
  const unsigned routines[] = {0x2f863, 0x2f642, 0x2f95e,
                               0x2fa45, 0x2fb2a, 0x2fc11};
  const unsigned currents[] = {0, 1, 35};
  const char *types[] = {"PORT", "VIEW", "port", "view"};
  const char *domains[] = {"WORLD", "DEVICE", "world", "device"};
  for (unsigned c = 0; c < 3; ++c)
    for (unsigned selector = 0; selector < 39; ++selector)
      for (unsigned t = 0; t < 4; ++t)
        for (unsigned d = 0; d < 4; ++d)
          for (unsigned count = 0; count <= 3; ++count) {
            put16(ports + 8, currents[c]);
            if (selector < 36)
              snprintf(params, 21, "%u", selector);
            else
              strcpy(params, selector == 36   ? "CUR"
                             : selector == 37 ? "cur"
                                              : "035");
            strcpy(params + 21, types[t]);
            strcpy(params + 42, domains[d]);
            unsigned char decoded[12];
            memset(decoded, 0xa5, sizeof decoded);
            require(query_parse(count, params, decoded) == 1 &&
                        u16(decoded) == (count && selector < 36 ? selector
                                         : count && selector == 38
                                             ? 35
                                             : currents[c]) &&
                        u32(decoded + 2) == (count > 1 ? t % 2 : 0) &&
                        u32(decoded + 6) == (count > 2 ? d % 2 : 0) &&
                        decoded[10] == 0xa5 && decoded[11] == 0xa5,
                    "original port-query decoder accepts exactly positional "
                    "port/type/domain prefixes and case-insensitive keywords");
            for (unsigned n = 0; n < 6; ++n) {
              put16(vars + 0x436, 0);
              const char *r = ((const char *(*)(void *, unsigned, void *))(
                  image + routines[n]))(ctx, count, params);
              require(r == NULL && u16(vars + 0x436) == 1,
                      "all six original port callbacks reject valid parser "
                      "success");
            }
          }
  const struct {
    unsigned count;
    const char *arg[4];
  } invalid[] = {
      {1, {""}},
      {1, {"36"}},
      {1, {"-1"}},
      {1, {"Z"}},
      {1, {"VIEW"}},
      {1, {"WORLD"}},
      {1, {"DEVICE"}},
      {1, {"1x"}},
      {2, {"CUR", "DEVICE"}},
      {2, {"CUR", "WORLD"}},
      {2, {"CUR", ""}},
      {2, {"CUR", "BAD"}},
      {3, {"CUR", "VIEW", "PORT"}},
      {3, {"CUR", "PORT", "VIEW"}},
      {3, {"CUR", "PORT", ""}},
      {3, {"CUR", "PORT", "PIXEL"}},
      {3, {"CUR", "DEVICE", "VIEW"}},
      {4, {"CUR", "PORT", "WORLD", "DEVICE"}},
  };
  for (unsigned n = 0; n < sizeof invalid / sizeof invalid[0]; ++n) {
    memset(params, 0, sizeof params);
    for (unsigned a = 0; a < invalid[n].count; ++a)
      strcpy(params + 21 * a, invalid[n].arg[a]);
    unsigned char decoded[12];
    memset(decoded, 0xa5, sizeof decoded);
    require(query_parse(invalid[n].count, params, decoded) == 0,
            "original decoder rejects shorthand, bad/missing/misordered fields "
            "and extras; do not call inverted callbacks with invalid output");
  }
}

struct clip_surface {
  unsigned alive, w, h, color;
};
static struct clip_surface clip_surfaces[64];
static unsigned clip_maps[64], clip_dc_alive[64], clip_next_dc,
    clip_next_bitmap;
static unsigned clip_failure, clip_allocations, clip_deletes, clip_copies,
    clip_source;
static unsigned __attribute__((stdcall)) clip_dc(unsigned unused) {
  (void)unused;
  ++clip_allocations;
  if (clip_failure == 1)
    return 0;
  unsigned dc = ++clip_next_dc;
  require(dc < 64, "owned clipboard DC capacity");
  clip_dc_alive[dc] = 1;
  clip_maps[dc] = 0;
  return dc;
}
static unsigned __attribute__((stdcall)) clip_bitmap(unsigned unused,
                                                     unsigned w, unsigned h) {
  (void)unused;
  allocated_w = w;
  allocated_h = h;
  if (clip_failure == 2)
    return 0;
  unsigned bitmap = ++clip_next_bitmap;
  require(bitmap < 64, "owned clipboard bitmap capacity");
  clip_surfaces[bitmap] = (struct clip_surface){1, w, h, 0};
  return 100 + bitmap;
}
static unsigned __attribute__((stdcall)) clip_select(unsigned dc,
                                                     unsigned object) {
  require(dc < 64 && clip_dc_alive[dc], "select owned live DC");
  if (clip_failure == 3 && object >= 100 && object < 164)
    return 0;
  unsigned old = clip_maps[dc];
  clip_maps[dc] = object;
  return old ? old : 9000;
}
static unsigned __attribute__((stdcall)) clip_delete(unsigned object) {
  if (object >= 100 && object < 164) {
    require(clip_surfaces[object - 100].alive,
            "delete live original clipboard bitmap");
    clip_surfaces[object - 100].alive = 0;
    ++clip_deletes;
  }
  return 1;
}
static unsigned __attribute__((stdcall)) clip_delete_dc(unsigned dc) {
  require(dc < 64 && clip_dc_alive[dc], "delete owned live DC");
  clip_dc_alive[dc] = 0;
  return 1;
}
static unsigned __attribute__((stdcall)) clip_fill(unsigned dc, const int *r,
                                                   unsigned brush) {
  (void)r;
  (void)brush;
  require(dc < 64 && clip_maps[dc] >= 100 && clip_maps[dc] < 164,
          "fill owned bitmap");
  clip_surfaces[clip_maps[dc] - 100].color = 0;
  return 1;
}
static unsigned __attribute__((stdcall)) clip_copy(unsigned dst, int x, int y,
                                                   int w, int h, unsigned src,
                                                   int sx, int sy,
                                                   unsigned rop) {
  (void)x;
  (void)y;
  (void)w;
  (void)h;
  (void)sx;
  (void)sy;
  require(dst < 64 && src < 64 && clip_maps[dst] >= 100 &&
              clip_maps[src] >= 100 && rop == 0xcc0020,
          "COPY between owned clip surfaces");
  clip_source = src;
  ++clip_copies;
  if (clip_failure == 5)
    return 0;
  clip_surfaces[clip_maps[dst] - 100].color =
      clip_surfaces[clip_maps[src] - 100].color;
  return 1;
}
static unsigned __attribute__((stdcall))
clip_stretch(unsigned dst, int x, int y, int w, int h, unsigned src, int sx,
             int sy, int sw, int sh, unsigned rop) {
  ++stretches;
  memcpy(captured, (int[]){x, y, w, h, sx, sy, sw, sh}, 8 * sizeof(int));
  return clip_copy(dst, x, y, w, h, src, sx, sy, rop);
}
static void clip_prepare(unsigned source, int clipboard) {
  prepare(16, 16, 16, 16);
  memset(clip_surfaces, 0, sizeof clip_surfaces);
  memset(clip_maps, 0, sizeof clip_maps);
  memset(clip_dc_alive, 0, sizeof clip_dc_alive);
  clip_next_dc = clip_next_bitmap = 8;
  for (unsigned slot = 0; slot <= 7; slot += 7) {
    viewport(slot, 16, 16);
    unsigned char *p = entries + slot * 0x78;
    p[0] = slot ? 4 : 1;
    put32(p + 0x6c, slot + 1);
    put32(p + 0x70, 9000);
    put32(p + 0x74, 101 + slot);
    clip_maps[slot + 1] = 101 + slot;
    clip_dc_alive[slot + 1] = 1;
    clip_surfaces[slot + 1] =
        (struct clip_surface){1, 16, 16, slot ? 0x777777 : 0x111111};
  }
  put16(ports + 8, source);
  put16(ports + 0xa, clipboard);
  put32(ctx + 0x62, source + 1);
  put32(ctx + 0x6a, 1);
  clip_failure = clip_allocations = clip_deletes = clip_copies = clip_source =
      0;
}
static void clipboard_checks(void) {
  quiet_errors = 1;
  memcpy(image + 0x33821, original_protection, 5);
  hook(0x65cbf, noop);
  hook(0x68d70, (void (*)(void))memset);
  hook(0x69e60, (void (*)(void))memcpy);
  pointer(image + 0x964a0, (void *)clip_dc);
  pointer(image + 0x96460, (void *)clip_bitmap);
  pointer(image + 0x963c0, (void *)clip_select);
  pointer(image + 0x963d0, (void *)clip_delete);
  pointer(image + 0x964ac, (void *)clip_delete_dc);
  pointer(image + 0x9667c, (void *)clip_fill);
  pointer(image + 0x963e4, (void *)clip_copy);
  pointer(image + 0x96488, (void *)clip_stretch);
  unsigned xy[] = {1, 1, 3, 3, 0};
  for (unsigned source = 0; source <= 7; source += 7)
    for (unsigned assigned = 0; assigned < 2; ++assigned)
      for (unsigned protected = 0; protected < 2; ++protected) {
        clip_prepare(source, assigned ? 7 : -1);
        entries[7 * 0x78 + 0x17] = protected;
        unsigned char old7[0x78];
        memcpy(old7, entries + 7 * 0x78, 0x78);
        invoke(0xc1cf, xy);
        if (assigned && protected) {
          require(
              errors >= 1 && u16(ports + 0xa) == 7 &&
                  u16(ports + 8) == source &&
                  !memcmp(old7, entries + 7 * 0x78, 0x78) &&
                  clip_surfaces[8].alive && !clip_allocations && !clip_copies,
              "protected assigned clipboard rejects before deletion/allocation "
              "and preserves pointer/current/record/pixels");
        } else {
          unsigned target = assigned ? 7 : 1;
          unsigned final_source = assigned && source == 7 ? 0 : source;
          require(!errors && u16(ports + 0xa) == target &&
                      u16(ports + 8) == final_source &&
                      entries[target * 0x78] == 4 && allocated_w == 4 &&
                      allocated_h == 4 && stretches == 1 &&
                      !memcmp(captured, (int[]){0, 0, 4, 4, 1, 1, 3, 3},
                              8 * sizeof(int)) &&
                      clip_copies == 1 && clip_source == final_source + 1,
                  "clipboard absence chooses first unused; assigned target "
                  "reused; same-source deletion changes source to zero");
          unsigned bitmap_handle = u32(entries + target * 0x78 + 0x74);
          require(clip_surfaces[bitmap_handle - 100].color ==
                      (final_source ? 0x777777u : 0x111111u),
                  "new clipboard contains pixels from the actual "
                  "post-allocation source");
          if (!assigned)
            require(!memcmp(old7, entries + 7 * 0x78, 0x78) &&
                        clip_surfaces[8].alive,
                    "existing offscreen port is retained when first-unused "
                    "allocation wins");
        }
      }
  for (unsigned source = 0; source <= 7; source += 7)
    for (unsigned assigned = 0; assigned < 2; ++assigned)
      for (unsigned failure = 1; failure <= 4; ++failure) {
        clip_prepare(source, assigned ? 7 : -1);
        clip_failure = failure;
        if (failure == 4)
          put32(ports + 0x22, (uint32_t)-100000);
        unsigned char before[sizeof entries];
        memcpy(before, entries, sizeof before);
        invoke(0xc1cf, xy);
        unsigned target = assigned ? 7 : 1;
        require(errors >= 1 && u16(ports + 0xa) == 65535 &&
                    entries[target * 0x78] == 0 &&
                    u16(ports + 8) == (assigned && source == 7 ? 0 : source) &&
                    !clip_copies,
                "allocation failure clears clipboard, leaves target undefined "
                "and retains post-delete current selection");
        require(clip_surfaces[1].alive &&
                    (!assigned || !clip_surfaces[8].alive),
                "old assigned clipboard pixels are destroyed before failed "
                "allocation; screen survives");
        for (unsigned slot = 0; slot < 36; ++slot)
          if (slot != target)
            require(!memcmp(before + slot * 0x78, entries + slot * 0x78, 0x78),
                    "allocation failure retains every unrelated port record");
      }
  for (unsigned source = 0; source <= 7; source += 7) {
    clip_prepare(source, -1);
    for (unsigned slot = 1; slot < 36; ++slot)
      if (slot != 7)
        viewport(slot, 16, 16);
    unsigned char before[sizeof entries];
    memcpy(before, entries, sizeof before);
    invoke(0xc1cf, xy);
    require(errors == 1 && u16(ports + 0xa) == 65535 &&
                u16(ports + 8) == source &&
                !memcmp(before, entries, sizeof before) && !clip_allocations &&
                !clip_deletes && !clip_copies && clip_surfaces[8].alive,
            "no free slot fails without deleting existing offscreen ports or "
            "changing current/pixels/pointer");
    /* Existing assignment still wins when no free slot exists. */
    put16(ports + 0xa, 7);
    errors = 0;
    invoke(0xc1cf, xy);
    require(!errors && u16(ports + 0xa) == 7 && clip_copies == 1 &&
                !clip_surfaces[8].alive,
            "assigned clipboard is replaced without searching for a free slot");
  }
  for (unsigned protect = 0; protect < 2; ++protect) {
    clip_prepare(0, 7);
    entries[7 * 0x78 + 0x17] = protect;
    unsigned char old[0x78];
    memcpy(old, entries + 7 * 0x78, 0x78);
    unsigned reset =
        ((unsigned (*)(void *, int, unsigned))(image + 0x33112))(ctx, -2, 0) &
        65535;
    require(reset == 1 && !errors && u16(ports + 8) == 0 &&
                u16(ports + 0xa) == (protect ? 7 : 65535) &&
                clip_surfaces[8].alive == protect &&
                (protect ? !memcmp(old, entries + 7 * 0x78, 0x78)
                         : entries[7 * 0x78] == 0),
            "soft-reset port deletion retains protected clipboard and clears "
            "only deleted assignment");
  }
  for (unsigned copy_failure = 0; copy_failure < 2; ++copy_failure) {
    clip_prepare(0, -1);
    if (copy_failure)
      clip_failure = 5;
    else
      memset(entries + 0x3a, 0, 16);
    int rect[] = {1, 1, 4, 4};
    unsigned status =
        ((unsigned (*)(void *, void *))(image + 0x2866))(ctx, rect) & 65535;
    require(status == 1 && u16(ports + 0xa) == 1 && entries[0x78] == 4 &&
                u16(ports + 8) == 0,
            "copy failure is ignored after successful allocation; clipboard "
            "remains defined without rollback");
    require(clip_copies == copy_failure &&
                clip_surfaces[u32(entries + 0x78 + 0x74) - 100].color == 0,
            "trim or GDI copy failure leaves allocated clipboard clear instead "
            "of preserving old pixels");
  }
}

int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  memcpy(original_protection, image + 0x33821, 5);
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
  clipboard_checks();
  printf("PASS: %u port allocation/query/copy geometry checks\n", checks);
  return 0;
}
