# Level-0 Commands (G–R)

[◀ Prev: Level-0 Commands (Symbols & A–F)](08-level-0-commands-symbols-a-f.md) · [Contents](README.md) · [Next: Level-0 Commands (S–Z) ▶](10-level-0-commands-s-z.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

This page covers the level-0 commands with opcodes `G` through `R`, including the 3.0 corpus's most important single discovery: [`J` RIP_SET_BASE_MATH](#rip_set_base_math), the wire opcode that resolves the 2.00a4 draft's `b` collision and opens the standard prologue of nearly every RIPtel demo scene.

Commands: [`G` RIP_FILLED_CIRCLE](#rip_filled_circle), [`g` RIP_GOTOXY](#rip_gotoxy), [`H` RIP_HOME](#rip_home), [`h` RIP_HEADER](#rip_header), [`I` RIP_PIE_SLICE](#rip_pie_slice), [`i` RIP_OVAL_PIE_SLICE](#rip_oval_pie_slice), [`J` RIP_SET_BASE_MATH](#rip_set_base_math), [`j` RIP_POINT](#rip_point), [`K` RIP_FILLED_RECTANGLE](#rip_filled_rectangle), [`k` RIP_BACK_COLOR](#rip_back_color), [`L` RIP_LINE](#rip_line), [`l` RIP_POLYLINE](#rip_polyline), [`M` RIP_SET_COLOR_MODE](#rip_set_color_mode), [`m` RIP_MOVE](#rip_move), [`N` RIP_SET_BORDER](#rip_set_border), [`n` RIP_SET_COORDINATE_SIZE](#rip_set_coordinate_size), [`O` RIP_OVAL](#rip_oval), [`o` RIP_FILLED_OVAL](#rip_filled_oval), [`P` RIP_POLYGON](#rip_polygon), [`p` RIP_FILLED_POLYGON](#rip_filled_polygon), [`Q` RIP_SET_PALETTE](#rip_set_palette), [`R` RIP_RECTANGLE](#rip_rectangle).

## RIP_FILLED_CIRCLE

*Draw a filled circle in current color/line style*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `G` |
| **Arguments** | `x_center:XY y_center:XY radius:XY` |

**Format:** `!|G <x_center> <y_center> <radius>`
**Example:** `!|GDM2020`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_filled_circle)) · HLP (`RIP_FilledCircle`) · corpus (16 uses in POLYPOLY.RIP) — not in SyncTERM

Filled, aspect-ratio-corrected circle (added v2.A2); the interior uses the current fill pattern/color, the outline the current drawing color when borders are enabled. POLYPOLY.RIP uses it for the colored circle placed behind its [poly-polygons](08-level-0-commands-symbols-a-f.md#rip_poly_polygon) to show fill transparency.

## RIP_GOTOXY

*Move text cursor to row & column in Text Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `g` |
| **Arguments** | `x:2 y:2` |

**Format:** `!|g <x> <y>`
**Example:** `!|g0509`

**Attributes used:** Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_gotoxy)) · HLP (`RIP_GotoXY`, plus `$TWGOTO$` variable) · SyncTERM (ripper.c:14428) — not observed in the RIPtel demo corpus

Positions the text cursor in the TTY text window (0-based, unlike its 1-based ANSI equivalent). The graphics-only demos keep their text windows deactivated and never need it.

## RIP_HOME

*Move cursor to upper-left corner of Text Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `H` |
| **Arguments** | `<none>` |

**Format:** `!|H`
**Example:** `!|H`

**Attributes used:** none
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_home)) · HLP (`RIP_Home`, plus `$TWHOME$` variable) · SyncTERM (ripper.c:13687) — not observed in the RIPtel demo corpus

Homes the text cursor, equivalent to ANSI `ESC[1;1H`.

## RIP_HEADER

*Header command for subsequent RIPscrip sequence*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `h` |
| **Arguments** | `revision:2 flags:4 res:2` |

**Format:** `!|h <revision> <flags> <res>`
**Example:** `!|h010A0100`

**Attributes used:** Port, Base Math (UltraNums only)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_header)) · HLP (header-segment error strings — see below) — not observed in the RIPtel demo corpus, not in SyncTERM

