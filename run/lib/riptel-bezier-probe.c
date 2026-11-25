/* Original RIPtel special block parser and Bezier numerical/consumer probes.
 * Host/GDI services are controlled; no GUI or Windows rasterization. */
#include "riptel-probe-image.h"
#include <ctype.h>
static unsigned char parser[256], context[512], environment[128],
    environment_entry[16], options[0x108];
static char strings[2048];
static uint32_t arguments[16384], captured[16384];
static uint16_t character_types[256];
static unsigned calls, slot, count;
static unsigned verbose;
static unsigned zero(void) { return 0; }
static void capture(void *ctx, unsigned s, unsigned n, uint32_t *args,
                    const char *tail) {
  if (s == 40 || s == 49) {
    ((void (*)(void *, unsigned, uint32_t *, const char *))(
        image + (s == 40 ? 0x1f32e : 0x1f39f)))(ctx, n, args, tail);
    return;
  }
  if (s != 76 && s != 77 && s != 61 && s != 71)
    return;
  require(n <= 8193, "original numeric vector bound");
  ++calls;
  slot = s;
  count = n;
  memcpy(captured, args, n * sizeof *args);
  if (verbose) {
    printf("dispatch slot=%u count=%u:", s, n);
    for (unsigned i = 0; i < n; ++i)
      printf(" %u", args[i]);
    puts("");
  }
}
static void prepare_parser(unsigned radix, unsigned width) {
  memset(parser, 0, sizeof parser);
  memset(context, 0, sizeof context);
  memset(environment, 0, sizeof environment);
  memset(environment_entry, 0, sizeof environment_entry);
  memset(options, 0, sizeof options);
  memset(arguments, 0, sizeof arguments);
  pointer(context + 2, parser);
  pointer(context + 0xe, environment);
  pointer(environment, environment_entry);
  pointer(context + 0x36, options);
  pointer(parser + 0xa1, strings);
  pointer(parser + 0xa5, arguments);
  ((void (*)(void *))(image + 0x39d20))(parser);
  unsigned base_args[] = {radix}, width_args[] = {width, 0};
  ((void (*)(void *, unsigned, void *, void *))(image + 0x1f32e))(
      context, 1, base_args, NULL);
  ((void (*)(void *, unsigned, void *, void *))(image + 0x1f39f))(
      context, 2, width_args, NULL);
  require(parser[0x38] == radix && parser[0x39] == width,
          "original state setters select base and XY width");
  parser[0x9f] = parser[0xa0] = 13;
  pointer(parser + 0xaa, (void *)capture);
  pointer(parser + 0xb2, context);
  calls = slot = count = 0;
}
static void feed(const void *data, size_t length) {
  const unsigned char *p = data;
  for (size_t i = 0; i < length; ++i)
    ((void (*)(void *, unsigned))(image + 0x39e90))(parser, p[i]);
}
static void parse(const char *wire) {
  prepare_parser(36, 2);
  feed(wire, strlen(wire));
}

