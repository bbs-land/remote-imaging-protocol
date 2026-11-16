/* Execute original byte-parser, header and recovered command routines with
 * controlled host callbacks. No OS GUI is used; this is not a complete client.
 */
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
static unsigned calls, captured_slot, captured_count, execute_headers;
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
  if (execute_headers && slot == 32)
    ((void (*)(void *, unsigned, uint32_t *, const char *))(image + 0x1cae1))(
        ctx, count, args, tail);
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
static unsigned char view_entry[36 * 16];
static unsigned char style_table[32], style[36 * 0x61], viewport[128],
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
  hook(0x40ed0, (void (*)(void))zero);
  hook(0x3fcd0, (void (*)(void))width);
  hook(0x3fc70, (void (*)(void))height);
  pointer(image + 0x87908, character_types);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isspace(i) ? 8 : 0) | (isdigit(i) ? 4 : 0) |
                         (isupper(i) ? 1 : 0) | (islower(i) ? 2 : 0);
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
  for (unsigned extended = 0; extended < 2; ++extended)
    for (unsigned mode = 0; mode < 4; ++mode) {
      entry_prepare();
      style[0x24] = extended;
      uint32_t empty[] = {mode, 1, 0};
      command(0x20371, empty, "");
      const char *expected = mode == 0 ? "0\r" : mode == 1 ? "7\r" : "\r";
      require(extended ? output_calls == 0
                       : (output_calls == 1 && !strcmp(response, expected)),
              "r empty literal differs by font mode");
    }
  entry_prepare();
  replacement = "";
  uint32_t empty_expansion[] = {0, 1, 0};
  command(0x20371, empty_expansion, "$VAR$");
  require(output_calls == 1 && !strcmp(response, "0\r") && !text_data[0],
          "r nonempty expression expanded empty still replies");
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
  replacement = NULL;
  error_message = NULL;
  command(0x46f66, reserved, "");
  require(error_message != NULL, "empty literal directory rejects");
  error_message = NULL;
  command(0x46f66, reserved, "   ");
  require(!error_message && !host_state[0x462] &&
              !strcmp(made_directory, "ROOT\\"),
          "whitespace directory clears and attempts root creation");
  command(0x46f66, reserved, "$OFF$");
  replacement = "$OFF$";
  command(0x46f66, reserved, "$VAR$");
  put32(context + 0x5e, 0);
  error_message = NULL;
  command(0x46f66, reserved, "else");
  require(!error_message && !strcmp((char *)host_state + 0x462, "$OFF$"),
          "missing host callback suppresses directory command");
}
struct event {
  unsigned rva;
  int a, b, c;
};
static struct event events[4096];
static unsigned event_count;
static unsigned char driver_state[512], button_table[128],
    button_entries[36 * 28];
static unsigned char port_table[128], port_entries[36 * 0x78], text_table[128],
    text_entries[36 * 0xf1];
