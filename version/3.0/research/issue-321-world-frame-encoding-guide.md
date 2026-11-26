# Issue #321 — World-frame limits and numeric field capacity

[Issue #321](https://github.com/bbs-land/remote-imaging-protocol/issues/321) · [Encoding decision guide](../ripscrip/1.2-math-and-coordinates.md#decision-guide) · [Research backlog](issue-backlog.md)

RealDeuce filed #321 on 2026-11-26 at 04:09:00 UTC. The 04:16 UTC check found it with no comments. Branch `issue-321-world-frame-encoding-guide` starts at fork stable `df95d80b2c58f500acc00c9ca26485fd1ee53f65`.

The RIPscrip 3.0 white paper's discussion of resolution independence advertises 32,767 × 32,767. The #23 research already established the shipped `f` command's accepted range. Chapter 1.2's decision guide and limits table still used the advertised number as a field or drawing-coordinate limit. [Original white paper](../text/RIPScrip-3.x-technical-whitepaper.txt) · [Issue #23 wire matrices](issue-23-environment-query-names.md#native-callback-activation-and-wire-checkpoint) · [PR #56](https://github.com/bbs-land/remote-imaging-protocol/pull/56).

Static inspection of PC Micro RIPtel 3.1's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, confirms the two unsigned `ja` branches at RVAs `0x1f8e6` and `0x1f8ea`. Each compares its decoded DWORD with `0xffff`. Either oversized axis returns before the call to world-state setter `0x3e9d4`. The existing #23 fixture checks both saved dimensions and both active dimensions over the boundary matrix. Review used static instructions and the recorded fixture. [Artifact catalogue](../../../reference/rip-tools.md) · [Existing world-frame fixture](../../../run/lib/riptel-macro-probe.c).

`J1S` uses the fixed MegaNum selector `1S` = 64, then the short command `n3` sets both XY fields to width 3 at the next delimiter. Three MegaNum digits encode through `36³ − 1 = 46,655` and three UltraNum digits through `64³ − 1 = 262,143`. With the UltraNum alphabet, `800` = `8 × 64² = 32,768`, `F&&` = `15 × 64² + 63 × 64 + 63 = 65,535`, `G00` = `16 × 64² = 65,536`, and `001` = 1. Both report lines fit the wire fields. The first updates the world in an unprotected environment with graphics enabled. The second leaves both dimensions at their preceding values. Individual drawing commands define their coordinate interpretation. [Numeric modes](../ripscrip/1.2-math-and-coordinates.md#rip_set_base_math) · [Short coordinate-size command](issue-270-coordinate-and-fixed-field-edges.md#staged-captures).

```sh
sha256sum "$dll"
objdump -d --start-address=0x1001f874 --stop-address=0x1001f904 "$dll"
```
