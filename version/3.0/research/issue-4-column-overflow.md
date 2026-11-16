# Issue #4: column, article and overflow derivation

[Research backlog](issue-backlog.md) · [Current column reference](../ripscrip/3.1-column-text-system.md)

## Status and questions

Started 2026-11-16 for [issue #4](https://github.com/bbs-land/remote-imaging-protocol/issues/4), branch `issue-4-column-overflow`. No resolution PR yet; investigation is incomplete. The branch starts at #3’s delivered branch (PR #29) to reuse the primary-source tools and workflow. If #29 remains unmerged, a PR against upstream main will include that dependency; identify it explicitly and compare the issue-specific commits separately.

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

## Next action

Restore the original pending-line routine in the probe, feed raw text through `0x3ce24` with controlled font metrics/file sinks, and check lazy overflow file creation, complete-source retention, replay and end behavior. Trace macro abort consumption and scene/reset helper callers; then write exact NEWS/NEWSPAPR state transitions. Keep incomplete findings here rather than publishing a partial contract.

---

[Research backlog](issue-backlog.md) · [Current column reference](../ripscrip/3.1-column-text-system.md)
