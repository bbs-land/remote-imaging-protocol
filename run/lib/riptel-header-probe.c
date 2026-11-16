/* Execute the original byte parser and header validator. No OS GUI is used. */
#include "riptel-probe-image.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
static unsigned char parser[256], context[512];
static char strings[16384];
static uint32_t arguments[8194], captured[8194];
static unsigned calls, captured_slot, captured_count;
static unsigned zero(void) { return 0; }
static void capture(void *ctx, unsigned slot, unsigned count, uint32_t *args,
                    const char *tail) {
  (void)ctx;
  (void)tail;
  require(count < 8194, "callback argument bound");
  ++calls;
  captured_slot = slot;
  captured_count = count;
  memcpy(captured, args, count * 4);
}
static void parse(const char *wire) {
  memset(parser, 0, sizeof parser);
  pointer(parser + 0xa1, strings);
  pointer(parser + 0xa5, arguments);
  ((void (*)(void *))(image + 0x39d20))(parser);
  parser[0x38] = 36;
  parser[0x39] = 2;
  parser[0x9f] = 13;
  parser[0xa0] = 13;
  pointer(parser + 0xaa, (void *)capture);
  pointer(parser + 0xb2, context);
  calls = 0;
  captured_count = captured_slot = 0;
  for (; *wire; ++wire)
    ((void (*)(void *, unsigned))(image + 0x39e90))(parser,
                                                    (unsigned char)*wire);
}
static unsigned valid(unsigned count, uint32_t *args) {
  return ((unsigned (*)(void *, unsigned, void *))(image + 0x1774f))(
             context, count, args) &
         0xffff;
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  hook(0x13c49, (void (*)(void))zero);
  hook(0x13c59, (void (*)(void))zero); /* framing policy: accept RIP */
  parse("!|h01000000\r\n");
  require(calls == 1 && captured_slot == 32 && captured_count == 3,
          "base header dispatch");
  require(valid(captured_count, captured) == 1, "base header validates");
  for (unsigned type = 0; type < 10; ++type) {
    char wire[100], segment[20];
    if (type == 2 || type == 3)
      snprintf(segment, sizeof segment, "%u00000", type);
    else if (type == 5)
      strcpy(segment, "50000028");
    else if (type < 5)
      snprintf(segment, sizeof segment, "%u000", type);
    else
      snprintf(segment, sizeof segment, "%u00", type);
    snprintf(wire, sizeof wire, "!|h01000001%s\r\n", segment);
    parse(wire);
    require(calls == 1 && captured_slot == 32,
            "typed segment parsed under root header");
    require(captured[2] == 1 && captured[3] == type,
            "segment selector is numeric argument");
    require(valid(captured_count, captured) == (type != 9),
            "charset segment validator/descriptor mismatch");
  }
  parse("!|h01000003001070150002028\r\n");
  static const uint32_t expected[] = {1, 0, 3, 0, 1, 0, 7, 1, 5, 2, 0, 2, 8};
  require(calls == 1 && captured_count == 13 &&
              !memcmp(captured, expected, sizeof expected),
          "mixed header segments decode");
  require(valid(captured_count, captured) == 1, "mixed header validates");
  parse("!|h01000001\r\n");
  require(calls == 1 && !valid(captured_count, captured),
          "missing declared segment rejects");
  parse("!|h010000000000\r\n");
  require(calls == 1 && valid(captured_count, captured),
          "zero segment count ignores parsed extras");
  uint32_t args[] = {1, 0, 1, 5, 0x400, 0, 2, 8};
  require(!valid(8, args),
          "environment coordinate validation erroneously checks slot zero");
  args[5] = 2;
  args[6] = 63;
  require(valid(8, args),
          "environment coordinate validation does not check width field");
  args[4] = 0x1000;
  args[5] = 0;
  args[6] = 2;
  args[7] = 8;
  require(!valid(8, args),
          "RGB validation erroneously checks environment slot");
  args[5] = 1;
  args[7] = 63;
  require(valid(8, args), "RGB validator does not check RGB field");
  const char *absent[] = {"!|1N00000000000000\r\n", "!|1O00000000\r\n",
                          "!|1S00000000\r\n", "!|\0330000\r\n"};
  for (unsigned i = 0; i < 4; ++i) {
    parse(absent[i]);
    require(calls == 0, "unattested opcode has no dispatch");
  }
  parse("!|q0F\r\n");
  require(calls == 1 && captured_slot == 55 && captured[0] == 15,
          "FontAttrib q dispatch");
  parse("!|r010000hello\r\n");
  require(calls == 1 && captured_slot == 57 && captured_count == 3,
          "TextMetric r dispatch");
  parse("!|2\0330000TEST\r\n");
  require(calls == 1 && captured_slot == 121 && captured_count == 1,
          "SwitchDirectory level 2 ESC dispatch identity");
  printf("PASS: %u preliminary header/parser checks\n", checks);
  return 0;
}
