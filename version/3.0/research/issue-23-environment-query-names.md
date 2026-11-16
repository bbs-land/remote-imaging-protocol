# Issue #23: COLORMODE sentinel, WORLD names and environment ranges

[Research backlog](issue-backlog.md) · [COLORMODE](../ripscrip/5.1-text-variables.md#colormode) · [WORLD](../ripscrip/5.1-text-variables.md#world)

## Scope and artifact

Started 2026-11-16 after [PR #55](https://github.com/bbs-land/remote-imaging-protocol/pull/55) delivered #22. Branch **issue-23-environment-query-names** starts at **e5ed876**. [Issue #23](https://github.com/bbs-land/remote-imaging-protocol/issues/23) asks for the exact undefined COLORMODE reply; whether WORLDX/WORLDY are shipping aliases; corrected WORLDH/WORLDW syntax/examples; and consistent presentation of world-coordinate and RGB-bit restrictions. No resolution PR yet.

Use unchanged RIPtel 3.1 / driver 3.0.7 `extracted/ripscrip.dll`, 592896 bytes, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, base **0x10000000**, from the [catalogued mirror](../../../reference/rip-tools.md), [MSI source](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi) SHA-256 **04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c**. All addresses below are DLL RVAs; .text file offset = RVA−0xc00, .data = RVA−0x1a00. Prefer original native callbacks/registration and owned environment records; no GUI or real file mutation is needed.

## Original registration and first callback traces

COLORMODE registration **0x26bd6–0x26be1** names **0x17eac**; WORLD **0x26b30–0x26b3b** names **0x8d5e**; WORLDH **0x261f9–0x26204** names **0x8fe0**; WORLDW **0x26211–0x2621c** names **0x91e8**. WORLDX and WORLDY NUL-terminated strings are absent from the DLL. Absence alone is not full alias proof; check the entire original registration output and ordinary lookup behavior with the existing macro probe.

COLORMODE's one-argument numeric query checks original environment definition helper **0x3d6f9**; undefined branch **0x17fb8–0x17fc6** returns constant **0x7a100**, previously byte-verified as **-1**. Zero arguments / CUR query reads current environment's byte +7 directly. Records have **stride 16**, definition byte +0, width word +1, height word +3, base math byte +5, RGB bits byte +7; environment system current index is +0xa. Validate full outputs and no state/definition mutation natively.

## Range/policy discrepancy requiring investigation

Do **not** merely replace the inherited macro tables with the wire-setter restrictions before following actual consumers. Original COLORMODE **0x180af–0x180c5** validates **1..8**, stores the requested byte in the target environment, and only for a current target calls **0x3e943**. The latter may reject non-eight-bit active modes, but that would not automatically undo the earlier stored state. Query and active-mode restrictions may differ.

WORLD **0x8f6a–0x8f9b** compares dimensions against **65535**, with an AND-shaped rejection path (rejects only if both exceed), stores low words, then calls **0x3e9d4** only for the current target. The macro's explicit range is not evidently 1..32767. WORLDH/WORLDW have analogous 65535 validation and an apparently inverted CUR/set-target branch (**0x90e8–0x914d / 0x92f0–0x9355**) that needs native tests before documenting. These are unverified static leads, not published contracts. Investigate zero, 32767/32768, 65535/65536, asymmetric oversized WORLD values, current versus noncurrent storage and active-setter failures.

Next extend the existing native macro probe with original registration, definition, callbacks and controlled downstream activation while preserving target storage/range validation; then execute active setters separately if necessary. Record parser-reachable results and distinguish returned values, stored environment state, active world/RGB state, reports and variable-error flags. All requested naming/sentinel/range corrections remain one issue; do not advance until complete.

---

[Research backlog](issue-backlog.md) · [WORLD](../ripscrip/5.1-text-variables.md#world)
