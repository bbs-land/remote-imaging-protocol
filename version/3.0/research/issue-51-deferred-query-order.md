# Issue #51: deferred query order and live mutation

[Research backlog](issue-backlog.md) · [RIP_QUERY](../ripscrip/5.5-file-transfer-and-queries.md#rip_query)

Started after [PR #66](https://github.com/bbs-land/remote-imaging-protocol/pull/66) delivered #46. Branch `issue-51-deferred-query-order` starts at **8338765**. The refreshed open RealDeuce queue selects [#51](https://github.com/bbs-land/remote-imaging-protocol/issues/51), followed by #53, #54 and #58. Determine numbered/floating TW/viewport dispatch order, the meaning of resident and whether an earlier query can change later eligibility during the same click. Do not infer this from #6 expression ordering or #16 snapshot ownership.

Primary source is original RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`, and extracted `ripscrip.dll`, 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base0x10000000. Addresses are RVAs. Use native/static probes without Wine, desktop windows or audio. Remove scoped temporary builds/payloads promptly.

Initial static trace: physical dispatch slot85 selects wire query handler0xd3da. Modes1/2 store floating strings at port-system+0x2a / TW-system+0xc, modes3/4 at port record+0x64 / TW record+0x3f. Definition helper0x13e61 allocates owned copies and treats case-insensitive `$OFF$` as removal. Immediate mode calls0x13ed0, copying into host-command buffer before0x4ab3e. RIP_ProcessEvent0x43f5 left-down branch0x52e7 checks mouse fields backwards and calls0x2c424 only on a hit;0xe749 is merely rectangle hit testing. This does not yet identify blank-space deferred dispatch. Trace callers/event continuation and then execute the original dispatch with controlled host actions before any reference correction.

Next: locate actual deferred click loop; prove full ordering, rectangle boundaries and live versus snapshotted state using original native code, then update the reference and file one complete PR dependent on #66.
