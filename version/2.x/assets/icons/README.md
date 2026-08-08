# Icons — RIPterm Professional 2.0 Distribution

The complete `ICONS\` sub-directory from an installed **RIPterm Professional 2.0** (January 1995; files dated 1995-02-01), copied verbatim — 95 files: 91 `.BMP` icons, 3 `.BMH` hot-icon variants, and 1 `.RIP` demo script. Everything is preserved byte-exact; source install: `~/src/rip-tools/RIPTerm2.0/extracted/` (see [CONTRIBUTING.md](../../../../CONTRIBUTING.md)).

## The 2.x icon taxonomy

RIPscrip 2.0 replaced the 1.x icon formats with plain Windows BMPs. RIPTERM.DOC (§2.1.5) tabulates the mapping:

| File Description         | Icon   | Mask   | Hot    |
| ------------------------ | ------ | ------ | ------ |
| RIPscrip v1.x Icon Files | `.ICN` | `.MSK` | `.HOT` |
| RIPscrip v2.x Icon Files | `.BMP` | `.BMM` | `.BMH` |

RIPterm 2.0 converts v1.x icons to BMP automatically ("Convert icon … to 2.0 BMP format?"), both at start-up (disable with the `-I` switch) and on-the-fly. A `.BMH` "hot bitmap" is a plain BMP holding the pressed/highlighted state of its same-named `.BMP`, used by hot-icon buttons — the identical convention that RIPtel 3.1 still uses (see the [3.x file-formats page](../../../3.x/ripscrip/22-file-formats.md#bmh--button-highlight-variants)). Sound files (`.WAV`) also live in `ICONS\` per the manual; RIPterm 2.0's own distribution shipped none there (see [../audio/](../audio/README.md)).

Note: the 2.0 manual labels the v1.x hot-icon extension `.HOT`, while the published 1.54 specification calls hot icons `.HIC` — the manual's table appears to describe the mask/hot sidecar extensions as RIPterm's installer saw them in the field.

Every bitmap in the set is a Windows 3.x-format 16-color (4-bit) BMP. No `.BMM` mask files were distributed.

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

## SHADOW.RIP

A 12,485-byte **authentic RIPscrip 2.0 script** shipped in `ICONS\` — a drop-shadow text demo. It is direct wire-format evidence of the shipping 2.0 engine: it drives [RIP_EXTENDED_FONT_STYLE](../../ripscrip/08-level-0-commands-a-f.md#rip_extended_font_style) with the **26-fixed-character argument block** followed by the font name (`!|y00000X02020000001a1a000000marin`) — the same extended layout later observed throughout the RIPtel 3.0 corpus (see the [3.x RIP_EXTENDED_FONT_STYLE notes](../../../3.x/ripscrip/10-level-0-commands-s-z.md#rip_extended_font_style)), not the 13-character form published in the ALPHA 4 draft. It selects the `marin` outline font from [../fonts/MARIN.FF1](../fonts/README.md#atech-fastfont-outline-fonts-ff1).
