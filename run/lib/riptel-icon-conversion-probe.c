/* Original RIPtel legacy converter with memory-only file/Win32 services.
 * Keep original close/fstat control flow: a successful stat stand-in would
 * conceal the shipped timestamp defect. No GUI or host file mutation. */
#include "riptel-probe-image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
static unsigned char context[512], palette[0x900], fd_table[32 * 8];
static unsigned char source[65536], target[131072];
static unsigned source_size, target_size, positions[5];
static unsigned source_exists, target_exists, reads, writes, dialogs, reports;
static unsigned fail_read, fail_write, fail_open_source, fail_open_target;
static unsigned fail_delete_source, fail_delete_target, fail_times;
static unsigned clock_attempts, closed_source, closed_target, removed_source;
static char source_name[128], target_name[128], events[2048];
static unsigned event_count;
static void event(char ch) {
  require(event_count + 1 < sizeof events, "event capture bound");
  events[event_count++] = ch;
  events[event_count] = 0;
}
static int open_file(const char *name, unsigned flags, ...) {
  unsigned fd;
  if (flags == 0x8000) {
    event('S');
    require(!strcmp(name, source_name), "source name is unchanged");
    if (fail_open_source || !source_exists)
      return -1;
    fd = 3;
  } else {
    event('O');
    require(flags == 0x8101 && !strcmp(name, target_name),
            "destination opens binary write/create without truncation");
    if (fail_open_target)
      return -1;
    if (!target_exists)
      target_size = 0;
    target_exists = 1;
    fd = 4;
  }
  positions[fd] = 0;
  put32(fd_table + fd * 8, fd + 100);
  fd_table[fd * 8 + 4] = 1;
  return fd;
}
static int file_length(unsigned fd) {
  require(fd == 3 && fd_table[fd * 8 + 4], "file length uses live source");
  return source_size;
}
static int read_file(unsigned fd, void *dst, unsigned size) {
  require(fd == 3 && fd_table[fd * 8 + 4] && size <= sizeof source,
          "read uses live source and bounded count");
  ++reads;
  if (reads == fail_read)
    return -1;
  unsigned available = source_size - positions[fd];
  unsigned actual = size < available ? size : available;
  memcpy(dst, source + positions[fd], actual);
  positions[fd] += actual;
  return actual;
}
static int seek_file(unsigned fd, int offset, unsigned origin) {
  require(fd == 3 && origin == 0 && offset >= 0 &&
              (unsigned)offset <= source_size,
          "row seeks stay inside the source");
  positions[fd] = offset;
  return offset;
}
static int write_file(unsigned fd, const void *src, unsigned size) {
  require(fd == 4 && fd_table[fd * 8 + 4] &&
              positions[fd] + size <= sizeof target,
          "write uses live destination and bounded count");
  ++writes;
  if (writes == fail_write)
    return -1;
  memcpy(target + positions[fd], src, size);
  positions[fd] += size;
  if (positions[fd] > target_size)
    target_size = positions[fd];
  return size;
}
static int commit_file(unsigned fd) {
  require(fd == 3 && fd_table[fd * 8 + 4],
          "converter flushes the source before closing");
  event('F');
  return 0;
}
static unsigned __attribute__((stdcall)) close_handle(unsigned handle) {
  require(handle == 103 || handle == 104, "close has a fixture handle");
  if (handle == 103) {
    event('s');
    ++closed_source;
  } else {
    event('o');
    ++closed_target;
  }
  return 1;
}
static unsigned __attribute__((stdcall)) delete_file(const char *name) {
  if (!strcmp(name, source_name)) {
    event('D');
    ++removed_source;
    if (fail_delete_source)
      return 0;
    source_exists = 0;
    return 1;
  }
  require(!strcmp(name, target_name), "delete uses source or destination name");
  event('d');
  if (fail_delete_target)
    return 0;
  target_exists = 0;
  return 1;
}
static unsigned __attribute__((stdcall)) last_error(void) { return 5; }
static int apply_times(const char *name, const unsigned *times) {
  event('T');
  ++clock_attempts;
  require(!strcmp(name, target_name) && times,
          "utime attempt addresses the converted destination");
  require(!fd_table[3 * 8 + 4] && !fd_table[4 * 8 + 4] &&
              u32(image + 0x878bc) == 9,
          "original fstat has failed EBADF after both descriptors closed");
  /* Contents are uninitialized after failed original fstat; do not provide
   * fake source times or pretend their values are reproducible. */
  return fail_times ? -1 : 0;
}
static unsigned __attribute__((stdcall))
dialog(void *owner, const char *text, const char *caption, unsigned flags) {
  (void)owner;
  (void)text;
  (void)caption;
  (void)flags;
  ++dialogs;
  return 1;
}
static void report(void *ctx, unsigned a, unsigned b, unsigned c,
                   const char *name) {
  (void)ctx;
  (void)a;
  (void)b;
  (void)c;
  (void)name;
  ++reports;
}
static unsigned lookup(void *ctx, char *name, unsigned mode,
                       unsigned retrieve) {
  require(ctx == context && !strcmp(name, source_name) && mode == 0x33 &&
              retrieve == 0,
          "conversion export performs non-retrieving lookup of original name");
  return 0;
}
static void reset(unsigned width, unsigned height) {
  memset(context, 0, sizeof context);
  memset(palette, 0x37, sizeof palette);
  put16(context, 0x9df3);
  pointer(context + 0x1a, palette);
  memset(fd_table, 0, sizeof fd_table);
  put32(image + 0x958d4, 32);
  pointer(image + 0x958e0, fd_table);
  put32(image + 0x878bc, 0);
  memset(source, 0, sizeof source);
  memset(target, 0xa5, sizeof target);
  put16(source, width - 1);
  put16(source + 2, height - 1);
  source_size = 4 + height * 4 * ((width + 7) / 8) + 2;
  source_exists = 1;
  target_exists = target_size = reads = writes = dialogs = reports = 0;
  clock_attempts = closed_source = closed_target = removed_source = 0;
  fail_read = fail_write = fail_open_source = fail_open_target = 0;
  fail_delete_source = fail_delete_target = fail_times = 0;
  event_count = 0;
  events[0] = 0;
  strcpy(source_name, "C:\\ICONS\\TEST.ICN");
  strcpy(target_name, "C:\\ICONS\\TEST.BMP");
}
static unsigned convert(void) {
  return ((unsigned (*)(void *, const char *, const char *))(image + 0x2fd73))(
             context, source_name, target_name) &
         0xffff;
}
static const unsigned ega_rgb[] = {0x000000, 0x0000aa, 0x00aa00, 0x00aaaa,
                                   0xaa0000, 0xaa00aa, 0xaa5500, 0xaaaaaa,
                                   0x555555, 0x5555ff, 0x55ff55, 0x55ffff,
                                   0xff5555, 0xff55ff, 0xffff55, 0xffffff};
