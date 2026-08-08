# Command Reference Overview

[◀ Prev: Protocol Structure & Syntax](02-protocol-structure.md) · [Contents](README.md) · [Next: Window Commands ▶](04-window-commands.md)

The remainder of this specification details the RIPscrip command set. Each command has these aspects:

| Aspect | Meaning |
| --- | --- |
| **SYMBOL** | The symbolic constant that is referenced in the RIPscrip API Library code. This is the universal name for the command. |
| **LEVEL** | The Command Level. Sub-levels are represented with decimal points (eg, 1.3.5 for Level-1, Sub-level 3, Sub-Sub-level 5). This is for discussion purposes only. The decimal points are never part of the actual command. |
| **COMMAND** | The command type character identifying the command. |
| **ARGUMENTS** | The arguments or parameters for the command. Commands that do not require any arguments after the command type character are shown as `<none>`. Each argument is shown in the order it appears in the command, and is represented by a name. If an argument is numeric, it is followed by a width specifier indicating how many MegaNum digits the argument consists of (eg, `:2` means a 2-digit MegaNum, or a value between 0 and 1295). If an argument does not have a width specifier, it is by default a text argument, and should be the last argument on the line. If a command is variable length (see POLYGON), then it will appear with ellipses (`...`). |
| **FORMAT** | The format of the command, with the starting `!\|`, the level digits, the command type character, and the argument list, with the argument names in angle brackets. (These arguments are spaced apart, but these spaces never appear in the physical commands.) |
| **EXAMPLE** | An actual example of the RIPscrip command. |
| **DRAW COLOR** | If YES, then this command uses or affects the current Drawing Color. |
| **LINE PATRN** | If YES, then this command uses or affects the current Line Style Pattern. |
| **LINE THICK** | If YES, then this command uses or affects the current Line Style Thickness. |
| **FILL COLOR** | If YES, then this command uses or affects the current Fill Color. |
| **FILL PATRN** | If YES, then this command uses or affects the current Fill Pattern. |
| **WRITE MODE** | If YES, then this command will take advantage of the current Write Mode (eg, COPY, or XOR). |
| **FONT SIZES** | If YES, then this command uses or affects the current Font Size. |
| **VIEWPORT** | If YES, then this command adheres to the graphical viewport (draws inside it). Any RIPscrip command that adheres to the graphical viewport will be drawn (when received) only if the viewport is defined. If the viewport is not defined (0,0,0,0), then the command is parsed, but completely ignored. _(v1.54)_ |

In this Markdown edition, each command entry lists its Level, Command character, and Arguments in a table, followed by the Format and Example, and an **Attributes used** line naming only the aspects flagged YES in the original document.

## Command Index

### Window Commands

