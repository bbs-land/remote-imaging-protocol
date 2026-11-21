# Issue #120: Selective mouse-field deletion and painting

[Research backlog](issue-backlog.md) · [Selective deletion](../ripscrip/4.2-mouse-fields-and-pointer.md#rip_kill_enclosed_mouse_fields)

## Scope and checkpoint

Started 2026-11-21 on `issue-120-selective-field-deletion` from stable 833aa55 after #119 / PR #122 actual head 4a7f8d6 was normal-merged with an identical tree. [Issue #120](https://github.com/bbs-land/remote-imaging-protocol/issues/120) requests exact 1/2/4 predicates and union/type filters, selector WORLD/origin/empty/disabled context, optional painting 32/64/128/unused flags and failure ordering. Preserve adopted stored half-open hit geometry, stable-instance capture/hover lifetime and drawing-pattern contracts. Portable completion is allowed; no original/client/UI/probe execution or obsolete-client defect investigation.

## Primary sources and bounded findings

Original RIPtel 3.1 / RIPscrip 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Base 0x10000000; addresses below are RVAs; `.text` file offset = RVA−0xc00. LLVM objdump 19.1.7 / Python read bytes only. Immutable [ALPHA 4](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, §3.4.2.11 supplies flags 1/2/4, exclusions 8/16, optional painting 32/64/128, expressly says 64 overrides 32 and describes the selector interior with 128. #113/#116 already adopt stable field instances and half-open stored hit geometry. No original/client/UI/probe/renderer/callback execution or defect investigation.

Wire handler `0xc474–0xc53d` orders WORLD axes through `0x3112e` at `0xc4bf/0xc4cf`, converts both pairs through `0x31084` at `0xc4e0/0xc4f1`, constructs a rectangle, normalizes it at `0xc513 -> 0x2530b`, and calls deletion helper at `0xc52a -> 0x12d63`. It does **not** add a viewport origin or consult PORT0 activation, so the selector is absolute screen/device after current ENV WORLD conversion. The unrelated current/offscreen port does not supply a selector origin. Preserve the global protocol-disabled/command-parsing rules separately.

Helper `0x12d63` returns immediately for no region bits and processes fields newest first. Per-field helper `0x12e27` copies field +0x10..+0x1c (interaction rectangle), increments far edges at `0x12e57/0x12e5a`, filters simple/button type using record flag bit 4 and exclusions 8/16, then calls old rectangle predicates `0x25337/0x253f5`. Flag 4 uses the complement of the containment test, not a proven half-open disjoint test. Those predicates use old point/margin/corner conventions and do not establish a coherent complete half-open C/P/O classification, especially for a field covering the selector. Do not reproduce/investigate their discrepancies; complete the documentary contained/intersecting/outside classes explicitly.

Per-field paint at `0x12ed1 -> 0x12de2` precedes delete `0x12edd -> 0x27eee`, and the paint return is not a deletion permission. The paint helper tests 32 before 64 and requests stock brush 4 for 32, versus a current brush handle for 64. This is not proof of the draft's 64-priority/palette-index-0 meaning. Outer helper has additional 128 whole/interior/complement paths (including `0x12ef1`); it also gates on live count/type combinations. None of that supplies the requested safe match-independent once-only whole-selector paint policy. The explicit documentary precedence/interior rule takes priority; no native painting parity is claimed.

## Explicit portable classification and ordering

Adopt C=field wholly contained in selector, equal bounds included; P=positive-area intersection but **not C**, including a field covering the selector; O=disjoint (touch-only included). These partition all nonempty stored hit rectangles. Flag 1 selects C, 2 selects P (not C), 4 selects O; combinations are union, so 3 is all overlapping, 5 excludes crossing, 6 is not-contained, 7 all. Type exclusions filter only the deletion set. This reconciles the draft's “contained/non-intersecting” terminology explicitly instead of silently inheriting a corner-only legacy predicate.

Normalize WORLD before conversion, use absolute selector extents without viewport/screen clipping for classification, and treat any empty/converted-collapsed selector as a complete no-op even with flag 4. Reject unsupported bits outside 0xff before any mutation/paint rather than truncating. These empty/unsupported-bit rules are explicit safe completion. PORT0 inactivity or a selector beyond current viewport/screen does not suppress logical deletion. Existing global field snapshots are never relocated by a later viewport change.

Snapshot eligible instance tokens and their stored clipped **hit** rectangles at command entry. Remove selected temporary feedback, cancel capture only if its instance is selected, then delete the entire set while retaining survivor order/ownership and adopted hover/FIELDID semantics. Paint afterward from snapshots; failure never restores definitions or retargets surviving capture. Continue remaining requested paint attempts without logical rollback. This deterministic failure ordering is explicit completion, not a native transaction claim.

## Explicit portable painting completion

With no region bits do nothing, even with paint bits/128. With region bits, no 32/64 means deletion only. Otherwise 64 wins 32 as the primary text states. Without 128 paint each selected field's stored hit rectangle, not its unclipped face or recess. With 128 paint the **selector interior once instead**, independent of match count/exclusions; even flags 8+16 filtering every field do not suppress that explicitly requested whole-region paint. Flag 4 does not turn 128 into complement painting.

Painting is a physical-screen cleanup operation, clipped only to the canvas, independent of current port/viewport/PORT0 activation. Use COPY, no border; preserve selected families, graphics cursor, STYLE fields and W. For 32 use current palette **index 0**, not guaranteed RGB black and not STYLE background. For 64 use adopted Fill Style/Fill Color and pen background: mask one=fill, zero=background, opaque COPY. Pattern phase is physical-screen (0,0), eight device pixels with no WORLD magnification/restart at each region, consistent with the [existing fill-pattern contract](../ripscrip/2.3-shapes-and-fills.md#fill-pattern-origin-and-scaling). Fixed COPY and once-only mode avoid accidental repeated XOR semantics. The background, palette, clip and failure rules are explicitly chosen protocol completion; do not call the raw legacy DC brush path an exact oracle.

## Reproduction and remaining work

```sh
objdump -d --start-address=0x1000c474 --stop-address=0x1000c53d ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10012d63 --stop-address=0x10012ef1 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

Publish exact predicate/type/paint tables and six-field literal/coordinate examples for flags 0..7, exclusions, touching/covering and all paint combinations. Verify fingerprints/call/critical bytes, independent selection/coordinate/palette/pattern arithmetic and repository checks; refresh feedback and publish separately, consolidate actual head, then final coverage/ancestry audit. Keep prior contracts, co-author trailers, FastFont separation and scoped temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Selective deletion](../ripscrip/4.2-mouse-fields-and-pointer.md#rip_kill_enclosed_mouse_fields)
