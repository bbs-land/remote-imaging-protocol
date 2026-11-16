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

## Completed endpoint-domain and full-figure checks

The preserved 1.54 text's full-turn/empty intent is at lines **1228–1233**, and the 2.0 draft repeats it at **4325–4330** (OVAL_ARC full ellipse at **6693**). Their documented 0..360-full assertion is distinct from the actual RIPtel branches captured here. Original text remains byte-exact; this issue does not infer earlier-client runtime behavior from the later binary.

The expanded probe now passes **18,106,873 checks**. It retains the full 4096² original skewed-normalizer domain and adds **295,344 non-skewed handler invocations**: all six A/O/V/I/i/_ handlers, both border settings, radii 2/40/200, all 4096 possible angles in each endpoint position, and the twelve explicit paired cases at every profile. An independent arithmetic ray rule using the nineteen original table values at **RVA 0x76118 / file offset 0x75118** matches every captured endpoint. It asserts actual primitive/pass count and O's unchanged complete bounding rectangle, not just successful return. All eleven relevant original dispatch records have radix flag bits 3; selector **0x39d70** therefore uses current base math rather than a forced radix. These width/radix observations support the numeric range stated in the reference. This verifies identical-integer-endpoint branches, radial lines even without borders, default O behavior and noncoincident Arc/Pie/Chord arguments.

The exact table and quadrant formula are published in the [consumer contract](../techspecs/2.1-arc-consumer-contract.md#axis-aligned-endpoint-rays). [Microsoft Arc](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-arc), [Pie](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-pie), [Chord](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-chord) and [CDC Pie remarks](https://learn.microsoft.com/en-us/cpp/mfc/reference/cdc-class?view=msvc-170#pie) provide the primary API ray/direction/closure boundary. Any interpretation of rendered curve intersections is an API-contract derivation; the native probe itself records arguments only.

Full skewed &/- commands are also executed for both border settings and rotations 0/90/450. They request generator 0..360, then pass **literal 360** to Polygon, rather than forwarding all 361 generated samples. Rotation 450 produces the same first point as 90. An initial extra test incorrectly expected a 361-point Polyline for &: tracing the failing assertion to **0x1fa2a–0x1fa38** proved its actual Polygon(360), and **0x1c475/0x1c4c0** proves the filled sibling's same count. The fixture expectation was corrected from primary instructions. The full-figure and angular commands therefore have distinct closure counts; the specification does not conflate their consumers.

## Fill-attribution and source reconciliation

`git -C <artifact-root>/riplib pull --ff-only origin main` reports already up to date at **024032a**. Its segment 11 still attributes a manual arc/radii flood leak to the original DLL under `§BUG.6`, but positive native/static I/i/_ and skewed traces contradict that construction. The canonical 2.0 fill record, 3.0 delta, language entry and relevant vendor-summary endorsements are corrected together, with targeted-review scope recorded in the vendor READMEs. This withdraws that specific historical attribution; it does not claim a full revalidation of every unrelated RIPlib defect/extension or earlier-client rasterizer. The separate legacy RIP_FILL `§DEAD.7` account remains identified as adopted secondary evidence, not as a finding of this arc probe.

## Specification and next action

The reference now gives command-specific positive-sweep/degenerate decisions, all requested raw equal/full/wrapped/multiple cases and the exact skewed normalization algorithm. The numerical consumer page supplies the ray table/formula and all point closures, including internal center edges in a full skewed pie. Original O/V behavior and skewed field widths replace census/peer-only assumptions in the command entries and inventory. The documented-intent versus shipped-full-turn discrepancy is explicit; no uniform repaired policy is silently invented. Earlier-language angle behavior and exact Windows pixel reproduction remain outside the demonstrated scope.

`run/format` applied to all changed Markdown; `run/lint` passed; `run/check-links` passed **4859 links across 198 files** before this validation record; `git diff --check` and wrapper syntax passed. The final native probe passes **18,106,873 checks**. Original 1.54/2.0 text hashes remain unchanged and no task arc temporary directories remain. All issue subquestions are answered by the documented command-specific contract and stated API boundary.

Inspect the final issue-only change, inspect the final issue-only change, then commit/push and file the complete #14 PR dependent on #41. Issue-only comparison begins at **2950732**. Record delivery and refresh creation order before #15; do not merge, manually close, or wait for review.

## Temporary-file cleanup

The user requested cleanup on 2026-11-16. Removed 46 completed task-owned `/tmp` research scripts, PR bodies, debug binaries and captures (564,724 bytes) after their results were preserved in committed notes/tools or published PRs. Other tasks' temporary files were left alone. Future temporary outputs must use automatic cleanup or be removed immediately when consumed; persistent evidence belongs in the external artifact mirror and committed derivation notes.

---

[Research backlog](issue-backlog.md) · [Curves](../ripscrip/2.2-curves.md)
