# Issue #25: port-query argument grammar

[Research backlog](issue-backlog.md) · [PORTW/PORTH](../ripscrip/5.1-text-variables.md#portw) · [PORTY1](../ripscrip/5.1-text-variables.md#porty1)

## Scope and source

Started 2026-11-16 after [PR #57](https://github.com/bbs-land/remote-imaging-protocol/pull/57) delivered #24. Branch `issue-25-port-query-grammar` starts at `4be4e3a`. [Issue #25](https://github.com/bbs-land/remote-imaging-protocol/issues/25) asks whether PORTW accepts a two-field port/domain shorthand, which three-field grammar PORTH uses, and whether PORTY1's viewno/coordtype summary is real. Geometry changes are not requested.

Primary artifact: original RIPtel 3.1 extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, from [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), installer SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`. PE base `0x10000000`; addresses are RVAs. Original files remain unchanged; native Unix probes only, no GUI. Capstone 5.0.7 / clang 19.1.7 / FreeBSD 15.1. The [earlier #10 derivation](issue-10-port-geometry.md#unexpected-original-portwporth-query-defect) established the original width/height callback inversion but deferred this grammar question.

## Initial primary trace

- Shared decoder `0x2f743–0x2f862` accepts 0..3 arguments in fixed order **port, type, domain**. Defaults are current port / PORT / WORLD. Nonempty optional fields can only be omitted from the end. Keywords use case-insensitive comparisons. First field is CUR or a decimal slot accepted by `0x987b` (digits only, 0..35). Second is exactly PORT or VIEW. Third is exactly WORLD or DEVICE. No branch interprets DEVICE/WORLD in the second field.
- Original registration gives PORTH `0x2f642`, PORTW `0x2f863`, PORTX0 `0x2f95e`, PORTX1 `0x2fa45`, PORTY0 `0x2fb2a`, PORTY1 `0x2fc11`. They all call the same decoder; no alternate PORTY1 parser exists. Original registration name pushes are `0x26157/0x2616f/0x26186/0x2619e/0x261b6/0x261ce` respectively.
- All six callbacks invert the decoder-success gate, not only width/height. PORTY1 `0x2fc37–0x2fc4a` rejects AX=1, sets variable error and returns NULL. Invalid decoder input can fall through to calculation with partially uninitialized locals. Do not execute invalid forms through those callbacks as a grammar oracle; test the original decoder directly. Confirm valid forms through the complete macro pipeline.

Next: expand native decoder matrices for all selectors/optional prefixes/casing, explicitly reject shorthand/misordered/extra fields, assert all six callback identities and valid-form expansion failures, then correct the summaries/syntax and clearly retain the shipped callback defect.
