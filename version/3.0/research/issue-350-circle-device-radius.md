# Issue #350 — RIP_CIRCLE device radius

[Issue #350](https://github.com/bbs-land/remote-imaging-protocol/issues/350) asks how the outline circle's WORLD radius relates to a device pixel grid and to a physical display. RealDeuce filed it at 15:10:31 UTC on 2026-11-26. The 15:16 UTC check confirmed the report with no comments. Branch `issue-350-circle-device-radius` starts at fork stable `980e9a0927efd8345ef871ea45423642840550e5`. [Research backlog](issue-backlog.md).

## Source language

- [RIPScript 1.54](../../1.54/text/RIPScrip-1.54.txt), lines 1114–1139, SHA-256 `1ff57d53fb81c06604bdb6d9eb35150c90cbe7d5acd7b5ead6a0758f3d957497`. The promise of physical roundness names the fixed 640×350 EGA mode and says that its aspect ratio is understood by the designer and terminal.
- [RIPScript 2.00a4](../../2.0/text/RIPScrip-2.0-alpha-4.txt), §3.4.1.5, SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. The text retains the circular-result wording, removes the EGA qualification and adds that the radius is horizontal for aspect calculations. Its [WORLD projection](../ripscrip/1.3-world-view-virtual-canvas.md#translating-world-to-device-coordinates) uses the two WORLD extents and the two device pixel counts. It gives no second calculation for the physical width or height of a displayed pixel.

## Original driver trace

Use `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` from the [RIPtel 3.1 artifact catalogue](../../../reference/rip-tools.md). The driver SHA-256 is `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8` and its PE image base is `0x10000000`. Static reads used `run/riptel-inspect` with `--slot 21` and LLVM objdump 19.1.7. For `objdump -d --no-show-raw-insn`, add that base to the RVAs when passing `--start-address` and `--stop-address`. The following traces the radius geometry before rasterization.

1. Dispatch slot 21 at RVA `0x80b68` recognizes level 0 `C` and decodes three XY-width fields for handler `0x1aa3e`. The handler copies the center and radius into three locals at `0x1aa48–0x1aa5c`.
2. The call to `0x31084` at `0x1aa83` supplies both center pointers. That helper reads device width and height from drawing metrics `+0x2c/+0x30` and WORLD width and height from `+0x24/+0x28`. Each present pointer receives its own axis's integer quotient of the coordinate times device extent divided by WORLD extent. At `0x1aa92` the handler passes the radius as X with a null Y pointer. The radius therefore uses only the X quotient.
3. For a positive converted radius, the handler calls `0x154bb` at `0x1ab1e`. The wrapper forwards to `0x15437` at `0x154e1`. The latter subtracts and adds the same converted radius on both axes in `0x1545c–0x154ae`. The wrapper's temporary stroke-bound adjustments cancel before returning the rectangle at `0x15568–0x155ac`. The four resulting coordinates go to `GDI32.dll!Ellipse` at `0x1ab66`, via import thunk RVA `0x96480`. The zero-radius path instead calls the point helper at `0x1ab06`.
4. The two-coordinate conversion changes the center, and the X-only conversion changes the one scalar shared by both radii. There is no physical aspect input in those calculations. The 3.0 contract records a circle in device pixel coordinates. This resolves the inherited language without inferring a physical display requirement from a fixed EGA mode or defining a Windows-specific edge raster.

## Worked case

With default MegaNum and XY width 2, receive `!|fHS9Q|C8W4V1O` followed by CR. The fields are WORLD (640,350), center (320,175), radius 60. Use a 640×400 device with a full viewport at origin (0,0). The converted Y center is `floor(175 × 400 / 350) = 200`. The X center remains 320. The radius conversion is `floor(60 × 640 / 640) = 60`, and the nominal bounds passed to the ellipse primitive are (260,140,380,260). This case assumes the initial one-pixel line thickness.

If that raster fills a 4:3 display, the physical pixel width divided by height is `(4/3) / (640/400) = 5/6`. For a horizontal radius of 60 pixels, the matching physical vertical length would be 50 rows. Projecting the same WORLD radius through the Y ratio instead would give `floor(60 × 400 / 350) = 68`. The three interpretations therefore have distinct vertical radii. RIPscrip sends an X-based radius, and the original 3.0 device geometry uses radius 60 on each axis.

[Updated command](../ripscrip/2.2-curves.md#rip_circle).
