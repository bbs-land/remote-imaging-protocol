# Level-1 Commands

[◀ Prev: Level-0 Commands (S–Z)](10-level-0-commands-s-z.md) · [Contents](README.md) · [Next: Level-2 Commands ▶](12-level-2-commands.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

Level-1 commands are the user-interface and higher-level graphical constructs: queries, buttons, mouse fields, images, and formatted text regions. This is where the 3.0 era's multimedia claims become concrete — JPEG display ([`p`](#rip_image)), bitmap-skinned buttons ([`b`](#rip_load_bitmap) + [`U`](#rip_button)), the flowed-text column system ([`e`](#rip_extended_begin_text)), and a macro/conditional layer evaluated over command arguments before execution.

Commands: [`<ESC>` RIP_QUERY](#rip_query), [`A` (unidentified)](#1a-unidentified), [`B` RIP_BUTTON_STYLE](#rip_button_style), [`b` RIP_LOAD_BITMAP](#rip_load_bitmap), [`C` RIP_GET_IMAGE](#rip_get_image), [`c` RIP_SET_MOUSE_CURSOR](#rip_set_mouse_cursor), [`D` RIP_DEFINE](#rip_define), [`E` RIP_END_TEXT](#rip_end_text), [`e` RIP_EXTENDED_BEGIN_TEXT](#rip_extended_begin_text), [`F` RIP_FILE_QUERY](#rip_file_query), [`G` RIP_SCROLL](#rip_scroll), [`g` RIP_COPY_BLIT](#rip_copy_blit), [`I` RIP_LOAD_ICON](#rip_load_icon), [`i` RIP_IMAGE_STYLE](#rip_image_style), [`K` RIP_KILL_MOUSE_FIELDS](#rip_kill_mouse_fields), [`k` RIP_KILL_ENCLOSED_MOUSE_FIELDS](#rip_kill_enclosed_mouse_fields), [`M` RIP_MOUSE](#rip_mouse), [`N` (unidentified)](#1n-unidentified), [`O` (unidentified)](#1o-unidentified), [`P` RIP_PUT_IMAGE](#rip_put_image), [`p` RIP_IMAGE](#rip_image), [`R` RIP_READ_SCENE](#rip_read_scene), [`S` (unidentified)](#1s-unidentified), [`T` RIP_BEGIN_TEXT](#rip_begin_text), [`t` RIP_REGION_TEXT](#rip_region_text), [`U` RIP_BUTTON](#rip_button), [`W` RIP_WRITE_ICON](#rip_write_icon), [`w` RIP_PLAY_AUDIO](#rip_play_audio).

## RIP_QUERY

*Query the contents of a text variable*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `Σ` (the Escape character, ASCII 27) |
| **Arguments** | `mode:1 window_num:1 res:2 query_text` |

**Format:** `!|1<escape> <mode> <window_num> <res> <query_text>`
**Example:** `!|1<ESC>0000$COMPAT$`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_query)) · HLP (`RIP_Query`; query OBJECT/OPTION keywords, resident queries) · corpus (489 uses in 62 files) · SyncTERM (ripper.c:14881, descriptor `1#3#++`)