static unsigned char palette_table[128], palette_entries[36 * 0x302];
static void event(unsigned rva, int a, int b, int c) {
  require(event_count < 4096, "header event bound");
  events[event_count++] = (struct event){rva, a, b, c};
}
static unsigned op_16665(void *ctx, int a) {
  (void)ctx;
  event(0x16665, a, 0, 0);
  return 1;
}
static unsigned op_16220(void *ctx, int a, int b, int c) {
  (void)ctx;
  event(0x16220, a, b, c);
  return 1;
}
static unsigned op_16ba5(void *ctx) {
  (void)ctx;
  event(0x16ba5, 0, 0, 0);
  return 1;
}
static unsigned op_3468a(void *ctx, int a) {
  (void)ctx;
  event(0x3468a, a, 0, 0);
  return 1;
}
static unsigned op_3e41a(void *ctx) {
  (void)ctx;
  event(0x3e41a, 0, 0, 0);
  return 1;
}
static unsigned op_4558d(void *ctx, int a) {
  (void)ctx;
  event(0x4558d, a, 0, 0);
  return 1;
}
static unsigned op_391fd(void *ctx, int a) {
  (void)ctx;
  event(0x391fd, a, 0, 0);
  return 1;
}
static unsigned op_33112(void *ctx, int a, int b) {
  (void)ctx;
  event(0x33112, a, b, 0);
  return 1;
}
static unsigned op_33d82(void *ctx) {
  (void)ctx;
  event(0x33d82, 0, 0, 0);
  return 1;
}
static unsigned op_39ac8(void *ctx, int a) {
  (void)ctx;
  event(0x39ac8, a, 0, 0);
  return 1;
}
static unsigned op_83a5(void *ctx, int a) {
  (void)ctx;
  event(0x83a5, a, 0, 0);
  return 1;
}
static unsigned op_440f0(void *ctx, int a) {
  (void)ctx;
  event(0x440f0, a, 0, 0);
  return 1;
}
static unsigned op_3d7d1(void *ctx, int a) {
  (void)ctx;
  event(0x3d7d1, a, 0, 0);
  return 1;
}
static unsigned op_23d6e(void *ctx, int a, int b) {
  (void)ctx;
  event(0x23d6e, a, b, 0);
  return 1;
}
static unsigned op_24648(void *ctx, int a) {
  (void)ctx;
  event(0x24648, a, 0, 0);
  return 1;
}
static unsigned op_454c4(void *ctx, int a, int b) {
  (void)ctx;
  event(0x454c4, a, b, 0);
  return 1;
}
static unsigned op_390e4(void *ctx, int a, int b) {
  (void)ctx;
  event(0x390e4, a, b, 0);
  return 1;
}
static unsigned op_3378a(void *ctx, int a, int b) {
  (void)ctx;
  event(0x3378a, a, b, 0);
  return 1;
}
static unsigned op_2753a(void *ctx, int a, int b) {
  (void)ctx;
  event(0x2753a, a, b, 0);
  return 1;
}
static unsigned op_443c3(void *ctx, int a, int b) {
  (void)ctx;
  event(0x443c3, a, b, 0);
  return 1;
}
static unsigned op_3d8d0(void *ctx, int a, int b) {
  (void)ctx;
  event(0x3d8d0, a, b, 0);
  return 1;
}
static unsigned op_33f55(void *ctx, int a, int b) {
  (void)ctx;
  event(0x33f55, a, b, 0);
  return 1;
}
static unsigned op_34067(void *ctx, int a) {
  (void)ctx;
  event(0x34067, a, 0, 0);
  return 1;
}
static unsigned op_340bd(void *ctx, int a) {
  (void)ctx;
  event(0x340bd, a, 0, 0);
  return 1;
}
static unsigned op_33e20(void *ctx) {
  (void)ctx;
  event(0x33e20, 0, 0, 0);
  return 1;
}
static unsigned op_33e40(void *ctx, int a) {
  (void)ctx;
  event(0x33e40, a, 0, 0);
  return 1;
}
static unsigned op_2bf9e(void *ctx, int a, int b) {
  (void)ctx;
  event(0x2bf9e, a, b, 0);
  return 1;
}
static unsigned op_4597b(void *ctx, int a) {
  (void)ctx;
  event(0x4597b, a, 0, 0);
  return 1;
}
static unsigned op_2d663(void *ctx) {
  (void)ctx;
  event(0x2d663, 0, 0, 0);
  return 1;
}
static unsigned op_2d677(void *ctx, int a) {
  (void)ctx;
  event(0x2d677, a, 0, 0);
  return 1;
}
static unsigned op_18e33(void *ctx, int a, int b) {
  (void)ctx;
  event(0x18e33, a, b, 0);
  return 1;
}
static unsigned op_3e2e7(void *ctx) {
  (void)ctx;
  event(0x3e2e7, 0, 0, 0);
  return 1;
}
static unsigned op_12d63(void *ctx, int a, int b) {
  (void)ctx;
  event(0x12d63, a, b, 0);
  return 1;
}
static unsigned op_37e41(void *ctx) {
  (void)ctx;
  event(0x37e41, 0, 0, 0);
  return 1;
}
static unsigned op_2fcf6(void *ctx) {
  (void)ctx;
  event(0x2fcf6, 0, 0, 0);
  return 1;
}
static unsigned op_2badd(void *ctx) {
  (void)ctx;
  event(0x2badd, 0, 0, 0);
  return 1;
}
static unsigned op_2bb24(void *ctx) {
  (void)ctx;
  event(0x2bb24, 0, 0, 0);
  return 1;
}
static unsigned query_protection(void *ctx, int slot) {
  (void)ctx;
  (void)slot;
  return protection;
}
static unsigned in_use(void *ctx, int slot) {
  (void)ctx;
  (void)slot;
  return 1;
}
static void switch_button(void *ctx, int slot) {
  (void)ctx;
  event(0x4576f, slot, 0, 0);
  put16(button_table + 0xa, slot);
}
static void switch_style(void *ctx, int slot) {
  (void)ctx;
  event(0x394fe, slot, 0, 0);
  put16(style_table + 8, slot);
}
static void switch_port(void *ctx, int slot) {
  (void)ctx;
  event(0x3393c, slot, 0, 0);
  put16(port_table + 8, slot);
  put16(port_table + 0xa, slot);
}
static void switch_text(void *ctx, int slot) {
  (void)ctx;
  event(0xe0dc, slot, 0, 0);
  put16(text_table + 0xa, slot);
}
static void switch_palette(void *ctx, int slot) {
  (void)ctx;
  event(0x442c3, slot, 0, 0);
  palette_table[4] = slot;
}
static void switch_environment(void *ctx, int slot) {
  (void)ctx;
  event(0x3dac0, slot, 0, 0);
  put16(viewport + 0xa, slot);
}
static unsigned char backup_code[5];
static void header_hooks(void) {
  memcpy(backup_code, image + 0x23d6e, 5);
  hook(0x16665, (void (*)(void))op_16665);
  hook(0x16220, (void (*)(void))op_16220);
  hook(0x16ba5, (void (*)(void))op_16ba5);
  hook(0x3468a, (void (*)(void))op_3468a);
  hook(0x3e41a, (void (*)(void))op_3e41a);
  hook(0x4558d, (void (*)(void))op_4558d);
  hook(0x391fd, (void (*)(void))op_391fd);
  hook(0x33112, (void (*)(void))op_33112);
  hook(0x33d82, (void (*)(void))op_33d82);
  hook(0x39ac8, (void (*)(void))op_39ac8);
  hook(0x83a5, (void (*)(void))op_83a5);
  hook(0x440f0, (void (*)(void))op_440f0);
  hook(0x3d7d1, (void (*)(void))op_3d7d1);
  hook(0x23d6e, (void (*)(void))op_23d6e);
  hook(0x24648, (void (*)(void))op_24648);
  hook(0x454c4, (void (*)(void))op_454c4);
  hook(0x390e4, (void (*)(void))op_390e4);
  hook(0x3378a, (void (*)(void))op_3378a);
  hook(0x2753a, (void (*)(void))op_2753a);
  hook(0x443c3, (void (*)(void))op_443c3);
  hook(0x3d8d0, (void (*)(void))op_3d8d0);
  hook(0x33f55, (void (*)(void))op_33f55);
  hook(0x34067, (void (*)(void))op_34067);
  hook(0x340bd, (void (*)(void))op_340bd);
  hook(0x33e20, (void (*)(void))op_33e20);
  hook(0x33e40, (void (*)(void))op_33e40);
  hook(0x2bf9e, (void (*)(void))op_2bf9e);
  hook(0x4597b, (void (*)(void))op_4597b);
  hook(0x2d663, (void (*)(void))op_2d663);
  hook(0x2d677, (void (*)(void))op_2d677);
  hook(0x18e33, (void (*)(void))op_18e33);
  hook(0x3e2e7, (void (*)(void))op_3e2e7);
  hook(0x12d63, (void (*)(void))op_12d63);
  hook(0x37e41, (void (*)(void))op_37e41);
  hook(0x2fcf6, (void (*)(void))op_2fcf6);
  hook(0x2badd, (void (*)(void))op_2badd);
  hook(0x2bb24, (void (*)(void))op_2bb24);
  hook(0x45674, (void (*)(void))query_protection);
  hook(0x3935b, (void (*)(void))query_protection);
  hook(0x33821, (void (*)(void))query_protection);
  hook(0x27642, (void (*)(void))query_protection);
  hook(0x44508, (void (*)(void))query_protection);
  hook(0x3d9e1, (void (*)(void))query_protection);
  hook(0x338bc, (void (*)(void))in_use);
  hook(0x4460b, (void (*)(void))in_use);
  hook(0x3d6f9, (void (*)(void))in_use);
  hook(0x4576f, (void (*)(void))switch_button);
  hook(0x394fe, (void (*)(void))switch_style);
  hook(0x3393c, (void (*)(void))switch_port);
  hook(0xe0dc, (void (*)(void))switch_text);
  hook(0x442c3, (void (*)(void))switch_palette);
  hook(0x3dac0, (void (*)(void))switch_environment);
  hook(0x69e60, (void (*)(void))memcpy);
  hook(0x3c37e, noop);
}
static void header_prepare(void) {
  entry_prepare();
  pointer(context + 2, parser);
  pointer(context + 0x36, driver_state);
  memset(driver_state, 0, sizeof driver_state);
  memset(button_table, 0, sizeof button_table);
  memset(button_entries, 0, sizeof button_entries);
  memset(port_table, 0, sizeof port_table);
  memset(port_entries, 0, sizeof port_entries);
  memset(text_table, 0, sizeof text_table);
  memset(text_entries, 0, sizeof text_entries);
  memset(palette_table, 0, sizeof palette_table);
  memset(palette_entries, 0, sizeof palette_entries);
  pointer(context + 0x16, button_table);
  pointer(button_table, button_entries);
  put16(button_table + 0xa, 2);
  pointer(context + 0x22, port_table);
  pointer(port_table, port_entries);
  put16(port_table + 8, 2);
  put16(port_table + 0xa, 2);
  pointer(context + 0x12, text_table);
  pointer(text_table, text_entries);
  put16(text_table + 0xa, 2);
  pointer(context + 0x1a, palette_table);
  pointer(palette_table + 5, palette_entries);
  palette_table[4] = 2;
  put16(style_table + 8, 2);
  put16(viewport + 0xa, 2);
  event_count = 0;
}
static unsigned seen(unsigned rva) {
  unsigned n = 0;
  for (unsigned i = 0; i < event_count; ++i)
    n += events[i].rva == rva;
  return n;
}
static void apply_header(unsigned count, uint32_t *args) {
  ((void (*)(void *, unsigned, uint32_t *, const char *))(image + 0x1cae1))(
      context, count, args, NULL);
}
static unsigned char backup_state[8 * 0xb17], saved_present[10],
    saved_protected[10];
