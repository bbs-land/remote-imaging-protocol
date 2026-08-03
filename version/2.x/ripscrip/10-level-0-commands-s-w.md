# Level-0 Commands (S–W)

[◀ Prev: Level-0 Commands (G–R)](09-level-0-commands-g-r.md) · [Contents](README.md) · [Next: Level-1 Commands ▶](11-level-1-commands.md)

This page covers the Level-0 commands [RIP_SET_BASE_MATH](#rip_set_base_math), [RIP_SET_BORDER](#rip_set_border), [RIP_SET_COLOR_MODE](#rip_set_color_mode), [RIP_SET_COORDINATE_SIZE](#rip_set_coordinate_size), [RIP_SET_DRAWING_PALETTE](#rip_set_drawing_palette), [RIP_SET_PALETTE](#rip_set_palette), [RIP_SET_WORLD_FRAME](#rip_set_world_frame), [RIP_TEXT](#rip_text), [RIP_TEXT_WINDOW](#rip_text_window), [RIP_TEXT_XY](#rip_text_xy), [RIP_VIEWPORT](#rip_viewport), and [RIP_WRITE_MODE](#rip_write_mode).

## RIP_SET_BASE_MATH

*Added in RIPscrip v2.A0.*

*Sets the base math for most RIPscrip parameters*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `b` |
| **Arguments** | `base_math:2` |

**Format:** `!|b <base_math>`
**Example:** `!|b1S`

**Attributes used:** Base Math (MegaNums only)

This command sets the Base Math used extensively throughout the RIPscrip language.  By default, the RIPscrip language used Base-36 math for all of its numeric parameters.  This number can be changed to accomodate larger values for numeric parameters.  For example, a two digit base-36 number can hold a number from 0-1295.  A two digit Base-64 number can hold a number from 0-4095 which is considerably larger.

This can be used when you set the World Coordinate Frame to larger coordinates, even larger than 1024x1024.  Altering the Base Math of the parameters gives you the ability to handle larger numbers without expanding the size (byte-wise) of parameters throughout the language (see [RIP_SET_COORDINATE_SIZE](#rip_set_coordinate_size)).

**NOTE:** This command ALWAYS uses base-36 (MegaNum) math for its parameter.  The reason for this is that this is a universal command, and the Base Math may not be known at the moment the command is received.

When a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) command is received, the base math is reset to Base-36 (MegaNum) values.  If a scene is received after the Reset Windows command and it uses Base Math other than Base-36, then you MUST send a RIP_SET_BASE_MATH command immediately after the RIP_RESET_WINDOWS command.

The valid settings at this time are:

- Base-36 (`10` meganum)
- Base-64 (`1S` meganum)

If an illegal Set Base Math command is received (an illegal base math setting), RIPscrip will default to MegaNums.

## RIP_SET_BORDER

*Added in RIPscrip v2.A3.*

*Enable or disable borders on filled objects*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `N` |
| **Arguments** | `borders:2` |

**Format:** `!|N <borders>`
**Example:** `!|N01`

**Attributes used:** Base Math (current setting)

This command specifies whether borders are drawn with filled objects (eg, [RIP_FILLED_RECTANGLE](08-level-0-commands-a-f.md#rip_filled_rectangle), etc).  This command does not affect the command [RIP_BAR](08-level-0-commands-a-f.md#rip_bar).  When borders are enabled, the following commands will draw a border around the filled region:

- RIP_FILLED_RECTANGLE
- [RIP_FILLED_CIRCLE](08-level-0-commands-a-f.md#rip_filled_circle)
- [RIP_FILLED_OVAL](08-level-0-commands-a-f.md#rip_filled_oval)
- [RIP_PIE_SLICE](09-level-0-commands-g-r.md#rip_pie_slice)
- [RIP_OVAL_PIE_SLICE](09-level-0-commands-g-r.md#rip_oval_pie_slice)
- [RIP_FILLED_POLYGON](08-level-0-commands-a-f.md#rip_filled_polygon)
- [RIP_FILLED_POLY_BEZIER](08-level-0-commands-a-f.md#rip_filled_poly_bezier)
- [RIP_FILLED_ROUNDED_RECT](08-level-0-commands-a-f.md#rip_filled_rounded_rect)

Borders drawn around the filled regions are always drawn in the write mode COPY (this is to avoid strange pixel interactions in XOR, OR, AND and NOT modes when they interact with the filled-pixels behind them.  The borders are drawn using the current line thickness.  The polygon related command also adheres to line patterns; all other commands only use the line thickness.  The filled regions are initially drawn using the resolution independent methods of filling.  After that, the border is drawn based on standard methodologies.  For more details, see the section entitled ["THE MATHEMATICS OF GRAPHICS AND COORDINATES"](05-coordinates-and-math.md) for a more detailed explanation of these issues.

Setting the `<border>` parameter to "01" enables borders around filled objects.  A value of "00" indicates that borders are disabled.

When a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) or a [RIP_HEADER](09-level-0-commands-g-r.md#rip_header) command is executed (where the environment is reset), borders are automatically enabled for backwards compatibility.

## RIP_SET_COLOR_MODE

*Added in RIPscrip v2.A0.*

*Set the Color Drawing Mode (MAP or DIRECT RGB)*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `M` |
| **Arguments** | `mode:1 bits:1` |

**Format:** `!|M <mode> <bits>`
**Example:** `!|M18`

**Attributes used:** Base Math (current setting)

This command determines whether Color Mapping mode is in use, or if Direct RGB Colors are being used for color codes.  See [the beginning of this specification](06-color-audio-text.md) for a detailed explanation of the differences between these modes.

The `<mode>` parameter may take on the following values:

| Mode | Description |
|---|---|
| 0 | Color Mapping mode (default) |
| 1 | Direct RGB Color mode |

The `<bits>` parameter is used to determine how many bits are to be parsed in the Direct RGB Color mode.  This parameter is ignored if the Color Mapping Mode is in effect.

When in Direct RGB Color Mode, any Color Code parameter in the RIPscrip language that doesn't explicitly state that it uses only Color Map Mode will process a color value as a raw RGB value encoded in the same fashion as the [RIP_SET_DRAWING_PALETTE](#rip_set_drawing_palette) command.  These parameters, when in Direct RGB Color Mode, will be in ULTRA-NUM format regardless of the global Base Math settings for compression.  Color palette mode though uses the current base math settings for these color parameters.

In Color Mapping Mode, these color parameters will typically not be the lengthy four-digit sequence used in Direct RGB Color Mode.  When in Color Mapping Mode, the color parameters used throughout RIPscrip will obey the global Base Math setting - it's only in Direct RGB Color Mode where UltraNums are forced ON for these parameters.

## RIP_SET_COORDINATE_SIZE

*Added in RIPscrip v2.A1.*

*Sets the number of bytes used for XY coordinates*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `n` |
| **Arguments** | `byte_size:1 res:3` |

**Format:** `!|n <byte_size> <res>`
**Example:** `!|n2000`

**Attributes used:** Base Math (current setting)

This command alters the number of bytes that will be expected for all subsequent X or Y parameters in RIPscrip.  By default, all X or Y parameters are two bytes in length as denoted syntax-wise as x:XY or y:XY in the specification.  This command allows you to alter this size.  In 1.x versions of RIPscrip, the maximum size of a parameter was in the X direction and the maximum value was 639.  This could safely be represented in two meganum digits.  In 2.0 of RIPscrip and beyond, we introduce a world coordinate system into the specification which allows you to use a much higher resolution for your drawing environment.  This facilitates resolution independence.  To provide for future expandability when ultra-high resolution devices might exist, the possibility of an X or Y coordinate exceeding 4095 might occur.  Since this is the largest value that can be accomodated in a 2-byte UltraNum, we offer an option that provides for extensibility.

Using this parameter to expand the byte size beyond 2 will make a RIPscrip file bigger.  This is because every X/Y coordinate will be expanded in size by a certain number of bytes, and since these are the fundamental values in any graphical environment, it will result in larger RIPscrip files.  In practice, this command should be used with caution.  When situations permit, do not use this command, or use it with a byte size of 2 (the default).  This will keep files from ever getting larger than they would be by default.

Valid settings are from 2-5.  If any other values are specified, then a value of 2 will be assumed. *(v2.A3)*

A [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) command will reset the coordinate size back to two byte wide parameters.

**NOTE:** The `<res>` parameter is reserved and should be set to "000" for future expansion by TeleGrafix.

## RIP_SET_DRAWING_PALETTE

*Added in RIPscrip v2.A0.*

*Set Drawing Palette entries to RGB colors*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `D` |
| **Arguments** | `num:2 start:2 bits:1 c1:4 ... cn:4` |

**Format:** `!|D <num> <bits> <start> <c1> ... <cn>`
**Example:** `!|D030180GhZPzr3aZr3`

**Attributes used:** Draw Color, Back Color, Base Math (current setting)

This command will set one or more colors in the 256-entry Drawing Palette to arbitrary RGB color mappings.  This allows you to customize the Drawing Palette with extended color information beyond what you can normally do with the Desktop Palette.  This command is more flexible in nature in that it allows you to access colors between 0-255 and gives you the added flexibility to store more than 2 bits of red, green and blue information thus allowing you to select colors out of a much larger master palette.

The `<num>` parameter specifies how many color entries are contained in this given Palette command.  This is used in determining how many bytes long the command should be.

**NOTE:** The four-digit color parameters are not meganums - they are always ULTRANUMS regardless of the setting of the global base-math value!  With four digits at base-64 math, you can achieve 24-bits of precision in one four-digit number.

If you break-down the four-digit UltraNum color values into their binary equivalents (three bytes), the MSB would contain the Red component, the LSB would contain the Blue component and the byte in between contains the Green component.  The `<bits>` parameter lets you specify the number of bits for each component. Under no circumstances are `<bit>` values above 8 allowed, since this would overflow a four-digit UltraNum parameter (beyond 24 bits).

The `<start>` parameter determines the starting Color Palette Entry number that the first color in the command will be assigned to.  Every following parameter will correspond to the next highest Color Palette Entry number.

Color Entry number zero (0) is the screen's background color.  It is normally set to RGB color 0/0/0, or Black.  Changing this color will alter the background color of the entire screen/environment, so great care should be taken if you alter color number 0.

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

This command modifies the 16-color Desktop palette by choosing from the 64 colors in the master palette.  This allows you to alter the colors in your RIPscrip graphics scenes.  Once a Set Palette command is processed, any colors on the screen that had their corresponding palette entries changed will instantly switch to the new color set (providing the terminal is running in palette mode).  You may obtain color cycling effects by using this command.  The default 16-color RIP palette is restored by the [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) command.

See the [RIP_COLOR](08-level-0-commands-a-f.md#rip_color) command for an exact description of the RGB valus used in this command.

Color 00 of the 16-color RIP palette is always the screen's background color (which is typically Black).

## RIP_SET_WORLD_FRAME

*Added in RIPscrip v2.A0.*

*Sets the coordinates for the World Frame*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `f` |
| **Arguments** | `x_dim:XY y_dim:XY` |

**Format:** `!|f <x_dim> <y_dim>`
**Example:** `!|fSGSG`

**Attributes used:** Base Math (current setting)

This command sets the dimensions of the World Coordinate Frame.  This Frame is the Global Coordinate Space of the RIPscrip drawing screen.  It is not the exact size of the Device Frame - that is a lower level coordinate system.  With this frame, you can "map" your scenes into something, say a 1024x1024 grid and let the terminal worry about mapping it into whatever device coordinates it is using (eg, 640x350, 800x600, etc).  Any coordinates used for setting viewports or other such things will be specified in World Frame Coordinates.  Actual drawing primitives will use the current Drawing Frame's coordinates - another level of coordinate mapping.  This allows you to do things like draw full screen images inside a tinier window, leaving additional drawing space on the screen for other images.

The actual Device Coordinate Frame is not specified in the RIPscrip language anywhere - it is handled entirely by the remote terminal, because only it knows the device coordinates of the environment that it is running under (although the host can query these values from the terminal).

The `<x_dim>` parameter is how wide the drawing area is in logical coordinates.  The `<y_dim>` parameter is how tall the drawing area is in logical coordinates.

### Translating World to Device Coordinates

To translate from World Coordinates to Device Coordinates, you need to know four things:

```text
1/2) The World Frame's dimensions (ie, x_dim and y_dim)
3/4) The dimensions of the actual display device (x_max, y_max)
```

After you have these four values, you can translate any X/Y coordinate pair in World Coordinates to Device Coordinates with the following equations:

```text
EQUATION 1 - TRANSLATE WORLD TO DEVICE COORDINATES
──────────────────────────────────────────────────

           ╔═══════════════════╤═══════════════════╗
           ║       Xw * x_max  │       Yw * y_max  ║
           ║  Xd = ──────────  │  Yd = ──────────  ║
           ║         x_dim     │         y_dim     ║
           ╚═══════════════════╧═══════════════════╝
```

Where (Xw,Yw) is a point in World Coordinates.  We are solving for the point (Xd,Yd) which is in Device Coordinates.  The values x_max and y_max are the physical dimensions of the display device (eg, x_max=640, y_max=350).  The values x_dim and y_dim are the dimensions of the World Coordinate Frame (eg, x_dim=1000, y_dim=1000).

As an example, let's say we are drawing onto a 640x350 display device and our World Coordinate Frame is 1000x1000 logical pixels in size.  If we plot a pixel at coordinates right in the middle of the screen at (500,500), here is what we would have:

```text
     500 * 640   320,000
Xd = ───────── = ─────── = 320
       1000       1000


     500 * 350   175,000
Yd = ───────── = ─────── = 175
       1000       1000
```

This gives us a final Device coordinate of (320,175) which is smack in the middle of the device screen as we wanted.

### Translating Device to World Coordinates

If we solve the above equations for Xw and Yw, we get the inverse equations - translations from Device Coordinates to World Coordinates:

```text
EQUATION 2 - TRANSLATE DEVICE TO WORLD COORDINATES
──────────────────────────────────────────────────

           ╔═══════════════════╤═══════════════════╗
           ║       Xd * x_dim  │       Yd * y_dim  ║
           ║  Xw = ──────────  │  Yw = ──────────  ║
           ║         x_max     │         y_max     ║
           ╚═══════════════════╧═══════════════════╝
```

### Precision of translation

If the World Coordinates are larger than the Device Coordinates, then when you translate World Coordinates to Device Coordinates, there will be pixel-perfect translation.  This means that translation will be unambiguous - they will convert to pixel coordinates without there being any discrepency.  For example, if your Device Coordinates are 640x350 and your World Coordinates are 1280x700, then a Point in World Coordinate space at (0,0) and (1,0) will both translate to the Device Coordinates (0,0) and (0,0) respectively.  This is because the World Coordinate space is twice as large as the Device Coordinate space.  If on the other hand the Device Coordinates were 1280x700 and the World Coordinate space were 640x350, then the World Points (0,0) and (1,0) would translate to (0,0) and (2,0) respectively.  Notice that in the World Coordinates, the pixels were adjacent.  But in the Device Coordinates, they are separated by a one pixel gap.

In summary, World Coordinates should always be equal to or greather than the size of the Device Coordinate Frame, otherwise disparity will exist in the translations.  Since coordinate translation will almost ALWAYS be from World -> Device coordinates, everything will be fine if the World Frame has a larger resolution than the Device Frame.  The same holds true for the inverse equations to translate from Device to World coordinates.  If the World Coordinate Frame is larger than the Device Coordinate Frame, then any point that is translated from Device -> World Coordinates (a Mouse X/Y location for example), might not translate over precisely - skipping over pixel spaces in the World Frame - this is simply because of the precision of translation and cannot be overcome mathematically since a Mouse Pointer's location is pixel based - it doesn't have any finer locations than on pixel boundary locations (there's no such thing as a fractional pixel location in graphics hardware - only mathematically).

To maintain backward compatibility with previous versions of the RIPscrip specification, after a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) command is received, the World Coordinate Frame is defined as having the dimensions 640x350 until reset by a RIP_SET_WORLD_FRAME or [RIP_HEADER](09-level-0-commands-g-r.md#rip_header) command.

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

This command displays text at the current location in the graphics window, as set with the [RIP_MOVE](09-level-0-commands-g-r.md#rip_move) command, or immediately after horizontal text drawn with [RIP_TEXT_XY](#rip_text_xy) or a previous RIP_TEXT command.  Only horizontal text updates the current graphical cursor position used with the RIP_TEXT related commands.  Any form of screen reset (RIP_RESET or [RIP_HEADER](09-level-0-commands-g-r.md#rip_header) with reset options) will reset this current position to (0,0).  A RIP_ERASE_VIEWPORT command does not reset this position.

The text is also affected by the most recent settings of these commands:

| Command | Description of Command |
|---|---|
| [RIP_FONT_STYLE](08-level-0-commands-a-f.md#rip_font_style) | font style (vector font, direction, size) |
| [RIP_WRITE_MODE](#rip_write_mode) | drawing mode (normal or XOR) |
| [RIP_COLOR](08-level-0-commands-a-f.md#rip_color) | drawing color (from the 16-color palette) |
| [RIP_EXTENDED_FONT_STYLE](08-level-0-commands-a-f.md#rip_extended_font_style) | extended font styles (True Type style) |

The current drawing position is set immediately to the right of the drawn text.  Subsequent Line, Circle or other such commands will not affect this position.  This provides a means so that you can quickly do another RIP_TEXT command (presumably in another color) at a later time and have the text show up immediately after the previous text (even after subsequent graphical operations like circles, rectangles, lines, etc).

**IMPORTANT NOTE:** Note that this "current graphics cursor location" is only updated for horizontal text moving from left-to-right in "left justified" mode.  Any other orientation does not reset the location! *(v2.A4)*

## RIP_TEXT_WINDOW

*Define the size and location of the Text Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `w` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY wrap:1 size:1` |

**Format:** `!|w <x0> <y0> <x1> <y1> <wrap> <size>`
**Example:** `!|w00001B0M10`

**Attributes used:** Base Math (current setting)

This command specifies the dimensions of the virtual TTY window that will display all ASCII/ANSI (non-RIPscrip) data coming across the connection.  (x0,y0) defines the upper-left corner of the window in text-based character-cell coordinates.  (x1,y1) defines the lower-right corner of the window (inclusive).  There may be two simultaneous windows on the screen, one for TTY text, and one for the display of RIPscrip graphics (a viewport), and they may overlap.

Bytes received over the modem are first checked for RIPscrip commands.  All bytes that don't conform to the RIPscrip syntax are treated as ANSI/ASCII and displayed in the TTY window (if defined).  User keystrokes that are echoed by the BBS would also appear in the text window by this scheme.

### Font sizes

The text window may be deactivated, ignoring all non-RIPscrip bytes, by setting all RIP_TEXT_WINDOW parameters to zero (0).  The X and Y parameters' ranges vary depending on the setting of the `<size>` parameter which governs the font size used for the output text.  Valid settings for the `<size>` parameter and the ranges for X/Y values are as follows:

| Size | Font Size (*) | X Range | Y Range | Columns | Rows |
|---|---|---|---|---|---|
| 0 | 8x8 | 0-79 | 0-42 | 80 | 43 |
| 1 | 7x8 | 0-90 | 0-42 | 91 | 43 |
| 2 | 8x14 | 0-79 | 0-24 | 80 | 25 |
| 3 | 7x14 | 0-90 | 0-24 | 91 | 25 |
| 4 | 16x14 | 0-39 | 0-24 | 40 | 25 |

(*) Font sizes are based on a 640x350 device resolution

The font sizes in the preceding table vary depending on the resolution that the terminal software is running under.  The actual cell sizes of each font are calculated based on the number of rows and columns required.  For example, at a resolution of 800x600, the cell sizes are calculated by dividing the horizontal resolution by the number of columns (800/80) which yields a cell width of 10.  By doing the same thing for the same height we get 600/43 which equals 13.95348837.  You obviously cannot have a fractional cell size so we round down.  This gives us a cell height of 13.  This gives a vertical region of 559 scan lines of text information yielding 41 extra scan lines that don't get used.  This method can be used to determine the cell sizes for any arbitrary resolution. *(v2.A3)*

### Text window placement

Now that we know what the font cell sizes are, or more acurately, how to calculate them, we need to establish a standardized way to determine screen placement for text characters.  RIPscrip 1.54 was based on 640x350 resolution and text placement was very straightforward.  If the text window was to be defined using font #2 (8x14 under 640x350) and was to be from (10,15) to (20,25) in text coordinates, then our actual upper-left corner of the text window would be located at pixel (10x8,15x14) or (80,210).  Now that we have the upper-left corner of the text window, calculating an arbitrary row/column in the text window is simple based on the cell size. *(v2.A3)*

Under resolutions other than 640x350, we need to take very special care to make the text characters appear as close to the 640x350 counterparts as possible.  In order to calculate the text window's orientation as closely as possible in other resolutions, calculate the upper-left corner of the text window at 640x350 coordinates and scale those coordinates up to the actual resolution used on the terminal.  This will insure that the text window is located as closely to the 640x350 counterpart as possible.  As previously described, the font cell sizes can leave some "slop space" due to fractional columns/rows.  This means that you cannot be pixel-for-pixel accurate with text window placement at varying resolutions.  In this matter, the RIP_TEXT_WINDOW command is not truly resolution independent (see [RIP_EXTENDED_TEXT_WINDOW](08-level-0-commands-a-f.md#rip_extended_text_window) for a truly resolution independent way of creating text windows).

The `<wrap>` parameter applies to the horizontal dimension.  If `<wrap>` is set to 1, then any text that extends beyond the right margin of the window will wrap to the next line of the window, scrolling the window up if necessary.  If `<wrap>` is 0, then any text going beyond the right margin is truncated and no scrolling is performed; the cursor remains at the right margin.

When a text window is defined, its coordinates are relative to the actual screen, not the current graphical Viewport or the Device Coordinate Frame.

**NOTE:** If the text window currently being defined is identical to the currently defined text window, the cursor will not be relocated to the upper-left corner of the window. The only aspect of the text window that can be different and still be deemed "identical" is the `<wrap>` parameter.  For the current and new text windows to be considered identical, the parameters `<x0>`, `<y0>`, `<x1>`, `<y1>` and `<size>` must be the same.

**NOTE:** A text window definition command is ignored if the current text window data table entry is protected! *(v2.A4)*

## RIP_TEXT_XY

*Draw text in current font/color at specific spot*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `@` |
| **Arguments** | `x:XY y:XY` and text-string |

**Format:** `!|@ <x> <y> <text-string>`
**Example:** `!|@0011hello world`

**Attributes used:** Draw Color, Write Mode, Font Style, Viewport, Port, Base Math (current setting)

This command is an efficient combination of [RIP_MOVE](09-level-0-commands-g-r.md#rip_move) and [RIP_TEXT](#rip_text).  The text is drawn at the specified location according to the same settings that apply to RIP_TEXT (see above).

The current drawing position is set immediately to the right of the drawn text.  Subsequent Line, Circle or other such commands will not affect this position.  This provides a means so that you can quickly do another RIP_TEXT command (presumably in another color) at a later time and have the text show up immediately after the previous text.  See the RIP_TEXT command for a more thorough discussion of this subject.

## RIP_VIEWPORT

*Define the size & location of the Graphics Window*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `v` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|v <x0> <y0> <x1> <y1>`
**Example:** `!|v00002E1M`

**Attributes used:** Port, Base Math (current setting)

This command defines the (X,Y) pixel boundaries of the RIPscrip graphics window, which will contain all RIPscrip graphics output.  ASCII/ANSI text will be displayed in the current TTY window defined by the [RIP_TEXT_WINDOW](#rip_text_window) command above.  (x0,y0) defines the upper-left corner of the graphics viewport, and (x1,y1) defines the lower-right corner (inclusive).  The viewport may be deactivated, so RIPscrip graphics commands are ignored, by setting all parameters to zero (0).

The viewport is physically bound to the current [drawing port](02-drawing-ports.md).  In other words when this command is executed, it will modify the viewport for the current drawing port.  This in effect, modifies the clipping rectangle for that port.  If any of the viewport coordinates would extend beyond the boundaries of the drawing port, they will be adjusted to fit inside the port. *(v2.A3)*

Graphics displayed in the viewport are "truncated" at this rectangular border, meaning if a circle would normally extend outside one of the borders, it will be chopped, only displaying the portion of the circle that is contained inside the viewport boundaries.

Coordinates that specify the boundary of the Graphics Viewport are specified in World Coordinates.  The interior area of the Viewport is then considered the Drawing Frame.  The Drawing Coordinate frame are set to the same size/dimension as the World Coordinate Frame area that the viewport inhabits.  For example, if you define a viewport from (100,100) to (299,299) in World Coordinates, then your Drawing Frame will be a drawing area with a resolution of 200x200 logical drawing pixels.  In other words, when you set a viewport, you immediately have a 1:1 mapping of logical drawing pixels to World Coordinate Pixels.  If you wish to change the X/Y Drawing Frame resolution to some other value, use the RIP_SET_DRAWING_FRAME command after setting the viewport.  If you draw anything before executing this command, then the privimitives will be drawing things in the current Drawing Frame's coordinates (a sub-set of the World Coordinates).  The Drawing Frame's coordinates do not take on another mapping until you actually set them with a RIP_SET_DRAWING_FRAME command. *(v2.A0)*

To re-activate a viewport that was previously deactivated, either send a correctly configured RIP_SET_VIEWPORT command, or issue a query command with a `$AVP$` [text variable](17-text-variables-general.md) (Activate ViewPort). *(v2.A4)*

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

This command sets the current drawing mode for most of the graphics primitives:

| Mode | Description | Logical |
|---|---|---|
| 00 | Normal drawing mode (overwrite) | (COPY) |
| 01 | Exclusive-OR  drawing mode | (XOR) |
| 02 | Logical OR drawing mode | (OR) |
| 03 | Logical AND drawing mode | (AND) |
| 04 | Inverse drawing mode | (NOT) |

### Normal (COPY) mode

In normal mode, things are drawn in the current drawing color over top of whatever is in the graphics viewport.  This is the typical mode of operation in a GUI environment.

### XOR mode

In the XOR mode, instead of changing each pixel to the current drawing color, the pixel is merged with the destination pixel using the logical operation XOR.  This is a bitwise manipulation of the destination and source pixel values.  In a monochrome environment, the following bit combinations would yield the following:

| A | B | A XOR B |
|---|---|---|
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 0 |

### OR mode

In the OR mode, instead of changing each pixel to the current drawing color, the pixel is merged with the destination pixel using the logical operation OR.  The bits in the source pixel are OR'ed with the bits in the destination pixel to achieve the final result.  In a monochrome environment, the following bit combinations would yield the following: *(v2.A1)*

| A | B | A OR B |
|---|---|---|
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 1 |

### AND mode

In the AND mode, instead of changing each pixel to the current drawing color, the pixel is merged with the destination pixel using the logical operation AND.  The bits in the source pixel are AND'ed with the bits in the destination pixel to achieve the final result.  In a monochrome environment, the following bit combinations would yield the following: *(v2.A1)*

| A | B | A AND B |
|---|---|---|
| 0 | 0 | 0 |
| 0 | 1 | 0 |
| 1 | 0 | 0 |
| 1 | 1 | 1 |

### NOT mode

In the NOT mode, the color of the source pixel is completely ignored.  The bits in the destination pixel's color are inverted (0's become 1's and 1's become 0's).  In other words, black becomes white, light gray becomes dark gray, etc (this is assuming that the color palette is a 16 color palette with default settings - 256 color palettes may react differently depending on the colors defined.

### Scope and color effects

This command affects all line drawing operations (line, rectangle, circle, oval, pie slice, pixel, etc).  It also affects any fill-based operations (filled bar, internal parts of a pie slice, filled circle, etc).  It only affects level-0 drawing primitives.  It also affects fonts that are drawn to the screen (including the default font). *(v2.A1)*

The effects of OR, AND, XOR and NOT can cause distinctly different resultant colors in 16 color modes as compared to 256 color modes.  For example, if you have a color of 7 (0111 binary) in a 16 color mode, and you NOT that color, you get 8 (1000 binary).  Under a standard 16 color palette, color 7 is light gray and color 8 is dark gray.  Under a 256 color palette though, color 7 (00000111 binary) will convert to 248 (11111000 binary).  This color is definitely not dark gray - it is by default, an RGB of (63,54,0) which is a shade of brownish yellow. *(v2.A3)*

---

[◀ Prev: Level-0 Commands (G–R)](09-level-0-commands-g-r.md) · [Contents](README.md) · [Next: Level-1 Commands ▶](11-level-1-commands.md)
