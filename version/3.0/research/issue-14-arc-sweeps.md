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

## Next action

Build a hash-guarded native arc probe, reusing the committed image mapper and controlled entry/DC/clip services. First execute pure 0x138e8 across the full two-digit UltraNum start/end domain, preserving raw versus normalized pairs. Trace final Arc/Ellipse/Polygon/Polyline/flood arguments for A/O/V/I/i/_ and skewed ]/[ /+, with border on/off. Cover raw equal angles, 0..360, 450..810, 0..720, decreasing/wrapped pairs and translated equal pairs, keeping each backend distinct until evidence supports a shared rule. Capture the original point generator where needed; do not substitute a modern trigonometric renderer for the original decisions. Distinguish normative positive-sweep policy from shipped defects explicitly before filing a complete PR.

## Temporary-file cleanup

The user requested cleanup on 2026-11-16. Removed 46 completed task-owned `/tmp` research scripts, PR bodies, debug binaries and captures (564,724 bytes) after their results were preserved in committed notes/tools or published PRs. Other tasks' temporary files were left alone. Future temporary outputs must use automatic cleanup or be removed immediately when consumed; persistent evidence belongs in the external artifact mirror and committed derivation notes.

---

[Research backlog](issue-backlog.md) · [Curves](../ripscrip/2.2-curves.md)
