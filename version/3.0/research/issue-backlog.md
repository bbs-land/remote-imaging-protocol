# Primary-source issue backlog

[Contents](../ripscrip/README.md) · [Agent workflow](../../../AGENTS.md)

## Resume here

Updated 2026-11-16. Issue #3 is delivered in [PR #29](https://github.com/bbs-land/remote-imaging-protocol/pull/29), from branch `issue-3-extended-font-style`; its [derivation note](issue-3-extended-font-style.md) contains the complete evidence record. Review/merge is pending.

Issue #4 is delivered in [PR #30](https://github.com/bbs-land/remote-imaging-protocol/pull/30), branch `issue-4-column-overflow`; see its [derivation note](issue-4-column-overflow.md). Review/merge is pending. It depends on #29 and provides an issue-specific comparison from `ca92e10`.

Issue #5 is delivered in [PR #31](https://github.com/bbs-land/remote-imaging-protocol/pull/31), branch `issue-5-scene-headers`; see its [derivation note](issue-5-scene-headers.md). Review/merge is pending. It depends on #30 and provides an issue-only comparison from `996b1a3`; primary specification commit is `ac4c168`.

Active: [issue #6](https://github.com/bbs-land/remote-imaging-protocol/issues/6), macro evaluation and predefined variables. Refreshed open issues confirm it is the oldest issue without a complete resolution PR. Branch `issue-6-macro-evaluation` starts at `9b1872a` to reuse the delivered #5 tools. Read the [issue note](issue-6-macro-evaluation.md), then continue the recorded primary traces and native probe: 179 original registrations and 66,198 native checks now cover the requested variable effects and macro pipelines, including actual memory-record deletion and a captured template-chain wrong-offset defect. Parser/conditional checks are complete; publish the derived contracts in chapters 5.1/5.2 and the variable index, then validate and file the complete #6 PR dependent on #31; no partial resolution has been filed. No resolution PR yet. All new commits carry the requested Codex co-author trailer.

## Queue and delivery

Initial queue (2026-11-16): issues #3 through #28, all filed by RealDeuce. Refresh open issues in creation order before selecting the next unaddressed issue. Issue #2 (BradHawthorne) is overlapping evidence, not the next queue entry. Completed resolution PRs may await review while research advances; dependent PRs may be stacked. Keep this page current as each issue progresses.

Fork: [RealDeuce/remote-imaging-protocol](https://github.com/RealDeuce/remote-imaging-protocol). Upstream base at start: `2fb17724b6122a5ad5cd1df38b69b8cce3a7079f`.

## Artifact root

The authorized local mirror is `/usr/home/admin/RIP/rip-tools/`, outside this checkout. It corresponds to `~/src/rip-tools/` in the [catalogue](../../../reference/rip-tools.md). Original artifacts remain byte-exact. Research notes identify sources and hashes so another machine can reproduce the investigation.

---

[Contents](../ripscrip/README.md) · [Agent workflow](../../../AGENTS.md)
