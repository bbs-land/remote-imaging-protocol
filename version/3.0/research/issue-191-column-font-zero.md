# Issue #191 - Classic font zero in article columns

[Backlog](issue-backlog.md) · [Column font realization](../ripscrip/3.1-column-text-system.md#classic-font-zero-in-columns) · [Issue #191](https://github.com/bbs-land/remote-imaging-protocol/issues/191)

RealDeuce filed #191 at **2026-11-23T21:01:29Z**. Body SHA-256 `56f8b49c6f54948d5140c4dd76f9e53e16209b2e1aa267dd136366b034f63251`. Start on `issue-191-column-font-zero` at fork stable **0a4475686b491222534530e0b31979bbecfada5b**, after PR #194. The **21:10:44Z** GitHub audit found #191 followed by #193. The [closed duplicate #192](https://github.com/bbs-land/remote-imaging-protocol/issues/192#issuecomment-7113914226) supplies a one-glyph fixture for both article routes. This issue specifies classic font zero only. [#173](issue-173-prewrapped-region-text.md) owns prewrapped placement and [#4](issue-4-column-overflow.md) owns raw article and overflow state.

## Sources and reproduction

Use the original RIPtel 3.1 PC Micro `ripscrip.dll` (592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`) and `default.rff` (SHA-256 `c734c387ae1902335de818249025c6fcf6d28322eb13dfc82737f2503827706c`). [#187](issue-187-classic-font-zero.md) gives their provenance and shared face widths, size and spacing tables. With `DLL` set to the original DLL, LLVM 19.1.7 uses preferred addresses based at `0x10000000`. All addresses in the rest of this note are RVAs.

```sh
sha256 -q "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10038c0c --stop-address=0x10038cdc "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1003bb7e --stop-address=0x1003bddc "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1003fc20 --stop-address=0x1003fcc6 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1003ce24 --stop-address=0x1003d43d "$DLL"
```

## Outline adapter and metric refresh

- Branch `0x3bc18–0x3bc57` checks classic mode and font 0, then prepares `0x38c0c` with NULL text. That adapter loads the active ENV WORLD at `0x38c62 → 0x3fb30`, reads the size- and device-dependent spacing at `0x38c89 → 0x1000` and the point size at `0x38c97`. `0x38cac` passes literal `DEFAULT EX`, then `0x38cc8` calls `0x40ed0` with 26 arguments. Both rotations are zero, with left/baseline placement. The label or ordinary text anchors from `Y` do not replace column alignment. Classic shadow bit 7 is carried as outline shadow selector 4 or 0.
- `0x3b82f` calls that same draw adapter for prewrapped text. The raw label-independent painter calls at `0x3c8b2`, `0x3c908` and `0x3c932`. Search highlight supplies the chosen foreground for the particular segment. The port and viewport determine the surface and clip under the existing rules.
- `0x3bd77` stores the FastFont line step at article-state `+1`, `0x3bd8b` measures a space for `+7`, `0x3bd9a` stores ascent at `+5` and `0x3bda9` descender at `+3`. `0x3bdc0 → 0x3fa20` populates 256 per-byte widths at `+0x2082`. In `0x3fa67–0x3fb1c` those widths use the realized advance, X DPI, spacing adjustment and zero for out-of-range slots. The dirty flag clears at `0x3bdcb` after refresh. The raw feeder calls the refresh at `0x3cee1`. The adopted `1t` refresh from #173 is `0x3c084`. Column creation does not latch a bitmap bank or a font metric state.
- The ascent reader `0x3fc20` uses font context `+0xc8`, descender `0x3fc40` uses `+0xcc`. `0x3fc70` instead returns the sum of the native height part `+0xca`, descender `+0xcc` and the `+0xba` value scaled by Y DPI/1440, truncated. `0x40170` lays out the preparation record at `+0x8a`, and `0x4f12b–0x4f130` copies the signed byte at style cache row `+7` to preparation record `+0x30` (context `+0xba`). The [cache record](../../2.0/techspecs/3.2-fastfont-fonts.md#atfcfg---the-engines-font-catalog-cache) has a file-index word before the style. The shipped face has style byte `+5` = 15 at file offset `0xf3`. This identifies the leading input separately from hinted ascent/descent. Use the stable portable design reference A=750 and D=250 in em units, scaled/truncated separately as in #187/#190. The portable step adds the recovered leading term to those two DEVICE metric values. This is an explicit completion of layout metrics, without assuming equality of all hinted native font heights.

## Placement and wrap boundaries

- The column width stored at `0x3bf08` is inclusive right minus left plus one. For font zero, the raw feeder reads DEVICE width at `0x3cf3a`. `0x3d149–0x3d15f` adds the next byte width to the pending width and takes the wrap branch on **greater than or equal**, where possible retaining the useful word boundary. `0x3d03f–0x3d075` similarly tests a separator using the prepared space metric, with two spaces after `. ! ? :`. Wrap caused by that space carries neither a trailing space into the painted line nor a leading separator into the next. The renderer uses measured byte contributions and its existing line segmentation and justification.
- Prewrapped width is the [spacing-adjusted line advance](issue-173-prewrapped-region-text.md#horizontal-layout-cursor-and-clipping) including spaces. `0x3b83b` reads that metric for the first of two segments. It keeps the existing `1t` modes, signed extra width and fixed-point distribution, without a horizontal-fit rejection.
- `0x3c50d–0x3c53e` tests the raw line against inclusive bottom with `Y + A + D`. `0x3c62d` uses `Y + A` as its paint baseline, as does #173's prewrapped layout. `0x3bf70` advances by the prepared step and retests the next line. `0x3cfbf–0x3cfde` handles a blank paragraph through pending-line flush then a single-space `1t` placement. Its paired-newline gate excludes the second byte of a CRLF. A retained blank gets normal fit and advancement, including the established first-line suppression on a later column. No raw-flow, output or overflow ownership changes follow from selecting the face.
- The first default `A` and the space in the 1024×768, equal WORLD fixture both have 500 design units. Q=11, P=109 and both DPI values 72. Each step is 7 pixels. The portable metrics are A=8, D=2, L=10; at column top 100 the reference Y is 108. With inclusive W=21 the raw string `A A` wraps because its candidate width is 21. W=22 admits it. The same `1t1A A` at W=22 adds one device pixel to the existing gap, putting the second A at left+15. No original client or glyph rasterizer ran.

## Verification

Verified both artifact hashes, 17 original call targets, six instruction signatures, the face leading input, and the documented wire fields. Independent arithmetic checked the 21/22-pixel wrap boundary, the second justified origin, exact inclusive-bottom fit, and a scale where the leading term adds a device pixel. The text chapter outside RIP_REGION_TEXT is byte-identical to the branch base. Formatting and links passed.

## Resume

The specification now describes column metrics and boundary examples. Verify and deliver the focused PR before #193.
