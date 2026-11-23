# Issue #184 - RIP_COPY_BLIT geometry and exposed-source fill

[Research backlog](issue-backlog.md) · [RIP_COPY_BLIT](../ripscrip/4.0-icons-and-bitmaps.md#rip_copy_blit) · [Issue #184](https://github.com/bbs-land/remote-imaging-protocol/issues/184)

RealDeuce filed the report at **2026-11-23T19:09:23Z**. The body SHA-256 is `b182b6f457c483c62612b1130af8a3206444137cc422c3665937c6049ea67af7`. The report asks for dynamic XY fields, full-port paired clipping, fill of the available old source minus written destinations, and rendered ANSI pixel ownership. The [earlier example correction](https://github.com/bbs-land/remote-imaging-protocol/pull/37) supplies the same corpus literal. This work starts on `issue-184-copy-blit` from fork stable **1b1b194443ac6dfc577d33f7a82f04d412a37d81**. The **19:25:33Z** GitHub audit found #184 alone uncovered and no review, discussion or inline feedback in the overlap.

## Documentary basis

[ALPHA 4 `3.4.2.4](../../2.0/text/RIPScrip-2.0-alpha-4.txt) assigns current Base Math, Modes 0–5, `Uses Viewport: NO`and`Uses Port: YES`. Its description preserves the copied destination and fills old source graphics which are still exposed. `5.7.2 gives the separate deactivation rule for later graphics commands. The [common syntax](../ripscrip/1.1-protocol-and-command-hierarchy.md#ripscrip-protocol---syntax-and-general-structure) defines the optional final reserved digit as zero when omitted. The [port-copy producer table](../ripscrip/7.0-drawing-ports.md#remembered-copy-pairs) gives the existing pair lifetime. The [scroll derivation](issue-164-scroll-contract.md) provides the applicable precedent for paired full-port clipping, already rendered ANSI pixels, the fill pattern phase and blink planes. Its special converted same-line rejection is opcode-specific.

Starting SHA-256 fingerprints can be reproduced with `git show 1b1b194443ac6dfc577d33f7a82f04d412a37d81:<path>`.

- `version/3.0/ripscrip/4.0-icons-and-bitmaps.md`: `f742493e2821582434fbc17c4d379a141d3c27c9ec9d77e42651750ea54690c0`.
- `version/3.0/ripscrip/9.0-command-reference.md`: `fcb1bcbabf3990cfdb55ec5adf1f91ca40019aa2d4fbefa5941a0fc5d377e83e`.
- `version/3.0/ripscrip/1.1-protocol-and-command-hierarchy.md`: `1bdd96ef2e3af28d433a1329b4949aa14b7b19d1ac3ad9b1e657e0b676291c2a`.
- `version/3.0/ripscrip/7.0-drawing-ports.md`: `bb4b9cb71da592d89def383efbbce2f096b43556e0a366f1bc04bab2c538db1a`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

## Bounded original evidence

Use the original RIPtel 3.1 PC Micro `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` from the [external catalogue](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. All addresses are RVAs. The preferred image base is `0x10000000`. With `DLL` set to this artifact, the LLVM 19.1.7 command line uses preferred addresses.

```sh
sha256 -q "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1000b7a4 --stop-address=0x1000bd3b "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10031084 --stop-address=0x10031145 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x100343e4 --stop-address=0x1003445b "$DLL"
llvm-objdump19 -p "$DLL"
```

- Read physical dispatch slot 96 at `0x81720` (table `0x80820`, 40-byte stride). Its opcode is level 1 `g`, callback `0xb7a4`, count 8, types `FF FF FF FF FF FF 01 01` and flags `0x000b`. The last field is one numeric digit. The handler reads the six coordinates and the mode at argument `+0x18` with no read of `+0x1c`. Its upper bound check at `0xb7e3–0xb7e6` rejects decoded modes above 5. The normative acceptance of an omitted reserved digit comes from general wire syntax.
- `0xb81f` and `0xb82f` call the same endpoint ordering helper `0x3112e` for X and Y. `0xb840`, `0xb851` and `0xb862` call `0x31084` for each corner and the destination anchor. That helper multiplies and divides each axis. This call chain adds neither a port origin nor a viewport origin. `0xb873` and `0xb877` increment the converted source spans. The copy call at `0xb8c2` uses both dimensions, raster operation `0x00cc0020` (SRCCOPY) and the same `context+0x62` DC for both sides. The path has no equality test on the converted origins.
- The setup call `0xb80c → 0x13cfc` selects the pen through IAT `0x963c0` (`SelectObject`). It does not install a clip. Both fill arms call `0x343e4` at `0xb9be` and `0xbb5d`. The helper selects the current screen or offscreen drawing context. These calls alone do not establish which pixels a portable compositor can read and write or a two-plane contract.
- `0xb8f0` calls `IntersectRect` at IAT `0x9668c`. The overlap arm computes source minus intersection with `CombineRgn`, mode 4 (`RGN_DIFF`), at `0xb985` and fills through `FillRgn`. The disjoint arm uses `FillRect` at `0xbcf0` for mode 5, with the stock object 4. The report identifies the inclusive far values passed to the historical rectangle functions. The protocol uses the inclusive source coverage in both phases and preserves all actual destination pixels. No replication of the historical fill boundaries is needed.
- For overlap, the three solid-brush branches start at `0xb9f9`, `0xba4b` and `0xba9e`. Modes 1, 2 and 3 use draw, background and fill color. Mode 4 takes the brush at `context+0x3a → +0xc`. Mode 5 uses the stock black brush. Match these to the ALPHA 4 mode table, independently of backend presentation. Resolve import names by walking PE import descriptors and matching `FirstThunk + 4 * index` to the IAT RVA. Translate source RVAs to file offsets with `PointerToRawData + RVA − VirtualAddress` inside their sections.

## Portable result

Retain the deactivation gate and restrict both accesses and fill to the physical intersection of the full current port and its backing. Work in the shared screen coordinate system for screen ports and in the bitmap coordinate system for offscreen ports. The viewport rectangle contributes neither an origin nor a bound. Protected active ports remain eligible for content drawing.

Let `S` be the normalized inclusive source represented as a half-open device rectangle, `A` the available bounds and `t` the converted anchor minus the converted source origin. The copied source is `C=S ∩ A ∩ (A−t)`, its written destination is `D=C+t`, and the exposed old source is `E=(S ∩ A)∖D`. Copy the pre-command pixels of `C`, then apply the selected mode only to `E`. If the converted translation is zero, `D=S∩A` and `E` is empty. A lost destination leaves the available old source exposed. Both endpoints of a copied pixel pair must be valid. This avoids filling a copied pixel or leaving a nominal but unwritten overlap intact.

Apply the same coverage to each rendered blink plane, reading its own pre-command pixels, preserving terminal cells and history, cursors, port definition and remembered pairs. Use the existing device-anchored pattern phase. These compositor choices extend the published intent and the shared [RIP_SCROLL](../ripscrip/4.0-icons-and-bitmaps.md#rip_scroll) rules to this opcode. No original client or drawing function was run.

## Resume

Apply the grammar and geometry in the `1g` entry and command index, validate literal and pixel examples, and file a PR closing #184. Consolidate its actual source and refresh the ordered queue.
