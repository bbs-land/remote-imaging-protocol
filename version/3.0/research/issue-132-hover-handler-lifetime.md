# Issue #132: Resident hover handler lifetime

[Research backlog](issue-backlog.md) · [Mouse fields](../ripscrip/4.2-mouse-fields-and-pointer.md) · [Queries](../ripscrip/5.5-file-transfer-and-queries.md)

## Scope and checkpoint

Started 2026-11-22 on `issue-132-hover-handler-lifetime` from fork stable **6a903af** after [PR #134](https://github.com/bbs-land/remote-imaging-protocol/pull/134) actual head **beea018** was normal-merged with an identical tree. [Issue #132](https://github.com/bbs-land/remote-imaging-protocol/issues/132) asks for membership without handlers, movement versus per-callback handler definition snapshots, deletion/reset/focus/disable/restore lifetime and definition edges/bounds. Preserve #51's absence of click dispatch for stored modes 1–4 and #113 stable-instance traversal/FIELDID/physical-click rules. Protocol documentation only; no original/client/UI/probe execution or bug investigation, no RIPscrip 1.54 changes.

Report body SHA-256 `007085ed2d82b38f39a62c54fb743435f00ff348d0e17788449c944aac3a4ef6`, updated 2026-11-22T02:24:13Z, zero comments at 2026-11-22T02:43:05Z. No newer report at this checkpoint.

## Research next

Read the unchanged original query definition/hover consumers and documented/corpus reset sequences, including TELKILL.FN. Separate recovered definition/consumer facts from explicit membership/continuation/reset completion. Publish callback-count/state vectors and cross-links, validate literal/source bytes and repository checks, then a separate PR and normal merge of its actual head into fork stable. Finish with a complete issue/comment/review/ancestry audit.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
