# Issue #205 — Block-mode requests and completion

[Research backlog](issue-backlog.md) · [RIP_ENTER_BLOCK_MODE](../ripscrip/5.5-file-transfer-and-queries.md#rip_enter_block_mode)

## Scope and checkpoint

[RealDeuce report #205](https://github.com/bbs-land/remote-imaging-protocol/issues/205), created 2026-11-23 23:49:02 UTC, body SHA-256 `824dac9896a1ee2885b90cdd4afd2d097aa146063a9e920e729331ef2caad411`, zero comments on intake. The 2026-11-24 00:04:19Z GitHub audit found only #205 uncovered, with all 100 open PR heads ancestral. Started from fork stable `34515c5d1ad7d457344150aa4ae485f31f79454b`.

The DLL handler confirms the numeric precheck and the empty-name receive threshold. Next trace the protocol dispatch tables, posted messages, connected-client dispatch and completed-file consumer. Keep the inherited language profile distinct from DLL acceptance, and record the scope of any static transfer or failure evidence. No native transfer, client or GUI execution.
