# Issue #239 — Color-mode field width

[Issue #239](https://github.com/bbs-land/remote-imaging-protocol/issues/239) · [CM wire format](../ripscrip/2.0-color-modes-and-palettes.md#color-mode-wire-format) · [Research backlog](issue-backlog.md)

## Sources

RealDeuce filed #239 at 18:34:59 UTC on 2026-11-24. The 18:36 UTC in-thread check found the report with no comments. Branch `issue-239-rgb-numeric-width` starts at fork stable `9360394eb7f1ea8a78e130b3b6868fc1094b8842`. The report supplies dispatch-hook captures for active RGB precisions 1–8 and for palette mode in both radices. Its scope is wire collection. [Issue #23](issue-23-environment-query-names.md#native-callback-activation-and-wire-checkpoint) establishes the separate wire M gate, the COLORMODE macro range, and the shared active precision setter.

The inherited [ALPHA 4 COLOR entry](../../2.0/ripscrip/2.0-color-modes-and-palettes.md#rip_color) gives a precision-dependent RGB width table. Its [numeric parameter rule](../../2.0/text/RIPScrip-2.0-alpha-4.txt) at lines 3981–4026 permits leading-zero removal on the last transmitted numeric field. The recovered 3.0 collector uses a fixed full RGB width, then applies that ordinary final-field rule. See the [delimiter trace](issue-237-fixed-numeric-dispatch.md).

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. Addresses below are RVAs at image base `0x10000000`. Subtract `0xc00` for file offsets in `.text`. The descriptor array at RVA `0x80820` has file offset `0x7ee20` and stride 40.

## Width and radix

Slots 22 (`c`), 43 (`k`), 58 (`S`) and 59 (`s`) are the original entries with a signed width descriptor of −2. Each uses that descriptor for its final color field. The preceding `S` pattern field and the eight `s` pattern fields have literal width 2. The original deprecated `F` record does not use −2.

- Width helper `0x39de0` reads the signed descriptor. For −2, `0x39e05–0x39e11` returns 2 when parser byte `+0x3a` is zero. For active precision 1 through 6 the selector table at `0x39e68` chooses the return at `0x39e30`. Precisions 7 and 8 choose `0x39e36`. Both return 4.
- Radix helper `0x39d70` checks the same active byte and the current descriptor at `0x39d76–0x39d96`. A nonzero precision and descriptor −2 select UltraNum. With zero precision the four commands have flags `0x0003` and use the current Base Math at parser offset `+0x38`. The earlier pattern descriptors still use current Base Math in RGB mode.
- The regular numeric decoder uses base-64 accumulation at `0x211d6–0x211f5` or base-36 accumulation at `0x211a9–0x211c8`. Fully collected fields wait for a command delimiter. The [shortening branch](issue-237-fixed-numeric-dispatch.md#collection-and-dispatch) still commits a nonempty short final field on that delimiter using the field's own radix. A following digit before the delimiter belongs to the unfinished short field until it reaches full width.

The supplied capture examples agree with positional arithmetic. UltraNum `1234` equals 270,532 and `1a34` equals 409,796. The examples dispatch those values for all active precisions 1–8. The `S` example forwards both the pattern selector and the decoded color. Palette `07` equals 7 in base 36 and `3&` equals 255 in base 64. Subsequent bytes after a full final field follow the already documented suffix-tolerance rule. These are raw decoded arguments. The bit precision still describes the intended packed color components. Handler validation and rendering for values outside the selected precision are beyond the reported collection trace.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 22
run/riptel-inspect "$dll" --slot 43
run/riptel-inspect "$dll" --slot 58
run/riptel-inspect "$dll" --slot 59
run/riptel-inspect "$dll" --rva 0x39e54 --length 29
objdump -d --start-address=0x10039d70 --stop-address=0x10039e54 "$dll"
```

---

[Research backlog](issue-backlog.md) · [CM wire format](../ripscrip/2.0-color-modes-and-palettes.md#color-mode-wire-format)
