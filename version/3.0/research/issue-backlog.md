# Primary-source issue backlog

[Contents](../ripscrip/README.md) · [Agent workflow](../../../AGENTS.md)

## Resume here

Updated 2026-11-16. Issue #3 is delivered in [PR #29](https://github.com/bbs-land/remote-imaging-protocol/pull/29), from branch `issue-3-extended-font-style`; its [derivation note](issue-3-extended-font-style.md) contains the complete evidence record. Review/merge is pending.

Issue #4 is delivered in [PR #30](https://github.com/bbs-land/remote-imaging-protocol/pull/30), branch `issue-4-column-overflow`; see its [derivation note](issue-4-column-overflow.md). Review/merge is pending. It depends on #29 and provides an issue-specific comparison from `ca92e10`.

Issue #5 is delivered in [PR #31](https://github.com/bbs-land/remote-imaging-protocol/pull/31), branch `issue-5-scene-headers`; see its [derivation note](issue-5-scene-headers.md). Review/merge is pending. It depends on #30 and provides an issue-only comparison from `996b1a3`; primary specification commit is `ac4c168`.

Issue #6 is delivered in [PR #32](https://github.com/bbs-land/remote-imaging-protocol/pull/32), branch `issue-6-macro-evaluation`, dependent on #31. Its [derivation note](issue-6-macro-evaluation.md) records the complete answer, original instruction coverage, controlled-service limits and **66,202 native checks**; specification commit `1ccedf7`. The PR links an issue-only comparison from `9b1872a`. Review/merge is pending.

Issue #7 is delivered in [PR #34](https://github.com/bbs-land/remote-imaging-protocol/pull/34), branch `issue-7-framing-identification`, dependent on #32. Its [derivation note](issue-7-framing-identification.md) records the complete framing/identification answer, static client traces, **203 native framing checks** and **66,209 macro checks**; specification commit `a66fd63`. The PR links an issue-only comparison from `e450e1d`. Review/merge is pending.

Issue #8 is delivered in [PR #35](https://github.com/bbs-land/remote-imaging-protocol/pull/35), branch `issue-8-bmm-masks`, dependent on #34. Its [derivation note](issue-8-bmm-masks.md) records all BMM/renderer/button references, **4,975 native checks**, controlled-GDI limits, depth/palette/mode/key behavior and a four-pixel oracle; specification commit `1a42258`. The issue-only comparison starts at `a6da898`. Review/merge is pending.

Issue #9 is delivered in [PR #36](https://github.com/bbs-land/remote-imaging-protocol/pull/36), branch `issue-9-rgb-examples`, dependent on #35. Its [derivation note](issue-9-rgb-examples.md) records primary draft lines/hash and C grammar, both arithmetic corrections, original-text preservation and required checks; correction commit `23335d5`. The issue-only comparison starts at `cfa54b5`. Review/merge is pending.

Issue #10 is delivered in [PR #37](https://github.com/bbs-land/remote-imaging-protocol/pull/37), branch `issue-10-port-geometry`, dependent on #36. Its [derivation note](issue-10-port-geometry.md) records **1,207 allocation/query/copy-geometry checks** and **66,218 macro checks**, exact 640×480 allocation, no-scale clipping, all-zero precedence, failed PORTW/PORTH expansion and the diagram correction; specification commit `9034655`. The issue-only comparison starts at `1a0bcc8`. Review/merge is pending.

Issue #11 is delivered in [PR #39](https://github.com/bbs-land/remote-imaging-protocol/pull/39), branch `issue-11-pixel-pen-units`, dependent on #37. Its [derivation note](issue-11-pixel-pen-units.md) records **165,027 native checks**, both world-coordinate inputs, X's omitted origin, POINT's thickness square, the viewport producer's direct context coordinates and exact width/flag/byte behavior; specification commit `37553e5`. The issue-only comparison starts at `068f84c`. Review/merge is pending.

Issue #12 is delivered in [PR #40](https://github.com/bbs-land/remote-imaging-protocol/pull/40), branch `issue-12-fill-masks`, dependent on #39. Its [derivation note](issue-12-fill-masks.md) records all twelve original masks, **2,754 native checks**, canonical row encodings, direct BAR/K consumers and drawing-context phase at two world scales. All 288 inherited diagram rows match the original table; historical text remains byte-exact. Specification commit `ab03c0d`; issue-only comparison starts at `e0bc91f`. Review/merge is pending.

Active: [#13](https://github.com/bbs-land/remote-imaging-protocol/issues/13), branch `issue-13-polygon-counts` from `2754e2d` (delivered #12 / PR #40). Read the [derivation note](issue-13-polygon-counts.md). Original dispatch is fixed-2 count then repeating XY; multi-contour < has two fixed-2 initial numerals then repeating XY, including a possible later-contour-count width quirk. Parser argument cap is 0x2001; single-contour loops appear to convert N+1 pairs but pass N to GDI. Native verification passes 215,434 checks: original J/n settings at widths 2..5, both radices, all decoded points/draw counts, later-contour XY counts, 8193-value truncation, maximum contours/aggregate vertices and the stale extra conversion. Read the completed derivation before publishing exact limits/grammar and the PR. No PR yet.

## Queue and delivery

Initial queue (2026-11-16): issues #3 through #28, all filed by RealDeuce. Refresh open issues in creation order before selecting the next unaddressed issue. The 2026-11-16 refresh adds RealDeuce issue #33 after #28 (created later), and the post-#39 refresh adds #38 after #33. Issue #2 (BradHawthorne) is overlapping evidence, not the next queue entry. Completed resolution PRs may await review while research advances; dependent PRs may be stacked. Keep this page current as each issue progresses.

Fork: [RealDeuce/remote-imaging-protocol](https://github.com/RealDeuce/remote-imaging-protocol). Upstream base at start: `2fb17724b6122a5ad5cd1df38b69b8cce3a7079f`.

## Artifact root

The authorized local mirror is `/usr/home/admin/RIP/rip-tools/`, outside this checkout. It corresponds to `~/src/rip-tools/` in the [catalogue](../../../reference/rip-tools.md). Original artifacts remain byte-exact. Research notes identify sources and hashes so another machine can reproduce the investigation.

---

[Contents](../ripscrip/README.md) · [Agent workflow](../../../AGENTS.md)
