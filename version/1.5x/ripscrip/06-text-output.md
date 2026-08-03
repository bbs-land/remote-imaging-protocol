# Text Output & Fonts

[◀ Prev: Colors & Attributes](05-colors-and-attributes.md) · [Contents](README.md) · [Next: Drawing Primitives ▶](07-drawing-primitives.md)

This section covers the commands for positioning and drawing graphics text: [RIP_MOVE](#rip_move), [RIP_TEXT](#rip_text), [RIP_TEXT_XY](#rip_text_xy), and [RIP_FONT_STYLE](#rip_font_style).

## RIP_MOVE

*Move the current drawing position to (X,Y)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `m` |
| **Arguments** | `x:2, y:2` |

**Format:** `!|m <x> <y>`

**Example:** `!|m0509`

**Attributes used:** Viewport

This command moves the current graphics drawing cursor to (x,y).  You could use this to draw text at a certain point, but you'd probably use [RIP_TEXT_XY](#rip_text_xy) instead.  This command is primarily provided for future development which will make use of its ability to relocate the current drawing position without physically drawing anything.

## RIP_TEXT

*Draw text in current font/color at current spot*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `T` |
| **Arguments** | `text-string` |

**Format:** `!|T <text-string>`

**Example:** `!|Thello world`

**Attributes used:** Draw Color, Write Mode, Font Sizes, Viewport

This command displays text at the current location in the graphics window, as set with the [RIP_MOVE](#rip_move) command.  The text is also affected by the most recent settings of these commands:

| Command | Description of Command |
|---|---|
| [RIP_FONT_STYLE](#rip_font_style) | font style (character set, direction, size) |
| [RIP_WRITE_MODE](05-colors-and-attributes.md#rip_write_mode) | drawing mode (normal or XOR) |
| [RIP_COLOR](05-colors-and-attributes.md#rip_color) | drawing color (from the 16-color RIP palette) |

The drawing position is placed at the end of the last character drawn.

The current drawing position is set immediately to the right of the drawn text.  Subsequent Line, Circle or other such commands will not affect this position.  This provides a means so that you can quickly do another RIP_TEXT command (presumably in another color) at a later time and have the text show up immediately after the previous text.

## RIP_TEXT_XY

*Draw text in current font/color at specific spot*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `@` |
| **Arguments** | `x:2, y:2 and text-string` |

**Format:** `!|@ <x> <y> <text-string>`

**Example:** `!|@0011hello world`

**Attributes used:** Draw Color, Write Mode, Font Sizes, Viewport

This command is an efficient combination of [RIP_MOVE](#rip_move) and [RIP_TEXT](#rip_text).  The text is drawn at the specified location according to the same settings as apply to RIP_TEXT (see above).

The current drawing position is set immediately to the right of the drawn text.  Subsequent Line, Circle or other such commands will not affect this position.  This provides a means so that you can quickly do another RIP_TEXT command (presumably in another color) at a later time and have the text show up immediately after the previous text.

## RIP_FONT_STYLE

*Select current font style, orientation and size*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `Y` |
| **Arguments** | `font:2, direction:2, size:2, res:2` |

**Format:** `!|Y <font> <direction> <size> <res>`

**Example:** `!|Y01000400`

**Attributes used:** Font Sizes

This command sets the font, direction and size for [RIP_TEXT](#rip_text) commands.

| Font | Description of Font | |
|---|---|---|
| 00 | Default 8x8 font | Bit-Mapped |
| 01 | Triplex Font | Scalable |
| 02 | Small Font | Scalable |
| 03 | Sans Serif Font | Scalable |
| 04 | Gothic [Old English] Font | Scalable |
| 05 | Script Font | Scalable |
| 06 | Simplex Font | Scalable |
| 07 | Triplex Script Font | Scalable |
| 08 | Complex Font | Scalable |
| 09 | European Font | Scalable |
| 0A | Bold Font | Scalable |

For the Direction parameter, use 00 to indicate horizontal and 01 for vertical.

For the Size parameter, use 01 for the normal default size, 02 for x2 magnification, 03 for x3 magnification, ... , and 0A for x10 magnification.

> **NOTE:**  The Default font is bit-mapped and looks best when drawn in size 1.  In sizes greater than one, the individual pixels are enlarged, giving a jagged look.  This may not be the desired effect.  The fonts 1 - 4 are smooth scalable vector fonts.

---

[◀ Prev: Colors & Attributes](05-colors-and-attributes.md) · [Contents](README.md) · [Next: Drawing Primitives ▶](07-drawing-primitives.md)
