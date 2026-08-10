# DONE - completed work log

Completed items moved out of [TODO.md](TODO.md), grouped by area (newest first within each area). Convention: when a TODO item completes, it moves here with its date and summary - TODO.md stays active-only (see [CONTRIBUTING.md](CONTRIBUTING.md)).

## Specifications (Markdown reference editions)

- [x] Convert RIPscrip 1.54 specification to linked Markdown (`version/1.54/ripscrip/`, 21 files)
- [x] Convert RIPscrip 2.00 alpha 4 specification to linked Markdown (`version/2.30/ripscrip/`, 22 files)
- [x] Convert the RIPscrip 3.0 technical white paper to linked Markdown (`version/3.1/ripscrip/` files 01-08), plus an editorial reconstructed reference (files 09-11) from SyncTERM `ripper.c` evidence with per-claim citations
- [x] Update the main README Specifications table when the 3.x edition lands
- [x] First errata pass (2026-08-08): SyncTERM `ESC[2!` resume failure marked as a SyncTERM bug; 2.x-product-era provenance notes (WAV audio, JPEG) added to the 3.x edition - the errata-annotation effort itself continues in TODO

## Documentation restructure

**1.54 fidelity audit + punctuation normalization (2026-08-10).** Nine read-only audits checked every command, text variable, table row and behavioral claim in `version/1.54/{ripscrip,techspecs}/` against the canonical `version/1.54/text/RIPScrip-1.54.txt`, the shipped RIPterm 1.54 install (`strings RIPTERM.EXE`, `RIPTERM.DOC`, `WHATSNEW.DOC`, 219 real icon files), RIPtermJS, SyncTERM, and the 2.00 ALPHA 4 spec (to prove forward-porting).

**Result: zero hallucinated and zero forward-ported features.** 53/53 commands in 9.0 map one-to-one onto the spec's definition blocks; 74/74 pre-defined text variables match bidirectionally; no 2.x material (`$IFS$`, `$TERMINFO$`, UltraNums, FastFont, `.BMP`/`.BMH`/`.BMM`, MicroANSI, JPEG, WAV) is presented as 1.54 anywhere. Notably `$SAVE$`/`$RESTORE$` are genuine 1.54, not 2.x as suspected.

16 real defects found and fixed, all "true but unmarked/miscited" rather than fabrication:

- `$RIPVER$` silently corrected `RIPSCRIP015300` → `RIPSCRIP015410` (right, per `RIPTERM.EXE`) but unmarked and contradicting its own heading - editor's note added; 9.2's dangling "see the editor's note below" now links to it.
- 9.2 gained two `WHATSNEW.DOC`-attested strings: `RIPSCRIP015005` (v1.50, pre-vendor-code) and `RIPSCRIP015311` (v1.53, recording that v1.53.00 "was incorrectly responding as Vendor Code #2 (same as Qmodem Pro)").
- 6.1's icon-formula note claimed verification against "the RIPterm 1.54 **and 2.0** distributions" - false: RIPterm 2.0 ships **zero** `.ICN`/`.HIC`/`.MSK` (92 `.BMP` instead). Re-censused to 219 files (212+3+4), zero formula mismatches.
- `RIP_ENTER_BLOCK_MODE`'s 4-digit field was glossed as "file size"; the spec's layout is `mode:1 proto:1 file_type:2 res:4` - it is the reserved `res` field, and 1.54 block mode has no file-size field.
- `RIP_FILE_QUERY` mode 02 returns size only, not size+date+time.
- Write-mode scope corrected to the spec's own per-command matrix (text, lines, rectangles, polygons/polylines, Beziers - circles/ovals/arcs/bars/pixels/flood fills are `Uses Write Mode: NO`).
- Citation fixes: `char_height()` → `font_height()`, `BGI.fontFiles` (nonexistent) → `BGI.fontFileList`, corpus example `@1P65` → `@1O65`, "`!|W0` opens" → "appears in the opening line of".
- `$RESTALL$` (a spec typo appearing once, with no definition and absent from `RIPTERM.EXE`) corrected to `$RESTOREALL$` with the erratum recorded.
- `.MSK` masks and 5.3's worked examples marked as era-attested/editorial - both are genuine but appear nowhere in the spec text, so a reader checking only the spec would wrongly call them fabrications.
- 9.1's `$>file$` gloss ("`.RIP` extension appended if omitted") retracked to the spec's actual wording, "with or without the .RIP extension".

