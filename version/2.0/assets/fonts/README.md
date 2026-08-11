# Fonts - RIPterm 2.x Distributions

The 54 files in this directory are the font-system assets of the RIPterm 2.x line, from two recovered installs:

- **RIPterm Professional 2.0** (released January 23-24, 1995; installed files dated 1995-01-27 to 1995-02-01) - 48 files: the complete `FONTS\` sub-directory of an installed copy, plus `RIPTERM.FNT` from the program directory. Source: `~/src/rip-tools/RIPTerm2.0/extracted/`.
- **RIPterm 2.30** (shareware "Evaluation Edition", October 27, 1997) - 6 files: the five `.RFF` outline fonts (dated 1997-04-18/19, the only 1997-dated assets in the entire 2.30 install) plus `ATF.CFG` (carried unchanged from the 2.20 build of 1995-11-28). Source: `~/src/rip-tools/artifacts/ripterm-2.30/extracted/FONTS/`.

Everything is preserved byte-exact (`version/*/assets/**` is `-text`; the `*.CHR`/`*.BGI`/`*.FNT`/`*.FF1`/`*.RFF`/`*.EXE` patterns store the binaries via Git LFS). See the provenance table in [CONTRIBUTING.md](../../../../CONTRIBUTING.md).

The shipped `FONTS\` directory evolved across the line: 2.0 shipped 10 `.CHR` + 5 `.FF1` + per-resolution `.FNT` system fonts + 26 widget `.IMG`s + `DEMO_ONE.EXE`/`EGAVGA.BGI`; the 2.20/2.30 installs pare this down to 10 `.CHR` + the five outline fonts + `ATF.CFG` (the `.FNT`/`.IMG` UI assets evidently moved into the program's resource files - the `RIPTERM.RES` container and matching EXE strings exist, though the packaging is unproven since the RES directory format is undecoded - and the MicroANSI `RIPTERM.FNT` was revised and renamed `RIPTERM.MAF` - same 263,585-byte size and "RIPterm v2.0 MicroANSI Font File" header, new content).

RIPTERM.DOC (§2.1.4) requires font files - "`.CHR`, `.FNT`, or `.FF1` file extension" - to live in the `FONTS\` sub-directory under the RIPterm directory.

## Borland BGI stroked fonts (`.CHR`)

The same ten stroked vector fonts distributed with RIPterm 1.54 (see the [1.5x fonts README](../../../1.54/assets/fonts/README.md) for the format description and RIP_FONT_STYLE font numbers). Eight are byte-identical to their 1.54 counterparts; **`LITT.CHR` and `LCOM.CHR` were revised** for 2.0 (5,131 → 5,151 and 12,083 → 12,079 bytes respectively).

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

The scalable outline-font engine behind [RIP_EXTENDED_FONT_STYLE](../../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style) (`!|y`, added in spec revision 2.A1 as "True Type style" fonts). The `.FF1` files are **Atech Software's FastFont format** - `.FF1` is Atech's own extension, and each file (except the rebranded `DEFAULT.FF1`) ends with the trailer `COPR:` + "Copyright 1991, Atech Software, Carlsbad CA". `DEFAULT.FF1`'s trailer instead reads "RIPterm 2.0 font system Copyright TeleGrafix Communications Inc. 1995".

These five families are the **direct ancestors of RIPscrip 3.x's `.RFF` fonts** - and the recovered RIPterm 2.30 install pins the transition to **RIPterm 2.30, not RIPtel**: 2.30 replaces all five `.FF1` files with `.RFF` conversions dated April 1997 (see the next section). See the [3.x fonts README](../../../3.0/assets/fonts/README.md) and the [3.x file-formats page](../../../3.0/ripscrip/6.1-content-file-roles.md#fonts-are-not-content-files) for the decoded header/style-record layout.

| File | Size (bytes) | Family |
| --- | --: | --- |
| `COBB.FF1` | 62,311 | Cobb |
| `DEFAULT.FF1` | 31,228 | Default (fallback; TeleGrafix-rebranded trailer) |
| `DIXON.FF1` | 41,657 | Dixon |
| `MARIN.FF1` | 56,112 | Marin (used by the shipped demo script [SHADOW.RIP](../icons/README.md#shadowrip)) |
| `SYMBOL.FF1` | 43,187 | Symbol |

`DEMO_ONE.EXE` (117,175 bytes) is the FF1 **font viewer** utility that README.DOC documents: "a utility for viewing the FF1 outline font files used by RIPterm v2.0 … located in the FONTS directory."

## RIPscrip FastFont outline fonts (`.RFF`) - RIPterm 2.30

RIPterm 2.30 (October 1997) ships the same five families **converted from `.FF1` to `.RFF`** - a reformat, not a rename: the magic differs (e.g. `DIXON.FF1` starts `bd 7a 00 01`, `DIXON.RFF` starts `3b a3 00 00`), the header fields are reordered, and the glyph payload matches from about offset 0x38 on. RIPterm 2.30's `RIPTERM.EXE` correspondingly changes its font-browser filter from `%s*.ff1` to `%s*.rff`. All five files are **byte-identical (md5) to the copies RIPtel 3.1 ships** - the RIPscrip-3 font set originates here; RIPtel only adds three more families (BRUSH, EUREKA, OAKLAND).

| File          | Size (bytes) | File date  | Family (vs `.FF1` size) |
| ------------- | -----------: | ---------- | ----------------------- |
| `COBB.RFF`    |       62,725 | 1997-04-18 | Cobb (62,311)           |
| `DEFAULT.RFF` |       31,596 | 1997-04-19 | Default (31,228)        |
| `DIXON.RFF`   |       42,071 | 1997-04-18 | Dixon (41,657)          |
| `MARIN.RFF`   |       56,526 | 1997-04-18 | Marin (56,112)          |
| `SYMBOL.RFF`  |       43,601 | 1997-04-18 | Symbol (43,187)         |

`ATF.CFG` (684 bytes, dated 1995-11-28) is the Atech font-manager configuration file shipped alongside the fonts in the 2.20/2.30 `FONTS\` directory (byte-identical in both; RIPtel 3.1's `atf.cfg` differs).

## Terminal and system fonts (`.FNT`)

- `RIPTERM.FNT` (263,585 bytes) - header `RIPterm v2.0 MicroANSI Font File`: the text-window (TTY/ANSI) font container, backing the five selectable System Fonts (80×25, 80×43, 91×25, 91×43, 40×25 - RIPTERM.DOC §4.4). It shares only its name with 1.54's 18,596-byte `RIPTERM.FNT`; the format is new, and the same MicroANSI header/lineage continues in RIPtel 3.1's `RIPscrip.maf` (see the [3.x notes](../../../3.0/ripscrip/2.0-color-modes-and-palettes.md)).
- Four per-resolution system fonts used for RIPterm's own UI at each video mode; internal headers name the character sets:

| File           | Size (bytes) | Header        |
| -------------- | -----------: | ------------- |
| `0640X350.FNT` |        3,211 | `sys08 8x8`   |
| `0640X480.FNT` |        3,723 | `SYS16 8x13`  |
| `0800X600.FNT` |        4,165 | `SYS 24 9x13` |
| `1024X768.FNT` |        5,573 | `SYS 24 9x13` |

## UI widget images (`.IMG`)

26 small images for RIPterm's check-box and radio-button controls, pre-scaled per display resolution (640×350, 640×480, 800×600, 1024×768, and a 1024-variant set) - direct evidence of 2.0's resolution-scaled UI. The format is undocumented: a header of 16-bit little-endian words (width, height, then four more words) followed by planar 16-color pixel data.

| Prefix | Files | Content (width×height per resolution) |
| --- | --- | --- |
| `CHK350/480/600/768/1024.IMG` | 5 | Check-mark glyph: 7×8, 8×10, 9×12, 13×16, 15×24 |
| `CK350/480/600/768/1024_1/_2.IMG` | 10 | Check-box, two states each: 16×12, 16×16, 20×20, 25×25, 32×34 |
| `RD350/480/600/768/1024_1/_2.IMG` | 10 | Radio button, two states each: 15×11, 16×17, 20×21, 25×25, 32×35 |
| `TELEGRFX.IMG` | 1 | 16×12 TeleGrafix glyph |
