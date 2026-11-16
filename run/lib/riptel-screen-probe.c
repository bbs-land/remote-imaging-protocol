/* Original RIPtel screen serializers, codecs and backup manager. Virtual CRT
 * files and controlled indexed GDI operations; no Windows pixel-parity claim.
 */
#include "riptel-probe-image.h"
#include <ctype.h>
#include <stdarg.h>
#include <strings.h>
static unsigned char ctx[512], palettes[0x900], colors[36 * 0x302], ports[128],
    port_records[36 * 0x78], variables[0x700], strings[128],
    defaults_system[0x300], backups[8 * 0xb17], style_system[16],
    styles[36 * 0x61], button_system[128], buttons[36 * 0x1c], env_system[128],
    environments[36 * 16], engine[16], engine_state[0x400], callbacks[0x100];
static char params[21], result[128];
static uint16_t character_types[256];
static unsigned errors, allocations, uploads, invalidations, status_visible,
    status_calls;
static unsigned capture_source, present_target, fail_read_open;
static unsigned char hardware_palette[1024], screen_pixels[32],
    other_pixels[32];
static void formatted(char *dest, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsprintf(dest, fmt, ap);
  va_end(ap);
}
static unsigned report(void *engine, unsigned title, unsigned code, ...) {
  (void)engine;
  (void)title;
  (void)code;
  ++errors;
  return 0;
}
static const char *error_text(unsigned value) {
  (void)value;
  return "fixture failure";
}
static unsigned allocate(void *target, unsigned size) {
  require(size && size <= 8192, "bounded original bitmap allocation request");
  void *p = calloc(1, size);
  require(p != NULL, "fixture bitmap allocation");
  pointer(target, p);
  ++allocations;
  return 1;
}
static void release(void *target) {
  void *p = (void *)(uintptr_t)u32(target);
  if (p) {
    require(allocations != 0, "owned bitmap allocation release");
    free(p);
    --allocations;
  }
  pointer(target, NULL);
}
struct file {
  char name[128];
  unsigned char bytes[32768];
  unsigned exists, opened, length, position;
};
static struct file files[32];
static unsigned opens, unlinks;
static int file_index(const char *name) {
  for (unsigned n = 0; n < 32; ++n)
    if (files[n].exists && !strcmp(files[n].name, name))
      return n;
  return -1;
}
static unsigned __attribute__((stdcall)) attributes(const char *name) {
  return file_index(name) < 0 ? 0xffffffffu : 0;
}
static int file_open(const char *name, unsigned flags, unsigned mode) {
  require(mode == 0x180 && (flags == 0x8302 || flags == 0x8100),
          "original generated backup open flags/mode");
  ++opens;
  if (flags == 0x8100 && fail_read_open) {
    fail_read_open = 0;
    return -1;
  }
  int i = file_index(name);
  if (flags == 0x8100 && i < 0)
    return -1;
  if (flags == 0x8302) {
    require(strstr(name, "RIPTERM") == NULL && strlen(name) < 128,
            "generated private backup path, no RIPTERM.SAV alias path");
    if (i < 0)
      for (unsigned n = 0; n < 32; ++n)
        if (!files[n].exists) {
          i = n;
          break;
        }
    require(i >= 0, "bounded virtual backup file count");
    strcpy(files[i].name, name);
    files[i].exists = 1;
    files[i].length = 0;
  }
  require(!files[i].opened, "one owned descriptor per virtual file");
  files[i].opened = 1;
  files[i].position = 0;
  return i + 1000;
}
static struct file *descriptor(int fd) {
  require(fd >= 1000 && fd < 1032 && files[fd - 1000].opened,
          "original stream uses an open owned descriptor");
  return files + fd - 1000;
}
static int file_close(int fd) {
  descriptor(fd)->opened = 0;
  return 0;
}
static int file_unlink(const char *name) {
  int i = file_index(name);
  ++unlinks;
  if (i < 0)
    return -1;
  require(!files[i].opened, "original cleanup closes before unlinking");
  files[i].exists = 0;
  return 0;
}
static unsigned file_write(int fd, const void *p, unsigned n) {
  struct file *f = descriptor(fd);
  require(f->position + n <= sizeof f->bytes,
          "bounded original metadata/pixel write");
  memcpy(f->bytes + f->position, p, n);
  f->position += n;
  if (f->length < f->position)
    f->length = f->position;
  return n;
}
static unsigned file_read(int fd, void *p, unsigned n) {
  struct file *f = descriptor(fd);
  require(f->position + n <= f->length, "bounded original metadata/pixel read");
  memcpy(p, f->bytes + f->position, n);
  f->position += n;
  return n;
}
struct bitmap {
  unsigned used, width, height;
  unsigned char pixels[32];
};
struct dc {
  unsigned used, bitmap;
};
static struct bitmap bitmaps[4];
static struct dc dcs[4];
static unsigned __attribute__((stdcall)) create_bitmap(unsigned dc, unsigned w,
                                                       unsigned h) {
  (void)dc;
  w &= 0xffff;
  h &= 0xffff;
  require(w == 8 && h == 4,
          "physical screen dimensions, independent of viewport/current port");
  for (unsigned n = 0; n < 4; ++n)
    if (!bitmaps[n].used) {
      memset(bitmaps + n, 0, sizeof bitmaps[n]);
      bitmaps[n].used = 1;
      bitmaps[n].width = w;
      bitmaps[n].height = h;
      return 200 + n;
    }
  require(0, "bounded GDI bitmap count");
  return 0;
}
static unsigned __attribute__((stdcall)) create_dc(unsigned source) {
  (void)source;
  for (unsigned n = 0; n < 4; ++n)
    if (!dcs[n].used) {
      dcs[n].used = 1;
      dcs[n].bitmap = 1;
      return 100 + n;
    }
  require(0, "bounded GDI memory DC count");
  return 0;
}
static unsigned __attribute__((stdcall)) select_object(unsigned dc,
                                                       unsigned object) {
  require(dc >= 100 && dc < 104 && dcs[dc - 100].used,
          "owned memory DC selection");
  unsigned previous = dcs[dc - 100].bitmap;
  dcs[dc - 100].bitmap = object;
  return previous;
}
static unsigned __attribute__((stdcall)) delete_object(unsigned object) {
  require(object >= 200 && object < 204 && bitmaps[object - 200].used,
          "owned bitmap deletion");
  bitmaps[object - 200].used = 0;
  return 1;
}
static unsigned __attribute__((stdcall)) delete_dc(unsigned dc) {
  require(dc >= 100 && dc < 104 && dcs[dc - 100].used,
          "owned memory DC deletion");
  dcs[dc - 100].used = 0;
  return 1;
}
static unsigned char *dc_pixels(unsigned dc) {
  if (dc == 10)
    return screen_pixels;
  if (dc == 11)
    return other_pixels;
  require(dc >= 100 && dc < 104 && dcs[dc - 100].used, "known DC pixel source");
  unsigned b = dcs[dc - 100].bitmap;
  require(b >= 200 && b < 204 && bitmaps[b - 200].used,
          "selected owned bitmap");
  return bitmaps[b - 200].pixels;
}
static unsigned __attribute__((stdcall)) bitblt(unsigned dst, int x, int y,
                                                int w, int h, unsigned src,
                                                int sx, int sy, unsigned rop) {
  require(x == 0 && y == 0 && sx == 0 && sy == 0 && w == 8 && h == 4 &&
              rop == 0xcc0020,
          "original complete-screen SRCCOPY coordinates and dimensions");
  memcpy(dc_pixels(dst), dc_pixels(src), 32);
  if (dst == 10)
    present_target = dst;
  else
    capture_source = src;
  return 1;
}
static unsigned __attribute__((stdcall)) get_object(unsigned b, unsigned size,
                                                    void *out) {
  require(b >= 200 && b < 204 && bitmaps[b - 200].used && size == 24,
          "original BITMAP query");
  memset(out, 0, size);
  put32((unsigned char *)out + 4, 8);
  put32((unsigned char *)out + 8, 4);
  put32((unsigned char *)out + 12, 8);
  put16((unsigned char *)out + 16, 1);
  put16((unsigned char *)out + 18, 8);
  return 24;
}
static unsigned __attribute__((stdcall)) get_bits(unsigned b, unsigned size,
                                                  void *out) {
  require(b >= 200 && b < 204 && bitmaps[b - 200].used && size == 32,
          "owned bitmap read");
  memcpy(out, bitmaps[b - 200].pixels, size);
  return size;
}
static unsigned __attribute__((stdcall)) set_bits(unsigned b, unsigned size,
                                                  const void *in) {
  require(b >= 200 && b < 204 && bitmaps[b - 200].used && size == 32,
          "owned bitmap load");
  memcpy(bitmaps[b - 200].pixels, in, size);
  return size;
}
static unsigned __attribute__((stdcall))
select_palette(unsigned dc, unsigned pal, unsigned background) {
  (void)dc;
  (void)background;
  require(pal == 500, "current hardware palette handle");
  return 500;
}
static unsigned __attribute__((stdcall)) realize(unsigned dc) {
  (void)dc;
  return 1;
}
static unsigned __attribute__((stdcall))
palette_entries(unsigned pal, unsigned start, unsigned count, const void *in) {
  require(pal == 500 && start == 0 && count == 256,
          "original palette upload size");
  memcpy(hardware_palette, in, 1024);
  ++uploads;
  return count;
}
static unsigned __attribute__((stdcall)) set_rect(int *r, int x, int y,
                                                  int right, int bottom) {
  memcpy(r, (int[]){x, y, right, bottom}, 16);
  return 1;
}
static void invalidate(void *c, unsigned dc, const int *r) {
  require(c == ctx && dc == 10 && r[0] == 0 && r[1] == 0 && r[2] == 8 &&
              r[3] == 4,
          "physical screen invalidation only");
  ++invalidations;
}
static unsigned status_callback(void *c, int mode) {
  require(c == ctx && mode >= -1 && mode <= 1,
          "original status callback request");
  ++status_calls;
  if (mode >= 0)
    status_visible = mode;
  return status_visible;
}
static unsigned char *palette(unsigned n) { return colors + n * 0x302; }
static unsigned char *record(int slot) {
  return backups + 6 * 0xb17 + (slot < 0 ? 0 : (slot + 1) * 0x87);
}
static unsigned family_manager(unsigned rva, unsigned family, int slot) {
  return ((unsigned (*)(void *, unsigned, int))(image + rva))(ctx, family,
                                                              slot) &
         0xffff;
}
static unsigned manager(unsigned rva, int slot) {
  return family_manager(rva, 6, slot);
}
static void action(unsigned rva, const char *slot) {
  memset(params, 0, sizeof params);
  if (slot)
    strcpy(params, slot);
  put16(variables + 0x436, 0);
  const char *v = ((const char *(*)(void *, unsigned, void *))(image + rva))(
      ctx, slot ? 1 : 0, params);
  require(!v && !u16(variables + 0x436),
          "original screen callback action succeeds syntactically");
}
static void prepare(void) {
  require(!allocations,
          "all task bitmap allocations released before next fixture");
  memset(engine, 0, sizeof engine);
  memset(engine_state, 0, sizeof engine_state);
  memset(callbacks, 0, sizeof callbacks);
  memset(ctx, 0, sizeof ctx);
  put16(ctx, 0x9df3);
  pointer(ctx + 6, engine);
  pointer(engine, engine_state);
  pointer(engine + 4, callbacks);
  pointer(callbacks + 0x38, (void *)status_callback);
  memset(palettes, 0, sizeof palettes);
  memset(ports, 0, sizeof ports);
  memset(port_records, 0, sizeof port_records);
  memset(variables, 0, sizeof variables);
  memset(strings, 0, sizeof strings);
  memset(defaults_system, 0, sizeof defaults_system);
  memset(backups, 0, sizeof backups);
  memset(files, 0, sizeof files);
  memset(bitmaps, 0, sizeof bitmaps);
  memset(dcs, 0, sizeof dcs);
  memset(style_system, 0, sizeof style_system);
  memset(styles, 0xa5, sizeof styles);
  memset(button_system, 0, sizeof button_system);
  memset(buttons, 0xb6, sizeof buttons);
  memset(env_system, 0, sizeof env_system);
  memset(environments, 0xc7, sizeof environments);
  pointer(ctx + 0x1a, palettes);
  pointer(palettes + 5, colors);
  put16(palettes + 0x82b, 256);
  put32(palettes + 0x82d, 500);
  pointer(ctx + 0x22, ports);
  pointer(ports, port_records);
  pointer(ctx + 0x26, variables);
  pointer(variables + 0x432, result);
  pointer(ctx + 0x32, strings);
  pointer(ctx + 0x72, defaults_system);
  strcpy((char *)defaults_system + 0x16, "CACHE");
  pointer(ctx + 0x3e, backups);
  put16(ctx + 0x7a, 3);
  put32(ctx + 0x62, 11);
  put32(ctx + 0x6a, 10);
  pointer(ctx + 0xa, style_system);
  pointer(style_system, styles);
  put16(style_system + 8, 5);
  pointer(ctx + 0x16, button_system);
  pointer(button_system, buttons);
  put16(button_system + 0xa, 6);
  pointer(ctx + 0xe, env_system);
  pointer(env_system, environments);
  put16(env_system + 0xa, 7);
  put16(ports + 8, 1);
  put32(ports + 0x3a, 8);
  put32(ports + 0x3e, 4);
  port_records[0] = 1;
  put32(port_records + 0x6c, 10);
  port_records[0x78] = 4;
  put32(port_records + 0x78 + 0x6c, 11);
  for (unsigned f = 0; f < 8; ++f)
    for (unsigned r = 0; r < 21; ++r)
      put32(backups + f * 0xb17 + r * 0x87 + 0x81, 0xffffffff);
  for (unsigned n = 0; n < 36; ++n) {
    for (unsigned j = 0; j < 768; ++j)
      palette(n)[j] = (n + j * 3) % 64;
    put16(palette(n) + 0x300, 2);
  }
  for (unsigned n = 0; n < 32; ++n) {
    screen_pixels[n] = n % 16;
    other_pixels[n] = 200 + n;
  }
  put16(image + 0x7de70, 0);
  errors = uploads = invalidations = opens = unlinks = capture_source =
      present_target = fail_read_open = 0;
  status_visible = status_calls = 0;
  action(0x30782, NULL); /* Actual SBARON -> engine callback/cache. */
}
static void capture_scope_checks(void) {
  const unsigned indices[] = {0, 5, 17, 35};
  for (unsigned a = 0; a < 4; ++a)
    for (unsigned b = 0; b < 4; ++b)
      for (unsigned flags = 0; flags < 4; ++flags) {
        prepare();
        unsigned p = indices[a], q = indices[b];
        palettes[4] = p;
        unsigned char rgb[768], pixels[32];
        memcpy(rgb, palette(p), 768);
        memcpy(pixels, screen_pixels, 32);
        action(0x2493a, NULL);
        int file = file_index((char *)record(-1));
        require(file >= 0 &&
                    !strcmp((char *)record(-1), "CACHE\\00300001.$$$") &&
                    files[file].length == 1578 &&
                    !memcmp(files[file].bytes, rgb, 768) &&
                    !memcmp(files[file].bytes + 778, pixels, 32) &&
                    !memcmp(files[file].bytes + 810, rgb, 768) &&
                    capture_source == 10,
                "original stream is outer RGB, bitmap header/pixels, nested "
                "RGB; source port zero");
        palettes[4] = q;
        memset(palette(q), 33, 768);
        put16(palette(q) + 0x300, flags);
        memset(screen_pixels, 0xee, 32);
        memset(other_pixels, 0xdd, 32);
        put16(ports + 8, 35);
        port_records[35 * 0x78] = 4;
        put32(port_records + 35 * 0x78 + 0x6c, 11);
        port_records[0x78] = flags % 3;
        port_records[0x78 + 0x17] = 1;
        memcpy(port_records + 0x78 + 0x3a, (int[]){1, 2, 3, 4}, 16);
        memcpy(port_records + 35 * 0x78 + 0x3a, (int[]){6, 7, 8, 9}, 16);
        unsigned char old_colors[sizeof colors], old_ports[sizeof port_records],
            old_system[sizeof ports];
        memcpy(old_colors, colors, sizeof colors);
        memcpy(old_ports, port_records, sizeof port_records);
        memcpy(old_system, ports, sizeof ports);
        action(0x3075a, NULL); /* Actual SBAROFF before restoring. */
        unsigned prior_status_calls = status_calls;
        action(0x2475d, NULL);
        require(!memcmp(screen_pixels, pixels, 32) && other_pixels[0] == 0xdd &&
                    present_target == 10 && !memcmp(palette(q), rgb, 768) &&
                    u16(palette(q) + 0x300) == flags && palettes[4] == q &&
                    uploads == 2 && invalidations == 1,
                "screen restore retargets both palette copies despite entry "
                "protection, presents to zero");
        for (unsigned i = 0; i < 256; ++i)
          for (unsigned c = 0; c < 3; ++c)
            require(hardware_palette[4 * i + c] ==
                        (rgb[3 * i + c] == 63 ? 255 : rgb[3 * i + c] * 4),
                    "original upload restores saved six-bit RGB mapping for "
                    "index pixels");

        memcpy(old_colors + q * 0x302, rgb, 768);
        require(
            !memcmp(old_colors, colors, sizeof colors) &&
                !memcmp(old_ports, port_records, sizeof port_records) &&
                !memcmp(old_system, ports, sizeof ports) &&
                u16(style_system + 8) == 5 && u16(button_system + 0xa) == 6 &&
                u16(env_system + 0xa) == 7 && styles[0] == 0xa5 &&
                buttons[0] == 0xb6 && environments[0] == 0xc7 &&
                !status_visible && !engine_state[0x30c] &&
                status_calls == prior_status_calls,
            "screen backup changes no "
            "port/viewports/selections/style/button/environment/status state");
        require(!errors && !allocations && record(-1)[0] && !files[file].opened,
                "base backup persists with descriptors and bitmap allocations "
                "closed");
        action(0x2475d, "BASE");
        require(uploads == 4 && !errors && !allocations && record(-1)[0],
                "base can restore repeatedly");
      }
}
static void manager_checks(void) {
  for (unsigned alias = 0; alias < 2; ++alias)
    for (unsigned slot = 0; slot < 10; ++slot)
      for (unsigned protected = 0; protected < 2; ++protected) {
        prepare();
        char name[8];
        snprintf(name, sizeof name, "%u", slot);
        unsigned save_rva =
            alias ? (slot ? 0x24a25 + (slot - 1) * 0x17 : 0x249e2) : 0x2493a;
        unsigned restore_rva =
            alias ? (slot ? 0x2486b + (slot - 1) * 0x17 : 0x24816) : 0x2475d;
        const char *selector = alias ? NULL : name;
        action(save_rva, selector);
        unsigned char *r = record(slot);
        unsigned char saved[32];
        memcpy(saved, screen_pixels, 32);
        if (protected)
          require(manager(0x23e84, slot) == 1,
                  "original numbered snapshot protection");
        unsigned before_opens = opens;
        screen_pixels[0] = 99;
        action(save_rva, selector);
        require(opens == before_opens + (protected ? 0 : 1) &&
                    u16(backups + 6 * 0xb17 + 0xb15) == 1,
                "protected snapshot blocks overwrite; unprotected overwrite "
                "reuses occupancy");
        action(restore_rva, selector);
        require(
            screen_pixels[0] == (protected ? saved[0] : 99) &&
                !!r[0] == protected &&
                u16(backups + 6 * 0xb17 + 0xb15) == protected && !errors &&
                !allocations,
            "numbered restore reads protected saves; only unprotected saves "
            "are consumed");
      }
  prepare();
  action(0x2493a, "2");
  fail_read_open = 1;
  screen_pixels[0] = 199;
  action(0x2475d, "2");
  require(screen_pixels[0] == 199 && !record(2)[0] &&
              u16(backups + 6 * 0xb17 + 0xb15) == 0,
          "numeric RESTORE consumes unprotected snapshot even when read-open "
          "fails");
  prepare();
  for (unsigned n = 0; n < 10; ++n) {
    screen_pixels[0] = n;
    action(0x2493a, "PUSH");
  }
  require(u16(backups + 6 * 0xb17 + 0xb13) == 10 &&
              u16(backups + 6 * 0xb17 + 0xb15) == 10,
          "original screen stack fills ten independent snapshots");
  unsigned old_opens = opens;
  action(0x2493a, "PUSH");
  require(opens == old_opens, "full screen PUSH has no file effect");
  for (unsigned n = 10; n > 0; --n) {
    action(0x2475d, "POP");
    require(screen_pixels[0] == n - 1 &&
                u16(backups + 6 * 0xb17 + 0xb13) == n - 1,
            "original stack restore is LIFO and consumes each snapshot");
  }
  require(u16(backups + 6 * 0xb17 + 0xb15) == 0 && !errors && !allocations,
          "screen stack cleanup closes and removes consumed virtual files");
}
static void palette_transaction_checks(void) {
  prepare();
  palettes[4] = 5;
  put16(palette(17) + 0x300, 3);
  unsigned char all_colors[sizeof colors], pixels[32];
  memcpy(all_colors, colors, sizeof colors);
  memcpy(pixels, screen_pixels, 32);
  require(
      family_manager(0x23bd3, 6, -1) == 1 &&
          family_manager(0x23bd3, 4, -1) == 1,
      "SAVEALL screen-then-whole-palette components use original serializers");
  palettes[4] = 17;
  memset(palette(17), 11, 768);
  memset(palette(5), 22, 768);
  memset(screen_pixels, 99, 32);
  action(0x3075a, NULL);
  unsigned old_status_calls = status_calls;
  require(family_manager(0x23ca6, 6, -1) == 1 && palettes[4] == 17 &&
              !memcmp(palette(17), all_colors + 5 * 0x302, 768),
          "RESTOREALL screen phase initially writes saved RGB into live "
          "palette 17");
  require(family_manager(0x23ca6, 4, -1) == 1 && palettes[4] == 5 &&
              !memcmp(colors, all_colors, sizeof colors) &&
              !memcmp(screen_pixels, pixels, 32) && !status_visible &&
              engine_state[0x30c] == 0 && status_calls == old_status_calls,
          "later whole-palette phase restores saved index 5/all records, "
          "including protected 17");
  require(!errors && !allocations,
          "coupled screen/palette components leave owned memory closed");
}

