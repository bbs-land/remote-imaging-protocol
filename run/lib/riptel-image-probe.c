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
  put16(ctx, 0x9df3);
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
  put32(ports + 0x3a, 640);
  put32(ports + 0x3e, 480);
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

static unsigned lookup_calls, access_calls, placeholders, notices, redraws;
static unsigned lookup_image(void *c, char *name, unsigned mode,
                             unsigned retrieve) {
  require(c == ctx && !strcmp(name, "TEST.JPG") && mode == 0x35 &&
              retrieve <= 1,
          "original image presentation looks up one JPG resource");
  ++lookup_calls;
  return 0;
}
static int access_image(const char *name, unsigned mode) {
  require(!strcmp(name, "TEST.JPG") && mode == 0,
          "controlled existing JPEG access check");
  ++access_calls;
  return 0;
}
static unsigned is_jpeg(const char *name) {
  require(!strcmp(name, "TEST.JPG"),
          "controlled existing JPEG signature classification");
  return 1;
}
static void placeholder(void *c, int a, int b, int d, int e, const char *name) {
  (void)a;
  (void)b;
  (void)d;
  (void)e;
  (void)name;
  require(c == ctx, "captured image placeholder drawing boundary");
  ++placeholders;
}
static void notice(void *c, unsigned id, unsigned where) {
  (void)c;
  (void)id;
  (void)where;
  ++notices;
}
static void redraw(void *c, void *a, void *b, unsigned flags) {
  (void)a;
  (void)b;
  require(c == ctx && ((flags & 0xffff) == 0 || (flags & 0xffff) == 32),
          "captured image area redraw geometry boundary");
  ++redraws;
}
static void presentation_checks(void) {
  hook(0x3f80e, (void (*)(void))lookup_image);
  hook(0x758e0, (void (*)(void))access_image);
  hook(0xf363, (void (*)(void))is_jpeg);
  hook(0x383b0, (void (*)(void))placeholder);
  hook(0x41b8, (void (*)(void))notice);
  hook(0x3ea2e, (void (*)(void))redraw);
  hook(0x654a8, (void (*)(void))strlen);
  hook(0x68910, (void (*)(void))strcpy);
  for (unsigned flags = 0; flags < 64; ++flags)
    for (unsigned entry = 0; entry < 2; ++entry)
      for (unsigned area = 0; area < 3; ++area) {
        prepare();
        memcpy(port(0) + 0x4a, (int[]){10, 20, 110, 120}, 16);
        if (area == 1)
          wire_style(0, 0, 0, 0, flags);
        else {
          if (area == 2)
            memset(port(0) + 0x4a, 0, 16);
          put16(port(0) + 0x5a, flags);
        }
        source_width = 10;
        source_height = 5;
        fills = file_opens = file_closes = file_deletes = decoder_steps =
            table_allocations = scan_copies = 0;
        lookup_calls = access_calls = placeholders = notices = redraws = 0;
        if (entry)
          ((void (*)(void *, const char *))(image + 0x69d8))(ctx, "TEST.JPG");
        else
          ((unsigned (*)(void *, const char *, unsigned))(image + 0x3880e))(
              ctx, "TEST.JPG", 1);
        unsigned ignored = area == 2 && !entry;
        require(!errors && file_deletes == 0 && lookup_calls == !ignored,
                "all presentation paths retain image files for every style "
                "flag combination");
        require(file_opens == !ignored && file_closes == file_opens &&
                    decoder_steps == file_opens,
                "all-zero style falls back while only raw-zero explicit "
                "presentation is ignored");
        require(!placeholders && notices == ignored,
                "raw-zero image sentinel blocks explicit entry only, with "
                "notification");
      }
}

static unsigned char reset_objects[6][1024], reset_style_records[36 * 0x61],
    reset_environment[512];