The non-printable ESC command character keeps queries under host control (users cannot normally type ESC). Mode 0 processes the query text immediately; modes 1–4 defer it to a mouse click in a viewport or text window (see the 2.x entry). The corpus is saturated with the immediate form used as a *command executor* rather than a data query: `$COMPAT$` (set 1.54-compatible environment), `$COFF$$DTW$` (cursor off + deactivate text window), `$D(1)$` (delay — 197 uses driving the wipe animations), `$MCURSOR(n)$`, `$SBAROFF$`, and conditional dispatch such as `!|1<ESC>0000<<IF $TGMENU_WIPES$="1">>$>WIPE01.FN$<<else>>$NULL$<<ENDIF>>`. Deferred modes 5/6 appear in the corpus wired to mouse-field *entry/exit* status-line updates (`.MSE` overlays), a 3.0 extension of the documented click modes. One authoring typo (CURVES.RIP's `|1<ESC>…` with no `!`) shows the shipping driver's tolerant introducer handling.

## 1A (Unidentified)

*Unknown — text-flow settings?* *(hypothesis)*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `A` |
| **Arguments** | observed: 6 digits (3×`2#`); SyncTERM descriptor: `2#2#2#2#2#2#2#` (7 fields — does **not** match) |

**Format:** `!|1A <arg1> <arg2> <arg3>` *(layout unresolved)*
**Example:** `!|1A010000`

**Attributes used:** unknown
**Evidence:** corpus (NEWS.RIP, single use) · SyncTERM (ripper.c:9177 — descriptor-only, no handler)

One of SyncTERM's descriptor-only leads, now corpus-observed exactly once: NEWS.RIP issues `!|1A010000` immediately before building its flowed newspaper article, between a divider [RIP_LINE](09-level-0-commands-g-r.md#rip_line) and the linked `1T…01/11/21/31` text columns. The observed 6-character argument contradicts SyncTERM's 7-field descriptor, so neither layout can be trusted. Position and value (`01…`) suggest justification/hyphenation settings for the text-flow system *(hypothesis)*.

## RIP_BUTTON_STYLE

*Button style definition*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `B` |
| **Arguments** | `wid:XY hgt:XY orient:2 flags:4 size:2 dfore:2 dback:2 bright:2 dark:2 surface:2 grp_no:2 flags2:2 uline_col:2 corner_col:2 other_port:1 res:5` |

**Format:** `!|1B <wid> <hgt> <orient> <flags> <bevsize> <dfore> <dback> <bright> <dark> <surface> <grp_no> <flags2> <uline_col> <corner_col> <other_port> <res>`
**Example:** `!|1B0000020PVS080F000F080700000F07000000`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_button_style)) · HLP (`RIP_ButtonStyle`; limits — label ≤255 chars, hotkey ≤255, groups 0–35, types "plain, icon or snapshot") · corpus (47 uses in 18 files) · SyncTERM (ripper.c:15015)

The most complex command in the protocol: it defines how subsequent [RIP_BUTTON](#rip_button) commands render — type (icon/clipboard/plain), special effects (bevel, chisel, recess, sunken), label orientation and colors, radio/checkbox groups (see the 2.x entry for both full flag tables). TeleGrafix left the positional crib for its 36-character argument block as a comment in SHOWFONT.FN:

```text
!|! wwhhooffffssffbbBBddssgg22uuccprrrrr
```

HLP's "snapshot" button type corresponds to the 2.00a4 clipboard button. The 3.0 driver keeps 36 button-style slots (0–35), switched with [RIP_SWITCH_BUTTON_STYLE](12-level-2-commands.md#rip_switch_button_style).

## RIP_LOAD_BITMAP

*Loads and displays a disk-based bitmap to screen*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `b` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY duplicate_port:1 mode:1 flags:2 trans_color:2 res:4 filename` |

**Format:** `!|1b <x0> <y0> <x1> <y1> <duplicate_port> <mode> <flags> <trans_color> <res> <filename>`
**Example:** `!|1b0000HS0Y000G000000STRIP6.BMP`

**Attributes used:** Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_load_bitmap)) · HLP (`RIP_LoadBitmap`; also the `$<FILE.BMP$` macro shorthand) · corpus (69 uses in 28 files) — not in SyncTERM

The scaling, dithering, transparency-capable bitmap loader (added v2.A1) that superseded [RIP_LOAD_ICON](#rip_load_icon). The corpus uses it constantly: the example (DRAGON.RIP) stretches STRIP6.BMP across a 640×34 header band; DBACK.FN stamps TORCH.BMP with flag 8 + `trans_color` for transparent-color compositing; menu scenes place TELBUT.BMP button skins with `(x1,y1)` zero for verbatim size. Flags select duplicate-to-port copies, palette commitment, auto-dithering, and transparency (full table in the 2.x entry). HLP notes the driver "Can't show compressed bitmaps".

## RIP_GET_IMAGE

*Copy rectangular image to the clipboard port*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `C` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY res:1` |

**Format:** `!|1C <x0> <y0> <x1> <y1> <res>`
**Example:** `!|1CEM6XFZ7U0`

**Attributes used:** Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_get_image)) · HLP (`RIP_GetImage`; clipboard port concept, `$SCB$`/`$RCB$`/`$P$` variables) · corpus (4 uses in LANDSCPE.RIP) · SyncTERM (ripper.c:15847)

