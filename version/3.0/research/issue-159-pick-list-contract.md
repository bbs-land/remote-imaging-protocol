# Issue #159: Pick-list parsing and HCL pass boundaries

[Research backlog](issue-backlog.md) · [Pick-list reference](../ripscrip/5.4-popup-lists.md)

## Scope and checkpoint

Started 2026-11-23T02:08Z on `issue-159-pick-list-contract` from fork stable **430f052d0e1d93af6a56aff343d379a5709cdcd5**, after #158 / PR #160 actual-head identical-tree consolidation. [#159](https://github.com/bbs-land/remote-imaging-protocol/issues/159) created/updated **2026-11-23T02:02:26Z**, zero comments, body SHA-256 `ff0e85755e3c696a1bab861e0019fe18ac361659bec20aa2cfe33165b89bfb6e`, unchanged at start. Last issue check 02:05:02Z, next about 02:15:02Z. Resolve shipped HCL splice/pass/transmission and bounded list parsing, correcting legacy SyncTERM recursion/automatic-answer claims; preserve #6/#53/#93 pipelines. Protocol-only, no native instructions/dialog/client/probe execution or bug investigation.

Issue #159 was created/updated **2026-11-23T02:02:26Z**, zero comments, body SHA-256 `ff0e85755e3c696a1bab861e0019fe18ac361659bec20aa2cfe33165b89bfb6e`, unchanged at start.

## Starting reference hashes

Reproduce with `git show 430f052d0e1d93af6a56aff343d379a5709cdcd5:<path>`:

- `version/3.0/ripscrip/5.4-popup-lists.md`: SHA-256 `b14a0a8502aadf2e69223d32944ee632680a5d54aa4bcecfac088bb232e7cbfb`.
- `version/3.0/ripscrip/5.2-templates-and-conditionals.md`: SHA-256 `619dafa1d2f53fe802fd342606e64233749b58f2a9044683476fe83318077f7d`.
- `version/3.0/ripscrip/5.3-local-playback.md`: SHA-256 `1db78131b20e068a46bdd8862540710b5133f1f54ab61adebb1ac754c90e4b25`.
- `version/3.0/text/RIPScrip-3.x-technical-whitepaper.txt`: SHA-256 `bf5fd27703bdc4ef311a3dc57a15dd31b0e5ceb37026ac7d015a0fcd3265ab34`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.
- `version/3.0/research/issue-6-macro-evaluation.md`: SHA-256 `4a9cdf428332f9b0177b2e338c974ba957fb5f2840e7669977371853ee7b1592`.
- `version/3.0/research/issue-53-host-control-brackets.md`: SHA-256 `68650c291d76b0fb096b3110e24121494a873ae961e325c54addbfea127cc0cc`.
- `version/3.0/research/issue-93-template-embedding-limits.md`: SHA-256 `acf56d18792a8d626ceabdce606d79e1d506b27ca049a2f5d93319578f586114`.

## Documentary and source identity

ALPHA 4 §4.7 (lines 12068–12160) supplies list/required/question/description/hotkey syntax, 64 entries and historical RIPterm 1.53 total-length 1024. The 3.x white paper at lines 1323–1341 describes transmitting a prefix before selection; the original response executor below contradicts that ordering. The current 3.0 recursion/auto-answer paragraph cites legacy SyncTERM, not RIPtel; it cannot override the identified original scanner. Preserve earlier-version records.

Catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`: **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base **0x10000000**, LLVM objdump **19.1.7** static Intel disassembly. Below all addresses are **RVAs**, not file offsets. Reproduce each bounded range with `llvm-objdump19 --disassemble --x86-asm-syntax=intel --start-address=0x10000000+RVA --stop-address=0x10000000+END "$rip159_dll"`, replacing the arithmetic expressions by their numeric VA sums and the variable by the catalogued artifact path. `run/riptel-inspect` independently fingerprints/maps bytes; it never runs the artifact.

## Static parsing and pass ledger

- Close finder **0x376a–0x379b** skips backslash plus next byte, returns first unescaped `))`; it has **no nesting counter/quote syntax**. Prompt separator finder **0x3d8c–0x3db9** finds first unescaped `::`; entry delimiter finder **0x3dba–0x3df8** finds unescaped comma or @, stops at unescaped `))`, also skips backslash pairs.
- List parser **0x39d1–0x3d8b** recognizes immediate required `*` at 0x3a58, then position helper 0x379c. Default prompt at **RVA 0x79540** is exactly **`Please enter requested value`** followed by NUL, not the inherited `Choose one of the following:`. Explicit separator with zero-length question creates **empty prompt**. Prompt is first 60 raw source bytes then backslash-pair removal (0x3a96–0x3aca).
- Entry loop caps at **64** (0x3ae3). Keep original response pointer/length separately from display. Without @, response is display; with @ the suffix is display. A second unescaped @ in the display is parse failure. Leading/consecutive commas create empty entries; a trailing comma adds no last empty entry because pointer reaches close. Empty response with a nonempty description is valid. Empty entire choices has zero native entries; a portable no-dialog empty-result policy is needed to avoid an unselectable required modal list.
- Display helper **0x3df9–0x3ec3** computes min(raw length,60)+1 allocation but its following copy loop is not bounded by that cap. Do not execute/investigate the overlong path or turn it into required unsafe behavior. Explicit portable completion will bound each raw display copy at 60 bytes before normalization. It maps `_` to `~`, then removes backslash quoting pairs through **0x654b5–0x654e8** (delete slash, skip quoted next byte), giving separate display/hotkey text. The prompt only removes backslash pairs and does not normalize underscores. Display/hotkey processing never changes the selected original response span.
- Response copy **0x3d25–0x3d58** bounds to caller buffer size minus 1. Scanner calls with buffer **1024** at 0x4b32e/0x4b347: selected response maximum **1023 raw bytes**, excluding NUL. Cancellation **0x3d70–0x3d81** returns successful empty string. No separate total-list 1024 check exists in this path; inherited working-response 4096 prefix remains #93's independent boundary. First 64 choices are usable, any additional choices ignored; outer close still determines complete consumed source.
- Scanner **0x4b2fd–0x4b394** recognizes `((` only when mask **0x10** set (0x4b305). Found-close list success removes entire original list and splices selection via **0x4b25b → 0x65987**, returning the scan pointer **after inserted bytes**. Found-close parse failure consumes the full original list with no insertion (0x4b37f–0x4b38f) and continues. Missing close reaches ordinary-byte handling without consumption/dialog; later source bytes still undergo normal scanning, not a blanket literal/abort branch.
- Response executor **0x4acd9–0x4ad50** calls scanner twice, first mask **0xff5f** (0x4ace3) retaining bit 0x10 while deferring controls/backslash; then full flags. Echo/send **0x4ad37/0x4ad41** follows both passes, so this response's prefix is **not transmitted before the modal choice**. No top-level interpreter recursion follows selection. Selected dollar/playback/control/list syntax can run only if a remaining normal pass reaches it unescaped; no third pass, preprocessing or block segmentation restarts. Pass-two selected syntax remains raw.

## Position source limits and intended portable scope

Position helper **0x379c–0x39d0** accepts ASCII digit runs in `x,y:`, `x,:`, `,y:` or `,:`; no sign, whitespace, decimal point, Base Math or UltraNum decoding. An unrecognized prefix resets both outputs to -1 and returns its original start, so it remains question/choice text. Omitted axes start at -1.

Native digit conversion is decimal through 0x68cc0, generally low signed 16 bits; the Y-only branch **0x386a** explicitly sign-extends **AL** instead. Both present axes call **0x310b8(context,&axis,NULL)** at **0x3c0b/0x3ce7**; this is its X-scaling argument, even for Y. Helper 0x310b8–0x310f1 itself implements axis conversion with signed multiply/divide and low-word stores. These are bounded static identities, not permission to investigate overflow/stale fields or require an asymmetric Y-only truncation/scaling accident as protocol intent.

Dialog initialization **0x3483 onward**, position portion **0x352e–0x35bf**, centers -1 axes using parent client size minus dialog size, otherwise offsets relative to parent client origin; it fits the result against the physical parent/client rectangle and clamps below zero. The native overflow adjustment includes far-edge minus-dialog-size-minus-one and OS window/client coordinate bookkeeping, which are not portable pixel styling. The intended canonical profile will explicitly use unsigned decimal 0..65535 per supplied axis, normal corresponding WORLD-to-DEVICE conversion in wide arithmetic, omission/zero-WORLD-axis centering, and fitting to the available client surface (oversize pins to origin). Unrecognized/out-of-range numeric prefix remains body text; labels/question/response bytes stay separate. Keep native numeric/geometry details here and clearly label this bounded interoperable position choice in the reference.

## Next action

Rewrite 3.0 pick-list chapter around close/delimiter/source-span parsing, exact default versus blank prompt, required-versus-option-star, empty/cancel/malformed/limits and two-pass timing. Add explicit safe label/zero-choice/numeric-placement choices, distinguish legacy 1024 total length from actual selected buffer and 4096 HCL boundary, and independent ordinary/pass-one/pass-two/escaped-nested/prefix/position/limit vectors. Verify bytes, source hashes and abstract cases, run full style/link/diff checks, refresh issue, publish separate complete PR and normal-merge actual source head with identical tree, audit feedback/coverage/ancestry and resume ten-minute watch. No original code/dialog execution; co-author trailers, FastFont separation and temporary cleanup persist.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