static unsigned deleted_count;
static unsigned save_exists(void *ctx, unsigned type, unsigned slot) {
  (void)ctx;
  (void)type;
  return slot < 10 && saved_present[slot];
}
static unsigned save_protected(void *ctx, unsigned type, unsigned slot) {
  (void)ctx;
  (void)type;
  return slot < 10 && saved_protected[slot];
}
static unsigned delete_save(void *record) {
  unsigned offset = (unsigned char *)record - backup_state;
  unsigned slot = offset / 0x87 - 1;
  require(slot < 10 && saved_present[slot], "known saved area deletion");
  saved_present[slot] = 0;
  ++deleted_count;
  return 1;
}
static void backup_checks(void) {
  header_prepare();
  pointer(context + 0x3e, backup_state);
  memset(backup_state, 0, sizeof backup_state);
  memset(saved_present, 0, sizeof saved_present);
  memset(saved_protected, 0, sizeof saved_protected);
  saved_present[0] = saved_present[1] = saved_present[2] = 1;
  saved_protected[0] = 1;
  deleted_count = 0;
  put16(backup_state + 0xb15, 3);
  memcpy(image + 0x23d6e, backup_code, 5);
  hook(0x24020, (void (*)(void))save_exists);
  hook(0x23fc4, (void (*)(void))save_protected);
  hook(0x212a1, (void (*)(void))delete_save);
  ((void (*)(void *, unsigned, unsigned))(image + 0x16bf4))(context, 0x10, 0);
  require(deleted_count == 1 && saved_present[0] && !saved_present[1] &&
              saved_present[2],
          "saved-area flag deletes only first unprotected numbered area");
  require(u16(backup_state + 0xb15) == 2, "saved-area count decremented once");
}
static void header_checks(void) {
  header_hooks();
  for (unsigned flags = 0; flags < 64; ++flags) {
    header_prepare();
    uint32_t args[] = {1, flags, 0};
    apply_header(3, args);
    require(seen(0x16665) == (flags < 32 && (flags & 1) ? 1 : 0),
            "global hard reset precedence");
    require(seen(0x16220) ==
                (flags < 32 && !(flags & 1) && (flags & 2) ? 1 : 0),
            "global soft reset precedence");
    require(seen(0x3e41a) == (flags < 32 && (flags & 16) ? 1 : 0),
            "global refresh deletion flag");
    require(u16(viewport + 0x1c) == (flags < 32 && (flags & 4) ? 1 : 0),
            "global input lock flag");
    require(seen(0x394fe) == (flags < 32 && (flags & 8) ? 1 : 0),
            "global table-zero switch flag");
    require(u16(viewport + 0x1a) == (flags < 32 ? 1 : 0),
            "global invalid flags do not change revision");
  }
  static const unsigned consumers[] = {0x16bf4, 0x16d58, 0x16ec1,
                                       0x170c8, 0x172c5, 0x173f5,
                                       0x17658, 0x176dd, 0x176ef};
  static const unsigned maxima[] = {255,   255, 65535, 524287, 255,
                                    65535, 31,  1,     15};
  for (unsigned type = 0; type < 9; ++type)
    for (unsigned bit = 0; bit < 19; ++bit) {
      if ((1u << bit) > maxima[type])
        continue;
      header_prepare();
      /* Original segment control flow; leaf operations recorded above. */
      if (type < 5)
        ((void (*)(void *, unsigned, unsigned))(image + consumers[type]))(
            context, 1u << bit, 3);
      else if (type == 5)
        ((void (*)(void *, unsigned, unsigned, unsigned, unsigned))(
            image + consumers[type]))(context, 1u << bit, 3, 2, 8);
      else
        ((void (*)(void *, unsigned))(image + consumers[type]))(context,
                                                                1u << bit);
      require(error_message == NULL, "segment single-bit effects execute");
      if (type < 6) {
        unsigned shift = type == 2 ? 1 : 0;
        unsigned backup_type = type == 5 ? 7 : type;
        if (bit == 3 + shift || bit == 4 + shift) {
          require(seen(0x23d6e) == 1,
                  "segment has one saved-area deletion call");
          unsigned found = 0;
          for (unsigned i = 0; i < event_count; ++i)
            found |= events[i].rva == 0x23d6e &&
                     events[i].a == (int)backup_type &&
                     events[i].b == (bit == 3 + shift ? -1 : -2);
          require(found,
                  "base versus numbered-save deletion and object family");
        }
        if (bit == 5 + shift)
          require(seen(0x24648) == 1, "segment stack-clear action");
      }
    }
  header_prepare();
  ((void (*)(void *, unsigned, unsigned))(image + 0x172c5))(context, 0x40, 3);
  require(seen(0x3e2e7) == 1 && palette_table[4] == 2,
          "palette flag 64 calls current upload service; leaf semantics "
          "checked by table probe");
  header_prepare();
  execute_headers = 1;
  parse("!|h01000001500G0230|L001002003004\r\n");
  require(
      calls == 2 && captured_count == 4 && captured[0] == 1 &&
          captured[1] == 2 && captured[2] == 3 && captured[3] == 4,
      "header coordinate setting changes following original parser command");
  header_prepare();
  parse("!|h0100000150040220|q0a\r\n");
  require(calls == 2 && captured_slot == 55 && captured[0] == 36,
          "header radix setting changes following parser command");
  header_prepare();
  parse("!|h01000201502O2220\r\n");
  require(calls == 1 && parser[0x38] == 36 && parser[0x39] == 2 &&
              u32(viewport + 0x24) == 1280 && u32(viewport + 0x28) == 960,
          "constructed header initializes extended scene environment");
  execute_headers = 0;
  parse("!|h01000002900200060\r\n");
  require(calls == 1 && captured_count == 8 && valid(8, captured),
          "misaligned charset and port segments can pass validator");
  header_prepare();
  apply_header(captured_count, captured);
  require(viewport[0x10] == 0 && !seen(0x3393c),
          "misaligned charset sequence applies charset then mouse rather than "
          "port");
  header_prepare();
  protection = 1;
  ((void (*)(void *, unsigned, unsigned))(image + 0x16d58))(context, 0x40, 3);
  require(style[2 * 0x61 + 0x21] == 0,
          "protected style current reset skips copying defaults");
  protection = 0;
  ((void (*)(void *, unsigned, unsigned))(image + 0x16d58))(context, 0x40, 3);
  require(style[2 * 0x61 + 0x21] & 2,
          "unprotected style current reset copies defaults");
  header_prepare();
  protection = 1;
  ((void (*)(void *, unsigned, unsigned, unsigned, unsigned))(image + 0x173f5))(
      context, 0x500, 3, 4, 6);
  require(parser[0x38] == 64 && parser[0x39] == 4,
          "environment direct setters apply even while protected");
  header_prepare();
  ((void (*)(void *, unsigned, unsigned, unsigned, unsigned))(image + 0x173f5))(
      context, 0x1400, 3, 63, 63);
  require(parser[0x39] != 63 && parser[0x3a] != 63,
          "invalid environment values silently rejected by downstream setters");
  backup_checks();
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
  const char *continued[] = {"!|h01\\\r\n0000010010\r\n",
                             "!|h01000001\\\r\n0010\r\n",
                             "!|h0100000100\\\r\n10\r\n"};
  for (unsigned i = 0; i < 3; ++i) {
    parse(continued[i]);
    require(calls == 1 && valid(captured_count, captured),
            "header continuation within or between segments");
  }
  char counted[100];
  strcpy(counted, "!|h0100000A");
  for (unsigned i = 0; i < 10; ++i)
    strcat(counted, "0000");
  strcat(counted, "\r\n");
  parse(counted);
  require(calls == 1 && valid(captured_count, captured),
          "ten declared segments accepted");
  strcpy(counted, "!|h0100000B");
  for (unsigned i = 0; i < 11; ++i)
    strcat(counted, "0000");
  strcat(counted, "\r\n");
  parse(counted);
  require(calls == 1 && !valid(captured_count, captured),
          "eleven declared segments rejected");
  entry_checks();
  header_checks();
  printf("PASS: %u header/parser/entry checks\n", checks);
  return 0;
}
