# Issue #255 — Outline-font and metric text boundaries

[Issue #255](https://github.com/bbs-land/remote-imaging-protocol/issues/255) · [RIP_EXTENDED_FONT_STYLE](../ripscrip/3.0-text-output-and-fonts.md#the-shipped-3x-wire-layout) · [RIP_TEXT_METRIC](../ripscrip/3.0-text-output-and-fonts.md#rip_text_metric) · [Research backlog](issue-backlog.md)

RealDeuce filed #255 at 21:11:37 UTC on 2026-11-24. The 21:18 UTC in-thread check found the report with no comments. Branch `issue-255-font-text-boundaries` starts at fork stable `f4683d040ba24b279f6a5ef82b8ee9df9d94001d`. The [outline style derivation](issue-3-extended-font-style.md) identifies the eleven field roles. The [zero-field derivation](issue-240-period-zero-field.md) traces the shared one-byte shorthand.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses are RVAs at image base `0x10000000`. Subtract `0xc00` for `.text` file offsets.

| Slot | Opcode | Numeric widths                  | Flags    |
| ---- | ------ | ------------------------------- | -------- |
| 57   | `r`    | 1, 1, 4                         | `0x0007` |
| 75   | `y`    | 1, 1, 4, 2, 2, 2, 2, 2, 2, 2, 6 | `0x000e` |

## Text starts after the actual last field

For `y`, the low flag pair `10` forces UltraNum. For `r`, `11` selects active Base Math. Both descriptors have text bit `0x04`. State 3 checks for a leading period at `0x3a043–0x3a04e`, before digit validation. The branch at `0x3a6d2` stores zero and advances one field. After the final field it uses `0x3a788–0x3a79f` to choose text state 5. A period after buffered digits reaches the validator, fails both digit and delimiter tests, and takes discard at `0x3a8a3`.

Only `y` has final-reserve bit `0x08`. For a full spelling of its final field, `0x3a8ae–0x3a8cb` counts valid digits and `0x3a8d1–0x3a905` supplies zero and selects text. No accumulator is needed for that reserve. Six `&` digits therefore also supply zero. The preceding ignored `WW` field uses the ordinary decoder. For `r`, the regular conversion path at `0x3a92b–0x3a97a` supplies the decoded value of the reserve. It enters text at `0x3aa01–0x3aa18` for the full-width form. The handler reads mode and domain at `0x2038d–0x203bf` and does not use the reserve. The `y` handler checks spacing and presence of the name at `0x1ae75–0x1ae95`, after the parser has dispatched.

These paths give `y` eleven numeric fields in every case. Without shorthand they occupy 26 digits. Replacing any field with a leading period consumes one byte for that field and shifts the text start accordingly. The 13 field characters in the [historical draft](../../2.0/text/RIPScrip-2.0-alpha-4.txt) describe a different field list.

## Staged captures

The report supplies the original parser arguments from a handler capture. Each first feed contains `!|` followed by the body. Feed CR separately. All first feeds produce zero dispatches. The `y` results below hold under both initial Base Math settings. Spacing and name checks happen after these captures.

| Body | Arguments at CR | Name |
| --- | --- | --- |
| `y00000W0M000000001a1a000000Dixon` | (0, 0, 32, 22, 0, 0, 0, 0, 100, 100, 0) | Dixon |
| `y00000W0M000000001a1a.Dixon` | (0, 0, 32, 22, 0, 0, 0, 0, 100, 100, 0) | Dixon |
| `y...0M....1a..Dixon` | (0, 0, 0, 22, 0, 0, 0, 0, 100, 0, 0) | Dixon |
| `y...0M....1a.00.000Dixon` | No dispatch | — |

| Body           | Base Math 36 at CR | Base Math 64 at CR       |
| -------------- | ------------------ | ------------------------ |
| `r01.Hello`    | (0, 1, 0), `Hello` | (0, 1, 0), `Hello`       |
| `r01000Hello`  | (0, 1, 17), `ello` | (0, 1, 17), `ello`       |
| `r01????Hello` | No dispatch        | No dispatch              |
| `r01abcdHello` | No dispatch        | (0, 1, 9591207), `Hello` |

`H` is digit 17 in both bases, so three zeros followed by `H` complete the four-digit `r` reserve. The first lowercase letter of `abcd` fails MegaNum validation. In UltraNum, those bytes have values 36, 37, 38 and 39.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 57
run/riptel-inspect "$dll" --slot 75
objdump -d --start-address=0x1003a8ae --stop-address=0x1003a90c "$dll"
objdump -d --start-address=0x1001ae75 --stop-address=0x1001ae9b "$dll"
```
