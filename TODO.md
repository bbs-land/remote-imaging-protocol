# TODO

Planned work for this repository, grouped by area. Working conventions live in [CONTRIBUTING.md](CONTRIBUTING.md). **Completed items are removed** - this file stays active-only. **Discuss:** items need a decision before the tasks under them can be finalized.

Likely next up: the open **Discuss** questions in the rollup at the bottom; VitePress site scaffold; the machine-readable command/variable tables.

The **documentation restructure is complete** (2026-08-09) - `version/{1.54,2.0,3.0}/{ripscrip,techspecs}/` now carry the numbered two-layer hierarchy with the creator/implementer audience split, and the shared glossary lives at `version/glossary.md`. The structure and its principles are documented in [version/README.md](version/README.md).

## Specifications (Markdown reference editions)

- [ ] Extend the 3.x reconstruction as new evidence is analyzed: the RIP 2 C Library manual PDF (**image-only scans - needs OCR before it can be interrogated**, noted 2026-08-08), extraction of the RIPtel 3.10 / RIPterm 2.30 installers, and the seven known-but-unimplemented command descriptors (see `version/3.0/ripscrip/9.0-command-reference.md`)
- [ ] Annotate the conversions with errata and clarifications discovered from implementations - as clearly-marked editor's notes, never silent edits (first pass done 2026-08-08)
- [ ] Back-fill the unfinished 2.x §2.9 (`[BEGIN REWORD]` placeholder in the source) based on actual implementation behavior (SyncTERM `ripper.c`, icy_parser_core), cited and marked as reconstructed
- [ ] Grow `version/glossary.md` as new terms surface, and keep the canonical terms used consistently across pages
- [ ] Wire `run/check-links` and `run/lint` into CI so cross-links stay valid and Markdown stays formatted (both scripts exit non-zero on failure; the Python → Deno port of the link checker is done, 2026-08-11)
- [ ] **Decode `char_rot2`, the unconfirmed sub-field in RIP_EXTENDED_FONT_STYLE's shipped 26-character block** (`version/2.0/ripscrip/3.0-text-output-and-fonts.md`). The 2.30 wire layout was reconstructed from 18 `!|y` uses in the shipping RIPterm 2.30 install and corroborated against six field names in 2.30's own `RIPTERM.HLP` error strings, but one slot varies independently of the adjacent `shadow` field across the corpus in a way that fits neither name, so it is marked unconfirmed rather than asserted. Settle it from a wider `!|y` corpus, `RIPTERM.EXE` parse code, or the RIP 2 C Library manual once OCR'd
- [ ] **Decide the redaction policy for TeleGrafix contact details in `version/*/text/`.** The 2.0 document's contact block was removed 2026-08-10, but inline mentions survive in both files (`RIPScrip-1.54.txt:86`, `RIPScrip-2.0-alpha-4.txt:452` - "Communications, Inc. at (714) 379-2131") and 1.54's full contact block (lines 41-48) is still present. **Discuss:** redact consistently across all `text/` documents, or leave the 1.54 record intact as verbatim history

## RIPlib alignment (`version/3.0-riplib/`, `3.1-riplib/`, `3.2-riplib/`)

