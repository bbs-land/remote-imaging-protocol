# Icons - RIPtel 3.1 `ICONS\` directory

The complete `ICONS\` directory from the RIPtel Visual Telnet 3.1 install (TeleGrafix Communications, Oct 1997; RIPscrip driver 3.0.7), copied verbatim - 234 files. This is both RIPtel's demo corpus - the primary surviving body of real RIPscrip 3.0 scripts, and the source material for much of the 3.x reconstruction in this repo - and its UI art. Breakdown: 102 `.BMP`, 48 `.FN`, 35 `.RIP`, 7 `.JPG`, 7 `.DEF`, 6 `.RET`, 5 `.MSE`, 5 `.BMH`, 4 each `.TXT`/`.MNU`/`.EXT`/`.ENT`, 3 `.COL`.

Per-file findings below are drawn from the [script census](../../research/riptel-script-census.md) (every script parsed; 22,921 commands, 0 parse errors; TeleGrafix's own comments preserved), the [binary-format triage](../../research/riptel-binary-formats.md), and the [File Formats](../../ripscrip/6.1-content-file-roles.md) reference. The flowed-text machinery the column demos exercise is documented in [Column Text System](../../ripscrip/3.1-column-text-system.md).

Storage is byte-exact (`version/*/assets/** -text` in the repo [.gitattributes](../../../../.gitattributes)): the scripts keep their DOS-era CRLF line endings and the SOH (0x01) start-of-scene introducer bytes, and the `.TXT` story files keep their bare-CR line breaks - all of it significant test data. `.BMP`/`.JPG` files are tracked via Git LFS.

Note on audio: RIPscrip 3.0's local-playback search order is host directory first, then `ICONS\` - WAV audio files would live in this same directory, but RIPtel 3.1 shipped none (see [../audio/](../audio/README.md)).

Two files are referenced by scripts but were never shipped: `SHADOWDO.FN` (called by SHADMOVE.RIP) and `TELLISTS.RET` (referenced by TELLISTS.MNU).

## 1. Demo scenes (`.RIP`, 35 files)

| File | Demo |
| --- | --- |
| `BOUNDS.RIP` | Bounded-text demo: draws its bounding box ("Show our bounding box"), then the corpus's sole use of the new `"` RIP_BOUNDED_TEXT command wrapping text inside it. |
| `BUTTONS.RIP` | Button and mouse-field showcase: bitmap-skinned buttons and radio/checkbox controls (RADIONEW/CHECKBOX BMPs), `<<IF $RETURN$...>>` conditional navigation. |
| `CURVES.RIP` | Bezier and poly-Bezier curve demo. |
| `DRAGON.RIP` | Animated dragon story: DBACK.FN backdrop (STRIP6/TORCH/GODRAG3 art), DRAGON.TXT prose flowed into `1e` text columns, DL.FN/DR.FN page-turn blits. |
| `EAGLE.RIP` | Vector line-art scene (eagle) - RIP_LINE-heavy drawing with its own text window. |
| `FONTS.RIP` | Font-system demo and a documentation goldmine: carries TeleGrafix's field-layout crib comments for `1e` (column regions) and `y` (RIP_EXTENDED_FONT_STYLE), plus the 16-combination char×text rotation grid; gateway to FONTTEXT.COL and SHOWFONT.RIP. |
| `FOUND.RIP` | Line-art scene on the TELENGIN "engineering" menu (subject unverified - name inference). |
| `HAWK.RIP` | Vector line-art scene (hawk), companion to EAGLE.RIP. |
| `IMAGES.RIP` | Bitmap/JPEG display demo: tiled BRICK.BMP, 256COLOR.BMP at multiple scales, GEAR.BMP, BRIDGE02.JPG, and the corpus's only RIP_VIEWPORT uses. |
| `LANDSCPE.RIP` | Landscape art scene: palette animation (RIP_ONE_PALETTE), Beziers, and the corpus's only RIP_GET_IMAGE uses. |
| `LGF1.RIP` | Art scene on the TELDRAW menu, with palette manipulation (subject unverified - name inference). |
| `MARKER.RIP` | "RIPscrip Markers": first page of the new `;` RIP_MARKER glyph table (marker types 00-0C). |
| `MARKER2.RIP` | Second marker page (types 0E and up), cycling fill colors per marker. 361 marker commands across the pair. |
| `N2_BUSI.RIP` | Newspaper business page: bar-chart graphics (RIP_BAR). |
| `N2_HORO.RIP` | Newspaper horoscope page: raw horoscope text flowed into `1e` columns. |
| `N2_PHOTO.RIP` | Newspaper photo page: cycles BEACH2/BRIDGE02/DUSK_SEA/FIRCLOUD JPEGs using `$INUSE(TV,NEXT_IMG)$` conditionals. |
| `N2_TITLE.RIP` | Newspaper title page: region-text manifesto on RIPscrip's JPEG/sound/24-bit-color future, with ASTRO/GALAXY/JUPITER JPEGs. |
| `NEWCMDS.RIP` | The "Rosetta stone": comment-labeled showcase that names the six new skewed-oval drawing primitives (`&` `-` `]` `[` `+` `_`) outright, drawn over a coordinate grid. |
| `NEWS.RIP` | 1.54-style flowed news article (`1T` linked columns, drop cap) - a 1995 TeleGrafix press essay on RIPscrip 2.0 - plus JUPITER.JPG. |
| `NEWSPAPR.RIP` | Newspaper front page, written as a reusable template: config variables name the story files (STORY01/02.TXT) and authors, resets overflow buffers, links the N2_* pages and DEMO-01/02.COL layouts. |
| `ONLINE.RIP` | "Online" title/art scene (Beziers); carries the inline comment "Set base math to MegaNums (base 36)" that identified the `J` opcode. |
| `POLYGONS.RIP` | Polygon primitive demo. |
| `POLYPOLY.RIP` | RIP_POLY_POLYGON demo: multi-contour polygons with even-odd fill ("so you can see the transparency aspect"), with and without borders. |
| `SAILBOAT.RIP` | Vector line-art sailboat (engineering menu). |
| `SEABYME1.RIP` | Seascape art scene built from filled poly-Beziers (name inference: "sea by me"). |
| `SEANITE.RIP` | Night-time seascape companion scene, also filled-poly-Bezier art (name inference: "sea at night"). |
| `SHADMOVE.RIP` | Moving drop-shadow text demo: assigns title text and colors, then calls `SHADOWDO.FN` - which does not ship. |
| `SHAPES.RIP` | Primitive gallery: every shape command including all skewed-oval variants, rounded rects, arcs, pies, and chords. |
| `SHOWFONT.RIP` | Interactive outline-font specimen viewer: RADIONEW radio buttons pick the face, `$&FONT_NAME$` interpolation renders it via SHOWFONT.FN. |
| `SHUTTLE.RIP` | Space-shuttle vector line drawing (engineering menu). |
| `SPACSHUT.RIP` | Second space-shuttle line drawing (engineering menu). |
| `SPECLEFX.RIP` | Special-effects demo: SHADOW.FN drop-shadow text, FXSHWIMG.FN JPEG reveals, image-cycling variables. |
| `TUNNEL.RIP` | Palette-animation tunnel effect: 65 RIP_ONE_DRAWING_PALETTE writes cycling colors through concentric filled ovals. |
| `TWEATHER.RIP` | Weather-map scene (bars, rectangles, filled polygons) - matches the "Weather Map" demo advertised in RIPtel's dialing directory. |
| `WIPE.RIP` | Random-wipe dispatcher: plays `$>wipe<<rand(24,2,4)>>.fn$` when `$TGMENU_WIPES$` is enabled. |

## 2. Function / wipe library (`.FN`, 48 files)

`.FN` files are the same wire format as `.RIP` - the extension is TeleGrafix's authoring convention for function/subroutine scenes invoked via `$>file$`/RIP_READ_SCENE.

| File | Function |
| --- | --- |
| `BLUEBACK.FN` | Solid dark-blue background - the under-256-color branch of the ubiquitous `<<IF $COLORS$<"256">>` background pick. |
| `BLUEFADE.FN` | 256-color faded blue background: the corpus's only RIP_SET_DRAWING_PALETTE use, plus custom fill patterns. |
| `DBACK.FN` | Dragon-story backdrop: STRIP6.BMP border strips, TORCH.BMP torches, GODRAG3.BMP dragon art, and the story's `1e` text columns. |
| `DL.FN` | Dragon page-turn: RIP_COPY_BLIT slide animation (leftward, per the name - inference), then re-reads DBACK.FN. |
| `DR.FN` | Dragon page-turn, rightward counterpart of DL.FN. |
| `FXSHWIMG.FN` | "FX show image" helper for SPECLEFX.RIP: sets the mouse cursor and image style, then displays a JPEG (ASTRO.JPG default). |
| `MAKEPORT.FN` | Fully commented port-setup helper: "Set 2 byte X/Y coordinates", "Set color palette mode", "Delete all ports", "Create port #1 to be full screen sized". |
| `MENU.FN` | Main-menu builder: resets the environment, draws the blue background, then reads MENU.DEF, MENU.MNU, and MENU.MSE. |
| `NEWPORT.FN` | Deletes and recreates drawing port #1 at full-screen size, then switches to it. |
| `ORIGIN.FN` | Environment reset to the standard prologue (base-36 math, 2-byte coordinates, 256-color mode, 1280×960 world) plus default colors (uncommented; role inferred from contents). |
| `REGISTER.FN` | Draws the "unregistered version" notice at the bottom of the main menu (paired with REGISTER.MSE). |
| `SHADOW.FN` | Drop-shadowed text routine used by SPECLEFX.RIP: offset RIP_EXTENDED_FONT_STYLE text with `$D(1)$` delays. |
| `SHOWFONT.FN` | Specimen-page renderer behind SHOWFONT.RIP: prints `$&FONT_NAME$` samples; carries the `1B` RIP_BUTTON_STYLE field-layout crib comment. |
| `TELCMDS.FN` | Builds the new-commands submenu: kill fields, blue-fade port, TELCMDS.DEF variables, TEL3X3 button screen + mouse fields, optional wipe. |
| `TELDEMO.FN` | Full-screen RIP_PORT_COPY snapshot helper (uncommented; role inferred from contents - note the near-collision with TELDEMOS.FN). |
| `TELDEMOS.FN` | Builds the demos submenu (TELDEMOS.DEF + TEL3X2 screen + fields). |
| `TELDRAW.FN` | Builds the drawing-demos submenu (TELDRAW.DEF + TEL3X2). |
| `TELENGIN.FN` | Builds the engineering-demos submenu (TELENGIN.DEF + TEL3X2). |
| `TELKILL.FN` | "Kill all mouse fields and entry/exit queries." |
| `TELLISTS.FN` | Builds the telnet-site-listings screen (TELLISTS.MNU + TELLISTS.MSE). |
| `TELPORT.FN` | Creates full-screen drawing port #1 with the blue-fade backdrop; its comments spell out the whole environment reset ("Set base math to MegaNums (36)", "Set world coordinates to 1280x960", …). |
| `TELQUEST.FN` | Builds the TELQUEST submenu (TELQUEST.DEF + TEL3X2). |
| `WIPE.FN` | Random-wipe dispatcher - same `$>wipe<<rand(24,2,4)>>.fn$` body as WIPE.RIP. |
| `WIPE00.FN` - `WIPE24.FN` | The 25-transition screen-wipe library: each wipes the previous screen off with a different pattern of RIP_PORT_COPY animation, ending switched back to port 0. Nearly pure port-copy - the `2C` opcode's 9,104 corpus occurrences (40% of all commands) live almost entirely here. |

## 3. Menu system (`.DEF` `.MNU` `.MSE` `.RET` `.ENT` `.EXT`, 30 files)

RIPtel's demo menus are a small framework of chained scenes, all in ordinary RIPscrip wire syntax - the extensions encode roles, per the census:

| Ext | Role | Confidence |
| --- | --- | --- |
| `.DEF` | Menu definition: sets `<<LABn>>` button labels, `<<CMDn>>` per-button command variables, and hover status messages as user variables, later macro-expanded into the `.MNU`/`.MSE` scenes. | Verified (TeleGrafix's own comments) |
| `.MNU` | Menu scene: draws title, copyright line, and the TELBUT.BMP button images with `<<LABn>>` labels. | Verified |
| `.MSE` | Mouse-field overlay: creates the fields over the button images, builds a status-line backup port, and installs mouse entry/exit queries. | Verified |
| `.RET` | Return/nav script: kills fields, rebuilds the port, redraws the owning menu, optionally through a wipe. | Verified |
| `.ENT` | Mouse-field _entry_ handler, run by the entry query (draws/restores the status-line area - "Paste original screen image back"). | Strong hypothesis (binding to entry queries inferred from `.MSE` comments) |
| `.EXT` | Mouse-field _exit_ handler, mirror of `.ENT`. | Strong hypothesis |

| File | Contents |
| --- | --- |
| `MENU.DEF` | Labels, commands, and hover status messages for the nine main-menu buttons. |
| `MENU.MNU` | Main-menu scene: nine TELBUT.BMP buttons ("Show the nine button images") + the unregistered-version notice (REGISTER.FN). |
| `MENU.MSE` | Mouse fields over the nine buttons; status-line backup port; entry/exit queries. |
| `MENU.ENT` / `MENU.EXT` | Status-line restore on field entry/exit ("Paste original screen image back"). |
| `MENU.RET` | Return to the main menu: kill fields, blue-fade port, redraw, optional WIPE01 transition. |
| `REGISTER.MSE` | Mouse fields for the registration notice. |
| `TEL3X2.MNU` / `TEL3X3.MNU` | Shared 3×2 / 3×3 button-screen scenes: title and copyright in Marin with dropshadow, TELBUT/TELBACK buttons, `<<LABn>>` labels. |
| `TEL3X2.MSE` / `TEL3X3.MSE` | Shared mouse-field overlays for those screens (status-line port + entry/exit queries). |
| `TEL3X2.ENT` / `TEL3X2.EXT` / `TEL3X3.ENT` / `TEL3X3.EXT` | Status-line save/restore handlers for the shared screens. |
| `TELCMDS.DEF` | Submenu config for the drawing-command demos: BUTTONS, CURVES, FONTS, IMAGES, MARKER, POLYGONS, POLYPOLY, SHAPES, SPECLEFX. |
| `TELCMDS.RET` | Return to the commands submenu, "Choose a random wipe to wipe off the screen image". |
| `TELDEMOS.DEF` | Submenu config for the headline demos: DRAGON, NEWSPAPR, ONLINE, plus links into the other submenus. |
| `TELDEMOS.RET` | Shared return path used by most demo scenes' back buttons. |
| `TELDRAW.DEF` | Submenu config for the drawing/art demos: LANDSCPE, LGF1. |
| `TELDRAW.RET` | Return to the drawing submenu with a random wipe. |
| `TELENGIN.DEF` | Submenu config for the "engineering" demos: FOUND, SAILBOAT, SHUTTLE, SPACSHUT, TWEATHER. |
| `TELENGIN.RET` | Return to the engineering submenu with a random wipe. |
| `TELLISTS.DEF` | Config for the telnet-site-listings screen. |
| `TELLISTS.MNU` | Telnet site listings: mouse fields set `$-=WEBURL=http://...$` then `$GOTOURL(WEBURL)$` - 1997-vintage web integration. References `TELLISTS.RET`, which does not ship. |
| `TELLISTS.MSE` / `TELLISTS.ENT` / `TELLISTS.EXT` | Mouse-field overlay and status-line handlers for the listings screen. |
| `TELQUEST.DEF` | Config for the TELQUEST submenu, wired back to MENU.RET / TELQUEST.RET ("QUEST" = questionnaire? - name inference). |
| `TELQUEST.RET` | Return to the main menu from the TELQUEST screens. |

## 4. Column scenes (`.COL`, 3 files)

Reusable page layouts for the 3.0 [column text system](../../ripscrip/3.1-column-text-system.md): each defines `1e` column regions and re-renders the current `$overflow(...)$` page, so prev/next buttons simply re-invoke the same `.COL` file.

| File | Contents |
| --- | --- |
| `DEMO-01.COL` | Newspaper main-story layout: three linked `1e` columns on stream 1, prev/next paging via `$overflow(1,prev,setverbose)$` / `$overflow(1,next,setverbose)$`, page counter, and nav mouse fields. |
| `DEMO-02.COL` | Secondary-story layout on stream 2, same paging machinery. |
| `FONTTEXT.COL` | Column layout used by FONTS.RIP to page the FONTSTOR.TXT story about the font and column systems. |

## 5. Story text (`.TXT`, 4 files)

Plain ASCII prose with bare-CR line breaks and no markup at all. RIP_READ_SCENE (`1R`) flows a `.TXT` target as raw text into the previously defined `1e` columns, auto-paginating into overflow buffers - layout comes entirely from the calling scene.

| File | Contents |
| --- | --- |
| `DRAGON.TXT` | The dragon story flowed by DRAGON.RIP - Jim Thompson's Boardwatch preface. |
| `STORY01.TXT` | TeleGrafix RIPscrip 3 press release - main story flowed by NEWSPAPR.RIP. |
| `STORY02.TXT` | Second TeleGrafix press release - the secondary NEWSPAPR.RIP story. |
| `FONTSTOR.TXT` | TeleGrafix's own description of the dual font system and the "powerful column system", flowed by FONTS.RIP. |

## 6. Images (102 `.BMP`, 7 `.JPG`, 5 `.BMH`)

BMPs load via `1b` RIP_LOAD_BITMAP or the `$<FILE.BMP$` macro; JPEGs display via `1i`/`1p` RIP_IMAGE_STYLE/RIP_IMAGE or `$(FILE.JPG$`. A `.BMH` is a plain BMP holding the pre-rendered highlight/pressed state of its same-named `.BMP` control skin, auto-paired by RIPtel (never referenced by script; see [File Formats](../../ripscrip/6.1-content-file-roles.md#bmh---button-highlight-variants)).

### UI skins and controls (16 `.BMP` + 5 `.BMH`)

| File | Description |
| --- | --- |
| `BACK.BMP` | 53×19 "back" navigation button - placed bottom-right by nearly every demo scene. |
| `BLANKBUT.BMP` | 21×17 blank small-button face. |
| `BUTTON.BMP` / `BUTTON.BMH` | 22×17 generic pushbutton skin + highlight state. |
| `CHECKBOX.BMP` / `CHECKBOX.BMH` | 17×17 checkbox control + highlight state (BUTTONS.RIP). |
| `HELPICON.BMP` | 64×44 help icon. |
| `LG_LEFT.BMP` / `LG_RIGHT.BMP` | 37×27 left/right arrow buttons (paging - name inference). |
| `NAVIGATE.BMP` | 53×19 navigation button used by FONTS.RIP and NEWSPAPR.RIP. |
| `NSEW_DIR.BMP` | 47×37 compass/direction pad (name inference). |
| `RADIO.BMP` / `RADIO.BMH` | 15×11 radio button + highlight state. |
| `RADIOBUT.BMP` / `RADIOBUT.BMH` | 25×17 radio button variant + highlight state. |
| `RADIONEW.BMP` / `RADIONEW.BMH` | 15×15 radio button used by BUTTONS.RIP and SHOWFONT.RIP font selectors, + highlight state. |
| `TELBACK.BMP` | 151×52 large "back" menu button (TEL3X2/TEL3X3 screens). |
| `TELBUT.BMP` | 151×52 large menu-button face - the skin behind every TEL* and MENU button. |
| `VCR2.BMP` | 394×48 VCR-style transport control bar (name inference). |
| `XBUTTON.BMP` | 22×17 pushbutton variant with an X mark (name inference). |

### Textures and backgrounds (3 `.BMP`)

| File | Description |
| --- | --- |
| `BRICK.BMP` | 64×28 brick tile, tiled by IMAGES.RIP. |
| `CONCRETE.BMP` | 39×14 concrete tile. |
| `STRIP6.BMP` | 320×17 decorative border strip, tiled by DBACK.FN around the dragon story. |

### Demo scene art (11 `.BMP`)

| File | Description |
| --- | --- |
| `256COLOR.BMP` | 80×50 256-color test image, shown at three scales by IMAGES.RIP. |
| `COLUMN.BMP` | 85×201 classical column clip-art (unreferenced - name inference). |
| `COMPUTER.BMP` | 122×225 computer clip-art (unreferenced - name inference). |
| `DAILER2.BMP` | 173×186 phone-dialer graphic (unreferenced - name inference). |
| `GEAR.BMP` | 93×70 256-color gear image (IMAGES.RIP). |
| `GODRAG3.BMP` | 185×93 dragon artwork for the DRAGON.RIP story backdrop. |
| `MAILBOX.BMP` | 146×189 mailbox clip-art (unreferenced - name inference). |
| `TORCH.BMP` | 65×128 torch sprite flanking the dragon story. |
| `WORLD-1.BMP` / `WORLD-2.BMP` / `WORLD-3.BMP` | 640×97-114 world-map bands - a world map split into three strips (unreferenced - name inference). |

### Photos (7 `.JPG`)

320×200 (GALAXY 320×240) baseline JFIF photos for the JPEG demos (N2_TITLE/N2_PHOTO/NEWSPAPR/SPECLEFX/IMAGES/NEWS). Subjects inferred from names:

| File           | Description                                         |
| -------------- | --------------------------------------------------- |
| `ASTRO.JPG`    | Space/astronomy photo; FXSHWIMG.FN's default image. |
| `BEACH2.JPG`   | Beach photo (N2_PHOTO gallery).                     |
| `BRIDGE02.JPG` | Bridge photo (N2_PHOTO, IMAGES.RIP).                |
| `DUSK_SEA.JPG` | Sea at dusk (N2_PHOTO gallery).                     |
| `FIRCLOUD.JPG` | Fiery clouds/sunset (N2_PHOTO gallery).             |
| `GALAXY.JPG`   | Galaxy photo (N2_TITLE, NEWSPAPR).                  |
| `JUPITER.JPG`  | Jupiter photo (N2_TITLE, NEWSPAPR, NEWS.RIP).       |

### Menu-icon library (72 `.BMP`)

BBS-menu icon art, overwhelmingly in a uniform ~64×44 16-color format - the size and palette of the classic RIPscrip 1.54 icon library, evidently converted from `.ICN` to BMP (the 3.0 driver has ICN→BMP conversion built in - hypothesis). Only FILECAB1 and the control skins above are actually referenced by the shipped scripts; all other descriptions are **inferred from filenames**.

| File | Description (name inference unless noted) |
| --- | --- |
| `3&HALF.BMP` | 3½-inch diskette. |
| `AE5.BMP` | American Express card logo (variant; card-shaped 67×36 like AMEREXP/VISA). |
| `ALCHEMY.BMP` | "Alchemy" - door game / fantasy icon. |
| `AMEREXP.BMP` | American Express card logo. |
| `ANDROIDS.BMP` | "Androids" game icon. |
| `ASCII.BMP` | ASCII/text-mode icon. |
| `BLADEMTR.BMP` | "Blade Master" game icon. |
| `BORDGAME.BMP` | Board games icon. |
| `BOUNCY.BMP` | "Bouncy" game icon. |
| `BOXES.BMP` | Boxes icon. |
| `BRICKS.BMP` | Bricks icon (icon-sized; distinct from the BRICK.BMP tile). |
| `BRIDGE.BMP` | Bridge (likely the card game, given the casino/poker set). |
| `CALCULTR.BMP` | Calculator (45×48). |
| `CAMEL.BMP` | Camel. |
| `CASINO.BMP` | Casino games. |
| `CHESS.BMP` | Chess. |
| `DATANK.BMP` | "Data Tank" (database?) icon. |
| `DC1.BMP` | Unidentified (74×43). |
| `DCVR.BMP` | Discover card logo. |
| `DIALDEMO.BMP` | Dialing-demo icon. |
| `DISKET2.BMP` | Diskette. |
| `DOLLAR.BMP` | Dollar sign. |
| `EERASE.BMP` | Message menu: erase message. |
| `EMAIL.BMP` | E-mail. |
| `EMODIFY.BMP` | Message menu: modify message. |
| `ERALL.BMP` | Message menu: read all messages. |
| `ERNEW.BMP` | Message menu: read new messages. |
| `EWRITE.BMP` | Message menu: write message. |
| `FEEDBAK2.BMP` | Feedback to sysop. |
| `FILECAB1.BMP` | File cabinet - used as a button icon by BUTTONS.RIP (referenced). |
| `FL1.BMP` / `FL1B.BMP` / `FL2.BMP` | File-library icons (variants). |
| `GAL_DATA.BMP` | 54×34 mini icon set ("GAL" = gallery/galleria menu?): database. |
| `GAL_DOLR.BMP` | Mini icon: dollar/billing. |
| `GAL_EWRT.BMP` | Mini icon: write message. |
| `GAL_FILE.BMP` | Mini icon: files. |
| `GAL_GAME.BMP` | Mini icon: games. |
| `GAL_INFO.BMP` | Mini icon: information. |
| `GAL_MAIL.BMP` | Mini icon: mail. |
| `GAL_NEWS.BMP` | Mini icon: news. |
| `GAMES.BMP` | Games (71×52). |
| `GE.BMP` | Unidentified ("GEnie" online service?). |
| `GOLF.BMP` | Golf game. |
| `GUESSWHT.BMP` | "Guess What" game. |
| `HANGMAN.BMP` | Hangman game. |
| `HORIZON.BMP` | Horizon scene icon. |
| `IC.BMP` | Unidentified. |
| `ICONDEMO.BMP` / `ICONDEM2.BMP` | Icon-demo icons. |
| `ICONS.BMP` | Icons menu. |
| `JUMBLE.BMP` | Word-jumble game. |
| `KYRANDIA.BMP` | "Legend of Kyrandia" (MajorBBS door game). |
| `MAGIC.BMP` | Magic. |
| `MC.BMP` | MasterCard logo. |
| `MODEDEMO.BMP` | Mode-demo icon. |
| `MOSAIC.BMP` | Mosaic. |
| `MOUSDEMO.BMP` | Mouse-demo icon. |
| `NEWS.BMP` / `NEWS2.BMP` | News icons. |
| `NOSHUF.BMP` | "No Shuffle" card game. |
| `POKER.BMP` | Poker. |
| `REGISTRY.BMP` | Registration/registry. |
| `ROBOWARS.BMP` | "RoboWars" game. |
| `SEABATLE.BMP` | "Sea Battle" game. |
| `SMASH.BMP` | "Smash" game. |
| `SOLITAIR.BMP` | Solitaire. |
| `VISA.BMP` | Visa card logo. |
| `WC_FILE.BMP` | File area ("WC" = Wildcat!?). |
| `WORDZ.BMP` | "Wordz" word game. |
| `WOW.BMP` | "WOW" icon. |
| `YAHTZEE.BMP` | Yahtzee game. |
