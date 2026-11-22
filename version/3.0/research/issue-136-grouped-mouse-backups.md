# Issue #136: Grouped mouse backup capture and publication

[Research backlog](issue-backlog.md) · [Mouse backups](../ripscrip/7.2-data-backup-areas.md#mouse-field-table-save-area)

## Scope and checkpoint

Started 2026-11-22T04:28:40Z on `issue-136-grouped-mouse-backups` from fork stable **49a69454a0bd6055c0d11bb415759d309519de33**. The user requested ten-minute checks; the in-session monitor found [#136](https://github.com/bbs-land/remote-imaging-protocol/issues/136) at 04:05:21Z after its creation at 04:02:44Z. The latest empty check was 04:25:42Z; next check is due about 04:35:42Z. Continue the established research/PR workflow by default and queue newer reports behind this one. No persistent scheduler was installed; checks require the active session. Report body SHA-256 `450dc057804b7d7865ca10c96df456fbf98a4c126ca742346dffbbf2a3d3ea2c`, updated 2026-11-22T04:02:44Z, zero comments at this checkpoint.

The report adopts #131 / PR #134 grouped ownership/publication and #132 / PR #135 mouse-family handler restore together. Complete precisely which selected state, per-owner/per-slot bodies and logical button configuration a mouse backup captures, and how applying it affects the current global baselines/publication slots without providers/painting or old continuation identity. Preserve base/numbered/stack manager semantics, saved handlers, fresh field tokens/outside membership and SOFT template preservation. No original/client/UI/probe execution or bug investigation; no 1.54/text/asset changes.

## Next evidence and completion

Read ALPHA 4's mouse save definition and unchanged original mouse serializer/restore, plus the adopted #131/#132 models. Distinguish the native one-body record from the explicitly completed multi-slot owner model. Specify successful snapshot ownership, selected versus pressed state, raw bodies versus aggregate caps, exclusion of global ordinary baselines/publication tags, pure publication rebuild at restore and partial failure boundaries. Include literal command/state/provider vectors, validate source and arithmetic, checkpoint notes, publish one complete PR and normal-merge its actual head into fork stable. Refresh all feedback/coverage/current head ancestry before returning to monitoring.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
