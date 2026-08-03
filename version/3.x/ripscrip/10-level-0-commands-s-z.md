# Level-0 Commands (S–Z)

[◀ Prev: Level-0 Commands (G–R)](09-level-0-commands-g-r.md) · [Contents](README.md) · [Next: Level-1 Commands ▶](11-level-1-commands.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

This page covers the level-0 commands with opcodes `S` through `z`, including the 3.0 outline-font system's key command, [`y` RIP_EXTENDED_FONT_STYLE](#rip_extended_font_style) — descriptor-only in SyncTERM, but the fourth most-used command in the RIPtel corpus, with TeleGrafix's own field-layout crib preserved in FONTS.RIP.

Commands: [`S` RIP_FILL_STYLE](#rip_fill_style), [`s` RIP_FILL_PATTERN](#rip_fill_pattern), [`T` RIP_TEXT](#rip_text), [`t` RIP_POLY_BEZIER_LINE](#rip_poly_bezier_line), [`U` RIP_ROUNDED_RECT](#rip_rounded_rect), [`u` RIP_FILLED_ROUNDED_RECT](#rip_filled_rounded_rect), [`V` RIP_OVAL_ARC](#rip_oval_arc), [`v` RIP_VIEWPORT](#rip_viewport), [`W` RIP_WRITE_MODE](#rip_write_mode), [`w` RIP_TEXT_WINDOW](#rip_text_window), [`X` RIP_PIXEL](#rip_pixel), [`x` RIP_FILLED_POLY_BEZIER](#rip_filled_poly_bezier), [`Y` RIP_FONT_STYLE](#rip_font_style), [`y` RIP_EXTENDED_FONT_STYLE](#rip_extended_font_style), [`Z` RIP_BEZIER](#rip_bezier), [`z` RIP_POLY_BEZIER](#rip_poly_bezier).

## RIP_FILL_STYLE

*Set current fill style (predefined) & fill color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `S` |
| **Arguments** | `pattern:2 color:CM` |

**Format:** `!|S <pattern> <color>`
**Example:** `!|S010W`

**Attributes used:** Fill Style, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_fill_style)) · HLP (`RIP_FillStyle`) · corpus (777 uses in 36 files) · SyncTERM (ripper.c:13967)

Selects one of the twelve predefined fill patterns and the fill color for subsequent fill operations (see the 2.x entry for the pattern bitmaps). The corpus's most common form is `!|S01cc` — solid fill in a Drawing Palette color (`0W` = entry 32 and up, the custom fade colors) — cycled per-shape in MARKER2.RIP and per-band in the fade backgrounds. It is the second most-used state command in the corpus after [RIP_COLOR](08-level-0-commands-symbols-a-f.md#rip_color).

## RIP_FILL_PATTERN

*Set user-definable (custom) fill pattern/color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `s` |
| **Arguments** | `c1:2 c2:2 c3:2 c4:2 c5:2 c6:2 c7:2 c8:2 col:CM` |

**Format:** `!|s <c1> <c2> <c3> <c4> <c5> <c6> <c7> <c8> <col>`
**Example:** `!|s4Q2D4Q2D4Q2D4Q2D01`

**Attributes used:** Fill Style, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_fill_pattern)) · HLP (`RIP_FillPattern`, error "Pattern value exceeds 255") · corpus (69 uses in 5 files) · SyncTERM (ripper.c:14670)

Defines a custom 8×8 fill pattern as eight row bitmasks plus a fill color, overriding [RIP_FILL_STYLE](#rip_fill_style). The example (BLUEFADE.FN) defines an alternating dither pattern (`4Q2D` = 170/85, i.e. `10101010`/`01010101`) used to blend adjacent fade bands — the workhorse of the corpus's smooth 256-color gradients.

## RIP_TEXT

*Draw text in current font at current XY location*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `T` |
| **Arguments** | `text-string` |

**Format:** `!|T <text-string>`
**Example:** `!|Thello world`

**Attributes used:** Draw Color, Write Mode, Font Style, Viewport, Port
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_text)) · HLP (`RIP_Text`, plus `$TEXTXY$` variable) · SyncTERM (ripper.c:14026) — not observed in the RIPtel demo corpus

