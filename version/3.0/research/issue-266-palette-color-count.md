# Issue #266 — Drawing-palette fields and decoded color count

[Issue #266](https://github.com/bbs-land/remote-imaging-protocol/issues/266) · [Bulk palette write](../ripscrip/2.0-color-modes-and-palettes.md#rip_set_drawing_palette) · [Single palette write](../ripscrip/2.0-color-modes-and-palettes.md#rip_one_drawing_palette) · [Research backlog](issue-backlog.md)

RealDeuce filed #266 at 22:23:35 UTC on 2026-11-24. The 22:25 UTC verification check found it with no comments. Branch `issue-266-palette-color-count` starts at fork stable `ad464cde66165ff3eb2f2a0ea4e971e173da4587`. The [palette publication derivation](issue-174-palette-publication.md) establishes six-bit storage, indexed publication and the portable remaining-range rule. This report addresses collection before those operations.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The 40-byte records start at RVA `0x80820`, signed count at offset 16, signed widths at offset 20 and flags at offset 38. Addresses below are RVAs at image base `0x10000000`.

| Slot | Opcode | Signed count | Widths     | Flags    | Handler   |
| ---- | ------ | ------------ | ---------- | -------- | --------- |
| 23   | `D`    | −4           | 2, 2, 1, 4 | `0x0002` | `0x1f46a` |
| 24   | `d`    | 3            | 2, 1, 4    | `0x0002` | `0x1cf95` |

The [radix selector](issue-250-meganum-receive-alphabet.md#validation-before-conversion) forces UltraNum for flag pair `10`. This applies to the header values as well as the colors. The [leading-period branch](issue-240-period-zero-field.md) stores one zero, increments the field and argument indices, and clears the digit count. A period inside a partially collected field reaches digit validation, which rejects it.

For `D`, the negative count gives a three-field minimum and a repeating fourth width descriptor. At `0x3a733–0x3a73b`, the completed-period branch keeps a variable command in numeric state. The full-field branch checks the signed count at `0x3a9ac–0x3a9b4` with the same result. The delimiter path at `0x3a1b6–0x3a1d7` accepts a variable command after at least the three header fields. The preceding short-field paths can add one nonempty short color when the delimiter arrives. Surplus values are collected as more colors. In `d`, collecting three fields enters the [fixed-field delimiter wait](issue-237-fixed-numeric-dispatch.md) instead.

At `0x1f4ba–0x1f4c8` the bulk handler tests declared count minus total arguments against −3, equivalent to requiring exactly the declared number of colors after the three header operands. This happens before the palette stores. The precision checks are `0x1f52c–0x1f52f` for `D` and `0x1cff2–0x1cff5` for `d`. Both require eight. The existing portable range rule rejects the entire update if the start and count exceed the logical palette.

## Staged captures

The report supplied the original parser arguments with separate `!|`, body and CR feeds. No dispatch occurs before CR. The results hold under both initial Base Math settings. A collected tuple still goes through the handler checks.

| Body                    | Arguments at CR                      |
| ----------------------- | ------------------------------------ |
| `D..8`                  | (0, 0, 8)                            |
| `D..8.`                 | (0, 0, 8, 0), count mismatch         |
| `D02.8.1234`            | (2, 0, 8, 0, 270532)                 |
| `D02.81234.`            | (2, 0, 8, 270532, 0)                 |
| `D02.8.1`               | (2, 0, 8, 0, 1)                      |
| `D02.81.`               | No dispatch                          |
| `D01008.1234`           | (1, 0, 8, 0, 270532), count mismatch |
| `D01008. note`          | No dispatch                          |
| `d.8. note`, `d.8.1234` | (0, 8, 0)                            |
| `d0z8&`                 | (61, 8, 63)                          |
| `d..4ZHM`               | (0, 0, 1193046), precision rejected  |

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 23
run/riptel-inspect "$dll" --slot 24
objdump -d --start-address=0x1001f4ba --stop-address=0x1001f54b "$dll"
objdump -d --start-address=0x1001cff2 --stop-address=0x1001d011 "$dll"
```
