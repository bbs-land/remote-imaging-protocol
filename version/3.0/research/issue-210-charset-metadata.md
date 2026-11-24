# Issue #210 — Character-set and language providers

[GitHub issue #210](https://github.com/bbs-land/remote-imaging-protocol/issues/210) · [ANSI terminal profile](../ripscrip/1.4-terminal-ansi-view.md#byte-character-sets-in-riptel-31)

## Scope and checkpoint

RealDeuce filed this issue at 2026-11-24 01:20:19 UTC. The issue had no comments at intake. Use RIPtel 3.1's PC Micro DLL, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, static addresses as RVAs. Callback entries `0x2bb73`, `0x2bb93`, `0x2bc98`, `0x2be34` and `0x2bebe` establish CDCS, CHARSET, DCS, LANG and SDCS respectively.

The shared table access at context offset `0x0e`, 16-byte stride and index offset `0x0a` identify the [environment subsystem](issue-172-local-playback-baud.md#live-rate-and-setter-grammar). These are ENV references. Text windows have a separate subsystem with its own byte-to-glyph state.

## Resume

Finish the selector, protection, initialization, preservation and ANSI-output traces. Update the built-in chapter, index, ENV state description and terminal cross-reference, then verify and deliver the PR.
