# Drawing Primitives

[◀ Prev: Text Output & Fonts](06-text-output.md) · [Contents](README.md) · [Next: Line & Fill Styles ▶](08-line-fill-styles.md)

This section covers the RIPscrip drawing primitive commands: [RIP_PIXEL](#rip_pixel), [RIP_LINE](#rip_line), [RIP_RECTANGLE](#rip_rectangle), [RIP_BAR](#rip_bar), [RIP_CIRCLE](#rip_circle), [RIP_OVAL](#rip_oval), [RIP_FILLED_OVAL](#rip_filled_oval), [RIP_ARC](#rip_arc), [RIP_OVAL_ARC](#rip_oval_arc), [RIP_PIE_SLICE](#rip_pie_slice), [RIP_OVAL_PIE_SLICE](#rip_oval_pie_slice), [RIP_BEZIER](#rip_bezier), [RIP_POLYGON](#rip_polygon), [RIP_FILL_POLYGON](#rip_fill_polygon), [RIP_POLYLINE](#rip_polyline), and [RIP_FILL](#rip_fill).

## RIP_PIXEL

_Draws a one pixel using current drawing color_

|               |            |
| ------------- | ---------- |
| **Level**     | 0          |
| **Command**   | `X`        |
| **Arguments** | `x:2, y:2` |

**Format:** `!|X <x> <y>`

**Example:** `!|X1122`

**Attributes used:** Draw Color, Viewport _(v1.54)_

This command will draw a single pixel in the current drawing color at the given (x,y) graphics position. This command is included for completeness, but in practice it would be extremely inefficient to make much use of it.

## RIP_LINE

_Draw a line in the current color/line style_

|               |                          |
| ------------- | ------------------------ |
| **Level**     | 0                        |
| **Command**   | `L`                      |
| **Arguments** | `x0:2, y0:2, x1:2, y1:2` |

**Format:** `!|L <x0> <y0> <x1> <y1>`

**Example:** `!|L00010A0E`

**Attributes used:** Draw Color, Line Pattern, Line Thick, Write Mode, Viewport _(v1.54)_

This command will draw a line in the current drawing color, using the current line style, pattern and thickness. The line is drawn from (x0,y0) to (x1,y1) in the graphics viewport.

## RIP_RECTANGLE

_Draw a rectangle in current color/line style_

|               |                          |
| ------------- | ------------------------ |
| **Level**     | 0                        |
| **Command**   | `R`                      |
| **Arguments** | `x0:2, y0:2, x1:2, y1:2` |

**Format:** `!|R <x0> <y0> <x1> <y1>`

**Example:** `!|R00010A0E`

**Attributes used:** Draw Color, Line Pattern, Line Thick, Write Mode, Viewport _(v1.54)_

This command draws a rectangle in the current drawing color, using the current line style, pattern and thickness. (x0,y0) and (x1,y1) are any two opposing corners of the rectangle. If x0=x1 or y0=y1 then the command will draw a single vertical or horizontal line. The rectangle interior is not filled by RIP_RECTANGLE.

## RIP_BAR

_Draw filled rectangle with fill color/pattern_

|               |                          |
| ------------- | ------------------------ |
| **Level**     | 0                        |
| **Command**   | `B`                      |
| **Arguments** | `x0:2, y0:2, x1:2, y1:2` |

**Format:** `!|B <x0> <y0> <x1> <y1>`

**Example:** `!|B00010A0E`

**Attributes used:** Fill Color, Fill Pattern, Viewport _(v1.54)_

This command fills a rectangular region with the current fill color and pattern. No border is drawn.

## RIP_CIRCLE

_Draw circle in current color and line thickness_

|               |                                    |
| ------------- | ---------------------------------- |
| **Level**     | 0                                  |
| **Command**   | `C`                                |
| **Arguments** | `x_center:2, y_center:2, radius:2` |

**Format:** `!|C <x_center> <y_center> <radius>`

**Example:** `!|C1E180M`

**Attributes used:** Draw Color, Line Thick, Viewport _(v1.54)_

This command draws a circle in the current drawing color and line thickness. The `<radius>` is in pixel units. This command understands aspect ratios and will draw a truly circular circle instead of an oblong circle (ellipse) like on other graphics systems. The aspect ratio is currently based on the EGA 640x350 resolution and is understood by both the GUI designer and the Terminal Program.

NOTE: This command uses the line thickness setting, but not the line patterns. In other words, you can draw a circle with a thick or a thin border, but not a dashed or dotted border.

## RIP_OVAL

_Draw elliptical arc in current color/line style_

|               |                                                   |
| ------------- | ------------------------------------------------- |
| **Level**     | 0                                                 |
| **Command**   | `O`                                               |
| **Arguments** | `x:2, y:2, st_ang:2, end_ang:2, x_rad:2, y_rad:2` |

**Format:** `!|O <x> <y> <st_ang> <end_ang> <x_rad> <y_rad>`

**Example:** `1E1A18003G150Z`

**Attributes used:** Draw Color, Line Thick, Viewport _(v1.54)_

This command draws an elliptical arc similar to the circular [RIP_ARC](#rip_arc) command. The center of the ellipse is (x,y) and the arc is drawn starting from `<st_ang>` and proceeding counterclockwise to `<end_ang>` (see RIP_ARC above for details).

The X radius is half the full width of the ellipse, the Y radius is half the full height. The ellipse is drawn according to the current line thickness, but the current line pattern has no effect.

## RIP_FILLED_OVAL

_Draw filled ellipse using current color/pattern_

|               |                                            |
| ------------- | ------------------------------------------ |
| **Level**     | 0                                          |
| **Command**   | `o`                                        |
| **Arguments** | `x_center:2, y_center:2, x_rad:2, y_rad:2` |

**Format:** `!|o <x_center> <y_center> <x_rad> <y_rad>`

**Example:** `!|o1G2B0M0G`

**Attributes used:** Draw Color, Line Thick, Fill Color, Fill Pattern, Viewport _(v1.54)_

This command draws a complete filled ellipse on the screen. The interior of the ellipse is drawn using the current fill pattern and fill color. The outline of the ellipse is drawn using the current drawing color and line thickness.

## RIP_ARC

_Draw circular arc in current color/line thickness_

|               |                                              |
| ------------- | -------------------------------------------- |
| **Level**     | 0                                            |
| **Command**   | `A`                                          |
| **Arguments** | `x:2, y:2, start_ang:2, end_ang:2, radius:2` |

**Format:** `!|A <x> <y> <start_ang> <end_ang> <radius>`

**Example:** `!|A1E18003G15`

**Attributes used:** Draw Color, Line Thick, Viewport _(v1.54)_

This command draws a circular arc, or a segment of a circle. Drawing begins at `<start_ang>` and terminates at `<end_ang>`. The angles are represented starting at zero for the 3 o'clock position and increasing counterclockwise through a full circle to 360:

```text
                          90
                           |
                     180---|--- 0
                           |
                          270
```

The arc drawing begins at the `<start_angle>` and continues counter-clockwise to the `<end_angle>`. A full circle will be displayed if `<start_ang>`=0 and `<end_ang>`=360. This command recognizes aspect ratios like the circle command does. It does not take advantage of line patterns but does comply with line thickness.

If both angles are equal, nothing is drawn.

## RIP_OVAL_ARC

_Draw an elliptical arc_

|               |                                              |
| ------------- | -------------------------------------------- |
| **Level**     | 0                                            |
| **Command**   | `V`                                          |
| **Arguments** | `x:2, y:2, st_ang:2, e_ang:2, radx:2 rady:2` |

**Format:** `!|V <x> <y> <st_ang> <e_ang> <radx> <rady>`

**Example:** `!|V1E18003G151Q`

**Attributes used:** Draw Color, Line Thick, Viewport _(v1.54)_

This command draws an elliptical arc, or a segment of an ellipse. Drawing begins at `<st_ang>` and terminates at `<e_ang>`. The angles are represented starting at zero for 3 o'clock position and increasing counterclockwise through a full ellipse at 360 degrees:

```text
                          90
                           |
                     180---|--- 0
                           |
                          270
```

The arc drawing begins at the `<st_ang>` and continues counterclockwise to the `<e_ang>`. A complete ellipse will be displayed if `<st_ang>`=0 and `<e_ang>`=360. This command does not utilize "aspect ratios" because of the nature of an Ellipse. It does not take advantage of line patterns but does comply with line thickness.

## RIP_PIE_SLICE

_Draws a circular pie slice_

|               |                                              |
| ------------- | -------------------------------------------- |
| **Level**     | 0                                            |
| **Command**   | `I`                                          |
| **Arguments** | `x:2, y:2, start_ang:2, end_ang:2, radius:2` |

**Format:** `!|I <x> <y> <start_ang> <end_ang> <radius>`

**Example:** `!|I1E18003G15`

**Attributes used:** Draw Color, Line Thick, Fill Color, Fill Pattern, Viewport _(v1.54)_

This command draws a "pie slice". The slice is circular. It obeys all of the same commands as the Arc command described above. The ends of the arc are connected to the Center-Point of the Arc with two straight lines. These two lines converge at the Center-Point. The interior of the Slice is filled with the current Fill Color and Pattern. The exterior (outline) of the Slice is drawn using the current drawing color and line thickness. The Line Pattern feature does not apply to this command.

## RIP_OVAL_PIE_SLICE

_Draws an elliptical pie slice_

|               |                                              |
| ------------- | -------------------------------------------- |
| **Level**     | 0                                            |
| **Command**   | `i`                                          |
| **Arguments** | `x:2, y:2, st_ang:2, e_ang:2, radx:2 rady:2` |

**Format:** `!|i <x> <y> <st_ang> <e_ang> <radx> <rady>`

**Example:** `!|i1E18003G151Q`

**Attributes used:** Draw Color, Line Thick, Fill Color, Fill Pattern, Viewport _(v1.54)_

This command draws an "elliptical pie slice". It obeys all of the same commands as the Elliptical Arc command described above. The ends of the arc are connected to the Center-Point of the Arc with two straight lines. These two lines converge at the Center-Point. The interior of the Slice is filled with the current Fill Color and Pattern. The exterior (outline) of the Slice is drawn using the current drawing color and line thickness. The Line Pattern feature does not apply to this command.

## RIP_BEZIER

_Draw a bezier curve_

|               |                                                 |
| ------------- | ----------------------------------------------- |
| **Level**     | 0                                               |
| **Command**   | `Z`                                             |
| **Arguments** | `x1:2 y1:2 x2:2 y2:2 x3:2 y3:2 x4:2 y4:2 cnt:2` |

**Format:** `!|Z <x1> <y1> <x2> <y2> <x3> <y3> <x4> <y4> <cnt>`

**Example:** `!|Z0A0B0C0D0E0F0G0H1G`

**Attributes used:** Draw Color, Line Pattern, Line Thick, Write Mode, Viewport _(v1.54)_

This command provides customizable curves. Four control points are used to create the shape of the curve. The curves beginning point is at point (x1,y1) and it ends at (x4,y4). Points (x2,y2) and (x3,y3) are not necessarily on the curve, but are used to pull the curve in their direction. The diagram below indicates how points 2 and 3 can be utilized to form the desired curve. Note that points 2 and 3 are not actually on the curve, but points 1 and 4 are.

```text
                          X2

                         *****
                       **     ****
                      *           **            X4
                     *              **          *
                   X1                 *       **
                                       *    **
                                        ****

                                         X3
```

NOTE: Points 2 and 3 are not actually on the curve - points 1 and 4 are.

The last parameter of this command is the `<cnt>` parameter. This determines how many "segments" the curve should be drawn in. Each segment is in fact, a straight line. The more segments you allow, the smoother the curve may be. If a curve does not have a significant amount of "curviness" then a low "count" can improve performance of the curve drawing. Bezier Curves use "floating point" math internally for its processing. All parameters specified for this command are simple integers however.

Each segment of the Bezier Curve will be drawn using the current line style pattern and thickness. You can achieve some unusual effects using patterned lines for Bezier Curves. If XOR is active when you draw a bezier curve, you will find gaps at the intersections of each segment. _(v1.54)_

NOTE: This command is implemented in C using double floating point numbers. To make sure you do not have round-off or precision errors in another language, you should also use an equivalent floating point type. In Borland's Turbo Pascal, do not use REAL's, but use DOUBLE's instead! _(v1.54)_

## RIP_POLYGON

_Draw polygon in current color/line-style_

|               |                                         |
| ------------- | --------------------------------------- |
| **Level**     | 0                                       |
| **Command**   | `P`                                     |
| **Arguments** | `npoints:2, x1:2, y1:2, ... xn:2, yn:2` |

**Format:** `!|P <npoints> <x1> <y1> ... <xn> <yn>`

**Example:** `!|P03010105090905`

**Attributes used:** Draw Color, Line Pattern, Line Thick, Write Mode, Viewport _(v1.54)_

This command will draw a multi-sided closed polygon. The polygon is drawn using the current drawing color, line pattern and thickness. The `<npoints>` parameter is between 2 and 512 and indicates how many (x,y) coordinate pairs will follow, which is also the number of sides of the polygon. The polygon interior is not filled by RIP_POLYGON.

The polygon is enclosed by the last vertex between xn,yn and x1,y1. In other words, you do not have to connect the end to the beginning - it is automatically done for you.

## RIP_FILL_POLYGON

_Draw filled polygon in current color/fill pattern_

|               |                                         |
| ------------- | --------------------------------------- |
| **Level**     | 0                                       |
| **Command**   | `p`                                     |
| **Arguments** | `npoints:2, x1:2, y1:2, ... xn:2, yn:2` |

**Format:** `!|p <npoints> <x1> <y1> ... <xn> <yn>`

**Example:** `!|p03010105050909`

**Attributes used:** Draw Color, Line Pattern, Line Thick, Fill Color, Fill Pattern, Write Mode, Viewport _(v1.54)_

This command is identical to [RIP_POLYGON](#rip_polygon), except that the interior of the polygon is filled with the current fill color and fill pattern. The actual outline of the polygon is drawn using the current drawing color, line pattern and thickness.

NOTE: You will get unusual effects if the lines of the polygon overlap, creating a polygon with internal "gaps". (The rule is this: regions that are "inside" the polygon an even number of times due to overlap are NOT filled.) The interior fill does not utilize Write Mode, but the outline of the polygon does.

## RIP_POLYLINE

_Added in RIPscrip v1.54._

_Draw a Poly-Line (multi-faceted line)_

|               |                                         |
| ------------- | --------------------------------------- |
| **Level**     | 0                                       |
| **Command**   | `l`                                     |
| **Arguments** | `npoints:2, x1:2, y1:2, ... xn:2, yn:2` |

**Format:** `!|l <npoints> <x1> <y1> ... <xn> <yn>`

**Example:** `!|l03010105090905`

**Attributes used:** Draw Color, Line Pattern, Line Thick, Write Mode, Viewport

This command will draw a multi-faceted line. It is identical in nature to the [RIP_POLYGON](#rip_polygon) command above, except that the last point is NOT connected to the first point. Generally speaking, a Poly-Line is not an "enclosed area". It should not be filled unless you are very careful to close the shape so that it leaves a fillable area. The segments of the Poly-Line are drawn using the current drawing color, line pattern, thickness and Drawing Write Mode.

The `<npoints>` parameter is between 2 and 512 and indicates how many (x,y) coordinate pairs will follow, which is also the number of sides of the Poly-Line.

An example of a five sided (6 vertices) Poly-Line might be:

```text
               X                 P4         X
             P1 \                  X       / P6
                 \                /|      /
                  \              / |     /
                   \            /  |    /
                    \          /   |   /
                     X--------X    |  /
                   P2       P3     | /
                                 P5|/
                                   X
```

## RIP_FILL

_Flood fill screen area with current fill settings_

|               |                      |
| ------------- | -------------------- |
| **Level**     | 0                    |
| **Command**   | `F`                  |
| **Arguments** | `x:2, y:2, border:2` |

**Format:** `!|F <x> <y> <border>`

**Example:** `!|F25090F`

**Attributes used:** Fill Color, Fill Pattern, Viewport _(v1.54)_

This command performs a "flood fill" emanating from the given `<x,y>` point. The fill "oozes" in all directions up to `<border>` color, but the border itself is not changed. Whatever is inside the border that's not the border color gets changed to the current fill color and fill pattern. If the border color does not completely enclose the `<x,y>` point, the fill will continue to the edges of the viewport.

If the point on the screen that is chosen as the "fill point" is the same color as the fill border color, then no Fill Operation will be performed! This restriction is necessary because of some limitations of Flood Fill abilities in different hardware/software environments like Microsoft Windows, etc). _(v1.54)_

---

[◀ Prev: Text Output & Fonts](06-text-output.md) · [Contents](README.md) · [Next: Line & Fill Styles ▶](08-line-fill-styles.md)
