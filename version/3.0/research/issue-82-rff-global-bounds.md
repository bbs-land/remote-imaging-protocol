# Issue #82: FastFont global bounding-box fields

[Research backlog](issue-backlog.md) · [FastFont format](../../2.0/techspecs/3.2-fastfont-fonts.md)

## Scope and checkpoint

Resolve [#82](https://github.com/bbs-land/remote-imaging-protocol/issues/82) on branch `issue-82-rff-global-bounds` from fork stable **a529850**. The report identifies metrics words +0x16/+0x18/+0x1a/+0x1c as signed xmin/ymin/xmax/ymax, with reordered copies in RFF/FF1 headers. Confirm field identity/signedness through original metric consumers and framed glyph-record unions, correct the 2.0/3.0 labels, and explain the horizontal reference used with flag 0x400 without changing resolved font rendering behavior.

Refresh 2026-11-19 01:42:05 UTC finds no #82 comments or later repository issue/inline comments after the preceding delivery. All prior issue PRs remain separate. Use original byte inspection and read-only instruction tracing; do not execute fonts/client code, launch GUI probes or investigate obsolete client bugs. Record original artifact hashes, offsets/RVAs, reproducible comparisons, derivation and any limits incrementally.

## Next action

Read the font format and existing metric derivations, use repository inspection tooling where available, compare all original bundled font metrics/header copies against valid framed glyph bboxes, then trace signedness and realized-bounds/spacing consumers. Publish exact mappings and distinguishing examples; format/lint/check links, refresh feedback, file a complete PR closing only #82, and consolidate into fork stable. Retain co-author trailers and scoped temporary-file cleanup.

---

[Research backlog](issue-backlog.md) · [FastFont format](../../2.0/techspecs/3.2-fastfont-fonts.md)
