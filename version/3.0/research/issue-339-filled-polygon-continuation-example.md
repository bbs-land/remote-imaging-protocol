# Issue #339 — Continued filled-polygon example

[Issue #339](https://github.com/bbs-land/remote-imaging-protocol/issues/339) · [Framing example](../ripscrip/1.1-protocol-and-command-hierarchy.md#chaining-escaping-and-continuation) · [Research backlog](issue-backlog.md)

RealDeuce filed #339 on 2026-11-26 at 05:24:57 UTC. The 05:26 UTC check found it with no comments. Branch `issue-339-filled-polygon-continuation-example` starts at fork stable `6458ebcb7dcfc2a9acfe8563cae1c776f94f9d3c`.

The example needs two independent checks. The trailing unpaired backslash keeps the current framing state across CR or CRLF. A final unescaped terminator dispatches the accumulated command. Lowercase `p` then needs a two-digit count and exactly that many XY-width coordinate pairs. The previous `04` example had 26 coordinate digits at XY width 2, where four pairs require 16. [Original framing captures](issue-7-framing-identification.md#distinguishing-streams-and-stages) · [Original polygon widths](issue-13-polygon-counts.md#exact-counts-and-single-contour-closure).

The example uses the reported stream. The joined bytes are `!|p040A0A2S0A2S0M0A0M`. At Base Math 36 and width 2, `04` declares four vertices and the 16 coordinate digits split into `0A0A`, `2S0A`, `2S0M` and `0A0M`. These give (10,10), (100,10), (100,22) and (10,22) in traversal order, enclosing a nonzero area. The continuation is between the last X and Y fields. It carries no inline annotation, so its backslash immediately precedes the physical line ending. No separate closing vertex is needed. [Polygon contract](../ripscrip/2.3-shapes-and-fills.md#polygon-counts-and-wire-limits).