static void palette_and_pixels(void) {
  /* Two rows expose bottom-up order, every index and partial-byte padding. */
  for (unsigned width = 1; width <= 32; ++width) {
    for (unsigned changed = 0; changed < 2; ++changed) {
      reset(width, 2);
      memset(palette, changed ? 0xff : 0, sizeof palette);
      unsigned plane_bytes = (width + 7) / 8;
      for (unsigned y = 0; y < 2; ++y)
        for (unsigned x = 0; x < width; ++x) {
          unsigned pixel = (x + y * 7) % 16;
          for (unsigned plane = 0; plane < 4; ++plane)
            if (pixel & (8 >> plane))
              source[4 + y * 4 * plane_bytes + plane * plane_bytes + x / 8] |=
                  0x80 >> (x % 8);
        }
      require(convert() == 1, "valid legacy icon converts successfully");
      require(!dialogs && !reports && target_exists && !source_exists,
              "success produces destination and deletes source");
      if (strcmp(events, "SOFosTD"))
        fprintf(stderr, "observed events: %s\n", events);
      require(!strcmp(events, "SOFosTD"),
              "success order: open/create/flush/closes/utime/delete");
      unsigned stride = ((width + 1) / 2 + 3) & ~3u;
      require(target_size == 118 + stride * 2 && target[0] == 'B' &&
                  target[1] == 'M' && u32(target + 2) == target_size &&
                  u32(target + 10) == 118 && u32(target + 14) == 40 &&
                  u32(target + 18) == width && u32(target + 22) == 2 &&
                  u16(target + 26) == 1 && u16(target + 28) == 4 &&
                  u32(target + 30) == 0 && u32(target + 46) == 16,
              "original output is a 4-bit BI_RGB BMP with sixteen RGBQUADs");
      for (unsigned i = 0; i < 16; ++i)
        require(u32(target + 54 + i * 4) == ega_rgb[i],
                "conversion uses fixed default EGA RGB and reserved zero");
      for (unsigned y = 0; y < 2; ++y)
        for (unsigned x = 0; x < width; ++x) {
          unsigned byte = target[118 + (1 - y) * stride + x / 2];
          unsigned pixel = (x & 1) ? byte & 15 : byte >> 4;
          require(pixel == (x + y * 7) % 16,
                  "conversion preserves plane indexes in bottom-up BMP rows");
        }
    }
  }
}
static void effects_and_failures(void) {
  unsigned char stat_buffer[36];
  reset(16, 2);
  memset(stat_buffer, 0xa5, sizeof stat_buffer);
  require(((int (*)(unsigned, void *))(image + 0x6bf40))(3, stat_buffer) ==
                  -1 &&
              u32(image + 0x878bc) == 9,
          "original fstat rejects a closed descriptor with EBADF");
  for (unsigned i = 0; i < sizeof stat_buffer; ++i)
    require(stat_buffer[i] == 0xa5,
            "failed original fstat leaves stat untouched");
  for (unsigned which = 0; which < 3; ++which) {
    reset(16, 2);
    fail_times = which == 0;
    fail_delete_source = which == 1;
    fail_delete_target = which == 2;
    require(convert() == 1 && clock_attempts == 1 && removed_source == 1,
            "time/delete failure does not change successful conversion status");
    require(source_exists == (which == 1),
            "source survives only when its deletion service fails");
  }
  reset(16, 2);
  target_exists = 1;
  target_size = 1000;
  require(convert() == 1 && target_size == 1000 && u32(target + 2) == 134,
          "existing destination retains its old tail after shorter output");
  for (unsigned which = 0; which < 2; ++which) {
    reset(16, 2);
    fail_open_source = which == 0;
    fail_open_target = which == 1;
    require(convert() == 0 && source_exists && !removed_source &&
                !clock_attempts,
            "open failure retains the source and skips timestamp/delete tail");
  }
  for (unsigned failed = 1; failed <= 5; ++failed) {
    reset(16, 2);
    fail_write = failed;
    require(
        convert() == 0 && source_exists && !target_exists && dialogs == 1,
        "every output header/palette/row write failure removes output only");
  }
  for (unsigned failed = 1; failed <= 9; ++failed) {
    reset(16, 2);
    fail_read = failed;
    require(convert() == 0 && source_exists && !target_exists && dialogs == 1,
            "every input header/plane read failure removes output only");
  }
  const char *extensions[] = {"ICN", "HIC", "MSK"};
  for (unsigned i = 0; i < 3; ++i) {
    reset(16, 2);
    snprintf(source_name, sizeof source_name, "C:\\ICONS\\TEST.%s",
             extensions[i]);
    ((void (*)(void *, const char *, unsigned))(image + 0x6a3c))(
        context, source_name, 0);
    require(!dialogs && !reports && target_exists && !source_exists,
            "original export converts each legacy extension to BMP");
    require(!strcmp(events, "SOFosTD"), "export uses the same converter tail");
  }
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  load_image(argv[1]);
  hook(0x6b560, (void (*)(void))open_file);
  hook(0x75790, (void (*)(void))file_length);
  hook(0x6b1c0, (void (*)(void))read_file);
  hook(0x6b100, (void (*)(void))seek_file);
  hook(0x6aed0, (void (*)(void))write_file);
  hook(0x6ae60, (void (*)(void))commit_file);
  hook(0x6bd50, (void (*)(void))apply_times);
  hook(0x38ddf, (void (*)(void))report);
  hook(0x68d70, (void (*)(void))memset);
  hook(0x68910, (void (*)(void))strcpy);
  hook(0x65431, (void (*)(void))strcpy);
  hook(0x65442, (void (*)(void))strcat);
  hook(0x654a8, (void (*)(void))strlen);
  hook(0x3f80e, (void (*)(void))lookup);
  pointer(image + 0x965cc, (void *)close_handle);
  pointer(image + 0x966f4, (void *)dialog);
  pointer(image + 0x965dc, (void *)delete_file);
  pointer(image + 0x96600, (void *)last_error);
  palette_and_pixels();
  effects_and_failures();
  printf("PASS: %u legacy-icon conversion checks\n", checks);
  return 0;
}
