# Level-0 Commands (Symbols & A–F)

[◀ Prev: Protocol Definition & Syntax](07-protocol-definition.md) · [Contents](README.md) · [Next: Level-0 Commands (G–R) ▶](09-level-0-commands-g-r.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

Level-0 commands are the graphical building blocks of RIPscrip: the simple primitives for drawing lines, circles, and graphical text, plus the commands necessary to the basic setup and operation of the language (base math, color modes, world coordinates). This page covers the commands whose opcode is a symbol character, followed by the letter opcodes `A` through `f`. The 3.0 era added an entire family of symbol-opcode drawing primitives — the six skewed-oval commands named in TeleGrafix's own NEWCMDS.RIP demo comments — plus bounded text, markers, and poly-polygons, none of which appear in any published specification.

Symbol commands: [`!` RIP_COMMENT](#rip_comment), [`#` RIP_NO_MORE](#rip_no_more), [`$` text-variable trigger](#-text-variable-trigger), [`*` RIP_RESET_WINDOWS](#rip_reset_windows), [`(` RIP_GROUP_BEGIN](#rip_group_begin), [`)` RIP_GROUP_END](#rip_group_end), [`=` RIP_LINE_STYLE](#rip_line_style), [`>` RIP_ERASE_EOL](#rip_erase_eol), [`@` RIP_TEXT_XY](#rip_text_xy), [`&` RIP_SKEWED_OVAL](#rip_skewed_oval), [`-` RIP_FILLED_SKEWED_OVAL](#rip_filled_skewed_oval), [`]` RIP_SKEWED_OVAL_ARC](#rip_skewed_oval_arc), [`[` RIP_SKEWED_OVAL_PIE_SLICE](#rip_skewed_oval_pie_slice), [`+` RIP_SKEWED_OVAL_CHORD](#rip_skewed_oval_chord), [`_` RIP_FILLED_OVAL_CHORD](#rip_filled_oval_chord), [`"` RIP_BOUNDED_TEXT](#rip_bounded_text), [`;` RIP_MARKER](#rip_marker), [`<` RIP_POLY_POLYGON](#rip_poly_polygon). Letter commands: [`A` RIP_ARC](#rip_arc), [`a` RIP_ONE_PALETTE](#rip_one_palette), [`B` RIP_BAR](#rip_bar), [`b` RIP_EXTENDED_TEXT_WINDOW](#rip_extended_text_window), [`C` RIP_CIRCLE](#rip_circle), [`c` RIP_COLOR](#rip_color), [`D` RIP_SET_DRAWING_PALETTE](#rip_set_drawing_palette), [`d` RIP_ONE_DRAWING_PALETTE](#rip_one_drawing_palette), [`E` RIP_ERASE_VIEW](#rip_erase_view), [`e` RIP_ERASE_WINDOW](#rip_erase_window), [`F` RIP_FILL](#rip_fill), [`f` RIP_SET_WORLD_FRAME](#rip_set_world_frame).

## RIP_COMMENT

*Put in a comment as part of a RIPscrip sequence*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `!` |
| **Arguments** | `string...` |

**Format:** `!|!This is a comment`
**Example:** `!|!Show RIP_SKEWED_OVAL`

**Attributes used:** Base Math (N/A)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_comment)) · corpus (1,683 comment commands across the RIPtel demos, 305 with prose)

Everything from the `!` opcode to the next `|` or end of line is ignored. Comments may be line-continued with backslashes and may follow other commands on the same line (`!|c0F|!set white`). The comment string is explicitly exempt from text-variable substitution.

In the RIPtel corpus, comments are the single most valuable evidence stream: `!|command|! comment` chains are ubiquitous, and TeleGrafix's authors used comments to name six otherwise-undocumented drawing primitives outright (NEWCMDS.RIP) and to leave positional field-layout cribs above complex commands (`!|! xxyyxxyycaffffccrrrrrrrr` for [`1e`](11-level-1-commands.md#rip_extended_begin_text), `!|!sfFFFFZZOOSSCCBBCCWWRRRRRR` for [`y`](10-level-0-commands-s-z.md#rip_extended_font_style)).

