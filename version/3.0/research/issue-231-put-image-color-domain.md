# Issue #231 — PUT_IMAGE Boolean color operands

[Issue #231](https://github.com/bbs-land/remote-imaging-protocol/issues/231) · [PUT_IMAGE](../ripscrip/4.0-icons-and-bitmaps.md#rip_put_image) · [Research backlog](issue-backlog.md)

## Source boundary

RealDeuce filed #231 at 16:59:03 UTC on 2026-11-24. The 17:05 UTC in-thread check found the open report with no discussion or resolution PR. Branch `issue-231-put-image-color-domain` starts from fork stable `8b11b5183e670b7ab137591faadb949fcf09eb6d`.

The [RIPscrip 2.0 ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) at lines 9887–9944 defines the `1P` mode field, source clipboard and current destination. Modes 01, 02 and 03 combine its source with the destination already present. [The inherited operation equations](../../2.0/techspecs/2.3-drawing-composition.md#operations-and-operands) distinguish Boolean operations on indices before lookup from Boolean operations on resolved RGB components. The primary record does not choose a domain for unlike logical colors, define capture versus paste resolution for a retained index, or prescribe behavior across modern backings that mix logical indices and RGB.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies original RIPtel 3.1 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`. SHA-256 is `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. Addresses below are RVAs with image base `0x10000000`. Subtract `0xc00` for file offsets of the instructions in `.text`.

The [previous paste trace](issue-229-put-image-viewport.md#source-selection-and-clipping) establishes the unscaled branch. The mode selection at `0x136c2–0x136f5` assigns `0x00660046`, `0x00ee0086` or `0x008800c6` for modes 01, 02 and 03 respectively. These are SRCINVERT, SRCPAINT and SRCAND. At `0x1371b–0x13732` the two port records yield their DCs. Screen port types use the master screen DC. At `0x13735–0x13749` the routine passes the two DCs, source and destination geometry, and the ROP to GDI32 BitBlt through IAT `0x963e4`. Port allocation calls GDI32 CreateCompatibleBitmap through IAT `0x96460` at `0x335f5` using its reference DC, then selects the bitmap into the offscreen DC at `0x33650`.

Microsoft describes [format conversion by BitBlt](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-bitblt) when the DC formats differ and [device-compatible bitmap formats](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createcompatiblebitmap). From this call boundary the original ROP and surfaces are established. Mixed logical operand promotion and exact display-independent values require a portable rule. The static reads and API documentation do not establish output pixels on any particular Windows display.

## Portable completion

Preserve the distinction between an index in the current RIP drawing palette and a direct RGB pixel. The environment's [color mode](../ripscrip/2.0-color-modes-and-palettes.md#rip_set_color_mode) specifies how later color parameters decode. GET_IMAGE copies the logical identity of a captured pixel, including the sample chosen by the existing capture geometry. Switching ENV or selecting another palette leaves a captured index as an index. When an RGB-domain paste needs that index's color, use the current published drawing palette at the paste. A captured direct RGB value retains its color. A pixel whose color belongs to a separate terminal-local palette can contribute its actual scene RGB without conflating that palette's indices with RIP drawing indices.

When both operands are drawing indices, use the logical destination's index width and store the resulting index. The 4-bit and 8-bit masks are inherited indexed examples. Using the destination width for a source from another indexed domain is the portable completion. A physical display color limit alone does not reduce the destination's logical width. Any direct RGB operand promotes the operation to all three 8-bit channels, with a literal RGB result. A later palette change can recolor the index result; it leaves the literal result intact. This choice preserves exact mixed results in both directions independent of host DC format or nearest-color presentation.

The destination supplies the logical scene pixel visible immediately before the paste on the selected port. The ordinary shared text and graphics raster supplies actual painted ANSI background pixels where direct-color ANSI is supported. Transient presentation overlays are outside the scene. Offscreen ports supply their stored image pixel. Neither Draw Color nor STYLE background serves as an implicit destination. The policy governs PUT_IMAGE modes 01–03. It retains the independent geometry, mode fields and remembered-pair rules.

The [composition examples](../ripscrip/4.0-icons-and-bitmaps.md#boolean-paste-color-examples) use fresh destination pixels for each mode. In all mixed rows index 1 resolves through the palette published before the paste, even though that index was red at capture. The 16-color and 256-color cases use source index A6 and destination index 05. Mask 0F gives 03, 07 and 04. Mask FF gives A3, A7 and 04. Deliberately unlike palette entries distinguish a computed result index from RGB channel arithmetic.

```sh
sha256sum "$dll"
objdump -p "$dll"
objdump -d --start-address=0x100335e4 --stop-address=0x10033657 "$dll"
objdump -d --start-address=0x100136a9 --stop-address=0x10013763 "$dll"
```

---

[Research backlog](issue-backlog.md) · [PUT_IMAGE](../ripscrip/4.0-icons-and-bitmaps.md#rip_put_image)
