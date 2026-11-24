# Issue #207 — Backup query aggregates

[Research backlog](issue-backlog.md) · [INUSE](../ripscrip/5.1-text-variables.md#inuse) · [ISPROT](../ripscrip/5.1-text-variables.md#isprot)

## Scope and checkpoint

[RealDeuce report #207](https://github.com/bbs-land/remote-imaging-protocol/issues/207), created 2026-11-24 00:50:29 UTC, body SHA-256 `b80a80f0266f58b73b365e514e50fe683a4b2f928f0eda1f6bbc347f69749aa0`, zero comments at intake. Started from fork stable `c6651034555a49ba7f1ab704412cbb240b6ab256` after the 02:01:14Z audit. The native callbacks confirm generation of literal Boolean digits, numbered lookup through S9, and aggregate scans ending before S8. Next reconcile the intended ten-slot profile, BACKUP and live protection predicates, then validate fixture vectors.

## Resume

Publish the aggregate contract and update the backup-page cross-reference.