/* Use unsigned arithmetic for original x86 wrap, signed division for IDIV. */
static int32_t wrap_add(int32_t a, int32_t b) {
  return (int32_t)((uint32_t)a + (uint32_t)b);
}
static int32_t lerp(int32_t a, int32_t b, unsigned i, unsigned n) {
  int32_t product = (int32_t)(((uint32_t)b - (uint32_t)a) * i);
  return wrap_add(a, product / (int)n);
}
static int expected_coordinate(const int *p, unsigned axis, unsigned i,
                               unsigned n) {
  if (!i)
    return p[axis];
  if (i == (n > 1 ? n - 1 : 1))
    return (int16_t)p[6 + axis];
  int32_t q[4];
  for (unsigned j = 0; j < 4; ++j)
    q[j] = (uint32_t)p[2 * j + axis] << 8;
  for (unsigned level = 3; level; --level)
    for (unsigned j = 0; j < level; ++j)
      q[j] = lerp(q[j], q[j + 1], i, n);
  /* Explicit floor division by 256 rather than implementation-defined SAR. */
  int32_t shifted = q[0] / 256 - (q[0] < 0 && q[0] % 256 != 0);
  return (int16_t)shifted;
}
static int generated[2 * 4096 + 8];
static void original_generate(const int *p, unsigned n, int *out, int *bounds) {
  ((void (*)(int, int, int, int, int, int, int, int, unsigned, int *, int *))(
      image + 0x2c9e0))(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], n, out,
                        bounds);
}
static void check_generator(const int *p, unsigned n) {
  unsigned written = n > 1 ? n : 2;
  for (unsigned i = 0; i < sizeof generated / sizeof generated[0]; ++i)
    generated[i] = 0x34567812;
  int bounds[] = {INT32_MAX, INT32_MAX, INT32_MIN, INT32_MIN};
  int expected[] = {INT32_MAX, INT32_MAX, INT32_MIN, INT32_MIN};
  original_generate(p, n, generated, bounds);
  for (unsigned i = 0; i < written; ++i)
    for (unsigned a = 0; a < 2; ++a) {
      int v = expected_coordinate(p, a, i, n);
      if (generated[2 * i + a] != v) {
        fprintf(stderr, "N=%u i=%u axis=%u actual=%d expected=%d\n", n, i, a,
                generated[2 * i + a], v);
      }
      require(generated[2 * i + a] == v,
              "original fixed-point de Casteljau sample");
      if (v < expected[a])
        expected[a] = v;
      if (v > expected[a + 2])
        expected[a + 2] = v;
    }
  require(!memcmp(bounds, expected, sizeof bounds),
          "original generated-point bounds");
  require(generated[2 * written] == 0x34567812 &&
              generated[2 * written + 1] == 0x34567812,
          "original writes N points, or two scratch endpoints for N=0/1");
}
static unsigned char draw_context[512], graphics[32], style[0x61], world[128],
    ports[128], entry[0x78];
