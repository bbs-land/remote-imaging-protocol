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

## Native parser checkpoint and recovered record shape

New `run/riptel-bezier-probe DLL [ORIGINAL_CORPUS_FILE]` uses original byte parsing/reset and J/n state setters, with controlled framing predicates and ASCII character classes. The unchanged DLL hash is verified before a temporary i386 executable runs, with automatic cleanup and core dumps disabled. Initial hardcoded original t/z vectors pass **16 checks** without a corpus file (the exact running total is reported by the tool); optional corpus replay prints accepted curve vectors for inspection. Both disputed samples are accepted, rather than malformed or a second opcode grammar.

The original byte-exact **extracted/shapes.rip** has **3709 bytes**, SHA-256 **196526ce2f9752aff4512856d4aed38c1752bd3055bf47e6f364b8b8f08b103f**. It explicitly sets `J10` at physical line 14 and `n2000` at 15. Its complete first z is at **line 70** (the census shows only a prefix). Original parsing yields count **46**: header **8,40**, then eight records, with types **2,5,5,5,6,0,0,0**. The first record is **type 2**, followed by pairs **8K GC / 8K ES / AS F8 = (308,588)/(308,532)/(388,548)**. There is **no separate base-point header**. The next record starts with type 5. Line 71 gives the second z, eight units lower in each coordinate. Optional whole-file parsing confirms these and the four filled/two open poly variants, while drawing remains controlled separately.

The t sample similarly yields header **3,40**, then type **2** with pairs **JA 62 / HA 62 / JE A6**, type **6** with pairs **HU A6 / JG A6 / HC EE**, then type **0** with pair **JC EG**. The old header-based 0/1/2-only grammar does not decode this sample correctly either. Original parser acceptance of 5/6 is direct evidence, not an inference from names.

The shared consumer uses only the class table for rendering: **0/3/4 are point/line records**, **1/2/5/6 are cubic-start records with two controls**. Source **0x2d036–0x2d0da** takes a curve's first three pairs as start/control 1/control 2, and its end point from the **next record's first pair**. A last curve in a closed/filled command takes the first record's first pair instead. This explains the terminal-curve restriction in open mode t; it is not a prohibition on every final curve. Confirm each alias and closure natively before publication.

The numerical generator **0x2c9e0–0x2cc2a** is entirely integer arithmetic, using coordinates shifted left eight bits, signed divisions, arithmetic right shifts and signed 16-bit output. It does not use x87/double floating point. Its loop emits P0, interior indices **i=1..N-2** at a fixed-point version of t=i/N, then P3; for N>=2 it writes **N points**, not N+1. N=0/1 skip division and still write two scratch endpoints, while Z forwards the original N to Polyline. These are static numerical findings pending full native sample/rounding verification; do not substitute an ideal Bernstein renderer as their historical oracle.

## Native numerical and consumer checkpoint

The expanded probe passes **16,900,892 checks** with FreeBSD 15.1 amd64, clang 19.1.7, `-m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`. Reproduce from the checkout with `run/riptel-bezier-probe <mirror>/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`; append the original `extracted/shapes.rip` to print corpus vectors. Workspace allocation and the actual drawing arrays are guarded and captured; no Windows rasterization or GUI runs.

Original routines executed include parser/reset/J/n, generator **0x2c9e0**, single-curve helper **0x2cc30**, common typed consumer **0x2cda0**, Z handler **0x19f7b** and world conversion **0x31084**. Numerical checks compare every generated coordinate for **all N=0..4095** against independently expressed modular integer de Casteljau arithmetic; additional negative, fractional and signed-16-bit extreme profiles cover N=0/1/2/3/4/5/40/1295/4094/4095. Bounds and the first unwritten sentinel are checked. Every single-curve drawing count 0..4095 is captured. Z and poly controls are tested at unit and half scale, including odd coordinates that truncate before evaluation.

The oracle confirms the loop and rounding exactly: signed division truncates toward zero at each interpolation; coordinates start with eight fractional bits; the final arithmetic shift floors negative fractional results; interior/final outputs narrow to signed 16 bits. Modular 32-bit differences/products/additions reproduce original overflow. Three rounds of linear interpolation expand algebraically to the ordinary cubic Bernstein equation with final terms **X4*t^3 / Y4*t^3**, not undefined X/Y or a generic B-spline. The standalone first output keeps its full input integer; later generated outputs narrow. The poly consumer additionally zero-extends its converted endpoint's low 16 bits before generation and point-class outputs' low 16 bits when storing.

Native mode/type/border matrices confirm 0/3/4 point records and 1/2/5/6 cubic records; the same inputs are tested as open, closed and filled, with both border settings. A curve ends at the next record's start. A terminal curve rejects the **whole open t** without an error report or drawing; z/x accept it and wrap its endpoint to the first record's start. A terminal point need not repeat the first point: Polygon closes z/x, while t uses Polyline. No explicit closure duplicate is appended by this consumer.

