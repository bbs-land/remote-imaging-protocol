# Issue #8: BMM masks and compositing

[Research backlog](issue-backlog.md) · [Inherited icon formats](../../2.0/techspecs/3.0-icon-formats.md#bmm---icon-masks) · [3.0 bitmap commands](../ripscrip/4.0-icons-and-bitmaps.md)

## Status and scope

Started 2026-11-16 for [issue #8](https://github.com/bbs-land/remote-imaging-protocol/issues/8), branch `issue-8-bmm-masks`, based on `a6da898` (delivered #7 / PR #34). GitHub refreshed after that delivery confirms #8 is the oldest RealDeuce issue without a complete resolution PR. Investigation incomplete; no resolution PR yet.

Required answers: accepted BMM pixel depths; bit/index versus resolved palette RGB; mask polarity; source/mask composition order under COPY/XOR/OR/AND/NOT; dimensions and missing/mismatched behavior; interaction with BMP transparent-color flags; and an independent small icon/mask pixel oracle. Do not prescribe a plausible 1-bit/nonzero-index convention from missing corpus examples. Establish whether a BMM is automatically consumed as an attached mask at all, or only an explicitly loaded bitmap using ordinary put modes.

## Primary evidence and starting points

Use the unchanged RIPtel 3.1 DLL / RIPscrip driver 3.0.7, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred image base **0x10000000**; all DLL addresses below are RVAs unless stated otherwise. [Issue #3 recovery/provenance](issue-3-extended-font-style.md#artifact-recovery-2026-11-16). The static unpacked original client retains the identity in [issue #5](issue-5-scene-headers.md); EXE addresses are relative to **0x400000**.

The inherited 2.0 format page records BMM as the renamed MSK BMP container and leaves its raster convention unverified. No BMM has been recovered from the known installs; that absence does not establish its decoder or mask semantics. Original 1.54 manual AND/OR stamping is a lead, not an automatic 3.0 mask contract.

DLL extension constants near RVA `0x7a8e4–0x7a914` contain `.MSK`, `.BMM`, `.HIC`, `.BMH`, `.ICN`, `.BMP`. Trace every BMM cross-reference, the original load-icon/load-bitmap handlers, button bitmap display and generic BMP decoder/compositor before inferring any automatic paired-mask behavior. Derive accepted headers/depths and transparent-color handling from executable branches, with controlled file/GDI services where needed.

No GUI/Wine is authorized on the desktop: use native/static probes. If actual GDI execution becomes necessary, follow AGENTS.md’s private Xvfb plus task-specific Wine prefix and explicit DISPLAY rule. Every new commit includes Codex co-authorship.

## Next action

Capture exact dispatch/extension references and BMP decoder/compositor entry points. Trace explicit and button loads through file lookup, depth/palette conversion, size checks and raster operations. Preserve incremental derivations and distinguishing fixture expectations before publishing any mask rule.

---

[Research backlog](issue-backlog.md) · [Inherited icon formats](../../2.0/techspecs/3.0-icon-formats.md#bmm---icon-masks) · [3.0 bitmap commands](../ripscrip/4.0-icons-and-bitmaps.md)
