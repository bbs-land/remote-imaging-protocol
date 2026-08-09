# The Remote Imaging Protocol — Version Documentation

RIPscrip (Remote Imaging Protocol Script language) is a vector-graphics scripting language created at TeleGrafix Communications, Inc. (Huntington Beach, California) and introduced in 1992. It let Bulletin Board Systems send interactive, mouse-driven graphical interfaces — lines, shapes, fills, fonts, icons, buttons, and clickable regions — as compact streams of printable ASCII over dial-up modems, at a time when the alternative was plain ASCII/ANSI text. The language was principally designed by Jeff Reeder, with Jim Bergman and Mark Hayton credited alongside him on the original work.

A RIP-capable terminal interpreted the command stream and rendered graphics in a viewport while ordinary text flowed to a separate text window. TeleGrafix's own clients were **RIPterm** (dial-up/modem BBS connections) and later **RIPtel** (telnet) — RIPscrip is the language both spoke — alongside a number of third-party programs. Buttons and mouse fields sent host commands back to the BBS, making full point-and-click services possible over a 2400-baud connection. Conceptually it occupied the niche that Flash and SVG would later fill on the web: compact, resolution-oriented vector scenes over thin pipes.

RIPscrip saw real adoption in the BBS world of 1993–1996 — supported by major BBS packages and terminal programs — though it never displaced ANSI art as the dominant online graphics form. The public World Wide Web drained the BBS ecosystem rapidly in the late 1990s, ending RIPscrip development and, ultimately, TeleGrafix itself. See [HISTORY.md](HISTORY.md) for the company and release timeline, and [RIGHTS.md](RIGHTS.md) for the copyright/trademark situation and this repository's licensing.

## What this documentation is

Other than the original `.txt` documents preserved under `version/<v>/text/`, this is **not** a 1:1 translation of the original specification files. It is intended as **correct reference material** — for understanding and preserving past compatibility, and for building future implementations. Corrections, reconstructions, and editorial notes are clearly marked and cited.

## The versions

| Version | Status of the record |
| --- | --- |
| [1.5x](1.5x/ripscrip/README.md) | **Fixed history.** RIPscrip 1.54 (July 1993) is the widely deployed classic standard, fully documented by TeleGrafix's published specification: EGA 640×350×16 graphics, drawing primitives, fills, fonts, icons, mouse fields, buttons, and text variables. The Markdown edition mirrors the original published text. |
| [2.x](2.x/ripscrip/README.md) | **Partially unknown.** The last published document is the 2.00 ALPHA 4 draft (December 1994) — ports, data tables, world coordinates, 256-color and direct-RGB modes, audio, DIB bitmaps. But the shipping "RIPscrip-2" engine (RIPterm 2.20/2.3, 1995–1997 — DOS releases throughout, installers and programs alike) moved beyond the draft with no further specification ever published. Recovering that era requires ongoing research and manual reconstruction. |
| [3.x](3.x/ripscrip/README.md) | **Entirely a reconstruction.** RIPscrip 3.x absolutely shipped — released as the RIPtel Visual Telnet 3.0 and 3.1 clients — but TeleGrafix never published a 3.0 language reference, only a December 1996 technical [white paper](3.x/whitepaper/README.md) (by Jeff Reeder). The 3.x edition here is rebuilt from available sources and materials: the RIPtel 3.1 client, its help files and demo-script corpus, and SyncTERM's open-source implementation, with per-claim evidence tags. |
| [next](next/README.md) | **Future enhancements.** A placeholder for forward-looking, unofficial extensions (modern image/audio formats, font handling, UTF-8) that implementations may adopt and that could become a 3.5x/4.x enhancement of the specification. |
| [baseline](baseline/techspecs/README.md) | **Non-RIP baseline.** Specification references beyond RIPscrip itself: the ANSI/VT-x text emulation RIPterm and RIPtel actually documented and shipped (CP437, VT-102, Doorway, auto-sense), plus modern terminals (SyncTERM, icy_term) as reference points with historically unevidenced features explicitly marked. |

## Layout per version

```
version/<v>/
  assets/     Original fonts/icons/audio distributed with that version
  ripscrip/   Markdown reference edition of the specification (reconstructed
              with evidence tags for 3.x)
  text/       Original specification text, preserved
  techspecs/  Original documentation of binary formats and implementation
              details (full doc at a format's earliest version; later
              versions document deltas)
```

Renderer/implementation guidance (canvas sizes, aspect-ratio policy, asset handling) deliberately lives outside these language docs, in [IMPLEMENTATION.md](IMPLEMENTATION.md).

## Further reading

- [HISTORY.md](HISTORY.md) — TeleGrafix and the RIPterm/RIPtel release timeline
- [RIGHTS.md](RIGHTS.md) — trademarks, copyright status, and repository licensing
- [Wikipedia: RIPscrip](https://en.wikipedia.org/wiki/RIPscrip) — general reference
- [BBS Documentary library — RIPscrip](http://www.bbsdocumentary.com/library/PROGRAMS/GRAPHICS/RIPSCRIPT/) — archived specs, white papers, and sample art