The 2.0-era scene header: it declares the revision of the RIPscrip code that follows (up to the next [RIP_NO_MORE](08-level-0-commands-symbols-a-f.md#rip_no_more)) and performs selective resets — MegaNum/UltraNum selection, world-frame auto-set, hard/soft reset, table clears, input lockout — via a rich flags field (see the 2.x entry for the full flag table).

The 3.0 driver clearly retains and extends this mechanism even though the demo corpus (local scene files) never sends the command itself: RIPSCRIP.HLP's error strings describe **header segments** carrying flags plus entry numbers for the button style, graphics style, drawing port, text window, color palette, environment, mouse field, audio, and graphics screen tables — "Invalid general header flags", "Invalid coordinate size in environment header segment", "Invalid direct RGB bit count in environment header segment", and "Invalid RIPscrip revision code". The demos achieve the same effect piecewise with `!|*` plus the `J10|n2000|M08|fZKQO` prologue.

## RIP_PIE_SLICE

*Draws a circular pie slice*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `I` |
| **Arguments** | `x:XY y:XY start_ang:2 end_ang:2 radius:XY` |

**Format:** `!|I <x> <y> <start_ang> <end_ang> <radius>`
**Example:** `!|I1E18003G15`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_pie_slice)) · HLP (`RIP_PieSlice`) · SyncTERM (ripper.c:13695) — not observed in the RIPtel demo corpus

Circular pie slice: arc endpoints joined to the center, interior filled with the current fill pattern. Angles run counterclockwise from 0° at 3 o'clock; the elliptical equivalents actually exercised by the demos are [RIP_OVAL_PIE_SLICE](#rip_oval_pie_slice) and the new [skewed](08-level-0-commands-symbols-a-f.md#rip_skewed_oval_pie_slice) family.

## RIP_OVAL_PIE_SLICE

*Draws an elliptical pie slice*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `i` |
| **Arguments** | `x:XY y:XY st_ang:2 e_ang:2 radx:XY rady:XY` |

**Format:** `!|i <x> <y> <st_ang> <e_ang> <radx> <rady>`
**Example:** `!|iOY8U006O2I1E`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_oval_pie_slice)) · HLP (`RIP_OvalPieSlice`) · corpus (4 uses in SHAPES.RIP) · SyncTERM (ripper.c:14445)

Elliptical pie slice. The SHAPES.RIP example sweeps 0° to 240° (`6O` = 240) with radii `2I`×`1E` at 1280×960 world coordinates.

## RIP_SET_BASE_MATH

*Sets the base math for most RIPscrip parameters*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `J` |
| **Arguments** | `base_math:2` |

**Format:** `!|J <base_math>`
**Example:** `!|J10`

**Attributes used:** Base Math (MegaNums only for its own parameter)
**Evidence:** 2.00a4 (as the *colliding* letter `b` — [2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_set_base_math)) · HLP (`RIP_SetBaseMath`, plus `$BASEMATH$` variable) · corpus (94 uses in 90 files; ONLINE.RIP inline comment "Set base math to MegaNums (base 36)") — not in SyncTERM

Selects the numeric base for subsequent parameters: Base-36 MegaNums (`10` = 36 expressed in base 36) or Base-64 UltraNums (`1S`). The command's own parameter is always read as a MegaNum, since the base may be unknown when it arrives; a [RIP_RESET_WINDOWS](08-level-0-commands-symbols-a-f.md#rip_reset_windows) resets the base to 36, so this command must immediately follow any reset in a non-default-base scene.

