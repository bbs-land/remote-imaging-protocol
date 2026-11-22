# Issue #130: Real-font metrics and retained TEXTDATA

[Research backlog](issue-backlog.md) · [TEXT_METRIC](../ripscrip/3.0-text-output-and-fonts.md#rip_text_metric)

## Scope and checkpoint

Started 2026-11-22 on `issue-130-text-metric-state` from stable 21728db. [Issue #130](https://github.com/bbs-land/remote-imaging-protocol/issues/130) requests actual font-family width/height formulas and numeric examples, preparation/cache/failure behavior, retained TEXTDATA/reset/backup scope, setter/prompt snapshot timing and byte/codepage boundaries. Adopt #5 prefix/modes/domains/signed reply/empty distinction, #86 ordinary outline width and #124 graphical expansion. RIPscrip 1.54 is outside scope. Explicit portable completion is permitted; no original/client/UI/probe execution or obsolete-client defect investigation.

Initial body SHA-256 `c7705ad304cbf9758e08d1fd49c2376b6ebfbfed3b8bdfe66d771ec0f6b31685`, updated 2026-11-22T01:37:57Z; zero comments. Refresh started 2026-11-22T02:20:24Z finds only #130/#131 uncovered and no newer discussion/inline/review feedback. Complete/publish/consolidate #130 before #131.

## Original sources and bounded findings

Pinned RIPtel 3.1 / RIPscrip 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [mirror](../../../reference/rip-tools.md): 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Base 0x10000000; all addresses below are RVAs. `.text` file offset = RVA−0xc00; other sections use PE mapping. LLVM objdump/Python read unchanged bytes only. Primary ALPHA 4 text SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982` supplies classic size/cell conventions; adopted #5/#86/#124 remain controlling. No original/client/UI/provider/probe execution or bug investigation.

- Handler `0x20371–0x209ce` validates mode/domain before scratch/expansion. Sentinel comparison `0x2040e -> 0x655e2` precedes the ordinary expansion at `0x2044f -> 0x4b0e4` (flags 0x81, limit 1022). At `0x20463 -> 0x65431`, expanded bytes are copied to text-system context +0x32 then +0x4a **before** the font-family branch `0x2047f` and original-empty check `0x20660/0x20668`. Thus a literal-empty outline request clears retained bytes before its no-reply exit; a later preparation failure cannot imply rollback of completed replacement.
- Classic selection `0x2049b -> 0x10f0` precedes metric services. Width `0x1410` uses CHR width bytes and sums before scaling at `0x15b5–0x15fb`. Height `0x1640` reads classic state +0x22 then adds one at `0x166e–0x1674`; loader `0x12ef–0x1306` computes that state as trunc((org_to_cap−org_to_dec)*Yscale). Neither CHR branch swaps dimensions for direction. Existing byte sign-extension/native low-word accumulation do not override the explicit finite unsigned-byte profile.
- Outline call `0x2080d -> 0x40ed0` uses NULL text/zero coordinates and ignores return; width `0x20825 -> 0x3fcd0` and height calls use metric services afterward. Height service `0x3fc70` combines state metric quantities; it does not by itself establish a complete portable typographic/rotated/ink-height meaning. Adopt one explicit geometric height instead of guessing tentative RFF alignment words.
- TEXT_XY compares its raw argument against the same exact sentinel at `0x20d8e/0x20d91`, copying retained bytes at `0x20d9d -> 0x65431`; its non-sentinel ordinary expansion is at `0x20dcb`. This is an additional **consumer sentinel**, not a general TEXTDATA callback. The complete 179 registrations already pinned in #124 contain no TEXTDATA. Do not reserve that ordinary user name globally.
- The buffer is outside STYLE/PORT/ENV records and their table codecs. Read locations show ownership, not a complete native reset promise; cold/reset/backup lifetime below is explicitly chosen. Ordinary T and non-sentinel @ do not become implicit writes to this retained buffer.

## Original font inputs

Original MARIN.RFF (56,526 bytes, SHA-256 `4b0a392baa74a1849b9fabedacc39c1434784370175f7708ea151b851255ed68`) has U=1000, signed global bbox (−78,−916,1059,250), and F advance 556. Use the section/base extraction in [#86](issue-86-outline-advance-policy.md); metrics base 0x212, signed bbox at base+0x16 and advance table pointer at base+0x26. Its vertical span is 1166; typographic zone guesses are unnecessary.

Original TRIP.CHR (16,677 bytes, SHA-256 `831963618a5dbf3562e12c3b4b8bab3ed3879143de3c7cd9293be171a44282db`) has 223 characters from byte 32, metrics (cap,baseline,descender)=(24,0,−7) at 0x88, and unsigned advance bytes A=19/B=20 after the offset table at `0x90+2*223`. CHR size ratio tables at DLL RVAs 0x76078/0x76088 and 0x76098/0x760a8 reproduce 3/5,2/3,3/4,1,4/3,5/3,2,5/2,3,4 for sizes 1..10. These are data reads, not native metric runs.

## Explicit portable metric and state completion

Outline modes 0/2 use exactly #86's shared cached X-width, with per-byte device truncation and final gap, regardless of rotations. Add an unrotated global design-bbox vertical span to that same atomic metric snapshot: h1440=round_half_up((ymax−ymin)*Q*20/U), H=trunc(h1440*Dy/1440). No horizontal-face percentage, shadow/decorations, baseline/justification, glyph ink or S/R rotation changes this height. Modes 1/3 use H, including effective empty text when the original raw argument was nonempty. Invalid/cold outline snapshot yields both width/height zero; preparation failure retains the entire old snapshot.

Classic CHR profile uses unsigned advances, finite wide sums, the documented charsize ratio, and rational reference-device scales D/640 and E/350: complete width trunc(sum(a)*sx), per-byte width trunc(a*sx), height trunc((cap−descender)*sy)+1. Bitmap font zero uses the documented 80×43 cell realization floor(D/80)×floor(E/43), multiplied by integer size. Neither classic direction swaps logical measurements; no outline spacing percentage applies. Failed classic preparation returns zero measurements for this query, retains the outline cache and creates no cross-family stale-font fallback. These formulas/failure handling are explicit completion, not a claim to reproduce all native font-zero/CHR services.

Expand first under #124, commit successful retained bytes, then snapshot live STYLE/font/current ENV WORLD/PORT and device frame together. Provider setters/prompts therefore affect that final snapshot; no partial font state is committed before expansion completes. Outline metric preparation with NULL text may replace the shared snapshot even for an effective empty string, but never draws or advances the graphics cursor. Inactive viewport/zero WORLD prevent outline preparation, retaining old/cold metrics; the no-draw query itself can still reply. Definition protection does not gate read-only measurement. Domain WORLD conversion uses the post-expansion frame and existing signed narrowing; zero WORLD dimensions multiply to zero, while nonpositive device dimensions make conversion invalid/no reply. Classic metrics can still be prepared without an active drawing viewport.

TEXTDATA is interpreter-global bytes, initially empty; completed non-sentinel r expansion replaces it (including literal empty), exact r/@ sentinel reads without expansion/replacement, ordinary T/@ do not assign it. Failed expansion retains the old value; later font/metric failure retains a completed replacement. Full SOFT/drawing/hard reset clears it; individual table resets/selection, viewport changes, header segments without a full reset and table/screen backups/restores do not copy/restore it. Rendering/prompt snapshots are invalidated by scene reset; they cannot restore stale retained text afterward. These lifetime rules are explicit policy, not inferred native allocation state.

Use the raw C-string prefix before first NUL for sentinel/empty classification. Exact case-insensitive `$TEXTDATA$` followed by NUL is the sentinel; extra non-NUL suffix or leading space is not. The literal-empty outline rule applies to an empty prefix, not to nonempty source that expands empty. Outside exact sentinel consumers, TEXTDATA is an ordinary unknown/user name, subject to the existing consumer permissions. Measure normalized original codepage bytes; a separate UTF-8 transport adapter must transcode before counting and cannot turn UTF-8 code units into new RIP per-character positions.

## Validation and reproduction

Published complete family/formula/height, shared preparation/cache, retained-buffer/reset/sentinel and stage/byte rules with real MARIN/TRIP/bitmap success, failure and truncation vectors. Read-only verification passes four pinned hashes (driver, draft and two fonts), eight direct call targets, six critical instruction sequences, all four original ratio arrays, signed RFF/CHR fields and real-font arithmetic, sixteen rotation pairs, signed-domain and NUL/sentinel vectors. Manually reviewed atomic failure retention, no cursor/draw effects, protection/inactivity, post-provider snapshot, reset/backup exclusions, literal/expanded empties and UTF-8 adaptation boundary.

An initial verification used 0x1666e/0x16672 instead of the actual classic service 0x166e/0x1672; corrected the note/check to match the disassembly. Draft CR markers were normalized to one literal backslash in Markdown. These were research/notation corrections, not client observations. Refresh feedback and publish/consolidate separately before #131. No original/client/UI/provider/probe execution or bug investigation.

```sh
objdump -d --start-address=0x100203db --stop-address=0x100204a9 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x10020660 --stop-address=0x10020671 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x100015b5 --stop-address=0x10001625 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

Repository lint, 6,146 links across 253 files and diff checks pass. Refresh started 2026-11-22T02:28:52Z confirms unchanged #130/#131 with zero comments.

## Delivery

[PR #133](https://github.com/bbs-land/remote-imaging-protocol/pull/133), specification 3d7161d, is open/ready and closes exactly #130. It continues #129 with an issue-only comparison from stable 21728db. Consolidate the actual source head by an ordinary merge and exact-tree check before #131. Scoped API request files were removed automatically.

## Final consolidation checkpoint

[PR #133](https://github.com/bbs-land/remote-imaging-protocol/pull/133) actual head **0788745** is normal-merged into fork stable through **9d666f8**, with an identical source tree before workflow notes. The full audit started 2026-11-22T02:52:54Z verifies all 66 current PR heads ancestral, all 67 open RealDeuce issues covered and no newer report/comment/review feedback. See the [backlog merge ledger](issue-backlog.md#resume-here). No active investigation remains; source/policy boundaries, co-author and cleanup requirements persist.

---

[Research backlog](issue-backlog.md) · [TEXT_METRIC](../ripscrip/3.0-text-output-and-fonts.md#rip_text_metric)
