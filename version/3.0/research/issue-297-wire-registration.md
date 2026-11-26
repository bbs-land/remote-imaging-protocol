# Issue #297 — Level 9 client registration

[Issue #297](https://github.com/bbs-land/remote-imaging-protocol/issues/297) · [Client registration](../ripscrip/5.5-file-transfer-and-queries.md#client-registration-9r) · [Research backlog](issue-backlog.md)

RealDeuce filed #297 at 16:26:59 UTC on 2026-11-25. The catch-up check at 16:37:08 UTC found it with no comments. Branch `issue-297-wire-registration` starts at fork stable `59127006a7d9ab78491dd4d4f04f67a4aa785389`.

The [PC Micro package](../../../reference/rip-tools.md) supplies `extracted/ripscrip.dll` with SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Its matching unpacked `riptel-unpacked.exe` has SHA-256 `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. Driver addresses are RVAs at base `0x10000000`. Client addresses are VAs at base `0x400000`. The check used read-only instructions and the reported source observations.

## Collector and callback boundary

- Descriptor slot 127 at RVA `0x81bf8` has level key 9, opcode R, count 3, widths 4/2/8, and flags `0x000f`. Low bits `11` use active Base Math. The other bits select text and mark the final numeric field reserved.
- Full-width digits or leading periods complete the three fields. The eight-digit reserve path stores zero at `0x3a8da`. The delimiter check at `0x3a09d–0x3a0a1` rejects an incomplete text-bearing prefix. State 5 at `0x3a3d4–0x3a3fb` routes the ordinary delimiters to dispatch even with an empty tail. The original collector also has a historical numeric `<` branch. It can finish a short field and preserve its buffered value, including when it bypasses the normal reserved-digit store. The portable forms use full-width fields or a leading period. [Shared reserve collector](issue-245-image-reserved-fields.md#three-parser-paths) · [Historical field terminator](issue-281-header-numeric-shorthand.md#numeric-collection-and-selector-boundaries).
- Handler `0x252f2` passes the context, count, decoded argument array and text to `ripRegistration`. The wrapper at `0x32faa–0x32fc0` calls slot 73, offset `+0x124`, when installed. On a valid instance with an empty slot, it returns. There is no wrapper-level mode or text test and no macro expansion before the callback.
- The standalone EXE installs callback VA `0x405120` at slot `0x49` in the setup sequence at `0x409217–0x409224`. The callback checks client state and count 3, then handles modes 0 through 3. The mode 3 route can post the client transmit message `0x4c8`. This distinguishes the installed service from the silent empty-slot route. The portable command adopts that empty-slot boundary and preserves ordinary delimiter recovery.

```sh
sha256sum "$dll" "$exe"
run/riptel-inspect "$dll" --slot 127
objdump -d --start-address=0x100252f2 --stop-address=0x1002530b "$dll"
objdump -d --start-address=0x10032f92 --stop-address=0x10032fdb "$dll"
objdump -d --start-address=0x409217 --stop-address=0x409227 "$exe"
```