int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x655e2, (void (*)(void))strcasecmp);
  hook(0x68440, (void (*)(void))formatted);
  hook(0x68c10, (void (*)(void))atol);
  hook(0x6b9a0, (void (*)(void))memmove);
  hook(0x65ca3, (void (*)(void))allocate);
  hook(0x65cbf, (void (*)(void))release);
  hook(0x41b8, (void (*)(void))report);
  hook(0x6bce0, (void (*)(void))error_text);
  hook(0x6b560, (void (*)(void))file_open);
  hook(0x6b490, (void (*)(void))file_close);
  hook(0x6b480, (void (*)(void))file_unlink);
  hook(0x6aed0, (void (*)(void))file_write);
  hook(0x6b1c0, (void (*)(void))file_read);
  hook(0x447c0, noop);
  hook(0x44802, noop);
  hook(0x13c69, (void (*)(void))invalidate);
  for (unsigned i = 0; i < 256; ++i)
    character_types[i] = isdigit(i) ? 4 : 0;
  pointer(image + 0x87908, character_types);
  pointer(image + 0x964e8, (void *)attributes);
  pointer(image + 0x96460, (void *)create_bitmap);
  pointer(image + 0x964a0, (void *)create_dc);
  pointer(image + 0x963c0, (void *)select_object);
  pointer(image + 0x963d0, (void *)delete_object);
  pointer(image + 0x964ac, (void *)delete_dc);
  pointer(image + 0x963e4, (void *)bitblt);
  pointer(image + 0x963c4, (void *)get_object);
  pointer(image + 0x9646c, (void *)get_bits);
  pointer(image + 0x963bc, (void *)set_bits);
  pointer(image + 0x963f4, (void *)select_palette);
  pointer(image + 0x963e0, (void *)realize);
  pointer(image + 0x96474, (void *)palette_entries);
  pointer(image + 0x96618, (void *)set_rect);
  capture_scope_checks();
  manager_checks();
  palette_transaction_checks();
  printf("PASS: %u screen backup/manager checks\n", checks);
  return 0;
}