Copies a screen rectangle to the clipboard port (the first free offscreen port, tracked by the clipboard pointer). LANDSCPE.RIP grabs small texture tiles this way for re-stamping with [RIP_PUT_IMAGE](#rip_put_image). For port-addressed copies the 3.0 corpus overwhelmingly prefers the level-2 [RIP_PORT_COPY](12-level-2-commands.md#rip_port_copy).

## RIP_SET_MOUSE_CURSOR

*Sets the mouse cursor (pointer) to various shapes*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `c` |
| **Arguments** | `cursor_style:2 res:4` |

**Format:** `!|1c <cursor_style> <res>`
**Example:** `!|1c06`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_set_mouse_cursor)) · HLP (`RIP_SetMouseCursor`; also the `$MCURSOR(n)$` variable) · corpus (2 uses in FXSHWIMG.FN) — not in SyncTERM

Switches the pointer among seven styles (00 arrow … 06 hourglass). FXSHWIMG.FN brackets its slow JPEG display with `!|1c06` / `!|1c00` — hourglass on, arrow back — omitting the reserved digits entirely, which the shipping driver's truncation-tolerant parser accepts. Most corpus cursor changes go through the `$MCURSOR(n)$` variable inside [RIP_QUERY](#rip_query) commands instead.

## RIP_DEFINE

*Define a text variable*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `D` |
| **Arguments** | `flags:3 res:2 text` |

**Format:** `!|1D <flags> <res> <text>`
**Example:** `!|1D00700text_var,60:?question?default data`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_define) — flagged obsolete there already) · HLP (`RIP_Define`; the DLL also exports `RIP_DefineTextVariable`) · SyncTERM (ripper.c:15899) — not observed in the RIPtel demo corpus

Creates a text variable, optionally saved to the terminal's database (RIPSCRIP.DB in the 3.0 driver) and optionally prompting the user. Already declared obsolete in 2.00a4 in favor of the enhanced in-line variable syntax, and the corpus confirms the succession: the demos define hundreds of variables exclusively through the `$-=NAME=value$` set form and the `$[directives]NAME[@question][=default]$` prompt syntax, never `1D`.

## RIP_END_TEXT

*End a rectangular text region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `E` |
| **Arguments** | `<none>` |

**Format:** `!|1E`
**Example:** `!|1E`

**Attributes used:** Viewport, Port, Base Math (N/A)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_end_text)) · HLP (`RIP_EndText`) · corpus (20 uses in 8 files) · SyncTERM (ripper.c:16090)

