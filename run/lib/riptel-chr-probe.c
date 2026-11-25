/* Execute original classic CHR loader and glyph consumer. GDI records calls;
 * no Windows surface, GUI, peer font renderer or on-disk font modification. */
#include "riptel-probe-image.h"
#include <stdarg.h>
#include <strings.h>
static unsigned char ctx[512], state[128], settings[0x300], world[128];
static unsigned char original[65536], loaded[65536];
static unsigned font_length, font_number, opens, reads, closes, invalidations;
static const char *names[] = {"trip.chr", "litt.chr", "sans.chr", "goth.chr",
                              "scri.chr", "simp.chr", "tscr.chr", "lcom.chr",
                              "euro.chr", "bold.chr"};
struct event {
  unsigned kind;
  int a, b, c, d;
};
struct capture {
  unsigned count;
  struct event events[4096];
  unsigned char state[128];
};
static struct capture actual, baseline, removed;
static unsigned selected_pen, next_pen;
static void record(unsigned kind, int a, int b, int c, int d) {
  require(actual.count < 4096, "bounded original glyph drawing trace");
  actual.events[actual.count++] = (struct event){kind, a, b, c, d};
}
static unsigned color(void *c, unsigned index) {
  require(c == ctx, "color lookup context");
  return index & 0xffff;
}
static unsigned file_open(const char *path, unsigned flags) {
  const char *base = strrchr(path, '\\');
  require(base && !strcasecmp(base + 1, names[font_number - 1]) &&
              flags == 0x8000,
          "original classic loader selects requested CHR filename");
  ++opens;
  return 3;
}
static unsigned file_length(unsigned fd) {
  require(fd == 3, "fixture file descriptor");
  return font_length;
}
static unsigned file_read(unsigned fd, void *dest, unsigned count) {
  require(fd == 3 && dest == loaded && count == font_length,
          "original complete font read");
  memcpy(dest, original, count);
  ++reads;
  return count;
}
static unsigned file_close(unsigned fd) {
  require(fd == 3, "original closes font");
  ++closes;
  return 0;
}
static void formatted(char *dest, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsprintf(dest, fmt, args);
  va_end(args);
}
static unsigned __attribute__((stdcall))
message(unsigned hwnd, const char *text, const char *title, unsigned flags) {
  (void)hwnd;
  (void)flags;
  fprintf(stderr, "Unexpected original message: %s: %s\n", title, text);
  require(0, "original client would show a dialog");
  return 0;
}
static unsigned __attribute__((stdcall))
create_pen(unsigned style, unsigned width, unsigned rgb) {
  (void)rgb;
  require(style == 0 && width == 1, "classic glyph uses solid one-pixel pen");
  return next_pen++;
}
static unsigned __attribute__((stdcall)) select_object(unsigned dc,
                                                       unsigned object) {
  require(dc == 1, "controlled glyph DC");
  unsigned old = selected_pen;
  selected_pen = object;
  return old;
}
static unsigned __attribute__((stdcall)) delete_object(unsigned object) {
  require(object == 10 || object == 11, "original releases both glyph pens");
  return 1;
}
static unsigned __attribute__((stdcall)) move(unsigned dc, int x, int y,
                                              void *previous) {
  require(dc == 1 && !previous, "original MoveToEx arguments");
  record(1, x, y, 0, 0);
  return 1;
}
static unsigned __attribute__((stdcall)) line(unsigned dc, int x, int y) {
  require(dc == 1, "original LineTo DC");
  record(2, x, y, 0, 0);
  return 1;
}
static unsigned __attribute__((stdcall)) pixel(unsigned dc, int x, int y,
                                               unsigned rgb) {
  require(dc == 1, "original SetPixel DC");
  record(3, x, y, rgb, 0);
  return rgb;
}
static void invalidate(void *c, unsigned dc, const int *r) {
  require(c == ctx && dc == 1, "original invalidation context");
  ++invalidations;
  record(4, r[0], r[1], r[2], r[3]);
}
static unsigned __attribute__((stdcall))
unexpected_polygon(unsigned dc, const void *p, unsigned n) {
  (void)dc;
  (void)p;
  (void)n;
  require(0, "no classic glyph polygon/fill operation");
  return 0;
}
static unsigned __attribute__((stdcall))
unexpected_multi_polygon(unsigned dc, const void *points,
                         const unsigned *counts, unsigned n) {
  (void)dc;
  (void)points;
  (void)counts;
  (void)n;
  require(0, "no classic glyph multi-contour fill");
  return 0;
}
static void load_font(const char *directory, unsigned number) {
  char path[4096];
  snprintf(path, sizeof path, "%s/%s", directory, names[number - 1]);
  FILE *f = fopen(path, "rb");
  if (!f)
    fail("open original CHR");
  require(!fseek(f, 0, SEEK_END), "seek font");
  long size = ftell(f);
  rewind(f);
  require(size > 0x100 && size < (long)sizeof original - 4,
          "font length bound");
  font_length = size;
  require(fread(original, 1, font_length, f) == font_length,
          "read original font bytes");
  fclose(f);
  font_number = number;
  require(original[0x80] == '+' && u16(original + 0x81) <= 255 &&
              original[0x87] == 0,
          "original stroke-font shape and zero scan flag");
}
static void prepare(unsigned orientation, unsigned size, unsigned conversion,
                    unsigned mode) {
  memset(ctx, 0, sizeof ctx);
  memset(state, 0, sizeof state);
  memset(settings, 0, sizeof settings);
  memset(world, 0, sizeof world);
  memset(loaded, 0, sizeof loaded);
  memset(&actual, 0, sizeof actual);
  pointer(ctx + 0x52, state);
  pointer(ctx + 0x72, settings);
  strcpy((char *)settings + 0x16, "fixture");
  pointer(ctx + 0xe, world);
  put32(ctx + 0x62, 1);
  put32(world + 0x24, conversion ? 1280 : 640);
  put32(world + 0x28, conversion ? 960 : 480);
  put32(world + 0x2c, 640);
  put32(world + 0x30, 480);
  pointer(state + 0x2c, loaded);
  put16(state + 0xa, 0xffff);
  opens = reads = closes = invalidations = 0;
  selected_pen = 7;
  next_pen = 10;
  ((void (*)(void *, unsigned, unsigned, unsigned))(image + 0x10f0))(
      ctx, font_number, orientation, size);
  require(opens == 1 && reads == 1 && closes == 1 && u16(state + 0x26) == 0 &&
              u16(state + 8) == font_number &&
              !memcmp(loaded, original, font_length),
          "original classic loader installs unchanged font and scaling");
  put16(state, mode);
  put16(state + 2, 7);
  put16(state + 0x28, 3);
  put16(state + 4, 3);
  put16(state + 6, 5);
  put16(state + 0x14, 100);
  put16(state + 0x16, 200);
}
static unsigned glyph_start(unsigned code) {
  unsigned first = loaded[0x84], n = u16(loaded + 0x81);
  require(code >= first && code < first + n, "fixture valid glyph code");
  return 0x80 + u16(loaded + 0x85) + u16(loaded + 0x90 + 2 * (code - first));
}
static unsigned strip_scans(unsigned code) {
  unsigned start = glyph_start(code), out = start, removed_count = 0;
  for (unsigned p = start;; p += 2) {
    require(p + 4 <= font_length + 4,
            "original glyph record with readable lookahead");
    unsigned opcode = (loaded[p] >> 7) * 2 + (loaded[p + 1] >> 7);
    if (opcode != 1) {
      loaded[out++] = loaded[p];
      loaded[out++] = loaded[p + 1];
    } else
      ++removed_count;
    if (!opcode)
      break;
  }
  return removed_count;
}
static void draw(unsigned code, unsigned conversion) {
  ((void (*)(void *, unsigned, unsigned, unsigned))(image + 0x18c0))(
      ctx, code, conversion, 0);
  require(selected_pen == 7 && next_pen == 12 && invalidations == 1 &&
              !u16(state + 0x26),
          "original glyph restores pen, releases objects and invalidates once");
  memcpy(actual.state, state, sizeof state);
}
static void same_capture(const struct capture *expected, const char *why) {
  require(actual.count == expected->count &&
              !memcmp(actual.events, expected->events,
                      actual.count * sizeof(struct event)) &&
              !memcmp(actual.state, expected->state, sizeof state),
          why);
}
static void corpus_checks(void) {
  unsigned total_scans = 0;
  for (unsigned g = 0; g < u16(original + 0x81); ++g) {
    unsigned code = original[0x84] + g;
    for (unsigned orientation = 0; orientation < 2; ++orientation)
      for (unsigned conversion = 0; conversion < 2; ++conversion)
        for (unsigned mode = 0; mode < 2; ++mode) {
          prepare(orientation, 4, conversion, mode);
          draw(code, conversion);
          baseline = actual;
          prepare(orientation, 4, conversion, mode);
          unsigned scans = strip_scans(code);
          draw(code, conversion);
          same_capture(&baseline, "all original glyph drawing/state unchanged "
                                  "when SCAN records removed");
          if (!orientation && !conversion && !mode)
            total_scans += scans;
        }
  }
  require(
      total_scans == (font_number == 10 ? 6u : 0u),
      "complete original font census contains exactly six BOLD scan records");
}
static void scan_flag_checks(void) {
  const unsigned affected[] = {0x22, 0x25, 0x3a, 0x3b, 0x3d};
  for (unsigned orientation = 0; orientation < 2; ++orientation)
    for (unsigned conversion = 0; conversion < 2; ++conversion)
      for (unsigned mode = 0; mode < 2; ++mode)
        for (unsigned size = 1; size <= 10; size = size == 1 ? 4 : 10) {
          for (unsigned g = 0; g < 5; ++g) {
            prepare(orientation, size, conversion, mode);
            draw(affected[g], conversion);
            baseline = actual;
            for (unsigned flag = 0; flag < 256; ++flag) {
              original[0x87] =
                  flag; /* Owned test copy, before original loading. */
              prepare(orientation, size, conversion, mode);
              draw(affected[g], conversion);
              same_capture(&baseline, "all scan_flag byte values preserve "
                                      "affected glyph drawing/state");
            }
            original[0x87] = 0;
          }
          if (size == 10)
            break;
        }
}
static void payload_checks(void) {
  /* A SCAN directly between two DRAWs makes any unintended pen movement
   * observable; the shipped instances happen to be followed by MOVE. */
  const unsigned char program[] = {0x83, 4,    0x85, 0x86, 0, 0x80,
                                   0x89, 0x8a, 0x8b, 12,   0, 0};
  for (unsigned orientation = 0; orientation < 2; ++orientation)
    for (unsigned mode = 0; mode < 2; ++mode) {
      prepare(orientation, 4, 0, mode);
      unsigned start = glyph_start(0x22);
      memcpy(loaded + start, program, sizeof program);
      strip_scans(0x22);
      draw(0x22, 0);
      removed = actual;
      for (unsigned payload = 0; payload < 16384; ++payload) {
        prepare(orientation, 4, 0, mode);
        start = glyph_start(0x22);
        memcpy(loaded + start, program, sizeof program);
        loaded[start + 4] = payload & 127;
        loaded[start + 5] = 0x80 | (payload >> 7);
        draw(0x22, 0);
        same_capture(&removed, "all SCAN coordinate payloads are nonpainting "
                               "and leave pen/state unchanged");
      }
    }
}

