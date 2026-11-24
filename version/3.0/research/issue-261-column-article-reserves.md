# Issue #261 — Article selection and column text boundaries

[Issue #261](https://github.com/bbs-land/remote-imaging-protocol/issues/261) · [Article selector](../ripscrip/3.1-column-text-system.md#rip_select_article) · [Extended column](../ripscrip/3.1-column-text-system.md#defining-a-column-region---1e) · [Research backlog](issue-backlog.md)

RealDeuce filed #261 at 21:58:05 UTC on 2026-11-24. The 21:58 UTC in-thread check found it with no comments. Branch `issue-261-column-article-reserves` starts at fork stable `cad237a289cd647f79201339b412b271018e24ff`. [Issue #2](https://github.com/bbs-land/remote-imaging-protocol/issues/2) supplied the `1A` identity and full-width descriptor. The [column overflow derivation](issue-4-column-overflow.md) establishes article selection and flow semantics.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The 40-byte command records start at RVA `0x80820`, signed widths start at offset 20, and flags are at offset 38. Addresses below are RVAs at image base `0x10000000`.

| Slot | Opcode | Numeric widths                | Flags    |
| ---- | ------ | ----------------------------- | -------- |
| 86   | `1A`   | 2, 4                          | `0x000b` |
| 92   | `1E`   | None                          | `0x0000` |
| 93   | `1e`   | XY, XY, XY, XY, 1, 1, 4, 2, 8 | `0x000f` |
| 105  | `1T`   | XY, XY, XY, XY, 1, 1          | `0x0003` |
| 106  | `1t`   | 1                             | `0x0007` |

Both `1A` and `1e` have low flag pair `11`, so numeric fields use active Base Math. Bit `0x08` normalizes the final reserve. The [numeric-only delimiter branch](issue-244-port-reserved-fields.md) allows `1A` to omit the reserve while finalizing a nonempty shortened article, or to finish a short reserve, on the delimiter. Once both numeric fields have completed, surplus is ignored. For `1e`, bit `0x04` sends the parser into text state after the eight-digit reserve or its one-period spelling. Both use the [shared period branch](issue-240-period-zero-field.md); a period within a partly collected field discards the command.

At `0xdc62`, `1A` loads operand 0 only. The bound at `0xdc6d–0xdc70` tests the unsigned value against 36. The failure branch reports an invalid article number. The valid branch calls the existing selector at `0x3c399`. That selector flushes pending text when changing articles via `0x3c3d0–0x3c3da`, scans defined columns for the new owner at `0x3c3dd–0x3c409`, sets flow availability at `0x3c40f–0x3c418`, clears the pending buffers and stores the selected article at `0x3c460`. The `1e` handler reads geometry through highlight at `0xa5fa–0xa623` and receives search text separately.

## Staged captures

The report feeds `!|`, each body, and CR separately with XY width 2. No command dispatches before CR. The table gives the collected arguments before the handlers execute.

| Body | Base Math | Arguments at CR |
| --- | --- | --- |
| `1A4`, `1A04`, `1A04.` | 36 | (4, 0) |
| `1A0401`, `1A040001 notes` | 36 | (4, 0) |
| `1A040.` | 36 | No dispatch |
| `1e..1E1E.100020C1234ZAZZHello` | 36 | (0, 0, 50, 50, 0, 1, 2, 12, 0), `Hello` |
| `1e..1E1E.100020C.Hello` | 36 | (0, 0, 50, 50, 0, 1, 2, 12, 0), `Hello` |
| `1e..1E1E.100020C0000.Hello` | 36 | No dispatch |
| `1e..1E1E.100020C0000Hello` | 36 | No dispatch |
| `1e..1E1E.100020C0000Hello` | 64 | (0, 0, 78, 78, 0, 1, 2, 12, 0), `o` |

The Base Math 64 row consumes `0000Hell` as the reserve. Base Math 36 accepts `H` but rejects the lowercase `e` while still collecting it. The report also confirms that `1T` can collect a period for each of its two one-digit selectors and waits for the delimiter, that `1t` can collect a period for its one-digit justification before text, and that `1E` ignores surplus until dispatch.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 86
run/riptel-inspect "$dll" --slot 93
objdump -d --start-address=0x1000dc58 --stop-address=0x1000dc96 "$dll"
objdump -d --start-address=0x1003c399 --stop-address=0x1003c467 "$dll"
```
