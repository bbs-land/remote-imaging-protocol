# Issue #227 — Simple mouse field post-action reset

[Issue #227](https://github.com/bbs-land/remote-imaging-protocol/issues/227) · [RIP_MOUSE](../ripscrip/4.2-mouse-fields-and-pointer.md#rip_mouse) · [Research backlog](issue-backlog.md)

## Sources and scope

RealDeuce filed the report on 2026-11-24 at 13:48:12 UTC. At intake it had no comments. Branch `issue-227-mouse-post-action-reset` starts at fork stable `abae22a1c393d0414f1b1d7d3d8b7d7b0e5d85a1`. The paginated intake found 113 open RealDeuce issues and 111 open PRs. Only #227 lacked a closing declaration, and fork stable contained all 111 PR heads.

The [1.54 specification](../../1.54/text/RIPScrip-1.54.txt) at lines 1792–1793 and 1826–1829 defines the single digit and describes full-screen text and clearing for value 1. The [2.0 ALPHA 4 specification](../../2.0/text/RIPScrip-2.0-alpha-4.txt) at lines 9775–9787 and 9808–9811 retains that meaning and specifies current Base Math. Neither passage gives the host result gate or defines what happens if a callback outlives its source field. The [existing gesture contract](issue-113-mouse-gesture-identity.md) supplies press ownership and release commitment. The [SOFT derivation](issue-105-outer-soft-reset.md) supplies the reset scope.

The original [PC Micro distribution](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. LLVM objdump 19.1.7 read the original PE statically. All instruction addresses below are RVAs at image base `0x10000000`. For these instructions the file offset equals RVA minus `0xc00`.

## Instruction evidence

The constructor at `0x0cef8` reads decoded argument 6 at `0x0cf21` and saves it at `0x0cf31`. At `0x0d1c1–0x0d1c9` zero skips the OR of bit 1 (mask `0x02`) into the field flags at record offset `0x20`. Every nonzero value reaching this test requests the same behavior. The separate limit of 255 at `0x0d04b` tests the next argument. The wire width remains one current-base digit.

On release, the simple-field hit test at `0x2c8e3` gates the call to `0x2c297` at `0x2c8f6`. In its simple-field branch the dispatcher copies host source, installs the active field context at `0x2c304`, runs the host program at `0x2c313` and removes the context at `0x2c318`. The low-word comparison at `0x2c321` skips the post-action helper for `0xffff`. Other results reach `0x2c329`. The helper's simple-field arm tests mask `0x02` at `0x2c3df`, then calls `0x16220(context,0,1,1)` at `0x2c3ee`. Bare `*` calls the same routine with the same arguments at `0x1f31b`. This identifies the outer drawing reset, including its field deletion and TW 0 initialization.

The helper reads flags through the field record passed back after host return. That establishes ordering for a surviving record. For mutation and suspension, the portable completion must specify who owns the pending flag and which outcome consumes it.
