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

## Registration map and first native checks

The actual variable-engine initializer is **`0x25c02–0x26d3b`**; the earlier `0x2670e` location is the RESET registration _inside_ that initializer, not its entry point. Executing the original initializer with controlled allocation and a capture hook at registration function `0x2556d` records **179 built-in registrations**. This is stronger evidence than searching function-name strings.

| Name      | Registration name push RVA | Callback RVA |
| --------- | -------------------------- | ------------ |
| CTRL      | `0x25d4e`                  | `0x2827e`    |
| RAND      | `0x25f76`                  | `0x28e96`    |
| SHIFT     | `0x25fee`                  | `0x29319`    |
| TEXTXY    | `0x26036`                  | `0x30ede`    |
| XY        | `0x26276`                  | `0x34c36`    |
| PCB       | `0x2665b`                  | `0x2d3ab`    |
| XFER      | `0x26b4c`                  | `0x29365`    |
| HOSTDIR   | `0x26c65`                  | `0x28635`    |
| OFFSCREEN | `0x26cd8`                  | `0x2f549`    |

There is **no P registration** in that complete initializer. Do not yet declare P completely unavailable: inspect special-prefix handling and client-added registration before settling alias behavior. All four formerly “unrecovered” processors have live callbacks; they cannot be excluded on corpus silence.

`run/riptel-macro-probe <dll>` currently passes **65,755 preliminary macro/variable checks**, including the full original registration sequence. It executes the original selected variable handlers, numeric predicate, signed comparison branches and case-insensitive fallback, with controlled CRT formatting/conversion, random-number and GetAsyncKeyState providers. The exact hooks are in `run/lib/riptel-macro-probe.c`. This is not yet full macro-pipeline execution. Wrapper verifies the unchanged DLL hash, builds native 32-bit code in a private temporary directory and invokes no GUI services.

### RAND

Original `0x28e96–0x2902f` accepts **one through three** parameters laid out in 21-byte parsed slots: `maximum`, optional minimum output `length`, optional `minimum` (default 0). A bare RAND is rejected. Maximum is decimal 1..32767; length is decimal <=5, default 0; minimum is decimal strictly below maximum. Decimal validation uses `0x65589`, conversion `0x68cc0`. Errors set macro-state +0x436 and return NULL. Final complete parser-token/blank-argument behavior remains to test.

It calls original CRT rand `0x68fc0`, then computes `minimum + ((maximum - minimum + 1) * rand_value) / 32767`, signed integer division. Because CRT rand can return **32767**, the formula can return **maximum+1**, a shipped endpoint defect. The preliminary probe supplies every random value 0..32767 to the original RAND handler with arguments `(9,3,2)` and verifies padded results `002` through rare `010`. Padding is a minimum width, not truncation. Original rand state is global RVA `0x87b20`; the code updates an LCG and returns `(state >> 16) & 0x7fff`. Full seed/lifecycle policy is still to trace if it matters to the contract; do not substitute RIPlib’s unverified generator.

### Modifier and OFFSCREEN queries

SHIFT and CTRL require zero arguments, call GetAsyncKeyState with virtual keys 0x10/0x11 respectively and return `"1"` when AH is nonzero, otherwise `"0"`. They read current modifier state, not inject keys. They call the provider even before rejecting extra arguments; errors set +0x436 and return NULL. Native tests cover up/down and argument rejection with a controlled stdcall provider.

OFFSCREEN (`0x2f549–0x2f641`) accepts zero/one parameter. Omitted/FREE returns port-system +0x22 as unsigned decimal; TOTAL returns +0x1e; USED returns TOTAL-FREE. **CLIP is an additional accepted option**, returning the current physical-target port number (+0xa) as signed decimal. Other options or >1 parameter reject. The native oracle uses total 500000/free 300000/physical 7 and obtains `300000`, `200000`, `500000`, `7`. No bitmap-contents readback exists in this handler. Trace allocation accounting before asserting the stored budget’s units universally.

### XY and TEXTXY

