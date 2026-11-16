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

## Next action

Continue #11 only. Add/reuse native original-routine fixtures for X/j placement, viewport/HDC-origin setup, final drawing arguments and CreatePen width. Preserve original conversion rather than mocking it; cover requested 2:1 and 640×350-world/640×480-device frames, nonzero viewport/screen-port origins and offscreen origins. Trace j's 0x2551b clip helper and current-HDC transforms to settle coordinate ownership. Exercise thickness flag 0 versus bit 2 and boundary values for stored-byte truncation, without treating a fixture failure as original behavior. Derive completely, update specification/notes/AGENTS as needed, validate and file one complete PR dependent on #37 with an issue-only comparison from 068f84c.

---

[Research backlog](issue-backlog.md) · [Lines reference](../ripscrip/2.1-lines.md)
