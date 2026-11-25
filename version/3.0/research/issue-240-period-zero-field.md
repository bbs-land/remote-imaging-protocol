# Issue #240 — Period as a zero numeric field

[Issue #240](https://github.com/bbs-land/remote-imaging-protocol/issues/240) · [Numeric period rule](../ripscrip/1.1-protocol-and-command-hierarchy.md#period-as-a-zero-field) · [Research backlog](issue-backlog.md)

## Sources

RealDeuce filed #240 at 18:35:01 UTC on 2026-11-24. The 18:36 UTC in-thread check found it behind #239. Work started from fork stable `8782540e758a703a52f8ccacc0cf44af8cd50c2b`, after PR #241. The report supplies the parser's command and argument captures from the [issue #7 framing fixture](../../../run/lib/riptel-framing-probe.c) using CRLF. It includes both Base Math settings for the WRITE_MODE distinctions. Earlier corpus evidence supplies a shipped `W.` use. [Issue #237](issue-237-fixed-numeric-dispatch.md) establishes the completed fixed-list suffix rules.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. Addresses are RVAs at image base `0x10000000`. File offsets of these `.text` instructions are RVA minus `0xc00`.

## Collection path

At `0x3a043–0x3a04e` the ordinary numeric collector checks its current-field byte count at parser offset `+0x0c`. A period (`0x2e`) with zero bytes buffered jumps to `0x3a6d2` before either radix-specific digit check. It stores integer zero at `0x3a6db`, increments the descriptor position and argument count at `0x3a6f6–0x3a701`, and resets the current-field byte count at `0x3a704`. This consumes one wire byte and one argument position. The next width is loaded at `0x3a721–0x3a72b` if a descriptor remains. The existing continuation, text and fixed-list dispatch paths apply after collection.

A period with digits already buffered bypasses the zero branch. Neither the MegaNum validator at `0x210b2` nor the UltraNum validator at `0x210d0` accepts it as a digit. It also falls outside the delimiter comparisons at `0x3a082–0x3a094`, so the unfinished command enters discard at `0x3a8a3`. A fully collected fixed numeric command is already in state 8 at `0x3a469`, which ignores that byte among its surplus. These branches explain why `W0.` fails and `W04.` forwards 4.

The dispatch table identifies WRITE_MODE slot 68 with a two-digit field, COLOR slot 22 with CM, LINE slot 44 with four XY fields, FILL_STYLE slot 58 with widths 2 and CM, and SET_COLOR_MODE slot 46 with widths 1 and 1. The [captured examples](../ripscrip/1.1-protocol-and-command-hierarchy.md#period-as-a-zero-field) illustrate a complete zero in the middle of a LINE and the distinct final-field cases. The reported M example forwards (1,0) to its handler, whose [wire RGB precision gate](../ripscrip/2.0-color-modes-and-palettes.md#rip_set_color_mode) still applies.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 68
objdump -d --start-address=0x1003a01b --stop-address=0x1003a0a7 "$dll"
objdump -d --start-address=0x1003a6d2 --stop-address=0x1003a741 "$dll"
objdump -d --start-address=0x1003a469 --stop-address=0x1003a49b "$dll"
```

---

[Research backlog](issue-backlog.md) · [Numeric period rule](../ripscrip/1.1-protocol-and-command-hierarchy.md#period-as-a-zero-field)
