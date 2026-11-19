# Issue #96: outer angle scanner and branch state

[Research backlog](issue-backlog.md) · [Templates and conditionals](../ripscrip/5.2-templates-and-conditionals.md)

## Scope and checkpoint

Resolve [#96](https://github.com/bbs-land/remote-imaging-protocol/issues/96) on `issue-96-angle-worker-boundaries` from fork stable **d046bb4**. #97 synthetic outline effects/decorations follows, with a separate complete PR. Refresh 2026-11-19 23:38:20 UTC finds 49 open RealDeuce issues, no comments on the two new reports and no newer discussion/inline feedback since the preceding checkpoint.

Preserve #6 processing order, insertion rescanning, inactive-parent behavior and eight wrapper passes; #90/PR #92 conditional operand bytes and reduction rules; and #93/PR #95 later response clipping. Specify outer delimiter/escape/keyword/name/template recognition, lookahead versus inner literal rules, branch decision/validation/provider order, malformed controls and safe branch-depth limits. Read original sources/instructions only; do not execute original code/UI/callbacks, reproduce unsafe paths or investigate obsolete-client bugs. Distinguish source facts from explicit portable completion choices.

## Original sources and method

Use the unchanged `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` from the [artifact catalogue](../../../reference/rip-tools.md): **592,896 bytes**, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. It is PE32 with image base `0x10000000`, RIPscrip driver 3.0.7 shipped by RIPtel 3.1. LLVM objdump 19.1.7 was used read-only. Addresses below are RVAs, not file offsets. The cited `.text` instructions map to file offset RVA minus `0xc00`; cited `.data` strings map to RVA minus `0x1a00`. `run/riptel-inspect` verifies identity and sections.

The [preserved ALPHA 4 record](../../2.0/text/RIPScrip-2.0-alpha-4.txt) is 1,202,490 bytes, SHA256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. It supplies inherited macro/template context, not the later outer conditional worker. The [#6 evidence](issue-6-macro-evaluation.md), [#90 operand contract](issue-90-conditional-operand-grammar.md) and [#93 clipping policy](issue-93-template-embedding-limits.md) remain adopted. Source-level evidence below fills the requested outer boundaries; it does not infer them from a peer parser or reuse the ordinary argument lexer.

**No original instruction, client, GUI, prompt, provider/callback or unsafe path was executed.** No probe or temporary executable was built. These are bounded instruction/data reads plus explicitly labeled protocol completion choices. Earlier native fixture counts remain evidence only for their original declared boundaries. An initial disassembly range began in the middle of an unrelated instruction; its stray decoding was ignored, not treated as source evidence.

## Read-only scanner evidence

| RVA | Observation / conclusion |
| --- | --- |
| `0x4c2d1–0x4c314` | Only backslash immediately followed by two `<` bytes takes the special escape path. State 0 removes one byte and moves the cursor past the retained pair; inactive state removes three bytes. This is not generic backslash unquoting. |
| `0x65942–0x65964` | Deletion helper copies from `pointer + count` back to pointer, confirming the one-versus-three-byte deletions. |
| `0x4c32f–0x4c354` | Search for `>>`; when its immediately preceding byte is backslash, restart after both bytes. No parity or quote state is tested. |
| `0x4c35d–0x4c383` | Independently search for `<<`, skip immediately backslash-preceded pairs, and reject an eligible nested pair before the selected end. |
| `0x4c389`, `0x4c6cb`, `0x4ca0b`, `0x4ca6f` | Case-insensitive fixed-length comparisons with `IF `, `ELSEIF `, `ELSE>>`, `ENDIF>>`. No leading-whitespace skip precedes classification. |
| `0x4caaa–0x4cb20` | `?` plus alphanumeric slot byte requires closing `>>` at the next fixed positions. Decode one case-folded MegaNum slot, not an arbitrary template name. |
| `0x4cb8a–0x4cc26` | Fallback copies the complete body, formats `$&%s$`, parses it through the ordinary dollar parser and evaluates it; parsing/provider failure is a preprocessing failure. No activity check gates this lookup. |
| `0x4cb77–0x4cb85`, `0x4cc45–0x4cc6d` | Template insertion advances past inserted text; named insertion returns to scan at the insertion point. Existing deferred/immediate distinction remains. |
| `0x4cc6f–0x4cc8f` | Non-directive bytes are retained by incrementing the cursor in active text, deleted one at a time otherwise. |

Original string RVAs: `0x83e8c` = `IF `, `0x83d20` = `ELSEIF `, `0x83c6c` = `ELSE>>`, `0x83c14` = `ENDIF>>`, `0x83edc` = `>>`, `0x83eb8` = `<<`. Each exact ASCII space/closing pair matters. These strings are checked as bytes, not inferred from a documentation synopsis.

For named whitespace examples, the normal dollar parser is decisive: name state `0x158d5–0x15940` accepts alphanumeric/subsequent underscore and its documented delimiters, not a trailing space/HT. Prefix state 1 likewise does not ignore a leading space. Therefore `<<ELSE >>`, `<<ENDIF >>`, `<< IF ...>>` and an IF word followed by HT instead of space route to fallback syntax failure. Conversely `<<IF>>`, `<<IFX>>`, `<<ELSEIF>>` and longer identifier forms are ordinary named/provider references, subject to their normal definition/permission rules. Empty and dollar-wrapped angle bodies are not a second optional-dollar spelling.

## Branch evidence and protocol completion

| RVA | Observation / limit |
| --- | --- |
| `0x4c471–0x4c4b6` | IF first validates expression/group structure, then checks parent state before its value-evaluation path. Inactive parent pushes excluded state 2. |
| `0x4c7bd–0x4c80a` | ELSEIF syntax-check precedes the current-state test; only state 1 (eligible false branch) evaluates values. |
| `0x4c9ca–0x4c9ea` | A true eligible ELSEIF switches to active state 0; encountering a later ELSEIF from active state locks it out as state 2. |
| `0x4ca1f–0x4ca60` | ELSE rejects depth zero and a repeated ELSE marker; activates only an eligible false branch, otherwise excludes it. |
| `0x4ca83–0x4ca9b`, `0x4cc9b–0x4ccbf` | ENDIF rejects depth zero and pops; nonzero open depth at worker end is failure. |
| `0x4c3a1`, `0x4c6e3` | Native depth guards compare with 16, including an ELSEIF guard. The portable contract counts open IF frames, rather than requiring identical guard placement. |

The published [branch table](../ripscrip/5.2-templates-and-conditionals.md#branch-frames-validation-and-provider-order) uses parent activity, selected-history and ELSE-seen flags instead of depending on fixed stack slots. A fresh frame initializes its own history; ENDIF restores its parent. It preserves the evidenced normal IF/ELSEIF selection gate and previously adopted non-short-circuit expression evaluation.

The following are **explicit portable completion choices**, not claims that every native malformed/boundary path already enforces them:

1. Validate IF/ELSEIF without operand-value providers, including template operands; only eligible branches evaluate values. The native operand parser can retrieve a template while tokenizing, so source state gates alone do not prove a universal callback-free native validation phase.
2. Require the inner conditional parse to end at the same delimiter selected by outer lookahead; reject a boundary mismatch before value evaluation. Quotes/backslash parity remain local to their respective stages.
3. Allow at most 16 simultaneously open IF frames, active or inactive. ELSEIF/ELSE reuse a frame and are allowed at depth 16. Reject a seventeenth push before providers/storage access, rather than reproducing native fixed-array behavior.
4. Reject ELSEIF after ELSE and repeated/stray controls. Preserve earlier selection history so skipped ELSEIFs cannot reopen a selected chain. No malformed-branch recovery behavior or stale frame memory is made normative.
5. Bound raw fallback body to 255 bytes and require one complete synthesized provider reference before lookup, using safe storage for results under the existing preprocessor bound. This addresses the fixed native copy buffer without investigating unsafe writes or inventing truncation.
6. Standalone named/template lookups still occur in inactive text, independently of expression-provider gating. Named insertion is immediately scanned under the current state. An inactive deferred template result is discarded as explicit output policy; its lookup/selection effects are not rolled back. This avoids implying that deferred rescanning permits unselected output to survive.

These choices leave #92 operand bytes/reduction and #95 later clipping unchanged. Failure is nonfatal preprocessing rejection, without a promise to roll back providers already completed in an earlier header or stage.

## Stage and provider-order derivation

- A protected introducer skips recognition only in its current worker pass. One actual backslash before `<<N>>` leaves `<<N>>` after that pass; two leave one. A later wrapper pass can perform the N lookup. A backslash before an ordinary X is not removed at this stage.
- Lookahead neither enters double quotes nor consumes protection bytes. Thus quoted inner `<<` fails the outer gate unless preceded by a backslash; that prefix is then consumed by the operand parser's local escape. A protected `>>` needs protection at both gates. Two backslashes before it satisfy outer lookahead but decode to one backslash at the operand stage, exposing an unescaped terminator while the quote is still open; rejection remains the adopted operand result.
- Exact IF/ELSEIF plus ASCII space is required at the outer stage; additional conditional whitespace is allowed only after that matched space. ELSE/ENDIF require their closing pair immediately. Invalid near-keywords fall back to ordinary reference parsing; they are not silently normalized controls.
- In the published A/B/C chain, provider orders are respectively A; A,B; A,B,C; A,B,C for the four selected branches. Later headers are syntax-checked even after selection. A malformed skipped header fails before its values are queried.
- In inactive-parent nested conditions, only the outer eligible expression is evaluated; nested IF/ELSEIF providers are skipped. A standalone named D substitution in the inactive body still retrieves D, after which its ordinary returned text is discarded. A standalone template lookup likewise occurs but its inactive output is discarded under the explicit portable rule.
- Depth 16 with a deepest eligible ELSEIF is valid; a seventeenth IF is rejected before its operand providers. Every new frame clears selected/ELSE history; sibling frames cannot inherit it.

All examples in the reference explicitly name the stage. They are source-derived normal scanning or declared policy vectors, not observed native outputs. The exact backslash spellings were checked as byte counts, avoiding host-language quoting ambiguity.

## Reproduction and delivery

Bounded disassembly:

```sh
objdump -d --start-address=0x1004c244 --stop-address=0x1004c450 ripscrip.dll
objdump -d --start-address=0x1004c650 --stop-address=0x1004c811 ripscrip.dll
objdump -d --start-address=0x1004c9bf --stop-address=0x1004cc9b ripscrip.dll
objdump -d --start-address=0x10065942 --stop-address=0x10065965 ripscrip.dll
```

Read-only exact constants:

```python
from pathlib import Path
from hashlib import sha256
b = Path("ripscrip.dll").read_bytes()
assert len(b) == 592896
assert sha256(b).hexdigest() == "83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8"
for rva, expected in ((0x83e8c, b"IF "), (0x83d20, b"ELSEIF "),
                      (0x83c6c, b"ELSE>>"), (0x83c14, b"ENDIF>>"),
                      (0x83edc, b">>"), (0x83eb8, b"<<")):
    offset = rva - 0x1a00
    assert b[offset:offset + len(expected) + 1] == expected + bytes([0])
```

Original DLL fingerprint/exact constants, all six staged outer-gate spellings and first-selected branch/provider-count cases are checked. Every frame/depth/structure rule is reviewed against the declared portable policy, without original code execution. `run/lint`, `run/check-links` (0 bad of 5,772 links across 236 files) and `git diff --check` pass. Refresh at 2026-11-19 23:49:37 UTC finds no #96/#97 comments or newer discussion/inline feedback. Delivered in [PR #98](https://github.com/bbs-land/remote-imaging-protocol/pull/98), specification **9591d0d**, branch `issue-96-angle-worker-boundaries`. GitHub confirms open/ready/mergeable and exactly #96 as closing reference. Consolidate its actual head into fork stable. Then start #97. Keep co-author trailers and scoped API temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Templates and conditionals](../ripscrip/5.2-templates-and-conditionals.md)
