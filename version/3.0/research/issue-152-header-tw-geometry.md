# Issue #152: HEADER text-window geometry, cursor and erase

[Research backlog](issue-backlog.md) · [HEADER](../ripscrip/1.1-protocol-and-command-hierarchy.md#text-window-flags)

## Scope and checkpoint

Started 2026-11-23T00:41:34Z on `issue-152-header-tw-geometry` from fork stable **e6b7834eeb97833c8698b4801aecb605a27ba03b**. Scheduled check **00:37:44Z** detected [#152](https://github.com/bbs-land/remote-imaging-protocol/issues/152), created/updated 00:31:04Z, zero comments, body SHA-256 `e441d8ca16921bdc8ecaf7e408fbaf4e48656c5dc7fc419dc38d2c9313f9535b`. [#153](https://github.com/bbs-land/remote-imaging-protocol/issues/153) follows separately; its body SHA-256 is `5b1ceaa9c73ed0bc8c7a97a7f3bc814c3fdd849978a9647028803e736e9abe76`, created/updated 00:33:53Z, zero comments. Next issue check due about 00:47:44Z.

Complete type-3 geometry/cursor/erase field sets and gates from primary MTW intent, accepted header/TW contracts and the report's supplied static evidence. Do not investigate the cross-window native effect as a software bug or execute an original client/probe. Preserve live RESET(TW), outer SOFT, whole-family backup, ESC snapshot and entry-raster boundaries. No 1.54/text/assets changes; separate complete PRs and stable ancestry remain required.

## Starting sources and limits

Hashes, reproducible with `git show e6b7834:<path>`:

- `version/3.0/ripscrip/1.1-protocol-and-command-hierarchy.md`: SHA-256 `4c819f21b0f95fd9f4123f97b629febded2b2d56d758494951c255583e10a3de`.
- `version/3.0/ripscrip/1.4-terminal-ansi-view.md`: SHA-256 `76896610753e8a18e8f86a39a924cab2260e95fe354eecbceb7943942cee130f`.
- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `bb78e79c9a32a5db2bd251c6159270f203cea41780efeb5d70282fdf5b116245`.
- `version/3.0/ripscrip/1.3-world-view-virtual-canvas.md`: SHA-256 `ac68cd504e0ccd1701b5ba7df3f67a944233023b463757d8c14844c02166cfc8`.
- `version/3.0/techspecs/3.3-microansi-maf-delta.md`: SHA-256 `47785b08e3bf4f0795adddfad92b29ecdc8d1c88f554ac64bde565be98949f63`.
- `version/3.0/research/issue-5-scene-headers.md`: SHA-256 `678fee87426ccf7a67e67bd1317f7afc5aec26ce2132b618e3f666c64580c0a2`.
- `version/3.0/research/issue-101-extended-text-window-contract.md`: SHA-256 `467df2fb7f4800f68838082628b3ae839f6856c6f04f421c3322840b17b7e8b9`.
- `version/3.0/research/issue-148-live-tw-reset.md`: SHA-256 `c2189c0b1b33fbe383acb2e3ac1a7d09760f44aa1c33f9f82b8fa4e7752c9973`.
- `version/3.0/research/issue-150-ansi-esc-snapshot.md`: SHA-256 `c96048d56073faa002157e2ec9a278a8987fa36c89e0a9057164bdb62a6b3c6d`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

The issue supplies read-only observations of original DLL SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, with RVAs, not file offsets: header geometry dispatch **0x83a5**, leaf **0x83fb** gates defined/active/unprotected; **0x847b–0x8553** reads existing font mode and writes zero origins, nominal counts/current-metric bounding and display rectangles. It does not repopulate the record or clear its fixed query/CSI-saved fields. Calls **0x8505/0x8514** use the current sentinel for live cursor setters even for a noncurrent target. Erase **0x4597b** is shared with wire e/ETW and its native leaf uses the current ANSI brush; caret **0x18dbc** skips inactive windows and allows active protected ones. These are **report-supplied static observations**, not new independently executed or inspected binary results.

Accepted [#5](issue-5-scene-headers.md) identifies the same dispatches and the existing header bit/validation/operation scope. Primary ALPHA 4 §5.6.10 MTW (14759 onward) specifies the target's existing font, retained ordinary/extended nature, margins reset, target cursor home without enabling a disabled cursor, no paint and retained ANSI colors. That target-oriented intent and the maintainer's protocol-before-implementation instruction control the cross-window correction: home the targeted window, not an unrelated selected one. This is an explicit protocol completion/correction, not preservation of a native wrong-target write or a new bug investigation.

Accepted [#101 erase reconciliation](issue-101-extended-text-window-contract.md#erase-reconciliation-protocol-before-implementation) establishes graphics palette index 0 for e/ETW, independent of STYLE/ANSI backgrounds, even though their shared native iterator uses the ANSI brush. Extend that documented protocol rule explicitly to HEADER 4096/8192; do not claim native paint-color identity. #148's fresh live RESET(TW,0) recipe and #105's outer SOFT are separate operations. #150's interpreter-owned ESC snapshot survives a non-hard HEADER.

## Chosen completion

HEADER geometry-reset stages (1024 all / 2048 current and the existing slot-zero geometry calls under 1/4) operate only on defined, active, unprotected targets at that stage. Retain existing font mode and ordinary/extended kind. Use that font's nominal columns/rows and current realized cell metrics, with accepted fallback only when no MAF metric is available. Set origins to zero and both bounding/display rectangles to [0,C*cw)×[0,R*ch); retain extended nature rather than running new automatic fitting or user-default substitution. Ordinary cell corners become 0..C-1/0..R-1; extended legacy cell-corner queries remain inapplicable. Reset target margins to the full nominal row range and target live cursor to (0,0), while retaining CSI-saved coordinates, activation, visibility/shape requests, ANSI colors/rendition/modes/tabs/charset, fixed query and other non-geometric TW state. Geometry itself does not paint/select/clear queries or modify the unrelated current TW. The provided wrong-current cursor calls are not protocol requirements.

HEADER erase 4096/8192 applies to defined active TWs, including protected ones; fill the whole current target bounding rectangle, limited by physical screen bounds and independent of graphics PORT/viewport/W/fill. Adopt current graphics PAL index 0, not the native ANSI brush, STYLE background or fixed RGB black. Home only each erased target's live cursor; retain its CSI-saved/ANSI/definition/query state. Caret hide/show likewise requires a defined active target, allows protection, changes only requested visibility/presentation and does not activate an inactive TW. Other header flags retain their separate selection/deletion/query/backup/global-prelude effects.

The key vector uses active extended TW 0, font 4 at 1024×768 with current MAF cell 20×24: nominal 40×25 makes both rectangles (0,0;800,600), not the entire 1024×768 raster. A different selected TW 5 retains its own cursor/font/state under type-3 flag 4. TW 0 keeps its fixed query, CSI-saved point, hidden cursor and ANSI state. Live RESET(TW,0) instead installs fresh ordinary user-default state and clears its fixed query under #148.

A second vector has distinct ANSI background 4, STYLE background 9 and PAL entry 0=(10,200,40): HEADER erase uses (10,200,40) over the target bounding box, including margins outside centered display content, despite protection or an inactive/offscreen graphics port. No fixed query/parser/ESC snapshot/other-table change is introduced by these three leaf operations.

## Independent geometry/state validation

Inline Python checks verify all ten baseline hashes from `git show e6b7834:<path>`. Four literal headers independently decode to revision 1, globals 0, one type-3 segment, slot 0 and flags **4 / 2048 / 2560 / 8192**. Examples are outside GFM tables so formatting cannot split a literal command pipe. The noncurrent extended-font-4 vector gives exactly 40×25 cells and 800×600 rectangles, homes only TW 0 and retains its CSI/query/ANSI/visibility state plus all of current TW 5. A **160-case** matrix checks five fonts, ordinary/extended kind, defined/active/protected gates and current/noncurrent targets; five metric products are 960×731, 1001×731, 960×750, 1001×750 and 800×600. Erase/caret eligibility checks distinguish protection from activation. Logical color/point checks distinguish palette 0, ANSI B, STYLE background and fixed black; include centered-display margins, exclusive far boundaries and physical clipping of a negative near edge.

These are independent arithmetic/abstract-state and wire-decoding checks, not execution of the original parser, client, probe, renderer or UI. The protocol correction is deliberately target-relative; no additional wrong-current software behavior was investigated. New reference crosslinks connect HEADER, ordinary TW lifecycle/erase sources and MTW without changing live RESET(TW) or the queued type-2 work.

Full `run/lint`, `run/check-links` and `git diff --check` pass; links report **0 bad of 6,387 across 264 files**. Focused prose-spacing review passes. Before publication #152 retains the initial body/update time and zero comments. The 00:48:04Z all-author issue check found no newer report; next due about 00:58:04Z. #153 remains queued and uncovered.

## Delivery and next action

[PR #154](https://github.com/bbs-land/remote-imaging-protocol/pull/154) is open/ready, specification **c4fa5949a7bbad59408ab301a4ddf0b7e2fc62b9**, with exactly #152 as closing reference and no closing pagination overflow. The report retains its initial body/update time and zero comments. Scoped `TemporaryDirectory(prefix='rip-152-pr-')` request files were removed automatically. No upstream merge/manual closure or original execution.

Normal-merge actual completed source head into fork stable with an identical tree, audit while explicitly keeping #153 queued/uncovered, and then start #153 from updated stable. Preserve FastFont separation, Codex trailers and cleanup; continue ten-minute checks during serial work.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
