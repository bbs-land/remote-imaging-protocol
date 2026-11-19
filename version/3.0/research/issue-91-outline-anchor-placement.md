# Issue #91: ordinary outline anchor and justification

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)

## Scope and checkpoint

Resolve [#91](https://github.com/bbs-land/remote-imaging-protocol/issues/91) on `issue-91-outline-anchor-placement` from fork stable **756b285**, after #90/PR #92 was completed and consolidated. The last comment refresh at 2026-11-19 19:10:05 UTC finds no #90/#91 comments or newer discussion/inline feedback.

Preserve adopted #3 fields/additive displacement, #22 raster/mapping, #24 cursor stores, #82 global bbox labels and #86/PR #88 advance/cache policy. Define zero-displacement glyph origin, all sixteen independent rotations, start/end/center and selector 3, signed spacing totals and midpoint rounding. The issue explicitly permits a portable policy rather than pixel reproduction. Buttons, flowed text, classic fonts and language 1.54 are outside scope. Do not investigate old client bugs or run client/UI/probes.

## Primary-source and adopted evidence boundary

The unchanged driver identity remains `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md): **592,896 bytes**, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, PE32 image base `0x10000000`, language driver 3.0.7 shipped by RIPtel 3.1. The preserved [ALPHA 4 record](../../2.0/text/RIPScrip-2.0-alpha-4.txt) is 1,202,490 bytes, SHA256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. These identities are retained for comparing the earlier evidence; **no new original driver instruction or probe was executed** for #91.

The relevant already recorded primary observations are:

| Source | Established result / limit |
| --- | --- |
| [#3 consumer/geometry note](issue-3-extended-font-style.md#baseline-shadow-and-spacing-findings), renderer RVA `0x40ed0`, placement tree `0x415b5–0x41e79` | Independent string and character rotations; native additive baseline-displacement matrix under three justifications. Published differential offsets do not establish every absolute zero-displacement origin. |
| [#24 cursor note](issue-24-drawing-state-ownership.md#text-cursor-stores), handlers `0x20169`/`0x20cbc` | TEXT draws at stored coordinates; outline TEXT_XY uses converted anchor plus viewport origin. Cursor update is separate and unconditional for accepted nonempty calls. Controlled widths 13/37 are not actual glyph geometry. |
| [#82 bounds note](issue-82-rff-global-bounds.md), [shared field map](../../2.0/techspecs/3.2-fastfont-fonts.md#metrics-block-at-base) | Signed global bbox coordinates are geometric bounds, not width/ascent/descent alignment fields. The meaning of alignment zones at `+0x42/+0x44/+0x46` is still marked probable. |
| [#86 advance/cache policy](issue-86-outline-advance-policy.md) | Ordinary per-byte step includes final gap and string-axis DPI; cached cursor width uses frozen X DPI independently. Character rotation can change realized horizontal bbox spacing reference. No universal raster-edge rounding is promised. |
| [TEXT_METRIC entry](../ripscrip/3.0-text-output-and-fonts.md#rip_text_metric) versus [ordinary y entry](../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style) | TEXT_METRIC has its own vertical justification flags. The ordinary y consumer supplies no separate vertical-justification selector. Do not silently import TEXT_METRIC's top/default alignment into ordinary graphics text. |

The original observations permit an explicit portable placement policy, as the issue requests. They do not prove that a baseline, bbox corner or particular probable typographic alignment line is the exact native zero-displacement anchor for all sixteen pairs. No further obsolete-client reverse engineering or bug investigation is needed to answer this policy question.

## Complete chosen placement contract

The [published placement section](../techspecs/2.4-outline-advance-and-cache.md#ordinary-text-anchor-and-justification) makes each of these choices explicit:

1. Use the **glyph design origin** as the baseline anchor. Local unrotated `y=0` defines that baseline; character rotation is about `(0,0)`. At zero stored displacement, start/selector 3 puts that point at the already computed command anchor for **all sixteen** independent quarter-turn pairs. No global-bbox corner, cap/ascender line or extra probable-zone offset is inserted.
2. No RFF `+0x1e`, `+0x20/+0x22` or `+0x42/+0x44/+0x46` metric moves the zero-displacement origin under this policy. This is a replacement alignment policy, not a claim those native consumers never read those fields or that their tentative format meanings have now been proven. Advance tables and the adopted bbox spacing use remain in force.
3. Let e(S) be the existing device string-direction unit vector and T the wide signed sum of current **ordinary spacing-adjusted steps**, including the last byte's gap. Start subtracts zero; end subtracts `e(S)*T`; center subtracts `e(S)*trunc(T/2)` with truncation toward zero; selector 3 subtracts zero. Do not use cached X width, low-word displacement, ink-union width or absolute T. R affects T only through the previously adopted spacing/realization inputs.
4. Add the adopted device-axis baseline delta once, after zero-displacement placement; do not rotate it again. Each later glyph origin adds e(S) times the signed prefix sum of preceding steps. Raster tile bearings and shadows add to those origins separately. Placement uses wide coordinates; no intermediate WORD wrap is required.
5. Leave accepted-call cursor stores and snapshot lifecycle intact. An ink-free glyph still has its advance, and justification does not replace the stored cursor with an ink corner or rendered endpoint. Buttons, flowed/classic text and RIPscrip 1.54 are excluded.

Rejected alternatives are: silently deriving a baseline from bbox ymin/ymax; assuming probable alignment zones are verified cap/ascender lines; rotating the adopted baseline-offset matrix again; measuring justification using cache width on vertical strings; omitting the final gap; forcing negative totals positive; using floor on a negative half; and treating selector 3 as an error or unspecified new alignment mode.

## Independent asymmetric fixture

This is a new **synthetic metric/geometry fixture**, not a font file, native measurement or mutation of a shipped font. All inputs are stated so another implementor can recompute the table without running RIPtel:

- `U=1000`, `Q=10`, `H=100`, `Dx=144`, `Dy=72`, source text AB with advances 750/400. Engine metrics are exactly 150/80; ordinary device advances are 15/8 on X and 7/4 on Y.
- Design bounds are `(-300,-900)..(1100,800)`, deliberately asymmetric around zero. The fixture realizes X/Y by 1/50 and 1/100, then rotates in device axes. Thus upright bounds are `(-6,-9)..(22,8)`, not centered or baseline-aligned by their top edge.
- The four stated realized rectangles are inputs for this fixture, using repeated `(x,y)->(y,-x)` quarter turns. Their horizontal spans are 28,17,28,17. This transform is an explicit fixture definition, **not** a newly mandated native/hinting/anisotropic raster pipeline.
- Positive profile: P=100 gives T=23 horizontally or 11 vertically. Negative profile: P=10 plus flag `0x0400` gives per-glyph gaps `trunc(-90*28/100)=-25` or `trunc(-90*17/100)=-15`. With two gaps, horizontal totals become -27/-7 and vertical totals -39/-19. These odd totals distinguish signed midpoint rounding and last-gap inclusion.

The published sixteen-row table covers all S/R pairs with four end/center vectors per row and start/selector 3 zero in every row. It is checked independently using direction-specific coordinate cases: S=0 subtracts from X, S=90 adds to Y, S=180 adds to X, S=270 subtracts from Y. For centering, positive halves 23/11 give 11/5; negative halves -27/-7/-39/-19 give -13/-3/-19/-9, rather than floored values one lower.

Anchor `(100,80)` examples separately expose design origin versus tile corner and ordinary Y advance versus cached X width. A quarter-turned glyph's fixture bearing is `(-9,-22)`; upward positive end placement gives origin `(100,91)` and tile `(91,69)`, not a cache-based Y=103. Upward upright negative center gives `(100,61)`, not the floor-based Y=60. Changing unused alignment-zone values cannot move an origin under the explicitly chosen policy.

## Verification and next action

This branch also merges corrected #92 head **63e6e15**, preserving the original #90 source history. Its initial-byte correction uses the original ctype table: alphabetic, not alphanumeric. The merge affects macro documentation/workflow only, independently of the placement policy below.

Independent corner rotations/spans, exact engine/device advances, both signed spacing profiles, all sixteen table rows and explicit tile/midpoint examples pass. `run/lint`, `run/check-links` (0 bad of 5,715 links across 234 files) and `git diff --check` pass. Refresh 2026-11-19 19:17:48 UTC finds no #90/#91 comments, newer discussion/inline feedback or new reviews across 44 open PR histories. Publish one complete PR closing only #91 and consolidate its actual head into fork stable. Keep prior source/text/assets byte-exact; no original execution or temporary raster assets are required. Co-author trailers and scoped API temporary cleanup persist.

---

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)
