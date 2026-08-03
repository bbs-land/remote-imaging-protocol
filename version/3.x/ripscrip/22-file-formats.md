# File Formats

[◀ Prev: Text Variables: Environment, Clipboard, Screen & Tables](21-text-variables-environment.md) · [Contents](README.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

This page summarizes every file format a RIPscrip 3.0 client reads or writes, as evidenced by the RIPtel 3.1 install. Where the 2.x edition documented [one format](../../2.x/ripscrip/21-icon-file-format.md) (the BMP/DIB icon), the 3.0 client ships a whole family: images, three font systems, a resource container, and a variable database. Full byte-level layouts live in the research appendix, [`riptel-binary-formats.md`](../research/riptel-binary-formats.md) — this page is the reference summary.

| Format | Role | Evidence |
|---|---|---|
| `.ICN` | Legacy 1.54 icons (read + converted only) | HLP |
| `.BMP` | Primary bitmap format (icons, buttons, wallpaper) | 2.00a4; WP; corpus |
| `.BMH` | Button-highlight BMP variant | corpus (ICONS/*.BMH) |
| `.JPG` | Compressed photographic images (incl. GrayPEG) | WP; HLP; corpus |
| `.RFF` | Scalable outline fonts (Atech FastFont) | corpus (FONTS/*.RFF) |
| `atf.cfg` | Outline-font engine catalog/cache | corpus (FONTS/atf.cfg) |
| `.maf` | MicroANSI bitmap terminal fonts | corpus (FONTS/RIPscrip.maf) |
| `.CHR` | Borland BGI stroked (vector) fonts | 2.00a4; corpus |
| `RIPSCRIP.RES` | Client resource container | corpus; HLP |
| `RIPSCRIP.DB` | Persistent text-variable database | HLP; corpus |

## Legacy `.ICN` Icons (RIPscrip 1.54)

The 1.54 icon format — Borland `getimage()` memory dumps, documented in the [1.5x edition](../../1.5x/ripscrip/20-icon-file-format.md) — is not a native 3.0 format, but the 3.0 driver retains read support by *converting* legacy icons to BMP on the fly: the DLL exports `RIP_ConvertIconToBmp` and carries the error strings "Can't convert RIP 1.54 Icon %s to 2.0 BMP format!" and "1.54 icon file %s has an invalid width". No `.ICN` files ship with RIPtel; hosts are expected to serve BMPs.

*Evidence: HLP (RIPSCRIP.HLP error strings; RIPTEL.EXE export table).*

## Windows BMP / DIB

The 2.00a4 decision to adopt the Windows Device-Independent Bitmap stands in 3.0. Supported: **monochrome (1-bit), 16-color (4-bit), 256-color (8-bit), and 24-bit uncompressed** BMPs. Explicitly *not* supported: RLE-compressed BMPs ("Can't show compressed bitmaps") and OS/2-style BMPs. Display options include stretching to any size, dithering, transparency, color-palette manipulation, and wallpapering. The complete header/color-table/pixel-block layout is documented in the [2.x Icon/DIB File Format page](../../2.x/ripscrip/21-icon-file-format.md) and is unchanged.

BMPs are pervasive in the corpus (102 of the 234 files in `ICONS/`): backgrounds, button skins (via [RIP_LOAD_BITMAP](11-level-1-commands.md)), and UI décor.

*Evidence: 2.00a4; WP (§3.7.1); HLP ("Can't show compressed bitmaps"); corpus.*

### `.BMH` — Button-Highlight Variants

A `.BMH` file is a **plain Windows BMP** whose extension marks it as the *highlighted/pressed* state of a same-named `.BMP` UI skin. RIPtel ships five pairs (BUTTON, CHECKBOX, RADIO, RADIOBUT, RADIONEW): each pair has identical dimensions, bit depth, and palette, differing only in pixel data (the `.BMH` shifted toward white/highlight tones). The client swaps the `.BMH` image in on hover/click for BMP-skinned buttons, checkboxes, and radio buttons.

*Evidence: corpus (ICONS/*.BMH byte comparison — see [binary triage §3](../research/riptel-binary-formats.md)).*

## JPEG (and GrayPEG)

JPEG remains the only compressed image format in shipping 3.0 (GIF was dropped over LZW licensing; PNG was announced but never shipped — see the [white paper](../whitepaper/05-interface-and-display.md)). All JPEGs are 24-bit; the white paper notes support for the **GrayPEG** grayscale variants. JPEG decoding is built into the driver (`jpegShow`, `LoadJPEGBuffer`, JPEG scaling-table error strings), images can be placed anywhere at any size, and progressive ("display while downloading") rendering was advertised but noted as not working in RIPtel 3.1. Seven JPEGs ship in the demo corpus, driven by [RIP_IMAGE](11-level-1-commands.md) and the `$(file.JPG$` [playback prefix](15-local-playback-popup-lists.md).

*Evidence: WP (§3.7.2, GrayPEG); HLP (jpeg* strings; readme progressive-display note); corpus (N2_PHOTO.RIP, SPECLEFX.RIP).*

## `.RFF` Outline Fonts — Atech FastFont

The scalable [outline-font system](10-level-0-commands-s-z.md) is powered by a **licensed third-party engine: Atech Software's "FastFont"** (of Atech's AllType font-conversion product). Every shipped `.RFF` ends with the trailer `COPR:` + "Copyright 1991, Atech Software, Carlsbad CA", and leaked buffer garbage inside the files names `*.FF1` (Atech's own extension) — `.RFF` is evidently "RIPscrip FastFont", renamed. Key structural facts (byte-accurate layout in [binary triage §1](../research/riptel-binary-formats.md)):

- **Identification:** no magic at offset 0 (the first u32 is the trailer offset); reliable signature bytes `10 00 02 02 2E 00 36 00 02 04 98 44` at offset 0x10 and the sentinel `&T` at 0x34.
- **Glyph complement:** 224 glyphs, characters 0x20–0xFF, on a 1000-units-per-em square.
- **Ten style variants per file:** a 10-record style table (46 bytes each) exposes base, `Th`in (85% width), `Cn` condensed (75%), `Wd` wide (125%), `Ex` expanded (150%), plus hollow versions of all five (`Ho`, `HT`, `HC`, `HW`, `HE`) — exactly the family-suffix names the corpus passes in extended-font-style commands ("Marin", "Cobb Th", …).
- **Trailer:** a kerning-pair table (char pair + signed delta in 1/1000 em) followed by the Atech copyright.
- **Shipped faces (8):** BRUSH, COBB, DEFAULT, DIXON, EUREKA, MARIN, OAKLAND, SYMBOL.

`FONTS/atf.cfg` is the engine's binary **catalog/cache**: filename + header copy for each RFF, then all 80 style records — a pre-scanned registry mapping style names to files so the rasterizer needn't open every font at startup. It is machine-generated and safe to rebuild.

*Evidence: corpus (FONTS/*.RFF, atf.cfg — [binary triage §1](../research/riptel-binary-formats.md)); corpus (FONTS.RIP, TEL3X3.MNU font usage); WP (§3.4.1 outline-font system).*

## `.maf` — MicroANSI Bitmap Fonts

`FONTS/RIPscrip.maf` holds the fixed-cell fonts used for ANSI/text-window emulation. Magic string: `\x04 "RIPterm v2.0 MicroANSI Font File" \x04\n\r\x00\x1a`. The file contains **six resolution records** ("640x480 VGA", "800x600 - VGA", "1024x768 - VGA", "Small 640x480", "799x599", "1023x767"), each pointing at **five font-size subrecords** of `cellW × cellH` raw 8-bit glyph bitmaps for the full 256-character (CP437) set. These are the per-resolution renderings behind the five terminal text modes (80×43, 91×43, 80×25, 91×25, 40×25) and the `$TWFONT$` "MicroANSI" font numbers.

*Evidence: corpus (FONTS/RIPscrip.maf — [binary triage §1](../research/riptel-binary-formats.md)); HLP (MicroANSI font-loading error strings; MESSAGES.HLP text-mode list).*

## `.CHR` — Borland BGI Stroked Fonts

The classic 1.54 vector "system font" set survives untouched: ten standard Borland BGI stroked-font files (BOLD, EURO, GOTH, LCOM, LITT, SANS, SCRI, SIMP, TRIP, TSCR), each beginning `PK\x08\x08` + "BGI Stroked Font V1.1". The format is Borland's, publicly documented, and unchanged; nothing RIPscrip-specific was added (LCOM/LITT were merely rebuilt with a newer Borland toolkit).

*Evidence: 2.00a4 (system-font lineage); corpus (FONTS/*.CHR); HLP ("BGI font" error strings).*

## `RIPSCRIP.RES` — Resource Container

The client's resource file, required at startup ("resource file RIPscrip.res required"). Magic string: `\x04 "RIPterm v2.0 Resource File" \x04\n\r\x00\x1a`, followed by a u16 section count (6) and a directory. Observed contents: a high-entropy blob (registration data?), standard VGA DAC palettes (16-color EGA set, 16-step grayscale, 256-color RGB cube, 6-bit components), at least one embedded BMP, and — a shipping accident — a trailing fragment of generated C source (`resource_tvopt.rsc`, the text-variable prompt-dialog template naming "MS Sans Serif"). The directory format remains undecoded *(hypothesis: 6 sections keyed by the count field)*.

Note the internal header says **"RIPterm v2.0"** — like the `.maf` magic, direct evidence that the RIPscrip 3.0 engine is the renamed RIPterm 2.x codebase (see [Introduction](01-introduction.md)).

*Evidence: corpus (RIPSCRIP.RES — [binary triage §5](../research/riptel-binary-formats.md)); HLP (resource-file-required string).*

## `RIPSCRIP.DB` — Text-Variable Database

The persistent store behind `$+VAR$` [permanent user variables](18-text-variables-general.md#persistence). Header: `\x04 "RIPscrip Text Variable Database" \x04`. The DLL describes it as an indexed record database with a hash table, and its recommended corruption remedy is deletion ("Database is corrupted - Try deleting RIPSCRIP.DB"). The shipped file is 400 bytes of empty index scaffolding.

*Evidence: HLP (header string, database error strings); corpus (RIPSCRIP.DB file).*

## Script Containers (`.RIP`, `.FN`, `.COL`, …)

Not binary formats: all script extensions in the corpus (`.RIP`, `.FN`, `.DEF`, `.MNU`, `.MSE`, `.RET`, `.ENT`, `.EXT`, `.COL`) are plain-ASCII RIPscrip command streams distinguished only by role. `.COL` "column layout" scenes and the raw-prose `.TXT` story files they flow are covered in the [Column Text System](17-column-text-system.md); playback search paths in [Local File Playback](15-local-playback-popup-lists.md).

*Evidence: corpus (116-file census — [script census](../research/riptel-script-census.md)).*

---

[◀ Prev: Text Variables: Environment, Clipboard, Screen & Tables](21-text-variables-environment.md) · [Contents](README.md)
