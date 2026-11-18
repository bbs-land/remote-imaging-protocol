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

## Source hierarchy and decisions

| Decision | Evidence and standing |
| --- | --- |
| X applies W, including non-COPY modes | Documentary: PIXEL §3.4.1.38 explicitly says Write Mode YES; WRITE_MODE §3.4.1.57 defines the operation. |
| j applies the same W to each pixel of its square | Reconstruction: POINT's drawing role and existing attributes inherit the general operation; no recovered protocol exception permits replacement-only points. Thick coverage is one contribution per pixel, not one per thickness unit. |
| I/i/_ fill W, then optional solid COPY border | Documentary for I/i: §3.4.1.47, lines 7329–7336 explicitly require COPY/current thickness, fill first, and patterns only for polygon-related borders. I/i individually exclude line patterns. Applying the same non-polygon filled-border rule to reconstructed _ is identified as inheritance. |
| K/u borders solid COPY | Same explicit documentary rule. Earlier native current-ROP/patterned-pen observations remain historical; the public reference must not promote them over the protocol. |
| -/+/[ fill W, then optional patterned COPY border | Explicit reconstruction inference: the established sampled, closed polygon contour makes these polygon-related for N's pattern rule. No recovered specification names these opcodes; the inference is stated in the delta and command entries. It is not inferred solely from a GDI function name. |
| Standalone &/] use line pattern, thickness, Draw/Back Color and W | Reconstruction of standalone polygon/polyline drawing roles, applying the documented line-style and general W rules. N controls filled borders, not standalone strokes. |
| N0 removes radial/closing outlines too | Documentary border-disable intent applied consistently to the preserved contour. A historical unconditional LineTo is not a protocol exception. A radial-only contour has no fill region. |
| Fill-pattern zero bits use Back Color; line gaps may be transparent | Fill definitions and LINE_STYLE §3.4.1.29, lines 6433–6442. Documented off_draw 0/1 select transparency/background. The previously recovered 2/3 width-unit extension and its value-3 behavior are labeled historical rather than invented independent bit semantics. |
| W04 complements D; bitmap mode 04 complements S | WRITE_MODE's inverse mode and the existing explicit image-mode definitions. They are separate operations, not two consecutive filters on an image. |
| Graphics text honors W | WRITE_MODE's graphics-text scope. Native fixed FastFont masked replacement is historical behavior, not another exception. Exact overlap grouping for glyphs is not settled by the point/stroke vectors. |

This resolves the intended protocol outcome requested by #74, rather than narrowing PIXEL's attributes to a replacement-only API. Original-client X/j outcomes under all five modes remain **unmeasured**: no native pixel matrix is claimed. The [SetPixel](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setpixel) and [FillRect](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-fillrect) documentation describes color/brush drawing, while [SetROP2](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setrop2) describes the foreground mix. Those API descriptions and the issue's static call sites are a boundary warning, not experimental proof of every client mode. Backend calls must produce the specified result regardless of API defaults.

## Logical phases, gaps and repeated visits

The [shared contract](../../2.0/techspecs/2.3-drawing-composition.md) publishes all five Boolean equations and a ten-row, independently specified 8-bit matrix. D=0xA5 is deliberately nonblack; point/fill source is 0x3C, border source 0x0F, Back Color 0x66. Point/fill results are 3C/99/BD/24/5A for COPY/XOR/OR/AND/NOT. Fill followed by a solid border is 0F in every column, because border COPY replaces the already-composed fill. An opaque patterned border gap similarly produces 66; a transparent gap leaves the prior fill/destination intact.

The four N/opaque-gap combinations have one fill phase with N0 and fill then COPY-outline with N1. For solid-only I/i/_ borders the gap policy is irrelevant. A filled full Pie's internal radius is the same solid COPY outline, absent with N0. Degenerate radial contours have no fill phase. These are **logical** phases; old flag 4 branches or exactly-one-background-flag native combined calls do not dictate normative call counts. The checker does not pretend that the old arc fixture, which substitutes pen/ROP services, established these color results.

Genuine standalone retraversals operate sequentially on the latest D: two identical XOR/NOT contributions restore it. In contrast, even-odd polygon winding determines region membership first; two coverings form a hole, three form one filled region. A repeated COPY border is idempotent. Shared joins/zero-length closure records do not alone establish additional pixel contributions. Exact edge membership, caps, joins and antialiasing remain outside this algebra. Preserve the previous normalizer: raw 0..720 is reduced to one turn, while parameter 90 and 450 genuinely revisit a point when both belong to a span such as 0..719.

## Read-only numeric recovery

