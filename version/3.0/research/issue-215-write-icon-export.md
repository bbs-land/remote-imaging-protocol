# Issue #215 — WRITE_ICON export flags and temporary assets

[GitHub issue #215](https://github.com/bbs-land/remote-imaging-protocol/issues/215) · [RIP_WRITE_ICON](../ripscrip/4.0-icons-and-bitmaps.md#rip_write_icon) · [shared temporary-file trace](issue-21-image-style.md#received-file-cleanup-and-encoded-stream-checkpoint)

## Scope and checkpoint

RealDeuce filed this at 2026-11-24 01:57:36 UTC. No comments were present at intake. Use RIPtel 3.1's PC Micro DLL SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8` and unpacked EXE SHA-256 `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. Read the wire handler at `0xdd67`, name writer at `0x27930` and export at `0x27a90` statically.

## Resume

Read the export and temporary-registration call graphs. Specify the filename, pixel-source, resampling and cleanup contract for supported bare-name requests, with positive geometry vectors.
