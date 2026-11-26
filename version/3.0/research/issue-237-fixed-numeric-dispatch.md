# Issue #237 — Fixed-width numeric collection and dispatch

[Issue #237](https://github.com/bbs-land/remote-imaging-protocol/issues/237) · [Numeric command dispatch](../ripscrip/1.1-protocol-and-command-hierarchy.md#numeric-collection-and-dispatch) · [Research backlog](issue-backlog.md)

## Sources and scope

RealDeuce filed #237 at 18:17:14 UTC on 2026-11-24. The 18:26 UTC in-thread check found the open report with no comments. Branch `issue-237-fixed-numeric-dispatch` starts at fork stable `989bdc5e33ea9a594dcaafaf0a33c05c11ca72ce`. The report supplies staged dispatch captures from the existing [framing probe](../../../run/lib/riptel-framing-probe.c) with a handler hook. Those captures establish arguments and dispatch timing for its stated inputs.

The [2.0 ALPHA 4 draft](../../2.0/text/RIPScrip-2.0-alpha-4.txt) at lines 3981–4026 defines consecutive numeric fields, shortening the last transmitted number, and omission of a trailing reserved field. Rule 15 at lines 4108–4113 prescribes discarding commands with excess data. The shipped 3.0 collector has a post-collection state that consumes surplus bytes before dispatch. [Issue #7](issue-7-framing-identification.md#distinguishing-streams-and-stages) establishes physical line endings, continuation and framing. [Issue #13](issue-13-polygon-counts.md#initial-original-dispatch-and-static-observations) establishes the separate variable-argument descriptor and budget.

The catalogued [PC Micro RIPtel 3.1 package](../../../reference/rip-tools.md) supplies original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. Addresses below are RVAs relative to image base `0x10000000`. For instruction offsets in `.text`, subtract `0xc00` from the RVA.

## Collection and dispatch

The dispatch table at RVA `0x80820` has 40-byte records. Slot 44 (`L`) has count +4 and four XY descriptors. Slot 28 (`f`) has count +2 and two XY descriptors. Slot 22 (`c`) has count +1 and the color-width descriptor. Slot 52 (`P`) has count −2, a two-digit number, and a repeated XY width. All four have flags `0x0003`.

- State 3 at `0x3a01b` collects numbers into the argument array. `0x3a90c–0x3a925` accumulates bytes until the current field is full. `0x3a960–0x3a97a` stores the decoded number and increments both the descriptor position and argument count. `0x3a996–0x3a9a7` sets the next field width while declared fields remain. On the ordinary fixed-field path, `0x3a9ac–0x3aa18` selects state 8 when the numeric fields end and the descriptor has no text. A signed negative count takes the branch at `0x3a9b4` and keeps collecting under the variable-argument rules. The count value inside a polygon stream is separate from the parser's signed descriptor count.
- The jump table at `0x3ab9c` maps state 8 to `0x3a469`. A pipe or a terminating CR, LF or form feed enters state 9 at `0x3a49b`. The other surplus bytes take the parser return path and leave the stored arguments intact. The line-ending checks at `0x3a481–0x3a495` suppress dispatch while a backslash is in either of the two prior-byte slots. Pipe goes directly to state 9. State 9 at `0x3a4a6` invokes the capture at `0x3a4e0` or the actual command handler at `0x3a515`, then returns to delimiter processing. So completing the fields and returning from a feed call have no dispatch effect by themselves.
- The delimiter branch in state 3 at `0x3a082–0x3a202` can store a nonempty shortened final field and dispatch on that same delimiter. It handles allowed omission of a trailing reserved argument separately. Non-numeric input other than a recognized delimiter reaches discard at `0x3a8a3`. In the reported `L0102030 commentary` case the space arrives before the final numeric field is complete. The delimiter then resumes framing without executing the partial LINE.

The report's ordinary `P040101060106060106` contains vertex count 4 and the four complete vertices (1,1), (6,1), (6,6), (1,6). The parser is still collecting numbers at that point. A delimiter forwards the collected fields. A space followed by commentary takes the discard branch. This example is below the existing numeric budget and does not alter the [polygon count rules](../ripscrip/2.3-shapes-and-fills.md#polygon-counts-and-wire-limits).

The [reference vectors](../ripscrip/1.1-protocol-and-command-hierarchy.md#numeric-collection-and-dispatch) retain the report's separate feed boundaries and expected slot transitions. Static reads confirm the state transitions. The reported captures supply the original execution outcomes.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 44 --rva 0x3ab9c --length 52
run/riptel-inspect "$dll" --slot 28
run/riptel-inspect "$dll" --slot 52
objdump -d --start-address=0x1003a01b --stop-address=0x1003a223 "$dll"
objdump -d --start-address=0x1003a469 --stop-address=0x1003a525 "$dll"
objdump -d --start-address=0x1003a8a3 --stop-address=0x1003aa1f "$dll"
```

---

[Research backlog](issue-backlog.md) · [Numeric command dispatch](../ripscrip/1.1-protocol-and-command-hierarchy.md#numeric-collection-and-dispatch)