| Symbol | Level | Command |
| --- | --- | --- |
| [RIP_TEXT_WINDOW](04-window-commands.md#rip_text_window) | 0 | `w` |
| [RIP_VIEWPORT](04-window-commands.md#rip_viewport) | 0 | `v` |
| [RIP_RESET_WINDOWS](04-window-commands.md#rip_reset_windows) | 0 | `*` |
| [RIP_ERASE_WINDOW](04-window-commands.md#rip_erase_window) | 0 | `e` |
| [RIP_ERASE_VIEW](04-window-commands.md#rip_erase_view) | 0 | `E` |
| [RIP_GOTOXY](04-window-commands.md#rip_gotoxy) | 0 | `g` |
| [RIP_HOME](04-window-commands.md#rip_home) | 0 | `H` |
| [RIP_ERASE_EOL](04-window-commands.md#rip_erase_eol) | 0 | `>` |

### Colors & Attributes

| Symbol | Level | Command |
| --- | --- | --- |
| [RIP_COLOR](05-colors-and-attributes.md#rip_color) | 0 | `c` |
| [RIP_SET_PALETTE](05-colors-and-attributes.md#rip_set_palette) | 0 | `Q` |
| [RIP_ONE_PALETTE](05-colors-and-attributes.md#rip_one_palette) | 0 | `a` |
| [RIP_WRITE_MODE](05-colors-and-attributes.md#rip_write_mode) | 0 | `W` |

### Text Output & Fonts

| Symbol                                             | Level | Command |
| -------------------------------------------------- | ----- | ------- |
| [RIP_MOVE](06-text-output.md#rip_move)             | 0     | `m`     |
| [RIP_TEXT](06-text-output.md#rip_text)             | 0     | `T`     |
| [RIP_TEXT_XY](06-text-output.md#rip_text_xy)       | 0     | `@`     |
| [RIP_FONT_STYLE](06-text-output.md#rip_font_style) | 0     | `Y`     |

### Drawing Primitives

| Symbol | Level | Command |
| --- | --- | --- |
| [RIP_PIXEL](07-drawing-primitives.md#rip_pixel) | 0 | `X` |
| [RIP_LINE](07-drawing-primitives.md#rip_line) | 0 | `L` |
| [RIP_RECTANGLE](07-drawing-primitives.md#rip_rectangle) | 0 | `R` |
| [RIP_BAR](07-drawing-primitives.md#rip_bar) | 0 | `B` |
| [RIP_CIRCLE](07-drawing-primitives.md#rip_circle) | 0 | `C` |
| [RIP_OVAL](07-drawing-primitives.md#rip_oval) | 0 | `O` |
| [RIP_FILLED_OVAL](07-drawing-primitives.md#rip_filled_oval) | 0 | `o` |
| [RIP_ARC](07-drawing-primitives.md#rip_arc) | 0 | `A` |
| [RIP_OVAL_ARC](07-drawing-primitives.md#rip_oval_arc) | 0 | `V` |
| [RIP_PIE_SLICE](07-drawing-primitives.md#rip_pie_slice) | 0 | `I` |
| [RIP_OVAL_PIE_SLICE](07-drawing-primitives.md#rip_oval_pie_slice) | 0 | `i` |
| [RIP_BEZIER](07-drawing-primitives.md#rip_bezier) | 0 | `Z` |
| [RIP_POLYGON](07-drawing-primitives.md#rip_polygon) | 0 | `P` |
| [RIP_FILL_POLYGON](07-drawing-primitives.md#rip_fill_polygon) | 0 | `p` |
| [RIP_POLYLINE](07-drawing-primitives.md#rip_polyline) _(v1.54)_ | 0 | `l` |
| [RIP_FILL](07-drawing-primitives.md#rip_fill) | 0 | `F` |

### Line & Fill Styles

| Symbol | Level | Command |
| --- | --- | --- |
| [RIP_LINE_STYLE](08-line-fill-styles.md#rip_line_style) | 0 | `=` |
| [RIP_FILL_STYLE](08-line-fill-styles.md#rip_fill_style) | 0 | `S` |
| [RIP_FILL_PATTERN](08-line-fill-styles.md#rip_fill_pattern) | 0 | `s` |

### Mouse Fields

| Symbol | Level | Command |
| --- | --- | --- |
| [RIP_MOUSE](09-mouse-fields.md#rip_mouse) | 1 | `M` |
| [RIP_KILL_MOUSE_FIELDS](09-mouse-fields.md#rip_kill_mouse_fields) | 1 | `K` |

### Text Regions

| Symbol                                                | Level | Command |
| ----------------------------------------------------- | ----- | ------- |
| [RIP_BEGIN_TEXT](10-text-regions.md#rip_begin_text)   | 1     | `T`     |
| [RIP_REGION_TEXT](10-text-regions.md#rip_region_text) | 1     | `t`     |
| [RIP_END_TEXT](10-text-regions.md#rip_end_text)       | 1     | `E`     |

### Images & Icons

| Symbol                                              | Level | Command |
| --------------------------------------------------- | ----- | ------- |
| [RIP_GET_IMAGE](11-images-icons.md#rip_get_image)   | 1     | `C`     |
| [RIP_PUT_IMAGE](11-images-icons.md#rip_put_image)   | 1     | `P`     |
| [RIP_WRITE_ICON](11-images-icons.md#rip_write_icon) | 1     | `W`     |
| [RIP_LOAD_ICON](11-images-icons.md#rip_load_icon)   | 1     | `I`     |

### Buttons

| Symbol                                             | Level | Command |
| -------------------------------------------------- | ----- | ------- |
| [RIP_BUTTON_STYLE](12-buttons.md#rip_button_style) | 1     | `B`     |
| [RIP_BUTTON](12-buttons.md#rip_button)             | 1     | `U`     |

### Advanced Commands

| Symbol | Level | Command |
| --- | --- | --- |
| [RIP_DEFINE](13-advanced-commands.md#rip_define) | 1 | `D` |
| [RIP_QUERY](13-advanced-commands.md#rip_query) | 1 | `<escape>` |
| [RIP_COPY_REGION](13-advanced-commands.md#rip_copy_region) | 1 | `G` |
| [RIP_READ_SCENE](13-advanced-commands.md#rip_read_scene) | 1 | `R` |
| [RIP_FILE_QUERY](13-advanced-commands.md#rip_file_query) | 1 | `F` |
| [RIP_ENTER_BLOCK_MODE](13-advanced-commands.md#rip_enter_block_mode) | 9 | `<escape>` |
| [RIP_NO_MORE](13-advanced-commands.md#rip_no_more) | 0 | `#` |

---

[◀ Prev: Protocol Structure & Syntax](02-protocol-structure.md) · [Contents](README.md) · [Next: Window Commands ▶](04-window-commands.md)
