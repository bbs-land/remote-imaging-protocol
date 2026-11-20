# Issue #104: Terminal character sets

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md) · [ANSI baseline](../../baseline/techspecs/ansi-vt-support.md)

## Scope and checkpoint

Started 2026-11-20 on `issue-104-terminal-character-sets` from fork stable ff2043e. [Issue #104](https://github.com/bbs-land/remote-imaging-protocol/issues/104) requests ANSI versus VT-102 designation/invocation, byte-to-glyph mapping and reset/save/definition state. #105 outer reset and #106 selection follow separately. Separate FastFont research branch `research-fastfont-fields` is preserved and excluded from this issue branch. Preserve RIPscrip 1.54 and prior #100/#101 answers unless exact new evidence requires a documented correction.

## Primary source and boundaries

Original RIPtel 3.1 / RIPscrip driver 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base `0x10000000`. All addresses below are RVAs. LLVM objdump 19.1.7 and Python read original bytes only. Section-aware file mapping: `.text` minus `0xc00`, `.rdata` minus `0x1000`, `.data` minus `0x1a00`. Original source assets and immutable protocol text stay unchanged.

The [ANSI baseline](../../baseline/techspecs/ansi-vt-support.md) cites RIPterm 1.54 Appendix B and changelog for US/UK/special designation and manual-clear history, but contains no complete RIPtel 3.1 per-byte mapping, mode gating, invocation or reset rules. Do not infer those from generic DEC expectations or transfer a 1.54 UI observation to a 3.1 host command. No peer implementation or new UI/native probe supplies this answer.

## Designation, mapping and invocation

TW system is context `+0x12`, current index `+0x0a`, record stride `0xf1`. The packed record dword at `+0x2e` holds font/color and charset fields. Original ANSI dispatch directory `.rdata` `0x762c0` has six-byte records `{u16 count, u32 table}`; individual entries are five-byte `{u8 final, u32 callback}`. Groups 3 and 4 respectively map finals `0/A/B` to G0 callbacks `0x379ff/0x37a21/0x37a43` and G1 `0x37a5c/0x37a7e/0x37aa0`. G0 designation bits are `0x3000` (US=0, UK=`0x1000`, special=`0x2000`); G1 bits `0xc000` (US=0, UK=`0x4000`, special=`0x8000`). Dispatcher `0x37ab9` and callbacks do not test the VT-102 choice, so designation recording is shared by ANSI/VT-102 parsing. Unmatched finals have no callback or designation effect.

Translator `0x2c94b` returns input >= `0x80` unchanged before mode lookup; below that, helper `0x2c9c7` calls client emulation getter `0x317f3(-1)`. ANSI mode returns the byte unchanged; VT-102 reads the current TW packed state. Bit `0x0800` chooses G1 rather than G0. UK translates only `#` to bank `0x9c`; special uses `.data` byte table RVA `0x7eec8`. Its first `0x60` entries are identity; its next 32 are:

```text
04 db 1a 1e 1b 18 f8 f1 19 17 d9 bf da c0 c5 c4
c4 c4 c4 c4 c3 b4 c1 c2 b3 f3 f2 14 f7 9c fa 20
```

This is a CP437 substitution table, not a table of generic DEC Unicode identities: e.g. checkerboard uses full block, five scan-line positions share a horizontal rule, and several control-picture symbols use arrows/triangle/paragraph sign. The canonical table names each bank glyph, including visual C0-bank positions; those identities are ordinary MicroANSI glyphs even though the numeric positions are also control codes in input streams. The supplied font container stores glyph slots 1–255 as documented in the [MAF reference](../techspecs/3.3-microansi-maf-delta.md).

Raw terminal output `0x4d2d0` calls the translator and ANSI parser, then has explicit SI/SO branches at `0x4d7ab/0x4d7d5`. In VT-102 mode they consume the character after calling `0x2c9c6(0/1)`. Original byte at that helper is **c3**, a single return, with no invocation-state write. In ANSI mode they fall through to ordinary glyph output. This bounded source fact establishes ineffective invocation and ANSI bank `0x0f`/`0x0e` glyph output; there is no need to investigate or fix an obsolete client's omission. Normal initialized state has invocation G0 and no effective SI/SO transition. G1 designation remains recordable/retained but is not made visible by SO in this byte profile. Do not silently replace this with a modern terminal's shift behavior.

Client emulation state lives outside TW records: getter/setter `0x317f3`, configuration `+0x306`, changes the ANSI/VT-102 choice and refreshes UI labeling but does not rewrite designation fields. Changing modes therefore gates retained mapping rather than initializing a new charset.

## Explicit portable glyph-stage completion

The report allows a normative portable mapping policy. Use original table **glyph identities**, but parse syntax and incoming controls before translating bytes selected for display. Mapped bank `0x1b` means a left-arrow glyph, never a recursively parsed escape; designation/CSI payload likewise remains syntax. Original output calls translator before parser, so that call order is not evidence for this cleaner boundary; reproducing its translated-control edge paths would be obsolete-client bug investigation. The policy is clearly labeled in the specification, and vector 4 exercises it. Exact pixels, modern Unicode font selection, generic DEC replacement glyphs and UTF-8 are outside the contract.

Invocation is not silently repaired by this policy: the documented VT-102 SI/SO no-op and ANSI glyph behavior remain original-source-derived. A working G1-shift extension must be declared separately.

## Reset, definition and saved state

Cold/initialized TW record fields are zero, meaning US/US and G0. Changed ordinary definition `0x8034` changes font bits `0x700`, colors and flags but preserves the higher charset bits in `+0x2e`. Identical definition, deactivation/reactivation and mode changes also retain them, matching #100. Extended definition calls `0x39935` to clear bytes through `+0x42`, then rebuilds the record; it resets designations/invocation as documented in #101. These are distinct operations.

CSI save `0x37177` writes saved cursor bytes `+0/+1`; restore `0x371c1` reads only those coordinates and does not restore charsets. ESC save `0x37404` instead copies the complete `0xf1`-byte TW record into TW-system `+0x88` and stores live coordinates in that snapshot's initial bytes. ESC restore `0x37455` copies it back before restoring/clamping live cursor. Thus the two forms have different charset semantics; the larger ESC snapshot is interpreter-owned and does not include global emulation configuration. Uninitialized-snapshot fallback is not newly specified here.

ESC-c callback `0x37978` calls ANSI reset `0x35320`. That helper resets colors, selected rendition/mode bits, cursor and margins/tab map but **does not write the charset field**; its `+0x2e` masks affect the low color byte only. Do not read the historical baseline's generic “initial state” summary as a guarantee to clear every RIPtel charset. The 3.0 scoped profile explicitly retains G0/G1/invocation across ESC c and keeps the separate global emulation choice.

RIP reset SOFT `0x16220` reaches TW discard-all-nonzero `0x39ac8(-2)` at `0x164c9`, selects TW 0 through `0xe0dc(0)` at `0x164d3`, then ordinary full-screen initializer `0x8577 -> 0x8034` at `0x164e8`. The discard helper loops 1–35, skips protected TWs and clears unprotected records. TW 0 is not cleared; its ordinary redefinition retains its charset fields. Protected surviving records retain their own fields; discarded ones have no surviving TW state. Global emulation choice is unchanged. This is the charset portion of #105's pending outer-reset work, not a premature claim to have resolved its other families.

The 1.54 manual-screen-clear changelog remains scoped to that product/UI. No 3.1 host clear/reset is given additional charset effects by that historical note.

## Reproduction and checks

From the repository root, replacing `DLL` with the original mirror path:

```sh
objdump -d --start-address=0x1002c94b --stop-address=0x1002c9c7 DLL
objdump -d --start-address=0x100379ff --stop-address=0x10037ab9 DLL
objdump -d --start-address=0x1004d7ab --stop-address=0x1004d807 DLL
objdump -d --start-address=0x10035320 --stop-address=0x1003540b DLL
objdump -d --start-address=0x10037177 --stop-address=0x100372cd DLL
objdump -d --start-address=0x10037404 --stop-address=0x100374c0 DLL
objdump -d --start-address=0x100164c6 --stop-address=0x100164ed DLL
```

Independent checks pass for the 128 original mapping bytes, one-byte invocation helper, six designation callback identities and driver hash; all seven literal vectors were manually reviewed as the published profile. The syntax-before-glyph vector is policy arithmetic/data evaluation, never a claimed native observation. Markdown lint, 5,870 links across 240 files and diff checks pass. [PR #107](https://github.com/bbs-land/remote-imaging-protocol/pull/107) is open/ready and closes exactly #104; specification commit 563ee59. Actual-head stable consolidation follows. Refresh 2026-11-20T21:46:08Z confirms unchanged #104–#106 bodies and zero comments.

---

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md)
