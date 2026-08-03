# Level-1 Commands

[◀ Prev: Level-0 Commands (S–W)](10-level-0-commands-s-w.md) | [Contents](README.md) | [Next: Level-2 Commands ▶](12-level-2-commands.md)

Level-1 commands are basic user-interface commands, and higher level graphical constructs like images, formatted text regions, and other such higher level concepts that aren't covered in the level-0 command set. *(v2.A4)*

## RIP_BEGIN_TEXT

*Define a rectangular text region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `T` |
| **Arguments** | `x1:XY y1:XY x2:XY y2:XY res:2` |

**Format:** `!|1T <x1> <y1> <x2> <y2> <res>`
**Example:** `!|1T00110011`

**Attributes used:** Viewport, Port, Base Math (current setting)

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

These commands ignore the current font "direction"; all text is always displayed horizontally in the current font type, style and size. *(v2.A3)*

NOTE:  The `res` parameter is two bytes wide and is RESERVED for future use and should be set to "00".

## RIP_BUTTON

*Define a Mouse Button*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `U` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY hotkey:2 flags:1 res:1 text` |

**Format:** `!|1U <x0> <y0> <x1> <y1> <hotkey> <flags> <res> <text>`
**Example:** `!|1U010100003200iconfile<>Label<>HostCmd^m`

**Attributes used:** Font Style, Viewport, Port (only for "non-mouse" type buttons), Base Math (current setting)

This command physically creates a new Button using a previously defined [RIP_BUTTON_STYLE](#rip_button_style) in the current button style.  You may have at most 128 different Mouse Buttons (you may have any number of non-Mouse Buttons).  The `<slot>` parameter determines which button style slot is used to determine how this button appears. *(v2.A1)*

The `<x0>` and `<y0>` parameters for this command designate the upper-left corner of the Button.  This (X,Y) location may not be the actual "absolute" corner position of the Button, as it may be adjusted via the Special Effects functions that are part of the RIP_BUTTON_STYLE command (see above).

The `<x1>` and `<y1>` parameters are only used for Plain Buttons when you have not specified a specific Height and Width in the RIP_BUTTON_STYLE command.  These parameters are used in Dynamically Sized Buttons.  If the Height and Width in the RIP_BUTTON_STYLE are non-zero, then these two parameters are set to zero.

The (x0,y0) and (x1,y1) parameters will be modified by the following values for the different special effects:

| Effect Type | X0 Modifier | Y0 Modifier | X1 Modifier | Y1 Modifier |
|---|---|---|---|---|
| Bevel | -bevel size | -bevel size | +bevel size | +bevel size |
| Recess | -2 | -2 | +2 | +2 |
| Sunken | 0 | 0 | 0 | 0 |
| Chisel | 0 | 0 | 0 | 0 |

```text
[BEGIN REWORD]

  <<< Discuss resolution independence & scaling of the above >>>

