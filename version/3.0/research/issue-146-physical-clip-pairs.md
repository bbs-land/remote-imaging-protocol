# Issue #146: Physical clipping and remembered-pair geometry

[Research backlog](issue-backlog.md) · [Remembered pairs](../ripscrip/7.0-drawing-ports.md#remembered-copy-pairs)

## Scope and checkpoint

Started 2026-11-22 on `issue-146-physical-clip-pair-boundary` from fork stable **f14e410915f11f64d2287eec82ebf4770c3baf41**. The in-session check at 2026-11-22T09:32:44Z detected [#146](https://github.com/bbs-land/remote-imaging-protocol/issues/146), created/updated 09:31:40Z with zero comments and body SHA-256 `1c20066b29f46e8a53341796d9ecc912f0065f356258d1e0898c7ca6439bae98`. Next issue check due about 09:42:44Z. User confirmed research-and-PR processing.

This boundary follow-up adopts #144 / PR #145's ownership, units, selectors, named producers, protection, lifecycle and backup rules. Complete the meaning of final rectangles and successful nonempty transfer when a logical viewport exceeds physical backing. **No new original-client/probe/bug investigation or execution**, exact sampling, new buffer/damage layer or 1.54 change.

## Sources and limits

Starting-file hashes, reproducible with `git show f14e410:<path>`:

- `version/3.0/ripscrip/7.0-drawing-ports.md`: SHA-256 `77c9538f63c76821d1ce6d0648298713ac7c77dd36840e6ce57bb5584f9c227c`.
- `version/3.0/ripscrip/1.3-world-view-virtual-canvas.md`: SHA-256 `11a01cdf019368d24deee857404a8ae3b8302438aed095ec64b0424f08eb40c9`.
- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `53150b95cf2b0652c36f4ab25194fd82a32aea6890c54a1f65954c1b698b70e6`.
- `version/3.0/research/issue-10-port-geometry.md`: SHA-256 `799617764268f8b2ada3eebe9cbe26e3e8621a812dd1620a971048b369497ee3`.
- `version/3.0/research/issue-11-pixel-pen-units.md`: SHA-256 `e70757f43c1b0d36618add863551690697a754c412425e73f6ed592e265e290c`.
- `version/3.0/research/issue-144-remembered-port-copy-pairs.md`: SHA-256 `ebf33803296e4d71f7c48ee3bc33ee5df16cadf8261aa7d27a01ec699d65d281`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

Accepted [#11 viewport evidence](issue-11-pixel-pen-units.md#viewport-producer-and-screen-port-origin) and the viewport reference establish stored bounds without port intersection: the recorded device rectangle [700,50;711,61) is beyond a 640-pixel port. This is existing evidence only. The [#10 copy contract](issue-10-port-geometry.md) establishes pre-trim equal-versus-unequal classification and coupled versus independent logical viewport trimming; its captured copy arguments do not establish physical raster coverage or a scaling filter. ALPHA 4 §5.11.1 lines 17736–17790 establish viewport-relative ownership/reversal but no integer rounding of physical scaled correspondences. [#144](issue-144-remembered-port-copy-pairs.md) explicitly completed pair publication without distinguishing this further physical stage. Its phrase “actually transferred” needs a precise logical boundary rather than new pixel-derived metadata.

## Chosen portable completion (draft)

S*/D* mean the final integer **logical compositor rectangles after protocol viewport trimming and before physical storage/sample clipping**. Preserve the initial branch and mapping during raster clipping; do not refit the surviving physical region. Publish those logical rectangles on successful completion of a logically eligible nonempty named transfer, even if physical clipping/sample rejection writes zero pixels. Determine logical emptiness/eligibility separately: a legacy service's success for an empty/ineligible logical no-op does not publish. An actual failed transfer retains the previous pair on an existing definition even if an outer helper ignores the failure. New/replaced definitions still start absent.

For the requested source logical [0,4) versus physical [0,2), destination [0,3), publish Rs=[0,4), Rd=[0,3); the source physical edge maps to destination 3/2 under the unchanged nominal map. Neither 1 nor 2 becomes a remembered destination edge; no rounding, coverage span or filter choice enters P. Equal-size [0,4)→[0,4) likewise stores both logical full rectangles, even if a raster service can return a smaller coupled physical pair. A physically clipped destination preserves the same nominal mapping and pair. P is geometry, not evidence every cell was written, an exact damage rectangle or a mask of initialized pixels. PCB reverses that logical pair and applies current logical/physical limits; no pixel restoration or inverse-fit guarantee is added.

Reject physical coverage-derived or outward/inward-rounded pairs because scaled source clipping can have fractional correspondence, different samplers can cover different edge cells and scattered/zero sample coverage need not be representable by one integer rectangle pair. Retain existing raster choices/physical bounds rather than silently changing #10 or enlarging storage. No separate empty-coverage metadata is needed.

## Next action

Qualify the central pair/publication language and affected producer/PCB summaries, add exact independent physical-source/destination/no-coverage vectors, verify all requested cases and preserved #144 rules, format/lint/link/diff and refresh report. Publish a separate PR, normal-merge its actual head into stable with an identical source tree before workflow notes, audit coverage/feedback/ancestry, then continue ten-minute monitoring.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
