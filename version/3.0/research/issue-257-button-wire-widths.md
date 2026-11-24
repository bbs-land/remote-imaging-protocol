# Issue #257 — Button wire widths and style selection flags

[Issue #257](https://github.com/bbs-land/remote-imaging-protocol/issues/257) · [Button style](../ripscrip/4.3-buttons.md#rip_button_style) · [Button](../ripscrip/4.3-buttons.md#rip_button) · [Switch button style](../ripscrip/4.3-buttons.md#rip_switch_button_style) · [Research backlog](issue-backlog.md)

RealDeuce filed #257 at 21:21:44 UTC on 2026-11-24. The 21:28 UTC in-thread check found #257 and #258. Branch `issue-257-button-wire-widths` starts at fork stable `e8ee456a300ef3da2b868e1943f9a8593cdd2265`. The [earlier button-style trace](issue-27-button-style-contract.md) establishes the handlers and style defaults. The [undefined-slot derivation](issue-112-undefined-button-style.md) establishes first-use selection.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The 40-byte records start at RVA `0x80820`. Signed widths start at offset 20 and flags are at offset 38. Addresses below are RVAs at image base `0x10000000`.

| Raw table slot | Opcode | Numeric widths | Flags |
| --- | --- | --- | --- |
| 87 | `1B` | XY, XY, 2, 4, ten fields of width 2, 1, 5 | `0x000b` |
| 107 | `1U` | XY, XY, XY, XY, 2, 1, 1 | `0x000f` |
| 112 | `2B` | 1, 2 | `0x0003` |

The raw index 112 corresponds to callback slot 111 in the report. These commands all use active Base Math because each low flag pair is `11`.

## Ordered style protection

The `2B` handler at `0x46d08` loads target and flags separately at `0x46d0f–0x46d11`. The usual pending-input service runs first. The same [global graphics gate as STYLE](issue-79-switch-style-flags.md#verified-instruction-sequence) reads option word `+0x104` through context `+0x36` and exits on nonzero. The unsigned test at `0x46d2e–0x46d31` rejects targets 36 and above before table changes. This branch calls the error reporter with the slot-number message at RVA `0x82c88`.

| RVA | Operation |
| --- | --- |
| `0x46d4a–0x46d59` | Bit 4, protect current slot. |
| `0x46d5c–0x46d6b` | Bit 8, unprotect current slot. |
| `0x46d6e–0x46d75` | Select target with `0x4576f`, which defaults an undefined slot before selection. |
| `0x46d78–0x46d87` | Bit 1, protect current slot after selection. |
| `0x46d8a–0x46d99` | Bit 2, unprotect current slot after selection. |

All protection calls use `0x454c4` with current-slot sentinel −1. It resolves that slot at `0x454e2`, leaves slot zero unprotected via `0x454e6–0x45503`, and checks the in-use byte at `0x45508` before writing protection for an ordinary current slot. No protection-helper return aborts the switch. If source and target match, the same record receives both pairs in order, with any required first-use initialization at selection.

## Staged captures and receive boundaries

The report feeds `!|`, command body and CR separately with Base Math 36 and XY width 2. Only the CR feed can dispatch. The sample `1B` prefix through corner color is `1B0000020074000C000F080200000F07`. Its leading 14 numeric values are `(0, 0, 2, 256, 0, 12, 0, 15, 8, 2, 0, 0, 15, 7)`.

| Body or suffix             | Arguments collected at CR            |
| -------------------------- | ------------------------------------ |
| `2B04`                     | SWITCH_BUTTON_STYLE(0, 4)            |
| `2B400`, `2B4.`            | SWITCH_BUTTON_STYLE(4, 0)            |
| `2B4`                      | No dispatch                          |
| `1B` prefix alone          | No dispatch                          |
| Prefix plus `0`            | `other_port` 0, reserve 0            |
| Prefix plus `..`           | `other_port` 0, reserve 0            |
| Prefix plus `F12AZ9 notes` | `other_port` 15, reserve 0           |
| `1U..1E1E.0Z<>label`       | `(0, 0, 50, 50, 0, 0, 0)`, `<>label` |

For `1B`, bit `0x08` allows an omitted final reserve at `0x3a0e6–0x3a102` or a valid shortened one before a delimiter. The preceding one-digit `other_port` must still complete. The handler at `0xb325` reads through operand index 13, corner color. After the final reserve fills, surplus bytes are ignored until dispatch. For `1U`, the last field is also normalized by bit `0x08`, but text bit `0x04` switches to text as soon as that field completes. An initial period follows the [shared zero-field rule](issue-240-period-zero-field.md); the [bitmap prefix derivation](issue-249-bitmap-prefixes-port-write.md) records the same reserve-to-text mechanism.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 87
run/riptel-inspect "$dll" --slot 107
run/riptel-inspect "$dll" --slot 112
objdump -d --start-address=0x10046d08 --stop-address=0x10046da0 "$dll"
objdump -d --start-address=0x100454c4 --stop-address=0x1004551b "$dll"
```
