# Issue #243 — Numeric shorthand in Bézier commands

[Issue #243](https://github.com/bbs-land/remote-imaging-protocol/issues/243) · [Bézier numeric shorthand](../ripscrip/2.2-curves.md#numeric-shorthand-in-bézier-commands) · [Research backlog](issue-backlog.md)

RealDeuce filed #243 at 19:29:54 UTC on 2026-11-24. The 19:37 UTC in-thread check found it ahead of #244. Branch `issue-243-bezier-numeric-shorthand` starts at fork stable `25d0bc3dd66265e5e08c951e1597bda13a0360d4`. The report supplies staged dispatch captures in both bases with XY width 2 and a separately fed CR. [Issue #15](issue-15-bezier-contracts.md#native-parser-checkpoint-and-recovered-record-shape) recovers the original descriptors, type classes and consumer checks. [Issue #237](issue-237-fixed-numeric-dispatch.md) establishes fixed-list completion and short final fields. [Issue #240](issue-240-period-zero-field.md) establishes the ordinary numeric zero-field shortcut.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. Addresses below are RVAs at image base `0x10000000`. Subtract `0xc00` for file offsets in `.text`.

- Slot 76 (`Z`) has flags `0x0003` and positive count 9 with eight XY descriptors followed by literal width 2. The last numeric field can be shortened at a delimiter at `0x3a10d–0x3a202`. When that field reaches full width, the ordinary fixed-list path enters state 8 and consumes surplus before dispatch. The first coordinate uses the zero shortcut at `0x3a04b–0x3a04e` and `0x3a6d2`.
- Slots 61 (`t`), 71 (`x`) and 77 (`z`) have positive count 2, widths 2 and 2, and flags `0x0013`. At `0x3a9ba–0x3a9f6` flag `0x10` sends the collector to state 4 after the headers. The period path at `0x3a733–0x3a77d` makes the same transition when a period completes the second header.
- State 4 checks for a delimiter or a literal selector at `0x3a223–0x3a25a`. A period is below the digit range and takes the discard branch at `0x3a261`. A valid selector binds the record descriptor and reprocesses that byte at width 1 in state 3 at `0x3a3a9–0x3a3be`. The following coordinates use the ordinary collector, with its period branch and one-field advancement. Complete coordinates return to the typed selector state. The ordinary delimiter path can finalize a short last coordinate.
- At a completed record boundary a space is an invalid selector. A single extra literal selector starts an incomplete record, so a delimiter before its coordinates gives no dispatch in the reported case. A valid typed list has exactly the declared record count. The existing #15 consumer validates the decoded list and count.

The [wire vectors](../ripscrip/2.2-curves.md#numeric-shorthand-in-bézier-commands) retain the report's raw arguments and stages. For example, `z02.0010100606` supplies the numeric header (2,0), followed by point records (0,1,1) and (0,6,6). Changing the first type byte to a period reaches a different parser state than changing its X coordinate to a period.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 76
run/riptel-inspect "$dll" --slot 77
objdump -d --start-address=0x1003a223 --stop-address=0x1003a3d4 "$dll"
objdump -d --start-address=0x1003a6d2 --stop-address=0x1003a788 "$dll"
objdump -d --start-address=0x1003a9ac --stop-address=0x1003aa1f "$dll"
```

---

[Research backlog](issue-backlog.md) · [Bézier numeric shorthand](../ripscrip/2.2-curves.md#numeric-shorthand-in-bézier-commands)
