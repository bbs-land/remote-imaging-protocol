# Issue #323 — Classic stroke-font metrics on button labels

[Issue #323](https://github.com/bbs-land/remote-imaging-protocol/issues/323) · [Stroke-font label specification](../ripscrip/4.3-buttons.md#classic-stroke-font-labels) · [Research backlog](issue-backlog.md)

RealDeuce filed #323 on 2026-11-26 at 04:27:14 UTC. The 04:36 UTC check found it with no comments. Branch `issue-323-classic-button-stroke-metrics` starts at fork stable `43c3d91162b0573f06e1e17e3db861f535455fe6`.

The ALPHA 4 Triplex size 1 table gives Top 6, Base 18 and Drop 22 at 640×350. The draft's size ratio is 3/5 and its flag 8192 prose excludes side labels. The 3.0 specification uses metrics from the selected face for strokes and preserves the draft's side placement exclusion. [ALPHA 4 record](../../2.0/text/RIPScrip-2.0-alpha-4.txt) · [Historical stroke tables](../ripscrip/4.3-buttons.md#inherited-descender-and-stroke-tables) · [Font-zero derivation](issue-190-button-font-zero.md).

## Stroke measurement and positioning

Static inspection used PC Micro RIPtel 3.1's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The corresponding recovered `trip.chr` has SHA-256 `831963618a5dbf3562e12c3b4b8bab3ed3879143de3c7cd9293be171a44282db`. Its stroke body begins at file offset `0x80`. Bytes `0x88–0x8a` are `18 00 f9`, or signed 24, 0 and −7. The width table begins after the 223 little-endian glyph offsets at `0x90`. The entries for U and Q are 22 and 19. DLL instruction addresses here are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md) · [CHR structure and stroke origin](../../1.54/techspecs/3.1-bgi-stroked-fonts.md#file-structure).

- The descriptor's classic bit and nonzero font nibble select `0x2b0bd`. This branch calls alignment setter `0x13d0` with selectors 0 and 2, left and top, and selects the captured font and size at `0x2b0e3` with direction 0. The vertical and horizontal scale-table pairs at `0x76078/0x76088` and `0x76098/0x760a8` contain the same nonzero-size ratios. Size 1 reads numerator 3 and denominator 5. The packed orientation in the common placement branch denotes the button label's position around the face.
- The load path `0x12ef–0x1306` stores `B = S(cap − dec)` at font state +0x22, where `S` is signed truncation after multiplying by the selected ratio. `0x2540` returns `B − S(cap)`. `0x2500` returns `B`. `0x25b0` returns `B − S(dec)`. The button calls all three with conversion disabled. It measures label height at `0x2b13c` with `0x1640`, whose stroke path returns `B + 1`. Subtracting the `0x2540` result gives `S(cap) + 1`. The width routine `0x1410` sums the width entries before scaling the sum at `0x15e8`.
- At `0x2b14b` the stroke branch jumps over the outline metrics' DEVICE-to-WORLD conversions. The common placement code converts the four inner face bounds and the two face spans at `0x2b280–0x2b2df`. It uses the resulting WORLD face geometry for the label. When the descender flag and byte predicate both hold, `0x2b3c9` or `0x2b40f` subtracts `B` from `B − S(dec)`, leaving the additional height `−S(dec)`. Above subtracts the effective height and 5. Center uses half the remaining face height. Below starts at the bottom edge plus 5. The original shares an adjusted center branch with the side orientations. The portable rule follows the draft's explicit side exclusion. The ASCII entries of the existing descender predicate `0x65a39` match declared `low_char`, including Q.
- The classic paint call at `0x2b621` uses paired WORLD coordinates and the conversion flag. At `0x23a9` vertical selector 2 jumps to `0x23e6`, which retains the provided Y. The upright stroke walker `0x1a8b–0x1a98` adds the saved `B` to that Y, `0x1b0d–0x1b2f` subtracts the scaled local stroke Y, and `0x1b4e` and `0x1c35` project complete points. The projection at `0x310df–0x310ec` multiplies a WORLD ordinate by the DEVICE height and divides by WORLD height. The button's vertical style and effects use its own selectors. No separate device height scale appears between these operations.

## Underline coordinates and examples

The classic decoration branch at `0x2b661–0x2b6a4` tests the underline flag, mouse eligibility and hotkey byte, then takes the first match in the uppercase comparison string. `0x2b6d4` measures the prefix and adds it to the WORLD X. `0x2b6e7–0x2b704` picks `B` for an ordinary matched glyph and `B − S(dec)` for a marked glyph, then adds 3 and the label's anchor Y. This path reads the matched glyph for its descender test, independently of the centering flag and the other label bytes. `0x2b712–0x2b732` measures the glyph width and decrements it once. `0x2b743` projects the start coordinates and `0x2b753` projects that length separately. `0x2b862–0x2b87d` draws the horizontal line to its endpoint. The portable half-open interval expresses the swept pixels. A nonpositive portable length produces no stroke.

For the report's 80×40 centered face, `B = trunc(31 × 3/5) = 18`, the capital-to-baseline height is 14, the descent is 4, and the placement height is 15 or 19. For `UQ`, the scaled width is 24 and the Q prefix width is 13. Thus label X is 128, Q starts at X 141, and the underline spans `[141,151)` with a 640-wide canvas. At 1:1 the anchor is Y 112 with flag 8192 clear and Y 110 when set. The baselines are Y 130 and 128. With Q underlined, the marks are Y 137 and 135. With creation WORLD 640×350 and a 640×700 canvas, project the same WORLD points to baselines Y 260 and 256 and marks Y 274 and 270. With a 640×700 creation WORLD on that taller canvas, an 80×40 face at the same numeric coordinates again has the original 1:1 results.

Verification read the recovered font bytes, decoded the original scale tables with their PE mapping, traced the static paint and metric instructions, and evaluated the fixture arithmetic. The original renderer was not executed.

```sh
sha256sum "$dll" "$trip"
od -An -tx1 -j 128 -N 16 "$trip"
objdump -d --start-address=0x10002500 --stop-address=0x10002613 "$dll"
objdump -d --start-address=0x1002b0a9 --stop-address=0x1002b150 "$dll"
objdump -d --start-address=0x1002b661 --stop-address=0x1002b8aa "$dll"
```
