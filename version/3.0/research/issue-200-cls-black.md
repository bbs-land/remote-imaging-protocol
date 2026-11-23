# Issue #200 — Physical CLS fill

[Research backlog](issue-backlog.md) · [CLS](../ripscrip/5.1-text-variables.md#cls)

## Scope and checkpoint

[RealDeuce report #200](https://github.com/bbs-land/remote-imaging-protocol/issues/200), created 2026-11-23 23:01:04 UTC, body SHA-256 `858ff331c3ab85686db499f4a94a28b1457d9299995275d64a8012ed16c1cdec`, zero comments on intake. Started from fork stable `ed0313f388790981f7ef83762ec029a2a1197e85`. The 23:05:47Z audit queues #200 followed by #201.

## Source and boundary

The [2.00 Alpha 4 draft](../../2.0/text/RIPScrip-2.0-alpha-4.txt), §5.10.2, gives the no-reset contract and the older background-color wording. This is a correction to the reconstructed 3.0 reference for the shipped fill request. The [earlier HARD trace](issue-158-hard-reset-profile.md#original-source-ledger-and-limits) describes the same physical helper outside the standalone CLS context. The [status-bar derivation](issue-199-status-bar.md) identifies the separate status option and its portable presentation ownership.

Use the unchanged [PCMicro RIPtel 3.1 driver](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses below are RVAs from `0x10000000`; the .text file offset is RVA−0xc00. LLVM objdump 19.1.7 and raw PE reads verify these instructions.

## Argument and draw target trace

- Registration at `0x26473–0x2647e` pushes handler `0x24737` and name `CLS` at `0x7e420`. A nonzero argument count sets variable error +0x436 at `0x24746`. With zero arguments the only direct action is `0x24752 → 0x2fcf6` before returning NULL. HARD also calls `0x2fcf6` at `0x168dc`, within its larger reset sequence.
- The helper queries the shared status service at `0x2fd00`. On nonzero, it sends selector 0 at `0x2fd13` before filling and selector 1 at `0x2fd67` afterward. It brackets the draw with caret presentation calls `0x18cf6` and `0x18bf7` at `0x2fd1c` and `0x2fd57`. These are transient presentation steps, outside the portable state contract.
- At `0x2fd24`, instance +0x22 supplies the port system. `0x2fd27–0x2fd29` requests stock object 4 through the `GetStockObject` IAT slot at `0x9640c`. The SDK identifies stock object 4 as BLACK_BRUSH. The returned brush is sent to `FillRect` through IAT slot `0x9667c` at `0x2fd39`. Its rectangle is port system +0x32 and its HDC is field +0x6c of the first port record, from array base at port system +0. The same rectangle/DC are passed to invalidation `0x13c69` at `0x2fd4c`. [Microsoft SDK header](https://github.com/microsoft/win32metadata/blob/main/generation/WinSDK/RecompiledIdlHeaders/um/wingdi.h#L1772-L1778) · [GetStockObject](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getstockobject) · [FillRect](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-fillrect).
- The fill path does not select the current drawing port or consult its viewport, palette, STYLE or W. The use of PORT0's HDC and the physical rectangle is the operation boundary. It does not establish Windows clipping or realized palette behavior. The already accepted fixed RGB black fill from the [HARD profile](../ripscrip/1.3-world-view-virtual-canvas.md#hard-reset-state-and-lifetime) supplies the portable result independently of display implementation.

The portable vector exercises red index 0, an offscreen pixel, the current inactive port, and a saved screen. `m000` has UltraNum value `48 × 64³ = 0xc00000`, so under the [palette storage contract](../ripscrip/2.0-color-modes-and-palettes.md#stored-color-and-palette-publication) the stored red is 48 and the published red is 192. The CLS result is literal black while the palette readback still reports stored red. The prior SCREEN snapshot can subsequently restore the red pixel. Field definitions and ANSI cursor/rendition survive the fill, and the status indicator can be refreshed independently of PORT0.

## Reproduction

```sh
dll=../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
sha256 "$dll"
llvm-objdump19 -p "$dll" | rg 'GetStockObject|FillRect'
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10024737 --stop-address=0x1002475d "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1002fcf6 --stop-address=0x1002fd73 "$dll"
```

Decode raw relative calls and import thunk slots from their PE sections to reproduce the addresses. The 125 bytes of the helper from RVA `0x2fcf6` match the earlier HARD checkpoint. No original execution or native graphical probing.

## Verification

Matched the original DLL and shared HARD helper hashes, nine call sites, five instruction signatures, and both imported API names. Decoded every literal vector line, including the red UltraNum, the offscreen port's flags and extents, and the retained palette and port reply. Checked formatting, 6,842 links before delivery metadata, and git diff. No original client or graphical execution.

## Resume

Publish the [CLS reference](../ripscrip/5.1-text-variables.md#cls), merge the actual PR head into fork stable, and refresh GitHub from 23:04:47Z or earlier before #201.

---

[Research backlog](issue-backlog.md) · [CLS](../ripscrip/5.1-text-variables.md#cls)

## Delivery

[PR #203](https://github.com/bbs-land/remote-imaging-protocol/pull/203) is open for upstream review. Specification commit **a170972c62867522d715af7e25484df72c95cc53**.
