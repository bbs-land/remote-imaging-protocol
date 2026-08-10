# Contributing

This repository preserves and extends the Remote Imaging Protocol (RIPscrip) specifications. This document records the technical conventions used while generating and maintaining the documentation, so that new material stays consistent with what exists.

# Agents

When running parallel worker agents, limit the number of parallel agents to 3 unless told otehrwise. You should also use a `WORKING.md` file in the project directory to coordinate such work. Tracking a work summary, testing details if any as well as the immediate task list, list of agents as they start, progress and finish a given task. This should allow for relatively easy continuation of broken work, or incomplete work due to a session window limit.

## Checkout & repository setup

This repo uses **Git LFS** for binary content - install it and enable it before (or right after) cloning:

```sh
git lfs install     # once per machine
git clone <repo>    # LFS content downloads automatically
# in an existing clone that predates LFS: git lfs pull
```

`.gitattributes` governs everything (see the file for the full list):

- Default: `* text=auto eol=lf` - text files are normalized to LF in the repo and on checkout.
- **`.rip` and `.ans` files are exempt** (`-text`, both upper/lower case): line endings are significant test data, so they are stored and checked out byte-for-byte - CRLF or LF, however the file exists. Do not "fix" them. These files are **CP437-encoded, not UTF-8** - they are DOS-era terminal streams, and CP437 is the assumed terminal representation. Do not re-encode them; view with a CP437-aware tool or via `iconv -f CP437 -t UTF-8`. (Possible future `.utf8.rip`/`.utf8.ans` extensions for UTF-8 variants are an open discussion - see TODO.md.) Any new format analogous to `.rip`/`.ans` - a terminal stream or other file whose line endings are significant - must get its own `-text` exemption pattern in `.gitattributes` **before** it is committed, or the LF default will rewrite its line endings.
- RIP-era binary formats are tracked via LFS, case-insensitively: icons (`.icn`, `.hic`), fonts (`.chr`, `.bgi`, `.fnt`, `.ff1`, `.rff`), images (`.pcx`, `.bmp`, `.dib`, `.gif`, `.jpg/.jpeg`, `.png`), audio (`.wav`, `.mid`), and archives/executables (`.zip`, `.exe`). Add new binary extensions to `.gitattributes` with the same LFS pattern before committing such files. Tiny binary sidecar formats (`.bmh` hot icons, `.img` widget images) are deliberately stored plain, covered by the `version/*/assets/** -text` rule.
- **Before adding files of any new type, check `.gitattributes` covers the extension** - LFS pattern for binary formats, `-text` exemption for line-ending-significant formats (see the `.rip`/`.ans` rule above) - and add the missing pattern first. Getting this wrong is expensive after the fact: the `.RFF` fonts were originally committed without an LFS rule, and scrubbing the raw blobs required rewriting history (2026-08-08 force-push; see the note in [README.md](README.md#repository-layout)).

## Repository conventions

- **TODO/DONE workflow:** [TODO.md](TODO.md) holds **active work only**. When an item completes, move it (with its date and a result summary) to [DONE.md](DONE.md) under the matching area heading - don't leave checked-off items accumulating in TODO.md. Update TODO.md before/after each work session as tasks start and finish.
- **Line endings:** LF only, in every file - except `.rip`/`.ans`, which are preserved as-is (see above).
- **Encoding:** Everything is UTF-8 - except `.rip`/`.ans` files, which are CP437 (see above). The original specification text files have been normalized from their as-sourced form: CP437 → UTF-8 (box-drawing and block glyphs preserved; the C0-range CP437 glyphs `0x10/0x11/0x1E/0x1F` mapped to `►◄▲▼`, which codecs treat as control bytes), CRLF → LF, and sheet-feed printer form feeds (`0x0C` page breaks from the original `.PRN` output) removed. Content is otherwise byte-faithful - page headers/footers and layout are untouched.
- **Reference editions:** Only the original `.txt` documents under `version/<v>/text/` are a verbatim record - never "fix" them. The Markdown editions under `version/<v>/ripscrip/` are **correct reference material**, not 1:1 translations of the spec files: corrections, reconstructions, and clarifications are welcome when clearly marked (editor's notes, evidence tags) and cited (spec section, or reference implementation file).
- **Original documentation** (techspecs, guides, history) lives outside the `ripscrip/` specification directories and may freely correct, annotate, and extend - citing sources for reverse-engineered details.

## Markdown style (all Markdown files)

- Bullet points use hyphens (`-`), never asterisks (`*`).
- **No hard word-wraps** inside paragraphs or bullet points - one line per paragraph/bullet. Editors should rely on virtual/soft wrapping; hard wraps break rendering.
- Use **Prettier** for Markdown formatting in your editor - the repo's `.prettierrc` sets `proseWrap: never` so it unwraps rather than re-wraps.
- Each page: one H1, then a nav line at top and bottom (after a `---` rule): `[◀ Prev: X](file.md) · [Contents](README.md) · [Next: Y ▶](file.md)`
- Command entries: `## RIP_XXX` heading, italic one-line function summary, then a GFM table for Level/Command/Arguments.
- `**Format:**` and `**Example:**` lines use inline code and stay **outside** tables (pipes inside code spans break GFM cells).
- ASCII-art tables become GFM tables (escape `|` as `\|`); diagrams and C structs become fenced ` ```text ` / ` ```c ` blocks.
- Anchors follow GitHub slug rules: lowercase, spaces → `-`, underscores kept (`#rip_text_window`), `$` stripped (`$DATE$` → `#date`).
- Cross-link liberally between pages; run `python3 tools/check-links.py` after edits (validates every file/anchor target across the doc trees; skips fenced and inline code).

## Technical specifications (`techspecs/`)

New original documentation of binary formats, structures, and implementation details lives in `version/<v>/techspecs/`:

- Document each format in the **earliest version** where it appears (e.g. 1.5x font and icon formats belong in `version/1.54/techspecs/`).
- Later versions **reference** the earlier document and describe only the **changes/differences** (e.g. the icon format changed between 1.x and 2.x - `version/2.30/techspecs/` documents the delta, not the whole format).
- Cite where each detail comes from: the specification section, or the reference implementation source file (`repo:path/file.c`), so claims can be re-verified.
- **Format-first, software-only:** all techspecs center on the formats, assuming a software-only implementation - not talking directly to hardware like the legacy clients. The original hardware and software libraries may be referenced only insofar as they explain the structure to be implemented (e.g. Borland fonts defining the `.CHR` layout, clip-region semantics in icons) - not as operational detail (audio playback libraries, driver stacks, video registers; a modern OS/canvas provides those).
- **No DOS driver detail:** VESA modes and DOS video-output/driver specifics are immaterial to modern implementations - document logical canvas/viewport/window sizes; era driver specifics survive only as brief commentary.
- **Don't duplicate the `ripscrip/` reference pages:** link to them for wire commands and semantics. Techspecs cover what the spec docs don't - binary layouts, decoded structures, rendering behavior, and reconciliations against shipped files.

## Reference repositories (`~/src/rip-tools/`)

Peer repositories used for reference (reverse-engineering behavior, formats, and rendering details) are cloned into **`~/src/rip-tools/`** - always use this directory so paths are consistent across sessions and machines.

| Directory | Upstream | Language | Relevance |
| --- | --- | --- | --- |
| `~/src/rip-tools/sbbs/` | https://gitlab.synchro.net/main/sbbs.git | C | Synchronet + **SyncTERM** - `src/syncterm/ripper.c` is the most complete open RIP implementation (claims RIP 3.0 compatibility). |
| `~/src/rip-tools/RIPtermJS/` | https://github.com/cgorringe/RIPtermJS | JavaScript | RIPscrip 1.54 renderer for HTML canvas. |
| `~/src/rip-tools/icy_tools/` | https://github.com/mkrueger/icy_tools | Rust | BBS tool collection; `icy_term` includes RIPscrip emulation - useful prior art for the planned Rust library. |
| `~/src/rip-tools/pablodraw/` | https://github.com/cwensley/pablodraw | C# | ANSI/RIPscrip art editor/viewer (maintained fork of blocktronics/pablodraw). |
| `~/src/rip-tools/fTelnet/` | https://github.com/rickparrish/fTelnet | TypeScript | HTML5 WebSocket BBS client with experimental RIP support. |
| `~/src/rip-tools/qodem/` | https://codeberg.org/AutumnMeowMeow/qodem | C | Qmodem clone; detects and discards the RIP auto-sense (`CSI !`) - reference for the detection handshake only. |

Key entry points into the RIP code:

- `sbbs/src/syncterm/ripper.c` - the entire SyncTERM RIP implementation (~19,000 lines).
- `icy_tools/crates/icy_parser_core/src/rip/` - dedicated Rust RIP parser, with test scripts in `tests/rip/` and sample data in `benches/rip_data/`.
- `pablodraw/Source/Pablo/Formats/Rip/` - C# RIP format implementation (commands, rendering, editing).

Clones are shallow (`git clone --depth 1`); run `git fetch --unshallow` in a repo if its history is needed. When adding a new reference repo, clone it into `~/src/rip-tools/` and add a row to this table.

### Original binaries & documents (`~/src/rip-tools/artifacts/`)

Original-era binaries and documents for reverse-engineering reference, with provenance. Additional preservation clone: `~/src/rip-tools/RIPterm154/` (https://github.com/cgorringe/RIPterm154) holds the original `RIPTM154.ZIP` **RIPterm 1.54** plus an unpacked DOS install and a ready-to-run DOSBox setup.

| Path | What | Provenance |
| --- | --- | --- |
| `artifacts/ripterm-1.52/RIPTERM.ZIP` | RIPterm v1.52 (DOS) - `RIPTERM.EXE`, `RIPTERM.FNT`, `.ICN` icons, docs | https://archive.org/details/RIPTERM_ZIP |
| `artifacts/ripterm-2.30/RIPT2300.zip` | RIPterm v2.30 shareware distribution (complete, 1,960,399 bytes): `INSTALL.BAT`, PKZIP self-extracting `SHAREWAR.EXE` (DOS installer), `FILE_ID.DIZ`, `LICENSE.DOC`, `README.DOC` | [VOGONS "Looking for RIPTerm" thread attachment](https://www.vogons.org/viewtopic.php?t=67912) (recovered 2026-08-08); mirrored at [files.bbs.land](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTerm%202.30/RIPT2300.zip) |
| `artifacts/ripterm-2.30/rtrm2300.exe` | RIPterm v2.30 shareware installer - **truncated/damaged** (1,048,309 of ~2,000,000 bytes; install aborts partway; kept for provenance only) | Wayback: `web.archive.org/web/20000819065943/http://www.telegrafix.com/products/ripterm/rtrm2300.exe` |
| `artifacts/riptel-3.10/rtel3100.exe` | RIPtel Visual Telnet v3.10 installer (Win16 NE) - the RIPscrip-3 client | Wayback: `web.archive.org/web/20010411110821/http://www.telegrafix.com/products/riptel/rtel3100.exe` |
| `artifacts/ripaint-1.52/RIPAINT.ZIP` | RIPaint v1.52 (DOS) - includes sample `.RIP` scripts | https://archive.org/details/RIPAINT_ZIP |
| `artifacts/docs/RIP2C-Library-Manual.pdf` | TeleGrafix RIP 2 C Library manual (SDK-era 2.x documentation) | https://archive.org/details/rip-2-c-library-manual |
| `artifacts/tools/ICUPD263.ZIP` | IconUpDater v2.63 (RIP icon updater door) | https://archive.org/details/ICUPD263.ZIP |
| `artifacts/tools/sysmon.zip` | TeleGrafix SysMon RIP tool (1997) | Wayback: `telegrafix.com/asr/tools/sysmon.zip` (1997-07-03 capture) |
| `artifacts/proboard/pb_220d.zip` | ProBoard BBS v2.20d (TeleGrafix-era release) | Wayback: `telegrafix.com/products/proboard/pb_220d.zip` (2000-10-10 capture) |
| `artifacts/searchlight/SLBBS510.EXE` | Searchlight BBS v5.10 (LHa SFX; TeleGrafix-era, RIP-supporting BBS) | Wayback: `telegrafix.com/products/searchlight/SLBBS510.EXE` (2001-09-05 capture) |

| `artifacts/RIPtel/` | **Extracted RIPtel 3.1 install** (driver 3.0.7, Oct 1997): `RIPTEL.EXE`, WinHelp docs (`RIPTEL.HLP`, `RIPSCRIP.HLP`), ~110 authentic RIPscrip 3.0 demo scripts in `ICONS/` (.RIP/.FN/.DEF/.MNU/.MSE/.RET/.ENT/.EXT/.COL), 10 `.CHR` stroke fonts + 8 `.RFF` outline fonts in `FONTS/`, demo BMP/JPG images | Extracted from `artifacts/riptel-3.10/rtel3100.exe` install |

| `RIPTerm2.0/extracted/` | **Installed RIPterm Professional 2.0** (released Jan 23-24, 1995; files dated 1995-01-27 to 1995-02-01): `RIPTERM.EXE` (32-bit DOS4GW protected mode), the full `RIPTERM.DOC` manual, `README.DOC`/`THANKYOU.DOC`, RIPtech 1.0 diagnostics (`RIPTECH.EXE`/`.DOC`), HMI sound drivers (`HMIDRV.386`/`HMIDET.386`), `RIPTERM.WAV`, MicroANSI `RIPTERM.FNT`, splash `RIPTERM.BMP`, keyboard macros (`.MAC`), phone directory (`.PHO`), video-mode database (`.VID`), `FONTS\` (10 `.CHR`, 5 Atech `.FF1`, per-resolution `.FNT` + `.IMG` widgets, `DEMO_ONE.EXE` FF1 viewer), `ICONS\` (91 `.BMP`, 3 `.BMH`, `SHADOW.RIP`). Fonts/icons/audio are preserved in-repo under `version/2.30/assets/`. | Installed from the original two-disk commercial release | | `artifacts/ripterm-2.30/extracted/` | **Complete RIPterm 2.30 file set** (251 files; binaries dated 1995-11-28, 2.3-era files 1997-10): `RIPTERM.EXE` (32-bit DOS4GW protected mode), full `RIPTERM.DOC` manual, `RIPTERM.FAQ`, `RIPTERM.HLP`, `README.DOC`/`ARTWORK.DOC`/`RIPTECH.DOC`, DIALCONV/RIPTECH/REGISTER executables, HMI sound drivers, splash bitmaps, keyboard macros, `RIPterm.app` config, Windows DOS-box conveniences (`.PIF`/`.ICO`), `FONTS\` and `ICONS\` directories | Unpacked from `RIPT2300.zip`'s `SHAREWAR.EXE` (PKZIP self-extractor) |

When adding a new artifact, keep this provenance table up to date. RIPdraw has not yet been located in any archive.

### Distribution assets in-repo (`version/<v>/assets/`)

Original fonts/icons/audio distributed with each version are preserved in `version/<v>/assets/{fonts,icons,audio}/`, each with a README documenting every file. These are historical artifacts: `.gitattributes` stores everything under `assets/` **byte-exact** (`-text`; binary extensions also go through LFS). Do not re-encode, convert, or "fix" asset files; additions require a README row and provenance. Audio directories are mostly empty - neither RIPterm nor RIPtel had a dedicated audio directory (host-supplied WAVs lived alongside the icons in `ICONS\`) - and exist to receive future recoveries; the sole recovered file is RIPterm 2.0's own `RIPTERM.WAV` under `version/2.30/assets/audio/`.

## Implementation guidance

Renderer/terminal implementation details (canvas sizes, aspect-ratio policy) live in [version/IMPLEMENTATION.md](version/IMPLEMENTATION.md) - deliberately outside the language docs themselves, which document the language only.

## Website tooling

The documentation website will be built with **VitePress**, with **Deno** as the runtime for any generation/tooling scripts. Keep generated output out of the repository; scripts should be runnable via `deno task`.
