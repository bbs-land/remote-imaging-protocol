# Remote Imaging Protocol (RIPscrip)

Documentation and extended information around RIPScrip/RIPTerm/RIPtel specifications.

| **Documentation** | **Terminal** | **Paint** |
| --- | --- | --- |
| [RIPScrip 3.x](version/3.1/ripscrip/README.md) | [RIPTel 3.1](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi) - Windows 7+ (patched, PCMicro) | - |
| [RIPScrip 2.x](version/2.30/ripscrip/README.md) | [RIPTerm 2.30](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTerm%202.30/RIPT2300.zip) DOS | - |
| [RIPScrip 1.5x](version/1.54/ripscrip/README.md) | [RIPTerm 1.54](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTerm%201.54/RIPTM154.ZIP) DOS | [RIPaint 1.52](https://files.bbs.land/rip/RIPaint%20Installers/RIPaint%201.52/RIPAINT.ZIP) DOS |

Missing is a a more recent RIPaint version (RIPaint-2).

More: [Version overview](version/README.md) · [History](version/HISTORY.md) · [Downloads](version/DOWNLOADS.md) · [Rights & licensing](version/RIGHTS.md) · [Future enhancements](version/next/README.md) · [Implementation notes](version/IMPLEMENTATION.md)

## RIPscrip

RIPscrip ("Remote Imaging Protocol Script language") is a text-based script language for displaying online graphics, created by TeleGrafix Communications, Inc. in 1992. It was designed to bring a Graphical User Interface to Bulletin Board Systems (BBSes) over the limited bandwidth of dial-up modems, at a time when most online services were plain ASCII/ANSI text.

**Naming:** RIPscrip is the language itself. **RIPterm** was TeleGrafix's BBS client for dial-up/modem connections, and **RIPtel** its telnet-based successor; RIPaint and RIPdraw were the companion art tools.

Rather than transmitting raster images, RIPscrip is an **object-oriented (vector) drawing language**: scenes are composed of hundreds or thousands of compact drawing operations - lines, circles, polygons, fills, fonts, buttons, and mouse regions - streamed as printable text. Key design points:

- **7-bit ASCII** - commands use only printable ASCII characters, so the protocol works over X.25 networks and other carriers that cannot pass 8-bit binary data, and can be layered onto any existing text-based host without modification.
- **Compact command syntax** - a command line starts with `!` and each command is introduced by the universal delimiter `|`, e.g. `!|c0A|L00010A0E`. Commands are organized into levels (Level-0 graphics primitives, Level-1 user-interface objects such as icons, buttons and mouse fields, and higher levels for ports, tables, and system functions in later versions).
- **"MegaNum" base-36 numbers** - numeric parameters are encoded in hexa-tri-decimal (digits `0-9` and `A-Z`), compacting values to roughly 3/5 of their decimal width.
- **Mixed graphics and text** - RIPscrip statements coexist with ordinary ASCII text and ANSI/VT-100 directives on the same connection; the terminal routes each to a graphics window (viewport) or a TTY text window.
- **Interactivity** - mouse fields, styled buttons, text variables, pop-up lists, and host command templates let the host build clickable, form-driven interfaces; the terminal sends back host commands when the user interacts.
- **Auto-sensing** - a host emits the ANSI query `ESC[!` and a RIPscrip-capable terminal (such as TeleGrafix's RIPterm) responds with its version, while ordinary terminals silently ignore it.

The language was freely licensed for implementation (though not public domain - TeleGrafix retained the copyright), and was adopted by BBS packages and terminal programs beyond TeleGrafix's own RIPterm, RIPaint, and RIPdraw tools.

### Version history

| Version | Date | Notes |
| --- | --- | --- |
| **[1.5x](version/1.54/ripscrip/README.md)** | July 1993 | The widely deployed classic standard: EGA 640×350×16 graphics, drawing primitives, fills, fonts, icons, mouse fields, buttons, and text variables. [RIPScrip-1.54.txt](version/1.54/text/RIPScrip-1.54.txt) |
| **[2.x](version/2.30/ripscrip/README.md)** | December 1994 | A major proposed redesign (never finalized): drawing ports, data tables and backup areas, world coordinates, 256-color and direct-RGB modes, audio playback, DIB bitmaps, and an expanded multi-level command set. [RIPScrip-2.0-alpha-4.txt](version/2.30/text/RIPScrip-2.0-alpha-4.txt) |
| **[3.x](version/3.1/ripscrip/README.md)** | December 1996 | A technical white paper (by Jeff Reeder) summarizing the next-generation architecture and goals - resolution independence, Internet/WWW applications, and multimedia - rather than a full command reference. [White paper](version/3.1/text/RIPScrip-3.x-technical-whitepaper.txt) |
| **[next](version/next/README.md)** | - | Placeholder for future, unofficial enhancements to the specification - modern image/audio formats, font handling, and UTF-8 - candidates for a 3.5x/4.x revision. |

The Markdown editions are split into linked sections suitable for browsing and for generating a documentation website.

### About this documentation

Other than the original `.txt` documents preserved under `version/<v>/text/`, this repository is **not** a 1:1 translation of the specification files - it is meant to be **correct reference material** for past compatibility and future implementations. The state of the record varies by version:

- **1.54 / 1.5x** is fixed history - fully documented by the published specification, converted to Markdown from the original text.
- **2.x** is somewhat unknown - the published record stops at the 2.00 ALPHA 4 draft, while the shipping 2.2-era engine (RIPterm 2.20/2.30) moved beyond it; recovering that era needs further research and manual work.
- **3.x** is entirely a reconstruction from available sources and materials (RIPtel 3.1, its help files and demo corpus, and SyncTERM), with per-claim evidence tags.

See the [version overview](version/README.md) for details.

## Repository layout

```
version/
  1.54/
    assets/     Fonts and icons distributed with RIPterm 1.54
    ripscrip/   Language reference for content creators (numbered pages)
    techspecs/  Binary formats and implementation details (icons, fonts, .RIP files, MegaNum, rendering behavior)
    text/       Original specification text
  2.30/
    assets/     Fonts, icons, and audio distributed with RIPterm Professional 2.0 and RIPterm 2.20/2.30
    ripscrip/   Language reference for the 2.x generation (self-contained, 1.54 material backfilled)
    techspecs/  Formats new in the 2.x products (BMP icons, FastFont .FF1/.RFF, MicroANSI fonts, audio, palettes)
    text/       Original specification text (2.00 alpha 4)
  3.1/
    assets/     Fonts, icons, and demo content distributed with RIPtel 3.1
    research/   Research notes from reverse-engineering 3.0-era artifacts
    ripscrip/   Reconstructed 3.x language reference with per-claim evidence tags
    techspecs/  Deltas from the 2.x formats (FastFont additions, MicroANSI, storage model)
    text/       RIPscrip 3.0 technical white paper (original text)
  baseline/
    techspecs/  Non-RIP baseline references: ANSI/VT-x emulation in RIPterm/RIPtel, modern terminal comparison
  next/         Future, unofficial enhancement proposals (3.5x/4.x candidates)
  README.md     Version overview - the record per version, and how the docs are organized
  GLOSSARY.md   Canonical terminology shared across every version
  HISTORY.md    TeleGrafix company and RIPterm/RIPtel release timeline
  DOWNLOADS.md  Preserved original distributions on the files.bbs.land mirror
  RIGHTS.md     Trademarks, copyright status, and repository licensing
  IMPLEMENTATION.md  Renderer/terminal implementation guidance
```

> **Note:** On 2026-08-08 the git history was rewritten (force-pushed) to convert the `version/3.1/assets/fonts/*.RFF` font binaries to Git LFS pointers throughout history - they had originally been committed as raw blobs. Only the commits touching those files changed; file contents are unaffected. Re-clone (or hard-reset to `origin/main`) if you have an older checkout.

## Trademarks, copyright & licensing

RIPscrip, RIPterm, RIPaint, and RIPdraw were trademarks of TeleGrafix Communications, Inc. The original specification texts reproduced here (under `version/*/text/`) are Copyright © 1992-1997 TeleGrafix Communications, Inc. and are preserved for historical and interoperability purposes; with the company's closure the rights are effectively in limbo - see [version/RIGHTS.md](version/RIGHTS.md) for the full picture.

Unless noted otherwise, all other documentation in this repository is dedicated to the public domain under [CC0 1.0](LICENSE). Future library implementations developed in this repository will be **ISC** licensed for liberal usage.
