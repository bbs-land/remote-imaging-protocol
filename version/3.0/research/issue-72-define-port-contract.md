# Issue #72: DEFINE_PORT protocol contract

[Research backlog](issue-backlog.md) · [RIP_DEFINE_PORT](../ripscrip/7.0-drawing-ports.md#rip_define_port)

## Scope and evidence policy

[Issue #72](https://github.com/bbs-land/remote-imaging-protocol/issues/72) was opened against integration PR #71 and asks about DEFINE_PORT flags, nonzero offscreen coordinates, protected redefinition and failed replacement state. Branch `issue-72-define-port-contract` starts at **e800949**. The maintainer clarified on 2026-11-17 that this work documents the **protocol**, not the behavior or defects of an obsolete implementation. Protocol documentation establishes the intended contract. Implementation observations do not silently replace explicit documented rules or fill unspecified failure states with client-specific recovery behavior.

Initial client-path tracing in checkpoint `0415b34` preceded that clarification. It is not the basis of the normative answer. Its exploratory probe changes are excluded from this PR, and no further implementation-defect investigation is part of this work. This derivation supersedes that checkpoint's next-action instructions.

## Primary protocol sources

- [RIPscrip 2.00 ALPHA 4 specification](../../2.0/text/RIPScrip-2.0-alpha-4.txt), §3.4.3.1, lines 10358–10460. The preserved repository text record is 1,202,490 bytes, SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. This supplies the explicit DEFINE_PORT rules inherited by the 3.0 reconstruction.
- The same specification's data-table discussion, lines 1985–2056, distinguishes protected configuration from drawing into protected ports. Protected configuration cannot be deleted or modified; drawing to its surface is a separate operation.
- [RIPscrip 3.x technical white paper](../text/RIPScrip-3.x-technical-whitepaper.txt), drawing-port discussion around lines 996–1008 and data-table protection around lines 1175–1186. The text record is 89,253 bytes, SHA-256 `bf5fd27703bdc4ef311a3dc57a15dd31b0e5ceb37026ac7d015a0fcd3265ab34`. It confirms screen/offscreen ports, independent viewports and protected persistent definitions; it does not replace DEFINE_PORT's flag or resource-failure rules.

Reproduce the relevant protocol reading with `sed -n '10358,10460p' version/2.0/text/RIPScrip-2.0-alpha-4.txt`. Neither original text nor distributed assets are edited. The existing #10 extent-boundary correction remains applicable to valid zero-origin offscreen requests and screen rectangles; its implementation evidence does not override the explicit offscreen near-coordinate rule.

## Documented flags

The command's flag table explicitly defines bits 1, 2, 4 and 8: offscreen storage, selection of the new port, an inactive initial viewport and initial protection, respectively. All combinations are defined by OR'ing those independent bits. Their success postconditions are:

- Storage is offscreen if bit 1 is set, screen otherwise.
- Select the new port if bit 2 is set; otherwise retain the previous current selection on successful definition.
- The new viewport covers the port and is active unless bit 4 is set.
- The new definition is protected if bit 8 is set, unprotected otherwise.

For decimal flags 15, all four conditions apply together: offscreen, current, inactive viewport, protected. Initial configuration belongs to the definition operation; the new protection does not block another initial-state bit in that operation. There is no documented exclusion between the four flags.

No additional bits are defined in this table. The reference reserves higher bits and requires senders to leave them zero. The four-character argument width gives an encoding capacity, not additional semantics. Receiver treatment of a nonzero reserved bit is not specified by these sources; accepting or rejecting such a value must not be presented as a recovered protocol guarantee. In particular, the documented actions of bits 4/8 are retained even if a historical client omitted them.

## Offscreen origin and extents

The coordinate paragraph tells senders to use x0=y0=0 for offscreen ports. The flag-1 description explicitly says those values are treated as zero even if supplied otherwise. These statements establish the following protocol reading:

1. For a screen port, both corners describe screen placement and their difference describes its extent, after world/device conversion.
2. For an offscreen port, substitute (0,0) for the near corner; the far corner gives the dimensions from that origin. Canonical senders supply zero near coordinates.
3. Retain the already established extent-boundary convention: no extra inclusive pixel is added.

With equal world/device dimensions, offscreen (2,1)–(6,4) therefore denotes **6×4 at (0,0)** under the documented ignored-near-coordinate rule. A sender wanting 4×3 sends (0,0)–(4,3). The suggested 4×3 interpretation in the issue follows a client-specific subtraction path, not the protocol's explicit flag-1 text. Merely zeroing the stored origin after subtracting the original near corner does not implement that documented rule.

A portable request converts to ordered, positive dimensions supported by the implementation. The protocol sources do not assign special meanings to zero extents, reversed corners or fixed-width integer wraparound. The reference identifies those limits without adopting a particular historical arithmetic result as an allocation feature. The ordinary full-world example remains 640×480 at a 2:1 world/device ratio, with last pixel (639,479).

## Protected redefinition and resource failures

The protected-target paragraph explicitly states both rejection and selection of screen port 0. Combined with the data-table protection rule, it preserves the target's definition/protection/contents while changing current selection to 0. For current 2 and protected target 1, the documented result is therefore **current 0, target 1 unchanged**. Flag 2 does not convert a failed definition into a successful selection of that target.

The same paragraph describes ordinary unprotected replacement as deletion followed by creation. It does not establish transactional replacement, rollback, or post-allocation-failure table/current/clipboard state. Consequently:

| Case | Protocol guarantee |
| --- | --- |
| Protected existing target | Reject; retain protected target; select port 0 |
| Successful unprotected replacement | New definition and requested initial-state flags apply |
| Insufficient resources while creating/replacing a port | No guarantee that the previous target/pixels survive; final target/current/clipboard state is unspecified by these sources |

This last row applies whether the target was current or another port was current. It does not prescribe a dangling index, implicit screen-port creation, a forced selection, restoration of old pixels or a particular clipboard side effect from an implementation's allocator/rebinder. Those details do not become protocol requirements through observation of a single client.

Offscreen storage is distinct from clipboard capture/assignment. DEFINE_PORT flag 1 is not GET_IMAGE. A producer recovering from resource failure can explicitly select port 0, reconstruct the definitions it needs and establish a fresh clipboard capture. The reference describes this recovery guidance separately from the guarantees actually established by the source.

## Delivery status

The four issue topics now have a protocol-level disposition: retain all four documented flags; reserve unspecified higher bits; make the offscreen ignored-near-coordinate rule explicit; preserve the protected-target fallback; and explicitly bound the guarantees for resource failure and malformed extents. No replacement for undocumented allocation behavior is invented. Documentation checks pass: `run/format`, `run/lint`, `run/check-links` (0 bad of 5,440 links across 222 files) and `git diff --check`. Source-text hashes were recomputed from the preserved files. No probe/source files, original text or assets are changed relative to e800949. Issue #72 still has no comments at the delivery refresh. Delivered in [PR #73](https://github.com/bbs-land/remote-imaging-protocol/pull/73), dependent on #71. Specification commit **ad1c37f**; issue-only comparison starts at **e800949**. GitHub confirms the PR is open, ready and mergeable, with exactly one closing issue reference, #72. No issue was manually closed and no PR was merged.

---

[Research backlog](issue-backlog.md) · [RIP_DEFINE_PORT](../ripscrip/7.0-drawing-ports.md#rip_define_port)
