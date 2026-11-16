# Primary-source issue backlog

[Contents](../ripscrip/README.md) · [Agent workflow](../../../AGENTS.md)

## Resume here

Updated 2026-11-16. Issue #3 is delivered in [PR #29](https://github.com/bbs-land/remote-imaging-protocol/pull/29), from branch `issue-3-extended-font-style`; its [derivation note](issue-3-extended-font-style.md) contains the complete evidence record. Review/merge is pending.

Issue #4 is delivered in [PR #30](https://github.com/bbs-land/remote-imaging-protocol/pull/30), branch `issue-4-column-overflow`; see its [derivation note](issue-4-column-overflow.md). Review/merge is pending. It depends on #29 and provides an issue-specific comparison from `ca92e10`.

Active: [issue #5](https://github.com/bbs-land/remote-imaging-protocol/issues/5), branch `issue-5-scene-headers`; read its [derivation note](issue-5-scene-headers.md). No resolution PR yet. The branch starts at `996b1a3` to reuse the delivered #3/#4 tools and workflow.

Next action: recover header dispatch discrimination and segment effects from the driver, then trace each unresolved entry point. Stay on #5 until every requested item is resolved or explicitly classified from complete primary evidence. All new commits carry the requested Codex co-author trailer.

## Queue and delivery

Initial queue (2026-11-16): issues #3 through #28, all filed by RealDeuce. Refresh open issues in creation order before selecting the next unaddressed issue. Issue #2 (BradHawthorne) is overlapping evidence, not the next queue entry. Completed resolution PRs may await review while research advances; dependent PRs may be stacked. Keep this page current as each issue progresses.

Fork: [RealDeuce/remote-imaging-protocol](https://github.com/RealDeuce/remote-imaging-protocol). Upstream base at start: `2fb17724b6122a5ad5cd1df38b69b8cce3a7079f`.

## Artifact root

The authorized local mirror is `/usr/home/admin/RIP/rip-tools/`, outside this checkout. It corresponds to `~/src/rip-tools/` in the [catalogue](../../../reference/rip-tools.md). Original artifacts remain byte-exact. Research notes identify sources and hashes so another machine can reproduce the investigation.

---

[Contents](../ripscrip/README.md) · [Agent workflow](../../../AGENTS.md)
