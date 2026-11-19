# Issue #93: template embedding limits and suffix effects

[Research backlog](issue-backlog.md) · [Templates](../ripscrip/5.2-templates-and-conditionals.md)

## Scope and checkpoint

Resolve newly arrived [#93](https://github.com/bbs-land/remote-imaging-protocol/issues/93) on `issue-93-template-embedding-limits` from fork stable **7a06ba6**, after #90/PR #92 and #91/PR #94 are complete and consolidated. It was created at 2026-11-19 19:17:46 UTC and has no comments at initial inspection.

Preserve adopted two ordinary passes, same-pass insertion skipping, deferred outer control/backslash conversion, preprocessing/segmentation order, intended chain correction and eight-pass angle behavior. Specify exact 4096-byte enforcement timing/quantity, discarded versus retained suffix effects, direct response/template-definition bounds and boundary/second-pass growth vectors. The report explicitly accepts portable policy and does not request unsafe native reproduction. Read protocol sources; label choices, do not investigate client bugs or execute original routines/UI/callbacks.

## Initial sources and next action

Preserved ALPHA 4 lines 12521–12523 say embedding that grows beyond 4096 bytes is truncated; lines 12675–12677 separately give the final chaining bound. Earlier #6 source/probe evidence establishes storage of raw template bodies, ordinary response executor `0x4acd9`, scanner `0x4b0e4`, same-pass insertion skipping and the two host passes. It does not establish every requested truncation/suffix guarantee. Choose and document a complete safe policy, independently calculate vectors, publish one complete PR closing #93 and consolidate its actual head into fork stable. Preserve co-author trailers and scoped temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Templates](../ripscrip/5.2-templates-and-conditionals.md)
