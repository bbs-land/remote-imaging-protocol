# Issue #91: ordinary outline anchor and justification

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)

## Scope and checkpoint

Resolve [#91](https://github.com/bbs-land/remote-imaging-protocol/issues/91) on `issue-91-outline-anchor-placement` from fork stable **756b285**, after #90/PR #92 was completed and consolidated. The last comment refresh at 2026-11-19 19:10:05 UTC finds no #90/#91 comments or newer discussion/inline feedback.

Preserve adopted #3 fields/additive displacement, #22 raster/mapping, #24 cursor stores, #82 global bbox labels and #86/PR #88 advance/cache policy. Define zero-displacement glyph origin, all sixteen independent rotations, start/end/center and selector 3, signed spacing totals and midpoint rounding. The issue explicitly permits a portable policy rather than pixel reproduction. Buttons, flowed text, classic fonts and language 1.54 are outside scope. Do not investigate old client bugs or run client/UI/probes.

## Next action

Check the existing primary-source/format evidence and distinguish glyph design origin, typographic alignment fields, realized bbox and cursor. State a complete explicit placement policy where zero-baseline behavior is not established, with independently calculated asymmetric-bounds/unequal-DPI/positive-and-negative-total vectors. Publish a separate complete PR closing #91, consolidate its actual head into fork stable and refresh feedback/coverage. Preserve co-author trailers and scoped temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)
