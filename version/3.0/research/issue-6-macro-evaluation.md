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

## Original preprocessing and host-command pipeline probes

The native probe now executes original `0x15602` dollar parsing, `0x25927` built-in lookup, `0x25a7b`/`0x25b21` evaluation, `0x4b6e3` expression parsing, `0x4bd00` comparison, `0x4c244`/`0x4c213` preprocessing, `0x4b0e4` ordinary-code expansion, `0x4ab3e` host command segmentation, `0x4acd9` response execution and the template storage/chaining functions reached from them. The 179 captured registrations are installed as sorted original-format 10-byte records, preserving each original callback and flags. User-variable storage is a controlled name/value fixture at `0xf16b`; template storage/getter and the pipeline itself run unchanged. Playback/transmission/time/input providers are captured/controlled, with no GUI or external transmission.

Current total: **65,815 preliminary checks**. New distinguishing oracles:

- Fixture N=`2`, source `<<IF $N$<"10">>YES<<ELSE>>NO<<ENDIF>>` becomes `YES` through the actual parser and evaluator. The left operand is a variable/template reference; a quoted pair of literal operands is not the established source grammar.
- CMD=`>TEST.FN`, source `$<<CMD>>$` preprocesses to `$>TEST.FN$` and invokes original playback-prefix routing once, with captured filename `TEST.FN` and no output text.
- One `0x4b0e4` pass on `$A$`, with A=`$SHIFT$`, leaves `$SHIFT$` literally. Its original insertion helper `0x65987` returns a pointer **after** inserted text, and the dollar-code scan continues there. This corrects the earlier unresolved “current-position” interpretation.
- Original host response routine `0x4acd9` calls `0x4b0e4` **twice**, first with flags masked by `0xff5f` (deferring control-character and backslash processing), then with full flags. Host entry `0x4ab3e` runs angle preprocessing once before segmentation and these two ordinary passes. With A=`$B$` and B=`$SHIFT$`, host source `$A$` transmits literal `$SHIFT$`; there is no third ordinary pass.
- Original template #3=`$SHIFT$`, source `$?3$` transmits modifier state (`1` with the controlled down-key provider). Template #4=`$?3$`, source `$?4$` instead transmits literal `$SHIFT$`: first-level inserted content gets the second ordinary pass, second-level content remains raw.
- Host source `[3:]<<N>>-$SHIFT$`, with N=`2`, stores **`2-$SHIFT$`** as template #3 and transmits nothing. Angle preprocessing therefore occurs at definition receipt; ordinary dollar code is stored for later use. The inherited “evaluate templates only on use” claim needs this qualification.
- A=`<<B>>`, B=`C`, source `<<A>>` becomes `C`. The named-angle insertion branch `0x4cc45–0x4cc6d` keeps scanning at the insertion position, allowing immediate named-macro re-expansion within a worker pass.
- Angle-template syntax is **`<<?N>>`**. Its insertion branch `0x4cb5f–0x4cb85` advances beyond inserted content; nested angle directives in that content wait for another worker pass. A fixture chain template 0=`<<?1>>` through 8=`<<?9>>`, template 9=`END`, turns source `<<?0>>` into **`<<?8>>`** after one wrapper call (eight changed passes, reported as success). A second wrapper call reaches `END`. The eight-pass cap is not a general bound on immediate named-macro recursion.
- `<<IF $N$=0 AND $SHIFT$=1>>Y<<ELSE>>N<<ENDIF>>`, N=`1`, returns `N` but still calls the modifier provider. AND does **not** short-circuit leaf evaluation; boolean reduction occurs after operands are evaluated.
- N=`0`, A=`X`, source `<<IF $N$=1>><<A>>$SHIFT$<<ELSE>>N<<ENDIF>>` becomes `N`. Both N and A are read, but SHIFT is not. A named angle macro inside inactive branch text is still evaluated before its result is removed; ordinary dollar code in that discarded text never reaches the later pass. Do not describe inactive text as having no preprocessing effects.
- N=`$SHIFT$`, source `<<IF $N$="$SHIFT$">>Y<<ELSE>>N<<ENDIF>>` becomes `Y` with no modifier query. Right-hand quoted text is literal, and comparison does not recursively expand dollar text found in operand values.

