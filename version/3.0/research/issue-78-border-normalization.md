# Issue #78: SET_BORDER value normalization

[Research backlog](issue-backlog.md) · [SET_BORDER](../ripscrip/2.3-shapes-and-fills.md#rip_set_border)

## Scope and checkpoint

Resolve [#78](https://github.com/bbs-land/remote-imaging-protocol/issues/78) on branch `issue-78-border-normalization` from fork stable **72edbfc**. The issue asks for the two-digit current-base value domain, zero/nonzero normalization, final-field shortening and current STYLE protection. It is independent of the already resolved border drawing operations. Issue #79 (SWITCH_STYLE width/protection flags) is queued next; both new issues have no comments at the 2026-11-18 15:46:29 UTC refresh. No later issue comments appeared after the preceding delivery watermark.

Use the preserved ALPHA 4 protocol documentation and read-only original instruction paths. Explicit protocol rules control; implementation evidence fills missing accepted-domain details. Do not investigate client bugs or launch native/GUI probes. Record exact source identity, normalization/protection branches, independent decode examples, uncertainty and validation before filing a complete issue-specific PR.

## Next action

Read SET_BORDER and syntax rule 10 in the preserved protocol; verify original handler 0xdcf1 and the two-digit dispatch descriptor. Then publish the normalized value contract with source boundaries, format/lint/check links, refresh comments, file the PR and consolidate completed work into fork stable. Keep all commits co-authored by Codex and remove scoped temporary API payloads.

---

[Research backlog](issue-backlog.md) · [SET_BORDER](../ripscrip/2.3-shapes-and-fills.md#rip_set_border)
