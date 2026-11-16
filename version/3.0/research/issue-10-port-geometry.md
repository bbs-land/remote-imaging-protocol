# Issue #10: Port extents, copy clipping and viewport arithmetic

[Research backlog](issue-backlog.md) · [Drawing ports](../ripscrip/7.0-drawing-ports.md)

## Scope and primary source

Started 2026-11-16 for [issue #10](https://github.com/bbs-land/remote-imaging-protocol/issues/10), oldest RealDeuce issue without a complete resolution PR after #36 publication. Branch `issue-10-port-geometry` starts at `1a0bcc8` (delivered #9). Required answers: exact physical port dimensions and PORTW/PORTH results for `2P10000ZKQO00030000` at world 1280×960/device 640×480; precedence of all-zero destination versus zero lower-right verbatim copy; whether clipping preserves an initially unscaled copy, especially 4×1 into a 2×1 viewport at x=1; correct the inherited viewport diagram's screen coordinate 125 to 115. Exact Windows raster/filter reproduction is not requested.

Primary binary: unchanged RIPtel 3.1 driver 3.0.7 DLL, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred base 0x10000000, provenance in the [first issue note](issue-3-extended-font-style.md#artifact-recovery-2026-11-16). All binary addresses below are RVAs. No GUI probes; only static/native routines with controlled OS services.

## Initial static derivation

Dispatch physical slot **115**, level 2 `P`, has seven numeric fields `[1,XY,XY,XY,XY,4,4]` and handler **0x466ec**. It converts both corners with original world-to-device helper **0x31084**, subtracts lower minus upper at **0x467a0–0x467b5** with **no +1**, and calls original port initializer **0x3326f** at **0x467e1** with those differences as width/height. Offscreen initialization calls CreateCompatibleBitmap at **0x335f5** using those same widths/heights, sets origin to zero, bounds/viewport right and bottom to width/height, and stores width/height words at entry+0x13/+0x15 (**0x3370c–0x33753**). This strongly indicates a 640×480 allocation for the issue's example, not 641×481 or a physical 1280×960 bitmap. Execute the complete routine and actual query callbacks before publishing query results.

World conversion **0x31084** reads context+0x0e; state offsets +0x24/+0x28 are world width/height and +0x2c/+0x30 are device width/height. Each coordinate is multiplied by device extent and unsigned-divided by world extent. The 1280,960 endpoint therefore becomes 640,480 exactly.

Dispatch physical slot **113**, level 2 `C`, has twelve numeric fields and handler **0x46372**. After coordinate conversion it checks all four source coordinates for zero at **0x4656e–0x46591**, choosing the complete source viewport. It checks **all four destination coordinates first** at **0x465ba–0x465dd**, choosing the complete destination viewport. Only otherwise does it check both destination end coordinates for zero (**0x465df–0x46613**) and synthesize an end using the source rectangle's width/height. Neither branch establishes the final clipping behavior alone.

Original copy compositor **0x134d0** decides scaling by comparing **pre-trim** widths/heights at **0x13533–0x13555**. In the equal-size path it trims each rectangle via **0x13456**, passing the other rectangle as an adjustment target. In the scaling path it passes a null adjustment target and trims each independently. It then calls **0x136a9** (BitBlt, no scale) or **0x13763** (StretchBlt) without recomputing the branch. Trim helper **0x13456** clips right/bottom and subtracts the trimmed amounts from the counterpart's end when supplied. This indicates initial verbatim copies retain one-to-one dimensions under clipping; verify the concrete case natively before updating the inherited blanket clip-then-fit prose.

The viewport diagram arithmetic is independent: screen port origin (25,25) + port-relative viewport origin (40,40) + viewport point (50,50) = **(115,115)**. The point is (90,90) relative to the port. Both 2.0 and 3.0 Markdown copies contain 125; original text remains historical evidence and must be preserved.

## Next action

Add a native port routine probe, preserving original define-port initializer, coordinate conversion and copy compositor; capture only bitmap allocation and final GDI rectangle arguments. Derive actual PORTW/PORTH callbacks and their argument/domain conventions for exact answers, without taking over unrelated issue #25. Cover all-zero destination, zero-endpoint verbatim, equal/unequal explicit extents, clipping and the requested 4×1 case. Keep findings incremental and do not publish a resolution PR until all subquestions are complete.

---

[Research backlog](issue-backlog.md) · [Drawing ports](../ripscrip/7.0-drawing-ports.md)