**Verbatim `text/` documents (owner edits, 2026-08-10):** the TeleGrafix contact block was redacted from `version/2.30/text/RIPScrip-2.0-alpha-4.txt` (13 lines), and `version/1.54/text/RIPScrip-1.54.txt` was re-encoded to valid UTF-8 - it was the last document still holding raw CP437 high bytes (30 lines of box-drawing art in the Radio Button template examples, which the CP437→UTF-8 normalization in CONTRIBUTING.md had missed). All three `text/` documents are now valid UTF-8, closing the hazard where a UTF-8 editor would silently replace those bytes with U+FFFD.

**Punctuation:** em dashes (2,203), en dashes (398) and U+2212 minus signs (90) in Markdown converted to plain ASCII hyphens - 2,691 substitutions across 142 files. `version/*/text/` excluded (verbatim history, and they contained no em/en dashes anyway). The minus signs were all arithmetic (`width-1`, `-7`, `fileSize - 48`, "SyncTERM returns -1"), so ASCII is also the more consistent rendering. The dash conversion broke 45 anchors, because headings containing a dash slugged to `--` and now slug to `---`; all 45 were repaired programmatically via the repo's own `slugify()`. Deliberately left alone: `…` (246), `×` (561, as in 640×350), `·` (822, nav separators), `→` (107), `≤` (22). Link checker: 0 bad of 3,844 links across 148 files; Prettier clean.

**Prelude/introduction trim pass (2026-08-10).** Every page under `version/{1.54,2.30,3.1,baseline}/{ripscrip,techspecs}/` opened with an "on this page" prelude - the block between the top nav line and the first `##` heading. Across 122 pages these totalled ~18,000 words, many of them 150-360-word single paragraphs restating the page before the reader reached it. Trimmed to **11,755 words (-35%)**; 116 files changed, 451 insertions / 218 deletions, link checker clean throughout.

Three prelude forms, chosen per page: a **one-sentence scope line** (default, subject stated directly - no "This page covers…", of which zero remain); **scope line + category anchor list** for long pages; **scope line + one italic pointer line** where cross-version lineage needed to stay discoverable. Depth varies deliberately - pages whose "prelude" is actually original TeleGrafix prose (`1.54/5.2`, `1.54/5.3`) were barely touched, and six pages were verified as needing **no change at all** (`1.54/9.2`, `2.30/9.2`, `3.1/9.2`, the `2.30/techspecs` README and its `4.0` delta, `2.30/techspecs/3.5`).

Rules that governed the pass: nav lines, H1s and the 3.x `_Reconstructed edition_` notes untouchable (all 33 verified intact); no cross-link dropped unless verified present elsewhere on the page; evidence tags, version tags, notation legends, normative rules and corpus-attestation findings treated as **content** - relocated into the section that owns them, never deleted; only roadmap/reading-order prose deleted outright.

Corrections made along the way, beyond the trimming:

- `1.54/1.2` had gained "the **two** numeric foundations of RIPscrip 1.54" - 1.54 has only MegaNums; UltraNums and the Base Math selector arrive in 2.x. Corrected.
- `2.30/2.0` and `3.1/2.0` claimed direct-RGB values are "always four-digit UltraNums", contradicting RIP_COLOR's own 1-4-digit table on the same pages. Dropped.
- `1.54/9.3`, `2.30/9.3` and `3.1/9.3` scoped themselves as "everything that flows terminal → host", omitting the `## Host-to-terminal ANSI control` section (and, on 3.1, mislabelling its direction). Rewritten to name both directions.

Method: per chapter-sized group, a **sonnet** and an **opus** agent proposed independently; a **fable** agent then arbitrated against the live files and made the edit. The arbiters repeatedly caught proposal errors in both directions - links claimed present in the body that were not (and vice versa), an invented scope claim, and three cases of a proposal silently dropping evidence-tagged or version-tagged content.

**COMPLETE (2026-08-09).** The direction (2026-08-03) and the decisions from the 2026-08-08 interview, as delivered:

