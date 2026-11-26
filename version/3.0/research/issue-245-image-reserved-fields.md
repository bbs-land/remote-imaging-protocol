# Issue #245 — Image reserved-field collection

[Issue #245](https://github.com/bbs-land/remote-imaging-protocol/issues/245) · [GET_IMAGE](../ripscrip/4.0-icons-and-bitmaps.md#rip_get_image) · [JPEG command collection](../ripscrip/4.1-jpeg-images.md#image-command-collection-examples) · [Research backlog](issue-backlog.md)

RealDeuce filed #245 at 19:39:57 UTC on 2026-11-24. The audit after #243 found it behind #244. Branch `issue-245-image-reserved-fields` starts at fork stable `86ec1913a7a017fb604ba53467c59c837c0639c4`. The report supplies staged argument and filename captures at XY width 2 and both bases. The [2.0 ALPHA 4 parameter rule](../../2.0/text/RIPScrip-2.0-alpha-4.txt) at lines 4019–4026 describes optional trailing reserved fields. The original descriptor determines the behavior of each 3.0 command. [Issue #244](issue-244-port-reserved-fields.md#reserved-collection-and-switch) identifies the reserved flag and delimiter branches. [Issue #240](issue-240-period-zero-field.md) establishes period shorthand.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. The addresses below are RVAs at image base `0x10000000`. Subtract `0xc00` for instruction file offsets in `.text`.

| Slot | Command | Numeric widths        | Descriptor flags | Handler  |
| ---- | ------- | --------------------- | ---------------- | -------- |
| 89   | `1C`    | XY, XY, XY, XY, 1     | `0x0003`         | `0xc1cf` |
| 98   | `1i`    | XY, XY, XY, XY, 4, 12 | `0x000b`         | `0xc39a` |
| 103  | `1p`    | 4, followed by text   | `0x000f`         | `0xc2c6` |

## Three parser paths

- GET_IMAGE lacks descriptor bit `0x08`. It requires the one-byte field and decodes ordinary digits to their value. The period branch completes that field with zero. A delimiter with the field missing cannot shorten an earlier coordinate in its place. At `0xc1d9–0xc1e6` the consumer reads arguments 0 through 3, with no fifth argument load in the capture handler. Its two far-edge increments at `0xc26d–0xc275` operate on the converted coordinates. The [existing capture geometry](issue-42-clipboard-capture-state.md#full-static-selectionfailure-trace-native-verification-next) therefore remains independent of the digit.
- IMAGE_STYLE uses bit `0x08` and one reserved width of 12. Numeric validation at `0x3a066–0x3a07c` precedes the reserved digit count and zero store at `0x3a8ae–0x3a905`. The period branch supplies the same zero in one byte. At a delimiter the trailing field may be omitted or shortened, and flags may be shortened if that field is omitted. The zero argument comes from the same branches as the port cases. A space or period after some digits in the unfinished reserved field discards the command.
- IMAGE combines the reserved flag with text bit `0x04`. It counts four valid reserved digits or consumes a leading period, stores zero, and enters text collection at completion (`0x3a8f1–0x3a905` or `0x3a788–0x3a79f`). The next byte starts the filename. The delimiter branch at `0x3a09a–0x3a0a1` rejects an unfinished numeric field before applying any trailing-reserved omission rule. The IMAGE handler reads the supplied text at `0xc2d3` and checks it at `0xc2da–0xc2e5`.

The [GET_IMAGE](../ripscrip/4.0-icons-and-bitmaps.md#rip_get_image) and [JPEG examples](../ripscrip/4.1-jpeg-images.md#image-command-collection-examples) preserve the supplied captures. The fixed-list surplus rule applies to `1C` and `1i` after numeric completion. For `1p`, completion begins the filename.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 89
run/riptel-inspect "$dll" --slot 98
run/riptel-inspect "$dll" --slot 103
objdump -d --start-address=0x1000c1cf --stop-address=0x1000c39a "$dll"
objdump -d --start-address=0x1003a082 --stop-address=0x1003a223 "$dll"
objdump -d --start-address=0x1003a8ae --stop-address=0x1003a90c "$dll"
```

---

[Research backlog](issue-backlog.md) · [Image command collection](../ripscrip/4.1-jpeg-images.md#image-command-collection-examples)
