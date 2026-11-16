# Issue #8: BMM masks and compositing

[Research backlog](issue-backlog.md) · [Inherited icon formats](../../2.0/techspecs/3.0-icon-formats.md#bmm---icon-masks) · [3.0 bitmap commands](../ripscrip/4.0-icons-and-bitmaps.md)

## Status and scope

Started 2026-11-16 for [issue #8](https://github.com/bbs-land/remote-imaging-protocol/issues/8), branch `issue-8-bmm-masks`, based on `a6da898` (delivered #7 / PR #34). GitHub refreshed after that delivery confirms #8 is the oldest RealDeuce issue without a complete resolution PR. Derivation and implementor specification complete; resolution PR pending publication.

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

## Completed routing audit

All seven previously listed `.BMM` addresses identify the **immediate operand**, one byte after its PUSH instruction. The actual PUSH RVAs and complete dispositions are:

| PUSH RVA | Owner/path | Role |
| --- | --- | --- |
| 0xbf08 | RIP_FileQuery (`RIP_FileQuery()` error identity at string RVA 0x7a924) | Normalizes an explicitly queried MSK filename to BMM before one file lookup; no raster drawing. |
| 0xc738 | RIP_LOAD_BITMAP | Normalizes the explicitly requested MSK name; one display call at 0xc957. |
| 0xcc7e | RIP_LOAD_ICON | Same explicit normalization; one display call at 0xcdde. |
| 0x279f2 | Clipboard icon writer 0x27930, called by write-icon handler at 0xdde5 | Normalizes output filename and calls 0x27a90 to write the clipboard raster. This is **not** a file-delete function; that early uncommitted working hypothesis was rejected. |
| 0x28540 | FILEEXISTS text-variable callback | Normalizes the filename before checking existence; diagnostic identity `tvarProcFILEEXISTS()`. |
| 0x389e3 | Local bitmap playback 0x38910 | Normalizes the explicitly supplied name; one COPY/no-key display call at 0x38b34. |
| 0x46b3e | RIP_PortWrite | Normalizes output filename before writing the chosen port rectangle via 0x27a90; diagnostic identity `RIP_PortWrite()`. |

The common renderer 0x4a410 has exactly six direct CALL sites in the complete DLL disassembly: **0xc957, 0xcdde, 0x2aa42, 0x2aafa, 0x2abb9, 0x38b34**. The middle three belong to `ripButtonDraw()`: hot-icon handling replaces the extension with BMH at 0x2a9ea and calls 0x4a410 at 0x2aa42; failure replaces it with BMP at 0x2aaa1 and calls at 0x2aafa; the ordinary icon branch calls at 0x2abb9. All three pass COPY and key -1. They never construct or fetch a companion BMM. This audit includes both name construction and downstream reader/display logic; it does not infer absence merely from the asset census.

Consequently the resolution is **no automatic attached BMM raster** in these RIPtel 3.1 paths. BMM is a BMP filename role usable for explicit stamping. A host wanting a manual two-image composition must issue both loads, with the mask in AND mode and the source in OR mode. Different put modes have their ordinary meanings; the extension does not choose an implicit combine order or Boolean transparency test. Absence or different dimensions of an unreferenced same-name BMM has no effect.

## Full original renderer with controlled drawing boundary

The expanded [`run/riptel-bitmap-probe`](../../../run/riptel-bitmap-probe), including [`riptel-bitmap-services.h`](../../../run/lib/riptel-bitmap-services.h), passes **4,975 checks**. Reproduce from the checkout:

```sh
run/riptel-bitmap-probe /path/to/unchanged/ripscrip.dll
```

The wrapper verifies the DLL hash before mapping; it builds native i386 code with `cc -m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`. This checkpoint ran on FreeBSD 15.1 amd64 with clang 19.1.7 and Capstone 5.0.7. No original file or installed font is modified; no GUI is involved.

Original instructions now execute **0x4a410** end to end, including BMP reader **0x4edc5**, geometry **0x49340**, 4-to-8-bit expansion **0x48a20**, palette remapping **0x4ea60**, display **0x4e42b**, mask generation **0x493c0**, background/source capture orchestration **0x48f20/0x49000**, requested-ROP intermediate composition, pure merge **0x49f90** and final COPY. The native fixture supplies an in-memory file (`fopen/fread/ftell/fseek/fclose` hooks), global-memory APIs, palette entries, RGB drawing surfaces, compatible-DC creation **0x48d60**, HBITMAP-to-DIB extraction **0x4de5f**, error capture and invalidation. The full hook/IAT list is executable in `renderer_checks()`; filename/lookup and early handler controls remain in `main()`. Palette activation mode 1 is not exercised; fixtures use the existing current palette (renderer mode 2). GUI dialogs are counted, never displayed.

The surface model implements ordinary GDI RGB raster operations and nearest-neighbor scaling. Its RGB encoding is independently specified by Microsoft’s [GDI BITMAPINFOHEADER documentation](https://learn.microsoft.com/en-us/previous-versions/dd183376%28v%3Dvs.85%29); [StretchDIBits](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits) documents the DIB_RGB_COLORS interpretation used by the actual import call. These current API references explain the controlled boundary; they are **not** proof that a modern header/compression/OS capability existed in the 1997 client. Original decoding/remapping/classification/merge are executed, whereas Windows rasterization, device-palette quantization and arbitrary stretch filters are not emulated or claimed tested.

Fixtures are valid 40-byte Windows BMP headers, one plane, BI_RGB (0), positive height, packed color table and bottom-up DWORD-padded rows. Ordinary explicit BMM loads accept depths **1/4/8/16/24/32** and all five modes. Palette index 0 is deliberately **white**, index 1 **black**, raster 0,1,0,1: AND retains the background at index-zero white pixels and clears at index-one black pixels. Thus a nonzero-index transparency rule predicts the wrong result. Other modes agree with the independent RGB oracle. The reader opens exactly the requested file, once per invocation; there is no companion lookup within the original downstream code.

The complete flag-8 route runs both black and white keys for every depth/mode. Depths 1/4/8/24 succeed and retain the original background at selected pixels; every other pixel is the requested ROP of source and original background. All 16-bit cases return AX=1 and a captured unknown-format error without changing the destination. All 32-bit cases succeed but select no transparent pixels, confirming the isolated classifier defect through final output. Flag-8 masks are generated internally, rescaled with the source geometry and applied **after** the intermediate ROP; they are not BMM files.

Downscaling 4×1 to 2×1 and upscaling to 8×1 exercise original geometry and generated-mask orchestration with COPY/XOR/OR/AND/NOT, keyed and ordinary. Both use exactly one requested file; the fixture’s nearest-neighbor output and untouched pixels outside the rectangle match the independent oracle. This establishes independent dimensions and matched source/internal-mask target geometry, not original Windows stretch-filter parity. Missing explicitly requested BMM returns AX=1 before GDI, with no drawing; the higher command’s already-probed lookup failure takes its ordinary placeholder path. No BMM-specific missing-file or size-mismatch policy exists.

A final explicit two-load oracle uses 1-bit MASK.BMM (palette 0 white, 1 black; raster 0x50) in AND mode and a 24-bit SOURCE.BMP containing black, RGB(F0,F0,0F), black, RGB(0F,0F,F0) in OR mode. On background RGB(12,34,56), final pixels are background, RGB(F0,F0,0F), background, RGB(0F,0F,F0). The fixture preserves the intermediate surface while replacing the in-memory file between the two supplied loads; no automatic second read is credited.

### Color-key input and palette translation

The wire key is validated at **0x44385** with third argument 0: only **0..255** are accepted. Original **0x16140** is then called with third argument 0. Its comparison at 0x161d3/0x161d5 preserves the supplied index when it is **below** the signed-word limit at palette+0x82b; otherwise it reads that entry’s current drawing-palette RGB and finds a nearest index through **0x16070**. An earlier transient reading reversed this branch; the native tests settle it. Initialization at 0x43efb–0x43f31 sets that limit to **2 for GetDeviceCaps(BITSPIXEL)==2, 16 for ==4, otherwise 236**. Do not describe it as a universal 256-entry pass-through or a file-palette index.

Native checks restore these original routines after handler-routing tests, replace only palette locking **0x447c0/0x44802**, and exercise all 256 valid indices at limits 2/16/236. Logical palette entry 200 deliberately duplicates entry 7: 200 translates to 7 at limits 2/16, remains 200 at limit 236; other unique entries return themselves. Exact matches stop at the first matching index; for nonzero equal-distance candidates 0x16070 replaces the previous best on a tie, so the last minimum wins unless an exact match already stopped the search. Validation checks 0,1,255,256,1295. The full renderer tests supply the resulting key directly and a fixed OS grayscale palette, preserving this boundary explicitly.

Internally 1-bit classification compares the source bit to zero/nonzero translated key; 4-bit data is expanded to 8-bit and mapped to the current OS palette; 8-bit pixels are mapped by nearest RGB palette entry before index equality; 24-bit pixels compare exact B/G/R against GetPaletteEntries(key,1). This differs from testing raw file index or generic RGB proximity for every depth. The 32-bit defect and 16-bit rejection described above still apply.

### Fixture failures resolved

The first full-render attempt rejected an apparently huge temporary surface: the native replacement had read entire argument slots although original 0x48d60 uses signed **low 16-bit** dimensions (upper bits may be stale). Masking/sign-extending those low words fixed the ABI. The next run drew correctly but counted `DestroyBitmapDC()` mismatch errors because the SelectObject fixture always returned 1; maintaining each DC’s selected bitmap restored the original previous-object/cleanup contract. These were harness defects, not client failures, and are not published as rendering limitations.

## Publication validation

The [bitmap-mask delta](../techspecs/3.0-bitmap-mask-delta.md) answers every #8 subquestion, with qualified pointers from the inherited 2.0 format page and updated 3.0 command/content-role entries. `run/riptel-bitmap-probe` passes 4,975 checks; repository-wide `run/lint`, `run/check-links` (0 bad of 4,665 links across 191 files) and `git diff --check` pass. The first link check caught Prettier angle-wrapping a URL containing parentheses; percent-encoding that URL fixed it. AGENTS.md now records the signed-low-word and SelectObject fixture requirements.

## Next action

The required #8 subquestions are now derived. Publish the qualified RIPtel 3.1 bitmap-mask delta, including explicit RGB algebra, supported profiled depths, absent/unreferenced/mismatched cases, exact key selection and the small independent mask/source oracle. Replace the 2.0 page’s speculative monochrome rule with a scope-qualified pointer; update 3.0 command/content-role pages. Keep the inherited icon decoder-field discrepancy recorded for the broader command-layout work rather than inferring unnamed fields here. Run required checks, commit with co-authorship, file the complete #8 PR dependent on #34 with an issue-only comparison from a6da898, record delivery, then refresh GitHub before #9.

---

[Research backlog](issue-backlog.md) · [Inherited icon formats](../../2.0/techspecs/3.0-icon-formats.md#bmm---icon-masks) · [3.0 bitmap commands](../ripscrip/4.0-icons-and-bitmaps.md)
