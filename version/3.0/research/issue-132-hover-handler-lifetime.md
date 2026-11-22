# Issue #132: Resident hover handler lifetime

[Research backlog](issue-backlog.md) · [Mouse fields](../ripscrip/4.2-mouse-fields-and-pointer.md) · [Queries](../ripscrip/5.5-file-transfer-and-queries.md)

## Scope and checkpoint

Started 2026-11-22 on `issue-132-hover-handler-lifetime` from fork stable **6a903af** after [PR #134](https://github.com/bbs-land/remote-imaging-protocol/pull/134) actual head **beea018** was normal-merged with an identical tree. [Issue #132](https://github.com/bbs-land/remote-imaging-protocol/issues/132) asks for membership without handlers, movement versus per-callback handler definition snapshots, deletion/reset/focus/disable/restore lifetime and definition edges/bounds. Preserve #51's absence of click dispatch for stored modes 1–4 and #113 stable-instance traversal/FIELDID/physical-click rules. Protocol documentation only; no original/client/UI/probe execution or bug investigation, no RIPscrip 1.54 changes.

Report body SHA-256 `007085ed2d82b38f39a62c54fb743435f00ff348d0e17788449c944aac3a4ef6`, updated 2026-11-22T02:24:13Z, zero comments at 2026-11-22T02:43:05Z. No newer report at this checkpoint.

## Primary sources and recovered facts

Original RIPtel 3.1 / RIPscrip 3.0.7 DLL in the [mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base 0x10000000. Addresses below are RVAs; text file offset = RVA−0xc00. LLVM objdump 19.1.7 reads unchanged bytes only. The [#51](issue-51-deferred-query-order.md) and [#113](issue-113-mouse-gesture-identity.md) findings remain adopted; their old native fixtures are not rerun.

- Wire query `0xd3da` obtains mode/window but checks missing/empty text at `0xd405–0xd417` before definition; it reports nonfatally through `0xd633–0xd640`, leaving the old handler untouched. Mode 5/6 branches `0xd608/0xd611` select mouse-system +0x1c/+0x20 and call `0x13e61` at `0xd61a`; neither uses the window number or reserved values. Parser numeric framing still applies.
- Shared definition `0x13e61` ignores NULL/empty at `0x13e67–0x13e6e`, frees old storage at `0x13e75`, compares the complete case-insensitive C string at `0x13e83 -> 0x655e2` against `$OFF$` (RVA 0x7ab08), otherwise allocates `strlen+1` at `0x13e91/0x13e9c` and copies at `0x13ec5`. No variable/angle evaluation happens during storage. Exact sentinel equality uses bytes before the first NUL, not a leading-substring test.
- Movement `0x5283–0x52e7` updates the point and calls hover at `0x52da` without an equality-to-old-point filter. Original hover exit writes membership 0 at `0x40a1`, then checks valid identity at `0x40b5` and **current** exit pointer at `0x40bb`; entry writes 1 at `0x4130`, then checks identity/current entry pointer at `0x4175/0x417b`. Thus membership changes even with absent handlers. Call sites `0x40cb/0x418f -> 0x13ed0` fetch the shared source separately for each transition; wrapper copies it to host working storage at `0x13ee1` before `0x13ef2 -> 0x4ab3e`. This supports live-per-callback selection, not a movement-wide frozen source. Safe source ownership across deletion/suspension is explicit completion of the live-index implementation.
- Field removal `0x27eee` frees that field's action/associated body and compacts records; whole-field clear `0x27f8f` removes records/clears the 0x5180-byte record array and template storage, but neither frees shared +0x1c/+0x20. Full drawing reset `0x16220` calls field clear at `0x16287` and separately frees both handlers at `0x163e1/0x163f0`. HEADER global HARD/drawing bits call the corresponding full reset; mouse-only deletion flags do not imply that reset.
- Mouse-family backup `0x230a8` writes both shared handler strings at `0x2311c/0x23155 -> 0x21ec2`, before field records. Restore `0x23274` clears fields then frees both handlers at `0x23296/0x232a4`, reads their saved values at `0x23324/0x23363 -> 0x2288f`, then reads fields. Thus an applying **mouse-family backup restore** restores handler definitions too; merely deleting live fields does not. Adopted #113 deliberately discards saved transient membership/press, even though original raw field serialization includes the inside-state byte. No transactional restore guarantee is added.
- Query scratch fallback allocation is **1024 bytes** at `0x153ba`; the definition helper itself allocates the complete supplied string. These bounds do not prove a safe native acceptance/clipping limit for every input. Choose an explicit 1023-byte stored source maximum, rejecting excess before mutation, separately from the enclosing parser and 4096-byte host working-prefix policy. No long-input original execution or defect reproduction.

Original corpus [TELKILL.FN](../assets/icons/TELKILL.FN), 226 bytes, SHA-256 `ccca13d9c9bd6e4f2cd3ff85c4e8af44d1f9f29967fddfd14ef6bc3f49c69136`, issues `!|1K`, then separately `!|1` + real ESC + `5000$OFF$` and `6000$OFF$`. This independently supports field deletion versus handler removal. The exact bytes are unchanged.

## Explicit portable completion

Track membership independently per stable field instance, starting outside, regardless of handler availability. Read the applicable global handler immediately before each surviving transition callback; update membership even if absent. Redefinition alone does not reset membership, synthesize transitions or restart a traversal. At callback commitment own the full raw source, numeric FIELDID and continuation; replacing/removing handlers or deleting/resetting fields cannot retarget or replay it. Snapshot movement point/tokens/hit rectangles/transition candidates, exits then entries newest first; skip removed tokens and defer new fields to a later movement. Serialize movement traversals across suspended callbacks without holding field/table locks; definition/deletion/reset/cancellation notifications may still apply. No replay on resume.

Local focus loss/pointer-delivery disable cancels capture and clears memberships without synthetic exits, retains definitions/handlers and invalidates pending uncommitted traversal callbacks. Reenable/refocus does not synthesize movement: next delivered movement may enter even at the same point. Full reset clears fields/membership/handlers; field-only deletion preserves handlers and surviving membership. Mouse-family restore replaces handlers with saved definitions but gives restored fields fresh tokens/outside state; plain replacement of field definitions without restoring a mouse-family backup retains current handlers. Save alone changes nothing live. HEADER follows the specific operations it actually requests, including full reset, field-only deletion or input disable; pure selection/refresh/cursor changes do not implicitly clear handlers.

## Validation and delivery

The reference now publishes definition/sentinel/bound rules, independent membership/per-callback commitment, the full reset/deletion/focus/restore table and sixteen callback-count/state vectors. Read-only checks pass the DLL/corpus fingerprints, TELKILL deletion/removal ordering, 21 direct-call targets, five decisive instruction sequences and section-mapped sentinel bytes. Independent transition/count checks cover nine scenarios (absent/entry-only/exit-only/both handlers, live removal/replacement, exit-before-entry replacement and duplicate-ID replacement deferral); twelve definition cases verify sentinel/empty/length behavior separately. Suspension, boundaries and backup vectors were reviewed against the adopted stage/token rules. No original routines/providers execute.

Refresh 2026-11-22T02:48:27Z finds unchanged #130/#131/#132 bodies and zero comments. This branch also normal-merges stable 8a33e47, which incorporates corrected #134 actual head 6797f9b with an identical source tree; the existing SOFT template-preservation policy controls over native numbered-slot cleanup. Original source facts and explicit lifetime/storage choices remain separate. Run repository checks, publish/consolidate a separate PR, then finish the full feedback/coverage/ancestry audit.

Reproduce the source checks using Python hashlib/struct on the unchanged files, mapping the OFF sentinel through PE section headers and decoding E8 relative call targets; no client/probe execution. Disassembly:

```sh
objdump -d --start-address=0x10004035 --stop-address=0x100041b8 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10013e61 --stop-address=0x10013efd ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x100230a8 --stop-address=0x100234d2 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