## RIP_NO_MORE

*End of RIPscrip Scene*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `#` |
| **Arguments** | `<none>` |

**Format:** `!|#`
**Example:** `!|#`

**Attributes used:** none
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_no_more)) · corpus (287 uses in 95 files) · SyncTERM (ripper.c:13315)

Marks the end of a RIPscrip scene, allowing the terminal to activate mouse regions and respond to queued clicks. It also re-enables user input previously disabled by a [RIP_HEADER](09-level-0-commands-g-r.md#rip_header) command. Hosts should send several `!|#` commands consecutively for noise immunity; terminals should also time out and assume NO_MORE if the stream goes quiet. Nearly every RIPtel demo scene ends with one (287 uses in 95 of 116 files).

## $ (Text-Variable Trigger)

*Evaluate a text variable in command position* *(hypothesis — undocumented)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `$` |
| **Arguments** | `NAME$` (the remainder of a `$…$` text-variable reference) |

**Format:** `!|$<variable-name>$`
**Example:** `!|$COFF$`

**Attributes used:** Base Math (N/A)
**Evidence:** SyncTERM (ripper.c:13336 — source comment: "Undocument RIP_QUERY thing") — not in the 2.00a4 spec, not in the RIPSCRIP.HLP inventory, not observed in the RIPtel demo corpus

A `$` immediately after the command header causes the text-variable machinery to evaluate `$NAME$` purely for its side effect — a command-position shortcut for what [RIP_QUERY](11-level-1-commands.md#rip_query) does with a full `!|1<ESC>` header. Only SyncTERM attests this form; both the name and exact semantics are *(hypothesis)*.

## RIP_RESET_WINDOWS

*Clear Graphic/Text Windows & reset to full screen*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `*` |
| **Arguments** | `<none>` |

**Format:** `!|*`
**Example:** `!|*`

**Attributes used:** Port
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_reset_windows)) · HLP (`RIP_ResetWindows`) · corpus (31 uses in 31 files) · SyncTERM (ripper.c:13348)

Performs a full environment reset: world frame back to 640×350, text window full screen, default palette, screen cleared, mouse fields and buttons deleted, ports #1–35 deleted, base math back to MegaNums, coordinate size back to 2. See the 2.x entry for the complete reset checklist. In the RIPtel demos it opens standalone scenes, immediately followed by the standard prologue `J10|n2000|M08|fZKQO` that re-establishes the 3.0-era 1280×960 environment (see [RIP_SET_BASE_MATH](09-level-0-commands-g-r.md#rip_set_base_math)).

## RIP_GROUP_BEGIN

*Start a grouping of RIPscrip commands*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `(` |
| **Arguments** | `<none>` |

**Format:** `!|(`
**Example:** `!|(`

**Attributes used:** none
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_group_begin)) — not in the RIPSCRIP.HLP inventory, not observed in the RIPtel demo corpus, not in SyncTERM

Groups blocks of commands into logical, nestable bundles for the benefit of paint programs; terminals ignore it. Any open groups are closed by a [RIP_NO_MORE](#rip_no_more). The 2.00a4 spec suggests stripping groupings from files placed online, which is consistent with their total absence from the shipping demo corpus.

## RIP_GROUP_END

*End a grouping of RIPscrip commands*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `)` |
| **Arguments** | `<none>` |

**Format:** `!|)`
**Example:** `!|)`

**Attributes used:** none
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_group_end)) — not in the RIPSCRIP.HLP inventory, not observed in the RIPtel demo corpus, not in SyncTERM

Ends a group started with [RIP_GROUP_BEGIN](#rip_group_begin).

## RIP_LINE_STYLE

*Defines a line's pattern and thickness*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `=` |
| **Arguments** | `off_draw:1 style:1 user_pat:4 thick:2` |

**Format:** `!|= <off_draw> <style> <user_pat> <thick>`
**Example:** `!|=001EKF03`

**Attributes used:** Line Style, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_line_style)) · HLP (`RIP_LineStyle`) · corpus (120 uses in 31 files) · SyncTERM (ripper.c:13369, descriptor `2#4#2#`)

