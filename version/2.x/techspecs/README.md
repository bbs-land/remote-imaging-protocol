# RIPscrip 2.x Technical Specifications

[Contents](README.md) · [Next: Icon Formats ▶](icon-formats.md)

Original byte-level documentation of the binary file formats and implementation details of the RIPscrip 2.x product family — RIPterm Professional 2.0 through the final **RIPterm 2.30** release, which is treated throughout as the definitive 2.x reference. These pages are original techspecs, not spec conversions: every reverse-engineered claim cites the ALPHA 4 specification section, a recovered document, or an artifact path with observed bytes, and details that could not be verified are marked as such.

Formats are documented in the earliest version family where they appear (see [CONTRIBUTING.md](../../../CONTRIBUTING.md)); the 1.x icon and font formats live in `version/1.5x/techspecs/`, and the RIPscrip 3.x techspecs document only their deltas against these pages. Note that none of the open-source RIP reference implementations (SyncTERM `ripper.c`, icy_tools, RIPtermJS, PabloDraw) parses the 2.x binary containers documented here — they implement the 1.54 wire language — so these pages are currently the only implementer documentation for these formats.

Primary sources: the RIPterm 2.30 distribution (`~/src/rip-tools/artifacts/ripterm-2.30/extracted/`, incl. the full `RIPTERM.DOC` manual), the installed RIPterm Professional 2.0 (`~/src/rip-tools/RIPTerm2.0/extracted/`), the byte-exact asset archives under [`version/2.x/assets/`](../assets/fonts/README.md), the `RIPScrip-2.0-alpha-4.txt` specification and its [Markdown edition](../ripscrip/README.md), and the RIPtel 3.1 install for cross-checking formats the 3.0 engine inherited.

## Contents

1. [Icon Formats](icon-formats.md) — the 2.x icon family as a delta from 1.x: `.BMP` Windows DIB icons as shipped, `.BMM` masks, `.BMH` hot bitmaps
2. [JPEG Images](jpeg-images.md) — the photographic image format: the baseline JFIF profile shipped content uses, the decoder envelope, and scaling/aspect/palette rendering semantics
3. [Atech FastFont Outline Fonts](fastfont-fonts.md) — the scalable outline-font system: the `.FF1` format and the `.RFF` reformat (RIPterm 2.30), header/style/kerning layouts, `ATF.CFG`
4. [MicroANSI Terminal Fonts](microansi-fonts.md) — the "RIPterm v2.0 MicroANSI Font File" container: `RIPTERM.FNT` and its `RIPTERM.MAF` revision, full directory/glyph layout
5. [UI Resources](ui-resources.md) — RIPterm's own per-resolution system fonts (`0640X350.FNT` …) and the `.IMG` planar widget-image format
6. [Audio](audio.md) — digitized `.WAV` playback: the WAVE (PCM) interchange format and its playback semantics
7. [Palette & Direct RGB](palette-rgb.md) — the 256-entry drawing palette, direct-RGB color mode, and the SVGA video modes actually shipped

---

[Contents](README.md) · [Next: Icon Formats ▶](icon-formats.md)