Terminates a formatted text block — both the 1.54 [RIP_BEGIN_TEXT](#rip_begin_text)/[RIP_REGION_TEXT](#rip_region_text) form and the 3.0 [`1e` column-region](#rip_extended_begin_text) form, whose raw-text content runs until this command.

## RIP_EXTENDED_BEGIN_TEXT

*Open a flowed-text column region* *(hypothesized name)*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `e` |
| **Arguments** | `x0:2 y0:2 x1:2 y1:2 column:1 stream:1 flags:4 unknown:2 reserved:8` *(from TeleGrafix's crib; see below)* |

**Format:** `!|1e <x0> <y0> <x1> <y1> <column> <stream> <flags> <cc> <reserved>`
**Example:** `!|1e3W7DGRMD0100010000000000`

**Attributes used:** Draw Color, Font Style, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (21 uses in 8 files: NEWSPAPR.RIP, DEMO-01.COL, N2_HORO.RIP, DBACK.FN, DRAGON.RIP and others; field map in FONTS.RIP) — not in the 2.00a4 spec, not in SyncTERM. Name *(hypothesis)*; HLP corroborates the mechanism (`RIP_SelectArticle`, "Invalid text article number", "Invalid text column number").

The heart of the "powerful column system" RIPtel's FONTSTOR.TXT advertises. TeleGrafix's field crib in FONTS.RIP:

```text
!|! xxyyxxyycaffffccrrrrrrrr
```

— bounding rectangle (2 digits per coordinate), `c` = column number, `a` = article/stream number, `ffff` = flags, `cc` = unknown, `rrrrrrrr` = reserved. After the command, **raw text lines with no `!|` introducer** are the column content, terminated by [RIP_END_TEXT](#rip_end_text) (`1E`); content can equally be flowed in by [RIP_READ_SCENE](#rip_read_scene) pointed at a plain `.TXT` file, an `$OVERFLOW(stream,…)$` page, or a `$&VARIABLE$`. Multi-column layouts chain regions with column numbers 0, 1, 2… sharing one stream (DEMO-01.COL links three columns); text that does not fit paginates into numbered overflow buffers, paged with `$overflow(stream, cur|next|prev[,setverbose])$` wired to prev/next buttons and cleared with `$RESET(OVERFLOW)$`. The RIPSCRIP.HLP inventory's otherwise-unplaced `RIP_SelectArticle` name almost certainly belongs to this article/column machinery *(hypothesis)*.

## RIP_FILE_QUERY

*Query existing information on a particular file*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `F` |
| **Arguments** | `mode:2 res:4 filename...` |

**Format:** `!|1F <mode> <res> <filename>`
**Example:** `!|1F010000testfile.icn`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_file_query)) · HLP (`RIP_FileQuery`) · SyncTERM (ripper.c:16104) — not observed in the RIPtel demo corpus

Asks the terminal whether a file exists in its local cache, optionally returning size/date/time — the host side of the 3.0 resource-caching story. The self-contained demo corpus has no host to answer, so it never appears there; SyncTERM implements it with its per-BBS cache directory.

## RIP_SCROLL

*Copy (scroll) screen region up/down (formerly RIP_COPY_REGION)*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `G` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY mode:1 res:1 dest_line:2` |

**Format:** `!|1G <x0> <y0> <x1> <y1> <mode> <res> <dest_line>`
**Example:** `!|1G080G140M0005`

**Attributes used:** Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_scroll)) · HLP (`RIP_Scroll`) · SyncTERM (ripper.c:16246 — **documentation-only stub**: full 1.54 spec comment, then a bare `break;`) — not observed in the RIPtel demo corpus

Vertically scrolls a screen rectangle to a destination scan line, with a mode selecting what fills the vacated area. It is the one command SyncTERM documents in-source but does not implement — recognized and swallowed. The corpus does all of its region moving with [RIP_COPY_BLIT](#rip_copy_blit) and level-2 [RIP_PORT_COPY](12-level-2-commands.md#rip_port_copy) instead.

## RIP_COPY_BLIT

*Copy a screen area to a new location (bit blit)*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `g` |
| **Arguments** | `x0:2 y0:2 x1:2 y1:2 dx0:2 dy0:2 mode:1 res:1` |

**Format:** `!|1g <x0> <y0> <x1> <y1> <dx0> <dy0> <mode> <res>`
**Example:** `!|1g0000XCQO280000`

**Attributes used:** Draw Color, Back Color, Fill Style, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_copy_blit)) · HLP (`RIP_CopyBlit`) · corpus (88 uses in 6 files) — not in SyncTERM

Generic same-port bit-blit (added v2.A1): moves the rectangle (x0,y0)–(x1,y1) so its top-left lands at (dx0,dy0), with a mode selecting how the vacated area is cleared. The corpus uses it as a *within-screen* slide primitive in the wipe/transition library (WIPE00–02.FN, DL.FN, DR.FN): the example shifts a 1080×960-wide slab 72 world-units right in one step of a push transition.

## RIP_LOAD_ICON

*Loads and displays a disk-based icon to current port*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `I` |
| **Arguments** | `x:XY y:XY mode:2 clipboard:1 scale:1 res:1 filename` |

**Format:** `!|1I <x> <y> <mode> <clipboard> <scale> <res> <filename>`
**Example:** `!|1I001101010button`

**Attributes used:** Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_load_icon)) · HLP (`RIP_LoadIcon`; plus 1.54-compat strings "Can't convert RIP 1.54 Icon %s to 2.0 BMP format!") · SyncTERM (ripper.c:16302) — not observed in the RIPtel demo corpus

The 1.54-lineage icon stamper (paste modes COPY/XOR/OR/AND/NOT, optional clipboard copy, 640×350-relative scaling). The 3.0 driver retains it chiefly for backward compatibility — HLP shows built-in `.ICN`→BMP conversion — but the corpus ships no `.ICN` files at all and always uses [RIP_LOAD_BITMAP](#rip_load_bitmap).

## RIP_IMAGE_STYLE

*Alter subsequent displayed image settings*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `i` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY flags:4 res:12` |

**Format:** `!|1i <x0> <y0> <x1> <y1> <flags> <res>`
**Example:** `!|1i1E4I80940004000000000000`

**Attributes used:** Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_image_style)) · HLP (`RIP_ImageStyle`; also the `$IMGSTYLE(cur,x0,y0,x1,y1)$` variable) · corpus (13 uses in 6 files) — not in SyncTERM

