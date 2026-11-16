/* Original byte-parser polygon count/width observations. Controlled framing
 * policy and drawing services; no GUI or Windows rasterization. */
#include "riptel-probe-image.h"
#include <ctype.h>
static unsigned char parser[256], context[512], environment[128],
    environment_entry[16], options[0x108];
static char strings[2048], wire[200000];
static uint32_t arguments[8196], captured[8196];
static uint16_t character_types[256];
static unsigned calls, slot, count, position;
static const char alphabet[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz#&";
static unsigned zero(void) { return 0; }
static void capture(void *ctx, unsigned s, unsigned n, uint32_t *args,
                    const char *tail) {
  if (s == 40 || s == 49) {
    ((void (*)(void *, unsigned, uint32_t *, const char *))(
        image + (s == 40 ? 0x1f32e : 0x1f39f)))(ctx, n, args, tail);
    return;
  }
  require(n <= 8193, "original parser argument capture bound");
  ++calls;
  slot = s;
  count = n;
  memcpy(captured, args, n * sizeof *args);
}
static void parse(unsigned radix, unsigned width) {
  memset(parser, 0, sizeof parser);
  memset(arguments, 0, sizeof arguments);
  memset(captured, 0, sizeof captured);
  pointer(parser + 0xa1, strings);
  pointer(parser + 0xa5, arguments);
  ((void (*)(void *))(image + 0x39d20))(parser);
  memset(context, 0, sizeof context);
  memset(environment, 0, sizeof environment);
  memset(environment_entry, 0, sizeof environment_entry);
  memset(options, 0, sizeof options);
  pointer(context + 2, parser);
  pointer(context + 0xe, environment);
  pointer(environment, environment_entry);
  pointer(context + 0x36, options);
  unsigned base_args[] = {radix}, width_args[] = {width, 0};
  ((void (*)(void *, unsigned, void *, void *))(image + 0x1f32e))(
      context, 1, base_args, NULL);
  ((void (*)(void *, unsigned, void *, void *))(image + 0x1f39f))(
      context, 2, width_args, NULL);
  require(parser[0x38] == radix && parser[0x39] == width,
          "original J/n handlers establish tested radix and coordinate width");
  parser[0x9f] = parser[0xa0] = 13;
  pointer(parser + 0xaa, (void *)capture);
  pointer(parser + 0xb2, context);
  calls = count = slot = 0;
  wire[position++] = '\r';
  wire[position++] = '\n';
  wire[position] = 0;
  for (unsigned i = 0; i < position; ++i)
    ((void (*)(void *, unsigned))(image + 0x39e90))(parser,
                                                    (unsigned char)wire[i]);
}
static void start(char command) {
  wire[0] = '!';
  wire[1] = '|';
  wire[2] = command;
  position = 3;
}
static void number(unsigned value, unsigned width, unsigned radix) {
  if (position + width + 3 >= sizeof wire)
    require(0, "wire fixture bound");
  for (unsigned i = 0; i < width; ++i) {
    wire[position + width - i - 1] = alphabet[value % radix];
    value /= radix;
  }
  if (value)
    require(0, "fixture value fits declared field width");
  position += width;
}
static void configured_start(char command, unsigned radix, unsigned width) {
  start('J');
  number(radix, 2, 36);
  wire[position++] = '|';
  wire[position++] = 'n';
  number(width, 1, radix);
  number(0, 3, radix);
  wire[position++] = '|';
  wire[position++] = command;
}
static void points(unsigned n, unsigned width, unsigned radix) {
  for (unsigned i = 0; i < n; ++i) {
    number(1, width, radix);
    number(2, width, radix);
  }
}
static unsigned char graphics[32], style[0x61], world[128], ports[128],
    entries[0x78];
static unsigned drawing_calls, expected_contours, expected_vertices;
static unsigned expected_counts[2048];
static const char *error_message;
static unsigned one(void) { return 1; }
static void report(void *ctx, unsigned level, const char *where,
                   const char *message) {
  (void)ctx;
  (void)level;
  (void)where;
  error_message = message;
}
static unsigned __attribute__((stdcall)) select_object(unsigned dc,
                                                       unsigned object) {
  (void)dc;
  (void)object;
  return 1;
}
static unsigned __attribute__((stdcall)) background_mode(unsigned dc,
                                                         unsigned mode) {
  (void)dc;
  (void)mode;
  return 2;
}
static void verify_points(const int *p, unsigned n) {
  for (unsigned i = 0; i < n; ++i)
    require(p[2 * i] == 6 && p[2 * i + 1] == 5,
            "original conversion preserves every supplied point plus viewport "
            "offset");
}
static unsigned __attribute__((stdcall)) draw(unsigned dc, const int *p,
                                              unsigned n) {
  require(dc == 1 && n == expected_vertices && expected_contours == 1,
          "original Polygon/Polyline receives declared N, without an implicit "
          "vertex");
  ++drawing_calls;
  verify_points(p, n);
  return 1;
}
static unsigned __attribute__((stdcall)) multi_draw(unsigned dc, const int *p,
                                                    const unsigned *counts,
                                                    unsigned contours) {
  require(dc == 1 && contours == expected_contours && contours <= 2048,
          "original PolyPolygon receives declared contour count");
  unsigned total = 0;
  for (unsigned i = 0; i < contours; ++i) {
    require(counts[i] == expected_counts[i],
            "original preserves each contour vertex count");
    total += counts[i];
  }
  require(total == expected_vertices,
          "original accumulates vertices across all contours");
  verify_points(p, total);
  ++drawing_calls;
  return 1;
}
static void consumer(unsigned rva) {
  memset(context, 0, sizeof context);
  memset(graphics, 0, sizeof graphics);
  memset(style, 0, sizeof style);
  memset(world, 0, sizeof world);
  memset(ports, 0, sizeof ports);
  memset(entries, 0, sizeof entries);
  pointer(context + 0xa, graphics);
  pointer(graphics, style);
  pointer(context + 0xe, world);
  put32(world + 0x24, 640);
  put32(world + 0x28, 480);
  put32(world + 0x2c, 640);
  put32(world + 0x30, 480);
  pointer(context + 0x22, ports);
  pointer(ports, entries);
  put32(entries + 0x3a, 5);
  put32(entries + 0x3e, 3);
  put32(context + 0x62, 1);
  drawing_calls = 0;
  error_message = NULL;
  /* Original single-contour loops convert one extra pair beyond supplied N.
   * Readable sentinels expose that defect without an out-of-buffer read. */
  captured[count] = 123;
  captured[count + 1] = 456;
  ((void (*)(void *, unsigned, uint32_t *, const char *))(image + rva))(
      context, count, captured, NULL);
  require(!error_message && drawing_calls == 1,
          "well-formed parsed polygon reaches original drawing consumer");
  if (rva != 0x1e80a) {
    require(u32(image + 0x8b610 + 8 * expected_vertices) == 128 &&
                u32(image + 0x8b614 + 8 * expected_vertices) == 459,
            "original converts a stale extra pair but does not pass it to GDI");
  }
}
static void multi_case(unsigned radix, unsigned width, unsigned contours) {
  start('<');
  number(contours, 2, radix);
  expected_contours = contours;
  expected_vertices = 0;
  for (unsigned i = 0; i < contours; ++i) {
    number(expected_counts[i], i ? width : 2, radix);
    points(expected_counts[i], width, radix);
    expected_vertices += expected_counts[i];
  }
  parse(radix, width);
  require(calls == 1 && slot == 13 &&
              count == 1 + contours + 2 * expected_vertices,
          "aggregate multi-contour command parses without loss");
  consumer(0x1e80a);
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  hook(0x13c49, (void (*)(void))zero);
  hook(0x13c59, (void (*)(void))zero);
  pointer(image + 0x87908, character_types);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isspace(i) ? 8 : 0) | (isdigit(i) ? 4 : 0) |
                         (isupper(i) ? 1 : 0) | (islower(i) ? 2 : 0);
  unsigned hooks[] = {0x45038, 0x18cf6, 0x18bf7, 0x3988f, 0x398ee,
                      0x13c69, 0x343e4, 0x342ad, 0x34319, 0xe6e9};
  for (unsigned i = 0; i < sizeof hooks / sizeof hooks[0]; ++i)
    hook(hooks[i], noop);
  unsigned selections[] = {0x13cfc, 0x13ce5, 0x13d10, 0x13d27};
  for (unsigned i = 0; i < sizeof selections / sizeof selections[0]; ++i)
    hook(selections[i], (void (*)(void))one);
  hook(0x3445b, (void (*)(void))zero);
  hook(0x3d9e1, (void (*)(void))zero);
  hook(0x38e47, (void (*)(void))report);
  pointer(image + 0x963f0, (void *)background_mode);
  pointer(image + 0x963c0, (void *)select_object);
  pointer(image + 0x9641c, (void *)draw);
  pointer(image + 0x9647c, (void *)draw);
  pointer(image + 0x96470, (void *)multi_draw);
  const unsigned handlers[] = {0x1ed28, 0x1eb03, 0x1bc78};
  const char commands[] = {'l', 'P', 'p'};
  const unsigned slots[] = {45, 52, 53};
  for (unsigned radix = 36; radix <= 64; radix += 28)
    for (unsigned width = 2; width <= 5; ++width) {
      unsigned maximum = radix * radix - 1;
      for (unsigned c = 0; c < 3; ++c) {
        configured_start(commands[c], radix, width);
        number(maximum, 2, radix);
        points(maximum, width, radix);
        parse(radix, width);
        require(calls == 1 && slot == slots[c] && count == 1 + 2 * maximum &&
                    captured[0] == maximum && captured[1] == 1 &&
                    captured[count - 1] == 2,
                "original two-digit maximum count and repeated XY width");
        for (unsigned j = 1; j < count; ++j)
          require(
              captured[j] == (j % 2 ? 1u : 2u),
              "original decodes every XY numeral in the maximum-size command");
        expected_contours = 1;
        expected_vertices = maximum;
        consumer(handlers[c]);
      }
      start('<');
      number(2, 2, radix);
      number(3, 2, radix);
      points(3, width, radix);
      number(2, width, radix);
      points(2, width, radix);
      parse(radix, width);
      require(calls == 1 && slot == 13 && count == 13 &&
                  !memcmp(captured,
                          (unsigned[]){2, 3, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2},
                          13 * sizeof(unsigned)),
              "multi-contour first two numerals fixed two, later counts and "
              "coordinates XY");
      expected_counts[0] = 3;
      expected_counts[1] = 2;
      expected_contours = 2;
      expected_vertices = 5;
      consumer(0x1e80a);
    }
  strcpy(wire, "!|J10|n3000|<0203001002001002001002002001002001002");
  position = strlen(wire);
  parse(36, 2);
  require(calls == 1 && slot == 13 && count == 13 &&
              !memcmp(captured,
                      (unsigned[]){2, 3, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2},
                      13 * sizeof(unsigned)),
          "exact reference stream changes XY width and parses later count as "
          "three digits");
  expected_counts[0] = 3;
  expected_counts[1] = 2;
  expected_contours = 2;
  expected_vertices = 5;
  consumer(0x1e80a);
  /* 8193 numeric arguments are accepted; subsequent fields are discarded.
   * These intentionally inconsistent counts are parser-capture-only cases. */
  for (unsigned n = 8191; n <= 8194; ++n) {
    start('P');
    for (unsigned i = 0; i < n; ++i)
      number(1, 2, 64);
    parse(64, 2);
    require(
        calls == 1 && slot == 52 && count == (n <= 8193 ? n : 8193),
        "numeric collection cap accepts 8193, silently discards excess fields");
  }
  /* Aggregate maxima at coordinate width two. */
  expected_counts[0] = 4093;
  expected_counts[1] = 2;
  multi_case(64, 2, 2);
  expected_counts[0] = expected_counts[1] = expected_counts[2] = 1295;
  expected_counts[3] = 209;
  multi_case(36, 2, 4);
  /* With width three, a later MegaNum contour count can exceed 1295. */
  expected_counts[0] = 2;
  expected_counts[1] = 4093;
  multi_case(36, 3, 2);
  /* Every contour needs at least five numeric fields: one count + two pairs. */
  for (unsigned radix = 36; radix <= 64; radix += 28) {
    unsigned maximum = radix == 36 ? 1295 : 1638;
    for (unsigned i = 0; i < maximum; ++i)
      expected_counts[i] = 2;
    multi_case(radix, 2, maximum);
    if (radix == 64) {
      start('<');
      number(1639, 2, radix);
      for (unsigned i = 0; i < 1639; ++i) {
        number(2, 2, radix);
        points(2, 2, radix);
      }
      parse(radix, 2);
      require(calls == 1 && count == 8193 && captured[0] == 1639,
              "1639 minimum-size contours exceed numeric budget; do not call "
              "unsafe consumer");
    }
  }
  printf("PASS: %u polygon parser checks\n", checks);
  return 0;
}
