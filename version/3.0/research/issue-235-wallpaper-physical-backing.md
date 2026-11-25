# Issue #235 — Wallpaper seed outside physical backing

[Issue #235](https://github.com/bbs-land/remote-imaging-protocol/issues/235) · [Wallpaper backing contract](../ripscrip/4.0-icons-and-bitmaps.md#wallpaper-physical-backing) · [Research backlog](issue-backlog.md)

## Evidence

RealDeuce filed #235 at 17:31:44 UTC on 2026-11-24. The 17:36 UTC in-thread check found the open report with no comments. Branch `issue-235-wallpaper-physical-backing` starts at fork stable `25ddd51e24c65b7d1977e118be4e76c7008520e3`.

[Issue #58](issue-58-load-bitmap-flags.md#native-matrix-and-wallpaper-geometry) establishes the viewport-edge trim, phase and source contents for flags 16 and 32. Its 1,400 controlled grids confirm the periodic geometry on backed seeds. The [VIEWPORT producer](issue-11-pixel-pen-units.md#viewport-producer-and-screen-port-origin) stores its rectangle without intersecting it with physical storage. [Issue #146](issue-146-physical-clip-pairs.md) preserves logical source correspondence under physical source clipping for named transfers. Wallpaper uses its own repeated COPY stage and does not publish a remembered pair.

The [PC Micro package](../../../reference/rip-tools.md) supplies original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. Addresses below are RVAs with image base `0x10000000`. File offsets for these instructions are RVA minus `0xc00`.

- `0x0ca99` tests flag 16 after the duplication path. At `0x0cab1–0x0cacd` the handler fetches the current port's stored viewport at record offsets `0x3a–0x46`. At `0x0cad3–0x0cae4` it selects flag 32 and passes the initial renderer rectangle and this viewport to wallpaper helper `0x3ea2e`.
- The helper checks current viewport activation at `0x3ea4e` and calls the rectangle reconciler at `0x3ea82`. The reconciler at `0x3f0a6` compares the two rectangles, trims the seed's right and bottom at `0x3f0c4–0x3f0d7`, then checks positive extents. At `0x3ead3–0x3eaeb` wallpaper derives width and height from differences of the resulting seed edges. The backing dimensions are absent from that calculation.
- At `0x3eacd` the helper sets ROP2 to COPY. It sends same-device BitBlt COPY requests through IAT `0x963e4`, beginning at `0x3eb37`. The native calls establish the periodic device geometry, while the [GDI BitBlt contract](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-bitblt) assigns clipping to the destination DC. The prior controlled service results do not establish the fate of missing source pixels on real Windows devices.

## Portable completion

The accepted geometry gives the dimensions and origin of the logical seed after viewport trim. Treat the physically backed subset as available samples at their original positions in the tile. Copy those pre-wallpaper pixels at the existing periodic offsets. Every unavailable sample skips its corresponding destination write. A wholly unbacked seed makes no wallpaper writes. This retains the established logical correspondence over physical bounds, without shrinking or recentering the tile or introducing a fill color for missing storage. A transparent pixel retained by the initial color-key pass is an available sample if the backing contains it. That sample copies like any other composited pixel.

Stagger uses the logical width, including when the physically backed subset has a different width. The seed row is row zero. For a one-pixel-high seed at Y=1, rows just above and below are both odd. In the report's example, W=4 retains offsets 0 and 1 as A and B, with offsets 2 and 3 absent. For X=0–5 the aligned row is A B G G A B. An odd row shifts by 2 and gives G G A B G G. A vertical counterpart retains the logical height, so alternate rows of a two-row tile whose second row is absent skip all writes. The backing checks concern both source samples and destination accesses. The earlier draw and duplication effects survive either partial or zero coverage.

```sh
sha256sum "$dll"
objdump -d --start-address=0x1000ca99 --stop-address=0x1000caec "$dll"
objdump -d --start-address=0x1003ea2e --stop-address=0x1003f0a6 "$dll"
objdump -d --start-address=0x1003f0a6 --stop-address=0x1003f12f "$dll"
```

---

[Research backlog](issue-backlog.md) · [Wallpaper backing contract](../ripscrip/4.0-icons-and-bitmaps.md#wallpaper-physical-backing)
