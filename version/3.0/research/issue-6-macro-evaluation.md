# Issue #6: macro evaluation and predefined variables

[Research backlog](issue-backlog.md) · [Macro reference](../ripscrip/5.2-templates-and-conditionals.md)

## Status and scope

Started 2026-11-16 for [issue #6](https://github.com/bbs-land/remote-imaging-protocol/issues/6), branch `issue-6-macro-evaluation`, starting from `9b1872a` (delivered PR #31, dependent on #30/#29). Queue refresh confirms #6 is the oldest RealDeuce issue without a complete resolution PR. Delivered in [PR #32](https://github.com/bbs-land/remote-imaging-protocol/pull/32), dependent on #31; complete primary-source answers published and validation passed. Specification commit: `1ccedf7`. Earlier checkpoints below preserve the investigation history and are superseded by the final findings.

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
- Host source `[3:]<<N>>-$SHIFT$`, with N=`2`, stores **`2-$SHIFT$`** as template #3 and transmits nothing. Angle preprocessing therefore occurs when that definition’s host command is processed (button activation, not universally network receipt); ordinary dollar code is stored for later use. The inherited “evaluate templates only on use” claim needs this qualification.
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

## Completed variable effect and parser checks

The expanded native probe now passes **66,146 preliminary checks**. It additionally executes original HOSTDIR setter/getter and path construction, XFER keyword decoding, RESET keyword dispatch/TV memory-record deletion, remaining XY/TEXTXY domains and the actual dollar parser for variable argument lists. Persistent-store lookup/deletion, directory creation, prompts and the final block-transfer request are captured host services; no filesystem resource mutation, network transfer or desktop dialog is performed.

### HOSTDIR confirmed

Exact clear keyword is **`OFF`**, not `$OFF$`. `$HOSTDIR$` returns the current name uppercased in place, or `<NONE>` when absent. `$HOSTDIR(name)$` trims and uppercases, clears on exact OFF without creation, otherwise enforces <=12 bytes, forbidden characters `\:*?+/",;><=[]|` and exact `.`/`..`, stores the name and attempts install-root + backslash + name creation. A whitespace-only supplied name becomes empty and still attempts creation of `ROOT\`. The original setter and creation-path helpers run unchanged; controlled creation always fails, but selection remains changed. There is no level-2 ESC host-callback gate on this variable path. Successful setters return NULL with no syntax-error flag and are consumed as active expansions. Registered `$HOSTDIR(test)$` is checked through the real dollar parser and callback lookup.

### XFER confirmed

Exact type keywords are **RIP=0, NONE=1, IMAGE=3, ACTIVE=5**; the short unresolved constant is RIP. Protocol names/codes match the prior trace (0..8). Native checks exercise both SEND/RECEIVE across all nine protocols and all four types, observing four decoded block-mode fields `(mode, protocol, type, 0)`. For SEND and RECEIVE protocols 0..4, at least one filename is required; additional filename arguments are joined with **spaces** and `<>` is appended. For RECEIVE protocols 5..8, filenames are optional and even supplied names are ignored: the forwarded tail is just `<>`. The variable produces no replacement text and invokes `RIP_ENTER_BLOCK_MODE` at `0x24b4e`; transport capability/refusal is then the same as that command, not a second independent transfer engine. Do not infer that every keyword implies RIPtel client support beyond its documented transfer capability.

### RESET(TV,name) confirmed

`$RESET(TV)$` alone is a syntax error; it does not clear all variables and is not a text-window reset. Exactly `(TV,name)` is required. Name validation `0x15fdc` requires length below 20, an alphabetic first character and subsequent alphanumeric/underscore bytes; the zero-length edge passes this helper but normal tokenization still needs its own result classification. The callback finds one matching memory record first, otherwise consults the persistent store. Missing records are silent no-ops.

A memory fixture uses the original list layout at macro-state +0x654 (count), +0x65c (head), +0x660 (tail), node data pointer +0xa and record flags +0x7a. The actual lookup `0xe962`/`0xe99c` and list deletion `0x63dbe` run unchanged. Native tests verify count/head clear after deleting an unprotected record. If record flag bit 1 is clear, deletion asks “Requested record is protected, Delete anyway?”; captured response 7 (No) retains it and 6 (Yes) deletes it. The code cancels specifically on 7; other responses are not separately rejected. Persistent-record lookup is a controlled service returning a real-format record; original TV control flow calls persistent-delete `0x14f7b`. Prompt handling is shared between both branches. Syntax failures set +0x436; successful/no-op deletion returns no replacement text.

### XY/TEXTXY confirmed

Actual text-window cell helpers `0xe2e9` and `0xe34d` are now executed. With captured mouse (297,321), 8×16 cells and 80×25 geometry, text-query `$XY$` returns `37:20`; explicit DEVICE returns `0297:0321`, WORLD returns `0594:0642` under ratio 2, overriding the text-query default. They clamp cell locations to each window’s bounds; two-/four-character decimal widths are minimum padding, not a universal 99/9999 cap. Results use signed 16-bit formatting. Invalid domains reject. World/device conversion uses the existing global viewport/world dimensions, with integer truncation.

TEXTXY stores/returns world cursor values and accepts optional port (CUR or decimal 0..35), then optional WORLD/DEVICE. `TEXTXY(CUR,WORLD)` on (297,321) returns `297:321`, and DEVICE returns `148:160` for ratio 2. With an explicit port selector, an absent port or **deactivated viewport** returns `-1`: `0x33f00` establishes entry +0x17 bit 2 as viewport-disabled state. Omitted port bypasses this check, so bare TEXTXY still returns `297:321` under the same disabled bit. Port 36 rejects as syntax rather than returning the sentinel. All domains, return formatting and this omitted-vs-explicit difference are checked.

### RAND parser, conversion and error propagation

Original `0x68c10` decimal conversion and `0x68cc0` wrapper now run unchanged; they wrap modulo 32 bits instead of saturating. Removed the earlier native strtol hooks to avoid imposing the host C library’s overflow policy. Actual parsed forms confirm bare/missing max, max 0/32768, width 6 and minimum equal to max reject; max 1 with random provider 32767 returns **2**; `(9,5,8)` with provider 0 returns `00008`.

Compatibility quirks: maximum token `4294967297` wraps to 1 and is accepted; width `4294967295` wraps to -1 and passes the signed <=5 check, producing no padding; minimum `4294967295` wraps to -1 and passes the signed minimum<maximum check, returning `-1` at random 0. For ordinary range-valid inputs the published grammar remains maximum 1..32767, length 0..5 and minimum 0..maximum-1, but the exact shipped conversion/range predicates must be recorded as quirks rather than claimed to reject all oversized tokens. The original CRT generator uses multiplier **214013** and increment **2531011** modulo 2^32 and yields `(state >> 16) & 32767`; no new portable sequence algorithm should be invented.

The inner built-in evaluator `0x25a7b` returns code 2 for handler rejection, but wrapper **`0x25b21` maps this to 0**, suppressing user-variable fallback for a rejected known built-in. The probe now checks the public wrapper’s 0 result. Earlier failed expectations of code 2 were harness errors, not a different driver branch. Abort code 3 remains distinct and propagates.

### P is not a predefined paste alias

The full DLL initializer has 179 registrations, including PCB and no P. Real dollar parsing/lookup of `$P$` falls through to the user-variable callback, not PCB. A static audit of the unpacked original RIPtel client (same SHA-256 as [issue #5](issue-5-scene-headers.md)) finds its RegisterTextVariable import at VA `0x45b7fc`, thunk RVA `0x1c054`; call sites `0x6f87–0x72fb` register TGREGINFO and TGMENU_* names (including TGMENU_TIPS at name VA `0x43ec24`), and `0x1ae4d`/`0x1aebc` install/remove **FILENAME**. None registers P. These are EXE RVAs relative to **0x400000**, not DLL addresses. Reproduce with `objdump -p`, then `objdump -d` cross-references to thunk VA `0x41c054` and the constant name pointers. Thus the help-table `$P$` paste shorthand is not part of the executable predefined-variable contract; P may be an ordinary user variable. PCB is the actual built-in.

## Final conditional and template findings

Native worker tests prove a chain of **sixteen named angle macros** expands completely in **one** worker call. Therefore the wrapper’s eight-pass limit does not prevent cyclic named macro evaluation within a pass; do not state a universal recursion limit of eight.

AND and OR have **equal precedence and reduce left to right**. For N=`1`, `N=1 OR N=0 AND N=0` is false, while `N=1 OR (N=0 AND N=0)` is true. Original reduction routine `0x4bf60` scans adjacent Boolean/operator/Boolean triples, then NOT/Boolean pairs, then parenthesized single Boolean values, repeating until one result remains. This order also means `NOT N=1 AND N=0` is true, not the result implied by conventional NOT precedence. The probe checks these original results. A quoted left literal (`<<IF "2"<"10">>…`) is rejected; use a variable/template left operand for the distinguishing numeric comparison.

### Shipped template-chain offset defect

The inherited slot-chain notation `[ABC]` is parsed, and `0x4af4a` visits the specified slots in list order, copying each template and attempting to replace its `$?$` markers with the previous result before the final two ordinary passes. **However, the actual substitution call is defective.** At `0x4b06a–0x4b077`, EAX is the placeholder pointer in the template-copy buffer (EBX), but the code subtracts EDI, the _different feed buffer_, to compute insertion position. Correct position would be placeholder minus EBX. `0x65987` interprets that position as signed 16 bits. Consequently marker replacement is allocation-dependent and can truncate or write outside the intended buffer; no portable, deterministic chained output can be asserted for this driver.

The initial native `[34]X` example with templates containing `$?$` failed because of this wrong offset. GDB located the invalid write downstream of the original call; inspection showed that `0x6bcb0` is strncpy, not memcpy, so corrected that supporting CRT hook as well. The invalid-offset fault remains inherent in the caller’s arithmetic. The final probe intercepts only the problematic insertion call, verifies distinct destination=`<>` and input=`X`, limit 4096, and exact erroneous position `(destination+1)-input` instead of 1, then returns NULL to avoid the unsafe write. It verifies original call operands, **not** successful chain rendering. Keep this as a shipped defect; do not publish the earlier hypothetical `[<X>]` output as observed RIPtel behavior.

Host entry flags are also context-dependent: ordinary graphic text at `0x201de` uses expansion flags 0x83 with preprocessing enabled, TEXT_METRIC `0x2044f` uses 0x81, directory switching `0x47015` uses 0x83, host-command callers `0xb2d4`/`0x13ef2` use 0xff (another button-state path `0x2c313` uses 0x77). In `0x25a7b`, registration bit 1 requires evaluator flag 1, registration bit 2 requires flag 4; wrapper flag 2 enables user-variable fallback. The host’s first ordinary pass clears flags 0x20/0x80, the second uses the full caller flags. These are implementation flags, not wire fields; explain their observable context restrictions instead of introducing a user-configurable protocol mask.

## Final parser and mixed-stage cases

The final research checkpoint passes **66,198 native checks**. Empty comma fields are **omitted**, not retained as empty positional arguments: `RAND(9,)` is one argument, `RAND(9,,2)` is two arguments `(9,2)` and returns `00` at random 0, and `RAND(,9)` is one argument `(9)`. Empty parentheses yield no arguments: `RAND()` is rejected like bare RAND, but `HOSTDIR()` is the getter. `HOSTDIR("")` is rejected by the dollar parser; quoted-empty text is not an empty-string argument facility. `RESET(TV,)` has only one argument and rejects, as does filename-less `XFER(SEND,ZMODEM,NONE,)`. Spaces inside unquoted arguments are preserved: `HOSTDIR(MY BBS)` has one `MY BBS` argument; leading/trailing spaces also reach the HOSTDIR normalizer. These examples all use the original parser and registered callbacks.

Nested IF within an inactive parent is syntax-checked without evaluating its comparison provider: an inner SHIFT query is not called. Missing ENDIF rejects preprocessing. A dollar-result string containing new angle text does not restart preprocessing: A=`<<N>>`, host source `$A$` transmits literal `<<N>>` and does not query N. Marker-free template chains work: template 3=`$SHIFT$`, template 4=`FOUR`, source `[34]X` transmits `FOUR` without querying SHIFT; substitution/composition completes before the final ordinary passes, and a template without `$?$` discards the earlier feed. Marker-bearing chaining remains qualified by the proven offset defect above.

All requested issue subquestions now have primary-source answers and independently distinguishing oracles. The normal reference entries must include the integer-conversion/endpoint, pass-order/precedence, omitted-token and template-offset quirks. The inherited template narrative should be clearly qualified so its intended marker-substitution examples are not presented as observed deterministic RIPtel output. Preserve original version/text sources unchanged.

## Publication review

The reference now distinguishes definition host-command processing/activation from network receipt; the `[3:]` native fixture proves the former. Comparison evaluation failure returns Boolean false from `0x4bd00`; do not turn that into a blanket claim that every evaluation error aborts preprocessing. Missing ENDIF is a separately checked structural failure.

Final parser boundary inspection: `0x15afe–0x15b8c` caps nonempty argument count at 40 and each stored argument at 20 bytes; `0x15ed9–0x15ef6` omits empty fields. Added native checks at 20/21 bytes and 40/41 arguments to verify those limits independently of callback arity. XFER consequently has at most 37 filename arguments after its three required keywords. The final probe count is **66,202**; intermediate preliminary counts above are historical checkpoints.

## Publication and validation

Published the complete contract in chapters 5.1/5.2 and the variable index, with related cursor, clipboard and directory cross-references corrected. The inherited template examples are qualified by the shipped offset defect; the ordinary-text/TEXT_METRIC/host consumer rules and all issue subquestions are covered. Original text records and assets remain unchanged.

Validation on 2026-11-16: `run/riptel-macro-probe <unchanged-dll>` captured 179 original registrations and passed **66,202 native checks**. `run/format`, `run/lint`, `run/check-links` (**0 bad of 4,579 links across 188 files**) and `git diff --check` pass. No GUI/Wine was used. The probe controls external services and captures the unsafe chain insertion rather than executing its invalid write.

Delivery: [PR #32](https://github.com/bbs-land/remote-imaging-protocol/pull/32), branch `issue-6-macro-evaluation`, dependent on #31. [Issue-only comparison](https://github.com/RealDeuce/remote-imaging-protocol/compare/9b1872a...issue-6-macro-evaluation); specification commit `1ccedf7`. GitHub refresh confirms #7 is the next oldest RealDeuce issue without a complete resolution PR. Do not merge or manually close #6; continue serially on #7.

## Integration review: exact TEXTXY cursor and fractional metrics

Checked [the 2026-11-17 01:46:52 UTC comment](https://github.com/bbs-land/remote-imaging-protocol/issues/6#issuecomment-6935322623) during integration. Original callback 0x30ede–0x31051 reads current port cursor DWORDs+0x5c/+0x60. It does not consult the last rendered string, its start, justification anchor or glyph raster position. The earlier checkpoint's phrase “stores/returns world cursor values” was imprecise: **WORLD is a raw read of stored values**, whose actual producer units are established by the later [#24 cursor derivation](issue-24-drawing-state-ownership.md#text-cursor-stores). MOVE and outline paths store converted device positions including viewport origin; classic TEXT_XY has its separately documented mixed-unit defect.

Original MOVE 0x1cef0 replaces the cursor without drawing. Nonempty TEXT 0x20169 advances stored X by signed low-word string metric and leaves Y alone, even for centered/right justification or rotated text. Empty TEXT returns before drawing/metrics at 0x20189–0x20193 and leaves the cursor unchanged. Empty TEXT_XY rejects before replacing the cursor; its existing native case distinguishes that error from empty TEXT's silent return. These are observable query semantics, independent of the ownership question in #24.

Serialization is original signed-decimal `%hd:%hd`: colon separator, no padding, no plus sign, no hexadecimal, no fractional portion. WORLD narrows the stored integers to signed 16 bits. DEVICE first applies original world-to-device conversion 0x310b8 to each complete stored coordinate, including origin, with truncation toward zero, then narrows/formats. It does not subtract the viewport origin or invert the producer conversion. New native macro cases verify negative conversion `(-297,-321)`→`-148:-160` under ratio 2 and WORLD wrapping `(65535,32768)`→`-1:-32768`.

Classic width helper 0x1410 does not retain fractional cursor advances:0x15b5–0x15e6 sums raw signed glyph widths into a 16-bit accumulator;0x15e8–0x15fb multiplies by its active integer numerator, divides by denominator using signed IDIV and returns the low word. It truncates the **complete string**, not each character separately. Controlled CHR metric state with raw A/B widths 5 and scale 5/4 gives theoretical 6.25 per glyph: original metric returns 6 for `A`,12 for `AB`, with no fractional carry between separate TEXT calls. Raster glyph placement and TEXTXY cursor storage are separate consumers.

Concrete original-handler fixture: world 1280×960, device 800×600, viewport origin(5,7), classic font 1, the above A/B metrics and otherwise ordinary horizontal text. MOVE(10,20) stores `(trunc(10×800/1280)+5,trunc(20×600/960)+7)` = **(11,19)**. TEXT("AB") advances X by 12, yielding **(23,19)**. Original TEXTXY callback returns **`23:19`** for WORLD/default, **`14:11`** for DEVICE. A later MOVE(30,40), with no new text, changes stored cursor to **(23,32)** and bare TEXTXY returns **`23:32`**. Empty T or empty @ leaves the last stored pair unchanged (with the different error behavior above). Thus `m(10,20);T("AB")` has no font-independent literal answer; under unit world/device mapping and origin 0 it is `10 + integer_string_width(AB):20`. The fixture supplies every input needed for an exact literal result.

Reproduction: existing `run/riptel-font-probe DLL` now executes original 0x1410 as well as MOVE/TEXT stores for that fixture and passes **196,170 checks**. Its glyph drawing/font realization remain controlled; no real Windows rasterization or actual shipped font width is inferred from fixture A/B. The macro probe separately runs the original parser/callback using those measured stored values and actual world/device sizes. It confirms literal result bytes; the two probe phases are not one end-to-end client process. The updated macro probe passes **3,460,252 checks**. Same original DLL/hash, Capstone 5.0.7, FreeBSD 15.1/clang 19.1.7, scoped temporary builds and no GUI.
---

[Research backlog](issue-backlog.md) · [Macro reference](../ripscrip/5.2-templates-and-conditionals.md)
