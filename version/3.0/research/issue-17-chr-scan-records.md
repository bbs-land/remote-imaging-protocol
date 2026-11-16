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

## Next action

Trace the original classic glyph reader/drawing loop downstream of 0x10f0 and its state +0x30/+0x34/+0x38. Build a bounded native fixture from the byte-exact BOLD file, capture its line/polygon/scan consumers and nonpainting state changes, and compare the actual six records plus controlled scan_flag/opcode perturbations. Preserve original scan decisions; never replace the decoder with a peer implementation. Once all painting/header questions are settled, correct the earliest BGI techspec with an explicit RIPtel-specific consumer note, update inherited 3.0 references, run required checks and file the complete #17 PR.

---

[Research backlog](issue-backlog.md) · [BGI format](../../1.54/techspecs/3.1-bgi-stroked-fonts.md)
