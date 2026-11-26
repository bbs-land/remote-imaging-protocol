# Issue #340 — Minimal scene's classic font selection

[Issue #340](https://github.com/bbs-land/remote-imaging-protocol/issues/340) · [Minimal scene](../ripscrip/1.1-protocol-and-command-hierarchy.md#a-minimal-constructed-scene) · [Research backlog](issue-backlog.md)

RealDeuce filed #340 on 2026-11-26 at 05:26:54 UTC. The 05:27 UTC audit found it with no comments. Branch `issue-340-minimal-scene-font-annotation` starts at fork stable `5f28a11a3a38f832dd2a38a7c07ce7dbe5b6ae6d`.

Original dispatch descriptor 74 and handler `0x1c87e` give `Y` four two-digit fields. After the minimal scene selects Base Math 36, `Y02000909` splits into `02` (Small, LITT.CHR), `00` (horizontal), `09` (size 9), and `09` (flags 9). The classic ratios in ALPHA 4 and the original loader's scale tables give `3/1` at index 9. The alignment consumer at `0x39808` interprets flag 1 as left and flag 8 as top. Thus the last field selects both. [Original flag derivation](issue-186-font-style-flags.md#validation-and-style-commit) · [Original size-table trace](issue-323-classic-button-stroke-metrics.md#stroke-measurement-and-positioning) · [Classic font and size table](../ripscrip/3.0-text-output-and-fonts.md#direction-and-size).

The same minimal scene and annotation appear in chapters 1.1 and 6.0. The corrected annotation gives the face, direction, ratio and alignment in both places and keeps the existing wire example. [Authoring guide example](../ripscrip/6.0-writing-rip-files.md#a-minimal-constructed-scene).