Count zero rejects any curve-bearing poly command silently, but all-point records ignore count, including zero. Count one contributes one drawn point per curve, although the generator writes two scratch endpoints; subsequent records overwrite the extra scratch endpoint. Z forwards N=0/1 to Polyline without rejecting it, drawing no curve under its minimum-two-point API contract. For N>=2, the generator writes/draws **N points**, at **0,1/N,...,(N-2)/N,1**: the missing `(N-1)/N` is an original sampling defect, not N+1 sampling. N=2 is just the two endpoints.

The generated-cap matrix confirms **C\*N+L** actual submitted points. The separate check reserves one: if **C\*N+L+1 > 4095**, it reduces N to **floor((4094-L)/C)** uniformly. One closed curve requested at 4095 yields 4094 points; a curve plus a point uses N=4093 and still submits 4094; two curves plus a point requested at 2047 use N=2046 and submit 4093. Zero after reduction rejects. The 80,000-byte allocation is not the wire or generated cap.

Original parsing verifies all seven literal types, all three opcodes, both radix settings and XY widths 2..5; both headers stay width 2 independently of XY. Complete accepted vectors satisfy **2+3*L+7*C <= 8193**. Tests at 2730/2731 point records and 1170/1171 curve records confirm the shared 8193-value collection truncation and whole-command rejection of the oversize mismatched vectors. At 1170 closed curves, N=40 reduces to 3 and submits 3510 points. Invalid internal type, incomplete vector, block-count mismatch and excess complete block reject without drawing; zero blocks alone reports `Too few segments`.

Controlled hooks: parser framing/protection **0x13c49 / 0x13c59 / 0x3d9e1**, dispatch bookkeeping **0x45038**, allocation/free **0x6bd40 / 0x6bd30**, reporter **0x38e47**, synchronization **0x3988f / 0x398ee**, selection helpers **0x13ce5 / 0x13cfc / 0x13d10 / 0x13d27**, handler guards/state setup **0x3445b / 0x18cf6 / 0x343e4 / 0x342ad / 0xe6e9 / 0x34319 / 0x18bf7**. Imports replaced: **0x963f0 SetBkMode**, **0x963c0 SelectObject**, **0x9641c Polygon**, **0x9647c Polyline**; ASCII classes via **0x87908**. The controlled drawing DC is 1, master DC 2, skipping unrelated invalidation-region construction. The source DLL stays unchanged; only its mapped service entries change. A first handler fixture crashed because return-side host services 0x34319/0x18bf7 were initially omitted; adding those hooks fixed the harness. This was not a driver rejection or curve defect. Every failed/completed build directory was removed by EXIT cleanup, and core dumps were disabled.

## Complete answer and validation

The [corrected curves reference](../ripscrip/2.2-curves.md#rip_bezier) now answers all four issue questions: ordinary cubic Bernstein geometry with X4/Y4 endpoint terms; exact integer sampling/range/zero/rounding and independent input/output caps; open-only terminal-curve rejection with whole-primitive semantics and closed/filled closure; and complete annotated original z/t decodes. The filled command, command inventory and census clarification use the same grammar. The [consumer techspec](../techspecs/2.2-bezier-consumer-contract.md) preserves arithmetic and topology separately from wire syntax. Earlier-client runtime behavior is not inferred, and verbatim text/assets remain untouched.

Final native invocation with `extracted/shapes.rip` passes **16,900,918 checks**. It additionally submits the complete original first-z vector through the unchanged common consumer with a controlled 1280×960 world / 640×480 device conversion, capturing **203 points**, first (154,294), last (154,324); the polygon supplies the last straight closure. The file's complete scene is not rendered: unrelated command callbacks are deliberately skipped. An explicit four-point numerical oracle `(0,0),(0,0),(0,0),(64,0), N=4` gives **(0,0),(1,0),(8,0),(64,0)**, proving the missing t=3/4 sample independently of the general numerical expression.

The wrapper's scoped build directory is removed after each success or failure. No source artifact was modified and no GUI process was started. `run/lint` passes, `run/check-links` reports **0 bad of 4894 links across 200 files**, and `git diff --check` passes. PR delivery is recorded below once published.

## Delivery

[PR #44](https://github.com/bbs-land/remote-imaging-protocol/pull/44) is open, branch `issue-15-bezier-contracts`, dependent on #43. Primary specification commit **d04a73d**; numerical checkpoint **c9e4394**; parser checkpoint **825e8e0**. The [issue-only comparison](https://github.com/RealDeuce/remote-imaging-protocol/compare/496a98c...issue-15-bezier-contracts) starts at the delivered #14 checkpoint. No merge or manual issue closure was performed. All new commits include the requested Codex co-authorship trailer. The post-delivery creation-order refresh selects **#16** next; #42 remains after #38.

---

[Research backlog](issue-backlog.md) · [Curves](../ripscrip/2.2-curves.md#rip_bezier)
