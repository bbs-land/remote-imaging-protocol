# Issue #14: Arc angle ordering and sweep policy

[Research backlog](issue-backlog.md) · [Curves](../ripscrip/2.2-curves.md) · [Pie slices](../ripscrip/2.3-shapes-and-fills.md#rip_pie_slice)

## Scope and source

Started 2026-11-16 after [PR #41](https://github.com/bbs-land/remote-imaging-protocol/pull/41) delivered #13. GitHub creation-order refresh confirms [issue #14](https://github.com/bbs-land/remote-imaging-protocol/issues/14) is the oldest RealDeuce issue without a complete resolution PR. Branch `issue-14-arc-sweeps` starts at **2950732**. Required answers: correct start/end order or wrap rule; equal-angle behavior for A/O/V/pie/chord/skewed variants; 360/720 and displaced/multiple-turn normalization; and full-turn pie/chord boundaries without extra internal strokes. The user seeks a positive geometric sweep contract independently of historical flood-fill raster defects.

Primary artifact is the unchanged RIPtel 3.1 / driver 3.0.7 `extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred base 0x10000000, under the [catalogued](../../../reference/rip-tools.md) external mirror `artifacts/riptel-3.1-pcmicro/`. All addresses below are RVAs; all file offsets must be labelled separately. No GUI is needed. Preserve original text/assets and include Codex co-authorship in every new commit.

## Original dispatch map

| Slot | Opcode | Handler | Width descriptors |
| ---- | ------ | ------- | ----------------- |
| 17   | A      | 0x19b50 | XY,XY,2,2,XY      |
| 50   | O      | 0x1d297 | XY,XY,2,2,XY,XY   |
| 66   | V      | 0x1d497 | XY,XY,2,2,XY,XY   |
| 38   | I      | 0x1e066 | XY,XY,2,2,XY      |
| 39   | i      | 0x1dc59 | XY,XY,2,2,XY,XY   |
| 82   | _      | 0x1bb18 | XY,XY,2,2,XY,XY   |
| 3    | &      | 0x1f904 | XY,XY,2,2,2       |
| 9    | -      | 0x1c348 | XY,XY,XY,XY,2     |
| 81   | ]      | 0x1fac7 | XY,XY,XY,XY,2,2,2 |
| 80   | [      | 0x1fee1 | XY,XY,XY,XY,2,2,2 |
| 7    | +      | 0x1fca6 | XY,XY,XY,XY,2,2,2 |

These original records already contradict some census-only fixed-width descriptions. Resolve angle semantics first, carrying any directly relevant width corrections into the final reference; do not infer geometry from other implementations.

## Initial static findings

**O and V are not interchangeable in the shipped DLL.** O's **0x1d297–0x1d496** reads the center and radii at decoded slots 0/1/4/5 but never the angle slots 2/3. Nonzero radii reach GDI **Ellipse at 0x1d40e**; zero-radius paths draw a point/axis line. V's separate **0x1d497–0x1d5c1** reads both angles and invokes the elliptical-arc helper **0x11b20** at **0x1d556**. The current reference's claim that SyncTERM fall-through/help-name silence proves an original-driver alias is therefore unsupported and must be replaced after native confirmation.

A **0x19b50** sends raw start/end to circular helper **0x10dd0** at **0x19c7b**. That helper normalizes each angle separately only when greater than 360 (strict greater-than), computes endpoint rays using the original five-degree lookup table and ultimately delegates to GDI. V's helper **0x11b20** starts with a similar separate normalization. Neither helper uses the skewed-family normalizer below. Exact equal/whole-turn behavior must be traced to the final GDI arguments and its documented contract; do not assume the inherited "equal means empty" sentence is an original-driver observation.

The skewed arc/chord/pie handlers call in-place angle helper **0x138e8–0x139b4**: ] at **0x1fb26**, + at **0x1fd05**, [ at **0x1ff40**. This helper treats pairs in 0..360 differently from pairs above 360, and has order-dependent branches; deriving one simple modulo formula without exhaustive native comparison would be premature. Their point generator is **0x10160**, also used by filled skewed oval and full skewed oval paths. It normalizes rotation at **0x1018d**, then generates samples from the processed start through end **inclusively** (count=end-start+1 at **0x101c8–0x101d3**). Equal endpoints may therefore generate one point; whether anything is drawn depends on each consumer and border path, not just this sample count.

The non-skewed pie/chord handlers and their full-turn construction remain untraced in this issue. Existing fill-defect notes are context, not a substitute for confirming angle handling and exact outline/fill consumer arguments.

## Native checkpoint: normalization and nine curve consumers

New `run/riptel-arc-probe` verifies the unchanged DLL hash and compiles a temporary native i386 process with cleanup on exit and core dumps disabled. On FreeBSD 15.1 amd64 / clang 19.1.7, the initial checkpoint passes **16,777,931 checks**: all **4096² = 16,777,216** raw two-digit UltraNum start/end pairs through original **0x138e8**, plus captured A/O/V/I/i/_ and skewed ]/+ /[ calls. Raw normalized-span counts are **362 zero**, **9,085,623 partial (<360)**, **46,326 exactly 360**, and **7,644,905 greater than 360**. No Windows rasterization executes.

An arithmetic reduction independently matches every raw pair. Preserve the <=360 fast path separately: if both raw values are <=360, leave start unchanged and add 360 to end only when start>end. Otherwise, when start<end, subtract 360*floor(start/360) from both, then if end>=720 replace end with `360 + (end-360)%360`. Otherwise subtract 360*floor(end/360) from both, reduce any remaining start-end>360 by `360*floor((start-end-1)/360)` from start, subtract a further 360 from start if start>=360, and finally add 360 to end. All operations here apply to nonnegative decoded angles 0..4095. This deliberately preserves historical discontinuities; it is not the common modulo-positive-sweep formula.

Observed original non-skewed calls at center (100,100), radii 40×20, default identity world/device scale:

| Raw angles | A / V | I / i | _ | O |
| --- | --- | --- | --- | --- |
| 0..0 | MoveToEx(140,100), LineTo(140,100) | Center-to-(140,100) line | No drawing call | Complete Ellipse |
| 0..360 | Same zero-length line | Same center-to-edge line | No drawing call | Complete Ellipse |
| 360..0 | Same zero-length line | Same center-to-edge line | No drawing call | Complete Ellipse |
| 450..810 | Zero-length line at upper ray | Center-to-upper-ray line | No drawing call | Complete Ellipse |
| 0..720 | Zero-length line at right ray | Center-to-right-ray line | No drawing call | Complete Ellipse |
| 270..90 | Arc with bottom/top rays | Pie with bottom/top rays | Chord with bottom/top rays | Complete Ellipse |
| 90..270 | Arc with top/bottom rays | Pie with top/bottom rays | Chord with top/bottom rays | Complete Ellipse |

A and V test the computed integer endpoint **coordinates** for equality, then bypass GDI Arc on equality; I/i instead draw a radial line, and _ bypasses drawing entirely. Other endpoints go to actual GDI Arc/Pie/Chord APIs. The integer endpoints come from five-degree table lookup, so endpoint-ray coincidences also arise for non-equal abstract angles. For example, A/V 0..1 and 0..719 both send (140,100) and (139,100), two different integer points on the same ray. The exact API ray contract therefore matters if deriving a geometric sweep; an abstract modulo delta alone does not reproduce original decisions.

The skewed path differs. Native ] samples `(S..E)` inclusively and sends `E-S+1` points to Polyline. + appends the first point and sends `E-S+2` points to Polygon; [ appends the **center and first point**, unconditionally, sending `E-S+3` points. Both border settings are exercised. For raw 0..360, ] has 361 samples, + has 362 total points and [ has 363; [ retains the center even on the closed full ellipse, so its border path contains internal radial edges. Raw 360..0 becomes 360..360 (one arc sample); raw 450..450 becomes 90..450 (361 samples); raw 0..720 becomes 0..360 (361 samples); raw 0..719 retains 720 samples. These observations rule out silently replacing the original normalizer with a uniform clamp or modulo policy.

The filled non-skewed handlers call **GDI Pie/Chord directly**. They do not assemble an arc/radii boundary and then invoke ExtFloodFill in the traced paths. Existing repository attribution of an original-driver manual flood leak to these handlers is contradicted by positive native/static evidence and must be reconciled before delivery. The local RIPlib note says its `draw_pie` used a leaking flood fill; it must not be silently promoted into an observed original RIPtel implementation.

Exact current hook boundary: no-op entry/caret/UI/DC/clip/ROP/invalidation **0x45038, 0x18cf6, 0x18bf7, 0x3988f, 0x398ee, 0x13c69, 0x343e4, 0x342ad, 0x34319, 0xe6e9**; selection helpers **0x13cfc, 0x13ce5, 0x13d10, 0x13d27** return handle 1; disabled viewport **0x3445b** returns zero; error reporter **0x38e47** captures errors. Controlled IAT calls are SelectObject **0x963c0**, SetBkMode **0x963f0**, SetRect **0x96618**, InflateRect **0x96698**, MoveToEx **0x963dc**, LineTo **0x963cc**, Arc **0x96464**, Ellipse **0x96480**, Pie **0x9649c**, Chord **0x96498**, Polygon **0x9641c**, Polyline **0x9647c**. Actual handlers, endpoint calculations, shared normalizer, original one-degree rotated point generator **0x10160**, world conversion and original topology edits execute. No bitmap/palette/raster outcomes are simulated.

One fixture error was corrected: the initial Pie substitution used 0x96494 instead of the actual IAT **0x9649c**, causing a call into an unresolved import. The corrected IAT is verified by annotated original call **0x11a5d**. A scoped Python TemporaryDirectory held the debug executable and removed it immediately; the new wrapper disables cores to avoid crash-dump storage. This failure is not driver behavior.

## Next action

Build a hash-guarded native arc probe, reusing the committed image mapper and controlled entry/DC/clip services. First execute pure 0x138e8 across the full two-digit UltraNum start/end domain, preserving raw versus normalized pairs. Trace final Arc/Ellipse/Polygon/Polyline/flood arguments for A/O/V/I/i/_ and skewed ]/[ /+, with border on/off. Cover raw equal angles, 0..360, 450..810, 0..720, decreasing/wrapped pairs and translated equal pairs, keeping each backend distinct until evidence supports a shared rule. Capture the original point generator where needed; do not substitute a modern trigonometric renderer for the original decisions. Distinguish normative positive-sweep policy from shipped defects explicitly before filing a complete PR.

## Temporary-file cleanup

The user requested cleanup on 2026-11-16. Removed 46 completed task-owned `/tmp` research scripts, PR bodies, debug binaries and captures (564,724 bytes) after their results were preserved in committed notes/tools or published PRs. Other tasks' temporary files were left alone. Future temporary outputs must use automatic cleanup or be removed immediately when consumed; persistent evidence belongs in the external artifact mirror and committed derivation notes.

---

[Research backlog](issue-backlog.md) · [Curves](../ripscrip/2.2-curves.md)
