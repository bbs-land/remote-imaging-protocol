# TODO

Planned work for this repository, grouped by area. Working conventions live in [CONTRIBUTING.md](CONTRIBUTING.md). **Completed items move to [DONE.md](DONE.md)** - this file stays active-only. **Discuss:** items need a decision before the tasks under them can be finalized.

Likely next up: the open **Discuss** questions in the rollup at the bottom; VitePress site scaffold; the machine-readable command/variable tables.

The **documentation restructure is complete** (2026-08-09) - `version/{1.54,2.30,3.1}/{ripscrip,techspecs}/` now carry the numbered two-layer hierarchy with the creator/implementer audience split, and the shared glossary lives at `version/GLOSSARY.md`. The structure and its principles are documented in [version/README.md](version/README.md); the work log is in [DONE.md](DONE.md).

## Specifications (Markdown reference editions)

- [ ] Extend the 3.x reconstruction as new evidence is analyzed: the RIP 2 C Library manual PDF (**image-only scans - needs OCR before it can be interrogated**, noted 2026-08-08), extraction of the RIPtel 3.10 / RIPterm 2.30 installers, and the seven known-but-unimplemented command descriptors (see `version/3.1/ripscrip/9.0-command-reference.md`)
- [ ] Annotate the conversions with errata and clarifications discovered from implementations - as clearly-marked editor's notes, never silent edits (first pass done 2026-08-08, see DONE.md)
- [ ] Back-fill the unfinished 2.x §2.9 (`[BEGIN REWORD]` placeholder in the source) based on actual implementation behavior (SyncTERM `ripper.c`, icy_parser_core), cited and marked as reconstructed
- [ ] Grow `version/GLOSSARY.md` as new terms surface, and keep the canonical terms used consistently across pages
- [ ] Port the link/anchor checker (`tools/check-links.py`, currently Python) to a Deno script; wire into CI so cross-links stay valid
- [ ] **Decode `char_rot2`, the unconfirmed sub-field in RIP_EXTENDED_FONT_STYLE's shipped 26-character block** (`version/2.30/ripscrip/3.0-text-output-and-fonts.md`). The 2.30 wire layout was reconstructed from 18 `!|y` uses in the shipping RIPterm 2.30 install and corroborated against six field names in 2.30's own `RIPTERM.HLP` error strings, but one slot varies independently of the adjacent `shadow` field across the corpus in a way that fits neither name, so it is marked unconfirmed rather than asserted. Settle it from a wider `!|y` corpus, `RIPTERM.EXE` parse code, or the RIP 2 C Library manual once OCR'd
- [ ] **Decide the redaction policy for TeleGrafix contact details in `version/*/text/`.** The 2.30 document's contact block was removed 2026-08-10, but inline mentions survive in both files (`RIPScrip-1.54.txt:86`, `RIPScrip-2.0-alpha-4.txt:452` - "Communications, Inc. at (714) 379-2131") and 1.54's full contact block (lines 41-48) is still present. **Discuss:** redact consistently across all `text/` documents, or leave the 1.54 record intact as verbatim history

## RIPscrip 3.x research

- [ ] Inventory all known 3.0-era material beyond the white paper (RIPtel, WebRunner, TeleGrafix press releases, beta announcements, Usenet/FAQ posts)
- [ ] Hunt for the "RIPscrip 3.0 protocol specification" and "RIP-2-C 3.0 developer tools" TeleGrafix listed as products (confirmed to have existed via RIPtel help text)

## Technical specifications (`version/<v>/techspecs/`)

Rules in CONTRIBUTING.md: earliest-version placement, format-first/software-only, no VESA/driver detail, no `ripscrip/` duplication. Build-out completed 2026-08-08 (see DONE.md); follow-up candidates, ranked:

- [ ] FastFont follow-ups (optional, post-decode): cross-validate metrics fields via an AllType 2.0 FF1→Type 1 round-trip; fold stem-hint record semantics into the spec if grid-fitted rendering is ever wanted
- [ ] **RIPaint 1.52 interrogation** for tool-side formats (patterns/palettes/project files, if any beyond `.RIP`) - may glean useful information even though RIPterm 1.54 is the canonical implementation of the generation; no functional 1.52→1.54 delta is known and no later 1.xx RIPaint is believed to have existed (the next known RIPaint is the unrecovered 2.x)
- [ ] Low priority, preservation-only: full decode of the UI system `.FNT` glyph tables (host-invisible), and the `.MAC` keystroke-macro text format (`TYPE=EMULATION` files)

## Documentation website

Decisions made: **VitePress** for the site, **Deno** as the runtime for all generation/tooling scripts (`deno task`).