Defines the display rectangle and flags (aspect-preserve, delete-after, no-clear, commit-palette) for subsequently displayed image files. The example (N2_TITLE.RIP) sets flag 4 — don't clear the area first — for its inset newspaper photo. Corpus uses also appear in the truncated 12-character form `!|1i00009F6Q0004` with the reserved block omitted. Pairs with [RIP_IMAGE](#rip_image) below.

## RIP_KILL_MOUSE_FIELDS

*Destroys all previously defined hot mouse regions*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `K` |
| **Arguments** | `<none>` |

**Format:** `!|1K`
**Example:** `!|1K`

**Attributes used:** Base Math (N/A)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_kill_mouse_fields)) · HLP (`RIP_KillMouseFields`; also the `$MKILL$` variable) · corpus (8 uses in 8 files) · SyncTERM (ripper.c:16370)

Forgets all mouse regions; used at scene start so one scene's fields don't leak into the next. The corpus's TELKILL.FN ("Kill all mouse fields and entry/exit queries") wraps it as a reusable subroutine scene.

## RIP_KILL_ENCLOSED_MOUSE_FIELDS

*Destroys any Mouse Fields inside a region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `k` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY flags:4` |

**Format:** `!|1k <x0> <y0> <x1> <y1> <flags>`
**Example:** `!|1k00003G2H`

**Attributes used:** Fill Style (per flags), Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_kill_enclosed_mouse_fields)) · HLP (`RIP_KillEnclosedMouseFields`) — not observed in the RIPtel demo corpus, not in SyncTERM

Selectively destroys mouse fields contained in, intersecting, or outside a rectangle, with flags controlling which field types survive and whether the vacated regions are cleared or pattern-filled (added v2.A0; full flag tables in the 2.x entry). The demos always clear all fields at once instead.

## RIP_MOUSE

*Defines a rectangular hot mouse field*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `M` |
| **Arguments** | `num:2 x0:XY y0:XY x1:XY y1:XY clk:1 clr:1 res:5 text` |

**Format:** `!|1M <num> <x0> <y0> <x1> <y1> <clk> <clr> <res> <text>`
**Example:** `!|1M000G7Y8ICC1000000ID=2:$-=RETURN=>NEWSPAPR.RIP$$>TWEATHER.RIP$`

**Attributes used:** Viewport, Port (screen ports only), Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_mouse)) · HLP (`RIP_Mouse`; `$FIELDID$` variable, mouse-field entry/exit queries) · corpus (100 uses in 31 files) · SyncTERM (ripper.c:16381)

Ties a screen rectangle to a host-command string sent on click. The 3.0 corpus reveals two undocumented extensions to the `<text>` parameter:

- **`ID=n:` prefix** — assigns a numbered mouse-field identity, readable back through the `$FIELDID$` variable and used by the `.MSE` overlay scenes' entry/exit queries to drive hover status lines and cursor changes (the example, from NEWSPAPR.RIP's navigation, is field ID 2).
- **Inline conditionals and macros** — `<<if $RETURN$!="">>$<<RETURN>>$<<else>>$NULL$<<endif>>` as a host command (BUTTONS.RIP), `$GOTOURL(WEBURL)$` launching a web URL from a variable (TELLISTS.MNU/.MSE — 1997-era web integration).

Mouse fields remain limited to screen ports and are scanned last-in-first-out, as documented.

## 1N (Unidentified)

*Unknown — the only 5-digit fields in SyncTERM's table*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `N` |
| **Arguments** | SyncTERM descriptor: `2#2#5#5#+5#` |

