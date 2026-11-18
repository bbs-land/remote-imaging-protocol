# Issue #79: SWITCH_STYLE width and protection flags

[Research backlog](issue-backlog.md) · [SWITCH_STYLE](../ripscrip/7.1-data-tables.md#rip_switch_style)

## Scope and checkpoint

Resolve [#79](https://github.com/bbs-land/remote-imaging-protocol/issues/79) after #78 was delivered in PR #80. Branch `issue-79-switch-style-flags` starts from fork stable **48767dc**, which merges #80 head acc1cb1 without changing its tree. Verify the one-plus-two numeric fields, current-base/fixed-base boundary, final-field shortening, protection order/conflicts, same-slot case, destination domain, slot-zero diagnostics and graphics-disable scope. Existing #24 defaults/ownership and other completed issue notes remain intact.

Read the original protocol first; use original dispatch/handler instructions for undocumented extension semantics. No native/client bug or GUI investigation is authorized. Record documentary rules, recovered extension behavior and genuinely unspecified states separately.

## Next action

Verify physical slot 121, handler 0x46e41, selector 0x394fe and protection helper 0x390e4. Reconcile the language entry and command inventory, preserve the documented shortened zero example if rule 10 permits it, and distinguish diagnostics from rollback. Then complete checks, refresh comments, file one complete PR closing #79 and consolidate into fork stable.

---

[Research backlog](issue-backlog.md) · [SWITCH_STYLE](../ripscrip/7.1-data-tables.md#rip_switch_style)
