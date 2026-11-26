# Issue #317 — Coordinate queries before the first mouse sample

[Issue #317](https://github.com/bbs-land/remote-imaging-protocol/issues/317) · [Mouse coordinate capture](../ripscrip/5.1-text-variables.md#mouse-coordinate-capture) · [Research backlog](issue-backlog.md)

RealDeuce filed #317 on 2026-11-25 at 23:56:29 UTC. The 00:06 UTC check on 2026-11-26 found it with no comments. Branch `issue-317-initial-mouse-query-point` starts at fork stable `5944d175b6cbefc7e97dc48a1e11eba2d704d2a6`.

ALPHA 4 §5.5.4–5.5.7 describes the single coordinates, pair, button digits and WORLD, DEVICE and text-window interpretations. It describes position values without specifying an unavailable-position response. Prior macro research established the actual signed decimal formats and the result for an ordinary supplied DEVICE point. [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) · [Known position vectors](issue-6-macro-evaluation.md#xy-and-textxy) · [MSTAT derivation](issue-211-mstat-presence.md).

## Original capture and consumers

Static inspection used PC Micro RIPtel 3.1's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Driver addresses are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md).

- The ordinary scanner at `0x4b0e4` calls `0x2814d` at `0x4b12d` before scanning response bytes. For an immediate query the ordinary path is shared with other HCL responses and captures on entry to each pass. The capture stores the button flags at macro state +0x43c and two DEVICE coordinates at +0x440/+0x444 at `0x4b13c–0x4b14e`. [Ordinary mode-0 route](issue-170-local-scene-boundaries.md).
- The capture helper calls GetCursorPos at `0x2815a` through IAT `0x966d8` and ScreenToClient at `0x28167` through IAT `0x966d4`. This lets the standalone frontend obtain a client-relative point before receiving a pointer event. The following instructions combine three held-button bits from separate mouse state. The capture path has no MSTAT eligibility gate. [GetCursorPos](https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-getcursorpos) · [ScreenToClient](https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-screentoclient).
- X reads +0x440 at `0x34b73`; Y reads +0x444 at `0x34eba`. XY reads both at `0x34c45` and `0x34c53`. XYM reads both at `0x34d3c` and `0x34d4a` and reads the flags at `0x34e2e` for a DEVICE reply. The formats at `0x802c4`, `0x802d4` and `0x80314` are `%04hd`, `%04hd:%04hd` and `%04hd:%04hd:%c%c%c`. The existing WORLD and text-cell cases apply their own coordinate conversion and format selection.

## Portable result for an absent position

The original frontend's synchronous capture supplies the ordinary position without requiring a first move. For a frontend that has neither a current point to sample nor a retained event point, use DEVICE `(0,0)` as a portable capture value. It lets numeric queries finish without prompting or synthesizing movement. Subsequent valid points replace that fallback for the next capture. Held buttons and presence have their own sources.

In the proposed 640 × 480 and WORLD 1280 × 960 fixture, all four expressions use the same starting DEVICE point, and XYM appends `000` in the specified unheld state. After capture of `(297,321)`, bare X and XY double their DEVICE components, and explicit DEVICE for Y and XYM keeps the original coordinates. The two published responses keep the literal slashes and add no terminator.

```sh
sha256sum "$dll"
objdump -p "$dll"
objdump -d --start-address=0x1002814d --stop-address=0x1002818f "$dll"
objdump -d --start-address=0x1004b0e4 --stop-address=0x1004b17d "$dll"
objdump -d --start-address=0x10034b62 --stop-address=0x10034f83 "$dll"
```
