# Issue #9: RGB example and palette-index arithmetic

[Research backlog](issue-backlog.md) · [Color reference](../ripscrip/2.0-color-modes-and-palettes.md) · [Inherited color reference](../../2.0/ripscrip/2.0-color-modes-and-palettes.md)

## Scope and source

Started 2026-11-16 for [issue #9](https://github.com/bbs-land/remote-imaging-protocol/issues/9), the oldest RealDeuce issue without a complete resolution PR after #35 publication. Branch `issue-9-rgb-examples`, base `cfa54b5` (delivered #8). This is a bounded editorial/arithmetic correction: the issue explicitly requires no disassembly, and no client behavior or palette entry needs changing.

Primary source: [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. Lines 3539–3550 contain contradictory packed hex/table-blue values versus binary/decimal; lines 3640–3644 contain the correct arithmetic formula and an incorrectly parenthesized C equivalent. Preserve this original text record byte-for-byte. Repository search for `04140`, `00001100`, `INDEX = 32` and `BLUE.*GREEN.*RED` locates inherited copies in the 2.0 and 3.0 Markdown color chapters; the separate canvas/palette techspec uses the already-correct multiplication formula.

## Derivation

The original concatenated binary `000001000001010000001100` splits into red `00000100` = 0x04 = 4, green `00010100` = 0x14 = 20, blue `00001100` = 0x0c = 12. Consequently choose packed hex **04140C** and table blue **0Ch**, preserving the already-consistent binary and decimal rows. Choosing 04140D/13 would be another possible editorial example, but would require changing every blue bit-string/decimal value; it is not needed to fix a protocol ambiguity.

The intended palette formula is `32 + BLUE + GREEN*4 + RED*32`. In C the shifts must be grouped as `32 + BLUE + (GREEN << 2) + (RED << 5)`. Addition binds tighter than shifting, and chained shifts group left to right: the old text means `((32 + BLUE + GREEN) << (2 + RED)) << 5`. For BLUE=1, GREEN=2, RED=3 the intended index is **137**, whereas that old grouping gives **35840** on an integer type wide enough to represent it. The corrected expression's input ranges (BLUE 0..3, GREEN 0..7, RED 0..6) yield palette indices 32..255. The C grammar and shift semantics are specified in the primary [WG14 N1570 committee draft §6.5.7](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf), printed pp. 94–95. That source explains the expression, not RIPtel runtime behavior.

A direct Python arithmetic calculation reproduced hex 04140C / components 4,20,12 and index values 137 / 137 / 35840 for multiplication, corrected shifts and original C grouping respectively. No runtime client probe or new regression test is warranted for these reversible prose/example changes.

## Next action

Correct both Markdown color chapters, use a proper C code fence/semicolon and include the 137 example. In 2.0 put the original-draft discrepancy in errata; in 3.0 add an inline reconstruction evidence pointer. Keep original text, palette table and unrelated implementation claims untouched. Format and run the required repository checks, then file one PR dependent on #35 with an issue-only comparison from cfa54b5, and record delivery before selecting #10.

---

[Research backlog](issue-backlog.md) · [Color reference](../ripscrip/2.0-color-modes-and-palettes.md) · [Inherited color reference](../../2.0/ripscrip/2.0-color-modes-and-palettes.md)
