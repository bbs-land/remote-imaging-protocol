# Issue #58: LOAD_BITMAP flag16 and unknown bits

[Research backlog](issue-backlog.md) · [RIP_LOAD_BITMAP](../ripscrip/4.0-icons-and-bitmaps.md#rip_load_bitmap)

Started after [PR #69](https://github.com/bbs-land/remote-imaging-protocol/pull/69) delivered #54. Branch `issue-58-load-bitmap-flags` starts at **f87d465**. [Issue #58](https://github.com/bbs-land/remote-imaging-protocol/issues/58) asks the meaning of flag16 in canonical `!|1b0000HS0Y000G000000STRIP6.BMP`, including scaling, palette adoption, dithering, transparency, duplicate ports and unknown flag handling. This is the final unaddressed issue in the refreshed RealDeuce queue; refresh again before completing the overall goal.

Primary source is original RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`, and extracted `ripscrip.dll`, 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base0x10000000. Addresses are RVAs. Reuse native bitmap probes with controlled services; no GUI/Wine/audio or Windows raster-parity claim. Remove scoped temporary builds/request payloads promptly.

Next: verify the physical LOAD_BITMAP descriptor and handler, trace every flags consumer through the original loader/render/duplicate paths and run a native combination/unknown-bit matrix. Do not infer flag16 from the stretched corpus appearance alone.