- **Two-layer numbered hierarchy ordered by learning concerns** - `1.x` fundamentals (intro → protocol/command hierarchy → math/coordinates → world view → terminal/ANSI view), `2.x` drawing, `3.x` text, `4.x` media and interactive objects, `5.x` host interaction, `6.x` authoring, `7.x` ports/tables/backup areas, reference pinned at chapter `9` so it aligns across versions. Flat numbered files (`1.0-introduction.md`, …), so version deltas read section-by-section.
- **Audience split** - `ripscrip/` for the content creator (language semantics, no binary internals); `techspecs/` for the implementer (binary layouts, parser edge cases, rendering behavior); wire syntax layered across both and cross-linked, never duplicated.
- **Self-contained vs delta** - each version's `ripscrip/` stands alone with prior-generation content backfilled in; `techspecs/` stay delta-based per the earliest-version rule.
- **Pinned-release directories** - `version/{1.54,2.30,3.1}/`, named for each generation's definitive release; the `version-transition/` staging area is retired.
- **Shared glossary** - spec-first canonical terms with modern aliases, final home `version/GLOSSARY.md`.

Where it is written down: [version/README.md](version/README.md) ("How the docs are organized") and [CONTRIBUTING.md](CONTRIBUTING.md).

- [x] **Promotion into `version/`** (2026-08-09): the six staged trees replaced the old editions as `version/{1.54,2.30,3.1}/{ripscrip,techspecs}/` (dirs renamed from `1.5x`/`2.x`/`3.x` to the definitive releases), the glossary landed at `version/GLOSSARY.md`, and `version-transition/` was retired. Repo-wide link rewrite: 502 broken links repaired in three passes - renamed path segments and collapsed `../../../version/…` depth (96 files), 174 links retargeted from pre-restructure page names by resolving each anchor to the numbered page that now carries the heading (so e.g. `#rip_button` lands on `4.3-buttons.md`, not a generic index), then the version-root docs, three orphaned anchors and one code-span path by hand. The Markdown white-paper edition was retired in the same pass (the verbatim `3.1/text/` document remains and all references point there). Verified 0 bad of 3,862 links across 147 files, Prettier clean
- [x] **Version READMEs finalized** (2026-08-09): `version/README.md` rewritten to carry the structure and organizing principles (layout tree, numbering, audience split, self-contained vs delta, format-first techspecs, glossary), version table relabelled 1.54/2.30/3.1, root `README.md` layout block corrected; each tree README gained its source-document identity and provenance carried over from the old editions (spec revision and date, definitive release, record status), and the techspecs READMEs gained **Primary sources** sections
- [x] Restructure the 3.x docs to mirror the 2.x layout: `version/3.1/ripscrip/` is now a 22-file reconstructed protocol reference with per-claim evidence tags; the white-paper conversion moved to `version/3.1/whitepaper/` _(that Markdown edition was later retired in the promotion pass - only the verbatim text under `version/3.1/text/` remains)_
- [x] ~~**Discuss:** whether 1.5x/2.x docs also get feature-aligned companion pages~~ - superseded by the numbered-restructure direction (2026-08-08); the taxonomy feeds the website sidebar generation
- [x] Draft the proposed TOC outlines (2026-08-08): `version-transition/{v1.54,v2.30,v3.1}/{ripscrip,techspecs}/README.md` - numbered a.b outlines with current-file/section tracking references, pending review rounds (see TODO)
- [x] Stub the shared glossary (2026-08-08): `version-transition/GLOSSARY.md` seeded with the display-model and protocol terms, spec-first canonical names with aliases
- [x] **Content migration into `version-transition/`** (2026-08-08, six agents + sub-waves): all six trees built per the approved outlines - v1.54 ripscrip (27 pages, thematic regroup of the 21-page edition + reference tables) and techspecs (8, incl. the new connection-directory model); v2.30 ripscrip (30 pages, ~11,900 lines, self-contained with v1.54 backfill merged) and techspecs (9, incl. the stream-conventions research); v3.1 ripscrip (31 pages preserving the reconstruction's evidence tags, incl. the 116-row 9.0 command inventory) and techspecs (4, incl. the RIPtel host-directory delta). Audience split, encoding-basics-creator-side, flood-fill removal notes, `$BLIP$`, key/value persistence abstraction, and case-insensitive storage all applied. Final verification: 0 bad of 5,727 links across 237 files, Prettier clean. `version/` untouched; awaiting user review, then promotion

- [x] **Contents pages finalized** (2026-08-09): the seven `version-transition/` READMEs rewritten from drafting outlines into real tables of contents - the `_from:_`/`_backfill:_` migration-tracking references to the old `version/<v>/` pages removed (~210 links), every entry title aligned to its page's actual H1, stale scope text refreshed (FastFont glyph outlines now decoded; v3.1 6.2 "verify during migration" resolved), techspecs H1s dropped "(proposed outline)", and the transition README's structure/phase sections brought current. Verified: every page listed exactly once with no phantom entries, 0 bad of 5,605 links across 241 files, Prettier clean
- [x] Review round(s) on the drafted TOC outlines in `version-transition/` (2026-08-08/09): numbering, grouping and naming iterated with the user across several rounds (chapter-2 reordering, Text as its own chapter, tone/sound split from audio files, asset delivery & storage sections, encoding basics creator-side, reference pinned at chapter 9), then the outlines retired into Contents pages

## RIPscrip 3.x research

- [x] Mine SyncTERM's `ripper.c` (claims "RIP 3.0 compatible") for the actual 3.0 command set, syntax changes, and behaviors; document with citations (→ `version/3.1/ripscrip/09-11`)
- [x] Compare other client sources for any 3.0 handling (all 1.54-only in code; RIPtermJS ships 2.x/3.x docs and RIP 2.0 samples)
- [x] Obtain RIPtel itself and reverse-engineer: 3.1 install extracted to `~/src/rip-tools/artifacts/RIPtel/` and analyzed - 116 authentic 3.0 scripts censused (11 new opcodes incl. the skewed-oval family), RIPSCRIP.HLP string table yielded the full ~90-command inventory and limits, column system + `<<IF>>` macro layer discovered (→ `version/3.1/research/` and `version/3.1/ripscrip/12`)
- [x] Write up format findings as proper techspecs pages (2026-08-08): `.RFF`/`ATF.CFG` in `version/2.30/techspecs/fastfont-fonts.md` + 3.x delta `rff-additions.md`; `.maf` in `microansi-fonts.md` + delta `maf-fonts.md` (directory records corrected to 60 bytes/name[36]; font subrecords are 50-byte headers + 255 glyphs - no truncation after all); `.BMH` in `icon-formats.md`; the RES/DB/HLP container decodes (help-resource format fully decoded: sparse ID-indexed offset table + CP437 strings) live in `version/3.1/research/riptel-resource-containers.md` - client packaging, not spec surface, so kept out of techspecs per discussion

## Technical specifications (`version/<v>/techspecs/`)

- [x] **Generation-naming adjustment (phase 3b)** (2026-08-09, three Sonnet agents): refined convention applied - pinned names for spec titles ("RIPscrip 2.30 -"/"RIPscrip 3.1 -" H1s) and specific releases/products/wire facts; family names (2.x/3.x) for generation-level references. ~480 conversions: v1.54 forward refs → 2.x; v2.30 generic generation prose + two page retitles ("Writing 2.x .RIP Files", "2.x Stream Conventions") → 2.x, forward refs → 3.x; v3.1 generic refs → 3.x (~330 edits, subheading renames with anchors fixed, the 3.0-identity/driver artifacts deliberately pinned); GLOSSARY introduction markers restored to introducing releases (2.0+); borderline citation-vs-generic calls documented in the agent reports. Verified 0 bad of 5,816 links, Prettier clean
- [x] **Pre-review QA sweep of version-transition/** (2026-08-08/09): (Q1) version-designation normalization - ~250 edits across ~90 files: trees self-identify as 1.54/2.30/3.1, prior-generation refs normalized to released versions, "2.3"→"2.30" mislabels expanded, H1s retitled, spec-lineage/quotes/wire strings/paths preserved; (Q2, Sonnet) text-fidelity sanity check vs the original 1.54 and ALPHA 4 spec texts - ~48/53 and 16/95 command entries verified word-for-word, all commands and variables complete, one dropped index row fixed (RIP_PLAY_AUDIO in v2.30 9.0), zero contradictions, `$RIPVER$` self-contradiction editor's note added. Verified 0 bad links, Prettier clean
- [x] **Chapter-9 reference alignment completed** (2026-08-09): 9.2-versions.md and 9.3-host-command-reference.md now exist in all three trees (9.0 commands · 9.1 variables · 9.2 versions · 9.3 host/control reference)
- [x] **FastFont glyph-outline encoding DECODED** (2026-08-08 - apparently the first published decode; no prior public documentation or decoder exists): glyph records are self-describing - u16 tag, s16 bbox (y-down, 1000/em), path/tail lengths, then an opcode stream (`00` moveto, `01` lineto, `02` cubic Bézier, s16 absolute coords) with implicit contour closure; offsets 224×u32 @base+0x20C, widths 224×u16 @base+0x4C. Verified: strict parser passes 2,905/2,912 glyph records across all 13 fonts, all 3,157 contours close exactly, ASCII renders are unmistakably correct letterforms. Bonus: SYMBOL ships corrupted tail glyphs (byte-identical FF1/RFF) and has NO kerning table (trailer doc corrected); COBB is monospaced. `.FF1`/`.RFF` fonts are now fully software-rasterizable. Docs: `version-transition/v2.30/techspecs/3.2-fastfont-fonts.md` (full layout) + v3.1 delta; evidence trail `temp/fastfont-outline-research.md`; working parser `temp/fastfont_parse.py`
- [x] **Version identification reference pages** (2026-08-08, user request): `ripscrip/9.2-versions.md` in all three trees - every known `RIPSCRIPxxyyvs` string with provenance (1.x: `015410` binary+spec+SyncTERM, `015400` DOC sample, `015300` stale 1.53 doc example; 2.x: `020000` byte-verified in the 2.0/2.2x/2.30 engines; 3.x: `03000` HLP FAQ, `030001` SyncTERM, RIPtel wire reply unrecoverable), vendor-code tables, and an editor's note on 1.54 spec's own `$RIPVER$` self-contradiction; v1.54's host-command reference renumbered 9.2→9.3 to keep 9.2 aligned across versions
- [x] **2.x `.RIP` stream-convention delta** (2026-08-08): written as `version-transition/v2.30/techspecs/1.0-stream-conventions-delta.md` from a full byte survey of all 37 shipped 2.x-era scripts - SOH `\x01|*` opener in 18/36 (spec basis: 2.x protocol rule 12), prologue/epilogue families, CRLF-only, a 104-byte raw text line (parsers must not clamp at 80), RIP_QUERY's literal-ESC command byte, heavy `\` continuation traffic; SHADOW.RIP `|k00` strip corrected to 160→10 (assets README fixed accordingly)

**Completed 2026-08-08** - full build-out of `version/{1.5x,2.x,3.x}/techspecs/` (21 pages) and the new `version/baseline/techspecs/` (ANSI/VT-x baseline references). Conventions for these pages (format-first/software-only, no VESA/driver detail, no `ripscrip/` duplication) are codified in CONTRIBUTING.md.

### 1.5x

- [x] Font formats (2026-08-08): `bgi-stroked-fonts.md` (`.CHR` headers/opcodes decoded, size→scale ratio table - size 4 is 1:1) and `bitmap-fonts.md` (`RIPTERM.FNT` container fully reverse-engineered: 5 CP437 charsets × 255 glyphs, 42-byte directory entries)
- [x] Icon format (2026-08-08): `icon-format.md` - `.ICN`/`.MSK`/`.HIC` layout with worked decode; size formula `4 + h·4·⌈w/8⌉ + 2` verified against all 191 shipped files (spec's "one trash byte" corrected to two; editor's notes added to the spec page and icons README)
- [x] `.RIP` file conventions (2026-08-08): `rip-file-format.md` - line structure, continuation, escaping, ANSI mixing, prologue conventions, the RIP_LOAD_ICON optional-`res`-field wild-data hazard
- [x] MegaNum encoding (2026-08-08): `meganum-encoding.md` - digit set, widths, implemented edge cases (lowercase accepted, `-` = digit 0, early termination on `|`)
- [x] Terminal behavior (2026-08-08): `terminal-behavior.md` - clipping, write/put modes, line/fill pattern semantics, resets (BGI-inherited behaviors marked where unverified against RIPterm)

### 2.x

- [x] Icon format changes from 1.x (2026-08-08): `icon-formats.md` - `.BMP`/`.BMH` delta (shipped icons are full BMPs despite the spec's bare-DIB description; `.BMH` = plain BMP pressed state; `.BMM` never shipped, format marked inferred)
- [x] DIB/BMP support details (2026-08-08): folded into `icon-formats.md` incl. the writer convention (sizeImage=0, XPelsPerMeter=width) and the `biCompression` erratum
- [x] JPEG images (2026-08-08): `jpeg-images.md` - baseline JFIF profile of all shipped content, decoder envelope from embedded IJG error strings, scaling/aspect/palette rendering semantics
- [x] Audio format support (2026-08-08): `audio.md` - WAVE (PCM) interchange format, RIFF layout, playback semantics; RIPTERM.WAV decode (PCM mono 8-bit 11,127 Hz); no shipped script uses `!|1w`
- [x] Palette / direct-RGB (2026-08-08): `palette-rgb.md` - resolution/color tiers as shipped, 1024×768/256 ceiling, unexposed 1280×1024 tier evidenced in the 2.30 EXE + MicroANSI/IMG assets
- [x] Also (2026-08-08): `fastfont-fonts.md` (`.FF1` + `.RFF` fully decoded - 54-byte FF1 header, u16→u32 trailer-offset widening as the likely reformat motive, byte-identical glyph payloads, ATF.CFG structure), `microansi-fonts.md` (container decoded: u32 directory @0x2A, 60-byte resolution records incl. unshipped 1280×1024 set, MAF glyph-art revision), `ui-resources.md` (`.IMG` widget format decoded: 12-byte header, plane-sequential pre-shifted EGA planes; system `.FNT` partially decoded)

### 3.x

- [x] 3.x techspecs written as deltas (2026-08-08, `version/3.1/techspecs/`): `rff-additions.md` (BRUSH/EUREKA/OAKLAND decoded; upper-case style suffixes prove a separate converter run; no copyright trailer; BRUSH's >64 KiB trailer confirms the u16→u32 widening motive), `maf-fonts.md` (RIPscrip.maf: 6 resolutions incl. windowed-size sets, third glyph-art revision). The RES/DB/HLP container decodes were relocated to `version/3.1/research/riptel-resource-containers.md` (client packaging, not spec surface). JPEG is documented at its earliest appearance in `version/2.30/techspecs/jpeg-images.md`; MIDI/sequenced music confirmed to have never materialized in any spec or product

### baseline (`version/baseline/techspecs/`)

- [x] ANSI/VT-x support as documented and shipped in RIPterm/RIPtel (2026-08-08, `ansi-vt-support.md`): 1.54's RIPTERM.DOC Appendix B sequence table reproduced; 2.x four-toggle emulation (ANSI/RIPscrip/Doorway/VT-102); RIPtel per-bookmark ANSI/VT-102; auto-sense responses `RIPSCRIP015410`/`020000`/`03000…` with binary evidence
- [x] Modern reference points (2026-08-08, `modern-terminal-reference.md`): SyncTERM/icy_term feature sets with a per-feature historical-evidence column - ANSI music, sixel, 256-color SGR, xterm mouse, bracketed paste, OSC 8, UTF-8 all confirmed absent from every RIPterm/RIPtel document and binary examined; Doorway mode and VT-102 are the historically evidenced ones

### Decisions

- Dropped after discussion (2026-08-08): decoding the `RIPTERM.RES`/`RIPSCRIP.RES` container directories - a detail of the prior software's packaging, not part of the specification surface an alternative implementation needs. `RIPSCRIP.DB` likewise abstracted out of the spec docs: persistence is documented as a plain key/value store (name ≤ 20 chars → value ≤ 255 chars); the byte decodes live in `version/3.1/research/riptel-resource-containers.md`

## History & ecosystem

- [x] TeleGrafix company/product timeline - `version/HISTORY.md` (2026-08-08): cited release timeline (specs, RIPterm, RIPaint/RIPdraw, RIPtel), company arc through the 2006 domain expiry, and the principals (Clawson, Reeder, Bergman, Hayton)
- [x] Rights/trademark status - `version/RIGHTS.md` (2026-08-08): original legal wording, common-law trademark findings, in-limbo status, repository licensing (CC0 docs / original `.txt` terms / ISC future libraries)
- [x] Decide where it lives: under `version/` (HISTORY.md, RIGHTS.md, README.md overview) rather than a top-level `history/`

## Reference materials (`~/src/rip-tools/`)

- [x] Research and clone open-source implementations for local grepping: `sbbs` (SyncTERM), `icy_tools`, `pablodraw`, `RIPtermJS`, `fTelnet`, `qodem` - documented in CONTRIBUTING.md
- [x] Download original runtime binaries for reverse-engineering reference, under `~/src/rip-tools/`, documented in CONTRIBUTING.md (RIPdraw and further era tools still open in TODO):
  - [x] RIPterm 1.54 (`RIPterm154/` preservation repo, incl. original ZIP + DOSBox setup) and RIPterm 1.52 (`artifacts/ripterm-1.52/`)
  - [x] RIPterm 2.30 shareware, DOS (`artifacts/ripterm-2.30/RIPT2300.zip` - complete distribution via VOGONS, 2026-08-08; the Wayback `rtrm2300.exe` is truncated and kept for provenance only)
  - [x] RIPtel Visual Telnet 3.10, Win16 (`artifacts/riptel-3.10/`, via Wayback)
  - [x] RIPaint 1.52 (`artifacts/ripaint-1.52/`, includes sample `.RIP` files)
  - [x] RIP 2 C Library manual PDF (`artifacts/docs/`) - SDK-era 2.x docs
  - [x] Era tools: IconUpDater 2.63, SysMon, ProBoard 2.20d, Searchlight BBS 5.10 (`artifacts/tools|proboard|searchlight/`)
- [x] Extract distribution assets into `version/<v>/assets/{fonts,icons,audio}/` with per-directory READMEs: 1.5x (184 icons + 11 fonts from RIPTM154.ZIP), 2.x (95 icons + 48 fonts + `RIPTERM.WAV` from the RIPterm Professional 2.0 install), 3.x (234 icons/demo files + 20 fonts from RIPtel 3.1); byte-exact via `.gitattributes` assets rule
- [x] Populate `version/2.30/assets/` - done 2026-08-08 from the two-disk RIPterm Professional 2.0 install (`~/src/rip-tools/RIPTerm2.0/extracted/`): complete `FONTS\` (10 `.CHR`, 5 Atech `.FF1`, per-resolution `.FNT` + widget `.IMG`s, `DEMO_ONE.EXE`), `ICONS\` (91 `.BMP`, 3 `.BMH`, `SHADOW.RIP`), `RIPTERM.FNT`, and `RIPTERM.WAV`, with per-file READMEs
- [x] Recover the 2.2/2.3-era assets - complete RIPterm 2.30 shareware distribution recovered 2026-08-08 (VOGONS; `artifacts/ripterm-2.30/RIPT2300.zip`, full 251-file set unpacked to `artifacts/ripterm-2.30/extracted/` incl. `FONTS\` and `ICONS\`); deltas vs the 2.0 set staged into `version/2.30/assets/` and the asset READMEs extended (2026-08-08 interrogation)

## Housekeeping

- [x] Commit the existing work - everything in the repo is committed on `main` (2026-08-08)
- [x] Add `.gitattributes`: LF normalization by default; `.rip`/`.ans` exempt (`-text`, byte-for-byte, CRLF preserved for testing); RIP-era binary formats + `.zip`/`.exe` tracked via Git LFS (case-insensitive)
- [x] Enable Git LFS in the repo (`git lfs install --local`; hooks updated) - contributors need `git lfs install` once per machine, see CONTRIBUTING
- [x] Licensing decided (2026-08-08, see `version/RIGHTS.md` and root `LICENSE`): all documentation is CC0 1.0 unless noted; the original `.txt` specification texts keep their TeleGrafix terms; future library/code implementations will be ISC
- [x] Markdown conventions adopted (2026-08-08, see CONTRIBUTING.md): hyphen bullets, no hard word-wraps (editors soft-wrap), Prettier with `.prettierrc` (`proseWrap: never`); full-repo reformat applied
- [x] Open-question resolved: licensing for original (non-TeleGrafix) content and code - CC0 docs / original `.txt` terms / ISC libraries (see `version/RIGHTS.md`)

## Earlier milestone snapshot (through 2026-08-08 morning)

Complete: 1.5x and 2.x Markdown reference editions; 3.x edition rebuilt to mirror the 2.x layout (22-page reconstructed reference with evidence tags, whitepaper conversion at `version/3.1/whitepaper/`, research notes at `version/3.1/research/`); RIPtel 3.1 fully analyzed (script census, help-file extraction, binary formats); assets staged for 1.5x and 3.x with per-file READMEs; `temp/syncterm-missing-feature-rip3.md` gap checklist; Git LFS + `.gitattributes` (LF default, `.rip`/`.ans` and `assets/**` byte-exact); reference repos + binaries in `~/src/rip-tools/` (see CONTRIBUTING); `tools/check-links.py` validates all doc links. Rendering guidance in `version/IMPLEMENTATION.md`.

History/licensing/conventions pass (2026-08-08):

- **Expanded history** - `version/HISTORY.md`: cited TeleGrafix/product timeline (spec releases 1.50→1.54, RIPterm 2.0/2.20/2.30, RIPtel 3.0/3.1, Searchlight/ProBoard era, domain expiry 2006), Pat Clawson's death (2015, identification caveat noted), Jeff Reeder as the only remaining principal likely to have more information.
- **Licensing details** - `version/RIGHTS.md`: original TeleGrafix "freely licensed" wording quoted, common-law trademark findings, IP-in-limbo status; root `LICENSE` (CC0 1.0) covers all documentation unless noted, original `.txt` specs keep TeleGrafix terms, future libraries will be ISC. Main README licensing section updated to match.
- **Version overview** - `version/README.md`: Wikipedia-informed protocol overview, per-version record status (1.5x fixed history / 2.x partially unknown / 3.x reconstruction), RIPscrip vs RIPterm (dial-up) vs RIPtel (telnet) naming distinction.
- **Future enhancements** - `version/next/` placeholder linked from the main README and `version/IMPLEMENTATION.md` (new "Future directions" section).
- **3.x errata** - SyncTERM's `ESC[2!` resume failure documented as a SyncTERM bug (not a protocol delineation); 2.x-product-era provenance recorded (WAV audio spec'd at 2.0 ALPHA 3, shipped in RIPterm Pro 2.0 per the recovered install; 2.20.01 extended the wire protocol past ALPHA 4).
- **Markdown conventions** - CONTRIBUTING/AGENTS updated: reference-editions rule replaces "faithful conversion", hyphen bullets, no hard word-wraps (soft wrap in editors), Prettier with `.prettierrc` (`proseWrap: never`); all repo Markdown reformatted/unwrapped.
- **Docs layout** - `IMPLEMENTATION.md` moved to `version/IMPLEMENTATION.md` with all links updated.
- **RIPterm 2.0 install interrogated & 2.x assets staged** - the two-disk RIPterm Professional 2.0 install (`~/src/rip-tools/RIPTerm2.0/extracted/`, files dated Feb 1995) catalogued in CONTRIBUTING; `version/2.30/assets/` populated (48 fonts incl. 5 Atech `.FF1` outline fonts + widget `.IMG`s, 95 icons incl. `SHADOW.RIP`, `RIPTERM.WAV`) with per-file READMEs. Key findings folded into the docs: WAV audio, JPEG, SVGA/256-color, and BMP icons all shipped in **2.0 (Jan 1995)**, not 2.2+; the 26-character RIP_EXTENDED_FONT_STYLE block predates 3.0 (SHADOW.RIP); `.FF1` → `.RFF` font lineage confirmed; auto-sense string `RIPSCRIP020000`; `.FF1`/`.RFF` added to LFS (existing `.RFF` files renormalized).
- **RIPterm 2.30 recovery & interrogation** - complete shareware distribution recovered via VOGONS; 2.2/2.3-era asset deltas staged; all docs corrected to reflect the 2.x line as DOS-only; `version/DOWNLOADS.md` added cataloguing the files.bbs.land mirror; Test Drive 2.00.01/2.10/2.11 releases evidenced and added to HISTORY.md
