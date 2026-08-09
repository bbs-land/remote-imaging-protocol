# TODO

## Status snapshot (2026-08-08)

**Everything in the repo is committed on `main`.**

Complete: 1.5x and 2.x Markdown reference editions; 3.x edition rebuilt to mirror the 2.x layout (22-page reconstructed reference with evidence tags, whitepaper conversion at `version/3.x/whitepaper/`, research notes at `version/3.x/research/`); RIPtel 3.1 fully analyzed (script census, help-file extraction, binary formats); assets staged for 1.5x and 3.x with per-file READMEs; `temp/syncterm-missing-feature-rip3.md` gap checklist; Git LFS + `.gitattributes` (LF default, `.rip`/`.ans` and `assets/**` byte-exact); reference repos + binaries in `~/src/rip-tools/` (see CONTRIBUTING); `tools/check-links.py` validates all doc links (0 broken at last run). Rendering guidance in `version/IMPLEMENTATION.md`.

Completed 2026-08-08 (history/licensing/conventions pass):

- **Expanded history** — `version/HISTORY.md`: cited TeleGrafix/product timeline (spec releases 1.50→1.54, RIPterm 2.0/2.20/2.3, RIPtel 3.0/3.1, Searchlight/ProBoard era, domain expiry 2006), Pat Clawson's death (2015, identification caveat noted), Jeff Reeder as the only remaining principal likely to have more information.
- **Licensing details** — `version/RIGHTS.md`: original TeleGrafix "freely licensed" wording quoted, common-law trademark findings, IP-in-limbo status; root `LICENSE` (CC0 1.0) covers all documentation unless noted, original `.txt` specs keep TeleGrafix terms, future libraries will be ISC. Main README licensing section updated to match.
- **Version overview** — `version/README.md`: Wikipedia-informed protocol overview, per-version record status (1.5x fixed history / 2.x partially unknown / 3.x reconstruction), RIPscrip vs RIPterm (dial-up) vs RIPtel (telnet) naming distinction.
- **Future enhancements** — `version/next/` placeholder linked from the main README and `version/IMPLEMENTATION.md` (new "Future directions" section).
- **3.x errata** — SyncTERM's `ESC[2!` resume failure documented as a SyncTERM bug (not a protocol delineation); 2.x-product-era provenance recorded (WAV audio spec'd at 2.0 ALPHA 3, shipped in RIPterm Pro 2.0 per the recovered install; 2.20.01 extended the wire protocol past ALPHA 4).
- **Markdown conventions** — CONTRIBUTING/AGENTS updated: reference-editions rule replaces "faithful conversion", hyphen bullets, no hard word-wraps (soft wrap in editors), Prettier with `.prettierrc` (`proseWrap: never`); all repo Markdown reformatted/unwrapped.
- **Docs layout** — `IMPLEMENTATION.md` moved to `version/IMPLEMENTATION.md` with all links updated.
- **RIPterm 2.0 install interrogated & 2.x assets staged** — the two-disk RIPterm Professional 2.0 install (`~/src/rip-tools/RIPTerm2.0/extracted/`, files dated Feb 1995) catalogued in CONTRIBUTING; `version/2.x/assets/` populated (48 fonts incl. 5 Atech `.FF1` outline fonts + widget `.IMG`s, 95 icons incl. `SHADOW.RIP`, `RIPTERM.WAV`) with per-file READMEs. Key findings folded into the docs: WAV audio, JPEG, SVGA/256-color, and BMP icons all shipped in **2.0 (Jan 1995)**, not 2.2+; the 26-character RIP_EXTENDED_FONT_STYLE block predates 3.0 (SHADOW.RIP); `.FF1` → `.RFF` font lineage confirmed; auto-sense string `RIPSCRIP020000`; `.FF1`/`.RFF` added to LFS (existing `.RFF` files renormalized).

Likely next candidates: techspecs pages for the binary formats; the open **Discuss** questions in the rollup at the bottom; VitePress site scaffold.

---

Planned work for this repository, grouped by area. Working conventions live in [CONTRIBUTING.md](CONTRIBUTING.md). Checked items are done; **Discuss:** items need a decision before the tasks under them can be finalized.

## Specifications (Markdown reference editions)

