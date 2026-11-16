/* Original RIPtel angle normalization and curve-consumer observations.
 * GDI records calls/points only; it does not rasterize a Windows surface. */
#include "riptel-probe-image.h"
static unsigned char ctx[512], graphics[32], style[0x61], world[128],
    ports[128], entry[0x78];
static unsigned errors, events;
struct event {
  const char *kind;
  int values[12];
  unsigned count;
  int points[1600];
};
static struct event recorded[16];
static unsigned zero(void) { return 0; }
static unsigned one(void) { return 1; }
static void report(void *c, unsigned severity, const char *where,
                   const char *message) {
  (void)c;
  (void)severity;
  fprintf(stderr, "%s: %s\n", where, message);
  ++errors;
}
static struct event *record(const char *kind) {
  require(events < 16, "curve event bound");
  struct event *e = &recorded[events++];
  memset(e, 0, sizeof *e);
  e->kind = kind;
  return e;
}
static unsigned __attribute__((stdcall)) mode(unsigned dc, unsigned value) {
  (void)dc;
  (void)value;
  return 1;
}
static unsigned __attribute__((stdcall)) set_rect(int *r, int x, int y,
                                                  int right, int bottom) {
  memcpy(r, (int[]){x, y, right, bottom}, 4 * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall)) inflate(int *r, int x, int y) {
  r[0] -= x;
  r[1] -= y;
  r[2] += x;
  r[3] += y;
  return 1;
}
static unsigned __attribute__((stdcall)) move(unsigned dc, int x, int y,
                                              void *old) {
  (void)old;
  struct event *e = record("MoveToEx");
  memcpy(e->values, (int[]){(int)dc, x, y}, 3 * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall)) line(unsigned dc, int x, int y) {
  struct event *e = record("LineTo");
  memcpy(e->values, (int[]){(int)dc, x, y}, 3 * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall))
arc(unsigned dc, int l, int t, int r, int b, int sx, int sy, int ex, int ey) {
  struct event *e = record("Arc");
  memcpy(e->values, (int[]){(int)dc, l, t, r, b, sx, sy, ex, ey},
         9 * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall))
pie(unsigned dc, int l, int t, int r, int b, int sx, int sy, int ex, int ey) {
  struct event *e = record("Pie");
  memcpy(e->values, (int[]){(int)dc, l, t, r, b, sx, sy, ex, ey},
         9 * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall))
chord(unsigned dc, int l, int t, int r, int b, int sx, int sy, int ex, int ey) {
  struct event *e = record("Chord");
  memcpy(e->values, (int[]){(int)dc, l, t, r, b, sx, sy, ex, ey},
         9 * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall)) ellipse(unsigned dc, int l, int t,
                                                 int r, int b) {
  struct event *e = record("Ellipse");
  memcpy(e->values, (int[]){(int)dc, l, t, r, b}, 5 * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall)) polygon(unsigned dc, const int *p,
                                                 unsigned n) {
  struct event *e = record("Polygon");
  require(dc == 1 && n <= 800, "original polygon point bound");
  e->count = n;
  memcpy(e->points, p, 2 * n * sizeof(int));
  return 1;
}
static unsigned __attribute__((stdcall)) polyline(unsigned dc, const int *p,
                                                  unsigned n) {
  struct event *e = record("Polyline");
  require(dc == 1 && n <= 800, "original polyline point bound");
  e->count = n;
  memcpy(e->points, p, 2 * n * sizeof(int));
  return 1;
}
static void prepare(unsigned border) {
  memset(ctx, 0, sizeof ctx);
  memset(graphics, 0, sizeof graphics);
  memset(style, 0, sizeof style);
  memset(world, 0, sizeof world);
  memset(ports, 0, sizeof ports);
  memset(entry, 0, sizeof entry);
  pointer(ctx + 0xa, graphics);
  pointer(graphics, style);
  put16(style + 0x22, border);
  pointer(ctx + 0xe, world);
  put32(world + 0x24, 640);
  put32(world + 0x28, 480);
  put32(world + 0x2c, 640);
  put32(world + 0x30, 480);
  pointer(ctx + 0x22, ports);
  pointer(ports, entry);
  put32(ports + 0x14, 640);
  put32(ports + 0x18, 480);
  put32(entry + 0xb, 640);
  put32(entry + 0xf, 480);
  put32(ctx + 0x62, 1);
  errors = events = 0;
}
static void invoke(unsigned rva, unsigned *args) {
  ((void (*)(void *, unsigned, void *, const char *))(image + rva))(ctx, 0,
                                                                    args, NULL);
  require(!errors, "original curve handler reports no error");
}
/* Independently reduced arithmetic description of original helper 0x138e8.
 * Keep its <=360 fast path distinct from the translated larger-angle cases. */
