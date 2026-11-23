# Issue #164: RIP_SCROLL coordinates, clipping and source fill

[Research backlog](issue-backlog.md) · [RIP_SCROLL](../ripscrip/4.0-icons-and-bitmaps.md#rip_scroll)

## Scope and checkpoint

Started 2026-11-23T03:08Z on `issue-164-scroll-contract` from fork stable **d7b49fcb929b98ec8efa792496f985d6d55971fd**, after #161 / PR #163 was merged with an identical source tree. [#164](https://github.com/bbs-land/remote-imaging-protocol/issues/164) created/updated **2026-11-23T03:01:22Z**, zero comments, body SHA-256 `288cc05edd2e5608a930c724e2b4d460027caefd67525fe4268060673962306c`. Complete coordinate decoding/origin, inclusive normalization/alignment, same-line timing, coupled port clipping, exposed-source fill/corner sampling, physical ANSI/blink-plane ownership and pattern phase. Preserve the RIP 1.54 contract and accepted port/pattern/copy-pair rules. Protocol-only; no original instruction/client/UI/provider execution or bug investigation. Last all-author check 03:07:43Z, next about 03:17:43Z.

## Starting reference hashes

Reproduce with `git show d7b49fcb929b98ec8efa792496f985d6d55971fd:<path>`:

- `version/3.0/ripscrip/4.0-icons-and-bitmaps.md`: SHA-256 `0fbc0bbd84611e1e83e8f62d90590f5fb8d7754d10917e7a678b5be39f77df3f`.
- `version/3.0/ripscrip/9.0-command-reference.md`: SHA-256 `48b6edf08667da5857acf1b606ebba6d29377b8f44f996d40249a159014a0b0e`.
- `version/3.0/ripscrip/7.0-drawing-ports.md`: SHA-256 `bb4b9cb71da592d89def383efbbce2f096b43556e0a366f1bc04bab2c538db1a`.
- `version/3.0/ripscrip/2.3-shapes-and-fills.md`: SHA-256 `31ff9ee4d72339e03c438be93c9155542385b0a3fd9fd8e594ad1db9dd3b311b`.
- `version/3.0/research/issue-12-fill-masks.md`: SHA-256 `54e785f1b6270024e4cc7b0bed0343591f49412d54fdb0716daa8cc147763f7c`.
- `version/3.0/research/issue-144-remembered-port-copy-pairs.md`: SHA-256 `7d993df415dab411c9d30b0ef80bd17cecc240d6b94a68473ee1ac4776532656`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

## Fingerprinted source and initial observations

Catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred image base **0x10000000**. LLVM objdump **19.1.7**, static disassembly only; `run/riptel-inspect` fingerprints and maps original bytes without execution. Addresses are **RVAs**, not file offsets. Reproduce disassembly with numeric VA = image base + RVA and `llvm-objdump19 -d --start-address=... --stop-address=...`.

The driver table at **0x80820**, 129 records of 40 bytes, has level 1 G at **slot 95**: handler **0xd7e0**, seven values, types **[255,255,255,255,1,1,255]**, flags **0x3**. Extract with `run/riptel-inspect <original DLL> --rva 0x80820 --length 5160`. Type 255 is current XY width, including the destination; the inherited fixed `dest_line:2` is inconsistent with this record.

Handler **0xd7e0–0xdc57** normalizes X and Y independently via **0x3112e**, converts both source corners and destination Y with **0x31084**, with no viewport-origin addition, and compares the converted destination with normalized converted source top before drawing. The zero reserved field adds one to the SetRect right/bottom boundaries at **0xd883–0xd88a**; **0xe72a** only copies a RECT. No eight-pixel alignment appears in this handler. Bounded helper/clip/consumer reads remain to establish exact geometry, sampled corner and mode 4 state; physical ANSI/blink-plane completion must be distinguished from the original single-DC evidence.

The ALPHA 4 entry at preserved text lines 10199–10263 documents vertical overlap, no horizontal movement, viewport bypass but drawing-port clipping, and the seven fill meanings. It does not settle the requested clipping/corner edge cases. Do not silently replace its original fixed-width draft record.

## Completed bounded source reading

Reproduce the following ranges with the objdump command above. `llvm-readobj19 --coff-imports <original DLL>` independently maps the relevant IAT slots: **0x96618 SetRect**, **0x96690 IsRectEmpty**, **0x96610 OffsetRect**, **0x963b8 GetPixel**, **0x963e4 BitBlt**, **0x96418 CreateSolidBrush**, **0x9667c FillRect**.

| Original RVA | Observation and inference limit |
| --- | --- |
| 0xd81c–0xd839 / 0x3112e–0x31144 | Normalize unsigned X and Y endpoint pairs independently before conversion |
| 0xd83c–0xd86a / 0x31084–0x310b7 | Convert both source corners and destination Y with device/world dimensions. No port/viewport origin is added; screen coordinates are shared-surface absolute, bitmap coordinates local |
| 0xd86d–0xd89d | SetRect from converted corners; **0xd883–0xd88a adds one to right/bottom when res=0**. A nonzero reserved field skips that addition; it is not adopted as a new portable extension. 0xe72a merely copies a RECT and does not establish inclusivity |
| 0xd89f–0xd8db | Compare destination against converted normalized top before drawing; reject equality or unsigned mode >6 |
| 0xd8e0–0xd907 | Copy S to a destination rectangle and offset by signed low-word destination-minus-top. No byte/eight-pixel alignment operation. Adopt ordinary wide signed geometry rather than low-word wrap as a protocol feature |
| 0xd910 / 0x343e4–0x3445a | Select current drawing DC: screen types use shared master screen DC; bitmap types use their own. No viewport-origin relocation in the scroll handler |
| 0xd919–0xd97d | Build old-source strip and sample before copy. Raw up sample is **(R-2,B-2)** and raw down sample **(L+1,T+1)**; raw failed GetPixel becomes 0x01000000. Those are inset implementation coordinates, not the ALPHA 4 named corners. Publish the draft's actual corner meaning and explicitly choose unavailable-corner RGB black; do not investigate/reproduce the client's edge behavior |
| 0xd97d–0xd9a6 | Same-DC BitBlt, source normalized origin, destination offset rectangle, ROP **0x00cc0020 SRCCOPY**. This proves vertical replacement without scaling, not a complete independently clipped source-access contract |
| 0xda14–0xdb38 | Modes 1/2/3 create solid brushes from current STYLE drawing/background/fill values at offsets 0/2/0x1a, resolved through 0x3dce0 |
| 0xdb39–0xdb56 | Mode 4 passes the current existing brush at object block +0x0c directly to FillRect. No per-strip brush reconstruction or phase reset; accepted #12 supplies mask colors/device-context phase |
| 0xdb5b–0xdba2 | Mode 5 constructs the fixed-black brush rather than consulting current STYLE background |
| 0xdba3–0xdc20 | Mode 6 creates a solid RGB brush from the pre-copy sample. All fill cases use FillRect; mode 0 skips fill. The old handler's private strip adjustments are not a proof of the portable set-difference edge contract |

Primary API context: [BitBlt](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-bitblt) defines SRCCOPY and explicitly clips only on the destination DC; it does not by itself prove source clipping. [GetPixel](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getpixel) returns an invalid value outside the selected clipping region. [FillRect](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-fillrect) excludes far boundaries and uses its supplied brush. These API facts explain the source evidence limits; no modern Windows/vintage palette raster parity is asserted.

## Protocol decisions and rejected interpretations

- Preserve ALPHA 4 intent: vertical overlapping copy, bypass viewport but obey current port, seven exposed-source modes and named true corners. Correct only current 3.0 destination width from the actual descriptor; do not change preserved text or the RIP 1.54 eight-pixel/legacy contract.
- **Explicit coupled clipping/fill completion:** A is full port bounds intersected with backing. C contains source pixels for which both source and translated destination are in A; D=C+(0,d); E=(S intersect A) minus D. This preserves each original row correspondence, rejects outside-port reads even on a shared screen, and fills still-available source when destination clipping eliminates all copying. It does not adopt private DC clip history or infer a new logical viewport from the copied area.
- **Explicit corner completion:** sample the true original normalized corner before any writing; an unavailable exact corner fills RGB black, without shifting to the clipped rectangle or native inset point. Do not call native inset behavior a normative bug fix.
- Patterned fill uses the accepted #12 device-context phase and background, not current W. Copy preserves painted pixels; a patterned destination does not retile source pixels. Both COPY and FillRect are replacement operations; no line pen/border or STYLE mutation.
- **Explicit physical/blink ownership:** include rendered ANSI pixels; apply independently to each supported blink phase, sampling that plane's own corner in mode 6. Original single-DC calls do not prove any two-plane legacy storage. Do not expose/move TW cells, history, margins or cursors, or add new rendering layers/buffers to the protocol.
- Preserve port/STYLE/ENV/PAL/TW selection and stored geometry/cursor/query/image/clipboard/remembered pair. #144's producer table is closed to its named operations; this pixel scroll is not another producer. Keep ordinary active/protected drawing gates. Snapshot preparation must precede writes; failure is a no-op, later rendering has no pixel rollback promise. These ownership/failure choices are explicit completion.
- No source-derived change is made to the distinct RIP_COPY_BLIT, its six modes or the existing port-copy compositor geometry. Its neighboring prose is not sufficient to generalize this scroll-specific clipping policy.

## Specification checkpoint

The current 3.0 command entry and index now publish dynamic destination width, surface-origin WORLD decoding, normalization/inclusive unit extents, converted same-line timing, coupled clipping/E fill, all modes, true-corner fallback, existing pattern phase, ANSI/blink-plane ownership and retained-state/failure boundaries. Exact unaligned/reversed/overlap/edge/no-copy/three-digit/pattern/two-plane vectors separate original evidence from explicit completion. The independent checks below pass; publication remains.

## Independent validation

At 2026-11-23T03:19Z an inline Python check passed all **six starting-file hashes**, the original DLL length/hash, slot 95's handler/types, **nine exact instruction-byte sequences** and **seven direct call targets**. The source reads stayed within the ranges above. Microsoft's [BitBlt](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-bitblt), [GetPixel](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getpixel) and [FillRect](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-fillrect) references were checked to interpret source COPY, unavailable reads and half-open brush coverage. They do not prove RIPtel's protocol port-edge or two-plane behavior.

Two independently expressed abstract geometries agree in **7,680 cases**: A in `[(2,2;7,8),(0,0;6,6),(3,1;5,4)]`; each supplied X endpoint in `[0,3,6,9]`, each Y endpoint in `[0,2,5,9]`; destination top in `[0,1,3,6,12]`; WORLD/device scale 1 or 2. Normalize WORLD endpoints, floor-divide by scale, add one for converted inclusive far positions and compare converted top before clipping. One model enumerates set membership; the independent interval model computes:

`C=[max(L,Ax0),min(R,Ax1)) × [max(T,Ay0,Ay0-d),min(B,Ay1,Ay1-d))`, where R/B are S's half-open far boundaries. Translate C by d for D; subtract D from the independent available-source intersection for E. Treat nonpositive interval extents as empty and d=0 as a pre-copy/pre-fill rejection.

All seven modes across both abstract pixel planes give **107,520 mode/plane cases**, checking row correspondence, disjoint D/E and unchanged pixels outside their union. Separate assertions reproduce the published fresh-image upward/downward results, reversed endpoints, one-row extent, true-corner colors, port-edge C/D/E, unavailable named-corner black, entirely lost destination fill, entirely unavailable source no-op, converted same-line rejection and mask `BFB` phase. All **eight full wire literals** decode independently with widths `[XY,XY,XY,XY,1,1,XY]` and exact length; the three-digit vector keeps three destination digits. No native instructions, renderer, GUI, allocator or provider ran; these are arithmetic/set/ownership checks of the adopted protocol and published examples.

The first Markdown validation caught and corrected one bad 1.54 link and a raw wire pipe inside a GFM table. Full wire literals now remain outside tables; the check runs after formatting to detect corruption. Originals/text/assets/1.54 remain unchanged.

## Publication status

Content complete; finish lint/link/diff and report refresh, then publish a PR closing exactly #164, record its actual full source head, normal-merge to fork stable with identical-tree proof and audit feedback/coverage/ancestry before continuing the watch. Last ten-minute all-author check **03:17:55Z** found no newer issue; next about **03:27:55Z**.

Full `run/lint`, `run/check-links` and `git diff --check` pass; links report **0 bad of 6,493 across 270 files**. Full diff review and exact prefix/suffix assertions preserve the unrelated commands and complete Masks/transparency section. Prose spacing and all eight full wire literals were reviewed after formatting. #164 remains unchanged at 03:01:22Z with zero comments before publication. No original instructions or UI/provider ran; no task temporary files were created.