Template getter `0x489f8` reads pointer `host_command_state + 0x100e + 4*slot`. Setter `0x48838` stores raw text; `0x48985` handles group/button integration. Host segmentation is `0x4ab3e`, delimiter parser `0x4ad51`, block handling `0x4ae9d`, chain substitution `0x4af4a`. Their exact caller flags, chain order, inactive nested conditions, OR/precedence and malformed-input policy still need final coverage before publication.

Rejected setups: the original insertion helper reverses source strings temporarily in place. A user-fixture callback returning a native C read-only string crashed at `0x75ad2`; copying its result to a mutable buffer, as the real user-variable engine does, fixed the harness. Original template setter returns a 16-bit status in AX while leaving upper EAX bits unspecified; masking to 16 bits fixed an erroneous full-EAX equality assertion. Neither is evidence of a driver defect on valid data.

## Remaining variable traces

HOSTDIR `0x28635–0x28812` is a zero/one-argument getter/setter. Getter uppercases the stored name and returns `<NONE>` when absent. Setter trims and uppercases, treats exact `OFF` (constant `0x7ea44`, still to byte-check) specially, validates <=12 bytes and the same forbidden-character/dot rules as level-2 ESC, then uses original host-directory setter and create-path helper. Unlike the level-2 command it does not have the same explicit callback gate. Verify exact OFF spelling and parser expansion order before publishing.

XFER `0x29365–0x2962d` requires at least three arguments: SEND/RECEIVE, protocol, file type, then filenames where required. Protocol mapping KERMIT=4, XMODEM=0, XMODEMCRC=1, XMODEM1K=2, XMODEM1KG=3, YMODEM=5, YMODEMG=6, ZMODEM=7, ZMODEMCR=8. File types NONE=1, IMAGE=3, a short unresolved keyword at `0x7ecd4`=0, ACTIVE=5. SEND and non-batch RECEIVE require a fourth argument; batch RECEIVE can omit filenames. It joins additional filenames with a space, appends the block-mode terminator from `0x7df18`, and invokes the real `RIP_ENTER_BLOCK_MODE` handler `0x24b4e` with four decoded fields `(mode,protocol,type,0)`. Final client capabilities and parser-token limits remain to test; do not treat a callback request as proof every protocol is supported by RIPtel.

RESET keyword table at `0x82428` is pairs of name/callback pointers. TV maps to **`0x42f50`**, while TW separately maps to `0x42d52`; they are not aliases. TV requires exactly two arguments (keyword and variable name), validates that name (`0x15fdc`), looks it up (`0xe99c`), and deletes either the memory record (`0x63dbe`) or persistent record (`0x14f7b`). Protection can prompt through MessageBoxA; capture the response instead of using a desktop window. Native deletion/protection and persistent-record probes remain required.

OFFSCREEN budget accounting uses `0x339e4(width,height)` = unsigned 16-bit width × height; deletion `0x33074–0x33092` restores that amount to free budget. Thus these are pixel-count units, not bitmap content bytes. Keep any bitmap allocation details outside the language contract unless needed to explain that result.

## Next action

Finish native HOSTDIR/XFER/RESET(TV), remaining XY/TEXTXY and RAND parser boundary cases; settle P from full special-prefix and client registration evidence. Complete host chain/condition/flag coverage from the original routines above, then publish the complete #6 answer and PR. The pipeline fixture hooks deliberately do not establish user-variable storage/prompt behavior; trace those separately where RESET(TV) requires them.

---

[Research backlog](issue-backlog.md) · [Macro reference](../ripscrip/5.2-templates-and-conditionals.md)
