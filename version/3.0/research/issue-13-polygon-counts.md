# Issue #13: Polygon wire counts and multi-contour limits

[Research backlog](issue-backlog.md) · [Polygon reference](../ripscrip/2.3-shapes-and-fills.md#rip_polygon) · [Polyline reference](../ripscrip/2.1-lines.md#rip_polyline)

## Scope and source

Started 2026-11-16 after [PR #40](https://github.com/bbs-land/remote-imaging-protocol/pull/40) delivered #12. GitHub creation-order refresh confirms [issue #13](https://github.com/bbs-land/remote-imaging-protocol/issues/13) is the oldest RealDeuce issue without a complete resolution PR. Branch `issue-13-polygon-counts` starts at **2754e2d**. Required answers: actual count width/radix and maximum valid prefixes for l/P/p; whether 4096 means storage or closure; multi-contour coordinate widths, aggregate versus per-contour vertex limits and maximum contour count.

Use unchanged RIPtel 3.1 / driver 3.0.7 `ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base 0x10000000, from the [catalogued original installer](../../../reference/rip-tools.md). Artifact mirror is outside this checkout, under `artifacts/riptel-3.1-pcmicro/`. All addresses below are RVAs. No GUI is needed; preserve original text and assets. All new commits have Codex co-authorship.

## Initial original dispatch and static observations

| Physical slot | Command | Handler RVA | Signed descriptor count | Width descriptors |
| --- | --- | --- | --- | --- |
| 45 | l | 0x1ed28 | -2 | 2, XY |
| 52 | P | 0x1eb03 | -2 | 2, XY |
| 53 | p | 0x1bc78 | -2 | 2, XY |
| 13 | < | 0x1e80a | -4 | 2, 2, XY, XY |

The original width helper **0x39de0** returns the literal descriptor width or, for signed byte -1, parser+0x39 coordinate size. The variable-argument numeric paths **0x3a819–0x3a82f / 0x3a996–0x3a9ac** update width only while the descriptor index is below the absolute descriptor count; later arguments retain the last width. Thus all l/P/p coordinates use XY. An important lead for <: its first two numerals are fixed width 2, then **every remaining numeral retains XY width**, potentially including later contour counts. Verify this through the original parser before publishing a simplified repeated `nverts:2` grammar.

The numeric accumulator stores/increments its argument count and compares with **0x2001 (8193)** at **0x3a800 / 0x3a97d**, then enters state 8. Pending questions: whether 8193 is accepted, how excess input is discarded and whether dispatch still occurs. The numeric field's own two-character range is separately 1295 in base 36 or 4095 in base 64; no special 4096 representation has been found.

Original P/l prologues reject only a one-argument input or an odd coordinate count, then use args[0] as the GDI vertex count. Their conversion loops initialize remaining count to **N+1** at **0x1ebca–0x1ebcf / 0x1edef–0x1edf4** and read that many pairs, but pass **N** to Polygon/Polyline at **0x1ec99 / 0x1eebe**. Filled p repeats the same pattern at **0x1bd2d–0x1bd41**. This is a possible extra-read defect, not evidence that an implicit closing point occupies the published Nth vertex. Native fixtures must provide trailing readable sentinel slots and capture the GDI count rather than treating stale data as an intentional closure. Do not run a count beyond the verified scratch extent.

Multi-contour < uses a counts array at **0x89610** and points at **0x8b610**; the distance is 8192 bytes / 2048 count words. It requires at least two vertices per contour (**0x1e8a7**), accumulates all points into one buffer, checks supplied argument positions during traversal (**0x1e88e / 0x1e8d7**) and calls PolyPolygon with the declared number of contours and separate per-contour counts (**0x1ea72** on border-disabled path). No per-contour reallocation or independent 4096-point workspace appears. Need establish the aggregate parser budget, max contour count in each radix and all minimum/truncated cases natively.

## Next action

Add a hash-guarded native polygon probe using the original byte parser/reset and count/coordinate handlers with captured drawing services. Verify 2/3/4-digit coordinate settings, both radices, two-digit maximum count prefixes, the 8193-argument boundary, later-contour count widths, aggregate multi-contour capacity and extra sentinel conversion without rasterization. Keep unsafe excess/incomplete inputs on the parser-capture path only. Record the exact controlled hook boundary before specification edits or a resolution PR.

---

[Research backlog](issue-backlog.md) · [Polygon reference](../ripscrip/2.3-shapes-and-fills.md#rip_polygon)
