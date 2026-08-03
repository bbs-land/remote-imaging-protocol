# TODO

Planned work for this repository, grouped by area. Working conventions live in
[CONTRIBUTING.md](CONTRIBUTING.md). Checked items are done; **Discuss:** items
need a decision before the tasks under them can be finalized.

## Specifications (faithful Markdown editions)

- [x] Convert RIPscrip 1.54 specification to linked Markdown (`version/1.5x/ripscrip/`, 21 files)
- [x] Convert RIPscrip 2.00 alpha 4 specification to linked Markdown (`version/2.x/ripscrip/`, 22 files)
- [ ] Convert the RIPscrip 3.0 technical white paper to linked Markdown (`version/3.x/ripscrip/`)
- [ ] Update the main README Specifications table when the 3.x edition lands (currently marked *text only for now*)
- [ ] Annotate the conversions with errata and clarifications discovered from
      implementations — as clearly-marked editor's notes, never silent edits
- [ ] Back-fill the unfinished 2.x §2.9 (`[BEGIN REWORD]` placeholder in the
      source) based on actual implementation behavior (SyncTERM `ripper.c`,
      icy_parser_core), cited and marked as reconstructed
- [ ] Port the link/anchor checker (currently a scratchpad Python script) into
      the repo as a Deno script; wire into CI so cross-links stay valid

## RIPscrip 3.x research

The white paper is woefully incomplete — no command reference exists. Flush
out everything else that can be found:

- [ ] Inventory all known 3.0-era material beyond the white paper (RIPtel,
      WebRunner, TeleGrafix press releases, beta announcements, Usenet/FAQ posts)
- [ ] Mine SyncTERM's `ripper.c` (claims "RIP 3.0 compatible") for the actual
      3.0 command set, syntax changes, and behaviors; document with citations
- [ ] Compare other client sources for any 3.0 handling
- [ ] Obtain RIPtel itself and any 3.0-era SDK/developer docs (see Reference
      materials below); reverse-engineer as needed
- [ ] Write up findings in `version/3.x/techspecs/`

## Technical specifications (`version/<v>/techspecs/`)

Original documentation of binary formats and implementation details. Rule
(see CONTRIBUTING.md): full documentation lives in the **earliest** version
where a format appears; later versions document only the differences.

### 1.5x

- [ ] Font formats — the stroked/bitmap fonts used by RIPterm (Borland BGI
      `.CHR` stroked fonts + bitmap font behavior), metrics, and how fonts
      map to `RIP_TEXT`/`RIP_FONT_STYLE`
- [ ] Icon format (`.ICN`) — full binary layout, expanding on the spec's
      appendix with implementation detail
- [ ] `.RIP` file conventions — line structure, continuation, embedded ANSI,
      auto-sensing prologue conventions used by real files
- [ ] MegaNum/base-36 encoding notes and edge cases as implemented
- [ ] Terminal behavior details not fully specified (clipping, write modes,
      fill pattern semantics) as observed in reference implementations

### 2.x

- [ ] Icon format changes from 1.x (the format changed — document the delta)
- [ ] DIB/BMP support details
- [ ] Audio format support (WAV playback, sound variables)
- [ ] Palette / direct-RGB implementation details

### 3.x

- [ ] Whatever the 3.x research above uncovers (vector fonts, JPEG, MIDI,
      resolution independence)

## Documentation website

Decisions made: **VitePress** for the site, **Deno** as the runtime for all
generation/tooling scripts (`deno task`).

- [ ] Scaffold the VitePress site (config, theme, `deno task` wrappers)
- [ ] Generate sidebar/nav from the existing directory + README structure
      rather than hand-maintaining it
- [ ] Verify rendering of box-drawing/CP437-derived content, wide GFM tables,
      and `text`/`c` fenced blocks
- [ ] Landing page drawing from the main README
- [ ] **Discuss:** deployment target (GitHub Pages via Actions? custom domain
      under bbs.land?) and repo/branch strategy for generated output

## Machine-readable spec & grammar

- [ ] **Discuss:** the source-of-truth form — options: hand-maintained
      JSON/TOML command tables; a formal EBNF grammar for the wire syntax;
      or Rust types in a spec crate that export JSON. (These compose — e.g.
      EBNF for syntax + tables for the command inventory.)
- [ ] Command inventory: every command across 1.54 / 2.0a4 / 3.x with level,
      code, arguments (types/widths), and version introduced/changed
- [ ] Text variable inventory (same treatment)
- [ ] Grammar for the byte-level syntax: `!|` lines, delimiters, MegaNums,
      escapes (`\|`, `\\`), line continuation, embedded text
- [ ] Use it to cross-check the Markdown editions (doc ↔ table consistency)

## Reference test corpus

- [ ] **Discuss:** layout — top-level `corpus/` shared by docs/tests/crates,
      per-version `version/<v>/samples/`, or fixtures inside the Rust crates
- [ ] **Discuss:** encoding-variant extensions — `.rip`/`.ans` are assumed
      **CP437** (the DOS terminal representation) and stored byte-for-byte;
      decide on future extensions for other encodings (e.g. `.utf8.rip` /
      `.utf8.ans`, or others) and how tooling should detect/declare encoding
