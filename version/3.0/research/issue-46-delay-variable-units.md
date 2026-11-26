# Issue #46: D text-variable delay units and timing

[Research backlog](issue-backlog.md) · [D variable](../ripscrip/5.1-text-variables.md#d)

Started after [PR #65](https://github.com/bbs-land/remote-imaging-protocol/pull/65) delivered #42. Branch `issue-46-delay-variable-units` starts at **325a2c9**. [Issue #46](https://github.com/bbs-land/remote-imaging-protocol/issues/46) asks to reconcile milliseconds summary versus sixtieths detail/inventory and confirm D(1)/D(60), without changing 1.54. Its proposed equivalence to wire 3D's nonblocking input gate must be checked against the actual variable callback.

Primary source is original RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`, and extracted `ripscrip.dll`, 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base 0x10000000. Addresses are RVAs. No real waits/audio/desktop GUI; native fake-clock service only. Clean scoped task temporary builds/request payloads promptly.

Initial trace: registration at 0x264e6 selects callback **0x2832d**, which requires one parameter and checks only its first byte's digit class before original numeric conversion 0x68c10. It calls standalone **0x282ca(duration)**, not the wire 3D handler/input gate. Helper multiplies by 1000 then unsigned-divides by 60; values>3900 split into ((duration-1)/3900) chunks of 65000 ms, plus the final remainder's floored conversion. Each interval busy-polls **timeGetTime IAT 0x966fc** until its unsigned time>=deadline. No Sleep/message-pump/nonblocking pause state appears. This predicts D(1) target 16 ms and D(60) target 1000 ms, with timer-resolution overshoot separate. No explicit 1..65535 gate appears in the callback; test zero/large/numeric-prefix and timer-wrap behavior before publication. No #46 reference edits/PR yet.

Next: registered-callback/native fake-clock duration matrix and full expansion, then reconcile D entry/inventory and file complete PR dependent on #65.

## Native fake-clock checkpoint

`run/riptel-macro-probe` now passes **3,459,218 checks**. Original registration confirms D callback 0x2832d. Original callback, numeric conversion, busy-wait helper 0x282ca and full expansion execute. Only the **stdcall timeGetTime import 0x966fc** is substituted: each expected interval returns a start, then deadline-1, then deadline, so the exact comparison/rounding is tested without sleeping or real waiting. Zero interval returns the equal deadline on its first poll. Polls are bounded; no real GUI/audio/time delay is introduced. Existing variable-engine controls from earlier notes remain; this adds no new callback/processor hook.

Exhaustive durations **0..65536** verify `chunks=(n-1)/3900` when n>3900, remainder `n-chunks*3900`, each chunk 65000 ms and final `floor(remainder*1000/60)`. Total requested clock interval is exactly `floor(n*1000/60)` over this domain. Therefore **D(1) targets 16 ms** and **D(60) targets 1000 ms**; the unit is a sixtieth, with fractional milliseconds truncated. Actual wall-clock elapsed time remains subject to clock granularity/phase/scheduling; native fake time establishes target arithmetic, not an exact physical runtime.

Original D requires exactly one argument, but it does **not** enforce the draft's 1..65535 gate. Zero and 65536 pass; only the first character is checked as a digit before original atoi-like prefix conversion. Native `1A`/`1.5` become 1, `60x`/`00060` become 60; plus/minus/blank/letter/space starts reject without clock calls. This is a shipped parser quirk, not an invitation to advertise arbitrary oversized conversions as portable protocol values. The documented ordinary input range remains separately identified. All missing/extra-argument cases reject. Full dollar expansion consumes D(1),D(60),D(0),D(65536),D(1A) into no text after synchronous polling completes.

A controlled timer start 0xfffffff8 with D(1) makes deadline wrap to 8; the next unsigned timer value 0xfffffff9 already compares above 8, so original delay returns immediately after that poll. This records the existing wrap defect rather than pretending its unsigned absolute comparison is rollover safe. Default native execution does not install any context-level asynchronous input gate or dispatch the wire 3D handler: helper takes only duration, busy-polls its timer and returns on the same evaluation call stack.

Next: change the D summary to sixtieths, publish 1/60 target arithmetic, distinguish blocking helper from wire scheduling and mark validation/wrap quirks; update inventory/evidence, run required checks and file complete PR.

## Reference correction and validation scope

D summary now says sixtieths and gives exact integer clock targets for 1/60/3900. The reference distinguishes timer targets from physical runtime, synchronous evaluation from the wire-delay mechanism, documented ordinary range from observed prefix/zero/oversize quirks and unsigned deadline wrap. Inventory agrees and retains corpus cadence evidence; earlier-version reference/text and wire-delay contract remain untouched.

Reproduction: `run/riptel-macro-probe /path/to/original/ripscrip.dll` (FreeBSD 15.1/clang 19.1.7/native -m32); original DLL hash verified by wrapper. **3,459,218 checks pass**, with the fake-clock service bounding all polls and no real delay/audio/GUI. Scoped wrapper builds and PR payloads are removed on completion. Final required checks and PR link follow at delivery.

Delivered in [PR #66](https://github.com/bbs-land/remote-imaging-protocol/pull/66), dependent on #65. Specification commit **28cfba2**, issue-only comparison starts at **325a2c9**. Required checks pass: 3,459,218 native macro checks, formatting, 0 bad of 5,307 links and diff check. Task temporary build/request files are removed; no real waits/audio/GUI. Refreshed queue selects #51 next; do not merge or manually close #46.

[Issue #290](issue-290-wire-delay.md) subsequently verified that wire 3D also calls the same timer helper through its own thunk. It records the shared synchronous timer and the portable stream-buffering rule.
