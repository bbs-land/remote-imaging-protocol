# Issue #264 — Palette, environment and text-window switch flags

[Issue #264](https://github.com/bbs-land/remote-imaging-protocol/issues/264) · [Shared switch contract](../ripscrip/7.1-data-tables.md#table-switch-wire-fields-and-protection) · [Research backlog](issue-backlog.md)

RealDeuce filed #264 at 22:18:21 UTC on 2026-11-24. The 22:18 UTC in-thread check found it after #263. Branch `issue-264-table-switch-flags` starts at fork stable `69b16ed06ff8bf733e56aeaa8a2ce2db0b76980d`. [Issue #2](https://github.com/bbs-land/remote-imaging-protocol/issues/2) identifies the family descriptor widths. The [STYLE derivation](issue-79-switch-style-flags.md) and [text-window selection derivation](issue-106-text-window-selection.md) establish the existing STYLE behavior and portable TW first use and ANSI state.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The 40-byte descriptors start at RVA `0x80820`, signed widths at offset 20 and flags at offset 38. Addresses below are RVAs at image base `0x10000000`.

| Slot | Opcode | Handler   | Protection helper | Selector  |
| ---- | ------ | --------- | ----------------- | --------- |
| 111  | `2A`   | `0x46c64` | `0x443c3`         | `0x442c3` |
| 114  | `2E`   | `0x46da0` | `0x3d8d0`         | `0x3dac0` |
| 119  | `2T`   | `0x46ece` | `0x2753a`         | `0x0e0dc` |
| 121  | `2Y`   | `0x46e41` | `0x390e4`         | `0x394fe` |

Each descriptor declares widths 1 and 2 and flags `0x0003`. The low flag pair selects active Base Math. Both arguments are required. The [ordinary numeric collector](issue-237-fixed-numeric-dispatch.md) finalizes a shortened nonempty final flags field at the delimiter and ignores surplus after a complete command. A [leading period](issue-240-period-zero-field.md) completes either field with zero. A period inside a field discards the unfinished command.

## Ordered operations

Each handler reaches the graphics option test before table writes. The unsigned limit check of 36 follows at `0x46c8a` for PAL, `0x46dc6` for ENV, `0x46ef4` for TW and `0x46e67` for STYLE. PAL, ENV and TW call the ordinary error reporter on the out-of-range branch. STYLE exits directly.

| Operation                    | PAL call  | ENV call  | TW call   |
| ---------------------------- | --------- | --------- | --------- |
| Bit 4, protect old current   | `0x46cb3` | `0x46dec` | `0x46f1a` |
| Bit 8, unprotect old current | `0x46cc8` | `0x46dfe` | `0x46f2c` |
| Select target                | `0x46cd2` | `0x46e08` | `0x46f36` |
| Bit 1, protect new current   | `0x46ce7` | `0x46e1a` | `0x46f48` |
| Bit 2, unprotect new current | `0x46cfc` | `0x46e2c` | `0x46f5a` |

The protection calls pass current-entry sentinel −1. The slot-zero checks run at `0x443e5–0x44400` (PAL), `0x3d8f1–0x3d90c` (ENV) and `0x27551–0x2756e` (TW). Protecting zero reports its normal error. The switch handler continues after the helper call. Unprotecting zero leaves it unprotected. Selection defaults or activates the target under its table rules before the destination requests. In the portable TW profile, this means copying the departing logical window into an undefined target with initial protection clear, then applying bits 1 and 2. Connection-owned partial ANSI input follows the existing TW policy. Numeric collection completes in the old Base Math before ENV selection.

## Staged captures

The original report feeds `!|`, the body, then CR separately. The body feed produces no dispatch. The same suffix cases apply to all four commands. With suffixes `4`, `40`, `4. note`, `401`, `405 note`, `4057`, `.0F` and `40.`, the results on CR are respectively discard, `(4, 0)`, `(4, 0)`, `(4, 1)`, `(4, 5)`, `(4, 5)`, `(0, 15)` and discard under either base. Suffix `a05` discards in base 36 and dispatches `(36, 5)` in base 64, where all four handlers reject the target before applying the flags.

With different defined nonzero slots, flags 12 clear source protection after requesting protection, and flags 3 clear destination protection after requesting protection. With the same slot and flags 9, source unprotect precedes destination protect, leaving it protected. Flags 6 leave the same slot unprotected. Slot zero stays unprotected across both requests with flags 15. The protection calls still occur in the original order.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 111
run/riptel-inspect "$dll" --slot 114
run/riptel-inspect "$dll" --slot 119
run/riptel-inspect "$dll" --slot 121
objdump -d --start-address=0x10046c64 --stop-address=0x10046f66 "$dll"
```
