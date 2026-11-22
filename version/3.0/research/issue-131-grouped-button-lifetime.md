# Issue #131: Grouped selection and template lifetime

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md) · [Templates](../ripscrip/5.2-templates-and-conditionals.md)

## Scope and checkpoint

Started 2026-11-22 on `issue-131-grouped-button-lifetime` from stable 9d666f8 after #130 / PR #133 actual head 0788745 was normal-merged with an identical tree. [Issue #131](https://github.com/bbs-land/remote-imaging-protocol/issues/131) asks for persistent selection/pressed feedback timing, per-instance/per-slot associated definitions and composition, checkbox OFF/old-radio host evaluation, required-radio and suspended mutation lifetime. Adopt #116 opaque creation, #113 pointer commitment, #118 keyboard commitment, #124/#128 redraw/prompt stages and earlier template limits. RIPscrip 1.54 is excluded. No original/client/UI/probe execution or obsolete-client bug investigation.

Initial issue body SHA-256 `86ade2d7d7a0f4a52afd2cb3eb31d3c7b4ca3d75118a7ad233cd63dd3dc31c1f`, updated 2026-11-22T01:54:42Z; unchanged with zero comments at 02:28:52Z. Newly arrived #132 hover handler lifetime is queued afterward.

## Primary sources and bounded original facts

