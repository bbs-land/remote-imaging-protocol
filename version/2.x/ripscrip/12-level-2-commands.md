# Level-2 Commands

[← Level-1 Commands](11-level-1-commands.md) · [Contents](README.md) · [Level-3 & Level-9 Commands →](13-level-3-9-commands.md)

---

Level-2 commands are "context" related commands. They are used when switching from one [data table](03-data-tables.md) to another, or are actually commands for manipulating actual data tables, or their entries. _(v2.A4)_

## RIP_DEFINE_PORT

_Define a drawing port_

_Added in RIPscrip v2.A3._

|               |                                                    |
| ------------- | -------------------------------------------------- |
| **Level**     | 2                                                  |
| **Command**   | `P`                                                |
| **Arguments** | `port-num:1 x0:XY y0:XY x1:XY y1:XY flags:4 res:4` |

**Format:** `!|2P <port-num> <x0> <y0> <x1> <y1> <flags> <res>`

**Example:** `!|2P300002F2F00010000`

**Attributes used:** viewport, port, base math (current setting)

This command physically defines a new graphical [drawing port](02-drawing-ports.md). The port can be either onscreen (a video port) or an offscreen bitmap port (a Clipboard Port). When defined, the newly created port becomes the current graphical drawing port (if the `<flags>` say to - see below).

If the `<port-num>` that you are trying to define is already defined, and the port isn't protected, it is redefined (ie, deleted then re-created using the new parameters). If the port is defined and protected, then the port definition operation will fail and the port will be switched back to the screen port (#0) as previously described.

The `<port-num>` parameter determines which entry in the master Port Table you wish to define. When working with ports, you need to be concerned about the port number you're working with - that's the location of the port, and that's how you access it when you need to switch ports - by port number. You are allows up to 36 separate grpahical ports (0-35 in decimal). Port #0 cannot be re-defined because it is always defined as the Screen Port. You can however, alter the viewport for Port #0 to make a sub-area on the screen to draw in (and clip). The viewport for Port #0 may also be deactivated as can the viewport on any other graphical Port. _(v2.A4)_

To create a port, you need to specify the upper-left coordinate corner of the port and the lower-right coordinate of the rectangle bounding the port. The configuration of coordinates is different depending on the type of graphical port as declared by the `<flags>` parameter (see below). For Video Ports (onscreen ports), the `<x0>` and `<y0>` parameters define the upper-left corner of the port on the video screen, and the `<x1>` and `<y1>` values define the lower-right corner of the port. Together, they define the pixel width and height of the port (this adheres to global world coordinate systems). For Clipboard Ports (offscreen bitmap ports), you cannot have an upper-left corner - so the `<x0>` and `<y0>` parameters must be set to zero.