Selects one of four built-in line styles (solid, dotted, centered, dashed) or a custom 16-bit pattern, an arbitrary pixel thickness, and — new in 2.0 — whether the "off" pixels of a patterned line are transparent or drawn in the background color. The example above, from ONLINE.RIP, sets a custom pattern `1EKF` at thickness 3. NEWCMDS.RIP prologues use it as documented ("Solid line 3 pixel wide": `!|=00000003`). HLP error strings add that a custom line pattern "can't have blank pattern".

## RIP_ERASE_EOL

*Erase current line from cursor to end of line*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `>` |
| **Arguments** | `<none>` |

**Format:** `!|>`
**Example:** `!|>`

**Attributes used:** Back Color, Base Math (N/A)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_erase_eol)) · HLP (`RIP_EraseEOL`) · SyncTERM (ripper.c:13426) — not observed in the RIPtel demo corpus

Erases the current text line from the cursor to end of line, filling with the current graphics background color (unlike ANSI `ESC[K`, which uses the ANSI background color). The corpus never uses it, but a `$TWERASEEOL$` command-variable equivalent exists in the RIPtel help.

## RIP_TEXT_XY

*Draw text in current font/color at specific spot*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `@` |
| **Arguments** | `x:XY y:XY` and text-string |

**Format:** `!|@ <x> <y> <text-string>`
**Example:** `!|@HS2ARIPscrip Buttons`

**Attributes used:** Draw Color, Write Mode, Font Style, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_text_xy)) · HLP (`RIP_TextXY`) · corpus (638 uses in 40 files) · SyncTERM (ripper.c:13445, descriptor `2#2#++`)

