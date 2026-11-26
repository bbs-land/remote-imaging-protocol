# Issue #311 — Positioned variable prompt anchor and bounds

[Issue #311](https://github.com/bbs-land/remote-imaging-protocol/issues/311) · [Positioned variable prompts](../ripscrip/5.1-text-variables.md#positioned-variable-prompts) · [Research backlog](issue-backlog.md)

RealDeuce filed #311 on 2026-11-25 at 21:59:11 UTC. The 22:06 UTC check found it with no comments. Branch `issue-311-prompt-dialog-placement` starts at fork stable `546c3531776617daa82732030aec32ad5b5a5af5`.

ALPHA 4 §4.2.1.8 supplies decimal WORLD coordinates and independent centering when a field is omitted. It leaves the unpositioned expression to client placement. The original RIPtel help reiterates the decimal and centering description at help file offset `0x47256` and shows the single-axis forms. The anchor, fitting rule and explicit numeric boundary need an implementation contract. [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) · [Help extraction](riptel-help-extraction.md).

## Original coordinate path

Static inspection used PC Micro RIPtel 3.1's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Driver addresses are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md).

- The user-expression parser initializes record fields +0x361 and +0x365 to DWORD `0xffff` at `0x1566b` and `0x1567b`. Omitted-axis branches preserve or write the same value. The supplied digit runs call the decimal helper `0x68c10` via `0x15d08` and `0x15d5c`, with direct DWORD stores at `0x15d10` and `0x15d64`. The decimal spelling `65535` therefore becomes the same stored value as omission.
- Projection helper `0xea02`, called by the provider at `0xeb6b`, substitutes `-1` for `0xffff` in each field. It calls `0x31084` separately for positive X at `0xea4a` and positive Y at `0xea5d`. Explicit zero stays zero. The converter uses per-axis DEVICE × coordinate divided by WORLD, reading X extents from ENV state +0x2c/+0x24 and Y extents from +0x30/+0x28. The unsigned divides at `0x3109c` and `0x310b1` do not provide a zero-extent placement rule.
- Dialog initialization gets the outer dimensions with GetWindowRect at `0x18881`, obtains the parent at `0x1889b` and its client rectangle at `0x188a2), and maps that client origin to screen space with ClientToScreen at `0x188d2`. The position branches at `0x188d8–0x18911` add supplied offsets to the client origin. Each omitted-axis branch halves the difference between the client and dialog extents before adding the origin.
- The fit branches at `0x18911–0x1894a` adjust an overflowing coordinate to the client's far edge minus the dialog extent minus one. The following guards at `0x1894e–0x18958` floor negative screen coordinates at zero. MoveWindow at `0x18966` receives the resulting top-left and measured size. The APIs establish the measurement and coordinate spaces. [GetWindowRect](https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-getwindowrect) · [GetClientRect](https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-getclientrect) · [ClientToScreen](https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-clienttoscreen) · [MoveWindow](https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-movewindow).

## Portable completion

Use the documented distinction between omitted and supplied axes. The 0 through 65535 range and checked conversion follow the established portable pick-list profile. Explicit `65535` is the upper numeric value, and omission alone is the wire centering syntax. An axis whose WORLD extent is zero defaults to centering. The drawing area's top-left is the coordinate origin. The clamp fits the measured rectangle up to the exclusive far edge. A larger dialog pins at the affected axis origin with its controls kept accessible. [Pick-list placement](../ripscrip/5.4-popup-lists.md#position-prefix-and-placement).

The reference vectors check origin anchoring, independent centering with odd free space, asymmetric axis projection, both far edges, the maximum coordinate and zero WORLD extent. Placement applies when the existing directive and value rules cause a dialog to open. [Variable directive rules](issue-87-user-variable-directives.md).

```sh
sha256sum "$dll"
objdump -p "$dll"
objdump -d --start-address=0x10015cfc --stop-address=0x10015d95 "$dll"
objdump -d --start-address=0x1000ea02 --stop-address=0x1000ea68 "$dll"
objdump -d --start-address=0x10031084 --stop-address=0x100310b8 "$dll"
objdump -d --start-address=0x1001887c --stop-address=0x1001896c "$dll"
```