XY (`0x34c36–0x34d2c`) copies captured mouse coordinates from macro-state +0x440/+0x444, narrows to signed words and formats **decimal** `%04hd:%04hd`, not hexadecimal. Default world conversion uses original `0x310f2`; explicit DEVICE skips conversion. Native device coordinates 297/321 produce **`0297:0321`**, and a world/device ratio of 2 produces **`0594:0642`**. The text-query path uses `%02hd:%02hd` after original cell-coordinate helpers; that branch and complete domain/error cases remain to probe.

TEXTXY (`0x30ede–0x31051`) accepts up to two parameters: a drawing-port selector (default current), then WORLD/DEVICE (default WORLD). It reads the selected port’s stored graphics text cursor at +0x5c/+0x60, narrows to signed words and formats `%hd:%hd` (unpadded decimal). DEVICE calls `0x310b8`; WORLD uses stored values. When a supplied port selector fails the definition/type check, the result is **`-1`** (constant at `0x7a100`, bytes `2d 31 00`). Native tests confirm `297:321` without padding for omitted arguments and the `-1` sentinel for explicit CUR with an unavailable port. The no-argument path bypasses the explicit selector’s definition check; do not generalize that check to all forms. An initial note incorrectly assumed this shared constant was empty; reading its bytes and testing the actual branch corrected that before publication. Port selector validation, definition flag and coordinate-conversion domains still need original-handler probes.

### Comparison and preprocessing leads

Comparison routine **`0x4bd00–0x4bf5f`** chooses numeric comparison when **both** operands pass **`0x4bc9c`**: nonempty decimal digits, optionally one leading minus, with at least one digit. Otherwise it uses case-insensitive string comparison. Operator codes 0..5 are `=`, `!=`, `>`, `>=`, `<`, `<=`. Numeric conversion is signed via `0x68c10`. Native comparison-level checks distinguish 2<10 (true), 10<2 (false) and nonnumeric case-insensitive ordering. Full source expression tests must use the actual parser’s operand grammar: `0x4b6e3` reads a variable/template on the left and an optionally quoted literal on the right; a pair of quoted literals is not established as legal source syntax. An assigned variable with value `2` compared to `"10"` is the planned distinguishing end-to-end expression.

Host-response routine `0x4b0e4` optionally calls preprocessor `0x4c213(context,text,4096)` before scanning ordinary dollar codes/templates/control characters. Preprocessor wrapper `0x4c213–0x4c243` repeats worker **`0x4c244–0x4d01f`** at most **eight passes**: worker 0=failure, 1=stable success, 2=changed. Eight changed passes still return success, so this is bounded reprocessing, not unrestricted fixed-point expansion. Worker handles IF/ELSEIF/ELSE/ENDIF, `<<NAME>>` expansion through synthesized `$&NAME$`, and template embedding. Host-response scan reinserts successful dollar-variable results at the current scan position; flags and exact helper return semantics still need tracing to define re-expansion correctly. Do not prematurely generalize one pipeline’s pass order to all command handlers.

### Rejected native probe setups

A TEXTXY harness attempt installed five-byte jump hooks at adjacent one-byte synchronization stubs `0x34488` and `0x34489`; the hooks overlapped and crashed. Leaving both original RET instructions untouched fixed that harness fault. A second expectation incorrectly assumed omitted TEXTXY arguments ran the explicit-selector definition check; the actual branch skips that check, whereas explicit CUR returns the verified `-1` sentinel when unavailable. Neither failure indicates a driver crash for these normal inputs. Do not publish the initial expectation.

## Next action

Extend the native probe through original expression parsing and the complete preprocessor/host-response pipeline, preserving real registered lookup where possible. Trace caller flags and template-definition/use stages; test mixed expansion, side effects, short-circuiting and eight-pass boundaries. Finish TEXTXY/XY text-query coverage, RAND parser boundaries, HOSTDIR/XFER effects, RESET(TV) and P alias/special-path classification. Only then update normative reference entries and file a complete PR.

---

[Research backlog](issue-backlog.md) · [Macro reference](../ripscrip/5.2-templates-and-conditionals.md)
