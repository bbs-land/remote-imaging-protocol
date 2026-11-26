# Issue #33: filled-rectangle border pattern and write mode

[Research backlog](issue-backlog.md) · [Filled rectangles](../ripscrip/2.3-shapes-and-fills.md#rip_filled_rectangle) · [Border control](../ripscrip/2.3-shapes-and-fills.md#rip_set_border)

Started after [PR #62](https://github.com/bbs-land/remote-imaging-protocol/pull/62) delivered #28. Branch `issue-33-filled-rectangle-borders` starts at **4819dcb**. Refreshed GitHub creation-order queue selects [issue #33](https://github.com/bbs-land/remote-imaging-protocol/issues/33): K claims current line style/pattern/thickness, while N says nonpolygon borders are solid COPY. Derive original pen/ROP selection with borders enabled/disabled, dotted/custom pens and multiple thickness/write-mode inputs. Do not infer border behavior from #12's brush-phase proof or SyncTERM's interpretation.

Primary source: RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`; original extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base 0x10000000. All addresses are RVAs. Native/static only, controlled GDI capture and no desktop GUI. Clean scoped temporary builds/request files promptly; original artifacts/text stay unchanged.

Existing `run/riptel-fill-probe` executes original K/BAR consumers and separates brush/pen selections. Next: inspect K's border-pen producer and active ROP, extend that probe to capture original selections across line styles/widths/modes, then reconcile the reference and file its complete PR. No #33 answer published yet.

## Original producer/consumer trace and native checkpoint

Physical dispatch **K slot 42 → 0x1bee5**, **N slot 48 → 0xdcf1**, **W slot 68 → 0x2102c**. Original K normalizes/converts corners and selects its DC/clip, calls **0xe6e9** to set the current style write mode, then loads fill brush **0x13d27**. Border WORD style+0x22 selects the current pen via **0x13cfc** (objects+8) or NULL_PEN via **0x13ce5**. It does **not** replace that pen with a solid pen or set COPY for the border. With borders and ordinary line flags it draws fill first with NULL_PEN, then selects NULL_BRUSH **0x13d10**, current pen and TRANSPARENT background, and calls Rectangle a second time. When line flags are exactly 1 it instead uses current pen plus fill in one OPAQUE Rectangle call. Borders disabled uses only fill+NULL_PEN. Both passes retain the same ROP. Final drawing state is restored without changing the stored line mask, line type or write mode.

The line-style producer **0x1cd15** stores type byte +0x1c, thickness byte +0x1d and custom WORD +0x1e. It maps line types 0/1/2/3/4 to **CreatePen styles 0/2/3/1/4** (SOLID/DOT/DASHDOT/DASH/DASHDOTDOT), calls CreatePen at **0x1ce83** and caches the result at objects+8. Thus K uses the actual GDI pen, not the arbitrary custom bit mask. For custom type 4, that mask is retained in the style but the GDI request is fixed DASHDOTDOT; K has no custom-mask raster loop. Current write mode 0/1/2/3/4 maps through **0xe6b3** to ROP2 **13/7/15/9/6**. The only ROP2-setting call in K is the current-mode helper, before either Rectangle. Default COPY must not be confused with forced COPY.

Extended `run/riptel-fill-probe` passes **119,770 checks**. New native matrix is scales 1/2 × screen/offscreen × line flags 0..3 × line types 0..4 × input widths 1/2/3/255/256 × write modes 0..4 × borders 0/1 (**4,000 original K executions**). It executes original fill/line/W/N handlers, pen construction, viewport/DC/clip setup, write-mode mapping and complete K consumer. Captures assert pen handles/style/width, brush, ROP2, background mode and drawing pass count, and verify stored style/mask/write mode are unchanged. Width scaling/byte truncation agrees with #11. Width 256 stored as zero requests a one-pixel GDI pen. No Windows rasterization is executed.

Controlled service delta from #12: stdcall CreatePen **0x963d4** captures style/width/color and returns unique owned pen tokens; InflateRect **0x96698** performs direct integer bounds expansion. SelectObject independently tracks pens/brushes/bitmaps. Rectangle records each actual selection and mode. Existing SetROP2/SetBkMode stand-ins now keep correct previous/current state instead of returning an assumed constant. Existing synchronization/palette/protection/color controls remain exactly those catalogued in the [#12 note](issue-12-fill-masks.md). Core/GUI/audio paths are not involved.

GDI boundary: Microsoft's primary [CreatePen contract](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createpen), read 2026-11-16, says dotted/dashed styles are supported only at width one or less; width>1 yields a solid pen, while width zero means one device pixel. That explains a **wide solid border**, not an unconditional K solid/COPY policy. Native captures establish the original requests; this API-defined raster limitation is a separate source-based inference. Pattern-gap background behavior is explicit in the original one-pass/two-pass calls, not an invented GDI simulation.

Next: reconcile K and N prose, document the original custom-pen limitation and width distinction, run required checks, file complete PR dependent on #62. Original text/assets/artifacts remain unchanged and wrapper task build paths are removed on EXIT.

## Complete reconciliation and validation

The K entry now states current logical pen plus current write mode, including a width-one dotted/XOR example, the custom-type fixed GDI pen limitation and documented wide-pen solid behavior. It also distinguishes one opaque combined pass (line flags exactly 1) from separate fill and transparent-gap border passes (flags 0/2/3). N's contradictory unconditional solid/COPY paragraph is replaced by the original verified K rule; this does not generalize untested raster behavior to other filled primitives or earlier clients. The exact requested answer is therefore **not an unconditional solid COPY border**: the selected pen and write mode survive, subject to the documented GDI mapping/width limits. No exact Windows pixel-raster claim is made.

Reproduction (FreeBSD 15.1 / clang 19.1.7, native i386; wrapper verifies the DLL hash and removes its scoped build):

```sh
run/riptel-fill-probe /path/to/original/ripscrip.dll
run/lint
run/check-links
git diff --check
```

The native matrix passes **119,770 checks**, including all #12 brush/pattern cases and 4,000 new K executions. Historical text, earlier-version reference and original assets/artifacts are unchanged. Final required checks and PR link are recorded at delivery.

Delivered in [PR #63](https://github.com/bbs-land/remote-imaging-protocol/pull/63), dependent on #62. Specification commit **982a59e**, issue-only comparison starts at **4819dcb**. Required checks pass: 119,770 native fill checks, formatting, 0 bad of 5,249 links and diff check. No task temporary build/request files remain. Refreshed creation-order queue selects #38 next; do not merge or manually close #33.
