# Issue #209 — Backspace mode and transmit policy

[GitHub issue #209](https://github.com/bbs-land/remote-imaging-protocol/issues/209) · [3.0 text variables](../ripscrip/5.1-text-variables.md)

## Scope and checkpoint

RealDeuce filed this at 2026-11-24 01:19:52 UTC. There were no comments at intake. The shipping RIPtel 3.1 DLL SHA-256 is `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`; the unpacked EXE SHA-256 is `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. Static reading isolates received text, local echo, field editing, and generated output. No original program is launched.

Callbacks at DLL RVAs `0x30881` and `0x30944` route to `0x31eed` and `0x31f68`. The DLL text parser handles ASCII BS at `0x4d4db`, and the key handler queries transmit mode at `0x5188`. EXE configuration strings start at RVAs `0x48d54` and `0x48d64`.

## Parameter and literal verification

The callbacks use the shared case-insensitive comparator at DLL RVA `0x655e2`. Each accepts zero arguments or one selector. BSMODE uses `0x308d3` for DEST and `0x308f7` for NONDEST. Its query at `0x30928` chooses `0x7970c` for true, `0x79714` for false. BSXMIT uses `0x30996` for BS and `0x309ba` for DEL. Its query at `0x309eb` chooses `0x79714` for true, `0x7970c` for false. Those strings are ASCII `1` and `0`, respectively. Therefore the issue's preliminary query polarity for BSXMIT needs correction. The internal BSXMIT flag itself is true for DEL.

## Ownership and routes

EXE RVAs `0x9034–0x9051` bind callback IDs 24 and 23 to `0x8bd6` and `0x8c05`. They use process state at VAs `0x44fb84` and `0x44f8c0`. Set and query selectors through the DLL exports mirror the hook return at settings offsets `0x312` and `0x311`. Without hooks the callback allocation is zeroed, and DLL `0x3132a` explicitly initializes destructive mode to true. EXE INI startup `0x409473` calls `0xda77`; its reads at `0xdb30/0xdb44` default to 1 and 0 under `[setup]`. The separate writer reaches those keys at `0xdd0c/0xdd1f`. EXE `0x17a35` applies dial-entry settings through `0x16d77`; profile bits 1 and 2 at offset `0xf3` overwrite live display and transmit state. A reference to either client-global setting appears only in the callback, startup/settings UI and profile sites. A DLL reset or a connection change without profile application leaves the hooked values in place. The same callbacks serve playback in the live instance.

## ANSI screen and keyboard

DLL `0x4d4db` handles BS in the ordinary ANSI display path. `0x9b5b` reads the zero-based column, `0x9f29` decrements it, and both screen branches exit at column zero. The destructive branch passes character `0x20` to cell painter `0xde30`, which reads the selected text window attribute word (foreground bits 0–3, background bits 4–7 and face bits 8–10). The painting path is separate from the scrollback helper. Comparisons and vectors here describe the visible ANSI cell and cursor.

At DLL `0x5116` hotkey actions can consume a character before the ordinary BS branch at `0x5123`. The latter calls the transmit-state API at `0x5188`. The half-duplex branches echo `08` at `0x51b1` and `0x51f4` before translating the transmitted byte. `GetKeyState(VK_CONTROL)` and the arithmetic at `0x51c2–0x51da` and `0x5205–0x521d` set the outgoing byte. When the flag is clear, the full-duplex jump at `0x51e7` skips the Ctrl test and retains `08`. A static check verified 10 selectors and reply literals, nine call targets, both defaults and 48 modeled erasure/key/duplex combinations. Both flow through DLL `0x5229` to `0x6947`, which posts the byte string through local message `0x4c8`. The EXE's handler at `0x844e` reaches the send function only with an open socket. The keyboard setting's only DLL query consumer outside its own built-in is this ordinary event. A generated output string taking the send route has already selected its bytes. Field editing in a separate edit control is outside the ANSI renderer and ordinary key route.

## Verification

Read both shipping PE files without running the client. Verified callback selectors, literal addresses and string values, key choices, current text attributes, setup defaults and dial profile application. Checked Markdown, links and diff whitespace.

## Resume

Trace the configured defaults, frontend hook bindings, the DLL cell writer and keyboard branch, and the boundary for reset, reconnect and playback. Then update the reference and command index and deliver the individual PR.

## Delivery

[PR #220](https://github.com/bbs-land/remote-imaging-protocol/pull/220) is open for upstream review. Specification commit b00d38fda2a8d0701bf5ce8203a9fb3836a26c41.

The actual PR head b85d5d8694e31bd72241e9e31de8114cdedb316a was normal-merged with an identical tree in 5dba85225c1ed61d33296f4d848cd6b106be22d2. At 02:12:13Z, all 105 open PR heads were ancestors and no new comments or reviews were present. Continue in order with #210, #211, #212, #213, #214, #215.
