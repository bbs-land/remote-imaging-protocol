# Issue #307 — Drawing write-mode operands on mixed surfaces

[Issue #307](https://github.com/bbs-land/remote-imaging-protocol/issues/307) · [3.0 drawing operands](../techspecs/2.3-drawing-composition.md#drawing-color-operands-on-mixed-surfaces) · [Research backlog](issue-backlog.md)

RealDeuce filed #307 on 2026-11-25 at 19:24:14 UTC. The 19:26 UTC check found it with no comments. Branch `issue-307-drawing-mixed-color-operands` starts at fork stable `12f7e4d236ca3b5d6e8f457c7b2f52456ed0c2ac`.

ALPHA 4 §3.4.1.57 defines XOR, OR, AND and destination complement on covered drawing pixels. PIXEL's Write Mode attribute in §3.4.1.38 makes the equations apply to X. ALPHA 4 §2.7.1 discusses resolving the drawing palette for RGB framebuffers. Neither clause gives an output representation for an operation with one drawing index and one literal RGB operand. [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) · [X/j drawing contract](issues-74-76-drawing-contract.md).

## Original surfaces and color operations

Static inspection used PC Micro RIPtel 3.1's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md).

- W handler `0x2102c` admits modes 0–4 and stores the mode at current STYLE +4 at `0x2109a` for an eligible style. The mode converter at `0xe6b3` gives SetROP2 values 13, 7, 15, 9 and 6 in mode order. The installer at `0xe719` applies that mix to the selected drawing HDC from context +0x62. SetROP2 combines the pen or brush with the surface's current pixel and R2_NOT inverts the existing surface color. PIXEL's documented Write Mode attribute establishes the required operation regardless of which backend primitive plots its sample. [SetROP2 reference](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setrop2) · [Primitive scope](issues-74-76-drawing-contract.md).
- The port HDC selector at `0x343e4` takes its screen HDC from the first port record +0x6c at `0x34450`. The bitmap branch at `0x3444b` takes the selected record's HDC. Both store the chosen handle at context +0x62 at `0x34453`. The ANSI cell painter loads the same screen HDC via context +0x22, port record array and first record +0x6c at `0xde77–0xde8b`. It sends that handle as the destination for the paint call at `0xdf69`. This establishes a shared screen paint destination. [Port selection trace](issue-11-pixel-pen-units.md).
- PIXEL resolves its source through `0x3dce0` at `0x1e296`. That helper has branches for native palette-index COLORREFs and for building an RGB value from the selected palette, including the table lookup at `0x3dd70–0x3dd84`. This distinguishes the native surface/color setup from the logical operand representation. The original host calls supply evidence of the selected mix and sharing of the screen canvas. Native raster output across supported display formats was outside the investigation.

## Portable composition

Extend the existing two-domain drawing equations with the promotion rule already used for binary PUT_IMAGE and PORT_COPY. For two actual RIP drawing indices use index arithmetic and retain an index. An RGB operand requires three eight-bit channels, with any RIP drawing index resolved through the selected published drawing palette at command start. A literal RGB result stays literal. W04 takes only the destination and uses its domain. The screen input is the current scene pixel, including ordinary terminal paint, while an offscreen input is its stored bitmap pixel. This treats altered scene pixels the same regardless of the renderer's private layer arrangement. The transfer operations retain their separate source-oriented mode 04. [PUT_IMAGE derivation](issue-231-put-image-color-domain.md) · [PORT_COPY derivation](issue-233-port-copy-color-domain.md).

Independent vectors use the report's `#A0B0C0` palette entry and `#112233` literal in both operand directions. A before/after palette edit distinguishes lookup at draw time and storage as RGB. Additional index-index, indexed inversion and two successive RGB inversions check that the result type follows the specified operation.

```sh
sha256sum "$dll"
objdump -d --start-address=0x1000e6b3 --stop-address=0x1000e72a "$dll"
objdump -d --start-address=0x100343e4 --stop-address=0x1003445b "$dll"
objdump -d --start-address=0x1000de30 --stop-address=0x1000df6f "$dll"
objdump -d --start-address=0x1001e252 --stop-address=0x1001e2ae "$dll"
objdump -d --start-address=0x1003dce0 --stop-address=0x1003dde8 "$dll"
```
