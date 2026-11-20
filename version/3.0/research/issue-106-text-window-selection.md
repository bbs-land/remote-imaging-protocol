# Issue #106: Text-window selection state boundary

[Research backlog](issue-backlog.md) · [Window selection](../ripscrip/1.4-terminal-ansi-view.md#rip_switch_text_window) · [CUR](../ripscrip/5.1-text-variables.md#cur)

## Scope and checkpoint

Started 2026-11-20 on `issue-106-text-window-selection` from stable 7c606f0 after #105 / PR #109 actual head 773da35 was merged with an identical tree. [Issue #106](https://github.com/bbs-land/remote-imaging-protocol/issues/106) requests first-use state copying, existing/protected selection, connection-wide partial ANSI input and pixel/cell/history boundaries for `2T` and CUR(TW,n). New #108 geometry-query grammar follows in creation order. Preserve the previously adopted table-copy/backup, definition and charset contracts and separate FastFont research.

## Remaining work

Read ALPHA 4 selection/state/protection rules and inspect bounded original selection/copy/ANSI-parser consumers without executing them. Distinguish primary first-use-copy protocol from native fallback initialization, and choose explicit boundaries only where original documentation is incomplete. Add exact field/ownership table and literal transition vector; validate, publish separately and consolidate. No original/client/UI/probe execution or bug investigation; no task temporary files.

---

[Research backlog](issue-backlog.md) · [Window selection](../ripscrip/1.4-terminal-ansi-view.md#rip_switch_text_window)