static struct {
  uint32_t before;
  int data[20000];
  uint32_t after;
} workspace;
static unsigned allocations, frees, errors, draws, draw_count, draw_kind;
static int draw_points[2 * 4096];
static void *allocate(unsigned bytes) {
  require(bytes == sizeof workspace.data && allocations == frees,
          "one original 80000-byte workspace allocation");
  ++allocations;
  workspace.before = workspace.after = 0x1234abcd;
  memset(workspace.data, 0x56, sizeof workspace.data);
  return workspace.data;
}
static void release(void *p) {
  require(p == workspace.data && allocations == frees + 1,
          "original workspace released exactly once");
  require(workspace.before == 0x1234abcd && workspace.after == 0x1234abcd,
          "workspace guards remain intact");
  ++frees;
}
static void report(void *c, unsigned severity, const char *where,
                   const char *message) {
  (void)c;
  (void)severity;
  (void)where;
  (void)message;
  ++errors;
}
static unsigned __attribute__((stdcall)) set_mode(unsigned dc, unsigned value) {
  (void)dc;
  (void)value;
  return 1;
}
static unsigned __attribute__((stdcall)) select_object(unsigned dc,
                                                       unsigned object) {
  (void)dc;
  (void)object;
  return 1;
}
static unsigned draw(unsigned dc, const int *points, unsigned n,
                     unsigned kind) {
  require(dc == 1 && n <= 4095,
          "controlled DC and original drawing point count");
  ++draws;
  draw_count = n;
  draw_kind = kind;
  memcpy(draw_points, points, n * 2 * sizeof(int));
  return n >= 2;
}
static unsigned __attribute__((stdcall)) polygon(unsigned dc, const int *p,
                                                 unsigned n) {
  return draw(dc, p, n, 1);
}
static unsigned __attribute__((stdcall)) polyline(unsigned dc, const int *p,
                                                  unsigned n) {
  return draw(dc, p, n, 2);
}
static void prepare_drawing(unsigned border, unsigned scale) {
  memset(draw_context, 0, sizeof draw_context);
  memset(graphics, 0, sizeof graphics);
  memset(style, 0, sizeof style);
  memset(world, 0, sizeof world);
  memset(ports, 0, sizeof ports);
  memset(entry, 0, sizeof entry);
  pointer(draw_context + 0xa, graphics);
  pointer(graphics, style);
  put16(style + 0x22, border);
  pointer(draw_context + 0xe, world);
  put32(world + 0x24, 640 * scale);
  put32(world + 0x28, 480 * scale);
  put32(world + 0x2c, 640);
  put32(world + 0x30, 480);
  pointer(draw_context + 0x22, ports);
  pointer(ports, entry);
  put32(ports + 0x14, 640);
  put32(ports + 0x18, 480);
  put32(entry + 0xb, 640);
  put32(entry + 0xf, 480);
  put32(entry + 0x6c, 2);
  put32(draw_context + 0x62, 1);
  allocations = frees = errors = draws = draw_count = draw_kind = 0;
}
static void invoke_poly(unsigned mode, unsigned n, unsigned *args) {
  ((void (*)(void *, unsigned, void *, unsigned))(image + 0x2cda0))(
      draw_context, n, args, mode);
  require(allocations == 1 && frees == 1,
          "common consumer releases allocation");
}
static void numerical_checks(void) {
  const int profiles[][8] = {
      {10, 20, 71, 203, 311, 17, 409, 307},
      {-11, -3, 17, -211, -306, 19, -411, 309},
      {0, 0, 0, 0, 0, 0, 1, -1},
      {-32768, 32767, 32767, -32768, -32000, 32000, 32767, -32768}};
  for (unsigned n = 0; n <= 4095; ++n)
    check_generator(profiles[0], n);
  const unsigned selected[] = {0, 1, 2, 3, 4, 5, 40, 1295, 4094, 4095};
  for (unsigned p = 1; p < sizeof profiles / sizeof profiles[0]; ++p)
    for (unsigned i = 0; i < sizeof selected / sizeof selected[0]; ++i)
      check_generator(profiles[p], selected[i]);
  /* Observe Z's real drawing consumer even for zero/one-point requests. */
  for (unsigned n = 0; n <= 4095; ++n) {
    prepare_drawing(0, 1);
    const int *p = profiles[0];
    ((void (*)(void *, unsigned, int, int, int, int, int, int, int, int,
               unsigned))(image + 0x2cc30))(draw_context, 1, p[0], p[1], p[2],
                                            p[3], p[4], p[5], p[6], p[7], n);
    require(allocations == 1 && frees == 1 && !errors && draws == 1 &&
                draw_kind == 2 && draw_count == n,
            "Z forwards original count to Polyline");
    if (n >= 2)
      require(draw_points[2 * (n - 1)] == p[6] &&
                  draw_points[2 * (n - 1) + 1] == p[7],
              "Z drawing reaches final endpoint for N>=2");
  }
}
static void consumer_checks(void) {
  static const unsigned curve_types[] = {1, 2, 5, 6};
  static const unsigned line_types[] = {0, 3, 4};
  /* Each wire alias is consumed identically; open last curves reject wholly. */
  for (unsigned mode = 0; mode < 3; ++mode)
    for (unsigned border = 0; border < 2; ++border)
      for (unsigned c = 0; c < 4; ++c)
        for (unsigned l = 0; l < 3; ++l) {
          unsigned args[] = {2,  4,  curve_types[c], 10,  20, 40, 80,
                             70, 90, line_types[l],  110, 120};
          prepare_drawing(border, 1);
          invoke_poly(mode, 12, args);
          require(!errors && draws == (mode == 0 && border ? 2u : 1u) &&
                      draw_kind == (mode == 2 ? 2u : 1u) && draw_count == 5,
                  "curve/point type aliases and open/closed/filled consumer");
          int p[] = {10, 20, 40, 80, 70, 90, 110, 120};
          for (unsigned i = 0; i < 4; ++i)
            for (unsigned a = 0; a < 2; ++a)
              require(draw_points[2 * i + a] == expected_coordinate(p, a, i, 4),
                      "curve ends at next record's first pair");
          require(draw_points[8] == 110 && draw_points[9] == 120,
                  "terminal point retained without explicit closure duplicate");
          unsigned last[] = {
              2,  4, line_types[l], 110, 120, curve_types[c], 10, 20, 40, 80,
              70, 90};
          prepare_drawing(border, 1);
          invoke_poly(mode, 12, last);
          require(!errors &&
                      (mode == 2 ? draws == 0
                                 : draws == (mode == 0 && border ? 2u : 1u)),
                  "open terminal curve rejects whole command; closed/filled "
                  "accepts");
          if (mode != 2)
            require(
                draw_count == 5 && draw_points[8] == 110 &&
                    draw_points[9] == 120,
                "closed terminal cubic wraps to first record's start point");
        }
  const unsigned counts[] = {0,    1,    2,    3,    40,  1295,
                             2046, 2047, 4093, 4094, 4095};
  for (unsigned mode = 0; mode < 3; ++mode)
    for (unsigned k = 0; k < sizeof counts / sizeof counts[0]; ++k) {
      unsigned n = counts[k],
               args[] = {3,   n,   2,   10,  20,  40,  80, 70,  90, 6,
                         110, 120, 140, 160, 180, 190, 0,  210, 220};
      prepare_drawing(0, 1);
      invoke_poly(mode, 19, args);
      unsigned effective = 2 * n + 2 > 4095 ? (4094 - 1) / 2 : n;
      require(!errors && draws == (n ? 1u : 0u),
              "zero count rejects any curve-bearing poly command");
      if (n)
        require(draw_count == 2 * effective + 1,
                "shared subdivision count reduces to generated cap");
      unsigned lines[] = {3, n, 0, 10, 20, 3, 40, 80, 4, 70, 90};
      prepare_drawing(0, 1);
      invoke_poly(mode, 11, lines);
      require(!errors && draws == 1 && draw_count == 3,
              "all-line records ignore subdivision count including zero");
    }
  for (unsigned mode = 0; mode < 3; ++mode) {
    const unsigned bad[][13] = {{0, 4},
                                {2, 4, 7, 10, 20, 0, 30, 40},
                                {3, 4, 0, 10, 20, 0, 30, 40},
                                {2, 4, 0, 10, 20, 0, 30},
                                {1, 4, 0, 10, 20, 0, 30, 40}};
    const unsigned sizes[] = {2, 8, 8, 7, 8};
    for (unsigned i = 0; i < 5; ++i) {
      unsigned args[32] = {0};
      memcpy(args, bad[i], sizeof bad[i]);
      prepare_drawing(0, 1);
      invoke_poly(mode, sizes[i], args);
      require(
          !draws && errors == (i == 0 ? 1u : 0u),
          "malformed vector rejects whole command; most failures are silent");
    }
  }
}

