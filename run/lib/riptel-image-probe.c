/* Original RIPtel image-style state and JPEG orchestration observations.
 * OS/file/decoder controls are recorded with the corresponding research note.
 * No GUI or real image-file mutation. */
#include "riptel-probe-image.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
static unsigned char ctx[512], ports[128], entries[36 * 0x78], world[128],
    vars[0x700], options[0x400], image_state[0x500];
static char result[256], params[40 * 21];
static uint16_t character_types[256];
static unsigned errors;
static unsigned char *port(unsigned n) { return entries + n * 0x78; }
static void report(void *c, unsigned level, const char *where,
                   const char *why) {
  (void)c;
  (void)level;
  (void)where;
  (void)why;
  ++errors;
}
static void formatted(char *dst, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsnprintf(dst, sizeof result, fmt, args);
  va_end(args);
}
static unsigned __attribute__((stdcall)) set_rect(int *r, int x, int y,
                                                  int right, int bottom) {
  memcpy(r, (int[]){x, y, right, bottom}, 16);
  return 1;
}
static void defined_port(unsigned n, unsigned type, unsigned flags) {
  unsigned char *p = port(n);
  p[0] = type;
  p[0x17] = flags;
  put16(p + 1, n);
  put32(p + 0xb, 640);
  put32(p + 0xf, 480);
  put32(p + 0x42, 640);
  put32(p + 0x46, 480);
  put32(p + 0x6c, n + 1);
}
static void prepare(void) {
  memset(ctx, 0, sizeof ctx);
  memset(ports, 0, sizeof ports);
  memset(entries, 0, sizeof entries);
  memset(world, 0, sizeof world);
  memset(vars, 0, sizeof vars);
  memset(options, 0, sizeof options);
  memset(image_state, 0, sizeof image_state);
  memset(params, 0, sizeof params);
  pointer(ctx + 0x22, ports);
  pointer(ports, entries);
  pointer(ctx + 0xe, world);
  pointer(ctx + 0x26, vars);
  pointer(vars + 0x432, result);
  pointer(ctx + 0x36, options);
  pointer(ctx + 0x56, image_state);
  put32(world + 0x24, 1280);
  put32(world + 0x28, 960);
  put32(world + 0x2c, 640);
  put32(world + 0x30, 480);
  defined_port(0, 1, 0);
  put32(ctx + 0x62, 1);
  result[0] = 0;
  errors = 0;
}
static void wire_style(unsigned x0, unsigned y0, unsigned x1, unsigned y1,
                       unsigned flags) {
  unsigned values[] = {x0, y0, x1, y1, flags, 0};
  ((void (*)(void *, unsigned, const void *, const char *))(image + 0xc39a))(
      ctx, 6, values, NULL);
}
static const char *macro(unsigned count, const char *const *values) {
  memset(params, 0, sizeof params);
  for (unsigned n = 0; n < count; ++n) {
    require(strlen(values[n]) <= 20, "bounded macro argument");
    strcpy(params + n * 21, values[n]);
  }
  put16(vars + 0x436, 0);
  return ((const char *(*)(void *, unsigned, const void *))(image + 0x2f1be))(
      ctx, count, params);
}
static void rect_is(unsigned n, int a, int b, int c, int d, unsigned flags,
                    const char *why) {
  require(!memcmp(port(n) + 0x4a, (int[]){a, b, c, d}, 16) &&
              u16(port(n) + 0x5a) == flags,
          why);
}
static void style_checks(void) {
  require(image[0x26c77] == 0x68 && u32(image + 0x26c78) == 0x1002f1be,
          "original registration selects IMGSTYLE processor");
  for (unsigned flags = 0; flags <= 64; ++flags) {
    prepare();
    put16(port(0) + 0x5a, 0xbeef);
    wire_style(20, 40, 220, 240, flags);
    if (flags <= 63) {
      require(!errors, "wire style accepts exactly six flag bits");
      rect_is(0, 10, 20, 111, 121, flags,
              "wire image rectangle uses device conversion and far endpoint "
              "plus one");
    } else {
      require(errors == 1 && u16(port(0) + 0x5a) == 0xbeef,
              "invalid wire flags reject without storing flags");
    }
  }
  prepare();
  wire_style(0, 0, 0, 0, 2);
  rect_is(0, 0, 0, 1, 1, 2,
          "all-zero wire image style stores a one-pixel extent");
  wire_style(220, 240, 20, 40, 3);
  rect_is(0, 10, 20, 111, 121, 3,
          "reversed image-style corners normalize before conversion");
  for (unsigned type = 1; type <= 4; type += 3)
    for (unsigned status = 0; status < 4; ++status) {
      prepare();
      defined_port(5, type, status);
      put16(ports + 8, 5);
      put32(ctx + 0x62, type == 1 ? 1 : 6);
      memcpy(port(5) + 0x4a, (int[]){10, 20, 111, 121}, 16);
      put16(port(5) + 0x5a, 15);
      const char *query[] = {"CUR"};
      const char *value = macro(1, query);
      require(value && !strcmp(value, status & 2 ? "-1" : "20:40:222:242:15"),
              "IMGSTYLE queries defined screen/offscreen port unless viewport "
              "disabled");
      const char *set[] = {"CUR", "20", "40", "220", "240"};
      value = macro(5, set);
      require((status & 2 ? value && !strcmp(value, "-1") : !value) &&
                  !u16(vars + 0x436),
              "IMGSTYLE setter skips disabled viewport with minus one, "
              "otherwise returns no text");
      rect_is(
          5, 10, 20, 111, 121, status ? 15 : 0,
          "five-argument macro clears flags only for writable enabled target");
      require(u16(ports + 8) == 5 && u32(ctx + 0x62) == (type == 1 ? 1u : 6u),
              "macro retains current screen/offscreen target");
    }
  prepare();
  const char *unknown[] = {"5", "20", "40", "220", "240"};
  const char *value = macro(5, unknown);
  require(value && !strcmp(value, "-1") && !u16(vars + 0x436) &&
              port(5)[0] == 0,
          "undefined target yields minus one without defining or selecting it");
  const char *bad[] = {"36", "20", "40", "220", "240"};
  require(!macro(5, bad) && u16(vars + 0x436),
          "invalid selector is a variable error");
  const char *keywords[] = {"ASPECT", "DELETE", "NOCLEAR", "USEPAL"};
  for (unsigned flags = 0; flags < 16; ++flags) {
    prepare();
    defined_port(5, 4, 0);
    put16(port(5) + 0x5a, 63);
    const char *values[9] = {"5", "20", "40", "220", "240"};
    unsigned count = 5;
    for (unsigned n = 0; n < 4; ++n)
      if (flags & (1u << n))
        values[count++] = keywords[n];
    require(!macro(count, values) && !errors && !u16(vars + 0x436),
            "all IMGSTYLE keyword combinations set successfully");
    rect_is(5, 10, 20, 111, 121, flags,
            "macro flags replace previous flags with recognized keyword OR");
    require(u16(ports + 8) == 0 && u32(ctx + 0x62) == 1,
            "temporary target selection restores previous current port/DC");
  }
  prepare();
  const char *numeric[] = {"0", "20", "40", "220", "240", "3"};
  require(!macro(6, numeric) && u16(vars + 0x436),
          "numeric IMGSTYLE flag argument is not a keyword");
}
static unsigned fills, file_opens, file_closes, file_deletes, decoder_steps;
static unsigned table_allocations, scan_copies, source_width, source_height;
static unsigned char source_row[32], destination_row[1024];
static void *source_row_pointer = source_row;
static int fill_rects[8][4];
static unsigned char fake_dib[256], fake_error[256];
static unsigned short map_tables[2][2048];
static unsigned zero(void) { return 0; }
static void dimensions(const char *name, unsigned *w, unsigned *h) {
  (void)name;
  *w = source_width;
  *h = source_height;
}
static void *open_image(const char *name, const char *mode) {
  require(name && mode && !strcmp(name, "TEST.JPG"),
          "JPEG opens only controlled file");
  ++file_opens;
  return fake_dib;
}
static int close_image(void *f) {
  require(f == fake_dib, "JPEG closes controlled file");
  ++file_closes;
  return 0;
}
static void *decoder_error(void *p) {
  (void)p;
  return fake_error;
}
static void *line_dib(unsigned width, unsigned height) {
  require((short)width > 0 && (short)height == 1,
          "bounded temporary JPEG line bitmap");
  return fake_dib;
}
static void *dib_header(void *p) {
  (void)p;
  return fake_dib;
}
static unsigned short *map_allocate(unsigned count) {
  require((short)count > 0 && (unsigned short)count < 2048 &&
              table_allocations < 2,
          "bounded JPEG coordinate map allocation");
  return map_tables[table_allocations++];
}
static unsigned __attribute__((stdcall)) stock(unsigned n) {
  require(n == 4, "original image clear uses BLACK_BRUSH");
  return 1004;
}
static unsigned __attribute__((stdcall)) fill(unsigned dc, const int *r,
                                              unsigned brush) {
  require(dc == 1 && brush == 1004 && fills < 8,
          "captured black FillRect on current drawing DC");
  memcpy(fill_rects[fills++], r, 16);
  return 1;
}
static unsigned __attribute__((stdcall)) offset(int *r, int x, int y) {
  r[0] += x;
  r[2] += x;
  r[1] += y;
  r[3] += y;
  return 1;
}
static unsigned __attribute__((stdcall)) one_arg(unsigned a) {
  (void)a;
  return 1;
}
static unsigned __attribute__((stdcall)) delete_image(const char *name) {
  require(name && *name, "captured nonempty image deletion name");
  ++file_deletes;
  return 1;
}
static unsigned __attribute__((stdcall))
scan_copy(unsigned dc, int x, int y, int w, int h, int sx, int sy, int sw,
          int sh, const void *bits, const void *header, unsigned usage,
          unsigned rop) {
  (void)x;
  (void)y;
  (void)sx;
  (void)sy;
  (void)sw;
  (void)sh;
  (void)usage;
  require(dc == 1 && w > 0 && h == 1 && bits == destination_row &&
              header == fake_dib,
          "original JPEG scanline reaches captured current-DC StretchDIBits");
  require(rop == 0xcc0020, "original JPEG scanline uses explicit SRCCOPY");
  ++scan_copies;
  return 1;
}
static unsigned decoder_step(void *c, unsigned dc) {
  require(c == ctx && dc == 1 && !decoder_steps,
          "one controlled decoded-image preparation phase");
  ++decoder_steps;
  put32(image_state + 0x231, source_width);
  put32(image_state + 0x235, source_height);
  require(((unsigned (*)(void *))(image + 0x29880))(ctx) == 1,
          "original JPEG preparation executes with controlled dimensions");
  put32(image_state + 0x26a, source_width);
  put32(image_state + 0x286, 1);
  pointer(image_state + 0x14a, &source_row_pointer);
  pointer(image_state + 0x13a, destination_row);
  memset(source_row, 0x5a, sizeof source_row);
  ((void (*)(void *, unsigned))(image + 0x29ad0))(ctx, dc);
  return 1;
}
static void jpeg_checks(void) {
  hook(0x57e80, (void (*)(void))dimensions);
  hook(0x68f90, (void (*)(void))open_image);
  hook(0x68ec0, (void (*)(void))close_image);
  hook(0x57fb0, (void (*)(void))decoder_error);
  hook(0x57720, noop);
  hook(0x4aa40, noop);
  hook(0x6c868, (void (*)(void))zero);
  hook(0x57790, noop);
  hook(0x29c30, (void (*)(void))decoder_step);
  hook(0x4e13a, (void (*)(void))line_dib);
  hook(0x4e103, (void (*)(void))dib_header);
  hook(0x15210, (void (*)(void))map_allocate);
  pointer(image + 0x9640c, (void *)stock);
  pointer(image + 0x9667c, (void *)fill);
  pointer(image + 0x96610, (void *)offset);
  pointer(image + 0x963e0, (void *)one_arg);
  pointer(image + 0x965dc, (void *)delete_image);
  pointer(image + 0x96454, (void *)scan_copy);
  for (unsigned tall = 0; tall < 2; ++tall)
    for (unsigned flags = 0; flags < 64; ++flags) {
      prepare();
      source_width = tall ? 5 : 10;
      source_height = tall ? 10 : 5;
      memcpy(port(0) + 0x4a, (int[]){10, 20, 110, 120}, 16);
      put16(port(0) + 0x5a, flags);
      fills = file_opens = file_closes = file_deletes = decoder_steps =
          table_allocations = scan_copies = 0;
      unsigned status =
          ((unsigned (*)(void *, const char *, int, int, int, int))(
              image + 0x37fb5))(ctx, "TEST.JPG", 10, 20, 110, 120) &
          0xffff;
      require(status == 1 && !errors && file_opens == 1 && file_closes == 1 &&
                  decoder_steps == 1,
              "original JPEG orchestration/preparation completes for all style "
              "flags");
      require(file_deletes == 0, "no original local JPEG wrapper/preparation "
                                 "deletion for any flag combination");
      require(fills == !(flags & 4),
              "NOCLEAR suppresses original preparation FillRect");
      require(scan_copies > 0,
              "original scanline consumer paints controlled decoded row");
      if (fills) {
        const int *r = fill_rects[0];
        require(r[0] == ((flags & 1) && tall ? 35 : 10) &&
                    r[2] == ((flags & 1) && tall ? 85 : 110) &&
                    r[1] == ((flags & 1) && !tall ? 45 : 20) &&
                    r[3] == ((flags & 1) && !tall ? 95 : 120),
                "ASPECT clear covers the fitted image only, leaving original "
                "margins outside it");
      }
    }
}

int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  const unsigned controlled[] = {0x45038, 0x18cf6, 0x18bf7, 0x343e4,
                                 0x342ad, 0x34319, 0x13c69, 0xe6e9};
  for (unsigned n = 0; n < sizeof controlled / sizeof controlled[0]; ++n)
    hook(controlled[n], noop);
  hook(0x38e47, (void (*)(void))report);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x655e2, (void (*)(void))strcasecmp);
  hook(0x65604, (void (*)(void))atoi);
  hook(0x68440, (void (*)(void))formatted);
  pointer(image + 0x87908, character_types);
  for (unsigned c = 0; c < 256; ++c)
    character_types[c] = (isdigit(c) ? 4 : 0) | (isspace(c) ? 8 : 0) |
                         (isupper(c) ? 1 : 0) | (islower(c) ? 2 : 0);
  pointer(image + 0x96618, (void *)set_rect);
  style_checks();
  jpeg_checks();
  printf("PASS: %u image-style checks\n", checks);
  return 0;
}
