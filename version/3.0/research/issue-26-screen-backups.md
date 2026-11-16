# Issue #26: screen backup capture and restore transactions

[Research backlog](issue-backlog.md) · [Screen save area](../ripscrip/7.2-data-backup-areas.md#screen-save-area) · [Screen variables](../ripscrip/5.1-text-variables.md#screen-related-text-variables)

## Scope and primary artifact

Started 2026-11-16 after [PR #59](https://github.com/bbs-land/remote-imaging-protocol/pull/59) delivered #25. Branch `issue-26-screen-backups` starts at `4a04786`. [Issue #26](https://github.com/bbs-land/remote-imaging-protocol/issues/26) asks for exact SAVE/RESTORE versus numbered aliases versus SAVEALL/RESTOREALL capture sets, viewport/current-port and palette/current-palette/protection ownership, status-bar visibility, consume/protection behavior and real versus inherited RIPTERM*.SAV side effects. Pixels need not be reproduced exactly. Stay on this issue until every question is answered.

Original RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`; extracted `ripscrip.dll` 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Original bytes remain unchanged in the external artifact mirror. Addresses are RVAs, PE base `0x10000000`. Capstone 5.0.7 / clang 19.1.7 / FreeBSD 15.1. Native/static first; never desktop Wine. Use scoped temporary builds and remove after use.

## Initial callback leads

Original registration identifies SAVE `0x2493a`, RESTORE `0x2475d`, SAVEALL `0x27332`, RESTOREALL `0x270c2`. Numbered wrappers: SAVE0 `0x249e2`, SAVE1 `0x24a25`, then 0x17-byte strides through SAVE9 `0x24add`; RESTORE0 `0x24816`, RESTORE1 `0x2486b`, then 0x17-byte strides through RESTORE9 `0x24923`. Name-push RVAs SAVE `0x2689a`, RESTORE `0x2672a`, SAVEALL `0x26997`, RESTOREALL `0x26827`; numbered aliases cover `0x268b1..0x26980` and `0x26741..0x26810`. These are static leads; confirm original registration natively.

Existing [#16](issue-16-table-lifecycle.md) and [#24](issue-24-drawing-state-ownership.md) establish generic copy/reset and particular STYLE/PORT backup defects, not the screen transaction. In particular, STYLE restore loses its current index and PORT restore discards saved screen cursors. Keep actual screen capture versus wrapper compositions separate, and test protected/redefined/deleted target behavior rather than copying inherited ownership lists.

Next: trace original callbacks into backup manager/serializers, identify exact memory/file/bitmap boundaries and transaction ordering, then add native capture/restore and alias/protection fixtures before updating the specification. No completed answer or #26 specification edits yet.
