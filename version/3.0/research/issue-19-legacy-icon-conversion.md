# Issue #19: legacy icon conversion palette and filesystem effects

[Research backlog](issue-backlog.md) · [Legacy icons](../ripscrip/6.1-content-file-roles.md#legacy-icn-icons-ripscrip-154)

## Scope and primary source

Started 2026-11-16 after [PR #48](https://github.com/bbs-land/remote-imaging-protocol/pull/48) delivered #18. Branch **issue-19-legacy-icon-conversion** starts at **36c17d2**. [Issue #19](https://github.com/bbs-land/remote-imaging-protocol/issues/19) asks which RGB palette becomes the legacy ICN/HIC/MSK conversion’s BMP color table, whether startup and on-demand conversions share it, and whether RIPtel 3.1 actually inherits source deletion and timestamp preservation from the 2.x UI documentation. Establish file orchestration, not just the decoder.

Unchanged original RIPtel 3.1 / driver 3.0.7 `extracted/ripscrip.dll`, 592896 bytes, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, preferred base **0x10000000**. Original installer and [mirror catalogue](../../../reference/rip-tools.md): `artifacts/riptel-3.1-pcmicro/`; [MSI URL](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 **04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c**. Client startup trace will use the separate static UPX-unpacked copy `riptel-unpacked.exe` (605696 bytes, SHA-256 **953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13**), EXE RVAs relative to **0x400000**, per the [header derivation](issue-5-scene-headers.md). Original packed EXE remains unchanged. No GUI/Wine on the user’s desktop; scoped native probes remove task-owned temporary files.

DLL addresses below are RVAs. Parse PE sections before reading bytes: .text offsets = RVA−0xc00, .rdata = RVA−0x1000, .data = RVA−0x1a00; the strings/static conversion table discussed below are **.data**, not .rdata.

## First static trace

Export **RIP_ConvertIconToBmp at 0x6a3c** validates the instance, looks up the supplied name through **0x3f80e** with flags 0x33 and no retrieval, builds a BMP destination name through extension helper **0x65549**, and calls converter **0x2fd73** at **0x6aec**. That converter opens the source, uppercases the destination and opens it without truncation, then writes a 14-byte BMP file header, 40-byte BITMAPINFOHEADER, 64-byte color table and bottom-up 4-bit rows.

The palette write at **0x2ffd6–0x2ffe0** directly supplies **0x7f2e0**, exactly **64 constant bytes** (file offset **0x7d8e0**), without reading current drawing/OS palette state. These are ordinary default EGA RGBQUADs: slot 1 B,G,R = aa,00,00 (RGB 0000aa), slot 6 = 00,55,aa (RGB aa5500), full white at 15. Recover all sixteen values and verify native output before publishing the exact policy. This is a specific fixed conversion table, not evidence of sampling the current desktop palette.

A significant timestamp lead needs verification: the success tail **0x30355–0x303ab** calls 0x6ae60 on the source descriptor, closes output and source via **0x6b490**, then invokes **0x6bf40** with that source descriptor. It copies two fields from the resulting stack stat structure, calls **0x6bd50** on the output, and deletes the source through **0x6b480**, returning AX=1. The native checkpoint below confirms these are fstat/utime/unlink, and that stat-after-close invalidates timestamp preservation. All direct converter calls in the DLL disassembly lead through the export.

## Native checkpoint: palette, raster and filesystem effects

New `run/riptel-icon-conversion-probe DLL` executes original converter **0x2fd73**, export **0x6a3c** and extension helpers **0x65549 / 0x65660 / 0x65766**, close **0x6b490**, handle invalidation **0x70250**, handle lookup **0x702e0**, fstat **0x6bf40** and unlink/remove **0x6b480 / 0x6b450**. Memory-only services replace open **0x6b560**, filelength **0x75790**, read **0x6b1c0**, seek **0x6b100**, write **0x6aed0**, commit **0x6ae60**, utime **0x6bd50**, error report **0x38ddf**, non-retrieving lookup **0x3f80e** and four memory/string primitives **0x68d70 / 0x68910 / 0x65431 / 0x65442 / 0x654a8** (five entry points). IAT controls: CloseHandle **0x965cc**, DeleteFileA **0x965dc**, MessageBoxA **0x966f4** and GetLastError **0x96600**. Original lower CRT functions retain their own error/descriptor behavior. No disk files are created/deleted by the modeled conversion, and no GUI is shown. The wrapper SHA-verifies the DLL, disables cores and removes its temporary build directory automatically.

Initial matrix passes **6,499 checks**, FreeBSD 15.1 amd64 / clang 19.1.7, including widths 1–32, two distinguishable rows, all sixteen indexes, both zeroed and poisoned current-palette structures; original BMP header/palette/bottom-up raster are captured. File effects cover every header/palette/row write failure, every input header/plane read failure, source/destination open failure and source-delete/utime failure.

Fixed output RGB values by slot 0–15:

```text
000000 0000aa 00aa00 00aaaa aa0000 aa00aa aa5500 aaaaaa
555555 5555ff 55ff55 55ffff ff5555 ff55ff ffff55 ffffff
```

These are RGB hex; the 64 file bytes are B,G,R,zero RGBQUADs. Input plane indexes survive conversion. Reprogramming drawing slot 1 to red cannot change the table’s slot 1 RGB **0000aa**; the conversion never samples that state. Later display/palette remapping remains the separate [bitmap rendering contract](issue-8-bmm-masks.md).

The actual normal event order is source open → destination open/create → source flush → destination close → source close → failed fstat → utime attempt → source deletion. **0x6bf40** sees the original closed-descriptor flag and returns **-1 / errno 9 (EBADF)** without writing any of the 36-byte stat structure. The standalone canary test confirms every byte remains untouched. The converter does not test the return; it copies uninitialized stat+0x18/+0x1c access/modification fields into the utime pair, then calls utime on the destination. The exact attempted times are stack residue, not an exact reproducible source timestamp. The probe captures this call and closed/error state without substituting valid times; it does not execute Win32 time-setting with that residue. Conversion still reports AX=1 and attempts deletion even when utime fails. Deletion errors are also ignored, so source survival depends on the filesystem operation succeeding.

Destination **0x8101** means binary/write/create with **no truncation**: original open decoder **0x6b688–0x6b703** maps its masked 0x100 mode to CreateFile disposition **4 / OPEN_ALWAYS**. A pre-existing longer destination retains a stale tail after the newly written BMP body; its embedded bfSize describes the new body. On conversion read/write failure, source is retained and the destination is closed/deleted; output-open failure retains source. This is not atomic replacement.

The original export always requests extension literal **BMP** at **0x79d5c**, including HIC/MSK input; native export cases confirm every one writes TEST.BMP. Do not silently carry over the older ICN→BMP / HIC→BMH / MSK→BMM conversion table as this export’s output naming behavior.

## Client and playback trace in progress

The static unpacked EXE imports RIP_ConvertIconToBmp at VA **0x45b830**, reached through thunk RVA **0x36f8e**. Its direct call is **0x206cf** inside **0x203e0**, after transfer engine **0x24020** returns. The completed-file list at global VA **0x44c0c0** is traversed; extension tests **0x2067b–0x206bf** select ICN/HIC/MSK and pass those names to the same export. Thus this actual conversion path uses the fixed table and defective timestamp tail above. Resolve startup claims against the complete reference scan before publishing.

Original LOAD_BITMAP **0xc569** and LOAD_ICON **0xcb38** normalize `.ICN`→`.BMP`, `.HIC`→`.BMH`, `.MSK`→`.BMM` **before** their one lookup and ordinary BMP display. For example, bitmap normalization **0xc6eb–0xc740**, lookup **0xc781**. A raw legacy file merely present beside a missing normalized bitmap does not reach converter 0x2fd73 in those handler paths. Expand the existing routing fixture to verify all three names and missing cases. Do not call filename normalization itself conversion.

## Rejected readings / harness corrections

- Initial static shorthand mistakenly labeled **0x65424** destination removal; its target **0x75820** actually uppercases the filename. The first captured event sequence (`SOFosTD`, without a destination-delete event) exposed this. The original output-open decoder independently confirms non-truncating OPEN_ALWAYS.
- First probe used an incorrect guessed DeleteFileA IAT slot; gdb showed the unresolved import jump. Correct slot is **0x965dc**, established by disassembling **0x6b450**. The failed wrapper and scoped debug build removed their binaries; no core was retained. This is a harness error, not a driver defect.
- The inherited claim that RIPtel ships no ICN files is too broad: this specific recovered MSI cabinet explicitly lists LORDARM[R]/other LORD legacy ICNs alongside BMPs. `lordarmr.icn` is 50566 bytes, SHA-256 **18dea2a2a2dc7a758936f430bd22023ec290428f0f2704bda56561799c21f65f**, header 7f 02 9d 00 (640×158); its paired BMP is 50678 bytes, SHA-256 **f66bf0ccc57a1a66cf09ffdf2f0728514c5d6eb299dbe848b3a35cb84ce6c562**. `cabextract -l artifacts/riptel-3.1-pcmicro/msi/Data1.cab` proves these are cabinet contents, not newly converted probe output. Correct the source-scope claim without changing assets.

## Next action

Finish complete EXE converter call/reference scan and startup scope, add legacy/missing routing checks, verify the original full-file palette/raster at a larger width, then update the 3.0 reference and cross-version conversion link with explicit historical defects. Add AGENTS.md cautions, run required checks and file #19 only after the startup/on-demand premise is fully reconciled.

---

[Research backlog](issue-backlog.md) · [Legacy icons](../ripscrip/6.1-content-file-roles.md#legacy-icn-icons-ripscrip-154)
