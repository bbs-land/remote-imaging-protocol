# Issue #214 — Bounded single-string outline fitting

[GitHub issue #214](https://github.com/bbs-land/remote-imaging-protocol/issues/214) · [RIP_BOUNDED_TEXT](../ripscrip/3.0-text-output-and-fonts.md#rip_bounded_text)

## Scope and checkpoint

RealDeuce filed the issue at 2026-11-24 01:51:23 UTC. No comments were present at intake. Use static source reads of RIPtel 3.1 DLL SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Native RVA `0x1a0da` handles level-zero slot 1. The handler uses the FastFont preparation and outline cache machinery documented for ordinary text, plus a separate temporary size search.

## Resume

Trace the geometry, setup, width/height measurements, point-size correction and final draw against the existing outline contracts. Then document the portable fit and state boundary with independent metrics and update the command and corpus summaries.
