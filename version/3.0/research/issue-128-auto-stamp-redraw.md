# Issue #128: Auto-stamp redraw and destination handoff

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md) · [Graphical continuations](../ripscrip/5.2-templates-and-conditionals.md#creation-redraw-and-prompt-continuations)

## Scope and checkpoint

Started 2026-11-22 on `issue-128-auto-stamp-redraw` from fork stable a857052. [Issue #128](https://github.com/bbs-land/remote-imaging-protocol/issues/128) requests an explicit reconciliation of auto-stamp's converted CLIPBOARD face/source/label geometry and selected restore/provider paths, plus intentional PORT6 replacement/PORT0 handoff versus external destination invalidation. Adopt earlier #27/#119/#124 rules, host-slot opacity, one-pass permissions/bounds and no automatic activation. RIPscrip 1.54 is outside scope. Source facts and portable continuation policy must remain separate; no original/client/UI/probe execution or obsolete-client bug investigation.

Initial refresh started 2026-11-22T01:03:45Z: 64 open RealDeuce issues, 62 PRs, only #128 uncovered, zero comments and no newer discussion/inline/review feedback since 2026-11-21T22:29:35Z. Report body SHA-256 `da68bc20dfd70761682baf30dbf82a9db23cddbd288685445aaed1e9a04e6e52`, updated 2026-11-21T23:42:54Z.

## Primary sources and documentary precedence

Pinned original RIPtel 3.1 / RIPscrip 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [mirror](../../../reference/rip-tools.md): 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Base 0x10000000; addresses below are RVAs; `.text` file offset = RVA−0xc00. LLVM objdump / Python read unchanged instructions/data only. No client, UI, renderer/provider or original instruction was executed.

The preserved [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, BUTTON_STYLE flag 64 explicitly says capture includes special effects, excludes RECESS and precedes label drawing. It also expressly says auto-clip is **ignored when its target/clipboard is the current drawing port**. This documentary guard takes precedence over a legacy helper reaching deletion/replacement. Earlier #27 established the current-clipboard mechanism and successful flag writes, not a protocol exception authorizing self-destruction of the effective destination. Correct the missing guard in the reconstructed reference; retain those bounded source findings without requiring their unsafe case. This is protocol work, not a client defect investigation.

The report's two premises therefore need distinctions: a capture helper's +1 does not necessarily add a logical image pixel after the caller's far adjustment, and a helper's same-target fallback does not authorize a protocol destination handoff. Do not silently treat either as an adopted exact geometry/continuation rule.

## Bounded original source findings

| Location | Finding |
| --- | --- |
| `0x2adac–0x2aef7` | Renderer maintains face/effect/capture rectangles separately; far-coordinate adjustments precede capture. Initial persistent hit expansion is at `0x2aee2`; successful auto-clip has another hit adjustment at `0x2b03a`. These are not a reason to reopen adopted once-only half-open hit policy |
| `0x2ae41–0x2ae58` | Copy of a drawing rectangle has far X/Y decremented; `0x2aef4–0x2aef7` copies that rectangle to capture storage |
| `0x2af8b -> 0x2866` | Capture occurs after face/effects, before label |
| `0x287d–0x2896` | Helper computes `(far-near)+1` on each axis; pair this with the caller's convention before asserting an extra visible row/column |
| `0x28fe -> 0x33112`, `0x294e -> 0x3326f` | Existing clipboard slot is deleted, then allocated/redefined; source record is fetched afterward at `0x2982–0x299a`. This raw path is not the documentary same-target guard |
| `0x29ed -> 0x134d0` | Copy from the then-current source to the clipboard; no promise that deleted source pixels survive |
| `0x2b040–0x2b063` | Successful capture changes field and actual current button-style flags to `(flags & 0x7c37) | 1` before label expansion; failures preserve earlier nontransactional effects as already adopted |
| `0x2c338/0x2c380` | Both selection/unselection wrappers update state and call the common renderer at `0x2c377/0x2c3be` with initial=0. Neither is a saved-image-return shortcut |
| `0x2a76a -> 0x4b0e4`, `0x2ac4c -> 0x2a00`, `0x2b0a1 -> 0x4b0e4` | Common nonempty filename preparation precedes clipboard paste; nonempty label expansion follows. A successful full redraw reaches both providers, selected or not |
| `0x2aca6–0x2ad59` | Clipboard natural-size write is creation-gated; it does not establish a new size on every later redraw |

The #119 note already says the raw clipboard paste helper uses different backing/viewport quantities and is not a complete coherent geometry oracle. This follow-up does not investigate those legacy discrepancies. The primary effect-inclusive capture intent and the adopted stable hit/creation geometry instead receive an explicit portable completion.

## Chosen geometry and provider completion

Retain original inner face F, captured creation WORLD/font/style/destination and once-clipped hit H. At successful auto-stamp, freeze a separate baked-image footprint C: F expanded by the already converted BEVEL, containing CHISEL/SUNKEN pixels, excluding RECESS and label. Clearing BEVEL prevents drawing it again; it does not erase the baked outer footprint or redefine F/H. Keep a source crop of size C and map it one-for-one to C. Source viewport changes cannot resize/recenter/stretch an existing stamp; larger sources crop at their viewport origin, smaller/partially unavailable sources pad with current destination palette index 0 under COPY. Empty/inverted/wholly unavailable sources fail the redraw before the label; definitions and earlier effects persist under #124.

A logical 20×10 face at (100,50), bevel 3, gives F=[100,120)×[50,60), C=[97,123)×[47,63), size 26×16; stored hit is the prior destination intersection of C. Logical clipboard viewport is [0,26)×[0,16). Passing inclusive capture corners (97,47)..(122,62) to a +1 helper allocates exactly 26×16. If a backend overallocates 27×17, its final row/column are capacity only and must remain outside that logical viewport. No allocation convention grows F/H or label bounds. This effect-inclusive source geometry is explicit protocol completion, not a claim that the original rectangle bookkeeping reproduces it exactly.

Both actual selected/unselected full redraws repeat the separate nonempty filename and label stages using live variables, under #124 permissions/limits. Converted CLIPBOARD does not load the former icon or recapture. A pixels-only temporary-feedback restoration executes zero providers and cannot substitute for a requested semantic full redraw. No-op input that changes no presentation likewise executes none. Empty slots and earlier failures keep their normal skip boundaries. A changed source/value must be observed at the next actual redraw rather than hidden behind a selected-state cache shortcut.

## Same-target guard and continuation disposition

Compare capture target with the **effective destination**: PORT0 for a mouse-active field, captured current port for a nonmouse decoration. On equality, ignore this instance's automatic-capture stage before deletion, allocation, clipboard/source changes or capture-success style writes. Keep its original type/effects, render the label on the same destination, and leave the style request for subsequent new definitions. This creation-time skipped attempt is not retried by that instance's later redraws. It is an explicit once-only completion of the documentary ignore rule.

Consequently the requested PORT6=current clipboard=nonmouse destination example has **no internal handoff**: PORT6 keeps its generation/pixels, label remains on PORT6 and any label prompt freezes/checks that identity. Changing an unrelated clipboard port is not a destination-generation change. A mouse-active button whose effective PORT0 differs from an unrelated current/clipboard PORT6 can still capture into PORT6 without moving its already captured PORT0 destination.

If an implementation has **already** committed a replacement before detecting the stale destination, #124 still forbids rollback of completed clipboard/style effects, but it must abandon that stale drawing continuation. It cannot rebind the label to PORT0 merely because a legacy helper fell back there. A command-authorized destination transfer would need an explicit protocol rule; auto-stamp provides none for this guarded case. External replacement during suspension likewise invalidates. This resolves the report's handoff-versus-cancellation choice explicitly while respecting the primary documentary rule, rather than presenting legacy fallback as intended protocol.

## Remaining work and reproduction

Publish the geometry/source/provider and same-target/late-invalidity tables with independent 20×10/bevel-3, source-crop/padding, label and PORT6 vectors. Verify source hashes/calls/critical instruction sequences, flag masks and geometry/provider-count arithmetic; run repository checks, refresh feedback, publish separately and consolidate its actual head before final coverage/ancestry audit.

```sh
objdump -d --start-address=0x10002866 --stop-address=0x10002b1e ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002adac --stop-address=0x1002b0a9 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002c338 --stop-address=0x1002c3c7 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

---

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md) · [Graphical continuations](../ripscrip/5.2-templates-and-conditionals.md#creation-redraw-and-prompt-continuations)
