# Issue #212 — PolyMarker projection and composition

[GitHub issue #212](https://github.com/bbs-land/remote-imaging-protocol/issues/212) · [earlier source report #2](https://github.com/bbs-land/remote-imaging-protocol/issues/2) · [RIP_POLY_MARKER](../ripscrip/2.1-lines.md#rip_marker)

## Scope and checkpoint

RealDeuce filed the report at 2026-11-24 01:43:46 UTC. The report had no comments at intake. The original RIPtel 3.1 DLL SHA-256 is `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8` and its image base is `0x10000000`. Issue #2 is an earlier source report that establishes the name, wire widths, bounds and locations of the contour data. It covers additional topics independently.

## Wire and contour evidence

Dispatch slot 12 selects `0x1e4ff` and field types XY, XY, 2, XY, XY, 2, 2. The diagnostic at `0x7d994` reads `RIP_PolyMarker()`. The handler compares the three scalar values with 36, 360 and 3 at `0x1e52f`, `0x1e550` and `0x1e574`. Center and size both call `0x31084`, which multiplies by each device extent then divides by its matching world extent. The viewport addition applies to the center only. Size zero becomes one on each axis. For type zero the handler shifts each full size right once and sends the two radii and 0–360 span to `0x10160`.

The descriptor table at `0x7ca48` contains 36 six-byte pairs of little-endian count and absolute 32-bit point address. The first count is zero because the ellipse path constructs its own vertices. Each other descriptor addresses little-endian signed int32 X/Y pairs. The extracted data records all 462 vertices and hashes the source segments. The consumer at `0xf3c6` computes each scale from the projected dimension times the double constants 0.01 and 16384 at `0x76168/0x76170` and converts to an integer with `0x69fb0`. It shifts signed scaled vertices before rotating with the existing separate tables. The final rotation dot products are summed before their signed shift. The published arithmetic uses exact rational scales and widened intermediates for ordinary nonnegative world geometry.

## Selection and color policy

The Polygon import is at `0x9641c`, SetBkMode at `0x963f0`, SelectObject at `0x963c0` and SetROP2 at `0x96408`. Marker dispatch at `0x1e683` uses the low flag bit and then bit value 2. The pen selector `0x13cfc`, null brush `0x13d10`, fill brush `0x13d27` and null pen `0x13ce5` route drawing as shown below. The line background-mode bit is STYLE byte `0x21` mask 4. There is no N-bit test in these decisions.

| Flag and gap style | Original Polygon calls |
| --- | --- |
| 0 or 2 | Current pen, null brush. The line flag selects opaque or transparent background mode. |
| 3 | Null pen, current brush, opaque background mode. |
| 1, transparent line gaps | Null pen/current brush in opaque background mode at `0x1e74f`; current pen/null brush in transparent background mode at `0x1e779`. |
| 1, opaque line gaps | Current pen/current brush in opaque background mode at `0x1e779`. |

The handler leaves the selected drawing context's ROP2 intact. The W mode mapping and installation use `0xe6b3` and `0xe719`. SetROP2 applies to both pens and interiors. These facts support using W for each requested marker contribution. The separate and combined color policies in the portable contract identify which operation applies at a conceptual overlap. The single opaque-pass source selection is an explicit completion of the combined case for implementors. [Microsoft SetROP2 documentation](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setrop2), [Microsoft polygon overview](https://learn.microsoft.com/en-us/windows/win32/gdi/about-polygons)

The read-only verification compared all 462 vertices and both 360-entry Q14 arrays with the DLL, evaluated four projected points, checked all four flag selections and verified all 15 color vectors.

## Verification

Verified dispatch width, bounds, all 462 original contour vertices, shared Q14 arrays, geometry vectors, native GDI selections and portable color samples. Checked Markdown, links and whitespace.

## Resume

Extract the descriptor data, verify rotation and per-flag drawing routes, define portable composition and check independent vectors before delivery.

## Delivery

[PR #223](https://github.com/bbs-land/remote-imaging-protocol/pull/223) is open for upstream review. Specification commit e5ac4206ec67998bb48bc7c00c35975f3c274f20.