- [ ] Scaffold the VitePress site (config, theme, `deno task` wrappers)
- [ ] Generate sidebar/nav from the directory + README structure rather than hand-maintaining it (the numbered `N.M` hierarchy under `version/<v>/` is designed to feed this)
- [ ] Verify rendering of box-drawing/CP437-derived content, wide GFM tables, and `text`/`c` fenced blocks
- [ ] Landing page drawing from the main README
- [ ] **Discuss:** deployment target (GitHub Pages via Actions? custom domain under bbs.land?) and repo/branch strategy for generated output

## Machine-readable spec & grammar

- [ ] **Discuss:** the source-of-truth form - options: hand-maintained JSON/TOML command tables; a formal EBNF grammar for the wire syntax; or Rust types in a spec crate that export JSON. (These compose - e.g. EBNF for syntax + tables for the command inventory.)
- [ ] Command inventory: every command across 1.54 / 2.0a4 / 3.x with level, code, arguments (types/widths), and version introduced/changed
- [ ] Text variable inventory (same treatment)
- [ ] Grammar for the byte-level syntax: `!|` lines, delimiters, MegaNums, escapes (`\|`, `\\`), line continuation, embedded text
- [ ] Use it to cross-check the Markdown editions (doc ↔ table consistency)

## Reference test corpus

- [ ] **Discuss:** layout - top-level `corpus/` shared by docs/tests/crates, per-version `version/<v>/samples/`, or fixtures inside the Rust crates
- [ ] **Discuss:** encoding-variant extensions - `.rip`/`.ans` are assumed **CP437** (the DOS terminal representation) and stored byte-for-byte; decide on future extensions for other encodings (e.g. `.utf8.rip` / `.utf8.ans`, or others) and how tooling should detect/declare encoding
- [ ] Collect real-world `.RIP` art and BBS screens (art packs, archived BBS distributions), with provenance notes
- [ ] Extract sample icons, fonts, and scripts from original TeleGrafix distributions (see Reference materials)
- [ ] Author targeted conformance scripts per command/feature
- [ ] Golden-output renders (reference images) for regression testing

## Rust libraries (in this repo)

Goal: reusable, wasm-capable crates (wasm and/or cdylib for bindings from other languages).

- [ ] **Discuss:** workspace layout and crate naming (e.g. `crates/` with `ripscrip-parse`, `ripscrip-render`, `ripscrip-wasm`?), and scope sequencing (parser → renderer → terminal integration?)
- [ ] Cargo workspace scaffold
- [ ] Parser crate - target RIPscrip 1.54 first; emit a typed command stream
- [ ] Renderer crate - EGA-faithful rasterization (fills, write modes, fonts)
- [ ] wasm bindings + minimal browser demo; cdylib C ABI for other languages
- [ ] Conformance harness driven by the test corpus and machine-readable spec
- [ ] Study prior art: `icy_tools/crates/icy_parser_core/src/rip/`, SyncTERM `ripper.c`, PabloDraw `Source/Pablo/Formats/Rip/`

## History & ecosystem

- [ ] **Discuss:** remaining scope - a catalog of implementations past and present (clients, BBS packages, doors, editors, libraries) with status; preserved press articles/FAQs/adverts; a gallery of notable RIP scenes; how it all feeds the website
- [ ] Deepen HISTORY.md as sources surface: Boardwatch scans, BBS Dev News #46 full text, authoritative USPTO TSDR trademark check

## Reference materials (`~/src/rip-tools/`)

- [ ] RIPdraw - not yet located in any archive; keep hunting
- [ ] Other era tools worth having (third-party RIP editors, BBS-side RIP doors/menus) as discovered - the archived `ftp.telegrafix.com` index pages on the Wayback Machine list more candidates
- [ ] Extract sample scripts into the test corpus (once its layout is decided)
- [ ] Note DOS emulation setup for running them (DOSBox-X config) if/when behavioral testing is needed

## Housekeeping

- [ ] Verify LFS behavior on the remote (GitHub LFS quota/bandwidth) when the first binary content is pushed; decide whether large `artifacts/`-style binaries belong in-repo at all or stay in `~/src/rip-tools/`
- [ ] **Discuss:** overall priority order across the areas above (several can proceed in parallel; nothing has been sequenced yet)

## Open questions (rollup)

Collected from the sections above - each needs a short discussion:

1. Priority/sequencing across areas
2. Test corpus layout
3. Machine-readable spec form (tables / EBNF / Rust-as-source)
4. History & ecosystem remaining scope (implementations catalog, press archive, gallery) - location resolved: `version/`
5. Website deployment target and publishing strategy
6. Rust workspace layout, crate naming, and scope sequence
7. Encoding-variant extensions for corpus files (`.rip`/`.ans` = CP437 for now; `.utf8.rip`/`.utf8.ans` or similar for future UTF-8 variants?)