Draws text at the current graphics cursor position (set by [RIP_MOVE](09-level-0-commands-g-r.md#rip_move) or left after previous horizontal text). The corpus always uses the combined [RIP_TEXT_XY](08-level-0-commands-symbols-a-f.md#rip_text_xy) form instead, but the mechanism survives — the `$TEXTXY$` variable in RIPtel's help reads back the "last graphical text X/Y" this command family maintains.

## RIP_POLY_BEZIER_LINE

*Draw a poly-bezier curve (open-ended)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `t` |
| **Arguments** | `num:2 count:2 x_base:XY y_base:XY ... type:1 <block> ... ` (typed segment blocks — see [RIP_POLY_BEZIER](#rip_poly_bezier)) |

**Format:** `!|t <num> <count> <x_base> <y_base> [<type> <block>]...`
**Example:** `!|t03142JA62HA62JEA66HUA6JGA6HCEE0JCEG`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_poly_bezier_line)) · corpus (12 uses in ONLINE.RIP, CURVES.RIP, SHAPES.RIP) · SyncTERM (ripper.c:14721 — shared bezier parser, open/unfilled flags) — poly-bezier commands are absent from the RIPSCRIP.HLP name inventory, though its error strings validate them ("The last segment of a poly-bezier cannot be a curve")

Open-ended variant of [RIP_POLY_BEZIER](#rip_poly_bezier): the chain's endpoints are not joined, just as [RIP_POLYLINE](09-level-0-commands-g-r.md#rip_polyline) relates to [RIP_POLYGON](09-level-0-commands-g-r.md#rip_polygon). The corpus example (3 segments, 14 lines per curve) draws the flowing curve accents in the CURVES demo.

## RIP_ROUNDED_RECT

*Draw a rectangle with rounded corners*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `U` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY rad:2` |

**Format:** `!|U <x0> <y0> <x1> <y1> <rad>`
**Example:** `!|U8K3KDK6C0K`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_rounded_rect)) · HLP (`RIP_RoundedRect`) · corpus (2 uses in SHAPES.RIP)

Rectangle with truly circular corner arcs of radius `<rad>` (added v2.A3). SHAPES.RIP demonstrates it (radius `0K` = 20) alongside its filled sibling.

## RIP_FILLED_ROUNDED_RECT

*Draw a filled rectangle with rounded corners*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `u` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY rad:2` |

**Format:** `!|u <x0> <y0> <x1> <y1> <rad>`
**Example:** `!|u8K7GDKA80K`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_filled_rounded_rect)) · HLP (`RIP_FilledRoundedRect`) · corpus (4 uses in SHAPES.RIP)

Filled rounded rectangle; interior in the current fill pattern/color, border (when enabled) always in COPY mode.

## RIP_OVAL_ARC

*Draw an elliptical arc*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `V` |
| **Arguments** | `x:XY y:XY st_ang:2 e_ang:2 radx:XY rady:XY` |

**Format:** `!|V <x> <y> <st_ang> <e_ang> <radx> <rady>`
**Example:** `!|VOY4Y006O2I1E`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_oval_arc)) · HLP (`RIP_OvalArc`) · corpus (2 uses in SHAPES.RIP) · SyncTERM (ripper.c:13848 — falls through to `O`)

Elliptical arc from `<st_ang>` counterclockwise to `<e_ang>`. Identical to [RIP_OVAL](09-level-0-commands-g-r.md#rip_oval) for historical reasons — SyncTERM implements `V` as a literal fall-through to `O`'s handler, and RIPSCRIP.HLP keeps only this name for the pair. The SHAPES.RIP example sweeps 0°–240°.

## RIP_VIEWPORT

*Define the size & location of the Graphics Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `v` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|v <x0> <y0> <x1> <y1>`
**Example:** `!|v1EB5GUIC`

**Attributes used:** Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_viewport)) · HLP (`RIP_Viewport`, plus `$AVP$`/`$DVP$`/`$MVP$` variables) · corpus (3 uses in IMAGES.RIP) · SyncTERM (ripper.c:14725)

Sets the clipping viewport of the current drawing port, in world coordinates. IMAGES.RIP uses it to clip bitmap stamping regions, then restores full frame with `!|v0000ZKQO`. Setting all coordinates to zero deactivates the viewport; the `$AVP$` variable (or a new viewport command) reactivates it.

## RIP_WRITE_MODE

*Set drawing mode for graphics primitives*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `W` |
| **Arguments** | `mode:2` |

**Format:** `!|W <mode>`
**Example:** `!|W00`

**Attributes used:** Write Mode, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_write_mode)) · HLP (`RIP_WriteMode`) · corpus (44 uses in 35 files) · SyncTERM (ripper.c:14062)

Selects the raster operation for level-0 primitives: COPY (00), XOR (01), OR (02), AND (03), NOT (04). The corpus uses COPY almost exclusively, with a few `03` (AND) uses for masking effects — and one file contains a stray argument of `.`, which the shipping driver's tolerant MegaNum parser accepts (a `.` terminates numeric parsing without error).

## RIP_TEXT_WINDOW

*Define the size and location of the Text Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `w` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY wrap:1 size:1` |

**Format:** `!|w <x0> <y0> <x1> <y1> <wrap> <size>`
**Example:** `!|w0000000000`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_text_window)) · HLP (`RIP_TextWindow`; the five MicroANSI text modes 80x43/91x43/80x25/91x25/40x25 appear in MESSAGES.HLP) · corpus (14 uses in 14 files) · SyncTERM (ripper.c:14783)

