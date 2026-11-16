/* Original RIPtel variable/comparison routines with controlled host services.
 * No GUI. The wrapper verifies the supplied DLL before this maps its image. */
#include "riptel-probe-image.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
static unsigned char context[512], variables[0x700], ports[128], view[128],
    port_entries[36 * 0x78];
static char result[8192], params[3 * 21];
static uint16_t character_types[256];
struct registration {
  const char *name;
  unsigned callback, flags;
};
static struct registration registered[256];
static unsigned registered_count;
static unsigned allocate(void **target, unsigned size) {
  require(size == 0x668 || size == 0x1000, "known variable-engine allocation");
  *target = size == 0x668 ? (void *)variables : (void *)result;
  return 1;
}
static unsigned register_variable(void *ctx, const char *name, void *callback,
                                  unsigned flags, unsigned unused) {
  (void)ctx;
  (void)unused;
  require(registered_count < 256, "registration bound");
  registered[registered_count++] =
      (struct registration){name, (unsigned char *)callback - image, flags};
  return 0;
}
static unsigned registered_callback(const char *name) {
  for (unsigned i = 0; i < registered_count; ++i)
    if (!strcmp(registered[i].name, name))
      return registered[i].callback;
  return 0;
}
static unsigned random_value, key_state, errors, port_defined, modifier_calls;
static unsigned definition(void *ctx, unsigned slot) {
  (void)ctx;
  (void)slot;
  return port_defined;
}
static unsigned random_number(void) { return random_value; }
static unsigned __attribute__((stdcall)) async_key(unsigned key) {
  ++modifier_calls;
  require(key == 0x10 || key == 0x11, "modifier query key code");
  return key_state;
}
static void report(void *ctx, unsigned level, const char *where,
                   const char *message) {
  (void)ctx;
  (void)level;
  (void)where;
  (void)message;
  ++errors;
}
static void formatted(char *dst, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsprintf(dst, fmt, args);
  va_end(args);
}
static int decimal(const char *s) { return (int)strtol(s, NULL, 10); }
static void prepare(void) {
  memset(context, 0, sizeof context);
  memset(variables, 0, sizeof variables);
  memset(ports, 0, sizeof ports);
  memset(view, 0, sizeof view);
  memset(port_entries, 0, sizeof port_entries);
  port_defined = 1;
  pointer(ports, port_entries);
  pointer(context + 0x26, variables);
  pointer(variables + 0x432, result);
  pointer(context + 0x22, ports);
  pointer(context + 0xe, view);
  put32(view + 0x24, 1280);
  put32(view + 0x28, 960);
  put32(view + 0x2c, 640);
  put32(view + 0x30, 480);
  memset(params, 0, sizeof params);
  result[0] = 0;
  errors = 0;
}
static const char *invoke(unsigned rva, unsigned count) {
  put16(variables + 0x436, 0);
  return ((const char *(*)(void *, unsigned, void *))(image + rva))(
      context, count, params);
}
static void equal(const char *actual, const char *expected, const char *why) {
  require(actual && !strcmp(actual, expected), why);
}
static unsigned char registry[256 * 10], host_commands[0x1200];
static unsigned user_calls, playback_calls;
static char last_playback[512], fixture_result[4096];
static const char *fixture_name[16], *fixture_value[16];
static unsigned fixture_count;
static int registration_order(const void *a, const void *b) {
  return strcmp(((const struct registration *)a)->name,
                ((const struct registration *)b)->name);
}
static unsigned user_variable(void *ctx, void *parsed, const char **out) {
  (void)ctx;
  ++user_calls;
  for (unsigned i = 0; i < fixture_count; ++i)
    if (!strcasecmp((char *)parsed, fixture_name[i])) {
      snprintf(fixture_result, sizeof fixture_result, "%s", fixture_value[i]);
      *out = fixture_result;
      return 1;
    }
  fixture_result[0] = 0;
  *out = fixture_result;
  return 1;
}
static unsigned playback(void *ctx, const char *name, unsigned flags) {
  (void)ctx;
  (void)flags;
  ++playback_calls;
  snprintf(last_playback, sizeof last_playback, "%s", name);
  return 1;
}
static unsigned now(unsigned *value) {
  if (value)
    *value = 1000;
  return 1000;
}
static unsigned snapshot(void *ctx, void *xy) {
  (void)ctx;
  put32(xy, 297);
  put32((char *)xy + 4, 321);
  return 0;
}
static void release_scratch(void **p) {
  if (*p)
    free(*p);
  *p = NULL;
}
static unsigned scratch_allocate(void **p, unsigned size) {
  *p = calloc(1, size);
  return *p != NULL;
}
static void pipeline_prepare(void) {
  prepare();
  qsort(registered, registered_count, sizeof registered[0], registration_order);
  for (unsigned i = 0; i < registered_count; ++i) {
    pointer(registry + i * 10, (void *)registered[i].name);
    pointer(registry + i * 10 + 4, image + registered[i].callback);
    put16(registry + i * 10 + 8, registered[i].flags);
  }
  pointer(variables + 0x44e, registry);
  put16(variables + 0x44c, registered_count);
  memset(host_commands, 0, sizeof host_commands);
  pointer(context + 0x2e, host_commands);
  hook(0xf16b, (void (*)(void))user_variable);
  hook(0x65453, (void (*)(void))strchr);
  hook(0x65493, (void (*)(void))strncpy);
  hook(0x655f3, (void (*)(void))strcmp);
  hook(0x65965, (void (*)(void))strstr);
  hook(0x68918, (void (*)(void))strcat);
  hook(0x68dd0, (void (*)(void))strlen);
  hook(0x75940, (void (*)(void))strncasecmp);
  hook(0x6bcb0, (void (*)(void))memcpy);
  hook(0x6bb20, (void (*)(void))isalnum);
  hook(0x68b20, (void (*)(void))toupper);
  hook(0x65ca3, (void (*)(void))scratch_allocate);
  hook(0x65cbf, (void (*)(void))release_scratch);
  hook(0x6f230, noop);
  hook(0x68ff0, (void (*)(void))now);
  hook(0x69c90, (void (*)(void))now);
  hook(0x2814d, (void (*)(void))snapshot);
  hook(0x324a, (void (*)(void))playback);
  user_calls = playback_calls = modifier_calls = fixture_count = 0;
  last_playback[0] = 0;
}
static void fixture(const char *name, const char *value) {
  require(fixture_count < 16, "fixture bound");
  fixture_name[fixture_count] = name;
  fixture_value[fixture_count++] = value;
}
static unsigned preprocess(char *text) {
  return ((unsigned (*)(void *, char *, unsigned))(image + 0x4c213))(
      context, text, 4096);
}
static unsigned expand_codes(char *text, unsigned flags, unsigned pre) {
  return ((unsigned (*)(void *, char *, unsigned, unsigned, unsigned))(
             image + 0x4b0e4))(context, text, 4096, flags, pre) &
         0xffff;
}
static void pipeline_checks(void) {
  char text[4096];
  pipeline_prepare();
  fixture("N", "2");
  strcpy(text, "<<IF $N$<\"10\">>YES<<ELSE>>NO<<ENDIF>>");
  require(preprocess(text) == 1,
          "original conditional pipeline accepts variable/literal");
  equal(text, "YES",
        "original conditional comparison distinguishes numeric ordering");
  require(user_calls == 1, "conditional value read once");
  pipeline_prepare();
  fixture("CMD", ">TEST.FN");
  strcpy(text, "$<<CMD>>$");
  require(expand_codes(text, 0xff, 1) == 1,
          "computed playback expansion succeeds");
  equal(text, "", "computed playback consumed");
  equal(last_playback, "TEST.FN",
        "preprocessor composes dollar playback directive");
  require(playback_calls == 1 && user_calls == 1,
          "computed playback evaluated once");
  pipeline_prepare();
  fixture("A", "$SHIFT$");
  key_state = 0x8000;
  strcpy(text, "$A$");
  require(expand_codes(text, 0xff, 1) == 1, "dollar result expansion succeeds");
  equal(text, "$SHIFT$",
        "inserted dollar result is not rescanned for dollar codes");
  pipeline_prepare();
  fixture("A", "<<B>>");
  fixture("B", "C");
  strcpy(text, "<<A>>");
  require(preprocess(text) == 1, "nested named macro expansion succeeds");
  equal(text, "C", "named macro result is rescanned by preprocessor");
  require(user_calls == 2, "nested macros evaluate twice");
  pipeline_prepare();
  fixture("N", "1");
  key_state = 0x8000;
  strcpy(text, "<<IF $N$=0 AND $SHIFT$=1>>Y<<ELSE>>N<<ENDIF>>");
  require(preprocess(text) == 1, "boolean conditional parses");
  equal(text, "N", "false AND result");
  require(modifier_calls == 1, "AND does not short-circuit operand evaluation");
  pipeline_prepare();
  fixture("N", "0");
  fixture("A", "X");
  strcpy(text, "<<IF $N$=1>><<A>>$SHIFT$<<ELSE>>N<<ENDIF>>");
  require(expand_codes(text, 0xff, 1) == 1, "inactive branch expansion");
  equal(text, "N", "inactive branch removed");
  require(user_calls == 2 && modifier_calls == 0,
          "named angle macros in inactive text still evaluate; dollar code is "
          "discarded");
  pipeline_prepare();
  fixture("N", "$SHIFT$");
  strcpy(text, "<<IF $N$=\"$SHIFT$\">>Y<<ELSE>>N<<ENDIF>>");
  require(preprocess(text) == 1, "quoted right operand parses");
  equal(text, "Y", "quoted right operand is literal text");
  require(modifier_calls == 0,
          "comparison does not recursively evaluate dollar text in values");
}
static char transmitted[8192];
static unsigned transmissions;
static void output_capture(void *ctx, const char *text) {
  (void)ctx;
  ++transmissions;
  strncat(transmitted, text, sizeof transmitted - strlen(transmitted) - 1);
}
static void *host_scratch(void *ctx) {
  (void)ctx;
  return calloc(1, 8192);
}
static void host_release(void *ctx, void *p) {
  (void)ctx;
  free(p);
}
static unsigned host_command(char *text) {
  return ((unsigned (*)(void *, char *, int, unsigned))(image + 0x4ab3e))(
             context, text, -1, 0xff) &
         0xffff;
}
static void template_value(unsigned slot, const char *value) {
  require((((unsigned (*)(void *, const char *, unsigned))(image + 0x48838))(
               context, value, slot) &
           0xffff) == 1,
          "original template storage");
}
static void host_pipeline_checks(void) {
  char text[4096];
  hook(0x15396, (void (*)(void))host_scratch);
  hook(0x153fe, (void (*)(void))host_release);
  hook(0x6947, (void (*)(void))output_capture);
  hook(0x7705, noop);
  pipeline_prepare();
  fixture("A", "$B$");
  fixture("B", "$SHIFT$");
  strcpy(text, "$A$");
  require(host_command(text) != 0xffff,
          "original host pipeline accepts nested variables");
  equal(transmitted, "$SHIFT$", "host does exactly two ordinary dollar passes");
  transmitted[0] = 0;
  transmissions = 0;
  pipeline_prepare();
  key_state = 0x8000;
  template_value(3, "$SHIFT$");
  strcpy(text, "$?3$");
  require(host_command(text) != 0xffff, "host template embedding accepted");
  equal(transmitted, "1",
        "first-level template contents receive second dollar pass");
  transmitted[0] = 0;
  transmissions = 0;
  template_value(4, "$?3$");
  strcpy(text, "$?4$");
  require(host_command(text) != 0xffff, "nested template embedding accepted");
  equal(transmitted, "$SHIFT$", "second-level template contents remain raw");
  transmitted[0] = 0;
  transmissions = 0;
  pipeline_prepare();
  fixture("N", "2");
  strcpy(text, "[3:]<<N>>-$SHIFT$");
  require(host_command(text) != 0xffff,
          "template definition host block accepted");
  equal((char *)(uintptr_t)u32(host_commands + 0x100e + 3 * 4), "2-$SHIFT$",
        "angle macros evaluate at template definition while dollar code is "
        "stored");
  require(!transmissions, "template definition does not transmit");
  pipeline_prepare();
  for (unsigned i = 0; i < 9; ++i) {
    char value[32];
    snprintf(value, sizeof value, "<<?%u>>", i + 1);
    template_value(i, value);
  }
  template_value(9, "END");
  strcpy(text, "<<?0>>");
  require(preprocess(text) == 1,
          "eight changed preprocessor passes report success");
  equal(text, "<<?8>>",
        "template-driven angle recursion stops after eight passes");
  require(preprocess(text) == 1,
          "remaining angle template chain can run again");
  equal(text, "END",
        "second preprocessing invocation finishes remaining chain");
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  hook(0x38e47, (void (*)(void))report);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x65442, (void (*)(void))strcat);
  hook(0x654a8, (void (*)(void))strlen);
  hook(0x655e2, (void (*)(void))strcasecmp);
  hook(0x68440, (void (*)(void))formatted);
  hook(0x68c10, (void (*)(void))decimal);
  hook(0x68cc0, (void (*)(void))decimal);
  hook(0x68fc0, (void (*)(void))random_number);
  pointer(image + 0x966d0, (void *)async_key);
  pointer(image + 0x87908, character_types);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = (isdigit(i) ? 4 : 0) | (isspace(i) ? 8 : 0) |
                         (isupper(i) ? 1 : 0) | (islower(i) ? 2 : 0);
  hook(0x65ca3, (void (*)(void))allocate);
  hook(0x68d70, (void (*)(void))memset);
  hook(0x2556d, (void (*)(void))register_variable);
  prepare();
  require(((unsigned (*)(void *))(image + 0x25c02))(context) == 1,
          "original variable-engine initialization registers names");
  require(registered_callback("RAND") == 0x28e96 &&
              registered_callback("SHIFT") == 0x29319 &&
              registered_callback("CTRL") == 0x2827e &&
              registered_callback("TEXTXY") == 0x30ede &&
              registered_callback("HOSTDIR") == 0x28635 &&
              registered_callback("XFER") == 0x29365 &&
              registered_callback("OFFSCREEN") == 0x2f549 &&
              registered_callback("XY") == 0x34c36,
          "original registrations identify requested processors");
  require(!registered_callback("P") && registered_callback("PCB") == 0x2d3ab,
          "P is not an alias in original built-in registration list");
  printf("Captured %u original variable registrations\n", registered_count);
  prepare();
  strcpy(params, "9");
  strcpy(params + 21, "3");
  strcpy(params + 42, "2");
  for (random_value = 0; random_value <= 32767; ++random_value) {
    const char *value = invoke(0x28e96, 3);
    char expected[32];
    snprintf(expected, sizeof expected, "%03u", 2 + 8 * random_value / 32767);
    equal(value, expected, "RAND inclusive scale and rare maximum-plus-one");
    require(!errors && !u16(variables + 0x436), "valid RAND parameters");
  }
  require(!invoke(0x28e96, 0) && errors && u16(variables + 0x436),
          "bare RAND rejects missing required maximum");
  prepare();
  for (unsigned modifier = 0; modifier < 2; ++modifier) {
    unsigned rva = modifier ? 0x2827e : 0x29319;
    key_state = 0;
    equal(invoke(rva, 0), "0", "modifier up result");
    key_state = 0x8000;
    equal(invoke(rva, 0), "1", "modifier down result");
    require(!invoke(rva, 1) && u16(variables + 0x436),
            "modifier rejects arguments");
  }
  prepare();
  put32(ports + 0x1e, 500000);
  put32(ports + 0x22, 300000);
  put16(ports + 0xa, 7);
  equal(invoke(0x2f549, 0), "300000", "OFFSCREEN defaults FREE");
  strcpy(params, "USED");
  equal(invoke(0x2f549, 1), "200000", "OFFSCREEN USED budget");
  strcpy(params, "TOTAL");
  equal(invoke(0x2f549, 1), "500000", "OFFSCREEN TOTAL budget");
  strcpy(params, "CLIP");
  equal(invoke(0x2f549, 1), "7", "OFFSCREEN CLIP returns physical port");
  strcpy(params, "CONTENTS");
  require(!invoke(0x2f549, 1) && u16(variables + 0x436),
          "OFFSCREEN invalid option rejects");
  prepare();
  put32(variables + 0x440, 297);
  put32(variables + 0x444, 321);
  strcpy(params, "DEVICE");
  equal(invoke(0x34c36, 1), "0297:0321", "XY decimal device coordinates");
  equal(invoke(0x34c36, 0), "0594:0642", "XY default world conversion");
  hook(0x338bc, (void (*)(void))definition);
  put32(port_entries + 0x5c, 297);
  put32(port_entries + 0x60, 321);
  equal(invoke(0x30ede, 0), "297:321", "TEXTXY unpadded world cursor");
  port_defined = 0;
  strcpy(params, "CUR");
  equal(invoke(0x30ede, 1), "-1", "TEXTXY unavailable port sentinel");
  /* Literal comparison operands skip macro lookup but run the original
   * numeric predicate, signed comparisons and case-insensitive fallback. */
  typedef unsigned (*compare_fn)(void *, char *, char *, unsigned, unsigned);
  compare_fn compare = (compare_fn)(image + 0x4bd00);
  char two[] = "2", ten[] = "10", a[] = "a", b[] = "B";
  require(compare(context, two, ten, 4, 1) == 1, "numeric 2 is less than 10");
  require(compare(context, ten, two, 4, 1) == 0,
          "numeric 10 is not less than 2");
  require(compare(context, a, b, 4, 1) == 1,
          "nonnumeric ordering uses case-insensitive strings");
  pipeline_checks();
  host_pipeline_checks();
  printf("PASS: %u preliminary macro/variable checks\n", checks);
  return 0;
}