static unsigned one(void) { return 1; }
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
static unsigned __attribute__((stdcall)) empty_rect(const int *r) {
  return r[0] >= r[2] || r[1] >= r[3];
}
static void reset_fixture(void) {
  prepare();
  memset(reset_objects, 0, sizeof reset_objects);
  memset(reset_style_records, 0, sizeof reset_style_records);
  memset(reset_environment, 0, sizeof reset_environment);
  pointer(ctx + 0x16, reset_objects[0]);
  pointer(ctx + 0x1e, reset_objects[1]);
  pointer(ctx + 0x3a, reset_objects[2]);
  pointer(ctx + 0x46, reset_objects[3]);
  pointer(ctx + 0xa, reset_objects[4]);
  pointer(ctx + 0x12, reset_objects[5]);
  pointer(reset_objects[4], reset_style_records);
  pointer(world, reset_environment);
  put16(ports + 8, 5);
  defined_port(5, 1, 1);
  defined_port(6, 1, 1);
  defined_port(7, 1, 0);
  for (unsigned n = 0; n < 8; ++n) {
    memcpy(port(n) + 0x4a, (int[]){10 + n, 20 + n, 110 + n, 120 + n}, 16);
    put16(port(n) + 0x5a, 63);
  }
}
static void reset_checks(void) {
  const unsigned controls[] = {
      0x16b51, 0x3c37e, 0x3988f, 0x398ee, 0x27f8f, 0x3e41a, 0x3c1cf, 0x13c69,
      0x35320, 0x391fd, 0x394fe, 0x11210, 0x3dce0, 0x10f0,  0x13d0,  0xe0dc,
      0x3170d, 0x8577,  0x3d7d1, 0x3468a, 0x16b97, 0x3e8eb, 0x3e943, 0x3e989,
      0x3e9d4, 0x31052, 0x3dc72, 0x3e126, 0x3e254, 0x18e33, 0x37e41, 0x23f20,
      0x24660, 0x454c4, 0x4576f, 0x390e4, 0x2753a, 0x2fcf6, 0x3d8d0, 0x440f0,
      0x441ed, 0x442c3, 0x3e2e7, 0x4558d, 0x39ac8, 0x3dac0, 0x32c1f};
  for (unsigned n = 0; n < sizeof controls / sizeof controls[0]; ++n)
    hook(controls[n], (void (*)(void))one);
  pointer(image + 0x96418, (void *)one_arg);
  pointer(image + 0x9667c, (void *)three_args);
  pointer(image + 0x963d0, (void *)one_arg);
  pointer(image + 0x963d4, (void *)three_args);
  const unsigned two_slots[] = {0x963c0, 0x963f0, 0x96400, 0x963f8};
  for (unsigned n = 0; n < 4; ++n)
    pointer(image + two_slots[n], (void *)two_args);
  pointer(image + 0x96690, (void *)empty_rect);
  for (unsigned offscreen = 0; offscreen < 2; ++offscreen)
    for (unsigned keep = 0; keep < 2; ++keep) {
      reset_fixture();
      port(5)[0] = offscreen ? 4 : 1;
      ((void (*)(void *, unsigned, unsigned, unsigned))(image + 0x16220))(
          ctx, keep, 0, 0);
      require(!errors,
              "original reset completes with unrelated subsystems controlled");
      rect_is(5, -1, -1, -1, -1, 0,
              "reset clears protected old-current image style before selection "
              "changes");
      rect_is(0, -1, -1, -1, -1, 0,
              "reset initializes port-zero image style independently");
      rect_is(6, 16, 26, 116, 126, 63,
              "reset preserves other protected surviving port image style");
      require(
          port(5)[0] == (offscreen ? 4 : 1) && port(6)[0] == 1 &&
              port(7)[0] == (keep ? 1 : 0),
          "original port deletion skips protected ports and obeys keep-ports "
          "route");
      require(u16(ports + 8) == (keep ? 5 : 0),
              "original reset current-port selection matches keep-ports route");
    }
  for (unsigned current = 0; current < 2; ++current)
    for (unsigned header = 0; header < 2; ++header)
      for (unsigned reset = 0; reset < (header ? 3u : 1u); ++reset) {
        reset_fixture();
        put16(ports + 8, current ? 5 : 0);
        if (header) {
          unsigned args[] = {1, reset, 0};
          ((void (*)(void *, unsigned, const void *, const char *))(
              image + 0x1cae1))(ctx, 3, args, NULL);
        } else {
          ((void (*)(void *, unsigned, const void *, const char *))(
              image + 0x1f2f9))(ctx, 0, NULL, NULL);
        }
        require(!errors, "original RESET_WINDOWS/header reset entry completes");
        if (header && !reset) {
          rect_is(0, 10, 20, 110, 120, 63,
                  "header without reset preserves zero-port image style");
          rect_is(
              5, 15, 25, 115, 125, 63,
              "header without reset preserves protected target image style");
        } else {
          rect_is(0, -1, -1, -1, -1, 0,
                  "reset/header default image area uses all-minus-one marker "
                  "and zero flags");
          require(u16(ports + 8) == 0,
                  "reset/header selects port zero after reset");
          if (header && reset == 1) {
            require(!port(5)[0] && !port(6)[0] && !port(7)[0],
                    "hard header reset removes nonzero protected ports");
          } else {
            if (current)
              rect_is(
                  5, -1, -1, -1, -1, 0,
                  "drawing reset changes protected prior-current image style");
            else
              rect_is(
                  5, 15, 25, 115, 125, 63,
                  "drawing reset preserves protected noncurrent image style");
            rect_is(6, 16, 26, 116, 126, 63,
                    "drawing reset leaves other protected survivor image "
                    "flags/area intact");
          }
        }
      }
}

