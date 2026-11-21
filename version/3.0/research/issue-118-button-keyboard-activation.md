# Issue #118: Button keyboard activation

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md#rip_button)

## Scope and checkpoint

Started 2026-11-21 on `issue-118-button-keyboard-activation` from stable 1b8ab99. [Issue #118](https://github.com/bbs-land/remote-imaging-protocol/issues/118) requests byte/case/control/extended matching, zero/default/wildcard precedence, consumption/commit/repeat/feedback, callback and pointer lifetime, coordinate variables, disable/lock and normal-input fallback. #119 image sizing and #120 selective deletion follow separately. Preserve prior style/payload/pointer/host-stage rules and separate FastFont research. Explicit portable completion is allowed.

## Sources and bounded evidence

Original RIPtel 3.1 / RIPscrip driver 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Image base 0x10000000; addresses below are RVAs. LLVM objdump 19.1.7 / Python read original bytes only; `.text` file offset = RVA−0xc00, `.data` = RVA−0x1a00. Immutable [ALPHA 4](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, §3.4.2.2 gives hotkey/control/255/default-Enter and no execution on initial selection. Existing #113/#116 supply adopted field snapshot and deferred host stages. No original client/event/provider/renderer/probe is executed or defect investigated.

Original constructor stores the supplied hotkey low byte at field +0x2b (`0xae71–0xae77`). Matcher `0x556f–0x55f9` uppercases **input** through `0x68b20` at `0x55a5`, scans resident records backwards, accepts wildcard 255 (`0x55c6`), then CR-default bit (`0x55cb–0x55d4`) or exact stored-byte equality (`0x55e4`). It checks current TW flags and header input lock (`0x5597 -> 0x16be8`). Event code calls it at `0x50f5`, then directly invokes keyboard action dispatcher `0x2c424` at `0x5116` on success; a separate tab/default-CR route exists at `0x5009–0x503e`. Hotkey-enable getter `0x31a10` is an independent client setting, used before the ordinary matcher. Earlier action paths `0x2c55b` / `0x2c5c0` provide immediate feedback and call host action without waiting for a later key-up; the old feedback wait is not a portable timing requirement.

These facts support translated-character, newest-first and immediate action concepts, but do not establish symmetric ASCII folding, a complete explicit/default/wildcard priority, zero semantics, extended-event translation, stable mutation affinity or independent disable semantics. In particular, do not call input-only uppercasing proof that both stored A/a bindings match identically. Do not investigate obsolete-client edge paths to invent such claims.

## Explicit portable completion

The reference explicitly adopts a portable keyboard profile: one translated CP437/control byte per eligible event, ASCII folding on both sides, hotkey 0 unbound, 1..254 explicit (128..254 exact bytes), 255 wildcard over byte events including NUL/255 but never a semantic noncharacter key. Lookup precedes terminal encoding of noncharacter keys. Choose explicit match, then CR-only flag-2 default, then wildcard, newest instance within each class. Only resident mouse-active 1U instances participate, not simple fields/decorations or the current style record.

A matching translated key-down/character event commits once immediately; each delivered repeat is a fresh lookup/action, key-up never commits. Consume the event even for empty/failing host actions. Unmatched input follows the existing terminal path unchanged. Local shortcuts/encoding remain client policy. Pointer-only disable leaves keyboard bindings available; HKEYOFF disables keyboard lookup; HEADER input lock suppresses button activation and normal terminal forwarding under its existing input-lock rule. Those independent gate choices are explicit completion, not claims of every old client configuration path.

Before committing a keyboard action, snapshot stable instance/ID/unexpanded action, cancel any outstanding pointer capture (same or other instance) and remove its temporary feedback without changing physical mouse coordinates/buttons. A later release cannot click or transfer that canceled gesture. The keyboard feedback is momentary, with no fixed delay; cleanup touches only a still-surviving instance. Expand the snapshot using the already adopted activation-time pipeline/reset-before-host/FIELDID stack. Deletion, replacement, SOFT or restore during callbacks never dereferences an obsolete field; the running callback retains its numeric ID until return. X/Y/XY/M continue to reflect actual pointer state; no field center or synthetic click is supplied. These are explicitly stronger portable lifetime rules than the source's live indices.

## Reproduction and validation

```sh
objdump -d --start-address=0x1000556f --stop-address=0x100055f9 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10005009 --stop-address=0x10005123 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1000ae64 --stop-address=0x1000ae7a ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

The reference publishes the complete matching/gate/event/lifetime profile and literal style plus seven instance commands. Original DLL fingerprint, four call targets and hotkey store bytes pass verification. Literal decoding confirms the 36-byte style, all seven hotkeys/default bits, positive field bounds and host identity strings. Ten matching/priority/zero/high-byte cases pass independent calculations; repeat, consumption, disable/lock, pointer cancellation and deletion/reset/FIELDID scope were manually reviewed as the explicit policy. Each physical key translation yields one candidate rather than double-counting raw and character notifications. Repository lint/link/diff checks pass (6,039 links before delivery metadata). No original event/provider/renderer execution.

Refresh at 2026-11-21T18:25:48Z confirms unchanged #118–#120 bodies, zero comments and no discussion/inline/review feedback. Publish #118 separately, consolidate its actual head, then #119/#120. Keep co-author trailers and scoped cleanup.

---

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md#rip_button)
