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

## Next action

Trace `0x39e90`, its framing/ANSI caller and version-producing callbacks; capture original byte parsing with distinguishable level/escape/ESC streams. Record all service hooks and exact state layouts, then test the complete reachable state transitions before publishing the answer.

---

[Research backlog](issue-backlog.md) · [Framing reference](../ripscrip/1.1-protocol-and-command-hierarchy.md) · [Version replies](../ripscrip/9.2-versions.md)
