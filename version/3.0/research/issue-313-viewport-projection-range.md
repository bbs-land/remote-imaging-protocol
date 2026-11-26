# Issue #313 — RIP_VIEWPORT projection range

[Issue #313](https://github.com/bbs-land/remote-imaging-protocol/issues/313) · [Viewport range](../ripscrip/1.3-world-view-virtual-canvas.md#viewport-projection-range) · [Research backlog](issue-backlog.md)

RealDeuce filed #313 on 2026-11-25 at 23:08:30 UTC. The 23:16 UTC check found it with no comments. Branch `issue-313-viewport-projection-range` starts at fork stable `c77fb9d4e648d9f9fdba8e22804af7ef40bb4bd3`.

ALPHA 4 §3.4.1.56 declares four XY fields, an inclusive far corner and all-zero deactivation. The accepted 3.0 viewport trace establishes direct projection and stores the viewport independently of the port origin and backing surface. The documents supply no policy for a projected rectangle outside the DEVICE representation. [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) · [Existing 3.0 viewport producer](issue-11-pixel-pen-units.md#viewport-producer-and-screen-port-origin) · [Physical clipping at consumer time](issue-146-physical-clip-pairs.md).

## Original conversion and stored representation

Static inspection used PC Micro RIPtel 3.1's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Driver addresses are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md).

- The level-0 handler `0x20f87` copies four decoded DWORDs. At `0x20ff1` and `0x21001` it calls `0x3112e` to order each pair of unsigned WORLD coordinates. At `0x2101f` it passes the corners to `0x33cc6`.
- The viewport helper calls `0x31084` at `0x33ce0` and `0x33cf1` to project the corners into DEVICE units. The scale uses DEVICE width and height from ENV state +0x2c/+0x30 and WORLD widths +0x24/+0x28. There are fixed-width integer products and unsigned divides. The arithmetic path does not supply a wide projection or a portable zero-extent policy.
- At `0x33d2f` the helper constructs a local RECT via SetRect (IAT `0x96618`). It calls the rectangle order helper `0x2530b` at `0x33d39`, adds one to right and bottom at `0x33d44` and `0x33d47`, and tests the rectangle with IsRectEmpty (IAT `0x96690`) at `0x33d4b`. A nonempty result copies all four DWORDs into the port record at +0x3a and marks it active. An empty result returns without that copy. The original APIs use signed 32-bit LONG for RECT members. [Win32 RECT](https://learn.microsoft.com/windows/win32/api/windef/ns-windef-rect) · [Win32 LONG](https://learn.microsoft.com/windows/win32/winprog/windows-data-types) · [IsRectEmpty](https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-isrectempty).

## Portable completion

Compute the bounds without intermediate narrowing and check all four stored values after making the far corner exclusive. A successful definition yields the full projected clip for later operations. Reject a definition with an unrepresentable boundary before publishing either geometry or the active bit. The signed range comes from the established rectangle representation, and the pre-publication validation is a portable specification rule. This ensures that an invalid rectangle cannot substitute a different origin, clip or scaling reference. A WORLD extent of zero supplies no mathematical projection for a new rectangle. All-zero deactivation remains a separate operation.

The five-digit UltraNum field for 4,194,304 is `0G000`. Multiplying by 640 at WORLD width 1 gives the inclusive DEVICE coordinate 2,684,354,560. Adding one yields an unrepresentable right boundary, so the previous right boundary 20 remains observable through PORTX1 in DEVICE units. The bounded arithmetic checks also cover the exclusive endpoint limit, a product wider than 32 bits with a representable quotient, reversed corners and ordinary clipping beyond physical storage. [Prior port query definitions](issue-25-port-query-grammar.md).

```sh
sha256sum "$dll"
objdump -p "$dll"
objdump -d --start-address=0x10020f87 --stop-address=0x1002102c "$dll"
objdump -d --start-address=0x10031084 --stop-address=0x100310b8 "$dll"
objdump -d --start-address=0x10033cc6 --stop-address=0x10033d82 "$dll"
```
