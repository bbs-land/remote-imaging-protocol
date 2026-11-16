/* Original RIPtel variable/comparison routines with controlled host services.
 * No GUI. The wrapper verifies the supplied DLL before this maps its image. */
#include "riptel-probe-image.h"
#include <ctype.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
static unsigned char context[512], variables[0x700], ports[128], view[128],
    port_entries[36 * 0x78];
static char result[8192], params[16 * 21];
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
  hook(0x6bcb0, (void (*)(void))strncpy);
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
static unsigned char host_state[0x600], configuration[0x100], parser_state[256];
static char transfer_names[4096], directory_path[256];
static unsigned directory_calls, transfer_calls, transfer_args[4], dialog_calls,
    dialog_answer, persistent_present, persistent_deleted;
static unsigned char persistent_record[0x80];
static unsigned make_directory(const char *path) {
  ++directory_calls;
  snprintf(directory_path, sizeof directory_path, "%s", path);
  return (unsigned)-1;
}
static void transfer(void *ctx, unsigned count, const unsigned *args,
                     const char *names) {
  (void)ctx;
  require(count == 4, "XFER forwards four block-mode fields");
  ++transfer_calls;
  memcpy(transfer_args, args, sizeof transfer_args);
  snprintf(transfer_names, sizeof transfer_names, "%s", names);
}
static unsigned __attribute__((stdcall))
dialog(void *window, const char *message, const char *title, unsigned flags) {
  (void)window;
  (void)title;
  require(flags == 4 &&
              !strcmp(message, "Requested record is protected, Delete anyway?"),
          "TV deletion protection prompt");
  ++dialog_calls;
  return dialog_answer;
}
static unsigned persistent_lookup(void *cfg, const char *name, void *record) {
  (void)cfg;
  if (persistent_present && !strcasecmp(name, "DISKVAR")) {
    memcpy(record, persistent_record, sizeof persistent_record);
    return 0;
  }
  return 1;
}
static void persistent_delete(void *cfg) {
  (void)cfg;
  ++persistent_deleted;
}
static void effects_prepare(void) {
  pipeline_prepare();
  put16(context, 0x9df3);
  memset(host_state, 0, sizeof host_state);
  memset(configuration, 0, sizeof configuration);
  memset(parser_state, 0, sizeof parser_state);
  pointer(context + 0x32, host_state);
  pointer(context + 0x72, configuration);
  strcpy((char *)configuration + 0x16, "ROOT");
  pointer(context + 2, parser_state);
  pointer(parser_state + 0xa1, transfer_names);
  directory_calls = transfer_calls = dialog_calls = persistent_present =
      persistent_deleted = 0;
  directory_path[0] = transfer_names[0] = 0;
  hook(0x6c430, (void (*)(void))make_directory);
  hook(0x24b4e, (void (*)(void))transfer);
  hook(0x147e5, (void (*)(void))persistent_lookup);
  hook(0x14f7b, (void (*)(void))persistent_delete);
  pointer(image + 0x966f4, (void *)dialog);
}
static unsigned evaluate(const char *wire, const char **out) {
  unsigned short length = 0;
  unsigned parsed = ((unsigned (*)(void *, const char *, void *))(
      image + 0x15602))(variables, wire, &length);
  require(parsed == 1, "original dollar parser accepts test expression");
  return ((unsigned (*)(void *, void *, unsigned, const char **))(
             image + 0x25b21))(context, variables, 0xff, out) &
         0xffff;
}
static void effects_checks(void) {
  const char *out = NULL;
  effects_prepare();
  equal(invoke(0x28635, 0), "<NONE>", "HOSTDIR unset getter");
  strcpy(params, "  mixed  ");
  require(invoke(0x28635, 1) == NULL && !u16(variables + 0x436),
          "HOSTDIR successful setter is active");
  equal((char *)host_state + 0x462, "MIXED",
        "HOSTDIR setter normalizes stored name");
  equal(directory_path, "ROOT\\MIXED",
        "HOSTDIR create path rooted under installation");
  equal(invoke(0x28635, 0), "MIXED", "HOSTDIR getter returns selected name");
  strcpy(params, "OFF");
  invoke(0x28635, 1);
  require(!host_state[0x462] && directory_calls == 1,
          "HOSTDIR OFF clears without creation");
  strcpy(params, "   ");
  invoke(0x28635, 1);
  equal(directory_path, "ROOT\\",
        "HOSTDIR whitespace empties selection and attempts root");
  const char *invalid[] = {"a/b", "a\\b", ".", "..", "1234567890123"};
  for (unsigned i = 0; i < sizeof invalid / sizeof invalid[0]; ++i) {
    strcpy(params, invalid[i]);
    invoke(0x28635, 1);
    require(u16(variables + 0x436), "HOSTDIR invalid name rejects");
  }
  effects_prepare();
  require(evaluate("$HOSTDIR(test)$", &out) == 1 && !out,
          "HOSTDIR parser and registered setter");
  equal((char *)host_state + 0x462, "TEST",
        "HOSTDIR setter works without host callback gate");
  require(evaluate("$P$", &out) == 1 && user_calls == 1,
          "P follows user-variable lookup instead of built-in paste alias");
  effects_prepare();
  static const char *protocols[] = {"XMODEM",    "XMODEMCRC", "XMODEM1K",
                                    "XMODEM1KG", "KERMIT",    "YMODEM",
                                    "YMODEMG",   "ZMODEM",    "ZMODEMCR"};
  static const char *types[] = {"RIP", "NONE", "IMAGE", "ACTIVE"};
  static const unsigned type_codes[] = {0, 1, 3, 5};
  for (unsigned proto = 0; proto < 9; ++proto)
    for (unsigned mode = 0; mode < 2; ++mode)
      for (unsigned type = 0; type < 4; ++type) {
        strcpy(params, mode ? "SEND" : "RECEIVE");
        strcpy(params + 21, protocols[proto]);
        strcpy(params + 42, types[type]);
        strcpy(params + 63, "a.rip");
        strcpy(params + 84, "b.bmp");
        transfer_calls = 0;
        invoke(0x29365, 5);
        require(!u16(variables + 0x436) && transfer_calls == 1 &&
                    transfer_args[0] == mode && transfer_args[1] == proto &&
                    transfer_args[2] == type_codes[type] && !transfer_args[3],
                "XFER keyword map and decoded block-mode arguments");
        equal(transfer_names, mode || proto < 5 ? "a.rip b.bmp<>" : "<>",
              "XFER joins required filenames and ignores names for batch "
              "receive");
      }
  strcpy(params, "RECEIVE");
  strcpy(params + 21, "ZMODEM");
  strcpy(params + 42, "NONE");
  transfer_calls = 0;
  invoke(0x29365, 3);
  require(transfer_calls == 1 && !u16(variables + 0x436),
          "batch receive accepts no filename");
  strcpy(params, "SEND");
  transfer_calls = 0;
  invoke(0x29365, 3);
  require(!transfer_calls && u16(variables + 0x436), "send requires filename");
  strcpy(params, "RECEIVE");
  strcpy(params + 21, "KERMIT");
  transfer_calls = 0;
  invoke(0x29365, 3);
  require(!transfer_calls && u16(variables + 0x436),
          "KERMIT receive requires filename");
  effects_prepare();
  /* A real list node and data record; the original lookup and deletion run.
   * Attribute bit 1 denotes deletion without a protection prompt. */
  unsigned char *node = calloc(1, 14), *data = calloc(1, 0x7c);
  require(node && data, "TV record allocation");
  strcpy((char *)data, "MEMVAR");
  data[0x7a] = 1;
  pointer(node + 0xa, data);
  put32(variables + 0x654, 1);
  pointer(variables + 0x65c, node);
  pointer(variables + 0x660, node);
  strcpy(params, "TV");
  strcpy(params + 21, "MEMVAR");
  invoke(0x43d38, 1);
  require(u16(variables + 0x436) && u32(variables + 0x654) == 1,
          "RESET TV requires a variable name");
  invoke(0x43d38, 2);
  require(!u16(variables + 0x436) && !u32(variables + 0x654) &&
              !u32(variables + 0x65c) && !dialog_calls,
          "RESET TV deletes the actual unprotected memory list record");
  node = calloc(1, 14);
  data = calloc(1, 0x7c);
  require(node && data, "protected TV record allocation");
  strcpy((char *)data, "MEMVAR");
  pointer(node + 0xa, data);
  put32(variables + 0x654, 1);
  pointer(variables + 0x65c, node);
  pointer(variables + 0x660, node);
  dialog_answer = 7;
  invoke(0x43d38, 2);
  require(dialog_calls == 1 && u32(variables + 0x654) == 1,
          "protected memory deletion declined");
  dialog_answer = 6;
  invoke(0x43d38, 2);
  require(dialog_calls == 2 && !u32(variables + 0x654),
          "protected memory deletion confirmed");
  memset(persistent_record, 0, sizeof persistent_record);
  persistent_record[0x7a] = 1;
  persistent_present = 1;
  strcpy(params + 21, "diskvar");
  invoke(0x43d38, 2);
  require(persistent_deleted == 1 && !u16(variables + 0x436),
          "RESET TV locates persistent record and requests deletion");
  persistent_present = 0;
  strcpy(params + 21, "MISSING");
  invoke(0x43d38, 2);
  require(persistent_deleted == 1 && !u16(variables + 0x436),
          "missing TV deletion silently does nothing");
}
static unsigned chain_insert_calls;
static char *chain_insert_observe(char *target, char *input, unsigned position,
                                  unsigned limit) {
  ++chain_insert_calls;
  require(!strcmp(target, "<>") && !strcmp(input, "X") && limit == 4096,
          "original chain supplies template destination and feed string");
  unsigned expected_bad_offset =
      (unsigned)(uintptr_t)(target + 1) - (unsigned)(uintptr_t)input;
  require(position == expected_bad_offset && position != 1,
          "chain subtracts input buffer base instead of template base for "
          "placeholder offset");
  return NULL; /* Observe the invalid call without executing an unsafe write. */
}
static void remaining_variable_checks(void) {
  const char *out = NULL;
  effects_prepare();
  random_value = 0;
  require(evaluate("$RAND(9)$", &out) == 1,
          "RAND grammar with required maximum");
  equal(out, "0", "RAND default minimum and width");
  const char *bad[] = {"$RAND$", "$RAND(0)$", "$RAND(32768)$", "$RAND(9,6)$",
                       "$RAND(9,0,9)$"};
  for (unsigned i = 0; i < sizeof bad / sizeof bad[0]; ++i) {
    unsigned code = evaluate(bad[i], &out);
    require(code == 0 && !out && u16(variables + 0x436),
            "RAND parsed range/arity rejection");
  }
  random_value = 32767;
  require(evaluate("$RAND(1,0,0)$", &out) == 1, "RAND three-argument form");
  equal(out, "2", "RAND upper endpoint can exceed maximum by one");
  random_value = 0;
  require(evaluate("$RAND(9,5,8)$", &out) == 1,
          "RAND width and minimum grammar");
  equal(out, "00008", "RAND minimum-width zero padding");
  require(evaluate("$RAND(4294967297)$", &out) == 1,
          "RAND converted maximum wraps to one");
  equal(out, "0", "original decimal conversion wraps modulo 32 bits");
  require(evaluate("$RAND(9,4294967295)$", &out) == 1,
          "RAND signed negative wrapped width accepted");
  equal(out, "0", "wrapped negative width causes no padding");
  require(evaluate("$RAND(9,0,4294967295)$", &out) == 1,
          "RAND wrapped negative minimum passes signed comparison");
  equal(out, "-1", "wrapped minimum is used as signed value");
  effects_prepare();
  put32(port_entries + 0x5c, 297);
  put32(port_entries + 0x60, 321);
  require(evaluate("$TEXTXY(CUR,WORLD)$", &out) == 1,
          "TEXTXY explicit world domain");
  equal(out, "297:321", "TEXTXY world is unpadded signed decimal");
  require(evaluate("$TEXTXY(CUR,DEVICE)$", &out) == 1,
          "TEXTXY explicit device domain");
  equal(out, "148:160", "TEXTXY device conversion truncates independently");
  require(evaluate("$TEXTXY(36)$", &out) == 0 && !out,
          "TEXTXY port selector upper bound");
  port_entries[0x17] = 2;
  require(evaluate("$TEXTXY(CUR)$", &out) == 1,
          "TEXTXY port-type rejection result");
  equal(out, "-1", "TEXTXY supplied port failing type bit returns minus one");
  require(evaluate("$TEXTXY$", &out) == 1, "TEXTXY default skips type check");
  equal(out, "297:321",
        "TEXTXY omitted port differs from explicit CUR type check");
  effects_prepare();
  unsigned char text_table[128] = {0}, text_entry[0xf1] = {0};
  pointer(context + 0x12, text_table);
  pointer(text_table, text_entry);
  put16(text_table + 0x4c, 8);
  put16(text_table + 0x56, 16);
  put32(text_entry + 0x1a, 640);
  put32(text_entry + 0x1e, 480);
  text_entry[0x24] = 80;
  text_entry[0x25] = 25;
  put16(variables + 0x652, 1);
  put32(variables + 0x440, 297);
  put32(variables + 0x444, 321);
  require(evaluate("$XY$", &out) == 1, "XY text-query default");
  equal(out, "37:20", "XY actual device-to-cell helpers and decimal padding");
  require(evaluate("$XY(DEVICE)$", &out) == 1,
          "XY explicit domain overrides text query");
  equal(out, "0297:0321", "XY device remains decimal in text query");
  require(evaluate("$XY(WORLD)$", &out) == 1,
          "XY explicit world domain overrides text query");
  equal(out, "0594:0642", "XY world conversion in text query");
  require(evaluate("$XY(INVALID)$", &out) == 0 && !out,
          "XY invalid domain rejects");
  pointer(context + 0x12, NULL);
  pipeline_prepare();
  char names[16][8], values[16][24], text[4096];
  for (unsigned i = 0; i < 16; ++i) {
    snprintf(names[i], sizeof names[i], "N%u", i);
    if (i == 15)
      strcpy(values[i], "END");
    else
      snprintf(values[i], sizeof values[i], "<<N%u>>", i + 1);
    fixture(names[i], values[i]);
  }
  strcpy(text, "<<N0>>");
  require(((unsigned (*)(void *, char *, unsigned))(image + 0x4c244))(
              context, text, 4096) == 2,
          "one changed worker pass expands long named-macro chain");
  equal(text, "END", "sixteen named macros expand within one worker pass");
  require(user_calls == 16,
          "eight-pass wrapper does not bound immediate recursion");
  pipeline_prepare();
  transmitted[0] = 0;
  transmissions = 0;
  template_value(3, "<$?$>");
  unsigned char saved_insert[5];
  memcpy(saved_insert, image + 0x65987, 5);
  hook(0x65987, (void (*)(void))chain_insert_observe);
  strcpy(text, "[3]X");
  require(host_command(text) != 0xffff && chain_insert_calls == 1,
          "original chain reaches invalid-offset observation");
  memcpy(image + 0x65987, saved_insert, 5);
  pipeline_prepare();
  fixture("N", "1");
  strcpy(text, "<<IF $N$=1 OR $N$=0 AND $N$=0>>Y<<ELSE>>N<<ENDIF>>");
  require(preprocess(text) == 1, "mixed boolean operators parse");
  equal(text, "N", "unparenthesized AND and OR reduce left to right");
  strcpy(text, "<<IF $N$=1 OR ($N$=0 AND $N$=0)>>Y<<ELSE>>N<<ENDIF>>");
  require(preprocess(text) == 1, "parenthesized boolean operators parse");
  equal(text, "Y", "parentheses control boolean reduction");
  strcpy(text, "<<IF NOT $N$=1 AND $N$=0>>Y<<ELSE>>N<<ENDIF>>");
  require(preprocess(text) == 1, "NOT conditional parses");
  equal(text, "Y", "NOT reduction follows eligible binary reductions");
  strcpy(text, "<<IF \"2\"<\"10\">>Y<<ENDIF>>");
  require(preprocess(text) == 0 && errors,
          "quoted left literal is not the shipped comparison grammar");
  pipeline_prepare();
  fixture("N", "0");
  strcpy(text, "<<IF $N$=1>><<IF $SHIFT$=1>>X<<ENDIF>><<ELSE>>N<<ENDIF>>");
  require(preprocess(text) == 1 && modifier_calls == 0,
          "nested condition in inactive parent is syntax-checked without "
          "evaluating its comparison");
  equal(text, "N", "nested inactive condition output");
  strcpy(text, "<<IF $N$=1>>X");
  require(preprocess(text) == 0 && errors,
          "missing ENDIF rejects preprocessing");
  pipeline_prepare();
  fixture("A", "<<N>>");
  fixture("N", "X");
  transmitted[0] = 0;
  transmissions = 0;
  strcpy(text, "$A$");
  require(host_command(text) != 0xffff, "host insertion of angle text");
  equal(transmitted, "<<N>>",
        "dollar result does not restart angle preprocessing");
  require(user_calls == 1,
          "new angle name remains unevaluated after preprocessing stage");
  pipeline_prepare();
  transmitted[0] = 0;
  transmissions = 0;
  template_value(3, "$SHIFT$");
  template_value(4, "FOUR");
  strcpy(text, "[34]X");
  require(host_command(text) != 0xffff,
          "marker-free template chain runs without wrong-offset path");
  equal(transmitted, "FOUR",
        "last marker-free template replaces previous chain input");
  require(modifier_calls == 0,
          "discarded earlier chain template is not dollar-evaluated");
}
/* Issue #7: framing retains these quotes; verify the original host stages. */
static void host_quote_checks(void) {
  char text[4096];
  pipeline_prepare();
  transmitted[0] = 0;
  strcpy(text, "\\$SHIFT\\$");
  require(host_command(text) != 0xffff, "both quoted dollars pass host stages");
  equal(transmitted, "$SHIFT$",
        "final host pass removes quotes as literal text");
  require(modifier_calls == 0, "fully quoted variable does not query modifier");
  pipeline_prepare();
  transmitted[0] = 0;
  key_state = 0x8000;
  strcpy(text, "\\$SHIFT$");
  require(host_command(text) != 0xffff,
          "opening-only quote passes host stages");
  equal(transmitted, "1",
        "first host pass evaluates opening-only quoted variable");
  require(modifier_calls == 1,
          "opening-only quote is insufficient in two-pass host path");
}
static void parser_edge_checks(void) {
  static const struct {
    const char *wire;
    int parsed;
    unsigned count, code;
    const char *value;
  } cases[] = {{"$RAND()$", 1, 0, 0, NULL},
               {"$RAND(9,)$", 1, 1, 1, "0"},
               {"$RAND(9,,2)$", 1, 2, 1, "00"},
               {"$RAND(,9)$", 1, 1, 1, "0"},
               {"$HOSTDIR(MY BBS)$", 1, 1, 1, NULL},
               {"$HOSTDIR(  mixed  )$", 1, 1, 1, NULL},
               {"$HOSTDIR()$", 1, 0, 1, "<NONE>"},
               {"$HOSTDIR(\"\")$", -1, 0, 0, NULL},
               {"$RESET(TV,)$", 1, 1, 0, NULL},
               {"$XFER(SEND,ZMODEM,NONE,)$", 1, 3, 0, NULL}};
  for (unsigned i = 0; i < sizeof cases / sizeof cases[0]; ++i) {
    effects_prepare();
    random_value = 0;
    unsigned short n = 0;
    unsigned parsed = ((unsigned (*)(void *, const char *, void *))(
        image + 0x15602))(variables, cases[i].wire, &n);
    require((short)parsed == cases[i].parsed,
            "original empty-token parser result");
    require(u16(variables + 0x15) == cases[i].count,
            "empty comma fields do not occupy argument slots");
    if (parsed == 1) {
      const char *out = NULL;
      unsigned code = ((unsigned (*)(void *, void *, unsigned, const char **))(
                          image + 0x25b21))(context, variables, 0xff, &out) &
                      0xffff;
      require(code == cases[i].code, "empty-token handler result");
      if (cases[i].value)
        equal(out, cases[i].value, "empty-token replacement value");
      else
        require(!out, "empty-token active or rejected expression has no value");
    }
  }
  for (unsigned length = 20; length <= 21; ++length) {
    char wire[128] = "$XFER(";
    memset(wire + 6, 'A', length);
    strcpy(wire + 6 + length, ")$");
    unsigned short consumed = 0;
    int parsed = (short)((unsigned (*)(void *, const char *, void *))(
        image + 0x15602))(variables, wire, &consumed);
    require(parsed == (length == 20 ? 1 : -1),
            "original parser accepts 20-byte arguments and rejects 21");
  }
  for (unsigned count = 40; count <= 41; ++count) {
    char wire[128] = "$XFER(";
    for (unsigned i = 0; i < count; ++i)
      strcat(wire, i ? ",A" : "A");
    strcat(wire, ")$");
    unsigned short consumed = 0;
    int parsed = (short)((unsigned (*)(void *, const char *, void *))(
        image + 0x15602))(variables, wire, &consumed);
    require(parsed == (count == 40 ? 1 : -1),
            "original parser accepts 40 nonempty arguments and rejects 41");
  }
}
static void port_size_query_checks(void) {
  const char *names[] = {"PORTW",  "PORTH",  "PORTX0",
                         "PORTX1", "PORTY0", "PORTY1"};
  const unsigned callbacks[] = {0x2f863, 0x2f642, 0x2f95e,
                                0x2fa45, 0x2fb2a, 0x2fc11};
  const char *forms[] = {"",
                         "(CUR)",
                         "(1)",
                         "(35)",
                         "(CUR,PORT)",
                         "(CUR,VIEW)",
                         "(CUR,PORT,WORLD)",
                         "(CUR,PORT,DEVICE)",
                         "(cur,view,device)",
                         "(035,view,world)"};
  for (unsigned name = 0; name < 6; ++name) {
    require(registered_callback(names[name]) == callbacks[name],
            "original registration confirms all six port-query callbacks");
    for (unsigned form = 0; form < 10; ++form) {
      pipeline_prepare();
      put16(ports + 8, 1);
      port_entries[0x78] = 4;
      put32(port_entries + 0x78 + 0xb, 640);
      put32(port_entries + 0x78 + 0xf, 480);
      char text[4096], expected[4096];
      snprintf(text, sizeof text, "A$%s%s$B", names[name], forms[form]);
      strcpy(expected, text);
      unsigned status = expand_codes(text, 0xff, 1);
      require(status == 1 && !strcmp(text, expected),
              "ordinary expansion retains valid failed port-query expression");
      require(u16(variables + 0x436) == 1 && !errors,
              "all port-query parse-gate defects set variable error without "
              "host report");
    }
  }
}

