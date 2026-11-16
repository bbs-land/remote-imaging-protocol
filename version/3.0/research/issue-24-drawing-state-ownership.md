# Issue #24: drawing-state ownership, defaults and text cursors

[Research backlog](issue-backlog.md) · [Data tables](../ripscrip/7.1-data-tables.md) · [Text output](../ripscrip/3.0-text-output-and-fonts.md)

## Scope and source

Started 2026-11-16 after [PR #56](https://github.com/bbs-land/remote-imaging-protocol/pull/56) delivered #23. Branch `issue-24-drawing-state-ownership` starts at `e33a1d7`. [Issue #24](https://github.com/bbs-land/remote-imaging-protocol/issues/24) asks for port versus style cursor/image ownership, environment versus style color-mode/mouse ownership and activation, exact style defaults, backup scope and TEXT_XY cursor updates. Resolve every part before filing a PR.

Original source: RIPtel 3.1 installer [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`; extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The artifact mirror/provenance is described in the backlog. PE base is `0x10000000`; addresses below are RVAs. Original files remain unchanged. Use Capstone 5.0.7 disassembly and native i386 probes with controlled services; no desktop GUI.

## Initial leads and remaining work

- Confirmed earlier: ports have stride `0x78`, image rectangle/flags at `+0x4a..+0x5a` and cursor at `+0x5c/+0x60`; style records have stride `0x61`. This layout alone does not prove activation behavior. Trace original port/style selection and backup consumers before assigning ownership.
- Original drawing initialization starts at `0x6666`. Stores observed at `0x66bd` onward include style words `+0=15`, `+2=0`, bytes `+4=0`, `+5=1`, word `+6=4`, bytes `+8/+9=0`, word `+0x1a=15`, byte `+0x1c=0`, byte `+0x1d=1`, word `+0x1e=0`, byte `+0x20=0`, flags `+0x21=0`, word `+0x22=1`. Identify each field through its setters/consumers and compare per-slot initialization before publishing defaults.
- Style activation lead `0xe6e9`; port selection `0x3393c`; whole-port save/restore `0x219a9/0x22226`. Existing table probes deliberately stub activation and cannot establish activation semantics without extending their boundary.
- Environment records have stride 16; current index is system `+0xa`. #23 proves RGB bits at record `+7` and active configuration `+0x3a`; trace full activation, cursor and protection separately.
- TEXT handler `0x20169`, TEXT_XY `0x20cbc`, common text adapter `0x42870`. Trace anchor stores and post-render cursor gates with concrete fixtures.

Next: identify complete original style initialization/activation and environment ownership, then extend native reproduction to cover alternating selections, backups and cursor updates. No specification edits or completed answer yet.
