# Issue #97: synthetic outline effects and decoration boundary

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)

## Scope and checkpoint

Resolve [#97](https://github.com/bbs-land/remote-imaging-protocol/issues/97) on `issue-97-outline-effects-boundary` from fork stable **1407b59**, after #96/PR #98 was completed and consolidated. Last comment refresh 2026-11-19 23:49:37 UTC finds no #96/#97 comments or newer discussion/inline feedback.

Preserve adopted effect flags/q accumulation, glyph bytes/fill/hollow stamp, logical metrics, anchor/additive displacement, cursor/cache and W contracts from #3/#22/#86/#91/#77. Explicitly distinguish mandatory effect/state/composition behavior from renderer-defined solid bold/italic/decorative ink geometry; address coordinate freedom, metric bbox B, local-versus-string decoration paths, blank/missing/negative-span handling, shadow membership, W grouping, pen/pattern independence and ordering. The report expressly permits renderer freedom without pixel reconstruction. No client/UI/native routines/device probes or bug investigation.

## Primary and adopted sources

The [preserved ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) is 1,202,490 bytes, SHA256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. Its font discussion around lines 4846/4865/4957 identifies bold/italic/strikeout/underline intent but does not establish every requested coordinate transform, thickness, baseline or decoration group. Its numeric underline/strike table is not substituted for the already corrected 3.0 bit mapping.

The original RIPtel 3.1 DLL identity remains `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` from the [artifact catalogue](../../../reference/rip-tools.md): 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base `0x10000000`, RIPscrip driver 3.0.7. This issue uses already recorded primary/adopted evidence; **no new original instruction, renderer, font/client/UI, prompt or device probe was executed**. No font bytes, native fixture or temporary raster file was created or modified.

| Existing source | Established boundary |
| --- | --- |
| [#3 fields/shadow derivation](issue-3-extended-font-style.md), adapter `0x42870`, renderer `0x40ed0` | Bold/italic face flags, underline/strike flags, independent rotations, additive baseline offsets and glyph shadow displacement/color/order. Original glyph branch `0x41ee8–0x42016` and line-decoration branch `0x40880–0x40a71` share shadow signs. That observation is not a complete portable band/grouping schedule. |
| [#22 FastFont derivation](issue-22-fastfont-rendering.md) | Raw unsigned code mapping, nonzero interior fill and the specifically recovered hollow filter/stamp, including its BOLD coefficient/clamps. Exact ink-edge pixels and universal solid-bold/shear geometry are outside that proof. |
| [#82 bbox derivation](issue-82-rff-global-bounds.md) | Signed global geometric bbox and realized horizontal spacing span; no universal cap/ascender/decorative baseline is recovered from it. |
| [#86 advance policy](issue-86-outline-advance-policy.md), [#91 placement policy](issue-91-outline-anchor-placement.md) | Synthetic effects are ink-only; actual supplied metric-face differences remain allowed. Logical per-byte steps, negative totals, origins, last gap, justification and cursor/cache are already defined independently. |
| [#74–#76 drawing reconciliation](issues-74-76-drawing-contract.md), [W algebra](../../2.0/techspecs/2.3-drawing-composition.md#operations-and-operands) | Graphics text must honor W; original fixed glyph-copy observations are historical, and overlapping glyph coverage grouping remains renderer-defined. This does not prove a mandatory decoration-specific grouping or forced vector-border COPY. |

The report expressly permits an explicit renderer-freedom statement without pixel reconstruction. The resulting [portable boundary](../techspecs/3.2-fastfont-additions.md#portable-synthetic-effects-and-decorations) identifies what remains free and makes the necessary metric/color/attribute invariants explicit. It does not hide an unspecified ink question behind a claim of native parity.

## Complete disposition of the questions

1. **Solid bold/italic geometry remains renderer-defined.** Strength/shear/pivot, design- versus device-space basis and transform order relative to per-axis DPI/R are not given canonical numbers. Honor the effects, requested R and existing design origins. The recovered hollow BOLD table is a separate adopted exception, not a solid-bold width or universal shear. A profile can choose different decoration thickness/shape under those effects.
2. **Explicit metric-B independence.** Hold the selected metric face/size/width/frame/rotations/spacing constant: synthetic bold/italic/hollow/shadow/decoration ink does not change the metric bbox B, `r[c]`, steps, T, justification, cached width or cursor. Compute B before synthetic ink, tracking actual ink bounds separately. The actual supplied different-face metric exception remains. This closes the ambiguity whereby “advance unchanged” could otherwise still permit effect-expanded B to change flag-0x0400 spacing. It is portable policy, not proof that every original synthetic metric producer ignores those inputs.
3. **Decoration geometry/coverage remains renderer-defined.** No required numerical line offset/thickness or RFF alignment field. A profile can follow the local glyph baseline R or string progression S and choose unspaced, adjusted-last-gap, signed-path or visual extents. Spaces/blank/missing glyphs, final-gap coverage and negative extents are explicit choices; none changes the logical metrics/origins. Current realized geometry governs the choice, not an unrelated cold/stale cursor snapshot. This does not promise that a band always equals T or Woutline.
4. **Explicit paint/attribute independence.** Foreground bands use Draw Color and the normal clip/W path. Vector LINE_STYLE type/width/off-draw, N border, fill pattern and Fill Color do not become font decoration attributes merely because a backend draws a band with a vector API. This is a portable policy grounded in the text attribute scope, not a claim that every native internal pen is identical.
5. **Shadow/grouping/order boundary.** Glyph shadow copies the effected glyph coverage once at the adopted device offset, in Back Color, before its own foreground. Decorations may join that shadow or remain unshadowed; a chosen decoration shadow uses the same offset/color and precedes its own foreground. Their glyph/text group membership and all additional cross-glyph/decoration/shadow order are renderer-defined under W. No fixed-COPY bypass, new whole-text-union rule or separate-W-per-band requirement is introduced.

This explicitly leaves the requested ink freedoms open while answering whether they are portable obligations. Rejected shortcuts: inventing solid-bold strength from hollow data; freezing a shear angle without a source/policy; deriving underline position from probable RFF zones; substituting R for S in logical progression; measuring B from affected ink; forcing all band spans to cached X width; importing vector border/pattern behavior into text; forcing one overlap/XOR result despite the adopted grouping boundary; or silently ignoring effect flags.

## Independent allowed-profile calculations

These are **synthetic permitted profiles**, not original font or native observations. Reuse the complete [#91 asymmetric fixture](../techspecs/2.4-outline-advance-and-cache.md#independent-placement-vectors): Q=10, U=1000, Dx=144, Dy=72, advances 750/400, S=0°, R=90°, metric B=17. Ordinary engine metrics are 150/80; X advances 15/8 give T=23 at P=100. At P=10 plus bbox spacing, each extra gap is -15, yielding steps 0/-7 and T=-7. Styles do not change those values.

Use a simple rectangle with design corners `(0,-200)`, `(200,-200)`, `(200,0)`, `(0,0)`. Upright X/Y scales are 1/50 and 1/100, so the unstyled rectangle is `(0,-2)..(4,0)` in device units.

- A design-space half-Y shear `x := x - y/2`, then DPI scaling, gives pre-rotation X extent 0..6. Quarter-turn `C90(x,y)=(y,-x)` yields ink bounds `(-2,-6)..(0,0)`.
- Scaling first and applying the same numerical half-Y shear in device space gives X extent 0..5, then ink bounds `(-2,-5)..(0,0)`. Both obey the freedom boundary despite unequal DPI. Their horizontal ink span 2 is not metric B=17.
- An illustrative one-device-unit X dilation can grow that ink span to 4 without changing B; it is not a new normative BOLD coefficient.
- A local-baseline decoration profile can use unspaced Y-DPI spans `trunc(150*72/1440)=7` and `80*72/1440=4` along R. A continuous-string profile instead uses horizontal T=23 or interval `[-7,0]` for negative T. Both can use a chosen one-device-unit band at local/string baseline-normal offset +1/-1; those offsets and coverage on blank/missing glyphs are illustrative, not required.
- At S=90°, ordinary positive T=11 differs from cached X total 23; a decoration's chosen current geometry must not be dictated by a stale cursor snapshot. For a logical XOR pixel, destination 0x5 and foreground 0x3 yield 0x6 for a grouped glyph+band or 0x5 after two separate foreground contributions. Those differing grouped/separate results are permitted by the explicitly retained composition freedom, not a new pixel oracle.

These cases isolate style ink from metric/state behavior, R from S, unequal-DPI transform order, negative spacing and coverage grouping. No exact image generation or renderer test is required to express the conformance boundary.

## Verification and delivery

Independent exact-rational shear/corner bounds, positive/negative layout, local-band spans, illustrative bold ink span and logical grouping arithmetic pass; the original ALPHA 4 fingerprint matches. `run/lint`, `run/check-links` (0 bad of 5,806 links across 237 files) and `git diff --check` pass. Refresh 2026-11-20 00:01:45 UTC finds no #96/#97 comments or newer discussion/inline feedback; all 47 open PR review histories have no newer feedback. Delivered in [PR #99](https://github.com/bbs-land/remote-imaging-protocol/pull/99), specification **f17a924**, branch `issue-97-outline-effects-boundary`. GitHub confirms open/ready/mergeable and exactly #97 as closing reference. Consolidate its actual head into fork stable. Original sources/assets and adopted earlier contracts remain unchanged except explicit cross-reference/metric-ink clarification. Retain Codex trailers and scoped API temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)
