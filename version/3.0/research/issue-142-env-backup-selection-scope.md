# Issue #142: ENV backup and foreign selections

[Research backlog](issue-backlog.md) · [Environment table](../ripscrip/7.1-data-tables.md#environment-table)

## Scope and checkpoint

Started 2026-11-22T08:12:09Z on `issue-142-env-backup-selection-scope` from fork stable **73dfe86c6601990e931237984712197c354999fd**. The in-session ten-minute check detected [#142](https://github.com/bbs-land/remote-imaging-protocol/issues/142) at 08:09:49Z, after creation at 08:07:54Z. Updated 2026-11-22T08:07:54Z, zero comments, body SHA-256 `3347a27f258e089320499ee9de79796ff1fd3a24b5f38d732cac4d9fea7775c1`. Next check is due about 08:19:49Z. The user explicitly confirmed research-and-PR processing for new reports; monitoring requires this active session, without a persistent scheduler.

This is editorial reconciliation with accepted #24 / PR #57 ENV ownership and 7.1/7.2 capture scope, separate from #140 / PR #141. Remove/qualify the inherited five foreign-selection bullets in SENV and clarify RENV's activation scope. Preserve existing WORLD, Base Math, coordinate width, color precision, pointer and stored baud-emulation rules, including existing activation sequencing. No new transaction, original/client/probe/bug investigation, tests or 1.54/text/assets changes.

## Accepted sources and derivation

Read these files at starting stable 73dfe86 (reproduce with `git show 73dfe86:<path>`):

- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `cfed0dbcc6b6ecb62d6b2b24ca0496fbaf70093c48b135fb58c7447a25aed640`.
- `version/3.0/ripscrip/7.1-data-tables.md`: SHA-256 `b402c0b7d2336ef3af52911843fb716b5670d8ba9f84712baec60d30cad15ca9`.
- `version/3.0/ripscrip/7.2-data-backup-areas.md`: SHA-256 `31ceb1384ad7f3e5a4d7f7cdb90f375fd9f6886213cdb42369b51b371ea95a6e`.
- `version/3.0/ripscrip/9.1-text-variable-reference.md`: SHA-256 `191fc89f0d73bb07fd0c62a6022a5f903a9b7d3aaba12f2010b425f9f820b01e`.
- `version/3.0/research/issue-24-drawing-state-ownership.md`: SHA-256 `d1094046ecff37fc17326823a7aa18a4dc788740c191daf307039b4f79e3f292`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

The accepted [ENV ownership section](../ripscrip/7.1-data-tables.md#environment-table) explicitly separates STYLE/BUT/PORT/TW/PAL selectors and records from ENV. The [ENV save area](../ripscrip/7.2-data-backup-areas.md#environment-table-save-area) captures all 36 ENV records and the current ENV index. The [#24 derivation](issue-24-drawing-state-ownership.md#ownership-and-switching-observations) already records the original selector, no foreign selection switches and the existing setter-before-index sequencing defect. The documentary baud-emulation setting remains under [BAUDEMUL](../ripscrip/5.1-text-variables.md#baudemul); no new activation or transport behavior is inferred.

ALPHA 4's broad environment/SENV lists include the five foreign current indices (context lines 1925 onward and SENV §5.10.19 lines 17506 onward). The later accepted ownership boundary supersedes those bullets in the corrected 3.0 reference; preserve earlier text verbatim. SENV captures ENV-owned values/table and current ENV index. RENV restores/activates that ENV family under the existing selector rules, while the five foreign selectors retain their live values. Other table records and mouse fields are not restored by the ENV component. This does not suppress the existing effects of activating WORLD/math/color/pointer configuration or replace the inherited stored baud setting with a new rule.

## Next action

Correct the SENV capture list and RENV activation paragraph, link the authoritative table/backup/activation/baud entries, and align ENV index summaries. Add a defined-state example showing restored ENV index alongside unchanged foreign selections, without asserting a new atomic cross-table activation or byte-perfect ENV result beyond existing sequencing rules. Verify six source hashes, selected-index example and absence of unqualified foreign-capture claims, then full repository formatting/lint/link/diff checks. Publish separately, consolidate actual head into fork stable, audit feedback/coverage/ancestry and continue ten-minute checks.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
