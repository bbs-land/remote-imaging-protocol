# Issue #124: Graphical expansion and prompt lifetime

[Research backlog](issue-backlog.md) · [Templates](../ripscrip/5.2-templates-and-conditionals.md) · [Buttons](../ripscrip/4.3-buttons.md)

## Scope and checkpoint

Started 2026-11-21 on `issue-124-graphical-expansion` from fork stable eb346cb. [Issue #124](https://github.com/bbs-land/remote-imaging-protocol/issues/124) asks for filename, label, ordinary T/@ and TEXT_METRIC stage/provider permissions; bounded expansion and escaping; creation/redraw timing, prompt continuation/cancellation and later failure effects. Existing raw 1U slots, host activation and image geometry are adopted; RIPscrip 1.54 is excluded. Explicit portable completion is allowed; no client/UI/provider/probe execution or obsolete-client defect investigation.

Initial issue body SHA-256 `d7879b7046b193a55936b4b8be4fccee6860ff41167f1c780185e2c535e218d7`, updated 2026-11-21T20:05:33Z, zero comments. Scan started 2026-11-21T22:12:26Z; no newer discussion/inline/review feedback. #125 follows after separate publication and stable consolidation.

## Primary evidence and exact findings

Use unchanged RIPtel 3.1 / RIPscrip 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [artifact mirror](../../../reference/rip-tools.md): 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Image base 0x10000000; addresses below are RVAs; `.text` file offset = RVA−0xc00. LLVM objdump reads instructions only. ALPHA 4 [preserved text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, supplies user-variable modes and separate host features. Adopted #6/#84/#87/#93/#96 and #113/#116/#118/#119 remain controlling; especially #87 cancellation is `NONE` or transparent no-output, not necessarily an abort.

| Consumer / location | Bounded source fact |
| --- | --- |
| Ordinary graphical text `0x201c5–0x201de` | Preprocess=1, flags 0x83, ordinary insertion limit 0x3fe (1022); preceding copy terminates at +0x3ff |
| TEXT_METRIC `0x20441–0x2044f` | Preprocess=1, flags 0x81, limit 1022; exact TEXTDATA bypass remains separate |
| Filename `0x2a75a–0x2a76a` | Preprocess=1, flags 0x83, ordinary limit 255, type-independent nonempty first-slot preparation; filename helper follows at `0x2a779 -> 0x3f71a` |
| Label `0x2b08b–0x2b0a1` | Preprocess=1, flags 0x83, ordinary limit 255; face/effects and successful auto-capture `0x2af8b -> 0x2866`, style flag write at `0x2b063`, precede it |
| Renderer entry/creation | Creation captures field style/font/geometry before `0xb19c -> 0x2a4d3`; renderer installs stored creation WORLD at `0x2a651 -> 0x3e9d4`; later redraw uses the same renderer |

Registration initializer `0x25c02–0x26d3b` has exactly 179 calls to `0x2556d`: 64 class-1, 95 class-2 and 20 class-3 entries. EBP becomes 2 at `0x262c4` and 3 at `0x26b5d`. Evaluator `0x25abb–0x25acd` permits a registration if class bit 2 and caller bit 4 match, **or** class bit 1 and caller bit 1 match. This is not an AND requirement for mixed registrations. Graphical flags enable class 1/3, including argument-dependent setters; WORLD is class 2 at `0x26b3b` and is blocked even for a query, whereas WORLDH/WORLDW are class 1. CUR/COORDSIZE/OVERFLOW/HOSTDIR are class 3 at `0x26c0f/0x26bf8/0x26d0c/0x26c6b`. MKILL is class 2 at `0x265ee`. The reference publishes both complete eligible name sets, rather than deriving permissions from descriptive Value/Active labels.

Wrapper `0x25b88–0x25bab` maps recognized rejection to 0 before user fallback; caller bit 2 alone enables that fallback (`0x25b98 -> 0xf16b`). Ordinary scanner `0x4b2af–0x4b2e4` aborts on result 3, removes/replaces on success 1, but on rejection advances beyond the original reference **without deleting it**. Thus a blocked MKILL is literal and following text continues. The separate template test `0x4b1c3` needs bit 8, playback gate `0x25b31` needs bit 0x40, control decoding `0x4b3b1` needs 0x20; all are absent here. Outer backslash test `0x4b192` needs 0x80, which is present.

Angle named fallback synthesizes `$&%s$` at `0x4cbd6`, calls the same evaluator with flags 3 at `0x4cc08/0x4cc10`; conditional operands use flags 3 at `0x4bd71/0x4bd75`. They share the data-class gate and allow user providers even for TEXT_METRIC. Existing angle-template insertion and conditional branch/provider rules remain adopted. The ordinary routine's preprocessor call hard-codes 4096 (`0x4b156–0x4b15f`) rather than propagating the smaller caller limit. This does not establish safe per-stage graphical storage/output behavior. No native overflow path is investigated.

## Explicit protocol completion

Publish per-consumer limits of 255 bytes for filename/label and 1022 for T/@/metric, enforced at entry and every angle/ordinary replacement before further scanning, including within an angle worker. This replaces the ambiguous graphical preprocessor-size interaction with a safe explicit prefix policy; host 4096 rules stay unchanged. Raw 127/255 button bounds still reject before expansion. Treat each input/value as its prefix before NUL, exclude terminators from limits, never resurrect discarded suffixes and retain one-pass insertion skipping. Recognized blocked/rejected/unavailable ordinary built-ins and disabled template/playback tokens stay literal with no fallback; angle provider failure rejects preprocessing. Success with no text consumes the token; hard provider abort rejects the current rendering operation. Unsupported extension names are reserved before user lookup only if the client recognizes them, not an invented catch-all user namespace ban.

Creation freezes style/font/creation WORLD/destination identity after pure preflight and before filename expansion; completed setter effects still affect live state and later providers, not captured geometry. Filename runs before image selection/load; face/effects and successful auto-stamp precede label expansion. Store original source slots and re-expand on every actual corresponding redraw; an auto-stamped clipboard field follows its adopted clipboard path without loading its former icon again. Repeated labels can prompt again under #87. No host source is evaluated as a side effect of rendering.

Prompts suspend the current continuation without holding renderer locks. Successful input, documented `NONE` cancellation and transparent no-output continue once under #87; hard abort is separate. Tokens/destination generations survive suspension, with no stale restoration or new replacement field. Pending creation is not input-eligible until successful rendering. Later creation failure admits no new field but does not undo completed providers/pixels/clipboard/style effects. Redraw failure retains existing definitions/state and prior action commitment. Pointer releases during a held redraw retain their physical ordering and eligibility; keyboard actions already committed remain once-only. No render prompt itself creates a new FIELDID action scope. These are explicit asynchronous lifetime/failure policies, not claims of native modal parity.

## Reproduction and validation

```sh
objdump -d --start-address=0x10025a7b --stop-address=0x10025bb2 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002af77 --stop-address=0x1002b0a9 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1004b0e4 --stop-address=0x1004b2e6 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

Published the complete four-consumer permission/bound table, all 84 eligible original names, rejection/special-token matrix, per-stage prefix/NUL/escaping rules, creation/redraw/prompt lifecycle and independent literal/capture-failure vectors. Read-only verification passes both hashes, thirteen exact call targets, twenty-one critical instruction sequences, all 179 static registrations (64/95/20 by class), 537 registration/caller gate cases and equality of the two published name sets. Byte arithmetic verifies the 255-byte raw label, ten-byte retained CUR suffix, single backslash, insertion skipping, 255/1022 boundaries and NUL/value-versus-source handling. Manual review covers ordinary versus transparent cancellation, hard abort, held/keyboard action commitment, late font/file/capture effects, TEXTDATA bypass and stale destination/instance protection.

Before validation, corrected draft-only examples from two backslashes to the intended single outer quote, counted `$CUR(BUT)$` as ten bytes, and used a permitted 255-byte BIG user value rather than an impossible 300-byte stored value. The valid 255-byte raw label expands to 504 bytes before the chosen 255-byte clip. No protocol behavior was inferred from those checking errors. Refresh feedback and publish/consolidate separately before #125. No original/client/UI/provider/probe execution or bug investigation.

Repository checks pass: Markdown lint, 6,088 links across 250 files and diff whitespace. Refresh started 2026-11-21T22:20:35Z finds unchanged #124/#125 and zero comments.

---

[Research backlog](issue-backlog.md) · [Templates](../ripscrip/5.2-templates-and-conditionals.md) · [Buttons](../ripscrip/4.3-buttons.md)
