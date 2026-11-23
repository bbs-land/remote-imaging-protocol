/* Original query definition and event dispatch; controlled memory/host
 * services. No Windows event loop, desktop, audio or actual host transmission.
 */
#include "riptel-probe-image.h"
#include <strings.h>
static unsigned char ctx[512], ports[128], port_entries[36 * 0x78], tws[128],
    tw_entries[36 * 0xf1], mouse[128], fields[2 * 0xa3], keyboard[128],
    options[512], host[8192], scratch[8192];
static unsigned requests, field_events, errors;
static char last_request[256];
static unsigned allocate(void *p, unsigned n) {
  void *v = calloc(1, n);
  require(v != NULL, "query owned allocation");
  pointer(p, v);
  return 1;
}
static void release(void *p) {
  free((void *)(uintptr_t)u32(p));
  put32(p, 0);
}
static void *temp(void *c) {
  require(c == ctx, "query scratch context");
  return scratch;
}
static void untmp(void *c, void *p) {
  require(c == ctx && p == scratch, "query scratch release");
}
static void report(void *c, unsigned level, const char *name, const char *s) {
  (void)c;
  (void)level;
  (void)name;
  fprintf(stderr, "unexpected driver error: %s\n", s);
  ++errors;
}
static void execute(void *c, const char *text, int identity, unsigned mode) {
  require(c == ctx && identity == -1 && mode == 255,
          "original query execution arguments");
  require(strlen(text) < sizeof last_request, "bounded captured query");
  strcpy(last_request, text);
  ++requests;
}
static void field_event(void *c, void *field, int index, int keyboard_event,
                        int event) {
  require(c == ctx && field == fields && (index & 0xffff) == 0 &&
              !keyboard_event && event >= 1 && event <= 3,
          "original click reaches matched field boundary");
  ++field_events;
}
static void define(unsigned mode, unsigned slot, const char *text) {
  uint32_t a[] = {mode, slot, 0};
  ((void (*)(void *, unsigned, void *, const char *))(image + 0xd3da))(ctx, 3,
                                                                       a, text);
  require(!errors, "query definition accepted");
}
static void *query_slot(unsigned mode, unsigned n) {
  if (mode == 1)
    return ports + 0x2a;
  if (mode == 2)
    return tws + 0xc;
  if (mode == 3)
    return port_entries + n * 0x78 + 0x64;
  if (mode == 4)
    return tw_entries + n * 0xf1 + 0x3f;
  return mouse + (mode == 5 ? 0x1c : 0x20);
}
static void event(unsigned message, unsigned x, unsigned y) {
  require(((unsigned (*)(void *, unsigned, unsigned, unsigned, unsigned))(
              image + 0x43f5))(ctx, 123, message, 0,
                               (x & 0xffff) | ((y & 0xffff) << 16)) == 0,
          "original event returns zero");
}
static void setup(void) {
  put16(ctx, 0x9df3);
  pointer(ctx + 0x22, ports);
  pointer(ports, port_entries);
  pointer(ctx + 0x12, tws);
  pointer(tws, tw_entries);
  pointer(ctx + 0x1e, mouse);
  pointer(mouse, fields);
  pointer(ctx + 0x32, keyboard);
  pointer(ctx + 0x36, options);
  pointer(ctx + 0x2e, host);
  put32(ctx + 0x5e, 1);
  put32(ctx + 0x66, 123);
  put16(mouse + 0x4e, 0xffff);
  put16(mouse + 0x18, 0xffff);
  for (unsigned n = 0; n < 36; ++n) {
    unsigned char *p = port_entries + n * 0x78;
    p[0] = 1;
    put32(p + 0x1a, 0);
    put32(p + 0x1e, 0);
    put32(p + 0x22, 640);
    put32(p + 0x26, 480);
    memcpy(p + 0x2a, p + 0x1a, 16);
    unsigned char *t = tw_entries + n * 0xf1;
    put32(t + 0x12, 0);
    put32(t + 0x16, 0);
    put32(t + 0x1a, 640);
    put32(t + 0x1e, 480);
    t[0x24] = 80;
    t[0x25] = 30;
    put16(t + 0x26, 0x8001);
    define(3, n, "NUMBERED_PORT");
    define(4, n, "NUMBERED_TW");
    require(
        !strcmp((char *)(uintptr_t)u32(query_slot(3, n)), "NUMBERED_PORT") &&
            !strcmp((char *)(uintptr_t)u32(query_slot(4, n)), "NUMBERED_TW"),
        "both numbered definitions stored at every slot");
  }
  for (unsigned m = 1; m <= 2; ++m) {
    define(m, 0, m == 1 ? "FLOATING_PORT" : "FLOATING_TW");
    require(u32(query_slot(m, 0)) != 0, "floating query stored");
  }
}
static void click_checks(void) {
  const unsigned xy[][2] = {{0, 0},     {1, 1},     {10, 10},      {639, 479},
                            {640, 480}, {641, 481}, {65535, 65535}};
  unsigned char port_before[sizeof port_entries], tw_before[sizeof tw_entries];
  memcpy(port_before, port_entries, sizeof port_entries);
  memcpy(tw_before, tw_entries, sizeof tw_entries);
  for (unsigned p = 0; p < 36; ++p)
    for (unsigned t = 0; t < 36; ++t)
      for (unsigned pos = 0; pos < sizeof xy / sizeof *xy; ++pos) {
        put16(ports + 8, p);
        put16(tws + 0xa, t);
        unsigned before = requests;
        event(0x200, xy[pos][0], xy[pos][1]);
        event(0x201, xy[pos][0], xy[pos][1]);
        event(0x200, 12, 12); /* held movement */
        event(0x202, xy[pos][0], xy[pos][1]);
        event(0x201, xy[pos][0], xy[pos][1]); /* next independent click */
        event(0x202, xy[pos][0], xy[pos][1]);
        require(requests == before && !field_events && !errors,
                "all 74 stored click queries remain unexecuted");
        require(u16(ports + 8) == p && u16(tws + 0xa) == t &&
                    !memcmp(port_before, port_entries, sizeof port_entries) &&
                    !memcmp(tw_before, tw_entries, sizeof tw_entries),
                "click does not mutate query tables or current selections");
      }
}
static void positive_checks(void) {
  unsigned before = requests;
  define(0, 0, "IMMEDIATE");
  require(
      requests == before + 1 && !strcmp(last_request, "IMMEDIATE"),
      "original immediate mode executes through the captured host boundary");
  define(5, 0, "ENTER");
  define(6, 0, "EXIT");
  put16(mouse + 0x16, 1);
  put32(fields + 0x10, 10);
  put32(fields + 0x14, 10);
  put32(fields + 0x18, 20);
  put32(fields + 0x1c, 20);
  put16(fields + 0xa0, 1);
  event(0x200, 15, 15);
  require(requests == before + 2 && !strcmp(last_request, "ENTER"),
          "original hover mode five executes on entry");
  event(0x200, 30, 30);
  require(requests == before + 3 && !strcmp(last_request, "EXIT"),
          "original hover mode six executes on exit");
  event(0x201, 15, 15);
  event(0x202, 15, 15);
  require(
      field_events == 2 && requests == before + 3,
      "original button down/up selects field without any deferred click query");
  for (unsigned m = 1; m <= 6; ++m) {
    unsigned n = m == 3 || m == 4 ? 35 : 0;
    define(m, n, "$oFf$");
    require(!u32(query_slot(m, n)), "case-insensitive OFF clears stored query");
  }
}
int main(int argc, char **argv) {
  require(argc == 2, "one original DLL");
  load_image(argv[1]);
  hook(0x65ca3, (void (*)(void))allocate);
  hook(0x65cbf, (void (*)(void))release);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x654a8, (void (*)(void))strlen);
  hook(0x655e2, (void (*)(void))strcasecmp);
  hook(0x15396, (void (*)(void))temp);
  hook(0x153fe, (void (*)(void))untmp);
  hook(0x38e47, (void (*)(void))report);
  hook(0x3988f, noop);
  hook(0x398ee, noop);
  hook(0x2badd, noop);
  hook(0x2bb24, noop);
  hook(0x32fdb, noop); /* host status-line notification only */
  hook(0x4ab3e, (void (*)(void))execute);
  hook(0x2c424, (void (*)(void))field_event);
  setup();
  click_checks();
  positive_checks();
  for (unsigned n = 0; n < 36; ++n) {
    release(query_slot(3, n));
    release(query_slot(4, n));
  }
  for (unsigned m = 1; m <= 2; ++m)
    release(query_slot(m, 0));
  release(query_slot(5, 0));
  release(query_slot(6, 0));
  printf("riptel-query-probe: %u checks passed (no GUI)\n", checks);
  return 0;
}