- [ ] Collect real-world `.RIP` art and BBS screens (art packs, archived BBS
      distributions), with provenance notes
- [ ] Extract sample icons, fonts, and scripts from original TeleGrafix
      distributions (see Reference materials)
- [ ] Author targeted conformance scripts per command/feature
- [ ] Golden-output renders (reference images) for regression testing

## Rust libraries (in this repo)

Goal: reusable, wasm-capable crates (wasm and/or cdylib for bindings from
other languages).

- [ ] **Discuss:** workspace layout and crate naming (e.g. `crates/` with
      `ripscrip-parse`, `ripscrip-render`, `ripscrip-wasm`?), and scope
      sequencing (parser → renderer → terminal integration?)
- [ ] Cargo workspace scaffold
- [ ] Parser crate — target RIPscrip 1.54 first; emit a typed command stream
- [ ] Renderer crate — EGA-faithful rasterization (fills, write modes, fonts)
- [ ] wasm bindings + minimal browser demo; cdylib C ABI for other languages
- [ ] Conformance harness driven by the test corpus and machine-readable spec
- [ ] Study prior art: `icy_tools/crates/icy_parser_core/src/rip/`,
      SyncTERM `ripper.c`, PabloDraw `Source/Pablo/Formats/Rip/`

## History & ecosystem

- [ ] **Discuss:** scope — candidates: TeleGrafix company/product timeline
      (RIPterm/RIPaint/RIPdraw/RIPtel, the 2.x alpha demise, the 3.0/RIPtel
      era); a catalog of implementations past and present (clients, BBS
      packages, doors, editors, libraries) with status; preserved press
      articles/FAQs/adverts; a gallery of notable RIP scenes
- [ ] Decide where it lives (`history/` top-level?) and how it feeds the website

## Reference materials (`~/src/rip-tools/`)

- [x] Research and clone open-source implementations for local grepping:
      `sbbs` (SyncTERM), `icy_tools`, `pablodraw`, `RIPtermJS`, `fTelnet`,
      `qodem` — documented in CONTRIBUTING.md
- [x] Download original runtime binaries for reverse-engineering reference,
      under `~/src/rip-tools/`, and document each (source URL, version,
      contents) in CONTRIBUTING.md:
  - [x] RIPterm 1.54 (`RIPterm154/` preservation repo, incl. original ZIP +
        DOSBox setup) and RIPterm 1.52 (`artifacts/ripterm-1.52/`)
  - [x] RIPterm 2.30 shareware, Win16 (`artifacts/ripterm-2.30/`, via Wayback)
  - [x] RIPtel Visual Telnet 3.10, Win16 (`artifacts/riptel-3.10/`, via Wayback)
  - [x] RIPaint 1.52 (`artifacts/ripaint-1.52/`, includes sample `.RIP` files)
  - [ ] RIPdraw — not yet located in any archive; keep hunting
  - [x] RIP 2 C Library manual PDF (`artifacts/docs/`) — SDK-era 2.x docs
  - [x] Era tools: IconUpDater 2.63, SysMon, ProBoard 2.20d, Searchlight
        BBS 5.10 (`artifacts/tools|proboard|searchlight/`)
  - [ ] Other era tools worth having (third-party RIP editors, BBS-side RIP
        doors/menus) as discovered — the archived `ftp.telegrafix.com` index
        pages on the Wayback Machine list more candidates
- [ ] Extract fonts/icons/sample scripts from these distributions into the
      test corpus (once its layout is decided)
- [ ] Note DOS emulation setup for running them (DOSBox-X config) if/when
      behavioral testing is needed

## Housekeeping

- [ ] Commit the existing work (everything to date is uncommitted on `main`)
- [x] Add `.gitattributes`: LF normalization by default; `.rip`/`.ans`
      exempt (`-text`, byte-for-byte, CRLF preserved for testing); RIP-era
      binary formats + `.zip`/`.exe` tracked via Git LFS (case-insensitive)
- [x] Enable Git LFS in the repo (`git lfs install --local`; hooks updated) —
      contributors need `git lfs install` once per machine, see CONTRIBUTING
- [ ] Verify LFS behavior on the remote (GitHub LFS quota/bandwidth) when the
      first binary content is pushed; decide whether large `artifacts/`-style
      binaries belong in-repo at all or stay in `~/src/rip-tools/`
- [ ] **Discuss:** licensing — the reproduced specs remain © TeleGrafix; the
      original documentation, machine-readable spec, corpus metadata, and Rust
      code need an explicit license (and possibly separate ones)
- [ ] **Discuss:** overall priority order across the areas above (several can
      proceed in parallel; nothing has been sequenced yet)

## Open questions (rollup)

Collected from the sections above — each needs a short discussion:

1. Priority/sequencing across areas
2. Test corpus layout
3. Machine-readable spec form (tables / EBNF / Rust-as-source)
4. History & ecosystem scope and location
5. Website deployment target and publishing strategy
6. Rust workspace layout, crate naming, and scope sequence
7. Licensing for original (non-TeleGrafix) content and code
8. Encoding-variant extensions for corpus files (`.rip`/`.ans` = CP437 for
   now; `.utf8.rip`/`.utf8.ans` or similar for future UTF-8 variants?)
