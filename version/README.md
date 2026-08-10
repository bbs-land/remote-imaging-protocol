# The Remote Imaging Protocol - Version Documentation

RIPscrip (Remote Imaging Protocol Script language) is a vector-graphics scripting language created at TeleGrafix Communications, Inc. (Huntington Beach, California) and introduced in 1992. It let Bulletin Board Systems send interactive, mouse-driven graphical interfaces - lines, shapes, fills, fonts, icons, buttons, and clickable regions - as compact streams of printable ASCII over dial-up modems, at a time when the alternative was plain ASCII/ANSI text. The language was principally designed by Jeff Reeder, with Jim Bergman and Mark Hayton credited alongside him on the original work.

A RIP-capable terminal interpreted the command stream and rendered graphics in a viewport while ordinary text flowed to a separate text window. TeleGrafix's own clients were **RIPterm** (dial-up/modem BBS connections) and later **RIPtel** (telnet) - RIPscrip is the language both spoke - alongside a number of third-party programs. Buttons and mouse fields sent host commands back to the BBS, making full point-and-click services possible over a 2400-baud connection. Conceptually it occupied the niche that Flash and SVG would later fill on the web: compact, resolution-oriented vector scenes over thin pipes.

RIPscrip saw real adoption in the BBS world of 1993-1996 - supported by major BBS packages and terminal programs - though it never displaced ANSI art as the dominant online graphics form. The public World Wide Web drained the BBS ecosystem rapidly in the late 1990s, ending RIPscrip development and, ultimately, TeleGrafix itself. See [HISTORY.md](HISTORY.md) for the company and release timeline, and [RIGHTS.md](RIGHTS.md) for the copyright/trademark situation and this repository's licensing.

## What this documentation is

Other than the original `.txt` documents preserved under `version/<v>/text/`, this is **not** a 1:1 translation of the original specification files. It is intended as **correct reference material** - for understanding and preserving past compatibility, and for building future implementations. Corrections, reconstructions, and editorial notes are clearly marked and cited.

## The versions

| Version | Status of the record |
| --- | --- |
| [1.54](1.54/ripscrip/README.md) | **Fixed history.** RIPscrip 1.54 (July 1993) is the widely deployed classic standard, fully documented by TeleGrafix's published specification: EGA 640×350×16 graphics, drawing primitives, fills, fonts, icons, mouse fields, buttons, and text variables. The Markdown edition mirrors the original published text. |
| [2.30](2.30/ripscrip/README.md) | **Partially unknown.** The last published document is the 2.00 ALPHA 4 draft (December 1994) - ports, data tables, world coordinates, 256-color and direct-RGB modes, audio, DIB bitmaps. But the shipping "RIPscrip-2" engine (RIPterm 2.20/2.30, 1995-1997 - DOS releases throughout, installers and programs alike) moved beyond the draft with no further specification ever published. Recovering that era requires ongoing research and manual reconstruction. |
| [3.1](3.1/ripscrip/README.md) | **Entirely a reconstruction.** RIPscrip 3.x absolutely shipped - released as the RIPtel Visual Telnet 3.0 and 3.1 clients - but TeleGrafix never published a 3.0 language reference, only a December 1996 technical [white paper](3.1/text/RIPScrip-3.x-technical-whitepaper.txt) (by Jeff Reeder). The 3.x edition here is rebuilt from available sources and materials: the RIPtel 3.1 client, its help files and demo-script corpus, and SyncTERM's open-source implementation, with per-claim evidence tags. |
| [next](next/README.md) | **Future enhancements.** A placeholder for forward-looking, unofficial extensions (modern image/audio formats, font handling, UTF-8) that implementations may adopt and that could become a 3.5x/4.x enhancement of the specification. |
| [baseline](baseline/techspecs/README.md) | **Non-RIP baseline.** Specification references beyond RIPscrip itself: the ANSI/VT-x text emulation RIPterm and RIPtel actually documented and shipped (CP437, VT-102, Doorway, auto-sense), plus modern terminals (SyncTERM, icy_term) as reference points with historically unevidenced features explicitly marked. |

## Layout

```
version/
  GLOSSARY.md      Shared, spec-first glossary of terms used across all versions
  <v>/             Pinned release directories: 1.54, 2.30, 3.1
    ripscrip/      Language reference for the content creator (numbered pages)
    techspecs/     Technical specifications for the implementer (numbered pages)
    text/          Original specification text, preserved verbatim
    assets/        Original fonts/icons/audio distributed with that version
  3.1/research/    Reverse-engineering records behind the 3.x reconstruction
  baseline/        Non-RIP references (ANSI/VT-x emulation, modern terminals)
  next/            Forward-looking, unofficial extension proposals
```

Version directories are named for the **definitive release** of each generation - 1.54, 2.30, 3.1 - not the generation (1.x/2.x/3.x), which is how the prose refers to the families.

## How the docs are organized

Each version documents the **same features in the same order**, so version deltas are legible by reading the same numbered section across trees.

- **Two-layer numbering ordered by learning concerns** - chapter `N`, section `N.M`, flat files named `N.M-slug.md`: `1.x` fundamentals (introduction → protocol/command hierarchy → math/coordinates → world view → terminal/ANSI view), `2.x` drawing, `3.x` text, `4.x` media and interactive objects, `5.x` host interaction, `6.x` authoring and files, `7.x` ports/tables/backup areas where the generation has them. The reference chapter is **pinned at `9`** in every version (`9.0` commands · `9.1` text variables · `9.2` version identification · `9.3` host/control characters) so it aligns everywhere; other chapters may be missing or diverge per version.
- **Audience split** - `ripscrip/` serves the **content creator** (language semantics: what commands do, coordinates, what an icon is; no binary internals). `techspecs/` serves the **implementer** (binary layouts, parser edge cases, rendering behavior). Wire syntax is layered across both: authoring-level syntax (base-36/base-64 basics, escaping, line limits) in `ripscrip/`, parser edge cases in `techspecs/`, cross-linked rather than duplicated.
- **Self-contained vs delta** - each version's `ripscrip/` docs are **self-contained**: a creator reads only the version they are working against, so content carried from prior generations is backfilled in. The `techspecs/` stay **delta-based** - a format is documented in full at the earliest version where it appears, and later versions document only what changed (see [CONTRIBUTING.md](../CONTRIBUTING.md#technical-specifications-techspecs)).
- **Format-first, software-only techspecs** - they center on the formats, assuming a modern software implementation; era hardware and driver detail (VESA modes, video registers, audio libraries) is out of scope except where it explains a structure.
- **Shared glossary** - [GLOSSARY.md](GLOSSARY.md) holds the canonical terms, spec-first (TeleGrafix's own names - world coordinates, drawing port, viewport, text window) with modern aliases listed per entry; pages use those terms consistently.

Renderer/implementation guidance (canvas sizes, aspect-ratio policy, asset handling) deliberately lives outside these language docs, in [IMPLEMENTATION.md](IMPLEMENTATION.md).

## Further reading

- [GLOSSARY.md](GLOSSARY.md) - canonical terminology shared by every version
- [HISTORY.md](HISTORY.md) - TeleGrafix and the RIPterm/RIPtel release timeline
- [RIGHTS.md](RIGHTS.md) - trademarks, copyright status, and repository licensing
- [Wikipedia: RIPscrip](https://en.wikipedia.org/wiki/RIPscrip) - general reference
- [BBS Documentary library - RIPscrip](http://www.bbsdocumentary.com/library/PROGRAMS/GRAPHICS/RIPSCRIPT/) - archived specs, white papers, and sample art
