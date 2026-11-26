# Issue #301 — MVP and HEADER viewport maximize rectangle

[Issue #301](https://github.com/bbs-land/remote-imaging-protocol/issues/301) · [MVP](../ripscrip/5.1-text-variables.md#mvp) · [HEADER geometry and fill](../ripscrip/1.1-protocol-and-command-hierarchy.md#port-bounds-reset-and-black-fill)

RealDeuce filed #301 on 2026-11-25 at 16:41:22 UTC. The 16:45 UTC audit found it with no comments. Branch `issue-301-viewport-maximize-rectangle` starts at fork stable `0fd1dab8df5ea90ac0dc8109f724853680213290`.

ALPHA 4's MVP entry says the destination viewport becomes full screen while only the viewport definition changes. The #153 investigation established the shared leaf's defined, active and unprotected gates. It retained a full-port rule for HEADER pending identification of the system rectangle. [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) · [Earlier HEADER derivation](issue-153-header-port-clipping.md).

## Original callers and rectangle source

Static inspection used the PC Micro RIPtel 3.1 package's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md).

- MVP's callback at `0x2f4d4` calls `0x34067` at `0x2f4eb` for zero arguments using the current sentinel. For a list, it validates at `0x2f4fa` before dispatching each selector through `0x34067` at `0x2f534`. HEADER type 2 tests masks `0x800` and `0x1000` and calls the same helper at `0x17066` and `0x17079` with all and current sentinels respectively.
- The shared helper resolves current at `0x34076`, visits slots 0 through 35 for all, or range-checks a numeric target. Its leaf at `0x3400d` gates definition, activation and protection through `0x338bc`, `0x33fb0` and `0x33821`. At `0x34051–0x3405d` it copies four DWORDs from port subsystem +0x32 to the target's stored viewport at record +0x3a (stride `0x78`).
- The port initializer supplies the port subsystem +0x32 address as the output rectangle at `0x33ace–0x33ad8`. The USER32 import table starts at IAT RVA `0x9660c`. GetClientRect is the fifteenth entry, at `0x96644` as used by the call. Instructions at `0x33ae2–0x33af5` subtract the corresponding left and top components from right and bottom and pass the widths to the screen initializer.
- GetClientRect supplies a rectangle at origin (0,0) with exclusive right and bottom equal to the client area's pixel size. The shared source therefore represents the RIP screen canvas in DEVICE coordinates. Its storage is independent of the target port. [GetClientRect reference](https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-getclientrect).

## Specification and discriminating vector

Use the full screen canvas rectangle for both MVP and HEADER 2048/4096. This replaces #153's provisional full-port rectangle for those flags. The shared operation copies the same rectangle for screen and bitmap ports. Eligibility and ordinary drawing/query conversion follow the existing rules. Live RESET(VIEW) and the HEADER full-port black fill use their separately specified port extents.

With an 800 × 600 screen canvas, let active unprotected screen port 5 have DEVICE bounds `[100,80,140,110)` and active unprotected bitmap port 9 have backing 4 × 3. Apply `$MVP(5,9)$` or HEADER all geometry `!|h01000001200W00` independently. Both viewports become `[0,0,800,600)`. DEVICE viewport width and height queries give 800 and 600 for each. The bitmap storage remains 4 × 3, so raster writes are limited to its existing pixels. Screen-port storage is still the shared screen canvas.

```sh
sha256sum "$dll"
objdump -p "$dll"
objdump -d --start-address=0x10033abc --stop-address=0x10033afa "$dll"
objdump -d --start-address=0x1003400d --stop-address=0x100340bd "$dll"
objdump -d --start-address=0x1002f4d4 --stop-address=0x1002f548 "$dll"
objdump -d --start-address=0x1001705b --stop-address=0x10017081 "$dll"
```
