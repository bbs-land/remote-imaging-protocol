# Issue #200 — Physical CLS fill

[Research backlog](issue-backlog.md) · [CLS](../ripscrip/5.1-text-variables.md#cls)

## Scope and checkpoint

[RealDeuce report #200](https://github.com/bbs-land/remote-imaging-protocol/issues/200), created 2026-11-23 23:01:04 UTC, body SHA-256 `858ff331c3ab85686db499f4a94a28b1457d9299995275d64a8012ed16c1cdec`, zero comments on intake. Started from fork stable `ed0313f388790981f7ef83762ec029a2a1197e85`. The 23:05:47Z audit queues #200 followed by #201.

The read-only original-driver trace confirms the shared physical black fill helper from HARD, called directly by CLS. Next verify the brush and DC/rectangle inputs, caret/status presentation boundary and a distinguishing protocol vector. Preserve the original Alpha 4 record, the no-reset behavior and the status presentation contract.