- [x] Convert RIPscrip 1.54 specification to linked Markdown (`version/1.5x/ripscrip/`, 21 files)
- [x] Convert RIPscrip 2.00 alpha 4 specification to linked Markdown (`version/2.x/ripscrip/`, 22 files)
- [x] Convert the RIPscrip 3.0 technical white paper to linked Markdown (`version/3.x/ripscrip/` files 01–08), plus an editorial reconstructed reference (files 09–11) from SyncTERM `ripper.c` evidence with per-claim citations
- [x] Update the main README Specifications table when the 3.x edition lands
- [ ] Extend the 3.x reconstruction as new evidence is analyzed: the RIP 2 C Library manual PDF, extraction of the RIPtel 3.10 / RIPterm 2.30 installers, and the seven known-but-unimplemented command descriptors (see `version/3.x/ripscrip/10-reconstructed-command-set.md`)
- [ ] Annotate the conversions with errata and clarifications discovered from implementations — as clearly-marked editor's notes, never silent edits
  - [x] First errata pass (2026-08-08): SyncTERM `ESC[2!` resume failure marked as a SyncTERM bug; 2.x-product-era provenance notes (WAV audio, JPEG) added to the 3.x edition
- [ ] Back-fill the unfinished 2.x §2.9 (`[BEGIN REWORD]` placeholder in the source) based on actual implementation behavior (SyncTERM `ripper.c`, icy_parser_core), cited and marked as reconstructed
- [ ] Port the link/anchor checker (currently a scratchpad Python script under tools/check-links.py) into the repo as a Deno script; wire into CI so cross-links stay valid.

## Documentation restructure (direction, 2026-08-03)

Future documentation should **align consistently across versions by feature/subfeature** rather than mirroring each source document's structure (the 3.x whitepaper layout need not be preserved). Goal: each version's docs represent the whole feature space comparably (protocol syntax, drawing, fonts, UI objects, ports/tables, host commands, text variables, file formats), making version deltas legible.

- [x] Restructure the 3.x docs to mirror the 2.x layout: `version/3.x/ripscrip/` is now a 22-file reconstructed protocol reference with per-claim evidence tags; the white-paper conversion moved to `version/3.x/whitepaper/`
- [ ] **Discuss:** whether 1.5x/2.x docs also get feature-aligned companion pages, and how the taxonomy feeds the website layer

## RIPscrip 3.x research

The white paper is woefully incomplete — no command reference exists. Flush out everything else that can be found:

- [ ] Inventory all known 3.0-era material beyond the white paper (RIPtel, WebRunner, TeleGrafix press releases, beta announcements, Usenet/FAQ posts)
- [x] Mine SyncTERM's `ripper.c` (claims "RIP 3.0 compatible") for the actual 3.0 command set, syntax changes, and behaviors; document with citations (→ `version/3.x/ripscrip/09–11`)
- [x] Compare other client sources for any 3.0 handling (all 1.54-only in code; RIPtermJS ships 2.x/3.x docs and RIP 2.0 samples)
- [x] Obtain RIPtel itself and reverse-engineer: 3.1 install extracted to `~/src/rip-tools/artifacts/RIPtel/` and analyzed — 116 authentic 3.0 scripts censused (11 new opcodes incl. the skewed-oval family), RIPSCRIP.HLP string table yielded the full ~90-command inventory and limits, column system + `<<IF>>` macro layer discovered (→ `version/3.x/research/` and `version/3.x/ripscrip/12`)
- [ ] Write up format findings as proper techspecs pages (`.RFF` Atech FastFont, `.maf` MicroANSI fonts, `.BMH`, `RIPSCRIP.RES`/`.DB`) in `version/<v>/techspecs/` per the earliest-version rule
- [ ] Hunt for the "RIPscrip 3.0 protocol specification" and "RIP-2-C 3.0 developer tools" TeleGrafix listed as products (confirmed to have existed via RIPtel help text)

## Technical specifications (`version/<v>/techspecs/`)

Original documentation of binary formats and implementation details. Rule (see CONTRIBUTING.md): full documentation lives in the **earliest** version where a format appears; later versions document only the differences.

### 1.5x

- [ ] Font formats — the stroked/bitmap fonts used by RIPterm (Borland BGI `.CHR` stroked fonts + bitmap font behavior), metrics, and how fonts map to `RIP_TEXT`/`RIP_FONT_STYLE`
- [ ] Icon format (`.ICN`) — full binary layout, expanding on the spec's appendix with implementation detail
- [ ] `.RIP` file conventions — line structure, continuation, embedded ANSI, auto-sensing prologue conventions used by real files
- [ ] MegaNum/base-36 encoding notes and edge cases as implemented
- [ ] Terminal behavior details not fully specified (clipping, write modes, fill pattern semantics) as observed in reference implementations

### 2.x

- [ ] Icon format changes from 1.x (the format changed — document the delta)
- [ ] DIB/BMP support details
- [ ] Audio format support (WAV playback, sound variables)
- [ ] Palette / direct-RGB implementation details

### 3.x

- [ ] Whatever the 3.x research above uncovers (vector fonts, JPEG, MIDI, resolution independence)

## Documentation website

Decisions made: **VitePress** for the site, **Deno** as the runtime for all generation/tooling scripts (`deno task`).

