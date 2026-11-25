# Issue #279 — RIP_FILL descriptor and framing

[Issue #279](https://github.com/bbs-land/remote-imaging-protocol/issues/279) · [RIP_FILL](../ripscrip/2.3-shapes-and-fills.md#rip_fill) · [Legacy syntax](../../1.54/ripscrip/2.3-shapes-and-fills.md#rip_fill) · [Research backlog](issue-backlog.md)

RealDeuce filed #279 at 23:51:21 UTC on 2026-11-24. The 23:59 UTC in-thread check found it with no comments. Branch `issue-279-rip-fill-framing` starts at fork stable `9471a1be123bdc4b288c95d5cce75239b9a92461`. [Issue #2](https://github.com/bbs-land/remote-imaging-protocol/issues/2) provided the earlier inactive-handler lead.

The [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses below are RVAs at image base `0x10000000`. Physical slot 27 at RVA `0x80c58` is level 0 `F`, count zero, flags zero. Its handler at `0x1b2fd` is one `C3` byte, RET. The PE import inventory contains neither ExtFloodFill nor FloodFill.

RIPlib main was refreshed at `024032a`. Its [later driver crosswalk](https://github.com/BradHawthorne/riplib/blob/024032a263649cbd842bcf855ac1fe9238606e6f/docs/spec/14-divergence-register.md#L591-L605) identifies the same inactive slot. Its earlier `§DEAD.7` notes a different live-path claim. The fingerprinted artifact resolves the PC Micro 3.1 attribution. The recovered behavior leaves the legacy 1.54 syntax and RIPlib's own rendering extension available in their respective profiles.

## Collector and delimiter

Opcode resolution enters state 8 at `0x3a699–0x3a6a5` because the record has zero fields and the text bit is clear. In state 8, `0x3a469–0x3a46c` sends a pipe straight to dispatch state 9. The line-ending comparisons at `0x3a46e–0x3a495` allow ordinary continuation checks and otherwise dispatch on CR, LF or form feed. Other bytes take the return path and never enter numeric or text collection. The outer buffer routine at `0x5820–0x5846` passes the incoming byte directly to the RIP parser, so the surplus ESC stays inside RIP framing.

A backslash before a pipe in state 8 is surplus, and the pipe takes the dispatch branch even there. The report's combined-feed case produces the same F and COLOR records with that first pipe and a later chain separator. Its combined capture does not identify which pipe dispatched F. The state-8 branch establishes the boundary. The text field case in the [existing escape probe](issue-7-framing-identification.md#distinguishing-streams-and-stages) uses a separate quote path.

## Report captures

| Input | Dispatch and text route |
| --- | --- |
| `!\|F\|c01` or `!\|F1E180F\|c01`, then CRLF | F with zero arguments and empty tail, then COLOR(1). No raw text. |
| `!\|Ffree form?`, ESC, `[!\|c01`, then LF | Same two dispatches. No raw text or ANSI reply. |
| `!\|F.`, backslash, pipe, `0F\|c01`, then CR | Same final dispatch list. |
| `!\|F`, then CRLF, LF or FF, then `OK` | F dispatches at the terminator. `OK` goes to raw text. |
| `!\|F` alone | No dispatch. |

The bare command followed by `c01` also produced the same result in Base Math 64 at XY width 5. The report used a capture callback for the arguments and separately verified the original handler byte.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 27
run/riptel-inspect "$dll" --rva 0x1b2fd --length 1
objdump -d --start-address=0x1003a469 --stop-address=0x1003a4a6 "$dll"
objdump -p "$dll"
```