**Format:** `!|1N <a:2> <b:2> <c:5> <d:5> [<e:5>]...` *(identity unknown)*
**Example:** *(none attested)*

**Attributes used:** unknown
**Evidence:** SyncTERM (ripper.c:9181 — descriptor-only, no handler) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP inventory, not observed in the RIPtel demo corpus

The most tantalizing descriptor-only lead: its 5-digit MegaNum fields (range 0–60,466,175) are unique in SyncTERM's entire table, a range suggesting file sizes or byte offsets. Whatever traffic taught SyncTERM's authors this shape, no other evidence of `1N` survives.

## 1O (Unidentified)

*Unknown — four-coordinate region command*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `O` |
| **Arguments** | SyncTERM descriptor: `2#2#2#2#` |

**Format:** `!|1O <x0> <y0> <x1> <y1>` *(identity unknown; field roles are hypothesis)*
**Example:** *(none attested)*

**Attributes used:** unknown
**Evidence:** SyncTERM (ripper.c:9178 — descriptor-only, no handler) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP inventory, not observed in the RIPtel demo corpus

Four 2-digit fields — the shape of a rectangle/region command *(hypothesis)*.

## RIP_PUT_IMAGE

*Pastes the clipboard port's contents to another port*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `P` |
| **Arguments** | `x:XY y:XY mode:2 res:1` |

**Format:** `!|1P <x> <y> <mode> <res>`
**Example:** `!|1PV4OM000`

**Attributes used:** Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_put_image)) · HLP (`RIP_PutImage`; `$P$` paste-clipboard variable) · corpus (1 use in NEWSPAPR.RIP) · SyncTERM (ripper.c:16481)

Pastes the clipboard port's contents at (x,y) in the active drawing port with a raster mode. Its single corpus appearance re-stamps a captured screen region in the newspaper demo; everywhere else the demos use the more general [RIP_PORT_COPY](12-level-2-commands.md#rip_port_copy).

## RIP_IMAGE

*Display a scalable photo type image*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `p` |
| **Arguments** | `res:4 filename` |

**Format:** `!|1p <res> <filename>`
**Example:** `!|1p0000ASTRO.JPG`

**Attributes used:** Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_image)) · HLP (`RIP_Image`; JPEG engine strings — jpegShow, LoadJPEGBuffer; the `$(FILE.JPG$` macro) · corpus (13 uses in 6 files, 16 JPEG references) — not in SyncTERM

