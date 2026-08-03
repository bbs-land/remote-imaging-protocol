# Contributing

This repository preserves and extends the Remote Imaging Protocol (RIPscrip)
specifications. This document records the technical conventions used while
generating and maintaining the documentation, so that new material stays
consistent with what exists.

## Checkout & repository setup

This repo uses **Git LFS** for binary content — install it and enable it
before (or right after) cloning:

```sh
git lfs install     # once per machine
git clone <repo>    # LFS content downloads automatically
# in an existing clone that predates LFS: git lfs pull
```

`.gitattributes` governs everything (see the file for the full list):

- Default: `* text=auto eol=lf` — text files are normalized to LF in the
  repo and on checkout.
- **`.rip` and `.ans` files are exempt** (`-text`, both upper/lower case):
  line endings are significant test data, so they are stored and checked out
  byte-for-byte — CRLF or LF, however the file exists. Do not "fix" them.
  These files are **CP437-encoded, not UTF-8** — they are DOS-era terminal
  streams, and CP437 is the assumed terminal representation. Do not
  re-encode them; view with a CP437-aware tool or via
  `iconv -f CP437 -t UTF-8`. (Possible future `.utf8.rip`/`.utf8.ans`
  extensions for UTF-8 variants are an open discussion — see TODO.md.)
- RIP-era binary formats are tracked via LFS, case-insensitively: icons
  (`.icn`, `.hic`), fonts (`.chr`, `.bgi`, `.fnt`), images (`.pcx`, `.bmp`,
  `.dib`, `.gif`, `.jpg/.jpeg`, `.png`), audio (`.wav`, `.mid`), and
  archives/executables (`.zip`, `.exe`). Add new binary extensions to
  `.gitattributes` with the same LFS pattern before committing such files.

## Repository conventions

- **Line endings:** LF only, in every file — except `.rip`/`.ans`, which are
  preserved as-is (see above).
- **Encoding:** Everything is UTF-8 — except `.rip`/`.ans` files, which are
  CP437 (see above). The original specification text files
  have been normalized from their as-sourced form: CP437 → UTF-8 (box-drawing
  and block glyphs preserved; the C0-range CP437 glyphs `0x10/0x11/0x1E/0x1F`
  mapped to `►◄▲▼`, which codecs treat as control bytes), CRLF → LF, and
  sheet-feed printer form feeds (`0x0C` page breaks from the original `.PRN`
  output) removed. Content is otherwise byte-faithful — page headers/footers
  and layout are untouched.
- **Faithful conversion:** The Markdown editions under `version/<v>/ripscrip/`
  are faithful conversions of the original TeleGrafix documents. Content is
  not corrected, modernized, or reworded — typos and errors in the source are
  preserved verbatim. Editorial commentary, when unavoidable (e.g. the
  unfinished `[BEGIN REWORD]` placeholder in 2.x §2.9), is clearly marked as
  an editor's note.
- **Original documentation** (techspecs, guides, history) lives outside the
  `ripscrip/` conversion directories and may freely correct, annotate, and
  extend — citing sources (spec section, or reference implementation file)
  for reverse-engineered details.

## Markdown style (conversion + new docs)

- Each page: one H1, then a nav line at top and bottom (after a `---` rule):
  `[◀ Prev: X](file.md) · [Contents](README.md) · [Next: Y ▶](file.md)`
- Command entries: `## RIP_XXX` heading, italic one-line function summary,
  then a GFM table for Level/Command/Arguments.
- `**Format:**` and `**Example:**` lines use inline code and stay **outside**
  tables (pipes inside code spans break GFM cells).
- ASCII-art tables become GFM tables (escape `|` as `\|`); diagrams and
  C structs become fenced ` ```text ` / ` ```c ` blocks.
- Anchors follow GitHub slug rules: lowercase, spaces → `-`, underscores kept
  (`#rip_text_window`), `$` stripped (`$DATE$` → `#date`).
