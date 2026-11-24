# Issue #263 — Resource and query text boundaries

[Issue #263](https://github.com/bbs-land/remote-imaging-protocol/issues/263) · [Local scene playback](../ripscrip/5.5-file-transfer-and-queries.md#rip_read_scene) · [Host directory selection](../ripscrip/6.2-asset-delivery-and-storage.md#rip_switch_directory) · [Research backlog](issue-backlog.md)

RealDeuce filed #263 at 22:10:51 UTC on 2026-11-24. The 22:18 UTC in-thread check found it ahead of #264. Branch `issue-263-resource-query-text-boundaries` starts at fork stable `495426a5126308e633f3a3b05d806d83f2fe26ef`. The [scene playback derivation](issue-170-local-scene-boundaries.md) identifies both widths on `1R`; the earlier reference still displayed them as one field. This issue tests where the numeric collector hands off all five text tails.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses below are RVAs at image base `0x10000000`. The 40-byte descriptors start at RVA `0x80820`, signed widths at offset 20 and flags at offset 38.

| Physical slot | Opcode | Numeric widths | Handler RVA | Numeric operands used |
| --- | --- | --- | --- | --- |
| 85 | `1<ESC>` | 1, 1, 2 | `0xd3da` | Mode, plus window for numbered residents |
| 91 | `1D` | 3, 2 | `0xbd39` | Flags |
| 94 | `1F` | 2, 4 | `0xbde4` | Mode |
| 104 | `1R` | 2, 6 | `0xd64d` | None |
| 110 | `2<ESC>` | 4 | `0x46f66` | None |

All five flag words are `0x000f`. The low pair selects active Base Math. State 3 validates each ordinary digit before it counts toward the field. A [leading period](issue-240-period-zero-field.md) takes the separate branch at `0x3a043–0x3a04e`, stores zero and advances one field. Only the last field uses `0x08` to normalize a run of valid digits to zero at `0x3a8ae–0x3a8da`. Bit `0x04` selects text state after that field, whether it completed by digits or a leading period. There is no delimiter between the numeric prefix and the text. An interior period discards the command while still in numeric collection.

The READ_SCENE handler takes the filename at `0xd686`, ignoring the numeric array. FILE_QUERY reads its mode at `0xbdf5`. The [DEFINE parser](issue-175-rip-define.md) receives flags from the command and ignores the reserve. The directory handler takes its text pointer at `0x46f80`. QUERY loads mode and window at `0xd3e2–0xd3e4`. The mode-zero branch at `0xd482–0xd48c` passes text to the host-command executor at `0x13ed0`. The numbered branches use the window argument. The remaining query/storage rules are in the [deferred query derivation](issue-51-deferred-query-order.md).

## Staged captures

The original framing fixture feeds `!|`, the body and CR separately. No dispatch occurs before CR. `<ESC>` stands for a single byte `0x1b`. The table shows arguments at dispatch, before the handlers interpret text. The initial Base Math is 36 unless shown otherwise.

| Body | Numeric operands at CR | Text at CR |
| --- | --- | --- |
| `1R0A43AAAAChild.RIP` | (10, 0) | `Child.RIP` |
| `1R..Child.RIP`, `1R00.Child.RIP`, `1R.000000Child.RIP` | (0, 0) | `Child.RIP` |
| `1R0000.Child.RIP`, `1R.00.Child.RIP`, `1R.Child.RIP` | No dispatch | — |
| `1R0a.Upper` (Base Math 36) | No dispatch | — |
| `1R0a.Upper` (Base Math 64) | (36, 0) | `Upper` |
| `1F04.Mixed.RIP` | (4, 0) | `Mixed.RIP` |
| `1F..Mixed.RIP` | (0, 0) | `Mixed.RIP` |
| `1F0400.Mixed.RIP` | No dispatch | — |
| `2<ESC>.LOCAL` | (0) | `LOCAL` |
| `2<ESC>00.LOCAL` | No dispatch | — |
| `1D004.Var:data`, `1D0047ZVar:data` | (4, 0) | `Var:data` |
| `1D..Var:data` | (0, 0) | `Var:data` |
| `1D0040.Var:data` | No dispatch | — |
| `1<ESC>001Zhello`, `1<ESC>...hello` | (0, 0, 0) | `hello` |
| `1<ESC>5..enter` | (5, 0, 0) | `enter` |
| `1<ESC>000.hello` | No dispatch | — |

The directory command's logical callback index is 121 in the report. Its physical table index is 110.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 85
run/riptel-inspect "$dll" --slot 91
run/riptel-inspect "$dll" --slot 94
run/riptel-inspect "$dll" --slot 104
run/riptel-inspect "$dll" --slot 110
objdump -d --start-address=0x1000d64d --stop-address=0x1000d70b "$dll"
```