static int date_fixture[9];
static unsigned date_clock_calls;
static unsigned date_now(unsigned *value) {
  ++date_clock_calls;
  if (value)
    *value = 1234567;
  return 1234567;
}
static void *date_localtime(const unsigned *value) {
  require(value && *value == 1234567,
          "date callback uses current local-time service");
  return date_fixture;
}
static void date_time_checks(void) {
  require(registered_callback("DATETIME") == 0x38f2a &&
              registered_callback("YEAR") == 0x390ca,
          "original registrations select date/time processors");
  hook(0x6f230, noop);
  hook(0x68ff0, (void (*)(void))date_now);
  hook(0x69c90, (void (*)(void))date_localtime);
  prepare();
  memcpy(date_fixture, (int[]){50, 38, 14, 5, 11, 93, 0, 338, 0},
         sizeof date_fixture);
  equal(invoke(0x38f2a, 0), "12/05/93 14:38:50",
        "original DATETIME December 5 example");
  equal(invoke(0x390ca, 0), "93", "original YEAR two-digit result");
  /* Every valid date in 1970..2037, including leap dates and the century
   * rollover. Expected bytes use explicit decimal fields, not host strftime. */
  unsigned ordinal = 0, dates = 0;
  const unsigned month_days[] = {31, 28, 31, 30, 31, 30,
                                 31, 31, 30, 31, 30, 31};
  for (unsigned year = 1970; year <= 2037; ++year) {
    unsigned yday = 0;
    date_fixture[5] = year - 1900;
    char expected_year[3];
    snprintf(expected_year, sizeof expected_year, "%02u", year % 100);
    memset(result, 0xa5, 32);
    const char *value = invoke(0x390ca, 0);
    require(value == result && !memcmp(value, expected_year, 3) &&
                (unsigned char)result[3] == 0xa5,
            "YEAR is exactly two decimal digits then storage NUL");
    for (unsigned month = 0; month < 12; ++month) {
      unsigned days =
          month_days[month] +
          (month == 1 && year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
      date_fixture[4] = month;
      for (unsigned day = 1; day <= days; ++day) {
        date_fixture[3] = day;
        date_fixture[6] = (ordinal++ + 4) % 7;
        date_fixture[7] = yday++;
        ++dates;
        for (unsigned late = 0; late < 2; ++late) {
          date_fixture[0] = late ? 59 : 0;
          date_fixture[1] = late ? 59 : 0;
          date_fixture[2] = late ? 23 : 0;
          char expected[32];
          snprintf(expected, sizeof expected, "%02u/%02u/%02u %02d:%02d:%02d",
                   month + 1, day, year % 100, date_fixture[2], date_fixture[1],
                   date_fixture[0]);
          memset(result, 0xa5, 32);
          value = invoke(0x38f2a, 0);
          require(value == result && !memcmp(value, expected, 18) &&
                      (unsigned char)result[18] == 0xa5,
                  "DATETIME exact 17 bytes plus NUL at both day endpoints");
          require(!u16(variables + 0x436) && !errors,
                  "valid date has no variable error");
        }
      }
    }
  }
  memcpy(date_fixture, (int[]){0, 0, 0, 5, 11, 93, 0, 338, 0},
         sizeof date_fixture);
  for (unsigned second = 0; second < 86400; ++second) {
    date_fixture[0] = second % 60;
    date_fixture[1] = second / 60 % 60;
    date_fixture[2] = second / 3600;
    char expected[32];
    snprintf(expected, sizeof expected, "12/05/93 %02d:%02d:%02d",
             date_fixture[2], date_fixture[1], date_fixture[0]);
    memset(result, 0xa5, 32);
    const char *value = invoke(0x38f2a, 0);
    require(
        value == result && !memcmp(value, expected, 18) &&
            (unsigned char)result[18] == 0xa5,
        "every time of day uses zero-padded 24-hour fields without newline");
  }
  const unsigned callbacks[] = {0x38f2a, 0x390ca};
  for (unsigned n = 0; n < 2; ++n) {
    unsigned rva = callbacks[n];
    for (unsigned count = 1; count <= 2; ++count) {
      unsigned before = date_clock_calls;
      require(!invoke(rva, count) && u16(variables + 0x436) == 1,
              "date/year callbacks reject arguments");
      require(before == date_clock_calls,
              "invalid date/year arity rejects before reading the clock");
    }
  }
  /* Surrounding literal text exposes any returned newline or storage NUL in
   * the original dollar-code parser/insertion path. */
  const char *wires[] = {"A$DATETIME$B/$YEAR$C", "A$DATETIME()$B/$YEAR()$C"};
  for (unsigned n = 0; n < 2; ++n) {
    pipeline_prepare();
    hook(0x68ff0, (void (*)(void))date_now);
    hook(0x69c90, (void (*)(void))date_localtime);
    memcpy(date_fixture, (int[]){50, 38, 14, 5, 11, 93, 0, 338, 0},
           sizeof date_fixture);
    char text[4096];
    strcpy(text, wires[n]);
    require(expand_codes(text, 0xff, 1) == 1,
            "original date/year dollar expansion succeeds");
    equal(text, "A12/05/93 14:38:50B/93C",
          "date/year insertion preserves surrounding literals without newline");
    require(!u16(variables + 0x436) && !errors,
            "date/year macro expansion reports no error");
  }
  printf("Original DATETIME: 12/05/93 14:38:50; %u dates and 86400 times\n",
         dates);
}
/* FILEDEL keeps original ordinary lookup, basename parsing and unlink. Only
 * access/DeleteFileA and the user-configured search-path provider are services.
 */
static unsigned char file_state[0x700], file_options[0x700], root_path[256],
    search_object[8], search_methods[32];
static char search_cache[1024], configured_search[1024];
static char access_paths[32][512], delete_paths[16][512];
static unsigned access_count, delete_count, present_mask, delete_failure;
static const char *search_provider(void *ctx, const char *set) {
  require(ctx == context && !set,
          "FILEDEL only queries configured search path");
  return configured_search;
}
static int file_access(const char *path, unsigned mode) {
  require(mode == 0 && access_count < 32,
          "bounded existence-only access query");
  snprintf(access_paths[access_count], sizeof access_paths[0], "%s", path);
  return (present_mask & (1u << access_count++)) ? 0 : -1;
}
static unsigned __attribute__((stdcall)) file_delete(const char *path) {
  require(delete_count < 16, "bounded deletion capture");
  snprintf(delete_paths[delete_count++], sizeof delete_paths[0], "%s", path);
  return !delete_failure;
}
static unsigned __attribute__((stdcall)) file_last_error(void) { return 5; }
static void file_configure(const char *host, const char *search) {
  put16(context, 0x9df3);
  memset(file_state, 0, sizeof file_state);
  memset(file_options, 0, sizeof file_options);
  memset(root_path, 0, sizeof root_path);
  memset(search_methods, 0, sizeof search_methods);
  pointer(context + 0x32, file_state);
  pointer(context + 0x36, file_options);
  pointer(context + 0x72, root_path);
  pointer(context + 6, search_object);
  pointer(search_object, search_cache);
  pointer(search_object + 4, search_methods);
  pointer(search_methods + 0x10, (void *)search_provider);
  strcpy((char *)root_path + 0x16, "C:\\RIP");
  strcpy((char *)file_state + 0x462, host);
  strcpy(configured_search, search);
  access_count = delete_count = present_mask = delete_failure = 0;
  hook(0x758e0, (void (*)(void))file_access);
  pointer(image + 0x965dc, (void *)file_delete);
  pointer(image + 0x96600, (void *)file_last_error);
}
static void file_prepare(const char *host, const char *search) {
  prepare();
  file_configure(host, search);
}
static void filedel_checks(void) {
  require(registered_callback("FILEDEL") == 0x28434,
          "original registration identifies FILEDEL callback");
  for (unsigned host = 0; host < 2; ++host)
    for (unsigned hit = 0; hit < (host ? 5u : 4u); ++hit) {
      file_prepare(host ? "HOST" : "", "D:\\LIB;EXTRA");
      present_mask = 1u << hit;
      strcpy(params, "X.BMP");
      require(!invoke(0x28434, 1) && !u16(variables + 0x436),
              "FILEDEL has no text result or error for nonempty argument list");
      unsigned candidates = host ? 4 : 3;
      require(access_count == (hit < candidates ? hit + 1 : candidates) &&
                  delete_count == (hit < candidates ? 1u : 0u),
              "FILEDEL deletes first ordinary-lookup hit or none");
      const char *expected[] = {"C:\\RIP\\HOST\\X.BMP", "C:\\RIP\\ICONS\\X.BMP",
                                "D:\\LIB\\X.BMP", "C:\\RIP\\EXTRA\\X.BMP"};
      for (unsigned n = 0; n < access_count; ++n)
        equal(access_paths[n], expected[n + !host],
              "FILEDEL lookup order includes shared and user search paths");
      if (delete_count)
        equal(delete_paths[0], expected[hit + !host],
              "FILEDEL passes first resolved path to original unlink");
    }
  file_prepare("HOST", "D:\\LIB;EXTRA");
  present_mask = 15;
  delete_failure = 1;
  strcpy(params, "X.BMP");
  require(
      !invoke(0x28434, 1) && access_count == 1 && delete_count == 1 &&
          !u16(variables + 0x436),
      "deletion failure is ignored and does not fall through to lower tiers");
  const struct {
    const char *input, *candidate, *wire_candidate;
  } cases[] = {
      {"X.BMP", "C:\\RIP\\HOST\\X.BMP", "C:\\RIP\\HOST\\X.BMP"},
      {"C:X.BMP", "C:\\RIP\\HOST\\C:X.BMP", "C:\\RIP\\HOST\\C:X.BMP"},
      {"C:\\DIR\\X.BMP", "C:\\RIP\\HOST\\C:\\DIR\\X.BMP",
       "C:\\RIP\\HOST\\C:DIRX.BMP"},
      {"DIR/X.BMP", "C:\\RIP\\HOST\\X.BMP", NULL},
      {"DIR\\X.BMP", "C:\\RIP\\HOST\\DIR\\X.BMP", "C:\\RIP\\HOST\\DIRX.BMP"},
      {"../X.BMP", "C:\\RIP\\HOST\\X.BMP", NULL},
      {"..\\X.BMP", "C:\\RIP\\HOST\\..\\X.BMP", "C:\\RIP\\HOST\\..X.BMP"},
      {"\\X.BMP", "C:\\RIP\\HOST\\\\X.BMP", "C:\\RIP\\HOST\\X.BMP"},
      {"/X.BMP", "C:\\RIP\\HOST\\X.BMP", NULL},
      {"\\\\S\\X.BMP", "C:\\RIP\\HOST\\\\\\S\\X.BMP", NULL},
      {"*.BMP", "C:\\RIP\\HOST\\.BMP", NULL},
      {"X?.BMP", "C:\\RIP\\HOST\\.BMP", NULL},
      {"X.BMP/", NULL, NULL},
      {"X.BMP\\", "C:\\RIP\\HOST\\X.BMP\\", NULL},
      {".", "C:\\RIP\\HOST\\.", "C:\\RIP\\HOST\\."},
      {"..", "C:\\RIP\\HOST\\..", "C:\\RIP\\HOST\\.."},
      {"", NULL, NULL},
      {" A.BMP", "C:\\RIP\\HOST\\ A.BMP", "C:\\RIP\\HOST\\ A.BMP"},
      {"A B.BMP", "C:\\RIP\\HOST\\A B.BMP", "C:\\RIP\\HOST\\A B.BMP"},
      {"A:B.BMP", "C:\\RIP\\HOST\\A:B.BMP", "C:\\RIP\\HOST\\A:B.BMP"},
      {"[X].BMP", "C:\\RIP\\HOST\\.BMP", NULL},
      {"X*Y.BMP", "C:\\RIP\\HOST\\Y.BMP", NULL},
      {"X;Y.BMP", "C:\\RIP\\HOST\\Y.BMP", NULL},
      {"X|Y.BMP", "C:\\RIP\\HOST\\Y.BMP", NULL},
  };
  for (unsigned n = 0; n < sizeof cases / sizeof cases[0]; ++n) {
    file_prepare("HOST", "");
    present_mask = 1;
    strcpy(params, cases[n].input);
    require(!invoke(0x28434, 1) && !u16(variables + 0x436),
            "FILEDEL skips or processes each argument without variable error");
    require(access_count == (cases[n].candidate ? 1u : 0u) &&
                delete_count == access_count,
            "original FILEDEL path transformation determines attempted lookup");
    if (cases[n].candidate)
      equal(delete_paths[0], cases[n].candidate,
            "original FILEDEL forwards exact transformed path to unlink");
  }
  const char *literal_names[] = {"x.icn", "X.HIC", "X.MSK", "X"};
  for (unsigned n = 0; n < 4; ++n) {
    file_prepare("HOST", "");
    present_mask = 1;
    strcpy(params, literal_names[n]);
    invoke(0x28434, 1);
    char expected[128];
    snprintf(expected, sizeof expected, "C:\\RIP\\HOST\\%s", literal_names[n]);
    equal(delete_paths[0], expected,
          "FILEDEL preserves supplied case/extension and supplies no default "
          "extension");
  }
  for (unsigned c = 0; c < 256; ++c) {
    unsigned char rejected[] = {34, 42, 43, 44, 47, 59, 60,
                                61, 62, 63, 91, 93, 124};
    unsigned accepted = c >= 32 && c <= 127;
    for (unsigned n = 0; n < sizeof rejected; ++n)
      if (c == rejected[n])
        accepted = 0;
    unsigned actual = ((unsigned (*)(unsigned))(image + 0x65c44))(c) & 0xffff;
    require(actual == accepted,
            "original filename-tail character predicate for every byte");
  }
  file_prepare("HOST", "");
  require(!invoke(0x28434, 0) && u16(variables + 0x436) == 1 && !access_count &&
              !delete_count,
          "FILEDEL requires at least one argument");
  file_prepare("HOST", "");
  present_mask = 7;
  strcpy(params, "ONE.BMP");
  strcpy(params + 21, "");
  strcpy(params + 42, "TWO.BMP");
  require(!invoke(0x28434, 3) && !u16(variables + 0x436) && delete_count == 2,
          "FILEDEL continues past an empty argument and returns no text");
  equal(delete_paths[0], "C:\\RIP\\HOST\\ONE.BMP",
        "first FILEDEL argument uses current host");
  equal(delete_paths[1], "C:\\RIP\\HOST\\TWO.BMP",
        "later FILEDEL argument is independently resolved");
  for (unsigned n = 0; n < sizeof cases / sizeof cases[0]; ++n) {
    if (!*cases[n].input)
      continue;
    for (unsigned full = 0; full < 2; ++full) {
      pipeline_prepare();
      file_configure("HOST", "");
      present_mask = 1;
      char text[4096], original[4096];
      snprintf(text, sizeof text, "A$FILEDEL(%s)$B", cases[n].input);
      strcpy(original, text);
      require(expand_codes(text, full ? 0xff : 0xff5f, 1) == 1,
              "ordinary FILEDEL text processing succeeds even when expression "
              "is rejected");
      require(
          access_count == (cases[n].wire_candidate ? 1u : 0u) &&
              delete_count == access_count && !u16(variables + 0x436),
          "original dollar argument grammar determines callback reachability");
      if (cases[n].wire_candidate) {
        equal(text, "AB", "active FILEDEL expansion returns no value");
        equal(delete_paths[0], cases[n].wire_candidate,
              "original parser/backslash handling forwards exact filename to "
              "deletion");
      } else if (!(full && !strcmp(cases[n].input, "X.BMP\\"))) {
        equal(text, original, "rejected FILEDEL expression remains literal");
      } else {
        equal(text, "A$FILEDEL(X.BMP)$B",
              "full ordinary pass removes trailing escape after parser "
              "rejection");
      }
    }
  }
  printf("Original FILEDEL: shared/search-path lookup, exact path cases and "
         "all 256 name bytes verified\n");
}

static unsigned char environment_records[36 * 16], active_environment[128],
    environment_options[512];
static unsigned char *env_record(unsigned n) {
  return environment_records + n * 16;
}
static void install_environment_records(void) {
  memset(environment_records, 0, sizeof environment_records);
  memset(active_environment, 0, sizeof active_environment);
  pointer(context + 2, active_environment);
  memset(environment_options, 0, sizeof environment_options);
  pointer(context + 0x36, environment_options);
  pointer(view, environment_records);
  env_record(0)[0] = env_record(5)[0] = 1;
  put16(env_record(0) + 1, 1280);
  put16(env_record(0) + 3, 960);
  put16(env_record(5) + 1, 640);
  put16(env_record(5) + 3, 350);
  env_record(0)[5] = 36;
  env_record(5)[5] = 64;
  env_record(5)[7] = 8;
}
static void environment_fixture(void) {
  prepare();
  install_environment_records();
}
static void environment_query_checks(void) {
  require(registered_callback("COLORMODE") == 0x17eac &&
              registered_callback("WORLD") == 0x8d5e &&
              registered_callback("WORLDH") == 0x8fe0 &&
              registered_callback("WORLDW") == 0x91e8,
          "original registration identifies color/world callbacks");
  require(!registered_callback("WORLDX") && !registered_callback("WORLDY"),
          "complete original registry has no WORLDX/WORLDY aliases");
  hook(0x3c37e, noop);
  hook(0x45038, noop);
  environment_fixture();
  equal(invoke(0x17eac, 0), "0", "COLORMODE current palette query");
  strcpy(params, "CUR");
  equal(invoke(0x17eac, 1), "0", "COLORMODE explicit current query");
  strcpy(params, "5");
  equal(invoke(0x17eac, 1), "8", "COLORMODE defined RGB query");
  strcpy(params, "6");
  equal(invoke(0x17eac, 1), "-1",
        "COLORMODE undefined environment returns exact minus one");
  require(!u16(variables + 0x436) && !env_record(6)[0],
          "undefined query has no error/definition side effect");
  const unsigned world_callbacks[] = {0x8d5e, 0x8fe0, 0x91e8};
  const char *world_expected[] = {"1280:960", "960", "1280"};
  for (unsigned n = 0; n < 3; ++n) {
    environment_fixture();
    equal(invoke(world_callbacks[n], 0), world_expected[n],
          "canonical world query axis/order");
    strcpy(params, "6");
    equal(invoke(world_callbacks[n], 1), "-1",
          "world undefined target returns minus one");
    require(!u16(variables + 0x436),
            "world undefined query is not syntax error");
  }
  for (unsigned current = 0; current < 2; ++current)
    for (unsigned bits = 0; bits <= 9; ++bits) {
      environment_fixture();
      strcpy(params, current ? "CUR" : "5");
      strcpy(params + 21, "RGB");
      snprintf(params + 42, 21, "%u", bits);
      const char *value = invoke(0x17eac, 3);
      unsigned target = current ? 0 : 5;
      require(!value, "COLORMODE setter returns no value");
      require(
          u16(variables + 0x436) == (bits < 1 || bits > 8),
          "COLORMODE macro accepts all one-through-eight RGB precision values");
      require(env_record(target)[7] == (bits >= 1 && bits <= 8 ? bits
                                        : current              ? 0
                                                               : 8),
              "COLORMODE stores requested precision without eight-only "
              "restriction");
      require(active_environment[0x3a] ==
                  (current && bits >= 1 && bits <= 8 ? bits : 0),
              "original active color setter also accepts precision one through "
              "eight");
    }
  const unsigned values[] = {0, 1, 32767, 32768, 65535, 65536, 65537};
  for (unsigned current = 0; current < 2; ++current)
    for (unsigned x = 0; x < 7; ++x)
      for (unsigned y = 0; y < 7; ++y) {
        environment_fixture();
        strcpy(params, current ? "CUR" : "5");
        snprintf(params + 21, 21, "%u", values[x]);
        snprintf(params + 42, 21, "%u", values[y]);
        invoke(0x8d5e, 3);
        unsigned rejected = values[x] > 65535 && values[y] > 65535;
        require(u16(variables + 0x436) == rejected,
                "WORLD rejects only when both dimensions exceed 65535");
        unsigned target = current ? 0 : 5;
        require(
            u16(env_record(target) + 1) ==
                    (rejected ? current ? 1280 : 640 : values[x] & 65535) &&
                u16(env_record(target) + 3) ==
                    (rejected ? current ? 960 : 350 : values[y] & 65535),
            "WORLD stores low words including zero and dimensions above 32767");
        require(u32(view + 0x24) ==
                        (current && !rejected ? values[x] & 65535 : 1280) &&
                    u32(view + 0x28) ==
                        (current && !rejected ? values[y] & 65535 : 960),
                "WORLD active setter zero-extends stored low-word dimensions");
      }
  const char *targets[] = {"CUR", "0", "5", "6", "BOGUS"};
  for (unsigned current = 0; current < 2; ++current)
    for (unsigned axis = 0; axis < 2; ++axis)
      for (unsigned target = 0; target < 5; ++target)
        for (unsigned n = 0; n < 7; ++n) {
          environment_fixture();
          put16(view + 0xa, current ? 5 : 0);
          put32(view + 0x24, current ? 640 : 1280);
          put32(view + 0x28, current ? 350 : 960);
          strcpy(params, targets[target]);
          snprintf(params + 21, 21, "%u", values[n]);
          unsigned callback = axis ? 0x8fe0 : 0x91e8;
          invoke(callback, 2);
          unsigned rejected = target == 0 || values[n] > 65535;
          require(
              u16(variables + 0x436) == rejected,
              "WORLDH/W setters reject CUR but accept non-CUR target strings");
          unsigned active = current ? 5 : 0, offset = axis ? 3 : 1;
          unsigned old = axis ? (current ? 350 : 960) : (current ? 640 : 1280);
          require(
              u16(env_record(active) + offset) == (rejected ? old : values[n]),
              "WORLDH/W numeric or unknown target changes current environment");
          require(!env_record(6)[0],
                  "WORLDH/W does not define ignored undefined target");
          require(u32(view + (axis ? 0x28 : 0x24)) ==
                      (rejected ? old : values[n]),
                  "WORLDH/W original active coordinate cache follows current "
                  "record");
        }
  for (unsigned x = 0; x < 7; ++x)
    for (unsigned y = 0; y < 7; ++y) {
      environment_fixture();
      unsigned arguments[] = {values[x], values[y]};
      ((void (*)(void *, unsigned, const void *, const char *))(
          image + 0x1f874))(context, 2, arguments, NULL);
      unsigned accepted = values[x] <= 65535 && values[y] <= 65535;
      require(u16(env_record(0) + 1) == (accepted ? values[x] : 1280) &&
                  u16(env_record(0) + 3) == (accepted ? values[y] : 960),
              "wire world frame accepts zero through 65535, rejects either "
              "oversized dimension");
      require(u32(view + 0x24) == (accepted ? values[x] : 1280) &&
                  u32(view + 0x28) == (accepted ? values[y] : 960),
              "wire world active state also has no 32767 gate");
    }
  for (unsigned bits = 0; bits <= 9; ++bits) {
    environment_fixture();
    unsigned arguments[] = {1, bits};
    ((void (*)(void *, unsigned, const void *, const char *))(image + 0x1f3fd))(
        context, 2, arguments, NULL);
    require(env_record(0)[7] == (bits == 8 ? 8 : 0) &&
                active_environment[0x3a] == (bits == 8 ? 8 : 0),
            "wire RGB command alone requires exactly eight bits");
  }
  pipeline_prepare();
  install_environment_records();
  char expression[4096] = "$COLORMODE(6)$";
  require(expand_codes(expression, 0x83, 1) == 1,
          "original undefined color macro expansion succeeds");
  equal(expression, "-1", "ordinary COLORMODE expansion emits exact minus one");
  require(!u16(variables + 0x436),
          "undefined color expansion sets no variable error");
  strcpy(expression, "$WORLDH(5)$:$WORLDW(5)$");
  expand_codes(expression, 0x83, 1);
  equal(expression, "350:640",
        "canonical H/W names dispatch height/width in macro pipeline");
  fixture("WORLDX", "custom-X");
  fixture("WORLDY", "custom-Y");
  strcpy(expression, "$WORLDX$:$WORLDY$");
  expand_codes(expression, 0x83, 1);
  equal(expression, "custom-X:custom-Y",
        "unregistered legacy X/Y names use ordinary user-variable namespace");
  pipeline_prepare();
  install_environment_records();
  strcpy(expression, "$WORLDH(CUR,1000)$");
  expand_codes(expression, 0x83, 1);
  require(u16(variables + 0x436) && u16(env_record(0) + 3) == 960,
          "ordinary WORLDH CUR setter has same original target error");
  pipeline_prepare();
  install_environment_records();
  strcpy(expression, "$WORLDH(5,1000)$");
  expand_codes(expression, 0x83, 1);
  require(!u16(variables + 0x436) && u16(env_record(0) + 3) == 1000 &&
              u16(env_record(5) + 3) == 350,
          "ordinary WORLDH setter modifies current, not named target");
  pipeline_prepare();
  install_environment_records();
  strcpy(expression, "$WORLDW(BOGUS,1234)$");
  expand_codes(expression, 0x83, 1);
  require(!u16(variables + 0x436) && u16(env_record(0) + 1) == 1234,
          "ordinary WORLDW parser permits ignored invalid target text");
  pipeline_prepare();
  install_environment_records();
  strcpy(expression, "$COLORMODE(CUR,RGB,1)$");
  expand_codes(expression, 0x83, 1);
  require(!u16(variables + 0x436) && env_record(0)[7] == 1 &&
              active_environment[0x3a] == 1,
          "ordinary color macro accepts and activates one-bit precision");
  strcpy(expression, "$COLORMODE$");
  expand_codes(expression, 0x83, 1);
  equal(expression, "1", "ordinary color query reports one-bit precision");
}

/* Screen wrappers retain original dispatch; manager boundary is captured here.
 * A separate screen/manager probe must establish real snapshot effects. */
struct backup_action {
  unsigned op, family;
  int slot;
};
static struct backup_action backup_actions[32];
static unsigned backup_action_count, backup_manager_success;
static unsigned backup_action(void *c, unsigned op, unsigned family, int slot) {
  require(c == context && backup_action_count < 32, "bounded backup dispatch");
  backup_actions[backup_action_count++] =
      (struct backup_action){op, family, slot};
  return backup_manager_success;
}
static unsigned backup_save(void *c, unsigned family, int slot) {
  return backup_action(c, 0, family, slot);
}
static unsigned backup_restore(void *c, unsigned family, int slot) {
  return backup_action(c, 1, family, slot);
}
static unsigned backup_push(void *c, unsigned family) {
  return backup_action(c, 2, family, -4);
}
static unsigned backup_pop(void *c, unsigned family) {
  return backup_action(c, 3, family, -4);
}
static unsigned backup_delete(void *c, unsigned family, int slot) {
  return backup_action(c, 4, family, slot);
}
static void backup_trace(unsigned op, int slot, unsigned all,
                         unsigned consume) {
  const unsigned order[] = {3, 2, 5, 6, 4, 0, 1, 7};
  unsigned n = all ? 8 : 1;
  require(backup_action_count == n * (1 + consume),
          "backup wrapper exact operation count, including ignored failures");
  for (unsigned i = 0; i < n; ++i) {
    unsigned family = all ? order[i] : 6;
    require(backup_actions[i].op == op && backup_actions[i].family == family &&
                backup_actions[i].slot == slot,
            "original screen/all wrapper family order and destination");
    if (consume)
      require(backup_actions[n + i].op == 4 &&
                  backup_actions[n + i].family == family &&
                  backup_actions[n + i].slot == slot,
              "numeric restore deletes after all restore attempts regardless "
              "status");
  }
}
static void screen_dispatch_checks(void) {
  hook(0x23bd3, (void (*)(void))backup_save);
  hook(0x23ca6, (void (*)(void))backup_restore);
  hook(0x23d0b, (void (*)(void))backup_push);
  hook(0x211f6, (void (*)(void))backup_pop);
  hook(0x23d6e, (void (*)(void))backup_delete);
  const char *names[] = {"SAVE", "RESTORE", "SAVEALL", "RESTOREALL"};
  const unsigned callbacks[] = {0x2493a, 0x2475d, 0x27332, 0x270c2};
  for (unsigned i = 0; i < 4; ++i)
    require(registered_callback(names[i]) == callbacks[i],
            "original registration confirms screen/all callback");
  for (unsigned success = 0; success < 2; ++success) {
    backup_manager_success = success;
    for (unsigned all = 0; all < 2; ++all)
      for (unsigned restore = 0; restore < 2; ++restore)
        for (unsigned which = 0; which < 14; ++which) {
          prepare();
          backup_action_count = 0;
          unsigned count = which == 0 ? 0 : 1;
          if (which == 1)
            strcpy(params, "BASE");
          else if (which == 2)
            strcpy(params, "base");
          else if (which == 3)
            strcpy(params, restore ? "pop" : "push");
          else if (which >= 4)
            snprintf(params, 21, "%u", which - 4);
          require(!invoke(callbacks[2 * all + restore], count) &&
                      !u16(variables + 0x436),
                  "original backup wrapper action returns empty with no syntax "
                  "error");
          backup_trace(which == 3 ? (restore ? 3 : 2) : restore,
                       which < 3    ? -1
                       : which == 3 ? -4
                                    : (int)which - 4,
                       all, restore && which >= 4);
        }
    for (unsigned restore = 0; restore < 2; ++restore)
      for (unsigned slot = 0; slot < 10; ++slot) {
        char name[32];
        snprintf(name, sizeof name, "%s%u", restore ? "RESTORE" : "SAVE", slot);
        unsigned rva = restore ? (slot ? 0x2486b + (slot - 1) * 0x17 : 0x24816)
                               : (slot ? 0x24a25 + (slot - 1) * 0x17 : 0x249e2);
        require(
            registered_callback(name) == rva,
            "all 20 original numbered aliases registered, including RESTORE0");
        prepare();
        backup_action_count = 0;
        require(!invoke(rva, 0) && !u16(variables + 0x436),
                "numbered alias accepts no parameters");
        backup_trace(restore, slot, 0, restore);
        backup_action_count = 0;
        strcpy(params, "0");
        require(!invoke(rva, 1) && u16(variables + 0x436) &&
                    !backup_action_count,
                "numbered alias rejects an argument before dispatch");
      }
  }
  for (unsigned i = 0; i < 4; ++i)
    for (unsigned invalid = 0; invalid < 4; ++invalid) {
      prepare();
      backup_action_count = 0;
      strcpy(params, invalid == 0   ? "10"
                     : invalid == 1 ? "-1"
                     : invalid == 2 ? "OTHER"
                                    : "0");
      strcpy(params + 21, "1");
      require(!invoke(callbacks[i], invalid == 3 ? 2 : 1) &&
                  u16(variables + 0x436) && !backup_action_count,
              "backup wrapper invalid selector/count has no manager effects");
    }
  const char *wire[] = {"A$SAVE$B", "A$RESTORE0$B", "A$SAVEALL(3)$B",
                        "A$RESTOREALL(3)$B"};
  for (unsigned i = 0; i < 4; ++i) {
    pipeline_prepare();
    backup_action_count = 0;
    char text[4096];
    strcpy(text, wire[i]);
    expand_codes(text, 0xff, 1);
    equal(text, "AB", "ordinary backup macro dispatch expands action to empty");
    require(!u16(variables + 0x436),
            "backup action pipeline has no variable error");
    backup_trace(i == 1 || i == 3,
                 i == 0   ? -1
                 : i == 1 ? 0
                          : 3,
                 i >= 2, i == 1 || i == 3);
  }
}

/* Capture original synchronous Beep requests, never play audio. Sweep guards
 * escape only after a complete 16-bit cycle, including malformed endpoints. */
static unsigned tone_count, tone_hash, tone_return = 1, tone_guard, tone_cycle;
static unsigned tones[16][2], last_tone[2];
static jmp_buf tone_escape;
static unsigned tone_digest(unsigned hash, unsigned f, unsigned ms) {
  return (hash * 16777619u ^ f) * 16777619u ^ ms;
}
static unsigned __attribute__((stdcall)) tone_beep(unsigned frequency,
                                                   unsigned ms) {
  if (tone_guard && tone_count == 65536) {
    tone_cycle = 1;
    longjmp(tone_escape, 1);
  }
  if (tone_count < 16) {
    tones[tone_count][0] = frequency;
    tones[tone_count][1] = ms;
  }
  last_tone[0] = frequency;
  last_tone[1] = ms;
  ++tone_count;
  tone_hash = tone_digest(tone_hash, frequency, ms);
  return tone_return;
}
static void tone_clear(void) {
  tone_count = tone_hash = tone_cycle = 0;
  memset(tones, 0, sizeof tones);
}
static void tone_invoke(unsigned rva, unsigned argc) {
  tone_clear();
  tone_guard = 1;
  if (!setjmp(tone_escape))
    require(!invoke(rva, argc), "tone action callback returns NULL");
  tone_guard = 0;
}
static void tone_checks(void) {
  const char *names[] = {"ALARM",  "BEEP",      "BLIP", "MUSIC",
                         "PHASER", "REVPHASER", "T"};
  const unsigned rvas[] = {0x8600, 0x868e, 0x873a, 0x87e6,
                           0x8892, 0x89d2, 0x8b0d};
  for (unsigned n = 0; n < 7; ++n)
    require(registered_callback(names[n]) == rvas[n],
            "original tone registration");
  pointer(image + 0x964fc, (void *)tone_beep);
  for (unsigned kind = 0; kind < 3; ++kind) {
    unsigned rva = kind == 0 ? 0x868e : kind == 1 ? 0x873a : 0x8b0d;
    unsigned duration = kind == 1 ? 25 : 75;
    for (unsigned argc = 0; argc <= 3; ++argc) {
      prepare();
      strcpy(params, "600");
      strcpy(params + 21, "17");
      tone_invoke(rva, argc);
      unsigned valid = argc <= 2 && (kind < 2 || argc == 2);
      require(u16(variables + 0x436) == !valid && tone_count == valid &&
                  !tone_cycle,
              "T requires two arguments; BEEP/BLIP accept omitted trailing "
              "arguments");
      if (valid)
        require(tones[0][0] == (argc ? 600u : 1000u) &&
                    tones[0][1] == (argc == 2 ? 17u : duration),
                "original simple-tone defaults and explicit arguments");
    }
    for (unsigned axis = 0; axis < 2; ++axis)
      for (unsigned value = 0; value <= 65536; ++value) {
        prepare();
        strcpy(params, "1");
        strcpy(params + 21, "1");
        snprintf(params + 21 * axis, 21, "%u", value);
        tone_invoke(rva, 2);
        unsigned valid = value <= 65535;
        require(u16(variables + 0x436) == !valid && tone_count == valid &&
                    !errors,
                "simple tones accept every unsigned WORD including zero, "
                "reject 65536");
        if (valid)
          require(tones[0][axis] == value && tones[0][1 - axis] == 1,
                  "frequency/duration reaches Beep unchanged");
      }
    for (unsigned axis = 0; axis < 2; ++axis) {
      const char *invalid[] = {"-1", "A", ""};
      for (unsigned n = 0; n < 3; ++n) {
        prepare();
        strcpy(params, "1");
        strcpy(params + 21, "1");
        strcpy(params + 21 * axis, invalid[n]);
        tone_invoke(rva, 2);
        require(u16(variables + 0x436) && !tone_count,
                "invalid simple-tone parameter rejects");
      }
    }
  }
  const unsigned music[][2] = {{1125, 17}, {775, 14}, {625, 10},
                               {725, 14},  {825, 10}, {600, 25}};
  const unsigned counts[] = {0, 1, 3, 4, 65535, 65536};
  for (unsigned which = 0; which < 2; ++which)
    for (unsigned n = 0; n < sizeof counts / sizeof counts[0]; ++n) {
      prepare();
      snprintf(params, 21, "%u", counts[n]);
      tone_clear();
      tone_guard = 0;
      unsigned rva = which ? 0x87e6 : 0x8600;
      require(!invoke(rva, 1), "repeating tone returns NULL");
      unsigned expected = 0, hash = 0;
      if (counts[n] <= 65535)
        for (unsigned repeat = 0; repeat < counts[n]; ++repeat)
          for (unsigned note = 0; note < (which ? 6u : 2u); ++note) {
            unsigned f = which ? music[note][0] : note ? 160 : 320;
            unsigned ms = which ? music[note][1] : note ? 225 : 200;
            hash = tone_digest(hash, f, ms);
            ++expected;
          }
      require(u16(variables + 0x436) == (counts[n] > 65535) &&
                  tone_count == expected && tone_hash == hash,
              "exact MUSIC/ALARM note sequence repeated with "
              "zero/count/overflow behavior");
    }
  for (unsigned which = 0; which < 2; ++which) {
    prepare();
    tone_clear();
    tone_guard = 0;
    require(!invoke(which ? 0x87e6 : 0x8600, 0),
            "default repeated tone returns NULL");
    require(!u16(variables + 0x436) && tone_count == (which ? 24u : 6u),
            "MUSIC defaults four repeats; ALARM three");
    prepare();
    tone_invoke(which ? 0x87e6 : 0x8600, 2);
    require(u16(variables + 0x436) && !tone_count,
            "MUSIC/ALARM reject extra parameters");
  }
  /* Every omitted suffix preserves that argument's default; invalid values
   * reject rather than falling back. Low-word loop arithmetic can wrap. */
  const unsigned values[] = {0, 1, 50, 2500, 65535, 65536};
  for (unsigned up = 0; up < 2; ++up)
    for (unsigned a = 0; a < 6; ++a)
      for (unsigned b = 0; b < 6; ++b)
        for (unsigned c = 0; c < 6; ++c)
          for (unsigned d = 0; d < 6; ++d) {
            unsigned v[] = {values[a], values[b], values[c], values[d]};
            prepare();
            for (unsigned i = 0; i < 4; ++i)
              snprintf(params + i * 21, 21, "%u", v[i]);
            tone_invoke(up ? 0x89d2 : 0x8892, 4);
            unsigned valid = v[0] <= 65535 && v[1] <= 65535 && v[2] > 0 &&
                             v[2] <= 65535 && v[3] > 0 && v[3] <= 65535 &&
                             (up ? v[0] < v[1] : v[0] > v[1]);
            unsigned expected = 0, hash = 0, f = v[0], cycle = 0;
            if (valid) {
              while (up ? f <= v[1] : f >= v[1]) {
                if (expected == 65536) {
                  cycle = 1;
                  break;
                }
                hash = tone_digest(hash, f, v[3]);
                ++expected;
                f = (up ? f + v[2] : f - v[2]) & 65535;
              }
            }
            require(u16(variables + 0x436) == !valid &&
                        tone_count == expected && tone_hash == hash &&
                        tone_cycle == cycle,
                    "sweep ordering, zero rejection, no default fallback and "
                    "exact 16-bit loop behavior");
          }
  for (unsigned up = 0; up < 2; ++up)
    for (unsigned argc = 0; argc <= 5; ++argc) {
      prepare();
      unsigned v[] = {up ? 50 : 2500, up ? 2500 : 50, 20, 2};
      for (unsigned i = 0; i < 4; ++i)
        snprintf(params + i * 21, 21, "%u", v[i]);
      strcpy(params + 84, "1");
      tone_invoke(up ? 0x89d2 : 0x8892, argc);
      unsigned valid = argc <= 4;
      require(u16(variables + 0x436) == !valid &&
                  tone_count == (valid ? 123u : 0u) &&
                  (!valid ||
                   (tones[0][0] == v[0] && tones[0][1] == 2 &&
                    last_tone[0] == (up ? 2490u : 60u) && last_tone[1] == 2)) &&
                  !tone_cycle,
              "sweep accepts each omitted suffix with exact defaults and "
              "rejects a fifth argument");
    }
  for (unsigned up = 0; up < 2; ++up)
    for (unsigned field = 0; field < 4; ++field) {
      prepare();
      strcpy(params, up ? "50" : "2500");
      strcpy(params + 21, up ? "2500" : "50");
      strcpy(params + 42, "20");
      strcpy(params + 63, "2");
      strcpy(params + field * 21, "A");
      tone_invoke(up ? 0x89d2 : 0x8892, 4);
      require(u16(variables + 0x436) && !tone_count,
              "nonnumeric sweep field rejects without default sound");
    }
  const char *valid_forms[] = {"$BEEP$",     "$BEEP(600)$", "$BEEP(600,17)$",
                               "$BLIP$",     "$BLIP(600)$", "$T(600,17)$",
                               "$MUSIC(1)$", "$PHASER$",    "$REVPHASER$"};
  for (unsigned api = 0; api < 2; ++api) {
    tone_return = api;
    for (unsigned n = 0; n < sizeof valid_forms / sizeof valid_forms[0]; ++n) {
      pipeline_prepare();
      tone_clear();
      char text[4096];
      snprintf(text, sizeof text, "A%sB", valid_forms[n]);
      require(
          expand_codes(text, 0xff, 1) == 1 && !strcmp(text, "AB") &&
              !u16(variables + 0x436) && tone_count,
          "full expansion consumes valid tone action even if OS Beep fails");
    }
  }
  const char *bad[] = {"A$T$B", "A$T(600)$B", "A$PHASER(1,2)$B",
                       "A$REVPHASER(2,1)$B", "A$PHASER(2500,50,20,0)$B"};
  for (unsigned n = 0; n < sizeof bad / sizeof bad[0]; ++n) {
    pipeline_prepare();
    tone_clear();
    char text[4096];
    strcpy(text, bad[n]);
    unsigned status = expand_codes(text, 0xff, 1);
    require(status == 1 && !strcmp(text, bad[n]) && !tone_count &&
                u16(variables + 0x436),
            "full expansion reports invalid tone instead of default fallback");
  }
  printf("Original tones: six-note MUSIC, BLIP 1000/25, required T pair, no "
         "added gaps; invalid sweeps reject; bounded WORD cycles\n");
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
  require(registered_callback("COPY") == 0x194f5 &&
              registered_callback("CUR") == 0x34f7d &&
              registered_callback("RESET") == 0x43d38 &&
              registered_callback("INUSE") == 0x12fbd &&
              registered_callback("ISPROT") == 0x2daaa &&
              registered_callback("PROT") == 0xe685 &&
              registered_callback("UNPROT") == 0xe69c,
          "original table-lifecycle callback registrations");
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
  effects_checks();
  remaining_variable_checks();
  parser_edge_checks();
  host_quote_checks();
  port_size_query_checks();
  date_time_checks();
  filedel_checks();
  environment_query_checks();
  screen_dispatch_checks();
  tone_checks();
  printf("PASS: %u macro/variable checks\n", checks);
  return 0;
}