Original artifact: `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` under the persistent mirror catalogued in [rip-tools](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. No modified DLL or native execution was used. LLVM objdump 19.1.7 displays image virtual addresses with base 0x10000000; subtract that base for the RVAs below. File offsets are not interchangeable with RVAs.

| Data | RVA | File offset | Bytes | SHA-256 |
| --- | --- | --- | --- | --- |
| 360 signed LE int32 sine coefficients | 0x7b638 | 0x79c38 | 1440 | `a08fb7e140b46078a0992959300412410fb2caa53f3f1c689dfcebdf5d4132fc` |
| 360 signed LE int32 cosine coefficients | 0x7b098 | 0x79698 | 1440 | `f4bde71f06d692151350f9837c06b7f12de444cccdd1d49cec9dd35db3beee72` |

All 720 JSON entries were compared with `struct.unpack('<360i', raw)` over those original byte slices, and both byte hashes verified. Scale is 16384. S[30]=8191 versus S[330]=-8192 and C[60]=C[300]=8192 rule out generating ideal rounded sine or deriving cosine by shifting this sine table.

Read-only reproduction, with `artifact_root` set to the persistent mirror:

```sh
objdump -d --start-address=0x10010201 --stop-address=0x10010244 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x1001fb37 --stop-address=0x1001fbb5 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
```

The table-byte comparison is reproducible from the repository root (read-only):

```sh
python3 - "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll" <<'PYTABLE'
import hashlib, json, pathlib, struct, sys
raw = pathlib.Path(sys.argv[1]).read_bytes()
profile = json.loads(pathlib.Path("version/3.0/techspecs/data/skewed-ellipse-q14.json").read_text())
assert hashlib.sha256(raw).hexdigest() == profile["dll_sha256"]
for name, offset in (("sine", 0x79c38), ("cosine", 0x79698)):
    table = raw[offset:offset + 1440]
    assert list(struct.unpack("<360i", table)) == profile[name]
    assert hashlib.sha256(table).hexdigest() == profile[name + "_sha256"]
    print(name, "360 original entries verified")
PYTABLE
```

Generator 0x10160 loads parameter sine/cosine, multiplies each by its converted radius and SARs each by 14 at 0x10206/0x10214. Rotation multiplies at 0x1021b/0x1021e/0x10221/0x10231, combines products before the SARs at 0x1022a/0x10238, then adds X/subtracts Y center at 0x10234/0x1023f. Therefore floor each radius product separately, then floor each combined rotation dot-product. SAR means floor toward negative infinity, not C signed division toward zero. Separately rounding both rotation products is also incorrect.

The ] producer converts center/radii with 0x31084 calls at 0x1fb40/0x1fb51, adds the selected viewport origin at 0x1fb77–0x1fb99, and calls the generator at 0x1fbb0. Conversion precedes rotation; the origin is added once to the center. The reconstructed geometric rule rotates the converted device ellipse counterclockwise with device Y downward. Nonuniform world 1280×960/device 640×960, center (100,80), origin (5,7), radii (20,10), rotation 90, parameter 0 gives (55,77); rotating in world space first instead gives (55,67).

The [arc consumer page](../techspecs/2.1-arc-consumer-contract.md#coordinate-evaluation-and-rotation) publishes exact formulas and nine numeric rows, with full coefficient data and a conversion vector. Center (100,80), radii (13,7), r=t=30 gives first-stage u=11, v=3 and final (108,72); t=330 gives (111,78), exercising negative-floor behavior. A wide product with Rx=131072 and zero angles gives (131072,0).

The complete Q14 constants and staged rounding define an **optional quantized-contour profile**. Published protocol documentation does not demand these exact trig approximations. Required reconstructed geometry/conversion order is separate from profile selection. Use wide arithmetic for that profile; original signed 32-bit product/bounding-box overflow is not required for protocol conformance. No new client overflow/bug investigation was performed.

## Similar-case audit and public changes

The review covered color modes, lines, curves, filled shapes, graphics text, image operations and their linked numeric/font technical pages. It found and addressed:

- K and u's native current-W/patterned-border claims conflicting with the explicit N rule; restored solid COPY without changing previously derived rectangle/radius geometry.
- N's blanket withdrawal of the documented COPY exception; replaced it with fill-W/border-COPY and explicit polygon classification.
- Filled polygon/poly-Bézier outline notes that said only “does not use Write Mode”; made COPY and fill-rule ordering explicit. The primitive-role table also covers G/o and poly-polygons.
- LINE_STYLE gap versus background fill, and custom-mask/wide-pen backend substitutions; preserved actual requested patterns rather than defining GDI limitations as protocol behavior.
- W04 versus image mode 04; cross-linked the separate source/destination operands and prevented applying global W after the explicit image operation.
- FastFont fixed-copy observations being readable as a protocol exemption; both the text command and font technical page now mark them historical and retain the documented W scope.
- The arc page's original N0 radial call table; retained it as a historical topology record with an immediate explicit protocol N0/COPY clarification.

RIPscrip 1.54 and all preserved original `text/`/asset files remain unchanged. This task does not broaden into font rasterization, client bug reproduction or exact GDI edge pixels.

## Validation and delivery checkpoint

`run/check-drawing-contract` is a read-only Deno checker using per-bit truth tables and independently specified expected vectors. It checks the published pixel and numeric Markdown rows, screen/offscreen abstract point footprints at thickness 1/3 (surface origin (0,0), point (3,3)), all five modes, N/gap combinations, fill parity, repeat visits, image/drawing NOT separation, all table bytes via hashes, nine numeric profile vectors and nonuniform conversion. It reads no executable and invokes no client or renderer. Deno 2.9.8 / TypeScript 6.0.3: **1,165 checks pass**. Both coefficient arrays were additionally compared with the original file via Python as described above. JSON uses the repository's text/LF default; no LFS/binary rule is needed.

Validation also passes Deno type checking/lint, wrapper help, repository Markdown formatting/lint, `run/check-links` (0 bad of 5,516 links across 225 files) and `git diff --check`. Manual prose review found no joined word/number shorthand. GitHub refresh at **2026-11-18 04:55:37 UTC** found no comments on #74/#75/#76 and no newer issue; dependencies #71/e800949 and #73/d8323a1 are unchanged and open.

Remaining delivery: combined PR closing #74/#75/#76, then normal ancestry-preserving merge into fork stable. Do not merge upstream or manually close issues. All new commits need the requested Codex co-author trailer; keep temporary API payloads in scoped temporary directories.

---

[Research backlog](issue-backlog.md) · [Write modes](../ripscrip/2.0-color-modes-and-palettes.md#rip_write_mode)
