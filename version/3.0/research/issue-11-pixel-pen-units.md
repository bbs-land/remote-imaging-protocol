# Issue #11: Pixel/point coordinates and pen-thickness units

[Research backlog](issue-backlog.md) · [Lines reference](../ripscrip/2.1-lines.md)

## Scope and source

Started 2026-11-16 for [issue #11](https://github.com/bbs-land/remote-imaging-protocol/issues/11), oldest RealDeuce issue without a complete resolution PR after #37 publication. Branch `issue-11-pixel-pen-units` starts at `068f84c` (delivered #10). Required answers: X versus j at world 1280×960/device 640×480, effects of port/viewport origins, thick=3 units under uniform and nonuniform conversion, and the inherited open-polyline N-vertices/N-1-segments wording. Exact edge/cap raster reproduction and marker behavior are outside this issue.

Use the unchanged RIPtel 3.1 / driver 3.0.7 DLL, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred base 0x10000000, with the established provenance and native isolation rules. Addresses below are RVAs. No GUI or desktop Wine.

## Initial static findings

Original dispatch: physical slot **70**, `X`, two XY arguments, handler **0x1e1d1**; slot **41**, `j`, two XY arguments, handler **0x1e2f8**; slot **14**, `=`, arguments `[1,1,4,2]`, handler **0x1cd15**. The current 3.0 POINT entry's proposed world-versus-device distinction is explicitly a hypothesis and must be replaced from actual code.

Both X and j call original world-to-device **0x31084** on their coordinates (at **0x1e218 / 0x1e33f**), then form a 1×1 rectangle by adding one to each converted coordinate for invalidation/placement and call viewport-offset helper **0x34231**. Thus both have world-coordinate input, not X-device versus j-world. The final drawing boundaries differ: X calls **SetPixel** at **0x1e2a8** using its converted coordinate variables and current context HDC; j runs **0x2551b** on the rectangle, creates a solid brush, then calls **FillRect** at **0x1e405**. Follow HDC origin/clip setup **0x343e4**, helper 0x2551b and the actual arguments before declaring full aliases or assigning port origin twice. Equal coordinate conversion does not alone prove identical write-mode/clipping behavior.

Line-style handler **0x1cd15** clamps thickness below 1 to 1 at **0x1cd5e–0x1cd64**. Importantly the first `off_draw` field is validated as **0..3** (not only documented 0/1). If its bit **2** is set (**0x1cdfd**), the thickness is converted through **0x31084 with only the X pointer**, then clamped to at least one (**0x1ce08–0x1ce1d**). Otherwise no coordinate conversion occurs. It stores only the **low byte** at graphics-style entry+0x1d (**0x1ce24–0x1ce35**), then zero-extends that stored byte and passes it as CreatePen width at **0x1ce7c–0x1ce83**. A blanket world- or device-thickness rule would miss this explicit flag and byte-width behavior. For the requested `=00000003` (first field 0), the apparent width is 3 device pixels; confirm the complete path natively, including nonuniform scaling and byte truncation, before publishing.

The first field's bit 0 still needs its consumer traced for off-pixel background policy; keep any new bit-2 explanation tied to original flag validation/conversion, not the existing draft's nonzero=background claim. No unrelated line-pattern/raster changes are needed.

The polyline sentence is a bounded inherited editorial error: an open path of N coordinate pairs has N-1 consecutive segments; the existing five-sided/six-vertex example already agrees. Locate all Markdown copies, correct those and preserve original text. Polygon count encodings and limits are separately queued as issue #13.

## Original drawing-path derivation

The earlier description of **0x2551b** as a possible clip helper was wrong: it is a **thickness-based rectangle expander**. It reads the current graphics-style byte at +0x1d. When that byte `t` exceeds 1 it calls InflateRect with `floor(t/2)` in each axis; otherwise it leaves the rectangle unchanged. POINT therefore fills an odd square of side **2\*floor(t/2)+1**, centered at its converted, viewport-offset point. Even widths round to the next odd square (2→3, 4→5). PIXEL always calls SetPixel once and ignores this thickness expansion. This distinction alone disproves full aliasing.

Original **0x343e4** chooses the drawing HDC: an onscreen subport (type 1) uses the master screen port's HDC; an offscreen port (type >1) uses its own. It does not set a GDI coordinate origin. **0x342ad** copies the current viewport rectangle to transient clip state and calls **0x3fbf**, which creates/selects a clipping region from those coordinates. It does not change the HDC mapping/origin either. The DLL's 209 imported entries include GetMapMode, but no SetMapMode, SetViewportOrg, SetWindowOrg or transform-setting API. **0x34319** restores the port-bound clip after drawing. Native fixtures use normal zero-origin DCs and capture both the selected HDC and the active rectangle at the moment of drawing.

The X handler computes a viewport-offset rectangle **only for invalidation** (0x1e220–0x1e252), then passes the **unoffset** converted x/y variables to SetPixel at 0x1e2a8. Thus with stored viewport origin `O`, actual drawing coordinate is **D(world_xy)**, not **O+D(world_xy)**. Clipping still uses the shifted viewport. With O=(65,65), input (2,2), world 1280×960 and device 640×480, the attempted pixel is (1,1), which is outside that clip and draws nothing. Invalidation nevertheless names the offset rectangle. This is a shipped origin bug, not device-coordinate wire input.

The j handler offsets its actual draw rectangle through 0x34231 and expands that rectangle through 0x2551b before FillRect. At thickness 1 it draws the pixel at **O+D(world_xy)**; at larger stored widths it fills the odd square described above. Both handlers use the same original world conversion 0x31084; with origin zero and thickness 1 both plot input (2,2) at device (1,1). Drawing coordinates are in the chosen screen HDC or offscreen bitmap; do not add the screen port's origin a second time when it is already included in the stored viewport.

Microsoft's primary API documentation explains the controlled boundary: [SetPixel](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setpixel) rejects a coordinate outside the clip; [FillRect](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-fillrect) excludes right/bottom and fills with the supplied brush; [CreatePen](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createpen) treats width 0 as a one-device-pixel pen. These API references explain the fixture and ordinary DC contract; the original instructions, not current Windows documentation, prove RIPtel's decisions.

### Viewport producer and screen-port origin

Following the actual producer matters: level-0 `v` handler **0x20f87** calls **0x33cc6**. The latter converts both endpoints, normalizes their order (0x2530b), adds one to right/bottom for inclusive endpoint input, and stores that rectangle directly at entry+0x3a. It **does not add an onscreen port's origin**, and it does not intersect with port bounds. Newly initialized onscreen ports begin with a viewport copied from their absolute screen bounds (as traced in issue #10); defining a viewport later replaces it with directly converted coordinates. Thus the draft's universal port-relative viewport-input explanation does not describe this shipped producer.

The native fixture starts an onscreen port at screen (25,25), then executes original `v` arguments `(80,80,118,118)` under 2:1 world/device conversion. Stored viewport is **[40,40,60,60)**, not [65,65,85,85). Original POINT(2,2), thickness 1 then fills **[41,41,42,42)**; original PIXEL attempts (1,1) and is clipped. This is distinct from a synthetic stored viewport O=(65,65), which POINT correctly offsets to (66,66). The public contract must identify stored origins and the producer's missing port-origin addition rather than repeating the intended coordinate composition as client behavior.

### Exact width/flag handling

For first field `f` and decoded thickness `w`, original handler behavior is: reject `f>3`; clamp w to at least 1; if `f&2`, convert **only X** by `floor(w*device_width/world_width)` and clamp again to at least 1; store the low byte; pass its zero-extension to CreatePen. The Y scale never enters this conversion. The fixed thickness field remains two numeric characters regardless of XY width, and its radix follows current base math; native descriptor/radix checks execute 0x39de0/0x39d70 using original dispatch slot 14.

For the requested first field 0 / thick 3, the pen request is **3 device pixels** both at world/device 1280×960→640×480 and at 640×350→640×480. With first field 2 the first case requests 1 after X scaling; the second still requests 3 despite its nonuniform Y scale. Thickness byte truncation is observable: unscaled 256 stores/passes 0 (GDI's cosmetic one-pixel pen), 257 stores/passes 1, 4095 stores/passes 255. POINT uses that stored byte, so stored zero still gives its unexpanded 1×1 rectangle.

The off-pixel background state is **not** simply first-field bit 0: original 0x1cec0 compares the entire first field to **1**. Exactly 1 sets graphics-style+0x21 bit 4; 0,2,3 clear it. Thus field 3 permits X-scaled thickness but does not retain field 1's background-off-pixel setting. This state decision is tested; exact line-pattern raster reproduction remains outside the issue.

## Reproducible native checkpoint

New [`run/riptel-lines-probe`](../../../run/riptel-lines-probe) / [source](../../../run/lib/riptel-lines-probe.c), catalogued in run/README and deno.json, passes **165,026 checks**. Reproduce `run/riptel-lines-probe /path/to/unchanged/ripscrip.dll`; the wrapper fingerprints the original DLL and compiles native i386 with `cc -m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`. This run used FreeBSD 15.1 amd64, clang 19.1.7 and Capstone 5.0.7. Code RVAs map to original file offsets by subtracting 0xc00. Primary bytes are never rewritten; only private mapped instruction/import slots receive controlled-service hooks.

Original X, j, LINE_STYLE, world conversion, stored viewport offset, DC selection, clip setup/restore, POINT expansion and ROP-selection instructions execute. Controlled services replace Win32 rectangle/region/pixel/fill/pen/brush/object calls; region and clipping capture explicitly model standard rectangle intersection, not original Windows rasterization. UI/context synchronization, caret hide/show, style locking, protection checks and palette-to-RGB lookup are controlled; exact hook/import lists are in `main()`. The fixture does **not** hook the tested coordinate, origin or width decisions. No GUI or font is loaded.

Placement cases cover three frames (2:1 reduction, 640×350 compatibility world to 640×480 device, X-only reduction), onscreen/offscreen HDC selection, zero/shifted viewport origins and widths 1/2/3/4/255/256. The full first-field 0..3 × thickness 0..4095 domain is checked under five uniform/nonuniform scale profiles, with independent arithmetic for conversion/clamp/byte storage and captured CreatePen width. A field-4 case verifies rejection. Descriptor checks vary XY width 1..4 and base math 36/64. The real viewport producer is exercised separately as described above.

The first attempt to add the viewport-producer fixture faulted because the native context lacked its options pointer at +0x36; the original handler checks options+0x104. Supplying a zeroed options block fixed the fixture. This was a harness construction error, not an original-client crash or a reason to omit that path.

## Next action

The issue's coordinate/thickness questions are derived, including the unexpected X origin defect, thickness-sensitive POINT, viewport producer origin behavior and optional X-scaled width flag. Publish these exact client behaviors in the 3.0 Lines chapter, qualify related viewport/port-coordinate prose, and distinguish corrected generic implementation policy from historical defects. Correct the open-polyline N/N-1 sentence in the 1.54/2.0/3.0 Markdown editions (2.0 errata, 3.0 evidence); preserve original text. Format, run required repository checks and file one complete PR dependent on #37 with an issue-only comparison from 068f84c. Then record delivery and refresh GitHub before issue #12.

---

[Research backlog](issue-backlog.md) · [Lines reference](../ripscrip/2.1-lines.md)
