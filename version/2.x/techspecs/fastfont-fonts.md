# Atech FastFont Outline Fonts (.FF1 / .RFF)

[◀ Prev: JPEG Images](jpeg-images.md) · [Contents](README.md) · [Next: MicroANSI Terminal Fonts ▶](microansi-fonts.md)

The RIPscrip 2.x scalable outline-font system — the engine behind [RIP_EXTENDED_FONT_STYLE](../ripscrip/08-level-0-commands-a-f.md#rip_extended_font_style) (`!|y`, "True Type style" fonts, spec v2.A1) — is a licensed third party: **Atech Software's "FastFont"** format, from Atech's AllType font-conversion product. This page is the base format document for both on-disk generations, which both first appear in the 2.x product family:

- **`.FF1`** — Atech's native FastFont container, as shipped in RIPterm Professional 2.0 (January 1995) and carried byte-identical (md5-verified) into the 2.20 distribution.
- **`.RFF`** ("RIPscrip FastFont") — a **reformat, not a rename**, introduced with the definitive **RIPterm 2.30** (files dated April 1997): the fixed header is reorganized and the style table expanded, while the glyph payload is carried over byte-for-byte. `RIPTERM.EXE` correspondingly changes its font-browser filter from `%s*.ff1` to `%s*.rff` (binary strings, 2.0 vs 2.30). RIPtel 3.1 ships these exact five `.RFF` files byte-identical and adds three families — see the [3.x delta](../../3.x/techspecs/rff-additions.md).

Both generations are preserved byte-exact in [`version/2.x/assets/fonts/`](../assets/fonts/README.md); prior partial decodes are in the [3.x file-formats page](../../3.x/ripscrip/22-file-formats.md#rff-outline-fonts--atech-fastfont) and the [RIPtel binary triage §1](../../3.x/research/riptel-binary-formats.md), which this page supersedes for the base format. Font files live in the `FONTS\` sub-directory (RIPTERM.DOC §2.1.4); `DEMO_ONE.EXE` in the 2.0 `FONTS\` directory is TeleGrafix's FF1 viewer utility ("a utility for viewing the FF1 outline font files", README.DOC).

All integers are little-endian. Offsets below were verified programmatically across all five shipped families (COBB, DEFAULT, DIXON, MARIN, SYMBOL) in both generations.

## Shipped files

| Family | `.FF1` (2.0/2.20) | `.RFF` (2.30) | Δ bytes | Trailer copyright |
| --- | --: | --: | --: | --- |
| Cobb | 62,311 | 62,725 | +414 | Atech |
| Default | 31,228 | 31,596 | +368 | TeleGrafix (rebranded) |
| Dixon | 41,657 | 42,071 | +414 | Atech |
| Marin | 56,112 | 56,526 | +414 | Atech |
| Symbol | 43,187 | 43,601 | +414 | Atech |

The size delta is exactly the added style records: 9 × 46 bytes for four families, 8 × 46 for Default (its `.FF1` already carried two records — see below). For every family, the file content is **byte-identical from the end of the FF1 style table to end-of-file**, modulo one rewritten absolute offset: `FF1[gso+4 ..] == RFF[0x206 ..]` where `gso` is the FF1 glyph-section offset (0x64, or 0x92 for Default) — verified as an exact common suffix of 62,207 / 31,078 / 41,553 / 56,008 / 43,083 bytes respectively.

## `.FF1` header (54 bytes)

Magic: the first four bytes are `BD 7A 00 01` in every file.

| Offset | Type | Value / meaning |
| --- | --- | --- |
| 0x00 | u16 | 0x7ABD — magic |
| 0x02 | u16 | 0x0100 — constant (format version 1.00 _(interpretation unverified)_) |
| 0x04 | u16 | 1 — constant |
| 0x06 | u16 | 0 — constant |
| 0x08 | u16 | 1000 — units per em |
| 0x0A | u16 | **style-record count** (1 in four families; 2 in DEFAULT.FF1) |
| 0x0C | u16 | 1 — constant |
| 0x0E | u16 | 32 — first character code |
| 0x10 | u16 | 224 — glyph count (chars 0x20–0xFF) |
| 0x12 | s16 | bbox xmin (per font: Cobb −32, Default −2, Dixon −44, Marin −78, Symbol −133) |
| 0x14 | s16 | descender / bbox ymin (−775, −750, −921, −916, −1110) |
| 0x16 | u16 | ascender / bbox ymax (613, 500, 1000, 1059, 1095) |
| 0x18 | u16 | per-font width metric, likely max advance (274, 250, 250, 250, 328) |
| 0x1A | u16 | 0x4498 — format signature, constant |
| 0x1C | u16 | 0x0204 — constant |
| 0x1E | u16 | 0x0036 — offset of style table |
| 0x20 | u16 | 0x002E — style-record size (46) |
| 0x22 | u16 | **glyph-section offset** = 0x36 + 46 × styleCount (0x64; DEFAULT: 0x92) |
| 0x24 | u16 | 0x0010 — constant |
| 0x26 | u16 | **trailer offset** (Cobb 0xF323, Default 0x75A2, Dixon 0xA19D, Marin 0xD808, Symbol 0xA810) |
| 0x28 | 14 bytes | zero |

Note the trailer offset is a **u16** — the format cannot address a trailer beyond 64 KiB. The `.RFF` reformat widens it to a u32 (see below), which was presumably a motive for the reformat: BRUSH.RFF (RIPtel) places its trailer at 0x1544D, beyond u16 range.

## Style records (46 bytes each)

Identical record format in both generations, at the offset/size given in the header:

| Offset | Type | Meaning |
| --- | --- | --- |
| +0 | u16 | engine-assigned font handle (descending sequence per file; not stable across files) |
| +2 | u8 | style flags: 0x00 base, 0x01 Thin, 0x02 Condensed, 0x04 Wide, 0x08 Expanded; +0x10 = Hollow modifier |
| +3 | u8 | horizontal scale percent: 100 base/hollow, 85 Th, 75 Cn, 125 Wd, 150 Ex |
| +4 | u8[8] | engine parameters `XX 0F 05 F8 1E 14 00 00`, where byte +4 = **hollow-outline flag** (0x00 solid, 0x01 hollow) |
| +12 | char[34] | NUL-terminated style name ("Dixon", "Dixon Th", … "Dixon HE"); padding after the NUL is **uninitialized buffer memory** — parsers must stop at the first NUL |

The uninitialized padding is the source of the leaked strings previously observed in the RIPtel copies (`*.FF1` filenames, `AllType`, PostScript tokens — [triage §1](../../3.x/research/riptel-binary-formats.md)); in DIXON.FF1 the padding is a repeating `58 02` pattern instead.

- Each `.FF1` carries only its **base face**: one record (e.g. handle 0x03E5, flags 0x00, scale 100, "Dixon"). `DEFAULT.FF1` uniquely carries two: "DEFAULT" and "DEFAULT EX" (flags 0x08, scale **140**).
- Each `.RFF` carries **ten records**: base + `Th`/`Cn`/`Wd`/`Ex` + Hollow versions of all five (`Ho`, `HT` 0x11, `HC` 0x12, `HW` 0x14, `HE` 0x18) — the exact family-suffix names scripts pass in `RIP_EXTENDED_FONT_STYLE` (e.g. `!|y…marin` in [SHADOW.RIP](../assets/icons/README.md#shadowrip)). The converter synthesized the missing styles at standard scales but **carried over** existing records: DEFAULT.RFF's "DEFAULT Ex" keeps the FF1's 140% scale while every other family's Ex is 150% — direct provenance evidence that the `.RFF` files were machine-converted from these exact `.FF1` files.

## Glyph section

At the glyph-section offset (`u16@0x22` in FF1; 0x202 = end of the 10-record style table in RFF) begins a block that opens `u16 = 1`, then a **u16 absolute file offset** pointing 16 bytes ahead (FF1 DIXON: `01 00 74 00` at 0x64 → 0x74; RFF DIXON: `01 00 12 02` at 0x202 → 0x212) — this is the only byte that differs between the FF1 and RFF payloads, because it is absolute and the section moved. It is followed by a metrics block echoing the header values (units/em 1000, signature 0x4498, the bbox values), then per-glyph width/advance tables and the outline data for the 224 glyphs. **The glyph outline encoding itself remains undecoded** (as it was in the [3.x triage](../../3.x/research/riptel-binary-formats.md)); renderers targeting authenticity should treat the payload as opaque and rasterize via metrics from the header until it is reverse-engineered.

## Trailer

At the trailer offset (u16@0x26 in FF1, u32@0x00 in RFF — same content in both generations):

```text
u16 tag        = 1
u16 totalLen   = length of this record including its 10-byte header
-- for the Atech-built families (kerning-pair table):
u16 0x00FF
u16 0x012C (= 300)
u16 pairCount
pairCount × { u8 char1, u8 char2, s16 delta }   ; delta in 1/1000 em
```

Worked example — DIXON kerning table at 0xA19D (identical bytes in DIXON.RFF at 0xA33B): tag=1, totalLen=230 (10 + 55×4), 55 pairs; first entries `FA` −118, `PA` −119, `TA` −137, `VA` −112, `WA` −106, `YA` −152, `wA` −88, `AC` −48, `LO` −88 … COBB carries a single dummy pair `||` with delta 0. DEFAULT's trailer record (tag=1, totalLen=1034) is **not** a kerning table — it embeds the text "RIPterm 2.0 font system Copyright TeleGrafix Communications" followed by undecoded binary data; DEFAULT was rebuilt by TeleGrafix rather than delivered by Atech, and its record format is unverified.

After the trailer record: a 5-byte section separator `B7 BC BA AB BE`, then `COPR:` NUL `Copyright 1991, Atech Software, Carlsbad CA` (or, for DEFAULT, `RIPterm 2.0 font system Copyright TeleGrafix Communications Inc. 1995`) running to end-of-file.

## `.RFF` header (54 bytes)

The reformat reorders the same fields, widens the trailer offset to u32, moves the per-file metrics after the constants, and terminates the fixed header with an ASCII sentinel:

| Offset | Type     | Value / meaning (DIXON.RFF values shown)           |
| ------ | -------- | -------------------------------------------------- |
| 0x00   | u32      | trailer offset (0xA33B) — **no magic at offset 0** |
| 0x04   | 12 bytes | zero                                               |
| 0x10   | u16      | 0x0010                                             |
| 0x12   | u8,u8    | 02 02                                              |
| 0x14   | u16      | 0x002E — style-record size                         |
| 0x16   | u16      | 0x0036 — style-table offset                        |
| 0x18   | u8,u8    | 02 04                                              |
| 0x1A   | u16      | 0x4498 — format signature                          |
| 0x1C   | u16      | width metric (250)                                 |
| 0x1E   | u16      | ascender (1000)                                    |
| 0x20   | s16      | descender (−921)                                   |
| 0x22   | s16      | bbox xmin (−44)                                    |
| 0x24   | u16      | 224 — glyph count                                  |
| 0x26   | u16      | 32 — first character                               |
| 0x28   | u16      | 1                                                  |
| 0x2A   | u16      | 10 — style-record count                            |
| 0x2C   | u16      | 1000 — units per em                                |
| 0x2E   | u16×3    | 1, 0, 1                                            |
| 0x34   | u16      | 0x5426 = ASCII `&T` — end-of-header sentinel       |

Reliable identification: `10 00 02 02 2E 00 36 00 02 04 98 44` at offset 0x10 plus `&T` at 0x34 (all eight RIPtel-era files conform — [triage §1](../../3.x/research/riptel-binary-formats.md)); for `.FF1`, the magic `BD 7A 00 01` at offset 0.

## `ATF.CFG` — the engine's font catalog cache

`FONTS\ATF.CFG` (684 bytes, in the definitive distribution) is the Atech engine's **binary** pre-scanned registry, mapping style names to files and handles so the rasterizer needn't open every font at startup. Decoded layout (byte-verified; framing corrected 2026-08-08 against the RIPtel copy, whose non-zero file indices expose the pair structure a single-digit index count leaves ambiguous):

```text
0x00  u16  0x218E                      ; differs per generation (RIPtel: 0x2654);
                                       ; meaning unverified — not a constant magic,
                                       ; not a simple byte-sum checksum
0x02  u16  fileCount   = 5
0x04  u16  totalStyles = 6
0x06  u32  0
0x0A  fileCount × 78-byte entries:
        char[14]  filename, NUL-padded ("COBB.FF1", "DEFAULT.FF1", …)
        u16       index of this font's first record in the style list (0,1,3,4,5)
        u32       0
        byte[54]  verbatim copy of the font's 0x00–0x35 header
        byte[4]   zero padding
0x18C (= fileSize − 48 × totalStyles = 0x0A + 78 × fileCount − 4):
      totalStyles × { u16 fileIndex, 46-byte style record }, ending exactly at EOF
        fileIndex = 0-based index of the owning file entry (0,1,1,2,3,4)
        style record = the 46-byte format above, carrying the engine handle from
        the source font; name padding carries uninitialized/cache data
        (ascending u32 sequences, leaked filename fragments)
```

The pair area's first `fileIndex` (0) overlaps the last file entry's zero padding by two bytes, which is what made the framing ambiguous: an earlier reading placed 46-byte records at 0x18E with u16 "separators" and 10 stray tail bytes — the "separators" are in fact the next record's `fileIndex`, and the "tail" is the final record's name padding.

Two archaeology notes: the file is dated 1995-11-28 and **catalogs the `.FF1` filenames even in the 2.30 distribution that ships only `.RFF` files** — it is the 2.20-era cache carried forward stale (byte-identical md5 across the 2.20.01 and 2.30 trees), which the engine evidently regenerates or tolerates; and it proves the catalog is machine-written and safe to rebuild. RIPtel 3.1's regenerated `atf.cfg` (4,470 bytes; 8 files, 80 styles) uses the same scheme — see the [3.x delta](../../3.x/techspecs/rff-additions.md#atfcfg--the-regenerated-catalog).

## Wire-format usage

`RIP_EXTENDED_FONT_STYLE` selects these fonts **by style-record name**, with size/rotation/effects in fixed fields. The shipping 2.x engine uses the 26-fixed-character argument block followed by the name (`!|y00000X02020000001a1a000000marin` — SHADOW.RIP), not the 13-character form in the ALPHA 4 draft; TeleGrafix's own field crib `sfFFFFZZOOSSCCBBCCWWRRRRRR` survives in the RIPtel corpus (FONTS.RIP). See the [RIP_EXTENDED_FONT_STYLE notes](../ripscrip/08-level-0-commands-a-f.md#rip_extended_font_style) and the [3.x command page](../../3.x/ripscrip/10-level-0-commands-s-z.md#rip_extended_font_style).

---

[◀ Prev: JPEG Images](jpeg-images.md) · [Contents](README.md) · [Next: MicroANSI Terminal Fonts ▶](microansi-fonts.md)
