# Issue #24: drawing-state ownership, defaults and text cursors

[Research backlog](issue-backlog.md) · [Data tables](../ripscrip/7.1-data-tables.md) · [Text output](../ripscrip/3.0-text-output-and-fonts.md)

## Scope and source

Started 2026-11-16 after [PR #56](https://github.com/bbs-land/remote-imaging-protocol/pull/56) delivered #23. Branch `issue-24-drawing-state-ownership` starts at `e33a1d7`. [Issue #24](https://github.com/bbs-land/remote-imaging-protocol/issues/24) asks for port versus style cursor/image ownership, environment versus style color-mode/mouse ownership and activation, exact style defaults, backup scope and TEXT_XY cursor updates. Resolve every part before filing a PR.

Original source: RIPtel 3.1 installer [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`; extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The artifact mirror/provenance is described in the backlog. PE base is `0x10000000`; addresses below are RVAs. Original files remain unchanged. Use Capstone 5.0.7 disassembly and native i386 probes with controlled services; no desktop GUI.

## Initial leads and remaining work

- Confirmed earlier: ports have stride `0x78`, image rectangle/flags at `+0x4a..+0x5a` and cursor at `+0x5c/+0x60`; style records have stride `0x61`. This layout alone does not prove activation behavior. Trace original port/style selection and backup consumers before assigning ownership.
- Original drawing initialization starts at `0x6666`. Stores observed at `0x66bd` onward include style words `+0=15`, `+2=0`, bytes `+4=0`, `+5=1`, word `+6=4`, bytes `+8/+9=0`, word `+0x1a=15`, byte `+0x1c=0`, byte `+0x1d=1`, word `+0x1e=0`, byte `+0x20=0`, flags `+0x21=0`, word `+0x22=1`. Identify each field through its setters/consumers and compare per-slot initialization before publishing defaults.
- Style activation lead `0xe6e9`; port selection `0x3393c`; whole-port save/restore `0x219a9/0x22226`. Existing table probes deliberately stub activation and cannot establish activation semantics without extending their boundary.
- Environment records have stride 16; current index is system `+0xa`. #23 proves RGB bits at record `+7` and active configuration `+0x3a`; trace full activation, cursor and protection separately.
- TEXT handler `0x20169`, TEXT_XY `0x20cbc`, common text adapter `0x42870`. Trace anchor stores and post-render cursor gates with concrete fixtures.

Next: identify complete original style initialization/activation and environment ownership, then extend native reproduction to cover alternating selections, backups and cursor updates. No specification edits or completed answer yet.

## Native ownership and text checkpoint

Extended the existing native table/font probes; original handlers/selectors remain original bytes, with the controlled-service boundary described below. `run/riptel-font-probe DLL` passes **196,166 checks** including new cursor cases. Table probes are being finalized before publishing counts. Each wrapper verifies the driver hash, builds with `cc -m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`, disables core dumps and removes its temporary directory by EXIT trap. FreeBSD 15.1 / clang 19.1.7 / clang-format 19; no Wine or GUI.

### Ownership and switching observations

Original style switch `0x394fe–0x39628`, called directly by wire `2Y` handler `0x46e41`, selects/defaults a `0x61`-byte style, rebuilds brush/ROP/pen and invalidates the font cache. It never reads or writes the port image/cursor fields or an environment record. Original port switch `0x3393c–0x339e3` selects a port/DC and changes no style record/selection. All **36 × 36** defined port/style combinations preserve complete port/environment arrays across style switching and complete style arrays across port switching. The sample port 1/style 1 → port 2/style 2 → style 1 leaves port 2's image/cursor active; switching just port 1 restores its state while retaining style 2. There are no competing copies or capture precedence.

The draft's first-use copy-current rule is also false for this driver: style switch copies fixed record `0x762e8` into undefined entries, just as initializer/delete `0x391fd` does. Native wire selections of all 35 nonzero unused styles start with drawing index 15 after changing the old style to 23. Original CUR calls the same selector. An explicit COPY is needed to clone a style.

Original environment switch `0x3dac0–0x3db93`, called by wire `2E` handler `0x46da0`, activates math, RGB precision, coordinate width and world size, marks cursor refresh if needed and finally changes the environment index. It does not switch STYLE/BUT/PORT/TW/PAL. RGB precision lives at environment record `+7` and active config `+0x3a`; mouse number at environment record `+8`. Style selection never activates either. Color wire handler `0x1f3fd` and cursor wire handler `0xdc96` test current **environment** protection; style protection is irrelevant. Actual cursor setter/presenter `0x3468a/0x34635` stores/reads environment `+8`, then calls SetCursor/ShowCursor; native fixtures capture those imports with distinct cursor handles 1000+n.

The original environment switch has a sequencing defect: it calls base/RGB/coordinate setters before changing the current index. Those setters write target values into the **departing** record's bytes `+5/+7/+6`, even if protected. World activation `0x31052` updates active DWORDs without copying dimensions into that old record. Cursor bytes stay independent. Native switches 0 → 5 → 0 prove that returning to 0 keeps 5's overwritten base/precision/coordinate size, but restores 0's saved dimensions/mouse. No other table selection changes. Target protection does not block selection.

The COLORMODE callback `0x17eac` requires a defined target; it does not define an unused environment and does not check protection. Noncurrent target RGB changes remain dormant until activation. MCURSOR `0x34750` also bypasses environment protection. Native flags 0..3 and current protected/unprotected style/environment combinations cover these differences. An initial fixture incorrectly expected COLORMODE to define an unused slot; original `0x18002` checks definition and sets the variable error instead. Corrected the fixture, not the driver.

### Defaults: rejected bootstrap lead

`0x6666` is an exported SDK **RIP_SetDefaultSettings** routine, not the normal driver startup/reset. It is not called by original initialization `RIP_InstanceInitEx` (which calls `0x16220` at `0x6401`); the RIPtel executable does not import that export. Its font 1/size 4/fill 0 stores are not the bootstrap defaults. Earlier #3 notes already identify that routine's outline values as its own dormant settings; keep those distinct from normal style initialization.

The fixed 97-byte record is in **.rdata**, RVA `0x762e8`, file offset **0x752e8**, not .data. Native reset/default comparisons establish drawing index **15**, background **0**, COPY mode 0, classic font **0**, size **1**, direction **0**, fill pattern **1** (solid), fill index **15**, line style 0, thickness 1, user line mask 0, odd rule 0, border enabled, font mode classic, alignment flags 0. User fill rows are zero. Dormant outline name/size/style/flags/distances/rotations are zero; spacing is 100. Normal reset's classic engine justification is separately set to horizontal left / vertical top (`0x16493–0x1649c`); with flags zero the original style-to-classic justification decoder `0x39808` also yields left/top. Do not interpret zero flags as baseline alignment. Need publish the exact table and reconcile the inherited lists.

### Backups

Original style save `0x217f7` writes current index (two bytes), then exactly **36 × 97** bytes; it contains no port or environment fields. Original restore `0x218bf` has an index destination defect: `0x218f8–0x21908` reads the saved index into the **current style record's first word**, then overwrites the entire table from the stream. It never restores the index field and reactivates the index that was current before restore (`0x21981–0x21987`). Native save at style 5, restore while style 7 is selected retains 7 and restores its saved foreground 73; 3,494 bytes consumed. Initial status comparison failed because this routine returns AX-only (`0x00420001` in the fixture); masking low word resolves the harness error, with every stream/index/field observation already matching.

Original port save/restore `0x219a9/0x22226` is extended to assert image rectangle/flags and cursor restoration as well as prior query/protection cases. It captures no style/environment record. Validate final counts after extension.

### Text cursor stores

Original MOVE `0x1cef0` converts the supplied point with `0x31084`, adds current viewport origin `+0x3a/+0x3e`, then stores port `+0x5c/+0x60`. TEXT `0x20169` renders at stored coordinates and adds the signed low-word width to stored X, leaving Y untouched. It does this for **all** justifications and orientations; no horizontal/left gate exists. Classic uses metric `0x1410`, outline uses `0x3fcd0`.

Original TEXT_XY `0x20cbc` always replaces both cursor coordinates after a nonempty, enabled draw, irrespective of font orientation/justification. Outline path converts its input point using `0x31084`; classic path does not convert before its post-draw cursor stores. It adds viewport origin and the metric to X, origin to Y. The classic width branch tests **font number byte +5**, not direction byte +8: nonzero classic fonts use `0x1410`, classic font zero falls through to outline metric `0x3fcd0`. The latter uses the cached outline realization even in classic mode. These are shipped defects, not MOVE+TEXT equivalence.

Native cursor matrix: scales 1/2; classic and outline; classic font byte 0/1; directions 0..1 classic, 0..3 outline; horizontal flags 0/1/2/4 and vertical flags 0/8/16/32/64. Original conversion/handlers/stores execute; controlled classic width is **13**, outline width **37** so branch selection is observable. Viewport origin (5,7), input (100,80), previous cursor (300,400). At scale one, outline TEXT_XY becomes **(142,87)** for every direction/justification; nonzero classic becomes **(118,87)**; classic zero becomes (142,87). At scale two, MOVE becomes (55,47), then outline TEXT becomes (92,47); outline TEXT_XY becomes (92,47), while classic TEXT_XY retains (118,87) or (142,87). Captured render-call anchors identify arguments at the controlled helper boundary, not final raster coordinates. Empty TEXT_XY rejects unchanged, disabled TEXT skips unchanged. No Windows pixel claim.

## Next

Finish port/default native assertions and count; document exact hook boundaries, add implementor tables/examples and cross-links, update AGENTS.md, run format/lint/link/diff checks and file one complete #24 PR dependent on #56.
