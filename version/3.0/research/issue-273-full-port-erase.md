# Issue #273 — Full-port black erase for E and EGW

[Issue #273](https://github.com/bbs-land/remote-imaging-protocol/issues/273) · [RIP_ERASE_VIEW](../ripscrip/1.3-world-view-virtual-canvas.md#rip_erase_view) · [EGW](../ripscrip/5.1-text-variables.md#egw) · [Research backlog](issue-backlog.md)

RealDeuce filed #273 at 23:05:54 UTC on 2026-11-24. The 23:08 UTC in-thread check found it with no comments. Branch `issue-273-full-port-erase` starts at fork stable `b52af23df0c0c54d3b9872128dda169580a2d1d9`. The [HEADER port derivation](issue-153-header-port-clipping.md) establishes the shared port fill path and the portable clipping policy. The older ERASE_VIEW entry describes the ALPHA 4 viewport/background semantics.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses below are RVAs at image base `0x10000000`. Physical table slot 25 has opcode `E`, no arguments, no text flag and handler `0x1ad6f`.

| Path | Original operation |
| --- | --- |
| Wire `E` | `0x1ad75` services pending work. `0x1ad7e` tests the current viewport gate. `0x1ad8e` calls `0x340bd` with −1 for current. |
| `$EGW$` with no arguments | `0x2f160` calls `0x340bd` with −1. |
| `$EGW(...)$` | `0x2f16f` prevalidates all selectors. A bad selector exits via `0x2f17c`. `0x2f19d–0x2f1a9` resolves and applies each valid selector in order. |
| Shared dispatcher | `0x340bd–0x34112` resolves −1 from the current index, loops 0–35 for −2, or forwards a numbered selector. |
| Shared leaf | `0x34125` tests defined, and `0x34138` tests active. There is no protection gate in the fill path. |

The EGW selector list uses the case-insensitive `ALL` comparison at `0x09980` and the shared current/numeric case at `0x098fc–0x09944`. The numeric range check is unsigned below 36. The macro leaves no response text.

## Original drawing requests and portable completion

The screen branch uses the four stored bounds at record offsets `+0x03`, `+0x07`, `+0x0b` and `+0x0f` to select a full-port region on the shared DC. At `0x341cc–0x341e1` it requests stock brush 4 and fills that rectangle. The bitmap branch at `0x34203–0x34216` issues the same full bounds and brush against the bitmap DC. It leaves that DC's inherited clipping to the backend. Both paths request invalidation for the filled rectangle. Brush 4 is `BLACK_BRUSH`. [Microsoft stock brush definition](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getstockobject).

The report observes the same call vectors for `E` and argumentless EGW in an active screen port with B = `[10, 18) × [20, 24)` and V = `[12, 15) × [21, 23)`, and a bitmap with B = `[0, 8) × [0, 4)` and V = `[2, 5) × [1, 3)`. Both fixtures leave WORLD at `0:0`. With `!|`, `E surplus` and CR fed separately, the one dispatch occurs at CR. Undefined and inactive targets yield zero fill calls. These captures establish the original requested rectangles and brushes.

Adopt the [existing HEADER black-fill policy](../ripscrip/1.1-protocol-and-command-hierarchy.md#port-bounds-reset-and-black-fill) at both additional entry points. Write opaque RGB black over B intersect physical backing. Screen targets overwrite shared pixels, including existing text in that rectangle. Bitmap targets update their own backing only. Treat the fill as a temporary scope and retain port definition and all metadata, including viewport, cursors, queries, image style and remembered copy pair. Palette edits after the fill leave its pixels black. This portable rule gives one consistent result without relying on inherited native DC clipping.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 25
objdump -d --start-address=0x1001ad6f --stop-address=0x1001ad97 "$dll"
objdump -d --start-address=0x1002f149 --stop-address=0x1002f1be "$dll"
objdump -d --start-address=0x100340bd --stop-address=0x10034231 "$dll"
```
