# Issue #113: Mouse gesture commitment and field identities

[Research backlog](issue-backlog.md) · [Mouse fields](../ripscrip/4.2-mouse-fields-and-pointer.md) · [Host identities](../ripscrip/5.0-host-commands.md#mouse-field-identities---idn)

## Scope and checkpoint

Started 2026-11-21 on `issue-113-mouse-gesture-identity` from stable 9a7dd05 after [PR #114](https://github.com/bbs-land/remote-imaging-protocol/pull/114) actual head ff10494 was consolidated with an identical tree (initial merge 8b3c171, final workflow cleanup 9a7dd05). [Issue #113](https://github.com/bbs-land/remote-imaging-protocol/issues/113) asks for shared mouse/button press/release commitment and drag behavior, gesture ownership across overlap/definition/deletion/reset/restore, ID prefix grammar and FIELDID lifetime. Explicit portable policy is allowed where original behavior does not establish one. Preserve #51's separate no-deferred-TW/PORT-click-dispatch answer, existing hit geometry/limit/rendering contracts and 1.54.

## Primary sources and scope boundary

Original RIPtel 3.1 / RIPscrip driver 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Preferred base `0x10000000`; addresses below are RVAs. LLVM objdump 19.1.7 and Python read original bytes only. PE file biases: `.text` minus `0xc00`, `.rdata` minus `0x1000`, `.data` minus `0x1a00`.

The immutable [ALPHA 4 record](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, §§3.4.2.11/12/15 (mouse deletion/definition), §§3.4.2.2/3 (button creation/style) and [RIPtel help extraction](riptel-help-extraction.md) establish click actions, held depression feedback, 128 shared fields, latest-first click scanning and FIELDID's identity purpose. They do not fully define mutation/callback lifetime. Original normal-event paths refine the intended physical interaction; explicit completion below handles missing safe ownership rules. No original code, input event, GUI, native probe, renderer or host callback is executed. Do not investigate obsolete-client mutation failures or use their recovery paths as protocol requirements.

## Normal pointer event and host commitment evidence

The previously traced [#51 event path](issue-51-deferred-query-order.md) is preserved. Mouse system is context +0x1e, field stride 0xa3, count +0x16, temporary FIELDID lookup index +0x18, held-button state +0x0a, captured press index +0x4e, point +0x44/+0x48. Left down `0x52e7–0x537d` scans fields newest first and chooses one hit, records its index at `0x534c`, and calls action dispatcher `0x2c424` with event 1 at `0x5359`. Left up `0x537d–0x5405` checks that a down/captured index exists and sends event 2 to that same index at `0x53cc`; it does not run a new hit scan. Movement `0x5283–0x52e7` sends event 3 to the held field at `0x52bd`, then independently runs hover helper `0x4035` at `0x52da`. Thus a press outside does not acquire a field merely by releasing inside, and a new overlapping definition is not inherently the press owner.

Dispatcher `0x2c424` separates simple fields from mouse buttons and radio-group modes. Simple pointer handler `0x2c87c` shows the normal contract directly: event 1 depresses but does not execute a host command; event 2 removes feedback and hit-tests the captured rectangle (`0x2c8e3`), invoking host action `0x2c297` only if inside (`0x2c8f6`); event 3 tests inside/outside and toggles temporary depression. Ordinary button handler `0x2c79c` likewise hit-tests release at `0x2c7f0` and invokes action at `0x2c823`; radio/group handler `0x2c5f9` has separate presentation/selection state and reaches action only in its release branch `0x2c6af–0x2c6f8` via `0x2c6ca`. The unified portable rule below states final release-position arming for all pointer fields/buttons, without investigating differences in the old grouped handler's cached movement state. Existing persistent radio/checkbox selection semantics and keyboard activation are not newly decoded here.

Host action `0x2c297` writes the field index into temporary FIELDID context immediately before `0x2c313 -> 0x4ab3e`, then resets it to −1 at `0x2c318`. Physical capture and temporary FIELDID context are different fields, with different lifetimes. The momentary press must not transmit/expand the host action; valid release commits once, with the already specified reset/host-command/label rules. None of this introduces the absent TW/PORT deferred-click loop from #51.

## Exact ID prefix grammar

Parser `0x27fd7–0x280e7` is shared: simple-field constructor calls it at `0xd067`, and button constructor calls at `0xac2a` before subsequent label/host splitting, storing its returned WORD at field +0xa0 (`0xad3a`). It recognizes ASCII case-insensitive I/D at the beginning, then literal `=`, at least one ASCII decimal digit, at most five digits (`0x28092–0x28096`), then literal `:` (`0x280ae`). No sign or whitespace fits those tests. Original ctype's digit bit is 4; explicit portable grammar is ASCII, independent of locale or Base Math. Leading zeroes count toward the five-digit limit.

After decimal conversion (`0x280b7 -> 0x68c10`), the original **unsigned upper bound 0x7fff** at `0x280c1–0x280c7` rejects values above 32767. Thus the exact accepted range is **0..32767**, not a wrapping 16-bit range and not a maximum of 99999 merely because five digits are scanned. This bound must be read before inferring stored-WORD behavior. Success deletes exactly 4+digit-count bytes by `0x280cf -> 0x65942`, whose bounded copy shifts the remainder over the prefix. It returns the ID WORD. Failure returns −1 at `0x280dc` without editing the string. There is one parse, no repeated stripping or macro interpolation of the number. Duplicate IDs are metadata, not unique storage keys; zero is valid and distinct from the no-prefix sentinel −1.

The prefix applies at the start of simple `1M` text or the full `1U` text payload **before** the button label/host-command split. A matching prefix only inside a later button host segment is not a definition-time identity. Surrounding command validity, escaped delimiters and host/macro expansion remain separate existing stages. Invalid ID syntax remains ordinary payload, not a definition error merely for looking like ID=.

## FIELDID and hover scope evidence

Registration `0x25df1–0x25dfc` binds FIELDID to callback `0x28399`. With no arguments and temporary index not −1, it reads signed identity WORD field +0xa0 and formats decimal via `%d` (`0x28412–0x28422`). An unidentified field therefore gives **−1 during its click action**. If no current callback index exists, `0x283cf–0x283f6` reports a recognized-variable evaluation error and returns NULL, with variable error flag set. That is unavailable context, not a last-hover/last-click value or an implicit zero/−1 substitution. Existing macro error handling governs the failed replacement; it is not an unknown-variable prompt.

Hover helper `0x4035–0x41b8` is two descending passes: exits, then entries. Each field has independent inside-state byte +0xa2, so overlapping identified fields may each emit hover callbacks, newest first within each pass; the one-winner rule is for click acquisition. Shared exit at `0x40cb` temporarily sets +0x18 to the exiting field (`0x40c1`) then clears it (`0x40d0`); entry does the same at `0x4185/0x418f/0x4194`. No-ID fields skip shared entry/exit query execution (`0x40b5–0x40b9`, `0x4175–0x4179`). This confirms the already established mode 5 entry / mode 6 exit numbering from #51; remove the stale “hypothesis” wording on the mouse page. Merely being under the pointer between callbacks does not keep FIELDID active.

## Explicit portable mutation and callback completion

The old event path stores table indices and the variable callback dereferences a live record. This establishes normal ownership only while definitions remain stable; it does not establish safe affinity through deletion/compaction/restore, a nested callback restoration stack or identity availability after callbacks delete their own field. The report authorizes a portable policy. Do not execute or investigate those old mutation edge paths.

Adopt the following explicit completion, labeled as such in the reference:

- A physical gesture captures a stable **field instance/generation**, its hit rectangle and its unexpanded host definition on press. Numeric ID is not that token. Appending an overlapping field, duplicating its ID or changing a button style cannot retarget it; variables still expand only at valid release. Deleting/replacing the captured instance cancels, without acquiring a replacement while the button remains held. A press that captured nothing stays unowned until release.
- `1K`/SOFT cancel capture and clear hover membership. Enclosed deletion cancels only if it removes the captured instance; deletion of other fields can compact storage without changing ownership. Applying any restored/replaced mouse-field table is a fresh interaction boundary, cancels capture/hover membership and does not restore a saved held gesture. Saving alone is not cancellation. Invalid/no-op restore requests that never apply a table have no such effect; an applying restore cancels even if later work fails. Remove temporary feedback before deletion's specified painting, with no extra scene erase.
- Clear the physical capture before executing a committed host action, so its own reset/deletion or nested playback cannot cause duplicate release actions. This does not undo a command already committed. Cancellation governs host activation and momentary depression, not a newly invented persistent group-selection rollback.
- Hover movement snapshots field tokens, processes exits before entries, newest first in each pass; skip deleted tokens and defer newly created fields to the next movement event. Deletion/reset/restore does not synthesize callbacks into removed fields. Surviving hover membership remains after unrelated deletion. Held capture and hover remain independent.
- FIELDID is a numeric snapshot scoped to synchronous entry/exit or committed click expansion, inherited by synchronous local playback/variable expansion. Nested field callbacks push their own ID and restore the outer snapshot on return. Deletion/reset/restore inside a running callback cancels future input work but does not invalidate that callback's ID; later queued work inherits no context. Backups save definitions, not an active gesture or callback stack. Outside a field callback the recognized-variable evaluation fails normally. This completion is deliberately stronger than the original live-index implementation; no native mutation or nesting equivalence is claimed.

## Reproduction and validation

Read original instructions, for example:

```sh
objdump -d --start-address=0x10027fd7 --stop-address=0x100280e7 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10028399 --stop-address=0x10028432 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10005283 --stop-address=0x10005405 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002c87c --stop-address=0x1002c94b ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10004035 --stop-address=0x100041b8 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

The reference now publishes the shared host-commitment rule, explicit capture/mutation/restore and callback-lifetime completion, source-derived hover passes and exact identity grammar. Cross-links connect mouse fields, buttons, host identities and FIELDID; the old mouse-page “mode numbering hypothesis” is corrected against the already established #51 source result. Earlier #51 deferred-click omission, 1.54 and immutable data remain unchanged.

Validation passes for both source fingerprints, fourteen decisive instruction sequences, fifteen call targets, FIELDID registration and the original ASCII ctype digit flags (only bytes 0x30..0x39 have digit bit 4). Twenty independent prefix vectors cover lower/upper bounds, zero, leading zeroes and the five-digit cap, signs, whitespace, missing colon/digits, wrong-base/high bytes, empty remainder and one-prefix-only stripping. Constructor review confirms `1U` copies its full incoming text before identity parsing, then proceeds to label/host splitting; identity metadata is not a field-instance handle. All nine gesture vectors, the four context/lifetime categories and nested 7→8→7/deletion example were reviewed manually as the declared protocol policy, not native observations. Prose spacing is reviewed separately from literal code. Repository lint, link and diff checks pass (5,991 links before delivery metadata).

The 2026-11-21T16:12:33Z refresh finds unchanged #113 (`60397dc9d6255a15135081bc03ed107b81e3c8122a030b709996a2c95198e867`), zero comments and no newer report. Publish separately, consolidate its actual head and audit all coverage/feedback. No original/client/UI/probe execution; co-author and temporary-cleanup requirements persist.

---

[Research backlog](issue-backlog.md) · [Mouse fields](../ripscrip/4.2-mouse-fields-and-pointer.md) · [Host identities](../ripscrip/5.0-host-commands.md#mouse-field-identities---idn)
