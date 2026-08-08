# Fonts — RIPterm Professional 2.0 Distribution

The 48 files in this directory are the font-system assets from TeleGrafix's **RIPterm Professional 2.0** (released January 23–24, 1995; installed files dated 1995-01-27 to 1995-02-01): the complete `FONTS\` sub-directory of an installed copy, plus `RIPTERM.FNT` from the program directory. They are preserved byte-exact (`version/*/assets/**` is `-text`; the `*.CHR`/`*.BGI`/`*.FNT`/`*.FF1`/`*.EXE` patterns store the binaries via Git LFS). Source install: `~/src/rip-tools/RIPTerm2.0/extracted/` (see the provenance table in [CONTRIBUTING.md](../../../../CONTRIBUTING.md)).

RIPTERM.DOC (§2.1.4) requires font files — "`.CHR`, `.FNT`, or `.FF1` file extension" — to live in the `FONTS\` sub-directory under the RIPterm directory.

## Borland BGI stroked fonts (`.CHR`)

The same ten stroked vector fonts distributed with RIPterm 1.54 (see the [1.5x fonts README](../../../1.5x/assets/fonts/README.md) for the format description and RIP_FONT_STYLE font numbers). Eight are byte-identical to their 1.54 counterparts; **`LITT.CHR` and `LCOM.CHR` were revised** for 2.0 (5,131 → 5,151 and 12,083 → 12,079 bytes respectively).

| File       | Size (bytes) | Font                    | vs 1.54     |
| ---------- | -----------: | ----------------------- | ----------- |
| `TRIP.CHR` |       16,677 | Triplex (font 1)        | identical   |
| `LITT.CHR` |        5,151 | Small (font 2)          | **revised** |
| `SANS.CHR` |       13,596 | Sans Serif (font 3)     | identical   |
| `GOTH.CHR` |       18,063 | Gothic (font 4)         | identical   |
| `SCRI.CHR` |       10,987 | Script (font 5)         | identical   |
| `SIMP.CHR` |        8,437 | Simplex (font 6)        | identical   |
| `TSCR.CHR` |       17,355 | Triplex Script (font 7) | identical   |
| `LCOM.CHR` |       12,079 | Complex (font 8)        | **revised** |
| `EURO.CHR` |        8,439 | European (font 9)       | identical   |
| `BOLD.CHR` |       14,670 | Bold (font 10)          | identical   |

`EGAVGA.BGI` (5,554 bytes) is the Borland Graphics Interface EGA/VGA video driver that accompanies the BGI font engine.

## Atech FastFont outline fonts (`.FF1`)

The scalable outline-font engine behind [RIP_EXTENDED_FONT_STYLE](../../ripscrip/08-level-0-commands-a-f.md#rip_extended_font_style) (`!|y`, added in spec revision 2.A1 as "True Type style" fonts). The `.FF1` files are **Atech Software's FastFont format** — `.FF1` is Atech's own extension, and each file (except the rebranded `DEFAULT.FF1`) ends with the trailer `COPR:` + "Copyright 1991, Atech Software, Carlsbad CA". `DEFAULT.FF1`'s trailer instead reads "RIPterm 2.0 font system Copyright TeleGrafix Communications Inc. 1995".

These five families are the **direct ancestors of RIPscrip 3.x's `.RFF` fonts**: RIPtel 3.1 ships all five renamed to `.RFF` ("RIPscrip FastFont", lightly revised — e.g. MARIN 56,112 → 56,526 bytes) and adds three more families (BRUSH, EUREKA, OAKLAND). See the [3.x fonts README](../../../3.x/assets/fonts/README.md) and the [3.x file-formats page](../../../3.x/ripscrip/22-file-formats.md#rff-outline-fonts--atech-fastfont) for the decoded header/style-record layout, which applies to these files as well.

| File | Size (bytes) | Family |
| --- | --: | --- |
| `COBB.FF1` | 62,311 | Cobb |
| `DEFAULT.FF1` | 31,228 | Default (fallback; TeleGrafix-rebranded trailer) |
| `DIXON.FF1` | 41,657 | Dixon |
| `MARIN.FF1` | 56,112 | Marin (used by the shipped demo script [SHADOW.RIP](../icons/README.md#shadowrip)) |
| `SYMBOL.FF1` | 43,187 | Symbol |

`DEMO_ONE.EXE` (117,175 bytes) is the FF1 **font viewer** utility that README.DOC documents: "a utility for viewing the FF1 outline font files used by RIPterm v2.0 … located in the FONTS directory."

## Terminal and system fonts (`.FNT`)

- `RIPTERM.FNT` (263,585 bytes) — header `RIPterm v2.0 MicroANSI Font File`: the text-window (TTY/ANSI) font container, backing the five selectable System Fonts (80×25, 80×43, 91×25, 91×43, 40×25 — RIPTERM.DOC §4.4). It shares only its name with 1.54's 18,596-byte `RIPTERM.FNT`; the format is new, and the same MicroANSI header/lineage continues in RIPtel 3.1's `RIPscrip.maf` (see the [3.x notes](../../../3.x/ripscrip/06-color-audio-text.md)).
- Four per-resolution system fonts used for RIPterm's own UI at each video mode; internal headers name the character sets:

| File           | Size (bytes) | Header        |
| -------------- | -----------: | ------------- |
| `0640X350.FNT` |        3,211 | `sys08 8x8`   |
| `0640X480.FNT` |        3,723 | `SYS16 8x13`  |
| `0800X600.FNT` |        4,165 | `SYS 24 9x13` |
| `1024X768.FNT` |        5,573 | `SYS 24 9x13` |

## UI widget images (`.IMG`)

26 small images for RIPterm's check-box and radio-button controls, pre-scaled per display resolution (640×350, 640×480, 800×600, 1024×768, and a 1024-variant set) — direct evidence of 2.0's resolution-scaled UI. The format is undocumented: a header of 16-bit little-endian words (width, height, then four more words) followed by planar 16-color pixel data.

| Prefix | Files | Content (width×height per resolution) |
| --- | --- | --- |
| `CHK350/480/600/768/1024.IMG` | 5 | Check-mark glyph: 7×8, 8×10, 9×12, 13×16, 15×24 |
| `CK350/480/600/768/1024_1/_2.IMG` | 10 | Check-box, two states each: 16×12, 16×16, 20×20, 25×25, 32×34 |
| `RD350/480/600/768/1024_1/_2.IMG` | 10 | Radio button, two states each: 15×11, 16×17, 20×21, 25×25, 32×35 |
| `TELEGRFX.IMG` | 1 | 16×12 TeleGrafix glyph |