**`J` is the confirmed wire opcode.** The 2.00a4 draft assigned SET_BASE_MATH to level-0 `b` — the *same letter* it assigned to [RIP_EXTENDED_TEXT_WINDOW](08-level-0-commands-symbols-a-f.md#rip_extended_text_window), an unresolved collision in the draft text. The RIPtel corpus settles it: 90 of 116 files open with the prologue `!|J10|n2000|M08|fZKQO`, and ONLINE.RIP annotates `J10` inline as "Set base math to MegaNums (base 36)". The shipping 3.0 driver moved SET_BASE_MATH to `J`, leaving `b` to the extended text window.

## RIP_POINT

*Draws one pixel using current drawing color (2.x form)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `j` |
| **Arguments** | `x:XY y:XY` |

**Format:** `!|j <x> <y>`
**Example:** `!|jNK62`

**Attributes used:** Draw Color, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** HLP (`RIP_Point`) · corpus (8 uses in SEABYME1.RIP, SEANITE.RIP) · SyncTERM (ripper.c:14518 — provenance comment "Not in Alpha docs") — not documented in the 2.00a4 spec

A world-coordinate point plot, the 2.x/3.x sibling of the 1.54 [RIP_PIXEL](10-level-0-commands-s-z.md#rip_pixel) (`X`), which both survive side by side. SyncTERM's authors flagged it as absent from the 2.0-Alpha documents; the RIPSCRIP.HLP inventory and the seascape demos confirm it is real (stars/detail dots in SEANITE.RIP). The distinction from `X` is presumed to be resolution-independent world-frame mapping *(hypothesis)*.

## RIP_FILLED_RECTANGLE

*Draw filled rectangle with fill style/line style*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `K` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|K <x0> <y0> <x1> <y1>`
**Example:** `!|K0000ZLQP`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_filled_rectangle)) · HLP (`RIP_FilledRectangle`) · corpus (163 uses in 13 files) · SyncTERM (ripper.c:13812)

Resolution-independent filled rectangle (added v2.A2), bordered when [RIP_SET_BORDER](#rip_set_border) enables borders. The BLUEFADE.FN example fills `(0,0)`–`(1281,961)` — one past the 1280×960 world frame, matching the non-inclusive lower-right convention of resolution-independent fills. This is the corpus's standard full-screen background primitive.

## RIP_BACK_COLOR

*Set background Drawing Color for graphics*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `k` |
| **Arguments** | `color:CM` |

**Format:** `!|k <color>`
**Example:** `!|k00`

**Attributes used:** Back Color, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_back_color)) · HLP (`RIP_BackColor`) · corpus (229 uses in 41 files) — not in SyncTERM

Sets the background pen color used for the "inactive" pixels of fills, some erase operations, and the off pixels of patterned lines (when [RIP_LINE_STYLE](08-level-0-commands-symbols-a-f.md#rip_line_style)'s `off_draw` flag is set). The census confirms the 2.00a4 letter assignment beyond doubt — 229 corpus uses, constantly re-tinted in the fade/shadow function scenes, and TEL3X2.MNU comments narrate it directly ("Background color a darkish brown/gold" before dropshadowed [RIP_EXTENDED_FONT_STYLE](10-level-0-commands-s-z.md#rip_extended_font_style) titles, whose shadows draw in this color).

## RIP_LINE

*Draw a line in the current color/line style*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `L` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|L <x0> <y0> <x1> <y1>`
**Example:** `!|L1C2J1H2K`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_line)) · HLP (`RIP_Line`) · corpus (7,574 uses in 23 files — the most-used drawing command) · SyncTERM (ripper.c:13800)

Draws a line from (x0,y0) to (x1,y1) using the current color, pattern, and thickness. By raw count this is the corpus's dominant primitive: the hand-digitized artwork scenes (SHUTTLE.RIP, SAILBOAT.RIP, HAWK.RIP, SPACSHUT.RIP, FOUND.RIP) are essentially thousands of short line strokes.

## RIP_POLYLINE

*Draw a Poly-Line (multi-faceted line)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `l` |
| **Arguments** | `npoints:2 x1:XY y1:XY ... xn:XY yn:XY` |

**Format:** `!|l <npoints> <x1> <y1> ... <xn> <yn>`
**Example:** `!|l0E0A20462046244B244B9G469G469K0A9K0A9G059G05240A240A200A20`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_polyline)) · HLP (`RIP_Polyline`) · corpus (6 uses in 3 files) · SyncTERM (ripper.c:14529)

Open multi-segment line — [RIP_POLYGON](#rip_polygon) without the closing segment. The 2.00a4 vertex limit of 512 is raised in the 3.0 driver: RIPtel's readme and HLP error strings allow **4096 vertices** for polygon-class objects. The N2_TITLE.RIP example above is a 14-vertex (`0E`) newspaper rule.

## RIP_SET_COLOR_MODE

*Set the Color Drawing Mode (MAP or DIRECT RGB)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `M` |
| **Arguments** | `mode:1 bits:1` |

**Format:** `!|M <mode> <bits>`
**Example:** `!|M08`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_set_color_mode)) · HLP (`RIP_SetColorMode`, plus `$COLORMODE$` variable) · corpus (94 uses in 90 files) — not in SyncTERM