Once defined, the viewport for the given port is defined as the size of the port itself (you may change it with a [RIP_VIEWPORT](10-level-0-commands-s-w.md#rip_viewport) command later if you wish). This makes it so that the entire port may be drawn into. If you need to define a sub-area of the port as a clipping boundary, modify the viewport for that graphical port.

The `<flags>` parameter for this command allows you to make some important distinctions to the port. The possible port flags are OR'd together to create one final number where each bit represents some flag value. The possible flag values and what they mean are described as follows:

| Value | Description |
| --- | --- |
| 1 | Port is an offscreen (Clipboard) port. When this flag is specified, the `<x0>` and `<y0>` parameter are assumed to be zero (even if they aren't set to 0, they are treated as if they were). When a port is declared as a Clipboard Port, all graphical operations are performed to an offscreen bitmap port and do not appear on the screen (without you specifically using a port copy operation). If this flag is omitted, then the port is treated as a Video Port and any graphics drawn to the port will show up on the screen immediately (providing the viewport for this port isn't deactivated). |
| 2 | Port is made the active drawing port immediately upon completion of this command. This has the net effect of creating the port and immediately switching to it. Without this flag, you would have to use a [RIP_SWITCH_PORT](#rip_switch_port) command to draw directly into this port. |
| 4 | Deactivate this port's viewport immediately upon creation. This has the net effect of creating the port and setting the viewport to a deactivated status. _(v2.A4)_ |
| 8 | The newly defined port is immediately protected. This means that the port cannot be deleted without explicitly unprotecting the port. |

> **NOTE:** The `<res>` parameter is reserved for future use and should be set to "0000" for future compatibility with later RIPscrip revisions.

## RIP_DELETE_PORT

_Deletes a specific port definition_

_Added in RIPscrip v2.A3._

|               |                                |
| ------------- | ------------------------------ |
| **Level**     | 2                              |
| **Command**   | `p`                            |
| **Arguments** | `port_num:1 dest_port:1 res:2` |

**Format:** `!|2s <port_num> <dest_port> <res>`

**Example:** `!|2s500000`

**Attributes used:** port, base math (current setting)

This command physically deletes one or more graphical ports. The exact port depends on the `<port_num>` parameter. If the port number specified is between 1-35, then the given port is deleted (providing of course that it is unprotected). If you attempt to deleted a protected port, then this command will fail. If you attempt to delete port #0 (the screen port), then you will be activating a special case of this command - all unprotected ports from 1-35 are automatically deleted. This is a good garbage collection command mode when you want to start with a new slate of ports without the overhead of sending many delete port commands.

The `<dest_port>` parameter defines the port number that will be switched to after the delete operation is finished. Typically, this will be set to port #0 (switch to screen port), but it need not be. If the destination port is undefined, then it will automatically switch to port #0. If you instruct this command to delete all ports, then the destination port may or may not be set to the screen port - it all depends on whether or not the destination port both exists, and is protected - remember, a protected port cannot be deleted with this command without first being unprotected. So, if you delete all ports and switch to another port that isn't port #0, and that port is protected, then that protected port is selected as the new active port. If however, that port doesn't exist (ie, it did exist at the beginning of this command, but wasn't protected), then the screen port #0 will be selected.

> **NOTE:** The `<res>` parameter is reserved for future use and should be set to "00" for future compatibility with later RIPscrip revisions.

## RIP_PORT_COPY

_Copies graphics data from one port to another_

_Added in RIPscrip v2.A3._

|  |  |
| --- | --- |
| **Level** | 2 |
| **Command** | `C` |
| **Arguments** | `source_port:1 sx0:XY sy0:XY sx1:XY sy1:XY dest-port:1 dx0:XY dy0:XY dx1:XY dy1:XY write-mode:1 res:5` |

**Format:** `!|2C <source_port> <sx0> <sy0> <sx1> <sy1> <dest_port> <dx0> <dy0> <dx1> <dy1> <write_mode> <res>`

**Example:** `!|2C300002020500002020100000`

**Attributes used:** viewport, port, base math (current setting)

This command copies graphics data from one port to another (or possibly even the same port). The area copied is a rectangular area based on the (sx0,sy0)..(sx1,sy1) parameters in the `<source_port>` and the (dx0,dy0)..(dx1,dy1) parameters in the `<dest_port>`. The `<source_port>` and `<dest_port>` parameters specify which port numbers are to be used for the source and destination respectively. They may be set to the same port number which signifies that a bit-blit operation is to occur from one location of the current port to another. Both the source and destination ports are specified as port numbers from 0-35. If either port doesn't exist (or is deactivated) then this command will do nothing.

The source port parameters (sx0,sy0)..(sx1,sy1) define the upper-left and lower-right corners of the rectangle in the source port that is to be the source image data. If all four parameters are set to zero, then the entire source port's viewport is used as the image. These parameters determine the portion of the source port that is to be used to copy to the destination port. The image data is taken from within the port's currently defined viewport. If the (sx1,sy1) parameters takes the image out of the bounds of the source port's viewport, then the values are adjusted to make it fit inside the viewport.

The destination port parameters (dx0,dy0)..(dx1,dy1) define the destination rectangle in the destination port that will receive the graphical data. The (dx0,dy0) parameters define the upper-left corner of the destination location of the bitmap data. If the (dx1,dy1) parameters are set to zero, then the image is copied from the source port verbatim - with the exact same pixel dimensions as the source image rectangle. If these two values are not set to zero, then scaling of the source image may be performed. Whether or not scaling is performed or not depends on one fact - does the physical pixel size (height and width) of the source image rectangle exactly match the pixel size (height and width) of the destination port rectangle? If the pixel dimensions match exactly, then no scaling is performed on the rectangle. However, if the pixel size in either the vertical or horizontal directions are different, then the image is scaled to fit in the desired rectangle. When we talk about "pixel size", we are referring to the low-level device pixel size of the image. This means that we take into account world coordinate translation and viewport coordinate translations first, then compare the raw pixel sizes of the source and destination rectangles. If they differ, then we perform scaling of the bitmap. Otherwise we copy it over verbatim. If the destination rectangle coordinates are set all to zeros, then the image is placed at (0,0) in the destination port's viewport and the image will fill the entire port's viewport.

The `<write_mode>` parameter determines the Raster Transfer mode that is used to write the image data to the destination rectangle. The possible value and their various effects on the destination image are as follows:

| Mode | Description                                          | Logical |
| ---- | ---------------------------------------------------- | ------- |
| 0    | Copy the image to the port verbatim                  | COPY    |
| 1    | Exclusive-OR image with the one already on the port  | XOR     |
| 2    | Logically OR image with the one already on the port  | OR      |
| 3    | Logically AND image with the one already on the port | AND     |
| 4    | Copy the inverse of the image onto the port          | NOT     |

Working with a port copy operation, the source rectangle for the image is contained in the source port number. The destination rectangle is based in the destination port. If either of these rectangles extend outside of the respective ports' viewports, then they are adjusted accordingly - possibly scaling the image when it wasn't intended.

> **NOTE:** The `<res>` parameter is reserved for future use and should be set to "00000" for future compatibility with later RIPscrip revisions.

## RIP_PORT_WRITE

_Writes port image to a disk-based bitmap file_

_Added in RIPscrip v2.A3._

|               |                                                     |
| ------------- | --------------------------------------------------- |
| **Level**     | 2                                                   |
| **Command**   | `W`                                                 |
| **Arguments** | `port_num:1 x0:XY y0:XY x1:XY y1:XY res:4 filename` |

**Format:** `!|2W <port_num> <x0> <y0> <x1> <y1> <res> <filename>`

**Example:** `!|2W5000020200000FILENAME.BMP`

**Attributes used:** viewport, port, base math (current setting)

This command physically takes the contents of the specific graphics port and writes it to a disk-based bitmap file (eg, a .BMP file). The specific area of the given port that is written to the disk is based on the (x0,y0)..(x1,y1) parameters. If all of these parameters are set to zeros, then the entire port's viewport is written to the desired bitmap file. If the (x1,y1) paraemters are set to zero, then the lower-right corner of the image is taken to be the lower-right corner of the port's viewport. The (x0,y0) parameters determine the upper-left corner of the image's location in the given graphics port. You could use any of the following three variations:

| X0 | Y0 | X1 | Y1 | Description |
| --- | --- | --- | --- | --- |
| 0 | 0 | 0 | 0 | Grab the entire port's viewport for the image to be saved. |
| 25 | 25 | 0 | 0 | Grab the image area from (25,25) to the lower-right of the port's viewport as the image data to be saved. |
| 25 | 25 | 100 | 100 | Save the image data from (25,25) to (100,100) in the given graphics port to the destination bitmap file. |

The `<port_num>` parameter determines which graphics port is to be the source of the image data. If that port is undefined, or the X/Y parameters place the graphics data off the port's viewport, then this command is ignored. If the (x0,y0) parameters fall inside the port's viewport, but the (x1,y1) parameters do not, then the lower-left corner is adjusted appropriately (as if the user specified (0,0) as the lower-left corner - see above).

> **NOTE:** The `<res>` parameter is reserved for future use and should be set to "0000" for future compatibility with later RIPscrip revisions.

## RIP_SET_REFRESH

_Sets a sequence to send host to refresh display_

_Added in RIPscrip v2.A1._

|               |                        |
| ------------- | ---------------------- |
| **Level**     | 2                      |
| **Command**   | `R`                    |
| **Arguments** | `res:4 refresh_string` |

**Format:** `!|2R <res> <refresh_string>`

**Example:** `!|2R0000^m`

**Attributes used:** base math (current setting)

This command sets a host command sequence in the terminal that can be sent to the host to refresh the current display screen. This refresh option would be chosen by the user to redisplay the screen in case line noise or some kind of corruption caused the screen to become distorted or corrupted. This can only be altered (or cleared) by a RIP_SET_REFRESH command or via a reset. To completely clear a refresh state, call this command with a `$OFF$` text variable as its host command or use the text variable `$NOREFRESH$`.

The host command portion of this sequence can contain any kind of host command information including control characters, [pick lists](15-local-playback-popup-lists.md), text variables or many other things (but not [templates](16-templates.md)).

## RIP_SWITCH_BUTTON_STYLE

_Switches to a new button style_

_Added in RIPscrip v2.A4._

|               |                |
| ------------- | -------------- |
| **Level**     | 2              |
| **Command**   | `B`            |
| **Arguments** | `bstyle_num:2` |

**Format:** `!|2B <bstyle_num>`

**Example:** `!|2B04`

**Attributes used:** base math (current setting)

This command physically switches the currently active button style. Any subsequent button operations will use this new button style.

You are allowed up to 36 separate button styles to switch between. Possible button style slot numbers are from 0-35 (0-Z).

When a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) or a proper [RIP_HEADER](09-level-0-commands-g-r.md#rip_header) command is executed, then all unprotected button style slots are reset to the default button style. Also, the current button style is reset to button style number 0.

## RIP_SWITCH_ENVIRONMENT

_Switches to a new environment_

_Added in RIPscrip v2.A4._

|               |             |
| ------------- | ----------- |
| **Level**     | 2           |
| **Command**   | `E`         |
| **Arguments** | `env_num:2` |

**Format:** `!|2E <env_num>`

**Example:** `!|2E04`

**Attributes used:** base math (current setting)

This command physically switches the currently active environment. Any subsequent RIPscrip operations will use this environment.

You are allowed up to 36 separate environments to switch between. Possible environment slot numbers are from 0-35 (0-Z).

When a RIP_RESET_WINDOWS or a proper RIP_HEADER command is executed, then all unprotected environment slots are reset to the default environment settings. Also, the current environment is reset to environment number 0.

## RIP_SWITCH_PALETTE

_Switches to a new color palette_

_Added in RIPscrip v2.A3._

|               |                 |
| ------------- | --------------- |
| **Level**     | 2               |
| **Command**   | `A`             |
| **Arguments** | `palette_num:2` |

**Format:** `!|2A <palette_num>`

**Example:** `!|2A04`

**Attributes used:** base math (current setting)

This command physically switches the currently active drawing color palette. When this happens, any graphics currently on-screen will immediately change colors based on the new palette (providing the terminal is running in a hardware mode that allows for color palettes). If the terminal is running in a mode that doesn't allow for color palettes (ie, 24-bit mode, etc), then only the internal color lookup table is changed for subsequent color palette mapping mode operations.

You are allowed up to 36 separate color palettes to switch between. Possible palette slot numbers are from 0-35 (0-Z).

When a RIP_RESET_WINDOWS or a proper RIP_HEADER command is executed, then all unprotected color palette slots are reset to the default color palette. Also, the current active color palette is reset to palette number 0.

## RIP_SWITCH_PORT

_Switches to a new port_

_Added in RIPscrip v2.A3._

|               |                            |
| ------------- | -------------------------- |
| **Level**     | 2                          |
| **Command**   | `s`                        |
| **Arguments** | `port-num:1 flags:2 res:3` |

**Format:** `!|2s <port-num> <flags> <res>`

**Example:** `!|2s500000`

**Attributes used:** port, base math (current setting)

This command physically changes whichever port is currently active. You may specify a port number from 0-35 (36 ports total). If the designated port doesn't exist, then the port is automatically switched to port #0 (the screen's port). This has the side-effect that when the operation fails (ie, the destination port doesn't exist), all subsequent graphics operations would be sent to the screen which would give you a visual impression that something is very wrong.

When the new port is activated, all subsequent drawing operations are performed on it instead of the previously defined port.

The `<flags>` parameter for this command allows you to specify one or more things that can alter the switching operation. The allowable flag values and their meanings are described as follows:

| Value | Description                                               |
| ----- | --------------------------------------------------------- |
| 1     | The destination port is immediately protected.            |
| 2     | The destination port is immediately un-protected          |
| 4     | The port being switched from is immediately protected.    |
| 8     | The port being switched from is immediately un-protected. |

If you try to switch to the same port that is already in use, nothing happens except for possibly the activation of one or more flag values. This allows you to protect or unprotect a port without switching ports. Port #0 cannot be either protected nor unprotected.

> **NOTE:** The `<res>` parameter is reserved for future use and should be set to "000" for future compatibility with later RIPscrip revisions.

## RIP_SWITCH_TEXT_WINDOW

_Switch to another Text Window (activate)_

_Added in RIPscrip v2.A0._

|               |                      |
| ------------- | -------------------- |
| **Level**     | 2                    |
| **Command**   | `T`                  |
| **Arguments** | `window_num:1 res:1` |

**Format:** `!|2T <window_num> <res>`

**Example:** `!|2T30`

**Attributes used:** base math (current setting)

In RIPscrip, you are allowed to have up to 36 separate [text windows](06-color-audio-text.md) on screen simultaneously (0-9, A-Z). These windows maintain their own "current ANSI attributes", cursor position, text window dimensions/location, cursor ON/OFF status and whether the window is actived or deactivated.

When you switch to another window, if that window hasn't been defined yet, then the current window definition is copied to the new window slot. When you switch to another window, the cursor is hidden in the previous window and re-drawn in the new window (if it was visible the last time the window was used).

If you switch to a window that is "deactivated", it will hide the cursor from whatever window was previously active and then execute the proper deactivate text window sequence for the newly selected text window slot. _(v2.A4)_

When a RIP_RESET_WINDOWS command is acted upon, any previously defined text window slots are erased and Window #0 is set to full screen mode (using the User's selected MicroANSI font), and the current window number is set to window #0.

## RIP_SWITCH_STYLE

_Switches to a new Drawing Style Context_

_Added in RIPscrip v2.A0._

|               |                     |
| ------------- | ------------------- |
| **Level**     | 2                   |
| **Command**   | `Y`                 |
| **Arguments** | `style_num:1 res:1` |

**Format:** `!|2Y <style_num> <res>`

**Example:** `!|2YG0`

**Attributes used:** draw color, back color, line style, fill style, write mode, font style, viewport, base math (current setting)

This command switches to a particular "drawing style". A drawing style is a combination of the following graphical settings: _(list as of v2.A1)_

- Current drawing color
- Current background drawing color
- Current fill pattern number (or user-defined fill pattern)
- Current fill color
- Current line pattern number (or user-defined line pattern)
- Line pattern odd drawing rule (see [RIP_LINE_STYLE](09-level-0-commands-g-r.md#rip_line_style))
- Current mouse cursor style number
- Current font number (or font name for extended fonts)
- Current font size, orientation and horizontal/vertical alignment
- Current write mode (raster/transfer operation)
- Current color mode (palette or direct RGB)
- Current X/Y location (used with [RIP_TEXT](10-level-0-commands-s-w.md#rip_text) commands)
- Current image mode settings (see [RIP_IMAGE_STYLE](11-level-1-commands.md#rip_image_style))

By default, all commands that alter the above modes will change the settings of Style #0. By switching Style Contexts, you are given the ability to alter which Style Context is being manipulated. After a given Style has been defined, you can quickly switch from one Style to another without having to re-transmit each and every mode command to recreate the environment.

The first time you switch to a new Style Context, the contents of the current Style are copied into the newly selected Style. You are allowed up to 36 separate Styles (0-9, A-Z).

Whenever a RIP_RESET_WINDOWS function is executed, the contents of all Styles are reset to a default status which is the Status of the drawing environment after Resetting is complete.

---

[← Level-1 Commands](11-level-1-commands.md) · [Contents](README.md) · [Level-3 & Level-9 Commands →](13-level-3-9-commands.md)
