# Issue #211 — Portable MSTAT pointer presence

[GitHub issue #211](https://github.com/bbs-land/remote-imaging-protocol/issues/211) · [MSTAT entry](../ripscrip/5.1-text-variables.md#mstat)

## Scope and checkpoint

RealDeuce filed the issue at 2026-11-24 01:25:44 UTC. No comments were present at intake. The original RIPtel 3.1 DLL SHA-256 is `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`.

## Source and portable choice

DLL callback RVA `0x34970` tests effective argument count and writes the variable error word at `0x34980` for any nonzero count. It then calls the IAT at `0x966e8` with the immediate decimal 19. Import analysis resolves USER32 GetSystemMetrics. Any nonzero result selects the bytes `YES` at `0x7f248`, while zero selects `NO` at `0x7f244`. The provider itself reads neither a local cursor or field record nor a press flag. The generic expander handles its invalid-argument flag even though the callback performs the API call. [Microsoft GetSystemMetrics documentation](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics)

The portable rule follows explicit current frontend presence when available, including a virtual mouse or compositor seat pointer. On event-only frontends it uses enabled mouse-equivalent input capability as the fallback. This gives a host the same answer before and after the first event and lets scripts choose mouse-aware UI on frontends without a presence metric. A frontend without enabled mouse-equivalent input returns NO. A known absence takes precedence over generic event capability. The rule concerns current disposition of mouse input for the client, independently of cursor visibility, focus, position, held presses and registered fields. Tablet/touch/remote input qualifies when mapped to mouse-equivalent movement and button actions. The DLL result and the frontend policy are distinct sources of evidence.

The read-only verification checked the call bytes, the metric operand, the import, both result literals, and the error-flag write. No input device or original terminal was launched.

## Resume

Publish the portable policy with examples and distinguish the local-playback held bit in both directions.
