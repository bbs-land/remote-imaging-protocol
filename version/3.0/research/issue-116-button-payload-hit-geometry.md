# Issue #116: Button payload, validation and hit geometry

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md#rip_button) · [Mouse fields](../ripscrip/4.2-mouse-fields-and-pointer.md)

## Scope and checkpoint

Started 2026-11-21 on `issue-116-button-payload-hit-geometry` from fork stable ebd81bc. [Issue #116](https://github.com/bbs-land/remote-imaging-protocol/issues/116) requests exact plain/icon/clipboard payload splitting and empty fields, safe definition-time versus deferred host validation, and stored/hit endpoint and degenerate/reversed/inactive/clipped PORT0 geometry. Preserve prior #27/#112 style/state, #113 gesture/identity completion and #6/#84/#93/#96 staged host expansion; earlier 1U example must agree with the recovered grammar. Explicit portable completion is allowed; original raster parity is not required.

## Remaining work

Read the primary button constructor/splitter, host staging and geometry/hit consumers, without original/client/UI/probe execution or obsolete-client bug investigation. Distinguish exact normal grammar/source facts from necessary portable validation/geometry choices. Maintain derivation checkpoints and AGENTS/backlog; validate complete examples and all requested states, publish a separate PR and consolidate actual head into fork stable. Initial refresh finds this single uncovered report and zero issue comments. Preserve 1.54, immutable text/assets, separate FastFont research, co-author trailers and scoped temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md#rip_button) · [Mouse fields](../ripscrip/4.2-mouse-fields-and-pointer.md)
