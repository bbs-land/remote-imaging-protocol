# Issue #208 — Legacy image suffix wording

[GitHub issue #208](https://github.com/bbs-land/remote-imaging-protocol/issues/208) · [preceding source trace for #19](issue-19-legacy-icon-conversion.md#completed-client-and-playback-trace)

## Scope and checkpoint

The report was filed by RealDeuce at 2026-11-24 00:58:12 UTC. The verified issue body SHA-256 is `bca082faf196701206334e7c14f13537990a29053175641d0a25f25ffeea55d5`. No comments were present at intake. The command entries and section 6.1 retained unqualified wording from Alpha 4 after [PR #49](https://github.com/bbs-land/remote-imaging-protocol/pull/49) documented the RIPtel 3.1 alias rule.

## Derivation

Use the [completed trace and prior native matrix](issue-19-legacy-icon-conversion.md#completed-client-and-playback-trace). The bitmap handler starts at DLL RVA `0xc569`, rewrites at `0xc6eb–0xc740`, and looks up the result at `0xc781`. The icon handler starts at `0xcb38` and follows the same selection. The matrix exercised both handlers, all three suffixes in upper and lower case, and present and missing normalized resources. In every case each handler performed one lookup with the normalized name. For example, `FRAME.hic` selects `FRAME.BMH`; a missing `FRAME.BMH` follows normal missing-resource handling, even if `FRAME.hic` exists. The unrelated explicit suffix case keeps the specified suffix, and the inherited omission default is `.BMP`.

The RIPtel 3.1 client's post-transfer call at EXE RVA `0x206cf` invokes the exported converter for legacy files selected for conversion. That export always asks for a `.BMP` output name. Keep the two events separately named in the reference and link the transfer command to the converter paragraph. The [package provenance](issue-19-legacy-icon-conversion.md#rejected-readings--harness-corrections) also establishes that some recovered LORD assets shipped as legacy ICNs alongside BMPs. This reconciliation relies on existing original-source and native evidence.

## Resume

Reconcile the load-command entries and the file-role summary, update inbound section links, and check the resulting documentation and delivery ancestry.