static void expected_pair(unsigned *s, unsigned *e) {
  if (*s <= 360 && *e <= 360) {
    if (*s > *e)
      *e += 360;
  } else if (*s < *e) {
    unsigned turns = *s / 360;
    *s -= 360 * turns;
    *e -= 360 * turns;
    if (*e >= 720)
      *e = 360 + (*e - 360) % 360;
  } else {
    unsigned turns = *e / 360;
    *s -= 360 * turns;
    *e -= 360 * turns;
    unsigned difference = *s - *e;
    if (difference > 360)
      *s -= 360 * ((difference - 1) / 360);
    if (*s >= 360)
      *s -= 360;
    *e += 360;
  }
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  setvbuf(stdout, NULL, _IOLBF, 0);
  load_image(argv[1]);
  unsigned long long empty = 0, full = 0, partial = 0, multiple = 0;
  for (unsigned s = 0; s < 4096; ++s)
    for (unsigned e = 0; e < 4096; ++e) {
      unsigned a = s, b = e, x = s, y = e;
      ((void (*)(unsigned *, unsigned *))(image + 0x138e8))(&a, &b);
      expected_pair(&x, &y);
      require(
          a == x && b == y && b >= a,
          "original full-domain skewed normalization matches exact arithmetic");
      unsigned sweep = b - a;
      if (!sweep)
        ++empty;
      else if (sweep < 360)
        ++partial;
      else if (sweep == 360)
        ++full;
      else
        ++multiple;
    }
  printf("normalized raw pairs: empty=%llu partial=%llu full=%llu multi=%llu\n",
         empty, partial, full, multiple);
  unsigned hooks[] = {0x45038, 0x18cf6, 0x18bf7, 0x3988f, 0x398ee,
                      0x13c69, 0x343e4, 0x342ad, 0x34319, 0xe6e9};
  for (unsigned n = 0; n < sizeof hooks / sizeof hooks[0]; ++n)
    hook(hooks[n], noop);
  unsigned selections[] = {0x13cfc, 0x13ce5, 0x13d10, 0x13d27};
  for (unsigned n = 0; n < sizeof selections / sizeof selections[0]; ++n)
    hook(selections[n], (void (*)(void))one);
  hook(0x3445b, (void (*)(void))zero);
  hook(0x38e47, (void (*)(void))report);
  pointer(image + 0x963c0, (void *)mode);
  pointer(image + 0x963f0, (void *)mode);
  pointer(image + 0x96618, (void *)set_rect);
  pointer(image + 0x96698, (void *)inflate);
  pointer(image + 0x963dc, (void *)move);
  pointer(image + 0x963cc, (void *)line);
  pointer(image + 0x96464, (void *)arc);
  pointer(image + 0x96480, (void *)ellipse);
  pointer(image + 0x9649c, (void *)pie);
  pointer(image + 0x96498, (void *)chord);
  pointer(image + 0x9641c, (void *)polygon);
  pointer(image + 0x9647c, (void *)polyline);
  const unsigned pairs[][2] = {{0, 0},     {90, 90}, {0, 360},  {360, 0},
                               {450, 810}, {0, 720}, {270, 90}, {90, 270},
                               {450, 450}, {0, 719}, {0, 1},    {4095, 0}};
  const unsigned handlers[] = {0x19b50, 0x1d297, 0x1d497,
                               0x1e066, 0x1dc59, 0x1bb18};
  const char names[] = {'A', 'O', 'V', 'I', 'i', '_'};
  for (unsigned h = 0; h < 6; ++h)
    for (unsigned n = 0; n < sizeof pairs / sizeof pairs[0]; ++n) {
      prepare(0);
      unsigned args[6] = {100, 100, pairs[n][0], pairs[n][1], 40, 20};
      invoke(handlers[h], args);
      printf("%c %u..%u:", names[h], pairs[n][0], pairs[n][1]);
      for (unsigned i = 0; i < events; ++i) {
        printf(" %s", recorded[i].kind);
        for (unsigned j = 1; j <= ((!strcmp(recorded[i].kind, "Arc") ||
                                    !strcmp(recorded[i].kind, "Pie") ||
                                    !strcmp(recorded[i].kind, "Chord"))
                                       ? 8u
                                   : !strcmp(recorded[i].kind, "Ellipse") ? 4u
                                                                          : 2u);
             ++j)
          printf(" %d", recorded[i].values[j]);
      }
      puts("");
    }
  const unsigned skew_handlers[] = {0x1fac7, 0x1fca6, 0x1fee1};
  const char skew_names[] = {']', '+', '['};
  for (unsigned border = 0; border < 2; ++border)
    for (unsigned h = 0; h < 3; ++h)
      for (unsigned n = 0; n < sizeof pairs / sizeof pairs[0]; ++n) {
        prepare(border);
        unsigned args[7] = {100, 100, 40, 20, pairs[n][0], pairs[n][1], 0};
        invoke(skew_handlers[h], args);
        unsigned a = pairs[n][0], b = pairs[n][1];
        expected_pair(&a, &b);
        unsigned generated = b - a + 1;
        unsigned extra = h == 0 ? 0 : h == 1 ? 1 : 2;
        require(events == (h && border ? 2u : 1u),
                "original skewed contour draw pass count");
        for (unsigned i = 0; i < events; ++i) {
          struct event *e = &recorded[i];
          require(!strcmp(e->kind, h ? "Polygon" : "Polyline") &&
                      e->count == generated + extra,
                  "skewed consumer retains normalized inclusive sample count "
                  "and closure points");
          if (h)
            require(e->points[2 * (e->count - 1)] == e->points[0] &&
                        e->points[2 * (e->count - 1) + 1] == e->points[1],
                    "skewed chord/pie appends first point for closure");
          if (h == 2)
            require(e->points[2 * (e->count - 2)] == 100 &&
                        e->points[2 * (e->count - 2) + 1] == 100,
                    "skewed pie appends center even for full/empty sweeps");
        }
        printf("%c border=%u %u..%u -> %u..%u: %s n=%u first=%d,%d "
               "lastArc=%d,%d\n",
               skew_names[h], border, pairs[n][0], pairs[n][1], a, b,
               recorded[0].kind, recorded[0].count, recorded[0].points[0],
               recorded[0].points[1], recorded[0].points[2 * (generated - 1)],
               recorded[0].points[2 * (generated - 1) + 1]);
      }
  printf("PASS: %u arc normalization/consumer checks\n", checks);
  return 0;
}
