# Issue #12: Preset fill-mask bytes, radix examples and tiling phase

[Research backlog](issue-backlog.md) · [Shapes and fills](../ripscrip/2.3-shapes-and-fills.md#fill-pattern-bitmaps)

## Scope and source

Started 2026-11-16 for [issue #12](https://github.com/bbs-land/remote-imaging-protocol/issues/12), oldest RealDeuce issue without a complete resolution PR after #39 publication. Branch `issue-12-fill-masks` starts at `e0bc91f` (delivered #11). Required answers: canonical twelve eight-byte masks, the 04/row-five ambiguity and all listed radix errors, inherited Markdown correction, and exact 8×8 tiling origin/coordinate units with nonzero port/viewport and adjacent-object examples. Stay on #12 until phase is fully established; no resolution PR yet.

Primary binary is unchanged RIPtel 3.1 / driver 3.0.7 DLL, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred base 0x10000000. All addresses are RVAs unless stated otherwise. Use native/static probes only; no GUI or desktop Wine. Original text/assets remain untouched.

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

The real table confirms **04 row five = 0x0e / decimal 14**, agreeing with the picture/hex and contradicting decimal 15 / radix 0F. Direct arithmetic confirms preset 05 row one F0=240=base36 **6O**; preset 0B row three/seven 22=34=base64 **0Y**. Byte FF=255 encodes base64 **3&**, not 3@ or 32. The original base64 lookup at **0x7dd30** maps `m`→48, `&`→63, `@`→0; invalid-character acceptance in the complete byte parser must not be inferred from this raw numeric lookup. These are canonical encoding corrections, not a request to treat @ as a valid digit. Both 2.0/3.0 Markdown tables contain all listed errors; the 1.54 table contains the 04 decimal/Mega and 05 Mega errors but has no Ultra column.

## Brush construction and phase leads

Both handlers call original brush rebuild **0x11210**. Preset 1 creates a solid brush. Every other preset/custom mask creates an **8×8, one-plane, one-bit bitmap** (CreateBitmap at 0x1132f), fills it white, then iterates row words at style+0x0a: low bit 0 goes to pixel X=7, low bit 7 goes to X=0, row number increments Y from 0 to 7 (**0x113af–0x11439**). SetPixel paints those selected pixels black. Original **CreatePatternBrush** at **0x11473** receives that bitmap. This inversion implements pattern-one as foreground: a monochrome bitmap zero uses DC text color, bitmap one uses background color, per Microsoft's primary [CreatePatternBrush contract](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createpatternbrush). It is not a reason to invert the published row bytes.

The DLL's 209 imports contain CreatePatternBrush and CreateSolidBrush but **no brush-origin setter/getter** or coordinate-origin setter; the only map-mode import is GetMapMode. The original DC/clip paths already traced in [issue #11](issue-11-pixel-pen-units.md) select a master screen HDC for onscreen ports, separate bitmap HDC for offscreen ports, and install clip regions without coordinate-origin changes. This suggests default device-context origin (0,0), not per-object or viewport rephasing. It remains necessary to execute brush creation and representative filled primitives with captured services and inspect any intermediate-surface composition that could change phase.

Microsoft's [Brush Origin documentation](https://learn.microsoft.com/en-us/windows/win32/gdi/brush-origin) explains default phase (0,0) at the window/client drawing context and continuity across objects unless the origin is reset. This explains a controlled GDI boundary; it does not alone prove every original RIPtel primitive follows it. The known legacy RIP_FILL patterned-brush defect is a separate exception (solid flood despite pattern) already documented in the fill-defects pages.

## Next action

Build a native fill-style/custom-brush probe that captures the original generated 8×8 bitmap, foreground/background assignment and final draw rectangles/DC/brush. Preserve original row/preset selection and bit loops; verify every preset and single-bit custom row, then establish device-context phase through actual consumer paths and adjacent filled objects under world scaling/nonzero stored viewport origins. Audit any intermediate surfaces before asserting a universal rule. Correct all inherited Markdown radix rows and publish an unambiguous eight-byte table, with original-text evidence/errata preserved. File a complete PR dependent on #39 only after every issue question is derived; issue-only comparison starts at e0bc91f.

---

[Research backlog](issue-backlog.md) · [Shapes and fills](../ripscrip/2.3-shapes-and-fills.md#fill-pattern-bitmaps)
