# Issue #275 — Master-color palette writes and publication

[Issue #275](https://github.com/bbs-land/remote-imaging-protocol/issues/275) · [RIP_SET_PALETTE](../ripscrip/2.0-color-modes-and-palettes.md#rip_set_palette) · [RIP_ONE_PALETTE](../ripscrip/2.0-color-modes-and-palettes.md#rip_one_palette) · [Research backlog](issue-backlog.md)

RealDeuce filed #275 at 23:12:40 UTC on 2026-11-24. The 23:18 UTC in-thread check found it with no comments. Branch `issue-275-master-palette-publication` starts at fork stable `1e689a30eaae57cee102d057c72aa73952dcba58`. The [D/d publication derivation](issue-174-palette-publication.md) establishes the six-bit palette record and portable publication logic.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses below are RVAs at image base `0x10000000`. Slot 18, `a`, has two two-digit fields and handler `0x1d135`. Slot 54, `Q`, has sixteen two-digit fields and handler `0x1f6a7`. Both descriptors have flags `0x0003` for active Base Math and ordinary fixed-field collection.

## Conversion and store order

Both handlers use the stored master-color bit pairs. Red uses bit positions 2 and 5 from high to low significance, green uses 1 and 4, and blue uses 0 and 3. Compare `0x1d1b8–0x1d1fd` and `0x1f728–0x1f76b`. The shared helper `0x4434e` calls `0x44321` per component. For levels 0, 1, 2 and 3, it returns 0, 84, 168 and 255. The handlers save each result shifted right by two into the selected palette's record. Its stored components are 0, 21, 42 and 63.

The graphics gate precedes palette edits. The `a` master range check is at `0x1d169–0x1d16c`, and the current-palette protection check is at `0x1d188–0x1d1aa`. For in-range indices, the immediate single-entry GDI call at `0x1d21d` is conditional on an index below 236. The logical store also occurs for entries 236–255. The protocol rejects indexes above 255 before updating the table.

`Q` checks current palette protection at `0x1f6d6` before its loop. The loop at `0x1f715–0x1f7cf` validates one master value, converts it, stores it in the current palette and advances. A value above 63 branches at `0x1f722` to the local error path at `0x1f81a` after any prior stores. The single 16-entry update at `0x1f7e6` is reachable only after all sixteen stores. The parser's digit checks occur before this handler, so a wire-format error has no stored prefix.

## Staged observations and portable policy

The report's original collector and handler captures use writable current palette 7 with an initially zeroed record. Only the delimiter dispatches. The GDI rows are call observations.

| Base Math | Body | Stored result and immediate request |
| --- | --- | --- |
| 36 | `a0F04`, `a0G04` | Entry 15 or 16 becomes (42, 0, 0), with request (168, 0, 0). |
| 36 | `a6J04`, `a6K04`, `a7304` | Entry 235, 236 or 255 becomes (42, 0, 0). Only 235 has an immediate request. |
| 64 | `a3&4`, `a.4` | Entry 255 or 0 becomes (42, 0, 0). Only 0 has an immediate request. |
| 36 | `a0G1S` | Master value 64 is rejected before storage. |
| 36 | `Q` plus fifteen `04` fields and `4` | All sixteen entries become (42, 0, 0). The final field is shortened. One update is requested. |
| 36 | `Q` plus fifteen `04` fields and `1S` | Entries 0–14 become (42, 0, 0). Value 64 fails. Entry 15 stays zero and there is no palette update request. |
| 36 | `Q` plus fifteen `04` fields and `0?` | No dispatch. The palette remains at its starting values. |

The protected versions of `a0G04` and the full valid Q report a protection error with no stores or publication. The portable rules accept logical indices 0–255, publish successful writes under the D/d mapping, and retain the witnessed Q prefix when a semantic range error occurs. That prefix is accessible through PALENTRY but becomes visible through the indexed mapping when its entries are next published. No physical 236-entry cutoff applies to portable publication.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 18
run/riptel-inspect "$dll" --slot 54
objdump -d --start-address=0x10044321 --stop-address=0x10044385 "$dll"
objdump -d --start-address=0x1001f715 --stop-address=0x1001f7ec "$dll"
```
