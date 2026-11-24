# Issue #214 — Bounded single-string outline fitting

[GitHub issue #214](https://github.com/bbs-land/remote-imaging-protocol/issues/214) · [RIP_BOUNDED_TEXT](../ripscrip/3.0-text-output-and-fonts.md#rip_bounded_text)

## Scope and checkpoint

RealDeuce filed the issue at 2026-11-24 01:51:23 UTC. No comments were present at intake. Use static source reads of RIPtel 3.1 DLL SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Native RVA `0x1a0da` handles level-zero slot 1. The handler uses the FastFont preparation and outline cache machinery documented for ordinary text, plus a separate temporary size search.

## Source checkpoints and distinctions

Dispatch slot 1 gives four XY values and one radix-two-digit scalar. The handler at `0x1a0da` loads only the first four values. Disabled/empty guards precede the classic-style test at `0x1a146`. Exact raw sentinel comparison against `0x7d2ec` uses the standard case-insensitive comparator. Its successful branch copies retained text from `context+0x32 → +0x4a`. The normal route at `0x1a1e0` uses preprocess=1, flags `0x83` and limit 1,022. The handler has diagnostics for classic fonts, zero box extents, zero metrics and no positive fit, all reaching the name `RIP_BoundedText()` at `0x7d308`.

Calls at `0x1a202/0x1a213` project the corners. `0x2530b` normalizes the box. The dimensions are simple differences, computed before the port's viewport origin is added to both corners at `0x1a2b6–0x1a2ff`. Clip setup is the standard port helper `0x342ad`. The fitted box receives no extra clip call.

## Temporary metrics, placement and publication

`0x40ed0` gets six NULL-text preparation call sites and one final draw call. All seven are the same 26-argument outline engine. The [ordinary adapter analysis](issue-3-extended-font-style.md#consumer-trace-and-corrected-field-interpretation) names the parameters. Here both direction arguments, vertical alignment and extra baseline are zero. Every call passes selected face effects, line-decoration flags, size, shadow direction and distance, and the spacing parameter (negated when flag `0x0400` asks for a font-box reference). Horizontal selector 2 is used on the initial NULL-text preparations; preparations for spacing and the final draw use selector 0. Only the final call at `0x1a9d1` passes the text pointer with anchor at the translated left and bottom. The native drawing context installs current W via `0xe6e9`.

The native starting width at `0x1a415` comes from cached text width helper `0x3fcd0`. Its height is the difference between `0x3fc30` (realized state word `+0xca`) and `0x3fbe0` (the signed value at `+0xba`, scaled with the engine's cached X DPI before truncation). The raw stored shadow distance is added to both measurements. This height need not equal the portable unrotated geometric span. The ratio code at `0x1a4cd–0x1a585` can seed from the larger ratio when both extents need to shrink. It remeasures as needed, decrementing an oversized candidate and advancing until it crosses the fit, then reducing back. The portable minimum-ratio proposal and size bound provide a deterministic start and preserve the surrounding search. At `0x1a819` spacing increases by five and at `0x1a8c1` it decreases by one. The adopted portable search also bounds spacing to the authoring range. The layout dimensions never use a word-wrap pass.

Size and spacing are stack locals, with no write to the style record or to the port cursor. Native renderer preparations can change the engine's real cache during trials. The adopted shared logical snapshot is atomic and publishes only the final preparation; this is the same portable failure scope as existing outline calls. Synthetic ink effects are likewise treated consistently with the [ordinary portable metric boundary](../techspecs/3.2-fastfont-additions.md#portable-synthetic-effects-and-decorations).

The read-only checks verified the five-field descriptor, all seven renderer and six width call targets, both projections, the relevant literals and both spacing-step instructions. Twelve modeled cases checked normalized or reversed bounds, enlargement, reduction, shadow, exact width, spacing overshoot and degenerate projection.

## Verification

Verified the dispatch entry, four loaded fields, original expansion, seven renderer calls, six cached-width calls, height helpers and search steps. Checked model vectors, Markdown, links and whitespace.

## Resume

Trace the geometry, setup, width/height measurements, point-size correction and final draw against the existing outline contracts. Then document the portable fit and state boundary with independent metrics and update the command and corpus summaries.

## Delivery

[PR #225](https://github.com/bbs-land/remote-imaging-protocol/pull/225) is open for upstream review. Specification commit 2cc89b7010179b83914eb19233ec2fe34a1f6386.