static void feed_number(unsigned value, unsigned width, unsigned radix) {
  static const char digits[] =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz#&";
  char text[5];
  for (unsigned i = width; i; --i) {
    text[i - 1] = digits[value % radix];
    value /= radix;
  }
  require(!value, "fixture numeral fits declared width");
  feed(text, width);
}
static void parser_checks(void) {
  const char commands[] = {'t', 'z', 'x'};
  const unsigned slots[] = {61, 77, 71};
  for (unsigned radix = 36; radix <= 64; radix += 28)
    for (unsigned width = 2; width <= 5; ++width)
      for (unsigned command = 0; command < 3; ++command)
        for (unsigned type = 0; type < 7; ++type) {
          prepare_parser(radix, width);
          char prefix[] = {'!', '|', commands[command]};
          feed(prefix, 3);
          feed_number(1, 2, radix);
          feed_number(radix * radix - 1, 2, radix);
          feed_number(type, 1, radix);
          unsigned pairs = (type == 0 || type == 3 || type == 4) ? 1 : 3;
          for (unsigned i = 0; i < 2 * pairs; ++i)
            feed_number(37 + i, width, radix);
          feed("\r\n", 2);
          require(calls == 1 && slot == slots[command] &&
                      count == 3 + 2 * pairs && captured[0] == 1 &&
                      captured[1] == radix * radix - 1 && captured[2] == type,
                  "original special parser keeps literal type and fixed "
                  "two-digit headers");
          for (unsigned i = 0; i < 2 * pairs; ++i)
            require(captured[3 + i] == 37 + i,
                    "original special records honor active XY width/radix");
        }
  /* Full accepted argument domain is separate from the generated output cap. */
  for (unsigned curves = 0; curves < 2; ++curves)
    for (unsigned extra = 0; extra < 2; ++extra) {
      unsigned n = (curves ? 1170 : 2730) + extra;
      prepare_parser(64, 2);
      feed("!|z", 3);
      feed_number(n, 2, 64);
      feed_number(40, 2, 64);
      for (unsigned i = 0; i < n; ++i) {
        feed_number(curves ? 2 : 0, 1, 64);
        for (unsigned j = 0; j < (curves ? 6u : 2u); ++j)
          feed_number(j + 1, 2, 64);
      }
      feed("\r\n", 2);
      unsigned total = 2 + n * (curves ? 7 : 3);
      require(
          calls == 1 && slot == 77 && count == (total > 8193 ? 8193 : total),
          "special block parser retains aggregate 8193-value truncation limit");
      if (!extra) {
        prepare_drawing(0, 1);
        invoke_poly(1, count, captured);
        require(!errors && draws == 1 && draw_count == (curves ? 3510u : 2730u),
                "maximum complete input records accepted with independent "
                "output reduction");
      } else {
        prepare_drawing(0, 1);
        invoke_poly(1, count, captured);
        require(!errors && !draws,
                "truncated oversized records reject without drawing");
      }
    }
}
static void world_checks(void) {
  const unsigned guards[] = {0x3445b, 0x18cf6, 0x343e4, 0x342ad,
                             0xe6e9,  0x34319, 0x18bf7};
  for (unsigned i = 0; i < sizeof guards / sizeof guards[0]; ++i)
    hook(guards[i], (void (*)(void))zero);
  for (unsigned scale = 1; scale <= 2; ++scale) {
    unsigned args[] = {11, 21, 41, 81, 71, 91, 111, 121, 40};
    int converted[8];
    for (unsigned i = 0; i < 8; ++i)
      converted[i] = args[i] / scale;
    prepare_drawing(0, scale);
    ((void (*)(void *, unsigned, unsigned *, const char *))(image + 0x19f7b))(
        draw_context, 9, args, NULL);
    require(!errors && draws == 1 && draw_count == 40,
            "original Z handler world conversion reaches drawing");
    for (unsigned i = 0; i < 40; ++i)
      for (unsigned a = 0; a < 2; ++a)
        require(draw_points[2 * i + a] ==
                    expected_coordinate(converted, a, i, 40),
                "Z converts and truncates control points before fixed-point "
                "evaluation");
    unsigned poly[] = {2, 40, 2, 11, 21, 41, 81, 71, 91, 0, 111, 121};
    prepare_drawing(0, scale);
    invoke_poly(2, 12, poly);
    require(!errors && draws == 1 && draw_count == 41,
            "original poly world conversion reaches drawing");
    for (unsigned i = 0; i < 40; ++i)
      for (unsigned a = 0; a < 2; ++a)
        require(draw_points[2 * i + a] ==
                    expected_coordinate(converted, a, i, 40),
                "poly converts and truncates control points before fixed-point "
                "evaluation");
  }
  /* One closed cubic can reach 4094 points; a following point reduces N by one.
   */
  for (unsigned following = 0; following < 2; ++following) {
    unsigned args[] = {1 + following, 4095, 2, 10, 20, 40, 80, 70, 90, 0,
                       110,           120};
    prepare_drawing(0, 1);
    invoke_poly(1, following ? 12 : 9, args);
    require(!errors && draws == 1 && draw_count == 4094,
            "generated cap is 4094 actual points, independent of workspace "
            "capacity");
  }
}
int main(int argc, char **argv) {
  if (argc != 2 && argc != 3)
    return 2;
  load_image(argv[1]);
  hook(0x13c49, (void (*)(void))zero);
  hook(0x13c59, (void (*)(void))zero);
  hook(0x45038, noop);
  hook(0x3d9e1, (void (*)(void))zero);
  pointer(image + 0x87908, character_types);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isspace(i) ? 8 : 0) | (isdigit(i) ? 4 : 0) |
                         (isupper(i) ? 1 : 0) | (islower(i) ? 2 : 0);
  parse("!|t03142JA62HA62JEA66HUA6JGA6HCEE0JCEG\r\n");
  require(
      calls == 1 && slot == 61 && count == 19 &&
          !memcmp(captured,
                  (unsigned[]){3, 40, 2, 694, 218, 622, 218, 698, 366, 6, 642,
                               366, 700, 366, 624, 518, 0, 696, 520},
                  19 * sizeof(unsigned)),
      "original t sample has typed records and no separate base-point header");
  parse("!|"
        "z081428KGC8KESASF85CGF8E4F8BCGC5DKGCEOGCE4GW5DKHGD0I0BWI06ASI0BWGWASFS"
        "09OGW09OI008KI0\r\n");
  require(
      calls == 1 && slot == 77 && count == 46 &&
          !memcmp(captured,
                  (unsigned[]){8,   40,  2,   308, 588, 308, 532, 388, 548, 5,
                               448, 548, 508, 548, 408, 588, 5,   488, 588, 528,
                               588, 508, 608, 5,   488, 628, 468, 648, 428, 648,
                               6,   388, 648, 428, 608, 388, 568, 0,   348, 608,
                               0,   348, 648, 0,   308, 648},
                  46 * sizeof(unsigned)),
      "original complete z census sample expands into eight accepted records");
  hook(0x6bd40, (void (*)(void))allocate);
  hook(0x6bd30, (void (*)(void))release);
  hook(0x38e47, (void (*)(void))report);
  hook(0x3988f, noop);
  hook(0x398ee, noop);
  hook(0x13ce5, (void (*)(void))zero);
  hook(0x13cfc, (void (*)(void))zero);
  hook(0x13d10, (void (*)(void))zero);
  hook(0x13d27, (void (*)(void))zero);
  pointer(image + 0x963f0, (void *)set_mode);
  pointer(image + 0x963c0, (void *)select_object);
  pointer(image + 0x9641c, (void *)polygon);
  pointer(image + 0x9647c, (void *)polyline);
  prepare_drawing(0, 2);
  invoke_poly(1, count, captured);
  require(!errors && draws == 1 && draw_count == 203 && draw_points[0] == 154 &&
              draw_points[1] == 294 && draw_points[404] == 154 &&
              draw_points[405] == 324,
          "complete original z corpus vector draws 203 points with implicit "
          "closure");
  check_generator((int[]){0, 0, 0, 0, 0, 0, 64, 0}, 4);
  require(!memcmp(generated, (int[]){0, 0, 1, 0, 8, 0, 64, 0}, 8 * sizeof(int)),
          "four-point cubic oracle skips t=3/4 and retains exact endpoint");
  numerical_checks();
  consumer_checks();
  parser_checks();
  world_checks();
  if (argc == 3) {
    verbose = 1;
    FILE *f = fopen(argv[2], "rb");
    if (!f)
      fail("open original corpus");
    prepare_parser(36, 2);
    int ch;
    while ((ch = fgetc(f)) != EOF) {
      unsigned char c = ch;
      feed(&c, 1);
    }
    fclose(f);
  }
  printf("PASS: %u parser/numerical/consumer checks\n", checks);
  return 0;
}
