# Issue #281 — RIP_HEADER numeric shorthand and recovery

[Issue #281](https://github.com/bbs-land/remote-imaging-protocol/issues/281) · [RIP_HEADER](../ripscrip/1.1-protocol-and-command-hierarchy.md#numeric-shorthand-and-segment-selectors) · [Research backlog](issue-backlog.md)

RealDeuce filed #281 at 01:08:18 UTC on 2026-11-25. The 01:09 UTC in-thread check found it with no comments. Branch `issue-281-header-numeric-shorthand` starts at fork stable `e58a96d090e9d3c8e556e2c3918f5fd53b78f735`. The existing scene header research records the semantic validator and each segment's actions. [Issue #5](issue-5-scene-headers.md).

The [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. All addresses below are RVAs at image base `0x10000000`. The report captures the original collector and calls the original validator separately. This derivation traces both against the original tables and collector instructions.

## Root and segment descriptors

Slot 32 at `0x80d20` is level 0 `h`, with widths `[2, 4, 2]` and handler `0x1cae1`. Slots 33 through 37 supply segment layouts. The type map at `0x81ccc` lists root slot 32 and maps type digits 0 through 9 to `[33, 33, 34, 34, 33, 35, 36, 36, 36, 37]`. All six records have flags `0x12`, selecting UltraNum and typed collection. Each segment includes its type as a width-one field. [Header framing and primary parser evidence](issue-5-scene-headers.md#header-framing-and-primary-parser-evidence).

## Numeric collection and selector boundaries

- State 3 at `0x3a01b–0x3a027` ignores DEL and branches on backslash. At `0x3a043–0x3a04e`, a leading period branches to whole-field zero completion at `0x3a6d2`. An interior period follows discard. The checks at `0x3a082–0x3a1b4` require a nonempty buffer and the last field of the active record for early completion at a delimiter. The ordinary conversion reads the buffered digits using the descriptor's UltraNum base. The descriptor flags leave the reserved-field shortcut disabled.
- Full-width and zero-field completion lead to state 4 at `0x3a9ba–0x3a9f6` and `0x3a733–0x3a77d`, respectively, when the record is complete. State 4 at `0x3a223–0x3a241` sends pipe, CR, LF and FF to dispatch state 9. The selector table at `0x3a247–0x3a261` accepts literal ASCII digits. Period and DEL fall outside the table's range and take the discard branch. A selected digit is reprocessed as a width-one number at `0x3a3a9–0x3a3c4`. The selector jump-table entry for `<` also leads to `0x3a261`.
- The adjacent `<` checks at `0x3a054–0x3a060` accept an additional historical numeric-field terminator. An empty buffer takes the same zero completion as a leading period. A nonempty buffer takes the conversion at `0x3a7a6–0x3a7fd`, then advances to the next ordinary field or to state 4 when the current record is complete. The driver accepts this spelling. The issue captures and documented authoring forms use the period and command-delimiter paths.
- The selector backslash branch at `0x3a397` saves state 4 and enters continuation state 6. The numeric backslash branch also enters state 6. At `0x3a400–0x3a429`, continuation skips DEL and physical line endings and reprocesses the next other byte in the saved state. A pipe therefore retains its meaning in that state. Discard state 12 at `0x3a581–0x3a5b9` checks for a pipe before its backslash continuation check and restarts command framing at the pipe.
- The header validator at `0x1774f` checks the complete decoded record. The declared segment count controls validation and application. It does not change field collection or selector boundaries.

## Staged report captures

The report fed the prefix and body first with live Base Math 36, then delivered CR or a pipe. The root and segment force the same UltraNum decoding in both cases.

| Body | At either delimiter |
| --- | --- |
| `h..01601` | Two whole-field zeros, count 1, type 6 and flags 1. Dispatch validates. |
| `h...` | Three whole-field zeros. Dispatch validates. |
| `h010000016.` | Revision 1, global flags 0, count 1, type 6 and flags 0. Dispatch validates. |
| `h0100000161` | Revision 1, global flags 0, count 1, type 6 and flags 1. Dispatch validates. |
| `h010000016` | The delimiter sees an empty flags field. No header dispatch. |
| `h01000001.01` | The period reaches the typed selector state. No header dispatch. |

The report also captured backslash consumption between numeric digits and immediately before a selector, ignored DEL inside a numeric field, discard on bare DEL at the selector boundary, and successful recovery to COLOR(2) through the pipe in `!|h01000001.01\|c02`.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 32
run/riptel-inspect "$dll" --rva 0x81ccc --length 44
objdump -d --start-address=0x1003a01b --stop-address=0x1003a42e "$dll"
objdump -d --start-address=0x1003a581 --stop-address=0x1003a5ca "$dll"
objdump -d --start-address=0x1003a7a6 --stop-address=0x1003a885 "$dll"
```
