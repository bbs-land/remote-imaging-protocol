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
  printf("PASS: %u parser observation checks\n", checks);
  return 0;
}
