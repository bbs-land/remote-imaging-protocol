# Issue #289 — ANSI mode 2 keyboard gating

[Issue #289](https://github.com/bbs-land/remote-imaging-protocol/issues/289) · [ANSI mode 2](../ripscrip/1.4-terminal-ansi-view.md#ansi-mode-2-keyboard-gating) · [Research backlog](issue-backlog.md)

RealDeuce filed #289 on 2026-11-25 at 06:42:06 UTC. The 07:19 UTC in-thread check found it with no comments. Branch `issue-289-ansi-keyboard-action-mode` starts at fork stable `96c7746d997e912b3bb094498835ffad180c40e5`.

## Standard and profile scope

ECMA-48 fifth edition §7.2.11 calls mode 2 Keyboard Action Mode. Setting it disables all or part of the manual input facilities, and resetting it enables them. The ordinary Reset Mode and Set Mode controls in ECMA-48 accept a list of mode parameters. The portable 3.0 profile uses one parameter for these ordinary controls, following the recovered RIPtel callback shape. [ECMA-48 fifth edition](https://www.ecma-international.org/wp-content/uploads/ECMA-48_5th_edition_june_1991.pdf).

## Original RIPtel instructions

The [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The report provided instruction observations. Verification here used the original instructions and prior derivations. Addresses are RVAs at image base `0x10000000`.

- Ordinary ANSI mode callbacks `0x36cba` and `0x36d33` compare the parameter count at parser +0x14 with 1. An arity mismatch returns before the mode branches. For parameter 2, `0x36ce2` sets mask `0x20` and `0x36d5b` clears it at selected TW record +0x27. The record stride is `0xf1`. The writes concern the TW record, independent of scene input state. [Ordinary TW state derivation](issue-100-ordinary-text-window-state.md#original-control-and-state-derivation).
- The ordinary keyboard matcher reads the current TW index before testing bit `0x20` at `0x558f`. A set bit exits the matcher. The separate HEADER-lock helper is called at `0x5597`. The branch checks the stored bit without an activation test. The earlier Tab and focused-Enter route `0x4fb6–0x5039` is a separate consumer. The native observations establish these two routes, while complete key delivery, half-duplex echo and application routing below are portable policy. [Hotkey derivation](issue-118-button-keyboard-activation.md) · [Tab navigation derivation](issue-197-tab-navigation.md).
- Per-window ownership and retention follow the already published definitions, selection, reset, copy and saved-window rules. ESC 7/8 captures and restores ordinary per-window modes by logical value. CSI s/u handles coordinates only. The [scene input release macros](issue-286-reset-input-lock.md) affect their respective connection gates. [ESC snapshot derivation](issue-150-ansi-esc-snapshot.md).

## Portable delivery boundary

Apply the selected TW's mode and the connection's keyboard scene gate as independent deny conditions before keyboard-backed terminal transmission, local half-duplex echo, RIP hotkeys and Tab/focused activation. The uniform placement closes the distinction between the two recovered key routes. A client-local menu or edit control may handle a key separately, while input delivered as terminal bytes or RIP keyboard actions uses the gate. Host-initiated responses and separately gated mouse input continue through their existing paths.

Use the retained mode of an inactive selected TW for outgoing eligibility. Its receive/display path still discards incoming text and cannot process a newly arrived mode sequence. Reactivate the window through its ordinary lifetime rules before sending mode controls. With the mode clear, inactivity alone leaves terminal transmission and RIP keyboard actions eligible while the display still discards text and echo. Changing mode 2 leaves focus/capture in its existing state. Keyboard-gated events are discarded at delivery.

The two-window example in the reference establishes effective gating after HEADER, RESET(KEYBOARD), selection and NO_MORE. TW 0 with a clear bit becomes eligible when the scene keyboard gate is cleared. TW 5 stays gated until its own CSI reset, including after NO_MORE. These are intended protocol results, with no claim of additional original key-delivery measurements.

```sh
sha256sum "$dll"
objdump -d --start-address=0x10036cba --stop-address=0x10036dac "$dll"
objdump -d --start-address=0x10005580 --stop-address=0x100055a4 "$dll"
objdump -d --start-address=0x10004fb6 --stop-address=0x1000503a "$dll"
```
