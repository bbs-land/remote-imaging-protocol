# Issue #17: CHR scan records and unused header bytes

[Research backlog](issue-backlog.md) · [BGI format](../../1.54/techspecs/3.1-bgi-stroked-fonts.md#stroke-opcodes) · [3.0 font roles](../ripscrip/6.1-content-file-roles.md#fonts-are-not-content-files)

## Scope and source

Started 2026-11-16 after [PR #45](https://github.com/bbs-land/remote-imaging-protocol/pull/45) delivered #16. The refreshed creation-order queue selects [issue #17](https://github.com/bbs-land/remote-imaging-protocol/issues/17). Branch **issue-17-chr-scan-records** starts at **b1defa8**. Establish the original rendering effect of six BOLD.CHR opcode-01 records, including scan_flag=0, provide geometric/expected-output evidence, correct the never-emitted assertion and TSCR unused-header observation. Do not adopt another implementation's IGNORE interpretation without original-consumer evidence.

Primary client artifact remains original RIPtel 3.1 / driver 3.0.7 `extracted/ripscrip.dll`, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, base 0x10000000, from the [catalogued mirror](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/`. Use native/static execution, no desktop GUI. Task-owned temporary files are scoped/removed; every new commit includes Codex co-authorship.

## Recovered bytes and census

The checkout currently holds Git LFS pointer files for the fonts, not their binary payloads. An initial direct parse of the pointer correctly failed to find a binary DOS-EOF marker; it is not a malformed CHR font. Work from the original installer-extracted lowercase `extracted/*.chr` files and compare their hashes with the checked-in LFS OIDs. Do not smudge/download the whole asset tree just to inspect these already recovered files.

**BOLD.CHR**: **14,670 bytes**, SHA-256 **ee8b45d7491e1bc61936923f8642f1766c58dee7c528fa542e727092a914d07e**. Both the 1.54 and 3.0 checked-in BOLD LFS OIDs match this same original payload. **TSCR.CHR**: **17,355 bytes**, SHA-256 **305e2a9657902fef2a52b25e398a5e2fca4cd0ace0fc0208704b2cb5c360b187**, likewise matching both versions' OIDs. TSCR body +11..+14 / file **0x8b..0x8e** is **02 03 00 00**, not all zero. This corrects the observed-value claim without assigning meaning to the unused bytes.

A complete glyph-offset walk of the ten installer CHR files reproduces exactly the issue's six opcode-01 records, all in BOLD. All ten have scan_flag=0. Read prefix header_size after the first 0x1a; read glyph count at header+1, first code at +4, cdefs at +5, offsets at +16. Glyph file start is `header_size+cdefs+offset[code-first]`. Decode two-byte records through the first END (both high bits zero); opcode index is `((byte1>>7)<<1)|(byte2>>7)`, distinguishing END=0, SCAN=1, MOVE=2, DRAW=3. This follows valid glyph tables rather than searching for 00 80 anywhere in the file.

| Code | Full file offset | Within glyph | SCAN and following records' bytes |
| ---- | ---------------- | ------------ | --------------------------------- |
| 0x22 | 0x035a           | 10           | `00 80 85 26 86 9c`               |
| 0x25 | 0x043c           | 32           | `00 80 9e 04 9e 85`               |
| 0x25 | 0x045e           | 66           | `00 80 9d 2a a1 aa`               |
| 0x3a | 0x0902           | 10           | `00 80 80 10 80 99`               |
| 0x3b | 0x091e           | 12           | `00 80 86 10 86 99`               |
| 0x3d | 0x0946           | 10           | `00 80 80 16 80 9b`               |

Selected geometry before painting interpretation: quote first contour `(0,38)->(1,28)->(2,38)->(1,40)->(0,38)`, SCAN, then MOVE `(5,38)` begins the second contour. Colon first rectangle `(0,0)->(0,8)->(6,8)->(6,0)->(0,0)`, SCAN, then MOVE `(0,16)` starts a rectangle up to y=25. Equals first bar `(0,13)->(0,18)->(22,18)->(22,13)->(0,13)`, SCAN, then MOVE `(0,22)` begins the second bar up to y=27. These are closed contours, but that alone does not prove scan/fill or no-op semantics. Do not publish a painting rule from geometry alone.

## Original classic-font consumer lead

Classic font loading is separate from the previously studied extended/RFF renderer. Original **0x10f0** is `bgiSetFontStyle` (matching its own error strings). It obtains the classic font state from context **+0x52**, resolves font IDs through pointer table **0x76048**, builds `%s\FONTS\%s`, opens via **0x6b560**, obtains a low-word file length through **0x75790**, reads the original font bytes via **0x6b1c0** into state **+0x2c**, and closes through **0x6b490**. GUI MessageBoxA is used on invalid direction/missing/read-error branches; native fixtures must capture that import rather than letting any UI run.

At **0x1243–0x1271**, it stores header pointer `buffer+0x80` at state **+0x34**, offsets `buffer+0x90` at **+0x38**, widths after 2*nchars at **+0x3c**, and stroke data after the width table at **+0x30**. Size/scaling numerators and denominators are populated from original tables **0x76078 / 0x76088 / 0x76098 / 0x760a8** in the following branch; this is a direct route to native classic glyph consumption without Wine.

Relevant original strings: BOLD.CHR at RVA **0x79030**, TRIP.CHR **0x7909c**; file offset to RVA for these .data strings is +0x1a00. The loader's filename-table pointer is a .rdata address and must use its own section mapping. Do not conflate file offsets with RVAs.

## Original glyph path and SCAN decision

The normal classic branches of TEXT/TEXT_XY reach **0x21a0** (TEXT_XY call **0x20e85**, after font selection at **0x20e24**); it calls the string consumer **0x1860** at **0x24bd**. That routine calls the single-glyph consumer **0x18c0** at **0x18a2** for nonzero classic font IDs. This is the active classic path, separate from RFF/extended-font rendering.

At **0x1a9c–0x1ad2**, the horizontal loop reads a record, advances its pointer by two, constructs the high-bit opcode and branches only for **0 END**, **2 MOVE**, **3 DRAW**. Opcode **1** falls through the default jump to the loop tail **0x1da1**, which continues reading because the END flag remains unset. The vertical loop **0x1dec–0x1e22** does exactly the same, defaulting to **0x210f**. It never interprets SCAN's low coordinate bits or changes pen state for that record. No contour close, flush, scan conversion or fill operation occurs. It is a consumed no-op, not END and not a replacement MOVE.

MOVE calls MoveToEx; DRAW calls LineTo and SetPixel for its endpoint. All geometry still comes from these explicit records. The routine uses the header's count/first-character fields and ordinary metrics/scales; neither it nor the original loader uses stroke-header **+7 scan_flag** to select a scan behavior. There is no dormant SCAN implementation enabled by changing that byte.

## Native probe and geometric oracle

New **run/riptel-chr-probe DLL FONT_DIRECTORY [SVG_OUTPUT]** verifies the DLL and all ten original CHR SHA-256 hashes before compiling a scoped i386 routine probe. **run/riptel-inspect DLL --chr-fonts FONT_DIRECTORY** provides a reproducible hash/header/valid-glyph scan census. The new supporting module is `run/lib/riptel-chr-fonts.ts`; expected font hashes match the 3.0 LFS OIDs. Source font payloads are read from the original artifact mirror, never rewritten.

On FreeBSD 15.1 amd64 / clang 19.1.7, `-m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`, the final probe passes **13,518,407 checks** when writing the SVG. It executes original loader **0x10f0**, glyph consumer **0x18c0**, string wrapper **0x1860**, width helper **0x1040**, bounds accumulator **0x10c0** and world conversion **0x310b8**. Every glyph-table entry in all ten files is invoked directly through the glyph helper at unit size in both orientations, both drawing passes/modes and converted/unconverted coordinates; removing SCAN records preserves every captured drawing/invalidation event and final state byte. This direct table-entry coverage does not claim to test the string wrapper's high-byte character mapping; affected ASCII glyphs are additionally verified through the original string wrapper.

For the five affected codes, the full **0..255 scan_flag byte domain** is tested **before original loading**, in both orientations, both modes, converted/unconverted coordinates and sizes 1/4/10. All resulting drawing/state matches the scan_flag=0 baseline. A controlled glyph puts SCAN **between two DRAW records without an intervening MOVE**, so a pen reset cannot hide behind the shipped fonts' following MOVE. All **16,384 possible low-seven-bit x/y payload pairs** preserve drawing/state in both orientations and modes. The test mutates only owned in-memory font fixtures; no original artifact changes.

The quote/colon/equals unit-scale original traces have **three MOVE calls, eight LineTo calls and eight endpoint SetPixel calls** each. They outline two four-edge closed contours and finally move to the advance position. A golden geometry check asserts the explicit source-derived points independently of the remove-SCAN comparison. **chr-scan-outline.svg** is generated directly from those original MoveToEx/LineTo arguments, using no fill. It was inspected with the headless rsvg-convert renderer; its temporary PNG/directory were removed immediately. This is a geometric oracle, not an exact Windows pixel image.

## Controlled-service and verification boundary

File services **0x6b560 / 0x75790 / 0x6b1c0 / 0x6b490** serve the original bytes from bounded memory and assert filename, flags, full read length and close behavior. Formatting **0x68440** uses host vsprintf. Color lookup **0x3dce0** returns controlled color IDs, and invalidation **0x13c69** records the original bounds. GDI imports record CreatePen **0x963d4**, SelectObject **0x963c0**, DeleteObject **0x963d0**, MoveToEx **0x963dc**, LineTo **0x963cc**, SetPixel **0x963c8**. Selection tracks its previous object. The original solid one-pixel pen, restoration, deletion and one final invalidation are asserted. Polygon **0x9641c**, FillRgn **0x96410** and PolyPolygon **0x96470** are failure traps; none is invoked. MessageBoxA **0x966f4** is also a failure trap, preventing any desktop dialog even on an unexpected original error path.

Native capture does not rasterize Windows pixels or establish an earlier client's runtime painting. The same BOLD/TSCR bytes are pinned in 1.54, so the data-observation corrections apply there; the **painting contract is explicitly RIPtel 3.1 / driver 3.0.7 evidence**. Original text/assets stay byte-exact. Build directories have EXIT cleanup and core dumps are disabled. Tool type checks/lint and documentation checks are recorded with delivery.

## Next action

The exact answer is recovered: opcode 01 occurs in six records, is consumed without painting/state change by RIPtel, and scan_flag does not enable it; TSCR's unused-name observation is 02 03 00 00. Correct the earliest BGI techspec with this explicitly scoped original-consumer evidence, link the geometry/reproduction notes from the 3.0 inherited reference, run required checks and publish the complete #17 PR.

---

[Research backlog](issue-backlog.md) · [BGI format](../../1.54/techspecs/3.1-bgi-stroked-fonts.md)
