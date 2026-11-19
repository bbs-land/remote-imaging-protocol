# Issue #97: synthetic outline effects and decoration boundary

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)

## Scope and checkpoint

Resolve [#97](https://github.com/bbs-land/remote-imaging-protocol/issues/97) on `issue-97-outline-effects-boundary` from fork stable **1407b59**, after #96/PR #98 was completed and consolidated. Last comment refresh 2026-11-19 23:49:37 UTC finds no #96/#97 comments or newer discussion/inline feedback.

Preserve adopted effect flags/q accumulation, glyph bytes/fill/hollow stamp, logical metrics, anchor/additive displacement, cursor/cache and W contracts from #3/#22/#86/#91/#77. Explicitly distinguish mandatory effect/state/composition behavior from renderer-defined solid bold/italic/decorative ink geometry; address coordinate freedom, metric bbox B, local-versus-string decoration paths, blank/missing/negative-span handling, shadow membership, W grouping, pen/pattern independence and ordering. The report expressly permits renderer freedom without pixel reconstruction. No client/UI/native routines/device probes or bug investigation.

## Sources and next action

Use preserved ALPHA 4 font-effect intent, the existing original #3/#22 derivations and adopted #86/#91 portable metric/placement policy. Do not infer a solid-bold thickness from the separate recovered hollow stamp or exact decoration lines from tentative font alignment zones. State a complete rendering-freedom boundary and concrete allowed-profile examples, then validate documentation and file one complete PR closing #97. Consolidate its actual head into fork stable; retain Codex trailers and scoped temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)
