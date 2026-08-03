# Text Regions

[◀ Prev: Mouse Fields](09-mouse-fields.md) · [Contents](README.md) · [Next: Images & Icons ▶](11-images-icons.md)

This section covers the commands [RIP_BEGIN_TEXT](#rip_begin_text), [RIP_REGION_TEXT](#rip_region_text) and [RIP_END_TEXT](#rip_end_text).

## RIP_BEGIN_TEXT

*Define a rectangular text region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `T` |
| **Arguments** | `x1:2, y1:2, x2:2, y2:2, res:2` |

**Format:** `!|1T <x1> <y1> <x2> <y2> <res>`

**Example:** `!|1T00110011`

**Attributes used:** Viewport *(v1.54)*

This command defines a rectangular portion of the graphics viewport that is to display text, usually a long stream of text.  Following this command should be a number of [RIP_REGION_TEXT](#rip_region_text) commands with the text to be displayed.  The [RIP_END_TEXT](#rip_end_text) terminates this stream of text, something like this:

```text
RIP_BEGIN_TEXT
     RIP_REGION_TEXT
     RIP_REGION_TEXT
     RIP_REGION_TEXT
     :
     RIP_REGION_TEXT
RIP_END_TEXT
```

There must be at least one RIP_REGION_TEXT command in between the header and the footer.  There may be as many as needed.

These commands ignore the current font "direction"; all text is always displayed horizontally.

NOTE:  The "res" parameter is two bytes wide and is RESERVED for future use.

## RIP_REGION_TEXT

*Display a line of text in rectangular text region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `t` |
| **Arguments** | `justify:1` and `text-string` |

**Format:** `!|1t <justify> <text-string>`

**Example:** `!|1t1This is a text line to be justified`

**Attributes used:** Draw Color, Write Mode, Font Sizes, Viewport *(v1.54)*

A number of these commands may come sandwiched between the [RIP_BEGIN_TEXT](#rip_begin_text) and [RIP_END_TEXT](#rip_end_text) commands.  The `<text-string>` is already word-wrapped in such a way that it will fit inside the rectangular region based on the current font, font size, and drawing color.

There are two possible settings for the `<justify>` parameter:

| Justify | Description |
|---|---|
| 0 | Don't right/left justify.  Left-justify only |
| 1 | Perform right/left margin justification of this line of text. |

If a text line falls off the bottom of the region, it is discarded -- the rectangular Text Region does not scroll.

This command is intended to import some sort of text file document directly into a RIPscrip scene and format it nicely to fit inside a simple rectangular area.  If the `<justify>` parameter is set to a value of "1" for a given RIP_REGION_TEXT line, then that line will be justified to both the left and right margins (the RIP_BEGIN_TEXT boundaries).  This is so that the displayed text aligns on both sides with the invisible boundaries.  This "justification" is done by splitting each RIP_REGION_TEXT line up into chunks of word-groups, broken up at their "white-space" locations.  Each spacer is then padded by however many pixels are necessary to keep each spacer uniformly of approximately equal size.  Only enough spare pixels are added to make sure that the right-edge of the text region alignts with the right border of the boundary.  The result is a nicely formatted text block. *(v1.54)*

## RIP_END_TEXT

*End a rectangular text region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `E` |
| **Arguments** | \<none\> |

**Format:** `!|1E`

**Example:** `!|1E`

**Attributes used:** Viewport *(v1.54)*

This command indicates the end of a formatted text block.  Only one of these "end" commands is necessary for each block.

---

[◀ Prev: Mouse Fields](09-mouse-fields.md) · [Contents](README.md) · [Next: Images & Icons ▶](11-images-icons.md)
