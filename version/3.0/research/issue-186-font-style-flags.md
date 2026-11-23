# Issue #186 - Classic font style alignment and shadow

[Backlog](issue-backlog.md) · [RIP_FONT_STYLE](../ripscrip/3.0-text-output-and-fonts.md#rip_font_style) · [Issue #186](https://github.com/bbs-land/remote-imaging-protocol/issues/186)

RealDeuce filed this report at **2026-11-23T20:06:05Z**. The report body SHA-256 is `1ef20e03dde8d91d1d368f7bbd213bd42fe556a8c93a0782702093b815ccc42b`. Work starts on `issue-186-font-style-flags` from fork stable **858138c339fea7a68e6bf9c5beecae9e5113764e**. The **20:11:43Z** queue audit found #186 first, then [#187](https://github.com/bbs-land/remote-imaging-protocol/issues/187), created **20:06:34Z**, report body SHA-256 `88af6e581308ef41d4a682e7ddc6cad6af021ea98ac5540b99ebd7a3b3bb7ba5`. Both had zero comments, with no new review or inline feedback. #187 owns the font-zero realization.

## Source and method

The [ALPHA 4 table](../../2.0/text/RIPScrip-2.0-alpha-4.txt), `3.4.1.22, assigns a different flag map. Retain it as historical source. The original RIPtel 3.1 PC Micro `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`from the [external catalogue](../../../reference/rip-tools.md) is 592,896 bytes, SHA-256`83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. All addresses here are RVAs. With `DLL`set to this artifact, LLVM 19.1.7 uses the image base`0x10000000` plus each RVA.

```sh
sha256 -q "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1001c87e --stop-address=0x1001ca84 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10039808 --stop-address=0x1003988f "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1002023c --stop-address=0x100202c0 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10020dff --stop-address=0x10020e8a "$DLL"
```

## Validation and style commit

- Physical descriptor 74 at RVA `0x80820 + 74 × 40` selects level 0 `Y`, handler `0x1c87e`, four two-digit numeric fields and descriptor flags `0x0003`. Decode with current base math. `0x1c897–0x1c89f` read font, direction, size and flags. `0x1c8ba–0x1c8e7` validate font 0–10, direction 0–1 and size 1–10. `0x1c8f0` checks protection of the selected STYLE. The ordinary terminal cursor and other tables do not supply these fields.
- `0x1c90e–0x1c987` counts horizontal bits `1,2,4` and vertical bits `8,16,32,64` independently and rejects either count above one. These checks and the other rejection paths precede the first style store. Follow `0x1ca64–0x1ca83` through diagnostic and unlock. A rejection preserves the whole current style, including classic/outline selection, the last font, size, direction, anchors and shadow.
- `0x1c988–0x1c9b4` inserts bit 1 for omitted horizontal alignment with font 0 and direction 0, bit 2 with font 0 and direction 1, and bit 8 for omitted vertical alignment with font 0 in either direction. For fonts 1–10, omitted groups stay zero in the record. Explicit group selections bypass those insertions. The `0x39808` consumer resolves absent horizontal and vertical groups to left and top respectively, so the stroked fonts default to left/top in both directions.
- At `0x1c9c3–0x1ca0c`, the selected style's font, direction, size, flag word and classic mode are updated. The flag WORD stores the supplied bits plus inserted defaults. Values at or above 256 are accepted as far as the defined groups are valid, and bits above bit 7 have no meaning for classic placement or shadow. They are not new 3.0 effects. `0x1ca48` installs the new choices, then `0x1ca58` calls the classic font setup. The [normal initial record](issue-24-drawing-state-ownership.md#defaults-rejected-bootstrap-lead) selects font 0, size 1 and direction 0 with a zero flag word, also decoding to left/top.

## Consumer and corpus

`0x39808` maps horizontal bits 1, 2 and 4 to renderer selectors 0 (left), 2 (right) and 1 (center). The vertical values 8, 16, 32 and 64 select respectively 2 (top), 0 (bottom), 3 (baseline) and 1 (middle). The writer at `0x13d0` installs the selector bytes. The CHR branch at `0x2334–0x24bd` uses the selected run alignment and font reference line in the specified direction. This is alignment state, independent of how large a font-zero label renders.

The classic `T` and `@` paths test bit `0x80` at `0x20258` and `0x20e37`, respectively. An enabled shadow supplies the background drawing color and offsets (1,1) to `0x2830`. Zero clears the draw-time shadow request. The draft's one-device-pixel lower-right placement describes the effect, with the shipped bit position above. The two ordinary text callers pass those selections to the classic drawing helper at `0x202bb` and `0x20e85`.

The report's three literals were checked byte-for-byte in the local RIPaint 2 sample tree `Rpaint2/RIPaint2/RIP/`, using binary reads.

| Sample | SHA-256 | Literal | Decimal flags | Resolved style |
| --- | --- | --- | --- | --- |
| CONCRPAP.RIP | `eb4bdfb1b2c3cfcca55e10dd85d2934485f07db03f42c4738b7707c3e87e8956` | `Y00000209` | 9 | Left/top, shadow off |
| SCREENUP.RIP | `eaed5592f3d29dac096fa7b75d5a6451d934f11caaa2beca66ae22d1cafc0f46` | `Y0000021T` | 65 | Left/middle, shadow off |
| DEMO1ST.RIP | `658efb2050778bb7146eb57261ba948fa32c703e79dbb1301c6f286545fec387` | `Y0000023T` | 137 | Left/top, shadow on |

The CONCRPAP wire WORLD size is 1280×960 and the text point `R4OO` is (976,888). At 640×480 with zero viewport origin it projects to (488,444). At either scale the left/top alignment follows the flag table. The [drawing-state derivation](issue-24-drawing-state-ownership.md) separately governs post-draw cursors. No original client or graphics routine was run for this documentation pass.

## Verification

The DLL hash, descriptor 74, 10 instruction signatures and nine exact call targets match. The three RIPaint 2 samples match the recorded hashes and wire fields. An independent alignment model verifies 24,576 combinations of flag values, representative font values and both directions, plus rejection for invalid parameter ranges and protected state. The literal plaque and text anchor project to the listed coordinates. The prior valid style remains selected after both conflicting examples. Repository formatting and link checks passed.

## Resume

The reference now describes the 3.0 flag table, omitted-group rules and rejection result. Deliver #186 and consolidate its actual source before starting #187.

## Delivery

[PR #188](https://github.com/bbs-land/remote-imaging-protocol/pull/188) targets upstream main from `issue-186-font-style-flags`. Specification commit **5739c2d7059f90f93049fded21f4739592f1a564**. Formatting, diff checks and 6,663 Markdown links passed.