static unsigned temp_appends, temp_reads, temp_closes, temp_unlinks;
static unsigned char temp_file[32];
static char temp_path[64];
static const char *temporary_path(void *c, const char *name) {
  require(c == ctx && !strcmp(name, "RIPSC007.TFL"),
          "original temp list uses instance identifier");
  strcpy(temp_path, name);
  return temp_path;
}
static void *open_temp(const char *name, const char *mode) {
  require(!strcmp(name, "RIPSC007.TFL"), "only memory temp-list file opens");
  require(!strcmp(mode, "a+t") || !strcmp(mode, "r"),
          "temp-list append/read modes");
  memset(temp_file, 0, sizeof temp_file);
  return temp_file;
}
static int append_temp(void *file, const char *fmt, const char *name) {
  require(file == temp_file && fmt && !strcmp(name, "TEST.JPG"),
          "original registration writes controlled JPEG name");
  ++temp_appends;
  return 0;
}
static int read_temp(void *file, const char *fmt, char *name) {
  require(file == temp_file && fmt && !temp_reads,
          "scripted single temp-list filename");
  strcpy(name, "TEST.JPG");
  temp_file[0xc] = 0x10;
  ++temp_reads;
  return 1;
}
static int close_temp(void *file) {
  require(file == temp_file, "temp-list closes only memory file");
  ++temp_closes;
  return 0;
}
static int unlink_temp(const char *name) {
  require(!strcmp(name, temp_unlinks ? "RIPSC007.TFL" : "TEST.JPG"),
          "purge deletes registered filename then list");
  ++temp_unlinks;
  return 0;
}
static void reception_checks(void) {
  for (unsigned type = 0; type < 1296; ++type) {
    prepare();
    unsigned args[] = {type, 0};
    unsigned char before[sizeof entries];
    memcpy(before, entries, sizeof before);
    ((void (*)(void *, unsigned, const void *, const char *))(image + 0x252c0))(
        ctx, 2, args, NULL);
    require(
        errors == (type > 1),
        "encoded-stream stub accepts only types zero/one, otherwise reports");
    require(!memcmp(before, entries, sizeof before),
            "encoded-stream stub leaves all image port state unchanged");
  }
  hook(0x3f66, (void (*)(void))temporary_path);
  hook(0x68f90, (void (*)(void))open_temp);
  hook(0x68ec0, (void (*)(void))close_temp);
  hook(0x6bc50, (void (*)(void))append_temp);
  hook(0x6bc90, (void (*)(void))read_temp);
  hook(0x6b480, (void (*)(void))unlink_temp);
  for (unsigned flags = 0; flags < 64; ++flags)
    for (unsigned zero_area = 0; zero_area < 2; ++zero_area) {
      prepare();
      put16(ctx + 0x7a, 7);
      put16(port(0) + 0x5a, flags);
      if (!zero_area)
        memcpy(port(0) + 0x4a, (int[]){10, 20, 110, 120}, 16);
      temp_appends = temp_reads = temp_closes = temp_unlinks = 0;
      ((void (*)(void *, const char *))(image + 0x765d))(ctx, "TEST.JPG");
      require(temp_appends == 1 && temp_closes == 1 && !temp_unlinks,
              "AddTempFile registers but does not immediately delete JPEG");
      ((void (*)(void *))(image + 0x18efa))(ctx);
      require(temp_reads == 1 && temp_closes == 2 && temp_unlinks == 2,
              "original temp purge is independent of image flags/zero area");
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
  presentation_checks();
  reset_checks();
  reception_checks();
  printf("PASS: %u image-style checks\n", checks);
  return 0;
}