Combination of [RIP_MOVE](09-level-0-commands-g-r.md#rip_move) and [RIP_TEXT](10-level-0-commands-s-z.md#rip_text); the workhorse text command of the corpus (638 uses, versus zero bare `T` commands). In 3.0 scenes it renders in whichever font was last selected — very often an [RIP_EXTENDED_FONT_STYLE](10-level-0-commands-s-z.md#rip_extended_font_style) outline font — and the text argument may contain `<<NAME>>` macros (`!|@809U<<LAB1>>` labels buttons from `.DEF`-file variables) or `$&VAR$` dereferences. NEWS.RIP draws a drop-cap this way (`@A91G` + one large character) with the flowed article text starting mid-word after it.

## RIP_SKEWED_OVAL

*Draw an ellipse rotated to an arbitrary angle*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `&` |
| **Arguments** | `x:2 y:2 x_rad:2 y_rad:2 rotation:2` *(field meanings from census analysis)* |

**Format:** `!|& <x> <y> <x_rad> <y_rad> <rotation>`
**Example:** `!|&W44W281810`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (NEWCMDS.RIP — named in TeleGrafix's own comment "Show RIP_SKEWED_OVAL" — and SHAPES.RIP; 3 uses) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP name inventory, not in SyncTERM

The first of six 3.0-era "skewed oval" primitives that exist in no published specification. It draws an ellipse outline whose axes are rotated by the fifth parameter — something the 1.54/2.x [RIP_OVAL](09-level-0-commands-g-r.md#rip_oval) family cannot do. The canonical name comes verbatim from NEWCMDS.RIP, whose comments demonstrate each new primitive over a coordinate grid. All five fields are 2-digit MegaNums; the rotation unit is presumed degrees *(hypothesis)*.

## RIP_FILLED_SKEWED_OVAL

*Draw a filled ellipse rotated to an arbitrary angle*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `-` |
| **Arguments** | `x:2 y:2 x_rad:2 y_rad:2 rotation:2` *(field meanings from census analysis)* |

**Format:** `!|- <x> <y> <x_rad> <y_rad> <rotation>`
**Example:** `!|-W48S281810`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (NEWCMDS.RIP — named in comment "Show RIP_FILLED_SKEWED_OVAL" — and SHAPES.RIP; 6 genuine uses) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP name inventory, not in SyncTERM

Filled counterpart of [RIP_SKEWED_OVAL](#rip_skewed_oval), taking the same five fields. NEWCMDS.RIP demonstrates it both "With a border" and "Without a border", proving it honors [RIP_SET_BORDER](09-level-0-commands-g-r.md#rip_set_border) like the documented filled primitives.

A corpus curiosity: six additional apparent uses of `-` in the `.DEF`/`.MNU` files are authoring typos — `!|-----` separator comment lines missing their `!` — which parse as this opcode with dash arguments (`-` is a valid MegaNum digit meaning 0). The shipping RIPtel driver tolerates them without visible failure.

## RIP_SKEWED_OVAL_ARC

*Draw an arc of a rotated ellipse*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `]` |
| **Arguments** | `x:2 y:2 x_rad:2 y_rad:2 start_ang:2 end_ang:2 rotation:2` *(field meanings from census analysis)* |

**Format:** `!|] <x> <y> <x_rad> <y_rad> <start_ang> <end_ang> <rotation>`
**Example:** `!|]50151G0M20601M`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (NEWCMDS.RIP — named in comment "Show a RIP_SKEWED_OVAL_ARC" — and SHAPES.RIP; 3 uses) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP name inventory, not in SyncTERM

Arc segment of a rotated ellipse: seven 2-digit fields extending the [RIP_SKEWED_OVAL](#rip_skewed_oval) layout with a start/end angle pair, in the position shown above.

## RIP_SKEWED_OVAL_PIE_SLICE

*Draw a filled pie slice of a rotated ellipse*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `[` |
| **Arguments** | `x:2 y:2 x_rad:2 y_rad:2 start_ang:2 end_ang:2 rotation:2` *(field meanings from census analysis)* |

**Format:** `!|[ <x> <y> <x_rad> <y_rad> <start_ang> <end_ang> <rotation>`
**Example:** `!|[503F1G0M20601M`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (NEWCMDS.RIP — named in comment "Show a RIP_SKEWED_OVAL_PIE_SLICE" — and SHAPES.RIP; 6 uses) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP name inventory, not in SyncTERM

Pie slice of a rotated ellipse; the arc ends are joined to the center and the interior filled, as with [RIP_OVAL_PIE_SLICE](09-level-0-commands-g-r.md#rip_oval_pie_slice). Same seven fields as [`]`](#rip_skewed_oval_arc); the border is controlled by [RIP_SET_BORDER](09-level-0-commands-g-r.md#rip_set_border) (NEWCMDS.RIP demos both states).

## RIP_SKEWED_OVAL_CHORD

*Draw a filled chord segment of a rotated ellipse*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `+` |
| **Arguments** | `x:2 y:2 x_rad:2 y_rad:2 start_ang:2 end_ang:2 rotation:2` *(field meanings from census analysis)* |

**Format:** `!|+ <x> <y> <x_rad> <y_rad> <start_ang> <end_ang> <rotation>`
**Example:** `!|+803F1G0M20601M`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (NEWCMDS.RIP — named in comment "Show a RIP_SKEWED_OVAL_CHORD" — and SHAPES.RIP; 6 uses) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP name inventory, not in SyncTERM

Chord segment: the arc's endpoints are joined by a straight line and the enclosed segment is drawn/filled. Same seven fields as [`]`](#rip_skewed_oval_arc); demonstrated with and without border in NEWCMDS.RIP.

## RIP_FILLED_OVAL_CHORD

*Draw a filled chord segment of an (unrotated) ellipse*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `_` |
| **Arguments** | `x:2 y:2 start_ang:2 end_ang:2 x_rad:2 y_rad:2` *(field meanings from census analysis)* |

**Format:** `!|_ <x> <y> <start_ang> <end_ang> <x_rad> <y_rad>`
**Example:** `!|_B03F90601G0M`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (NEWCMDS.RIP — named in comment "Show a RIP_FILLED_OVAL_CHORD" — and SHAPES.RIP; 6 uses) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP name inventory, not in SyncTERM

The odd one out of the family: TeleGrafix's comment names it *without* "skewed", and its argument layout differs — six fields, with the angle pair *before* the radii (matching the documented [RIP_OVAL](09-level-0-commands-g-r.md#rip_oval)/[RIP_OVAL_PIE_SLICE](09-level-0-commands-g-r.md#rip_oval_pie_slice) ordering) and no rotation field. It fills the chord segment of an axis-aligned ellipse; border behavior follows [RIP_SET_BORDER](09-level-0-commands-g-r.md#rip_set_border).

## RIP_BOUNDED_TEXT

*Draw text wrapped and clipped inside a rectangle* *(hypothesized name)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `"` |
| **Arguments** | `x0:2 y0:2 x1:2 y1:2 flags:2` and text-string *(field meanings from census analysis)* |

**Format:** `!|" <x0> <y0> <x1> <y1> <flags> <text-string>`
**Example:** `!|"2020A03000This is just another …`

**Attributes used:** Draw Color, Write Mode, Font Style, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (BOUNDS.RIP — comment "Show the bounded text command"; single use) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP name inventory, not in SyncTERM. Name *(hypothesis)*.

Draws a text string that wraps and clips inside the given bounding rectangle. BOUNDS.RIP draws an `!|R2020A030` rectangle first ("Show our bounding box") and then issues this command with the *same coordinates*, making the layout unambiguous: four 2-digit corner coordinates, a 2-digit flags field (`00` observed), then the text (elided above). A one-shot boxed-text primitive, distinct from the [RIP_BEGIN_TEXT](11-level-1-commands.md#rip_begin_text)/[`1e`](11-level-1-commands.md#rip_extended_begin_text) flowed-column machinery.

## RIP_MARKER

*Draw a predefined marker/symbol glyph at a point* *(hypothesized name)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `;` |
| **Arguments** | `x:2 y:2 marker_type:2 x_size:2 y_size:2 rotation:2 flags:2` *(last two fields uncertain; from census analysis)* |

**Format:** `!|; <x> <y> <marker_type> <x_size> <y_size> <rotation?> <flags?>`
**Example:** `!|;1L40001S1S0000`

**Attributes used:** Draw Color, Back Color, Fill Style, Write Mode, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (MARKER.RIP — title text "RIPscrip Markers" — and MARKER2.RIP; 361 uses) — not in the 2.00a4 spec, not in the RIPSCRIP.HLP name inventory, not in SyncTERM. Name *(hypothesis)*.

Stamps one of a set of predefined marker/symbol glyphs at a point, scaled by an x/y size pair, using the current fill style — MARKER2.RIP cycles the fill color (`S010G`…`S010V`) per marker to prove it. Fourteen-character arguments decompose as seven 2-digit fields: position, marker type (`00`–`0C` in the first demo row, `0E`… in MARKER2.RIP), sizes, then four more digits presumed rotation + flags *(hypothesis)* — observed values `0000`, `0003`, `8C03`. At 361 uses across two dedicated demo scenes, this is the most heavily exercised of the undocumented 3.0 primitives.

## RIP_POLY_POLYGON

*Draw a multi-contour polygon with even-odd fill* *(hypothesized name)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `<` |
| **Arguments** | `npoly:2` then per polygon: `nverts:2 x1:2 y1:2 ... xn:2 yn:2` *(field meanings from census analysis)* |

**Format:** `!|< <npoly> [<nverts> <x1> <y1> ... ]...`
**Example:** `!|<0304A010D010D030A03003BM1ACU2UA62U04A615CU15CU25A625`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting) *(inferred)*
**Evidence:** corpus (POLYPOLY.RIP — on-screen title `RIP_POLY_POLYGON`; 3 uses) — no such command exists in the 2.00a4 spec, not in the RIPSCRIP.HLP name inventory, not in SyncTERM. Name *(hypothesis, from the demo's own title)*.

A multi-contour polygon: a polygon count followed by, for each contour, a vertex count and that many coordinate pairs. Contours are filled by the even-odd rule — POLYPOLY.RIP places a colored circle behind the figure "so you can see the transparency aspect" through the holes — and the border is switched via [RIP_SET_BORDER](09-level-0-commands-g-r.md#rip_set_border) ("With borders:" / "Without borders:"). The example above encodes 3 contours (a 4-vertex outer box and two triangular holes). Vertex lists may be split across physical lines with the standard `\` continuation. This is a genuinely new object type: 2.00a4 has nothing comparable, and the single-contour [RIP_FILLED_POLYGON](09-level-0-commands-g-r.md#rip_filled_polygon) even-odd overlap behavior is only an accident of its fill rule.

## RIP_ARC

*Draw circular arc in current color/line thickness*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `A` |
| **Arguments** | `x:XY y:XY start_ang:2 end_ang:2 radius:XY` |

**Format:** `!|A <x> <y> <start_ang> <end_ang> <radius>`
**Example:** `!|A1E18003G0T`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_arc)) · HLP (`RIP_Arc`) · SyncTERM (ripper.c:13470) — not observed in the RIPtel demo corpus

Draws a segment of a circle counterclockwise from `<start_ang>` to `<end_ang>` (0° = 3 o'clock), honoring aspect ratio and line thickness but not line patterns. See the 2.x entry for full angle semantics. SyncTERM version-gates a 1.54 bug-compatibility quirk after arc drawing (a stale line-pattern register clobber), fixed in its 3.0 mode — evidence that 1.54 and 3.0 arc behavior were treated as distinct contracts.

## RIP_ONE_PALETTE

*Set RGB color of 16-color Desktop Palette*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `a` |
| **Arguments** | `color:2 value:2` |

**Format:** `!|a <color> <value>`
**Example:** `!|a0202`

**Attributes used:** Draw Color, Back Color, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_one_palette)) · HLP (`RIP_OnePalette`) · corpus (7 uses in LANDSCPE.RIP, LGF1.RIP) · SyncTERM (ripper.c:14221)

Changes one entry of the 16-color Desktop Palette to one of the 64 master-palette colors (0–63); HLP error strings confirm the 0–63 range ("Invalid system palette color value (>63)"). Used sparingly in the corpus for scene-specific tints; palette-cycling animation is also attested by the HLP string "Unable to animate palette".

## RIP_BAR

*Draw filled rectangle using fill style/no border*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `B` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|B <x0> <y0> <x1> <y1>`
**Example:** `!|B0000HS9Q`

**Attributes used:** Back Color, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_bar)) · HLP (`RIP_Bar`) · corpus (103 uses in 11 files) · SyncTERM (ripper.c:13517)

Fills a rectangle with the current fill pattern/color, never drawing a border (the only command that ignores the border option). The corpus example, from TWEATHER.RIP, fills from the origin to 640×about-350 in 640-world coordinates. Coordinates are normalized to upper-left/lower-right before drawing.

## RIP_EXTENDED_TEXT_WINDOW

*Define a resolution independent text window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `b` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY width:2 height:2 font_no:1 flags:4 reserved:3` |

**Format:** `!|b <x0> <y0> <x1> <y1> <width> <height> <font_no> <flags> <reserved>`
**Example:** `!|b0000ZKQO280P2000H000`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_extended_text_window)) · HLP (`RIP_ExtendedTextWindow`, plus `$ISEXTWIN$` variable) · SyncTERM (ripper.c:14255, descriptor `2#2#2#2#2#2#1#4#`) — not observed in the RIPtel demo corpus

A pixel-addressed, truly resolution-independent text window: a bounding rectangle plus desired columns/rows and font, with automatic font shrinking to fit, and a flags field controlling wrap, cursor, activation, erasure, and protection (see the 2.x entry for the full fitting algorithm and flag table).

The 2.00a4 draft assigned this letter `b` while *also* assigning `b` to RIP_SET_BASE_MATH — an outright collision in the draft. The RIPtel corpus resolves it: the shipping 3.0 driver moved SET_BASE_MATH to the wire opcode [`J`](09-level-0-commands-g-r.md#rip_set_base_math), leaving `b` to the extended text window, exactly as SyncTERM's descriptor table also has it.

## RIP_CIRCLE

*Draw circle in current color and line thickness*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `C` |
| **Arguments** | `x_center:XY y_center:XY radius:XY` |

**Format:** `!|C <x_center> <y_center> <radius>`
**Example:** `!|C1E180M`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_circle)) · HLP (`RIP_Circle`) · SyncTERM (ripper.c:13546) — not observed in the RIPtel demo corpus

Aspect-ratio-corrected circle in the current color and thickness. The demos favor [RIP_FILLED_OVAL](09-level-0-commands-g-r.md#rip_filled_oval) and [RIP_FILLED_CIRCLE](09-level-0-commands-g-r.md#rip_filled_circle) over the bare outline form.

## RIP_COLOR

*Set current Drawing Color for graphics*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `c` |
| **Arguments** | `color:CM` |

**Format:** `!|c <color>`
**Example:** `!|c0F`

**Attributes used:** Draw Color, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_color)) · HLP (`RIP_Color`) · corpus (359 uses in 49 files) · SyncTERM (ripper.c:14366)

Sets the drawing color for primitives, borders, and graphics text. In Color Map mode (the corpus default — the standard prologue sets `M08`) the parameter indexes the 256-entry Drawing Palette; in Direct RGB mode it becomes an UltraNum-encoded RGB value whose width follows the [RIP_SET_COLOR_MODE](09-level-0-commands-g-r.md#rip_set_color_mode) bit setting. HLP error strings pin the 3.0 driver's limits: palette indices 0–255, direct RGB "8 bits per channel only" in this driver. See the 2.x entry for the full palette and RGB encoding tables.

## RIP_SET_DRAWING_PALETTE

*Set Drawing Palette entries to RGB colors*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `D` |
| **Arguments** | `num:2 start:2 bits:1 c1:4 ... cn:4` |

**Format:** `!|D <num> <start> <bits> <c1> ... <cn>`
**Example:** `!|D0W0W8000000040008000C000G000K000O000S000W000a000e000i000m000 …`

**Attributes used:** Draw Color, Back Color, Base Math (current setting; color values always UltraNums)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_set_drawing_palette)) · HLP (`RIP_SetDrawingPalette`) · corpus (BLUEFADE.FN, single use — the 256-color faded background) · SyncTERM (ripper.c:9173 — descriptor-only `2#2#2#2#2#2#`, no handler)

