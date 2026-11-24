# Issue #213 — Affine ellipse wire and contour rules

[GitHub issue #213](https://github.com/bbs-land/remote-imaging-protocol/issues/213) · [RIPscrip 3.0 curves](../ripscrip/2.2-curves.md)

## Scope and checkpoint

RealDeuce filed the issue at 2026-11-24 01:51:18 UTC. No comments were present at intake. The PC Micro driver has MD5 `bade8b1f4e467ac7ad4edb2639738d4c` and SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The local RIPlib `docs/spec/12-dll-provenance.md` D-31 and `tests/fixtures/affine_oval.h` are cross-checks for source geometry. The shipping code shared by the five consumer handlers is at RVA `0xfa70`. The formulas and documented state describe the wire geometry and portable drawing rules.

## Descriptor and drawing checks

| Slot | Handler RVA | Calls shared generator | Projected pairs | Geometry mode |
| --- | --- | --- | --- | --- |
| 8 | `0x1d5c2` | `0x1d6fd` | 5 | 1 |
| 10 | `0x1d79b` | `0x1d88f` | 3 | 0 |
| 11 | `0x1dd70` | `0x1de97` | 5 | 2 |
| 83 | `0x1d963` | `0x1da8a` | 5 | 3 |
| 84 | `0x1b89b` | `0x1b97a` | 3 | 0 |

Each calls `0x31084` once per pair, then selects the port DC and applies the viewport clip. The consumers pass the unshifted projected positions to `0xfa70` and forward its points to Polygon or Polyline. There is no viewport-origin addition in these handlers and the DLL imports no GDI viewport-origin setter. The W installation helper is `0xe6e9` for all five. The filled branch tests STYLE word `+0x22` for N and byte `+0x21` mask 4 separately. See `0x1df00/0x1df07`, `0x1daf3/0x1dafa` and `0x1b9b3/0x1b9b9`. Even a filled request chooses a pen and null brush for an open result, reaching Polyline at `0x1dfdd` for the pie. The portable filled border uses COPY per the earlier language contract, regardless of native call grouping.

## Geometry source and verification

Integer angle at `0xf950` uses atan and quadrant adjustment, with a strict comparison at `0xfae8`. `0xfb24–0xfb99` emits the open one-point return. Center-point ray substitution is `0xfbd6–0xfc47`. Q14 summing, shift and low-word extension occur at `0xfc8c–0xfce8`. The 72nd endpoint pair is followed by a duplicate first pair for segment intersection, with the canonical full output count still 72. Helper `0xf5a0` handles the line intersection using slope and axis special cases, truncating X/Y at `0xf877/0xf882` before it checks segment and ray inequalities. The portable reference uses the equivalent determinant formula with wide or exact arithmetic for ordinary device coordinates.

The start/end searches retain the first hits at `0xfd82/0xfdd6`. The asymmetry at `0xfdee–0xfe23` is observable in the two collinear vectors. The same-segment ordering helper is `0xf515`, the equal-hit whole-ring case `0xfe27–0xfe52`, and mode closure `0x10095–0x100ea`. In its nonwrapping forward assembly branch the native caller sign-extends low-word hit coordinates at `0xff35/0xff3a/0xff72/0xff77`; the ordinary signed-device coordinate profile preserves the recorded point runs without requiring a rasterizer to emulate out-of-domain signed wrapping.

A read-only independent model matched all 52 checked-in RIPlib point runs (13 input sets across four geometry modes) including their closed flag, vertex count and point digest. The digest starts at 2166136261 and for each X then Y in the run, XORs its low 16 bits and multiplies by 16777619 modulo 2³². The quarter arc count/digest is 19/`0x0f678c4d`; the whole ring is 72/`0x2204b668`. The missed-start and missed-end arcs are 32/`0xa3b0038c` and 2/`0x01b98424`. RIPlib produced the fixture with `scripts/dll-affine-fixtures.py` and the pinned helper. This turn compared the static model with those prior outputs. It did not load GDI or run the historical client.

## Verification

Verified all five dispatch records and shared-generator calls. A separate read-only geometry model matched the closed flag, count and digest of all 52 pinned helper fixtures. Checked Markdown, links and whitespace.

## Resume

Verify all five descriptors, the integer hit and contour selection rules, the consumers' line and polygon decisions and the filled contribution policy. Reproduce the existing reference vectors without opening the legacy client. Publish the curve and index entries with implementer examples.

## Delivery

[PR #224](https://github.com/bbs-land/remote-imaging-protocol/pull/224) is open for upstream review. Specification commit 7f0a24cfaa084ef02a370f95a3db25656342785b.
