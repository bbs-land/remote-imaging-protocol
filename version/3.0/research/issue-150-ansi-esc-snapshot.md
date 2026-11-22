# Issue #150: ANSI ESC 7/8 text-window snapshot

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md)

## Scope and checkpoint

Started 2026-11-22T20:02:32Z on `issue-150-ansi-esc-snapshot` from fork stable **393c5ebee54687f329c85d8b17db641e4210a18a**. The scheduled check at **19:57:59Z** detected [#150](https://github.com/bbs-land/remote-imaging-protocol/issues/150), created/updated 19:51:09Z, zero comments, body SHA-256 `0975c60cb09e5bca5680b0f6a1319188b4c10c06dbb328636c4a9341167a344a`. Next check due about 20:07:59Z. Continue the user-confirmed serial research-and-PR/stable workflow.

Complete the logical payload, current-target/protection/inactive behavior, ownership, initialization/lifetime and recoverable failure of the existing interpreter-owned snapshot. Preserve #104/#106/#148 and whole-TW backup/COPY contracts. No original-client/probe/bug investigation/execution, raster reproduction, new VT-102 requirement or 1.54 change.

## Starting sources and exact evidence

Hashes, reproducible with `git show 393c5eb:<path>`:

- `version/3.0/ripscrip/1.4-terminal-ansi-view.md`: SHA-256 `c996fc4eedcdc5d6b9bb9b18126449db025f0ac4abb19439348f695928b4ee86`.
- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `92bb8301a3f2231a5e78c4efde84f68a8d4f9d9f738775eb5b8f596659191ab5`.
- `version/3.0/ripscrip/7.2-data-backup-areas.md`: SHA-256 `2cc1b4fbc8991e56098a90e4f71ba10dda2d7e17332d48420b7a80afab6e09a7`.
- `version/3.0/ripscrip/1.3-world-view-virtual-canvas.md`: SHA-256 `d04987435fcbb3b121fd49ee0ba02ceb6c27960b3a978ed44255c566f898cd2b`.
- `version/3.0/ripscrip/1.1-protocol-and-command-hierarchy.md`: SHA-256 `22af7a2170760c665bbf46e3566262d7fdd6fb7f1118987d15f04d8960480332`.
- `version/3.0/ripscrip/3.1-column-text-system.md`: SHA-256 `d4927897eede4fb87f68a4a2aa635ad5707cb28ba5771fe6554fb587773b2c1d`.
- `version/3.0/research/issue-104-terminal-character-sets.md`: SHA-256 `12d3606cffabd8424fad79d76993835df7aaede92f7a4aef877a146eacb00567`.
- `version/3.0/research/issue-106-text-window-selection.md`: SHA-256 `db4d51fe2df2808cf1d3b037c272f1a0ffc7f199e900cac73897bea5f9ffefd5`.
- `version/3.0/research/issue-148-live-tw-reset.md`: SHA-256 `c2189c0b1b33fbe383acb2e3ac1a7d09760f44aa1c33f9f82b8fa4e7752c9973`.
- `version/3.0/research/issue-16-table-lifecycle.md`: SHA-256 `2df08c1e1c5d95d64df3f857dbca4fede6836e76ee32cd6d19baa025754a4757`.

Accepted [#104](issue-104-terminal-character-sets.md#reset-definition-and-saved-state) records original ESC save RVA **0x37404**, full `0xf1` record copy into TW-system **+0x88**, followed by live-cursor coordinates stored in the snapshot's initial bytes. Accepted #106 identifies record **+0/+1** as CSI-saved coordinates. ESC restore **0x37455** copies the record back before restoring/clamping live position; CSI save/restore **0x37177/0x371c1** operate only on per-window saved coordinates. Therefore the old CSI-saved value is not independently retained in an ESC snapshot: its saved-cursor slots receive the captured live position. ESC 7 does not modify the source's live CSI-saved slots; ESC 8 installs the derived value in its destination. This is an exact consequence of already accepted evidence, not a new original-byte investigation.

The native byte-copy size does not define safe pointer ownership or all portable fields. #106 lists logical per-window fields versus interpreter/global exclusions; #148 expressly retains this larger snapshot across live TW reset. Whole-TW backup includes all 36 entries, selection and floating query, but not a separately identified ESC snapshot. #16's owned query duplication supports logical value ownership, not borrowing a historical pointer. The TW introduction's HLP article/column diagnostics do not decode a new setter or native field layout; the completion below handles any already exposed TW-local binding identifiers without inventing an article subsystem or copying its objects/global selection.

## Explicit portable completion to publish

One interpreter-owned optional snapshot S starts absent. ESC 7 captures the eligible current defined active TW by logical value, replaces S only after a successful owned capture, and retains the source record/selection/pixels. Protected sources may be read. Save kind, stored cell/bounding/display geometry and counts/metrics, logical font selection, definition/protection/activation, live position, derived CSI-saved position, colors/rendition/per-window modes, margins, explicit tabs, charset, requested cursor visibility/shape, fixed query and any already exposed TW-local article/column binding identifiers/unset values. The latter are values referencing live resources, not ownership of flowed articles/columns; unsupported bindings remain absent and no new commands are required.

ESC 8 applies S into whichever TW is current at execution, without selecting/recreating another slot or using a stored source index. Require a defined active unprotected target; undefined/inactive/protected target or absent S is a consumed no-op. Restore slot 0 always defined/unprotected, even if S came from a protected nonzero source; nonzero target receives saved protection. Preserve source-independent stored geometry/font rather than refitting/reprojecting or using the current user default. Restore live position with the existing bounds clamp, and CSI-saved coordinates from the captured live position; do not recover the pre-save CSI-saved value. Ordinary active save yields active state. Inactive raw text remains discarded; explicit reactivation can make a later ESC 8 eligible. ANSI ESC 7/8 do not require a VT-102 client mode.

Snapshot excludes source/current slot index, physical pixels/cell/history stores, all family backups, floating query, connection parser/input and global emulation/graphics/flow selection. Recognized escape sequences advance the parser normally; they do not restore a saved parser prefix. No query/provider/flow command runs. Resolve saved logical font and binding identifiers to current compatible resources without native handles/pointers; missing/incompatible resources cause recoverable restore failure rather than substitution, refitting, object recreation or partial publication. Destination query ownership is independent of S and source state.

Successful restore does not consume S; repeated use reads the same immutable values until a new successful ESC 7 or explicit clearing event. Ordinary/extended definition, MTW, selection, entry COPY/delete, live TW reset including TBL/ALL, whole-TW save/restore, CSI s/u, ESC c and SOFT retain it. HARD/accepted HEADER hard-reset flag and a new/ended interpreter connection session clear it; clear when the hard-reset operation is accepted/applied before fallible subsystem setup, without resurrection after later failure. Non-hard HEADER operations retain it. SOFT's screen clear is not undone by later state-only ESC 8.

Save failure keeps old S (including absence). Restore prepares all owned values/resources before committing; failure keeps target definition/query/selection and S intact. No Boolean reply/session termination, native pointer alias, extra save area, paint/clear/relocation or snapshot-dependent raster rollback is introduced. Cursor overlay realization may change after successful commit according to the selected record's requests; exact pixels remain renderer-defined.

## Independent logical validation

An inline Python logical model verifies all ten baseline hashes from `git show 393c5eb:<path>`, the live-to-CSI overwrite while the source's old CSI-saved value remains intact, literal 0→5 ordinary/extended geometry/font/query restoration, independent mutable query/tab ownership, source deletion/redefinition and whole-table replacement, repeated/uninitialized/inactive/protected paths, failed save with old/absent S, and atomic font/binding/query-ownership restore failures. A **708-case** matrix covers both saved kinds, protected/unprotected sources and all 36 target indices with active/protected/inactive/undefined states, respecting slot-zero invariants and preserving global state. The lifetime ownership cases enumerate **22 retaining events and four clearing events**, with representative record replacement and a SOFT-style black pixel buffer proving that state-only restore does not recover pixels. These are explicit-policy state checks, not native command, ANSI parser, provider, UI or raster executions. Literal byte pairs were checked without running a parser.

The payload/comparison/lifetime tables explicitly separate native evidence from safe logical ownership. The saved source index, live resource handles, whole-family backup owner and global flowed-text selection are excluded. Already exposed TW-local binding identifiers are copied only as values and require current compatible resources; diagnostics do not invent setters or object snapshots. The protection exception at zero is applied to the destination copy, leaving S unchanged. Recognized escape consumption advances the current parser normally rather than restoring a snapshot parser prefix.

Full `run/lint`, `run/check-links` and `git diff --check` pass; link validation reports **0 bad of 6,367 across 263 files**. A focused word/number-spacing scan passes. Before publication, #150 retains its original body/update time and zero comments. Latest all-author issue check 20:08:54Z found no newer report; next due about 20:18:54Z.

## Next action

Publish compact payload/comparison/lifetime tables and exact current-0→5, CSI-overwrite, changed geometry/font, source deletion/redefinition, protection/inactivity, repeated/uninitialized and failure examples. Verify source hashes and independent ownership/state/lifetime vectors, format/lint/link/diff, refresh/publish a separate PR, normal-merge its actual head into stable with an identical tree, audit and return to the ten-minute watch.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
