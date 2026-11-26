# Issue #302 — ANSI mode 12 and local echo

[Issue #302](https://github.com/bbs-land/remote-imaging-protocol/issues/302) · [ANSI mode 12](../ripscrip/1.4-terminal-ansi-view.md#ansi-mode-12-and-local-echo) · [Research backlog](issue-backlog.md)

RealDeuce filed #302 on 2026-11-25 at 16:41:29 UTC. The 16:45 UTC audit found it with no comments. Branch `issue-302-ansi-local-echo` starts at fork stable `ea715d15f7c55ee5c3701b8e4df4b84f2c6477a7`.

ECMA-48 fifth edition §7.2.15 identifies mode 12 as Send/Receive Mode. Monitor mode images local input. Simultaneous mode disconnects local input from imaging. Telnet ECHO concerns echo sent back across the transport and defaults to off in both directions. RFC 857 leaves local display of one's own input to the local system. [ECMA-48 fifth edition](https://www.ecma-international.org/wp-content/uploads/ECMA-48_5th_edition_june_1991.pdf) · [RFC 857](https://www.rfc-editor.org/rfc/rfc857.html).

## Original mode setter and input paths

Static inspection used the PC Micro RIPtel 3.1 package. The `extracted/ripscrip.dll` SHA-256 is `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The previously unpacked `riptel-unpacked.exe` SHA-256 is `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. Driver addresses are RVAs at base `0x10000000`. Standalone addresses are VAs at base `0x400000`. [Artifact catalogue](../../../reference/rip-tools.md).

- The ordinary, single-parameter mode callbacks `0x36cba` and `0x36d33` compare the mode with 12. The set branch passes 0 at `0x36d0b–0x36d0e` and the reset branch passes 1 at `0x36d84–0x36d87` to `ripHalfDuplex` at `0x31d41`. The helper calls the client hook at callback table +0x4c when installed and mirrors the result at settings +0x30e. Without a hook it uses that byte directly. A query passes -1. There is no TW record access in this mode branch.
- Standalone registration at `0x409073–0x40907b` binds slot 19 to `0x408b6e`. That callback stores an explicit setter argument in the requested value at `0x448b94` and live echo value at `0x448b90`. Queries read the live value. The direct driver references to the duplex helper outside the setters are keyboard and echo queries. The source of the standalone live value also includes the connection/profile paths at `0x416e1a`, `0x41d67d` and `0x41decc`.
- Telnet option handling at `0x41e884–0x41e917` can consult the requested value and then update the live value separately. The observed DONT branch sets the live value to 1 at `0x41e8ad`. Two DO branches set it to 0 at `0x41e8db` and `0x41e917`. The portable precedence below specifies effective peer transitions and ordinary receive ordering without requiring that standalone policy for accepting offers.
- For an ordinary character, the key handler queries mode at `0x5134`, skips local echo for DEL at `0x5141`, calls `RIP_EchoCharacters` at `0x515b` if eligible, and submits the send at `0x5178`. The echo helper at `0x7705` accepts `07–0D` or the signed-byte range `20–7F` and calls the ANSI renderer at `0x4d2d0`. High-bit bytes fail the signed comparison at `0x777f–0x7781`. Its direct renderer call bypasses the RIP scene receive collector.
- Enter queries duplex at `0x5236` and echoes CR and LF through the renderer at `0x524e` and `0x5259` before sending CR. The separate Backspace branches echo `08` before choosing the transmitted byte. The special-key translation paths beginning at `0x45fa` go to the send helper `0x6947` without a local echo call. For example, the branch at `0x466e–0x4688` copies the chosen translated sequence and submits it directly. [Backspace derivation](issue-209-backspace-modes.md) · [Mode 2 gates](issue-289-ansi-keyboard-action-mode.md).
- Connected clipboard text in the standalone path calls the echo import at `0x406714` and sends chunks at `0x40675c`. The import thunk at `0x41c03c` uses IAT `0x45b80c` for `RIP_EchoCharacters`. Two other standalone callers at `0x407e2f` and `0x407ef6` also request an explicit echo separately from their send. These call sites identify a batch/explicit echo operation and do not imply echo for all send buffers.

## Portable connection and input rules

Use a single live local echo setting per connection. This locates updates at the connection/Telnet/ANSI boundary and leaves TW records and snapshots independent of the setting. Reconnect initializes a new logical value. The raw default is full duplex. Telnet initially enables local echo in the absence of peer ECHO. A connection profile may initialize the value. A change in negotiated peer ECHO updates the setting, and a subsequent valid ANSI mode control can update it again. A duplicate or refused exchange is no effective transition. The choice to follow the latest effective event preserves both update paths without making the vintage ECHO offer-acceptance policy a protocol requirement.

Keep existing keyboard gates, then distinguish one ordinary character from Enter, Backspace and a translated key sequence. The ordinary local echo filter is ASCII `20–7E` plus `07–0D`. The same graphic/control admission rule applies to a portable text paste. This excludes DEL from portable local echo even when present in a batch, while the native shared helper's signed comparison alone admitted it. Paste uses the input gates and batch echo. Explicit generated output and local playback use their own paths. The reference vectors check connection-wide state across TW selection and distinct Enter display/send streams.

```sh
sha256sum "$dll" "$exe"
objdump -d --start-address=0x10036cba --stop-address=0x10036dac "$dll"
objdump -d --start-address=0x10031d41 --stop-address=0x10031dbc "$dll"
objdump -d --start-address=0x10007705 --stop-address=0x100077c0 "$dll"
objdump -d --start-address=0x10005116 --stop-address=0x10005283 "$dll"
objdump -d --start-address=0x408b6e --stop-address=0x408ba7 "$exe"
objdump -d --start-address=0x41e87b --stop-address=0x41e925 "$exe"
```
