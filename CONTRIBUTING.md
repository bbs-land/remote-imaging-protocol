# Contributing

This repository preserves and extends the Remote Imaging Protocol (RIPscrip) specifications. This document records the technical conventions used while generating and maintaining the documentation, so that new material stays consistent with what exists. Everything here applies to **any author, human or AI**; workflow guidance specific to automated (AI) agents lives in [AGENTS.md](AGENTS.md).

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

- **Markdown file naming:** outside the repository root, the only CAPITALIZED Markdown filename is `README.md` - every other `.md` file is lowercase (`glossary.md`, `history.md`, `errata.md`, `reference/rip-tools.md`). This makes each directory's `README.md` - the index for that directory - easy to spot at a glance. Root-level files (`README.md`, `CONTRIBUTING.md`, `AGENTS.md`, `TODO.md`, `DONE.md`, `LICENSE`) keep their conventional uppercase names.
- **TODO/DONE workflow:** [TODO.md](TODO.md) holds **active work only**. When an item completes, move it (with its date and a result summary) to [DONE.md](DONE.md) under the matching area heading - don't leave checked-off items accumulating in TODO.md. Update TODO.md before/after each work session as tasks start and finish.
- **Line endings:** LF only, in every file - except `.rip`/`.ans`, which are preserved as-is (see above).
- **Encoding:** Everything is UTF-8 - except `.rip`/`.ans` files, which are CP437 (see above). The original specification text files have been normalized from their as-sourced form: CP437 → UTF-8 (box-drawing and block glyphs preserved; the C0-range CP437 glyphs `0x10/0x11/0x1E/0x1F` mapped to `►◄▲▼`, which codecs treat as control bytes), CRLF → LF, and sheet-feed printer form feeds (`0x0C` page breaks from the original `.PRN` output) removed. Content is otherwise byte-faithful - page headers/footers and layout are untouched.
- **Version naming - language vs client:** `version/<v>/` directories are named for the **RIPscrip language version** (`1.54`, `2.0`, `3.0`), never for the client that shipped it. The two diverge: every 2.x client reports `RIPSCRIP020000` whether it is RIPterm 2.0, 2.20.01 or 2.30, and RIPtel 3.1 ships RIPscrip driver 3.0.7. Write the language version when the claim is about the protocol, the specification, or a page in this repository (`the 2.0 reference`, `the v3.0 techspec`, `2.x`); write the **full product name and version** when the claim is about a client's behavior, packaging, or release (`RIPterm 2.30 dropped the legacy .ICN art`, `RIPtel 3.1's help files`). `assets/` is the one exception by design - it preserves what the **last client of the generation** shipped, so `2.0/assets/` is RIPterm 2.30's fonts and icons and `3.0/assets/` is RIPtel 3.1's; describe those files by their product. Never rewrite section numbers, file names, or anchors that merely happen to read `3.1` or `2.30`.
- **Reference editions:** Only the original `.txt` documents under `version/<v>/text/` are a verbatim record - never "fix" them. The Markdown editions under `version/<v>/ripscrip/` are **correct reference material**, not 1:1 translations of the spec files: corrections, reconstructions, and clarifications are welcome when clearly marked (editor's notes, evidence tags) and cited (spec section, or reference implementation file).
- **Errata convention (2.0 only, for now):** `version/2.0/ripscrip/` is the one tree with a real draft-vs-shipped gap - its canonical text is a pre-release ALPHA 4 draft, not a final spec. For this tree, the chapter pages state the corrected, as-shipped reading an implementer should follow, with no inline qualification; draft-vs-shipped divergences and corrected draft errors are recorded instead in [`version/2.0/ripscrip/errata.md`](version/2.0/ripscrip/errata.md), grouped by chapter with the draft's text, the shipped reality and its evidence, and a pointer back to the page section it explains. This does not relax the rule above - corrections still need to be clearly marked and cited - it just moves where the marking lives for this tree. 1.54 shipped a final spec and needs no such split; 3.0 is an avowed reconstruction whose inline evidence tags are integral to the page, not a historical aside to file elsewhere.
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
- Later versions **reference** the earlier document and describe only the **changes/differences** (e.g. the icon format changed between 1.x and 2.x - `version/2.0/techspecs/` documents the delta, not the whole format).
- Cite where each detail comes from: the specification section, or the reference implementation source file (`repo:path/file.c`), so claims can be re-verified.
- **Format-first, software-only:** all techspecs center on the formats, assuming a software-only implementation - not talking directly to hardware like the legacy clients. The original hardware and software libraries may be referenced only insofar as they explain the structure to be implemented (e.g. Borland fonts defining the `.CHR` layout, clip-region semantics in icons) - not as operational detail (audio playback libraries, driver stacks, video registers; a modern OS/canvas provides those).
- **No DOS driver detail:** VESA modes and DOS video-output/driver specifics are immaterial to modern implementations - document logical canvas/viewport/window sizes; era driver specifics survive only as brief commentary.
- **Don't duplicate the `ripscrip/` reference pages:** link to them for wire commands and semantics. Techspecs cover what the spec docs don't - binary layouts, decoded structures, rendering behavior, and reconciliations against shipped files.

## Reference repositories & artifacts (`~/src/rip-tools/`)

External reference material - peer implementation repositories (SyncTERM, RIPtermJS, icy_tools, pablodraw, fTelnet, qodem) and original-era binaries/documents with provenance - lives outside this repository under `~/src/rip-tools/`, and is catalogued in [reference/rip-tools.md](reference/rip-tools.md). When adding a reference repo or artifact, update that catalogue (clone location, table row, provenance). Throughout, `~/` means the user's home (profile) directory; on Windows that is not literally `~/` (it is the profile directory, e.g. `%USERPROFILE%`) unless you are in an environment like MSYS2 bash (aka Git Bash) that resolves it.

## Distribution assets in-repo (`version/<v>/assets/`)

Original fonts/icons/audio distributed with each version are preserved in `version/<v>/assets/{fonts,icons,audio}/`, each with a README documenting every file. These are historical artifacts: `.gitattributes` stores everything under `assets/` **byte-exact** (`-text`; binary extensions also go through LFS). Do not re-encode, convert, or "fix" asset files; additions require a README row and provenance. Audio directories are mostly empty - neither RIPterm nor RIPtel had a dedicated audio directory (host-supplied WAVs lived alongside the icons in `ICONS\`) - and exist to receive future recoveries; the sole recovered file is RIPterm 2.0's own `RIPTERM.WAV` under `version/2.0/assets/audio/`.

## Implementation guidance

Renderer/terminal implementation details (canvas sizes, aspect-ratio policy) live in [version/implementation.md](version/implementation.md) - deliberately outside the language docs themselves, which document the language only.

## Website tooling

The documentation website will be built with **VitePress**, with **Deno** as the runtime for any generation/tooling scripts. Keep generated output out of the repository; scripts should be runnable via `deno task`.
