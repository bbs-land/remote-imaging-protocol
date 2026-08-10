# Icons - RIPterm 2.x Distributions

The 207 icon files of the RIPterm 2.x line, from two recovered installs (everything preserved byte-exact; see [CONTRIBUTING.md](../../../../CONTRIBUTING.md) for provenance):

- **RIPterm Professional 2.0** (January 1995; files dated 1995-02-01) - the complete `ICONS\` sub-directory, 95 files: 91 `.BMP` icons, 3 `.BMH` hot-icon variants, and 1 `.RIP` demo script. Source: `~/src/rip-tools/RIPTerm2.0/extracted/`.
- **RIPterm 2.30** (October 1997) - the 112 files added since 2.0: the RW\* button/icon art, `CYC*` button-cycle scripts, demo `.RIP` scenes, and the first shipped `.JPG` images. Every one is dated 1995-11-28 - the set entered the distribution with **RIPterm 2.20** and was carried into 2.30 byte-identical (2.30's entire `ICONS\` directory is a strict subset of the 2.20.01 install's). Source: `~/src/rip-tools/artifacts/ripterm-2.30/extracted/ICONS/`.

All 95 files of the 2.0 set remain in 2.30 unchanged, with one exception: `SHADOW.RIP` was revised in the 2.20 era (this directory keeps the 2.0 original; see [below](#shadowrip)).

## The 2.x icon taxonomy

RIPscrip 2.0 replaced the 1.x icon formats with plain Windows BMPs. RIPTERM.DOC (§2.1.5) tabulates the mapping:

| File Description         | Icon   | Mask   | Hot    |
| ------------------------ | ------ | ------ | ------ |
| RIPscrip v1.x Icon Files | `.ICN` | `.MSK` | `.HOT` |
| RIPscrip v2.x Icon Files | `.BMP` | `.BMM` | `.BMH` |

RIPterm 2.0 converts v1.x icons to BMP automatically ("Convert icon … to 2.0 BMP format?"), both at start-up (disable with the `-I` switch) and on-the-fly. A `.BMH` "hot bitmap" is a plain BMP holding the pressed/highlighted state of its same-named `.BMP`, used by hot-icon buttons - the identical convention that RIPtel 3.1 still uses (see the [3.x file-formats page](../../../3.1/ripscrip/6.1-content-file-roles.md#bmh---button-highlight-variants)). Sound files (`.WAV`) also live in `ICONS\` per the manual; RIPterm 2.0's own distribution shipped none there (see [../audio/](../audio/README.md)).

Note: the 2.0 manual labels the v1.x hot-icon extension `.HOT`, while the published 1.54 specification calls hot icons `.HIC` - the manual's table appears to describe the mask/hot sidecar extensions as RIPterm's installer saw them in the field.

Every bitmap in the 2.0 set is a Windows 3.x-format 16-color (4-bit) BMP; the 2.20-era additions introduce 256-color (8-bit) and 24-bit BMPs. No `.BMM` mask files were ever distributed.

The 2.20.01 install also shipped ~114 RIPscrip **1.x-format** icon files alongside the BMPs (107 `.ICN`, 4 `.MSK`, 3 `.HIC` - legacy art for the automatic converter); **RIPterm 2.30 dropped every one of them** from the distribution, keeping only the 2.x formats (`.BMP`/`.BMH`) plus `.JPG` and `.RIP`. The engine still lists the legacy extensions in its file dialogs, so support remained - only the shipped art was trimmed. (The legacy files survive in the 2.20.01 tree at `~/src/rip-tools/RIPTerm2.22/ICONS/`.)

## UI controls (8 `.BMP` + 3 `.BMH`)

Button/widget skins; each `.BMH` pairs with its same-named `.BMP`. `BUTTON`, `RADIO`, and `RADIOBUT` reappear byte-compatible in the RIPtel 3.1 corpus.

| File | Size (bytes) | Dimensions | Notes |
| --- | --: | --- | --- |
| `BUTTON.BMP` / `BUTTON.BMH` | 322 / 322 | 22×17 | Generic pushbutton skin + hot state |
| `RADIO.BMP` / `RADIO.BMH` | 206 / 206 | 15×11 | Radio button + hot state |
| `RADIOBUT.BMP` / `RADIOBUT.BMH` | 322 / 322 | 25×17 | Radio button variant + hot state |
| `BLANKBUT.BMP` | 322 | 21×17 | Blank button face |
| `XBUTTON.BMP` | 322 | 22×17 | "X" (close/cancel) button |
| `CONCRETE.BMP` | 398 | 39×14 | Concrete texture strip |
| `LG_LEFT.BMP` / `LG_RIGHT.BMP` | 658 / 658 | 37×27 | Large left/right arrow buttons |

## Gallery icons (8 `.BMP`, 54×34)

A uniform "GAL_" set of BBS-service gallery icons, 1,070 bytes each: `GAL_DATA.BMP`, `GAL_DOLR.BMP`, `GAL_EWRT.BMP`, `GAL_FILE.BMP`, `GAL_GAME.BMP`, `GAL_INFO.BMP`, `GAL_MAIL.BMP`, `GAL_NEWS.BMP`.

## Scene art (10 `.BMP`)

Larger artwork for the bundled demo screens:

| File           | Size (bytes) | Dimensions | Notes                           |
| -------------- | -----------: | ---------- | ------------------------------- |
| `WORLD-1.BMP`  |       35,638 | 640×111    | World-map banner strip (1 of 3) |
| `WORLD-2.BMP`  |       36,598 | 640×114    | World-map banner strip (2 of 3) |
| `WORLD-3.BMP`  |       31,158 | 640×97     | World-map banner strip (3 of 3) |
| `VCR2.BMP`     |        9,718 | 394×48     | VCR-style control bar           |
| `COMPUTER.BMP` |       14,518 | 122×225    | Desktop computer                |
| `MAILBOX.BMP`  |       14,482 | 146×189    | Mailbox                         |
| `DAILER2.BMP`  |       16,486 | 173×186    | Dialer artwork                  |
| `COLUMN.BMP`   |        8,962 | 85×201     | Classical column                |
| `8NCC1701.BMP` |       19,018 | 200×189    | Starship Enterprise (NCC-1701)  |
| `3&HALF.BMP`   |        3,066 | 85×67      | 3.5″ floppy disk                |

## Button icons (65 `.BMP`)

The general icon library: BBS-service icons (mail, news, files, registry, feedback), door-game logos (chess, poker, solitaire, Yahtzee, hangman, golf, Kyrandia, …), payment-card icons (`AMEREXP`, `VISA`, `MC`, `DCVR`, with `AE5`/`DC1` variants), and RIPterm demo icons (`DIALDEMO`, `ICONDEMO`, `MODEDEMO`, `MOUSDEMO`). Most are 64×44 (1,526 bytes) or 62×42 (1,462 bytes).

| File | Size (bytes) | Dimensions | File | Size (bytes) | Dimensions |
| --- | --: | --- | --- | --: | --- |
| `AE5.BMP` | 1,414 | 67×36 | `HELPICON.BMP` | 1,526 | 64×44 |
| `ALCHEMY.BMP` | 1,526 | 64×44 | `HORIZON.BMP` | 1,462 | 62×42 |
| `AMEREXP.BMP` | 1,414 | 67×36 | `IC.BMP` | 1,526 | 64×44 |
| `ANDROIDS.BMP` | 1,526 | 64×44 | `ICONDEM2.BMP` | 1,526 | 64×44 |
| `ASCII.BMP` | 1,462 | 62×42 | `ICONDEMO.BMP` | 1,526 | 64×44 |
| `BLADEMTR.BMP` | 1,526 | 64×44 | `ICONS.BMP` | 1,526 | 64×44 |
| `BORDGAME.BMP` | 1,526 | 64×44 | `JUMBLE.BMP` | 1,526 | 64×44 |
| `BOUNCY.BMP` | 1,462 | 62×42 | `KYRANDIA.BMP` | 1,526 | 64×44 |
| `BOXES.BMP` | 1,526 | 64×44 | `MAGIC.BMP` | 1,526 | 64×44 |
| `BRICKS.BMP` | 1,462 | 61×42 | `MC.BMP` | 1,206 | 64×34 |
| `BRIDGE.BMP` | 1,526 | 64×44 | `MODEDEMO.BMP` | 1,558 | 64×45 |
| `CALCULTR.BMP` | 1,270 | 45×48 | `MOSAIC.BMP` | 1,558 | 64×45 |
| `CAMEL.BMP` | 1,526 | 64×44 | `MOUSDEMO.BMP` | 1,494 | 64×43 |
| `CASINO.BMP` | 1,526 | 64×44 | `NEWS.BMP` | 1,462 | 62×42 |
| `CHESS.BMP` | 1,526 | 64×44 | `NEWS2.BMP` | 1,526 | 64×44 |
| `DATANK.BMP` | 1,462 | 62×42 | `NOSHUF.BMP` | 1,526 | 64×44 |
| `DC1.BMP` | 1,838 | 74×43 | `NSEW_DIR.BMP` | 1,006 | 47×37 |
| `DCVR.BMP` | 1,414 | 71×36 | `POKER.BMP` | 1,526 | 64×44 |
| `DIALDEMO.BMP` | 1,526 | 64×44 | `REGISTRY.BMP` | 1,494 | 63×43 |
| `DISKET2.BMP` | 1,526 | 64×44 | `ROBOWARS.BMP` | 1,526 | 64×44 |
| `DOLLAR.BMP` | 1,526 | 64×44 | `SEABATLE.BMP` | 1,526 | 64×44 |
| `EERASE.BMP` | 1,738 | 66×45 | `SMASH.BMP` | 1,526 | 64×44 |
| `EMAIL.BMP` | 1,462 | 62×42 | `SOLITAIR.BMP` | 1,526 | 64×44 |
| `EMODIFY.BMP` | 1,738 | 66×45 | `VISA.BMP` | 1,414 | 67×36 |
| `ERALL.BMP` | 1,526 | 64×44 | `WC_FILE.BMP` | 1,414 | 66×36 |
| `ERNEW.BMP` | 1,702 | 66×44 | `WORDZ.BMP` | 1,526 | 64×44 |
| `EWRITE.BMP` | 1,526 | 64×44 | `WOW.BMP` | 1,526 | 64×44 |
| `FEEDBAK2.BMP` | 1,494 | 63×43 | `YAHTZEE.BMP` | 1,526 | 64×44 |
| `FILECAB1.BMP` | 1,526 | 64×44 | `GAMES.BMP` | 1,990 | 71×52 |
| `FL1.BMP` | 1,526 | 64×44 | `GE.BMP` | 1,526 | 64×44 |
| `FL1B.BMP` | 1,702 | 66×44 | `GOLF.BMP` | 1,526 | 64×44 |
| `FL2.BMP` | 1,526 | 64×44 | `GUESSWHT.BMP` | 1,526 | 64×44 |
| `HANGMAN.BMP` | 1,526 | 64×44 |  |  |  |

## RIPterm 2.20/2.30 additions (112 files)

The files below entered the distribution with RIPterm 2.20 (all dated 1995-11-28) and shipped unchanged through 2.30.

### The RW\* art set (67 files) - Xtelnet Oy

RIPterm's stock button/window art. `ARTWORK.DOC` (new in the 2.2x install) manifests exactly these 67 files and attaches a credit and a usage restriction: _"Portions of artwork Copyright (c) Xtelnet Oy"_ (a Finnish firm), required on non-commercial use, with commercial online-service use requiring _"prior written notice from TeleGrafix"_. The `D`/`V` suffixes are the two button states (down/up variants); each `V` face pairs with a same-named `.BMH` hot state, the same convention as the 2.0 UI controls.

| Files | Count | Format | Size (bytes) |
| --- | --- | --- | --- |
| `RWBTN01D`-`RWBTN17D.BMP`, `RWBTN01V`-`RWBTN17V.BMP` | 34 | 36×36, 8-bit | 2,374 each |
| `RWBTN01V`-`RWBTN17V.BMH` (hot states) | 17 | 36×36, 8-bit | 2,374 each |
| `RWANIM01`-`RWANIM05.BMP` (animation frames) | 5 | 36×36, 8-bit | 2,374 each |
| `RWBTN20D/V.BMP`, `RWBTN21D/V.BMP` | 4 | 16×16, 8-bit | 1,334 each |
| `RWICON01`-`RWICON03.BMP` | 3 | 16×16, 8-bit | 1,334 each |
| `RWICON10.BMP`/`.BMH`, `RWICON11.BMP`/`.BMH` | 4 | 18×18, 24-bit | 1,062 each |

(There are no `RWBTN18`/`RWBTN19` - the manifest and the shipped set agree.)

### Button-cycle scripts (14 `.RIP`, 1,216 bytes each)

`CYC001UP/DN`, `CYC010UP/DN`, `CYC011UP/DN`, `CYC100UP/DN`, `CYC101UP/DN`, `CYC110UP/DN`, `CYC111UP/DN` - paired up/down button-cycling demo scripts (the three digits encode a flag combination).

### Demo scenes (21 `.RIP`)

Authentic RIPscrip 2.x demo scripts - wire-format evidence of the shipping 2.2-era engine alongside [SHADOW.RIP](#shadowrip):

| File           | Size (bytes) | File           | Size (bytes) |
| -------------- | -----------: | -------------- | -----------: |
| `DEMOFILE.RIP` |        1,123 | `N2_BUSI.RIP`  |        5,125 |
| `EAGLE.RIP`    |        2,206 | `N2_PHOTO.RIP` |          907 |
| `EXAMPLE1.RIP` |          685 | `N2_TITLE.RIP` |        4,767 |
| `EXAMPLE2.RIP` |          712 | `NEWS.RIP`     |        3,062 |
| `EXAMPLE3.RIP` |          713 | `RACECAR.RIP`  |       55,259 |
| `EXAMPLE4.RIP` |          772 | `SAILBOAT.RIP` |       20,006 |
| `FADEIN.RIP`   |       69,056 | `SHUTTLE.RIP`  |       24,934 |
| `FADEOUT.RIP`  |       69,058 | `SPACSHUT.RIP` |       11,671 |
| `FOUND.RIP`    |       17,611 | `TUNNEL.RIP`   |        2,759 |
| `FRACTMTN.RIP` |       67,557 | `WEATHER.RIP`  |        4,222 |
| `HAWK.RIP`     |        3,529 |                |              |

### JPEG images (7 `.JPG`) - the first shipped JPEG content

Demo photographs for the JPEG display feature (baseline JFIF 1.00; all 320×200 except `GALAXY.JPG` at 320×240): `ASTRO.JPG` (7,117 B), `BEACH2.JPG` (5,662 B), `BRIDGE02.JPG` (6,660 B), `DUSK_SEA.JPG` (4,114 B), `FIRCLOUD.JPG` (5,080 B), `GALAXY.JPG` (6,095 B), `JUPITER.JPG` (8,032 B).

### Miscellaneous bitmaps (3 `.BMP`)

| File          | Size (bytes) | Dimensions     |
| ------------- | -----------: | -------------- |
| `A045.BMP`    |        6,626 | 73×73, 8-bit   |
| `SKULLWP.BMP` |       36,946 | 193×183, 8-bit |
| `TILE14.BMP`  |       41,078 | 100×100, 8-bit |

## SHADOW.RIP

A 12,485-byte **authentic RIPscrip 2.0 script** shipped in `ICONS\` - a drop-shadow text demo. It is direct wire-format evidence of the shipping 2.0 engine: it drives [RIP_EXTENDED_FONT_STYLE](../../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style) with the **26-fixed-character argument block** followed by the font name (`!|y00000X02020000001a1a000000marin`) - the same extended layout later observed throughout the RIPtel 3.0 corpus (see the [3.x RIP_EXTENDED_FONT_STYLE notes](../../../3.1/ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style)), not the 13-character form published in the ALPHA 4 draft. It selects the `marin` outline font from [../fonts/MARIN.FF1](../fonts/README.md#atech-fastfont-outline-fonts-ff1).

The 2.20/2.30 installs carry a **revised** SHADOW.RIP (12,157 bytes, dated 1995-11-28, md5 `2038ea4f…` vs the 2.0 original's `6a877abd…`): the opening `!|l7040B040` command (unidentified - in the ALPHA 4 spec `l` is RIP_POLYLINE, but the arguments don't parse cleanly as one; what the shipping engine does with it is not established) is replaced by a reset introduced with an SOH prefix (`\x01|*`), and the repeated `|k00` (RIP_BACK_COLOR 0) commands are stripped - 160 occurrences down to 10 surviving `|k0A`/`|k0C` settings (byte survey 2026-08-08); the drawing content (still titled "RIPterm Pro 2.0") is otherwise the same. This directory preserves the **2.0 original**; the 2.20-era revision lives in `~/src/rip-tools/artifacts/ripterm-2.30/extracted/ICONS/SHADOW.RIP` (byte-identical in the 2.20.01 tree).
