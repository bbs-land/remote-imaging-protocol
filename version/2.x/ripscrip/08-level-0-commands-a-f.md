[◀ Prev: Protocol Definition & Syntax](07-protocol-definition.md) • [Contents](README.md) • [Next: Level-0 Commands (G–R) ▶](09-level-0-commands-g-r.md)

# Level-0 Commands (A–F)

Level-0 commands are the graphical building blocks of RIPscrip. They embody all of the simple primitive commands for actually drawing simple graphics like lines, circles, normal graphical text. _(v2.A4)_

This level also contains all of the commands necessary to the basic setup and operation of RIPscrip (eg, base math, color modes, etc). _(v2.A4)_

## RIP_ARC

_Draw circular arc in current color/line thickness_

|               |                                             |
| ------------- | ------------------------------------------- |
| **Level**     | 0                                           |
| **Command**   | `A`                                         |
| **Arguments** | `x:XY y:XY start_ang:2 end_ang:2 radius:XY` |

**Format:** `!|A <x> <y> <start_ang> <end_ang> <radius>` **Example:** `!|A1E18003G0T`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws a circular arc, or a segment of a circle. Drawing begins at `<start_ang>` and terminates at `<end_ang>`. The angles are represented starting at zero for the 3 o'clock position and increasing counterclockwise through a full circle to 360:

```text
     90°
      │
180°──┼──0°
      │
     270°
```

The arc drawing begins at the `<start_angle>` and continues counter-clockwise to the `<end_angle>`. A full circle will be displayed if `<start_ang>`=0 and `<end_ang>`=360. This command recognizes aspect ratios like the circle command does. It does not take advantage of line patterns but does comply with line thickness.

If both angles are equal, nothing is drawn.

The radius is considered to be in the horizontal direction for the purpose of aspect ratio calculations. _(v2.A3)_

Both angles can be greater than 360 degrees. The starting angle must be greater or equal to the ending angle. _(v2.A3)_

## RIP_BACK_COLOR

_Added in RIPscrip v2.A1._

_Set background Drawing Color for graphics_

|               |            |
| ------------- | ---------- |
| **Level**     | 0          |
| **Command**   | `k`        |
| **Arguments** | `color:CM` |

**Format:** `!|k <color>` **Example:** `!|k0004`

**Attributes used:** Back Color, Base Math (current setting)