[END REWORD]
```

The `<hotkey>` parameter is only used with Mouse Buttons.  It is the ASCII code for the keystroke that will activate this Button.  It is represented as a two-digit MegaNum.  If this character exists in the text label, and the Underline flag or hilight hotkey flag is enabled in the RIP_BUTTON_STYLE, then the character will be underlined in the label.  Control codes are allowable, and a value of 255 (decimal) corresponds to "any" key. *(v1.54)*

The `<flags>` parameter provides several different functions for each button.  The possible "combinatorial" flags for this parameter are listed in the following table.  Note that these values may be combined together (by adding their values) to arrive at the final flag parameter's value.

| Value | Description |
|---|---|
| 1 | Draw button as already selected |
| 2 | Button is "default" when \<ENTER\> is pressed |

Using a flag of 1 means that the Button is already "selected".  By selected, we mean that it is already clicked and should be initially drawn as clicked.  This is typically used for Radio Buttons and/or Check Boxes.  This only affects the image.  The Host Command WILL NOT be automatically sent to the host when a selected Button is drawn. If this parameter is set to 0, then the Button will be drawn in normal, unselected mode.

The `<text>` parameter for this command is somewhat different than those found in previously described RIPscrip commands.  All other RIPscrip commands only have one text parameter.  This command requires  anywhere from 0-3 text parameters.  The way RIPscrip accomplishes this is by separating each block in the `<text>` parameter with the delimiter `<>`.  This text parameter delimiter is not needed before the first text block, but is necessary between the 1st and 2nd blocks, and the 2nd and 3rd blocks.  Here is an example of a typical text parameter for this command:

```text
ICONFILE.BMP<>TEXT LABEL<>HOST COMMAND
```

The actual syntax of this text parameter is as follows:

```text
[icon-file][[<>text-label][<>host-command]]
```

The block described as ICONFILE.BMP is actually the Icon Filename that will be used for the Button if it is an Icon Button.  If it is not an Icon Button, then this block will read `<>` all by itself (a "null" block). *(v2.A1)*

Note that we actually specified a file extension of ".BMP" for our icon filename.  You shouldn't explicitly specify a filename extension like this if it is the default extension of the filename.  The reason for this is that some platforms don't use file extensions and their use is not really appropriate.  When in doubt, don't specify a file extension. *(v2.A1)*

The .BMP file extension does not need to be included as part of the filename.  If omitted, it will automatically be appended to the filename.  If an extension is provided, it will be used verbatim. *(v2.A1)*

The "TEXT LABEL" block is actually the text that will be used to descriptively label the Button.  You may also specify a "null" block for no label (i.e., `<>`).

The final block of the `<text>` parameter is the Host Command.  This block contains any text that should be sent to the Host after this Button is clicked.  This may contain any Control Characters, Pick-List definitions, Text Variables or Template Definitions.  This block might be "segmented" into multiple Host Command segments.  See the section entitled HOST COMMANDS in this Manual for additional information on these Host Command features.

When this command is stored in-memory, it is converted to global screen coordinates (for internal storage only).  This makes it so that if you have mouse button regions defined in multiple different viewports, that each field will be properly inverted at the right location regardless of the currently defined viewport. *(v1.54)*

NOTE: All Mouse Fields are scanned in "last in, first out" order.  This means that the last-most received Mouse Button will be the first one scanned for a mouse click. *(v1.54)*

Not all of the blocks in the `<text>` parameter need to be specified.  Here are examples of the valid combinations of text blocks:

| Parameter Example | Description of the Text Parameter |
|---|---|
| `icon<>label<>host_cmd` | Specify all three blocks |
| `<>label<>host_cmd` | 2 blocks specified; no icon |
| `icon<>label<>` | 2 blocks specified; no host  command |
| `icon<>label` | 2 blocks specified; no host  command |
| `<><>host_cmd` | 1 block  specified; no icon  or label |
| `<>label<>` | 1 block  specified; no icon  or host command |
| `<>label` | 1 block  specified; no icon  or host command |
| `icon<><>` | 1 block  specified; no label or host command |
| `icon<>` | 1 block  specified; no label or host command |
| `icon` | 1 block  specified; no label or host command |
| `<><><>` | A blank text parameter; all blocks omitted |
| `<><>` | A blank text parameter; all blocks omitted |
| `<>` | A blank text parameter; all blocks omitted |

Under RIPscrip 1.54 and earlier versions, button icon filenames ended with .ICN for normal icons, and .HIC for "Hot IcoN".  Under 2.0, these file extensions are different.  For normal button icons, the extension .BMP is used for a standard bitmap, and .BMH is used for a "hot bitmap". *(v2.A3)*

When a button is displayed that uses an icon/bitmap, special care must be taken when dealing with hot bitmaps.  If the flag is present in the button style indicating to use hot icons, then when the button is displayed as selected then the hot bitmap (.BMH) file's image is used to display the button.  If that file doesn't exist, then the normal bitmap button is displayed in an inverted state.  If neither of these files are present, then the RIP_BUTTON command is ignored. *(v2.A3)*

If the host command string of the button cannot be parsed properly, then the button is completely ignored. *(v2.A3)*

NOTE:  The `<res>` parameter is reserved for future use by TeleGrafix Communications, Inc..  It should be set to 0 for compatibility with future releases.

## RIP_BUTTON_STYLE

*Button style definition*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `B` |
| **Arguments** | `wid:XY hgt:XY orient:2 flags:4 size:2 dfore:2 dback:2 bright:2 dark:2 surface:2 grp_no:2 flags2:2 uline_col:2 corner_col:2 other_port:1 res:5` |

**Format:** `!|1B <wid> <hgt> <orient> <flags> <bevsize> <dfore> <dback> <bright> <dark> <surface> <grp_no> <flags2> <uline_col> <corner_col> <other_port> <res>`
**Example:** `!|1B0A0A010274030F080F080700010E07010A00`

**Attributes used:** Base Math (current setting)

This RIPscrip command is probably one of the most complex in the entire protocol.  It defines how subsequent [RIP_BUTTON](#rip_button) commands will be interpreted.  The purpose of this command is to define what a Button is and how they operate.  Buttons can have many different configurations, flags, and styles.  With the diversity of modes that the Button can take on, complexity is a necessary evil.

There are 36 separate button style slots that can be defined simultaneously.  This command allows you to alter the current button style slot.  This button style slot is used by the RIP_BUTTON command to define buttons of particular types.  Button style slots can be used to avoid retransmitting button styles over and over again. *(v2.A4)*

This command does not actually do anything visibly on the screen.  Simply put, this creates an internal definition for the Button mode which will be used with RIP_BUTTON commands after the definition is created.

### Label Orientation

Every Button can have an optional text label.  It can appear in several different locations compared to the Button itself.  This is specified in the `<orient>` parameter.  The actual text of the label is not specified with this command, it is specified when you actually create a Button (see RIP_BUTTON below).  The value that `<orient>` can be is as follows:

| Value | Description of Orientation |
|---|---|
| 00 | Display label above button |
| 01 | Display label to the left of button |
| 02 | Display label in the center of the button |
| 03 | Display label to the right of button |
| 04 | Display label beneath the button |

### Button Types

There are three basic "types" of Buttons.  There are Icon buttons, Clipboard buttons and Plain buttons.  Each of these differ in the way that they create the button's image.  A description of each type is as follows: *(v1.54)*

**ICON BUTTON (flag 128)** - An Icon Button means that the actual image of the button will be created by loading a bitmap icon image from the disk and load it at the given locatino.  Any special effects (see below) can be applied to the Icon to further enhance the image.  The filename for the Icon is supplied with the RIP_BUTTON command, as is the Icon's upper left X/Y coordinate.  Icon Buttons are always stamped in COPY mode. *(v1.54)*

**CLIPBOARD BUTTON (flag 1)** - A clipboard button uses the clipboard port contents for the base image of the button.  If no `<other_port>` is defined (ie, port #0), then the specific port that is used for the source image is determined by the clipboard pointer.  If the clipboard pointer isn't defined, then the RIP_BUTTON commandsd that follow are ignored.  If the `<other_port>` is from 1-35, then this specifies a previously defined port.  This port will be the source of the bitmap image of the button.  The entire contents of the port's viewport will be the image used for any subsequent RIP_BUTTON commands.  If the given port doesn't exist, then it the RIP_BUTTON command(s) are ignored.  Any special effects can be applied to this image to further enhance the overall button's image. *(v2.A3)*

**PLAIN BUTTON (flag 256)** - A plain button is exactly that, plain.  No special graphics are used to create the button.  The entire button region is simply filled-in with a solid colored rectangle using the `<surface>` color.  Any special effects can be further applied to enhance the button's appearance. *(v1.54)*

### Size and Color Parameters

The `<hgt>` and `<wid>` parameters represent the fixed height and width of the Button (applies only to Plain Buttons).  If both values are greater than zero, then this will represent the actual size of the Button (its dimensions are not specified by the RIP_BUTTON command).  If both of these are set to zero, then the actual RIP_BUTTON command will specify the size of the particular Button (dynamic sizing).

The `<bevsize>` parameter is only used if the BEVEL FLAG (flag 512) is specified.  When active, this parameter will determine how many pixels thick the bevel should be.  This may be any value greater or equal to zero. *(v1.54)*

There are a large number of flag values that can be combined to achieve a great many effects.  There are two flag parameters for the RIP_BUTTON_STYLE command, `<flags>` and `<flags2>`.  They are detailed in the two tables that follow in this Section.  You may combine any of the flags in the first table together simply by adding the "Value" of each one together and representing that number as a MegaNum.

The `<dfore>` and `<dback>` parameters are used with the text label.  The `<dfore>` parameter is the foreground color for the text.  It is always used to determine the color of the text label.  The `<dback>` color is the color of the dropshadow (if any).  This parameter is only used when you have specified the "Dropshadow" flag in the `<flags>` parameter (see below).

NOTE: There are seven (7) color parameters in this command.  These parameters are ALWAYS used in Color Map mode.  Direct RGB Color Mode cannot be used for these parameters.  With Color Map mode, you have up to 256 separate color combinations possible for each color, providing you with a great deal of flexibility.  The color parameters always use the Base Math set by the global Base Math setting. *(v2.A0)*

The `<bright>`, `<dark>` and `<surface>` parameters are used with Plain Buttons and with the Special Effects styles (see `<flags>` below).  These colors represent the highlighted color, the shadowed color, and the regular surface color that is used in Special Effects.  Typical color combinations for these colors might be White, Dark-Gray and Light-Gray respectively for a "chiseled steel" appearance.  Each of these values can contain a two-digit value representing any valid color code that may be used in the [RIP_COLOR](08-level-0-commands-a-f.md#rip_color) command.

In addition to the special effects colors, are two additional colors that can be used, `<uline_color>` which is used for the color of the underline (in the text label), and `<corner_color>` which is used to display the colors of corners for things like the Bevel, Recess, etc.

The `<grp_no>` parameter determines which Button Group subsequent RIP_BUTTON commands will be associated with.  Button Groups are used to maintain groups of Buttons for things like Radio Buttons and/or Checkbox Buttons.  See the section on the BUTTON COMMAND for more information on these modes, and what Button Groups can offer.  This value can range anywhere from 0-Z (i.e., 0-35).  You should not mix Checkbox and Radio buttons in the same group. -- unpredictable things may happen if you do.

### Flags Field 1

Some `<flags>` are mutually exclusive.  For example, you can only have one of the "Plain", "Icon", or "Clipboard" flags chosen at once.  To better assist you in determining which values can be combined with each other, the right-most five columns in the next two tables explain if the specific flag can be used under a specific condition.  For example, you cannot choose the "Hot Icon" flag if you are dealing with a Clipboard Button.  Another example is that you cannot underline the hotkey character in the label if it is not a Mouse Button.

The following table contains the possible flag values for the `<flags>` parameter.  Each of these values may be combined to achieve a "composite" group of flags.  See the preceding paragraphs for a more detailed explanation of this method.

| Value | Description of Flags Field #1 | Icn | Clp | Pln | Mse | NoMse |
|---|---|---|---|---|---|---|
| 1 | Button is a "Clipboard Button" | N | Y | N | Y | Y |
| 2 | Button is "Invertable" | Y | Y | Y | Y | N |
| 4 | Reset screen after button click | Y | Y | Y | Y | N |
| 8 | Display Chisel special effect | Y | Y | Y | Y | Y |
| 16 | Display Recessed special effect | Y | Y | Y | Y | Y |
| 32 | Dropshadow the label (if any) | Y | Y | Y | Y | Y |
| 64 | Auto-stamp image into Clipboard slot *(v2.A2)* | Y | Y | Y | Y | Y |
| 128 | Button is an "Icon Button" | Y | N | N | Y | Y |
| 256 | Button is a "Plain Button" | N | N | Y | Y | Y |
| 512 | Display Bevel special effect | Y | Y | Y | Y | Y |
| 1024 | Button is a Mouse Button | Y | Y | Y | Y | N |
| 2048 | Underline hot-key in label | Y | Y | Y | Y | N |
| 4096 | Make Icon Button use Hot Icons | Y | N | N | Y | N |
| 8192 | Adj. vertical centering of label | Y | Y | Y | Y | Y |
| 16384 | Button belongs to a Radio Group | Y | Y | Y | Y | N |
| 32768 | Display Sunken special effect | Y | Y | Y | Y | Y |

The Icon Button, Clipboard button and Plain flags have already been discussed.  Following, will be more discussion of the various flags used in the preceding table: *(v1.54)*

**BUTTON IS INVERTABLE (flag 2)** - This means that the button will be inverted when clicked.  This flag is only useful when combined with the "Button is a Mouse Button - flag 1024) flag.  Even if the button has special effects, those will be inverted as well as they are considered part of the button - all except for the Recessed effect.  The recessed effect is NEVER considered part of the actual button image, and will never be part of the mouse field, button's image or anything - it is just extra graphics. *(v1.54)*

**RESET SCREEN AFTER BUTTON CLICK (flag 4)** - This flag is used when the button is considered a Mouse Button (flag 1024).  What this means is that when the user clicks on the button, the screen will be reset exactly the same as a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) command will do.  The reset is performed before the host command is processed (if any). *(v1.54)*

**DISPLAY CHISEL SPECIAL EFFECT (flag 8)** - This displays a special effect on-top of the button image that gives the visual impression of an indented gutter just to the inside of the button's border.  The amount of indentation varies depending on the size of the button.  See below for a table of indentation values for the Chisel effect. *(v1.54)*

**DISPLAY RECESSED SPECIAL EFFECT (flag 16)** - This places a recessed one-pixel bevel around the exterior of the button.  It is never considered part of the button's image.  Its purpose is to give the button a more 3D look by making it appear that the button is "poking through" a hole in a dialog box.  This effect is accomplished by placing a black outline (one pixel wide) around the exterior of the button's image (including bevel, etc), then placing a one-pixel wide inverted bevel around the black outline. *(v1.54)*

**DROPSHADOW THE LABEL IF ANY (flag 32)** - This flag will instruct RIPscrip to place a dropshadowed version of the text label one pixel to the right and one pixel lower than the original label.  This is accomplished by drawing the label first in the `<dback>` color, then drawing the label offset up-left in the `<dfore>`. *(v1.54)*

**AUTO-STAMP IMAGE INTO CLIPBOARD SLOT (flag 64)** - This option is also known as "Auto-Clip".  What this means is right after the first button's image is rendered (including any special effects), it is automatically copied into another port specified by the `<other_port>` parameter.  If that port is defined as port #0, then this mode is treated like a [RIP_GET_IMAGE](#rip_get_image) command, copying the image onto some clipboard port, by determining the clipboard pointer and establishing an offscreen bitmap port located at the first unused port number.  If the `<other_port>` parameter is set to a value from 1-35, then a specific port number is used; that port is deleted (if it's in use) then re-defined.  If the `<other_port>` parameter specifies the same port as the currently active drawing port, or if the clipboard pointer points to the current drawing port, then auto-clip mode is ignored.  The Recessed special effect is not considered part of the button image for this flag and is not made part of the clipboard image.  After the image is copied into a clipboard port, the label is drawn (this is so that the label is not placed into a clipboard port), then a number of flags in the current Button Style definition are altered.  Specifically, the Icon and Plain flags are disabled, and Clipboard button enabled (thus making any subsequent buttons use the resultant Clipboard button image for their button's representation).  In addition, the chisel, bevel, auto-clip and sunken flags are disabled.  The final result is a Clipboard button with no special effects other than the Recessed effect (if any).  This is most often used with Icon Buttons where every subsequent button uses the same Icon over and over again - the net result of this is less "disk usage" whenever a button is created; in addition, buttons will draw faster too.  If this option is used, then the parameter `<other_port>` is used to determine which clipboard port to use for the stored image. *(v2.A3)*

**BUTTON IS AN ICON BUTTON (flag 128)** - See previous discussions on button types above. *(v1.54)*

**BUTTON IS A PLAIN BUTTON (flag 256)** - See previous discussions on button types above. *(v1.54)*

**DISPLAY BEVEL SPECIAL EFFECT (flag 512)** - When this flag is active, a bevel will be drawn `<size>` pixels thick on the outside of the base image of the button.  This makes the button that many pixels wider and taller in each direction.  See the RIP_BUTTON for a more detailed description of the affects of the button's final size. *(v1.54)*

**BUTTON IS A MOUSE BUTTON (flag 1024)** - When this flag is enabled, the button becomes a clickable mouse region.  When this option is in use, the Invert flag and several others are available (see the preceding chart).  When a button is a non-mouse button, then it is only used to draw a dialog box or an elaborate "static graphic" image of something that "looks" like a button. *(v1.54)*

**UNDERLINE HOT-KEYS IN LABEL (flag 2048)** - When this flag is active, the first occurence of the hot-key character in the button's label will be underlined using the `<uline_col>` color.  Special care must be taken when underlining the character, taking into consideration if the character has a segment that goes below the "baseline" of the font.  Consult the following section to determine what ASCII characters have these "descenders".  Characters with descenders have the underline drawn slightly lower than for characters without them. *(v1.54)*

**MAKE ICON BUTTON USE HOT ICONS (flag 4096)** - A Hot Icon is a button that has an alternative image when the button is "depressed".  Normally, when a button is an Icon button, some form of Icon File is used to create the image of the button (eg, EMAIL.BMP).  When the Hot Icon flag is in use, whenever that button is depressed, rather than using the normal Icon File for the button's image, a different Icon File is used for the button's image.  The filename would be the same filename as the original Icon, but with an extension of .BMH instead of .BMP.  When Hot Icon is in use, the alternative Icon is stamped in COPY mode.  If the file does not exist, then the original Icon's image is used, but pasted in NOT mode for the duration of the depression. *(v1.54)*

**ADJ. VERTICAL CENTERING OF LABEL (flag 8192)** - Some Labels may appear non-centered vertically when drawn using some fonts that are rather large.  With this in mind, you have the option to adjust the vertical centering.  What this does is take into consideration the height of any descenders of the label and calculate those into the overall height of the label before centering.  If this flag is not used, then the descenders are not taken into consideration when the vertical centering is calculated.  See the Font Metric tables below for more detailed information on font sizes and their associated metrics.  This command has no effect if the Label orientation is LEFT or RIGHT of the button.  It only applies to an orientation of TOP, BOTTOM or CENTER. *(v1.54)*

**BUTTON BELONGS TO A RADIO GROUP (flag 16384)** - When this flag is used, then any buttons defined in this button `<group_no>` are considered to be radio buttons where only one of the buttons can be clicked (selected) at any particular time.  If a button is not a radio button or a checkbox button, then the "selected" flag of the RIP_BUTTON command is ignored.  When a Radio Button is clicked, any other radio button in that button group that is selected is automatically de-selected and the current one selected.  Any host command is processed at the time of the button click.  If a Radio Button is drawn initally as selected, then that host command is processed at the time of its initial drawing.  See the section near the end of this document discussing [TEMPLATES](16-templates.md) for a more complete description of Radio Buttons and how they interact with their Button Group and with each other. *(v1.54)*

**DISPLAY SUNKEN SPECIAL EFFECT (flag 32768)** - When this option is enabled, a one-pixel inverted bevel is drawn exactly one pixel to the inside of the base image of the button.  This and the chisel effect are the only two special effects which physically "overwrite" portions of the base button image. *(v1.54)*

### Font Metrics and Descenders

This array defines which characters have descenders (portions of their font that go below the baseline).  This information is used in the vertical centering of button text labels.

```c
char low_char[256] =
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
    1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0
};
```

NOTE:  The low_char[] table is only truly useful under ENG configurations for the text window.  Other languages may have descenders for other characters. *(v2.A3)*

```c
struct METRIC
{
    unsigned int top;    // Scan lines from TOP OF CELL to char top
    unsigned int bow;    // Scan lines from TOC to crest of char
    unsigned int base;   // Scan lines from TOC to baseline
    unsigned int drop;   // Scan lines from TOC to lowermost pixel
};
```

The METRIC structure can be described visually as follows:

```text
      0 ──╔══════════╤══════════╗   Notice that the topmost scanline
          ║          │          ║   of a font cell is not necessarily
     TOP──╫─█────────┼──────────╢   the top of the character.  The
          ║ █        │          ║   "top" field of the structure
          ║ █        │          ║   contains the vertical offset
          ║ █        │          ║   from the top of the cell for all
     BOW──╫─█────────┼──────────╢   fonts in that set.
          ║ █ ███    │   ███ █  ║
          ║ ██   █   │  █   ██  ║
          ║ █     █  │ █     █  ║
          ║ █     █  │ █     █  ║
          ║ █     █  │ █     █  ║
          ║ █     █  │  █   ██  ║
    BASE──╫─█─────█──┼───███─█──╢
          ║          │       █  ║
          ║          │       █  ║
          ║          │       █  ║
          ║          │  █   █   ║
    DROP──╫──────────┼───███────╢
          ║          │          ║
    END ──╚══════════╧══════════╝