static void string_consumer_checks(void) {
  const unsigned affected[] = {0x22, 0x25, 0x3a, 0x3b, 0x3d};
  for (unsigned orientation = 0; orientation < 2; ++orientation)
    for (unsigned conversion = 0; conversion < 2; ++conversion)
      for (unsigned mode = 0; mode < 2; ++mode)
        for (unsigned g = 0; g < 5; ++g) {
          prepare(orientation, 4, conversion, mode);
          draw(affected[g], conversion);
          baseline = actual;
          prepare(orientation, 4, conversion, mode);
          char text[] = {(char)affected[g], 0};
          ((void (*)(void *, unsigned, const char *))(image + 0x1860))(
              ctx, conversion, text);
          require(selected_pen == 7 && invalidations == 1,
                  "original string consumer draws affected glyph");
          memcpy(actual.state, state, sizeof state);
          same_capture(&baseline, "original string wrapper reaches identical "
                                  "classic glyph consumer");
        }
}
static void geometry_checks(void) {
  const unsigned codes[] = {0x22, 0x3a, 0x3d};
  const int points[][22] = {
      {100, 222, 101, 232, 102, 222, 101, 220, 100, 222, 105,
       222, 106, 232, 107, 222, 106, 220, 105, 222, 114, 260},
      {100, 260, 100, 252, 106, 252, 106, 260, 100, 260, 100,
       244, 100, 235, 106, 235, 106, 244, 100, 244, 113, 260},
      {100, 247, 100, 242, 122, 242, 122, 247, 100, 247, 100,
       238, 100, 233, 122, 233, 122, 238, 100, 238, 129, 260}};
  for (unsigned g = 0; g < 3; ++g) {
    prepare(0, 4, 0, 0);
    draw(codes[g], 0);
    unsigned count = 0, lines = 0, pixels = 0;
    for (unsigned i = 0; i < actual.count; ++i) {
      const struct event *e = &actual.events[i];
      if (e->kind == 3)
        ++pixels;
      if (e->kind != 1 && e->kind != 2)
        continue;
      require(
          count < 11 &&
              e->kind == (count == 0 || count == 5 || count == 10 ? 1u : 2u) &&
              e->a == points[g][2 * count] && e->b == points[g][2 * count + 1],
          "quote/colon/equals original explicit contour geometry");
      ++count;
      if (e->kind == 2)
        ++lines;
    }
    require(count == 11 && lines == 8 && pixels == 8,
            "two closed outlines have no scan painting or connector");
  }
}
static void write_svg(const char *path) {
  FILE *f = fopen(path, "w");
  if (!f)
    fail("write glyph geometry SVG");
  fprintf(
      f,
      "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"720\" height=\"360\" "
      "viewBox=\"0 0 720 360\" role=\"img\" aria-labelledby=\"title desc\">\n"
      "  <title id=\"title\">RIPtel BOLD.CHR quote, colon and equals stroke "
      "geometry</title>\n"
      "  <desc id=\"desc\">Original MoveToEx and LineTo vectors at unit scale. "
      "Closed contours remain outlines; SCAN records add no painting or "
      "connections. This is a geometry oracle, not a Windows pixel "
      "rendering.</desc>\n"
      "  <rect width=\"720\" height=\"360\" fill=\"white\"/>\n"
      "  <g font-family=\"monospace\" font-size=\"16\" fill=\"black\">\n"
      "    <text x=\"20\" y=\"24\">BOLD.CHR: original stroke geometry, no "
      "fill</text>\n"
      "    <text x=\"20\" y=\"344\">SCAN adds no line, fill, closure or pen "
      "movement.</text>\n  </g>\n");
  const unsigned codes[] = {0x22, 0x3a, 0x3d};
  for (unsigned g = 0; g < 3; ++g) {
    prepare(0, 4, 0, 0);
    draw(codes[g], 0);
    fprintf(f,
            "  <g transform=\"translate(%u 48)\">\n    <text x=\"0\" y=\"0\" "
            "font-family=\"monospace\" font-size=\"16\">0x%02X</text>\n    <g "
            "transform=\"translate(20 -20) scale(4)\" fill=\"none\" "
            "stroke=\"black\" stroke-width=\"0.4\">\n",
            20 + g * 235, codes[g]);
    int x = 0, y = 0;
    for (unsigned i = 0; i < actual.count; ++i) {
      const struct event *e = &actual.events[i];
      if (e->kind == 2)
        fprintf(f, "      <path d=\"M %d %d L %d %d\"/>\n", x - 100, y - 200,
                e->a - 100, e->b - 200);
      if (e->kind == 1 || e->kind == 2) {
        x = e->a;
        y = e->b;
      }
    }
    fprintf(f, "    </g>\n  </g>\n");
  }
  fputs("</svg>\n", f);
  require(!fclose(f), "write complete geometry SVG");
}
int main(int argc, char **argv) {
  if (argc != 3 && argc != 4)
    return 2;
  load_image(argv[1]);
  hook(0x6b560, (void (*)(void))file_open);
  hook(0x75790, (void (*)(void))file_length);
  hook(0x6b1c0, (void (*)(void))file_read);
  hook(0x6b490, (void (*)(void))file_close);
  hook(0x68440, (void (*)(void))formatted);
  hook(0x3dce0, (void (*)(void))color);
  hook(0x13c69, (void (*)(void))invalidate);
  pointer(image + 0x966f4, (void *)message);
  pointer(image + 0x963d4, (void *)create_pen);
  pointer(image + 0x963c0, (void *)select_object);
  pointer(image + 0x963d0, (void *)delete_object);
  pointer(image + 0x963dc, (void *)move);
  pointer(image + 0x963cc, (void *)line);
  pointer(image + 0x963c8, (void *)pixel);
  pointer(image + 0x9641c, (void *)unexpected_polygon);
  pointer(image + 0x96410, (void *)unexpected_polygon);
  pointer(image + 0x96470, (void *)unexpected_multi_polygon);
  for (unsigned f = 1; f <= 10; ++f) {
    load_font(argv[2], f);
    corpus_checks();
  }
  scan_flag_checks();
  string_consumer_checks();
  geometry_checks();
  payload_checks();
  if (argc == 4)
    write_svg(argv[3]);
  printf("PASS: %u classic CHR loading/scan/consumer checks\n", checks);
  return 0;
}
