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

## Fingerprinted primary source and initial bounded observations

Catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base **0x10000000**. LLVM objdump/readobj **19.1.7** and existing `run/riptel-inspect` read only. All addresses are **RVAs**, not file offsets. Reproduce with `llvm-objdump19 -d --start-address=<base+RVA> --stop-address=<base+end> <original DLL>`. No temporary dump files or new probes.

#161 already establishes bitmap callback **0x259f7** → filename helper **0x26dd2** → **0x38910(context,name,1)**. The complete bitmap wrapper **0x38910–0x38bd1** checks raw-zero style **0x343b6**, inactive viewport **0x3445b**, and zero stored far boundaries before lookup. It normalizes the already adopted extensions and selects actual stored image area or current viewport via **0x34385** (all-minus-one means fallback). Both branches pass rectangle coordinates **unchanged**, fixed scale=1, palette mode=2, key=-1 and COPY **0x00cc0020** to ordinary BMP renderer **0x4a410** at **0x38b34**. Unlike JPEG common presentation, no far-edge subtraction appears in this caller.

The complete wrapper has **no image flag +0x5a read, aspect-fit call, initial fill or wallpaper call**. Its only post-render drawing is the existing failure placeholder, not successful-image wallpaper. Do not assume the JPEG flags or wire `1b` flags apply simply because the rectangle is shared. Renderer **0x4a410** calls existing BMP reader **0x4edc5**, geometry **0x49340**, palette activation only for mode 1, current-palette remapping **0x4ea60**, and the key-negative output path **0x4e42b**. Final geometry/palette/dither/clipping evidence and an explicit portable phase boundary remain to be completed.

## Next action

Finish bounded ordinary bitmap geometry/output/clip reads and source/policy distinction, publish exact target and flag applicability plus portable palette/phase vectors. Run meaningful independent arithmetic/source-byte checks and repository validation, open a separate PR closing #166, normal-merge its actual complete source head to stable with identical-tree proof, audit coverage/feedback/ancestry and continue the watch.