Displays a JPEG from the terminal's local cache inside the region set by [RIP_IMAGE_STYLE](#rip_image_style) — the flagship 3.0 multimedia feature (SyncTERM's `RIPSCRIP030001` mode has no raster support at all, making this a clean 3.0-only differentiator). The corpus's photo scenes (N2_PHOTO.RIP, N2_TITLE.RIP, IMAGES.RIP, FXSHWIMG.FN) cycle ASTRO/GALAXY/JUPITER/BEACH2/BRIDGE02/DUSK_SEA/FIRCLOUD JPEGs. SPECLEFX.RIP demonstrates the conditional slideshow idiom, the filename argument computed by the macro layer at execution time:

```text
!|1p0000<<IF $INUSE(TV,NEXT_IMG)$="0">>$-=NEXT_IMG=ASTRO.JPG$<<ENDIF>>$&NEXT_IMG$
```

— if the `NEXT_IMG` variable is unset, initialize it, then display whatever it dereferences to.

## RIP_READ_SCENE

*Playback local .RIP file*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `R` |
| **Arguments** | `res:8 filename...` |

**Format:** `!|1R <res> <filename>`
**Example:** `!|1R00000000DRAGON.TXT`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Font Style, Port (dependent on file contents), Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_read_scene)) · HLP (`RIP_ReadScene`; "Can't locate local RIPscrip scene file %s"; DLL export `RIP_PlaybackLocalRIPFile`) · corpus (92 uses in 34 files) · SyncTERM (ripper.c:16534)

Suspends the incoming stream and plays a local scene file, resuming afterward with any state changes intact. In the 3.0 corpus this is the *subroutine call* of an entire scene architecture — `.FN` function scenes, `.DEF` variable definitions, `.MSE` overlays — and it gains three undocumented argument forms:

- **Plain `.TXT` files**: `!|1R00000000DRAGON.TXT` inside an open [`1e` column region](#rip_extended_begin_text) flows the file's text into the columns, word-wrapped — it is *not* parsed as RIPscrip.
- **Overflow pages**: `!|1R00000000$OVERFLOW(1,CUR)$` re-reads the current page of a column stream's overflow buffer.
- **Computed filenames**: `!|1R00000000<<IF $COLORS$<"256">>BLUEBACK.FN<<ELSE>>BLUEFADE.FN<<ENDIF>>` — the corpus's ubiquitous color-depth dispatch — and `$&MAIN_STORY$` variable dereferences (NEWSPAPR.RIP).

As in 2.00a4, the command must be the last on its line, terminated by a carriage return.

## 1S (Unidentified)

*Unknown — two 4-digit fields*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `S` |
| **Arguments** | SyncTERM descriptor: `4#4#` |

**Format:** `!|1S <a:4> <b:4>` *(identity unknown)*
**Example:** *(none attested)*

**Attributes used:** unknown
**Evidence:** SyncTERM (ripper.c:9182 — descriptor-only, no handler) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP inventory, not observed in the RIPtel demo corpus

Two 4-digit MegaNum fields (each 0–1,679,615). No corpus sighting and no matching 2.00a4 command.

## RIP_BEGIN_TEXT

*Define a rectangular text region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `T` |
| **Arguments** | `x1:XY y1:XY x2:XY y2:XY res:2` |

**Format:** `!|1T <x1> <y1> <x2> <y2> <res>`
**Example:** `!|1T0F3M434G00`

**Attributes used:** Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_begin_text)) · HLP (`RIP_BeginText`) · corpus (12 uses in N2_TITLE.RIP, NEWS.RIP) · SyncTERM (ripper.c:16581)

Opens the classic rectangular text region filled by [RIP_REGION_TEXT](#rip_region_text) lines and closed by [RIP_END_TEXT](#rip_end_text). The 3.0 driver overloads the trailing "reserved" digits: NEWS.RIP chains four regions with trailing values `01`, `11`, `21`, `31` — column/stream selectors feeding the same flowed-text engine as [`1e`](#rip_extended_begin_text), linking the four page columns into one article stream *(field interpretation from census analysis)*.

## RIP_REGION_TEXT

*Display a line of text in rectangular text region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `t` |
| **Arguments** | `justify:1` and text-string |

**Format:** `!|1t <justify> <text-string>`
**Example:** `!|1t1Southern California was pounded`

**Attributes used:** Draw Color, Write Mode, Font Style, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_region_text)) · HLP (`RIP_RegionText`) · corpus (42 uses in N2_TITLE.RIP) · SyncTERM (ripper.c:16623)

