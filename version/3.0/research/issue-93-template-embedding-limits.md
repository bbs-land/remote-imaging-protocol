# Issue #93: template embedding limits and suffix effects

[Research backlog](issue-backlog.md) · [Templates](../ripscrip/5.2-templates-and-conditionals.md)

## Scope and checkpoint

Resolve newly arrived [#93](https://github.com/bbs-land/remote-imaging-protocol/issues/93) on `issue-93-template-embedding-limits` from fork stable **7a06ba6**, after #90/PR #92 and #91/PR #94 are complete and consolidated. It was created at 2026-11-19 19:17:46 UTC and has no comments at initial inspection.

Preserve adopted two ordinary passes, same-pass insertion skipping, deferred outer control/backslash conversion, preprocessing/segmentation order, intended chain correction and eight-pass angle behavior. Specify exact 4096-byte enforcement timing/quantity, discarded versus retained suffix effects, direct response/template-definition bounds and boundary/second-pass growth vectors. The report explicitly accepts portable policy and does not request unsafe native reproduction. Read protocol sources; label choices, do not investigate client bugs or execute original routines/UI/callbacks.

## Primary sources and what they establish

The [preserved ALPHA 4 record](../../2.0/text/RIPScrip-2.0-alpha-4.txt) is **1,202,490 bytes**, SHA256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. Lines **12521–12523** give the numerical 4096-byte limit after embedding and truncation when embedding grows beyond it. Lines **12675–12677** separately give the final chaining bound. Those paragraphs do not fully define raw-versus-decoded timing, mutation per insertion, discarded suffix callbacks, direct responses or stored definition limits.

The unchanged original RIPtel 3.1 driver remains `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md): 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, PE32 base `0x10000000`, RIPscrip driver 3.0.7. Existing [#6 derivation](issue-6-macro-evaluation.md) identifies raw template setter/getter `0x48838`/`0x489f8`, host executor `0x4acd9`, ordinary scanner `0x4b0e4`, and host preprocessing/segmentation before response processing. It establishes exactly two ordinary host passes, first-pass deferral of outer control/backslash decoding, local argument decoding in both passes, and skipping inserted results for the remainder of a pass. Its unsafe chain-offset finding and intended composition correction remain adopted; no new investigation of that defect is needed.

The [#53 control contract](issue-53-host-control-brackets.md) establishes that a trailing caret/backquote is literal, and normal `^m` becomes CR only when the pair reaches enabled ordinary control decoding. The [#84 stage contract](issue-84-dollar-argument-grammar.md) separates outer quoting from argument-local escaping. The [#87 directive policy](issue-87-user-variable-directives.md) supplies the assignment in the suffix example. None of those observations alone proves what an unbounded/unsafe native buffer would do at each requested boundary.

**No original instruction, client/UI, prompt, callback, delay, assignment or unsafe input was executed.** This report permits portable policy; source paragraphs and adopted stage rules provide the constraints, and the new enforcement/lifetime choices are labeled as choices. The vectors below are calculations, not native oracles. Original sources/text/assets are unchanged.

## Chosen complete policy

The [normative byte-limit section](../ripscrip/5.2-templates-and-conditionals.md#response-and-embedding-byte-limit) states:

1. Limit each individual response's **current working bytes** to 4096 at entry to ordinary pass one and immediately after every successful replacement in either pass. The entry stage is after existing preprocessing/bracket/chain composition; earlier effects are not undone. Direct responses without embedding use the same cap.
2. Replacement is exactly the byte prefix of `prefix + result + suffix`. Clip before the scanner proceeds, skip retained inserted bytes as already adopted, and scan only a retained suffix. An entirely discarded callback never runs in either pass. A retained complete callback can still run; overflow is not an unconditional stop on retained work. The callback that produced the growing result has already acted and is not rolled back.
3. Count encoded/current bytes, not decoded-final-only output. Pass two's current string can contain a decoded prefix and raw suffix. Later shrinkage does not restore discarded bytes. Exactly two ordinary passes remain; no angle/segmentation restart or third pass is introduced. Final text is bounded before separate client transport/newline translation, without a session-wide aggregate cap.
4. Store a maximum 4096 bytes per individual raw ordinary template definition when assigned/replaced; cap an effective assembled checkbox body before insertion, without deleting underlying button definitions or changing selection/radio requirements. Stored-definition and direct-response coverage are explicit choices, not falsely attributed to the inherited final embedding paragraph.
5. Slice raw bytes without token/escape repair. Existing incomplete-reference failure handling must not evaluate a partial callback or borrow missing bytes from discarded text. Retain a dangling outer backslash literally as an explicit edge choice, independently of argument-local rejection. Existing trailing caret/backquote literal behavior supplies the control example. Truncation itself is nonfatal.

Rejected interpretations: clipping only the final transmitted text; evaluating all discarded callbacks and merely hiding their output; restoring a hidden suffix after decoding; stopping all retained callbacks on any overflow; treating raw 4096 bytes as a 4096 decoded-character budget; imposing one aggregate cap across independent blocks; truncating graphical text by analogy; altering chain/angle pass order; or treating the optional native unsafe path as required behavior.

## Independent boundary derivation

Let `X[n]` mean n literal ASCII X bytes; byte lengths exclude storage NUL. Each example begins at the named storage/response stage, subject to earlier command limits and ordinary permission checks.

- Direct `X[4095]`/`X[4096]`/`X[4097]` become lengths 4095/4096/4096. Stored definitions with those lengths likewise retain 4095/4096/4096 bytes. Template delimiters are not counted.
- Template 3=`X[4095]` and response `$?3$Y` produce `X[4095]`+Y (4096). Template 3=`X[4096]` discards Y immediately at the pass-one insertion.
- With template 3=`X[4096]`, inserting it into `$?3$$D(1)$$-=FLAG=changed$` fills the complete working string. Both callback strings lie past the retained prefix. Neither D nor FLAG's assignment executes in either pass; the result is `X[4096]` and FLAG is unchanged.
- A retained-suffix control case starts with `$?3$$D(1)$`+`X[4086]` (4+6+4086=4096). Template 3=`ABCDE` replaces four bytes with five, dropping only the last X. D remains and runs in pass one; deleting its six-byte reference leaves `ABCDE`+`X[4085]`, length 4090. The lost X stays lost.
- Slot 3=`$?4$`, slot 4=`X[4096]`, source `$?3$Y`: pass one produces `$?4$Y`, length 5, and skips the inserted reference. Pass two grows/clips it to `X[4096]`, discarding Y; no third pass exists.
- Slot 3=`X[4092]`+`$?4$`, slot 4=`Z`, source `$?3$Y`: pass one retains exactly `X[4092]`+`$?4$` and discards Y. Pass two shrinks to `X[4092]`+Z, length 4093. A final-output-only cap would wrongly preserve Y, producing 4094 bytes.
- Slot 3=`X[4094]`+raw `^m`, source `A$?3$`: raw insertion length is 1+4094+2=4097. Clip before conversion to A+`X[4094]`+caret, length 4096; pass two retains the lone caret (last byte 0x5e). Decoding first would instead fit CR (0x0d).
- Replace raw `^m` with backslash+Z. The capped result ends in the backslash byte 0x5c; the explicit dangling-outer-quote policy retains it. Decoding first would instead end in Z (0x5a). Both spellings are outer text, not dollar arguments.

The arithmetic can be reproduced with ordinary byte concatenation/prefix slicing and the declared existing second-pass conversions; it does not need a callback or client harness. The published table covers 4095/4096/4097, immediate suffix loss, a retained callback, pass-two growth and later shrinkage, and control/quote boundary splitting. The explicit choices answer every subquestion without reopening adopted unsafe-chain or eight-angle-pass findings.

## Verification and next action

Original ALPHA 4 fingerprint and independent byte counts/prefixes, discarded-versus-retained suffixes, two-pass growth/shrinkage and both control boundary endings pass. `run/lint`, `run/check-links` (0 bad of 5,744 links across 235 files) and `git diff --check` pass. Refresh at 2026-11-19 19:29:09 UTC finds no #90/#91/#93 comments or newer discussion/inline feedback. Publish a complete PR closing only #93 and consolidate its actual head into fork stable. Then audit current issue coverage/comments and all PR-head ancestry. Preserve Codex trailers and scoped API payload cleanup; do not execute native code or mutate source artifacts.

---

[Research backlog](issue-backlog.md) · [Templates](../ripscrip/5.2-templates-and-conditionals.md)
