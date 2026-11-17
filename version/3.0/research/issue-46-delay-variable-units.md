# Issue #46: D text-variable delay units and timing

[Research backlog](issue-backlog.md) · [D variable](../ripscrip/5.1-text-variables.md#d)

Started after [PR #65](https://github.com/bbs-land/remote-imaging-protocol/pull/65) delivered #42. Branch `issue-46-delay-variable-units` starts at **325a2c9**. [Issue #46](https://github.com/bbs-land/remote-imaging-protocol/issues/46) asks to reconcile milliseconds summary versus sixtieths detail/inventory and confirm D(1)/D(60), without changing 1.54. Its proposed equivalence to wire3D's nonblocking input gate must be checked against the actual variable callback.

Primary source is original RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`, and extracted `ripscrip.dll`, 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base0x10000000. Addresses are RVAs. No real waits/audio/desktop GUI; native fake-clock service only. Clean scoped task temporary builds/request payloads promptly.

Initial trace: registration at0x264e6 selects callback **0x2832d**, which requires one parameter and checks only its first byte's digit class before original numeric conversion0x68c10. It calls standalone **0x282ca(duration)**, not the wire3D handler/input gate. Helper multiplies by1000 then unsigned-divides by60; values>3900 split into ((duration-1)/3900) chunks of65000ms, plus the final remainder's floored conversion. Each interval busy-polls **timeGetTime IAT0x966fc** until its unsigned time>=deadline. No Sleep/message-pump/nonblocking pause state appears. This predicts D(1) target16ms and D(60) target1000ms, with timer-resolution overshoot separate. No explicit 1..65535 gate appears in the callback; test zero/large/numeric-prefix and timer-wrap behavior before publication. No #46 reference edits/PR yet.

Next: registered-callback/native fake-clock duration matrix and full expansion, then reconcile D entry/inventory and file complete PR dependent on #65.