```

**Default Font (Font 0)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 0 | 2 | 6 | 7 |
| 2 | 0 | 4 | 13 | 15 |
| 3 | 0 | 6 | 20 | 23 |
| 4 | 0 | 8 | 27 | 31 |
| 5 | 0 | 10 | 34 | 39 |
| 6 | 0 | 12 | 41 | 47 |
| 7 | 0 | 14 | 48 | 55 |
| 8 | 0 | 16 | 55 | 63 |
| 9 | 0 | 18 | 62 | 71 |
| 10 | 0 | 20 | 69 | 79 |

**Triplex Font (Font 1)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 6 | 10 | 18 | 22 |
| 2 | 6 | 11 | 20 | 24 |
| 3 | 8 | 13 | 23 | 28 |
| 4 | 10 | 17 | 31 | 38 |
| 5 | 13 | 23 | 41 | 50 |
| 6 | 16 | 28 | 51 | 62 |
| 7 | 20 | 34 | 62 | 76 |
| 8 | 25 | 42 | 77 | 94 |
| 9 | 30 | 51 | 93 | 114 |
| 10 | 40 | 68 | 124 | 152 |

**Small Font (Font 2)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 2 | 3 | 5 | 6 |
| 2 | 2 | 4 | 6 | 7 |
| 3 | 2 | 3 | 6 | 7 |
| 4 | 3 | 5 | 9 | 11 |
| 5 | 4 | 7 | 12 | 14 |
| 6 | 5 | 9 | 15 | 18 |
| 7 | 6 | 10 | 18 | 22 |
| 8 | 7 | 12 | 22 | 27 |
| 9 | 9 | 15 | 27 | 33 |
| 10 | 12 | 20 | 36 | 44 |

**Sans Serif Font (Font 3)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 7 | 11 | 19 | 23 |
| 2 | 7 | 12 | 21 | 25 |
| 3 | 9 | 14 | 24 | 29 |
| 4 | 11 | 18 | 32 | 39 |
| 5 | 14 | 24 | 42 | 51 |
| 6 | 18 | 30 | 53 | 64 |
| 7 | 22 | 36 | 64 | 78 |
| 8 | 28 | 45 | 80 | 97 |
| 9 | 33 | 54 | 96 | 117 |
| 10 | 44 | 72 | 128 | 156 |

**Gothic Font (Font 4)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 7 | 11 | 19 | 23 |
| 2 | 7 | 12 | 21 | 25 |
| 3 | 9 | 14 | 24 | 29 |
| 4 | 11 | 18 | 32 | 39 |
| 5 | 14 | 24 | 42 | 51 |
| 6 | 18 | 30 | 53 | 64 |
| 7 | 22 | 36 | 64 | 78 |
| 8 | 28 | 45 | 80 | 97 |
| 9 | 33 | 54 | 96 | 117 |
| 10 | 44 | 72 | 128 | 156 |

**Script Font (Font 5)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 10 | 17 | 22 | 29 |
| 2 | 10 | 18 | 24 | 32 |
| 3 | 12 | 21 | 27 | 36 |
| 4 | 16 | 28 | 37 | 49 |
| 5 | 21 | 37 | 49 | 65 |
| 6 | 26 | 46 | 61 | 81 |
| 7 | 32 | 56 | 74 | 98 |
| 8 | 40 | 70 | 92 | 122 |
| 9 | 48 | 84 | 111 | 147 |
| 10 | 64 | 112 | 148 | 196 |

**Simplex Font (Font 6)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 8 | 13 | 21 | 25 |
| 2 | 9 | 14 | 23 | 27 |
| 3 | 10 | 16 | 26 | 31 |
| 4 | 13 | 21 | 35 | 42 |
| 5 | 17 | 28 | 46 | 55 |
| 6 | 22 | 35 | 58 | 69 |
| 7 | 26 | 42 | 70 | 84 |
| 8 | 32 | 52 | 87 | 104 |
| 9 | 39 | 63 | 105 | 126 |
| 10 | 52 | 84 | 140 | 168 |

**Triplex Script Font (Font 7)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 6 | 10 | 18 | 22 |
| 2 | 6 | 11 | 20 | 24 |
| 3 | 8 | 13 | 23 | 28 |
| 4 | 10 | 17 | 31 | 38 |
| 5 | 13 | 23 | 41 | 50 |
| 6 | 16 | 28 | 51 | 62 |
| 7 | 20 | 34 | 62 | 76 |
| 8 | 25 | 42 | 77 | 94 |
| 9 | 30 | 51 | 93 | 114 |
| 10 | 40 | 68 | 124 | 152 |

**Complex Font (Font 8)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 9 | 13 | 21 | 25 |
| 2 | 9 | 14 | 23 | 27 |
| 3 | 11 | 16 | 26 | 31 |
| 4 | 14 | 21 | 35 | 42 |
| 5 | 18 | 28 | 46 | 55 |
| 6 | 23 | 35 | 58 | 69 |
| 7 | 28 | 42 | 70 | 84 |
| 8 | 35 | 52 | 87 | 104 |
| 9 | 42 | 63 | 105 | 126 |
| 10 | 56 | 84 | 140 | 168 |

**European Font (Font 9)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 8 | 15 | 33 | 39 |
| 2 | 8 | 16 | 36 | 42 |
| 3 | 10 | 19 | 41 | 48 |
| 4 | 13 | 25 | 55 | 65 |
| 5 | 16 | 33 | 73 | 86 |
| 6 | 21 | 41 | 91 | 107 |
| 7 | 26 | 50 | 110 | 130 |
| 8 | 32 | 62 | 137 | 162 |
| 9 | 39 | 75 | 165 | 195 |
| 10 | 52 | 100 | 220 | 260 |

**Bold Font (Font 10)**

| Size | Top | Bow | Base | Drop |
|---|---|---|---|---|
| 1 | 12 | 18 | 36 | 40 |
| 2 | 14 | 20 | 40 | 44 |
| 3 | 15 | 23 | 45 | 50 |
| 4 | 20 | 30 | 60 | 67 |
| 5 | 27 | 40 | 80 | 89 |
| 6 | 34 | 50 | 100 | 111 |
| 7 | 40 | 60 | 120 | 134 |
| 8 | 50 | 75 | 150 | 167 |
| 9 | 60 | 90 | 180 | 201 |
| 10 | 80 | 120 | 240 | 268 |

NOTE:  The above metric tables are based on 640x350 resolutions.  For information on font scaling at other resolutions for the "system fonts" defined via the [RIP_FONT_STYLE](08-level-0-commands-a-f.md#rip_font_style) command, see the preceding section for the RIP_FONT_STYLE command.  This section defines the magnification factors for each font and how they relate to other resolutions. *(v2.A3)*

### Chisel Effect Insets

The Chisel effect draws a dropshadowed line around the inside of the button.  How far from the borders of the button are determined by this table:

| Height of Button | X inset | Y inset |
|---|---|---|
| 0 - 11 | 1 | 1 |
| 12 - 24 | 3 | 2 |
| 25 - 39 | 4 | 3 |
| 40 - 74 | 6 | 5 |
| 75 - 149 | 7 | 5 |
| 150 - 199 | 8 | 6 |
| 200 - 249 | 10 | 7 |
| 250 - 299 | 11 | 8 |
| 300 - | 13 | 9 |

```text
[BEGIN REWORD]

  <<< Think about resolution independence of chisel indent. >>>
  <<< Talk about bevel sizes too, along with recesses.      >>>