One pre-wrapped line inside a [RIP_BEGIN_TEXT](#rip_begin_text) region; justify `1` pads word spacing to align both margins, `0` leaves the line left-justified (the corpus uses `0` for each paragraph's final line — N2_TITLE.RIP's article, quoted in full in the census, is a TeleGrafix pitch for RIPscrip's JPEG/sound/24-bit future).

## RIP_BUTTON

*Define a Mouse Button*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `U` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY hotkey:2 flags:1 res:1 text` |

**Format:** `!|1U <x0> <y0> <x1> <y1> <hotkey> <flags> <res> <text>`
**Example:** `!|1U2E5O747G2000<>Hotkeys<>`

**Attributes used:** Font Style, Viewport, Port (non-mouse buttons only), Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_button)) · HLP (`RIP_Button`; limits label ≤255, hotkey ≤255) · corpus (75 uses in 17 files) · SyncTERM (ripper.c:16764)

Instantiates a button using the current [RIP_BUTTON_STYLE](#rip_button_style), with the three-part `icon<>label<>host-command` text parameter (all combinations in the 2.x entry). The 3.0 corpus shows the shipped button aesthetic: rather than 1.54-style chiseled plain buttons, scenes stamp **bitmap skins** with [RIP_LOAD_BITMAP](#rip_load_bitmap) (TELBUT.BMP, RADIONEW.BMP, CHECKBOX.BMP) and lay `1U` buttons or `1M` mouse fields over them; the corresponding `.BMH` files on disk are the pre-rendered **highlight-state** variants automatically shown while a hot-icon button is depressed (they are never named in any script — pairing is by filename convention). `1B` + `1U` with empty `<><>` blocks also serve as pure chisel-frame decorations. The example above is a plain radio-group demo button whose host command is empty — its behavior comes from the button-style flags.

## RIP_WRITE_ICON

*Write contents of the clipboard port to disk*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `W` |
| **Arguments** | `res:1 filename` |

**Format:** `!|1W <res> <filename>`
**Example:** `!|1W0filename.icn`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_write_icon)) · HLP (`RIP_WriteIcon`) · SyncTERM (ripper.c:16969) — not observed in the RIPtel demo corpus

Saves the clipboard port's viewport to disk as a BMP, for later reload with [RIP_LOAD_ICON](#rip_load_icon)/[RIP_LOAD_BITMAP](#rip_load_bitmap). The demos keep their snapshots in offscreen ports instead of round-tripping through disk.

## RIP_PLAY_AUDIO

*Play a local digitized audio file on the terminal*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `w` |
| **Arguments** | `res:4 filename` |

**Format:** `!|1w <res> <filename>`
**Example:** `!|1w0000filename.wav`

**Attributes used:** none
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/11-level-1-commands.md#rip_play_audio)) · HLP (`RIP_PlayAudio`; the `$)FILE.WAV$` macro, `AUDIO=TRUE` in RIPscrip.ini, DLL export `RIP_AudioSupport`) — not observed in the RIPtel demo corpus, not in SyncTERM

Starts background playback of a local `.WAV` file (`$OFF$` as the filename stops any playing sound). The 3.0 driver plainly supports it — audio configuration, tone-command error strings, and named sound variables (`$MUSIC$`, `$PHASER$`…) all appear in the HLP material — but the demo corpus ships no audio files and never issues the command.

---

[◀ Prev: Level-0 Commands (S–Z)](10-level-0-commands-s-z.md) · [Contents](README.md) · [Next: Level-2 Commands ▶](12-level-2-commands.md)
