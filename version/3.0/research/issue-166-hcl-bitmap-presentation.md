# Issue #166: HCL bitmap image-style presentation

[Research backlog](issue-backlog.md) · [Local bitmap playback](../ripscrip/5.3-local-playback.md#local-bitmap-playback---filebmp)

## Scope and checkpoint

Started 2026-11-23T03:48Z on `issue-166-hcl-bitmap-presentation` from stable **328f8191f526234bbc554b381aa86972eb9d4ef7**, after #164 / PR #165 actual full head b4ec4a1 was normal-merged with an identical tree. [#166](https://github.com/bbs-land/remote-imaging-protocol/issues/166), created/updated **2026-11-23T03:47:36Z**, zero comments, body SHA-256 `31253ae15c5f968e293135a27cf5d08e45ef53de7ba83f560c7451b7dcb5b770`. Resolve BMP target/far edges/fallback/all-zero/inactive/clipping; flag applicability, deletion/palette/wallpaper; portable indexed/true-color/palette/dither phase boundary and small pixel vectors. Preserve #21 JPEG, #58 wire bitmap and #161 filename contracts. Protocol-only; no original instructions/client/UI/file/renderer execution or obsolete bug investigation. Last all-author check 03:48:19Z, next about 03:58:19Z.

## Starting reference hashes

Reproduce with `git show 328f8191f526234bbc554b381aa86972eb9d4ef7:<path>`:

- `version/3.0/ripscrip/5.3-local-playback.md`: SHA-256 `263702f953d2a61d20aa67785649b5dc81fe06d827d2d5db8ef53fb34d247478`.
- `version/3.0/ripscrip/4.1-jpeg-images.md`: SHA-256 `3f5ba7004bcd3a11ec02b96a32d44b6e89011fa2ebfe69f63ef90b7cdc6714c2`.
- `version/3.0/ripscrip/4.0-icons-and-bitmaps.md`: SHA-256 `f742493e2821582434fbc17c4d379a141d3c27c9ec9d77e42651750ea54690c0`.
- `version/3.0/research/issue-21-image-style.md`: SHA-256 `868317ba2ffa02a20637160716360fd85281377dfcaf6c292788489a11eea4b9`.
- `version/3.0/research/issue-58-load-bitmap-flags.md`: SHA-256 `5c631b6b95194ce17879519b030fa1762f60011a703375f86d231961cf4b789d`.
- `version/3.0/research/issue-161-playback-filenames.md`: SHA-256 `e168c6ff42c5206debdb0cf9e28515ce7279eb542244b2ad1a75a64771c43b1a`.

- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `e22224e1402323d3be4eafe2cce7a871dcec77a28c2fd9a523f1103d328e8ea8`.

## Fingerprinted primary source and initial bounded observations

Catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base **0x10000000**. LLVM objdump/readobj **19.1.7** and existing `run/riptel-inspect` read only. All addresses are **RVAs**, not file offsets. Reproduce with `llvm-objdump19 -d --start-address=<base+RVA> --stop-address=<base+end> <original DLL>`. No temporary dump files or new probes.

#161 already establishes bitmap callback **0x259f7** → filename helper **0x26dd2** → **0x38910(context,name,1)**. The complete bitmap wrapper **0x38910–0x38bd1** checks raw-zero style **0x343b6**, inactive viewport **0x3445b**, and zero stored far boundaries before lookup. It normalizes the already adopted extensions and selects actual stored image area or current viewport via **0x34385** (all-minus-one means fallback). Both branches pass rectangle coordinates **unchanged**, fixed dither=1, palette mode=2, coordinate/stretch selector=0, key=-1 and COPY **0x00cc0020** to ordinary BMP renderer **0x4a410** at **0x38b34**. Unlike JPEG common presentation, no far-edge subtraction appears in this caller.

The complete wrapper has **no image flag +0x5a read, aspect-fit call, initial fill or wallpaper call**. Its only post-render drawing is the existing failure placeholder, not successful-image wallpaper. Do not assume the JPEG flags or wire `1b` flags apply simply because the rectangle is shared. Renderer **0x4a410** calls existing BMP reader **0x4edc5**, geometry **0x49340**, palette activation only for mode 1, current-palette remapping **0x4ea60**, and the key-negative output path **0x4e42b**. Final geometry/palette/dither/clipping evidence and an explicit portable phase boundary remain to be completed.

## Next action

Finish bounded ordinary bitmap geometry/output/clip reads and source/policy distinction, publish exact target and flag applicability plus portable palette/phase vectors. Run meaningful independent arithmetic/source-byte checks and repository validation, open a separate PR closing #166, normal-merge its actual complete source head to stable with identical-tree proof, audit coverage/feedback/ancestry and continue the watch.

## Completed source reading and derivation

The preserved [ALPHA 4 local bitmap entry](../../2.0/text/RIPScrip-2.0-alpha-4.txt), lines **12007–12031**, establishes image-area/viewport targeting, stretching to the viewport, current palette and automatic dithering. Its generic “adhere to the settings” phrase does not individually describe ASPECT/DELETE/clear/wallpaper behavior. The current reference's JPEG flag table is based on a different complete presentation path; do not import that path into BMP by analogy.

| Original RVA | Positive observation and scope |
| --- | --- |
| 0x259f7–0x25a38 | Owned filename copy, shared 0x26dd2 transformation, then **0x38910(context,name,1)**; preserve #161 naming rules |
| 0x38910–0x3896d | Raw-zero style suppression through 0x343b6; inactive viewport through 0x3445b; zero stored right or bottom suppression before lookup |
| 0x34385–0x343b5 | Only the all-minus-one rectangle selects default viewport fallback; it is not raw zero or wire all-zero style |
| 0x38a0e–0x38a25 / 0x342ad–0x34318 / 0x3fbf–0x4034 | Select current surface/DC, install current viewport region through CreateRectRgn/SelectClipRgn. This supplies the positive viewport clip evidence; protocol adds explicit port/backing intersection without inherited hidden region state |
| 0x38ab3–0x38b39 | Explicit-style path pushes stored +0x4a/+0x4e/+0x52/+0x56 **unchanged**; fallback pushes viewport +0x3a/+0x3e/+0x42/+0x46. Unlike JPEG wrapper, no far-edge decrement. Call 0x4a410 with dither **1**, palette mode **2**, coordinate/stretch selector **0**, key **-1**, current palette handle, COPY **0x00cc0020**, final service options 1/1 |
| 0x49340–0x493b6 / 0x4a47a–0x4a4ba | Geometry helper can substitute natural dimensions for zero far positions and can convert natural dimensions when its selector requests it; this wrapper has already guarded zero stored far positions and passes DEVICE target fields/selector zero. Positive ordinary target width/height are **R-L/B-T**, with no aspect fit |
| 0x4a4ba–0x4a4dd | File palette adoption 0x49120 is conditional on palette mode **1**; macro always requests **2**, so it does not enter that branch |
| 0x4a4dd–0x4a53f / 0x4ea60–0x4eb9f | Expand indexed DIB if required; remap source indexed colors against current palette entries. 0x4eaef obtains GetPaletteEntries; 0x4eb99 uses GetNearestPaletteIndex. File indices are not retained as destination indices |
| 0x4a594–0x4a5c5 / 0x4e42b–0x4e80f | Negative key takes direct DIB display, not mask composition. Whole source dimensions and target extents reach StretchDIBits with caller COPY. Host-specific conversion/stretch branches are outside exact pixel parity |
| 0x38b39–0x38bd1 | Realize current palette, existing failure-placeholder branch, clip cleanup and return. **No image flags are read, no aspect fitter/black clear/delete/wallpaper stage**; no call to 0x37fb5, 0x3f12f or 0x3ea2e in the complete wrapper |

The complete renderer call list in 0x4a410–0x4a90f was also reviewed. With key -1, the direct path reads the named BMP, derives the explicit target, remaps its indexed colors and draws; it never receives image-style flag bits or runs the JPEG/wallpaper helpers. #8's existing direct renderer/source trace establishes no automatic BMM companion. No original code was executed. The existing C observer signature at `run/lib/riptel-bitmap-probe.c:67` independently identifies **argument 8=dither, 9=palette mode, 10=coordinate/stretch selector**; do not mislabel the wrapper's final service options as ASPECT/NOCLEAR. Its earlier controlled routing observations are not new macro/native results.

The [StretchDIBits](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits) and [GetNearestPaletteIndex](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getnearestpaletteindex) primary API contracts explain destination stretching, source RGB interpretation and nearest logical-palette lookup. They do not require modern JPEG/PNG passthrough, identical 1997 color realization or any particular new dither algorithm.

## Explicit portable completion and scope limits

Publish successful supported BMP presentation: target is stored DEVICE I or playback-time V, clipped after mapping to V/B/backing, with no hidden backend region state. Nonpositive/empty target writes no pixels. Preserve port/style/table/query/cursor/clipboard/pair state and the local file; no new decode/error-placeholder contract is inferred. All six flags are ignored on this path, so there is no wallpaper seed/stagger calculation. The all-zero wire-style 1×1 target is distinct from raw-zero suppression and default fallback.

Current palette ownership is fixed; indexed input uses file RGB for interpretation, true-color input its decoded RGB. For reduced-color destinations choose automatic approximation/dithering using current available colors; a true-color destination can retain representable decoded RGB. Exact representable uniform colors remain exact. These portable color rules clarify documentation intent without specifying Windows reserved/system palette slots, a nearest-color tie metric, stretch filter or native dither pixels.

Dither/sampling coordinates are explicitly anchored to the **unclipped target origin**, with crop-equivalence for a fixed renderer profile/source/palette. Stateful error diffusion must preserve the conceptual omitted prefix, rather than restarting at clipped edges; no intermediate full image allocation is required. This is an explicit interoperability completion, not a phase recovered from the vintage device-dependent algorithms. It is independent of vector fill-mask origin/phase.

The small 2×1 uniform-red indexed-source vector has file index 0 red versus current palette index 0 green/7 red, a 4×4 target from wire corners 2,2..5,5, and all six flags set. It distinguishes current-palette RGB remapping, inclusive setter/direct exclusive consumer bounds, ignored aspect/clear/wallpaper/delete and retained outside pixels without imposing a stretch filter or approximate-color oracle. The 24-bit uniform-red equivalent and clipped/zero/fallback variants use the same exact-color rule. Geometry and fixed-call/source-byte validation are recorded below.

## Independent validation and specification checkpoint

At 2026-11-23T03:56Z, inline Python validation passes **seven baseline hashes**, the original DLL length/hash/read-only inspector, **13 exact byte sequences** and **13 direct call targets**. Full-wrapper disassembly confirms the positive fixed-argument BMP path with no image-flag operand or JPEG/aspect/black-clear/wallpaper call. This is static source evidence, not new native execution.

A separate in-memory **66-byte 8-bit BI_RGB BMP** encodes the 2×1 red/red vector: 14-byte file header, 40-byte info header, two RGBQUAD entries, two index-zero raster bytes plus DWORD padding. File entry 0 resolves red while destination entry 0 remains green and entry 7 red. No fixture file, renderer, original decoder or media/provider was run. The ordinary 24-bit uniform-red comparison is a decoded-color/geometry oracle, not an additional historical format claim.

The abstract model checks normalized/inclusive stored geometry for four source-coordinate sets, WORLD/device scale 1/2/3, three viewport clips, all 64 stored flags and active/inactive targets: **4,608 geometry/state combinations**. It independently compares rectangle intersection against pixel membership, exact red bounds and unchanged outside pixels/palette. Separate assertions cover 4×4/16 pixels, clipped 2×2/four pixels, all-zero 1×1 versus default viewport, reversed endpoints and the full wire literal's fields **[2,2,5,5,63,0]** with exactly 12 reserved zeros.

A chosen nearest-neighbor illustration checks full-target RRBB cropped to BB versus an incorrect refitted RB; this is an illustrative renderer profile, not a mandated filter. A separate chosen 2×2 ordered profile checks crop-equivalent image-local phase; it does not claim the old driver's dither algorithm. Original 1.54/text/assets remain unchanged, and exact pre-bitmap filename/scene/audio content is preserved.

The 3.0 playback entry, JPEG boundary cross-link and IMGSTYLE clarification now state the completed contract. Validate formatting/links/diff and refresh #166 before publishing one complete PR, then normal-merge its actual head to stable with identical-tree proof and audit feedback/coverage/ancestry. Last issue check 03:48:19Z, next about 03:58:19Z. No new original/client/UI/provider execution, no bug investigation and no task temporary files.

Full `run/lint`, `run/check-links` and `git diff --check` pass after formatting; the link check reports 0 bad of 6,509 across 271 files. Final review checks prose spacing, complete wire literals outside tables, retained filename/scene/audio/JPEG contracts and explicit source-versus-completion boundaries. Publication and stable handoff follow.

Pre-publication checks pass: full `run/lint`, subsequent changed-file lint, `run/check-links` (**0 bad of 6,512 across 271 files**), diff check and the independent source/arithmetic vectors above. #166 body/update/zero comments remain unchanged. The all-author check at **03:57:35Z** found no newer issue; next about **04:07:35Z**. No task temporary files or original/client/UI/provider execution.

## PR delivery

[PR #167](https://github.com/bbs-land/remote-imaging-protocol/pull/167), specification **85abf203bfde92da5ceb191775e1dec318fa55b6**, is OPEN/non-draft and closes exactly #166. Actual source branch is `issue-166-hcl-bitmap-presentation` from stable 328f819. Merge its full source head including this delivery metadata with an identical-tree proof, then audit coverage/feedback/ancestry. The task-owned `rip-166-pr-` request directory was automatically removed. No upstream merge or manual issue closure.
