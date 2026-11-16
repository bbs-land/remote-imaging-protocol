# Issue #33: filled-rectangle border pattern and write mode

[Research backlog](issue-backlog.md) · [Filled rectangles](../ripscrip/2.3-shapes-and-fills.md#rip_filled_rectangle) · [Border control](../ripscrip/2.3-shapes-and-fills.md#rip_set_border)

Started after [PR #62](https://github.com/bbs-land/remote-imaging-protocol/pull/62) delivered #28. Branch `issue-33-filled-rectangle-borders` starts at **4819dcb**. Refreshed GitHub creation-order queue selects [issue #33](https://github.com/bbs-land/remote-imaging-protocol/issues/33): K claims current line style/pattern/thickness, while N says nonpolygon borders are solid COPY. Derive original pen/ROP selection with borders enabled/disabled, dotted/custom pens and multiple thickness/write-mode inputs. Do not infer border behavior from #12's brush-phase proof or SyncTERM's interpretation.

Primary source: RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`; original extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base 0x10000000. All addresses are RVAs. Native/static only, controlled GDI capture and no desktop GUI. Clean scoped temporary builds/request files promptly; original artifacts/text stay unchanged.

Existing `run/riptel-fill-probe` executes original K/BAR consumers and separates brush/pen selections. Next: inspect K's border-pen producer and active ROP, extend that probe to capture original selections across line styles/widths/modes, then reconcile the reference and file its complete PR. No #33 answer published yet.