- [ ] Scaffold the VitePress site (config, theme, `deno task` wrappers)
- [ ] Generate sidebar/nav from the existing directory + README structure rather than hand-maintaining it
- [ ] Verify rendering of box-drawing/CP437-derived content, wide GFM tables, and `text`/`c` fenced blocks
- [ ] Landing page drawing from the main README
- [ ] **Discuss:** deployment target (GitHub Pages via Actions? custom domain under bbs.land?) and repo/branch strategy for generated output

## Machine-readable spec & grammar

- [ ] **Discuss:** the source-of-truth form — options: hand-maintained JSON/TOML command tables; a formal EBNF grammar for the wire syntax; or Rust types in a spec crate that export JSON. (These compose — e.g. EBNF for syntax + tables for the command inventory.)
- [ ] Command inventory: every command across 1.54 / 2.0a4 / 3.x with level, code, arguments (types/widths), and version introduced/changed
- [ ] Text variable inventory (same treatment)
- [ ] Grammar for the byte-level syntax: `!|` lines, delimiters, MegaNums, escapes (`\|`, `\\`), line continuation, embedded text
- [ ] Use it to cross-check the Markdown editions (doc ↔ table consistency)

## Reference test corpus

- [ ] **Discuss:** layout — top-level `corpus/` shared by docs/tests/crates, per-version `version/<v>/samples/`, or fixtures inside the Rust crates
- [ ] **Discuss:** encoding-variant extensions — `.rip`/`.ans` are assumed **CP437** (the DOS terminal representation) and stored byte-for-byte; decide on future extensions for other encodings (e.g. `.utf8.rip` / `.utf8.ans`, or others) and how tooling should detect/declare encoding
- [ ] Collect real-world `.RIP` art and BBS screens (art packs, archived BBS distributions), with provenance notes
- [ ] Extract sample icons, fonts, and scripts from original TeleGrafix distributions (see Reference materials)
- [ ] Author targeted conformance scripts per command/feature
- [ ] Golden-output renders (reference images) for regression testing

## Rust libraries (in this repo)

Goal: reusable, wasm-capable crates (wasm and/or cdylib for bindings from other languages).

- [ ] **Discuss:** workspace layout and crate naming (e.g. `crates/` with `ripscrip-parse`, `ripscrip-render`, `ripscrip-wasm`?), and scope sequencing (parser → renderer → terminal integration?)
- [ ] Cargo workspace scaffold
- [ ] Parser crate — target RIPscrip 1.54 first; emit a typed command stream
- [ ] Renderer crate — EGA-faithful rasterization (fills, write modes, fonts)
- [ ] wasm bindings + minimal browser demo; cdylib C ABI for other languages
- [ ] Conformance harness driven by the test corpus and machine-readable spec
- [ ] Study prior art: `icy_tools/crates/icy_parser_core/src/rip/`, SyncTERM `ripper.c`, PabloDraw `Source/Pablo/Formats/Rip/`

## History & ecosystem

- [x] TeleGrafix company/product timeline — `version/HISTORY.md` (2026-08-08): cited release timeline (specs, RIPterm, RIPaint/RIPdraw, RIPtel), company arc through the 2006 domain expiry, and the principals (Clawson, Reeder, Bergman, Hayton)
- [x] Rights/trademark status — `version/RIGHTS.md` (2026-08-08): original legal wording, common-law trademark findings, in-limbo status, repository licensing (CC0 docs / original `.txt` terms / ISC future libraries)
- [x] Decide where it lives: under `version/` (HISTORY.md, RIGHTS.md, README.md overview) rather than a top-level `history/`
- [ ] **Discuss:** remaining scope — a catalog of implementations past and present (clients, BBS packages, doors, editors, libraries) with status; preserved press articles/FAQs/adverts; a gallery of notable RIP scenes; how it all feeds the website
- [ ] Deepen HISTORY.md as sources surface: Boardwatch scans, BBS Dev News #46 full text, authoritative USPTO TSDR trademark check

## Reference materials (`~/src/rip-tools/`)

