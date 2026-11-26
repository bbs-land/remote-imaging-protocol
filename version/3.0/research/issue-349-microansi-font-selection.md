# Issue #349 — MicroANSI font selection

[Issue #349](https://github.com/bbs-land/remote-imaging-protocol/issues/349) asks which text-window state selects a glyph and why a smaller drawing area can change the raster for the same nominal text mode.

RealDeuce filed #349 at 15:01:26 UTC on 2026-11-26. The 15:06 UTC check found it with no comments. Branch `issue-349-microansi-font-selection` starts at fork stable `98016aef6472f265ddd11985fece4e3e8f83c3c3`. [Research backlog](issue-backlog.md).

## Sources

Use the [RIPtel 3.1 archive and extracted artifacts](../../../reference/rip-tools.md) under `rip-tools/artifacts/riptel-3.1-pcmicro/`. The driver RVAs here refer to the original `extracted/ripscrip.dll`, PE image base `0x10000000`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Client addresses are VAs in `riptel-unpacked.exe`, PE image base `0x00400000`, SHA-256 `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. The unpacking exposes the original client code and settings strings for static inspection. The worked examples read the original `extracted/ripscrip.maf`, 270,945 bytes, SHA-256 `8c0be38eee89b6935ed6f93885c14350f4a6db0a10a72a38d126811bbd090cef`. [Issue #347](issue-347-microansi-overview-counts.md) verifies all six records and thirty subrecords.

LLVM objdump 19.1.7 ran `objdump -d --no-show-raw-insn` with `--start-address` and `--stop-address` using VAs. Add the driver base to the RVAs below. Resolve strings and import thunks through the containing PE section's virtual address and raw file offset. The sequence below is a static trace through the original client and driver with byte reads from the shipped font file.

## Where the inputs come from

- **Client dimensions.** The client passes the drawing window handle at VA `0x406f3a–0x406f4f` to `RIP_InstanceInit`. The driver's initializer starts at RVA `0x5e1f`. Its `GetClientRect` call at `0x60b4` is followed by `W = right − left` in DI and `H = bottom − top` in BX at `0x60ba–0x60c4`. Those same dimensions reach the font initializer `0x47125` at the call at `0x63c7`. The intervening coarse screen classification and assignment of the default 640×350 WORLD do not supply the font dimensions.
- **User preference.** Client VA `0x40da94–0x40daa5` reads `[setup] DefaultFontSize` from `riptel.ini` via the profile getter at `0x40d9e2`, with default 2 and maximum 4. A read result above 4 is replaced by 2. The client keeps the result at VA `0x44f9d0`. Its callback 6, registered at `0x408fdb`, uses argument `−1` to read this value at `0x408cc1`. Driver `ripDefaultAnsiFont` at `0x3170d` invokes callback 6 and caches the result at configuration offset `+0x304`. Startup `0x6401 → 0x16220` uses the SOFT reset path. SOFT at `0x164de` and HARD at `0x1688c` obtain the current preference with `−1` and pass it to `0x8577`, the ordinary default-window definition helper.
- **TW mode.** The `w` handler `0x209cf` takes the supplied size 0–4 for an ordinary definition. The `b` handler selects a mode with the requested start index or its auto-size rule, using the actual cells. The active index lives at text-window system `+0x0a`. The driver stores the font index in bits `0x700` of the flags at TW record `+0x2e`. Selection `0xddfc → 0x4876d` also records the live index at ANSI state `+0x35`. It changes the active mode using the available font cache. For ordinary and extended definition and the first-use/reset rules, see the [ordinary window trace](issue-100-ordinary-text-window-state.md), [extended metrics trace](issue-101-extended-text-window-contract.md), and [live TW reset contract](../ripscrip/5.1-text-variables.md#live-text-window-reset-contract).

## Bank, subrecord and glyph

1. The client registers callback `0x3d` at VA `0x4090d4`. Its handler at `0x408d28` returns 1. The driver checks it through `ripCacheAnsiFonts` (`0x32c1f`). Font initializer `0x4716b–0x47253` compares the low words of the client width and height as unsigned values. It first tests exact pairs (640, 480), (800, 600) and (1024, 768). Then it tests `560 ≤ W < 640` and `430 ≤ H < 480` for key (639, 479), `640 ≤ W < 800` and `516 ≤ H < 600` for key (799, 599), and `880 ≤ W < 1024` and `688 ≤ H < 768` for key (1023, 767). A miss clears the MAF load flag at `0x47253`.
2. The initializer first computes fallback metrics by integer division by the five nominal column and row counts at `0x473d8–0x4745d`. It loops over modes 0–4 at `0x474bc` and calls the MAF loader `0x2d744` at `0x47536` with the chosen key and mode. The loader scans the directory for an exact dimension pair at `0x2d8a1–0x2d8b9` and reads the offset from the selected record's five-offset array at `0x2d8cd`. It reads the 50-byte header at `0x2d957` and the 255-glyph payload at `0x2da51`. The cell dimensions returned by a successful load replace the fallback values in the per-mode arrays at `0x4754b` and `0x47553`. If a load fails, the load flag is cleared for that mode and subsequent modes. With caching enabled, the fallback builder uses the nominal metrics and calls Windows `CreateFontA` at `0x476b6`.
3. The client keeps the resulting five mode caches. In the MAF path, `0x47857–0x4787e` gets the payload address for code `c` as `payload + (c − 1) × bytesPerGlyph` for `c > 0`. Code 0 skips the glyph fetch. The loop at `0x47d7f–0x47dac` covers a 16×16 grid of cells in byte order. `0x475f6–0x4763e` creates the corresponding bitmap and stores its drawing context at text-window system `+0x74 + 4×mode`. The payload buffer is released once the cached mode is built.
4. The terminal byte path at `0x4d2d0` applies the configured translation and ANSI processing before painting. The ordinary printable branch at `0x4db36–0x4db6f` passes the surviving byte and cell position to `0xde30`. The painter reads the current TW's mode bits and corresponding cell dimensions at `0xde52–0xde75`. It copies one cell from that mode's existing bitmap at `0xdf24–0xdf69`. The source pixel coordinates are `(c & 15) × cellWidth` and `(c >> 4) × cellHeight`. The draw uses the cached glyph and dimensions whenever this cache path is active. The MAF lookup happens during font initialization.

## Worked traces

Take a connection on which the client starts with the saved default font index 0. Font 0 has a nominal grid of 80×43. The original startup TW defines 80×42 cells using that font. The [default-window derivation](issue-353-default-text-window-rows.md) traces the separate row table and scroll margin. A printable ASCII `A` in normal character-set state arrives at the painter as code `0x41`, so its source cell is column 1, row 4 of the selected mode's bitmap. The original file offset for its raster is `fontOffset + 50 + 64 × bytesPerGlyph`.

| Startup client area | Record and key | Mode | Cell | Font offset | Bytes per glyph | `A` offset | Bitmap source pixel |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 640×480 | 0, 640×480 | 0 | 8×11 | `0x1A4` | 11 | `0x496` | (8, 44) |
| 620×460 | 3, 639×479 | 0 | 7×10 | `0x21F75` | 10 | `0x22227` | (7, 40) |
| 800×600 | 1, 800×600 | 0 | 10×13 | `0x643C` | 26 | `0x6AEE` | (10, 52) |
| 780×560 | 4, 799×599 | 0 | 9×12 | `0x27C13` | 24 | `0x28245` | (9, 48) |
| 1024×768 | 2, 1024×768 | 0 | 12×17 | `0x11B7F` | 34 | `0x12431` | (12, 68) |
| 1000×720 | 5, 1023×767 | 0 | 12×16 | `0x31E6B` | 32 | `0x3269D` | (12, 64) |

At 620×460 send `!|w0000030313` followed by CR. The size field selects mode 3 (91×25) in record 3. The `A` raster starts at `0x24A97` in its 6×18 subrecord, and the bitmap source pixel is (6, 72). A SOFT reset from the unprotected example window redefines TW 0 with the saved default 0. Subsequent `A` uses the original record 3, mode 0 raster at `0x22227` and bitmap source pixel (7, 40). The same client area supplies the bank throughout.

Print the original file values with Python 3.12. Set the filename to the extracted original MAF path.

```python
from pathlib import Path
from struct import unpack_from
import sys

data = Path(sys.argv[1]).read_bytes()
count, directory = unpack_from("<HI", data, 0x28)
for record_index in range(count):
    width, height, *fonts = unpack_from("<HH5I", data, directory + 60 * record_index)
    for mode in (0, 3) if record_index == 3 else (0,):
        offset = fonts[mode]
        cell_width, cell_height = unpack_from("<HH", data, offset)
        bytes_per_glyph, = unpack_from("<H", data, offset + 36)
        code = ord("A")
        glyph = offset + 50 + (code - 1) * bytes_per_glyph
        source = ((code & 15) * cell_width, (code >> 4) * cell_height)
        print(record_index, (width, height), mode, (cell_width, cell_height),
              hex(offset), bytes_per_glyph, hex(glyph), source)
```
