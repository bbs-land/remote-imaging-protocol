# Issue #101: Extended text windows and terminal controls

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md) · [Variables](../ripscrip/5.1-text-variables.md)

## Scope and checkpoint

Started 2026-11-20 on `issue-101-extended-text-window-contract` from fork stable 93fd0c3 after #100 / [PR #102](https://github.com/bbs-land/remote-imaging-protocol/pull/102). That stable merge includes the actual PR head dfbca09 and has an identical source tree. [Issue #101](https://github.com/bbs-land/remote-imaging-protocol/issues/101) requests automatic fitting, MAF versus nominal metrics, coordinate/undersize/validation rules, erase sources/gates, TWGOTO and COFF corrections. Preserve #16/#51 and 1.54. Separate primary facts from explicit completions of unspecified cases; no obsolete-client bug investigation or original/client/UI/probe execution.

## Evidence checkpoint and remaining work

Original `b` dispatch slot 20 -> RVA `0x1b075`, nine arguments, flags `0x0b`. Its first four types are XY; width/height are fixed two characters, font one, flags four, reserved three. It rejects zero requested dimensions, rejects fonts >= 5 unless flag 8, checks current TW protection, converts through WORLD helper `0x31084`, orders endpoints and rejects an empty device rectangle. The ordinary path does not add graphics PORT or viewport offsets. The original accepts additional undocumented flags up to `0x3ff`, including a PORT-relative branch under flag `0x80`; do not silently adopt undocumented flags into the six-flag protocol. Reserved argument is parsed but unread by this handler. Remaining: follow fitting/reset/clear and macro consumers; choose clearly labeled rules for genuinely undocumented cases; update reference and derivation; validate, publish and consolidate.

---

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md)