[END REWORD]
```

### Flags Field 2

This table describes the possible flag settings for the `<flags2>` parameter:

| Val | Description of Flags Field #2 | Icon | Clip | Plain | Mouse | No-Mouse |
|---|---|---|---|---|---|---|
| 1 | Button is in a check box group | Y | Y | Y | Y | N |
| 2 | Highlight hotkey character | Y | Y | Y | Y | N |
| 4 | Explode (zoom out when clicked) | Y | Y | Y | Y | N |
| 8 | Left Justify Label (top/ctr/btm) | Y | Y | Y | Y | Y |
| 16 | Right Justify Label (top/ctr/btm) | Y | Y | Y | Y | Y |
| 32 | Don't fill-in button interior *(v2.A0)* | N | N | Y | Y | Y |
| 64 | Fill-in interior in fill pattern *(v2.A0)* | N | N | Y | Y | Y |

Following is a more complete description of the flags described in the `<flags2>` parameter: *(v1.54)*

**BUTTON IS IN A CHECK BOX GROUP (flag 1)** - When this flag is selected, then the button `<group_no>` is considered a Check-box group.  When in this mode, the Radio Group flag is not accessible - A Button Group can be a Radio Button, a Check-box button or neither, but not more than one at the same time.  A Check-box button group is a group of buttons where each button in the group can be either ON or OFF in any combination.  In this way, more than one button in the group can be clicked at the same time.  See the TEMPLATES section later on in this document for a complete discussion of how the Host Commands are processed for Check-Box buttons.  If a check-box button is drawn as "pre-selected", then the Host Command is processed immediately upon inital drawing of the button (when it is received). *(v1.54)*

**HIGHLIGHT HOTKEY CHARACTER (flag 2)** - When this flag is active, then the first occurence of the hotkey character in the label will be hilighted using the `<uline_col>` color.  This gives the user a visual impression of what keystroke they need to type in order to activate the button without the mouse. *(v1.54)*

**EXPLODE (ZOOM OUT WHEN CLICKED) (flag 4)** - This optional flag is designed to "zoom out" from the button when the user clicks on it.  What is generally done is a dotted rectangle is drawn initially around the button and it is repeatedly redrawn over itself in XOR mode, constantly getting larger and larger until it hits the full size of the screen.  This gives the visual impression that the button is zooming out to another screen.  Each time another frame of the zooming rectangle is drawn, the previous rectangle on the inside of the new one is erased by XOR drawing the same rectangle over itself again.  This option does not need to be supported in order to be considered a RIPscrip terminal, although it would be nice.  In other words, this is not a "make or break" feature. *(v1.54)*

**LEFT JUSTIFY LABEL (TOP/CTR/BTM) (flag 8)** - If this option is enabled, then any TOP, CENTER or BOTTOM label orientations will be left justified to the left edge of the button.  The exact indentation of the label from the interior of the button's base image depends on whether the chisel effect is active.  If chisel is OFF, then the indentation is 10 pixel.  If chisel is ON, then the indentation is 20 pixels.  This option can be combined with the Adjust Vertical Centering flag only if the label orientation is CENTER. *(v1.54)*

**RIGHT JUSTIFY LABEL (TOP/CTR/BTM) (flag 16)** - If this option is enabled, then any TOP, CENTER or BOTTOM label orientations will be right justified to the right edge of the button.  The exact indentation of the label from the interior of the button's base image depends on whether the chisel effect is active.  If chisel is OFF, then the indentation is 10 pixel.  If chisel is ON, then the indentation is 20 pixels.  This option can be combined with the Adjust Vertical Centering flag only if the label orientation is CENTER. *(v1.54)*

**DON'T FILL-IN BUTTON INTERIOR** - If this option is enabled and the button is a Plain Button, then the interior of the Button will not be filled in with the surface color.  By default, any plain button will have its interior filled in with the surface color.  Use this option if you don't want the button face to be drawn. *(v2.A0)*

**FILL-IN INTERIOR IN FILL PATTERN** - If this option is enabled in conjunction with a plain button, then the interior of the button that would normally be filled in with the surface color will instead, use the current Fill Color and Fill Pattern to fill in the interior.  This allows for patterned button faces. *(v2.A0)*

## RIP_COPY_BLIT

*Added in RIPscrip v2.A1.*

*Copy a screen area to a new location (bit blit)*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `g` |
| **Arguments** | `x0:2 y0:2 x1:2 y1:2 dx0:2 dy0:2 mode:1 res:1` |

**Format:** `!|1g <x0> <y0> <x1> <y1> <dx0> <dy0> <mode> <res>`
**Example:** `!|1g080G140M112230`

**Attributes used:** Draw Color, Back Color, Fill Style, Port, Base Math (current setting)

This command is similar to the [RIP_SCROLL](#rip_scroll) command but is more generic in nature - it can move a rectangle of graphical information on the screen from one position to another - not just up or down.  It does this with high-speed bit-blit operations.  The rectangle contained in (x0,y0) to (x1,y1) is copied so that its upper-left corner is now at the location (dx0,dy0).

Some, part, or none of the original source image might be covered up by the moved screen data.  The parts of the original image which are still on the screen (unobscured) can be cleared to some color or state if you wish by using the `<mode>` parameter (like RIP_SCROLL does).  The possible settings for `<mode>` are:

| Mode | Description |
|---|---|
| 0 | Do nothing - leave the old graphics on the screen |
| 1 | Fill the old graphical area with the current drawing color (solid fill in COPY mode). |
| 2 | Fill the old graphical area with the current background drawing color (solid fill in COPY mode). |
| 3 | Fill the area with the current fill color (solid fill in COPY mode).  Fill pattern/styles are not used for this. |
| 4 | Fill the old graphical area with the current fill style/pattern in the current fill color. |
| 5 | Erase the affected area entirely to black. |

## RIP_DEFINE

*Define a text variable*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `D` |
| **Arguments** | `flags:3 res:2 text` |

**Format:** `!|1D <flags> <res> <text>`
**Example:** `!|1D00700text_var,60:?question?default data`

**Attributes used:** Base Math (current setting)

This command is used to create a text variable on the Client system (i.e., the Terminal system).  A text variable is more fully covered in the HOST COMMANDS section.  Simply put, a text variable is a piece of information assigned to a 1-12 character identifier that can either be saved to a local database file (static variables), or to memory (dynamic variables).  Variable Identifiers can be 1-12 characters in length.  You may use any alphanumeric character and underscores (_) in the identifier.  An underscore cannot be the first character, nor can the first character of an identifier be a number.

The `<flags>` parameter of this command combines three separate values into one MegaNum flag that determines how the variable definition will operate.  Here are the possible flag values:

| Value | Description of Flag |
|---|---|
| 001 | Save Variable to database |
| 002 | Cannot specify a blank response |
| 004 | Non-interactive query |

When a variable is flagged as "Save to Database", it becomes a part of the Client system's actual configuration.  The value is saved indefinitely until either changed, or manually erased.  You may choose not to allow the user to enter a blank response.  This basically requires them to enter some piece of information for the variable.

The last flag determines whether the definition is interactive or not.  An interactive definition will attempt to define the variable.  If it does exist, it pops the value up on the screen asking the user to confirm if the value is correct.  If it does not exist, a similar pop-up box will appear asking the user to enter some data for the given variable.

In a non-interactive situation, the Client system will check to see if the variable exists.  If it does, then nothing happens (unless a default response is specified in this command, whereby the Client's variable is updated with the new information).  If the value is not defined, then this definition becomes interactive by default, since the user actually has to enter something anyway.

The `<text>` parameter for this command is also segmented in nature, much like the [RIP_BUTTON](#rip_button) command is.  An example of a segmented `<text>` parameter for the RIP_DEFINE command might be as follows:

```text
FULL_NAME,30:?What is your full name?John Doe
```

The actual syntax of the Variable Define text parameter is as follows:

```text
variable-identifier[,field-width]:[?question-text?][default-value]
```

There are several different segments in this parameter as you can see.  The first section is the variable- identifier.  Immediately after it is an optional field-width.  If the field width is omitted, it will default to a value of  60.  You should limit the values of this width from 1-60.

Immediately following the identifier field-width parameter is a colon (:).  The colon indicates that the variable identifier field is completed and that the remainder of the text parameter is to contain the question and/or the default response (if any).  If no question or default response is provided, the text parameter would read `TEXT_VAR,width:` with no additional data.

The question-text is specified by a question mark (?) followed by the actual text of the question, followed by a trailing question mark.  The basic format of the question segment is as follows:

```text
?this is a question?
```

The remainder of the text parameter consists of a default-value for the variable's contents.  It may be omitted if you wish, to make it so that the user must enter his/her own value instead of some "canned response".

NOTE:  The `<res>` parameter is reserved for future use by TeleGrafix Communications, Inc..  It should be set to 00 for compatibility with future releases.

THIS COMMAND IS NOW OBSOLETE.  USE THE SPECIALLY ENHANCED TEXT VARIABLE SYSTEM WITHIN QUERIES OR HOST COMMANDS TO PRODUCE THE SAME RESULTS AS THIS COMMAND AND MUCH MORE! *(v2.A1)*

## RIP_END_TEXT

*End a rectangular text region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `E` |
| **Arguments** | \<none\> |

**Format:** `!|1E`
**Example:** `!|1E`

**Attributes used:** Viewport, Port, Base Math (N/A)

This command indicates the end of a formatted text block.  Only one of these "end" commands is necessary for each block.

## RIP_FILE_QUERY

*Query existing information on a particular file*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `F` |
| **Arguments** | `mode:2 res:4 filename...` |

**Format:** `!|1F <mode> <res> <filename>`
**Example:** `!|1F010000testfile.icn`

**Attributes used:** Base Math (current setting)

This command queries the existence of a particular file, regardless of type.  It is intended for host systems to determine if a particular Icon or RIP file exists on the terminal;s hard disk.

There are a variety of ways you can query for filenames.  The `<mode>` parameter determines the command's response.  This command instructs the terminal to send a response to the host immediately upon execution.

The following table is a listing of the possible values for `<mode>`:

| Mode | Description |
|---|---|
| 00 | Simply query the existence of the file.  If it exists, a "1" is returned.  Otherwise a "0" is returned to the Host (without a carriage return). |
| 01 | Same as 0, except a carriage return is added after the response. |
| 02 | Queries the existence of a file.  If it does not exist, a "0" is returned to the Host followed by a carriage return.  If it does exist, the returned text is a "1." followed by the file size (in decimal).  The return sequence is terminated by a carriage return.  An example of the returned text could be "1.20345". |
| 03 | Queries extended return information.  If the file does not exist, a "0" is returned followed by a carriage return.  If it does exist, the text returned to the Host is in the Format: `1.size.date.time <cr>`.  An example of a return statement could be `1.20345.01/02/93.03:04:30<cr>` |
| 04 | Queries extended return information.  If the file does not exist, a "0" is returned followed by a carriage return.  If it does exist, the text returned to the Host is in the Format: `1.filename.size.date.time <cr>`. An example of a return statement could be `1.MYFILE.RIP.20345.01/02/93.03:04:30 <cr>`.  Note that the file extension adds another period into the return text. |

## RIP_GET_IMAGE

*Copy rectangular image to the clipboard port*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `C` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY res:1` |

