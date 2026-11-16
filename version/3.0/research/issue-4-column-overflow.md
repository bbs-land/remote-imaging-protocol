# Issue #4: column, article and overflow derivation

[Research backlog](issue-backlog.md) · [Current column reference](../ripscrip/3.1-column-text-system.md)

## Status and questions

Started 2026-11-16 for [issue #4](https://github.com/bbs-land/remote-imaging-protocol/issues/4), branch `issue-4-column-overflow`. The complete answer is delivered in [PR #30](https://github.com/bbs-land/remote-imaging-protocol/pull/30); validation passed. Maintainer review/merge is pending. The branch starts at #3’s delivered branch (PR #29) to reuse the primary-source tools and workflow. If #29 remains unmerged, a PR against upstream main will include that dependency; identify it explicitly and compare the issue-specific commits separately.

Required answers: `1e` flags and `cc`; article/column ranges and numbering scope; flow order; retained content and page position after region redefinition; `$OVERFLOW(article,direction[,setverbose])$` results, side effects, notifications and reset rules; expected NEWS.RIP/NEWSPAPR.RIP states before/after navigation, end and reset. `1A` identification and wire layout remain in issue #2; trace its effects only as needed for this contract.

## Sources and reproduction

Use the exact driver recovered for [issue #3](issue-3-extended-font-style.md#artifact-recovery-2026-11-16): RIPtel 3.1 MSI, `ripscrip.dll` SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, under external mirror `artifacts/riptel-3.1-pcmicro/extracted/`. All code addresses in this note are RVAs; the preferred image base is `0x10000000`. Keep artifacts unchanged. Use `run/riptel-inspect` for fingerprint/dispatch/bytes and original-instruction probes for independent state observations. Any GUI work requires the private-display isolation in AGENTS.md.

Initial queue refresh confirms #3 has complete PR #29 and #4 is the oldest remaining issue. Existing column reference is corpus-based and labels bit 0 and `cc` as hypotheses; none of these hypotheses are evidence of driver behavior.

The external RIPlib source offers leads: `1A` handler RVA `0xdc58` calls article selection at `0x3c399`; context +0x2a points into the article system. Verify all leads in the identified artifact. RIPlib’s current simplified overflow implementation is not a primary source.

## Column and macro trace (2026-11-16, incomplete)

Dispatch: `1e` slot 93, handler `0xa5ed`, types `ff ff ff ff 01 01 04 02 08`, flags `0x000f` (current radix). Four coordinates use the current coordinate width, so the crib’s 24 characters assume two-character coordinates. A string tail follows the fixed fields. `1T` slot 105 (`0xa529`), `1E` slot 92 (`0xbdbd`), `1t` slot 106 (`0xd70b`). The handler identifies itself as `RIP_BeginExtendedText()`.

`1e` accepts column 0..35, article 1..35, flags 0..3. Bit 0 sets the article’s overflow enable byte; each definition overwrites it, so the last definition for that article wins. Bit 1 parses a search-word string and enables whole-token, case-insensitive highlighting. `cc` is the highlight palette index, checked 0..255 only when bit 1 is set, otherwise ignored. Up to eight whitespace-separated words of up to sixteen bytes are copied. An empty/whitespace-only list or a word longer than sixteen rejects; a ninth word reports an error but installs the first eight. The original matching helper is `0x3ad73`. The eight-character reserved argument is not read by the handler. These are now observed by the native probe as well as disassembly; macro expansion is an identity hook for the literal test inputs.

Context +0x2a points to a 26-byte article-system structure; its +0x16 points to a 0x2285-byte state. State +0 is selected article, +9 allocated column count, +0xb article-active flag. Article records have stride 0x18: +0xd line count, +0xf current column (255 initially, 36 exhausted), +0x10 device Y, +0x12 auxiliary flag, +0x13 overflow enable, +0x14 current zero-based page, +0x18 highest materialized page, +0x1c source-copy-created flag, +0x1d remainder-output handle, +0x21 complete-source-output handle. Column records at state+0x36d have stride 0xb2: +0 defined, +1 owning article, +2 world width, +4 device width, +6 world rectangle, +0x16 device rectangle, +0x26 flags, +0x28 word count, +0x29 highlight color, +0x2a eight 17-byte search slots.

Definition helper `0x3be44` replaces a global column slot, assigns its article, resets that article’s current column to 255, and closes its two output handles. It retains page counters and source-copy-created/auxiliary flags. Redefining an existing slot does not increment the global column count. Reassigning a column moves ownership to the new article. Selector `0x3c399` flushes pending text when changing article, resets pending word/line buffers, selects the requested article, and marks it active iff any column belongs to it. Flow helper `0x3ba81` scans ascending global column numbers after the current one, skips holes/other articles, sets starting Y and line count, and returns 36 when exhausted. Definition order and screen coordinates do not sort flow.

OVERFLOW registration at `0x26d01` points to `0x28854`. Macro parameters are separate 21-byte slots. Decimal article parser `0x98fc` accepts 0..35 or CUR; the OVERFLOW handler rejects explicit 0, but CUR resolves the current selection afterward. The filename helper `0x3bddc` returns `<NONE>` for article <1 or disabled overflow; otherwise `%03hu%c%04ld.OVR` using context +0x7a, article encoded 0..9/A..Z, and zero-based page. It does not check page existence.

Navigation increments/decrements the current page only when within [0, highest materialized page], closes both output handles on movement, and returns an empty string. On a boundary it returns NULL, leaves the page unchanged, and sets macro-state +0x438=1. SETVERBOSE additionally calls MessageBoxA with caption `Cannot Continue` and body `Beginning of document.` or `End of document.`. QUERY never moves; NEXT,QUERY returns next-page filename only if enabled and current < highest, else `<NONE>`. **Shipped defect:** PREV,QUERY checks only enable and returns current+1’s filename, even at the beginning. Numeric direction sets an already materialized zero-based page (validated as decimal digits; signed-word assignment quirk still needs boundary coverage). CUR returns the current filename; PAGE returns current+1 as decimal even when overflow is inactive. SET is equivalent to movement without a dialog. Third arguments other than SET/SETVERBOSE/QUERY reject for NEXT/PREV; only QUERY is accepted for CUR/PAGE; RESET requires exactly two arguments.

Per-article OVERFLOW(n,RESET) clears overflow enable, current/highest page, auxiliary/source-copy flags and output handles; it retains column definitions, selection and the current column position. Global RESET(OVERFLOW) callback `0x43a17` invokes `0x3c1cf(context,1)`: clears all column definitions, selection and layout/buffers as well as counters/source flags and handles. The same reset helper with second argument 0 retains page counters and source-copy-created flags but disables overflow and removes columns. Its callers and exact scene/reset lifecycle still require tracing.

## Native probe checkpoint

`run/riptel-column-probe <dll>` currently passes 17,715 checks: article/column ranges, flags, color domains, search matching/list boundaries, numbering/ownership/flow, redefine retention, macro navigation/query/error/dialog branches and both reset forms. It executes the original routines with controlled CRT, rectangle/coordinate, synchronization, protection, macro-expansion and pending-line callbacks. MessageBoxA is intercepted; the probe has no GUI. This is not a full client or glyph/layout validation. The pending-line callback must be restored and the raw-byte flow path exercised before treating pagination as complete.

Rejected probe expectation: a whitespace-only highlight list does not install a flag-cleared column. The driver trims the string before parsing and rejects it as an invalid search list; the test was corrected to match the original path.

## Raw flow, replay and boundary completion

Restored the original pending-line routine `0x3cb80` for the final probe phase. Original raw-byte feeder `0x3ce24`, wrap/justify routine `0x3c487`, line advance `0x3bf54`, column traversal, source capture `0x3cd1b`/`0x3cdef`, remainder output `0x3cc13` and end operation `0x3d488` execute unchanged. Controlled font metrics are ten-unit line advance, ascent eight, descent two, one-unit character and space widths; rectangle (0,0)-(9,19). Drawing calls are captured and CRT output files are represented by bounded in-memory streams. The exact hook lists are in `install_hooks()`, `flow_checks()` and the marked macro-name lookup check. No GUI services are used; MessageBoxA calls are captured as data.

Input `aaa bbb ccc ddd eee fff ggg hhh iii jjj kkk lll mmm nnn ooo ppp` creates `00710000.OVR` containing the complete source and `00710001.OVR` beginning `ccc ddd...`. It renders only the first two words on the first page. Seven subsequent NEXT/redefine/feed/end cycles retain every word in order and produce eight files total; the final page adds no remainder. P remains the selected page during replay. Re-rendering page 0 then replaces the forward limit with 1, despite files 2..7 still existing in the memory sink. **Correction to the preliminary state label:** +0x18 is the current known forward limit, not a monotonic highest materialized page or precomputed page count. Creation at `0x3ccf4–0x3ccf8` assigns P+1 directly. Disabled retention creates no files and discards excess prose.

Both output handles are closed at navigation/redefinition/end. The complete source is captured only at P=0 while the source-created flag is false and the forward limit is zero. End (`0x3d488`) flushes pending text through `0x3d442`, deactivates flow and closes streams, retaining definitions, selection and paging state. `1E` (`0xbdbd`) calls it through `0x3bf47`; scene-end `#` (`0x1cf6a`) calls it directly. A same-article definition drops pending text because selection resets the buffers without flushing when the selected identifier is unchanged; defining/selecting another article flushes the old pending line. Both observations are in the probe.

Macro evaluation `0x25a7b` resets +0x438 before invoking a registered handler and converts its set value to result code 3 (`0x25ae9–0x25af6`). `0x25b21` propagates this. The original macro evaluator is exercised with the actual OVERFLOW callback and a controlled name lookup: boundary returns 3, the next CUR evaluation succeeds and clears the prior abort. Host-response expansion `0x4b0e4`, call at `0x4b2a7`, branches on code 3 to `0x4b450`, releases scratch and returns failure; the following playback macro is not evaluated. Preprocessor consumers also terminate on code 3 (`0x4bd7d`, `0x4cc18`). Therefore SETVERBOSE is not just an optional message toggle on an otherwise successful empty expansion: boundary abort applies even to nonverbose SET.

Decimal numeric page selection tests expose signed-word storage at `0x289a2–0x289ac`: accepted 32768 is stored as 0xffff8000, and 65536 as zero. The validation first compares the original decimal value with the forward limit. The reference records this defect and the useful nonnegative domain 0..32767. PREV,QUERY’s erroneous P+1 filename is independently observed at P=0 and P=1. No-argument, one-argument, CUR article, invalid mode/operation/count, disabled overflow and per/global reset cases are checked.

Global RESET registration `0x2670e` registers `0x43d38`; its keyword table at RVA `0x82428` contains the OVERFLOW entry pointing to `0x43a17`. The probe now executes the complete keyword dispatcher as well as the callback. That callback calls `0x3c1cf(context,1)` and contains no unlink/delete operation. The routine’s second argument 0 preserves counters and source-created flags; the caller at `0x162f0` is the drawing-reset helper `0x16220`, invoked by `*` at `0x1f31b`. Thus drawing reset disables retention/removes columns while preserving paging counters; it is not equivalent to RESET(OVERFLOW).

`1T` uses the same column helper but does not install overflow/search flags. Native exhaustive identifier tests confirm both identifiers 0..35, including article zero. Raw-byte feeding rejects selected article <=0 (`0x3ce4c–0x3ce50`), distinguishing accepted compatibility storage from an active raw article. The original end operation also exits immediately for selected article zero; the probe verifies both early exits. `1e` follows global base math, independently verified with the original radix predicate under both settings; it is not always-base-64 like `y`.

## Demo provenance and interpretation

The MSI’s flat extraction uses `demo_01.col` / `demo_02.col`; the scripts themselves refer to `demo-01.col` / `demo-02.col`. Preserve this packaging observation rather than renaming original files. The specification uses the historical template names already present in the corpus reference.

| Extracted file | SHA-256 |
| --- | --- |
| `news.rip` | `5bc763f386e8c8c7ce3edf156bd312bb3d578490e842ede6d32ad70422f0fdc5` |
| `newspapr.rip` | `2bf71cb7abc4450504aa2cae25be5eea167e3e9774fd4ce94b36cb099a48f2d3` |
| `demo_01.col` | `4f2fdea5a7cee67f8f892924176387d5540dcb3da8332fcc0f88148c45a0df1b` |
| `demo_02.col` | `add152d0bf9200993122610478b507a41154ae0d7bc6c8ff16e953f7dd02d921` |
| `story01.txt` | `70e6c752ee8a8c4feb748b91b573a0ee008a3ff8f5a0c0e3c83c22a95db16a97` |
| `story02.txt` | `74d306e7013f89a9d6719605fb8377d5f7d7a658f3db97940a13771cd9c5e55d` |

NEWS.RIP defines legacy slots 0..3 of article 1 without enabling overflow; from clean state it cannot create remainders. NEWSPAPR.RIP issues global RESET(OVERFLOW), then defines article 1 in slots 0..2 and article 2 in slot 3 with retention enabled. The continuation templates redefine those slots and read CUR. The [reference state table](../ripscrip/3.1-column-text-system.md#the-col-page-template---worked-example) states P/H transitions and boundary/reset effects without inventing a universal pixel-dependent newspaper page count. It explicitly conditions remainder creation on whether text fits the selected font/device geometry. This is the complete state contract requested; native controlled-metric replay independently checks the mechanics.

## Validation and next action

`run/riptel-column-probe <dll>` passes 23,013 checks after completion of the raw-flow and macro-dispatch phases. The native build uses `-m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`; supporting C is formatted with clang-format 19. The wrapper checks SHA-256 before mapping the image and deletes only its temporary build directory. Probe source and documentation are committed; no artifact binaries are added.

The complete reference answers all issue #4 subquestions, with primary evidence and reproducible state oracles. Delivered in PR #30 after `run/lint`, `run/check-links` (4,510 links across 186 files), `git diff --check`, Bash syntax and the 23,013-check native probe passed. PR #30 declares its dependency on unmerged PR #29 and links the issue-specific comparison from `ca92e10`. Continue with issue #5 after refreshing GitHub state.

---

[Research backlog](issue-backlog.md) · [Current column reference](../ripscrip/3.1-column-text-system.md)
