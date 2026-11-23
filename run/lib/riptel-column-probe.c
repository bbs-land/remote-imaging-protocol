/* Original RIPtel column/article routines with controlled host services.
 * No Wine, X server or desktop access. The wrapper checks the complete DLL
 * hash. Machine-code hooks are listed in install_hooks(), flow_checks(), and
 * the explicitly marked macro-lookup check in main(). */
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
static unsigned closes, dialogs, flushes;
static unsigned char context[256], article_system[0x1a], state[0x2285];
static unsigned char macro_system[0x700], style_table[32], style[0x61];
static char scratch_buffer[8192], result_buffer[8192];
static uint16_t character_types[256];
static const char *error_message, *dialog_message;
#include "riptel-probe-image.h"
static unsigned zero(void) { return 0; }
static void *scratch(void *ctx) {
  (void)ctx;
  return scratch_buffer;
}
static void report(void *ctx, unsigned level, const char *name,
                   const char *message) {
  (void)ctx;
  (void)level;
  (void)name;
  error_message = message;
}
static void close_file(void *stream) {
  require(stream != NULL, "non-null close");
  ++closes;
}
static unsigned __attribute__((stdcall)) message_box(unsigned hwnd,
                                                     const char *message,
                                                     const char *title,
                                                     unsigned flags) {
  (void)hwnd;
  (void)title;
  (void)flags;
  ++dialogs;
  dialog_message = message;
  return 1;
}
static void __attribute__((stdcall)) set_rect(int32_t *rect, int x0, int y0,
                                              int x1, int y1) {
  rect[0] = x0;
  rect[1] = y0;
  rect[2] = x1;
  rect[3] = y1;
}
static void coordinates(void *ctx, uint16_t *x, uint16_t *y) {
  (void)ctx;
  (void)x;
  (void)y;
}
static void flush(void) { ++flushes; }
static void formatted(char *target, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vsprintf(target, format, args);
  va_end(args);
}
static void prepare(void);
static const char *overflow(unsigned, const char *, const char *, const char *);
static void equal(const char *, const char *, const char *);
static unsigned char *article(unsigned);
struct memory_file {
  char name[64], text[8192];
  unsigned length;
};
static struct memory_file files[32];
static unsigned file_count, render_count;
static char rendered[8192];
static unsigned char pending_code[5];
static void *open_file(const char *name, const char *mode) {
  require(!strcmp(mode, "wb"), "overflow opens binary output");
  unsigned i;
  for (i = 0; i < file_count; ++i)
    if (!strcmp(files[i].name, name))
      break;
  require(i < 32, "memory file limit");
  if (i == file_count)
    ++file_count;
  snprintf(files[i].name, sizeof files[i].name, "%s", name);
  files[i].length = 0;
  files[i].text[0] = 0;
  return &files[i];
}
static void put_byte(unsigned byte, struct memory_file *file) {
  require(file >= files && file < files + file_count, "known memory output");
  require(file->length + 1 < sizeof file->text, "memory output limit");
  file->text[file->length++] = (char)byte;
  file->text[file->length] = 0;
}
static unsigned measure(void *ctx, const char *text) {
  (void)ctx;
  return strlen(text);
}
static void metrics(void *ctx) {
  (void)ctx;
  put16(state + 1, 10);
  put16(state + 3, 2);
  put16(state + 5, 8);
  put16(state + 7, 1);
  for (unsigned i = 0; i < 256; ++i)
    put16(state + 0x2082 + 2 * i, 1);
  state[0xa] = 0;
}
static void draw(void *ctx, unsigned x, unsigned y, const char *text) {
  (void)ctx;
  (void)x;
  (void)y;
  ++render_count;
  require(strlen(rendered) + strlen(text) + 2 < sizeof rendered,
          "render capture bound");
  strcat(rendered, text);
  strcat(rendered, " ");
}
static void draw_color(void *ctx, unsigned x, unsigned y, unsigned color,
                       const char *text) {
  (void)color;
  draw(ctx, x, y, text);
}
static void flow_begin(unsigned col, unsigned art, unsigned flags) {
  uint32_t args[] = {0, 0, 9, 19, col, art, flags, 0, 0};
  ((void (*)(void *, unsigned, uint32_t *, const char *))(image + 0xa5ed))(
      context, 9, args, "");
}
static void feed(const char *text) {
  for (; *text; ++text)
    require((((unsigned (*)(void *, unsigned))(image + 0x3ce24))(
                 context, (unsigned char)*text) &
             0xffff) == 1,
            "raw byte consumed");
  ((void (*)(void *))(image + 0x3d488))(context);
}
static void flow_checks(void) {
  memcpy(image + 0x3cb80, pending_code, 5);
  hook(0x3bcb4, (void (*)(void))metrics); /* deterministic font dimensions */
  hook(0x3bb7e, noop);                    /* font/graphics setup */
  hook(0x3b471, (void (*)(void))draw_color);
  hook(0x3b2fb, (void (*)(void))draw);
  hook(0x3ae10, (void (*)(void))draw_color);
  hook(0x3fcd0, (void (*)(void))measure);
  hook(0x68918, (void (*)(void))strcat);
  hook(0x68f90, (void (*)(void))open_file);
  hook(0x6c400, (void (*)(void))put_byte);
  hook(0x3f776,
       noop); /* prepend output directory, identity for memory filenames */
  hook(0x18e92, noop); /* register generated file with host */
  prepare();
  style[0x24] = 1;
  file_count = render_count = 0;
  rendered[0] = 0;
  flow_begin(0, 1, 1);
  const char *text =
      "aaa bbb ccc ddd eee fff ggg hhh iii jjj kkk lll mmm nnn ooo ppp";
  feed(text);
  unsigned char *a = article(1);
  require(u32(a + 0x14) == 0 && u32(a + 0x18) == 1 && file_count == 2 &&
              render_count == 2,
          "first page produces only one remainder");
  equal(files[0].name, "00710000.OVR", "source page filename");
  equal(files[0].text, text, "page zero preserves complete source");
  equal(files[1].text, text + 8, "page one stores undisplayed tail");
  for (unsigned page = 1; page < 8; ++page) {
    equal(overflow(2, "1", "NEXT", NULL), "", "advance for replay");
    char name[64], content[8192];
    snprintf(name, sizeof name, "0071%04u.OVR", page);
    equal(overflow(2, "1", "CUR", NULL), name, "replay selects remainder");
    require(page < file_count, "requested page exists");
    snprintf(content, sizeof content, "%s", files[page].text);
    flow_begin(0, 1, 1);
    feed(content);
    require(u32(a + 0x14) == page, "template replay retains current page");
    require(u32(a + 0x18) == (page < 7 ? page + 1 : 7),
            "replay discovers at most one next page");
  }
  require(file_count == 8 && render_count == 15,
          "eight pages rendered fifteen text runs");
  equal(rendered,
        "aaa bbb ccc ddd eee fff ggg hhh iii jjj kkk lll mmm nnn ooo ppp ",
        "replay retains every word in order");
  require(overflow(3, "1", "NEXT", "SETVERBOSE") == NULL && dialogs == 1 &&
              u32(a + 0x14) == 7,
          "last replayed page is navigation boundary");
  equal(overflow(2, "1", "0", NULL), "", "select first retained source");
  flow_begin(0, 1, 1);
  feed(text);
  require(file_count == 8 && u32(a + 0x18) == 1,
          "replaying earlier page replaces known frontier");
  ((void (*)(void *, unsigned, void *))(image + 0x43a17))(context, 1, NULL);
  require(file_count == 8 && !u32(a + 0x18) && !a[0x13],
          "global reset forgets state without unlinking files");
  prepare();
  style[0x24] = 1;
  file_count = render_count = 0;
  rendered[0] = 0;
  flow_begin(0, 1, 0);
  feed(text);
  require(file_count == 0 && render_count == 2,
          "overflow off discards remaining text");
  prepare();
  style[0x24] = 1;
  file_count = render_count = 0;
  rendered[0] = 0;
  flow_begin(0, 1, 1);
  const char *pending = "aaa";
  for (const char *p = pending; *p; ++p)
    ((void (*)(void *, unsigned))(image + 0x3ce24))(context, *p);
  require(u16(state + 0x1c79) == 3 && render_count == 0,
          "text is pending before redefine");
  flow_begin(0, 1, 1);
  require(!u16(state + 0x1c79) && render_count == 0,
          "same article redefine drops pending text");
  for (const char *p = pending; *p; ++p)
    ((void (*)(void *, unsigned))(image + 0x3ce24))(context, *p);
  flow_begin(1, 2, 1);
  require(render_count == 1 && state[0] == 2,
          "article change flushes old pending text");
}
static void install_hooks(void) {
  hook(0x45038, noop);                   /* context/UI synchronization */
  hook(0x3445b, (void (*)(void))zero);   /* port protection false */
  hook(0x38e47, (void (*)(void))report); /* error sink */
  hook(0x15396, (void (*)(void))scratch);
  hook(0x153fe, noop); /* scratch ownership */
  hook(0x4b0e4, noop); /* macro expansion, identity for these literal inputs */
  hook(0x3988f, noop);
  hook(0x398ee, noop); /* context locks */
  hook(0x310b8,
       (void (*)(void))coordinates); /* identity world/device conversion */
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x654a8, (void (*)(void))strlen);
  hook(0x68dd0, (void (*)(void))strlen);
  hook(0x655e2, (void (*)(void))strcasecmp);
  hook(0x65604, (void (*)(void))atoi);
  hook(0x68cc0, (void (*)(void))atol);
  hook(0x68440, (void (*)(void))formatted);
  hook(0x68d70, (void (*)(void))memset);
  hook(0x68ec0, (void (*)(void))close_file);
  memcpy(pending_code, image + 0x3cb80, 5);
  hook(0x3cb80,
       (void (*)(void))flush); /* pending line sink, tested separately later */
  pointer(image + 0x96618, (void *)set_rect);
  pointer(image + 0x966f4, (void *)message_box);
  pointer(image + 0x87908, character_types);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isdigit(i) ? 4 : 0) | (isspace(i) ? 8 : 0);
}
static unsigned char *article(unsigned n) { return state + n * 0x18; }
static unsigned char *column(unsigned n) { return state + 0x36d + n * 0xb2; }
static void prepare(void) {
  memset(context, 0, sizeof context);
  memset(article_system, 0, sizeof article_system);
  memset(state, 0, sizeof state);
  memset(macro_system, 0, sizeof macro_system);
  memset(style_table, 0, sizeof style_table);
  memset(style, 0, sizeof style);
  pointer(context + 0x2a, article_system);
  pointer(article_system + 0x16, state);
  pointer(context + 0x26, macro_system);
  pointer(macro_system + 0x432, result_buffer);
  pointer(context + 0xa, style_table);
  pointer(style_table, style);
  put16(context + 0x7a, 7);
  ((void (*)(void *, unsigned))(image + 0x3c1cf))(context, 1);
  error_message = dialog_message = NULL;
  closes = dialogs = flushes = 0;
}
static void begin(unsigned col, unsigned art, unsigned flags, unsigned color,
                  const char *words) {
  uint32_t args[] = {10, 20, 109, 119, col, art, flags, color, 0xdeadbeef};
  error_message = NULL;
  ((void (*)(void *, unsigned, uint32_t *, const char *))(image + 0xa5ed))(
      context, 9, args, words);
}
static const char *overflow(unsigned count, const char *art,
                            const char *direction, const char *mode) {
  char args[3][21] = {{0}};
  if (art)
    snprintf(args[0], 21, "%s", art);
  if (direction)
    snprintf(args[1], 21, "%s", direction);
  if (mode)
    snprintf(args[2], 21, "%s", mode);
  error_message = dialog_message = NULL;
  dialogs = 0;
  put16(macro_system + 0x436, 0);
  put32(macro_system + 0x438, 0);
  return ((const char *(*)(void *, unsigned, void *))(image + 0x28854))(
      context, count, args);
}
static void equal(const char *actual, const char *expected, const char *label) {
  require(actual && !strcmp(actual, expected), label);
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  install_hooks();
  prepare();
  for (unsigned col = 0; col < 64; ++col)
    for (unsigned art = 0; art < 64; ++art) {
      begin(col, art, 0, 4095, "");
      require((error_message == NULL) == (col < 36 && art > 0 && art < 36),
              "1e column/article range");
      if (!error_message)
        require(column(col)[1] == art, "column ownership");
    }
  prepare();
  for (unsigned flags = 0; flags < 4096; ++flags) {
    begin(0, 1, flags, 255, "alpha beta");
    require((error_message == NULL) == (flags <= 3), "flags accept only 0..3");
    if (!error_message) {
      require(u16(column(0) + 0x26) == flags, "stored flags");
      require(article(1)[0x13] == (flags & 1), "overflow flag per article");
      require(column(0)[0x28] == ((flags & 2) ? 2 : 0), "search word count");
      require(column(0)[0x29] == ((flags & 2) ? 255 : 0), "search color");
    }
  }
  for (unsigned color = 0; color < 4096; ++color) {
    begin(0, 1, 2, color, "alpha");
    require((error_message == NULL) == (color <= 255),
            "highlight color 0..255");
    begin(0, 1, 0, color, "");
    require(error_message == NULL, "color ignored without highlight");
  }
  prepare();
  begin(8, 1, 3, 12, "alpha beta");
  require(!error_message, "search list accepted");
  equal((char *)column(8) + 0x2a, "alpha", "first search word");
  equal((char *)column(8) + 0x3b, "beta", "second search word");
  unsigned char *a = article(1);
  a[0xf] = 8;
  require(((unsigned (*)(void *, const char *))(image + 0x3ad73))(context,
                                                                  "ALPHA") == 1,
          "highlight case insensitive");
  require(((unsigned (*)(void *, const char *))(image + 0x3ad73))(
              context, "alphabet") == 0,
          "highlight whole token");
  begin(8, 1, 3, 12, "");
  require(error_message != NULL, "empty search rejects");
  begin(8, 1, 3, 12, "abcdefghijklmnopq");
  require(error_message != NULL, "17-character search rejects");
  begin(8, 1, 3, 12, "   ");
  require(error_message != NULL, "whitespace-only search rejects");
  begin(8, 1, 3, 12, "a b c d e f g h i");
  require(error_message && column(8)[0x28] == 8,
          "ninth word reports and keeps first eight");
  prepare();
  begin(9, 1, 1, 0, "");
  begin(3, 2, 1, 0, "");
  begin(7, 1, 1, 0, "");
  a = article(1);
  require(state[9] == 3, "global column count");
  ((void (*)(void *, unsigned))(image + 0x3c399))(context, 1);
  require((((unsigned (*)(void *))(image + 0x3ba81))(context) & 0xffff) == 7,
          "flow first lowest matching column");
  a[0xf] = 7;
  require((((unsigned (*)(void *))(image + 0x3ba81))(context) & 0xffff) == 9,
          "flow next skips other article");
  a[0xf] = 9;
  require((((unsigned (*)(void *))(image + 0x3ba81))(context) & 0xffff) == 36,
          "flow end sentinel");
  put32(a + 0x14, 2);
  put32(a + 0x18, 4);
  a[0x1c] = 1;
  a[0x12] = 1;
  pointer(a + 0x1d, (void *)0x1234);
  pointer(a + 0x21, (void *)0x5678);
  begin(7, 1, 1, 0, "");
  require(u32(a + 0x14) == 2 && u32(a + 0x18) == 4,
          "redefine preserves page counters");
  require(a[0xf] == 255 && !u32(a + 0x1d) && !u32(a + 0x21) && closes == 2,
          "redefine restarts column and closes streams");
  require(a[0x1c] && a[0x12] && state[9] == 3,
          "redefine preserves other article flags and count");
  begin(7, 2, 0, 0, "");
  require(column(7)[1] == 2 && state[9] == 3, "reassign global column");
  prepare();
  begin(0, 1, 1, 0, "");
  a = article(1);
  put32(a + 0x18, 2);
  equal(overflow(2, "1", "PAGE", NULL), "1", "page display is current+1");
  equal(overflow(2, "1", "CUR", NULL), "00710000.OVR", "current page filename");
  equal(overflow(3, "1", "NEXT", "QUERY"), "00710001.OVR",
        "next query filename");
  require(u32(a + 0x14) == 0, "query does not move");
  equal(overflow(2, "1", "NEXT", NULL), "", "next returns empty");
  require(u32(a + 0x14) == 1, "next advances");
  equal(overflow(3, "1", "PREV", "QUERY"), "00710002.OVR",
        "shipped prev query uses current+1");
  equal(overflow(2, "1", "PREV", NULL), "", "prev returns empty");
  require(u32(a + 0x14) == 0, "prev decrements");
  require(overflow(2, "1", "PREV", NULL) == NULL &&
              u32(macro_system + 0x438) == 1 && !dialogs,
          "prev at beginning sets abort");
  require(overflow(3, "1", "PREV", "SETVERBOSE") == NULL && dialogs == 1,
          "verbose beginning notifies");
  equal(dialog_message, "Beginning of document.",
        "beginning notification text");
  equal(overflow(2, "1", "2", NULL), "", "numeric selects zero-based page");
  require(u32(a + 0x14) == 2, "numeric page state");
  require(overflow(3, "1", "NEXT", "SETVERBOSE") == NULL && dialogs == 1 &&
              u32(macro_system + 0x438) == 1,
          "verbose end sets abort");
  equal(dialog_message, "End of document.", "end notification text");
  equal(overflow(3, "1", "NEXT", "QUERY"), "<NONE>", "next query at end");
  equal(overflow(2, "1", "RESET", NULL), "", "article reset returns empty");
  require(!a[0x13] && !u32(a + 0x14) && !u32(a + 0x18) && !a[0x1c] && !a[0x12],
          "article reset counters and flags");
  require(column(0)[0] && state[0] == 1,
          "article reset retains definitions and selection");
  equal(overflow(2, "1", "CUR", NULL), "<NONE>", "inactive current filename");
  equal(overflow(2, "1", "PAGE", NULL), "1", "inactive displayed page");
  require(overflow(2, "0", "CUR", NULL) == NULL && error_message &&
              u16(macro_system + 0x436) == 1,
          "article zero macro rejects");
  require(overflow(2, "36", "CUR", NULL) == NULL && error_message,
          "article 36 macro rejects");
  begin(0, 1, 1, 0, "");
  put32(a + 0x14, 2);
  put32(a + 0x18, 4);
  ((void (*)(void *, unsigned))(image + 0x3c1cf))(context, 0);
  require(!column(0)[0] && !state[9] && !state[0] && !a[0x13] &&
              u32(a + 0x14) == 2 && u32(a + 0x18) == 4,
          "layout reset retains counters");
  ((void (*)(void *, unsigned, void *))(image + 0x43a17))(context, 1, NULL);
  require(!u32(a + 0x14) && !u32(a + 0x18), "RESET OVERFLOW clears counters");
  prepare();
  for (unsigned col = 0; col < 64; ++col)
    for (unsigned art = 0; art < 64; ++art) {
      uint32_t args[] = {0, 0, 9, 19, col, art};
      error_message = NULL;
      ((void (*)(void *, unsigned, uint32_t *, const char *))(image + 0xa529))(
          context, 6, args, NULL);
      require((error_message == NULL) == (col < 36 && art < 36),
              "1T identifier domains include article zero");
    }
  prepare();
  begin(0, 1, 1, 0, "");
  a = article(1);
  equal(overflow(0, NULL, NULL, NULL), "00710000.OVR",
        "no parameters uses selected article");
  equal(overflow(1, "CUR", NULL, NULL), "00710000.OVR", "CUR article selector");
  equal(overflow(1, "1", NULL, NULL), "00710000.OVR",
        "single article returns current filename");
  require(overflow(4, "1", "CUR", NULL) == NULL && error_message,
          "four parameters reject");
  require(overflow(3, "1", "CUR", "SET") == NULL && error_message,
          "CUR accepts only QUERY third arg");
  require(overflow(3, "1", "RESET", "QUERY") == NULL && error_message,
          "RESET accepts no third arg");
  require(overflow(3, "1", "NEXT", "invalid") == NULL && error_message,
          "unknown mode rejects");
  require(overflow(2, "1", "1", NULL) == NULL && error_message,
          "uncreated numeric page rejects");
  require(overflow(2, "1", "-1", NULL) == NULL && error_message,
          "signed numeric page rejects");
  equal(overflow(3, "1", "PREV", "QUERY"), "00710001.OVR",
        "PREV QUERY at beginning returns nonexistent next filename");
  put32(a + 0x18, 65536);
  equal(overflow(2, "1", "32768", NULL), "", "large numeric page accepted");
  require(u32(a + 0x14) == 0xffff8000,
          "numeric page stored as signed 16-bit value");
  equal(overflow(2, "1", "65536", NULL), "", "wrapping numeric page accepted");
  require(u32(a + 0x14) == 0, "numeric page assignment wraps sixteen bits");
  prepare();
  uint32_t legacy_zero[] = {0, 0, 9, 19, 0, 0};
  ((void (*)(void *, unsigned, uint32_t *, const char *))(image + 0xa529))(
      context, 6, legacy_zero, NULL);
  require(state[0] == 0 && state[0xb] == 1,
          "legacy article zero can be selected and marked active");
  ((void (*)(void *))(image + 0x3d488))(context);
  require(state[0xb] == 1 && (((unsigned (*)(void *, unsigned))(
                                  image + 0x3ce24))(context, 'a') &
                              0xffff) == 0,
          "article zero end/feed early exits");
  prepare();
  begin(0, 1, 1, 0, "");
  a = article(1);
  unsigned char parser[256] = {0};
  pointer(parser + 0x30, image + 0x80820 + 93 * 40);
  parser[0x38] = 36;
  require(((unsigned (*)(void *))(image + 0x39d70))(parser) == 1,
          "1e follows global base 36");
  parser[0x38] = 64;
  require(((unsigned (*)(void *))(image + 0x39d70))(parser) == 0,
          "1e follows global base 64");
  char reset_parameters[21] = "OVERFLOW";
  ((void (*)(void *, unsigned, void *))(image + 0x43d38))(context, 1,
                                                          reset_parameters);
  require(!state[0] && !column(0)[0] && !u32(a + 0x18),
          "global RESET keyword dispatch");
  begin(0, 1, 1, 0, "");
  a = article(1);
  unsigned char registration[10] = {0}, parsed[86] = {0};
  pointer(registration + 4, image + 0x28854);
  put16(registration + 8, 1);
  pointer(macro_system + 0x44e, registration);
  hook(0x25927, (void (*)(void))zero); /* sole controlled macro name lookup */
  strcpy((char *)parsed, "OVERFLOW");
  put16(parsed + 21, 2);
  strcpy((char *)parsed + 23, "1");
  strcpy((char *)parsed + 44, "PREV");
  put32(a + 0x14, 0);
  put32(a + 0x18, 0);
  const char *expanded = NULL;
  unsigned status = ((unsigned (*)(void *, void *, unsigned, const char **))(
      image + 0x25b21))(context, parsed, 1, &expanded);
  require((status & 0xffff) == 3, "boundary propagates macro abort status");
  strcpy((char *)parsed + 44, "CUR");
  status = ((unsigned (*)(void *, void *, unsigned, const char **))(
      image + 0x25b21))(context, parsed, 1, &expanded);
  require((status & 0xffff) == 1 && expanded &&
              !strcmp(expanded, "00710000.OVR"),
          "following evaluation clears prior abort");
  flow_checks();
  printf("PASS: %u column/article/overflow checks\n", checks);
  return 0;
}
