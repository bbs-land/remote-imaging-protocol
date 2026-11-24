# Issue #213 — Affine ellipse wire and contour rules

[GitHub issue #213](https://github.com/bbs-land/remote-imaging-protocol/issues/213) · [RIPscrip 3.0 curves](../ripscrip/2.2-curves.md)

## Scope and checkpoint

RealDeuce filed the issue at 2026-11-24 01:51:18 UTC. No comments were present at intake. The PC Micro driver has MD5 `bade8b1f4e467ac7ad4edb2639738d4c` and SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The local RIPlib `docs/spec/12-dll-provenance.md` D-31 and `tests/fixtures/affine_oval.h` are cross-checks for source geometry. The shipping code shared by the five consumer handlers is at RVA `0xfa70`. The formulas and documented state describe the wire geometry and portable drawing rules.

## Resume

Verify all five descriptors, the integer hit and contour selection rules, the consumers' line and polygon decisions and the filled contribution policy. Reproduce the existing reference vectors without opening the legacy client. Publish the curve and index entries with implementer examples.
