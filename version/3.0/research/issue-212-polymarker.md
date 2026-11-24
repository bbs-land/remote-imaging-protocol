# Issue #212 — PolyMarker projection and composition

[GitHub issue #212](https://github.com/bbs-land/remote-imaging-protocol/issues/212) · [earlier source report #2](https://github.com/bbs-land/remote-imaging-protocol/issues/2) · [RIP_POLY_MARKER](../ripscrip/2.1-lines.md#rip_poly_marker)

## Scope and checkpoint

RealDeuce filed the report at 2026-11-24 01:43:46 UTC. The report had no comments at intake. The original RIPtel 3.1 DLL SHA-256 is `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8` and its image base is `0x10000000`. Issue #2 is an earlier source report that establishes the name, wire widths, bounds and locations of the contour data. It covers additional topics independently.

## Resume

Extract the descriptor data, verify rotation and per-flag drawing routes, define portable composition and check independent vectors before delivery.