Sets a run of 256-color Drawing Palette entries to arbitrary RGB values (always 4-digit UltraNums regardless of base math). BLUEFADE.FN's one enormous use (elided above) programs 32 entries starting at entry 32 with 8-bit components to build the signature blue-fade background — the command that gates every demo's `<<IF $COLORS$<"256">>` branch. HLP limits: base ≤255, ≤256 colors per command. SyncTERM knows the opcode only as an unimplemented descriptor whose 6-field shape does not match the real variable-length layout.

## RIP_ONE_DRAWING_PALETTE

*Set one Drawing Palette entry to an RGB color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `d` |
| **Arguments** | `entry:2 bits:1 rgb_color:4` |

**Format:** `!|d <entry> <bits> <rgb_color>`
**Example:** `!|d0W80000`

**Attributes used:** Draw Color, Back Color, Base Math (current setting; color value always UltraNums)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_one_drawing_palette)) · HLP (`RIP_OneDrawingPalette`) · corpus (65 uses in TUNNEL.RIP) — not in SyncTERM

Single-entry form of [RIP_SET_DRAWING_PALETTE](#rip_set_drawing_palette). TUNNEL.RIP hammers it 65 times (entry `0W` = 32 onward, 8-bit components) to animate its tunnel color ramp. Entry 0 is the screen background color; change it with care.

## RIP_ERASE_VIEW

*Clear Graphics Window to current background color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `E` |
| **Arguments** | `<none>` |

**Format:** `!|E`
**Example:** `!|E`

**Attributes used:** Back Color, Port, Base Math (N/A)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_erase_view)) · HLP (`RIP_EraseView`, plus `$EGW$` variable) · SyncTERM (ripper.c:13579) — not observed in the RIPtel demo corpus

