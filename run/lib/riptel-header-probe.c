/* Execute the original byte parser and header validator. No OS GUI is used. */
#include "riptel-probe-image.h"
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
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
static unsigned char view_entry[16];
static unsigned char style_table[32], style[0x61], viewport[128],
    host_state[0x600], configuration[0x100];
static char scratch_buffer[8192], text_data[1024], response[8192],
    made_directory[256];
static uint16_t character_types[256];
static unsigned output_calls, protection, directory_calls;
static const char *error_message, *replacement;
static void report(void *ctx, unsigned level, const char *where,
                   const char *message) {
  (void)ctx;
  (void)level;
  (void)where;
  error_message = message;
}
static void *scratch(void *ctx) {
  (void)ctx;
  return scratch_buffer;
}
static void expand(void *ctx, char *text) {
  (void)ctx;
  if (replacement && !strcmp(text, "$VAR$"))
    strcpy(text, replacement);
}
static void formatted(char *target, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vsprintf(target, format, args);
  va_end(args);
}
static unsigned width(void *ctx, const char *text) {
  (void)ctx;
  return strlen(text) * 3;
}
static unsigned height(void *ctx) {
  (void)ctx;
  return 7;
}
static void output(void *ctx, const char *text) {
  (void)ctx;
  ++output_calls;
  snprintf(response, sizeof response, "%s", text);
}
static unsigned protected_style(void *ctx, unsigned slot) {
  (void)ctx;
  (void)slot;
  return protection;
}
static int mkdir_capture(const char *path) {
  ++directory_calls;
  snprintf(made_directory, sizeof made_directory, "%s", path);
  return -1;
}
static void entry_prepare(void) {
  memset(context, 0, sizeof context);
  memset(style_table, 0, sizeof style_table);
  memset(style, 0, sizeof style);
  memset(viewport, 0, sizeof viewport);
  memset(host_state, 0, sizeof host_state);
  memset(configuration, 0, sizeof configuration);
  put16(context, 0x9df3);
  pointer(context + 0xa, style_table);
  pointer(style_table, style);
  pointer(context + 0xe, viewport);
  pointer(viewport, view_entry);
  put16(view_entry + 1, 640);
  put16(view_entry + 3, 480);
  pointer(context + 0x32, host_state);
  pointer(host_state + 0x4a, text_data);
  pointer(context + 0x72, configuration);
  strcpy((char *)configuration + 0x16, "ROOT");
  put32(context + 0x5e, 1);
  put32(viewport + 0x24, 1280);
  put32(viewport + 0x28, 960);
  put32(viewport + 0x2c, 640);
  put32(viewport + 0x30, 480);
  style[0x24] = 1;
  text_data[0] = response[0] = made_directory[0] = 0;
  error_message = replacement = NULL;
  output_calls = protection = directory_calls = 0;
}
static void entry_hooks(void) {
  hook(0x45038, noop);
  hook(0x3988f, noop);
  hook(0x398ee, noop);
  hook(0x38e47, (void (*)(void))report);
  hook(0x15396, (void (*)(void))scratch);
  hook(0x153fe, noop);
  hook(0x4b0e4, (void (*)(void))expand);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x68910, (void (*)(void))strcpy);
  hook(0x65493, (void (*)(void))strncpy);
  hook(0x654a8, (void (*)(void))strlen);
  hook(0x68dd0, (void (*)(void))strlen);
  hook(0x655e2, (void (*)(void))strcasecmp);
  hook(0x65442, (void (*)(void))strcat);
  hook(0x65976, (void (*)(void))strpbrk);
  hook(0x68440, (void (*)(void))formatted);
  hook(0x3935b, (void (*)(void))protected_style);
  hook(0x6947, (void (*)(void))output);
  hook(0x6c430, (void (*)(void))mkdir_capture);
  /* Font selection/rendering and primitive metrics use controlled dimensions.
   */
  hook(0x10f0, noop);
  hook(0x39808, noop);
  hook(0x2830, noop);
  hook(0x1410, (void (*)(void))width);
  hook(0x1640, (void (*)(void))height);
  hook(0x42810, (void (*)(void))zero);
  hook(0x3fb30, noop);
  hook(0x40ed0, noop);
  hook(0x3fcd0, (void (*)(void))width);
  hook(0x3fc70, (void (*)(void))height);
  pointer(image + 0x87908, character_types);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isspace(i) ? 8 : 0) | (isdigit(i) ? 4 : 0);
}
static void command(unsigned rva, uint32_t *args, const char *text) {
  ((void (*)(void *, unsigned, uint32_t *, const char *))(image + rva))(
      context, 3, args, text);
}
static void entry_checks(void) {
  entry_hooks();
  entry_prepare();
  for (unsigned attr = 0; attr < 4096; ++attr) {
    uint32_t args[] = {attr};
    error_message = NULL;
    put32(style + 0x53, 0x20);
    command(0x1c799, args, NULL);
    require((error_message == NULL) == (attr <= 15), "q range 0..15");
    require(u32(style + 0x53) == (0x20 | (attr <= 15 ? attr : 0)),
            "q accumulates low flags without clearing others");
  }
  uint32_t attr[] = {15};
  style[0x24] = 0;
  error_message = NULL;
  command(0x1c799, attr, NULL);
  require(error_message != NULL, "q rejects system font");
  style[0x24] = 1;
  protection = 1;
  error_message = NULL;
  command(0x1c799, attr, NULL);
  require(error_message != NULL, "q rejects protected style");
  protection = 0;
  for (unsigned extended = 0; extended < 2; ++extended)
    for (unsigned mode = 0; mode < 4; ++mode)
      for (unsigned domain = 0; domain < 2; ++domain) {
        entry_prepare();
        style[0x24] = extended;
        uint32_t args[] = {mode, domain, 0xffffff};
        command(0x20371, args, "ab");
        const char *expected = mode == 0   ? (domain ? "6\r" : "12\r")
                               : mode == 1 ? (domain ? "7\r" : "14\r")
                               : mode == 2 ? (domain ? "3:3\r" : "6:6\r")
                                           : (domain ? "7:7\r" : "14:14\r");
        require(!error_message && output_calls == 1 &&
                    !strcmp(response, expected),
                "r mode/domain output format and conversion");
        require(!strcmp(text_data, "ab"), "r updates retained TEXTDATA");
      }
  entry_prepare();
  uint32_t args[] = {2, 1, 0};
  strcpy(text_data, "ab");
  command(0x20371, args, "$TEXTDATA$");
  require(!strcmp(response, "3:3\r"), "r retained TEXTDATA sentinel");
  args[0] = 4;
  output_calls = 0;
  command(0x20371, args, "ab");
  require(error_message && !output_calls, "r invalid mode rejects");
  args[0] = 0;
  args[1] = 2;
  output_calls = 0;
  error_message = NULL;
  command(0x20371, args, "ab");
  require(error_message && !output_calls, "r invalid domain rejects");
  entry_prepare();
  uint32_t reserved[] = {0xffffffff};
  command(0x46f66, reserved, "  mixed  ");
  require(!error_message && !strcmp((char *)host_state + 0x462, "MIXED") &&
              !strcmp(made_directory, "ROOT\\MIXED"),
          "directory trim uppercase store and create");
  require(directory_calls == 1,
          "directory creation failure does not roll back selection");
  command(0x46f66, reserved, "$off$");
  require(!host_state[0x462] && directory_calls == 1,
          "literal OFF clears without creating directory");
  const char *invalid[] = {".",     "..", "1234567890123", "a/b", "a\\b",
                           "C:foo", "a?b"};
  for (unsigned i = 0; i < sizeof invalid / sizeof invalid[0]; ++i) {
    error_message = NULL;
    command(0x46f66, reserved, invalid[i]);
    require(error_message != NULL, "invalid directory rejects");
  }
  error_message = NULL;
  replacement = "$OFF$";
  command(0x46f66, reserved, "$VAR$");
  require(!error_message && !strcmp((char *)host_state + 0x462, "$OFF$"),
          "expanded OFF is literal directory name");
  put32(context + 0x5e, 0);
  error_message = NULL;
  command(0x46f66, reserved, "else");
  require(!error_message && !strcmp((char *)host_state + 0x462, "$OFF$"),
          "missing host callback suppresses directory command");
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
  entry_checks();
  printf("PASS: %u preliminary header/parser/entry checks\n", checks);
  return 0;
}