Chooses between Color Mapping mode (palette indices) and Direct RGB mode, with a bit-depth for the RGB encoding. Every corpus prologue sends `!|M08` — mode 0 (color map) with 8 bits — annotated in TELPORT.FN as "Set color palette mapping mode (not RGB encoding)". HLP error strings confirm the 3.0 driver's Direct RGB support is 8 bits per channel only ("RGB color mode only supports 8-bit color currently").

Note on the census: the level-0 `M` observed throughout the corpus is *this* command; the 1.54 [RIP_MOVE](#rip_move) shares the letter pair only in the sense that MOVE is lowercase `m`, which the corpus never uses.

## RIP_MOVE

*Move the current drawing position to (X,Y)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `m` |
| **Arguments** | `x:XY y:XY` |

**Format:** `!|m <x> <y>`
**Example:** `!|m0509`

**Attributes used:** Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_move)) · HLP (`RIP_Move`) · SyncTERM (ripper.c:14568) — not observed in the RIPtel demo corpus

Moves the graphics drawing cursor without drawing. The demos always position text with [RIP_TEXT_XY](08-level-0-commands-symbols-a-f.md#rip_text_xy) instead, so `m` never appears in the corpus — consistent with the 2.00a4 note that it was "primarily provided for future development".

## RIP_SET_BORDER

*Enable or disable borders on filled objects*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `N` |
| **Arguments** | `borders:2` |

**Format:** `!|N <borders>`
**Example:** `!|N01`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_set_border)) · HLP (`RIP_SetBorder`) · corpus (122 uses in 23 files) · SyncTERM (ripper.c:13842)

Turns borders around filled objects on (`01`) or off (`00`); borders always draw in COPY mode with the current line thickness. The corpus extends the documented list of affected commands: NEWCMDS.RIP and POLYPOLY.RIP toggle `N` around the new [filled skewed ovals](08-level-0-commands-symbols-a-f.md#rip_filled_skewed_oval), [chords](08-level-0-commands-symbols-a-f.md#rip_filled_oval_chord), and [poly-polygons](08-level-0-commands-symbols-a-f.md#rip_poly_polygon) to demonstrate "With a border / Without a border" — all the 3.0 primitives obey it.

## RIP_SET_COORDINATE_SIZE

*Sets the number of bytes used for XY coordinates*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `n` |
| **Arguments** | `byte_size:1 res:3` |

**Format:** `!|n <byte_size> <res>`
**Example:** `!|n2000`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_set_coordinate_size)) · HLP (`RIP_SetCoordinateSize`, plus `$COORDSIZE$` variable) · corpus (94 uses in 90 files) — not in SyncTERM

Sets how many digits every subsequent `XY` coordinate occupies (valid 2–5; default 2). The standard corpus prologue sends `!|n2000` — byte size 2, reserved `000` — annotated in TELPORT.FN as "Set X/Y coordinate size to 2 bytes". No corpus scene ever uses a size other than 2; a 2-digit MegaNum (0–1295) comfortably addresses the 1280×960 world frame.

## RIP_OVAL

*Draw elliptical arc in current color/line style*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `O` |
| **Arguments** | `x:XY y:XY st_ang:2 end_ang:2 x_rad:XY y_rad:XY` |

**Format:** `!|O <x> <y> <st_ang> <end_ang> <x_rad> <y_rad>`
**Example:** `!|O1NLT00A00505`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_oval)) · corpus (6 uses in 4 files) · SyncTERM (ripper.c:13850) — not in the RIPSCRIP.HLP name inventory (only `RIP_OvalArc`)

Elliptical arc, historically identical to [RIP_OVAL_ARC](10-level-0-commands-s-z.md#rip_oval_arc) (`V`) — SyncTERM literally implements `V` as a fall-through to `O`, and RIPSCRIP.HLP carries only the `RIP_OvalArc` name, confirming the 3.0 driver treats them as one command. The SEABYME1.RIP example draws a full ellipse (`00` to `A0` = 360°) of radius 5×5.

## RIP_FILLED_OVAL

*Draw filled ellipse using current color/pattern*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `o` |
| **Arguments** | `x_center:XY y_center:XY x_rad:XY y_rad:XY` |

**Format:** `!|o <x_center> <y_center> <x_rad> <y_rad>`
**Example:** `!|oCH5V0201`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_filled_oval)) · HLP (`RIP_FilledOval`) · corpus (153 uses in 9 files) · SyncTERM (ripper.c:14589)

