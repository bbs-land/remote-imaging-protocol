# Issue #22: FastFont fill, hollow width, overlap and byte mapping

[Research backlog](issue-backlog.md) · [FastFont format](../../2.0/techspecs/3.2-fastfont-fonts.md) · [RIPtel additions](../techspecs/3.2-fastfont-additions.md)

## Scope and status

Started 2026-11-16 after [PR #52](https://github.com/bbs-land/remote-imaging-protocol/pull/52) delivered #21. Branch **issue-22-fastfont-rendering** starts at **eb7ae2e**. [Issue #22](https://github.com/bbs-land/remote-imaging-protocol/issues/22) asks for the original solid contour fill rule, hollow stroke width and horizontal-style scaling, per-glyph versus whole-text non-COPY overlap, and byte 0x80–0xff mapping into the 224 raw font slots, including SYMBOL. Hinting and pixel-exact rasterization are outside scope. All four answers remain to establish; no resolution PR yet.

Use original RIPtel 3.1 driver 3.0.7 `extracted/ripscrip.dll`, 592896 bytes, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, base **0x10000000**, from the [catalogued MSI](../../../reference/rip-tools.md), [source](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi) SHA-256 **04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c**. Font bytes come from the persistent extracted mirror and must match checkout LFS OIDs. All instruction addresses below are DLL RVAs; .text file offset = RVA−0xc00, .data = RVA−0x1a00.

## Existing original-font evidence and initial leads

The [issue #3 derivation](issue-3-extended-font-style.md) already establishes original placement renderer **0x40ed0**, normal text adapter **0x42870**, font initializer **0x3ff90**, face lookup **0x3fb50**, realization **0x40170**, bitmap glyph engine **0x523f0** and advance query **0x4efd0**. Its native geometry fixture controls the font engine/bitmap output, so it cannot prove the font rasterizer's fill or hollow policy. Its separate isolated Wine oracle used the real Marin bytes and unmodified font engine for orientation only. Do not silently promote either into evidence for the remaining questions.

First static leads: renderer **0x41485 / 0x41580 / 0x41e98** reads input bytes with zero extension and calls glyph engine per character. Glyph presentation **0x40660** calls SetBitmapBits then BitBlt with ROP **0xb8074a**. The renderer's later loop/bitmap clearing and complete write-mode producer still require tracing before concluding coverage aggregation or XOR behavior. Cache lookup **0x55a30** subtracts the realized font's first character from the supplied code and indexes the cache; trace the uncached record consumer too and verify there is no earlier translation.

Next trace full original solid/hollow scan conversion and byte consumers, then construct distinguishing same-direction nested/overlap contours and capture width/byte cases. Prefer native/static work. If actual font-engine execution needs Wine, use the existing private-display/private-prefix wrapper pattern with DISPLAY explicit, logs outside the desktop and scoped cleanup; never run on the user's desktop. Remove task-owned temporary builds immediately after use.

---

[Research backlog](issue-backlog.md) · [RIPtel additions](../techspecs/3.2-fastfont-additions.md)