This command alters the setting of the background pen drawing color. This color is used for fill operations for the background pixels of the fill as well as some erase functions. The color of the foreground pixels of the fill pattern are specified with a [RIP_FILL_STYLE](#rip_fill_style) and [RIP_FILL_PATTERN](#rip_fill_pattern) command. Upon a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows), the background color is automatically set to black and remains that way unless overidden by a RIP_BACK_COLOR command.

The background color can also affect the way line patterns/styles appear on the screen. Normally a line pattern is a sequence of on and off pixels where the on pixels are drawn in the normal RIPscrip foreground drawing color as set by the [RIP_COLOR](#rip_color) command. The other pixels (the off pixels) are normally not drawn at all (ie, transparent). When you use the [RIP_LINE_STYLE](09-level-0-commands-g-r.md#rip_line_style) command, you can specify that these off pixels are to be drawn in the current background color (set with this command). See the RIP_LINE_STYLE for exact syntax on using this mode.

See the RIP_COLOR command for an exact description of the `<color>` parameter used in this command (both Color Palette mode and Direct RGB color mode are supported).

## RIP_BAR

_Draw filled rectangle using fill style/no border_

|               |                           |
| ------------- | ------------------------- |
| **Level**     | 0                         |
| **Command**   | `B`                       |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|B <x0> <y0> <x1> <y1>` **Example:** `!|B00010A0E`

**Attributes used:** Back Color, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)

This command fills a rectangular region with the current fill color and pattern. No border is drawn.

> **NOTE:** If the borders are enabled, then this command will still draw a rectangle with no border. This is the only RIPscrip command that ignores the border option. _(v2.A3)_

This command also does not adhere to the resolution independent method of performing fills. In other words, the fill actually goes to the very lower-left corner of the rectangle. In this manner, this command is not truly resolution independent. If you want to draw a resolution independent filled rectangle, use the [RIP_FILLED_RECTANGLE](#rip_filled_rectangle) with borders disabled. _(v2.A3)_

Before the rectangle is actually drawn, the (x0,y0) and (x1,y1) coordinates are "normalized". This means that they are adjusted so that they specify the upper-left and lower-right coordinates respectively. For example, if the parameters are received in the order of (50,25) and (25,75) - upper-right and lower-left respectively - they would be reorganized to be (25,25) and (50,75) before the rectangle is actually drawn. _(v2.A3)_

## RIP_BEZIER

_Draw a bezier curve_

|               |                                                         |
| ------------- | ------------------------------------------------------- |
| **Level**     | 0                                                       |
| **Command**   | `Z`                                                     |
| **Arguments** | `x1:XY y1:XY x2:XY y2:XY x3:XY y3:XY x4:XY y4:XY cnt:2` |

**Format:** `!|Z <x1> <y1> <x2> <y2> <x3> <y3> <x4> <y4> <cnt>` **Example:** `!|Z0A0B0C0D0E0F0G0H1G`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command provides customizable curves. Four control points are used to create the shape of the curve. The curves beginning point is at point (x1,y1) and it ends at (x4,y4). Points (x2,y2) and (x3,y3) are not necessarily on the curve, but are used to pull the curve in their direction. The diagram below indicates how points 2 and 3 can be utilized to form the desired curve. Note that points 2 and 3 are not actually on the curve (usually), but points 1 and 4 are.

```text
          X2

         ▄▀▀▀▄
       ▄▀     ▀▀▄▄
      █           ▀▄            X4
     █              ▀▄          █
   X1                 █       ▄▀
                       █    ▄▀
                        ▀▄▄▀

                         X3
```

The last parameter of this command is the `<cnt>` parameter. This determines how many "segments" the curve should be drawn in. Each segment is in fact, a straight line. The more segments you allow, the smoother the curve may be. If a curve does not have a significant amount of "curviness" then a low "count" can improve performance of the curve drawing.

The entire bezier curve is drawn using the current line pattern, thickness and write mode. _(v2.A3)_

The actual bezier curve can be described mathematically by the following parametric equations: _(v2.A3)_

```text
X = X1*(1-t)^3 + X2*3*t*(t-1)^2 + X3*3*t^2*(1-t) + X*4*t^3
Y = Y1*(1-t)^3 + Y2*3*t*(t-1)^2 + Y3*3*t^2*(1-t) + Y*4*t^3
```

These are the normal equational forms of the weighted sum B-Spline function using four control points instead of directional vectors. The variable T is varied from 0.0 through 1.0 over some increment to achieve the total curve. _(v2.A3)_

## RIP_CIRCLE

_Draw circle in current color and line thickness_

|               |                                     |
| ------------- | ----------------------------------- |
| **Level**     | 0                                   |
| **Command**   | `C`                                 |
| **Arguments** | `x_center:XY y_center:XY radius:XY` |

**Format:** `!|C <x_center> <y_center> <radius>` **Example:** `!|C1E180M`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws a circle in the current drawing color and line thickness. The `<radius>` is in pixel units. This command understands aspect ratios and will draw a truly circular circle instead of an oblong circle (ellipse) like on other graphics systems.

The radius is considered to be in the horizontal direction for the purpose of aspect ratio calculations. _(v2.A3)_

## RIP_COLOR

_Set current Drawing Color for graphics_

|               |            |
| ------------- | ---------- |
| **Level**     | 0          |
| **Command**   | `c`        |
| **Arguments** | `color:CM` |

**Format:** `!|c <color>` **Example:** `!|cA`

**Attributes used:** Draw Color, Base Math (current setting)

This command sets the color for drawing lines, circles, arcs, rectangles, and other graphics primitives, as well as the color for drawing graphics-text from the [RIP_TEXT](10-level-0-commands-s-w.md#rip_text) class of commands (not from ASCII/ANSI text). This command does not affect Fill colors or Fill Patterns (see below). It does affect the borders of graphic objects, for example the border of an ellipse drawn with the [RIP_FILLED_OVAL](#rip_filled_oval) command when borders are enabled. (The interior of the ellipse would be drawn according to the most recent [RIP_FILL_STYLE](#rip_fill_style) command).

The `<color>` parameter of this command serves two purposes depending on what Color Mode is currently set. The default mode, Color Map mode, signifies that the color parameter of this command will use the parameter as an index into the 256 color Drawing Palette color mapping table. In this mode, this parameter will use the current Base Math set by the global [RIP_SET_BASE_MATH](10-level-0-commands-s-w.md#rip_set_base_math) command. _(v2.A0)_

In Direct RGB Color Mode, the color parameter will be encoded in UltraNums (regardless of the current Base Math setting). The total number of digits used will vary depending on the number of bits per color component (R, G and B). The following table will provide a breakdown of bits vs. total number of UltraNum digits used for Direct RGB Color codes: _(v2.A0)_

| Bits | Total Digits |
| ---- | ------------ |
| 1    | 1            |
| 2    | 1            |
| 3    | 2            |
| 4    | 2            |
| 5    | 3            |
| 6    | 3            |
| 7    | 4            |
| 8    | 4            |

These numeric values will be an encoded RGB value allowing you to set arbitrary RGB colors directly with the Set Color commands. If the RGB color is not permissable, the closest match from the Color palette table will be located and used. The number of bits of precision used in the encoded RGB value will be determined by the [RIP_SET_COLOR_MODE](10-level-0-commands-s-w.md#rip_set_color_mode) command. _(v2.A0)_

When Direct RGB Colors Codes are used, the bits for each Red, Green and Blue are encoded as tightly as possible to ensure no wasted space. For example, if 6 bits were used for R, G and B, you would have the following layout in binary form: _(v2.A0)_

```text
rrrrrr gggggg bbbbbb
```

where each of "r", "g" and "b" could be individual ones or zeros. This binary representation of the color code is then converted to UltraNum using the proper number of digits from the preceding table. For example, if you had binary values: _(v2.A0)_

```text
  RED:  011011
GREEN:  001001
 BLUE:  100100
```

You would have the following binary number: _(v2.A0)_

```text
011011 001001 100100 (111,204 decimal)
```

And in UltraNum, the value would be: R9a _(v2.A0)_

When Color Mapping Mode is in use, the color parameter may be shorter than four digits. This is for space savings, as it is allowable in the specification for the last numeric parameter in a parameter list to be "shorter" than the full length. _(v2.A0)_

In Color Map mode, this command chooses one of the colors of the 256-color Drawing Palette defined by the class of Set Palette commands. Here is the default 16 lowest colors of the Desktop Palette (these correspond to the colors of ANSI text graphics): _(v2.A0)_

| 16-Color RIP Palette Color Code | Master 64-Color Palette Color Code (Base-10) | (B-36) | [B-64] | R/G/B (0-3) | Color |
| --- | --- | --- | --- | --- | --- |
| 00 | 0 | (00) | [0] | 0 0 0 | Black |
| 01 | 1 | (01) | [1] | 0 0 2 | Blue |
| 02 | 2 | (02) | [2] | 0 2 0 | Green |
| 03 | 3 | (03) | [3] | 0 2 2 | Cyan |
| 04 | 4 | (04) | [4] | 2 0 0 | Red |
| 05 | 5 | (05) | [5] | 2 0 2 | Magenta |
| 06 | 20 | (0K) | [K] | 2 1 0 | Brown |
| 07 | 7 | (07) | [7] | 2 2 2 | Light Gray |
| 08 | 56 | (1K) | [u] | 1 1 1 | Dark Gray |
| 09 | 57 | (1L) | [v] | 1 1 3 | Light Blue |
| 0A | 58 | (1M) | [w] | 1 3 1 | Light Green |
| 0B | 59 | (1N) | [x] | 1 3 3 | Light Cyan |
| 0C | 60 | (1O) | [y] | 3 1 1 | Light Red |
| 0D | 61 | (1P) | [z] | 3 1 3 | Light Magenta |
| 0E | 62 | (1Q) | [#] | 3 3 1 | Yellow |
| 0F | 63 | (1R) | [@] | 3 3 3 | White |

The raw color values (0-63) correspond to an RGB color definition in which two bits are used for each of the Red, Green and Blue components. The layout of the binary number is shown as follows: _(v2.A0)_

```text
           Primary    Secondary
         ├─────────┤ ├─────────┤
╔═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗
║▒▒▒│▒▒▒║   │   │   ║   │   │   ║
║▒▒▒│▒▒▒║ R │ G │ B ║ r │ g │ b ║
║▒▒▒│▒▒▒║   │   │   ║   │   │   ║
╚═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝
  80  40  20  10  8   4   2   1  (hex)
  7   6   5   4   3   2   1   0  (position)
```

Notice that each of the R, G and B sections are broken up into two separate bit sections in the color palette entry number. Also, the bits are reversed when they are encoded. Let's look at four separate colors of Red, Green and Blue to see how the bit patterns correspond to the actual Palette entries: _(v2.A0)_

```text
 RED   PALETTE ENTRY    GREEN  PALETTE ENTRY     BLUE  PALETTE ENTRY
 ═══════════════════     ═══════════════════     ═══════════════════
         xxRGBrgb                xxRGBrgb                xxRGBrgb
         ────────                ────────                ────────
 00      00000000        00      00000000        00      00000000
 01      00100000        01      00010000        01      00001000
 10      00000100        10      00000010        10      00000001
 11      00100100        11      00010010        11      00001001
```

Color 00 of the 16-color RIP palette is always the background color (which is typically Black).

## RIP_COMMENT

_Added in RIPscrip v2.A2._

_Put in a comment as part of a RIPscrip sequence_

|               |             |
| ------------- | ----------- |
| **Level**     | 0           |
| **Command**   | `!`         |
| **Arguments** | `string...` |

**Format:** `!|!This is a comment` **Example:** `!|!Everything from this point on is ignored`

**Attributes used:** Base Math (N/A)

A comment may be embedded inside a RIPscrip file. A comment is one line or longer in length. A comment is a special case of a RIPscrip command letter. In this case, it is the command letter "!". If a "!" is encountered right after a "!|", a "SOH |" or an "STX |" sequence, then the remainder of the command line is a comment. Comments may be line-continued with backslashes just like regular RIPscrip commands can. Some examples of comments might be:

```text
!|!This is a comment

!|c0F|!Set the color to white|c00|!Set the color to black

!|c0F|!this is a very long, continued\
comment line.  Line-continued comments\
adhere to standard RIPscrip line continuation rules
```

Note that the second example shows a comment starting after a legitimate RIPscrip command (set color). This is valid.

The RIP_COMMENT is treated exactly like any other RIPscrip command for the purposes of parsing RIPscrip commands, EXCEPT that the string is explicitly ignored for [text variables](17-text-variables-general.md). Any use of `|` will be interpreted as the beginning of the next command, and `\` will interpreted as a line continuation. No other characters need be escaped out.

## RIP_ERASE_EOL

_Erase current line from cursor to end of line_

|               |          |
| ------------- | -------- |
| **Level**     | 0        |
| **Command**   | `>`      |
| **Arguments** | `<none>` |

**Format:** `!|>` **Example:** `!|>`

**Attributes used:** Back Color, Base Math (N/A)

This command will erase the current text line in the TTY text window from the current cursor location (inclusive) to the end of the line. The erased region is filled with the current graphics background color. This differs from the ANSI command `ESC[K` which clears the area with the current ANSI background color.

## RIP_ERASE_VIEW

_Clear Graphics Window to current background color_

|               |          |
| ------------- | -------- |
| **Level**     | 0        |
| **Command**   | `E`      |
| **Arguments** | `<none>` |

**Format:** `!|E` **Example:** `!|E`

**Attributes used:** Back Color, Port, Base Math (N/A)

This command clears the Graphics Viewport to the current graphics background color. If the graphics viewport is not active (if the boundaries are 0,0,0,0), then this command is ignored. If the text and graphics windows overlap, then this command will clear the overlapping text window portion(s) also.

## RIP_ERASE_WINDOW

_Clears Text Window to current background color_

|               |          |
| ------------- | -------- |
| **Level**     | 0        |
| **Command**   | `e`      |
| **Arguments** | `<none>` |

**Format:** `!|e` **Example:** `!|e`

**Attributes used:** Base Math (N/A)

This clears the TTY text window to the current graphics background color (color #0) and positions the cursor in the upper-left corner of the window. _(v2.A4)_

The area that is erased is actually the bounding rectangle of the text window, not only the display region inside the bounding rectangle. This is different than ANSI commands which erase the text window to a certain color. ANSI/VT-102 commands only erase the display region, not the entire bounding rectangle. _(v2.A4)_

If the text window is not in use or deactivated, then this command is ignored. If the text window and any viewports overlap, then this command will clear the overlapping viewport(s) also. _(v2.A4)_

## RIP_EXTENDED_FONT_STYLE

_Added in RIPscrip v2.A1._

_Select current outline font style (True Type style)_

|  |  |
| --- | --- |
| **Level** | 0 |
| **Command** | `y` |
| **Arguments** | `direction:3 size:2 style:2 h_align:1 v_align:1 reserved:4 font_name_string` |

**Format:** `!|y <direction> <size> <style> <h_align> <v_align> <reserved> <font_name_string>` **Example:** `!|y0P01203000000courier`

**Attributes used:** Font Style, Base Math (current setting)

This command is an extended form of the [RIP_FONT_STYLE](#rip_font_style) command. The use of this command opens up a large number of other fonts that aren't like the older RIPscrip 1.x fonts. They are not vector based and they're not bitmapped based like those used with the RIP_FONT_STYLE command. The older fonts are very useful for many applications, but when it comes to production video graphics they leave a bit to be desired. This command utilizes True-Type (tm) style fonts which are "outline" fonts. By outline, we mean that they draw the outline of the font and fill-in the interior of the font. They also have a number of special attributes that you may assign to them (ie, Bold, Italic, Strikeout and Underline).

The addition of this extended font system fills a need in RIPscrip for solid fonts that are extensible (ie, specifying fonts not in the default system). Fonts are scalable to an arbitrary point size (unlike the fonts in RIP_FONT_STYLE which have pre-defined font sizes).

The commands [RIP_TEXT](10-level-0-commands-s-w.md#rip_text), [RIP_TEXT_XY](10-level-0-commands-s-w.md#rip_text_xy), [RIP_REGION_TEXT](11-level-1-commands.md#rip_region_text) and [RIP_BUTTON](11-level-1-commands.md#rip_button) use whichever font command is most recent (eg, RIP_FONT_STYLE or RIP_EXTENDED_FONT_STYLE). In other words, whichever font style that has been selected will be the one that is used for rendering the associated RIPscrip text related commands.

This command differs from the RIP_FONT_STYLE in several respects. One, it doesn't use built-in font sizes like the older format does (see RIP_FONT_STYLE); this command specifies fonts in a more universal format of Point Sizes. A "point" is technically defined as 1/72nd of an inch. Secondly, these fonts have the ability to alter the text "facing" which should be shown to the user (bold, italic, etc).

### Text Direction

This font system supports simple horizontal or vertical orientations for the text. The way you specify the font direction is different! The direction of the font is specified in tenths (1/10's) of degrees for future expadability. The possible values for the font `<direction>` are (in decimal, meganum, and ultranum): _(v2.A2)_

| Dec | Meg | Ult | Description                                          |
| --- | --- | --- | ---------------------------------------------------- |
| 000 | 000 | 000 | Horizontal text (left to right text)                 |
| 900 | 0P0 | 0E4 | Vertical text (bottom to top - rotated 90.0 degrees) |

The `<direction>` parameter may be enhanced in a future revision to support arbitrary rotation of the font (in tenths of a degree increments). For now, valid values for this parameter are 0.0 and 90.0 degrees. _(v2.A3)_

### Alignment

The `<h_align>` parameter determines the orientation of the displayed text around the specified text beginning coordinates. In other words, the text might begin at the coordinate and move left, right, or be centered around the location. The possible values for the `<h_align>` parameter are as follows:

| Value | Description of Horizontal Alignment                               |
| ----- | ----------------------------------------------------------------- |
| 0     | Align text to the left - X/Y location is the far left of the text |
| 1     | Right align text. The X/Y location is the far right of the text   |
| 2     | Center the text horizontally around the X/Y location              |

The `<v_align>` parameter is much like the `<h_align>` parameter in that it defines the orientation of the font in relation to the X/Y text coordinates. This command though performs vertical justification of the font instead of horizontal justification (like the `<h_align>` parameter). The possible values for this parameter are:

| Value | Description of Vertical Alignment |
| --- | --- |
| 0 | Align the text vertically with the X/Y location at the very top of the tallest character in the font. |
| 1 | Make the X/Y location of the text at the very bottom of the tallest character in the font. |
| 2 | Center the text vertically around the X/Y location (this takes descenders into consideration). |
| 3 | Align the text where the X/Y base point defines the location where the baseline of the font will appear. |

### Point Size and Resolution Independence

The `<size>` parameter defines the point size of the font to be displayed on the screen.

In order to maintain resolution independence with extended fonts and various resolutions, we need to formally define how a point size is rendered at different resolutions. For the purposes of RIPscrip font systems, we formally define a point size as one physical pixel on a 640x480 resolution device. Since a 640x480 device on a 13" monitor is exactly 1 point per pixel, this definition holds well. For other resolutions, we simply adjust the point size to some proportion. For example, a 1280x1024 resolution is exactly twice as wide as a 640x480 screen, but the height is 2.1333 times that of its 480 counterpart. So a little bit of adjustment must be made on the fonts to accomodate this precisely. The ideal method for this would be if you could apply certain scaling parameters to the font system in both the horizontal and vertical directions separately. Not all font systems will allow this though so a "closest approximation" might have to occur. In our 1280x1024 example, you might decide to just use twice the specified point size and leave it at that (and not worrying about the slight discrepency on the height portion of the font). When getting into resolution independence, it is not always possible to get 100.0% pixel perfect accuracy. If you can, wonderful, but it can't be expected 100.0% on all platforms.

For the standard RIPterm resolutions of 640x350, 640x480, 800x600, 1024x768 and 1280x1024, only two of the resolutions do not have perfectly square pixels - these are 640x350 and 1280x1024. These are not the only resolutions possible under RIPscrip - they're just the ones that TeleGrafix's RIP products utilize. The 640x350 resolution is like the 1280x1024 resolution in that the height of fonts will need to be adjusted to be pixel accurate with their 640x480 counterparts. If you cannot do this with your font system, then you might wish to reduce the point size a little bit so that the vertical component more closely matches the visual size of the font on a 640x480 screen (note that your horizontal size will shrink a little bit smaller than the 640x480 font - this is a compromise - the choice is your's).

### Style

The `<style>` parameter defines the style in which the font is to be rendered (eg, Normal, Bold, etc). The style options are organized as a set of flags which may be combined (OR'ed) together to produce a final, composite result:

| Style | Description        |
| ----- | ------------------ |
| 00    | Normal font sizing |
| 01    | Bold font          |
| 02    | Italic font        |
| 04    | Strike-out font    |
| 08    | Underlined font    |

### Font Names

The last, and possibly most important parameter in this command is the text parameter, `<font_name_string>`. This string of text defines the name of the font to activate. If a specified font is not recognized, then the default font in the RIP_FONT_STYLE is selected (with size of 1).

The possible (pre-defined) font names are:

| Name     | Description                            |
| -------- | -------------------------------------- |
| COURIER  | Courier mono-spaced font               |
| HELV     | Helvetica proportional font            |
| TIMESROM | Times Roman proportional font          |
| OLDENGL  | Old English (gothic) proportional font |
| SANSSERF | Sans Serif proportional font           |

These pre-defined fonts must be supported by all RIPscrip 2.0 and later software packages. A font name is up to eight characters in length. Other fonts may be specified by an application designer. In order for these fonts to be usable by the terminal system, they must already exist on the system and have some form of recognizable name associated with them. In RIPscrip 2.0, we define an extended font name as a name up to eight character in length. In order for the font to be recognized by the target terminal system it has to understand the same font name (presumably a font filename).

## RIP_EXTENDED_TEXT_WINDOW

_Added in RIPscrip v2.A4._

_Define a resolution independent text window_

|  |  |
| --- | --- |
| **Level** | 0 |
| **Command** | `b` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY width:2 height:2 font_no:1 flags:4 reserved:3` |

**Format:** `!|b <x0> <y0> <x1> <y0> <width> <height> <font_no> <flags> <reserved>` **Example:** `!|b0000ZKQO280P2000H000`

**Attributes used:** Base Math (current setting)

_[Editor's note: the ALPHA 4 draft assigns `b` to both this command and [RIP_SET_BASE_MATH](10-level-0-commands-s-w.md#rip_set_base_math). The shipping RIPscrip 3.0 driver kept `b` for the extended text window and moved RIP_SET_BASE_MATH to `J` — see the note on that entry.]_

This RIPscrip command is a more sophisticated way of defining a text window than by using the older [RIP_TEXT_WINDOW](10-level-0-commands-s-w.md#rip_text_window) command. The older command doesn't permit you the luxury of creating a truly resolution independent text window - this command does and is far superior in design to the older RIPscrip 1.xx RIP_TEXT_WINDOW command. If at all possible, you should use this command instead of the older command as it provides more flexibility and more methods of interfacing with the text window via text variable query statements.

This version of a text window works by establishing a bounding rectangle that is to contain the actual text window display region. This bounding rectangle is specified just like you would in drawing a filled rectangle, by indicating the upper-left X/Y coordinates and the lower-right X/Y coordinates of the bounding rectangle. Note that the lower-right corner of the bounding rectangle is non-inclusive to the bounding rectangle region, just like with filled rectangles. See the earlier section entitled ["THE MATHEMATICS OF GRAPHICS AND COORDINATES"](05-coordinates-and-math.md) for more detailed information on this subject and why it is important.

### Fitting the Text Window

Once a bounding rectangle is established, you indicate how many columns wide and how many lines tall you wish your text window to be, and also indicate the desired text font you wish to use for your window. The RIPscrip software will take your desired width, height and text font into consideration and determine if it can fit a text window inside your bounding rectangle with this font's dimensions. If it can make a text window exactly the width and height of your request, it does so - centering the text window inside the bounding rectangle as in the following diagram:

```text
╔═════════════════════════════════════════════════╗
║                                                 ║
║    ╔═══════════════════════════════╗            ║
║    ║                               ║            ║
║    ║    ╔╤╤╤╤╤╤╤╤╤╤╤╤╤╤╤╤╤╤╤╤╤╗    ║            ║
║    ║    ╟┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼╢    ║            ║
║    ║    ╟┼Text window display┼╢    ║            ║
║    ║    ╟┼┼┼┼┼┼rectangle┼┼┼┼┼┼╢    ║            ║
║    ║    ╟┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼╢    ║            ║
║    ║    ╟┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼╢    ║            ║
║    ║    ╚╧╧╧╧╧╧╧╧╧╧╧╧╧╧╧╧╧╧╧╧╧╝    ║            ║
║    ║                               ║            ║
║    ╚═══════════════════════════════╝            ║
║     Text window bounding rectangle              ║
║                                                 ║
╚═════════════════════════════════════════════════╝
                   Graphics Screen
```

If the font number provided would make either the width or height of the text window "too large" to fit inside the bounding rectangle, then several things may happen (in the following order):

1. The font number is checked. If a narrower or shorter font is available in the RIPscrip text window system, it will be chosen, then everything will be recalculated to see if the width and/or height can now be fit inside the bounding rectangle. If it can, then this new font will be used to accomodate the request. This evaluation process is incremental in nature. If the width and/or height is too large, then the next smaller font is checked, so on and so forth. Here is the table of fonts, their row/column sizes, and which font number will be checked if the width or the height (or both) is out of range for the given bounding rectangle:

   | Font | Columns | Rows | Shrink Width | Shrink Height | Shrink Both |
   | ---- | ------- | ---- | ------------ | ------------- | ----------- |
   | 0    | 80      | 43   | 1            | n/a           | n/a         |
   | 1    | 91      | 43   | n/a          | n/a           | n/a         |
   | 2    | 80      | 25   | 3            | 0             | 1           |
   | 3    | 91      | 25   | n/a          | 1             | n/a         |
   | 4    | 40      | 25   | 2            | 0             | 0           |

   If a new font must be chosen, then you evaluate whether you need to shrink the font in the width, height or in both directions. Consult the preceding table to determine which font number to check next. If the entry specifies "n/a", then you cannot shrink the font any further and must move on to step number 2 (see below).

2. If you have moved down to the smallest font and you still cannot obtain the desired height and width of your text window, you have no alternative then to reduce the width and/or height to make it fit inside the bounding rectangle. This is a last resort situation, but must be done if the text window is to fit inside the bounding rectangle. If the width is too wide, then one column will be trimmed off of the text window and then its new pixel width would be evaluated. If it still doesn't fit, you keep removing more columns, one-by-one, until you either get a window that fits, or you hit zero columns. If your bounding rectangle is so small that you can't have even a one column wide or one line tall text window then the text window is made exactly one line tall, or one column wide to accomodate the situation and the bounding box is adjusted accordingly to fit exactly the outer dimensions of the newly defined text window (albeit an incorrect one).

When the text window is actually defined, the cursor is placed in the upper-left corner (0,0) and any ANSI/VT-102 attributes are reset to normal defaults. The vertical scrolling margin for the text window defined in ANSI/VT-102 is set to the full height of the text window. The colors are set to light-gray text on a black background (low intensity white on black - standard ANSI colors).

### Flags

The `<flags>` parameter of this command allows you to specify some extra options to control how the text window is defined and how it will operate. The available flags, which must be OR'ed togther to create a final flags "value", are as follows:

| Flag | Description |
| --- | --- |
| 1 | Enable character-wrap. If this flag is not present, then "chop" mode is in use. See the RIP_TEXT_WINDOW command for more details on this mode of text window operation. |
| 2 | Disable the cursor immediately (as if a `$COFF$` text variable were processed). This hides the cursor immediately when the text window is created (see flag 16 for more on this). This doesn't mean that text won't be displayed, it only means that you won't see a cursor in the text window when it is current and active. |
| 4 | Deactivate the text window immediately upon creation. This acts as if immediately after defining the text window, a `$DTW$` text variable command were issued. Any raw text received by the terminal would not be displayed in this text window if it is the current one. When deactivated, the cursor for this text window is hidden and any text variables requesting information about this text window will return a result indicating that the text window is deactivated. Note, that the `$INUSE(TW)$` text variable will still respond that this text window is "in use", even though it is deactivated. |
| 8 | Ignore the font number parameter. This flag gives the text window definition software the task of finding the best font to use for the text window. This alleviates the coder from having to determine the proper font to use. This makes life a bit easier in choosing fonts. |
| 16 | Erase the text window immediately after it is created. This erases the contents of the entire bounding rectangle, not just the text window display area! The area is erased to color #0 (which is typically black). This operation is performed before the text window's cursor is displayed (if it is displayed at all based on flag 2), and before the text window can possibly be deactivated (via flag 4). |
| 32 | The text window is protected immediately after it is created. |

It should be noted that there are some fundamental differences in nature from the extended text window command and the older normal text window command. This one is obviously more powerful and has the benefit of being resolution independent. When working with [text variables](17-text-variables-general.md) to query the status or configuration of text windows, some text variables cannot query particular pieces of information about extended text windows (eg, `$TWX0$`, `$TWY0$`, `$TWX1$` and `$TWY1$`). The types of information that cannot be obtained are the text cell coordinates of the upper-left and lower-right corners of the extended text window - there aren't any - they're specified in graphical coordinates, not in text coordinates like with the RIP_TEXT_WINDOW command. See these text variables for more details about this kind of situation.

> **NOTE:** A text window definition command is ignored if the current text window data table entry is protected!
>
> The `<reserved>` parameter is reserved for future use in RIPscrip and should be set to "000" for compatibility with future releases of RIPscrip.

## RIP_FILL

_This command is no longer supported._

_Flood fill screen area with current fill settings_

|             |     |
| ----------- | --- |
| **Level**   | 0   |
| **Command** | `F` |

This command has been removed from the RIPscrip language. Due to the numerous issues trying to make it work reliably at all resolutions, it was decided that this command could not be implemented without compromising the integrity of the scene. Also, fills work differently under Windows, OS/2, Macintosh, Amiga, and DOS. With all of the complexities involved, having a reliable fill operation cannot be achieved under multiple resolutions and platforms. _(v2.A2)_

There are a number of alternatives to using a flood fill. In fact, a large number of screens could have been drawn much more efficiently using the filled object commands listed below, than using a fill. _(v2.A2)_

| Basic Drawing Objects | Filled Equivalent: |
| --- | --- |
| [RIP_PIXEL](09-level-0-commands-g-r.md#rip_pixel) | N/A |
| [RIP_LINE](09-level-0-commands-g-r.md#rip_line) | N/A |
| [RIP_RECTANGLE](09-level-0-commands-g-r.md#rip_rectangle) | [RIP_FILLED_RECTANGLE](#rip_filled_rectangle) |
| N/A | [RIP_BAR](#rip_bar) |
| [RIP_CIRCLE](#rip_circle) | [RIP_FILLED_CIRCLE](#rip_filled_circle) |
| [RIP_OVAL](09-level-0-commands-g-r.md#rip_oval) | [RIP_FILLED_OVAL](#rip_filled_oval) |
| [RIP_ARC](#rip_arc) | [RIP_PIE_SLICE](09-level-0-commands-g-r.md#rip_pie_slice) |
| [RIP_OVAL_ARC](09-level-0-commands-g-r.md#rip_oval_arc) | [RIP_OVAL_PIE_SLICE](09-level-0-commands-g-r.md#rip_oval_pie_slice) |
| [RIP_POLYGON](09-level-0-commands-g-r.md#rip_polygon) | [RIP_FILLED_POLYGON](#rip_filled_polygon) |
| [RIP_POLYLINE](09-level-0-commands-g-r.md#rip_polyline) | N/A |
| [RIP_BEZIER](#rip_bezier) | N/A |
| [RIP_POLY_BEZIER](09-level-0-commands-g-r.md#rip_poly_bezier) | [RIP_FILLED_POLY_BEZIER](#rip_filled_poly_bezier) |
| [RIP_POLY_BEZIER_LINE](09-level-0-commands-g-r.md#rip_poly_bezier_line) | N/A |
| [RIP_ROUNDED_RECT](09-level-0-commands-g-r.md#rip_rounded_rect) | [RIP_FILLED_ROUNDED_RECT](#rip_filled_rounded_rect) |

## RIP_FILL_PATTERN

_Set user-definable (custom) fill pattern/color_

|               |                                                  |
| ------------- | ------------------------------------------------ |
| **Level**     | 0                                                |
| **Command**   | `s`                                              |
| **Arguments** | `c1:2 c2:2 c3:2 c4:2 c5:2 c6:2 c7:2 c8:2 col:CM` |

**Format:** `!|s <c1> <c2> <c3> <c4> <c5> <c6> <c7> <c8> <col>` **Example:** `!|s11223344556677880F`

**Attributes used:** Fill Style, Base Math (current setting)

This command allows you to specify a user-defined, custom Fill Pattern. This pattern supersedes the predefined patterns of [RIP_FILL_STYLE](#rip_fill_style). A custom fill pattern is an 8x8 pixel array defining which pixels should be drawn in the current fill color (as set by the `<col>` parameter here). The other pixels in the fill area are set to the current pen background color.

Each of the eight parameters of this command, `<c1>` through `<c8>` represent bit-patterns for a line of the 8x8 pixel array. Each line is comprised of 8 pixels. The value of each parameter is the binary representation of these 8 pixels as follows:

| Bit | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| c1  | 128 | 64  | 32  | 16  | 8   | 4   | 2   | 1   |
| c2  | 128 | 64  | 32  | 16  | 8   | 4   | 2   | 1   |
| c3  | 128 | 64  | 32  | 16  | 8   | 4   | 2   | 1   |
| c4  | 128 | 64  | 32  | 16  | 8   | 4   | 2   | 1   |
| c5  | 128 | 64  | 32  | 16  | 8   | 4   | 2   | 1   |
| c6  | 128 | 64  | 32  | 16  | 8   | 4   | 2   | 1   |
| c7  | 128 | 64  | 32  | 16  | 8   | 4   | 2   | 1   |
| c8  | 128 | 64  | 32  | 16  | 8   | 4   | 2   | 1   |

So, c1 is the top, and the most-significant bit is to the left.

The `<col>` parameter of this command serves two purposes depending on what Color Mode is currently set. The default mode, Color Map mode, signifies that the color parameter of this command will use the parameter as an index into the 256 color Drawing Palette color mapping table. In this mode, this parameter will use the current Base Math set by the global [RIP_SET_BASE_MATH](10-level-0-commands-s-w.md#rip_set_base_math) command. _(v2.A0)_

In Direct RGB Color Mode, the color parameter will be a four-digit UltraNum (regardless of the current Base Math setting). This numeric value will be an encoded RGB value, allowing you to set arbitrary RGB colors directly with the Set Color commands. If the RGB color is not permissable, the closest match from the Color Map table will be located and used. The number of bits of precision used in the encoded RGB value will be determined by the [RIP_SET_COLOR_MODE](10-level-0-commands-s-w.md#rip_set_color_mode) command. _(v2.A0)_

When Color Mapping Mode is in use, the color parameter may be shorter than four digits. This is for space savings, as it is allowable in the specification for the last numeric parameter in a parameter list to be "shorter" than the full length. _(v2.A0)_

This command does not affect whether borders are enabled or disabled. See [RIP_SET_BORDER](10-level-0-commands-s-w.md#rip_set_border) command for details on how to alter the status of borders. _(v2.A3)_

> **NOTE:** The RIP_FILL_STYLE (predefined fill patterns) and this RIP_FILL_PATTERN (custom fill patterns) completely override each other's effects.

## RIP_FILL_STYLE

_Set current fill style (predefined) & fill color_

|               |                      |
| ------------- | -------------------- |
| **Level**     | 0                    |
| **Command**   | `S`                  |
| **Arguments** | `pattern:2 color:CM` |

**Format:** `!|S <pattern> <color>` **Example:** `!|S05000F`

**Attributes used:** Fill Style, Base Math (current setting)

This command defines the current fill pattern and fill color for use in subsequent graphics fill operations. There are twelve (12) predefined fill patterns. They are:

| Pattern | Description                | Example       | Misc          |
| ------- | -------------------------- | ------------- | ------------- |
| 00      | Fill with background color |               | (color #0)    |
| 01      | Solid Fill                 | `███████████` | (fill color)  |
| 02      | Line Fill                  | `-----------` | (thick lines) |
| 03      | Light Slash Fill           | `/  /  /  /`  | (thin lines)  |
| 04      | Normal Slash Fill          | `// // // //` | (thick lines) |
| 05      | Normal Backslash Fill      | `\\ \\ \\ \\` | (thick lines) |
| 06      | Light Backslash Fill       | `\  \  \  \`  | (thin lines)  |
| 07      | Light Hatch Fill           | `###########` | (thin lines)  |
| 08      | Heavy Cross Hatch Fill     | `XXXXXXXXXXX` | (thin lines)  |
| 09      | Interleaving Line Fill     | `+-+-+-+-+-+` | (thin lines)  |
| 0A      | Widely spaced dot fill     | `. : . : . :` | (pixels only) |
| 0B      | Closely spaced dot fill    | `:::::::::::` | (pixels only) |

The `<color>` parameter is the fill color for subsequent fill commands. The "active" pixels of the pattern become this color. The "inactive" pixels become the current pen background color. Fill pattern 00 will set the entire fill area to the background color. (In this case, the fill color doesn't matter).

The `<color>` parameter of this command serves two purposes depending on what Color Mode is currently set. The default mode, Color Map mode, signifies that the color parameter of this command will use the parameter as an index into the 256 color Drawing Palette color mapping table. In this mode, this parameter will use the current Base Math set by the global [RIP_SET_BASE_MATH](10-level-0-commands-s-w.md#rip_set_base_math) command. _(v2.A0)_

In Direct RGB Color Mode, the color parameter will be a four-digit UltraNum (regardless of the current Base Math setting). This numeric value will be an encoded RGB value, allowing you to set arbitrary RGB colors directly with the Fill Style commands. If the RGB color is not permissable, the closest match from the Color Map table will be located and used. The number of bits of precision used in the encoded RGB value will be determined by the [RIP_SET_COLOR_MODE](10-level-0-commands-s-w.md#rip_set_color_mode) command. _(v2.A0)_

When Color Mapping Mode is in use, the color parameter may be shorter than four digits. This is for space savings, as it is allowable in the specification for the last numeric parameter in a parameter list to be "shorter" than the full length. _(v2.A0)_

This command does not affect whether borders are enabled or disabled. See [RIP_SET_BORDER](10-level-0-commands-s-w.md#rip_set_border) command for details on how to alter the status of borders. _(v2.A3)_

### Fill Pattern Bitmaps

The following twelve diagrams show visually what each fill pattern appears like. Next to each diagram are the eight numerical values which represent the monochrome bit-pattern of each line of each pattern. Numbers are shown in Hexadecimal (base 16), decimal (base 10) and MegaNum (base 36): _(v1.54)_

```text
   BACKGROUND FILL                      SOLID FILL

╔════════╗  HEX DEC MEGA ULTRA   ╔════════╗  HEX DEC MEGA ULTRA
║        ║  00    0  00   00     ║████████║  FF  255  73   3@
║        ║  00    0  00   00     ║████████║  FF  255  73   3@
║        ║  00    0  00   00     ║████████║  FF  255  73   3@
║        ║  00    0  00   00     ║████████║  FF  255  73   3@
║        ║  00    0  00   00     ║████████║  FF  255  73   3@
║        ║  00    0  00   00     ║████████║  FF  255  73   3@
║        ║  00    0  00   00     ║████████║  FF  255  73   3@
║        ║  00    0  00   00     ║████████║  FF  255  73   3@
╚════════╝                       ╚════════╝
    00                               01


       LINE FILL                     LIGHT SLASH FILL

╔════════╗  HEX DEC MEGA ULTRA   ╔════════╗  HEX DEC MEGA ULTRA
║████████║  FF  255  73   3@     ║       █║  01    1  01   01
║████████║  FF  255  73   3@     ║      █ ║  02    2  02   02
║        ║  00    0  00   00     ║     █  ║  04    4  04   04
║        ║  00    0  00   00     ║    █   ║  08    8  08   08
║████████║  FF  255  73   3@     ║   █    ║  10   16  0G   0G
║████████║  FF  255  73   3@     ║  █     ║  20   32  0W   0W
║        ║  00    0  00   00     ║ █      ║  40   64  1S   10
║        ║  00    0  00   00     ║█       ║  80  128  3K   20
╚════════╝                       ╚════════╝
    02                               03


    NORMAL SLASH FILL               LIGHT BACKSLASH FILL

╔════════╗  HEX DEC MEGA ULTRA   ╔════════╗  HEX DEC MEGA ULTRA
║███     ║  E0  224  68   3W     ║████    ║  F0  240  60   3m
║██     █║  C1  193  5D   31     ║ ████   ║  78  120  3C   1u
║█     ██║  83  131  3N   23     ║  ████  ║  3C   60  1O   0y
║     ███║  07    7  07   07     ║   ████ ║  1E   30  0U   0U
║    ███ ║  0E   15  0F   0F     ║    ████║  0F   15  0F   0F
║   ███  ║  1C   28  0S   0S     ║█    ███║  87  135  3R   27
║  ███   ║  38   56  1K   0u     ║██    ██║  C3  195  5F   33
║ ███    ║  70  112  34   1m     ║███    █║  E1  225  69   3X
╚════════╝                       ╚════════╝
    04                               05


  LIGHT BACKSLASH FILL               LIGHT HATCH FILL

╔════════╗  HEX DEC MEGA ULTRA   ╔════════╗  HEX DEC MEGA ULTRA
║█ █  █ █║  A5  165  4L   2b     ║████████║  FF  255  73   3@
║██ █  █ ║  D2  210  5U   3I     ║█   █   ║  88  136  3S   28
║ ██ █  █║  69  105  2X   1f     ║█   █   ║  88  136  3S   28
║█ ██ █  ║  B4  180  50   2q     ║█   █   ║  88  136  3S   28
║ █ ██ █ ║  5A   90  2I   1Q     ║████████║  FF  255  73   32
║  █ ██ █║  2D   45  19   0j     ║█   █   ║  88  136  3S   28
║█  █ ██ ║  96  150  46   2M     ║█   █   ║  88  136  3S   28
║ █  █ ██║  4B   75  23   1B     ║█   █   ║  88  136  3S   28
╚════════╝                       ╚════════╝
    06                               07


 HEAVY CROSS HATCH FILL           INTERLEAVING LINE FILL

╔════════╗  HEX DEC MEGA ULTRA   ╔════════╗  HEX DEC MEGA ULTRA
║█      █║  81  129  3L   21     ║██  ██  ║  CC  204  5O   3C
║ █    █ ║  42   66  1U   12     ║  ██  ██║  33   51  1F   0p
║  █  █  ║  24   36  10   0a     ║██  ██  ║  CC  204  5O   3C
║   ██   ║  18   24  0O   0O     ║  ██  ██║  33   51  1F   0p
║   ██   ║  18   24  0O   0O     ║██  ██  ║  CC  204  5O   3C
║  █  █  ║  24   36  10   0a     ║  ██  ██║  33   51  1F   0p
║ █    █ ║  42   66  1U   12     ║██  ██  ║  CC  204  5O   3C
║█      █║  81  129  3L   21     ║  ██  ██║  33   51  1F   0p
╚════════╝                       ╚════════╝
    08                               09


 WIDELY SPACED DOT FILL           CLOSELY SPACED DOT FILL

╔════════╗  HEX DEC MEGA ULTRA   ╔════════╗  HEX DEC MEGA ULTRA
║█       ║  80  128  3K   20     ║█   █   ║  88  136  3S   28
║        ║  00    0  00   00     ║        ║  00    0  00   00
║    █   ║  08    8  08   08     ║  █   █ ║  22   34  0Y   04
║        ║  00    0  00   00     ║        ║  00    0  00   00
║█       ║  80  128  3K   20     ║█   █   ║  88  136  3S   28
║        ║  00    0  00   00     ║        ║  00    0  00   00
║    █   ║  08    8  08   08     ║  █   █ ║  22   34  0Y   24
║        ║  00    0  00   00     ║        ║  00    0  00   00
╚════════╝                       ╚════════╝
    0A                               0B
```

## RIP_FILLED_CIRCLE

_Added in RIPscrip v2.A2._

_Draw a filled circle in current color/line style_

|               |                                     |
| ------------- | ----------------------------------- |
| **Level**     | 0                                   |
| **Command**   | `G`                                 |
| **Arguments** | `x_center:XY y_center:XY radius:XY` |

**Format:** `!|G <x_center> <y_center> <radius>` **Example:** `!|G1E180M`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws a complete filled circle on the screen. The interior of the circle is drawn using the current fill pattern and fill color. The outline of the circle is drawn using the current drawing color and line thickness (if borders are enabled).

The radius is considered to be in the horizontal direction for the purpose of aspect ratio calculations. _(v2.A3)_

When borders are disabled, this command adheres to the resolution independent method of filled regions. _(v2.A3)_

## RIP_FILLED_OVAL

_Draw filled ellipse using current color/pattern_

|               |                                             |
| ------------- | ------------------------------------------- |
| **Level**     | 0                                           |
| **Command**   | `o`                                         |
| **Arguments** | `x_center:XY y_center:XY x_rad:XY y_rad:XY` |

**Format:** `!|o <x_center> <y_center> <x_rad> <y_rad>` **Example:** `!|o1G2B0M0G`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws a complete filled ellipse on the screen. The interior of the ellipse is drawn using the current fill pattern and fill color. The outline of the ellipse is drawn using the current drawing color and line thickness.

When borders are disabled, this command adheres to the resolution independent method of filled regions. _(v2.A3)_

## RIP_FILLED_POLYGON

_Formerly RIP_FILL_POLYGON._

_Draw filled polygon in current color/fill pattern_

|               |                                         |
| ------------- | --------------------------------------- |
| **Level**     | 0                                       |
| **Command**   | `p`                                     |
| **Arguments** | `npoints:2 x1:XY y1:XY ... xn:XY yn:XY` |

**Format:** `!|p <npoints> <x1> <y1> ... <xn> <yn>` **Example:** `!|p03010105050909`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)

This command is identical to [RIP_POLYGON](09-level-0-commands-g-r.md#rip_polygon), except that the interior of the polygon is filled with the current fill color and fill pattern. The actual outline of the polygon is drawn using the current drawing color, line pattern and thickness.

> **NOTE:** You will get unusual effects if the lines of the polygon overlap, creating a polygon with internal "gaps". (The rule is this: regions that are "inside" the polygon an even number of times due to overlap are NOT filled.) The interior fill does utilize Write Mode, but the outline of the polygon does not.

## RIP_FILLED_POLY_BEZIER

_Added in RIPscrip v2.A2._

_Draw a poly-bezier curve (multi-segmented)_

|  |  |
| --- | --- |
| **Level** | 0 |
| **Command** | `x` |
| **Arguments** | `num:2 count:2 x_base:XY y_base:XY ... type:1 x1:XY y1:XY x2:XY y2:XY x3:XY y3:XY ... type:1 x1:XY y1:XY ...` |

**Format:** `!|z <num> <count> <x_base> <y_base> ... <type> <x1> <y1> <x2> <y2> <x3> <y3> ... <type> <x1> <y1> ...` **Example:** `!|z`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)

This command is identical to [RIP_POLY_BEZIER](09-level-0-commands-g-r.md#rip_poly_bezier), except that the interior of the poly-bezier is filled with the current fill color and fill pattern. The actual outline of the poly-bezier is drawn using the current drawing color, line pattern and thickness.

> **NOTE:** You will get unusual effects if the lines of the poly-bezier overlap, creating a poly-bezier with internal "gaps". (The rule is this: regions that are "inside" the poly-bezier an even number of times due to overlap are NOT filled.) The interior fill does utilize Write Mode, but the outline of the the poly-bezier does not.

When borders are disabled, then the filled interior of the poly-bezier curve adheres to the resolution independent nature of filled regions. _(v2.A3)_

## RIP_FILLED_RECTANGLE

_Added in RIPscrip v2.A2._

_Draw filled rectangle with fill style/line style_

|               |                           |
| ------------- | ------------------------- |
| **Level**     | 0                         |
| **Command**   | `K`                       |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|K <x0> <y0> <x1> <y1>` **Example:** `!|K00010A0E`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws a filled rectangle in the current drawing color, using the current line style, pattern and thickness, and the current fill style. (x0,y0) and (x1,y1) are any two opposing corners of the rectangle. If x0=x1 or y0=y1 then the command will draw a single vertical or horizontal line (or possibly a single pixel) (if borders are off, then this command draws nothing in this situation).

This command operates similarly to the [RIP_BAR](#rip_bar) command when borders are disabled, however the lower-left and lower-right edges of the filled rectangle obey the resolution independent method of performing fills (see the section called ["THE MATHEMATICS OF GRAPHICS AND COORDINATES"](05-coordinates-and-math.md) for details on how this works. _(v2.A3)_

Before the rectangle is actually drawn, the (x0,y0) and (x1,y1) coordinates are "normalized". This means that they are adjusted so that they specify the upper-left and lower-right coordinates respectively. For example, if the parameters are received in the order of (50,25) and (25,75) - upper-right and lower-left respectively - they would be reorganized to be (25,25) and (50,75) before the rectangle is actually drawn. _(v2.A3)_

## RIP_FILLED_ROUNDED_RECT

_Added in RIPscrip v2.A3._

_Draw a filled rectangle with rounded corners_

|               |                                 |
| ------------- | ------------------------------- |
| **Level**     | 0                               |
| **Command**   | `u`                             |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY rad:2` |

**Example:** `!|u00010A0E09`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws a rounded corner rectangle which is filled in with the current fill pattern and fill color. The corners of the rectangle are not drawn at right angles like normal rectangles. The rounded rectangle has circular arcs drawn at all four corners of the drawn object. The radius of the arc that is used to fill-in the corners is specified in the `<rad>` parameter. The `<x0,y0>` and `<x1,y1>` parameters define the upper left and lower right corners of the rectangle as if the corners were actually specified as a normal rectangle. The circular arcs drawn in the corners of the rounded rectangle are truly circular in nature and adhere to aspect ratios relating to the actual video configuration of the destination application program.

If borders are enabled, then the outer portion of the rounded rectangle are drawn in the current line style, foreground pen color and line style. The border is drawn in "copy" mode - the border of this command (if any) is never drawn in any operation other than the COPY write mode.

## RIP_FONT_STYLE

_Select current vector/bitmap font style_

|               |                                     |
| ------------- | ----------------------------------- |
| **Level**     | 0                                   |
| **Command**   | `Y`                                 |
| **Arguments** | `font:2 direction:2 size:2 flags:2` |

**Format:** `!|Y <font> <direction> <size> <flags>` **Example:** `!|Y01000400`

**Attributes used:** Font Style, Base Math (current setting)

This command sets the font, direction and size for [RIP_TEXT](10-level-0-commands-s-w.md#rip_text) commands.

### Fonts

| Font | Description of Font       |            |
| ---- | ------------------------- | ---------- |
| 00   | Default 8x8 (*) font      | Bit-Mapped |
| 01   | Triplex Font              | Scalable   |
| 02   | Small Font                | Scalable   |
| 03   | Sans Serif Font           | Scalable   |
| 04   | Gothic [Old English] Font | Scalable   |
| 05   | Script Font               | Scalable   |
| 06   | Simplex Font              | Scalable   |
| 07   | Triplex Script Font       | Scalable   |
| 08   | Complex Font              | Scalable   |
| 09   | European Font             | Scalable   |
| 0A   | Bold Font                 | Scalable   |

(*) Font 00 is an 8x8 font at 640x350. Its actual cell-size varies depending on the current resolution. This is equivalent to the 80x43 font of the text window. See the [RIP_TEXT_WINDOW](10-level-0-commands-s-w.md#rip_text_window) for a more detailed discussion of font sizes and resolutions. _(v2.A3)_

### Direction and Size

For the Direction parameter, use 00 to indicate horizontal and 01 for vertical. Horizontal text appears from left to right. Vertical text is drawn with the base-line to the right, and is read from bottom to the top.

For the Size parameter, use 01 for the normal default size, 02 for level 2 magnification, 03 for level 3 magnification, and 0A for level 10 magnification. For the bitmapped font (00), these magnification factors are actual scaling factors - size 2 is twice as large as size 1, size 3 is three times as large, etc. The vector fonts (01-0A) are not direct magnification factors - they are based on the following table:

| Mag Factor | Actual Scaling Factor |
| ---------- | --------------------- |
| 01         | 3/5 of actual size    |
| 02         | 2/3 of actual size    |
| 03         | 3/4 of actual size    |
| 04         | 1/1 of actual size    |
| 05         | 4/3 of actual size    |
| 06         | 5/3 of actual size    |
| 07         | 2/1 of actual size    |
| 08         | 5/2 of actual size    |
| 09         | 3/1 of actual size    |
| 0A (10)    | 4/1 of actual size    |

The physical pixel dimensions of any particular font cell at a given magnification level vary depending on resolution in such a way that they appear to be the same size on the screen. For example, if a font at magnification level 4 is 35 pixels tall at 640x350 resolution, at 640x480 resolution, the font would be the same width, but would be 48 pixels tall. This makes sure that the fonts are resolution independent. _(v2.A3)_

### Justification and Special Effect Flags

The `<flags>` parameter allows you to alter the orientation and other special attributes of the subsequent text operations. The possible flag values which may be combined are as follows (they must be OR'd together): _(v2.A4)_

| Value | Description of Flag |
| --- | --- |
| 1 | Right justify text in relation to the base point |
| 2 | Center justify text horizontally around base point |
| 4 | Bottom justify the font (at the bottom of any descenders. |
| 8 | Baseline justify the font (base point is at the base line of the font). |
| 16 | Vertically center the text (base point is vertically in the center of the font cell). |
| 32 | Dropshadow the text using the current background drawing color (set with [RIP_BACK_COLOR](#rip_back_color)). The foreground text is drawn using the current foreground drawing color (set with [RIP_COLOR](#rip_color)). The shadow is drawn one pixel down and one pixel to the right of the font for both vertical and horizontal text. |

Flag values 1 and 2 are mutually exclusive. This means that either one of them may be specified, but not both! If both are omitted, then the text is "left justified". Flags 4, 8 and 16 are also mutually exclusive. If either of them are omitted, then the text is drawn with "top" justification. If both flags 1 and 2 are specified, or more than one of the flags 4, 8 and 16 are specified then the command is considered an error and is completely ignored. _(v2.A4)_

Each form of justification uses the base point as a starting point. Where the text is drawn in relation to this base point is a matter of the justification. Here is an example of horizontal text with all of the base point justifcation references pointed out: _(v2.A4)_

```text
             Center
   Left ──┐    │             ┌── Right
          ▼    ▼             ▼
         ┌──────────────┬──────────────┐
  Top ──▶│██████████    │              │
         │██        ██  │              │
         │██        ██  │              │
         │██████████    │██        ██  │
Center ──▶│██        ██  │██        ██  │
         │██        ██  │██        ██  │
  Base ──▶│██████████    │  ██████████  │
         │              │          ██  │
Bottom ──▶│              │  ██████████  │
         └──────────────┴──────────────┘
```

### Horizontal Text Justification Examples

The significance of each justification can be thought of in the following diagrams of horizontal text: _(v2.A4)_

```text
┌──────────────┬──────────────┐    ┌──────────────┬──────────────┐
│░░████████    │              │    │██████████    │            ░░│
│██        ██  │              │    │██        ██  │              │
│██        ██  │              │    │██        ██  │              │
│██████████    │██        ██  │    │██████████    │██        ██  │
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│██████████    │  ██████████  │    │██████████    │  ██████████  │
│              │          ██  │    │              │          ██  │
│              │  ██████████  │    │              │  ██████████  │
└──────────────┴──────────────┘    └──────────────┴──────────────┘
       Top Justified                      Top Justified
       Left Justified                    Right Justified


┌──────────────┬──────────────┐    ┌──────────────┬──────────────┐
│██████████    │              │    │██████████    │              │
│██        ██  │              │    │██        ██  │              │
│██        ██  │              │    │██        ██  │              │
│██████████    │██        ██  │    │██████████    │██        ██  │
│░░        ██  │██        ██  │    │██        ██  │██        ██░░│
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│██████████    │  ██████████  │    │██████████    │  ██████████  │
│              │          ██  │    │              │          ██  │
│              │  ██████████  │    │              │  ██████████  │
└──────────────┴──────────────┘    └──────────────┴──────────────┘
    Vertically Centered                Vertically Centered
       Left Justified                    Right Justified


┌──────────────┬──────────────┐    ┌──────────────┬──────────────┐
│██████████    │              │    │██████████    │              │
│██        ██  │              │    │██        ██  │              │
│██        ██  │              │    │██        ██  │              │
│██████████    │██        ██  │    │██████████    │██        ██  │
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│░░████████    │  ██████████  │    │██████████    │  ██████████░░│
│              │          ██  │    │              │          ██  │
│              │  ██████████  │    │              │  ██████████  │
└──────────────┴──────────────┘    └──────────────┴──────────────┘
    Base Line Justified                Base Line Justified
       Left Justified                    Right Justified


┌──────────────┬──────────────┐    ┌──────────────┬──────────────┐
│██████████    │              │    │██████████    │              │
│██        ██  │              │    │██        ██  │              │
│██        ██  │              │    │██        ██  │              │
│██████████    │██        ██  │    │██████████    │██        ██  │
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│██████████    │  ██████████  │    │██████████    │  ██████████  │
│              │          ██  │    │              │          ██  │
│░░            │  ██████████  │    │              │  ██████████░░│
└──────────────┴──────────────┘    └──────────────┴──────────────┘
      Bottom Justified                  Bottom Justified
       Left Justified                    Right Justified


┌──────────────┬──────────────┐    ┌──────────────┬──────────────┐
│██████████  ░░│              │    │██████████    │              │
│██        ██  │              │    │██        ██  │              │
│██        ██  │              │    │██        ██  │              │
│██████████    │██        ██  │    │██████████    │██        ██  │
│██        ██  │██        ██  │    │██        ██░░│██        ██  │
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│██████████    │  ██████████  │    │██████████    │  ██████████  │
│              │          ██  │    │              │          ██  │
│              │  ██████████  │    │              │  ██████████  │
└──────────────┴──────────────┘    └──────────────┴──────────────┘
        Top Justified                     Vertically Centered
    Horizontally Centered                Horizontally Centered


┌──────────────┬──────────────┐    ┌──────────────┬──────────────┐
│██████████    │              │    │██████████    │              │
│██        ██  │              │    │██        ██  │              │
│██        ██  │              │    │██        ██  │              │
│██████████    │██        ██  │    │██████████    │██        ██  │
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│██        ██  │██        ██  │    │██        ██  │██        ██  │
│██████████  ░░│  ██████████  │    │██████████    │  ██████████  │
│              │          ██  │    │              │          ██  │
│              │  ██████████  │    │            ░░│  ██████████  │
└──────────────┴──────────────┘    └──────────────┴──────────────┘
      Base Line Justify                  Bottom Justification
    Horizontally Centered                Horizontally Centered
```

### Vertical Text Justification Examples

The significance of each justification can be thought of in the following diagrams of vertical text: _(v2.A4)_

```text
┌──────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│                  │    │                  │    │░░                │
│      ██████████  │    │      ██████████  │    │      ██████████  │
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│      ██████      │    │      ██████      │    │      ██████      │
├──────────────────┤    ├──────────────────┤    ├──────────────────┤
│                  │    │░░                │    │                  │
│  ████  ████      │    │  ████  ████      │    │  ████  ████      │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│░░████████████    │    │██████████████    │    │██████████████    │
└──────────────────┘    └──────────────────┘    └──────────────────┘
   Left Justified       Horizontally Centered      Right Justified
    Top Justified           Top Justified           Top Justified


┌──────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│                  │    │                  │    │      ░░          │
│      ██████████  │    │      ██████████  │    │      ██████████  │
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│      ██████      │    │      ██████      │    │      ██████      │
├──────────────────┤    ├──────────────────┤    ├──────────────────┤
│                  │    │        ░░        │    │                  │
│  ████  ████      │    │  ████  ████      │    │  ████  ████      │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│████████░░████    │    │████████████░░    │    │██████████████    │
└──────────────────┘    └──────────────────┘    └──────────────────┘
   Left Justified       Horizontally Centered      Right Justified
 Vertically Centered     Vertically Centered     Vertically Centered


┌──────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│                  │    │                  │    │            ░░    │
│      ██████████  │    │      ██████████  │    │      ██████████  │
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│      ██████      │    │      ██████      │    │      ██████      │
├──────────────────┤    ├──────────────────┤    ├──────────────────┤
│                  │    │            ░░    │    │                  │
│  ████  ████      │    │  ████  ████      │    │  ████  ████      │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│████████████░░    │    │██████████████    │    │██████████████    │
└──────────────────┘    └──────────────────┘    └──────────────────┘
   Left Justified       Horizontally Centered      Right Justified
 Base Line Justify        Base Line Justify      Base Line Justified


┌──────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│                  │    │                  │    │                ░░│
│      ██████████  │    │      ██████████  │    │      ██████████  │
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│            ██  ██│    │            ██  ██│    │            ██  ██│
│      ██████      │    │      ██████      │    │      ██████      │
├──────────────────┤    ├──────────────────┤    ├──────────────────┤
│                  │    │                ░░│    │                  │
│  ████  ████      │    │  ████  ████      │    │  ████  ████      │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██    ██    ██    │    │██    ██    ██    │    │██    ██    ██    │
│██████████████  ░░│    │██████████████    │    │██████████████    │
└──────────────────┘    └──────────────────┘    └──────────────────┘
   Left Justified       Horizontally Centered      Right Justified
  Bottom Justified        Bottom Justified        Bottom Justified
```

### Dropshadowing

Here is how a font would look if it had dropshadowing enabled. The solid squares (█) are the foreground text drawn with the color set by the RIP_COLOR command, and the shaded squares (░) are the pixels drawn with the background drawing color set by the RIP_BACK_COLOR command. Note that for dropshadowed text, the dropshadow is drawn first, then the foreground text is drawn after the dropshadow is drawn. _(v2.A4)_

```text
██████████                ████  ████
██░░░░░░░░██            ██  ░░██  ░░██
██░░      ██░░          ██░░  ██░░  ██░░
██████████  ░░          ██░░  ██░░  ██░░
██░░░░░░░░██            ██░░  ██░░  ██░░
██░░      ██░░          ██░░  ██░░  ██░░
██████████  ░░          ██████████████░░
  ░░░░░░░░░░              ░░░░░░░░░░░░░░

Horizontal text          Vertical text
(with dropshadow)       (with dropshadow)
```

> **NOTE:** The Default font is bit-mapped and looks best when drawn in size 1. In sizes greater than one, the individual pixels are enlarged, giving a jagged look. This may not be the desired effect. The fonts 1 - A are smooth scalable vector fonts.

---

[◀ Prev: Protocol Definition & Syntax](07-protocol-definition.md) • [Contents](README.md) • [Next: Level-0 Commands (G–R) ▶](09-level-0-commands-g-r.md)