Original RIPtel 3.1 / RIPscrip 3.0.7 DLL in the [mirror](../../../reference/rip-tools.md): 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base 0x10000000; all addresses below are RVAs, text file offset = RVA−0xc00. LLVM objdump reads unchanged bytes only. ALPHA 4 text SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982` establishes radio mutual exclusion, checkbox toggles, creation-order concatenation, single-slot definition overwrite and no mixed radio/checkbox group. Its eager preselected-host prose is superseded by the explicitly adopted #116 no-creation-host rule; correct the inherited reference instead of silently reviving that exception.

- Original immediate radio route `0x2c4c7` selects target at `0x2c4e6 -> 0x2c338`, scans selected radio peers by group at `0x2c505–0x2c51f`, clears peers through `0x2c530 -> 0x2c380` and then processes the clicked action. Persistent selection and redraw are coupled in the native wrappers; do not claim those live-index paths establish safe async ownership.
- `0x2c338/0x2c380` set/clear/toggle selected bit 8 and call the same renderer at `0x2c377/0x2c3be`; pointer group route `0x2c5f9` handles distinct press/release/movement states. Adopted #113 commits only valid release, while #118 commits each eligible key delivery. No original instructions/events were executed to reproduce differences.
- Action `0x2c297` extracts stored host source at `0x2c2bb -> 0x2a383`, sets FIELDID index and calls host stages at `0x2c313 -> 0x4ab3e`; no stable numeric ID-to-storage identity is implied.
- Host entry preprocesses angle text at `0x4ab6e -> 0x4c213`, then segments at `0x4abca -> 0x4ad51`. Block definition dispatch `0x4aec9 -> 0x48985` happens after that preprocessing. Ordinary direct responses execute exactly two passes at `0x4acfa/0x4ad16 -> 0x4b0e4`.
- `0x48985` tests associated checkbox flag 1 at field +0x2a, routes to `0x4888a`, otherwise plain slot setter `0x48838`. The checkbox helper has **one** retained body pointer at field +0x36; it frees/replaces it, retains only if selected at `0x488c0`, scans same-group selected checkbox records ascending at `0x4892f–0x4895d`, concatenates strings and writes the group's slot at `0x48976`. This proves bounded single-body/creation-order behavior, **not** a complete per-instance/per-slot cross-group contract. The new multi-slot ownership/publisher model is explicit completion, not a claimed native record layout.
- Getter `0x489f8` returns global slot +0x100e+4*N; clear helper `0x489cd` frees all slots. Original `0x2c131` counts live radio fields by group without checking selection; missing ordinary-template handling calls it and highlights/notifies. This does not completely specify selected-empty versus undefined versus ordinary-slot overrides. The explicit selection requirement below resolves those cases rather than using pointer non-NULL as a universal selection test.

## Explicit completion and rationale

Commit persistent checkbox/radio state atomically at valid pointer release or each keyboard delivery, before optional redraw and host providers. Press/drag feedback previews only; outside release restores persistent state. Already selected radio commits another action without toggling off. Initial selected creation changes selection only after successful admission, can deselect an earlier radio peer, and never parses/executes host text or creates template associations. Reject mixed kinds at 1U preflight/admission, leaving the style record and old fields unchanged.

Retain a per-stable-instance map N→post-angle ordinary body only when a committed host definition block is reached, with last definition independently winning per N and the existing 4096-byte cap. Creation stores opaque full host source, not parsed association maps. OFF checkbox activations still preprocess/process **their own** source, including direct blocks/assignments/definitions; selected-owner assembly is pure capped byte concatenation, never re-execution of all selected or previously selected sources. Retain OFF owner maps without contributing them.

For each slot retain an ordinary baseline and one current publisher (ordinary or group). A reached grouped definition claims that slot; pure rebuilds after selection/deletion update only slots still published by that group, preserving a later ordinary assignment or another group's claim. Rebuild selected surviving owners in creation order, slot by slot, even for N different from the style group. Empty assembly remains empty while the group lives; dissolution of the last member releases its publications to the ordinary baseline. No implicit publisher stack resurrects an older group. These explicit choices make cross-slot/cross-group/ordinary writes deterministic without providers on deletion.

A committed continuation owns source/FIELDID and redraw token snapshots once. Deletion/reset/replacement cannot transfer it to a duplicate-ID/new field. Completed selection and effects do not roll back for redraw/prompt/host failure. Resume never replays selection. Reached association writes require the original owner token still live; otherwise skip those writes rather than silently converting them into ordinary slot assignments. Direct host blocks of the already committed source continue under their existing stage/error rules. Each pure rebuild uses a fresh atomic live-owner snapshot, never an old pre-prompt aggregate; newly added fields do not acquire the old activation or run their host sources.

Required-radio checks are per referenced slot N while live radio group N exists: require a selected live member, independent of body nonemptiness. A selected field with empty/undefined associated text satisfies selection and may embed empty; an ordinary nonempty slot cannot substitute for a missing radio selection. Last-member deletion removes the requirement. Missing-selection failure is nonfatal with best-effort notification/highlight, not a synthesized activation, automatic retry or connection termination. Input disable/HKEYOFF affect eligibility, not existing state or the requirement. These are explicit logical criteria; no native highlight pixels/timing are required.

## Validation and delivery

The reference now specifies event/feedback timing, owner/publisher rebuilds, required selection and suspended mutation with literal radio/checkbox commands and state/provider vectors. Corrected inherited eager creation and blank-radio wording. Read-only verification passes both source fingerprints and eleven decisive direct-call targets. Independent checks decode both 36-byte style records and all four numeric button prefixes, reproduce eight selection/publication transitions and the 3000+1096 capped assembly. Provider/gesture/required-selection vectors were reviewed against the adopted stage rules; these are protocol vectors, not original execution. The latest report refresh retains the original body and zero comments. Publish/consolidate separately before #132 after repository checks.

Delivered in [PR #134](https://github.com/bbs-land/remote-imaging-protocol/pull/134), specification 9a64f5b, verified open/ready with exactly #131 as closing reference. Formatting/link/diff checks pass (6,166 links). Normal-merge the actual branch head into fork stable before #132; keep upstream review open.

Reproduction uses Python hashlib/struct on the unchanged files and base-36 decoding of the literal styles/prefixes; no original routines execute. Decisive source disassembly commands:

```sh
objdump -d --start-address=0x10048838 --stop-address=0x100489cd ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002c4c7 --stop-address=0x1002c55b ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002c131 --stop-address=0x1002c18a ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

---

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md) · [Templates](../ripscrip/5.2-templates-and-conditionals.md)
