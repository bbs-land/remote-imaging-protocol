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
static unsigned random_value, key_state, errors, port_defined;
static unsigned definition(void *ctx, unsigned slot) {
  (void)ctx;
  (void)slot;
  return port_defined;
}
static unsigned random_number(void) { return random_value; }
static unsigned __attribute__((stdcall)) async_key(unsigned key) {
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
  printf("PASS: %u preliminary macro/variable checks\n", checks);
  return 0;
}
