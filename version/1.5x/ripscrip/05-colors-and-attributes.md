# Colors & Attributes

[◀ Prev: Window Commands](04-window-commands.md) · [Contents](README.md) · [Next: Text Output & Fonts ▶](06-text-output.md)

This section covers the commands that control drawing colors, palettes and write modes: [RIP_COLOR](#rip_color), [RIP_SET_PALETTE](#rip_set_palette), [RIP_ONE_PALETTE](#rip_one_palette), and [RIP_WRITE_MODE](#rip_write_mode).

## RIP_COLOR

*Set current Drawing Color for graphics*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `c` |
| **Arguments** | `color:2` |

**Format:** `!|c <color>`

**Example:** `!|cA`

**Attributes used:** Draw Color

This command sets the color for drawing lines, circles, arcs, rectangles, and other graphics primitives, as well as the color for drawing graphics-text from the [RIP_TEXT](06-text-output.md#rip_text) class of commands (not from ASCII/ANSI text).  This command does not affect Fill colors or Fill Patterns (see below).  It does affect the borders of graphic objects, for example the border of an ellipse drawn with the [RIP_FILLED_OVAL](07-drawing-primitives.md#rip_filled_oval) command.  (The interior of the ellipse would be drawn according to the most recent [RIP_FILL_STYLE](08-line-fill-styles.md#rip_fill_style) command.)

This command chooses one of the colors of the 16-color RIP palette defined by the [RIP_SET_PALETTE](#rip_set_palette).  Here is the default 16-color RIP palette:

| Value | Color |
|---|---|
| 00 | Black (00 is always the background color) |
| 01 | Blue |
| 02 | Green |
| 03 | Cyan |
| 04 | Red |
| 05 | Magenta |
| 06 | Brown |
| 07 | Light Gray |
| 08 | Dark Gray |
| 09 | Light Blue |
| 0A | Light Green |
| 0B | Light Cyan |
| 0C | Light Red |
| 0D | Light Magenta |
| 0E | Yellow |
| 0F | White |

## RIP_SET_PALETTE

*Set 16-color Palette from master 64-color palette*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `Q` |
| **Arguments** | `c1:2, c2:2, ... c16:2` |

**Format:** `!|Q <c1> <c2> ... <c16>`

**Example:** `!|Q000102030405060708090A0B0C0D0E0F`

**Attributes used:** Draw Color

This command modifies the 16-color RIP palette by choosing from the 64 colors in the master palette.  This allows you to alter the colors in your RIPscrip graphics scenes.  Once a Set Palette command is processed, any colors on the screen that had their corresponding palette entries changed will instantly switch to the new color set.  You may obtain color cycling effects by using this command.  The default 16-color RIP palette is restored by the [RIP_RESET_WINDOWS](04-window-commands.md#rip_reset_windows) command.  The default 16-color RIP palette is:

| 16-Color RIP Palette Color Code | Master 64-Color EGA Palette Color Code | Color |
|---|---|---|
| 00 | 0  (00) | Black |
| 01 | 1  (01) | Blue |
| 02 | 2  (02) | Green |
| 03 | 3  (03) | Cyan |
| 04 | 4  (04) | Red |
| 05 | 5  (05) | Magenta |
| 06 | 7  (06) | Brown |
| 07 | 20 (0K) | Light Gray |
| 08 | 56 (1K) | Dark Gray |
| 09 | 57 (1L) | Light Blue |
| 0A | 58 (1M) | Light Green |
| 0B | 59 (1N) | Light Cyan |
| 0C | 60 (1O) | Light Red |
| 0D | 61 (1P) | Light Magenta |
| 0E | 62 (1Q) | Yellow |
| 0F | 63 (1R) | White |

Color 00 of the 16-color RIP palette is always the background color (which is typically Black, or color 00 of the 64-color EGA palette).

## RIP_ONE_PALETTE

*Set color of 16-color Palette from Master Palette*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `a` |
| **Arguments** | `color:2 value:2` |

**Format:** `!|a <color> <value>`

**Example:** `!|a051B`

**Attributes used:** Draw Color

This command changes one color in the 16-color palette.  The color number is sent along with the new color value from the Master Color Palette. The color `<value>` must be in the range of 0-63.  Once a Set One Palette command is processed, any colors on the screen that correspond to the `<color>` number will be changed instantly to the new color value.  You may obtain color cycling effects by using this command.  The default RIP palette is restored when by [RIP_RESET_WINDOWS](04-window-commands.md#rip_reset_windows).

| 16-Color RIP Palette Color Code | Master 64-Color EGA Palette Color Code | Color |
|---|---|---|
| 00 | 0  (00) | Black |
| 01 | 1  (01) | Blue |
| 02 | 2  (02) | Green |
| 03 | 3  (03) | Cyan |
| 04 | 4  (04) | Red |
| 05 | 5  (05) | Magenta |
| 06 | 7  (06) | Brown |
| 07 | 20 (0K) | Light Gray |
| 08 | 56 (1K) | Dark Gray |
| 09 | 57 (1L) | Light Blue |
| 0A | 58 (1M) | Light Green |
| 0B | 59 (1N) | Light Cyan |
| 0C | 60 (1O) | Light Red |
| 0D | 61 (1P) | Light Magenta |
| 0E | 62 (1Q) | Yellow |
| 0F | 63 (1R) | White |

Color 00 of the 16-color RIP palette is always the background color (which is typically Black, or color 00 of the 64-color EGA palette).

## RIP_WRITE_MODE

*Set drawing mode for graphics primitives*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `W` |
| **Arguments** | `mode:2` |

**Format:** `!|W <mode>`

**Example:** `!|W00`

**Attributes used:** Write Mode

This command sets the current drawing mode for most of the graphics primitives:

| Mode | Description |
|---|---|
| 00 | Normal drawing mode (overwrite) |
| 01 | XOR (complimentary) mode |

In normal mode, things are drawn in the current drawing color over top of whatever is in the graphics viewport.  This is the typical mode of operation in a GUI environment.

In the XOR mode, instead of changing each pixel to the current drawing color, the pixel is inverted (black changes to white, red to green, etc.).  Drawing the same item a second time erases it completely.  This mode is useful for drawing something temporarily, or for animation.  The Rubber Band mode of most paint programs uses a mode like this.

---

[◀ Prev: Window Commands](04-window-commands.md) · [Contents](README.md) · [Next: Text Output & Fonts ▶](06-text-output.md)
