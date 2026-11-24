# Issue #258 — Simple mouse-field suffix and cursor reserve

[Issue #258](https://github.com/bbs-land/remote-imaging-protocol/issues/258) · [Mouse field](../ripscrip/4.2-mouse-fields-and-pointer.md#rip_mouse) · [Cursor](../ripscrip/4.2-mouse-fields-and-pointer.md#rip_set_mouse_cursor) · [Research backlog](issue-backlog.md)

RealDeuce filed #258 at 21:21:44 UTC on 2026-11-24. The 21:28 UTC in-thread check found #257 and #258. Branch `issue-258-mouse-field-suffix` starts at fork stable `c114c52f5e767f192b13259e5e1792f92dce9695` after the earlier-numbered #257. The [post-action reset derivation](issue-227-mouse-post-action-reset.md) establishes `clr`. The [cursor mapping derivation](issue-125-cursor-number-mapping.md) establishes cursor values and state ownership.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The 40-byte command records start at RVA `0x80820`. Signed widths begin at offset 20 and flags at offset 38. Addresses below are RVAs at image base `0x10000000`.

| Raw slot | Opcode | Numeric widths                | Flags    |
| -------- | ------ | ----------------------------- | -------- |
| 90       | `1c`   | 2, 4                          | `0x000b` |
| 101      | `1M`   | 2, XY, XY, XY, XY, 1, 1, 2, 3 | `0x000f` |

Both low flag pairs select current Base Math. Bit `0x08` normalizes the last field to zero. `1M` has text bit `0x04`, so its final reserve must finish before state 5 begins host text. `1c` has no text bit. It uses the delimiter branch that supplies zero for a missing or short trailing reserve, then waits for the delimiter when fully collected. The cursor handler at `0xdc96` loads argument 0 only. The [period derivation](issue-240-period-zero-field.md) explains why an initial period advances a whole numeric field and an interior period discards an unfinished command.

## Hotkey and default bits

The simple-field constructor at `0xcef8` loads operand index 7 at `0xcf2b`. After checking nonempty text at `0xd007–0xd017`, it rejects a value above 255 at `0xd04b–0xd062`. The error literal at `0x7aa58` names the hotkey. A nonzero value stores to field record byte `+0x2b` at `0xd1ca–0xd1d5`. The constructor tests `clk` bits with values 1, 2 and 4 at `0xd1a3–0xd1bf`, storing masks `0x01`, `0x20` and `0x40` in record flags at `+0x20`. The next branch stores the existing `clr` mask `0x02` independently.

The shared matcher at `0x556f` scans the live field array backward with stride `0xa3`, without checking the simple-field/button discriminator. At `0x55c0–0x55c9` it reads `+0x2b` and accepts 255 as wildcard. It checks CR against record flag `0x20` at `0x55cb–0x55d4` and Escape against `0x40` at `0x55d6–0x55df`. Otherwise `0x55e1–0x55e6` compares the stored hotkey to the uppercased input byte. The event path calls the matcher at `0x50f5` and dispatches the winner at `0x5116`. The simple-field branch at `0x2c485` calls `0x2c5c0`, which applies temporary feedback and invokes the common host action at `0x2c5ee`.

The original scan establishes which fields and bytes participate. The portable profile extends the [published button keyboard rules](../ripscrip/4.3-buttons.md#keyboard-activation) to simple fields, with explicit byte matches before an eligible CR or Escape default, then wildcard. Use a single shared ordering across instances. The existing input locks, focused Enter/Escape precedence and FIELDID snapshot rules apply. Simple fields use their original `clr` command flag through the already specified post-action reset policy.

## Staged captures

The report feeds `!|`, the body and CR separately with XY width 2. No body feed dispatches. The table gives collected arguments on CR before the handlers execute.

| Body | Base Math | Arguments at CR |
| --- | --- | --- |
| `1M...1E1E....click` | 36 | (0, 0, 0, 50, 50, 0, 0, 0, 0), `click` |
| `1M...1E1E..0F00Zclick` | 36 | (0, 0, 0, 50, 50, 0, 0, 15, 0), `click` |
| `1M...1E1E...click` | 36 | No dispatch |
| `1M...1E1E...click` | 64 | (0, 0, 0, 78, 78, 0, 0, 0, 0), `ck` |
| `1c6`, `1c0601`, `1c06.comment` | 36 | (6, 0) |
| `1c060.` | 36 | No dispatch |

The base-64 row uses `1E` = 64 + 14. Its `cli` completes the three-digit reserve, which normalizes to zero. The base-36 version rejects its first lowercase character while still in numeric state. In `1c06.comment`, the leading period completes the reserve. The following bytes go through the fixed numeric command's surplus state.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 90
run/riptel-inspect "$dll" --slot 101
objdump -d --start-address=0x1000d1a3 --stop-address=0x1000d1d6 "$dll"
objdump -d --start-address=0x1000556f --stop-address=0x100055f9 "$dll"
objdump -d --start-address=0x1002c5c0 --stop-address=0x1002c5f9 "$dll"
```
