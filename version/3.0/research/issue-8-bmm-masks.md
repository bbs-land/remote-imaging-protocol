# Issue #8: BMM masks and compositing

[Research backlog](issue-backlog.md) · [Inherited icon formats](../../2.0/techspecs/3.0-icon-formats.md#bmm---icon-masks) · [3.0 bitmap commands](../ripscrip/4.0-icons-and-bitmaps.md)

## Status and scope

Started 2026-11-16 for [issue #8](https://github.com/bbs-land/remote-imaging-protocol/issues/8), branch `issue-8-bmm-masks`, based on `a6da898` (delivered #7 / PR #34). GitHub refreshed after that delivery confirms #8 is the oldest RealDeuce issue without a complete resolution PR. Investigation incomplete; no resolution PR yet.

Required answers: accepted BMM pixel depths; bit/index versus resolved palette RGB; mask polarity; source/mask composition order under COPY/XOR/OR/AND/NOT; dimensions and missing/mismatched behavior; interaction with BMP transparent-color flags; and an independent small icon/mask pixel oracle. Do not prescribe a plausible 1-bit/nonzero-index convention from missing corpus examples. Establish whether a BMM is automatically consumed as an attached mask at all, or only an explicitly loaded bitmap using ordinary put modes.

## Primary evidence and starting points

Use the unchanged RIPtel 3.1 DLL / RIPscrip driver 3.0.7, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred image base **0x10000000**; all DLL addresses below are RVAs unless stated otherwise. [Issue #3 recovery/provenance](issue-3-extended-font-style.md#artifact-recovery-2026-11-16). The static unpacked original client retains the identity in [issue #5](issue-5-scene-headers.md); EXE addresses are relative to **0x400000**.

The inherited 2.0 format page records BMM as the renamed MSK BMP container and leaves its raster convention unverified. No BMM has been recovered from the known installs; that absence does not establish its decoder or mask semantics. Original 1.54 manual AND/OR stamping is a lead, not an automatic 3.0 mask contract.

DLL extension constants near RVA `0x7a8e4–0x7a914` contain `.MSK`, `.BMM`, `.HIC`, `.BMH`, `.ICN`, `.BMP`. Trace every BMM cross-reference, the original load-icon/load-bitmap handlers, button bitmap display and generic BMP decoder/compositor before inferring any automatic paired-mask behavior. Derive accepted headers/depths and transparent-color handling from executable branches, with controlled file/GDI services where needed.

No GUI/Wine is authorized on the desktop: use native/static probes. If actual GDI execution becomes necessary, follow AGENTS.md’s private Xvfb plus task-specific Wine prefix and explicit DISPLAY rule. Every new commit includes Codex co-authorship.

## Explicit BMM routing and initial native checks

Original dispatch entries: physical slot 88 (`1b`, RIP_LOAD_BITMAP) handler **0xc569**, slot 97 (`1I`, RIP_LOAD_ICON) handler **0xcb38**, slot 107 (`1U`, button) handler **0xa952**. Both explicit image handlers normalize legacy extensions and then forward a **single filename** to the ordinary BMP display routine **0x4a410**. A named BMM is not itself changed into an extra implicit transparency request in these paths.

All direct `.BMM`-constant references found in the DLL are RVAs **0xbf09, 0xc739, 0xcc7f, 0x279f3, 0x28541, 0x389e4, 0x46b3f**; `.MSK` references are paired nearby. Explicit loads at 0xc723/0xcc6b replace an explicitly supplied `.MSK` extension with `.BMM`; they do not search for a same-named mask alongside a BMP. Other references include conversion/file-management families and still need full classification before excluding automatic masks globally. Button rendering also still needs its primary trace.

RIP_LOAD_BITMAP maps mode **0/1/2/3/4** to GDI raster operations **0x00cc0020 / 0x00660046 / 0x00ee0086 / 0x008800c6 / 0x00330008** (COPY/XOR/OR/AND/NOT-source) at `0xc816–0xc854`; icon handler repeats that mapping at `0xcd5a–0xcd95`. The transparent flag 8 is independent: when absent it forwards signed key **-1**; when set, it validates/translates the supplied key through `0x44385`/`0x16140` and forwards it separately from the raster operation. Complete compositor handling is not yet established.

New [`run/riptel-bitmap-probe`](../../../run/riptel-bitmap-probe) / [C implementation](../../../run/lib/riptel-bitmap-probe.c) passes **48 preliminary checks** against the unchanged DLL. It executes the two explicit original handlers for five modes with `MASK.BMM`, verifies exactly one ordinary lookup/display and unchanged filename, checks MSK-to-BMM normalization, separate flag-8 key forwarding and missing-file lookup without a paired-file request. Final display routine `0x4a410`, lookup `0x3f80e`, string normalization `0x3f71a`, macro expansion and coordinate conversion are controlled services, alongside drawing/scratch/CRT/error providers listed in the source. These checks establish routing/argument decisions, not completed bitmap drawing or absence of a second lookup inside a hooked downstream display routine. No GUI or filesystem asset writes occur.

A failed initial icon-mode assertion was a **fixture-layout error**: the actual handler reads mode from decoded argument **3**, not 2 as the inherited reference suggests. Original dispatch slot 97 has **seven numeric fields**, widths `[XY,XY,1,1,1,1,1]`, followed by text. The bitmap route reads mode from argument 5. Native direct-handler checks now use the original offsets, but complete wire-field naming remains to derive before correcting the icon entry; do not silently import the inherited layout or infer a 2.x client change.

## Generic bitmap/compositor leads

`0x4a410` calls original file reader **0x4edc5**, geometry helper **0x49340**, palette activation **0x49120** when requested, color-table expansion **0x48a20**, remapping **0x4ea60**. With key <0 it invokes **0x4e42b** (StretchDIBitmap) with the requested ROP. With a key, it constructs a transparent mask through **0x493c0**, obtains background/source image DIBs through **0x48f20/0x49000**, applies the requested ROP to an intermediate composite for non-COPY modes, merges through **0x49f90**, then copies the result to the destination. Exact semantics, failure paths and all depth variants still need end-to-end verification.

`0x4edc5` reads a BMP file header, allocates/reads the remaining bytes into packed DIB memory and returns a handle. It does not branch on BMM extension or itself establish all accepted pixel depths. `GetBitsAddress` **0x4e1ec** knows depth products 1/4/8/16/24/32 and BI_BITFIELDS palette/mask offsets; this pointer helper alone is not sufficient proof that every complete loading/transparency path accepts every header/depth. `0x48a20` expands 4-bit pixels to 8-bit and extends indexed color tables; `0x4ea60` builds nearest-palette mappings and remaps 8-bit raster bytes before transparency selection. Trace final output and clipping rather than treating these helpers as a standalone BMP conformance specification.

### Transparent-color classifier observations

The native probe executes original pure pixel classifiers **0x49ba0 (1-bit), 0x49c90 (4-bit), 0x49d60 (8-bit), 0x49df0 (24-bit)** with controlled valid Windows DIB headers/pixel buffers. Key 0/1 on raster bits 0,1,0,1 returns equality-mask bytes 1,0,1,0 / 0,1,0,1; key 7 on nibble/index rasters 1,7,7,1 yields 0,1,1,0. These helpers compare the passed pixel indices without reading palette RGB. The 24-bit helper compares B/G/R components exactly; a one-component difference changes the mask. Mask byte 1 corresponds to matching transparent-color selection in these classifiers; its eventual retain/overwrite polarity is still to establish through the compositor, and these are internal color-key masks, not automatically file-backed BMM masks.

The selector table at **0x49b74** accepts depth products 1,4,8,24,32 for mask creation and sends **16** to the unknown-format error branch (despite other bitmap helpers supporting 16). The **32-bit classifier 0x49ea0** uses suspicious bit tests/constant arithmetic rather than the 24-bit component-equality loop; verify actual outcomes natively before documenting its behavior. Pure merge **0x49f90** has per-depth cases for 1/4/8/16/24/32 and preserves destination pixels where a mask byte is nonzero; its width/stride/layout and earlier-stage composite must still be checked. Do not generalize from these isolated helpers to the full flag-8 contract yet.

## Native mask/merge checkpoint

The preliminary probe now passes **100 checks**. It additionally executes original internal transparent-mask creation `0x493c0` for a valid 16-bit Windows DIB: the unknown-format branch reports one captured dialog and returns no mask. Controlled stdcall providers replace GlobalLock/GlobalUnlock (native fixture pointers), GetDeviceCaps (24-bit true-color / one plane) and MessageBoxA (capture only); allocator `0x65ca3`/release `0x65cbf` use native zeroed buffers. This proves the actual 16-bit rejection, not merely a decoded jump table.

Original **`0x49f90` dibMerge** now runs on 4×1 source/destination fixtures for depths **1,4,8,16,24,32**, using the original bit-address, row-width and per-depth copy instructions. Source is zero/black, destination all ones/white, selector bytes 0,1,0,1: merged output alternates source/destination, retaining destination exactly at selector-one pixels. Packed 1-bit result high nibble is 0x5; packed 4-bit result is 0x0f,0x0f. All larger-depth component bytes agree with that polarity. This verifies the internal merge; the selected source still depends on the earlier ROP/composite stage.

Original **`0x49ea0` 32-bit classifier** was executed with two black/white BGRA pixels, keys black, white and RGB(17,34,51). It marks neither pixel transparent in every case, including exact matching black and white. The routine’s final red-component test subtracts 0xff0000 from a sign-extended **16-bit** component argument before unsigned comparison, so normal byte-valued red components cannot set its Boolean bit; the later AND/test cannot mark any ordinary RGB key. Preserve this as a shipped color-key defect when publishing the fully traced path; do not substitute the 24-bit comparator for this original routine. No GUI was launched for either the rejected 16-bit case or the 32-bit defect.

## Next action

Continue #8 only. Finish BMM reference classification and button filename lookup, then exercise original BMP display/reader/GDI-boundary and transparent composition with tiny DIB fixtures. Verify accepted depths, palette-to-pixel transformations, 16/32-bit color-key differences, mode ordering and missing/mismatched dimensions. The current 100-check probe is preliminary and no mask rule or resolution PR is yet published. Keep findings under the 3.0 client scope; update the inherited 2.0 page with a qualified cross-reference rather than assuming backwards equivalence.

---

[Research backlog](issue-backlog.md) · [Inherited icon formats](../../2.0/techspecs/3.0-icon-formats.md#bmm---icon-masks) · [3.0 bitmap commands](../ripscrip/4.0-icons-and-bitmaps.md)
