# Issue #6: macro evaluation and predefined variables

[Research backlog](issue-backlog.md) · [Macro reference](../ripscrip/5.2-templates-and-conditionals.md)

## Status and scope

Started 2026-11-16 for [issue #6](https://github.com/bbs-land/remote-imaging-protocol/issues/6), branch `issue-6-macro-evaluation`, starting from `9b1872a` (delivered PR #31, dependent on #30/#29). Queue refresh confirms #6 is the oldest RealDeuce issue without a complete resolution PR. Investigation incomplete; no resolution PR yet.

Required answers:

- Expansion/evaluation order and recursive/re-expansion rules mixing `<<…>>`, `$…$`, slot templates and conditionals; cover both drawing arguments and host-command execution instead of assuming one universal pass order.
- Distinguish numeric from lexical ordering with `"2" < "10"`, plus the operator/operand rules needed to reproduce that result.
- Exact `$RAND$` arguments/range, `$TEXTXY$` result, `$OFFSCREEN$` budget versus contents, `$RESET(TV)$` state changes and `$P$` alias status.
- SHIFT/CTRL/HOSTDIR/XFER processor grammars, results and side effects, or explicit exclusion based on complete executable evidence.
- `$XY$` decimal versus hexadecimal formatting, with a distinguishing oracle.

Stay on #6 until every requested item has a complete primary-source answer. Do not use SyncTERM or RIPlib behavior as an oracle. Later queue issues with overlapping individual variable questions remain separate unless this investigation directly resolves their requirements.

## Primary evidence and starting points

Use the unchanged RIPtel 3.1 PCMicro MSI driver from the [issue #3 recovery](issue-3-extended-font-style.md#artifact-recovery-2026-11-16), DLL SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base 0x10000000. All addresses below are DLL RVAs unless explicitly identified otherwise. The complete dispatch table and host macro registrations must be inspected, not just embedded function-name strings.

Existing primary traces from [issue #4](issue-4-column-overflow.md): macro evaluator `0x25a7b` invokes a registered handler; wrapper `0x25b21`; host-response expansion `0x4b0e4`; other preprocessing consumers near `0x4bd7d` and `0x4cc18`; global variable registration `0x2670e`; RESET callback `0x43d38`; reset keyword table `0x82428`. Abort state +0x438 produces evaluator code 3 and stops the surrounding expansion before a later playback directive. Preserve that verified behavior while tracing normal evaluation and recursion.

Issue #5 directly traced `0x4b0e4` callers in directory and text-metric commands, but used controlled identity expansion for their isolated tests; those hooks do not establish macro ordering. Level-2 ESC directory switching is now fully specified, but does not prove that a HOSTDIR text processor shares its syntax or handler.

No GUI is needed for this investigation. Any later GUI work must follow AGENTS.md’s private Xvfb and explicit DISPLAY isolation rule. New commits use the requested Codex co-author trailer.

## Next action

Map the actual registered variable names and callbacks, trace the expression evaluator and both preprocessing/host-command pipelines, then build distinguishing original-instruction probes. Record interim results and rejected setups here before changing normative reference claims.

---

[Research backlog](issue-backlog.md) · [Macro reference](../ripscrip/5.2-templates-and-conditionals.md)