Filled ellipse; interior in the current fill pattern/color, outline per border setting. Heavily used for organic shapes in the illustrated scenes (TUNNEL.RIP's rings, N2_BUSI.RIP charts).

## RIP_POLYGON

*Draw polygon in current color/line-style*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `P` |
| **Arguments** | `npoints:2 x1:XY y1:XY ... xn:XY yn:XY` |

**Format:** `!|P <npoints> <x1> <y1> ... <xn> <yn>`
**Example:** `!|P0CLQ72PM72PM9AOI86OIC2PMAYPMD6LQD6LQAYMUC2MU86LQ9A`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_polygon)) · HLP (`RIP_Polygon`) · corpus (4 uses in 2 files) · SyncTERM (ripper.c:13871)

Closed multi-sided polygon outline; the closing segment is drawn automatically. The 1.54/2.x vertex limit of 512 is raised to **4096** in the 3.0 driver (readme + HLP error strings: "Vertex count doesn't match parameters"). The POLYGONS.RIP example above is a 12-vertex (`0C`) star.

## RIP_FILLED_POLYGON

*Draw filled polygon in current color/fill pattern*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `p` |
| **Arguments** | `npoints:2 x1:XY y1:XY ... xn:XY yn:XY` |

**Format:** `!|p <npoints> <x1> <y1> ... <xn> <yn>`
**Example:** `!|p08006K1F631V5H2F4Y354D2Y46174I004W`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_filled_polygon)) · HLP (`RIP_FilledPolygon`) · corpus (228 uses in 13 files) · SyncTERM (ripper.c:14596, as RIP_FILL_POLYGON)

Filled counterpart of [RIP_POLYGON](#rip_polygon) (formerly named RIP_FILL_POLYGON — SyncTERM retains the old name). Overlapping regions "inside" an even number of times are not filled; the 3.0-era multi-contour generalization of that rule is the new [RIP_POLY_POLYGON](08-level-0-commands-symbols-a-f.md#rip_poly_polygon). Vertex limit 4096 in the 3.0 driver. Landscape and eagle/bird artwork scenes use it as their principal solid-shape primitive.

## RIP_SET_PALETTE

*Set 16-color Desktop Palette from 64-clr palette*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `Q` |
| **Arguments** | `c1:2 c2:2 ... c16:2` |

**Format:** `!|Q <c1> <c2> ... <c16>`
**Example:** `!|Q000102030405060708090A0B0C0D0E0F`

**Attributes used:** Draw Color, Back Color, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_set_palette)) · HLP (`RIP_SetPalette`) · SyncTERM (ripper.c:13921) — not observed in the RIPtel demo corpus

Replaces all 16 Desktop Palette entries from the 64-color master palette in one command. The 256-color-era demos manipulate the Drawing Palette ([`D`](08-level-0-commands-symbols-a-f.md#rip_set_drawing_palette)/[`d`](08-level-0-commands-symbols-a-f.md#rip_one_drawing_palette)) instead, leaving this 16-color command to legacy scenes.

## RIP_RECTANGLE

*Draw a rectangle in current color/line style*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `R` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|R <x0> <y0> <x1> <y1>`
**Example:** `!|R2020A030`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_rectangle)) · HLP (`RIP_Rectangle`) · corpus (108 uses in 12 files) · SyncTERM (ripper.c:13948)

Unfilled rectangle between two opposing corners. The example is BOUNDS.RIP's "Show our bounding box" — drawn at the exact coordinates then reused by the new [RIP_BOUNDED_TEXT](08-level-0-commands-symbols-a-f.md#rip_bounded_text) command, which is how that command's argument layout was decoded.

---

[◀ Prev: Level-0 Commands (Symbols & A–F)](08-level-0-commands-symbols-a-f.md) · [Contents](README.md) · [Next: Level-0 Commands (S–Z) ▶](10-level-0-commands-s-z.md)
