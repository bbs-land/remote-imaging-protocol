# Fonts — RIPtel 3.1 `FONTS\` directory

The complete `FONTS\` directory from the RIPtel Visual Telnet 3.1 install (TeleGrafix Communications, Oct 1997; RIPscrip driver 3.0.7), copied verbatim — 20 files. This is the font inventory a real RIPscrip 3.0 terminal shipped with, covering all three of the protocol's text systems:

- **10 Borland BGI stroked fonts (`.CHR`)** — the classic RIPscrip 1.54 vector font set, selected on the wire by [`Y` RIP_FONT_STYLE](../../ripscrip/10-level-0-commands-s-z.md#rip_font_style) (font IDs 01–0A).
- **8 Atech FastFont outline fonts (`.RFF`)** — the RIPscrip 3.0 scalable outline-font system, selected by name via [`y` RIP_EXTENDED_FONT_STYLE](../../ripscrip/10-level-0-commands-s-z.md#rip_extended_font_style); each file exposes 10 style variants (see below).
- **`RIPscrip.maf`** — bitmap fonts for the ANSI/text terminal emulation, organized per screen resolution.
- **`atf.cfg`** — the outline-font engine's binary catalog cache.

Format internals are documented byte-level in the [binary-format triage](../../research/riptel-binary-formats.md); recovered help text on the font systems is in the [help extraction](../../research/riptel-help-extraction.md); the condensed reference pages are in [File Formats](../../ripscrip/22-file-formats.md). The definitive format techspecs are the 2.x [FastFont](../../../2.x/techspecs/fastfont-fonts.md) and [MicroANSI](../../../2.x/techspecs/microansi-fonts.md) pages plus the 3.x deltas in [`version/3.x/techspecs/`](../../techspecs/README.md) ([RFF additions](../../techspecs/rff-additions.md), [RIPscrip.maf](../../techspecs/maf-fonts.md)).

## Inventory

| File | Size (bytes) | Format | Role |
| --- | --: | --- | --- |
| `atf.cfg` | 4,470 | Binary ATF catalog (not text) | Pre-scanned registry for the outline-font engine: 8 entries caching each `.RFF` header verbatim, followed by all 80 style records (8 fonts × 10 styles, 46-byte records). Machine-written by RIPtel; safe to regenerate. |
| `BOLD.CHR` | 14,670 | Borland BGI stroked font (build Jun 5 1989) | 1.54 vector font `BOLD` — Bold, `Y` font ID 0A |
| `BRUSH.RFF` | 87,155 | Atech FastFont outline | "Brush" family + 9 style variants (contains leaked PostScript tokens — likely converted from a Type 1 source by Atech AllType) |
| `COBB.RFF` | 62,725 | Atech FastFont outline | "Cobb" family + 9 style variants; used by the font demos |
| `DEFAULT.RFF` | 31,596 | Atech FastFont outline | "Default" family + 9 style variants — the fallback outline font |
| `DIXON.RFF` | 42,071 | Atech FastFont outline | "Dixon" family + 9 style variants; used by the font demos |
| `EUREKA.RFF` | 70,789 | Atech FastFont outline | "Eureka" family + 9 style variants |
| `EURO.CHR` | 8,439 | Borland BGI stroked font (build May 17 1989) | 1.54 vector font `EURO` — European, `Y` font ID 09 |
| `GOTH.CHR` | 18,063 | Borland BGI stroked font (build Jun 5 1989) | 1.54 vector font `GOTH` — Gothic, `Y` font ID 04 |
| `LCOM.CHR` | 12,079 | Borland BGI stroked font (build Feb 11 1994) | 1.54 vector font `LCOM` — Complex, `Y` font ID 08; rebuilt with 1994 Borland tooling, format unchanged |
| `LITT.CHR` | 5,151 | Borland BGI stroked font (build Jun 12 1993) | 1.54 vector font `LITT` — Small, `Y` font ID 02; rebuilt with 1993 Borland tooling, format unchanged |
| `MARIN.RFF` | 56,526 | Atech FastFont outline | "Marin" family + 9 style variants — the demo corpus's workhorse outline font (menu titles, dropshadow text) |
| `OAKLAND.RFF` | 64,213 | Atech FastFont outline | "Oakland" family + 9 style variants |
| `RIPscrip.maf` | 270,945 | "RIPterm v2.0 MicroANSI Font File" | Bitmap terminal-emulation fonts: six resolution records (640×480 VGA, 800×600, 1024×768, plus "Small 640x480"/799×599/1023×767 variants), each pointing at 5 fixed-cell font sizes covering the full 256-char CP437 set |
| `SANS.CHR` | 13,596 | Borland BGI stroked font (build Jun 5 1989) | 1.54 vector font `SANS` — Sans Serif, `Y` font ID 03 |
| `SCRI.CHR` | 10,987 | Borland BGI stroked font (build Jun 5 1989) | 1.54 vector font `SCRI` — Script, `Y` font ID 05 |
| `SIMP.CHR` | 8,437 | Borland BGI stroked font (build Jun 5 1989) | 1.54 vector font `SIMP` — Simplex, `Y` font ID 06 |
| `SYMBOL.RFF` | 43,601 | Atech FastFont outline | "Symbol" family + 9 style variants — symbol/pi glyphs (name inference) |
| `TRIP.CHR` | 16,677 | Borland BGI stroked font (build Jun 5 1989) | 1.54 vector font `TRIP` — Triplex, `Y` font ID 01 |
| `TSCR.CHR` | 17,355 | Borland BGI stroked font (build Aug 3 1989) | 1.54 vector font `TSCR` — Triplex Script, `Y` font ID 07 |

## The three font systems

### BGI stroked fonts (`.CHR`)

Standard Borland BGI Stroked Font V1.1 files — the exact RIPscrip 1.54-era set (TRIP LITT SANS GOTH SCRI SIMP TSCR LCOM EURO BOLD), carried forward unchanged so `Y` RIP_FONT_STYLE keeps working in 3.0. LCOM and LITT carry later BGI toolkit build dates (1994/1993 vs 1989 for the rest) — rebuilt with a newer Borland font editor, but the format is the known [BGI `.CHR` spec](../../ripscrip/22-file-formats.md#chr--borland-bgi-stroked-fonts).

### Atech FastFont outline fonts (`.RFF`)

The RIPscrip 3.0 scalable-font system ("Adobe and TrueType style fonts" per the RIPtel help). The files are Atech Software's FastFont format renamed `.RFF` ("RIPscrip FastFont") — every file ends with an Atech copyright trailer, and leaked buffer garbage names `.FF1` files and Atech's AllType converter. The lineage is now directly confirmed: **RIPterm Professional 2.0 (January 1995) shipped five of these families as literal Atech `.FF1` files** — COBB, DEFAULT, DIXON, MARIN, SYMBOL, preserved in the [2.x font assets](../../../2.x/assets/fonts/README.md) — which were reformatted to `.RFF` for **RIPterm 2.30** (files dated April 1997; a header reorganization rather than a plain rename — see the [2.x `.RFF` notes](../../../2.x/assets/fonts/README.md#ripscrip-fastfont-outline-fonts-rff--ripterm-230)); RIPtel 3.1 ships those five byte-identical and extends the set with the BRUSH, EUREKA, and OAKLAND families. Each `.RFF` exposes **10 logical fonts**: the base face plus Thin (`Th`), Condensed (`Cn`), Wide (`Wd`), Expanded (`Ex`), and Hollow versions of all five (`Ho`, `HT`, `HC`, `HW`, `HE`). On the wire, [`y` RIP_EXTENDED_FONT_STYLE](../../ripscrip/10-level-0-commands-s-z.md#rip_extended_font_style) selects them by name string — `Marin`, `Cobb Cn`, `Dixon HE`, … — with size, rotation, bold, and dropshadow encoded in the fixed fields (TeleGrafix's own field-layout crib survives in the demo corpus: `sfFFFFZZOOSSCCBBCCWWRRRRRR`, FONTS.RIP). Header, style-record, and kerning-trailer layouts are decoded in the [binary-format triage](../../research/riptel-binary-formats.md#1-rff-outline-fonts-fontsrff-8-files--atech-software-fastfont-scalable-outline-format-h); glyph outline encoding remains undecoded. `atf.cfg` is the engine's startup cache of all 8 headers + 80 style records — see also [File Formats](../../ripscrip/22-file-formats.md#rff-outline-fonts--atech-fastfont).

### MicroANSI bitmap fonts (`RIPscrip.maf`)

Magic header `RIPterm v2.0 MicroANSI Font File` (the 3.0 engine is the renamed RIPterm 2.0 engine). Per-resolution records — 640×480, 800×600, 1024×768, and their "small" siblings — each carry five fixed-cell bitmap fonts (e.g. 8×11) for the full 256-character set. These render the ANSI/VT-102 terminal emulation and text windows, matching RIPtel's five terminal-font choices; see [`.maf` in File Formats](../../ripscrip/22-file-formats.md#maf--microansi-bitmap-fonts).

## Storage notes

Everything under `version/*/assets/**` is stored byte-exact (`-text` in the repo [.gitattributes](../../../../.gitattributes)); the `.CHR` files are additionally tracked via Git LFS. `atf.cfg`, the `.RFF` files, and `RIPscrip.maf` are binary despite their non-LFS extensions — do not open them with line-ending-normalizing tools.
