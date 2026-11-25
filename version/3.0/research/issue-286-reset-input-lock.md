# Issue #286 — RESET input gates and pointer state

[Issue #286](https://github.com/bbs-land/remote-imaging-protocol/issues/286) · [RESET](../ripscrip/5.1-text-variables.md#reset) · [HEADER scene gates](../ripscrip/1.1-protocol-and-command-hierarchy.md#scene-input-gates) · [Research backlog](issue-backlog.md)

RealDeuce filed #286 on 2026-11-25 at 06:08:00 UTC. The 06:18 UTC poll found it with no comments. Branch `issue-286-reset-input-lock` starts at fork stable `709a4397011d4593502c3a487769ceaa75ee338b`.

[ALPHA 4 §5.9.1](../../2.0/text/RIPScrip-2.0-alpha-4.txt) lines 16682–16691 provide distinct actions. RESET(MCURSOR) selects arrow and releases blocked mouse input. RESET(KEYBOARD) independently re-enables keyboard input. The verbatim source SHA-256 is `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. The portable profile tracks separate gates to implement both actions while preserving the HEADER and NO_MORE sequence. A release grants eligibility under any other existing gates. The scene pointer state is independent of delivery.

## Original callbacks and captured state

The [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The report used the ordinary macro dispatcher and controlled cursor presentation imports. This note compares its captures with the original instructions. Addresses are RVAs at image base `0x10000000`.

RESET at `0x43d38` reaches MCURSOR `0x42aa2` or KEYBOARD `0x42ad3`. Both require argument count one, including the family keyword. MCURSOR's calls at `0x42ab5` and `0x42abe` reach the ENV pointer setter `0x3468a` with 0 and the secondary-word setter `0x16b97` with the context. The pointer setter stores into the selected ENV record at +8 and calls the pointer presenter. The secondary setter writes only 1 at ENV system +0x1e. KEYBOARD's successful branch at `0x42ae0` goes directly to return. Extra arguments set the variable-error word at context+0x26 → +0x436.

[HEADER application](issue-5-scene-headers.md#header-framing-and-primary-parser-evidence) first calls release `0x16b51`, then flag 4 calls `0x16b24`. The latter selects pointer 6 and stores lock 1 at +0x1c and secondary 0 at +0x1e. NO_MORE handler `0x1cf6a` calls `0x16b51` at `0x1cf79` and again at `0x1cf8b` around its scene-end step. The release helper checks +0x1c. With an active lock and +0x1e zero, it first selects pointer 0. With an active lock and +0x1e nonzero, it preserves the selected pointer. Both locked paths clear both words at `0x16b81–0x16b87`. With no lock it returns without applying the pointer change.

Each report case starts locked with pointer 6 and secondary 0.

| Action | Pointer | Lock +0x1c | Secondary +0x1e |
| --- | --- | --- | --- |
| RESET(MCURSOR) | 0 | 1 | 1 |
| RESET(KEYBOARD) | 6 | 1 | 0 |
| RESET(MCURSOR), then NO_MORE release helper | 0 | 0 | 0 |
| RESET(KEYBOARD), then NO_MORE release helper | 0 | 0 | 0 |

The report also found that both arity-valid resets left the software variable-error flag clear and that each left the lock word at zero when it began unlocked. The helper capture measures pointer/lock state, with controlled UI calls.

The secondary word suppresses the locked scene's automatic arrow on full release. The bare MCURSOR variable and accepted numbered MCURSOR call `0x16b97` at `0x34778` or `0x347bd`, distinct from RESET(MCURSOR). Shared SOFT and HARD pointer resets call it at `0x1651b` and `0x1696e`. ENV activation calls it at `0x3db76` when the target and departing pointer numbers differ. The wire cursor handler at `0xdc96` calls only the shape setter after its protection check. These paths motivate a separate pending-return state rather than deciding from the visible shape alone. [SOFT pointer and lock evidence](issue-105-outer-soft-reset.md#input-audio-ownership-and-unaffected-state) · [Cursor state ownership](issue-24-drawing-state-ownership.md#ownership-and-switching-observations).

## Portable state examples

After a HEADER with flag 4, the starting portable state is mouse gated, keyboard gated, pointer 6, and return to arrow pending.

| Operation | Mouse gate | Keyboard gate | Pointer | Return to arrow pending |
| --- | --- | --- | --- | --- |
| RESET(MCURSOR) | Clear | Set | 0 | No |
| RESET(KEYBOARD) | Set | Clear | 6 | Yes |
| Both resets in either order | Clear | Clear | 0 | No |
| RESET(KEYBOARD), then NO_MORE | Clear | Clear | 0 | No |
| RESET(MCURSOR), then select pointer 4 with the MCURSOR variable, then NO_MORE | Clear | Clear | 4 | No |
| RESET(MCURSOR,1) or RESET(KEYBOARD,CUR) | Set | Set | 6 | Yes |

A full release clears any remaining gate. A new successful HEADER performs that release before an optional relock. SOFT and shared HARD retain existing scene gates even when they request arrow. Neither explicit input reset replays blocked input or restores focus/capture discarded when the gates were set. A local pointer disable or a per-window ANSI keyboard lock can still block its device.

```sh
sha256sum "$dll" version/2.0/text/RIPScrip-2.0-alpha-4.txt
objdump -d --start-address=0x10042aa2 --stop-address=0x10042aec "$dll"
objdump -d --start-address=0x10016b24 --stop-address=0x10016ba5 "$dll"
objdump -d --start-address=0x1001cf6a --stop-address=0x1001cf95 "$dll"
objdump -d --start-address=0x1003468a --stop-address=0x100346d1 "$dll"
```
