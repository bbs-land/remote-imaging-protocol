# Issue #15: Bézier equations, subdivision and block grammar

[Research backlog](issue-backlog.md) · [Curves](../ripscrip/2.2-curves.md#rip_bezier) · [Filled poly-Bézier](../ripscrip/2.3-shapes-and-fills.md#rip_filled_poly_bezier)

## Scope and source

Started 2026-11-16 after [PR #43](https://github.com/bbs-land/remote-imaging-protocol/pull/43) delivered #14. GitHub creation-order refresh confirms [issue #15](https://github.com/bbs-land/remote-imaging-protocol/issues/15) is the oldest RealDeuce issue without a complete resolution PR. Branch `issue-15-bezier-contracts` starts at **496a98c**. Required answers: cubic endpoint equation/terminology; subdivision count range, zero behavior, actual sample parameters and rounding; generated-vertex budget; terminal curve validation and closure for t/z/x; and an annotated original decode or corrected interpretation of the anomalous z census payload.

Use unchanged RIPtel 3.1 / driver 3.0.7 `extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base 0x10000000, from the [catalogued external artifact mirror](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/`. All addresses below are RVAs. Use native/static probes without GUI. Preserve original text/assets, clean up task-owned temporary files promptly, and include Codex co-authorship in every new commit.

## Original dispatch and consumers

| Slot | Opcode | Handler | Initial descriptor |
| --- | --- | --- | --- |
| 76 | Z | 0x19f7b | Nine numeric arguments: eight XY coordinates, then fixed width 2 |
| 77 | z | 0x1e449 | Two initial width-2 numerals; flags 19 |
| 61 | t | 0x1e4a4 | Two initial width-2 numerals |
| 71 | x | 0x1bc1d | Two initial width-2 numerals |

The z flags are **0x13**, including the special variable-block parser bit **0x10**. Its descriptor's positive count 2 is not proof that the rest is an opaque string or that the ordinary repeating-XY parser of issue #13 applies. The original byte parser's special descriptor table near **0x81c48** and states around **0x3a223–0x3a642** need tracing. Preserve the distinction between initial fixed fields and the expanded typed numeric vector handed to consumers.

All t/z/x wrappers reach **0x2cda0** with the actual decoded argument count/vector and a mode: **z=1**, **t=2**, **x=0** (calls **0x1e486 / 0x1e4e1 / 0x1bc5a**). The common routine allocates **0x13880 = 80,000 bytes** through allocator **0x6bd40**, validates the vector, expands segments and ultimately draws it. Allocator/free need controlled services in a native probe; preserve actual validation, numerical generator and point topology.

Z converts all four control points through world conversion **0x31084** before invoking **0x2cc30** at **0x1a0a1**. That helper allocates the same 80,000-byte workspace, calls generator **0x2c9e0**, then passes the original subdivision count to GDI **Polyline at 0x2cccf**. It does not by itself prove `count+1` samples are drawn. Separate generated samples from the final consumer's count, as in the polygon and full-skewed findings.

## Initial shared validation and expansion leads

The common poly routine reads low 16-bit header values at **0x2ce1a–0x2ce32** and rejects a first header value below 1. Its validation loop starts at numeric index 2, reads an internal type value, requires it below 7 and uses the seven-word class table **0x7ef48**:

```text
internal type: 0 1 2 3 4 5 6
class:         0 1 1 0 0 1 1
```

Class 0 advances over two following coordinates; class 1 advances over six. This is not yet a recovered wire-type grammar: special parsing may synthesize or transform types. Do not equate all internal types 0..6 with the reference's literal wire 0/1/2 before original byte-parser evidence.

At **0x2ce98–0x2cea3**, the last-block curve check appears to reject **mode 2 (t)** specifically, rather than all closed/filled forms. Both this lead and its exact malformed-input behavior need native confirmation. Other invalid-type/length/block-count branches in **0x2ceb1–0x2cf10** free the workspace and return without reaching drawing; they often do not call the error reporter, so absence of an error message is not proof of acceptance.

At **0x2cf11**, the routine computes a generated budget from the number of curve-class entries, subdivision count and line-class entries: `curves*count + lines + 1`. When that exceeds **0xfff (4095)**, it replaces the subdivision count with `floor((4094-lines)/curves)` at **0x2cf22–0x2cf2d**. If there are curves and the resulting count is zero, the following branch rejects. Allocation size alone is therefore not the usable generated-vertex limit. Need trace final count, implicit/explicit closures and rounding in generator 0x2c9e0 before publishing the contract.

## Census inputs to adjudicate

The committed census lists two z payloads from SHAPES.RIP, beginning:

```text
081428KGC8KESASF85CGF8E4F8BCGC5DKGCEOGCE4GW5DKHGD0I0BWI06ASI
081428CG48CEKAKF05C8F0DWF0B4G45DCG4EGG4DWGO5DCH8CSHSBOHS6AKH
```

The t comparison begins `03142JA62HA62JEA66HUA6JGA6HCEE0JCEG`. Applying the current reference's z header mechanically gives `08 14 28 KG` then invalid type C. That is the unresolved observation, not proof of a second grammar. Locate the original byte-exact SHAPES.RIP, replay its base/coordinate-size state and command through the original parser, inspect the expanded vector and actual acceptance/rejection, then annotate the exact bytes or correct the census/layout.

## Next action

Build a hash-guarded native Bézier probe with original byte parsing (including special block descriptors), shared validation and numerical generators, captured allocations/drawing and reproducible corpus inputs. Cover both radices and coordinate widths, count 0/1/max, open/closed/filled terminal line/curve combinations, count/length mismatches, sample positions/rounding and expansion-boundary reduction. Trace any x87/CRT conversion helpers before substituting services; preserve original numerical decisions where native execution can run them. Publish only after every issue subquestion has an exact primary-source answer; no specification edits or complete PR yet.

---

[Research backlog](issue-backlog.md) · [Curves](../ripscrip/2.2-curves.md#rip_bezier)
