/* Original RIPtel framing, ANSI autosense and buffer routing with controlled
 * drawing/log/output services. No GUI, network or filesystem mutation. */
#include "riptel-probe-image.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static unsigned char context[512], parser[256], api[32], callbacks[512],
    options[0x400], configuration[0x400], ansi[128], text_windows[32],
    window_entries[36 * 0xf1], variables[0x700];
static uint16_t ansi_args[24], character_types[256];
static uint32_t arguments[8194];
static char string_buffer[2048], raw[4096], reply[4096], tails[64][2048];
static unsigned slots[64], counts[64], calls, replies, raw_count, cursor_bol;
static unsigned zero(void) { return 0; }
static unsigned begin_line(void *ctx) {
  (void)ctx;
  return cursor_bol;
}
static void output(void *ctx, const char *text) {
  (void)ctx;
  ++replies;
  require(strlen(reply) + strlen(text) < sizeof reply, "reply capture bound");
  strcat(reply, text);
}
static void formatted(char *dst, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsprintf(dst, fmt, args);
  va_end(args);
}
static void capture(void *ctx, unsigned slot, unsigned count, uint32_t *args,
                    const char *tail) {
  (void)ctx;
  (void)args;
  require(calls < 64, "dispatch capture bound");
  slots[calls] = slot;
  counts[calls] = count;
  snprintf(tails[calls], sizeof tails[calls], "%s", tail);
  ++calls;
}
static void raw_route(void *ctx, unsigned ch) {
  /* The production text router may also intercept column flow and draw text.
   * This fixture has neither; preserve original ANSI parsing and dispatch. */
  unsigned consumed =
      ((unsigned (*)(void *, unsigned))(image + 0x37b7c))(ctx, ch & 255) &
      0xffff;
  if (!consumed) {
    require(raw_count + 1 < sizeof raw, "raw capture bound");
    raw[raw_count++] = ch;
    raw[raw_count] = 0;
  }
}
static void prepare(void) {
  memset(context, 0, sizeof context);
  memset(parser, 0, sizeof parser);
  memset(api, 0, sizeof api);
  memset(callbacks, 0, sizeof callbacks);
  memset(options, 0, sizeof options);
  memset(configuration, 0, sizeof configuration);
  memset(ansi, 0, sizeof ansi);
  memset(ansi_args, 0, sizeof ansi_args);
  memset(variables, 0, sizeof variables);
  put16(context, 0x9df3);
  pointer(context + 2, parser);
  pointer(context + 6, api);
  pointer(context + 0x12, text_windows);
  pointer(context + 0x26, variables);
  pointer(context + 0x36, options);
  pointer(context + 0x46, ansi);
  put32(context + 0x5e, 1);
  pointer(api, configuration);
  pointer(api + 4, callbacks);
  configuration[0x300] = 1;
  pointer(callbacks + 0x120, (void *)begin_line);
  pointer(text_windows, window_entries);
  pointer(ansi, ansi_args);
  pointer(parser + 0xa1, string_buffer);
  pointer(parser + 0xa5, arguments);
  ((void (*)(void *))(image + 0x39d20))(parser);
  parser[0x38] = 36;
  parser[0x39] = 2;
  parser[0x9f] = parser[0xa0] = 13;
  pointer(parser + 0xaa, (void *)capture);
  pointer(parser + 0xb2, context);
  calls = replies = raw_count = cursor_bol = 0;
  raw[0] = reply[0] = 0;
}
static void feed(const char *wire) {
  unsigned length = strlen(wire);
  unsigned used = ((unsigned (*)(void *, const char *, unsigned))(
      image + 0x57a2))(context, wire, length);
  require(used == length, "original buffer consumer consumes fixture");
}
static void equal(const char *actual, const char *expected, const char *why) {
  if (strcmp(actual, expected)) {
    fprintf(stderr, "%s: got [%s], expected [%s]\n", why, actual, expected);
    exit(1);
  }
  require(1, why);
}
static void version_checks(void) {
  prepare();
  feed("\033[!");
  equal(reply, "RIPSCRIP030001", "bare autosense exact bytes, no CR/LF");
  require(replies == 1, "single version response");
  feed("\033[0!");
  equal(reply, "RIPSCRIP030001RIPSCRIP030001", "selector zero response");
  reply[0] = 0;
  feed("\033[3!");
  equal(reply, "RIPSCRIP030001\r", "selector three adds one engine CR");
  reply[0] = 0;
  feed("\033[4!\033[?0!");
  require(reply[0] == 0, "unknown and private selectors do not reply");
  feed("\033[0;2!");
  equal(reply, "RIPSCRIP030001", "autosense uses first parameter only");
  reply[0] = 0;
  feed("\033[65536!");
  equal(reply, "RIPSCRIP030001", "ANSI selector accumulation wraps to 16 bits");
  const char *value =
      ((const char *(*)(void *, unsigned, void *))(image + 0x4b6c1))(context, 0,
                                                                     NULL);
  equal(value, "RIPSCRIP030001", "RIPVER same literal without terminator");
  value = ((const char *(*)(void *, unsigned, void *))(image + 0x4b6c1))(
      context, 1, NULL);
  require(!value && u16(variables + 0x436), "RIPVER rejects arguments");
  prepare();
  feed("\033[1!\033[!\033[0!\033[3!");
  require(configuration[0x300] == 0 && replies == 0,
          "disable suppresses version query responses");
  feed("!|c01\r\n");
  require(calls == 0, "disabled RIP command stays on text path");
  equal(raw, "!|c01\r\n", "disabled command is delivered as raw text");
  feed("\033[2!\r\n!|c02\r\n\033[!");
  require(configuration[0x300] == 1 && calls == 1 && slots[0] == 22,
          "enable resumes command processing");
  equal(reply, "RIPSCRIP030001", "re-enabled version query responds");
  prepare();
  feed("\033[1!\033[2!\001|c01\r\n");
  require(configuration[0x300] == 1 && calls == 1 && slots[0] == 22,
          "same-buffer enable takes effect before the next SOH command");
  prepare();
  feed("!|TBEFORE");
  ((unsigned (*)(void *, int))(image + 0x31420))(context, 0);
  feed("IGNORED\033[2!AFTER\r\n");
  require(calls == 1 && slots[0] == 60,
          "API disable and ANSI enable preserve pending RIP parser state");
  equal(tails[0], "BEFOREAFTER",
        "resumption continues pending text, omitting disabled interval");
}
static void framing_checks(void) {
  prepare();
  feed("abc!|c01\r\n");
  require(calls == 0, "ordinary mid-line exclamation is text");
  prepare();
  feed("abc\001|c01\r\n");
  require(calls == 1, "SOH starts RIP mid-line");
  prepare();
  feed("abc\002|c01\r\n");
  require(calls == 1, "STX starts RIP mid-line");
  prepare();
  cursor_bol = 1;
  feed("abc!|c01\r\n");
  require(calls == 1, "client beginning-of-line predicate admits exclamation");
  for (unsigned depth = 1; depth <= 10; ++depth) {
    char wire[32] = "!|";
    memset(wire + 2, '1', depth);
    wire[depth + 2] = 0;
    prepare();
    feed(wire);
    require(u32(parser + 0xc) == (depth <= 9 ? depth : 9),
            "level-prefix count across 1..10 boundary");
    require(u32(parser) == (depth <= 9 ? 2 : 12),
            "level-prefix state across 1..10 boundary");
  }
  prepare();
  feed("!|123");
  require(u32(parser + 0xc) == 3 && !memcmp(parser + 0x20, "123", 3),
          "third digit remains a level digit");
  feed("456789");
  require(u32(parser + 0xc) == 9 && u32(parser) == 2,
          "nine level digits remain in prefix state");
  feed("1");
  require(u32(parser) == 12, "tenth digit enters discard state");
  feed("L00000000|c01\r\n");
  require(calls == 0,
          "stale second prefix digit suppresses following level zero");
  feed("!|1K|c01\r\n");
  require(calls == 2,
          "one-digit prefix clears stale byte and restores dispatch");
  prepare();
  feed("!|123L00000000|c01\r\n");
  require(calls == 0,
          "unknown deep level is discarded and leaves stale prefix byte");
  prepare();
  feed("!|0c01|c02\r\n");
  require(calls == 1, "zero is opcode, not level digit");
  prepare();
  feed("!|T\\$SHIFT\\$-\\[-\\(-\\!-\\|-\\\\\r\n");
  require(calls == 1, "escaped string dispatches once");
  equal(tails[0], "\\$SHIFT\\$-\\[-\\(-\\!-|-\\\\",
        "framing retains host quoting, strips pipe escape only");
  prepare();
  feed("!|Tabc\\\r\ndef\r\n");
  require(calls == 1, "continued string dispatches once");
  equal(tails[0], "abcdef", "framing continuation removes slash and CRLF");
  prepare();
  feed("!|Tleft\nright\r\n");
  require(calls == 1 && slots[0] == 60, "bare LF terminates RIP text command");
  equal(tails[0], "left",
        "LF does not concatenate the following physical line");
  equal(raw, "right\r\n", "next physical line becomes raw text");
  prepare();
  feed("!|Tleft\fright\r\n");
  require(calls == 1 && slots[0] == 60,
          "form feed terminates RIP text command");
  equal(tails[0], "left", "form feed is a RIP framing terminator");
}
static void priority_checks(void) {
  prepare();
  feed("!|Tbefore\033[!after\r\n");
  require(calls == 1 && replies == 0, "ESC inside RIP string stays RIP data");
  equal(tails[0], "before\033[!after", "pending RIP string retains ESC bytes");
  prepare();
  feed("!|L00\033[!|c01\r\n");
  require(calls == 1 && replies == 0,
          "ESC corrupts unfinished numeric command without autosense");
  prepare();
  feed("!\033[!");
  equal(reply, "RIPSCRIP030001", "fallback from introducer routes ESC to ANSI");
  prepare();
  feed("\033\033[!");
  require(replies == 0, "ESC in pending ESC sequence is not a restart");
  prepare();
  feed("\033[12\033[!");
  require(replies == 0, "ESC does not globally restart unfinished ANSI CSI");
  feed("\033[!");
  equal(reply, "RIPSCRIP030001", "fresh idle ANSI sequence responds");
  prepare();
  feed("\033[12\030\033[!");
  equal(reply, "RIPSCRIP030001", "CAN cancels pending ANSI before fresh query");
  prepare();
  feed("\033[12\032\033[!");
  equal(reply, "RIPSCRIP030001", "SUB cancels pending ANSI before fresh query");
  prepare();
  feed("\033[12\001|c01\r\n");
  require(calls == 1 && u16(ansi + 0xc) == 2,
          "SOH starts RIP without discarding pending ANSI state");
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  hook(0x18cf6, noop);
  hook(0x18bd0, (void (*)(void))zero);
  hook(0x18bf7, noop);
  hook(0x320f2, noop);
  hook(0x4d2d0, (void (*)(void))raw_route);
  hook(0x6947, (void (*)(void))output);
  hook(0x68440, (void (*)(void))formatted);
  pointer(image + 0x87908, character_types);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isspace(i) ? 8 : 0) | (isdigit(i) ? 4 : 0) |
                         (isupper(i) ? 1 : 0) | (islower(i) ? 2 : 0);
  version_checks();
  framing_checks();
  priority_checks();
  printf("PASS: %u framing/identification checks\n", checks);
  return 0;
}