- Cross-link liberally between pages; run `python3 tools/check-links.py`
  after edits (validates every file/anchor target across the doc trees;
  skips fenced and inline code).

## Technical specifications (`techspecs/`)

New original documentation of binary formats, structures, and implementation
details lives in `version/<v>/techspecs/`:

- Document each format in the **earliest version** where it appears
  (e.g. the 1.5x font and icon formats belong in `version/1.5x/techspecs/`).
- Later versions **reference** the earlier document and describe only the
  **changes/differences** (e.g. the icon format changed between 1.x and 2.x —
  `version/2.x/techspecs/` documents the delta, not the whole format).
- Cite where each detail comes from: the specification section, or the
  reference implementation source file (`repo:path/file.c`), so claims can be
  re-verified.

## Reference repositories (`~/src/rip-tools/`)

Peer repositories used for reference (reverse-engineering behavior, formats,
and rendering details) are cloned into **`~/src/rip-tools/`** — always use
this directory so paths are consistent across sessions and machines.

| Directory | Upstream | Language | Relevance |
|---|---|---|---|
| `~/src/rip-tools/sbbs/` | https://gitlab.synchro.net/main/sbbs.git | C | Synchronet + **SyncTERM** — `src/syncterm/ripper.c` is the most complete open RIP implementation (claims RIP 3.0 compatibility). |
| `~/src/rip-tools/RIPtermJS/` | https://github.com/cgorringe/RIPtermJS | JavaScript | RIPscrip 1.54 renderer for HTML canvas. |
| `~/src/rip-tools/icy_tools/` | https://github.com/mkrueger/icy_tools | Rust | BBS tool collection; `icy_term` includes RIPscrip emulation — useful prior art for the planned Rust library. |
| `~/src/rip-tools/pablodraw/` | https://github.com/cwensley/pablodraw | C# | ANSI/RIPscrip art editor/viewer (maintained fork of blocktronics/pablodraw). |
| `~/src/rip-tools/fTelnet/` | https://github.com/rickparrish/fTelnet | TypeScript | HTML5 WebSocket BBS client with experimental RIP support. |
| `~/src/rip-tools/qodem/` | https://codeberg.org/AutumnMeowMeow/qodem | C | Qmodem clone; detects and discards the RIP auto-sense (`CSI !`) — reference for the detection handshake only. |

Key entry points into the RIP code:

- `sbbs/src/syncterm/ripper.c` — the entire SyncTERM RIP implementation
  (~19,000 lines).
- `icy_tools/crates/icy_parser_core/src/rip/` — dedicated Rust RIP parser,
  with test scripts in `tests/rip/` and sample data in `benches/rip_data/`.
- `pablodraw/Source/Pablo/Formats/Rip/` — C# RIP format implementation
  (commands, rendering, editing).

Clones are shallow (`git clone --depth 1`); run `git fetch --unshallow` in a
repo if its history is needed. When adding a new reference repo, clone it into
`~/src/rip-tools/` and add a row to this table.

### Original binaries & documents (`~/src/rip-tools/artifacts/`)

