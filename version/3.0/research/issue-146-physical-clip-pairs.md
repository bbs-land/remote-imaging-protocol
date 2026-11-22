# Issue #146: Physical clipping and remembered-pair geometry

[Research backlog](issue-backlog.md) · [Remembered pairs](../ripscrip/7.0-drawing-ports.md#remembered-copy-pairs)

## Scope and checkpoint

Started 2026-11-22 on `issue-146-physical-clip-pair-boundary` from fork stable **f14e410915f11f64d2287eec82ebf4770c3baf41**. In-session check **2026-11-22T09:32:44Z** detected [#146](https://github.com/bbs-land/remote-imaging-protocol/issues/146), created/updated 09:31:40Z, zero comments, body SHA-256 `1c20066b29f46e8a53341796d9ecc912f0065f356258d1e0898c7ca6439bae98`. Next check due about 09:42:44Z. User confirmed research-and-PR processing.

Adopt #144 / PR #145's ownership, units, selectors, producers, protection, lifecycle and backups. Complete the final-rectangle/publication boundary where logical viewport limits exceed physical storage. No new original-client/probe/bug investigation/execution, exact sampling, new buffer/damage layer or 1.54 change.

## Sources and documentary limits

Starting-file hashes, reproducible with `git show f14e410:<path>`:

- `version/3.0/ripscrip/7.0-drawing-ports.md`: SHA-256 `77c9538f63c76821d1ce6d0648298713ac7c77dd36840e6ce57bb5584f9c227c`.
- `version/3.0/ripscrip/1.3-world-view-virtual-canvas.md`: SHA-256 `11a01cdf019368d24deee857404a8ae3b8302438aed095ec64b0424f08eb40c9`.
- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `53150b95cf2b0652c36f4ab25194fd82a32aea6890c54a1f65954c1b698b70e6`.
- `version/3.0/research/issue-10-port-geometry.md`: SHA-256 `799617764268f8b2ada3eebe9cbe26e3e8621a812dd1620a971048b369497ee3`.
- `version/3.0/research/issue-11-pixel-pen-units.md`: SHA-256 `e70757f43c1b0d36618add863551690697a754c412425e73f6ed592e265e290c`.
- `version/3.0/research/issue-144-remembered-port-copy-pairs.md`: SHA-256 `ebf33803296e4d71f7c48ee3bc33ee5df16cadf8261aa7d27a01ec699d65d281`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

Accepted [#11 viewport evidence](issue-11-pixel-pen-units.md#viewport-producer-and-screen-port-origin) and the viewport reference establish bounds without port intersection: recorded [700,50;711,61) lies beyond a 640-pixel port. This is existing evidence only. The [#10 copy contract](issue-10-port-geometry.md) establishes pre-trim equal-versus-unequal classification and coupled versus independent logical viewport trimming; captured copy arguments do not establish physical raster coverage or a filter. ALPHA 4 §5.11.1 lines 17736–17790 establish viewport-relative ownership/reversal but no rounding of physical scaled correspondences. [#144](issue-144-remembered-port-copy-pairs.md) explicitly completed publication without distinguishing this further physical stage; “actually transferred” needs a precise logical boundary.

## Chosen portable completion and reasoning

S*/D* mean the final integer **logical compositor rectangles after protocol viewport trimming and before physical storage/sample clipping**. Preserve their resulting branch/mapping during raster clipping; do not refit the remaining physical area. Publish those logical rectangles on successful completion of a logically eligible nonempty named transfer, even if physical clipping/sample rejection writes zero pixels. Determine logical emptiness/eligibility separately: success returned for an empty/ineligible logical no-op does not publish. Actual transfer failure retains the previous pair on an existing definition even if an outer helper ignores that failure. New/replaced definitions still start absent under existing rules.

Requested logical source [0,4), physical [0,2), destination [0,3) stores Rs=[0,4), Rd=[0,3); the physical source boundary maps to destination 3/2. Neither rounded 1 nor 2 becomes a remembered edge. Equal logical 4→4 stores both full rectangles even if the backend returns a coupled 2→2 physical blit. A destination physically clipped at 1 keeps the 4→3 mapping, whose corresponding source boundary is 4/3; no 4→1 refit. A logically nonempty [2,4) source outside physical [0,2), or [2,4) destination outside physical [0,1), can complete successfully with zero writes and still publish. Logical viewport rejection or observed renderer failure retains old metadata regardless of an outer success return.

Reject physical coverage-derived or rounded pairs: unequal mapping can create fractional edges, different samplers can cover different edge cells, and scattered/zero coverage need not be one integer pair. P is geometry, not initialized-pixel/damage coverage. PCB reverses it under current limits; it cannot promise restoration of data never physically captured. This narrows #144's uncopied-region rationale to logical viewport exclusions while preserving all its other rules and raster freedom. No extra wire, buffer, metadata family or source investigation is needed.

## Independent validation

Inline Python checks at 2026-11-22T09:37:21Z verify all seven starting-file hashes from `git show f14e410:<path>`. Exact `fractions.Fraction` arithmetic gives the source physical edge's destination 3/2 and destination physical edge's source 4/3. A hypothetical center-position versus leading-edge sampler covers one versus two destination cells for the same 4→3 logical request and physical source [0,2); neither convention is selected as the protocol filter. The original mapping's first center corresponds to source 2/3, whereas an incorrect 4→1 physical refit would use 2. Equal-size clipping, both source-outside and destination-outside zero-write cases, and a logical-validity/completion publication matrix pass. The rational edge identity also passes 352 source-width/destination-width/boundary combinations, applicable to either axis. No original/native instructions, UI, provider or raster service ran.

The completed central reference defines the logical/physical boundary, preserved post-viewport mapping, no coverage-based rounding, and explicit publication status table. Independent vectors cover scaled/equal source limits, destination clipping, successful source/destination zero coverage, logical no-op and actual failure. PCB and GET_IMAGE summaries are qualified, and #144's historical derivation links this boundary clarification without changing its other accepted answers.

Full `run/lint`, `run/check-links` and `git diff --check` pass; link validation reports **0 bad of 6,331 across 261 files**. Prose spacing passes a focused scan. Before publication, #146 retains its initial body hash, update time and zero comments.

## Delivery and next action

[PR #147](https://github.com/bbs-land/remote-imaging-protocol/pull/147) is open/ready, specification **88a8b60d52b07edbcf01f484f52bb301c71b6add**, with exactly #146 as its closing reference and no closing pagination overflow. The report retains the initial body hash/update time and zero comments. Scoped `TemporaryDirectory(prefix='rip-146-pr-')` request files were removed automatically. No upstream merge or manual closure.

Normal-merge the actual completed branch head into fork stable with identical source tree before workflow notes, audit coverage/feedback/ancestry, then return to active-session ten-minute checks. Preserve FastFont separation, co-author trailers and protocol-only scope; no persistent scheduler.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
