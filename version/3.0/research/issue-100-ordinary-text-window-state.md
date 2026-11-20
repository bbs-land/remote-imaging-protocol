# Issue #100: Ordinary text-window redefinition

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md#rip_text_window)

## Scope and checkpoint

Started 2026-11-20 from fork stable b8c70aa on `issue-100-ordinary-text-window-state`. [Issue #100](https://github.com/bbs-land/remote-imaging-protocol/issues/100) asks for ordinary `w` redefinition state, pixel effects, deactivation/reactivation and a corrected font-2 example. #101 follows separately. Preserve the adopted #16 table/backup and #51 query-dispatch contracts and RIPscrip 1.54.

## Source plan and remaining work

Read the original ALPHA 4 command and text-window state model first, then inspect bounded original RIPtel 3.1 driver definition/deactivation consumers without executing original instructions, callbacks, prompts or UI. Separate protocol guarantees, read-only implementation evidence and genuinely unspecified modern extensions such as UTF-8. Do not import extended `b` reset rules into `w` by analogy. Remaining: trace state writes and display calls; resolve every report question; update the canonical reference; validate, publish a separate PR and merge its actual head into fork stable. No temporary files created.

---

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md)
