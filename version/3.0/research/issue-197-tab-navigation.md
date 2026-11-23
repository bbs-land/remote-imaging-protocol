# Issue #197 — Tab navigation and OPTION(LIST)

## Scope

[RealDeuce report #197](https://github.com/bbs-land/remote-imaging-protocol/issues/197), created 2026-11-23 22:37:52 UTC, body SHA-256 `afd5cca4d1cc954d2b2381e7a49b5fe2a1f327698cd45f3b68d902f5e29acdbb`, zero comments at intake. The 22:40:08Z audit found only #197 uncovered, with all 96 open PR heads ancestral. Started from fork stable `b0aada2e7d5053aee596b2f6f76efa86ab189a31`. Keep the adopted keyboard, pointer and field lifetime contracts. No original client or GUI execution.

## Initial source

Use the [catalogued](../../../reference/rip-tools.md) RIPtel 3.1 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` (592,896 bytes; SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`). Addresses are RVAs from image base `0x10000000`. LLVM objdump 19.1.7 confirms the shared setter, Tab dispatch, focus draw/clear, traversal comparison and five-entry list loop. Next trace live-field mutation callers and option table bytes, corroborate inherited documentation and write the portable protocol rules and vectors.
