# Issue #290 — Live wire RIP_DELAY

[Issue #290](https://github.com/bbs-land/remote-imaging-protocol/issues/290) · [RIP_DELAY](../ripscrip/5.5-file-transfer-and-queries.md#rip_delay) · [Research backlog](issue-backlog.md)

RealDeuce filed #290 on 2026-11-25 at 06:55:28 UTC. The 07:19 UTC in-thread poll found it with no comments. Branch `issue-290-wire-delay` starts at fork stable `8a06968dce2656d41b8a47d05aee3e9b3be89bb8`.

[ALPHA 4 §3.4.4.2](../../2.0/text/RIPScrip-2.0-alpha-4.txt) gives a four-digit current-radix field and a sixtieth-second unit. It directs buffering of the incoming stream and resumption after the pause. Keep those wire rules. The prior inference of supersession came from demo corpus use of the D variable and the help inventory. These sources record usage. The original dispatch table establishes support for the wire entry. [Issue #2](https://github.com/bbs-land/remote-imaging-protocol/issues/2).

## Descriptor and consumer

The [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses are RVAs at image base `0x10000000`.

Slot 122 at `0x81b30` has level key 3, opcode `D`, count 1, width 4 and flags `0x0003`. The low bits `11` use active Base Math in `0x39d70–0x39dd9`. Width 4 is a radix digit count; the descriptor uses neither XY width nor the RGB component precision. The usual last-field and period rules still apply. [Original radix decisions and collector evidence](issue-250-meganum-receive-alphabet.md).

The handler at `0x38bd2` loads the first decoded value and calls `0x282ca` at `0x38bd8`. The macro D callback separately calls `0x282ca` at `0x28380`. For a positive duration n above 3,900, the helper selects `floor((n − 1) / 3900)` full waits. Each polls `timeGetTime` through IAT `0x966fc` to a target 65,000 ms from its start. For any remaining n, `0x28304–0x28327` computes `floor(n × 1000 / 60)` and polls again to the computed deadline. These are requested intervals. The [prior D variable work](issue-46-delay-variable-units.md) verified the shared helper with controlled time. The current issue checked the same target from wire dispatch without executing the driver.

The positional examples in base 36 are `001O` = 36 + 24 = 60 and `002D` = 72 + 13 = 85. In base 64, `001O` carries a different value and `000y` encodes 60 under the recovered alphabet. The portable queue rule governs processing order without requiring the synchronous clock loop.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 122
objdump -d --start-address=0x10038bd2 --stop-address=0x10038be1 "$dll"
objdump -d --start-address=0x100282ca --stop-address=0x1002832d "$dll"
```
