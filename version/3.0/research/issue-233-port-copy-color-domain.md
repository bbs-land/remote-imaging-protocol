# Issue #233 — PORT_COPY logical color domains

[Issue #233](https://github.com/bbs-land/remote-imaging-protocol/issues/233) · [PORT_COPY](../ripscrip/7.0-drawing-ports.md#rip_port_copy) · [Research backlog](issue-backlog.md)

## Sources

RealDeuce filed #233 at 17:24:32 UTC on 2026-11-24. The 17:25 UTC in-thread check found the report without comments. Branch `issue-233-port-copy-color-domain` starts at fork stable `ab4a5239a7e6c741605ff66228e404835c3c06c6`.

The [2.0 ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) at lines 10518–10614 specifies the explicit ports, fit when their extents differ, and five operation codes. Modes 1–3 combine the source and destination. Mode 4 inverts the source. The draft supplies no rule for different logical color types or indexed widths. [Issue #231](issue-231-put-image-color-domain.md) defines a logical color policy for PUT_IMAGE. [Issue #10](issue-10-port-geometry.md#native-allocation-and-copy-checkpoint) recovers the geometry and ROPs for both PORT_COPY branches. [Issue #144](issue-144-remembered-port-copy-pairs.md) and [issue #146](issue-146-physical-clip-pairs.md) specify remembered geometry and physical storage limits.

The catalogued [PC Micro RIPtel 3.1 package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. Addresses below are RVAs at image base `0x10000000`. For these `.text` addresses, subtract `0xc00` for file offsets.

At `0x466bb` the wire handler calls copy compositor `0x134d0`. Its pre-trim equal-size test at `0x13533–0x13555` selects the unscaled or fit branch. The calls at `0x13690` and `0x13689` select `0x136a9` and `0x13763` respectively. Both leaves select the same Boolean ROPs:

| Mode | GDI operation | ROP          | Unscaled / fit instruction |
| ---- | ------------- | ------------ | -------------------------- |
| 1    | SRCINVERT     | `0x00660046` | `0x136e0` / `0x1379a`      |
| 2    | SRCPAINT      | `0x00ee0086` | `0x136e7` / `0x137a1`      |
| 3    | SRCAND        | `0x008800c6` | `0x136ee` / `0x137a8`      |
| 4    | NOTSRCCOPY    | `0x00330008` | `0x136f5` / `0x137af`      |

The unscaled leaf calls GDI32 BitBlt at `0x13749` via IAT `0x963e4`. The fit leaf calls GDI32 StretchBlt at `0x1381a` via IAT `0x96488`, with both rectangle extents and the chosen ROP. Microsoft documents source conversion to the destination's surface format for [BitBlt](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-bitblt) and [StretchBlt](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchblt). The StretchBlt contract fits the source before combining it with the destination. These establish the native operation order and passing of the two surfaces. Exact logical drawing-index width and RGB promotion are portable choices.

## Portable completion

Use the same binary modes as [PUT_IMAGE](../ripscrip/4.0-icons-and-bitmaps.md#boolean-paste-color-domain). The destination supplies the live scene pixel or stored offscreen pixel. A fit supplies one logical source sample to each operation. An actual indexed sample stays indexed. A constant source remains the same color and type through fitting. If a filter forms a new RGB sample, use it as RGB. Index resolution for RGB uses the current published graphics palette at command start. This supplies deterministic Boolean results while retaining the existing filter freedom.

For mode 4, interpret a drawing index in the destination's logical index width before complementing it. This follows the destination-domain choice for indexed binary modes and the native order of conversion before the GDI operation. It does not require the host palette's quantization. The previous destination pixel has no part in unary inversion. In particular, `0x06` from a 4-bit source becomes `0xF9` in an 8-bit destination, including when it replaces a direct RGB pixel. Its inverse in the 4-bit source domain would have been `0x09`. A direct RGB sample instead complements all three RGB bytes and produces literal RGB. Geometry, clipping and remembered-pair publication retain their existing rules.

The [vectors](../ripscrip/7.0-drawing-ports.md#port_copy-color-examples) cover index pairs, both mixed directions, both RGB operands, fit over a constant indexed color, and source inversion across logical widths. The constant fit avoids choosing a sampler. All-zero rectangles select each fixture port's full viewport.

```sh
sha256sum "$dll"
objdump -p "$dll"
objdump -d --start-address=0x100466ad --stop-address=0x100466c3 "$dll"
objdump -d --start-address=0x10013533 --stop-address=0x100136a9 "$dll"
objdump -d --start-address=0x100136a9 --stop-address=0x10013834 "$dll"
```

---

[Research backlog](issue-backlog.md) · [PORT_COPY](../ripscrip/7.0-drawing-ports.md#rip_port_copy)
