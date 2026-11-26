# Issue #299 — COMPAT ENV selectors and protection

[Issue #299](https://github.com/bbs-land/remote-imaging-protocol/issues/299) · [COMPAT](../ripscrip/5.1-text-variables.md#compat) · [Research backlog](issue-backlog.md)

RealDeuce filed #299 on 2026-11-25 at 16:35:20 UTC. The 16:37 UTC check found it with no comments. Branch `issue-299-compat-env-selectors` starts at fork stable `f3e4df69554c524e4a0c2bd5b91c1b2e66cf492a`.

ALPHA 4 §5.10.5 describes the omitted selector, CUR and a number from 0 through 35. Its unused-destination error has a more precise selector boundary in the shipping 3.0 driver. The playback-rate difference between ALPHA 4 and RIPtel already has its own source trace. [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) · [Playback rate derivation](issue-172-local-playback-baud.md).

## Original selection and update path

Static inspection used the PC Micro RIPtel 3.1 package's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md).

- COMPAT callback `0x30be8` branches on parsed argument count at `0x30c04`. Zero arguments take `0x30ce2`. One argument can take the same branch after matching CUR, using the string at VA `0x1007a104`. The branch reads the current ENV index at table state +0x0a and stores format fields with no definition or protection gate.
- The numbered path calls `0x0987b` at `0x30c41` to recognize a decimal selector below 36. It calls `0x3d6f9` at `0x30c56` before any format store. That helper accepts slot 0 at `0x3d776–0x3d788` and tests the definition mask `0x01` of a nonzero record at `0x3d799`. The flags byte is at record +0, with stride `0x10`. The numbered path tests definition before it compares the target with the selected index.
- Extra arguments, invalid numeric selectors and an undefined numbered target branch to `0x30cd4`, which sets the variable syntax flag at parser state +0x436 and skips the update.
- The numbered stores at `0x30c63–0x30cbb` and selected stores at `0x30ce2–0x30d3e` write words 640 and 350 at record +1 and +3, then bytes 36, 2 and 0 at +5, +6 and +7. Neither path changes the flags or other record fields. The numbered path compares destination with the selected index at `0x30cca`; the noncurrent path jumps directly to the common return.
- Both forms targeting the selected ENV call the RGB precision, coordinate width, Base Math and WORLD apply helpers at `0x30d42`, `0x30d4d`, `0x30d58` and `0x30d63`. These are active format operations within the selected slot. At `0x30d74` the callback returns zero, giving an empty expansion on success. There is no playback-rate setter in this path.

The [live ENV reset](issue-283-style-env-live-reset.md) establishes how a selected index can point to an undefined nonzero record. In that state COMPAT CUR still applies the format without marking it defined, and the numbered request for the same slot takes the syntax-error route. The portable reset contract already installs the same format defaults, so both the success and failure paths may leave subsequent parser values the same in this example. The selector type and syntax result remain observable; the definition query stays false throughout. Protection permits either form of COMPAT on a defined target. The noncurrent protected example tests stored-versus-active effects without requiring an ENV switch.

```sh
sha256sum "$dll"
objdump -d --start-address=0x10030be8 --stop-address=0x10030d7b "$dll"
objdump -d --start-address=0x1000987b --stop-address=0x100098bb "$dll"
objdump -d --start-address=0x1003d6f9 --stop-address=0x1003d7d1 "$dll"
```
