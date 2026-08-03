# Line & Fill Styles

[◀ Prev: Drawing Primitives](07-drawing-primitives.md) · [Contents](README.md) · [Next: Mouse Fields ▶](09-mouse-fields.md)

This section covers the commands that control line and fill styling: [RIP_LINE_STYLE](#rip_line_style), [RIP_FILL_STYLE](#rip_fill_style), and [RIP_FILL_PATTERN](#rip_fill_pattern).

## RIP_LINE_STYLE

*Defines a line style and thickness*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `=` |
| **Arguments** | `style:2, user_pat:4, thick:2` |

**Format:** `!|= <style> <user_pat> <thick>`

**Example:** `!|=01000001`

**Attributes used:** Line Pattern, Line Thick

This command establishes the current line pattern and thickness for many subsequent graphics primitive commands.  There are four built-in line styles plus provisions for custom line patterns.

| Style | Description | Binary | Hex |
|---|---|---|---|
| 00 | Normal, Solid Line | 1111111111111111 | FFFF |
| 01 | Dotted Line | 0011001100110011 | 3333 |
| 02 | Centered Line | 0001111000111111 | 1E3F |
| 03 | Dashed Line | 0001111100011111 | 1F1F |
| 04 | Custom Defined line (see about `<user_pat>` below) | | |

*(Binary and Hex columns: v1.54)*

| Thick | Description |
|---|---|
| 01 | Lines are 1 pixel wide |
| 03 | Lines are 3 pixels wide |

If the `<style>` is set to a value of 4 (custom pattern), then the `<user_pat>` parameter is used as a 16-bit representation of the pixels in the line pattern.  For example:

| Repeating Pattern | Binary Coding | Hex | Decimal | MegaNum |
|---|---|---|---|---|
| `- - - - - - - -` | 1010101010101010 | AAAA | 43690 | 0XPM |
| `----    ----` | 1111000011110000 | F0F0 | 61680 | 1BLC |

So, the most-significant-bit of `<user_pat>` is toward the starting point of the line or border that uses this fill pattern.  If the `<style>` parameter is not 4, then the `<user_pat>` parameter is ignored.

## RIP_FILL_STYLE

*Set current fill style (predefined) & fill color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `S` |
| **Arguments** | `pattern:2, color:2` |

**Format:** `!|S <pattern> <color>`

**Example:** `!|S050F`

**Attributes used:** Fill Color, Fill Pattern

This command defines the current fill pattern and fill color for use in subsequent graphics fill operations.  There are twelve (12) predefined fill patterns.  They are:

| Pattern | Description | Example | Misc |
|---|---|---|---|
| 00 | Fill with background color | | (color #0) |
| 01 | Solid Fill | | (fill color) |
| 02 | Line Fill | `-----------` | (thick lines) |
| 03 | Light Slash Fill | `/  /  /  /` | (thin lines) |
| 04 | Normal Slash Fill | `// // // //` | (thick lines) |
| 05 | Normal Backslash Fill | `\\ \\ \\ \\` | (thick lines) |
| 06 | Light Backslash Fill | `\  \  \  \` | (thin lines) |
| 07 | Light Hatch Fill | `###########` | (thin lines) |
| 08 | Heavy Cross Hatch Fill | `XXXXXXXXXXX` | (thin lines) |
| 09 | Interleaving Line Fill | `+-+-+-+-+-+` | (thin lines) |
| 0A | Widely spaced dot fill | `. : . : . :` | (pixels only) |
| 0B | Closely spaced dot fill | `:::::::::::` | (pixels only) |

The `<color>` parameter is the fill color for subsequent fill commands. The "active" pixels of the pattern become this color.  The "inactive" pixels become the current background color (color 00, typically black).  Fill pattern 00 will set the entire fill area to the background color.  (In this special case, the fill color doesn't matter.)

The following twelve diagrams show visually what each fill pattern appears like.  Next to each diagram are the eight numerical values which represent the monochrome bit-pattern of each line of each pattern.  Numbers are shown in Hexadecimal (base 16), decimal (base 10) and MegaNum (base 36): *(v1.54)*

```text
       BACKGROUND FILL                      SOLID FILL

     --------   HEX DEC MEGA          --------   HEX DEC MEGA
    |        |  00    0  00          |########|  FF  255  73
    |        |  00    0  00          |########|  FF  255  73
    |        |  00    0  00          |########|  FF  255  73
    |        |  00    0  00          |########|  FF  255  73
    |        |  00    0  00          |########|  FF  255  73
    |        |  00    0  00          |########|  FF  255  73
    |        |  00    0  00          |########|  FF  255  73
    |        |  00    0  00          |########|  FF  255  73
     --------                         --------
        00                               01



           LINE FILL                     LIGHT SLASH FILL

     --------   HEX DEC MEGA          --------   HEX DEC MEGA
    |########|  FF  255  73          |       #|  01    1  01
    |########|  FF  255  73          |      # |  02    2  02
    |        |  00    0  00          |     #  |  04    4  04
    |        |  00    0  00          |    #   |  08    8  08
    |########|  FF  255  73          |   #    |  10   16  0G
    |########|  FF  255  73          |  #     |  20   32  0W
    |        |  00    0  00          | #      |  40   64  1S
    |        |  00    0  00          |#       |  80  128  3K
     --------                         --------
        02                               03




        NORMAL SLASH FILL               LIGHT BACKSLASH FILL

     --------   HEX DEC MEGA          --------   HEX DEC MEGA
    |###     |  E0  224  68          |####    |  F0  240  60
    |##     #|  C1  193  5D          | ####   |  78  120  3C
    |#     ##|  83  131  3N          |  ####  |  3C   60  1O
    |     ###|  07    7  07          |   #### |  1E   30  0U
    |    ### |  0E   15  0F          |    ####|  0F   15  0F
    |   ###  |  1C   28  0S          |#    ###|  87  135  3R
    |  ###   |  38   56  1K          |##    ##|  C3  195  5F
    | ###    |  70  112  34          |###    #|  E1  225  69
     --------                         --------
        04                               05




      LIGHT BACKSLASH FILL               LIGHT HATCH FILL

     --------   HEX DEC MEGA          --------   HEX DEC MEGA
    |# #  # #|  A5  165  4L          |########|  FF  255  73
    |## #  # |  D2  210  5U          |#   #   |  88  136  3S
    | ## #  #|  69  105  2X          |#   #   |  88  136  3S
    |# ## #  |  B4  180  50          |#   #   |  88  136  3S
    | # ## # |  5A   90  2I          |########|  FF  255  73
    |  # ## #|  2D   45  19          |#   #   |  88  136  3S
    |#  # ## |  96  150  46          |#   #   |  88  136  3S
    | #  # ##|  4B   75  23          |#   #   |  88  136  3S
     --------                         --------
        06                               07




     HEAVY CROSS HATCH FILL           INTERLEAVING LINE FILL

     --------   HEX DEC MEGA          --------   HEX DEC MEGA
    |#      #|  81  129  3L          |##  ##  |  CC  204  5O
    | #    # |  42   66  1U          |  ##  ##|  33   51  1F
    |  #  #  |  24   36  10          |##  ##  |  CC  204  5O
    |   ##   |  18   24  0O          |  ##  ##|  33   51  1F
    |   ##   |  18   24  0O          |##  ##  |  CC  204  5O
    |  #  #  |  24   36  10          |  ##  ##|  33   51  1F
    | #    # |  42   66  1U          |##  ##  |  CC  204  5O
    |#      #|  81  129  3L          |  ##  ##|  33   51  1F
     --------                         --------
        08                               09




     WIDELY SPACED DOT FILL           CLOSELY SPACED DOT FILL

     --------   HEX DEC MEGA          --------   HEX DEC MEGA
    |#       |  80  128  3K          |#   #   |  88  136  3S
    |        |  00    0  00          |        |  00    0  00
    |    #   |  08    8  08          |  #   # |  22   34  0Y
    |        |  00    0  00          |        |  00    0  00
    |#       |  80  128  3K          |#   #   |  88  136  3S
    |        |  00    0  00          |        |  00    0  00
    |    #   |  08    8  08          |  #   # |  22   34  0Y
    |        |  00    0  00          |        |  00    0  00
     --------                         --------
        0A                               0B
```

## RIP_FILL_PATTERN

*Set user-definable (custom) fill pattern/color*

| | |
|---|---|
| **Level** | 0 |
| **Command** | `s` |
| **Arguments** | `c1:2 c2:2 c3:2 c4:2 c5:2 c6:2 c7:2 c8:2 col:2` |

**Format:** `!|s <c1> <c2> <c3> <c4> <c5> <c6> <c7> <c8> <col>`

**Example:** `!|s11223344556677880F`

**Attributes used:** Fill Color, Fill Pattern

This command allows you to specify a user-defined, custom Fill Pattern.  This pattern supersedes the predefined patterns of [RIP_FILL_STYLE](#rip_fill_style).  A custom fill pattern is an 8x8 pixel array defining which pixels should be drawn in the current fill color (as set by the `<col>` parameter here).  The other pixels in the fill area are set to the current background color (color 00, typically black).

Each of the eight parameters of this command, `<c1>` through `<c8>` represent bit-patterns for a line of the 8x8 pixel array.  Each line is comprised of 8 pixels.  The value of each parameter is the binary representation of these 8 pixels as follows:

| Bit | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---|---|---|---|---|---|---|---|---|
| c1 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| c2 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| c3 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| c4 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| c5 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| c6 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| c7 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| c8 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |

So, c1 is the top, and the most-significant bit is to the left.

NOTE:  The RIP_FILL_STYLE (predefined fill patterns) and this RIP_FILL_PATTERN (custom fill patterns) completely override each other's effects.

---

[◀ Prev: Drawing Primitives](07-drawing-primitives.md) · [Contents](README.md) · [Next: Mouse Fields ▶](09-mouse-fields.md)
