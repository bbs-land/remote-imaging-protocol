# RIPscrip 1.5x Technical Specifications

Original technical documentation for the binary formats and implementation behaviors of the RIPscrip 1.5x family (RIPterm 1.52/1.54, RIPaint 1.52). These pages are **not** part of TeleGrafix's specification text — they document, correct, and extend it from byte-level evidence: the original distribution files preserved in [`../assets/`](../assets/fonts/README.md), the RIPterm 1.54 install (`~/src/rip-tools/RIPterm154/`), and the open-source reference implementations catalogued in [CONTRIBUTING.md](../../../CONTRIBUTING.md#reference-repositories-srcrip-tools). Every reverse-engineered claim is cited; anything unverifiable is marked as such.

Formats are documented here in the earliest version family where they appear (see [CONTRIBUTING.md](../../../CONTRIBUTING.md#technical-specifications-techspecs)); the 2.x and 3.x techspecs document only deltas and link back to these pages.

## Contents

1. [Icon File Format (`.ICN`, `.MSK`, `.HIC`)](icon-format.md) — the planar EGA icon container: exact byte layout, the size formula the spec appendix gets wrong, mask files, and hot icons
2. [BGI Stroked Fonts (`.CHR`)](bgi-stroked-fonts.md) — the Borland vector font format behind RIP_FONT_STYLE fonts 1–10: header, stroke opcodes, metrics, scaling, and the EGAVGA.BGI driver
3. [Bitmap Fonts (`RIPTERM.FNT`)](bitmap-fonts.md) — RIPterm's five-charset system font container and the RIP_FONT_STYLE font 0 (default 8x8) behavior
4. [`.RIP` File Format](rip-file-format.md) — on-disk conventions for RIPscrip scene files: line structure, continuation, escaping, embedded ANSI, CRLF/CP437 storage, and prologue/epilogue conventions observed in real files
5. [MegaNum Encoding](meganum-encoding.md) — the base-36 numeric encoding: digit set, field widths, and reader/writer edge cases as actually implemented
6. [Terminal Behavior](terminal-behavior.md) — behaviors the 1.54 spec under-specifies, reconstructed from reference implementations: clipping, write modes, fill and line pattern semantics, icon stamping, font scaling

## Primary sources

- Specification: [`RIPScrip-1.54.txt`](../text/RIPScrip-1.54.txt) (verbatim) and the [Markdown reference edition](../ripscrip/README.md)
- Original binaries: [`../assets/fonts/`](../assets/fonts/README.md) (10 `.CHR` + `RIPTERM.FNT`) and [`../assets/icons/`](../assets/icons/README.md) (184 `.ICN`), plus the full RIPterm 1.54 install (`~/src/rip-tools/RIPterm154/DOS/RIPTERM/`, incl. `.MSK`/`.HIC` files and `RIPTERM.DOC`) and the RIPaint 1.52 distribution (`~/src/rip-tools/artifacts/ripaint-1.52/RIPAINT.ZIP`, incl. sample `.RIP` scenes and `RIPAINT.HLP`)
- Reference implementations: `sbbs:src/syncterm/ripper.c` (SyncTERM), `RIPtermJS:src/BGI.js` / `src/ripterm.js`, `icy_tools:crates/icy_parser_core/src/rip/`, `pablodraw:Source/Pablo/Formats/Rip/`
