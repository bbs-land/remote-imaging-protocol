# Issue #112: Undefined current button-style state

[Research backlog](issue-backlog.md) · [Button styles](../ripscrip/4.3-buttons.md#initial-and-reset-style)

## Scope and checkpoint

Started 2026-11-21 on `issue-112-undefined-button-style` from fork stable fc9588f. [Issue #112](https://github.com/bbs-land/remote-imaging-protocol/issues/112) follows #27 / PR #61 and #16 / PR #45: specify specific/table reset selection, explicit `1B` writes while the current slot is undefined, subsequent first-use defaulting and protection/query behavior when the already documented current-reset metadata overwrite hides a protected record. Preserve those earlier default/flag/reset answers, outer SOFT's separate selection of zero, 1.54 and separate FastFont research.

## Remaining work

Read the primary reset/selection/style-write/creation/query consumers without running original code or investigating the known obsolete-client reset defect. Track record, definition flag, protection and current index independently, and identify any necessary explicit portable completion. Publish complete state sequences and query results, maintain AGENTS/backlog, validate and file a separate PR, then consolidate its actual head into fork stable. Initial 2026-11-21T15:53:50Z scan finds 56 open RealDeuce issues and this single uncovered report, unchanged body and zero issue comments. Refresh all discussion/inline/review feedback overlapping 2026-11-20T22:17:25Z before publication. Co-author trailers and scoped temporary cleanup remain required.

---

[Research backlog](issue-backlog.md) · [Button styles](../ripscrip/4.3-buttons.md#initial-and-reset-style)
