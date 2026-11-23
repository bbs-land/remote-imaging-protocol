# Issue #158: HARD reset state and lifetime profile

[Research backlog](issue-backlog.md) · [HEADER globals](../ripscrip/1.1-protocol-and-command-hierarchy.md#global-header-flags)

## Scope and checkpoint

Started 2026-11-23T01:54Z on `issue-158-hard-reset-profile` from fork stable **266a26951d211a8f9069cefa16a95da7d33b4e41**, following #156 / PR #157 actual-head identical-tree consolidation. [#158](https://github.com/bbs-land/remote-imaging-protocol/issues/158), created/updated **2026-11-23T01:53:11Z**, zero comments, body SHA-256 `5b37ac7c53f333a108836597d1fff4196ca7cd6a2fee9f15226c3045f0d0e3ec`, requests the complete shared HARD inventory and HEADER versus live callback boundary. Last all-author check **01:54:27Z**, next about **02:04:27Z**. Work is protocol-only: bounded read-only source extraction, no original instructions/client/probe/UI execution or obsolete-software bug investigation.

## Starting reference hashes

Reproduce with `git show 266a26951d211a8f9069cefa16a95da7d33b4e41:<path>`:

- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.
- `version/3.0/ripscrip/1.1-protocol-and-command-hierarchy.md`: SHA-256 `36534258e8e1a89f8eb42cabd62373e15f1123f8df6c2475a8834e03ad7796d6`.
- `version/3.0/ripscrip/1.3-world-view-virtual-canvas.md`: SHA-256 `bf5d8ba2568ab0faedbb2719eee99998b19c2a52eea2d16ea1da8a6ac4d40111`.
- `version/3.0/ripscrip/1.4-terminal-ansi-view.md`: SHA-256 `ae5de69f841aeb718f9ae6fc564a890b235c6615fabe3b678bf3438759dba2e7`.
- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `41b0eac2a2513f96454b9ec66ca89f70c1ee6373a6870dd4c5bd97ba3238ddbe`.
- `version/3.0/ripscrip/5.2-templates-and-conditionals.md`: SHA-256 `2f91060d6b51b752e46056cee451471ab6beca0e0e529fecd41165497a78d93e`.
- `version/3.0/ripscrip/7.1-data-tables.md`: SHA-256 `40e91d9a1f8b400d3d0098c7cf0b1bb645b7698b0dfe86957aa68bcb6e7ea27f`.
- `version/3.0/ripscrip/7.2-data-backup-areas.md`: SHA-256 `c5114dc8871f1e017af45219c2bf49373410e6d3cb418a02268caaf93eeca5ee`.
- `version/3.0/research/issue-105-outer-soft-reset.md`: SHA-256 `c3c63d2e4de8d6464a1165656fdc07312ab03c9b9e852c2e843ef0e4ddd6a60a`.
- `version/3.0/research/issue-150-ansi-esc-snapshot.md`: SHA-256 `c96048d56073faa002157e2ec9a278a8987fa36c89e0a9057164bdb62a6b3c6d`.
- `version/3.0/research/issue-156-header-port-zero-reset.md`: SHA-256 `8a7d919f2a0c6fb6dd2e3c338a3383c19123c4cd17e81891507296627d0a6cbd`.
- `version/3.0/research/issue-6-macro-evaluation.md`: SHA-256 `4a9cdf428332f9b0177b2e338c974ba957fb5f2840e7669977371853ee7b1592`.
- `version/3.0/research/issue-87-user-variable-directives.md`: SHA-256 `2703a703d1edcce1e5852bde687a5b62e4bfdff63148162998c8c30cc43f8d02`.

## Documentary and accepted inputs

Immutable ALPHA 4 §2.2.2 (lines 1980–2047) says HARD overrides protection and resets the RIPscrip environment; HEADER prose (6315 onward) explicitly includes saved slots/base areas. RESET(HARD) prose (16678–16680) equates its reset with HEADER's HARD flag. Its Alpha 4 flag numbers are not substituted for the accepted 3.0 wire global bits. These broad statements do not enumerate every per-window field, connection parser, template/variable store or in-flight program lifetime.

Accepted #16 PAL result (all RGB defaults, nonzero undefined/unprotected, current 0), #24 STYLE/ENV defaults, #105 physical fixed-black/ownership boundaries, #100/#101/#104 parser-versus-window ownership, #106/#148 fresh TW recipe, #144 pair absence after full reset and #150 early ESC-snapshot clearing remain adopted. The portable completion will name every state family instead of treating “everything” as permission to erase persistent files or restart a connection/program.

## Original-source ledger and limits

Catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base **0x10000000**. LLVM objdump **19.1.7**, bounded static reads only. Addresses below are **RVAs**, objdump uses base-plus-RVA VAs. No original code executes.

```sh
llvm-objdump19 --disassemble --x86-asm-syntax=intel --start-address=0x10016665 --stop-address=0x10016b24 "$rip158_dll"
llvm-objdump19 --disassemble --x86-asm-syntax=intel --start-address=0x1002fcf6 --stop-address=0x1002fd73 "$rip158_dll"
llvm-objdump19 --disassemble --x86-asm-syntax=intel --start-address=0x10037e41 --stop-address=0x10037e56 "$rip158_dll"
```

Set `rip158_dll` to the original in the catalogued mirror. Existing notes already identify both callers; independently verify their direct-call bytes before publication. The complete outer routine confirms:

- **0x16695:** ENV-system revision marker becomes 0; mouse/button instances cleared **0x166b8**, refresh cleared **0x166c1** and again **0x16977**.
- **0x166c9–0x1671b:** old-current PORT image fallback/all -1 and flags 0, before family cleanup; later PORT 0 also receives image defaults **0x1697f–0x169d1**.
- **0x16720 → 0x37e41:** NULL sound request through the imported sound API, distinct from no audio-stop request in SOFT. Identify the import before asserting its exact external API name.
- **0x16730–0x1692f:** backup-family cleanup calls 0x23f20(family,-2) and 0x24660(family), for all family IDs 0..7. Existing backup ownership/helper identities govern protected saved-state deletion. No new family numbering is inferred from names.
- BUT/STYLE unprotection/defaulting then zero selection; PORT unprotect-all **0x1682e**, delete nonzero **0x16839**, select 0 **0x16843**; TW unprotect **0x16865**, delete nonzero **0x1686f**, select 0 **0x16879**. PAL unprotect/default/select0 **0x16902/0x1690d/0x16917**; ENV unprotect/default/select0 **0x1693b/0x16946/0x16950**.
- Both public callers use second argument 0, enabling user-default font lookup **0x1688c → 0x3170d** and ordinary full-screen TW initialization **0x16896 → 0x8577**, and physical clear **0x168dc → 0x2fcf6**.
- The clear helper requests BLACK_BRUSH **push 4 at 0x2fd27**, FillRect **0x2fd39** on PORT-0 screen DC using physical rectangle system+0x32, then invalidates. This precedes palette default/reset at 0x1690d. It is not STYLE-background or stored PAL-index-0 clearing. Exact inherited GDI clipping/palette realization remains outside the portable physical-black profile.
- Full column/overflow reset **0x1695b → 0x3c1cf(context,1)** (the established #4 global reset, not a font-engine routine), pointer 0 **0x16965**, input-presentation marker **0x1696e**, then ANSI per-window reset **0x169d6 → 0x35320**, classic font/justification **0x169e3/0x169ef**, active ENV parameters **0x16a00/0x16a11/0x16a22/0x16a37**. Caret enable/show **0x16b09/0x16b14**. DC/brush/pen/font selection/allocation is backend bookkeeping, not a protocol resource/file inventory.

The outer code does not reinitialize variable context +0x26 and has no HCL abort/restart request. It does write host-command scratch fields whose private meanings are not guessed. **Template cleanup is indirect:** the reached field-clear helper **0x27f8f** calls **0x489cd at 0x27fc6**; that helper iterates **all 36 slots**, freeing pointers at **context+0x2e → +0x100e+4\*N**. Reading only direct outer stores would incorrectly imply template preservation. HARD therefore clears ordinary/group-published template slots, baselines/associations/publication tags; unlike the separately adopted SOFT preservation exception, it does not retain effective slot bytes. This is reset-owned storage, distinct from the already owned current HCL source/continuation and the session/persistent user-variable stores, which survive. The #6 note called this a host-command pointer array without recording its context offset; this bounded read establishes +0x2e, separate from field context +0x1e and variable context +0x26.

Both callers independently decode to 0x16665 with second argument 0: HEADER **0x179cf** after accepted validation (hard precedes soft), and live HARD **0x42a8e**. HEADER subsequently writes its revision at 0x179f2 and unlocks prior input at 0x179f7, optionally relocking with bit 4. The HARD callback returns at 0x42aa1 with no abort flag or stop-the-rest-of-HCL operation. Its invalid-arity branch sets the ordinary variable error instead; accepted HARD does not enter that branch.

The already identified **PlaySoundA(NULL,NULL,0x20001)** wrapper at 0x37e41 is confirmed by the DLL import table and #5's earlier evidence. Microsoft's primary [PlaySound contract](https://learn.microsoft.com/en-us/previous-versions/dd743680%28v%3Dvs.85%29) confirms NULL stops waveform playback; this does not establish a MIDI-stop requirement. Additional reproducible reads: 0x1799e–0x17a0d, 0x42a7c–0x42aa2, 0x27f8f–0x27fd7, and 0x489cd–0x489f8, using the same image-base-plus-RVA convention.

**Important source boundary:** ordinary TW redefinition and the native ANSI reset are not by themselves a fresh all-fields/per-connection reset, and selecting existing PORT 0 is not proof that every old PORT-0 field is freshly initialized. Do not investigate native stale fields as bugs or publish accidental inheritance as intended HARD requirements. The complete successful logical reset will explicitly adopt fresh TW-0/PORT-0 state, clear all reset-owned queries, and keep connection parser/configuration/stores/program lifetime independent. Those completion choices must be labeled in the reference. Previously adopted PAL and ESC snapshot answers remain unchanged.

## Completed profile and validation checkpoint

The canonical [HARD inventory](../ripscrip/1.3-world-view-virtual-canvas.md#hard-reset-state-and-lifetime) enumerates defaults, forced fresh TW/PORT state, physical fixed-black clear, all eight saved families, queries/refresh/mouse state, full article/overflow state, early ESC snapshot and metric lifetime, all template slots, session/persistent variables, owned current HCL, connection parser/preferences/input lock and waveform audio. It explicitly labels fresh-state/query/clipboard choices and retains existing PAL/#150 answers. HEADER and live HARD share the reset but differ in subsequent revision/input-unlock stages. Related terminal/variable/template/column summaries link to it.

The fresh TW recipe deliberately uses the accepted live-reset defaults (including empty explicit tabs and US/US/G0), rather than inferring that the ordinary initializer/native ANSI reset force every field. The fresh active PORT recipe is broader than the existing partial type-2 initializer, and explicitly clears pointer/query/pair state. Pending ANSI syntax is retained as connection state; it is neither the per-window CSI save nor the separately cleared ESC snapshot. HARD clears all numbered templates, unlike the established SOFT preservation exception. Session/persistent variables and already owned HCL source continue; associated writes/redraws still require their live stable owner. No missing old template can be fetched after clear, although a previously copied body or later new definition can exist independently.

[#4's established layout reset](issue-4-column-overflow.md) identifies 0x3c1cf(context,1) as full RESET(OVERFLOW); HARD calls it at 0x1695b. Argument 0 under SOFT removes columns/disables flow while retaining counters/source flags. Correct the earlier #105 ledger's “font engine” label only; the separate adopted metric invalidation policy and existing column-reference rules are unchanged. No private resource meaning or implementation bug is inferred.

Independent inline Python checks pass **13 baseline hashes**, exact `!|h01000100` decode (revision 1, globals 1, count 0), **720 slot/protection/entry/font combinations**, shared-result equality apart from revision/input lock, default-font-4 40×25/20×24 = 800×600 nominal display on 1024×768 physical canvas, the continued HCL reply `AFTER::MEM:DISK`, later absent-template versus owned-copy/new-definition lifetime, retained variable shadow/empty values, pending-CSI/new-TW state and fixed-black/early-ESC-clear examples. These are abstract protocol/model arithmetic and manually reviewed ownership vectors, not original parser/raster execution.

The fingerprinted inspector independently rereads RVA **0x16665**, length **1215**, SHA-256 `165d7b8ffb64683ff4b23a30aec009caccae9a7309d44ec94a75d97b07335640`; RVA **0x489cd**, length **43**, SHA-256 `c4640e3d361c622b3047c53a9374c541d1de89027647f116acf3fce5cf55177a`; RVA **0x2fcf6**, length **125**, SHA-256 `2930333f7149c0dffd52d48cc5282de7b71fad04635db1179f9804d4024a9950`. Ten relative call targets, both caller zero-argument pushes and BLACK_BRUSH push 4 pass byte assertions. A read-only PE descriptor/thunk walk confirms **IAT RVA 0x96704 = PlaySoundA**. No original instructions, UI or native probes execute; no dump/test/temporary file persists.

Full repository `run/lint`, `run/check-links` and `git diff --check` pass: **0 bad of 6,450 links across 267 files**. Exact header/HCL literals remain outside GFM tables and targeted normal word/number spacing passes. #158 body/zero comments are unchanged at pre-publication refresh. The 2026-11-23T02:05:02Z issue check detected #159 (pick-list parsing/HCL), queued next; next poll about 02:15:02Z. Publish #158 separately, merge actual PR source head into fork stable with identical tree, audit all feedback/coverage/ancestry and continue the ten-minute watch.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
