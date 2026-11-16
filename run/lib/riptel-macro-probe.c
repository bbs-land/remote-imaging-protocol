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
  require(registered_callback("PORTW") == 0x2f863 &&
              registered_callback("PORTH") == 0x2f642,
          "original registration confirms port-size callback identities");
  const char *cases[] = {"A$PORTW$B", "A$PORTH$B", "A$PORTW(CUR,PORT,DEVICE)$B",
                         "A$PORTH(CUR,PORT,WORLD)$B"};
  for (unsigned n = 0; n < 4; ++n) {
    pipeline_prepare();
    put16(ports + 8, 1);
    port_entries[0x78] = 4;
    put32(port_entries + 0x78 + 0xb, 640);
    put32(port_entries + 0x78 + 0xf, 480);
    char text[4096];
    strcpy(text, cases[n]);
    unsigned status = expand_codes(text, 0xff, 1);
    require(status == 1 && !strcmp(text, cases[n]),
            "ordinary macro expansion retains failed port-size expression");
    require(
        u16(variables + 0x436) == 1 && !errors,
        "port-size parse-gate defect sets variable error without host report");
  }
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
  printf("PASS: %u macro/variable checks\n", checks);
  return 0;
}
