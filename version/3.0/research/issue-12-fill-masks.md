# Issue #12: Preset fill-mask bytes, radix examples and tiling phase

[Research backlog](issue-backlog.md) · [Shapes and fills](../ripscrip/2.3-shapes-and-fills.md#fill-pattern-bitmaps)

## Scope and source

Started 2026-11-16 for [issue #12](https://github.com/bbs-land/remote-imaging-protocol/issues/12), oldest RealDeuce issue without a complete resolution PR after #39 publication. Branch `issue-12-fill-masks` starts at `e0bc91f` (delivered #11). Required answers: canonical twelve eight-byte masks, the 04/row-five ambiguity and all listed radix errors, inherited Markdown correction, and exact 8×8 tiling origin/coordinate units with nonzero port/viewport and adjacent-object examples. Stay on #12 until phase is fully established; no resolution PR yet.

Primary installer is [RIPtel 3.1 riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`, extracted without installation into `<artifact-root>/artifacts/riptel-3.1-pcmicro/`. Primary binary is its unchanged `extracted/ripscrip.dll`, RIPtel 3.1 / driver 3.0.7, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred base 0x10000000. All addresses are RVAs unless stated otherwise. Use native/static probes only; no GUI or desktop Wine. Original text/assets remain untouched.

## Preset-table recovery

Original physical dispatch slot **58** (`S`) uses handler **0x1c679**, arguments `[2,CM]`; slot **59** (`s`, custom fill) uses **0x1c55f**, arguments `[2,2,2,2,2,2,2,2,CM]`. Preset handler copies eight **16-bit words** from table **RVA 0x7afd8** (original file offset **0x795d8**, data-section RVA minus 0x1a00) into graphics-style offsets +0x0a through +0x18 at **0x1c72d–0x1c75c**. Its index is preset*8+row. Upper bytes in the shipped table are all zero; the canonical eight row bytes are:

```text
00: 00 00 00 00 00 00 00 00
01: FF FF FF FF FF FF FF FF
02: FF FF 00 00 FF FF 00 00
03: 01 02 04 08 10 20 40 80
04: E0 C1 83 07 0E 1C 38 70
05: F0 78 3C 1E 0F 87 C3 E1
06: A5 D2 69 B4 5A 2D 96 4B
07: FF 88 88 88 FF 88 88 88
08: 81 42 24 18 18 24 42 81
09: CC 33 CC 33 CC 33 CC 33
0A: 80 00 08 00 80 00 08 00
0B: 88 00 22 00 88 00 22 00
```

The real table confirms **04 row five = 0x0e / decimal 14**, agreeing with the picture/hex and contradicting decimal 15 / radix 0F. Direct arithmetic confirms preset 05 row one F0=240=base 36 **6O**; preset 0B row three/seven 22=34=base 64 **0Y**. Byte FF=255 encodes base 64 **3&**, not 3@ or 32. The original base 64 lookup at **0x7dd30** maps `m`→48, `&`→63, `@`→0; invalid-character acceptance in the complete byte parser must not be inferred from this raw numeric lookup. These are canonical encoding corrections, not a request to treat @ as a valid digit. Both 2.0/3.0 Markdown tables contain all listed errors; the 1.54 table contains the 04 decimal/Mega and 05 Mega errors but has no Ultra column.

## Brush construction and initial phase leads

Both handlers call original brush rebuild **0x11210**. Preset 1 creates a solid brush. Every other preset/custom mask creates an **8×8, one-plane, one-bit bitmap** (CreateBitmap at 0x1132f), fills it white, then iterates row words at style+0x0a: low bit 0 goes to pixel X=7, low bit 7 goes to X=0, row number increments Y from 0 to 7 (**0x113af–0x11439**). SetPixel paints those selected pixels black. Original **CreatePatternBrush** at **0x11473** receives that bitmap. This inversion implements pattern-one as foreground: a monochrome bitmap zero uses DC text color, bitmap one uses background color, per Microsoft's primary [CreatePatternBrush contract](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createpatternbrush). It is not a reason to invert the published row bytes.

The DLL's 209 imports contain CreatePatternBrush and CreateSolidBrush but **no brush-origin setter/getter** or coordinate-origin setter; the only map-mode import is GetMapMode. The original DC/clip paths already traced in [issue #11](issue-11-pixel-pen-units.md) select a master screen HDC for onscreen ports, separate bitmap HDC for offscreen ports, and install clip regions without coordinate-origin changes. This was the initial default-context-origin lead. The completed native consumer checks and static audit below establish the positive direct-brush paths and their scope.

Microsoft's [Brush Origin documentation](https://learn.microsoft.com/en-us/windows/win32/gdi/brush-origin) explains default phase (0,0) at the window/client drawing context and continuity across objects unless the origin is reset. This explains the controlled GDI boundary, combined below with positive original-consumer evidence. The known legacy RIP_FILL patterned-brush defect is a separate exception (solid flood despite pattern) already documented in the fill-defects pages.

## Native verification and fixture boundary

`run/riptel-fill-probe <artifact-root>/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` passes **2,754 checks** on FreeBSD 15.1 amd64 with clang 19.1.7 (`-m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`). The wrapper first verifies the unchanged DLL hash, maps its PE into a temporary native i386 process using the shared image loader, substitutes only controlled dependencies and removes its temporary executable afterward. It neither modifies the artifact nor starts a GUI.

Original instructions execute both fill handlers, color validation **0x44385**, palette-index translation **0x16140**, preset lookup, row-word storage, brush construction **0x11210**, all 64 bit/row positions, world conversion **0x31084**, viewport offset **0x34231**, DC selection **0x343e4**, clipping **0x342ad/0x34319**, brush/pen selection and the two direct filled-rectangle consumers. All twelve presets are checked; preset 01 takes CreateSolidBrush while the other eleven create the expected inverted monochrome bitmap. Every one of the 64 custom single-bit positions is exercised. Original numeric decoders **0x211a9** (MegaNum) and **0x211d6** (UltraNum) independently verify the canonical two-digit encoding of every byte 0–255; the actual UltraNum alphabet ends in **#&**, not !&.

The exact original-routine substitutions are no-op hooks **0x45038, 0x18cf6, 0x18bf7, 0x3988f, 0x398ee, 0x447c0, 0x44802, 0x13c69** (entry/caret/UI synchronization, palette locks and invalidation), protection **0x3935b** returning zero, RGB lookup **0x3dce0** returning the fixture's color index as an identifiable color token, and error reporter **0x38e47** capturing errors. All supplied colors pass original mode-zero validation; the fixture's direct-palette boundary is 236. Controlled IAT services are GetStockObject **0x9640c**, SelectObject **0x963c0**, DeleteObject **0x963d0**, CreateCompatibleDC **0x964a0**, SelectPalette **0x963f4**, CreateBitmap **0x964a4**, SetPixel **0x963c8**, FillRect **0x9667c**, CreatePatternBrush **0x964a8**, CreateSolidBrush **0x96418**, DeleteDC **0x964ac**, SetTextColor **0x963f8**, SetBkColor **0x96400**, SetROP2 **0x96408**, SetRect **0x96618**, OffsetRect **0x96610**, CreateRectRgn **0x963e8**, SelectClipRgn **0x963d8**, Rectangle **0x96468** and SetBkMode **0x963f0**. Their actual stand-ins are preserved in the committed C source. Handles, allocation, palette/RGB realization and rasterization are controlled; no Windows pixel drawing or historical display-driver behavior is executed.

Initial fixture failures were **fixture defects**: SelectObject originally used one selection slot for an entire DC, letting selection of NULL_PEN overwrite the selected brush; GDI keeps these object classes separate. The final fixture tracks brush, pen and bitmap independently and returns their previous objects. A later all-byte decoder check also rejected a mistakenly supplied ! at digit 62; the original table and existing numeric chapter correctly require #. Neither failure indicates a driver defect.

## Consumers and exact phase derivation

Original RIP_BAR **0x19d9e** converts coordinates, offsets the rectangle by the **stored viewport** at 0x34231 and selects the current pattern brush at **0x19e8c**. It sets the current fill/background colors and calls GDI Rectangle at **0x19f13**. The original RIP_FILLED_RECTANGLE **0x1bee5** similarly selects the current brush at **0x1bff4** and calls Rectangle at **0x1c075** on its border-disabled path; its GDI colors are supplied as controlled initial DC state in this phase probe. Neither path creates an intermediate shape bitmap, scales the pattern, nor changes its origin.

For each consumer, the native probe draws adjacent decoded rectangles `(0,0,8*s,8*s)` and `(8*s,0,16*s,8*s)` with `s=1` and `s=2`, world sizes 640×480 / 1280×960 and device size 640×480. It supplies a nonzero screen-port origin `(2,1)` and stored viewport `[5,3,29,19)`, then repeats on an offscreen port with its own DC. All four combinations retain **one unchanged 8×8 brush**; the only compatible DC allocation is the brush-construction DC. BAR reaches Rectangle with `[5,3,15,13)` / `[13,3,23,13)`; K reaches it with `[5,3,14,12)` / `[13,3,22,12)`. These captures preserve the commands' different endpoint adjustments; they are not a claim of exact raster coverage. Original clip setup/restoration also runs. The port origin is not added a second time: the viewport producer's direct-context-coordinate behavior was established in [issue #11](issue-11-pixel-pen-units.md).

Static consumer audit supplements this direct execution. The shared **0x13d27** only selects objects+0x0c into the current HDC. Filled circle/oval paths select that brush at **0x1b42b/0x1b791** and call Ellipse at **0x1b4b9/0x1b812**; filled rounded rectangle selects it at **0x1c21c** and calls RoundRect at **0x1c2bd**; polygon paths select it at **0x1bdd4/0x1c442/0x1e710** and call Polygon at **0x1be54/0x1c4ce/0x1e779**; poly-polygon selects it at **0x1e996** and calls PolyPolygon at **0x1ea72**. These ordinary fill paths use the current DC, rather than resetting phase in an object-sized intermediate. The DLL's imports and string table contain no SetBrushOrg/SetBrushOrgEx, SetWindowOrgEx, SetViewportOrgEx or SetMapMode; the statically unpacked RIPtel client also imports no brush/coordinate-origin setter. This is supporting negative evidence alongside the positive consumer traces, not an assumption that missing help/corpus names prove behavior.

Combining the actual fixed-size brush and direct-context GDI calls with Microsoft's documented default brush-origin contract gives the exact reconstruction: tile origin **drawing-device-context (0,0)**, row 0 at Y mod 8=0, MSB at X mod 8=0, and an **8-device-pixel period** under world scaling. Onscreen ports share the master screen HDC; offscreen bitmaps each have their own HDC and bitmap-local (0,0). Viewports and clipping change covered pixels, not tile phase. Each new primitive samples the same tile at its final context coordinates. This is a derivation from original calls and the primary API contract; the native probe does not pretend to execute Windows' brush-phase implementation. An arbitrary external host that alters a supplied HDC's brush origin is outside the recovered RIPtel default.

For a custom mask `80 00 00 00 00 00 00 00`, a nonzero port `(2,1)` and stored viewport origin `(5,3)`, an interior point `(8,8)` in the first shape is foreground, and `(16,8)` in the adjacent shape is also foreground; `(9,8)` and `(10,9)` are background. Restarting at the viewport would incorrectly make `(5,3)` foreground; restarting at the port would incorrectly make `(10,9)` foreground. A world twice the device size scales coordinates 2:1 but does not enlarge the tile: its period is still eight device pixels (sixteen world units). Offscreen copies preserve already rendered pixels; copying a bitmap to a different destination does not re-tile its contents in the destination phase.

The legacy RIP_FILL border-color branch's solid-fill defect remains an explicit exception. This issue establishes ordinary pattern-brush phase independently of filled-shape boundary rasterization, pie/chord leakage, palette realization or historical 1.x/2.x phase behavior.

## Reference correction and validation

The 3.0 reference now publishes all twelve eight-byte masks, the foreground/background formula at final context coordinates, a nonzero port/viewport example, adjacent-object continuity, two world scales and the independent offscreen-origin/copy distinction. The 1.54 and 2.0 Markdown diagrams receive only arithmetic/caption corrections, with an inline editor's note and 2.0 errata respectively; their historical runtime phase is not inferred from the later DLL. A bounded extraction check of all **96 pictured rows in each of the three editions** verifies picture bits, hexadecimal, decimal, MegaNum and (where present) UltraNum against the original DLL's 96 table words. A separate extraction verifies the new canonical table against those same original words.

The preserved text SHA-256 values before/after correction are identical: `version/1.54/text/RIPScrip-1.54.txt` = `1ff57d53fb81c06604bdb6d9eb35150c90cbe7d5acd7b5ead6a0758f3d957497`; `version/2.0/text/RIPScrip-2.0-alpha-4.txt` = `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. No assets change.

`run/format` applied to all edited Markdown, `run/lint` passed, `run/check-links` passed **4,789 links across 195 files**, and `git diff --check` passed. The new C probe is formatted with clang-format 19 and builds with warnings as errors. Native verification passes 2,754 checks. All issue subquestions now have an exact answer within the stated primary-source scope.

## Delivery and continuation

Delivered in [PR #40](https://github.com/bbs-land/remote-imaging-protocol/pull/40), branch `issue-12-fill-masks`, dependent on #39. Specification commit **ab03c0d**; native-evidence checkpoint **011f1b9**. The [issue-only comparison](https://github.com/RealDeuce/remote-imaging-protocol/compare/e0bc91f...issue-12-fill-masks) starts at delivered #11's final commit. Review/merge is pending; no issue was manually closed and no PR was merged. Every new commit has Codex co-authorship.

Post-publication GitHub refresh confirms #13 (polygon wire counts/widths) is the oldest RealDeuce issue without a complete resolution PR. The queue remains #13 through #28, then #33 and #38. Start #13 on a branch from this delivery checkpoint and preserve the same primary-source/committed-note workflow.

---

[Research backlog](issue-backlog.md) · [Shapes and fills](../ripscrip/2.3-shapes-and-fills.md#fill-pattern-bitmaps)
