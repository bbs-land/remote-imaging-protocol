# Issue #53: host control notation, special keys and brackets

[Research backlog](issue-backlog.md) · [Host commands](../ripscrip/5.0-host-commands.md)

Started after [PR #67](https://github.com/bbs-land/remote-imaging-protocol/pull/67) delivered #51. Branch `issue-53-host-control-brackets` starts at **bf85f10**. [Issue #53](https://github.com/bbs-land/remote-imaging-protocol/issues/53) requests exact bytes/template effects for caret/backquote+`[[A`, `^[[Z`, raw `[`, trailing caret/backquote, `^?` and lowercase controls, plus accepted control range and malformed behavior. Resolve the apparent contradiction between listed key encodings and a blanket no-literal-bracket rule. Queue after this issue: #54, #58.

Primary source is original RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`, and extracted `ripscrip.dll`, 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base0x10000000. Addresses are RVAs. Native capture only; no GUI/audio/actual host transmission. Scoped builds/payloads clean themselves on completion.

Next: trace original host segmentation/control conversion and template expansion in #6’s existing macro probe. Test byte matrices through full original expansion before editing the reference or filing the PR. Preserve #6’s established evaluation order and distinguish callback controls from original parsing.
