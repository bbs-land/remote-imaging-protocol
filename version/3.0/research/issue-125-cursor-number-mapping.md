# Issue #125: Cursor numbering and presentation

[Research backlog](issue-backlog.md) · [Wire cursor](../ripscrip/4.2-mouse-fields-and-pointer.md#rip_set_mouse_cursor) · [MCURSOR](../ripscrip/5.1-text-variables.md#mcursor)

## Scope and checkpoint

Started 2026-11-21 on `issue-125-cursor-number-mapping` from stable 90819ee after #124 / PR #126 actual head c760134 was normal-merged with an identical tree. [Issue #125](https://github.com/bbs-land/remote-imaging-protocol/issues/125) asks for canonical 1c/MCURSOR numbers and accepted value 7, including storage/presentation, syntax and corpus reconciliation. Preserve #24 ownership/protection/reset, pointer input/capture separation and RIPscrip 1.54. Portable stock equivalents and explicit completion are allowed; no original/client/UI/probe execution or bug investigation.

Initial report SHA-256 `3d1cc88f9f80f1e9acef63213a8eabde05b9dd80fc91bace2398e65e9d34fdc8`, updated 2026-11-21T22:06:50Z; unchanged with zero comments at 22:20:35Z. Previous source location/control fixtures establish cursor state ownership, not original shape identity. Do not treat distinct test handles 1000+n as actual cursor resources.

## Primary documentary and original evidence

The immutable [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, §3.4.2.22 lists 0 arrow, 1 wristwatch, 2 crosshair, 3 I-bar, 4 pointing finger, 5 raised hand, 6 hourglass. §5.5.2 expressly says MCURSOR uses the same numbers. Thus the old corpus-summary “4 busy / 6 hand” conflicts with the primary document; script invocation alone never proves a different mapping.

Original RIPtel 3.1 / RIPscrip 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [artifact mirror](../../../reference/rip-tools.md) is 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Image base 0x10000000; code addresses below are RVAs, `.text` file offset = RVA−0xc00. Resource offsets below use the PE section table, **not** the text subtraction. LLVM objdump and Python read bytes only.

- Initializer `0x344bc–0x345b7` fills eight handle slots at context mouse-system +0x24. Its first branch maps logical 0..7 to group resources **2,7,5,1,3,10,6,4**, calling `0x345b8` at `0x344d4/0x344e1/0x344ef/0x344fd/0x3450b/0x34519/0x34527/0x34535`. Its alternate branch uses system arrow/wait for logical 0/6 and groups **8,14,13,9,11,12** for logical 1/2/3/4/5/7. Decode both arrays before assigning shapes.
- Loader `0x345b8` calls LoadCursorA with the resource module at context +0x72 then +0x31c; if a custom load fails, it requests system arrow 0x7f00. Presenter `0x34635` reads current environment record +8 and selects slot +0x24+4*n, falling back to arrow for a missing handle. It calls SetCursor at `0x34681`; no special “7 means hidden/no change” branch exists.
- Shared setter `0x3468a` stores the byte at environment record +8 only for unsigned word n<8 (`0x34695/0x34699`), then presents at `0x346be` and asks ShowCursor(TRUE) at `0x346c8`. Value 7 therefore stores/selects a real handle slot, independently of capture/input enable.
- MCURSOR `0x34750` accepts no argument for zero or one numeric argument n<8 (`0x347ac`), calling shared setter at `0x347c7`, without environment protection. Wire `0xdc96` checks protection through `0xdcbb` -> `0x3d9e1`, then calls the same setter at `0xdce6`. Its outer guard is n<10, but shared storage accepts only n<8. Do not claim the outer wire path itself rejects 8/9 or investigate its refresh-only legacy path; portable rejection is explicit completion.

## Resource identity and presentation

The DLL contains RT_GROUP_CURSOR (12) and RT_CURSOR (1), language 1033. Each group entry identifies an individual cursor. Individual data begins with two little-endian 16-bit hotspot coordinates followed by a BITMAPINFOHEADER, BGR0 palette, bottom-up XOR pixels and bottom-up AND mask. The logical-7 resources are:

| Resource key (type,id,language) | RVA | File offset | Bytes | SHA-256 |
| --- | --- | --- | --- | --- |
| (12,4,1033) color group | 0x99e5c | 0x8b05c | 20 | `7233cb3a954a8f80399c23e17a58404bb3885906977be8b91f38f4f1c21fa9c3` |
| (1,5,1033) color cursor | 0x99b70 | 0x8ad70 | 748 | `396e159a0ca0283576a11044d8abc4fd631edf572b1b9512cfb1e3ffb50333dc` |
| (12,12,1033) monochrome group | 0x9ac7c | 0x8be7c | 20 | `8a495f17bc472bfc5e6923d9efa687848fac027ad60694f9c3f10a4f7b194924` |
| (1,12,1033) monochrome cursor | 0x9ab48 | 0x8bd48 | 308 | `89068e2f5a1f54cb0fac361b683f1745212aae6f261cacb02d3ba01841f0f05f` |

Both decode to a **slashed-circle unavailable/prohibited symbol**, at 32×32 with hotspot (15,15), respectively 4-bit and 1-bit XOR planes plus the 1-bit AND mask. The DIB height 64 includes both planes; it is not a 32×64 visible cursor. A temporary source-data contact sheet of all fourteen custom resources was inspected and removed immediately, including its task-owned directory. No executable, DLL API, UI, cursor provider, Wine or original client ran. Shape identity is a bounded original-data finding, not a deduction from the prior 1000+n fixture handles.

The other resources visually agree with the documentary 0..6 table: clock/watch, crosshair, I-bar, pointing finger, raised palm and hourglass, plus arrow. Matching alternate monochrome variants agree. Microsoft [LoadCursor](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadcursorw) documents module resource loading versus system cursors; [stock cursor identifiers](https://learn.microsoft.com/en-us/windows/win32/menurc/about-cursors) identify arrow/I-beam/wait/cross/hand/unavailable equivalents. These platform equivalents support portable presentation choices; they do not establish RIP numbering.

Original `menu.fn` (643 bytes, SHA-256 `5bf8861c5b9d7c39798c51a499ca58387e6fcb04dccbe789680f9c77d95ba37d`) contains MCURSOR(6)/MCURSOR(0), so its pair is hourglass/arrow. `fxshwimg.fn` (1,752 bytes, SHA-256 `e5abc6e9b3f32ee5bbeed8ec7c3d50ef7b3f0f375c3ef9a3c7de8a52699858be`) contains wire 1c06/1c00 with the same meaning. Remove the unsupported “4 busy / 6 hand” summary rather than inventing separate wire/variable numbering. These are source bytes, not rendered runs.

## Explicit completion and validation

Adopt **0..7** for both forms, with 7=unavailable/prohibited visible cursor, not hidden/default/no-change. Store its distinct logical value even when an unsupported native backend falls back to arrow. Preserve environment ownership/protection/reset, arrow default and no-argument MCURSOR(0), input/capture/keyboard independence. Reject out-of-range wire and variable values before mutation as nonfatal errors; this resolves the old wire outer-guard ambiguity explicitly. Exact legacy pixels/hotspots, distinct watch versus hourglass artwork and a particular OS API are not requirements; use logical equivalents and retain the stored number.

Published the canonical eight-value table, both accepted-range rules, MCURSOR syntax/index and corpus corrections, plus exact wire/variable/protection/fallback/input examples. Read-only checks pass both artifact hashes, eighteen direct call targets, nine critical instruction sequences, both eight-slot mappings (fourteen custom IDs plus two stock slots), all fourteen resource-group links and DIB plane/header layouts, four logical-7 resource hashes/RVAs/file offsets/hotspots and 64 range/protection/backend arithmetic combinations. The two original script fingerprints and invocation bytes were independently read. Shape identification was reviewed from decoded source pixels, separately from logical setter/protection calculations; no native display parity is claimed.

Repository lint/link/diff checks pass (6,106 links across 251 files before delivery metadata). Refresh started 2026-11-21T22:27:34Z confirms unchanged #124/#125 and zero comments. Publish/consolidate separately, then audit all coverage/feedback/ancestry. The task-owned contact sheet/directory were removed immediately; no original/client/UI/probe execution or bug investigation.

```sh
objdump -d --start-address=0x100344bc --stop-address=0x100346d1 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10034750 --stop-address=0x100347d6 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1000dc96 --stop-address=0x1000dcf1 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

## Delivery

[PR #127](https://github.com/bbs-land/remote-imaging-protocol/pull/127), specification ec6be4d, is open/ready and closes exactly #125. It continues #126 with an issue-only comparison from stable 90819ee. Consolidate its actual source head by an ordinary merge with an exact-tree check, then refresh all coverage/feedback/ancestry. The scoped API payload directory was removed automatically.

---

[Research backlog](issue-backlog.md) · [Wire cursor](../ripscrip/4.2-mouse-fields-and-pointer.md#rip_set_mouse_cursor) · [MCURSOR](../ripscrip/5.1-text-variables.md#mcursor)
