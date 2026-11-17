# Issue #54: SWITCH_PORT protection order and undefined targets

[Research backlog](issue-backlog.md) · [RIP_SWITCH_PORT](../ripscrip/7.0-drawing-ports.md#rip_switch_port)

Started after [PR #68](https://github.com/bbs-land/remote-imaging-protocol/pull/68) delivered #53. Branch `issue-54-switch-port-precedence` starts at **9e9995f**. [Issue #54](https://github.com/bbs-land/remote-imaging-protocol/issues/54) asks same-target flags06/09, per-operand conflicts03/0c, source protection before missing-destination fallback and destination protection targeting. Keep the separate reserved-field layout issue #2 and broader state ownership #24 distinct. Next queued issue: #58.

Primary source is original RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`, and extracted `ripscrip.dll`, 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base0x10000000. Addresses are RVAs. Native/static only; no Wine/GUI/audio. Clean scoped temporary builds/payloads promptly.

Next: identify physical wire handler, trace protection calls and selection/fallback order, then extend existing native port/table probes with exhaustive flags/current/target/protection matrices. Do not infer wire fallback from the independent CUR setter, which can create undefined entries.
