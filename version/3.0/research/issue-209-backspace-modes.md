# Issue #209 — Backspace mode and transmit policy

[GitHub issue #209](https://github.com/bbs-land/remote-imaging-protocol/issues/209) · [3.0 text variables](../ripscrip/5.1-text-variables.md)

## Scope and checkpoint

RealDeuce filed this at 2026-11-24 01:19:52 UTC. There were no comments at intake. The shipping RIPtel 3.1 DLL SHA-256 is `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`; the unpacked EXE SHA-256 is `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. Static reading isolates received text, local echo, field editing, and generated output. No original program is launched.

Callbacks at DLL RVAs `0x30881` and `0x30944` route to `0x31eed` and `0x31f68`. The DLL text parser handles ASCII BS at `0x4d4db`, and the key handler queries transmit mode at `0x5188`. EXE configuration strings start at RVAs `0x48d54` and `0x48d64`.

## Resume

Trace the configured defaults, frontend hook bindings, the DLL cell writer and keyboard branch, and the boundary for reset, reconnect and playback. Then update the reference and command index and deliver the individual PR.
