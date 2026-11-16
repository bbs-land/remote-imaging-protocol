# Issue #19: legacy icon conversion palette and filesystem effects

[Research backlog](issue-backlog.md) · [Legacy icons](../ripscrip/6.1-content-file-roles.md#legacy-icn-icons-ripscrip-154)

## Scope and primary source

Started 2026-11-16 after [PR #48](https://github.com/bbs-land/remote-imaging-protocol/pull/48) delivered #18. Branch **issue-19-legacy-icon-conversion** starts at **36c17d2**. [Issue #19](https://github.com/bbs-land/remote-imaging-protocol/issues/19) asks which RGB palette becomes the legacy ICN/HIC/MSK conversion’s BMP color table, whether startup and on-demand conversions share it, and whether RIPtel 3.1 actually inherits source deletion and timestamp preservation from the 2.x UI documentation. Establish file orchestration, not just the decoder.

Unchanged original RIPtel 3.1 / driver 3.0.7 `extracted/ripscrip.dll`, 592896 bytes, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, preferred base **0x10000000**. Original installer and [mirror catalogue](../../../reference/rip-tools.md): `artifacts/riptel-3.1-pcmicro/`; [MSI URL](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 **04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c**. Client startup trace will use the separate static UPX-unpacked copy `riptel-unpacked.exe` (605696 bytes, SHA-256 **953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13**), EXE RVAs relative to **0x400000**, per the [header derivation](issue-5-scene-headers.md). Original packed EXE remains unchanged. No GUI/Wine on the user’s desktop; scoped native probes remove task-owned temporary files.

DLL addresses below are RVAs. Parse PE sections before reading bytes: .text offsets = RVA−0xc00, .rdata = RVA−0x1000, .data = RVA−0x1a00; the strings/static conversion table discussed below are **.data**, not .rdata.

## First static trace

Export **RIP_ConvertIconToBmp at 0x6a3c** validates the instance, looks up the supplied name through **0x3f80e** with flags 0x33 and no retrieval, builds a BMP destination name through extension helper **0x65549**, and calls converter **0x2fd73** at **0x6aec**. That converter opens the source, removes an existing destination, opens a new destination, writes a 14-byte BMP file header, 40-byte BITMAPINFOHEADER, 64-byte color table and bottom-up 4-bit rows.

The palette write at **0x2ffd6–0x2ffe0** directly supplies **0x7f2e0**, exactly **64 constant bytes** (file offset **0x7d8e0**), without reading current drawing/OS palette state. These are ordinary default EGA RGBQUADs: slot 1 B,G,R = aa,00,00 (RGB 0000aa), slot 6 = 00,55,aa (RGB aa5500), full white at 15. Recover all sixteen values and verify native output before publishing the exact policy. This is a specific fixed conversion table, not evidence of sampling the current desktop palette.

A significant timestamp lead needs verification: the success tail **0x30355–0x303ab** calls 0x6ae60 on the source descriptor, closes output and source via **0x6b490**, then invokes **0x6bf40** with that source descriptor. It copies two fields from the resulting stack stat structure, calls **0x6bd50** on the output, and deletes the source through **0x6b480**, returning AX=1. Identify each CRT function before claiming successful timestamp preservation: the apparent stat-after-close order could invalidate the older UI promise. Error returns are not checked in this tail. All direct converter calls in the DLL disassembly currently lead through the export; on-demand lookup may call it via an indirect export or separate route that still needs tracing.

## Next action

Identify CRT close/fstat/utime/delete semantics and trace all on-demand legacy conversion paths, then original EXE startup conversion. Execute the original converter with controlled in-memory descriptors, byte-exact RGB/pixel captures and success/failure event ordering. Determine actual timestamp outcome without replacing the failed stat with a successful fixture. Update implementor reference and cross-version scope only when all issue questions are answered.

---

[Research backlog](issue-backlog.md) · [Legacy icons](../ripscrip/6.1-content-file-roles.md#legacy-icn-icons-ripscrip-154)
