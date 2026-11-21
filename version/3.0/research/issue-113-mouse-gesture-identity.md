# Issue #113: Mouse gesture commitment and field identities

[Research backlog](issue-backlog.md) · [Mouse fields](../ripscrip/4.2-mouse-fields-and-pointer.md) · [Host identities](../ripscrip/5.0-host-commands.md#mouse-field-identities---idn)

## Scope and checkpoint

Started 2026-11-21 on `issue-113-mouse-gesture-identity` from stable 9a7dd05 after [PR #114](https://github.com/bbs-land/remote-imaging-protocol/pull/114) actual head ff10494 was consolidated with an identical tree (initial merge 8b3c171, final workflow cleanup 9a7dd05). [Issue #113](https://github.com/bbs-land/remote-imaging-protocol/issues/113) asks for shared mouse/button press/release commitment and drag behavior, gesture ownership across overlap/definition/deletion/reset/restore, ID prefix grammar and FIELDID lifetime. Explicit portable policy is allowed where original behavior does not establish one. Preserve #51's separate no-deferred-TW/PORT-click-dispatch answer, existing hit geometry/limit/rendering contracts and 1.54.

## Remaining work

Read primary event/action/hover/identity consumers without executing original code or investigating obsolete-client defects. Distinguish exact original normal-event facts from safe mutation/lifetime completion choices. Publish state transitions and grammar/lifetime examples, validate, file a separate PR and consolidate its actual head. Retain derivation checkpoints, AGENTS/backlog, co-author trailers and scoped temporary cleanup. Separate FastFont research remains excluded.

---

[Research backlog](issue-backlog.md) · [Mouse fields](../ripscrip/4.2-mouse-fields-and-pointer.md) · [Host identities](../ripscrip/5.0-host-commands.md#mouse-field-identities---idn)