Clears the graphics viewport to the background color; ignored if the viewport is deactivated. The demos instead clear by drawing full-frame [RIP_FILLED_RECTANGLE](09-level-0-commands-g-r.md#rip_filled_rectangle)s or via port copies.

## RIP_ERASE_WINDOW

*Clears Text Window to current background color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `e` |
| **Arguments** | `<none>` |

**Format:** `!|e`
**Example:** `!|e`

**Attributes used:** Base Math (N/A)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_erase_window)) · HLP (`RIP_EraseWindow`, plus `$ETW$` variable) · SyncTERM (ripper.c:14388) — not observed in the RIPtel demo corpus

Clears the TTY text window's full bounding rectangle to background color 0 and homes the cursor. Ignored when no text window is active — the demos typically deactivate the text window outright (`$DTW$`, or `!|w0000000000`).

## RIP_FILL

*Flood fill screen area with current fill settings* *(removed from the language)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `F` |
| **Arguments** | `x:XY y:XY border:CM` *(1.54 form)* |

**Format:** `!|F <x> <y> <border>`
**Example:** `!|F1E180F`

**Attributes used:** Back Color, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_fill) — documented as **removed**) · SyncTERM (ripper.c:13593, descriptor `2#2#2#` — still implements the 1.54 flood fill) — not in the RIPSCRIP.HLP name inventory, not observed in the RIPtel demo corpus

