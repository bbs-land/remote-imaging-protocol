# Issue #253 — GOTOXY and TEXT_WINDOW wire widths

[Issue #253](https://github.com/bbs-land/remote-imaging-protocol/issues/253) · [RIP_GOTOXY](../ripscrip/1.4-terminal-ansi-view.md#rip_gotoxy) · [RIP_TEXT_WINDOW](../ripscrip/1.4-terminal-ansi-view.md#rip_text_window) · [Research backlog](issue-backlog.md)

RealDeuce filed #253 at 21:04:14 UTC on 2026-11-24. The 21:08 UTC in-thread check found it with no comments. Branch `issue-253-text-window-wire-widths` starts at fork stable `00ed401f14e1e254fe2e76878a0ceca0f15bf61c`. The report identifies swapped width rules in the chapter and the command index. Prior [ordinary text-window](issue-100-ordinary-text-window-state.md) and [extended text-window](issue-101-extended-text-window-contract.md) derivations cover placement, units and state.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The dispatch table starts at RVA `0x80820` with a stride of 40 bytes. The signed field widths start at record offset 20 and flags at offset 38.

| Slot | Opcode | Signed widths                 | Flags    |
| ---- | ------ | ----------------------------- | -------- |
| 20   | `b`    | −1, −1, −1, −1, 2, 2, 1, 4, 3 | `0x000b` |
| 30   | `g`    | −1, −1                        | `0x0003` |
| 69   | `w`    | 2, 2, 2, 2, 1, 1              | `0x0003` |

The width helper at `0x39de0–0x39e00` reads the signed descriptor at record offset 20 plus the field index. A nonnegative descriptor returns directly. For −1, it returns the current XY width byte at parser offset `0x39`. The [radix selector](issue-250-meganum-receive-alphabet.md#validation-before-conversion) uses active Base Math for all three low flag pairs `11`. Thus `g` reads two XY-wide fields, `w` reads four fixed two-digit fields then two one-digit fields, and the four extended `b` corners use XY width. The width helper operates before the commands receive their text-cell or rectangle arguments.

## Staged captures

The report supplies original parser captures at the dispatch boundary. Feed `!|` and one body, then CR in a separate feed. In all cases the first feed produces zero dispatches. The arguments captured on CR precede any handler validation.

| Body              | Base Math | XY width | Arguments at CR               |
| ----------------- | --------- | -------- | ----------------------------- |
| `g0102`           | 36        | 2        | GOTOXY(1, 2)                  |
| `g0102`           | 36        | 3        | GOTOXY(36, 2)                 |
| `g0102`           | 64        | 3        | GOTOXY(64, 2)                 |
| `g001002`         | 36        | 3        | GOTOXY(1, 2)                  |
| `g00102`          | 36        | 3        | GOTOXY(1, 2)                  |
| `g.02`            | 36        | 3        | GOTOXY(0, 2)                  |
| `g0102tail`       | 36        | 3        | No dispatch                   |
| `w0000030310`     | 36        | 2 or 3   | TEXT_WINDOW(0, 0, 3, 3, 1, 0) |
| `w00000000300310` | 36        | 3        | TEXT_WINDOW(0, 0, 0, 0, 3, 0) |
| `w....10`         | 36        | 3        | TEXT_WINDOW(0, 0, 0, 0, 1, 0) |

At XY width 3, `g0102` collects `010` and short Y `2`. The digit sequence `010` is one radix unit, hence 36 or 64. `g0102tail` encounters the invalid MegaNum `t` while collecting the unfinished Y. In `w00000000300310`, `00 00 00 00 3 0` completes the declared fields. The surplus `0310` follows [the fixed-field delimiter wait](issue-237-fixed-numeric-dispatch.md). Each period in `w....10` takes the [whole zero-field branch](issue-240-period-zero-field.md), so each advances to another corner component.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 20
run/riptel-inspect "$dll" --slot 30
run/riptel-inspect "$dll" --slot 69
objdump -d --start-address=0x10039de0 --stop-address=0x10039e01 "$dll"
```
