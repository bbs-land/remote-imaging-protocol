# Issue #4: column, article and overflow derivation

[Research backlog](issue-backlog.md) · [Current column reference](../ripscrip/3.1-column-text-system.md)

## Status and questions

Started 2026-11-16 for [issue #4](https://github.com/bbs-land/remote-imaging-protocol/issues/4), branch `issue-4-column-overflow`. No resolution PR yet; investigation is incomplete. The branch starts at #3’s delivered branch (PR #29) to reuse the primary-source tools and workflow. If #29 remains unmerged, a PR against upstream main will include that dependency; identify it explicitly and compare the issue-specific commits separately.

Required answers: `1e` flags and `cc`; article/column ranges and numbering scope; flow order; retained content and page position after region redefinition; `$OVERFLOW(article,direction[,setverbose])$` results, side effects, notifications and reset rules; expected NEWS.RIP/NEWSPAPR.RIP states before/after navigation, end and reset. `1A` identification and wire layout remain in issue #2; trace its effects only as needed for this contract.

## Sources and reproduction

Use the exact driver recovered for [issue #3](issue-3-extended-font-style.md#artifact-recovery-2026-11-16): RIPtel 3.1 MSI, `ripscrip.dll` SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, under external mirror `artifacts/riptel-3.1-pcmicro/extracted/`. All code addresses in this note are RVAs; the preferred image base is `0x10000000`. Keep artifacts unchanged. Use `run/riptel-inspect` for fingerprint/dispatch/bytes and original-instruction probes for independent state observations. Any GUI work requires the private-display isolation in AGENTS.md.

Initial queue refresh confirms #3 has complete PR #29 and #4 is the oldest remaining issue. Existing column reference is corpus-based and labels bit 0 and `cc` as hypotheses; none of these hypotheses are evidence of driver behavior.

The external RIPlib source offers leads: `1A` handler RVA `0xdc58` calls article selection at `0x3c399`; context +0x2a points into the article system. Verify all leads in the identified artifact. RIPlib’s current simplified overflow implementation is not a primary source.

## Next action

Extract `1e`, `1T`, `1E`, `1t` dispatch descriptors; trace column creation and article selection, then locate the OVERFLOW and RESET macro handlers by string references. Map state ownership before designing navigation probes. Keep incomplete findings here rather than publishing a partial contract.

---

[Research backlog](issue-backlog.md) · [Current column reference](../ripscrip/3.1-column-text-system.md)
