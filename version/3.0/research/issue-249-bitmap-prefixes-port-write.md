# Issue #249 — Bitmap numeric prefixes and PORT_WRITE cleanup

[Issue #249](https://github.com/bbs-land/remote-imaging-protocol/issues/249) · [LOAD_ICON](../ripscrip/4.0-icons-and-bitmaps.md#rip_load_icon) · [PORT_WRITE](../ripscrip/7.0-drawing-ports.md#rip_port_write) · [Research backlog](issue-backlog.md)

RealDeuce filed #249 at 19:48:53 UTC on 2026-11-24. The 19:57 UTC in-thread check found it ahead of #250. Branch `issue-249-bitmap-prefixes-port-write` starts at fork stable `6c90bb636209e38687d7b51fb6ca3b23a35934db`. The report supplies staged parser captures and screen/offscreen writer observations at flags 0–3 with a RAM-only fixture. The [2.0 LOAD_ICON and LOAD_BITMAP reference](../../2.0/ripscrip/4.0-icons-and-bitmaps.md) preserves the inherited command spelling. [Issue #42](issue-42-clipboard-capture-state.md#load_icon-failure-retains-clipboard-state) establishes placeholder routing and state. [Issue #10](issue-10-port-geometry.md#post-pr-port_write-results) establishes the export geometry. [Issue #215](issue-215-write-icon-export.md) traces the same writer and its registry.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. Addresses below are RVAs at image base `0x10000000`. For `.text`, subtract `0xc00` for file offsets.

| Slot | Command | Numeric widths before filename |
| ---- | ------- | ------------------------------ |
| 97   | `1I`    | XY, XY, 1, 1, 1, 1, 1          |
| 88   | `1b`    | XY, XY, XY, XY, 1, 1, 2, 2, 4  |
| 120  | `2W`    | 1, XY, XY, XY, XY, 2, 2        |

All three descriptors have flags `0x000f`. Numeric fields use active Base Math. Descriptor bit `0x08` normalizes the last numeric field to zero. Bit `0x04` switches to text after that field completes. The [IMAGE collector](issue-245-image-reserved-fields.md#three-parser-paths) uses these same reserved and text branches. A one-byte period consumes one field and changes where subsequent fields, including the filename, begin.

## Leading LOAD_ICON digit

At `0xcb4f–0xcb65`, operand index 2 is saved separately from operand 3. The comparisons at `0xcd5a–0xcd95` select the ROP from operand 3. The render request at `0xcdde` receives that ROP and the requested icon anchor. There is no combination of the two digits into a single mode number. On ordinary lookup failure, `0xcd02–0xcd18` uses operand 2 as part of the far X of the placeholder rectangle, operand 3 as far Y, and the projected anchor as its near corner. The later render-failure call at `0xce12–0xce24` supplies zero far edges. The common marker helper tests for positive extents at `0x38570–0x3857b`. The nonpositive branch uses the marker's own dimensions at the anchor.

The portable rule emits zero in the leading field and consumes that field independently. For a missing or failed icon, an available marker uses its own device size at the anchor, with normal destination clipping. This gives one marker placement rule across lookup and render failures while preserving the existing state and clipboard behavior. The marker artwork and dimensions are implementation-defined.

## PORT_WRITE flags and registration

Operand index 5 is saved at `0x469b5–0x469c1`. At `0x46b76–0x46b8e` the caller passes scale zero and masks only bit value 1 for temporary registration, then calls writer `0x27a90`. The writer calls `0x18e92` at `0x27ab0` on that temporary path before export. The report's RAM-only observations are consistent with that argument routing. A crop covering (1,1) through exclusive (7,4) yields 6 × 3 on the screen source and 8 × 4 for the 8 × 4 offscreen backing under flags 0–3. Only 1 and 3 request registration.

For the portable profile, commit the file successfully before adding temporary ownership under the [shared lifetime rule](../ripscrip/6.2-asset-delivery-and-storage.md#temporary-resource-lifetime). On failure preserve the earlier resource and registry state. This adopts the same publication policy as WRITE_ICON. The historical call ordering establishes which bit requests registration.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 97
run/riptel-inspect "$dll" --slot 88
run/riptel-inspect "$dll" --slot 120
objdump -d --start-address=0x1000cb38 --stop-address=0x1000cef4 "$dll"
objdump -d --start-address=0x1003856a --stop-address=0x100385e0 "$dll"
objdump -d --start-address=0x1004699a --stop-address=0x10046bd8 "$dll"
```

---

[Research backlog](issue-backlog.md) · [PORT_WRITE](../ripscrip/7.0-drawing-ports.md#rip_port_write)