[RIPlib](https://github.com/BradHawthorne/riplib) is a parallel, actively developed portable C99 RIPscrip core. It reconstructs RIPscrip 3.0 from **disassembly of `RIPSCRIP.DLL` 3.0.7**, where this repository works from the RIPtel 3.1 help files and the 116-file demo corpus - complementary evidence, independently developed, not yet aligned. It also defines its own v3.1 (§A2G.1-7) and v3.2 (§A2G.8-13) extensions. Reconciled against riplib `main` @ `3e05ecb` (2026-06-30).

**Baseline conflicts** (questions of fact about RIPscrip 3.0, in [`version/3.0-riplib/`](version/3.0-riplib/README.md)) - ranked by impact:

- [ ] **Write-mode numbering** - the priority item. RIPlib assigns `|W01` = OR and `|W03` = XOR; 1.54, the 2.00a4/3.x table, 44 corpus uses, SyncTERM **and RIPlib's own `§DEAD.3`** all put XOR at `01` and OR at `02`. `|W01` is the common non-copy mode, so existing content renders wrong and nothing detects it ([page](version/3.0-riplib/ripscrip/2.0-write-modes.md))
- [ ] **`|J` and `|f`** - RIPlib assigns them to SAVE_ICON and FONT_ATTRIB; here they are RIP_SET_BASE_MATH and RIP_SET_WORLD_FRAME, and 90 of 116 corpus scenes open `!|J10|n2000|M08|fZKQO`. One `grep` of any demo script settles the wire question ([page](version/3.0-riplib/ripscrip/9.0-command-inventory-comparison.md))
- [ ] **Establish the provenance of RIPlib's punctuation-block names** (`&`, `-`, `+`, `[`, `]`, `_`, `;`, `<`) - here these are the 3.x skewed-oval family plus RIP_MARKER and RIP_POLY_POLYGON, named verbatim from TeleGrafix's own NEWCMDS.RIP comments. Same question for `|K` (filled rectangle vs kill-mouse-extended)
- [ ] **Reclassify `|!`, `|(`, `|)`, `|1R`** upstream - RIPlib lists them as its own extensions; all four are inherited TeleGrafix commands with matching behavior. Zero-cost correction
- [ ] **Verify RIPlib's baseline commands this repository cannot corroborate** - chiefly `RIP_GRADIENT_FILL` (`|28`), `RIP_FONT_ATTRIB` (`|f`) and the Level-2 digit opcodes, absent from the help inventory, the 2.00a4 draft and the corpus. Dumping the DLL's dispatch table and diffing it against the corpus opcode census would settle most of the items above in one pass
- [ ] **Text-escape attribution and rule 12** - `\!` is specification since 1.54, not a RIPlib extension; the SOH/STX alternate introducers are not implemented upstream, which is also why the v3.1 CSI trigger relaxation exists ([page](version/3.0-riplib/techspecs/1.0-stream-parsing-and-escapes.md))

**Extension review** ([`3.1-riplib/`](version/3.1-riplib/README.md), [`3.2-riplib/`](version/3.2-riplib/README.md)):

- [ ] **Raise the text-variable name collisions** - `$COMPAT$`, `$COPY$`, `$PROT$`, `$YEAR$` (v3.1) and `$HOUR$`, `$DOW$`, `$MONTH$` (v3.2) take names that denote different, corpus-attested things in 2.x/3.x. Every value RIPlib wants already has a canonical name (`$MHOUR$`, `$WDAY$`, `$MONTHNUM$`, `$DAY$`, `$FYEAR$`), so most of this is renameable at no cost to the features
- [ ] Decide whether any §A2G item is worth adopting into [`version/next/`](version/next/README.md) as a candidate for a future 3.5x/4.x revision (the state stack and the layout/introspection variables are the strongest candidates; both would need names that do not collide)

**Upkeep:**

- [ ] **Pull `git -C ~/src/rip-tools/riplib pull --ff-only origin main` at the start of any session touching these trees**, re-check `docs/spec/06-v31-extensions.md`, `06a-v32-extensions.md`, `07-variables.md`, `10-appendices.md` (§A.1, §A.8), `11-dll-deviations.md` and `CHANGELOG.md`, and update the deltas + the recorded commit in each tree's README (procedure: [reference/rip-tools.md](reference/rip-tools.md#riplib-is-a-moving-target---pull-main-regularly))
- [ ] Confirm the fill defects adopted from RIPlib's disassembly (`§BUG.6` pie/chord leak, `§DEAD.7` patterned flood brush) also hold in a **2.x-era** `RIPTERM.EXE` - currently generalized backward from the 3.0 driver on engine-lineage grounds, and flagged as such in [`version/2.0/techspecs/2.1-fill-defects.md`](version/2.0/techspecs/2.1-fill-defects.md)
- [ ] Re-check the other RIP_FILL claims that rest on 3.x help/corpus **silence**. `§DEAD.7` showed the 3.0 driver still implements flood fill, correcting a "not implemented at all" inference on [3.0's RIP_FILL entry](version/3.0/ripscrip/2.3-shapes-and-fills.md#rip_fill) and in [errata.md](version/2.0/ripscrip/errata.md#rip_fill-declared-removed-but-still-implemented) (which also notes the stray-`!|F` behavior was undetermined - it is now determined). Other claims derived the same way may need the same treatment

## RIPscrip 3.x research

- [ ] Inventory all known 3.0-era material beyond the white paper (RIPtel, WebRunner, TeleGrafix press releases, beta announcements, Usenet/FAQ posts)
- [ ] Hunt for the "RIPscrip 3.0 protocol specification" and "RIP-2-C 3.0 developer tools" TeleGrafix listed as products (confirmed to have existed via RIPtel help text)

## Technical specifications (`version/<v>/techspecs/`)

Rules in CONTRIBUTING.md: earliest-version placement, format-first/software-only, no VESA/driver detail, no `ripscrip/` duplication. Build-out completed 2026-08-08; follow-up candidates, ranked:

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
- [ ] Deepen history.md as sources surface: Boardwatch scans, BBS Dev News #46 full text, authoritative USPTO TSDR trademark check

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