- [x] Research and clone open-source implementations for local grepping: `sbbs` (SyncTERM), `icy_tools`, `pablodraw`, `RIPtermJS`, `fTelnet`, `qodem` — documented in CONTRIBUTING.md
- [x] Download original runtime binaries for reverse-engineering reference, under `~/src/rip-tools/`, and document each (source URL, version, contents) in CONTRIBUTING.md:
  - [x] RIPterm 1.54 (`RIPterm154/` preservation repo, incl. original ZIP + DOSBox setup) and RIPterm 1.52 (`artifacts/ripterm-1.52/`)
  - [x] RIPterm 2.30 shareware, DOS (`artifacts/ripterm-2.30/RIPT2300.zip` — complete distribution via VOGONS, 2026-08-08; the Wayback `rtrm2300.exe` is truncated and kept for provenance only)
  - [x] RIPtel Visual Telnet 3.10, Win16 (`artifacts/riptel-3.10/`, via Wayback)
  - [x] RIPaint 1.52 (`artifacts/ripaint-1.52/`, includes sample `.RIP` files)
  - [ ] RIPdraw — not yet located in any archive; keep hunting
  - [x] RIP 2 C Library manual PDF (`artifacts/docs/`) — SDK-era 2.x docs
  - [x] Era tools: IconUpDater 2.63, SysMon, ProBoard 2.20d, Searchlight BBS 5.10 (`artifacts/tools|proboard|searchlight/`)
  - [ ] Other era tools worth having (third-party RIP editors, BBS-side RIP doors/menus) as discovered — the archived `ftp.telegrafix.com` index pages on the Wayback Machine list more candidates
- [x] Extract distribution assets into `version/<v>/assets/{fonts,icons,audio}/` with per-directory READMEs: 1.5x (184 icons + 11 fonts from RIPTM154.ZIP), 2.x (95 icons + 48 fonts + `RIPTERM.WAV` from the RIPterm Professional 2.0 install), 3.x (234 icons/demo files + 20 fonts from RIPtel 3.1); byte-exact via `.gitattributes` assets rule
- [x] Populate `version/2.x/assets/` — done 2026-08-08 from the two-disk RIPterm Professional 2.0 install (`~/src/rip-tools/RIPTerm2.0/extracted/`): complete `FONTS\` (10 `.CHR`, 5 Atech `.FF1`, per-resolution `.FNT` + widget `.IMG`s, `DEMO_ONE.EXE`), `ICONS\` (91 `.BMP`, 3 `.BMH`, `SHADOW.RIP`), `RIPTERM.FNT`, and `RIPTERM.WAV`, with per-file READMEs
- [x] Recover the 2.2/2.3-era assets — complete RIPterm 2.30 shareware distribution recovered 2026-08-08 (VOGONS; `artifacts/ripterm-2.30/RIPT2300.zip`, full 251-file set unpacked to `artifacts/ripterm-2.30/extracted/` incl. `FONTS\` and `ICONS\`); deltas vs the 2.0 set staged into `version/2.x/assets/` and the asset READMEs extended (2026-08-08 interrogation)
- [ ] Extract sample scripts into the test corpus (once its layout is decided)
- [ ] Note DOS emulation setup for running them (DOSBox-X config) if/when behavioral testing is needed

## Housekeeping

- [x] Commit the existing work — everything in the repo is committed on `main` (2026-08-08)
- [x] Add `.gitattributes`: LF normalization by default; `.rip`/`.ans` exempt (`-text`, byte-for-byte, CRLF preserved for testing); RIP-era binary formats + `.zip`/`.exe` tracked via Git LFS (case-insensitive)
- [x] Enable Git LFS in the repo (`git lfs install --local`; hooks updated) — contributors need `git lfs install` once per machine, see CONTRIBUTING
- [ ] Verify LFS behavior on the remote (GitHub LFS quota/bandwidth) when the first binary content is pushed; decide whether large `artifacts/`-style binaries belong in-repo at all or stay in `~/src/rip-tools/`
- [x] Licensing decided (2026-08-08, see `version/RIGHTS.md` and root `LICENSE`): all documentation is CC0 1.0 unless noted; the original `.txt` specification texts keep their TeleGrafix terms; future library/code implementations will be ISC
- [x] Markdown conventions adopted (2026-08-08, see CONTRIBUTING.md): hyphen bullets, no hard word-wraps (editors soft-wrap), Prettier with `.prettierrc` (`proseWrap: never`); full-repo reformat applied
- [ ] **Discuss:** overall priority order across the areas above (several can proceed in parallel; nothing has been sequenced yet)

## Open questions (rollup)

Collected from the sections above — each needs a short discussion:

1. Priority/sequencing across areas
2. Test corpus layout
3. Machine-readable spec form (tables / EBNF / Rust-as-source)
4. History & ecosystem remaining scope (implementations catalog, press archive, gallery) — location resolved: `version/`
5. Website deployment target and publishing strategy
6. Rust workspace layout, crate naming, and scope sequence
7. ~~Licensing for original (non-TeleGrafix) content and code~~ — **resolved 2026-08-08**: CC0 docs / original `.txt` terms / ISC libraries (see `version/RIGHTS.md`)
8. Encoding-variant extensions for corpus files (`.rip`/`.ans` = CP437 for now; `.utf8.rip`/`.utf8.ans` or similar for future UTF-8 variants?)
