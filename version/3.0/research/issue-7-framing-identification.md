# Issue #7: framing and identification

[Research backlog](issue-backlog.md) · [Framing reference](../ripscrip/1.1-protocol-and-command-hierarchy.md) · [Version replies](../ripscrip/9.2-versions.md)

## Status and scope

Started 2026-11-16 for [issue #7](https://github.com/bbs-land/remote-imaging-protocol/issues/7), branch `issue-7-framing-identification`, based on `e450e1d` (delivered #6 / PR #32). GitHub refreshed in creation order after that delivery: #7 is the oldest open RealDeuce issue without a complete resolution PR. Primary-source traces and native distinguishing checks complete; publication pending, no resolution PR yet.

Required answers: level/sublevel depth and a third digit; framing versus host-evaluation escaping; ESC priority inside unfinished RIP/ANSI input; disable, enable and resumption; exact RIPtel autosense and RIPVER producers/terminators; and an explicitly allowed complete reply for independent implementations without claiming TeleGrafix identity. Publish a distinguishing byte-stream/reply table. Preserve the inherited nine-digit grammar as historical if the shipped parser differs, and stop presenting SyncTERM as the RIPtel oracle.

## Primary artifacts and starting points

Use the unchanged PCMicro RIPtel 3.1 driver recovered for [issue #3](issue-3-extended-font-style.md#artifact-recovery-2026-11-16): SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base **0x10000000**. DLL addresses in this note are RVAs unless stated otherwise. The original client and separately UPX-unpacked copy retain the identities in [issue #5](issue-5-scene-headers.md); EXE addresses are relative to **0x400000**.

Existing original byte parser: `0x39e90`; reset `0x39d20`; constructor prior-CR initialization `0x39cf1–0x39cfb`. The [header probe](../../../run/lib/riptel-header-probe.c) already captures actual parser dispatch using controlled framing-policy callbacks, but those callbacks cannot establish the very framing policy under investigation. Trace its callers and original policy routines as well as the byte parser. Reuse the shared native PE mapper, never substitute another implementation for source behavior.

Issue #6 establishes the host evaluator’s two ordinary passes, first deferring control/backslash processing, plus independent angle preprocessing; its [note](issue-6-macro-evaluation.md) is a lead for locating retained escape handling. Client reply routing: DLL `0x6947` sends message 0x4c8; EXE `0x74ee` routes to `0x844e`, then `0x63ee`, whose configured output translation can add LF after CR. Establish what the actual version producer supplies before discussing transport translation.

No GUI/Wine is needed. Capture outputs and policy services natively/static-only; every new commit uses `Co-authored-by: Codex <codex@openai.com>`.

## First static findings

The original byte parser, not SyncTERM, accepts **nine level digits** in `1`..`9`: `0x39f08–0x39f10` recognizes those digits and `0x3a654–0x3a66e` appends until count 9, then a tenth digit enters discard state 12. Thus a third digit remains part of the level key, not the command character. The complete shipped dispatch table contains only empty, 1, 2, 3 and 9 keys; deeper keys are syntactically recognized but have no shipped command handlers. Unknown prefixes/opcodes scan to pipe or line ending; the stale-prefix defect below qualifies successful later dispatch.

Text escape state `0x3aa63–0x3aae2` strips the backslash only before SOH, STX or pipe; otherwise it retains backslash plus next byte for later processing. This directly corrects the alleged global framing unescape. Continuation state `0x3a42e–0x3a464` skips CR/LF/form-feed and resumes the prior state on the first byte after the physical line ending. Native distinguishing cases remain to execute.

Literal **`RIPSCRIP030001`** is at data RVA `0x803b0` (file offset `0x7e9b0`), including the terminating NUL in storage. It is read by autosense callback **`0x355d9–0x35680`** and RIPVER callback **`0x4b6c1–0x4b6e2`**. The earlier reference’s strings-absence claim is false for this unchanged artifact. Autosense selector 0 sends the literal without CR/LF if emulation is enabled; selector 3 formats `%s%c` with a final CR. Selectors 1/2 call original `ripRIPscripEmulation` `0x31420` to disable/enable. RIPVER zero-argument returns the same literal; extra arguments set its syntax-error flag. An independent compatible reply can use the inherited **generic vendor 0** field; distinguish the explicit reconstruction policy from historical TeleGrafix bytes.

Original outer `RIP_ProcessBuffer` **`0x57a2–0x58e1`** checks emulation for every byte, tries RIP byte parsing first when enabled, then routes unconsumed/fallback text through **`0x4d2d0`**. That text router calls column-flow interception `0x3ce24` (not ANSI), then ANSI parser **`0x37b7c`** when the text-window path permits it. Original ANSI dispatch **`0x37ab9`** reaches autosense callback above. The ANSI path remains reachable while RIP processing is disabled; do not adopt SyncTERM’s one-way-disable limitation.

The ANSI parser recognizes ESC only in idle state (`0x37c05`); pending-sequence states have different handling. An ESC inside a pending RIP string reaches that RIP state rather than globally preempting it. Native tests must establish the exact interrupted-input outcomes and retention/resumption before publication.

## Native framing and identification checks

Added [`run/riptel-framing-probe`](../../../run/riptel-framing-probe) and its [C implementation](../../../run/lib/riptel-framing-probe.c), following the existing hash-guarded native i386 tool pattern. Reproduce:

```sh
run/riptel-framing-probe /path/to/unchanged/ripscrip.dll
run/riptel-macro-probe /path/to/unchanged/ripscrip.dll
```

The framing probe passes **203 checks**. It executes the original outer `RIP_ProcessBuffer`, byte parser/reset, ANSI parser/dispatch, emulation state getter/setter, autosense and RIPVER callbacks. Controlled hooks: `0x18cf6`, `0x18bd0`, `0x18bf7` (drawing/update services); `0x320f2` (logging); `0x6947` (reply capture); `0x68440` (CRT formatting); and `0x4d2d0` (text router replaced with original ANSI `0x37b7c` followed by a raw-byte capture). The fixture has no column flow or text rendering; this hook deliberately bypasses those unrelated paths and is not proof about ANSI-disabled or column-consumed text. Original parser framing-policy helpers `0x13c49`/`0x13c59` and original `0x31420` are **not** hooked. The client API beginning-of-line callback supplies controlled true/false states at callback-table +0x120; emulation uses the original default stored state without a host override.

State fixture: context +2 parser, +6 API root, +0x12 text-window table, +0x26 variables, +0x36 options, +0x46 ANSI, +0x5e nonzero host-services marker. Parser +0xa1 text buffer, +0xa5 numeric arguments, +0xaa capture dispatch, +0xb2 context, +0x38 radix 36 and +0x39 coordinate width 2. ANSI state +0xc is its pending-sequence state and +0 is its word-argument array. API root +0 points to configuration (enabled byte +0x300), +4 to callbacks. Original native instruction execution remains separate from the external services controlled here.

### Distinguishing streams and stages

- `!|123` leaves prefix count 3 and bytes `123`, not command `3`; depths 1..9 remain prefix state 2, a tenth digit enters discard state 12. `0` is never consumed as a level digit. Only single-digit 1/2/3/9 and level zero exist in the original complete command table.
- **Stale-prefix defect:** after a multilevel unknown key, the parser only NUL-terminates the current prefix at its new length; it leaves a prior nonzero second byte at parser +0x21. The level-zero lookup optimization at `0x39f76–0x39f98` then skips every empty-key record. `!|123L00000000|c01` plus CRLF dispatches neither command, and a subsequent ordinary level-zero line can remain suppressed. A one-digit valid prefix clears byte +0x21: appending `!|1K|c01` plus CRLF dispatches slots **99 then 22**. A full original parser reset also clears it. An initial probe expected successful level-zero recovery at the first pipe; the failed assertion was investigated against these original state bytes and instructions, establishing a shipped defect rather than changing the harness to conceal it.
- `!|T\$SHIFT\$-\[-\(-\!-\|-\\` plus CRLF dispatches exact tail `\$SHIFT\$-\[-\(-\!-|-\\`. Framing strips the quote for a literal pipe but preserves dollar/bracket/parenthesis/bang/backslash quotes for the subsequent consumer. Escaped SOH/STX follow the same framing-only removal branch as pipe.
- `!|Tabc\` CRLF `def` CRLF dispatches `abcdef` once. Bare LF **terminates**, rather than being unconditionally ignored: `!|Tleft` LF `right` CRLF dispatches `left` and sends `right` CRLF through raw text. Form feed also terminates. The old paragraph described SyncTERM’s different LF behavior as universal.
- ESC inside a pending text string is preserved: `!|Tbefore` ESC `[!after` CRLF dispatches that complete text, with no version response. `!|L00` ESC `[!|c01` CRLF discards the corrupted numeric command and dispatches only color. The ESC is not an ANSI restart in either case. After a lone `!`, however, ESC triggers the fallback to raw/ANSI parsing, so `!` ESC `[!` replies normally.
- ANSI itself recognizes ESC in idle state only. ESC ESC `[!` and ESC `[12` ESC `[!` do not reply. The inner ESC is handled as the pending escape/CSI’s final unsupported byte, then `[` and `!` are ordinary text. A fresh subsequent ESC `[!` replies. CAN (0x18) or SUB (0x1a) explicitly cancels pending ANSI; a following fresh query works. SOH can start RIP while ANSI is pending without clearing that ANSI state.
- Disable then ordinary RIP bytes produce raw text, not a command. Queries 0/3 while disabled do not reply. Enable is still decoded by ANSI, updates state before the next buffer byte, and admits a following SOH command in the same buffer. Neither emulation setter nor enable resets the RIP parser: deliberately API-disabling during `!|TBEFORE`, feeding `IGNORED` ESC `[2!AFTER` CRLF, dispatches **BEFOREAFTER**. This fixture distinguishes resumption from resetting and from interpreting disabled bytes as RIP content.

The macro probe now passes **66,209 checks** after adding original-host-stage quote oracles: host source `\$SHIFT\$` transmits literal `$SHIFT$` and never queries the modifier provider. Quoting only the opening dollar (`\$SHIFT$`) is insufficient: its first ordinary host pass evaluates SHIFT and the final output is `1` with the controlled down-state provider. Both original framing-tail retention and the consuming two-pass behavior are therefore checked; no global unescape is substituted.

### RIPtel client callbacks and output consequences

Static audit of the unpacked original EXE: `ripSetCallback` thunk VA **0x41c096**, IAT **0x45b7d0**. Calls at EXE RVA **0x8ffd** install emulation callback 0 (`0x8b04`); at **0x914d**, callback 72 / 0x48 (`0x8dba`) supplies beginning-of-line. Emulation callback accepts 0/1 to update its stored enabled flag VA 0x43c038 and menu state, and returns that flag for queries; it does not reset parser state. The beginning-of-line callback tests whether the count returned by `0x11027` is below 1. That function reads global VA **0x449508**; helpers `0x11005`/`0x11010`/`0x11017` reset/increment/decrement it, and text/scrollback callbacks maintain it. This is client-maintained line state, not a proven universal display-cursor rule. Keep the driver’s callback predicate explicit rather than importing SyncTERM’s ANSI cursor-column rule. The original driver additionally accepts `!` after previous CR/LF/form-feed when ANSI is idle, and SOH/STX directly in idle framing.

The exact engine identification value is **14 ASCII bytes**, hex `52 49 50 53 43 52 49 50 30 33 30 30 30 31`. Autosense default/0 and zero-argument RIPVER use that same literal. Autosense 3 adds `0d`; 1 disables and 2 enables with no reply. The callback uses only the first ANSI parameter; later parameters are ignored, other selector values produce no reply, and ordinary ANSI numeric accumulation wraps to 16 bits (`65536` becomes 0). Native checks distinguish 0;2 from selector 2 and verify the wrap. ANSI private-prefix `?0!` does not reach this autosense callback. A NUL terminates the internal C string but is not transmitted; the original client output path identified in #5 copies through the terminating NUL without sending it. Its configured CR-to-CRLF translation can affect selector 3’s CR, while default/0 have no line ending to translate.

Interpret the six suffix digits using the inherited **major(2), minor(2), vendor(1), vendor-subversion(1)** format, not the mistaken MMmmrr/revision reading. Thus RIPtel’s `030001` is language 03.00, generic vendor 0, subversion 1; it is not its product version 3.1 or DLL build 3.0.7. For new compatible engines, explicitly permit `RIPSCRIP030000` (generic vendor 0, subversion 0) as this reconstruction’s interoperability profile: default/0 reply exactly those 14 bytes without NUL/CR/LF; selector 3 adds one CR; RIPVER returns the phrase without a terminator. This is a documented compatible choice based on the inherited generic-vendor allocation, not a claimed historical TeleGrafix response.

## Next action

Publish these complete #7 answers in framing/identification entries and their related summary/index links, preserving historical help quotes as incomplete documentary text. Add the distinguishing stream/reply table and label the stale-prefix defect and generic-engine reply policy. Then run formatting, lint, links, diff and relevant probes; file a complete resolution PR dependent on #32, with issue-only comparison from `e450e1d`. Do not advance to #8 before delivery.

---

[Research backlog](issue-backlog.md) · [Framing reference](../ripscrip/1.1-protocol-and-command-hierarchy.md) · [Version replies](../ripscrip/9.2-versions.md)
