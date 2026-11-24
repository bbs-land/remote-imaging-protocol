# Issue #106: Text-window selection state boundary

[Research backlog](issue-backlog.md) · [Window selection](../ripscrip/1.4-terminal-ansi-view.md#rip_switch_text_window) · [CUR](../ripscrip/5.1-text-variables.md#cur)

## Scope and checkpoint

Started 2026-11-20 on `issue-106-text-window-selection` from stable 7c606f0 after #105 / PR #109 actual head 773da35 was merged with an identical tree. [Issue #106](https://github.com/bbs-land/remote-imaging-protocol/issues/106) requests first-use state copying, existing/protected selection, connection-wide partial ANSI input and pixel/cell/history boundaries for `2T` and CUR(TW,n). New #108 geometry-query grammar follows in creation order. Preserve the previously adopted table-copy/backup, definition and charset contracts and separate FastFont research.

## Primary sources and read-only identity

Original RIPtel 3.1 / RIPscrip driver 3.0.7 DLL in [the catalogued mirror](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base `0x10000000`. All addresses are RVAs; section biases `.text` minus `0xc00`, `.rdata` minus `0x1000`, `.data` minus `0x1a00`. LLVM objdump 19.1.7 and Python read original instructions/data; nothing from the client/parser/UI is executed.

Immutable [ALPHA 4](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`: §3.4.3.10 at line 10871 explicitly says an undefined wire-selected TW receives the current definition. The CUR entry's default-family table instead explicitly says unused TWs are full-screen in the user's default font, as retained in the [canonical CUR entry](../ripscrip/5.1-text-variables.md#cur). The per-window state list at lines 1875–1910 identifies ANSI attributes, cursor, font/geometry, visibility, margins and activation, and says window text contents are not preserved as part of a definition. Protection rules distinguish editable configuration from selection/output. Do not silently unify the two first-use rules or override the wire copy statement with implementation evidence.

## Original consumers and what they establish

Physical dispatch slot 119 -> wire `2T` handler `0x46ece`. For the ordinary reserved-zero form it validates the window number and calls selector `0xe0dc` at `0x46f36`. The original descriptor additionally carries historical protection flags; those wire-layout/extension questions are outside this issue and not newly adopted. CUR callback `0x34f7d` uses the same selector for TW at `0x35037`. Its query-only branch reads TW-system current index `+0x0a`; it does not select another window or move a cursor.

Selector `0xe0dc` validates 0–35, hides the old cursor through `0x18cf6`, stores destination index at `0xe11f`, realizes the destination font through `0xddfc`, then tests existence through `0xe028`. It does not reject an existing protected TW or reset its record. For an undefined target it calls user-default-font getter `0x3170d(-1)` at `0xe159` and full-screen ordinary initializer `0x8577` at `0xe163`, then presents the cursor. There is no source-record copy in this fallback. Thus native shared selection corroborates the CUR full-screen default, not the wire entry's explicit copy-current promise. This distinction is recorded rather than investigated as an obsolete-client defect.

This fallback is distinct from explicit table COPY helper `0xe17e`, already studied in [#16](issue-16-table-lifecycle.md): that helper validates source/destination, duplicates resident query text as owned data, copies a full `0xf1`-byte record and handles copied protection. Its successful record-copy semantics do not prove it is called by implicit wire first use. Do not change #16's explicit-COPY source-protection behavior to match this issue's separately chosen implicit-creation policy.

TW-system = context `+0x12`, record stride `0xf1`; per-record fields cover ordinary cell coordinates, bound/display rectangles, font, live/saved cursor, rendition/ANSI colors, modes, margins, tab map, charset fields and resident query. Separate ANSI parser context `+0x46` holds in-progress syntax/parameters, including state `+0x0c`; the selector, default font activation and ordinary initializer do not call parser reset `0x37b4b` or move those parser fields into a record. Existing TW selection therefore continues connection-wide partial ANSI input in the destination. The generalization to optional supported ANSI string parsers is a portable ownership completion, not a claim that every modern OSC/DCS grammar exists in RIPtel.

Source cursor helpers change presentation, not screen content. `0x8034` default definition also allocates no text-cell/history store and does no display erase/copy. This agrees with the explicit protocol pixel-overlap model. Global emulation/configuration, graphics family selections and input lock are separate ownership; none becomes part of a TW clone.

## Explicit protocol completion and first-use table

Wire first-use copying remains normative from the original command entry. Because the documentation never enumerated its full copy boundary, the reference explicitly chooses a complete logical per-window clone, retaining ordinary/extended geometry without refitting, font/activation, live and CSI-saved cursor, visibility/shape request, ANSI colors/rendition, modes/margins/tabs, charset and an independent resident-query value. Source and target do not share mutable logical fields. Optional direct ANSI colors are copied only if the implementation already offers them; this does not add new SGR/UTF-8 requirements or change graphics color mode.

New implicit wire destinations are **unprotected**, even when the source is protected. This is an explicit portable creation policy, not an invocation of explicit COPY's protection propagation. Existing protected targets remain protected and selectable. Selecting an inactive target/clone does not activate it. No first-use text cells, pixels or history are copied; any modern history store is not a definition clone. Allocation/resource failure behavior is not newly given transactional guarantees.

CUR's separately documented unused-slot path creates an active ordinary full-screen default-font window, normal 7/0 colors/rendition, origin live/CSI-saved cursor, visible/default shape, normal per-window modes with user's wrap preference, full margins, empty explicit tab map, US/US with G0 invoked, no resident query, unprotected. These fill in its fresh-default boundary, rather than inheriting the source's nondefault fields. Existing selection under either form leaves every target record field unchanged.

CSI-saved coordinates are per-record +0/+1. The larger ESC-7/8 snapshot instead lives in TW-system `+0x88`, as recovered in [#104](issue-104-terminal-character-sets.md), and is interpreter-owned; it is retained across selection and not cloned as a second per-window save slot. Partial ANSI parser input is likewise one connection-owned state, not a per-window byte buffer or first-use clone.

The reference's six-step example follows ordinary TW 0 -> wire first-use 3 -> modified/protected 3 -> existing 0/3 -> split ESC[31m -> CUR first-use 4. The split uses terminal bytes `1b 5b 33` followed, after selection, by `31 6d`: one CSI foreground operation affects 3 only. No raw bytes are inserted by the selection event. Additional protected-source and same-current cases are stated explicitly; no protocol output requires repainting cells/history.

## Reproduction, checks and remaining work

Bounded reads from the repository root, replacing `DLL` with the mirror path:

```sh
run/riptel-inspect DLL --slot 119
objdump -d --start-address=0x10046ece --stop-address=0x10046f66 DLL
objdump -d --start-address=0x10034f7d --stop-address=0x100350e9 DLL
objdump -d --start-address=0x1000e0dc --stop-address=0x1000e17e DLL
objdump -d --start-address=0x1000e17e --stop-address=0x1000e2b0 DLL
objdump -d --start-address=0x10037b4b --stop-address=0x10037c4a DLL
```

Four original selector/default call targets pass byte checks; the complete field table and six-step transition were manually reviewed against documentary source and explicit policy. No native clone/cursor/parser result is claimed. Markdown lint, 5,915 links across 242 files and diff checks pass. Refresh 2026-11-20T22:01:51Z finds unchanged #106/#108 bodies and no comments. [PR #110](https://github.com/bbs-land/remote-imaging-protocol/pull/110), specification bfb84a5, is open/ready and closes exactly #106. Actual-head stable consolidation follows, then #108 query grammar. No temporary artifacts, original text/assets changes or original/client/UI/probe execution.

---

[Research backlog](issue-backlog.md) · [Window selection](../ripscrip/1.4-terminal-ansi-view.md#rip_switch_text_window)
