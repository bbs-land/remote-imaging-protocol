# Issue #38: rounded-rectangle wire radius, geometry and interior

[Research backlog](issue-backlog.md) · [Rounded rectangles](../ripscrip/2.3-shapes-and-fills.md#rip_rounded_rect)

Started after [PR #63](https://github.com/bbs-land/remote-imaging-protocol/pull/63) delivered #33. Branch `issue-38-rounded-rectangle-geometry` starts at **f1e044a**. [Issue #38](https://github.com/bbs-land/remote-imaging-protocol/issues/38) asks for radius units/wire width, 640×350-world aspect example, oversize/clamping/order, normalized/reversed/negative/degenerate bounds, zero radius, border-disabled behavior and a literal interior/corner grid without demanding exact GDI pixels.

Primary source is the original RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`, and extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred base 0x10000000. RVAs below; code file offset is RVA minus 0xc00. Original artifacts stay in the external mirror. Native/static GDI request captures only, no desktop GUI; promptly clean scoped task temporary files.

Initial dispatch evidence: level-zero **U slot64 → 0x1f108** and **u slot65 → 0x1c0fe** both describe five arguments **[XY,XY,XY,XY,XY]**, contradicting the reference's fixed two-digit radius. This is a lead until byte-parser/handler evidence is verified. Existing fill probe can capture the original RoundRect service and current brush/pen/ROP; extend it to compare outline/filled geometry and border states after tracing producers. No #38 answer published yet.
