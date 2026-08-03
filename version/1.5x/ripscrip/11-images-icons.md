# Images & Icons

[◀ Prev: Text Regions](10-text-regions.md) · [Contents](README.md) · [Next: Buttons ▶](12-buttons.md)

This section covers the commands [RIP_GET_IMAGE](#rip_get_image), [RIP_PUT_IMAGE](#rip_put_image), [RIP_WRITE_ICON](#rip_write_icon) and [RIP_LOAD_ICON](#rip_load_icon).

## RIP_GET_IMAGE

*Copy rectangular image to clipboard (as icon)*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `C` |
| **Arguments** | `x0:2, y0:2, x1:2, y1:2, res:1` |

**Format:** `!|1C <x0> <y0> <x1> <y1> <res>`

**Example:** `!|1C001122330`

**Attributes used:** Viewport *(v1.54)*

This command instructs the terminal program to copy the rectangular region defined by (x0,y0) to (x1,y1) onto an internal Clipboard for future use.  This combined with the Paste Clipboard command can provide an extremely powerful and efficient mechanism to avoid baud-rate bandwidth limitations.  The (x0,y0) parameter MUST specify the upper-left corner of the region and the (x1,y1) parameter MUST specify the lower-right corner.  If the indicated coordinates are in anyway invalid, the command is ignored.  The Clipboard is completely overwritten by this command (the previous contents are lost).

NOTE:  The `<res>` parameter is RESERVED FOR FUTURE USE and should be set to zero.

## RIP_PUT_IMAGE

*Pastes the clipboard contents onto the screen*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `P` |
| **Arguments** | `x:2, y:2, mode:2, res:1` |

**Format:** `!|1P <x> <y> <mode> <res>`

**Example:** `!|1P0011010`

**Attributes used:** Viewport *(v1.54)*

This command takes the contents of the Clipboard (if any) and pastes the image onto the screen starting at the upper-left corner of the image of (x,y).  If the right edge of the image would go off-screen, the paste command is ignored.  The Height and Width of the clipboard image is recorded on the Clipboard, so this command doesn't need to supply it.

The `<mode>` parameter defines "how" the image will be pasted on the screen:

| Mode | Description | Logical |
|---|---|---|
| 00 | Paste the image on-screen normally | (COPY) |
| 01 | Exclusive-OR  image with the one already on screen | (XOR) |
| 02 | Logically OR  image with the one already on screen | (OR) |
| 03 | Logically AND image with the one already on screen | (AND) |
| 04 | Paste the inverse of the image on the screen | (NOT) |

NOTE:  The 1-byte `<res>` parameter is RESERVED FOR FUTURE USE and should be set to zero.

## RIP_WRITE_ICON

*Write contents of the clipboard (icon) to disk*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `W` |
| **Arguments** | `res:1, filename` |

**Format:** `!|1W <res> <filename>`

**Example:** `!|1W0filename.icn`

**Attributes used:** none

This command takes the contents of the Clipboard and writes it to a disk file.  This Icon file can be loaded later with a [RIP_LOAD_ICON](#rip_load_icon) command and stamped on the screen.

The command instructs the terminal to store an Icon on the terminal's disk drive, or on some appropriate storage media.  Path or sub-directory information is not allowed in the filename portion of the command.  If the clipboard is nonexistent (i.e., at the beginning of a scene), this command is ignored.  If an Icon by the same name already exists on disk, it is overwritten.

NOTE:  The `<res>` parameter is RESERVED FOR FUTURE USE and should be set to zero.

## RIP_LOAD_ICON

*Loads and displays a disk-based icon to screen*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `I` |
| **Arguments** | `x:2, y:2, mode:2, clipboard:1, res:2, filename` |

**Format:** `!|1I <x> <y> <mode> <clipboard> <res> <filename>`

**Example:** `!|1I001101010button.icn`

**Attributes used:** Viewport *(v1.54)*

This command instructs the terminal to read an Icon from disk and display it at the given upper-left (x,y) location.  If the width or height of the Icon would make it go off the right or left edge of the screen, the Icon will not be displayed.  The `<mode>` parameter defines the modes in which the Icon will be displayed on the screen.  The modes are identical to the [RIP_PUT_IMAGE](#rip_put_image) command, and are as follows:

The .ICN file extension does not need to be included as part of the filename.  If omitted, it will automatically be appended to the filename.  If an extension is provided, it will be used verbatim.

| Mode | Description | Logical |
|---|---|---|
| 00 | Paste the image on-screen normally | (COPY) |
| 01 | Exclusive-OR  image with the one already on screen | (XOR) |
| 02 | Logically OR  image with the one already on screen | (OR) |
| 03 | Logically AND image with the one already on screen | (AND) |
| 04 | Paste the inverse of the image on the screen | (NOT) |

If the `<clipboard>` parameter is 1, then the image pasted on screen AND also copied onto the Clipboard.  If 0, it is simply pasted on the screen.

The `<filename>` parameter must not contain any sub-directory or path information and must specify a valid Icon file name.  If the Icon cannot be located or an error occurs on the disk, then a box should be displayed on screen indicating that the given Icon File could not be loaded.  This visual prompt indicates that something is amiss to the end-user.

NOTE:  The 2-byte `<res>` parameter is RESERVED FOR THE FUTURE and unlike many other previously mentioned reserved parameters, should be set to "10".

---

[◀ Prev: Text Regions](10-text-regions.md) · [Contents](README.md) · [Next: Buttons ▶](12-buttons.md)
