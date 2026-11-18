# Issues #74–#76: drawing operations and contour arithmetic

[Research backlog](issue-backlog.md) · [Write modes](../ripscrip/2.0-color-modes-and-palettes.md#rip_write_mode)

## Scope and checkpoint

The user requested a combined resolution of [#74](https://github.com/bbs-land/remote-imaging-protocol/issues/74), [#75](https://github.com/bbs-land/remote-imaging-protocol/issues/75) and [#76](https://github.com/bbs-land/remote-imaging-protocol/issues/76), plus similar documentation gaps. This explicitly groups the three related issues instead of following the earlier one-issue-at-a-time workflow. Branch `issues-74-76-drawing-contract` starts at stable **c9a66b2**. All three issue bodies were read; none had comments at the initial refresh.

The common question is which operation applies to each drawing contribution: point/standalone stroke, region fill, optional border, background gap and repeated contour visit. The protocol-first policy applies: explicit protocol documentation controls, and original client calls are supporting historical evidence rather than normative overrides. Do not investigate implementation bugs or run desktop/native client mode probes for this task.

## Protocol source leads

Preserved ALPHA 4 source `version/2.0/text/RIPScrip-2.0-alpha-4.txt`, SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`:

- §3.4.1.38 / lines 6781 onward explicitly gives PIXEL Write Mode YES. WRITE_MODE defines COPY/XOR/OR/AND and destination inversion, and applies to pixels and fill-based operations. This takes precedence over an implementation's choice of SetPixel/FillRect.
- §3.4.1.47 / lines 7329–7336 explicitly makes filled-object borders COPY, using current thickness, with line patterns for polygon-related borders only. It specifies fill first, border afterward. That specific exception takes precedence over WRITE_MODE's general scope.
- PIE_SLICE / OVAL_PIE_SLICE explicitly exclude line patterns. The current K/u entries instead promote the client's pen/pass behavior; those are similar cases that need reconciliation under the maintainer's policy.

Preserve already derived angle normalization and contour topology. Separate their geometry from the operation applied to each contribution; N must gate optional borders, including radial contour portions. Distinguish repeated polygon fill winding/parity from repeated standalone stroke visits.

## Remaining work

- Publish one shared operation/pass contract and cross-link affected primitive entries. Produce independent nonblack-destination tables for all five modes, fills, borders, gaps and revisits, for screen/offscreen and thick points.
- Reconcile K/u, polygon-border notes, standalone versus filled skewed shapes, and the distinction between drawing NOT (destination inversion) and bitmap-put NOT (source inversion). Audit other copied client-only write-mode assertions.
- Recover the skewed contour's actual coordinate/conversion/rotation algorithm and table data as a reproducible numeric profile. Separate that profile and optional historical overflow from required geometry; do not impose Windows raster edge pixels.
- Record exact source sections, inference boundaries, arithmetic oracles and validation. File a combined complete PR closing all three issues, preserving original protocol text and RIPscrip 1.54.

---

[Research backlog](issue-backlog.md) · [Write modes](../ripscrip/2.0-color-modes-and-palettes.md#rip_write_mode)
