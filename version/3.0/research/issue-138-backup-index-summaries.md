# Issue #138: Backup variable index summaries

[Research backlog](issue-backlog.md) · [Variable index](../ripscrip/9.1-text-variable-reference.md#saverestore--backup-areas)

## Scope and checkpoint

Started 2026-11-22T04:57:28Z on `issue-138-backup-index-summaries` from fork stable **3313754**, after #136 / PR #137 actual head 7f074ac was normal-merged with an identical tree. The active ten-minute monitor detected [#138](https://github.com/bbs-land/remote-imaging-protocol/issues/138) at 04:55:56Z (created 04:46:47Z). Updated 2026-11-22T04:47:55Z, zero comments, body SHA-256 `61c69d2549f37f160c92c80d4adc310f6cb27e960ed55cbd2f67835096881c38`. Next issue check is due about 05:05:56Z.

This is an editorial reconciliation with the already accepted #26 full entries and screen/aggregate contract, plus the adopted #136 mouse-backup details. No new capture semantics, original/client investigation/execution, 1.54/text/assets changes or new tests are needed. Correct the index's extra viewport/style capture, wrong aggregate order and implication that fixed aliases are unavailable. Scope the inherited SyncTERM comparison to its historical recorded implementation.

## Accepted sources and derivation

Read these unchanged files at starting stable 3313754 (reproduce with `git show 3313754:<path>`):

- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `3a440c4992f9bc414f6d1c5dbeeab2d14e5f98c205d762d15767fbefd5ac4de5`.
- `version/3.0/ripscrip/7.2-data-backup-areas.md`: SHA-256 `2890649a789ab7e28e9e7fec221a0724cdebb855f4c2f9e694c9a08e6b51f260`.
- `version/3.0/research/issue-26-screen-backups.md`: SHA-256 `d766b3fb662d69f247057dde9a072a263c895123691b5d7fe7d10728a4a61555`.
- `version/3.0/ripscrip/9.1-text-variable-reference.md`: SHA-256 `23c0c2f7352ed8c53ce4ed65515d2dd1ffd9b34120bef4d97152d7e52dfe8268`.

The [#26 derivation](issue-26-screen-backups.md) already records original SAVE/RESTORE/SAVEALL/RESTOREALL registrations, exact wrapper/serializer addresses and prior native results. Full entries and 7.2 now agree: screen snapshots have port-zero pixels and current-palette RGB, not viewport/style/status/other-table state; restore binds RGB to the live selected palette. Both aggregate wrappers use **TW, PORT, MOUSE, SCREEN, PAL, BUT, STYLE, ENV**, with independent failure/consumption and no rollback. All twenty zero-argument SAVE0..9/RESTORE0..9 aliases are registered, including RESTORE0. Parameterized forms are preferred for new content, not replacements that remove these names. Original evidence need not be reproduced to fix stale summaries.

## Next action

Align the introduction, save/restore section and four summary rows with full entries; update the neighboring SMF/RMF summaries to point to the now-completed logical snapshot/handler/publication contract. Preserve historical evidence labels without treating a legacy SyncTERM omission as a language limit. Verify both eight-family sequences/recipes against detailed bullets, all alias ranges and cross-links; run repository formatting/lint/link/diff checks. Publish a separate complete PR, normal-merge its actual head into fork stable, audit feedback/coverage/ancestry and resume monitoring.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
