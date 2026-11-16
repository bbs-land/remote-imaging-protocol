# Primary-source issue backlog

[Contents](../ripscrip/README.md) · [Agent workflow](../../../AGENTS.md)

## Resume here

Updated 2026-11-16. Issue #3 is delivered in [PR #29](https://github.com/bbs-land/remote-imaging-protocol/pull/29), from branch `issue-3-extended-font-style`; its [derivation note](issue-3-extended-font-style.md) contains the complete evidence record. Review/merge is pending.

Issue #4 is delivered in [PR #30](https://github.com/bbs-land/remote-imaging-protocol/pull/30), branch `issue-4-column-overflow`; see its [derivation note](issue-4-column-overflow.md). Review/merge is pending. It depends on #29 and provides an issue-specific comparison from `ca92e10`.

Issue #5 is delivered in [PR #31](https://github.com/bbs-land/remote-imaging-protocol/pull/31), branch `issue-5-scene-headers`; see its [derivation note](issue-5-scene-headers.md). Review/merge is pending. It depends on #30 and provides an issue-only comparison from `996b1a3`; primary specification commit is `ac4c168`.

Issue #6 is delivered in [PR #32](https://github.com/bbs-land/remote-imaging-protocol/pull/32), branch `issue-6-macro-evaluation`, dependent on #31. Its [derivation note](issue-6-macro-evaluation.md) records the complete answer, original instruction coverage, controlled-service limits and **66,202 native checks**; specification commit `1ccedf7`. The PR links an issue-only comparison from `9b1872a`. Review/merge is pending.

Issue #7 is delivered in [PR #34](https://github.com/bbs-land/remote-imaging-protocol/pull/34), branch `issue-7-framing-identification`, dependent on #32. Its [derivation note](issue-7-framing-identification.md) records the complete framing/identification answer, static client traces, **203 native framing checks** and **66,209 macro checks**; specification commit `a66fd63`. The PR links an issue-only comparison from `e450e1d`. Review/merge is pending.

Issue #8 is delivered in [PR #35](https://github.com/bbs-land/remote-imaging-protocol/pull/35), branch `issue-8-bmm-masks`, dependent on #34. Its [derivation note](issue-8-bmm-masks.md) records all BMM/renderer/button references, **4,975 native checks**, controlled-GDI limits, depth/palette/mode/key behavior and a four-pixel oracle; specification commit `1a42258`. The issue-only comparison starts at `a6da898`. Review/merge is pending.

Active: [issue #9](https://github.com/bbs-land/remote-imaging-protocol/issues/9), RGB example and palette-index expression, branch `issue-9-rgb-examples` from `cfa54b5` (delivered #8 / PR #35). Read its [derivation note](issue-9-rgb-examples.md). Source arithmetic settles both corrections: packed 04140C / blue 12, and parenthesized C shifts giving index 137 for BLUE=1/GREEN=2/RED=3. Next: correct both Markdown chapters and the 2.0 errata, preserve original text, validate and file the complete PR. No disassembly is needed. All new commits carry Codex co-authorship.

## Queue and delivery

Initial queue (2026-11-16): issues #3 through #28, all filed by RealDeuce. Refresh open issues in creation order before selecting the next unaddressed issue. The 2026-11-16 refresh adds RealDeuce issue #33 after #28 (created later). Issue #2 (BradHawthorne) is overlapping evidence, not the next queue entry. Completed resolution PRs may await review while research advances; dependent PRs may be stacked. Keep this page current as each issue progresses.

Fork: [RealDeuce/remote-imaging-protocol](https://github.com/RealDeuce/remote-imaging-protocol). Upstream base at start: `2fb17724b6122a5ad5cd1df38b69b8cce3a7079f`.

## Artifact root

The authorized local mirror is `/usr/home/admin/RIP/rip-tools/`, outside this checkout. It corresponds to `~/src/rip-tools/` in the [catalogue](../../../reference/rip-tools.md). Original artifacts remain byte-exact. Research notes identify sources and hashes so another machine can reproduce the investigation.

---

[Contents](../ripscrip/README.md) · [Agent workflow](../../../AGENTS.md)
