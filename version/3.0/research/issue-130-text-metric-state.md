# Issue #130: Real-font metrics and retained TEXTDATA

[Research backlog](issue-backlog.md) · [TEXT_METRIC](../ripscrip/3.0-text-output-and-fonts.md#rip_text_metric)

## Scope and checkpoint

Started 2026-11-22 on `issue-130-text-metric-state` from stable 21728db. [Issue #130](https://github.com/bbs-land/remote-imaging-protocol/issues/130) requests actual font-family width/height formulas and numeric examples, preparation/cache/failure behavior, retained TEXTDATA/reset/backup scope, setter/prompt snapshot timing and byte/codepage boundaries. Adopt #5 prefix/modes/domains/signed reply/empty distinction, #86 ordinary outline width and #124 graphical expansion. RIPscrip 1.54 is outside scope. Explicit portable completion is permitted; no original/client/UI/probe execution or obsolete-client defect investigation.

Initial body SHA-256 `c7705ad304cbf9758e08d1fd49c2376b6ebfbfed3b8bdfe66d771ec0f6b31685`, updated 2026-11-22T01:37:57Z; zero comments. Refresh started 2026-11-22T02:20:24Z finds only #130/#131 uncovered and no newer discussion/inline/review feedback. Complete/publish/consolidate #130 before #131.

## Evidence plan and remaining work

Read primary font/metric documentation, adopted cached-width/RFF/CHR/layout notes, original r handler/font metric initialization and retained-text/reset/backup consumers. Record exact pinned source hashes/RVAs and source versus portable completion, derive numeric examples from original font bytes, validate independent byte/arithmetic/lifetime vectors and repository checks. Publish a separate PR, consolidate its actual head, then #131.

---

[Research backlog](issue-backlog.md) · [TEXT_METRIC](../ripscrip/3.0-text-output-and-fonts.md#rip_text_metric)
