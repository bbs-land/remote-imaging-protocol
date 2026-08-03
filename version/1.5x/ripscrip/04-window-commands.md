# Window Commands

[◀ Prev: Command Reference Overview](03-command-reference.md) · [Contents](README.md) · [Next: Colors & Attributes ▶](05-colors-and-attributes.md)

This section covers the commands that define and manipulate the text and graphics windows: [RIP_TEXT_WINDOW](#rip_text_window), [RIP_VIEWPORT](#rip_viewport), [RIP_RESET_WINDOWS](#rip_reset_windows), [RIP_ERASE_WINDOW](#rip_erase_window), [RIP_ERASE_VIEW](#rip_erase_view), [RIP_GOTOXY](#rip_gotoxy), [RIP_HOME](#rip_home), and [RIP_ERASE_EOL](#rip_erase_eol).

## RIP_TEXT_WINDOW

*Define the size and location of the Text Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `w` |
| **Arguments** | `x0:2, y0:2, x1:2, y1:2, wrap:1, size:1` |

**Format:** `!|w <x0> <y0> <x1> <y1> <wrap> <size>`

**Example:** `!|w00001B0M10`

**Attributes used:** none

This command specifies the dimensions of the virtual TTY window that will display all ASCII/ANSI (non-RIPscrip) data coming across the connection.  (x0,y0) defines the upper-left corner of the window in text-based character- cell coordinates.  (x1,y1) defines the lower-right corner of the window (inclusive).  There may be two simultaneous windows on the screen, one for TTY text, and one for the display of RIPscrip graphics (a viewport), and they may overlap.

Bytes received over the modem are first checked for RIPscrip commands.  All bytes that don't conform to the RIPscrip syntax are treated as ANSI/ASCII and displayed in the TTY window (if defined).  User keystrokes that are echoed by the BBS would also appear in the text window by this scheme.

The text window may be made invisible, ignoring all non-RIPscrip bytes, by setting all RIP_TEXT_WINDOW parameters to zero (0).  The X and Y parameters ranges vary depending on the setting of the `<size>` parameter which governs the font size used for the output text. Valid settings for the `<size>` parameter and the ranges for X/Y values are as follows:

| Size | Font Size | X Range | Y Range |
|---|---|---|---|
| 0 | 8x8 | 0-79 | 0-42 |
| 1 | 7x8 | 0-90 | 0-42 |
| 2 | 8x14 | 0-79 | 0-24 |
| 3 | 7x14 | 0-90 | 0-24 |
| 4 | 16x14 | 0-39 | 0-24 |

The `<wrap>` parameter applies to both the horizontal and vertical dimensions.  If `<wrap>` is set to 1, then any text that extends beyond the right margin of the window will wrap to the next line of the window, scrolling the window up if necessary.  If `<wrap>` is 0, then any text going beyond the right margin is truncated and no scrolling is performed; the cursor remains at the right margin.

> **NOTE:**  If the text window currently being defined is identical to the currently defined text window, the cursor will not be relocated to the upper-left corner of the window. The only aspect of the text window that can be different and still be deemed "identical" is the `<wrap>` parameter.  For the current and new text windows to be considered identical, the parameters `<x0>`, `<y0>`, `<x1>`, `<y1>` and `<size>` must be the same.

## RIP_VIEWPORT

*Define the size & location of the Graphics Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `v` |
| **Arguments** | `x0:2, y0:2, x1:2, y1:2` |

**Format:** `!|v <x0> <y0> <x1> <y1>`

**Example:** `!|v00002E1M`

**Attributes used:** none

This command defines the (X,Y) pixel boundaries of the RIPscrip graphics window, which will contain all RIPscrip graphics output.  ASCII/ANSI text will be displayed in the virtual TTY window defined by the [RIP_TEXT_WINDOW](#rip_text_window) command above.  (x0,y0) defines the upper-left corner of the graphics viewport, and (x1,y1) defines the lower-right corner (inclusive).  The viewport may be disabled, so RIPscrip graphics commands are ignored, by setting all parameters to zero (0).

Graphics displayed in the viewport are "truncated" at this rectangular border, meaning if a circle would normally extend outside one of the borders, it will be chopped, only displaying the portion of the circle that is contained inside the viewport boundaries.

Coordinates are specified based on a 640x350 pixel resolution, meaning X can be anywhere from 0 - 639, and Y can be anywhere from 0 - 349.  x0 must be less than x1 and y0 must be less than y1 unless all parameters are set to zero, indicating that the graphics window is disabled.

## RIP_RESET_WINDOWS

*Clear Graphics/Text Windows & reset to full screen*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `*` |
| **Arguments** | \<none\> |

**Format:** `!|*`

**Example:** `!|*`

**Attributes used:** none

This command will set the Text Window to a full 80x43 EGA hi-res text mode, place the cursor in the upper left corner, clear the screen, and zoom the Graphics Window to full 640x350 EGA screen.  Both windows are filled with the current graphics background color.  Also, all Mouse Regions and Mouse Buttons are deleted and the Clipboard is erased.  A system One might use this function before entering a text only mode that does not support RIP commands. *(v1.54)*  This command will also restore the default 16-color RIP palette (see [RIP_SET_PALETTE](05-colors-and-attributes.md#rip_set_palette) below).

## RIP_ERASE_WINDOW

*Clears Text Window to current background color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `e` |
| **Arguments** | \<none\> |

**Format:** `!|e`

**Example:** `!|e`

**Attributes used:** none

This clears the TTY text window to the current graphics background color and positions the cursor in the upper-left corner of the window.  If the text window is inactive, then this command is ignored.  If the text and graphics windows overlap, then this command will clear the overlapping portion also.

## RIP_ERASE_VIEW

*Clear Graphics Window to current background color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `E` |
| **Arguments** | \<none\> |

**Format:** `!|E`

**Example:** `!|E`

**Attributes used:** none

This command clears the Graphics Viewport to the current graphics background color.  If the graphics viewport is not active (if the boundaries are 0,0,0,0), then this command is ignored.  If the text and graphics windows overlap, then this command will clear the overlapping portion also.

## RIP_GOTOXY

*Move text cursor to row & column in Text Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `g` |
| **Arguments** | `x:2, y:2` |

**Format:** `!|g <x> <y>`

**Example:** `!|g0509`

**Attributes used:** none

This command sets the position of the text cursor in the TTY Text window, if it is active.  If inactive (if the dimensions are 0,0,0,0), then this command is ignored.  This command is equivalent to the ANSI/VT-100 command goto x/y, `<Esc>[x;yH`, except that the coordinates of that ANSI command are 1-based and the coordinates of this RIPscrip command are 0-based.

## RIP_HOME

*Move cursor to upper-left corner of Text Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `H` |
| **Arguments** | \<none\> |

**Format:** `!|H`

**Example:** `!|H`

**Attributes used:** none

This command positions the text cursor to the upper-left corner in the TTY Text Window, if it is active.

## RIP_ERASE_EOL

*Erase current line from cursor to end of line*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `>` |
| **Arguments** | \<none\> |

**Format:** `!|>`

**Example:** `!|>`

**Attributes used:** none

This command will erase the current text line in the TTY text window from the current cursor location (inclusive) to the end of the line.  The erased region is filled with the current graphics background color.  This differs from the ANSI command `ESC[K` which clears the area with the current ANSI background color.

---

[◀ Prev: Command Reference Overview](03-command-reference.md) · [Contents](README.md) · [Next: Colors & Attributes ▶](05-colors-and-attributes.md)
