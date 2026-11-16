/* Original RIPtel column/article routines with controlled host services.
 * No Wine, X server or desktop access. The wrapper checks the complete DLL
 * hash. All machine-code hooks are listed in install_hooks(). */
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
static unsigned char *image;
static unsigned checks, closes, dialogs, flushes;
static unsigned char context[256], article_system[0x1a], state[0x2285];
static unsigned char macro_system[0x700], style_table[32], style[0x61];
static char scratch_buffer[8192], result_buffer[8192];
static uint16_t character_types[256];
static const char *error_message, *dialog_message;
static uint16_t u16(const void *p) {
  uint16_t v;
  memcpy(&v, p, 2);
  return v;
}
static uint32_t u32(const void *p) {
  uint32_t v;
  memcpy(&v, p, 4);
  return v;
}
static void put16(void *p, uint16_t v) { memcpy(p, &v, 2); }
static void put32(void *p, uint32_t v) { memcpy(p, &v, 4); }
static void pointer(void *p, void *v) { put32(p, (uint32_t)(uintptr_t)v); }
static void fail(const char *s) {
  fprintf(stderr, "%s: %s\n", s, strerror(errno));
  exit(1);
}
static void require(int ok, const char *label) {
  ++checks;
  if (!ok) {
    fprintf(stderr, "FAIL: %s\n", label);
    exit(1);
  }
}
static void noop(void) {}
static void hook(uint32_t rva, void (*fn)(void)) {
  image[rva] = 0xe9;
  put32(image + rva + 1,
        (uint32_t)(uintptr_t)fn - (uint32_t)(uintptr_t)(image + rva + 5));
}
static void load(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f)
    fail("open DLL");
  if (fseek(f, 0, SEEK_END))
    fail("seek DLL");
  long size = ftell(f);
  rewind(f);
  require(size == 592896, "DLL size");
  unsigned char *raw = malloc((size_t)size);
  if (!raw)
    fail("allocate file");
  require(fread(raw, 1, (size_t)size, f) == (size_t)size, "read DLL");
  fclose(f);
  uint32_t pe = u32(raw + 0x3c), opt = pe + 24;
  require(u16(raw + pe + 4) == 0x14c && u32(raw + opt + 28) == 0x10000000,
          "PE32 image identity");
  int flags = MAP_PRIVATE | MAP_ANON;
#ifdef __FreeBSD__
  flags |= MAP_FIXED | MAP_EXCL;
#elif defined(MAP_FIXED_NOREPLACE)
  flags |= MAP_FIXED_NOREPLACE;
#endif
  image = mmap((void *)0x10000000, 0xa1000, PROT_READ | PROT_WRITE | PROT_EXEC,
               flags, -1, 0);
  if (image == MAP_FAILED)
    fail("map driver");
  require(image == (void *)0x10000000, "preferred image base available");
  for (unsigned i = 0; i < u16(raw + pe + 6); ++i) {
    unsigned char *s = raw + opt + u16(raw + pe + 20) + 40 * i;
    uint32_t rva = u32(s + 12), length = u32(s + 16), offset = u32(s + 20);
    require((uint64_t)offset + length <= (unsigned long)size &&
                (uint64_t)rva + length <= 0xa1000,
            "section bounds");
    memcpy(image + rva, raw + offset, length);
  }
  free(raw);
}
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
  load(argv[1]);
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
  printf("PASS: %u column/article/overflow checks\n", checks);
  return 0;
}
