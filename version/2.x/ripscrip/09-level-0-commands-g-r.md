# Level-0 Commands (G–R)

[◀ Prev: Level-0 Commands (A–F)](08-level-0-commands-a-f.md) · [Contents](README.md) · [Next: Level-0 Commands (S–W) ▶](10-level-0-commands-s-w.md)

This page covers the Level-0 commands [RIP_GOTOXY](#rip_gotoxy), [RIP_GROUP_BEGIN](#rip_group_begin), [RIP_GROUP_END](#rip_group_end), [RIP_HEADER](#rip_header), [RIP_HOME](#rip_home), [RIP_LINE](#rip_line), [RIP_LINE_STYLE](#rip_line_style), [RIP_MOVE](#rip_move), [RIP_NO_MORE](#rip_no_more), [RIP_ONE_DRAWING_PALETTE](#rip_one_drawing_palette), [RIP_ONE_PALETTE](#rip_one_palette), [RIP_OVAL](#rip_oval), [RIP_OVAL_ARC](#rip_oval_arc), [RIP_OVAL_PIE_SLICE](#rip_oval_pie_slice), [RIP_PIE_SLICE](#rip_pie_slice), [RIP_PIXEL](#rip_pixel), [RIP_POLYGON](#rip_polygon), [RIP_POLYLINE](#rip_polyline), [RIP_POLY_BEZIER](#rip_poly_bezier), [RIP_POLY_BEZIER_LINE](#rip_poly_bezier_line), [RIP_RECTANGLE](#rip_rectangle), [RIP_RESET_WINDOWS](#rip_reset_windows), and [RIP_ROUNDED_RECT](#rip_rounded_rect).

## RIP_GOTOXY

_Move text cursor to row & column in Text Window_

|               |           |
| ------------- | --------- |
| **Level**     | 0         |
| **Command**   | `g`       |
| **Arguments** | `x:2 y:2` |

**Format:** `!|g <x> <y>` **Example:** `!|g0509`

**Attributes used:** Port, Base Math (current setting)

This command sets the position of the text cursor in the TTY Text window, if it is active. If inactive (if the dimensions are 0,0,0,0), then this command is ignored. This command is equivalent to the ANSI/VT-100 command goto x/y, `<Esc>[x;yH`, except that the coordinates of that ANSI command are 1-based and the coordinates of this RIPscrip command are 0-based.

## RIP_GROUP_BEGIN

_Added in RIPscrip v2.A2._

_Start a grouping of RIPscrip commands_

|               |          |
| ------------- | -------- |
| **Level**     | 0        |
| **Command**   | `(`      |
| **Arguments** | \<none\> |

**Format:** `!|(` **Example:** `!|(`

**Attributes used:** none

This command has one special usage, that is to group blocks of commands together into logical groupings. This has no functional use to a terminal program, but to a paint program, it can be used to perform block moves of commands from one area in a scene to another. In order to accomodate this, a standardized method of doing group definitions is being created.

To specify a command grouping, specify the commands like this:

```text
!|(

  ...
  ... RIPscrip commands
  ...

!|)
```

When putting RIPscrip data files online on a host, you would typically want to remove groupings for efficiency, although this is not required. A terminal program should just ignore RIP_GROUP_BEGIN and [RIP_GROUP_END](#rip_group_end) commands, at it has no conceivable use for the information at this time.

Also note that a group may consist of several groups and commands. In other words, groups may be nested inside of each other for the purpose of bundling them together.

**NOTE:** Any currently active group definitions are "closed" when a [RIP_NO_MORE](#rip_no_more) command is encountered in a paint program or some kind of conversion utility that uses groups. _(v2.A3)_

## RIP_GROUP_END

_Added in RIPscrip v2.A2._

_End a grouping of RIPscrip commands_

|               |          |
| ------------- | -------- |
| **Level**     | 0        |
| **Command**   | `)`      |
| **Arguments** | \<none\> |

**Format:** `!|)` **Example:** `!|)`

**Attributes used:** none

This command ends a group of RIPscrip commands. See [RIP_GROUP_BEGIN](#rip_group_begin) for a detailed explanation.

## RIP_HEADER

_Added in RIPscrip v2.A1._

_Header command for subsequent RIPscrip sequence_

|               |                            |
| ------------- | -------------------------- |
| **Level**     | 0                          |
| **Command**   | `h`                        |
| **Arguments** | `revision:2 flags:4 res:2` |

**Format:** `!|h <flags> <revision> <res>` **Example:** `!|h010A0100`

**Attributes used:** Port, Base Math (Ultranums only)

This command was introduced in RIPscrip 2.0. It is specifically designed with two purposes in mind. First, it is defined to provide a consistent method for determining the revision of RIPscrip that is to follow in subsequent RIPscrip code (up to a [RIP_NO_MORE](#rip_no_more) command). Second, it provides a way of performing many different kinds of reset operations (if any) in one command - much more flexible than the older [RIP_RESET_WINDOWS](#rip_reset_windows) command. This command has the ability to set many states and situations for subsequent RIPscrip code (whether MegaNums are to be used, or UltraNums, what gets reset and what doesn't, etc).

The `<revision>` code of this command defines the revision of RIPscrip code that is to follow. The valid defined revisions are as follows:

| Revision | Description           |
| -------- | --------------------- |
| 00       | RIPscrip 1.54.00 code |
| 01       | RIPscrip 2.00.00 code |

The real heart of this command is the `<flags>` parameter. This field defines all of the attributes of the header command other then the revision field. The possible values which may be combined in this parameter (OR'ed together) are as follows:

| Value | Description |
| --- | --- |
| 0 | Do nothing |
| 1 | Use MegaNums for subsequent RIPscrip code |
| 2 * | Use UltraNums for subsequent RIPscrip code |
| 4 * | Auto-set world coordinate frame |
| 8 | Set world frame to 640x350 (backward compatible mode). NOTE: This flag should be used in revision "00" code. |
| 16 | Perform a hard reset - this resets everything. This includes clearing all data backup area, even if entries are protected. The entry is protected, then cleared. |
| 32 * | This performs a soft reset. The screen is cleared, all viewports are reset and viewport slot 0 is made full screen, text windows are deleted and text window 0 is made full screen. All resident queries are deleted, graphical style slots are erased as are the button style slots. This is identical in nature to the RIP_RESET_WINDOWS. |
| 64 | Clears port data table |
| 128 | Clears resident queries only |
| 256 | Clears mouse/button definitions only |
| 512 | Clears all data save slots and resets stack pointers |
| 1024 | Clears all base save areas |
| 2048 | Clears and stops any playing sound/music |
| 4096 | Clears the screen to background color (usually black) |
| 8192 | Resets all viewports of all drawing ports to the full size of their respective ports (no erasing performed). |
| 16384 | Clears all text window data table entries and makes entry 0 full screen in default text (no window clearing performed) |
| 32768 | Clears all graphical style data table entries |
| 65536 | Clears all button style data table entries |
| 131072 | Resets all palette data table entries to defaults |
| 262144 | All existing viewports are erased. If combined with flag 8192, then this is done first, then flag 8192 is executed. |
| 524288 | All existing text windows are erased. If combined with flag 16384, then this is done first, then flag 16384 is executed. |
| 1048576 * | Disables all mouse/keyboard input (discards) until a RIP_NO_MORE command is encountered thus signifying the end of a RIPscrip sequence. This prevents the user from accidentally interrupting the data stream with keyboard activity. The mouse cursor should be changed to something like an hourglass or disabled entirely. If no RIP_NO_MORE is found and no RIPscrip or raw text data is being received, then a RIP_NO_MORE should be assumed after some suitable timeout (up to the developer's discretion). |

**NOTE:** Items marked with a `*` are recommended defaults.

As you can see, the `<flags>` parameter allows you to reset only segments of things that you want to reset giving you considerable flexibility in starting off RIPscrip scenes.

Most of the flag options are fairly self-explanatory. Some mention should be made to several of them. Note that the flags described above are grouped into logically related flag groups. Flags 1 and 2 are mutually exlusive meaning only "1" or "2" may be used, but not both. Choosing either flag 1 or 2 resets coordinate sizes to 2 bytes.

Flag 4 requires some explanation. Auto-setting the world coordinate frame (WCF) will auto-set the world coordinate frame to a set of high-resolution world coordinates. The exact choices of coordinates chosen depend on whether you specified UltraNums or MegaNums. If you specify flag 4, then you must specify either flag 1 or flag 2 to determine which base math to use. MegaNums set the width of the world coordinate frame to 1280, and the WCF height to 960. This provides for a square world frame pixels. If UltraNums are used, then the width of the world frame is set to 4096 and the height is set to 3072. Again, this provides for square world frame pixels. If you physically need a larger world frame then you either need to alter the base math setting explicitly or alter the X/Y coordinate parameter size (see the next few commands).

Most video displays have a 4:3 width to height pixel ratio. For example, 640x480 is 4:3, 1024x768 is 4:3, etc. Note that 1280x1024 is NOT a 4:3 ratio (the 4:3 ratio for 1280 would be 1280x960.) _(v2.A2)_

The Hard Reset option (flag 16) will perform a complete reset of the RIPscrip environment. This is equivalent to choosing flags 64 through 131072. In effect, this clears everything including data save slots and the base save areas.

The Soft Reset option (flag 32) will perform a reset that is identical in nature to the RIP_RESET_WINDOWS reset. This is equivalent to choosing flags 1, 8, 64, 128, and 2048 through 131072. This doesn't delete any data save slots or base save areas, doesn't intentionally affect the world frame except that it sets it to 640x350, and chooses MegaNums as the numbers of choice.

**NOTE:** This command ALWAYS sets the mouse cursor back to the default arrow pointer unless the "disable mouse input" flag is used whereas the mouse will be temporarily changed to an hourglass or disabled entirely until a RIP_NO_MORE. After this, then the pointer will be reset back to an arrow. See [RIP_SET_MOUSE_CURSOR](11-level-1-commands.md#rip_set_mouse_cursor) for details.

## RIP_HOME

_Move cursor to upper-left corner of Text Window_

|               |          |
| ------------- | -------- |
| **Level**     | 0        |
| **Command**   | `H`      |
| **Arguments** | \<none\> |

**Format:** `!|H` **Example:** `!|H`

**Attributes used:** none

This command positions the text cursor to the upper-left corner in the TTY Text Window, if it is active. This is equivalent to the ANSI command `ESC[1;1H`.

## RIP_LINE

_Draw a line in the current color/line style_

|               |                           |
| ------------- | ------------------------- |
| **Level**     | 0                         |
| **Command**   | `L`                       |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|L <x0> <y0> <x1> <y1>` **Example:** `!|L00010A0E`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command will draw a line in the current drawing color, using the current line style, pattern and thickness. The line is drawn from (x0,y0) to (x1,y1) in the graphics viewport.

## RIP_LINE_STYLE

_Defines a line's pattern and thickness_

|               |                                         |
| ------------- | --------------------------------------- |
| **Level**     | 0                                       |
| **Command**   | `=`                                     |
| **Arguments** | `off_draw:1 style:1 user_pat:4 thick:2` |

**Format:** `!|= <off_draw> <style> <user_pat> <thick>` **Example:** `!|=01000001`

**Attributes used:** Line Style, Base Math (current setting)

This command establishes the current line pattern and thickness for many subsequent graphics primitive commands. There are four built-in line styles plus provisions for custom line patterns.

| Style | Description | Binary | Hex |
| --- | --- | --- | --- |
| 0 | Normal, Solid Line | 1111111111111111 | FFFF |
| 1 | Dotted Line | 0011001100110011 | 3333 |
| 2 | Centered Line | 0001111000111111 | 1E3F |
| 3 | Dashed Line | 0001111100011111 | 1F1F |
| 4 | Custom Defined line (see about `<user_pat>` below) |  |  |

If the `<style>` is set to a value of 4 (custom pattern), then the `<user_pat>` parameter is used as a 16-bit representation of the pixels in the line pattern. For example:

| Repeating Pattern | Binary Coding    | Hex  | Decimal | MegaNum |
| ----------------- | ---------------- | ---- | ------- | ------- |
| `- - - - - - - -` | 1010101010101010 | AAAA | 43690   | 0XPM    |
| `----    ----`    | 1111000011110000 | F0F0 | 61680   | 1BLC    |

So, the most-significant-bit of `<user_pat>` is toward the starting point of the line or border that uses this fill pattern. If the `<style>` parameter is not 4, then the `<user_pat>` parameter is ignored.

In RIPscrip v2.0, the off spaces of a patterned line can now be something other than transparent (as in 1.x RIPscrip format). If the `<off_draw>` parameter is set to a non-zero value, then the current pen background color is used to draw the off pixels of a patterned line. The values of the `<off_draw>` field are as follows:

| Value | Description                                |
| ----- | ------------------------------------------ |
| 0     | Draw off pixels as transparent pixels      |
| 1     | Draw off pixels using pen background color |

In earlier verions of RIPscrip (v1.54 and earlier), only two different line thickness options were allowed (1 pixel wide and 3 pixels wide). In 2.00 and later revisions, arbitrary thicknesses of lines are supported from 1 pixel on up. If the thickness is an even number than it is up to the destination system's graphics engine to determine whether the even pixel is drawn to the left or right of the center point in vertical orientations, or above/below for horizontal orientations. In any event, the thickness value of lines is specified by the `<thick>` parameter. _(v2.A1)_

When a [RIP_RESET_WINDOWS](#rip_reset_windows) or a [RIP_HEADER](#rip_header) command is executed that resets the environment, the current line style is set to a single pixel wide solid line. Off pixels are set to transparent mode. _(v2.A3)_

## RIP_MOVE

_Move the current drawing position to (X,Y)_

|               |             |
| ------------- | ----------- |
| **Level**     | 0           |
| **Command**   | `m`         |
| **Arguments** | `x:XY y:XY` |

**Format:** `!|m <x> <y>` **Example:** `!|m0509`

**Attributes used:** Viewport, Port, Base Math (current setting)

This command moves the current graphics drawing cursor to (x,y). You could use this to draw text at a certain point, but you'd probably use [RIP_TEXT_XY](10-level-0-commands-s-w.md#rip_text_xy) instead. This command is primarily provided for future development which will make use of its ability to relocate the current drawing position without physically drawing anything.

## RIP_NO_MORE

_End of RIPscrip Scene_

|               |          |
| ------------- | -------- |
| **Level**     | 0        |
| **Command**   | `#`      |
| **Arguments** | \<none\> |

**Format:** `!|#` **Example:** `!|#`

**Attributes used:** none

This command indicates that RIPscrip commands are complete. This allows the terminal program to activate Mouse Regions, or respond to queued up Mouse Clicks without disturbing the natural flow of the script transmission.

For noise-immunity, the Host should transmit three or more RIP_NO_MORE command consecutively to make sure the message gets to the terminal. The terminal should also time-out if no data is received for a while, and assume RIP_NO_MORE.

The actual duration of the "time-out" is entirely up to the implementor of their terminal program. A good recommended setting would be at least an entire second or more after the receipt of the last RIPscrip command. Raw ASCII/ANSI text does not contribute to the time-out in any way. Only an actual RIPscrip command could cause the time-out counter to be reset (thus starting the time-out countdown all over again). Timing-out is not a pre-requisite to supporting RIPscrip. It is a highly desirable feature to do this, but it is not required. _(v1.54)_

This command also re-activates user input previously disabled with a [RIP_HEADER](#rip_header) command. This is useful to prevent the user from clicking on buttons before the scene is completely drawn. See the RIP_HEADER command for more details on this mode of operation. _(v2.A1)_

## RIP_ONE_DRAWING_PALETTE

_Added in RIPscrip v2.A0._

_Set Drawing Palette entries to RGB colors_

|               |                              |
| ------------- | ---------------------------- |
| **Level**     | 0                            |
| **Command**   | `d`                          |
| **Arguments** | `entry:2 bits:1 rgb_color:4` |

**Format:** `!|d <entry> <bits> <rgb_color>` **Example:** `!|d018qR3P`

**Attributes used:** Draw Color, Back Color, Base Math (current setting)

This command will set a single entry in the Drawing Palette to an arbitrary RGB color. This allows you to customize the Drawing Palette with extended color information beyond what you can normally do with the Desktop Palette. This command is more flexible in nature in that it allows you to access colors between 0-255 and gives you the added flexibility to store more than 2 bits of RGB data - thus allowing you to select colors out of a much larger master palette.

The `<entry>` parameter specifies which Color Palette Entry number will be set with the RGB color data.

**NOTE:** The four-digit color parameters are not MegaNums - they are always ULTRA-NUMS regardless of the setting of the global base-math value! With four digits at base-64 math, you can achieve 24-bits of precision in one four-digit number.

If you break-down the four-digit UltraNum color values into their binary equivalents (three bytes), the MSB would contain the Red component, the LSB would contain the Blue component and the byte in between contains the Green component. This command allows you to specify a number of `<bits>` for each component. Under no circumstances are `<bit>` values above 8 allowed, since this would overflow a four-digit UltraNum parameter (beyond 24 bits).

Color Entry number zero (0) is the screen's background color. It is normally set to RGB color 0/0/0, or Black. Changing this color will alter the background color of the entire screen/environment, so great care should be taken if you alter color number 0.

## RIP_ONE_PALETTE

_Set RGB color of 16-color Desktop Palette_

|               |                   |
| ------------- | ----------------- |
| **Level**     | 0                 |
| **Command**   | `a`               |
| **Arguments** | `color:2 value:2` |

**Format:** `!|a <color> <value>` **Example:** `!|a051B`

**Attributes used:** Draw Color, Back Color, Base Math (current setting)

This command changes one color in the 16-color Desktop Palette. The color number is sent along with the new color value from the Master Color Palette. The color `<value>` must be in the range of 0-63. Once a Set One Palette command is processed, any colors on the screen that correspond to the `<color>` number will be changed instantly to the new color value (providing the terminal is running in palette mode). You may obtain color cycling effects by using this command. The default RIP palette is restored when by the [RIP_RESET_WINDOWS](#rip_reset_windows) command.

See the [RIP_COLOR](08-level-0-commands-a-f.md#rip_color) command for an exact description of the RGB values used in this command.

Color 00 of the 16-color RIP palette is always the background color (which is typically Black).

## RIP_OVAL

_Draw elliptical arc in current color/line style_

|               |                                                  |
| ------------- | ------------------------------------------------ |
| **Level**     | 0                                                |
| **Command**   | `O`                                              |
| **Arguments** | `x:XY y:XY st_ang:2 end_ang:2 x_rad:XY y_rad:XY` |

**Format:** `!|O <x> <y> <st_ang> <end_ang> <x_rad> <y_rad>` **Example:** `!|O1E1A18003G15`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws an elliptical arc similar to the circular [RIP_ARC](08-level-0-commands-a-f.md#rip_arc) command. The center of the ellipse is (x,y) and the arc is drawn starting from `<st_ang>` and proceeding counterclockwise to `<end_ang>` (see RIP_ARC above for details).

The X radius is half the full width of the ellipse, the Y radius is half the full height. The ellipse is drawn according to the current line thickness, but the current line pattern has no effect.

**NOTE:** This command is identical to the [RIP_OVAL_ARC](#rip_oval_arc) due to historical reasons. See RIP_OVAL_ARC for a discussion of the starting angle and the ending angle parameters of this command. _(v2.A3)_

## RIP_OVAL_ARC

_Draw an elliptical arc_

|               |                                              |
| ------------- | -------------------------------------------- |
| **Level**     | 0                                            |
| **Command**   | `V`                                          |
| **Arguments** | `x:XY y:XY st_ang:2 e_ang:2 radx:XY rady:XY` |

**Format:** `!|V <x> <y> <st_ang> <e_ang> <radx> <rady>` **Example:** `!|V1E18003G151Q`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws an elliptical arc, or a segment of an ellipse. Drawing begins at `<st_ang>` and terminates at `<e_ang>`. The angles are represented starting at zero for 3 o'clock position and increasing counterclockwise through a full ellipse at 360 degrees:

```text
          90°
           │
   180°────┼────0°
           │
          270°
```

The arc drawing begins at the `<st_ang>` and continues counterclockwise to the `<e_ang>`. A complete ellipse will be displayed if `<st_ang>`=0 and `<e_ang>`=360. This command does not utilize "aspect ratios" because of the nature of an Ellipse. It does not take advantage of line patterns but does comply with line thickness.

Both angles can be greater than 360 degrees. The starting angle must be greater or equal to the ending angle. _(v2.A3)_

## RIP_OVAL_PIE_SLICE

_Draws an elliptical pie slice_

|               |                                              |
| ------------- | -------------------------------------------- |
| **Level**     | 0                                            |
| **Command**   | `i`                                          |
| **Arguments** | `x:XY y:XY st_ang:2 e_ang:2 radx:XY rady:XY` |

**Format:** `!|i <x> <y> <st_ang> <e_ang> <radx> <rady>` **Example:** `!|i1E18003G151Q`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws an "elliptical pie slice". It obeys all of the same commands as the Elliptical Arc command described above. The ends of the arc are connected to the Center-Point of the Arc with two straight lines. These two lines converge at the Center-Point. The interior of the Slice is filled with the current Fill Color and Pattern. The exterior (outline) of the Slice is drawn using the current drawing color and line thickness. The Line Pattern feature does not apply to this command.

Both angles can be greater than 360 degrees. The starting angle must be greater or equal to the ending angle. _(v2.A3)_

When borders are disabled, this command adheres to the resolution independent method of filling regions. _(v2.A3)_

## RIP_PIE_SLICE

_Draws a circular pie slice_

|               |                                             |
| ------------- | ------------------------------------------- |
| **Level**     | 0                                           |
| **Command**   | `I`                                         |
| **Arguments** | `x:XY y:XY start_ang:2 end_ang:2 radius:XY` |

**Format:** `!|I <x> <y> <start_ang> <end_ang> <radius>` **Example:** `!|I1E18003G15`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws a "pie slice". The slice is circular. It obeys all of the same commands as the Arc command described above. The ends of the arc are connected to the Center-Point of the Arc with two straight lines. These two lines converge at the Center-Point. The interior of the Slice is filled with the current Fill Color and Pattern. The exterior (outline) of the Slice is drawn using the current drawing color and line thickness. The Line Pattern feature does not apply to this command.

Both angles can be greater than 360 degrees. The starting angle must be greater or equal to the ending angle. _(v2.A3)_

The radius is considered to be in the horizontal direction for the purpose of aspect ratio calculations. _(v2.A3)_

When borders are disabled, this command adheres to the resolution independent method of filling regions. _(v2.A3)_

## RIP_PIXEL

_Draws one pixel using current drawing color_

|               |             |
| ------------- | ----------- |
| **Level**     | 0           |
| **Command**   | `X`         |
| **Arguments** | `x:XY y:XY` |

**Format:** `!|X <x> <y>` **Example:** `!|X1122`

**Attributes used:** Draw Color, Write Mode, Viewport, Port, Base Math (current setting)

This command will draw a single pixel in the current drawing color at the given (x,y) graphics position. This command is included for completeness, but in practice it would be extremely inefficient to make heavy use of it.

## RIP_POLYGON

_Draw polygon in current color/line-style_

|               |                                         |
| ------------- | --------------------------------------- |
| **Level**     | 0                                       |
| **Command**   | `P`                                     |
| **Arguments** | `npoints:2 x1:XY y1:XY ... xn:XY yn:XY` |

**Format:** `!|P <npoints> <x1> <y1> ... <xn> <yn>` **Example:** `!|P03010105090905`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command will draw a multi-sided closed polygon. The polygon is drawn using the current drawing color, line pattern, thickness and raster operation (write mode). The `<npoints>` parameter is between 2 and 512 and indicates how many (x,y) coordinate pairs will follow, which is also the number of sides of the polygon. The polygon interior is not filled by RIP_POLYGON.

The polygon is enclosed by the last vertex between xn,yn and x1,y1. In other words, you do not have to connect the end to the beginning - it is automatically done for you.

## RIP_POLYLINE

_Added in RIPscrip v1.54._

_Draw a Poly-Line (multi-faceted line)_

|               |                                         |
| ------------- | --------------------------------------- |
| **Level**     | 0                                       |
| **Command**   | `l`                                     |
| **Arguments** | `npoints:2 x1:XY y1:XY ... xn:XY yn:XY` |

**Format:** `!|l <npoints> <x1> <y1> ... <xn> <yn>` **Example:** `!|l03010105090905`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command will draw a multi-faceted line. It is identical in nature to the [RIP_POLYGON](#rip_polygon) command above, except that the last point is NOT connected to the first point. Generally speaking, a Poly-Line is not an "enclosed area". The segments of the Poly-Line are drawn using the current drawing color, line pattern, thickness and Drawing Write Mode.

The `<npoints>` parameter is between 2 and 512 and indicates how many (x,y) coordinate pairs will follow, which is also the number of sides of the Poly-Line.

An example of a five sided (6 vertices) Poly-Line might be:

```text
P1 o                                             o P6
    ▀▄                          o P4           ▄▀
      ▀▄                      ▄▀█            ▄▀
        ▀▄                  ▄▀  █          ▄▀
          ▀▄              ▄▀    █        ▄▀
            ▀▄          ▄▀      █      ▄▀
              ▀o▄▄▄▄▄▄o▀        █    ▄▀
             P2       P3        █  ▄▀
                              P5█▄▀
                                o
```

## RIP_POLY_BEZIER

_Added in RIPscrip v2.A1._

_Draw a poly-bezier curve (multi-segmented)_

|  |  |
| --- | --- |
| **Level** | 0 |
| **Command** | `z` |
| **Arguments** | `num:2 count:2 x_base:XY y_base:XY ... type:1 x1:XY y1:XY ... type:1 x1:XY y1:XY x2:XY y2:XY x3:XY y3:XY ...` |

**Format:** `!|z <num> <count> <x_base> <y_base> ... <type> <x1> <y1> ... <type> <x1> <y1> <x2> <y2> <x3> <y3> ...` **Example:** See below

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command is a variation on the [RIP_BEZIER](08-level-0-commands-a-f.md#rip_bezier) command. The typical RIP_BEZIER command creates one single bezier curve. This command combines one or more bezier curves into a chain of connected bezier curves. Each segment (curve) of the poly-bezier can be of different types. It can be a bezier curve, or a simple straight line. The combination of these facilities allows the construction of say, a text character representation of a font that has some straight line segments and some curved segments. The curved portions (bezier curves) are actually comprised of many smaller straight line segments - when viewed on a monitor, they appear to be a continuous curve. The actual number of segments comprising a particular curve in the poly-bezier is determined by the command's `<count>` parameter.

This command is different than any other in RIPscrip in that its variable length portion of the command is made up of different blocks of parameters which can be different byte lengths! If you look at the definition, the beginning of the command includes a `<num>`, `<count>`, `<x_base>` and `<y_base>` parameter - the remainder of the command is variable length in nature.

### Parameter blocks

The `<num>` parameter defines how many parameter blocks are to be expected in this command. _(v2.A2)_

The `<count>` parameter defines how many "segments" are to be drawn for each bezier curve segment of this poly-bezier curve. This means that all bezier curve segments in a poly-bezier will have the same number of segments for their respective curves. _(v2.A2)_

The `<x_base>` and `<y_base>` parameters define the pixel beginning point for the entire poly-bezier "curve". To determine what type of parameter block follows, you need to look at the first byte of the segment; this is the `<type>` parameter.

The `<type>` parameter defines what type of parameter block is to be parsed. The possible types are:

| Type | Description                  |
| ---- | ---------------------------- |
| 0    | Straight line segment        |
| 1    | Smooth bezier curve segment  |
| 2    | Jointed bezier curve segment |

A "smooth" segment is a bezier curve whose beginning curve is totally smooth with the ending curve of a previous bezier curve. In practice on a terminal system, this doesn't mean anything to a terminal program because it is going to display a bezier curve as a normal bezier curve. The meaning of having a second bezier curve type comes into play when using paint program utilities that let you manipulate poly-bezier curves. A paint system might use this information to control how end-points and control points can be moved because they are "locked" with one or two points from the previous (or next) bezier curve. See the notes at the end of this command's description about smooth bezier curve joints.

If the `<type>` parameter indicates that the segment is a straight-line segment, then there are only two remaining parameters (not including the `<type>` parameter) in the block (the line's end-points) as in the following syntax definition:

```text
type:1 x1:XY y1:XY
```

The `<type>` parameter should be set to "0" for line segments. The (x1,y1) parameters define the endpoints of the line segment. The beginning point of the line is the end-point of the last segment (or the (x_base,y_base) parameters of the command header if this is the first parameter block).

If the `<type>` indicates that the segment is a bezier curve, then the parameter block contains six parameters as in the following syntax definition (not including the `<type>` parameter):

```text
type:1 x1:XY y1:XY x2:XY y2:XY x3:XY y3:XY
```

The first byte is the `<type>` value for this block (be set to 1 or 2 for a bezier curve segment). The remaining six parameters define the first two control points and the end-point for the bezier curve segment. The beginning point of the curve segment is defined as the end-point of the previous block (or the (x_base,y_base) parameter in the header of the command if this is the first parameter block).

Refer to the RIP_BEZIER command for a complete description of bezier curve beginning/end-points and how control points shape the curve to a desired result.

### Example

An example of a poly-bezier curve might be shown as follows: _(v2.A2)_

```text
Segment 1 ────────────┐   P2
  (line)      P1       (50,10)        P3 (110,10)
           (10,10) ■▒▒▒▒■▒▒         X
                   ▒       ▒   ┌────────────── Segment 2
                   ▒        ▒─┘                (bezier)
                   ▒        ▒
                   ▒        ▒       X P4 (110,70)
   Segment 0 ─────▒      ▒▒
     (line)        ▒ P5 ■▒▒▒
                   ▒(50,70) ▒          X P6 (120,70)
                   ▒         ▒───────────────── Segment 3
                   ▒         ▒                    (bezier)
                   ▒(50,130) ▒
        BASE       ▒    P8  ▒            P7 (120,130)
        POINT ────■▒▒▒▒■▒▒▒           X
                 P0/P9
              (10,130) └──────────────────────── Segment 4
                                                  (line)
```

The above diagram shows 5 different segments to the poly-bezier curve. Segments 0, 1 and 4 are straight line segments, and semgents 2 and 3 are actual bezier curves. All X/Y points (beginning-points, end-points and control-points are depicted with X's and are labeled P0 through P9. The parameters to represent this command might appear something like the following (in expanded decimal):

```text
     Decimal     MegaNum     Description
     ──────────────────────────────────────────────────
 !|z 05 ........ 05 ........ 5 segments
     10 ........ 0A ........ 10 lines per bezier curve
       10,130 ..   0A 3M ... Base point - P0
     0 ......... 0 ......... Segment 0 - Line
       10,10 ...   0A 0A ... Line endpoint - P1
     0 ......... 0 ......... Segment 1 - Line
       50,10 ...   1E 0A ... Line endpoint - P2
     1 ......... 1 ......... Segment 2 - Smooth Bezier
       110,10 ..   32 0A ... Control point - P3
       110,70 ..   32 1Y ... Control point - P4
       50,70 ...   1E 1Y ... Curve end point - P5
     2 ......... 2 ......... Segment 3 - Jointed Bezier
       120,70 ..   3C 1Y ... Control point - P6
       120,130 .   3C 3M ... Control point - P7
       50,130 ..   1E 3M ... Curve end point - P8
     0 ......... 0 ......... Segment 4 - Line
       10,130 ..   0A 3M ... Line endpoint - P9
```

Notice that in the preceding example we are not expressing numbers in meganums as would normally be used in RIPscrip files. This is for simple human readability for this example.

The completed command looks like this (base math setting = meganums): _(v2.A2)_

`!|z050A0A3M00A0A01E0A1320A321Y1E1Y23C1Y3C3M1E3M00A3M`

### Smooth joints

This command is often used to combine several segments together into one larger "curve". As previously mentioned, the end-point of one segment is the beginning point of the next segment (if any).

When you are dealing with bezier curve segments, it is possible to make sure that the end of one curve has a perfectly smooth curve with the beginning of the next curve. If two adjacent bezier curve blocks are to have a perfectly smooth "connecting joint", then you must make sure that the last control point and the end-point of the first curve are colinear with the first control point of the second curve. This is not a requirement of this command - it is generic in that it can display any combination of line segments and bezier curve segments. We are simply noting this mathematical attribute of bezier curves for the benefit of the RIPscrip programmer to generate smooth segmented curves in some sort of paint program.

A bezier curve can also have a "smooth joint" with a line segment. If the bezier curve is followed by a line segment, then to make sure that the joint has a smooth curve, you must make sure that the last control point and the end point of the bezier curve segment are colinear with the end-point of the following line segment. The same holds true of the segments are reversed (line segment followed by a bezier curve segment) - the only difference is that the beginning point of a line segment is determined by the end-point of its preceding block (or the base point if the line segment is the first parameter block) and you would be making the starting point of the bezier curve and the 1ST control point colinear.

**NOTE:** Poly-Bezier curves might exhibit pixel annomolies at the beginning and ending points of each segment if thick lines are used for the curve with XOR or other non-COPY write modes. The reason for this is because some graphics engines (Windows, Macintosh, MS-DOS, etc) might not handle things 100.0% perfectly for thick poly-lines in XOR mode (poly-lines are used to represent this command after all segments are broken down).

## RIP_POLY_BEZIER_LINE

_Added in RIPscrip v2.A2._

_Draw a poly-bezier curve (open-ended)_

|  |  |
| --- | --- |
| **Level** | 0 |
| **Command** | `t` |
| **Arguments** | `num:2 count:2 x_base:XY y_base:XY ... type:1 x1:XY y1:XY ... type:1 x1:XY y1:XY x2:XY y2:XY x3:XY y3:XY ...` |

**Format:** `!|z <num> <count> <x_base> <y_base> ... <type> <x1> <y1> ... <type> <x1> <y1> <x2> <y2> <x3> <y3> ...` **Example:** See below

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command is a variation on the [RIP_POLY_BEZIER](#rip_poly_bezier) command. The difference is that the RIP_POLY_BEZIER command is a closed object, meaning that the beginning and ending points are connected. With RIP_POLY_BEZIER_LINE, the object does not have it's end points connected. This is just like the difference between [RIP_POLYGON](#rip_polygon) and [RIP_POLYLINE](#rip_polyline).

This command uses the same parameter order as RIP_POLY_BEZIER.

## RIP_RECTANGLE

_Draw a rectangle in current color/line style_

|               |                           |
| ------------- | ------------------------- |
| **Level**     | 0                         |
| **Command**   | `R`                       |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY` |

**Format:** `!|R <x0> <y0> <x1> <y1>` **Example:** `!|R00010A0E`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws a rectangle in the current drawing color, using the current line style, pattern and thickness. (x0,y0) and (x1,y1) are any two opposing corners of the rectangle. If x0=x1 or y0=y1 then the command will draw a single vertical or horizontal line. The rectangle interior is not filled by RIP_RECTANGLE.

## RIP_RESET_WINDOWS

_Clear Graphic/Text Windows & reset to full screen_

|               |          |
| ------------- | -------- |
| **Level**     | 0        |
| **Command**   | `*`      |
| **Arguments** | \<none\> |

**Format:** `!|*` **Example:** `!|*`

**Attributes used:** Port

A system might use this function before entering a text-only mode that does not support RIP commands.

This command will:

- Set the World Coordinate Frame to 640x350 (backwards compat.)
- Set the Text Window to a full screen
- Set the Text Window to the user-selected default of the five Text Window fonts (80x43, 91x43, 80x25, 91x25, and 40x25) and place the text cursor in the upper left corner.
- Restore the default RIP color palette (see [RIP_SET_PALETTE](10-level-0-commands-s-w.md#rip_set_palette) and [RIP_SET_DRAWING_PALETTE](10-level-0-commands-s-w.md#rip_set_drawing_palette) below).
- Clear the entire screen to the current background color (which is black because the color palette was just reset!)
- Delete all Mouse Fields and Mouse Buttons
- Delete ports #1-35 (unprotected) - port #0 is made full screen
- Reset all graphical style data tables and Text Window data tables to suitable defaults.
- All "active data table entries" are set to entry #0 current slots are set to slot #0.
- Reset all button style data tables
- Set the mouse cursor back to the default arrow pointer. See [RIP_SET_MOUSE_CURSOR](11-level-1-commands.md#rip_set_mouse_cursor) for details.
- Set the current Base Math to Base-36 (meganums).
- Set the coordinate size to 2 byte MegaNums
- Set the color palette to map mode (not Direct RGB encoding).

**NOTE:** The data backup areas are not cleared by this command. _(v2.A3)_

## RIP_ROUNDED_RECT

_Added in RIPscrip v2.A3._

_Draw a rectangle with rounded corners_

|               |                                 |
| ------------- | ------------------------------- |
| **Level**     | 0                               |
| **Command**   | `U`                             |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY rad:2` |

**Example:** `!|U00010A0E09`

**Attributes used:** Draw Color, Back Color, Line Style, Write Mode, Viewport, Port, Base Math (current setting)

This command draws a rounded corner rectangle. The corners of the rectangle are not drawn at right angles like normal rectangles. The rounded rectangle has circular arcs drawn at all four corners of the drawn object. The radius of the arc that is used to fill-in the corners is specified in the `<rad>` parameter. The `<x0,y0>` and `<x1,y1>` parameters define the upper left and lower right corners of the rectangle as if the corners were actually specified as a normal rectangle. The circular arcs drawn in the corners of the rounded rectangle are truly circular in nature and adhere to aspect ratios relating to the actual video configuration of the destination application program.

---

[◀ Prev: Level-0 Commands (A–F)](08-level-0-commands-a-f.md) · [Contents](README.md) · [Next: Level-0 Commands (S–W) ▶](10-level-0-commands-s-w.md)
