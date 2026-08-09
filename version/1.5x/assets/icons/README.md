# Icons — RIPterm 1.54 Distribution

The 184 `.ICN` files in this directory are the icon assets from TeleGrafix's original RIPterm 1.54 distribution (`RIPTM154.ZIP`, released July 15, 1993; all files in the archive are dated 1993-07-15). They are preserved byte-exact: the repo's `.gitattributes` marks `version/*/assets/**` as `-text` (no line-ending normalization), and the `*.ICN` pattern stores the binaries via Git LFS.

## What `.ICN` files are

An `.ICN` file is a RIPscrip **icon** — a device-dependent bitmap in the exact binary format described in the spec's [Icon File Format](../../ripscrip/20-icon-file-format.md) page: a 4-byte header (width−1, height−1 as little-endian 16-bit integers) followed by planar scan-line data identical to Borland BGI's `getimage()`/`putimage()` buffer format, plus two trailing "trash" bytes (the spec says one, but every shipped file has two — see the [icon format techspec](../../techspecs/icon-format.md)). Icons are the disk-based counterpart of the clipboard image commands: [RIP_WRITE_ICON](../../ripscrip/11-images-icons.md#rip_write_icon) saves the clipboard to an icon file, and [RIP_LOAD_ICON](../../ripscrip/11-images-icons.md#rip_load_icon) stamps an icon file onto the screen (the `.ICN` extension is appended automatically when a RIP_LOAD_ICON filename omits it).

## How RIPterm used them

Per RIPTERM.DOC, icon files (`.ICN`, plus `.MSK` mask files and `.HIC` "hot icons" used with buttons) had to live in the `ICONS\` sub-directory of the RIPterm installation (optionally in per-host sub-directories, falling back to `ICONS\`). Hosts referenced them by bare filename — RIP_LOAD_ICON filenames may not contain path information. Icons also serve as button faces: an [RIP_BUTTON](../../ripscrip/12-buttons.md#rip_button) defined with the Icon Button flag (flag 128 in [RIP_BUTTON_STYLE](../../ripscrip/12-buttons.md#rip_button_style)) loads its image from an icon file, optionally with a `.HIC` hot icon as the alternate (selected) image. The original archive also shipped four `.MSK` masks (BUBBLE, CHROME, MOUSE, TANK) and three `.HIC` hot icons (BUTTON, RADIO, RADIOBUT) alongside these `.ICN` files; only the `.ICN` files are staged in this directory.

## File reference

Neither RIPTERM.DOC nor WHATSNEW.DOC describes individual icon files, so the descriptions below are inferred from the filenames (and obvious series), and grouped into families for readability. Pixel dimensions are read from each file's 4-byte header per the icon file format. A trailing `B` in a name (e.g. `80286B.ICN`) appears to mark an alternate rendering of the same artwork — typically a smaller, button-sized variant — but TeleGrafix documented no naming convention, so those pairings are inferred as well.

### Buttons, bevels & UI building blocks

| File | Size (bytes) | Dimensions | Description |
| --- | --: | --- | --- |
| `BIGBUT.ICN` | 606 | 40x30 | Large blank button face |
| `BLANKBEV.ICN` | 342 | 25x21 | Blank beveled button |
| `BLANKBUT.ICN` | 210 | 21x17 | Blank button face |
| `BLKBTN.ICN` | 1,382 | 63x43 | Black button face |
| `BLNKBUTN.ICN` | 1,414 | 64x44 | Blank icon-size button face |
| `BUTTON.ICN` | 210 | 22x17 | Basic button face (a `BUTTON.HIC` hot-icon companion shipped in the archive) |
| `BUTTON01.ICN` | 6,414 | 138x89 | Large sample button artwork |
| `BUTTON02.ICN` | 6,414 | 138x89 | Large sample button artwork |
| `BUTTON03.ICN` | 6,414 | 137x89 | Large sample button artwork |
| `BUBBLE.ICN` | 374 | 27x23 | Bubble-texture button (`BUBBLE.MSK` mask shipped in the archive) |
| `CHROME.ICN` | 846 | 48x35 | Chrome-texture button (`CHROME.MSK` mask shipped in the archive) |
| `COL_BEV.ICN` | 138 | 85x3 | Thin bevel rule / divider |
| `HBEV.ICN` | 54 | 24x4 | Small horizontal bevel strip |
| `LONGBEV.ICN` | 1,374 | 137x19 | Long beveled bar |
| `LONG_BEV.ICN` | 1,374 | 137x19 | Long beveled bar (same artwork family as `LONGBEV.ICN`) |
| `LONGBUT.ICN` | 1,026 | 133x15 | Long button bar |
| `MAINBUTN.ICN` | 1,026 | 118x17 | Main-menu button bar |
| `LGBUT.ICN` | 546 | 37x27 | Large button face |
| `LGREVBUT.ICN` | 566 | 39x28 | Large reversed (pressed) button face |
| `LG_LEFT.ICN` | 546 | 37x27 | Large left-arrow button |
| `LG_RIGHT.ICN` | 546 | 37x27 | Large right-arrow button |
| `UP.ICN` | 606 | 40x30 | Up-arrow button |
| `DOWN.ICN` | 606 | 40x30 | Down-arrow button |
| `EXIT.ICN` | 294 | 25x18 | Exit button |
| `LOOK.ICN` | 606 | 40x30 | View/look button |
| `PHONEBUT.ICN` | 678 | 41x28 | Telephone (dial) button |
| `RADIO.ICN` | 94 | 15x11 | Radio-button widget (`RADIO.HIC` hot-icon companion in the archive) |
| `RADIOBUT.ICN` | 278 | 25x17 | Radio-button widget (`RADIOBUT.HIC` hot-icon companion) |
| `XBUTTON.ICN` | 210 | 22x17 | Button with an X mark |
| `SWITCHES.ICN` | 1,050 | 72x29 | Toggle-switch panel widget |
| `DOTS.ICN` | 78 | 13x9 | Small dot-pattern element |
| `MDOTS.ICN` | 102 | 9x12 | Small dot-pattern element |
| `IC1.ICN` | 786 | 104x15 | Small bar/label element (icon-demo part) |
| `IC2.ICN` | 426 | 51x15 | Small bar/label element (icon-demo part) |
| `IC3.ICN` | 306 | 39x15 | Small bar/label element (icon-demo part) |
| `IC4.ICN` | 454 | 61x14 | Small bar/label element (icon-demo part) |
| `IC5.ICN` | 198 | 20x16 | Small bar/label element (icon-demo part) |
| `IC6.ICN` | 426 | 51x15 | Small bar/label element (icon-demo part) |
| `BIGICON.ICN` | 1,350 | 62x42 | Large icon frame sample |
| `HELPICON.ICN` | 1,414 | 64x44 | Help menu icon |

### Textures & pattern tiles

| File           | Size (bytes) | Dimensions | Description            |
| -------------- | -----------: | ---------- | ---------------------- |
| `BRICKS.ICN`   |        1,350 | 61x42      | Brick texture tile     |
| `BOXES.ICN`    |        1,414 | 64x44      | Boxes pattern tile     |
| `CONCRETE.ICN` |          286 | 39x14      | Concrete texture strip |
| `MOSAIC.ICN`   |        1,446 | 64x45      | Mosaic pattern tile    |

### RIPterm demo screens & panels

| File | Size (bytes) | Dimensions | Description |
| --- | --: | --- | --- |
| `BIGBAR.ICN` | 6,370 | 293x43 | Wide toolbar/banner bar |
| `DAILER.ICN` | 16,462 | 174x187 | Telephone dialing-pad panel ("dialer") |
| `DAILER2.ICN` | 16,374 | 173x186 | Dialing-pad panel, variant |
| `PADMENU.ICN` | 13,998 | 176x159 | Keypad menu panel |
| `VCR.ICN` | 7,902 | 331x47 | VCR-style playback control bar |
| `VCR2.ICN` | 9,606 | 394x48 | VCR-style control bar, wider variant |
| `DIALDEMO.ICN` | 1,414 | 64x44 | Demo menu tile: dialer demo |
| `ICONDEMO.ICN` | 1,414 | 64x44 | Demo menu tile: icon demo |
| `ICONDEM2.ICN` | 1,414 | 64x44 | Demo menu tile: icon demo, variant |
| `MODEDEMO.ICN` | 1,446 | 64x45 | Demo menu tile: display/write-mode demo |
| `MOUSDEMO.ICN` | 1,382 | 64x43 | Demo menu tile: mouse demo |
| `STDEMO.ICN` | 1,350 | 62x42 | Demo menu tile ("ST" demo; possibly Star Trek, cf. `ENTERPRZ.ICN`/`KLINGON.ICN`) |
| `VCRDEMO.ICN` | 1,414 | 64x44 | Demo menu tile: VCR demo |
| `BOUNCY.ICN` | 1,350 | 62x42 | Bouncing-ball demo tile |
| `ICONS.ICN` | 1,414 | 64x44 | "Icons" menu tile |
| `IC.ICN` | 1,414 | 64x44 | Icon-demo tile |
| `COMPUTER.ICN` | 14,406 | 122x225 | Desktop computer artwork |
| `WORLD-1.ICN` | 35,526 | 640x111 | Full-width world-map panorama, strip 1 of 3 |
| `WORLD-2.ICN` | 36,486 | 640x114 | World-map panorama, strip 2 of 3 |
| `WORLD-3.ICN` | 31,046 | 640x97 | World-map panorama, strip 3 of 3 |
| `HORIZON.ICN` | 1,350 | 62x42 | Horizon artwork tile |
| `PART.ICN` | 4,478 | 101x86 | Parts/particle artwork |

### TeleGrafix support-BBS menu icons

| File | Size (bytes) | Dimensions | Description |
| --- | --: | --- | --- |
| `RTERM1.ICN` | 1,098 | 49x39 | RIPterm area icon |
| `RBETA3.ICN` | 1,126 | 49x40 | Beta-test area icon |
| `RDEMO.ICN` | 966 | 48x40 | Demos area icon |
| `RFILES.ICN` | 1,098 | 49x39 | File library area icon |
| `RICONS2.ICN` | 1,098 | 49x39 | Icon library area icon (RIPTERM.DOC mentions a "RIPICONS file library section") |
| `RLIB.ICN` | 1,126 | 51x40 | Library area icon |
| `RUPGRADE.ICN` | 1,126 | 51x40 | Upgrade area icon |
| `RUPLOAD3.ICN` | 918 | 47x38 | Upload area icon |
| `SIGS.ICN` | 966 | 48x40 | SIGs (special-interest groups) area icon |
| `REGISTRY.ICN` | 1,382 | 63x43 | Registration icon |

### BBS service & menu tiles

| File | Size (bytes) | Dimensions | Description |
| --- | --: | --- | --- |
| `EMAIL.ICN` | 1,350 | 62x42 | E-mail menu tile |
| `EMAIL3.ICN` | 1,414 | 64x44 | E-mail menu tile, variant |
| `NEWS.ICN` | 1,350 | 62x42 | News/bulletins tile |
| `NEWS2.ICN` | 1,414 | 64x44 | News tile, variant |
| `NEWS3.ICN` | 1,414 | 64x44 | News tile, variant |
| `EWRITE.ICN` | 1,414 | 64x44 | E-mail action button: write |
| `ERALL.ICN` | 1,414 | 64x44 | E-mail action button: read all |
| `ERNEW.ICN` | 1,590 | 66x44 | E-mail action button: read new |
| `EERASE.ICN` | 1,626 | 66x45 | E-mail action button: erase |
| `EMODIFY.ICN` | 1,626 | 66x45 | E-mail action button: modify |
| `FILECAB1.ICN` | 1,414 | 64x44 | File cabinet (file areas) |
| `DISKETTE.ICN` | 1,350 | 62x42 | 5.25-inch diskette tile (file transfer) |
| `DISKET2.ICN` | 1,414 | 64x44 | Diskette tile, variant |
| `3&HALF10.ICN` | 1,098 | 49x39 | 3.5-inch diskette icon |
| `ASCII.ICN` | 1,350 | 62x42 | ASCII text-mode/transfer tile |
| `MAILBOX.ICN` | 14,370 | 146x189 | Mailbox artwork |
| `CALENDAR.ICN` | 13,734 | 173x156 | Desk calendar artwork |

### Hardware & electronics

| File | Size (bytes) | Dimensions | Description |
| --- | --: | --- | --- |
| `80286.ICN` | 2,910 | 82x66 | Intel 80286 CPU chip label |
| `80286B.ICN` | 2,366 | 73x59 | 80286 chip, smaller variant |
| `80386.ICN` | 2,646 | 77x66 | Intel 80386 CPU chip label |
| `80386B.ICN` | 2,130 | 72x59 | 80386 chip, smaller variant |
| `80486.ICN` | 3,174 | 87x72 | Intel 80486 CPU chip label |
| `80486B.ICN` | 2,366 | 73x59 | 80486 chip, smaller variant |
| `BIGCHIP.ICN` | 24,966 | 307x160 | Large IC chip artwork |
| `CHIPSAHO.ICN` | 7,046 | 125x110 | IC chip artwork ("chips ahoy") |
| `MEMCHIP.ICN` | 246 | 36x12 | Memory chip |
| `SMCHIP.ICN` | 798 | 48x33 | Small IC chip |
| `BATTERY.ICN` | 846 | 41x35 | Battery |
| `PLUG.ICN` | 1,798 | 56x64 | Power plug |
| `MOUSE.ICN` | 1,878 | 97x36 | Computer mouse (`MOUSE.MSK` mask shipped in the archive) |

### Money & payment-card logos

| File           | Size (bytes) | Dimensions | Description                |
| -------------- | -----------: | ---------- | -------------------------- |
| `AMEREXP.ICN`  |        1,302 | 67x36      | American Express card logo |
| `VISA.ICN`     |        1,302 | 67x36      | Visa card logo             |
| `MC.ICN`       |        1,094 | 64x34      | MasterCard logo            |
| `DISCOVER.ICN` |        1,302 | 71x36      | Discover card logo         |
| `DINERCLB.ICN` |        1,726 | 74x43      | Diners Club card logo      |
| `DOLLAR.ICN`   |        1,414 | 64x44      | Dollar-sign tile           |
| `QUARTER.ICN`  |        2,206 | 74x55      | U.S. quarter coin          |

### Third-party BBS & brand logos

| File           | Size (bytes) | Dimensions | Description                    |
| -------------- | -----------: | ---------- | ------------------------------ |
| `PCBOARD.ICN`  |        1,414 | 64x44      | PCBoard BBS logo               |
| `PCBOARD2.ICN` |        1,414 | 64x44      | PCBoard BBS logo, variant      |
| `WC_MAIN.ICN`  |        4,766 | 132x70     | Wildcat! BBS main-menu artwork |
| `WC_FILE.ICN`  |        1,302 | 66x36      | Wildcat! BBS file-menu tile    |
| `WC_MSG.ICN`   |        1,190 | 60x37      | Wildcat! BBS message-menu tile |
| `GE.ICN`       |        1,414 | 64x44      | GE logo tile                   |

### Games & entertainment tiles

| File | Size (bytes) | Dimensions | Description |
| --- | --: | --- | --- |
| `ALCHEMY.ICN` | 1,414 | 64x44 | Alchemy game tile |
| `ANDROIDS.ICN` | 1,414 | 64x44 | Androids game tile |
| `BORDGAME.ICN` | 1,414 | 64x44 | Board-games tile |
| `BRIDGE.ICN` | 1,414 | 64x44 | Bridge (card game) tile |
| `CASINO.ICN` | 1,414 | 64x44 | Casino games tile |
| `CHESS.ICN` | 1,414 | 64x44 | Chess tile |
| `GOLF.ICN` | 1,414 | 64x44 | Golf game tile |
| `GUESSWHT.ICN` | 1,414 | 64x44 | "Guess What" game tile |
| `HANGMAN.ICN` | 1,414 | 64x44 | Hangman tile |
| `JUMBLE.ICN` | 1,414 | 64x44 | Word-jumble tile |
| `KYRANDIA.ICN` | 1,414 | 64x44 | Kyrandia (BBS door game) tile |
| `POKER.ICN` | 1,414 | 64x44 | Poker tile |
| `ROBOWARS.ICN` | 1,414 | 64x44 | Robo-wars game tile |
| `SEABATLE.ICN` | 1,414 | 64x44 | Sea-battle game tile |
| `SOLITAIR.ICN` | 1,414 | 64x44 | Solitaire tile |
| `STRATWAR.ICN` | 1,350 | 62x42 | Strategic-war game tile |
| `TRIVIA.ICN` | 1,414 | 64x44 | Trivia game tile |
| `WORDZ.ICN` | 1,414 | 64x44 | Word game tile |
| `YAHTZEE.ICN` | 1,414 | 64x44 | Yahtzee tile |
| `NOSHUF.ICN` | 1,414 | 64x44 | Card-game tile ("no shuffle") |
| `GAMES.ICN` | 1,878 | 71x52 | Games menu tile |
| `GAMES2.ICN` | 1,414 | 64x44 | Games menu tile, variant |
| `MAGIC.ICN` | 1,414 | 64x44 | Magic tile |
| `ZEN.ICN` | 1,414 | 64x44 | Zen tile |
| `WOW.ICN` | 1,414 | 64x44 | "WOW" exclamation tile |
| `SMASH.ICN` | 1,414 | 64x44 | Smash artwork tile |
| `BLADEMTR.ICN` | 1,414 | 64x44 | Game tile ("Blade Master") |
| `ENTERPRZ.ICN` | 886 | 79x22 | Starship Enterprise sprite (Star Trek game art) |
| `KLINGON.ICN` | 710 | 60x22 | Klingon ship sprite |

### Tank-game sprites & animation frames

| File | Size (bytes) | Dimensions | Description |
| --- | --: | --- | --- |
| `1TANK.ICN` | 2,262 | 90x47 | Tank artwork (`TANK.MSK` mask shipped in the archive) |
| `1TANKA.ICN` | 2,162 | 81x49 | Tank artwork, variant |
| `1TANKBLK.ICN` | 2,982 | 92x62 | Tank artwork, black/silhouette variant |
| `DATANK.ICN` | 1,350 | 62x42 | Tank tile |
| `TNK.ICN` | 1,350 | 62x42 | Tank tile |
| `TNKGRAY.ICN` | 1,350 | 62x42 | Tank tile, gray variant |
| `BOOM1.ICN` | 1,878 | 66x52 | Explosion animation frame 1 |
| `BOOM2.ICN` | 1,878 | 66x52 | Explosion animation frame 2 |
| `BOOM3.ICN` | 5,346 | 117x89 | Explosion animation frame 3 (large) |
| `BOOM4.ICN` | 1,878 | 66x52 | Explosion animation frame 4 |
| `BOOM5.ICN` | 1,590 | 67x44 | Explosion animation frame 5 |
| `BOOM6.ICN` | 1,294 | 54x46 | Explosion animation frame 6 |
| `SHOTD.ICN` | 138 | 19x11 | Projectile sprite, downward |
| `SHOTL.ICN` | 94 | 16x11 | Projectile sprite, leftward |
| `SHOTU.ICN` | 186 | 20x15 | Projectile sprite, upward |
| `NOSHOT.ICN` | 110 | 16x13 | Empty/no-shot sprite |
| `ZAP.ICN` | 566 | 38x28 | Zap effect sprite |

### Character animation & other art

| File | Size (bytes) | Dimensions | Description |
| --- | --: | --- | --- |
| `MEOW1.ICN` | 646 | 37x32 | Cat-head animation frame 1 |
| `MEOW2.ICN` | 626 | 37x31 | Cat-head animation frame 2 |
| `MEOW3.ICN` | 606 | 37x30 | Cat-head animation frame 3 |
| `MEOW4.ICN` | 586 | 37x29 | Cat-head animation frame 4 |
| `MEOW5.ICN` | 566 | 37x28 | Cat-head animation frame 5 |
| `MEOW6.ICN` | 566 | 37x28 | Cat-head animation frame 6 |
| `MEOWBODY.ICN` | 1,494 | 44x62 | Cat body artwork (pairs with the `MEOW*` head frames) |
| `CATICON.ICN` | 1,414 | 64x44 | Cat icon tile |
| `WAGGER.ICN` | 1,350 | 61x42 | Dog ("wagger") tile |
| `CAMEL.ICN` | 1,414 | 64x44 | Camel artwork tile |
| `MUT.ICN` | 1,542 | 62x48 | Character sprite ("mut") |
| `MUT2.ICN` | 1,542 | 62x48 | Character sprite, variant 2 |
| `MUT3.ICN` | 1,542 | 62x48 | Character sprite, variant 3 |
| `ROCKET.ICN` | 566 | 38x28 | Rocket sprite |
| `PICKUP.ICN` | 1,458 | 81x33 | Pickup truck artwork |
| `PICKUP2.ICN` | 1,590 | 91x33 | Pickup truck artwork, variant |
| `FL1.ICN` | 1,414 | 64x44 | Tile artwork (purpose unidentified) |
| `FL1B.ICN` | 1,590 | 66x44 | `FL1` variant |
| `FL2.ICN` | 1,414 | 64x44 | Tile artwork (purpose unidentified) |
