# Issue #7: framing and identification

[Research backlog](issue-backlog.md) · [Framing reference](../ripscrip/1.1-protocol-and-command-hierarchy.md) · [Version replies](../ripscrip/9.2-versions.md)

## Status and scope

Started 2026-11-16 for [issue #7](https://github.com/bbs-land/remote-imaging-protocol/issues/7), branch `issue-7-framing-identification`, based on `e450e1d` (delivered #6 / PR #32). GitHub refreshed in creation order after that delivery: #7 is the oldest open RealDeuce issue without a complete resolution PR. Investigation incomplete; no resolution PR yet.

Required answers: level/sublevel depth and a third digit; framing versus host-evaluation escaping; ESC priority inside unfinished RIP/ANSI input; disable, enable and resumption; exact RIPtel autosense and RIPVER producers/terminators; and an explicitly allowed complete reply for independent implementations without claiming TeleGrafix identity. Publish a distinguishing byte-stream/reply table. Preserve the inherited nine-digit grammar as historical if the shipped parser differs, and stop presenting SyncTERM as the RIPtel oracle.

## Primary artifacts and starting points

Use the unchanged PCMicro RIPtel 3.1 driver recovered for [issue #3](issue-3-extended-font-style.md#artifact-recovery-2026-11-16): SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base **0x10000000**. DLL addresses in this note are RVAs unless stated otherwise. The original client and separately UPX-unpacked copy retain the identities in [issue #5](issue-5-scene-headers.md); EXE addresses are relative to **0x400000**.

Existing original byte parser: `0x39e90`; reset `0x39d20`; constructor prior-CR initialization `0x39cf1–0x39cfb`. The [header probe](../../../run/lib/riptel-header-probe.c) already captures actual parser dispatch using controlled framing-policy callbacks, but those callbacks cannot establish the very framing policy under investigation. Trace its callers and original policy routines as well as the byte parser. Reuse the shared native PE mapper, never substitute another implementation for source behavior.

Issue #6 establishes the host evaluator’s two ordinary passes, first deferring control/backslash processing, plus independent angle preprocessing; its [note](issue-6-macro-evaluation.md) is a lead for locating retained escape handling. Client reply routing: DLL `0x6947` sends message 0x4c8; EXE `0x74ee` routes to `0x844e`, then `0x63ee`, whose configured output translation can add LF after CR. Establish what the actual version producer supplies before discussing transport translation.

No GUI/Wine is needed. Capture outputs and policy services natively/static-only; every new commit uses `Co-authored-by: Codex <codex@openai.com>`.

## First static findings

The original byte parser, not SyncTERM, accepts **nine level digits** in `1`..`9`: `0x39f08–0x39f10` recognizes those digits and `0x3a654–0x3a66e` appends until count 9, then a tenth digit enters discard state 12. Thus a third digit remains part of the level key, not the command character. The complete shipped dispatch table contains only empty, 1, 2, 3 and 9 keys; deeper keys are syntactically recognized but have no shipped command handlers. Unknown prefixes/opcodes resynchronize at pipe or line ending.

Text escape state `0x3aa63–0x3aae2` strips the backslash only before SOH, STX or pipe; otherwise it retains backslash plus next byte for later processing. This directly corrects the alleged global framing unescape. Continuation state `0x3a42e–0x3a464` skips CR/LF/form-feed and resumes the prior state on the first byte after the physical line ending. Native distinguishing cases remain to execute.

Literal **`RIPSCRIP030001`** is at data RVA `0x803b0` (file offset `0x7e9b0`), including the terminating NUL in storage. It is read by autosense callback **`0x355d9–0x35680`** and RIPVER callback **`0x4b6c1–0x4b6e2`**. The earlier reference’s strings-absence claim is false for this unchanged artifact. Autosense selector 0 sends the literal without CR/LF if emulation is enabled; selector 3 formats `%s%c` with a final CR. Selectors 1/2 call original `ripRIPscripEmulation` `0x31420` to disable/enable. RIPVER zero-argument returns the same literal; extra arguments set its syntax-error flag. An independent compatible reply can use the inherited **generic vendor 0** field; distinguish the explicit reconstruction policy from historical TeleGrafix bytes.

Original outer `RIP_ProcessBuffer` **`0x57a2–0x58e1`** checks emulation for every byte, tries RIP byte parsing first when enabled, then routes unconsumed/fallback text through **`0x4d2d0`**. That text router calls column-flow interception `0x3ce24` (not ANSI), then ANSI parser **`0x37b7c`** when the text-window path permits it. Original ANSI dispatch **`0x37ab9`** reaches autosense callback above. The ANSI path remains reachable while RIP processing is disabled; do not adopt SyncTERM’s one-way-disable limitation.

The ANSI parser recognizes ESC only in idle state (`0x37c05`); pending-sequence states have different handling. An ESC inside a pending RIP string reaches that RIP state rather than globally preempting it. Native tests must establish the exact interrupted-input outcomes and retention/resumption before publication.

## Next action

Trace `0x39e90`, its framing/ANSI caller and version-producing callbacks; capture original byte parsing with distinguishable level/escape/ESC streams. Record all service hooks and exact state layouts, then test the complete reachable state transitions before publishing the answer.

---

[Research backlog](issue-backlog.md) · [Framing reference](../ripscrip/1.1-protocol-and-command-hierarchy.md) · [Version replies](../ripscrip/9.2-versions.md)