Original-era binaries and documents for reverse-engineering reference, with
provenance. Additional preservation clone: `~/src/rip-tools/RIPterm154/`
(https://github.com/cgorringe/RIPterm154) holds the original `RIPTM154.ZIP`
**RIPterm 1.54** plus an unpacked DOS install and a ready-to-run DOSBox setup.

| Path | What | Provenance |
|---|---|---|
| `artifacts/ripterm-1.52/RIPTERM.ZIP` | RIPterm v1.52 (DOS) — `RIPTERM.EXE`, `RIPTERM.FNT`, `.ICN` icons, docs | https://archive.org/details/RIPTERM_ZIP |
| `artifacts/ripterm-2.30/rtrm2300.exe` | RIPterm v2.30 shareware installer (Win16 NE) | Wayback: `web.archive.org/web/20000819065943/http://www.telegrafix.com/products/ripterm/rtrm2300.exe` |
| `artifacts/riptel-3.10/rtel3100.exe` | RIPtel Visual Telnet v3.10 installer (Win16 NE) — the RIPscrip-3 client | Wayback: `web.archive.org/web/20010411110821/http://www.telegrafix.com/products/riptel/rtel3100.exe` |
| `artifacts/ripaint-1.52/RIPAINT.ZIP` | RIPaint v1.52 (DOS) — includes sample `.RIP` scripts | https://archive.org/details/RIPAINT_ZIP |
| `artifacts/docs/RIP2C-Library-Manual.pdf` | TeleGrafix RIP 2 C Library manual (SDK-era 2.x documentation) | https://archive.org/details/rip-2-c-library-manual |
| `artifacts/tools/ICUPD263.ZIP` | IconUpDater v2.63 (RIP icon updater door) | https://archive.org/details/ICUPD263.ZIP |
| `artifacts/tools/sysmon.zip` | TeleGrafix SysMon RIP tool (1997) | Wayback: `telegrafix.com/asr/tools/sysmon.zip` (1997-07-03 capture) |
| `artifacts/proboard/pb_220d.zip` | ProBoard BBS v2.20d (TeleGrafix-era release) | Wayback: `telegrafix.com/products/proboard/pb_220d.zip` (2000-10-10 capture) |
| `artifacts/searchlight/SLBBS510.EXE` | Searchlight BBS v5.10 (LHa SFX; TeleGrafix-era, RIP-supporting BBS) | Wayback: `telegrafix.com/products/searchlight/SLBBS510.EXE` (2001-09-05 capture) |

| `artifacts/RIPtel/` | **Extracted RIPtel 3.1 install** (driver 3.0.7, Oct 1997): `RIPTEL.EXE`, WinHelp docs (`RIPTEL.HLP`, `RIPSCRIP.HLP`), ~110 authentic RIPscrip 3.0 demo scripts in `ICONS/` (.RIP/.FN/.DEF/.MNU/.MSE/.RET/.ENT/.EXT/.COL), 10 `.CHR` stroke fonts + 8 `.RFF` outline fonts in `FONTS/`, demo BMP/JPG images | Extracted from `artifacts/riptel-3.10/rtel3100.exe` install |

| `artifacts/ripterm-2.30/extracted/` | Base files from an **installed** RIPterm 2.30 copy (docs incl. `RIPTERM.DOC`/`ARTWORK.DOC`, DIALCONV/RIPTECH/REGISTER executables, HMI sound drivers, splash bitmaps, `RIPterm.app` config; binaries dated 1995-11-28, docs 1997-10). The install **aborted before the `ICONS\`/`FONTS\` components** — the RW* icon set and `.CHR`/`.FNT`/`.FF1` fonts remain unrecovered. | Installed from `rtrm2300.exe` under Wine (aborted run) |

When adding a new artifact, keep this provenance table up to date. RIPdraw
has not yet been located in any archive.

### Distribution assets in-repo (`version/<v>/assets/`)

Original fonts/icons/audio distributed with each version are preserved in
`version/<v>/assets/{fonts,icons,audio}/`, each with a README documenting
every file. These are historical artifacts: `.gitattributes` stores
everything under `assets/` **byte-exact** (`-text`; binary extensions also go
through LFS). Do not re-encode, convert, or "fix" asset files; additions
require a README row and provenance. Audio directories are empty (no audio
ever shipped — RIPtel stored WAVs alongside icons, with no dedicated
directory) and exist to receive future recoveries.

## Implementation guidance

Renderer/terminal implementation details (canvas sizes, aspect-ratio policy)
live in [IMPLEMENTATION.md](IMPLEMENTATION.md) — deliberately outside the
`version/` language docs, which document the language only.

## Website tooling

The documentation website will be built with **VitePress**, with **Deno** as
the runtime for any generation/tooling scripts. Keep generated output out of
the repository; scripts should be runnable via `deno task`.