RIPscrip 2.0 removed the 1.54 flood fill as unimplementable reliably across resolutions and platforms, directing authors to the filled-object commands instead; the 2.x entry carries the full replacement table. The 3.0 evidence is consistent on both sides: the RIPtel corpus never floods (and RIPSCRIP.HLP lists no `RIP_Fill`), while SyncTERM keeps the 1.54 behavior for legacy scenes.

## RIP_SET_WORLD_FRAME

*Sets the coordinates for the World Frame*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `f` |
| **Arguments** | `x_dim:XY y_dim:XY` |

**Format:** `!|f <x_dim> <y_dim>`
**Example:** `!|fZKQO`

**Attributes used:** Base Math (current setting)
**Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/10-level-0-commands-s-w.md#rip_set_world_frame)) · HLP (`RIP_SetWorldFrame`, plus `$WORLD$`/`$WORLDW$`/`$WORLDH$` variables) · corpus (151 uses in 103 files) · SyncTERM (ripper.c:14401)

The single biggest semantic change of the 2.x/3.x era: it redefines the logical coordinate space away from 1.54's fixed 640×350, making every subsequent coordinate frame-relative. It closes the standard corpus prologue in over 100 files as `!|fZKQO` — `ZK`/`QO` decode to **1280×960**, the square-pixel MegaNum world frame recommended by the 2.00a4 [RIP_HEADER](09-level-0-commands-g-r.md#rip_header) discussion. Alternate observed frames: `HSDC` = 640×480, `HR9S` = 639×352, `HRDC` = 639×480 (BLUEFADE.FN works at 640×350 and TELPORT.FN's comments then "reset our environment back to 1280x960 world coordinates"). ONLINE.RIP demonstrates the driver's argument-parsing tolerance with an inline trailing comment: `!|fZKQO                 Set world coordinats to 1280x960` *(sic)*.

---

[◀ Prev: Protocol Definition & Syntax](07-protocol-definition.md) · [Contents](README.md) · [Next: Level-0 Commands (G–R) ▶](09-level-0-commands-g-r.md)
