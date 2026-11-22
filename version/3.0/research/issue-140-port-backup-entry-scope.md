# Issue #140: SCB/RCB full PORT-family scope

[Research backlog](issue-backlog.md) · [PORT save area](../ripscrip/7.2-data-backup-areas.md#drawing-port-table-save-area)

## Scope and checkpoint

Started 2026-11-22T07:40:36Z on `issue-140-port-backup-entry-scope` from fork stable **95493d2643eaecfc1a02ca4264da2c0d8a1f406e**. The in-session ten-minute check detected [#140](https://github.com/bbs-land/remote-imaging-protocol/issues/140) at 07:39:11Z, after its creation at 07:38:41Z. Report updated 2026-11-22T07:38:41Z, zero comments, body SHA-256 `91a55a7eba4b0cd2843bc7b774d745f5f79b53a8b123155fa05d0874d3e8f4e5`. Next check due about 07:49:11Z. The user explicitly confirmed at 07:35Z that new issues should trigger research and a PR, not only notification. Keep the established serial PR/stable workflow; checks require the active session, with no persistent scheduler installed.

This editorial follow-up adopts #26 / PR #60 and #138 / PR #139. Align individual SCB/RCB entries, RCB's Format and 9.1 summaries with the accepted full PORT backup family. An empty clipboard pointer is saved state, not a reason to delete an otherwise valid PORT snapshot. Do not reopen screen/aggregate semantics, investigate/execute the original client, claim PCB implementation/pixel parity or change 1.54/text/assets.

## Accepted sources and derivation

Read these files at starting stable 95493d2; reproduce with `git show 95493d2:<path>`:

- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `3a440c4992f9bc414f6d1c5dbeeab2d14e5f98c205d762d15767fbefd5ac4de5`.
- `version/3.0/ripscrip/7.2-data-backup-areas.md`: SHA-256 `2890649a789ab7e28e9e7fec221a0724cdebb855f4c2f9e694c9a08e6b51f260`.
- `version/3.0/ripscrip/9.1-text-variable-reference.md`: SHA-256 `96b9917c72dab702b86365fae7f2e5c2a34dbaaf9f8a0f217ef68c181001e654`.
- `version/3.0/research/issue-26-screen-backups.md`: SHA-256 `d766b3fb662d69f247057dde9a072a263c895123691b5d7fe7d10728a4a61555`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

The accepted [PORT capture list](../ripscrip/7.2-data-backup-areas.md#drawing-port-table-save-area) includes selected port, 36 port definitions, offscreen contents, floating viewport query and assigned clipboard pointer; the following cursor exception and earlier query/protection derivations remain controlling. The [#26 record](issue-26-screen-backups.md) already establishes SCB/RCB as the PORT step of the eight-family wrappers and documents independent manager/storage/failure semantics. The [#16 whole-PORT backup result](issue-16-table-lifecycle.md#final-native-family-reset-and-backup-checkpoint) already records independently owned floating/per-screen-port query strings. No new original tracing or probe is necessary to reconcile the entry wording.

ALPHA 4 §5.11.2 lines 17795–17796 puts the closing dollar before the argument in Format but gives the correct `$RCB(opt:SOURCE)$` in Syntax; use that consistent syntax. Section 5.11.3 lines 17825–17840 supplies the clipboard-only/empty-file wording inherited by 5.1. This verifies the wording's documentary origin without asserting that an unexamined earlier client implemented it. That narrow wording conflicts with the already accepted full-family capture. Retain its historical scope explicitly, not as a second 3.0 rule. An empty assigned clipboard pointer must remain distinguishable from no PORT backup: a valid snapshot can contain definitions/offscreen data and an explicitly empty assigned pointer. Restore that pointer value without a paste, while applying the existing whole-PORT restore/protection/cursor exceptions. SCREEN and PCB remain separate operations.

## Editorial result and validation

The SCB/RCB entries and index now name the full PORT family; RCB's Format is corrected to `$RCB(source)$`. The PORT save-area chapter directly links these operations and states that empty assigned-pointer metadata does not discard definitions, queries or offscreen contents. The example saves usable PORT state with no assigned clipboard and restores that empty pointer after a later assignment, while retaining the existing family exceptions and reusable BASE. Screen restoration and PCB paste remain separate; no PCB implementation or exact pixel behavior is claimed. The ALPHA 4 empty-clipboard file-deletion sentence is explicitly historical wording superseded for this accepted 3.0 capture set.

Checks pass all five starting-file fingerprints against `git show 95493d2:<path>`, corrected Format versus syntax, full-family/empty-pointer/lifetime statements and both index rows with source evidence. Reviewed the empty-pointer example against the existing capture list, separate current selection and manager/restore exceptions. No new tests or original-client investigation/execution. Full lint/link/diff checks pass (6,256 links before final delivery metadata). Refresh at 07:42:23Z finds the original body unchanged, zero comments. Publish separately, consolidate actual head, audit feedback/coverage/ancestry and continue ten-minute checks.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
