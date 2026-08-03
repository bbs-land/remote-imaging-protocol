# Text Variables: Environment, Clipboard, Screen & Tables

[◀ Prev: Text Variables: Terminal & Reset](19-text-variables-terminal.md) • [Contents](README.md) • [Next: Icon/DIB File Format ▶](21-icon-file-format.md)

---

## System Environment Configuration Text Variables

These text variables return information on, or manipulate the basic RIPscrip environment. *(v2.A4)*

### $BASEMATH$

*Set/query base math for RIPscrip*

*Added in RIPscrip v2.A4.*

**Format:** `$BASEMATH(env_no, setting)$`

**Syntax:**

```text
$BASEMATH(opt:ENV_NO, opt:SETTING)$
              ──────      ───────
              CUR         36
              0-35        64
              ──────      ───────
          def=CUR     def=36
```

This command allows you to set or query the setting of the RIPscrip [Base Math](05-coordinates-and-math.md) configuration.  Valid base math settings are 36 (MegaNums) or 64 (UltraNums).

If you specify no parameters, then the base math of the current environment is returned ("36" or "64").

If you specify one parameter, then you are querying the base math of a specific environment.  You must specify the environment as a number from 0-35, or the value "CUR" to indicate the current environment.  If the environment isn't in use, then a value of "NONE" is returned.