**Format:** `!|1C <x0> <y0> <x1> <y1> <res>`
**Example:** `!|1C001122330`

**Attributes used:** Viewport, Port, Base Math (current setting)

This command instructs the terminal program to copy a rectangular region defined by (x0,y0) to (x1,y1) in the currently active drawing port to the first offscreen port found after port #0 (the screen port).  If no offscreen port is actually found, then the first empty port slot is used; the slot is defined as an offscreen bitmap port and is initialized to the dimensions of the source rectangle used in this command.  If an offscreen bitmap port is found, then it is deleted and re-initialized based on the dimensions of this command.  Once the offscreen bitmap port is initialized, the contents of the source rectangle are copied to the port, occupying the entire contents of the port.  If the port number that is found corresponds to the source port, then this command does nothing. *(v2.A3)*

In older versions of RIPscrip (v1.54 and earlier), there was only the screen and one offscreen bitmap port (the clipboard).  Under 2.0, you may have multiple offscreen bitmap ports (or clipboards).  There are three commands from RIPscrip 1.54 that pertain directly to the concept of a clipboard (RIP_GET_IMAGE, [RIP_PUT_IMAGE](#rip_put_image) and [RIP_WRITE_ICON](#rip_write_icon)).  To facilitate the integration of these commands in the architecture of RIPscrip 2.0, a clipboard pointer is used.  The clipboard pointer is created with the RIP_GET_IMAGE command is used.  When this command is executed, the software will scan from port #1 to port #35 (port #0 is skipped because it's the screen).  It searches for the first open (unused) port.  Once found, the clipboard pointer is set to point to this port.  This allows the RIP_PUT_IMAGE and RIP_WRITE_ICON command to know which port number is associated with the clipboard. *(v2.A3)*

When a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) command is executed, the contents of the port table are cleared and the clipboard pointer is cleared (ie, no clipboard exists after the operation is complete). *(v2.A3)*

If the offscreen port cannot be defined for whatever reason, then this command is not processed.  The only thing that will be accomplished in this case is that the clipboard pointer will be cleared and no clipboard data will exist. *(v2.A3)*

See the [RIP_DEFINE_PORT](12-level-2-commands.md#rip_define_port) command for more details about offscreen ports and the size limitations of them. *(v2.A3)*

## RIP_IMAGE

*Display a scalable photo type image*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `p` |
| **Arguments** | `res:4 filename` |

**Format:** `!|1p <res> <filename>`
**Example:** `!|1p0000filename.jpg`

**Attributes used:** Write Mode, Viewport, Port, Base Math (current setting)

This command will physically take a JPEG image file that is located on the remote terminal's hard disk (like Icon files) and display it in the image area set by a [RIP_IMAGE_STYLE](#rip_image_style) command.  See the RIP_IMAGE_STYLE command for more details on the options available to set for image viewing and manipulation. *(v2.A0)*

## RIP_IMAGE_STYLE

*Alter subsequent displayed image settings*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `i` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY flags:4 res:12` |

**Format:** `!|1i <x0> <y0> <x1> <y1> <flags> <res>`
**Example:** `!|1i00008G5B0002000000000000`

**Attributes used:** Viewport, Port, Base Math (current setting)

This command alters the settings for subsequently received (or displayed) image files.  RIPscrip supports the display of JPEG (Joint Photographic Experts Group) files.  These files can be transmitted to the remote terminal and viewed. The `<x0>`, `<y0>`, `<x1>` and `<y1>` parameters determine the size of the image area in the current viewport. *(v2.A0)*

If the `<x0>`, `<y0>`, `<x1>` and `<y1>` parameters are all set to zero (0), then Image Viewing is disabled.  Any explicitly viewed images, or any images received with [RIP_ENTER_BLOCK_MODE](13-level-3-9-commands.md#rip_enter_block_mode) will be ignored (however they will be deleted if flag 4 or 8 are active - see below). *(v2.A0)*

If the image view area is larger than the current viewport size, then the image area size will be adjusted according to the viewport's current size to fit. *(v2.A0)*

The image will be scaled to fit inside the specified display region.  If any of these coordinates extend beyond the dimensions of the viewport, they will be adjusted to fit. *(v2.A1)*

There is a `<flags>` parameter which alters some of the aspects of the image being displayed.  These flags can be OR'd together to create a combination of various flag values.  The possible flags are: *(v2.A0)*

| Value | Description |
|---|---|
| 1 | Maintain image aspect ratio - This will keep the image maintained in size no matter what the size of the display region is.  If the region isn't perfectly proportioned in relation to the actual bitmap, then areas of the display region above and below the image will be blacked out (a margin), or possibly left and right depending on proportions of the bitmap.  This will yield a visually accurate representation of the image. *(v2.A0)* |
| 2 | Delete image definition when complete.  This is only useful for images received via RIP_ENTER_BLOCK_MODE or via a UU-Encoded RIPscrip data block for text-only transfer.  When the image is rendered on the screen, it's definition and/or local disk file is deleted automatically upon completion of the drawing process. *(v2.A1)* |
| 4 | Do not clear the image area to the current background color before viewing the image. *(v2.A3)* |
| 8 | Commit the color palette of the image (if any) to the actual video hardware/color lookup table.  This is typically only of use when using the image style with GIF files.  This flag is ignored with JPEGs.  When active, any color palette in the image file to be displayed is used to alter the current video color system to make the image show up as close to the original image as possible. *(v2.A4)* |

If no RIP_IMAGE_STYLE command is processed before a JPEG image is to be displayed, then the image area is defined to be the full size of the viewport in whatever port is currently active.  When a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) or a [RIP_HEADER](09-level-0-commands-g-r.md#rip_header) command is received (ie, the environment is reset), then the image area is also reset to the maximum size of the current viewport. *(v2.A3)*

## RIP_KILL_ENCLOSED_MOUSE_FIELDS

*Added in RIPscrip v2.A0.*

*Destroys any Mouse Fields inside a region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `k` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY flags:4` |

**Format:** `!|1k <x0> <y0> <x1> <y1> <flags>`
**Example:** `!|1k00003G2H`

**Attributes used:** Fill Style (see below), Base Math (current setting)

This command will destroy any Mouse Fields or Mouse Active Buttons that are contained inside the given rectangle or that intersect the rectangle.  The exact operation of this function is dependent on the setting of the `<flags>` parameter.  The `<flags>` parameter can combine any of the following flags to determine the exact mode of operation:

**Region Specific Flags**

| Flags | Description |
|---|---|
| 1 | Kill only fields completely contained (non-intersecting) |
| 2 | Kill only fields that intersect bounding rectangle |
| 4 | Kill fields entirely outside bounding rectangle |

> If 1, 2 and 4 are not present, then NO fields are deleted.  One of these flags must be supplied for any mouse fields to be deleted.

**What Types of Fields not to Destroy**

| Flags | Description |
|---|---|
| 8 | Don't destroy simple Mouse Fields |
| 16 | Don't destroy active Mouse Button fields |

> If 8 and 16 are not present, then all types of mouse fields are destroyed.

**What to do With the Affected Region(s)**

| Flags | Description |
|---|---|
| 32 | Clear the affected field(s) to black (color 0) |
| 64 | Fill the affected field(s) with current fill color and fill pattern (64 overides 32 in the case that both of 32 and 64 are present). |
| 128 | Perform the 32/64 operations on the whole bounding rectangle (based on Region Specific Flags). |

> If 128 is omitted but 32 or 64 are used, then the mouse field's affected are cleared with the appropriate method.  If 128 is present as well, then instead of clearing the individual mouse fields with the given method, the entire region specified by the Region Specific Flags is affected (ie, the entire interior of the bounding rectangle is cleared, etc).  If 32, 64 and 128 are omitted, then the affected region/fields are left on the screen visually, but their mouse field definitions are destroyed internally.  If 128 is used, but 32 and 64 are omitted then nothing is done visually either.

## RIP_KILL_MOUSE_FIELDS

*Destroys all previously defined hot mouse regions*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `K` |
| **Arguments** | \<none\> |

**Format:** `!|1K`
**Example:** `!|1K`

**Attributes used:** Base Math (N/A)

This command will "forget" all Mouse Regions.  Use it at the beginning of each Scene, so that one scene's Mouse Regions don't get used in another.

## RIP_LOAD_BITMAP

*Loads and displays a disk-based bitmap to screen*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `b` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY duplicate_port:1 mode:1 flags:2 trans_color:2 res:4 filename` |

**Format:** `!|1b <x0> <y0> <x1> <y1> <duplicate_port> <mode> <flags> <trans_color> <res> <filename>`
**Example:** `!|1b001122330100000000button`

**Attributes used:** Viewport, Port, Base Math (current setting)

This command is nearly identical in nature to the [RIP_LOAD_ICON](#rip_load_icon) command but it is more flexible in that it supports arbitrary scaling of the bitmap to the destination screen and it supports dithering methods.  It can be made to exactly match the RIP_LOAD_ICON command. *(v2.A1)*

This command instructs the terminal to read a bitmap from disk and display it at the given upper-left (x0,y0) location.  If the (x1,y1) values are non-zero, then it specifies the lower-right corner on the screen for the bitmap's display region.  If the bitmap differs in size than the destination rectangle, then it is scaled (stretched) to fit into the proper region.  If the values of X1 and Y1 are zero, then the bitmap is shown verbatim in whatever resolution of the disk based file (this is more like the RIP_LOAD_ICON command above). *(v2.A3)*

The .BMP file extension does not need to be included as part of the filename.  If omitted, it will automatically be appended to the filename.  If an extension is provided, it will be used verbatim. *(v2.A1)*

If the width or height of the Icon would make it go off the right or bottom edge of the viewport, the bitmap will be clipped.  The `<mode>` parameter defines the modes in which the Icon will be displayed on the screen.  The modes are identical to the [RIP_PUT_IMAGE](#rip_put_image) and the RIP_LOAD_ICON commands, and are as follows: *(v2.A1)*

| Mode | Description | Logical |
|---|---|---|
| 0 | Paste the image onto the port normally | (COPY) |
| 1 | Exclusive-OR  image with one already on the port | (XOR) |
| 2 | Logically OR  image with one already on the port | (OR) |
| 3 | Logically AND image with one already on the port | (AND) |
| 4 | Paste the inverse of the image onto the port | (NOT) |

### Bitmap Flags

The `<flags>` parameter contains important bit-field values that can be OR'd together to produce a number of desired operations.  The possible flag values are:

**Flag 1** - Load the image onto the port and make a copy to another port.  This is used to make a carbon copy of the image once it is placed onto the active drawing port.  This means that after any color palette manipulation, dithering and viewport clipping, the final image is copied to some other drawing port as a type of backup copy.  This is often used to make high-speed copies of the same image without having to continually access the disk in order to load the image over and over again.  If this flag is specified, then the `<duplicate_port>` parameter determines which port number will receive the loaded image.  If you specify a value of 0 for the duplicate port number, then it will act like the RIP_LOAD_ICON command's clipboard mode - it loads the image onto the clipboard port specified by the clipboard pointer, or if the pointer hasn't been used yet, then it places the image on a newly created offscreen bitmap port located in the first unused port number.  The offscreen port will be redefined with the new bitmap's dimensions.  If the `<duplicate_port>` parameter specifies a value from 1-35, then it indicates a specific port number.  That port, if it doesn't exist, will be defined as an offscreen bitmap port with the exact same dimensions as the final bitmap.  If it does exist, then the port is deleted (even if it's a screen-port) and redefined as an offscreen bitmap port with the new bitmap's dimensions.  After the duplicate port is defined, the image is copied to it.  If the current drawing port happens to be the same port used for the duplicate port, then this flag is ignored.  If this flag is omitted, then no duplicate copy is made of the image and the `<duplicate_port>` parameter of this command is ignored. *(v2.A4)*

**Flag 2** - This flag indicates to take the color palette in the bitmap file and activate it in the current color lookup table.  This means that the color palette in the bitmap is copied into the video device's color palette (and into RIPscrip's).  This means that if any graphics are already on the screen when this happens, they might change in color as soon as the bitmap is loaded (this is due to the fact that the color palette is changed).  If the number of colors in the bitmap exceeds the number of colors in the active display device, then the actual color palette in use is used for the bitmap instead of the internal bitmap palette.  If the number of colors in the bitmap is equal or less than the number of colors for the active video mode then the image will be displayed exactly as it was intended.  If this is the case, then no dithering is necessary on the image so the dither flag (see below) parameter is ignored.  If the terminal is running in a mode that doesn't allow for a color palette in the video hardware, then dithering is ignored and the color palette inside the bitmap file is ignored - this is because the system is already running in a 32K, 64K or 24-bit color mode and there are more than plenty colors to represent the bitmap perfectly. *(v2.A4)*

If this flag is omitted, then the bitmap will use the current screen's color palette.  Typically this will be the default color palette for use with RIPscrip but it might be different.  This means that any pixel of the bitmap that is drawn to the screen is shown in whatever actual color is the absolute closest to the color palette number corresponding to that pixel in the bitmap (dithering may still apply - see below). *(v2.A4)*

If the bitmap has no color palette (eg, 24-bit bitmaps), then this flag is ignored. *(v2.A4)*

**Flag 4** - This flag enables auto-dithering mode.  This is an intelligent way of determining if dithering should be performed or not.  If there are more colors in the bitmap than in the video device or if the bitmap's color palette is not used to set the video device, then dithering is used to get the image as close to the original color scheme as possible.  If the bitmap's palette is committed to the video device then no dithering is necessary because the color palette in use has all (or more) of the necessary colors to display the image.  24-bit images will always enable dithering if this mode is used unless the terminal is running in 24-bit mode.  Dithering slows down image drawing so it should only be used for images that really need it. *(v2.A4)*

If this flag is omitted, then no dither is performed at all.  If the number of colors in the bitmap is higher than the number of colors in the display device then this will yield a lower quality image.  But if there are more colors in the display device than the bitmap then the quality of the image will be close to the actual bitmap (or exact). *(v2.A4)*

**Flag 8** - Place the bitmap image onto the screen with one of its colors "transparent".  This means that the color index specified in the `<trans_color>` parameter will be treated as a transparent color.  This color will "show through" to whatever graphics were behind the bitmap.  This applies to all forms of bitmaps (monochrome through 24-bit).  With bitmaps that have no palette, then only colors in the 24-bit range that "map" to this particular transparent color will actually appear as transparent.  When used in combination with dithering (when dithering is performed), transparent regions could be "grainy" in appearance due to the dithering process. *(v2.A4)*

If the bitmap is placed onto a duplicate port, then it is done in a specific order.  The image is shown on the current drawing port (with possible transparency) then the resulting image (after color palette translation and dithering are performed) is placed onto the duplicate port.  This is done so that the image in the duplicate port will already have its processing done on it so that subsequent duplicate operations will not require any color translation or dithering (a speed consideration).  If transparency mode is used, then the image duplicated includes any graphics "behind" the image that show through in the transparent locations. *(v2.A4)*

The `<filename>` parameter must not contain any sub-directory or path information and must specify a valid bitmap file name.  If the bitmap cannot be located or an error occurs on the disk, then a box should be displayed on screen indicating that the given bitmap could not be loaded.  This visual prompt indicates that something is amiss to the end-user.  The size of the box will be as big as the destination bitmap image on screen.  If the X1 and Y1 are zero, then some suitable box should be drawn. *(v2.A3)*

NOTE:  The 4-byte `<res>` parameter is RESERVED FOR THE FUTURE and should be set to "0000". *(v2.A4)*

## RIP_LOAD_ICON

*Loads and displays a disk-based icon to current port*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `I` |
| **Arguments** | `x:XY y:XY mode:2 clipboard:1 scale:1 res:1 filename` |

**Format:** `!|1I <x> <y> <mode> <clipboard> <scale> <res> <filename>`
**Example:** `!|1I001101010button`

**Attributes used:** Viewport, Port, Base Math (current setting)

This command instructs the terminal to read an icon from disk and display it at the given upper-left (x,y) location in the current drawing port.  If the width or height of the Icon would make it go off the right or left edge of the port's viewport, it will be truncated to fit inside the viewport. *(v2.A3)*

The `<mode>` parameter defines the modes in which the Icon will be displayed on the current drawing port.  The modes are identical to the [RIP_PUT_IMAGE](#rip_put_image) command, and are as follows:

The .BMP file extension does not need to be included as part of the filename.  If omitted, it will automatically be appended to the filename.  If an extension is provided, it will be used verbatim. *(v2.A1)*

| Mode | Description | Logical |
|---|---|---|
| 00 | Paste the image on-screen normally | (COPY) |
| 01 | Exclusive-OR  image with the one already on screen | (XOR) |
| 02 | Logically OR  image with the one already on screen | (OR) |
| 03 | Logically AND image with the one already on screen | (AND) |
| 04 | Paste the inverse of the image on the screen | (NOT) |

The `<clipboard>` parameter can be one of two values: *(v2.A2)*

| Value | Description |
|---|---|
| 0 | Load the image onto the port verbatim |
| 1 | Load the image onto the port and put it onto clipboard port |

If the `<clipboard>` parameter specifies to put the icon's image onto a clipboard port, the order of operation is the following: *(v2.A3)*

1. The image is loaded onto the current drawing port, truncated to the viewport if necessary.
2. A [RIP_GET_IMAGE](#rip_get_image) command is performed transparently to copy the resultant image in the current drawing port onto the clipboard port (it will be defined if necessary, otherwise it will be re-defined).  If the clipboard port is the same port as the current drawing port, then this phase is omitted.  The rectangle copied to the clipboard port is based on the resultant, possibly truncated image.

The `<scale>` parameter indicates whether the icon should be scaled to the device coordinates when it is loaded.  A `1` indicates that it should be scaled, and a `0` indicates that it not should be scaled.  This is important when dealing with older RIPscrip 1.54 icons.  Icons that are loaded with RIPscrip 1.54 need to be resolution independent.  This is performed when this command has a 1 in the scale parameter's position.  All scaling is performed in this command as if the icon were created in a 640x350 environment.  So, if an icon is loaded that is 35 pixels tall, and it is displayed in a 640x480 environment, it will be stretched vertically to make it 48 pixels tall.  This makes sure that icons loaded in this mode are displayed at the correct size so that they are resolution independent. *(v2.A3)*

NOTE: By default this value must be set to `1` (scaled)! *(v2.A2)*

The `<filename>` parameter must not contain any sub-directory or path information and must specify a valid Icon file name.  If the Icon cannot be located or an error occurs on the disk, then a box should be displayed on screen indicating that the given Icon File could not be loaded.  If this happens, then no clipboard operations are performed - except that the clipboard is cleared and the clipboard pointer is cleared as well.  This visual prompt indicates that something is amiss to the end-user. *(v2.A3)*

NOTE: The `<res>` parameter is reserved for future use by TeleGrafix Communications, Inc..  It should be set to 0 for compatibility with future releases. *(v2.A2)*

## RIP_MOUSE

*Defines a rectangular hot mouse field*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `M` |
| **Arguments** | `num:2 x0:XY y0:XY x1:XY y1:XY clk:1 clr:1 res:5 text` |

**Format:** `!|1M <num> <x0><y0><x1><y1> <clk><clr><res><text>`
**Example:** `!|1M00001122331100000host command^M`

**Attributes used:** Viewport, Port (NO; screen ports only), Base Math (current setting)

This command ties together three things:

- A region on the screen
- A mouse-click event
- A string of text to be transmitted by the terminal.

This command defines a rectangular region on the screen that functions as a "hot" mouse area.  If the user clicks the [left] mouse button while pointing inside the region, then the terminal must transmit the `<text>` string to the Host.  The (x0,y0) parameter MUST be the upper-left corner, and (x1,y1) MUST be the lower-right corner of the region.

The `<num>` parameter used to be used in the older RIPscrip v1.0 specification but is now obsolete.  For upwards compatibility, it should be set to "00". *(v1.54)*

The `<clk>` parameter, if 1, indicates that the region should be visibly inverted while the mouse button is down.  This offers visual feedback.  If `<clk>` is 0, the region will not be inverted while clicked.

The `<clr>` parameter, if 1, will physically zoom the text window to full screen size and clear the screen.  This is useful if the `<text>` parameter instructs the host to enter an area of the System that doesn't support RIPscrip graphics.

The `<text>` parameter is a Host command that gets sent when the field is clicked.  You may use a caret (^) or a backquote (`` ` ``) to represent control characters, (e.g., ^M for carriage return, `` `G ``, ^C, etc.).  Mouse fields may use Data text variables, active text variables, pick lists, control characters and local file playback directives.  At no time are templates permitted in simple mouse fields.  See the section on host commands for more details about these types of features of RIPscrip. *(v2.A3)*

Mouse fields are always created relative to port #0 (the screen port).  Under no circumstances are mouse fields allowed in offscreen drawing ports - you can't have a clickable mouse area in a region that cannot be clicked (ie, off the screen).  If the current drawing port is an offscreen port, or even a port other than port #0, then the mouse field is defined relative to screen port #0 for the purposes of defining its position.  The same applies to mouse-based buttons (see the [RIP_BUTTON](#rip_button) command). *(v2.A3)*

Mouse fields that extend partially outside of a viewport are truncated to fit inside the viewport. *(v2.A3)*

When this command is stored in-memory, it is converted to global screen coordinates (for internal storage only).  This makes it so that if you have mouse regions defined in multiple different viewports, that each field will be properly inverted at the right location regardless of the currently defined viewport. *(v1.54)*

NOTE: All Mouse Fields are scanned in "last in, first out" order.  This means that the last-most received Mouse Field will be the first one scanned for a mouse click.  You are limited to a maximum of 128 Mouse Regions or Mouse Buttons (128 total). *(v1.54)*

## RIP_PLAY_AUDIO

*Added in RIPscrip v2.A3.*

*Play a local digitized audio file on the terminal*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `w` |
| **Arguments** | `res:4 filename` |

**Format:** `!|1w <res> <filename>`
**Example:** `!|1w0000filename.wav`

**Attributes used:** none

This command will physically start a local .WAV file to play back on the user's remote terminal.  The audio file must already exist in the user's hard disk in the same location as their bitmapped icons.  If the file doesn't exist, then this command does nothing.  If you specify the `<filename>` parameter as `$OFF$` then you will in effect be shutting off any currnetly running sound file.  If a sound is already playing when this command is received, the older file that was playing will be terminated in place for the new file.  You may use [RIP_ENTER_BLOCK_MODE](13-level-3-9-commands.md#rip_enter_block_mode) to transfer audio files to the user's computer for subsequent playback.

NOTE:  The `<res>` parameter is for future expansion.  For compatibility with future relases of RIPscrip, it should be set to "0000".

## RIP_PUT_IMAGE

*Pastes the clipboard port's contents to another port*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `P` |
| **Arguments** | `x:XY y:XY mode:2 res:1` |

**Format:** `!|1P <x> <y> <mode> <res>`
**Example:** `!|1P0011010`

**Attributes used:** Viewport, Port, Base Math (current setting)

This command takes the contents of the port identified by the clipboard pointer (see the [RIP_GET_IMAGE](#rip_get_image) command above) and copies it to a position (x,y) in the currently active drawing port.  If the two ports refer to the same port number, then this command is ignored.  If the clipboard pointer is clear (ie, no clipboard port is in use), then this command is also ignored. *(v2.A3)*

The area in the active drawing port that is overwritten with the graphics data is determined by the dimensions of the clipboard port previously established with a RIP_GET_IMAGE function. *(v2.A3)*

The position on the active drawing port that is to receive the image is specified by the (x,y) parameters.  These define the upper left corner that will be the rectangle to receive the image data.  If any part of the image would go off the edge of the viewport (or the screen) then it is truncated appropriately. *(v2.A3)*

The image that is retrieved from the clipboard port does so based on the port's viewport (the entire contents of the viewport are copied to the active drawing port).  If the clipboard port is only used with the RIP_GET_IMAGE command, then the viewport will be the full size of the port.  If however, some port related commands are used on this clipboard port, then it is possible to alter the viewport inside this port.  This gives you the ability to paste sub-areas of the port's actual image onto the destination drawing port. *(v2.A3)*

The `<mode>` parameter defines "how" the image will be pasted on the active drawing port:

| Mode | Description | Logical |
|---|---|---|
| 00 | Paste the image on-screen normally | (COPY) |
| 01 | Exclusive-OR  image with the one already in the port | (XOR) |
| 02 | Logically OR  image with the one already in the port | (OR) |
| 03 | Logically AND image with the one already in the port | (AND) |
| 04 | Paste the inverse of the image in the port | (NOT) |

NOTE:  The 1-byte `<res>` parameter is RESERVED FOR FUTURE USE and should be set to zero.

## RIP_QUERY

*Query the contents of a text variable*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `Σ` (the Escape character, ASCII 27) |
| **Arguments** | `mode:1 window_num:1 res:2 query_text` |

**Format:** `!|1<escape> <mode> <window_num> <res> <query_text>`
**Example:** `!|1<escape>2Q00this is a query $COMMAND$^m`

**Attributes used:** Base Math (current setting)

NOTE: Σ is used to indicate the Escape character (ASCII 27 or ESC).

The Query Text Variable RIPscrip command instructs the terminal to immediately respond with some piece of information, whether statically stored (i.e., in a database), stored internally in RAM (dynamic information), or pre-defined Text Variables.

This command is unique in RIPscrip in the fact that the command character that is used is NOT a printable character.  We use the escape character (ASCII 27) to introduce this command as a measure of security.  Since the query command can query the terminal for some critical (potentially private) information, you would not want a user to be able to query another user's terminal for something like his address information, or something that he wouldn't want to otherwise divulge to unauthorized people.  Since most hosts do not allow the user to enter an escape character, this character is ideal for this purpose.  Using escape allows only the Host (under most circumstances) to be in control of any queries.

NOTE:  This command is very flexible in that you can specify control characters, pick-list definitions and Text Variables.  See the section entitled HOST COMMANDS for a more detailed explanation of these features.

Whether the information is transmitted instantly or not is dependent on the `<mode>` parameter.  The `<mode>` parameter determines when data queries are processed.  The possible settings for the `<mode>` parameter are as follows:

| Mode | Description |
|---|---|
| 0 | Process the query command NOW (upon receipt) |
| 1 | Process when mouse clicked in the current Graphics Viewport *(v2.A0)* |
| 2 | Process when mouse clicked in the current Text Window (any text variables that return X or Y mouse coordinates return TEXT coordinates, not graphics coordinates in this mode.  These coordinates are two-digit values instead of the graphical values that are four digits). *(v2.A0)* |
| 3 | Process when mouse clicked in the Graphics Viewport specified in the `<window_num>` parameter.  If that viewport doesn't exist (hasn't been defined yet), then this query command is ignored. *(v2.A0)* |
| 4 | Process when mouse clicked in the Text Window specified in the `<window_num>` parameter.  Any text variables that return X or Y mouse coordinates return TEXT coordinates based on that window's MicroANSI text font sizes, not graphics coordinates in this mode.  These coordinates are two-digit values instead of the graphical values that are four digits). *(v2.A0)* |

Note that modes 1-4 do not return the results of the Query instantly.  They query commands are processed when the user clicks the mouse either in the text window, or in the graphics window respectively.  Which text window or graphics viewport is based both on the value of `<mode>` parameter and on the `<window_num>` parameter. If the `<mode>` is either a 1 or 2, then the current window/viewports are the areas that are acted upon.  Modes 3 and 4 will take the window number from the `<window_num>` parameter as the text window/viewport to act upon.  If the associated window or viewport are not defined, then the query command is ignored.  These "queries after mouse clicks" are only acted upon if the user is clicking on something other than a Button or a Mouse Field. To disable these two special "deferred" query modes, issue the same command with the query string of `$OFF$`.  This will disable this mode.  Providing a `<text>` parameter of anything other than `$OFF$` will produce a revised query command (active). *(v2.A0)*

If more than one query command can be acted uon (ie, multiple viewport queries and/or text window queries are found that the mouse was clicked inside of), then processing of queries happens in a very specific order.  Note that all affected queries are processed - none are skipped.  The order in which queries are acted upon are as follows: *(v2.A4)*

1. The port/viewport table is scanned from 0-35.  If a query is found for a port that isn't deactivated, and the mouse was clicked inside that region (for screen ports only), then the query is processed and the next port/viewport is checked.
2. The text window table is scanned from 0-35.  If a query is found for a text window that isn't deactivated, and the mouse was clicked inside that window's display area, then the query is processed and the next text window is checked.
3. If a "current viewport" query, otherwise known as a floating viewport query, is found and the mouse was clicked inside the current viewport, then the floating viewport query is processed.
4. If a "current text window" query, otherwise known as a floating text window query, is found and the mouse was clicked inside the text window's display area, then the floating text window query is processed.

When the user clicks the mouse down, then all queries are checked.  The point where the mouse was originally clicked is the one that is checked for with the queries.  After all queries are processed, then the mouse click is ignored until the user releases the mouse button. *(v2.A4)*

Basically put, a Query command will be immediately acted upon by the Terminal program when received if the `<mode>` is 0.  The Query command's `<text>` parameter can contain any number of Host Command "segments", which can instruct the terminal "how to" send data to the host, and more specifically, what data to send to the host.

Some examples of query statements might be any of the following:

```text
                      ^m     Send a carriage return to the BBS now!
My name is $FULL_NAME$^m     Send text "My name is <insert-name-
                             here>" followed by a  carriage return
                             to the BBS.  The <insert-name-here>
                             will be replaced with whatever the
                             variable $FULL_NAME$ contains.
```

See the section entitled HOST COMMANDS for a detailed explanation of Host Commands, and what you can do with the Query command.

NOTE:  The `<res>` parameter is reserved for future use by TeleGrafix Communications, Inc..  It should be set to 000 for compatibility with future releases.

It should be mentioned that text window queries are performed before any viewport queries when resident queries are concerned. *(v2.A1)*

## RIP_READ_SCENE

*Playback local .RIP file*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `R` |
| **Arguments** | `res:8 filename...` |

**Format:** `!|1R <res> <filename>`
**Example:** `!|1R00000000testfile.rip`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Font Style, Port (dependent on the contents of the source file), Base Math (current setting)

This command instructs the remote terminal to playback a local .RIP file.  The current execution of RIPscrip commands will be temporarily suspended and the contents of the designated RIP file will begin executing.  Regardless of whether or not the current RIPscrip code coming across the modem is in the middle of a line or not, the RIP playback file will be assumed to start at the beginning of a line.  Therefore, if a RIP_READ_SCENE command is located in a .RIP file, it must be the very last command on the line, followed by a carriage return instead of a command delimiter (`|`).  This ensures that the loaded .RIP file will begin executing properly with the correct delimiters found in the correct places.

The RIP playback file can alter colors, fonts, or whatever.  Once the playback of the file is complete, the remaining RIPscrip code that was temporarily suspended will be resume execution.  Any changes that appeared in the loaded playback file will remain in effect when the resumed code is processed.  In other words, if you change a color or a font in the playback file and leave them changed, they will remain in effect during the resumed execution.

NOTE:  The `<res>` parameter is reserved for future development by TeleGrafix.  It should be set to "00000000" for compatibility with future releases.

## RIP_REGION_TEXT

*Display a line of text in rectangular text region*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `t` |
| **Arguments** | `justify:1` and text-string |

**Format:** `!|1t <justify> <text-string>`
**Example:** `!|1t1This is a text line to be justified`

**Attributes used:** Draw Color, Write Mode, Font Style, Viewport, Port, Base Math (current setting)

A number of these commands may come sandwiched between the [RIP_BEGIN_TEXT](#rip_begin_text) and [RIP_END_TEXT](#rip_end_text) commands.  The `<text-string>` should already word-wrapped in such a way that it will fit inside the rectangular region based on the current font, font size, and drawing color.

There are four possible settings for the `<justify>` parameter: *(v2.A3)*

| Justify | Description |
|---|---|
| 0 | Left justify this line |
| 1 | Right/left "full" justify this line |

If a text line falls off the bottom of the region, it is discarded -- the rectangular Text Region does not scroll.

This command is intended to import some sort of text file document directly into a RIPscrip scene and format it nicely to fit inside a simple rectangular area.  If the `<justify>` parameter is set to a value of "1" for a given RIP_REGION_TEXT line, then that line will be justified to both the left and right margins (the RIP_BEGIN_TEXT boundaries).  This is so that the displayed text aligns on both sides with the invisible boundaries.  This "justification" is done by splitting each RIP_REGION_TEXT line up into chunks of word-groups, broken up at their "white-space" locations.  Each spacer is then padded by however many pixels are necessary to keep each spacer of approximately equal size.  Only enough spare pixels are added to make sure that the right-edge of the text region aligns with the right border of the boundary.  The result is a nicely formatted text block. *(v1.54)*

## RIP_SCROLL

*This command was formerly RIP_COPY_REGION.*

*Copy (scroll) screen region up/down*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `G` |
| **Arguments** | `x0:XY y0:XY x1:XY y1:XY mode:1 res:1 dest_line:2` |

**Format:** `!|1G <x0> <y0> <x1> <y1> <mode> <res> <dest_line>`
**Example:** `!|1G080G140M0005`

**Attributes used:** Port, Base Math (current setting)

This command physically "copies" a rectangular region of the graphics screen up or down.  The `<dest_line>` parameter is the Y position that is the destination scan line to receive the region.  The Destination of the copy can overlap the original region, but cannot be on the same line.  You cannot move the image area left or right at all.  This command is designated for vertical scrolling of graphical data either up or down.

This is one of the only graphical output commands that DOES NOT adhere to the [RIP_VIEWPORT](10-level-0-commands-s-w.md#rip_viewport) command.  In other words, you can scroll graphical data outside the current graphical viewport (even over the text window!).  However, it does adhere to the boundaries of the current drawing port. *(v2.A3)*

The `<mode>` parameter instructs the terminal what to do with the part of the source rectangle that isn't covered up by the scrolled data.  You may specify several things to happen as defined by the mode values below: *(v2.A1)*

| Mode | Description |
|---|---|
| 0 | Do nothing - leave the old graphics on the screen |
| 1 | Fill the old graphical area with the current drawing color (solid fill in COPY mode). |
| 2 | Fill the old graphical area with the current background drawing color (solid fill in COPY mode). |
| 3 | Fill the area with the current fill color (solid fill in COPY mode).  Fill pattern/styles are not used for this. |
| 4 | Fill the old graphical area with the current fill style/pattern in the current fill color. |
| 5 | Erase the affected area entirely to black. |
| 6 | Fill the affected area with the color of a pixel from one of the corners of the affected rectangle.  If the image area was scrolled up, then the color is taken from the lower-right most pixel from the old graphical rectangle (left exposed).  If the data is scrolled down, then the rectangle is filled in with the color of the pixel in the upper-left most pixel.  This is mode is used so that if the area is a multi-colored graphic, that a suitable fill color is used. |

## RIP_SET_MOUSE_CURSOR

*Added in RIPscrip v2.A1.*

*Sets the mouse cursor (pointer) to various shapes*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `c` |
| **Arguments** | `cursor_style:2 res:4` |

**Format:** `!|1c <cursor_style> <res>`
**Example:** `!|1c030000`

**Attributes used:** Base Math (current setting)

This command allows you to alter the current mouse cursor shape that is shown on the screen.  This feature lets you change the cursor to another shape to give the user of the terminal an idea that something is going on.  For example, changing the cursor to the shape of an hourglass or a watch could give the idea that they should wait for a moment because something is going on.

The `<cursor_style>` parameter is an ID number that specifies which cursor style should be activated.  The possible values for this parameter are: *(v2.A4)*

| Style | Description |
|---|---|
| 00 | Standard "left-leaning" arrow cursor (default) |
| 01 | Wrist watch cursor |
| 02 | Cross-hair cursor |
| 03 | I-bar cursor (for text editing) |
| 04 | Pointing finger cursor |
| 05 | Hand held up cursor |
| 06 | Hourglass cursor |

When a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) or a [RIP_HEADER](09-level-0-commands-g-r.md#rip_header) command is executed, the mouse cursor is automatically reset to the default, standard pointer cursor.

## RIP_WRITE_ICON

*Write contents of the clipboard port to disk*

| | |
|---|---|
| **Level** | 1 |
| **Command** | `W` |
| **Arguments** | `res:1 filename` |

**Format:** `!|1W <res> <filename>`
**Example:** `!|1W0filename.icn`

**Attributes used:** Base Math (current setting)

This command takes the contents of the clipboard port and writes it to disk as a BMP type bitmap file.  The image written to disk is the image contained inside the viewport of the clipboard port (see the [RIP_PUT_IMAGE](#rip_put_image) command for more details about viewports and the clipboard port).  If the clipboard pointer indicates that there is no clipboard in use, then this command does nothing. *(v2.A3)*

Once the file is written to disk, you may subsequently use the [RIP_LOAD_ICON](#rip_load_icon) or [RIP_LOAD_BITMAP](#rip_load_bitmap) commands to load the icon back onto a drawing port. *(v2.A3)*

Path or sub-directory information is not allowed in the filename portion of the command.  If an Icon by the same name already exists on disk, it is overwritten. *(v2.A3)*

NOTE:  The `<res>` parameter is reserved for future use by TeleGrafix.  For future compatibility, it should be set to "0". *(v2.A3)*

---

[◀ Prev: Level-0 Commands (S–W)](10-level-0-commands-s-w.md) | [Contents](README.md) | [Next: Level-2 Commands ▶](12-level-2-commands.md)
