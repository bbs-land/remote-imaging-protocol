# Issue #119: Image-button face sizing

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md#rip_button)

## Scope and checkpoint

Started 2026-11-21 on `issue-119-image-button-sizing` from stable 915ca0f, after #118 / PR #121 actual head 073317e was normal-merged with an identical tree. [Issue #119](https://github.com/bbs-land/remote-imaging-protocol/issues/119) asks for complete per-type face sizes, clipboard source viewport/physical clipping/inactive eligibility, icon reference scaling and normal/hot size stability, and ignored style/far inputs versus dynamic-plain omission. Preserve prior type/payload/deferred-host/half-open destination eligibility and gesture rules; #120 follows. Portable completion is allowed.

## Sources and findings

Original RIPtel 3.1 / RIPscrip 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`; image base 0x10000000. RVAs below map to file offsets with `.text`−0xc00 / `.data`−0x1a00. LLVM objdump 19.1.7 / Python read only. Immutable [ALPHA 4](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, §3.4.2.3 explicitly restricts fixed style dimensions to PLAIN and defines the clipboard image as its whole viewport. Those rules take precedence over a generic constructor's size/far fallback. Prior #27 replaced paste service arguments and did not establish image sizing. No original/client/UI/probe/renderer execution or bug investigation.

Clipboard renderer's initial natural-size branch `0x2acbe–0x2ad40` reads current clipboard viewport edges +0x3a/+0x42 and +0x3e/+0x46, subtracts them and adds the resulting DEVICE width/height to the destination anchor, writing face/interaction far edges. There is no WORLD conversion there. Paste service `0x2a00` instead derives a source rectangle from port extents +0x2a..+0x36 and offsets it by viewport origin. These are not proof of a coherent partially clipped viewport contract. Apply the explicit primary whole-viewport image rule, not an accidental physical-port extent. Do not investigate that legacy discrepancy.

Icon routes call `0x4a410` with stretch=1; loader at `0x4a49e` invokes natural geometry helper `0x49340`, which obtains decoded bitmap width/height (`0x4e38e` / `0x4e348`) and calls current WORLD→device conversion at `0x4937e` before adding the anchor if far coordinates are omitted. The button renderer temporarily activates its **captured creation WORLD** (`0x2a635–0x2a651`, field +0x3a/+0x3c), then restores the caller frame. There is no fixed 640×350 stage here: do not transfer the separate legacy load-icon scale rule. Thus natural icon size is floor(decodedWidth×deviceWidth/creationWorldWidth), similarly Y. This is resolution-reference scaling, not always native device bitmap dimensions. Later current-WORLD changes do not rescale an existing instance.

Initial icon geometry writes at `0x2aa52–0x2aa89` are gated by initial draw and omitted natural far values, taking the rectangle returned by the image selected under the adopted normal/hot/fallback rules. Later redraws use stored extents. The coherent completion below fixes the first successful creation image's geometry and fits any differently sized later normal/hot image into it; an initially selected hot image can therefore establish a different initial face. It does not add a mandatory unused normal-image load or alter failure/fallback/key/palette rules.

## Explicit completion and reconciliation

The per-type table makes style size and all command far coordinates irrelevant for CLIPBOARD/ICON, including zeros, reversal and mixed/complete style sizes; only PLAIN applies corner ordering/omission/fixed-size guards. Source natural-image branches support the sizes above but do not establish every nonnatural-input combination; the primary plain-only restriction supplies that rule. Anchor still converts once in creation WORLD with the destination origin; image size does not move that anchor.

For clipboard reads, accept a defined, readable source with positive viewport even if inactive/protected: deactivation is a drawing gate, not a new read prohibition. Explicit safe completion rejects zero/inverted viewport or wholly unavailable backing intersection. A partial physical intersection does not shrink/stretch the logical image: retain whole viewport width/height, copy available pixels at their viewport-relative offset and fill missing face pixels with current destination palette **index 0**, COPY semantics. This is an explicit padding/eligibility choice, not a native raster claim or RGB-black reset rule. Do not mutate the source viewport/selection. Source geometry and chosen image face dimensions are captured at creation; later source/pointer/WORLD changes cannot reflow an existing field. Existing rendering can refresh pixels within its fixed face, without promising a frozen pixel snapshot.

Destination clipping, effects and label placement use the whole logical face, not the source physical intersection; #116 half-open hit/empty/destination rules apply afterward. Failed image eligibility creates no field/painting/host action. New icon dimensions that scale to nonpositive device size likewise fail before effects. The hot redraw completion preserves face/hit/effect/label geometry even when normal/hot dimensions differ.

## Reproduction and remaining work

```sh
objdump -d --start-address=0x1002acbe --stop-address=0x1002ad40 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10049340 --stop-address=0x100493b7 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002a635 --stop-address=0x1002a659 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

Publish literal style/image examples and cropped/non-square/inactive/partial source, WORLD and hot-size vectors. Verify source fingerprints/instructions/calls and field/geometry arithmetic, run repository checks, refresh feedback, publish separately and consolidate actual head, then #120. Keep source/policy boundaries, trailers and scoped temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md#rip_button)
