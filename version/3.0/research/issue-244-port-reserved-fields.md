# Issue #244 — Port reserved fields and switch flags

[Issue #244](https://github.com/bbs-land/remote-imaging-protocol/issues/244) · [Reserved port command fields](../ripscrip/7.0-drawing-ports.md#reserved-port-command-fields) · [Research backlog](issue-backlog.md)

RealDeuce filed #244 at 19:35:09 UTC on 2026-11-24. The 19:37 UTC in-thread check found it after #243. Branch `issue-244-port-reserved-fields` starts at fork stable `03c5e38e72efb8b609f6a6c1867a4684a3ac3ca5`. The report supplies original-parser captures at XY width 2 in both bases. [ALPHA 4](../../2.0/text/RIPScrip-2.0-alpha-4.txt) sections 3.4.3.1–3 and 3.4.3.9 give the historical sender spellings and current Base Math. The draft's `2s` signature includes an additional reserved field. [Issue #54](issue-54-switch-port-precedence.md) establishes the shipped switch handler and flag actions.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. Addresses are RVAs at image base `0x10000000`. Subtract `0xc00` for instruction file offsets in `.text`.

| Slot | Command | Numeric descriptor widths | Descriptor flags |
| --- | --- | --- | --- |
| 113 | `2C` | 1, XY, XY, XY, XY, 1, XY, XY, XY, XY, 1, 5 | `0x000b` |
| 115 | `2P` | 1, XY, XY, XY, XY, 4, 4 | `0x000b` |
| 116 | `2p` | 1, 1, 2 | `0x000b` |
| 118 | `2s` | 1, 2 | `0x0003` |

## Reserved collection and switch

The low descriptor flag pair selects active Base Math in the [radix helper](issue-239-rgb-numeric-width.md#width-and-radix). For the three reserved-field commands, bit `0x08` marks the last field as reserved. On ordinary digits the validator first runs at `0x3a066–0x3a07c`. The reserved branch `0x3a8ae–0x3a905` counts valid characters to the declared width and stores zero. A leading period reaches the [one-byte zero branch](issue-240-period-zero-field.md) before digit validation. Invalid characters in unfinished fields discard the command through `0x3a8a3`.

On a delimiter with one formal field outstanding, bit `0x08` causes a zero store at `0x3a0e6–0x3a102` whether that field is empty or contains valid short digits. With two fields outstanding, `0x3a0bc–0x3a0e4` permits shortening the preceding field. The short numeric conversion occurs at `0x3a10d–0x3a1b4`, then `0x3a1d9–0x3a1ff` appends zero for the omitted reserved field. This establishes both the short flags and omitted reserved case for `2P`. Once the full reserved field has been consumed, the [fixed-field waiting state](issue-237-fixed-numeric-dispatch.md#collection-and-dispatch) can ignore suffix bytes until dispatch.

The `2s` descriptor has two numeric fields. Its ordinary last-field rule permits one flags digit at a delimiter and its zero shortcut permits a period. In base 64, `a` is 36 and `a0` is 2,304. The two inputs therefore reach the parser's dispatch hook independently of the handler's port range check. Flags in these captures are raw numbers. The higher bits of the separate `2P` flags field remain governed by its existing rule.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 113
run/riptel-inspect "$dll" --slot 115
run/riptel-inspect "$dll" --slot 116
run/riptel-inspect "$dll" --slot 118
objdump -d --start-address=0x1003a082 --stop-address=0x1003a223 "$dll"
objdump -d --start-address=0x1003a8ae --stop-address=0x1003a90c "$dll"
```

---

[Research backlog](issue-backlog.md) · [Reserved port command fields](../ripscrip/7.0-drawing-ports.md#reserved-port-command-fields)