If you specify two parameters, then the first parameter must indicate which environment you're about to set (see above), and the second parameter must be the value "36" or "64".  If the environment isn't in use then a syntax error is generated.  When setting the base math, you are accomplishing the same thing as if you had used the [RIP_SET_BASE_MATH](10-level-0-commands-s-w.md#rip_set_base_math) command.

**Example:** `$BASEMATH(CUR, 36)$`
**Returns:** nothing

### $CLS$

*Clears the screen to background color (no reset)*

*Added in RIPscrip v2.A1.*

**Format:** `$CLS$`

**Syntax:** `$CLS$`

This command physically erases the entire screen to the current background color (color #0).  No resetting of anything is performed.  All this does is simply clear the screen.

**Example:** `$CLS$`
**Returns:** nothing

### $COLORMODE$

*Query/alter the color mode*

**Format:** `$COLORMODE(env_no, mode, bits)$`

**Syntax:**

```text
$COLORMODE(opt:ENV_NO, opt:MODE, opt:BITS)$
               ──────      ────      ────
               CUR         PAL       1-8:MODE=RGB
               0-35        RGB
               ──────      ────      ────
           def=CUR     def=PAL   def=<none>
```

If this variable has no parameters, then it queries the [color mode](06-color-audio-text.md) of the current environment.  It responds with a number that indicates whether or not the terminal is in direct RGB encoding mode or if it is in color palette mapping mode.  If the value is 0, then it is palette mapping mode.  If it a value from 1-8, then it is in direct RGB encoding mode and the value indicates how many bits of precision are used for the red, green and blue color components separately in RGB encoded data (a value of 8 indicates that 3x8 bits are used, or that we will be working with 24-bit color numbers). *(v2.A4)*

If only one parameter is supplied, then it must be the environment number to query (0-35), or the value CUR to query the current environment.  In either case, if the environment is in use, then the value returned is the same as if there were no parameters, based on the settings of that particular environment.  If that environment isn't in use, then a value of "-1" is returned. *(v2.A4)*

If you supply two or more parameters, then you are indicating that you wish to change the color mode setting.  To set the color mode to color palette mapping mode, supply the `<mode>` parameter as the value "PAL".  No other parameters are needed in this case.  To set RGB encoding mode, specify the `<mode>` keyword of "RGB" and then you must supply a third parameter which indicates the number of bits of precision from 1-8 (see above).  If the specified environment isn't in use, then a syntax error will be generated. *(v2.A4)*

**Example:** `$COLORMODE$`
**Returns:** `0` ... Request setting of current environment color mode.  In this example, that one's in palette mode.

**Example:** `$COLORMODE(CUR, PAL)$`
**Returns:** nothing ... Switch current environment to palette mode

**Example:** `$COLORMODE(5, RGB, 8)$`
**Returns:** nothing ... Enabled direct RGB color mode (bits=8) in environment #5

**Example:** `$COLORMODE(6)$`
**Returns:** `NONE` ... Returned if environment #6 isn't in use.

### $COLORS$

*Total number of colors of current video device*

*Added in RIPscrip v2.A1.*

**Format:** `$COLORS$`

**Syntax:** `$COLORS$`

This variable returns the total number of colors available on the destination video hardware device.  Typical results are 2, 16, 256, 32768, 65536 and 16,777,216 (24 bit color).  These aren't the only possible values, but are typical ones.

**Example:** `$COLORS$`
**Returns:** `256`

### $COMPAT$

*Sets environment to RIPscrip 1.54 settings*

*Added in RIPscrip v2.A4.*

**Format:** `$COMPAT(env_no)$`

**Syntax:**

```text
$COMPAT(opt:ENV_NO)$
            ──────
            CUR
            0-35
            ──────
        def=CUR
```

This text variable is designed to set an environment to older RIPscrip 1.54 settings for "backward compatibility".  If no parameters are specified, then the current environment will be modified to these settings.

If you specify a parameter, then you can only set one and it must be an environment number 0-35, or the value "CUR" for the current environment.  If the destination environment isn't in use, then a syntax error will be generated. The following environment settigns are altered:

- [World Coordinate Frame](05-coordinates-and-math.md) is set to 640x350
- Color mode is set to color palette mapping mode
- Coordinate sizes are set to 2 bytes
- Base math is set to base-36 numbers (MegaNums)
- Baud rate emualtion is set to full speed (0)

**Example:** `$COMPAT(5)$`
**Returns:** nothing

### $COORDSIZE$

*Set byte-size of X/Y coordinates*

*Added in RIPscrip v2.A4.*

**Format:** `$COORDSIZE(env_no, size)$`

**Syntax:**

```text
$COORDSIZE(opt:ENV_NO, opt:SIZE)$
               ──────      ────
               CUR         2-5
               0-35        ────
               ──────  def=2
           def=CUR
```

This command sets or queries the setting of the byte-width of RIPscrip coordinate parameters in raw RIPscrip code.  If no parameters are specified, then this command returns the setting of the coordinate size in the current environment.  The valid return values are "2" through "5".

If you specify only one parameter, then it must be an environment number from 0-35, or it must be "CUR" to indicate the current environment.  If the environment isn't in use when querying it's coordinate size, then the value "-1" is returned.

If you intend to set the environment's coordinate size, then you must specify two parameters.  The first one is the environment number (see above), and the second one must be the coordinate size itself (2, 3, 4 or 5).  If you specify an environment that isn't in use, or an invalid coordinate size value then a syntax error is generated.

**Example:** `$COORDSIZE(CUR, 2)$`
**Returns:** nothing

**Example:** `$COORDSIZE(5)$`
**Returns:** `2`

### $ISPALETTE$

*Reports if a color palette exists or not*

**Format:** `$ISPALETTE$`

**Syntax:** `$ISPALETTE$`

If the destination terminal is operating with a video device that has an actual color palette then this variable returns "1".  If however, the terminal is running on a device that's in 24-bit color mode where there is no color palette then this variable returns "0". *(v2.A4)*

**Example:** `$ISPALETTE$`
**Returns:** `1`

### $RBS$

*Restore a button style from a backup area*

*Added in RIPscrip v2.A4.*

**Format:** `$RBS(source)$`

**Syntax:**

```text
$RBS(opt:SOURCE)$
         ──────
         BASE
         0-9
         POP
         ──────
     def=BASE
```

This text variable restores the button style [data table](03-data-tables.md) from a [data backup area](04-data-backup-areas.md).  If the SOURCE parameter is omitted, then it is restored from the BASE save area of the backup data area.  Possible sources are "BASE" to restore from the base save area, the value 0-9 to read from a specific data save slot, or the value "POP" to indicate that you wish to pop the color palette data table from the button style data backup area's save stack.

**Example:** `$RBS(POP)$`
**Returns:** nothing

### $RCP$

*Restore a color palette from a backup area*

*Added in RIPscrip v2.A4.*

**Format:** `$RCP(source)$`

**Syntax:**

```text
$RCP(opt:SOURCE)$
         ───────────
         BASE
         0-9
         POP
         ───────────
     def=BASE
```

This text variable restores the color palette data table from a data backup area.  If the SOURCE parameter is omitted, then it is restored from the BASE save area of the backup data area.  Possible sources are "BASE" to restore from the base save area, the value 0-9 to read from a specific data save slot, or the value "POP" to indicate that you wish to pop the color palette data table from the color palette data backup area's save stack.

**Example:** `$RCP(POP)$`
**Returns:** nothing

### $RENV$

*Activates a previously snapshotted environment*

*Added in RIPscrip v2.A4.*

**Format:** `$RENV(source)$`

**Syntax:**

```text
$RENV(opt:SOURCE)$
          ──────
          BASE
          POP
          0-9
          ──────
      def=BASE
```

This command restores a previously saved environment data table back into the actual data table and puts it to use.  If you omit the slot parameter, then the environment is restored from the base save area of the environment data backup area.

If the slot value is set to "POP", then the data table is popped off of the environment data backup area's stack.

If the slot value is set to "BASE", then the data table is restored from the base save area of the environment data backup area.

If the slot value is set to a number from 0-9, then the environment data table is restored from a data save slot number in the environment data backup area.

**Example:** `$RENV(POP)$`
**Returns:** nothing

### $RESTOREALL$

*Restore all screen attributes*

**Format:** `$RESTOREALL(source)$`

**Syntax:**

```text
$RESTOREALL(opt:SOURCE)$
                ──────
                BASE
                POP
                0-9
                ──────
            def=BASE
```

This Active Text Variable restores the Text Windows coordinates, restores the contents of the clipboard, restores all mouse fields, and restores the contents of the screen.  It is equal to the following operations (in this order): *(v2.A4)*

- `$RTW(slot)$`
- [`$RCB(slot)$`](#rcb)
- [`$RMF(slot)$`](#rmf)
- [`$RGS(slot)$`](#rgs)
- [`$RBS(slot)$`](#rbs)
- [`$RESTORE(slot)$`](#restore)
- [`$RCP(slot)$`](#rcp)
- [`$RENV(slot)$`](#renv)

This command does not require any parameters.  If none are specified then the restore is slot-less and can be restored many times.  The ability to specify a slot gives you the ability to have restore one of many screen configurations.  You are allowed up to ten separate slots (0-9). *(v2.A1)*

In place of a slot number, you can provide a parameter of "POP" to perform a stack-based pop operation to restore the last pushed SAVEALL (eg, `$RESTOREALL(POP)$`). *(v2.A1)*

**Example:** `$RESTOREALL(0)$`
**Returns:** nothing

### $RESX$

*Horizontal resolution of current video device*

*Added in RIPscrip v2.A1.*

**Format:** `$RESX$`

**Syntax:** `$RESX$`

This variable returns the horizontal resolution of the video device in pixels.  Typical results for this might be 640, 800, 1024 or 1280. But this might be different depending on the various possible video devices in existance.

**Example:** `$RESX$`
**Returns:** `640`

### $RESY$

*Vertical resolution of current video device*

*Added in RIPscrip v2.A1.*

**Format:** `$RESY$`

**Syntax:** `$RESY$`

This variable returns the vertical resolution of the video device in scan lines.  Typical results for this might be 350, 480, 600, 768 or 1024.  But this might be different depending on the various possible video devices in existance.

**Example:** `$RESY$`
**Returns:** `768`

### $RGS$

*Restore a graphics style from a backup area*

*Added in RIPscrip v2.A4.*

**Format:** `$RGS(source)$`

**Syntax:**

```text
$RGS(opt:SOURCE)$
         ──────
         BASE
         0-9
         POP
         ──────
     def=BASE
```

This text variable restores the graphics style data table from a data backup area.  If the SOURCE parameter is omitted, then it is restored from the BASE save area of the backup data area.  Possible sources are "BASE" to restore from the base save area, the value 0-9 to read from a specific data save slot, or the value "POP" to indicate that you wish to pop the color palette data table from the graphics style data backup area's save stack.

**Example:** `$RGS(POP)$`
**Returns:** nothing

### $PALENTRY$

*Return RGB values of palette*

*Added in RIPscrip v2.A4.*

**Format:** `$PALENTRY(palno,start,stop)$`

**Syntax:**

```text
$PALENTRY(req:PALNO, req:START, opt:STOP)$
              ─────      ─────      ────
              CUR        0-255      START-255:START<>ALL
              0-35       ALL        ────
                                def=START
```

This variable returns one or more RGB values stored in one of the drawing color palettes.  The `<pal>` parameter determines which palette in the palette data table is to be inquired about, and its value can be from 0-35 to explicitly reference a specific color palette number, or it can be CUR to indicate the current color palette.

The `<start>` parameter must be specified, and indicates which palette entry in the palette is to be inquired about.  If the `<stop>` parameter is omitted, then only one entry will be queried, the `<start>` entry.  If `<stop>` is supplied, then it must be equal to or greater than `<start>` (not to exceed 255).  So if `<start>` is 5 and `<stop>` is 7 then three palette entries will be inquired about, starting with entry #5.

If the palette isn't in use at all, then this variable returns the value "-1" to the host.  If it is in use, then the string will be a formatted block of text.  The format of the return string to the host is:

```text
<bits> ; <pal-entry> [, <pal-entry> ...]
```

The `<bits>` field is the total number of bits of precision for the red, green or blue fields.  If the `<bits>` is 8, then red values can range from 0-255, etc.  The `<pal-entry>` is a segmented response in the format:

```text
red:green:blue
```

where each of red, green and blue are decimal numbers from 0 up to the total number of values allowed based on the `<bits>` parameter.

If more than one palette entry is requested, then eacch `<pal-entry>` field will be separated by a comma delimiter (,) as in the following example:

**Example:** `$PALENTRY(CUR, 5, 7);`
**Returns:** `6;42:0:42,42:21:0,42:42:42`

Notice how the `<bits>` is specified as "6;".  RIPterm maintains its own internal palette with 6 bits of precision - this isn't the only way to do it, its just the way RIPterm does it currently.  With 6 bits of precision, each red, green or blue component will never exceed 2 bytes of data (8 bits could occupy 3 bytes).

**Example:** `$PALENTRY(CUR, 15)$`
**Returns:** `8;255:255:255`

Lastly, the `<start>` parameter could be specified as "ALL".  If this parameter is present, then `<stop>` isn't required.  If ALL is specified, then all palette entries from 0-255 are returned as in the the following:

**Example:** `$PALENTRY(CUR, ALL)$`
**Returns:** `6;0:0:0,0:0:42, ... 63:63:42,63:63:63`

### $SAVEALL$

*Save all screen attributes*

**Format:** `$SAVEALL(destination)$`

**Syntax:**

```text
$SAVEALL(opt:DESTINATION)$
             ───────────
             BASE
             PUSH
             0-9
             ───────────
         def=BASE
```

This Active Text Variable saves the Text Window coordinates, the contents of the entire clipboard, all mouse fields, and the contents of the entire screen.  It is the same as performing the following operations (in this order): *(v2.A4)*

- `$STW(slot)$`
- [`$SCB(slot)$`](#scb)
- [`$SMF(slot)$`](#smf)
- [`$SGS(slot)$`](#sgs)
- [`$SBS(slot)$`](#sbs)
- [`$SAVE(slot)$`](#save)
- [`$SCP(slot)$`](#scp)
- [`$SENV(slot)$`](#senv)

This command does not require any parameters.  If none are specified then the saved information is slot-less and can be restored many times.  If you specify a slot number then that slot number is the slot that will be saved over.  You are allowed up to ten separate slots (0-9). *(v2.A2)*

In place of a slot number, you can provide a parameter of "PUSH" to perform a push-type stack saving operation (eg, `$SAVEALL(PUSH)$`). *(v2.A1)*

**Example:** `$SAVEALL(0)$`
**Returns:** nothing

### $SBS$

*Save a button style to the backup area*

*Added in RIPscrip v2.A4.*

**Format:** `$SBS(destination)$`

**Syntax:**

```text
$SBS(opt:DESTINATION)$
         ───────────
         BASE
         0-9
         PUSH
         ───────────
     def=BASE
```

This text variable saves the button style data table to a data backup area.  If the DESTINATION parameter is omitted, then it is saved to the BASE save area of the backup data area.  Possible destinations are "BASE" to save over the base save area, the value 0-9 to overwrite a specific data save slot, or the value "PUSH" to indicate that you wish to push the color palette data table onto the button style data backup area's save stack.

**Example:** `$SBS(PUSH)$`
**Returns:** nothing

### $SCP$

*Save a color palette to the backup area*

*Added in RIPscrip v2.A4.*

**Format:** `$SCP(destination)$`

**Syntax:**

```text
$SCP(opt:DESTINATION)$
         ───────────
         BASE
         0-9
         PUSH
         ───────────
     def=BASE
```

This text variable saves the color palette data table to a data backup area.  If the DESTINATION parameter is omitted, then it is saved to the BASE save area of the backup data area.  Possible destinations are "BASE" to save over the base save area, the value 0-9 to overwrite a specific data save slot, or the value "PUSH" to indicate that you wish to push the color palette data table onto the color palette data backup area's save stack.

**Example:** `$SCP(PUSH)$`
**Returns:** nothing

### $SENV$

*Records environmental configuration*

*Added in RIPscrip v2.A4.*

**Format:** `$SENV(destination)$`

**Syntax:**

```text
$SENV(opt:DESTINATION)$
          ───────────
          BASE
          PUSH
          0-9
          ──────────
      def=BASE
```

This command takes a snapshot of the RIPscrip configuration over the one of the data backup area's particular save regions.  This includes recording the values of:

- Current graphics style entry number
- Current button style entry number
- Current drawing port entry number
- Current text window entry number
- Current color palette entry number
- Current World coordinate dimensions (X and Y)
- Current base math settings (36 or 64)
- Current coordinate size (2 through 5)
- Current color mode (color palette mode or direct RGB mode)
- Current mouse pointer number
- Current baud rate emulation value
- Current environment data table entry currently active

This command saves an environment data table over an actual data backup area for later retrieval.  If you omit the slot parameter, then the environment is stored into the base save area of the environment data backup area.

If the slot value is set to "PUSH", then the data table is pushed onto the environment data backup area's stack.

If the slot value is set to "BASE", then the data table is stored into the base save area of the environment data backup area.

If the slot value is set to a number from 0-9, then the environment data table is stored into a data save slot number in the environment data backup area.

**Example:** `$SENV(PUSH)$`
**Returns:** nothing

### $SGS$

*Save a graphics style to the backup area*

*Added in RIPscrip v2.A4.*

**Format:** `$SGS(destination)$`

**Syntax:**

```text
$SGS(opt:DESTINATION)$
         ───────────
         BASE
         0-9
         PUSH
         ───────────
     def=BASE
```

This text variable saves the graphics style data table to a data backup area.  If the DESTINATION parameter is omitted, then it is saved to the BASE save area of the backup data area.  Possible destinations are "BASE" to save over the base save area, the value 0-9 to overwrite a specific data save slot, or the value "PUSH" to indicate that you wish to push the color palette data table onto the graphics style data backup area's save stack.

**Example:** `$SGS(PUSH)$`
**Returns:** nothing

### $WORLD$

*Set/query World coordinate frame*

*Added in RIPscrip v2.A4.*

**Format:** `$WORLD(env_no,width,height)$`

**Syntax:**

```text
$WORLD(opt:ENV_NO, opt:WIDTH,  opt:HEIGHT)$
           ──────      ─────       ──────
           CUR         1-65535     1-65535
           0-35        DEFAULT     DEFAULT
           ──────      ─────       ──────
       def=CUR     def=<none>  def=<none>
```

This command allows you to set or query an environment's world coordinate frame.

If you specify no parameters, then you are querying the contents of the current environment's world coordinate frame.  The return value to the host will be a value similar to the following "1234:5678" where "1234" is the width of the coordinate frame in the horizontal X direction, and the value "5678" is the height of the coordinate frame in the vertical Y direction.

If you specify one parameter, then you are querying the world coordinate frame of a specific environment from 0-35 or "CUR" for the the current environment.  The result returned to the host is in the same format as if you specified no parameters (see above).  If the requested environment isn't in use, then the value "-1" is returned.

If you wish to set the world coordinate frame for an environment, you have two choices.  You can set it to some basic set of default values or you can set it to a specific height and width.

If you wish to set the world coordinate frame to a set of default values then you specify two parameters - the environment number (0-35 or CUR), and the second parameter must be the value "DEFAULT".  The default width and height values will depend on that environment's Base Math setting.  If MegaNums are in use in that environment, then the world frame is set to 1280x960.  If it is UltraNums, it is set to 4096x3072.

If you wish to manually set the width and height of the world coordinate frame, then you must specify all three parameters.  The first one is the environment (0-35 or CUR).  The second parameter is the width of the world coordinate frame and the third and final parameter is the height of the coordinate frame.  Both of these values can not exceed the value 65535.  If the specified environment isn't in use, then a syntax error is generated.  This variation of `$WORLD` is the same as using the [RIP_SET_WORLD_FRAME](10-level-0-commands-s-w.md#rip_set_world_frame) RIPscrip command.

**Example:** `$WORLD(CUR,1000,1000)$`
**Returns:** nothing ... Sets current environment world frame to 1000x1000

**Example:** `$WORLD(5, DEFAULT)$`
**Returns:** nothing ... Sets environment 5's world frame to 1280x960 if MegaNums, or 4096x3072 if it's in UltraNum mode.

**Example:** `$WORLD(CUR)$`
**Returns:** `640:350` ... Returns world frame of current environment

### $WORLDH$

*Vert resolution of world coord system*

**Format:** `$WORLDH(env_no,height)$`

**Syntax:**

```text
$WORLDW(opt:ENV_NO, opt:HEIGHT)$
            ──────      ─────
            CUR         1-65535
            0-35        ─────
            ──────  def=<none>
        def=CUR
```

This function is used to either set or query the setting of the world coordinate system in the Y direction.  If no parameters are specified (eg, `$WORLDY$`) or a single "CUR" parameter is specified, then the current environment's world coordinate Y dimension is returned to the host.  If you would like to inquire about a specific environment's world Y setting, specify the environment table entry number as the only parameters (eg, `$WORLDY(5)$` inquires about environment #5's world coordinate Y setting). *(v2.A4)*

If you would like to alter the world coordinate system's Y setting, you need to specify two parameters.  The first parameter must be a table entry from 0-35 or the value CUR for the current environment. The second parameter must be a world coordinate setting to set the Y dimension to. *(v2.A4)*

Note, if you try to a value in an environment that is not in use, this command generates a syntax error.  If you attempt to query a value from an environment that is not in use, the value "-1" is returned to the host. *(v2.A4)*

**Example:** `$WORLDH$`
**Returns:** `4096`

**Example:** `$WORLDH(CUR, 1000)$`
**Returns:** nothing

### $WORLDW$

*Horiz resolution of world coord system*

**Format:** `$WORLDW$(env_no,width)`

**Syntax:**

```text
$WORLDW(opt:ENV_NO, opt:WIDTH)$
            ──────      ─────
            CUR         1-65535
            0-35        -----
            ──────  def=<none>
        def=CUR
```

This function is used to either set or query the setting of the world coordinate system in the X direction.  If no parameters are specified (eg, `$WORLDX$`) or a single "CUR" parameter is specified, then the current environment's world coordinate X dimension is returned to the host.  If you would like to inquire about a specific environment's world X setting, specify the environment table entry number as the only parameters (eg, `$WORLDX(5)$` inquires about environment #5's world coordinate X setting). *(v2.A4)*

If you would like to alter the world coordinate system's X setting, you need to specify two parameters.  The first parameter must be a table entry from 0-35 or the value CUR for the current environment. The second parameter must be a world coordinate setting to set the X dimension to. *(v2.A4)*

Note, if you try to a value in an environment that is not in use, this command generates a syntax error.  If you attempt to query a value from an environment that is not in use, the value "-1" is returned to the host. *(v2.A4)*

**Example:** `$WORLDW$`
**Returns:** `4096`

**Example:** `$WORLDW(CUR, 1000)$`
**Returns:** nothing

## Clipboard Related Text Variables

These text variables return information on, or manipulate the clipboard aspects of the port system. *(v2.A4)*

### $PCB$

*Paste Clipboard at last location*

*Added in RIPscrip v2.A4.*

**Format:** `$PCB(portno)$`

**Syntax:**

```text
$PCB(opt:PORTNO)$
         ──────
         CUR
         0-35
         ──────
     def=CUR
```

This text variable copies a portion of the specified port onto the current drawing port (these ports may be the same).  The area on the specified port that is considered to be the source is "remembered" from the last rectangle of data that was put on this port.  What this means is when you perform a `RIP_PORT_COPY` command, you copy a rectangle of data from one port to another.  The two rectangles of information are stored in the destination port of the copy operation. If the original port were port 0 and the destination port were port 5, then both rectangles of data are stored in port 5's definition. If you switch to port #0 and then perform a `$PCB(5)$` operation, then the rectangle of data on port #5 is copied back to its original location in the current port.

This variable is extremely useful for restoring graphics that were previously saved to an offscreen port.  It is typically used in dialog boxes when the user clicks on the "OK" button, where the dialog box should be erased and the original graphical screen would be restored.

If no parameters are specified, then the port that will be pasted from will be whatever port is associated with the clipboard pointer. See the `RIP_GET_IMAGE` command for more details on the clipboard pointer.

If you do specify a parameter, then it can be "CUR" to indicate the current port, or a value 0-35 to specify a specific port number to retrieve the data from.  Note that if the specified port doesn't exist, or if either the current port or the specified port are "deactivated", then this command is ignored.

It should be noted that the rectangles of information stored in the specified port are "viewport relative".  What this means is that the coordinates in those rectangles are based on viewports.  If you copy data from say port 0 to port 5, then change port 0's viewport before you perform a `$PCB(5)$` operation, then when you actually perform the `$PCB(5)$` command, the data will be pasted in a new location on the screen - not where it came from.  If you're going to be using this command, be careful how you manipulate your viewports.

**Example:** `$PCB(5)$` ... Paste port 5's data to the current port
**Returns:** nothing

### $RCB$

*Restore Clipboard*

**Format:** `$RCB$(source)`

**Syntax:**

```text
$RCB(opt:SOURCE)$
         ──────
         BASE
         POP
         0-9
         ──────
     def=BASE
```

This Active Text Variable restores the Clipboard from a previously executed [`$SCB$`](#scb) command.  Not only are the clipboard contents saved, but so is the last clipboard location, so Paste Clipboard ([`$PCB$`](#pcb)) restores the clipboard's contents AND location.

If you do not specify a slot parameter then the clipboard is restored from a slot-less file.  It is not deleted so you can do multiple identical clipboard restorations. *(v2.A1)*

If you do specify a slot parameter then you may restore the clipboard from any of ten different clipboard slots (0-9).  Once restored, the clipboard slot file is deleted. *(v2.A1)*

You may specify a slot number of "POP" to perform a stack-based pop operation (eg, `$RCB(POP)$`). *(v2.A1)*

**Example:** `$RCB(4)$`
**Returns:** nothing

### $SCB$

*Save Clipboard*

**Format:** `$SCB(destination)$`

**Syntax:**

```text
$SCB(opt:DESTINATION)$
         ───────────
         BASE
         PUSH
         0-9
         ───────────
     def=BASE
```

This Active Text Variable saves the Clipboard to disk for later retrieval by a Query or Host Command.  If the clipboard is empty, the temporary file is deleted so Restore Clipboard knows there shouldn't be a clipboard active.

If you do not specify a slot parameter then the clipboard is saved to a slot-less file.  When it is restored the file is not deleted so you can do multiple identical clipboard restorations. *(v2.A1)*

If you do specify a slot parameter then you may save the clipboard to one of ten different clipboard slots (0-9).  When you restore a clipboard slot, the clipboard file is deleted. *(v2.A1)*

You may specify a slot number of "PUSH" to perform a stack-based push operation (eg, `$SCB(PUSH)$`). *(v2.A1)*

**Example:** `$SCB(4)$`
**Returns:** nothing

## Mouse Field Related Text Variables

These text variables return information on, or manipulate the mouse field data table. *(v2.A4)*

### $MKILL$

*Kill Mouse Fields*

**Format:** `$MKILL(x0,y0,x1,y1,inout)$`

**Syntax:**

```text
$MKILL(opt:X0, opt:Y0, opt:X1, opt:Y1, opt:INOUT)$

       opt:X0        opt:Y0
           ──            ──
           0-65535       0-65535
           ──            ──
       def=<none>    def=<none>


       opt:X1        opt:Y1         opt:INOUT
           ──            ──             ─────
           0-65535       0-65535        IN
           ──            ──             OUT
       def=<none>    def=<none>         ─────
                                    def=IN
```

If no parameters are specified then all mouse fields currently defined are deleted (just like [RIP_KILL_MOUSE_FIELDS](11-level-1-commands.md#rip_kill_mouse_fields) does).  If you specify the five parameters, you are defining a box on the screen that should have the mouse fields inside or outside of it deleted. Whether mouse fields inside the box or outside the box are destroyed depends on the INOUT parameter.  If this parameter is specified as "IN" then all mouse fields inside the box are deleted.  If the parameter is set to "OUT" then all mouse fields outside the box are deleted.  Coordinates are specified in World coordinates. *(v2.A1)*

This Active Text Variable deletes all defined Mouse Fields exactly like `RIP_KILL_MOUSE_FIELDS` does.  The benefit is when the user clicks on a Mouse Fields or Button, the Mouse Fields are removed, but the graphics remain on the screen.  The fields could be subsequently re-defined quickly and easily without having to re-transmit an identical menu over again.

**Example:** `$MKILL$` ... Kill all mouse fields defined
**Example:** `$MKILL(0,0,639,100,IN)$` ... Kill all mouse fields inside the box (0,0) to (639,100).
**Returns:** nothing

### $RMF$

*Restore Mouse Fields*

**Format:** `$RMF(source)$`

**Syntax:**

```text
$RMF(opt:SOURCE)$
         ──────
         BASE
         POP
         0-9
         ──────
     def=BASE
```

This Active Text Variable restores any Mouse Fields saved with [`$SMF$`](#smf). You may have only one set of mouse fields saved at once.  If no mouse fields were saved, or if the number of fields saved is 0, then no mouse fields are  active.

If no slot parameter is specified, then the non-slot specific mouse fields saved with a `$SMF$` command is restored (the saved mouse field file is not deleted).  If a slot parameter is specified then the mouse fields saved with `$SMF(slot)$` is restored then the disk file is deleted. *(v2.A1)*

If you specify a slot number of "POP" then you will be performing a stack-based pop operation (eg, `$RMF(POP)$`). *(v2.A1)*

If you specify "BASE" instead of a slot number then you will be restoring the mouse fields from the mouse field data backup area's base save area. *(v2.A4)*

> **NOTE:**  You may restore slot-less saved Mouse Fields more than once is you wish.  In other words, if you do a `$SMF$` command, you may execute `$RMF$` one or more times.  But if you do a `$SMF(1)$` you may only do a `$RMF(1)$` once. *(v2.A1)*

**Example:** `$RMF(4)$`
**Returns:** nothing

### $SMF$

*Save Mouse Fields*

**Format:** `$SMF(destination)$`

**Syntax:**

```text
$SMF(opt:DESTINATION)$
         ───────────
         BASE
         PUSH
         0-9
         ───────────
     def=BASE
```

This Active Text Variable saves all defined Mouse Fields and Mouse Buttons to a temporary file for later retrieval.  This is designed especially for the graphical designer who wishes to pop-up a dialog box on the screen with one or more mouse fields, and when finished, to restore the screen and original mouse fields.  This command is intended to be used with the Restore Mouse Fields text variable [`$RMF$`](#rmf).

If no slot parameter is specified then the mouse field definitions will be stored to a non-slot specific file (similar to [`$SAVE$`](#save) with no parameter).  This file can be restored multiple times.  If you specify a slot, then the fields are stored to slot-specific files. When a saved mouse field definition in a particular slot is restored, the corresponding file is automatically deleted upon restoration. *(v2.A1)*

If you specify a slot number of "PUSH" then you will be performing a stack-based push operation (eg, `$SMF(PUSH)$`). *(v2.A1)*

If you specify "BASE" instead of a slot number then you will be storing the mouse fields into the mouse field data backup area's base save area. *(v2.A4)*

**Example:** `$SMF(4)$`
**Returns:** nothing

## Screen Related Text Variables

These text variables return information on, or manipulate the screen. *(v2.A4)*

### $RESTORE$

*Restore graphics screen*

**Format:** `$RESTORE(source)$`

**Syntax:**

```text
$RESTORE(opt:SOURCE)$
             ──────
             BASE
             POP
             0-9
             ──────
         def=BASE
```

If no slot parameter is specified, then the graphical screen stored in the file RIPTERM.SAV will be restored to the screen (the file will not be deleted). *(v2.A1)*

If you specify a slot number (0-9), then the file restored to the screen will be RIPTERM0.SAV through RIPTERM9.SAV based on the slot number.  These files are deleted after restoration is complete. *(v2.A1)*

If you specify "POP" instead of a slot number then a stack-based pop operation will be performed (eg, `$RESTORE(POP)$`). *(v2.A1)*

If you specify "BASE" instead of a slot number than the screen is restored from the screen data backup area's base save area. *(v2.A4)*

Only the graphics screen is restored, not the Clipboard, Mouse Fields or Text Window settings.

When the graphics screen is restored, the Graphics Viewport settings that were in effect when the screen was saved will be restored as well. *(v1.54)*

To restore the entire context of the graphics environment [`$RESTALL$`](#restoreall).

**Example:** `$RESTORE(3)$`
**Returns:** nothing

### $RESTOREx$

*Restore graphics screen (x=0-9)*

**Format:** `$RESTORE0$` - `$RESTORE9$`

**Syntax:** `$RESTORE0$` ... `$RESTORE9$`

The RESTORE0 through RESTORE9 screen restore variations use a filename of RIPTERM0.SAV through RIPTERM9.SAV allowing you to restore from up to ten different saved files.  The disk file associated with the screen is deleted upon restoration. *(v2.A1)*

When the graphics screen is restored, the Graphics Viewport settings that were in effect when the screen was saved will be restored as well. *(v1.54)*

To restore the entire context of the graphics environment [`$RESTOREALL$`](#restoreall). *(v2.A4)*

Also saved are the graphical style slots and button style slots. *(v2.A1)*

> **NOTE:**  This method of restoring graphical screens is obsolete.  Use the [`$RESTORE$`](#restore) function (see below) with a parameter. *(v2.A1)*

**Example:** `$RESTORE3$`
**Returns:** nothing

### $SAVE$

*Save graphics screen*

**Format:** `$SAVE(destination)$`

**Syntax:**

```text
$SAVE(opt:DESTINATION)$
          ───────────
          BASE
          PUSH
          0-9
          ───────────
      def=BASE
```

If no slot parameter is specified, then this command will save the contents of the entire graphics screen to a disk file called RIPTERM.SAV.  No mouse fields, text window locations or clipboard data is saved - just the graphics screen. *(v2.A1)*

If you specify a slot parameter then that identifies a specific screen slot to overwrite (0-9).  The files that the screen is stored to are RIPTERM0.SAV through RIPTERM9.SAV allowing you up to ten separate screen slots saved simultaneously. *(v2.A1)*

If you specify "PUSH" instead of a slot number then a stack-based save operation will be performed (eg, `$SAVE(PUSH)$`). *(v2.A1)*

If you specify "BASE" instead of a slot number than you the screen will be stored in the base save area of the screen data backup area. *(v2.A4)*

In addition to the Graphical data that is currently on-screen, the current Graphical Viewport settings are saved as well so that when a restore is done, the viewport will be properly restored as well. *(v1.54)*

If you wish to save the entire state of the RIPterm system, use [`$SAVEALL$`](#saveall).

Also saved are the graphical style slots and button style slots. *(v2.A1)*

**Example:** `$SAVE(7)$`
**Returns:** nothing

### $SAVEx$

*Save graphics screen (x=0-9)*

**Format:** `$SAVE0$` - `$SAVE9$`

**Syntax:** `$SAVE0$` ... `$SAVE9$`

If you choose the SAVE0 through SAVE9 variations, the filename that is saved to files RIPTERM0.SAV through RIPTERM9.SAV, allowing you to have multiple screens saved simultaneously.

In addition to the Graphical data that is currently on-screen, the current Graphical Viewport settings are saved as well so that when a restore is done, the viewport will be properly restored as well. *(v1.54)*

If you wish to save the entire state of the RIPterm system, use [`$SAVEALL$`](#saveall).

> **NOTE:**  This method of saving graphical screens is obsolete.  Use the [`$SAVE$`](#save) function (see below) with a parameter. *(v2.A1)*

**Example:** `$SAVE7$`
**Returns:** nothing

## Data Table and Backup Area Text Variables

These text variables return information on, or manipulate the data backup system. *(v2.A4)*

### $BACKSTAT$

*Return status information on a backup area*

*Added in RIPscrip v2.A4.*

**Format:** `$BACKSTAT(type)$`

**Syntax:**

```text
$BACKSTAT(req:TYPE,  opt:MODE)$
              ────       ────
              TW         USE
              BUT        PROT
              STYLE      ────
              PORT   def=USE
              MOUSE
              PAL
              ENV
              SCREEN
```

This function returns status information on the specified data backup area.  The information returned provides a detailed breakdown of the specified data backup area.  The following data backup areas may be specified with the TYPE parameter:

| Area | Description |
|------|-------------|
| `TW` | Text window backup area |
| `BUT` | Button style backup area |
| `STYLE` | Graphics style backup area |
| `PORT` | Drawing port backup area |
| `MOUSE` | Mouse field backup area |
| `PAL` | Color palette backup area |
| `ENV` | Environment backup area |
| `SCREEN` | Screen backup area |

The MODE parameter defines what kind of backup area status information you are requesting.  If it is omitted, or specified as "USE", then the data returned is a composite string of values detailing which areas of the backup area are "in use".  The format of this string of text is:

```text
base:stack:slots:free:s0:s1:s2:s3:s4:s5:s6:s7:s8:s9
```

The "base" field is set to "0" if the base save area is not in use, or "1" if it is in use.  The "stack" field is set to the number of entries that are currently saved on the stack ("0" if its empty). The "slots" field states how many entries are saved in the data save slots.  The "free" field determines how many stack/slot areas are not in use (ie, how many more can hold data).  The final ten entries "s0" through "s9" are set to "0" or "1" indicating if that specific data save slot is currently in use or not.  If you add up the "stack" and "slots" value, you will get a total value of entries stored in the stack/slot system.  If you add this total value up with the value of "free", then you get the maximum number of entries allowed to be saved in the stack/slot system (currently this should be set not exceed 10).

If the MODE parameter is set to "PROT", then you are requesting which data save slots in the data backup area are protected or not. The format returned to the host is:

```text
s0:s1:s2:s3:s4:s5:s6:s7:s8:s9
```

The contents of the "s0" through "s9" fields will be set to "0" to indicate that the data save slot is not protected, or "1" if it is protected.

**Example:** `$BACKSTAT(TW, USE)$`
**Returns:** `0:2:3:5:0:0:0:1:0:0:1:1:0:0`

**Example:** `$BACKSTAT(TW, PROT)$`
**Returns:** `0:1:0:0:1:0:0:1:0:0`

### $COPY$

*Copy object to 1/more locations*

**Format:** `$COPY(type,source,dest1,...)$`

**Syntax:**

```text
$COPY(req:TYPE, req:SOURCE, req:DEST1, ...)$
          ────      ──────
          TW        0-35:TYPE<>MOUSE,SCREEN,PORT
          BUT       CUR:TYPE<>MOUSE,SCREEN,PORT
          STYLE     TBL
          PAL       BASE
          PORT      S0-S9
          MOUSE     POP
          ENV
          SCREEN

      req:DEST1
          ─────
          0-35:TYPE<>MOUSE,SCREEN,PORT
          CUR:TYPE<>MOUSE,SCREEN,PORT
          TBL
          BASE
          S0-S9
          PUSH
```

This complex command embodies many different "data copy" operations. The basic types of copy operations allowed with this command are: *(v2.A3)*

Type `<type>` parameter defines what type of data object is to be copied from one location to another.  The following data types can be copied with the user of this command (these are the values that the `<type>` parameter can contain: *(v2.A4)*

| Keyword | Description |
|---------|-------------|
| `STYLE` | Graphics style data |
| `BUT` | Button style data |
| `TW` | Text window data |
| `PAL` | Color palette data |
| `PORT` | Drawing port data |
| `MOUSE` | Mouse field data |
| `SCREEN` | Graphics screen data |
| `ENV` | Environment data |

```text
      Data Table
    ╔════════════╗ 3         ╔═════════════════════════════════╗
    ║            ║  ────── ║         Base Save Area          ║
    ╟────────────╢          1╚═════════════════════════════════╝
    ║            ║                                5    12
    ╟────────────╢                                 │      │
    ║            ║                                 6     │
    ╟────────────╢           ╔═══╤═══╤═══╤═══╤═══╤═══╗    │
    ║   14      ║ 4         ║   │   │   │   │ 7 │   ║    │
    ╟──── │ ─────╢  ────── ║   │   │ ──────── │   ║    │
    ║     │      ║          2║   │   │   │   │   │   ║    │
    ╟──── │ ─────╢           ╚═══╧═══╧═══╧═══╧═══╧═══╝    │
    ║     │      ║            Data Save Slots    13      │
    ╟────────────╢                                 │      │
    ║            ║                                 11    10
    ╟────────────╢ 9         ╔═════════════════════════════════╗
    ║            ║  ────── ║         Data Save Stack         ║
    ╟────────────╢          8╚═════════════════════════════════╝
    ║            ║
    ╚════════════╝
```

**Copying entire Data Tables around (inter-table copying)**

1. Copy the actual Data Table into the data backup area's Base Data Save Area. *(v2.A3)*

   `Copy table -> Base area`

2. Copy the actual Data Table into a specific backup area's Data Save Slot (via a slot index number from 0-9). *(v2.A3)*

   `Copy table -> Save slot`

3. Copy the Base Data Save Area to the actual Data Table *(v2.A3)*

   `Copy Base area -> Table`

4. Copy a Data Save Slot (specified with an index number) directly into the actual Data Table. *(v2.A3)*

   `Copy Save slot -> Table`

5. Copy a Data Save Slot (specified with an index number) into the Base Data Save Area. *(v2.A3)*

   `Copy Save slot -> Base area`

6. Copy the Base Data Save Area into a specific Data Save Slot (specified by a slot index number from 0-9). *(v2.A3)*

   `Copy Base area -> Save slot`

7. Copy one Data Save Slot to another Data Save Slot (both slots are specified with slot index numbers). *(v2.A3)*

   `Copy Save slot -> Save slot`

8. Push an actual Data Table onto the Data Save Slot "stack" via the stack pointer. *(v2.A3)*

   `Copy data table -> Stack (push)`

9. Pop an actual Data Table off of the Data Save Slot "stack" via the stack pointer. *(v2.A3)*

   `Copy Stack (pop) -> table`

10. Push the Base Save Area onto the "stack" via the stack pointer. *(v2.A4)*

    `Copy Base area -> Stack (push)`

11. Push a Data Save Slot onto the "stack" via the stack pointer *(v2.A4)*

    `Copy Save slot -> Stack (push)`

12. Pop an actual Data Table off of the "stack" and place it into the Base Save Area. *(v2.A4)*

    `Copy Stack (pop) -> Base area`

13. Pop an actual Data Table off of the "stack" and place it into a specific Save slot. *(v2.A4)*

    `Copy Stack (pop) -> Save slot`

**Copying Data Table Entries around (intra-table copying)**

14. Copy one entry in an actual Data to one or more other entries inside the same Data Table *(v2.A3)*

    `Copy Table[index1] -> Table[index2]`

When you are referring to a copy operation that involves the entire data table, you use the keyword "TBL".  When referring to the Base Data Save Area, the keyword "BASE" is used.  A Data Save Slot number is specified with the letter "S" followed by the slot index number (eg, S0, S3, S5, etc).  An actual data table entry number is specified simply as an actual number from 0 to the total number of entries in the data table minus one. *(v2.A3)*

Destinations for entire data tables can be the BASE save area, a Data Save Slot (S0-S9) or you can PUSH it onto the stack. *(v2.A4)*

You can also specify the source as POP to indicate that you are copying (popping) data from the stack to some other destination (TBL, BASE or S0-S9).  If you specify only one destination location, then the contents of the stack are copied to that location, then the stack is "popped", which basically deletes the top item on the stack (the one that was just copied).  If you have multiple destinations, then the stack item is copied to each of those locations, then it is finally deleted from the stack after all of the copy operations are complete. *(v2.A4)*

If the source refers to an entry in a data table (not the entire data table), then the destination(s) must also be individual data table entry numbers. *(v2.A4)*

Some data tables allow you to directly access individual data entries inside the data table (ie, text window tables, graphical style tables, color palette tables, button style tables and drawing port tables).  You cannot directly access data table entries for the Mouse Field data table, and you cannot directly access the data table entries for the screen data table.  In these two cases, copying one data table entry over another entry in the same data table entry is not allowed. *(v2.A3)*

Some data tables allow you to have a specific entry selected as the current table entry.  For example, the text window data table has one data table entry selected as the current text window at any one time.  When a data table allows an entry to be the current entry, then the keyword "CUR" may be used to specify the current table entry when performing copy operations from one data table entry to another entry.  The following list describes which data tables have a current entry allowed: *(v2.A3)*

| Table Name | Allows current entry? | Total Entries | Entry Access |
|------------|-----------------------|---------------|--------------|
| Text window table | Yes | 36 | Yes |
| Drawing port table | No | 36 | No |
| Graphical style table | Yes | 36 | Yes |
| Button style table | Yes | 36 | Yes |
| Color palette table | Yes | 36 | Yes |
| Environment | Yes | 36 | Yes |
| Mouse field table | No | 128 | No |
| Graphics screen table | No | 1 | No |

You cannot copy a data table over itself, and you cannot copy a data table entry on top of itself.  All destination parameters are checked for validity before any actual copy operations are performed.  If any of them fail a syntax check then the entire `$COPY$` expression is rejected as a syntax error. *(v2.A4)*

Our of all six data tables that have 36 separate entries, one of them doesn't permit direct copying from one entry to another within the same data table.  That table is the drawing port table.  The reason for this is because of how complicated the subject of performing port copying from one entry to another.  Do you copy over just the graphical data, and if so, do you stretch it to fit in the destination port if the dimensions of the ports don't match?  On the other hand do you duplicate the port definition entirely, potentially copying over the bitmap data?  If so, how do you handle copying to port #0 which cannot be deleted or re-defined?  With all of these complicated issues involved, it was decided not to allow for ports to be copied from one entry to another.  Perhaps in the future we will allow for this kind of operation when more research on the issues can be performed. *(v2.A4)*

Note that copying individual text window table entries, or individual drawing port entries around also copies around the resident query associated with that particular text window or port! *(v2.A4)*

The following sections describe the various combinations of copy parameters: *(v2.A3)*

**Copy actual Data Table into the data backup area's Base Save Area**

`$COPY(TW, TBL, BASE)$`

**Copy actual Data Table into a specific backup area's Data Save Slot**

`$COPY(TW, TBL, S3)$`

**Copy the Base Data Save Area to the actual Data Table**

`$COPY(TW, BASE, TBL)$`

**Copy a Data Save Slot directly into the actual Data Table**

`$COPY(TW, S3, TBL)$`

**Copy a Data Save Slot into the Base Data Save Area**

`$COPY(TW, S3, BASE)$`

**Copy the Base Data Save Area into a specific Data Save Slot**

`$COPY(TW, BASE, S3)$`

**Copy one Data Save Slot to another Data Save Slot**

`$COPY(TW, S3, S5)$`

**Push an actual Data Table onto the Data Save Slot "stack"**

`$COPY(TW, TBL, PUSH)$`

**Pop an actual Data Table off of the Data Save Slot "stack"**

`$COPY(TW, POP, TBL)$`

**Copy one entry in an actual Data Table to one or more other entries**

`$COPY(TW, 3, 5)$`

You can also combine various copy operations into one copy command like this: *(v2.A3)*

`$COPY(TW, TBL, S3, BASE, S5)$`

Copies the entire data table into data save slot #3, #5 and into the base data save area.

`$COPY(TW, CUR, 5, 7, 9)$`

Copies the current data table entry into data table entry numbers 5, 7 and 9 (overwriting those entries with the data inside the current data table entry).

### $CUR$

*Select/query current data table entry*

*Added in RIPscrip v2.A4.*

**Format:** `$CUR(type,which)$`

**Syntax:**

```text
$CUR(req:TYPE,  opt:WHICH)$
         ────       ─────
         TW         0-35
         PORT       ─────
         STYLE  def=<none>
         PAL
         BUT
         ENV
```

This variable sets or inquires about the current data table entry number associated with a specific data table indicating by the TYPE parameter (which must be specified).  The possible TYPE parameters are as follows:

| Name | Description |
|------|-------------|
| `TW` | Text window data table |
| `PORT` | Drawing port data table |
| `STYLE` | Graphics style data table |
| `PAL` | Color palette data table |
| `BUT` | Button style data table |
| `ENV` | Environment data table |

The WHICH parameter is only used when setting the current entry for the given data table.  If you omit the WHICH parameter, then you are asking the terminal "which entry in the specified data table is the current one?".  In situations like this, this text variable will return a value from "0" to "35".

If the WHICH parameter is specified, then you are not inquiring about the current entry in that data table, you are setting it.  Possible values for WHICH are numbers from 0-35 to indicate which data table entry you wish to make the current one.  If the entry you specify isn't defined, then it is set to some suitable defaults based on the following table:

| Data Table | Description of Defaults |
|------------|-------------------------|
| `TW` | The text window is made full screen in the user's default font. |
| `PORT` | The port is defined as a screen port occupying the entire screen.  The viewport is made the full size of the port. |
| `STYLE` | The basic graphics style used upon a `$RESET$` operation is activated. |
| `PAL` | The standard 256 color lookup table is established and activated. |
| `BUT` | The basic button style used upon a `$RESET$` operation is activated. |
| `ENV` | A basic environment is established.  640x350 world coordinates, 2-byte coordinates, color palette mapping mode, mouse cursor 0, etc. |

**Example:** `$CUR(TW, 5)$`
**Returns:** nothing

**Example:** `$CUR(TW)$`
**Returns:** `5`

### $INUSE$

*Is a data object in use?*

*Added in RIPscrip v2.A4.*

**Format:** `$INUSE(data_object,element)$`

**Syntax:**

```text
$INUSE(req:DATA_OBJECT, req:ELEMENT)$
           ───────────      ───────
           PORT             1-35:DATA_OBJECT<>SCREEN,MOUSE
           TW               ALL:DATA_OBJECT<>SCREEN
           STYLE            ANY:DATA_OBJECT<>SCREEN
           BUT              S0-S9
           PAL              BASE
           MOUSE            ALLSLOTS
           ENV              ANYSLOTS
           SCREEN           STACK
                            BACKUP
```

This command determines if a specific data object is currently in use or not (ie, defined).  The type of data table that the DATA_OBJECT parameter refers to may be set to the following:

| Data Object | Description |
|-------------|-------------|
| `TW` | Text window data object |
| `PORT` | Drawing port data object |
| `STYLE` | Graphics style data object |
| `BUT` | Button style data object |
| `PAL` | Color palette data object |
| `ENV` | Environment data object |
| `MOUSE` | Mouse field data object |
| `SCREEN` | Graphics screen data object |

The exact aspect of the data object in question which is being inquired about is defined by the ELEMENT parameter, which must also be specified.  The possible values for ELEMENT and their meanings are as follows:

| Elements | Description |
|----------|-------------|
| `1-35` | A data table entry number from 1-35 (this is not valid for MOUSE or SCREEN data objects which do not directly addressable data table entries). |
| `ALL` | Are all data table entries from 1-35 in use?  Entry 0 is always in use so it is not included in this element.  SCREEN data objects do not have a data table associated with them so you cannot specify ALL for them. |
| `ANY` | Are any data table entries from 1-35 in use?  Entry 0 is always in use so it is not included in this element otherwise it would always be in use. |
| `S0-S9` | This specifies a data save slot number from 0-9 in the specified data object's data backup area. |
| `BASE` | Is the base save area of the data object's backup data area in use? |
| `ALLSLOTS` | Are all data save slots in the data object's backup data area in use? |
| `ANYSLOTS` | Are any data save slots in the data object's backup data area in use? |
| `STACK` | Is the data save stack in the data object's backup data area in use? |

If the specified data ELEMENT in the DATA_OBJECT is in use, then this command returns "1".  It returns "0" if the specified condition is not met.

**Example:** `$INUSE(TW, BASE)$`
**Returns:** `FALSE`

### $ISPROT$

*Is a data object protected?*

*Added in RIPscrip v2.A4.*

**Format:** `$ISPROT(data_object,element)$`

**Syntax:**

```text
$ISPROT(req:DATA_OBJECT, req:ELEMENT)$
            ───────────      ────────
            TW               CUR:DATA_OBJECT<>SCREEN,MOUSE
            PORT             1-35:DATA_OBJECT<>SCREEN,MOUSE
            BUT              ALL:DATA_OBJECT<>SCREEN,MOUSE
            STYLE            ANY:DATA_OBJECT<>SCREEN,MOUSE
            PAL              S0-S9
            MOUSE            ALLSLOTS
            ENV              ANYSLOTS
            SCREEN
```

This command returns a value indicating if the specified element in the desired data object is protected or not.  If it is protected, then it returns a "1".  "0" is returned if it is not protected.  If the specified data object element is not in use (ie, not defined), then this command returns "-1".  The type of data object that can be specified in the DATA_OBJECT parameter may be set to the following:

| Data Object | Description |
|-------------|-------------|
| `TW` | Text window data object |
| `PORT` | Drawing port data object |
| `STYLE` | Graphics style data object |
| `BUT` | Button style data object |
| `PAL` | Color palette data object |
| `ENV` | Environment data object |
| `MOUSE` | Mouse field data object |
| `SCREEN` | Graphics screen data object |

What is element of the data object that is to inquired about is defined by the ELEMENT parameter.  This parameter must be specified and may be set to the following values:

| Elements | Description |
|----------|-------------|
| `CUR` | Is the current data table entry in the specified data object data table protected?  MOUSE and SCREEN's cannot have their entries (if any) protected, so this not permitted for the CUR element (it would always return "0").  If the current data table entry is a value of 0, then this element will return the value "0" because this entry in a data table can never be protected. |
| `1-35` | A data table entry number from 1-35 (this is not valid for MOUSE or SCREEN data objects which do not directly protectable data table entries). |
| `ALL` | Are all data table entries from 1-35 protected?  All of these entries would also have to be "in use" for this to be true.  Entry 0 cannot be protected so it is not included in this element.  SCREEN and MOUSE data objects cannot have their table entries (if any) protected so these data objects are not permitted with the ALL element. |
| `ANY` | Are any defined data table entries in the data object's data table protected? |
| `S0-S9` | Is the specified data save slot (0-9) for the data object protected? |
| `ALLSLOTS` | Are all data save slots protected? |
| `ANYSLOTS` | Are any data save slots protected? |

**Example:** `$UNPROT(TW, S7)$`
**Returns:** nothing

### $PROT$

*Protect data from deletion*

*Added in RIPscrip v2.A4.*

**Format:** `$PROT(data_object,element1,...)$`

**Syntax:**

```text
$PROT(req:DATA_OBJECT, req:ELEMENT1, ...)$
          ───────────      ────────
          TW               CUR:DATA_OBJECT<>SCREEN,MOUSE
          PORT             1-35:DATA_OBJECT<>SCREEN,MOUSE
          BUT              ALL:DATA_OBJECT<>SCREEN,MOUSE
          STYLE            S0-S9
          PAL              ALLSLOTS
          ENV
          MOUSE
          SCREEN
```

This command protects a specific element of a given data object. The type of data object that can be specified in the DATA_OBJECT parameter may be set to the following:

| Data Object | Description |
|-------------|-------------|
| `TW` | Text window data object |
| `PORT` | Drawing port data object |
| `STYLE` | Graphics style data object |
| `BUT` | Button style data object |
| `PAL` | Color palette data object |
| `ENV` | Environment data object |
| `MOUSE` | Mouse field data object |
| `SCREEN` | Graphics screen data object |

What is element of the data object that is to be protected is defined by the ELEMENT parameter.  This parameter must be specified (you may specify more than one to protect multiple elements in one command). ELEMENT may be set to the following values:

| Elements | Description |
|----------|-------------|
| `CUR` | Protect the current data table entry in the specified data object data table.  MOUSE and SCREEN's cannot have their entries (if any) protected, so this is not permitted for the CUR element.  If the current data table entry is a value of 0, then this element is ignored because entry 0 cannot be protected in any data table. |
| `1-35` | A data table entry number from 1-35 (this is not valid for MOUSE or SCREEN data objects which do not directly protectable data table entries). |
| `ALL` | Protect all data table entries from 1-35 that are currently in use.  Entry 0 cannot be protected so it is not included in this element.  SCREEN and MOUSE data objects cannot have their table entries (if any) protected so these data objects are not permitted with the ALL element. |
| `S0-S9` | This protects a data save slot number from 0-9 in the specified data object's data backup area. |
| `ALLSLOTS` | Protect all data save slots in the data object's data backup area that are currently in use. |

If a data object element is attempted to be protected, but it is not in use then this command does nothing for that parameter.

**Example:** `$PROT(TW, S7)$`
**Returns:** nothing

### $UNPROT$

*Unprotects object*

*Added in RIPscrip v2.A4.*

**Format:** `$UNPROT(data_object,element1,...)$`

**Syntax:**

```text
$UNPROT(req:DATA_OBJECT, req:ELEMENT1, ...)$
            ───────────      ────────
            TW               CUR:DATA_OBJECT<>SCREEN,MOUSE
            PORT             1-35:DATA_OBJECT<>SCREEN,MOUSE
            BUT              ALL:DATA_OBJECT<>SCREEN,MOUSE
            STYLE            S0-S9
            PAL              ALLSLOTS
            MOUSE
            ENV
            SCREEN
```

This command unprotects a specific element of a given data object. The type of data object that can be specified in the DATA_OBJECT parameter may be set to the following:

| Data Object | Description |
|-------------|-------------|
| `TW` | Text window data object |
| `PORT` | Drawing port data object |
| `STYLE` | Graphics style data object |
| `BUT` | Button style data object |
| `PAL` | Color palette data object |
| `ENV` | Environment data object |
| `MOUSE` | Mouse field data object |
| `SCREEN` | Graphics screen data object |

What is element of the data object that is to be unprotected is defined by the ELEMENT parameter.  This parameter must be specified (you may specify more than one to unprotect multiple elements in one command).  ELEMENT may be set to the following values:

| Elements | Description |
|----------|-------------|
| `CUR` | Unprotect the current data table entry in the specified data object data table.  MOUSE and SCREEN's cannot have their entries (if any) unprotected, so this is not permitted for the CUR element.  If the current data table entry is a value of 0, then this element is ignored because entry 0 cannot be unprotected in any data table. |
| `1-35` | A data table entry number from 1-35 (this is not valid for MOUSE or SCREEN data objects which do not directly protectable data table entries). |
| `ALL` | Unprotect all data table entries from 1-35 that are currently in use.  Entry 0 cannot be protected so it is not included in this element.  SCREEN and MOUSE data objects cannot have their table entries (if any) protected so these data objects are not permitted with the ALL element. |
| `S0-S9` | This unprotects a data save slot number from 0-9 in the specified data object's data backup area. |
| `ALLSLOTS` | Unprotect all data save slots in the data object's data backup area that are currently in use. |

If a data object element is attempted to be unprotected, but it is not in use then this command does nothing for that parameter.

**Example:** `$UNPROT(TW, S7)$`
**Returns:** nothing

---

[◀ Prev: Text Variables: Terminal & Reset](19-text-variables-terminal.md) • [Contents](README.md) • [Next: Icon/DIB File Format ▶](21-icon-file-format.md)
