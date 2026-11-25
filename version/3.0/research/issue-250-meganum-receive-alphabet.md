# Issue #250 — MegaNum receive alphabet and discarded separators

[Issue #250](https://github.com/bbs-land/remote-imaging-protocol/issues/250) · [Receive alphabets](../ripscrip/1.2-math-and-coordinates.md#working-with-meganums-and-ultranums-by-hand) · [Framing vectors](../ripscrip/1.1-protocol-and-command-hierarchy.md#numeric-collection-and-dispatch) · [Research backlog](issue-backlog.md)

RealDeuce filed #250 at 19:55:38 UTC on 2026-11-24. The 19:57 UTC in-thread check found it alongside #249. Branch `issue-250-meganum-receive-alphabet` starts at fork stable `f9c6d1f6c3061c453b9aad1a7cdac71a58a5367a`. The earlier corpus discussion inferred dash-to-zero decoding from a lack of visible drawing and a SyncTERM implementation detail. A discarded command also produces no drawing. The original collector distinguishes those cases before conversion.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump read the PE statically. Addresses below are RVAs at image base `0x10000000`. Subtract `0xc00` for `.text` file offsets and `0x1a00` for `.data`.

## Validation before conversion

State 3 at `0x3a01b` collects ordinary numeric fields. The leading-period branch at `0x3a043–0x3a04e` precedes digit validation. At `0x3a066–0x3a07c`, the selected validator must accept a byte before it enters the digit path at `0x3a8ae`. The MegaNum validator at `0x210b2–0x210cf` accepts only ASCII `0x30–0x39` and `0x41–0x5a`. The UltraNum validator at `0x210d0–0x2119b` tests ctype masks `0x04` and `0x103` plus literal `#` and `&`. Reading the 128 ASCII entries of the original ctype table at `0x87912` verifies precisely digits and upper- and lowercase letters for those masks. The [conditional operand trace](issue-90-conditional-operand-grammar.md#left-scanner-and-provider-boundary) records the ctype pointer and the same mask definitions.

The conversion loops at `0x211a9–0x211c8` and `0x211d6–0x211f5` multiply by 36 or shift left six bits before adding each value from the table at `0x7dd30`. The table maps the UltraNum alphabet in order to 0–63, with uppercase A at 10, lowercase a at 36, `#` at 62 and `&` at 63. Its unused dash entry is zero. The validator keeps a dash out of both conversion loops. A dash is also outside the delimiter set compared at `0x3a082–0x3a094`, so a dash offered as a digit sends an unfinished command to discard at `0x3a8a3`. A separator line such as `!|-----------` selects slot 9 with its first dash. The next dash discards it before any of its five numeric fields are stored.

The [radix selector](issue-239-rgb-numeric-width.md#width-and-radix) at `0x39d70–0x39dd9` forces UltraNum for direct RGB on a CM field. Otherwise, descriptor low bits `01` select MegaNum, `10` select UltraNum, and `00` or `11` use active Base Math. The dispatch table at `0x80820` has 40-byte records. Concatenate the level key at offsets 5–14 with the opcode at offset 15 to identify a slot. The field count is at offset 16, signed widths at offset 20 and flags at offset 38.

| Slot | Command | Numeric widths    | Flags    |
| ---- | ------- | ----------------- | -------- |
| 9    | `-`     | XY, XY, XY, XY, 2 | `0x0003` |
| 22   | `c`     | CM                | `0x0003` |
| 40   | `J`     | 2                 | `0x0001` |
| 103  | `1p`    | 4                 | `0x000f` |
| 118  | `2s`    | 1, 2              | `0x0003` |
| 122  | `3D`    | 4                 | `0x0003` |

## Staged captures

The [report captures](https://github.com/bbs-land/remote-imaging-protocol/issues/250) use the original parser with Color Map mode and XY width 2. Every case has a first feed containing `!|` and the body, then a separate CR. No first feed dispatches. The published vectors record those captures. The following checks explain the different outcomes.

- `c0a` and its shortened form `ca` both decode to 36 in base 64. In base 36 the lowercase letter enters discard. A leading period in `c.` takes the [whole zero-field branch](issue-240-period-zero-field.md).
- `2s10a` needs its final `a` as a digit. By the time `2s100a` or `2s100-` reaches the suffix, its two fields are already pending in [state 8 at `0x3a469`](issue-237-fixed-numeric-dispatch.md). The suffix goes through the delimiter scan and never enters a digit validator.
- Slot 40 always uses MegaNum. `J1S` decodes to `1 × 36 + 28 = 64` under either initial setting. `J1s` fails validation under either setting.
- The four digits in slot 103 are a [reserved field before text](issue-245-image-reserved-fields.md#three-parser-paths). `1p0a00Tile.jpg` completes the field in base 64 and stores zero, then enters filename state 5 at `0x3a3d4`. `1p0000tile.jpg` completes the prefix in both bases. The filename cases come from the collected arguments before the image handler runs.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 9
run/riptel-inspect "$dll" --slot 40
run/riptel-inspect "$dll" --rva 0x7dd30 --length 128
objdump -d --start-address=0x100210b2 --stop-address=0x100211f6 "$dll"
objdump -d --start-address=0x1003a043 --stop-address=0x1003a0ae "$dll"
```