The 1.54 cell-addressed TTY text window (five fixed font sizes; see the 2.x entry for placement math). All corpus uses set every parameter to zero — the demos use it purely to **deactivate** the text window before full-screen graphics, or define minimal windows for status text. The resolution-independent replacement is [RIP_EXTENDED_TEXT_WINDOW](08-level-0-commands-symbols-a-f.md#rip_extended_text_window); the 3.0 driver's MicroANSI font file (`RIPscrip.maf`) supplies the bitmap fonts this command selects at 640×480 through 1024×768.

## RIP_PIXEL

*Draws one pixel using current drawing color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `X` |
| **Arguments** | `x:XY y:XY` |

**Format:** `!|X <x> <y>`
**Example:** `!|X0032`

**Attributes used:** Draw Color, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_pixel)) · HLP (`RIP_Pixel`) · corpus (7 uses in LANDSCPE.RIP, EAGLE.RIP) · SyncTERM (ripper.c:14084)

Single-pixel plot, kept for completeness; the 2.x-era sibling [RIP_POINT](09-level-0-commands-g-r.md#rip_point) (`j`) coexists with it in both the HLP inventory and the corpus.

## RIP_FILLED_POLY_BEZIER

*Draw a filled poly-bezier curve (multi-segmented)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `x` |
| **Arguments** | `num:2 count:2 x_base:XY y_base:XY ... type:1 <block> ...` (typed segment blocks — see [RIP_POLY_BEZIER](#rip_poly_bezier)) |

**Format:** `!|x <num> <count> <x_base> <y_base> [<type> <block>]...`
**Example:** `!|x061451AA8026T2Q4E56Y4Q9D4X7S6C5E85QFX5KI6925EGAUCFBT9OB8088D …`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_filled_poly_bezier)) · corpus (182 uses in 6 files) · SyncTERM (ripper.c:14866 — shared bezier parser, filled/closed flags) — absent from the RIPSCRIP.HLP name inventory, though its poly-bezier error strings apply

Closed, filled poly-bezier — [RIP_POLY_BEZIER](#rip_poly_bezier) with the interior filled in the current fill pattern/color (even-odd overlap rule, as with filled polygons). This is the corpus's premier organic-shape tool: the SEANITE.RIP/SEABYME1.RIP seascapes build waves, hills, and clouds from 182 filled poly-beziers (the example above, elided, is a 6-segment, 14-step wave form).

## RIP_FONT_STYLE

*Select current vector/bitmap font style*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `Y` |
| **Arguments** | `font:2 direction:2 size:2 flags:2` |

**Format:** `!|Y <font> <direction> <size> <flags>`
**Example:** `!|Y00000100`

**Attributes used:** Font Style, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_font_style)) · HLP (`RIP_FontStyle`; the BGI `.CHR` stroked fonts ship in RIPtel's FONTS directory) · corpus (23 uses in 12 files) · SyncTERM (ripper.c:14100, descriptor `2#2#2#1#1#`)

Selects among the classic fonts — the 8×8 bitmap font 00 and ten Borland BGI stroked vector fonts (01–0A) — with direction, magnification, and the v2.A4 justification/dropshadow flags (see the 2.x entry for the full tables). RIPtel 3.1 still ships the ten `.CHR` BGI fonts (TRIP, LITT, SANS, GOTH, SCRI, SIMP, TSCR, LCOM, EURO, BOLD), but the demos mostly reserve `Y` for utilitarian labels, preferring the outline-font system of [RIP_EXTENDED_FONT_STYLE](#rip_extended_font_style) below.

## RIP_EXTENDED_FONT_STYLE

*Select current outline font style (True Type style)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `y` |
| **Arguments** | 2.00a4: `direction:3 size:2 style:2 h_align:1 v_align:1 reserved:4 font_name_string` — **3.0 wire layout differs**; see the crib below |

**Format:** `!|y <s> <f> <flags> <size> <orientation> <str_rot> <char_rot?> <spacing?> <char_rot?> <shadow?> <reserved> <font_name>` *(3.0 layout; field roles beyond the crib are hypothesis)*
**Example:** `!|y0000BW1Q080000001a1a000000Marin`

**Attributes used:** Font Style, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_extended_font_style)) · HLP (`RIP_ExtendedFontStyle`; font error strings cover string & char rotation, char spacing, shadowing) · corpus (430 uses in 42 files — the corpus's fourth most-used command) · SyncTERM (ripper.c:9156 — descriptor-only `2#2#2#2#`, no handler)

The outline-font command: scalable "Adobe and TrueType style" fonts with bold/italic/strikeout/underline facings, arbitrary alignment, and — new in 3.0 — full rotation. SyncTERM knows the opcode only as a four-field descriptor with no implementation; the RIPtel corpus shows the real 3.0 argument block is far richer — **26 fixed characters followed by the font name**. TeleGrafix's own field-layout crib survives as a comment in FONTS.RIP (line 114):

```text
!|!sfFFFFZZOOSSCCBBCCWWRRRRRR
```

decoding as `s`(1) `f`(1), `FFFF` 4-digit flags, `ZZ` size, `OO` orientation, then `SS CC BB CC WW` (2 digits each), and `RRRRRR` reserved. FONTS.RIP's rotation demo varies the `SS` field (`00` → `E4`, `gC`, …) under per-line labels "0 x 0", "180 x 90", "270 x 180" and so on — covering all **16 combinations of string rotation × character rotation** advertised for the 3.0 font system. The common corpus idiom `…1a1a…` in the paired 2-digit fields reads as an x/y scale pair *(hypothesis)*, and TELLISTS.MNU comments narrate style flags directly ("Marin, centered, bold w/ dropshadow"). The observed layout does **not** match the 2.00a4 draft's 13-character argument list — this command was substantially reworked between the Alpha 4 draft and the shipping 3.0 driver.

Font names bind to the **`.RFF` "RIPscrip FastFont"** outline fonts (relabeled Atech FastFonts) that ship with RIPtel: families BRUSH, COBB, DEFAULT, DIXON, EUREKA, MARIN, OAKLAND, SYMBOL, each in 10 style variants (base, ` TH` thin, ` CN` condensed, ` WD` wide, ` EX` expanded, ` HO` hollow, and hollow combinations ` HT`/` HC`/` HW`/` HE`). The demos use Marin, Dixon, Cobb, and Symbol. The font-name argument may itself be a text variable: `!|y…000000$&FONT_NAME$` (SHOWFONT.FN). The five 2.00a4 predefined names (COURIER, HELV, TIMESROM, OLDENGL, SANSSERF) never appear in the corpus.

## RIP_BEZIER

*Draw a bezier curve*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `Z` |
| **Arguments** | `x1:XY y1:XY x2:XY y2:XY x3:XY y3:XY x4:XY y4:XY cnt:2` |

**Format:** `!|Z <x1> <y1> <x2> <y2> <x3> <y3> <x4> <y4> <cnt>`
**Example:** `!|ZCSIYDBIYDVIZEEJ014`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_bezier)) · HLP (`RIP_Bezier`) · corpus (22 uses in 4 files) · SyncTERM (ripper.c:14140)

