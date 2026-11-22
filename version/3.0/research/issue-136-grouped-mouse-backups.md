# Issue #136: Grouped mouse backup capture and publication

[Research backlog](issue-backlog.md) · [Mouse backups](../ripscrip/7.2-data-backup-areas.md#mouse-field-table-save-area)

## Scope and checkpoint

Started 2026-11-22T04:28:40Z on `issue-136-grouped-mouse-backups` from fork stable **49a69454a0bd6055c0d11bb415759d309519de33**. The user requested ten-minute checks; the in-session monitor found [#136](https://github.com/bbs-land/remote-imaging-protocol/issues/136) at 04:05:21Z after its creation at 04:02:44Z. The latest empty check was 04:25:42Z; next check is due about 04:35:42Z. Continue the established research/PR workflow by default and queue newer reports behind this one. No persistent scheduler was installed; checks require the active session. Report body SHA-256 `450dc057804b7d7865ca10c96df456fbf98a4c126ca742346dffbbf2a3d3ea2c`, updated 2026-11-22T04:02:44Z, zero comments at this checkpoint.

The report adopts #131 / PR #134 grouped ownership/publication and #132 / PR #135 mouse-family handler restore together. Complete precisely which selected state, per-owner/per-slot bodies and logical button configuration a mouse backup captures, and how applying it affects the current global baselines/publication slots without providers/painting or old continuation identity. Preserve base/numbered/stack manager semantics, saved handlers, fresh field tokens/outside membership and SOFT template preservation. No original/client/UI/probe execution or bug investigation; no 1.54/text/asset changes.

## Sources and bounded recovered facts

ALPHA 4 section 2.3.2.6, lines 2457–2477 of the unchanged [text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, says save all live simple/button definitions and boundaries/configuration, but restore no displayed button graphics. Source implementation evidence is the unchanged RIPtel 3.1 / RIPscrip 3.0.7 DLL from the [mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Base 0x10000000; addresses are RVAs and text file offsets are RVA−0xc00. LLVM objdump 19.1.7 reads bytes only.

- Mouse save `0x230a8` writes count at `0x230e3`, entry/exit strings at `0x2311c/0x23155 -> 0x21ec2`, each **0xa3-byte field record** at `0x231aa -> 0x215a5`, then its owned host string at `0x231c4` and single associated body at `0x231de -> 0x21ec2`. The full raw record includes field flags +0x20 (selected bit 8), captured button configuration including radio flag +0x29 bit 0x40/checkbox +0x2a bit 1, group +0x2c and identity +0xa0. It does not serialize the independent numbered-template slot table or publication tags.
- String writer `0x21ec2` writes a zero length for NULL; otherwise `0x21f10 -> 0x654a8`, `0x21f15` increments the length for NUL, then writes the length and bytes at `0x21f2b/0x21f5d`. Reader `0x2288f` reads the length, allocates separate storage at `0x228f9 -> 0x65ca3`, reads at `0x22924 -> 0x21777`, and installs that owned pointer at `0x2295b`. This supports byte preservation and independent storage, not a native multi-slot A[N] record.
- Restore `0x23274` clears live fields at `0x23285 -> 0x27f8f`, releases handlers at `0x23296/0x232a4`, restores count at `0x232ed`, handlers at `0x23324/0x23363 -> 0x2288f`, each record at `0x233c2 -> 0x21777` and field host/associated strings at `0x233dd/0x233f8 -> 0x2288f`. No host parsing, rendering or template-definition publication call is part of that serializer/reader. The clear helper also calls native global-slot cleanup at `0x27fc6 -> 0x489cd`; adopted #131/#132/SOFT protocol policies control instead of importing that cleanup into the completed publication model.
- The original serializer has one associated-body pointer, not per-slot maps. Saving all A[N], absent-versus-defined-empty entries, separately persistent selection and global-publication reconciliation are therefore **explicit completion** of #131's abstract state, not an asserted legacy file layout. Similarly, #113/#132 intentionally discard serialized transient hover/pressed state and give restored fields fresh instance tokens.

## Chosen completion and rationale

A successful mouse snapshot owns its ordered logical field definitions, persistent selection, group kinds/numbers, full retained per-owner A[N] maps and both handler definitions. Save only state already committed/reached at the capture point, including OFF members' maps and initially selected members with no map. Preserve individual retained bodies (each existing 4096-byte bound), not only the clipped aggregate. Exclude momentary depression, capture, hover membership, old tokens/continuations, displayed pixels, other data tables and **global B[N]/publisher/effective slots**. Save performs no host/label/filename evaluation; reuse never aliases mutable live maps.

At applying restore, keep current ordinary baselines and publication ownership across the operation's own teardown. Replace fields with saved definitions and fresh tokens, restoring selected bits without activation or creation rendering. A logical group for this reconciliation is its **kind and numeric group**, not a public ID or old token. A current group publisher survives only if a group of the same kind/number exists in the applied result, then rebuild its currently owned slots from restored selected A[N] in saved order. An ordinary publisher stays ordinary; another group's publication is not stolen merely because a restored owner has an A[N]. A vanished/type-changed publishing group releases to the live baseline. Saved maps do not save/reclaim publication rights. This avoids intermediate last-member teardown accidentally losing a still-surviving group's right to rebuild, while preserving later ordinary writes.

Use one pure reconciliation after the applied complete field records, including the remaining partial result on failure, without reverting earlier handler/field changes or other RESTOREALL families. Reject/no-op before application changes nothing. Do not admit an incomplete owner/map. Existing manager retention/protection and base/numbered/POP semantics remain controlling, not a new transaction guarantee. A restored radio selected state controls required-selection lookup independently from empty/absent bytes. Old committed sources/FIELDID may finish under #131/#132, but old association writes cannot target restored replacements. No synthesized host action, entry/exit, retry or graphics restoration.

## Validation and delivery

The reference now publishes capture/exclusion and current-publication reconciliation tables, reusable BASE/later ordinary assignment/cross-slot/other-group/type-change vectors, retained-body caps, selected/pressed and continuation/provider examples. Corrected RMF's inherited single-saved-set claim and tied it to actual per-family backup capacity/private storage. Read-only checks pass both original fingerprints, nineteen direct-call targets and four decisive byte sequences. Independent checks decode the 36-byte style and both numeric button prefixes/IDs/flags/payload starts, and verify ten publication/deep-copy/capped-body scenarios. Reviewed all zero-provider/gesture/required-selection examples against adopted #131/#132 stages. Full lint, link/diff checks pass (6,215 links before final metadata). Refresh at 04:34:30Z retains the original body and zero comments. Delivered in [PR #137](https://github.com/bbs-land/remote-imaging-protocol/pull/137), open/ready with exactly #136 as closing reference; specification 75b2699. Normal-merge the actual head. Continue in-session ten-minute checks; no original/client/UI/probe/provider execution.

```sh
objdump -d --start-address=0x100230a8 --stop-address=0x100234d2 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10021ec2 --stop-address=0x10021f8f ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002288f --stop-address=0x10022968 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

## Consolidation and monitoring checkpoint

[PR #137](https://github.com/bbs-land/remote-imaging-protocol/pull/137) actual head **7f074ac** is normal-merged into fork stable through **508eb71**, with an identical source tree. Audit started 2026-11-22T04:37:41Z verifies all 67 current PR heads ancestral, all 68 open RealDeuce issues covered, unchanged #136 and no new report/comment/review feedback. All five new commits through that merge carry Codex co-authorship. Only Markdown changed; original assets/text/1.54 and separate FastFont branch remain untouched. No original/client/UI/probe/provider execution or task temporary files. See the [backlog checkpoint](issue-backlog.md#resume-here).

No active issue investigation remains. In-session ten-minute checks continue, latest all-author check 04:35:34Z; next due about 04:45:34Z. Empty checks stay quiet and new RealDeuce reports use the established serial workflow by default. No persistent scheduler is installed.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
