# Fonts — RIPterm 1.54 Distribution

The 11 files in this directory are the font assets from TeleGrafix's original RIPterm 1.54 distribution (`RIPTM154.ZIP`, released July 15, 1993; all files in the archive are dated 1993-07-15). They are preserved byte-exact: the repo's `.gitattributes` marks `version/*/assets/**` as `-text` (no line-ending normalization), and the `*.CHR` / `*.FNT` patterns store the binaries via Git LFS.

RIPTERM.DOC lists all ten `.CHR` files plus `RIPTERM.FNT` as required parts of the distribution ("RIPterm may not be distributed unless accompanied by all of the following files"). In a RIPterm installation the `.CHR` files lived in the `FONTS\` sub-directory; `RIPTERM.FNT` sat alongside `RIPTERM.EXE`.

## Borland BGI stroked fonts (`.CHR`)

The `.CHR` files are Borland Graphics Interface (BGI) **stroked fonts**: vector fonts in which each glyph is a sequence of move/draw stroke opcodes rather than a bitmap, so they scale cleanly to any of RIPscrip's ten magnification sizes. Each file begins with the signature `PK\b\b` and an embedded banner (`BGI Stroked Font V1.1 ... Copyright (c) 1987,1988 Borland International`). The format is documented in Borland's BGI Toolkit documentation, and any Borland BGI font editor can read or modify these files — WHATSNEW.DOC notes that RIPterm externalized its graphical fonts into these disk files precisely so they would be "easily editable with any available Borland BGI font editor."

These are the fonts selected by the [RIP_FONT_STYLE](../../ripscrip/06-text-output.md#rip_font_style) command (`!|Y`) and drawn by [RIP_TEXT](../../ripscrip/06-text-output.md#rip_text) / [RIP_TEXT_XY](../../ripscrip/06-text-output.md#rip_text_xy). Fonts 1–4 date from the original RIPscrip 1.x releases; fonts 5–10 (`05`–`0A`) were added during the 1.5x series (WHATSNEW.DOC lists them, matching the 1.54 spec's font table).

## `RIPTERM.FNT` — the terminal's bitmap font file

`RIPTERM.FNT` is RIPterm's **system font data file** (its header reads "RIPterm Font File"). It is not a BGI font; it is a container of five bitmap character sets — `8x8`, `7x8`, `8x14`, `7x14`, and `16x14` — which back RIPterm's five selectable System Fonts for the text/ANSI window (80x43, 91x43, 80x25, 91x25, and 40x25 text modes respectively). RIP_FONT_STYLE font 0, the "Default 8x8 font," is likewise bit-mapped rather than stroked; in sizes above 1 its pixels are simply enlarged, giving a jagged look (see the spec note in [06-text-output.md](../../ripscrip/06-text-output.md#rip_font_style)).

## File reference

| File | Size (bytes) | Font name (per RIPTERM.DOC) | RIP_FONT_STYLE font # | Description |
| --- | --: | --- | --- | --- |
| `RIPTERM.FNT` | 18,596 | RIPterm system font data file | 0 (8x8 default) | Bitmap font container: 8x8, 7x8, 8x14, 7x14, 16x14 character sets for the text window / MicroANSI display |
| `TRIP.CHR` | 16,677 | Triplex Font | 1 | Serif "triplex" stroked font (three-stroke weight) |
| `LITT.CHR` | 5,131 | Small Font | 2 | Compact single-stroke font for small text |
| `SANS.CHR` | 13,596 | Sans Serif Font | 3 | Sans-serif stroked font |
| `GOTH.CHR` | 18,063 | Gothic Font | 4 | Gothic (Old English / blackletter) stroked font |
| `SCRI.CHR` | 10,987 | Script Font | 5 | Cursive script stroked font |
| `SIMP.CHR` | 8,437 | Simplex Font | 6 | Plain single-stroke ("simplex") font |
| `TSCR.CHR` | 17,355 | Triplex Script Font | 7 | Script font with triplex stroke weight |
| `LCOM.CHR` | 12,083 | Complex Font | 8 | Serif "complex" stroked font |
| `EURO.CHR` | 8,439 | European Font | 9 | European-style stroked font |
| `BOLD.CHR` | 14,670 | Bold Font | 10 (`0A`) | Bold outline stroked font (WHATSNEW.DOC: "Bold Font (outline font)") |

Font numbers 0–10 (`00`–`0A` in the protocol's 2-digit "meganum" text encoding) are exactly as tabulated in the 1.54 specification's [RIP_FONT_STYLE command](../../ripscrip/06-text-output.md#rip_font_style). Font 0 is the only bit-mapped entry; fonts 1–10 are scalable stroked fonts served by the `.CHR` files above. Parsing details for the `.CHR` format are in Borland's BGI documentation; documenting the stroked/bitmap font formats within this repository is tracked in the repo TODO.