Single cubic bezier: four control points plus a segment count (`14` = 40 segments in the LANDSCPE.RIP example). See the 2.x entry for the parametric equations and control-point geometry.

## RIP_POLY_BEZIER

*Draw a poly-bezier curve (multi-segmented, closed)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `z` |
| **Arguments** | `num:2 count:2 x_base:XY y_base:XY` then per segment: `type:1` + line endpoint (type 0) or two control points + endpoint (types 1/2) |

**Format:** `!|z <num> <count> <x_base> <y_base> [<type> <block>]...`
**Example:** `!|z081428KGC8KESASF85CGF8E4F8BCGC5DKGCEOGCE4GW5DKHGD0I0BWI06ASI …`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_poly_bezier)) · corpus (2 uses in SHAPES.RIP) · SyncTERM (ripper.c:14870 — shared parser, closed/unfilled flags) — absent from the RIPSCRIP.HLP name inventory, though its poly-bezier error strings apply

Chains straight-line segments (type 0) and smooth/jointed bezier segments (types 1/2) into one closed curve; see the 2.x entry for the block syntax, worked example, and smooth-joint math. HLP adds one 3.0 validation rule the 2.00a4 draft lacks: "The last segment of a poly-bezier cannot be a curve". SyncTERM funnels `z`, [`t`](#rip_poly_bezier_line), and [`x`](#rip_filled_poly_bezier) into a single shared implementation differing only in close/fill flags.

---

**HLP names without a known level-0 opcode.** Two drawing/state names in the RIPSCRIP.HLP inventory match no letter attested by any spec, corpus scene, or implementation: `RIP_TextMetric` (text-metric mode appears in HLP's text-window error strings) and `RIP_FontAttrib` ("Can't change system font attributes"). Their wire opcodes — if they ever had any — remain unknown. SyncTERM's descriptor table also reserves a **level-0 `<ESC>`** command (ripper.c:9206, manual parse, no handler), a counterpart to `1<ESC>`/`9<ESC>` that is likewise unobserved in the corpus.

---

[◀ Prev: Level-0 Commands (G–R)](09-level-0-commands-g-r.md) · [Contents](README.md) · [Next: Level-1 Commands ▶](11-level-1-commands.md)
