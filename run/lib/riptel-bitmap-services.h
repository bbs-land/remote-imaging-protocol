/* Controlled Win32/GDI boundary for original bitmap-pipeline observations.
 * RGB surfaces model documented API raster operations, not an original GDI
 * implementation. Original RIPtel decoding/remapping/mask/merge still executes.
 */
struct bitmap_surface {
  unsigned w, h, selected;
  uint32_t rgb[256];
};
static struct bitmap_surface surfaces[64];
static unsigned surface_count, file_cursor, file_size, opens, gdi_calls;
static unsigned char file_bytes[4096], renderer_entry[5];
static unsigned char active_palette[256 * 4]; /* R,G,B,flags */
struct memory_header {
  unsigned bytes;
};
static void *__attribute__((stdcall)) global_allocate(unsigned flags,
                                                      unsigned n) {
  (void)flags;
  struct memory_header *p = calloc(1, sizeof *p + n);
  require(p != NULL, "controlled global allocation");
  p->bytes = n;
  return p + 1;
}
static unsigned __attribute__((stdcall)) global_size(void *p) {
  return ((struct memory_header *)p)[-1].bytes;
}
static void *__attribute__((stdcall)) global_resize(void *p, unsigned n,
                                                    unsigned flags) {
  (void)flags;
  struct memory_header *old = (struct memory_header *)p - 1;
  unsigned previous = old->bytes;
  struct memory_header *q = realloc(old, sizeof *q + n);
  require(q != NULL, "controlled global resize");
  q->bytes = n;
  if (n > previous)
    memset((unsigned char *)(q + 1) + previous, 0, n - previous);
  return q + 1;
}
static void *__attribute__((stdcall)) global_free(void *p) {
  if (p)
    free((struct memory_header *)p - 1);
  return NULL;
}
static void *open_fixture(const char *name, const char *mode) {
  (void)mode;
  ++opens;
  require(!strcmp(name, "MASK.BMM") || !strcmp(name, "SOURCE.BMP"),
          "original renderer opens only requested filename");
  file_cursor = 0;
  return file_size ? file_bytes : NULL;
}
static unsigned read_fixture(void *dst, unsigned size, unsigned count,
                             void *f) {
  (void)f;
  unsigned n = size * count;
  if (n > file_size - file_cursor)
    n = file_size - file_cursor;
  memcpy(dst, file_bytes + file_cursor, n);
  file_cursor += n;
  return size ? n / size : 0;
}
static unsigned tell_fixture(void *f) {
  (void)f;
  return file_cursor;
}
static int seek_fixture(void *f, int offset, int whence) {
  (void)f;
  file_cursor = (whence == 2   ? file_size
                 : whence == 1 ? file_cursor
                               : 0) +
                offset;
  require(file_cursor <= file_size, "controlled file seek");
  return 0;
}
static unsigned pixel_stride(const unsigned char *info) {
  return ((u32(info + 4) * u16(info + 14) + 31) / 32) * 4;
}
static uint32_t pixel_rgb(const unsigned char *info, const unsigned char *bits,
                          unsigned x, unsigned y) {
  unsigned depth = u16(info + 14), height = u32(info + 8);
  const unsigned char *row = bits + (height - 1 - y) * pixel_stride(info);
  unsigned index;
  if (depth == 1)
    index = (row[x / 8] >> (7 - x % 8)) & 1;
  else if (depth == 4)
    index = (row[x / 2] >> (x % 2 ? 0 : 4)) & 15;
  else if (depth == 8)
    index = row[x];
  else if (depth == 16) {
    unsigned value = u16(row + 2 * x);
    return (((value >> 10) & 31) * 255 / 31 << 16) |
           (((value >> 5) & 31) * 255 / 31 << 8) | ((value & 31) * 255 / 31);
  } else {
    require(depth == 24 || depth == 32, "controlled RGB depth");
    const unsigned char *p = row + x * (depth / 8);
    return (p[2] << 16) | (p[1] << 8) | p[0];
  }
  const unsigned char *p = info + u32(info) + index * 4;
  return (p[2] << 16) | (p[1] << 8) | p[0];
}
static uint32_t raster(unsigned op, uint32_t src, uint32_t dst) {
  switch (op) {
  case 0xcc0020:
    return src;
  case 0x660046:
    return (src ^ dst) & 0xffffff;
  case 0xee0086:
    return (src | dst) & 0xffffff;
  case 0x8800c6:
    return src & dst;
  case 0x330008:
    return ~src & 0xffffff;
  default:
    require(0, "known GDI raster operation");
    return 0;
  }
}
static struct bitmap_surface *surface(unsigned id) {
  if (id >= 1000)
    id -= 1000;
  require(id && id <= surface_count, "controlled surface handle");
  return &surfaces[id];
}
static unsigned __attribute__((stdcall))
stretch_dib(unsigned dc, int dx, int dy, int dw, int dh, int sx, int sy, int sw,
            int sh, const void *bits, const void *info, unsigned usage,
            unsigned op) {
  ++gdi_calls;
  require(usage == 0 && dw > 0 && dh > 0, "RGB GDI fixture coordinates");
  struct bitmap_surface *d = surface(dc);
  for (int y = 0; y < dh; ++y)
    for (int x = 0; x < dw; ++x) {
      if (dx + x < 0 || dy + y < 0 || (unsigned)(dx + x) >= d->w ||
          (unsigned)(dy + y) >= d->h)
        continue;
      unsigned at = (dy + y) * d->w + dx + x;
      d->rgb[at] =
          raster(op, pixel_rgb(info, bits, sx + x * sw / dw, sy + y * sh / dh),
                 d->rgb[at]);
    }
  return dh;
}
static unsigned __attribute__((stdcall))
stretch_surface(unsigned dest, int dx, int dy, int dw, int dh, unsigned src,
                int sx, int sy, int sw, int sh, unsigned op) {
  struct bitmap_surface *d = surface(dest), *s = surface(src);
  require(dw > 0 && dh > 0, "controlled surface stretch size");
  for (int y = 0; y < dh; ++y)
    for (int x = 0; x < dw; ++x) {
      if (dx + x < 0 || dy + y < 0 || (unsigned)(dx + x) >= d->w ||
          (unsigned)(dy + y) >= d->h)
        continue;
      unsigned at = (dy + y) * d->w + dx + x;
      d->rgb[at] = raster(
          op, s->rgb[(sy + y * sh / dh) * s->w + sx + x * sw / dw], d->rgb[at]);
    }
  return 1;
}
static unsigned __attribute__((stdcall)) copy_surface(unsigned dest, int dx,
                                                      int dy, int w, int h,
                                                      unsigned src, int sx,
                                                      int sy, unsigned op) {
  return stretch_surface(dest, dx, dy, w, h, src, sx, sy, w, h, op);
}
static unsigned create_surface(unsigned reference, unsigned w, unsigned h,
                               unsigned bits, unsigned planes,
                               const unsigned char *selection, unsigned *dc,
                               unsigned *bitmap, unsigned pal, unsigned mask) {
  (void)reference;
  (void)bits;
  (void)planes;
  (void)pal;
  (void)mask;
  /* The original helper reads only the signed low word of these slots. */
  w = (short)w;
  h = (short)h;
  require(w && h && w * h <= 256 && surface_count < 63,
          "temporary surface size");
  unsigned id = ++surface_count;
  surfaces[id].selected = id + 1000;
  surfaces[id].w = w;
  surfaces[id].h = h;
  memset(surfaces[id].rgb, 0, sizeof surfaces[id].rgb);
  if (selection)
    for (unsigned y = 0; y < h; ++y)
      for (unsigned x = 0; x < w; ++x)
        surfaces[id].rgb[y * w + x] =
            selection[(h - 1 - y) * ((w + 3) & ~3) + x] ? 0xffffff : 0;
  *dc = id;
  *bitmap = id + 1000;
  return 1; /* Previous selected bitmap handle, not the newly created one. */
}
static unsigned __attribute__((stdcall)) select_object(unsigned dc,
                                                       unsigned obj) {
  struct bitmap_surface *s = surface(dc);
  unsigned previous = s->selected;
  s->selected = obj;
  return previous;
}
static unsigned __attribute__((stdcall)) destroy_object(unsigned obj) {
  (void)obj;
  return 1;
}
static unsigned __attribute__((stdcall)) two_arguments(unsigned a, unsigned b) {
  (void)a;
  (void)b;
  return 1;
}
static unsigned __attribute__((stdcall))
select_palette(unsigned dc, unsigned pal, unsigned force) {
  (void)dc;
  (void)pal;
  (void)force;
  return 1;
}
static unsigned __attribute__((stdcall)) set_rect(int *r, int x, int y, int x1,
                                                  int y1) {
  r[0] = x;
  r[1] = y;
  r[2] = x1;
  r[3] = y1;
  return 1;
}
static unsigned __attribute__((stdcall))
palette_entries(unsigned pal, unsigned first, unsigned count, void *dest) {
  (void)pal;
  if (count > 256 - first)
    count = 256 - first;
  memcpy(dest, active_palette + first * 4, count * 4);
  return count;
}
static unsigned __attribute__((stdcall)) nearest_index(unsigned pal,
                                                       unsigned rgb) {
  (void)pal;
  unsigned best = 0, distance = ~0u;
  for (unsigned n = 0; n < 256; ++n) {
    int r = (rgb & 255) - active_palette[n * 4],
        g = ((rgb >> 8) & 255) - active_palette[n * 4 + 1],
        b = ((rgb >> 16) & 255) - active_palette[n * 4 + 2];
    unsigned next = r * r + g * g + b * b;
    if (next < distance) {
      distance = next;
      best = n;
    }
  }
  return best;
}
static void *surface_dib(unsigned bitmap, unsigned pal) {
  (void)pal;
  struct bitmap_surface *s = surface(bitmap);
  unsigned stride = (s->w * 3 + 3) & ~3u;
  unsigned char *p = global_allocate(0, 40 + stride * s->h);
  put32(p, 40);
  put32(p + 4, s->w);
  put32(p + 8, s->h);
  put16(p + 12, 1);
  put16(p + 14, 24);
  for (unsigned y = 0; y < s->h; ++y)
    for (unsigned x = 0; x < s->w; ++x) {
      unsigned char *at = p + 40 + (s->h - 1 - y) * stride + x * 3;
      uint32_t v = s->rgb[y * s->w + x];
      at[0] = v;
      at[1] = v >> 8;
      at[2] = v >> 16;
    }
  return p;
}
static unsigned __attribute__((stdcall))
get_dib_bits(unsigned dc, unsigned bitmap, unsigned first, unsigned count,
             unsigned char *bits, unsigned char *info, unsigned usage) {
  (void)dc;
  (void)first;
  (void)usage;
  struct bitmap_surface *s = surface(bitmap);
  require(u16(info + 14) == 8, "mask readback is an eight-bit black/white DIB");
  unsigned stride = (s->w + 3) & ~3u;
  for (unsigned y = 0; y < count; ++y)
    for (unsigned x = 0; x < s->w; ++x)
      bits[y * stride + x] = s->rgb[(s->h - 1 - y) * s->w + x] ? 1 : 0;
  return count;
}
static void renderer_prepare(unsigned depth) {
  prepare();
  put32(context + 0x62, 1);
  put32(context + 0x6a, 1);
  put32(palette + 0x82d, 1);
  memset(surfaces, 0, sizeof surfaces);
  surface_count = 1;
  surfaces[1].w = 4;
  surfaces[1].h = 1;
  for (unsigned n = 0; n < 4; ++n)
    surfaces[1].rgb[n] = 0x123456;
  for (unsigned n = 0; n < 256; ++n) {
    active_palette[4 * n] = active_palette[4 * n + 1] =
        active_palette[4 * n + 2] = n;
    active_palette[4 * n + 3] = 0;
  }
  header(depth, 4, 1);
  unsigned colors = depth <= 8 ? 1u << depth : 0, bytes = pixel_stride(dib);
  file_size = 14 + 40 + colors * 4 + bytes;
  memset(file_bytes, 0, sizeof file_bytes);
  put16(file_bytes, 0x4d42);
  put32(file_bytes + 2, file_size);
  put32(file_bytes + 10, 14 + 40 + colors * 4);
  memcpy(file_bytes + 14, dib, 40);
  for (unsigned n = 0; n < colors; ++n) {
    unsigned gray = n == 0 ? 255 : n == 1 ? 0 : n;
    file_bytes[54 + 4 * n] = file_bytes[55 + 4 * n] = file_bytes[56 + 4 * n] =
        gray;
  }
  unsigned char *p = file_bytes + 54 + colors * 4;
  if (depth == 1)
    p[0] = 0x50;
  else if (depth == 4) {
    p[0] = 0x01;
    p[1] = 0x01;
  } else if (depth == 8) {
    p[0] = 0;
    p[1] = 1;
    p[2] = 0;
    p[3] = 1;
  } else
    for (unsigned n = 0; n < 4; ++n) {
      if (depth == 16)
        put16(p + 2 * n, n % 2 ? 0 : 0x7fff);
      else
        memset(p + n * (depth / 8), n % 2 ? 0 : 255, depth / 8);
    }
  opens = gdi_calls = 0;
}
static unsigned render_named(unsigned op, int key, const char *name, int w,
                             int h) {
  int rect[4] = {0};
  return ((unsigned (*)(void *, unsigned, const char *, int, int, int, int,
                        unsigned, unsigned, unsigned, int, unsigned, void *,
                        unsigned, unsigned, unsigned))(image + 0x4a410))(
             context, 1, name, 0, 0, w, h, 0, 2, 0, key, 1, rect, op, 0, 0) &
         0xffff;
}
static unsigned render(unsigned op, int key) {
  return render_named(op, key, "MASK.BMM", 4, 1);
}
static void renderer_checks(void) {
  memcpy(image + 0x4a410, renderer_entry, 5);
  hook(0x68f90, (void (*)(void))open_fixture);
  hook(0x6c2b0, (void (*)(void))read_fixture);
  hook(0x6ce30, (void (*)(void))tell_fixture);
  hook(0x6cd90, (void (*)(void))seek_fixture);
  hook(0x68ec0, (void (*)(void))zero);
  hook(0x68dd0, (void (*)(void))strlen);
  hook(0x68910, (void (*)(void))strcpy);
  hook(0x6b9a0, (void (*)(void))memmove);
  hook(0x13c69, noop);
  hook(0x48d60, (void (*)(void))create_surface);
  hook(0x4de5f, (void (*)(void))surface_dib);
  pointer(image + 0x964dc, (void *)global_allocate);
  pointer(image + 0x964ec, (void *)global_free);
  pointer(image + 0x964b8, (void *)global_size);
  pointer(image + 0x964b4, (void *)global_resize);
  pointer(image + 0x96454, (void *)stretch_dib);
  pointer(image + 0x96488, (void *)stretch_surface);
  pointer(image + 0x963e4, (void *)copy_surface);
  pointer(image + 0x963c0, (void *)select_object);
  pointer(image + 0x963d0, (void *)destroy_object);
  pointer(image + 0x964ac, (void *)destroy_object);
  pointer(image + 0x963e0, (void *)destroy_object);
  pointer(image + 0x963f4, (void *)select_palette);
  pointer(image + 0x96458, (void *)two_arguments);
  pointer(image + 0x96618, (void *)set_rect);
  pointer(image + 0x9645c, (void *)palette_entries);
  pointer(image + 0x96420, (void *)nearest_index);
  pointer(image + 0x96424, (void *)get_dib_bits);
  unsigned depths[] = {1, 4, 8, 16, 24, 32};
  unsigned operations[] = {0xcc0020, 0x660046, 0xee0086, 0x8800c6, 0x330008};
  for (unsigned d = 0; d < 6; ++d)
    for (unsigned op = 0; op < 5; ++op) {
      renderer_prepare(depths[d]);
      require(render(operations[op], -1) == 0 && errors == 0,
              "original renderer accepts ordinary BMP depth/mode fixture");
      require(opens == 1 && gdi_calls != 0,
              "original renderer reads one file and reaches GDI");
      for (unsigned x = 0; x < 4; ++x)
        require(surfaces[1].rgb[x] ==
                    raster(operations[op], x % 2 ? 0 : 0xffffff, 0x123456),
                "ordinary BMM ROP uses palette-resolved pixels, not implicit "
                "transparency");
    }
  for (unsigned d = 0; d < 6; ++d)
    for (unsigned op = 0; op < 5; ++op)
      for (unsigned key_white = 0; key_white < 2; ++key_white) {
        unsigned depth = depths[d];
        renderer_prepare(depth);
        /* A 1-bpp source keeps its file indices. Other indexed depths are
         * translated into the active palette before classification. */
        int key = depth == 1 ? !key_white : key_white ? 255 : 0;
        unsigned result = render(operations[op], key);
        if (depth == 16) {
          require(result == 1 && errors == 1,
                  "16-bit color key rejects source without drawing");
          for (unsigned x = 0; x < 4; ++x)
            require(surfaces[1].rgb[x] == 0x123456,
                    "16-bit failed color key preserves background");
          continue;
        }
        require(result == 0 && !errors,
                "original color-key renderer accepts depth/mode fixture");
        require(opens == 1, "color key still opens only the named file");
        for (unsigned x = 0; x < 4; ++x) {
          uint32_t src = x % 2 ? 0 : 0xffffff;
          int selected = depth != 32 && (key_white ? x % 2 == 0 : x % 2 != 0);
          require(
              surfaces[1].rgb[x] ==
                  (selected ? 0x123456 : raster(operations[op], src, 0x123456)),
              "color-key equality retains background after requested ROP; "
              "32-bit original classifier selects nothing");
        }
      }
  /* Different sizes do not trigger a companion-mask lookup or dimension
   * check. Original geometry and internal mask scaling still execute. */
  unsigned widths[] = {2, 8};
  for (unsigned n = 0; n < 2; ++n)
    for (unsigned op = 0; op < 5; ++op)
      for (unsigned keyed = 0; keyed < 2; ++keyed) {
        renderer_prepare(24);
        surfaces[1].w = 8;
        for (unsigned x = 0; x < 8; ++x)
          surfaces[1].rgb[x] = 0x123456;
        unsigned w = widths[n];
        require(render_named(operations[op], keyed ? 0 : -1, "MASK.BMM", w,
                             1) == 0 &&
                    !errors && opens == 1,
                "differently sized explicit bitmap uses independent geometry");
        for (unsigned x = 0; x < 8; ++x) {
          unsigned src_x = x * 4 / w;
          uint32_t src = src_x % 2 ? 0 : 0xffffff;
          uint32_t expected = x >= w || (keyed && src == 0)
                                  ? 0x123456
                                  : raster(operations[op], src, 0x123456);
          require(surfaces[1].rgb[x] == expected,
                  "source and generated color-key mask scale to same target");
        }
      }
  renderer_prepare(24);
  file_size = 0;
  require(render(0xcc0020, -1) == 1 && opens == 1 && !gdi_calls && !errors,
          "missing explicitly requested BMM fails before drawing");
  for (unsigned x = 0; x < 4; ++x)
    require(surfaces[1].rgb[x] == 0x123456,
            "missing explicit file does not change destination in renderer");
  renderer_prepare(1);
  require(render(0x8800c6, -1) == 0, "explicit AND mask succeeds");
  uint32_t masked[4];
  memcpy(masked, surfaces[1].rgb, sizeof masked);
  renderer_prepare(24);
  memcpy(surfaces[1].rgb, masked, sizeof masked);
  opens = 1; /* Two separately supplied file fixtures, same drawing surface. */
  uint32_t icon[] = {0, 0xf0f00f, 0, 0x0f0ff0};
  for (unsigned x = 0; x < 4; ++x) {
    unsigned char *at = file_bytes + 54 + 3 * x;
    at[0] = icon[x];
    at[1] = icon[x] >> 8;
    at[2] = icon[x] >> 16;
  }
  require(render_named(0xee0086, -1, "SOURCE.BMP", 4, 1) == 0 && opens == 2,
          "manual composition requires second explicit source load");
  for (unsigned x = 0; x < 4; ++x)
    require(surfaces[1].rgb[x] == (x % 2 ? icon[x] : 0x123456),
            "manual AND then OR oracle retains white-mask pixels");
}
