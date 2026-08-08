# Text Variables: Terminal & Reset

[◀ Prev: Text Variables: Mouse, Text Window & Ports](18-text-variables-mouse-window.md) · [Contents](README.md) · [Next: Text Variables: Environment, Clipboard, Screen & Tables ▶](20-text-variables-environment.md)

## Terminal Operation Text Variables

These text variables return information on, or manipulate a mode of operation of the RIPscrip terminal. _(v2.A4)_

### $APP$

_External Application Call_

_Added in RIPscrip v2.A1._

**Format:** `$APP(appno,argument)$`

**Syntax:**

```text
$APP(opt:APPNO, opt:ARGUMENT)$
         ─────      ────────
         0-9        A word
         ─────      ────────
     def=0      def=<none>
```

This text variable instructs the terminal to execute an external application. If you do not specify any parameters, then application number zero will be executed. If you do specify any parameters, you may specify one or both of them. If you supply the ARGUMENT parameter, you must specify the APPNO option. APPNO is a number from 0-9 indicating which application should be executed. If used, the ARGUMENT parameter is an arbitrary command line argument that will be added to the end of the application's command line string. This gives the ability to allow the host to control portions of the external application's operation.

**Example:** `$APP(0,FILENAME.TXT)`  
**Returns:** nothing

### $APPx$

_External Application Call (x=0-9)_

**Format:** `$APP0$` - `$APP9$`

**Syntax:** `$APP0$` ... `$APP9$`

This Active Text Variable instructs the terminal to execute an external application. By recommendation, `$APP0$` is the user's text editor. There are ten external application slots available, numbered 0 - 9. These are defined in the External menu in RIPterm.

This command is obsolete. You should be using the [`$APP(appno)$`](#app) command as it is more general in nature. Other than that, the new APP command performs the exact same operation. If you need to pass custom arguments to the external application, then use the more generic `$APP(appno)$` command instead. _(v2.A1)_

**Example:** `$APP1$`  
**Returns:** nothing

### $BAUDEMUL$

_Set/return baud rate emulation_

_Added in RIPscrip v2.A4._

**Format:** `$BAUDEMUL(env_no, baud_rate)$`

**Syntax:**

```text
$BAUDEMUL(opt:ENV_NO, opt:BAUD_RATE)$
              ──────      ─────────
              CUR         Number
              0-35        ─────────
              ──────  def=0
          def=CUR
```

This variable allows the host to determine the current baud rate emulation setting for use with local RIP file playback.

Without any parameters, this variable inquires about the baud rate setting for the current environment. If only one parameter is supplied, then it is inquiring about a specific environment number and that number can be from 0-35 or the value "CUR" for the current environment. If the environment being inquired about isn't in use, then this command returnes a value of "NONE".

A return value of "0" means that playback should occur at full speed. Typical return values can be 0, 300, 1200, 2400, 4800, 9600, 14400, 16800, 19200, 28800, 38400, 57600 and 115200. These are not the only values that can be returned. If you want to set a baud rate emulation value of 12345, baud you can.

If you specify two parameters, then the first one must be the environment to be modified (0-35 or CUR - see above). The second parameter can be a number from 0 on up, which specifies the baud rate which you wish to set to that environment. If the destination environment isn't in use, then a syntax erorr is generated.

**Example:** `$BAUDEMUL$`  
**Returns:** `9600`

**Example:** `$BAUDEMUL(CUR)$`  
**Returns:** `9600`

**Example:** `$BAUDEMUL(35)$`  
**Returns:** `NONE`

**Example:** `$BAUDEMUL(CUR, 2400)$`  
**Returns:** nothing - sets current environment to 2400 baud

### $D$

_Delay for a number of milliseconds_

_Added in RIPscrip v2.A1; revised in v2.A2._

**Format:** `$D(duration)$`

**Syntax:**

```text
$D(req:DURATION)$
       ────────
       1-65535
```

This command causes a delay to occur. During this time, the terminal program "stops" everything except for any sound related activity. The TIME parameter must be specified. Its value is specified in 60ths of a second. This command is useful to pause for a short time for things like prompts to go by or whatever.

**Example:** `$D(60)$`  
**Returns:** nothing

### $DWAYOFF$

_Turn Doorway Mode OFF_

**Format:** `$DWAYOFF$`

**Syntax:** `$DWAYOFF$`

This Active Text Variable disables the Doorway keyboard mode. This will return the keyboard to normal operation.

**Example:** `$DWAYOFF$`  
**Returns:** nothing

### $DWAYON$

_Turn Doorway Mode ON_

**Format:** `$DWAYON$`

**Syntax:** `$DWAYON$`

This Active Text Variable enables Doorway Mode. This is intended to be used by a Host system that wishes to take advantage of the Doorway mode available in Marshall Dudley's Doorway (tm) software package.

**Example:** `$DWAYON$`  
**Returns:** nothing

### $FILEDEL$

_Delete one or more host files_

_Added in RIPscrip v2.A4._

**Format:** `$FILEDEL(filename,file2`

**Syntax:**

```text
$FILEDEL(req:FILENAME, ...)$
             ────────
             A filename
```

This text variable exists so that a host system can clean up after itself, with the ability to delete files that it created, but no longer needs. At least one parameter must be specified, and it must be a filename that the host created, or downloaded to the terminal via `RIP_ENTER_BLOCK_MODE` or by some other means. No form of wildcard information is processed, nor is path information. If present, it will be ignored to the best of the terminal's ability. You may specify more than one filename parameter with this command to delete multiple files with one "shorter" command.

### $HKEYOFF$

_Disable Button Hotkeys_

**Format:** `$HKEYOFF$`

**Syntax:** `$HKEYOFF$`

This Active Text Variable turns off Button Hotkeys. This should be done when entering a full-screen editor, or any part of the system where the user is entering a string of text. This is to prevent the user from accidentally selecting a button when typing in text.

**Example:** `$HKEYOFF$`  
**Returns:** nothing

### $HKEYON$

_Enable Button Hotkeys_

**Format:** `$HKEYON$`

**Syntax:** `$HKEYON$`

This Active Text Variable turns on use of Button Hotkeys. When enabled, if the user presses a key associated with a button, it is selected just as if it were clicked. The Scroll Lock light on the keyboard is turned on.

**Example:** `$HKEYON$`  
**Returns:** nothing

### $NOREFRESH$

_Disables screen refresh expression_

_Added in RIPscrip v2.A1; revised in v2.A4._

**Format:** `$NOREFRESH$`

**Syntax:** `$NOREFRESH$`

This command disables a host defined refresh expression. When you do this, the refresh option for the terminal is disabled and cannot be selected (or if it can be selected, does nothing). This is equivalent to issuing a [RIP_SET_REFRESH](12-level-2-commands.md#rip_set_refresh) with a `$OFF$` parameter to disable refreshing.

**Example:** `$NOREFRESH$`  
**Returns:** none

### $OPTION$

_Enable/disable a software option_

_Added in RIPscrip v2.A3; revised in v2.A4._

**Format:** `$OPTION(option_name,mode)$`

**Syntax:**

```text
$OPTION(req:OPTION_NAME, opt:MODE)$
            ───────────      ────
            LIST             ON:OPTION_NAME<>LIST
            DOORWAY          OFF:OPTION_NAME<>LIST
            HOTKEY           QUERY:OPTION_NAME<>LIST
            STATBAR          ────
            TAB          def=<none>:OPTION_NAME=LIST
                             QUERY:OPTION_NAME<>LIST
```

This command allows you to turn on or turn off a specific system option in the RIPscrip application. The possible options that can be enabled or disabled are specified with the \<name\> parameter as follows: _(v2.A4)_

| Name    | Description                                                   |
| ------- | ------------------------------------------------------------- |
| HOTKEY  | Alter the status of Hotkey processing                         |
| TAB     | Alter the status of Tab mouse-field navigation                |
| DOORWAY | Alter the status of Doorway (tm) mode                         |
| STATBAR | Alter the status of the terminal program's status bar         |
| LIST    | Display a list of all options that can be toggled (see below) |

After the \<name\> parameter comes the \<mode\> parameter which specifies whether to turn the option on or off. The possible values for the \<mode\> parameter are "ON" and "OFF". _(v2.A4)_

The LIST keyword is designed to list out all options in the software that can be enabled or disabled. This allows for extendibility for custom RIPscrip packages by other vendors - some vendors may have certain features, and others may not. For example, if a specific terminal program doesn't support Doorway Mode, then the DOORWAY option wouldn't be available (and wouldn't show up if you did a `$OPTION(LIST)$` directive). The list is not carriage return delimited. The text returned to the host is not terminated with any carriage returns or anything like that. It's up to you to provide that kind of information in a button's host string or in a query string. The LIST directive though returns a list of all recognized keywords for that terminal. For example, RIPterm Pro returns the following for the `$OPTION(LIST)$` expression: _(v2.A4)_

```text
DOORWAY,HOTKEY,LIST,STATBAR,TAB
```

Note, the list is comma (,) delimited between keywords, but not after the last keyword. In addition, the keywords are returned in alphabetical order, converted to all capitals. _(v2.A4)_

You may ask the terminal to report the status of a particular option with this command as well. To do so, specify a \<mode\> of "QUERY" or omit it altogether. If the option is enabled, a "1" is returned to the host. If it is disabled, then "0" is returned. Again, no carriage returns or other delimiters are returned to the host.

**Example:** `$OPTION(DOORWAY, ON)$`  
**Returns:** nothing - enabled doorway mode

**Example:** `$OPTION(LIST)$`  
**Returns:** `DOORWAY,HOTKEY,LIST,STATBAR,TAB`

**Example:** `$OPTION(DOORWAY, QUERY)$`  
**Returns:** `0`

**Example:** `$OPTION(DOORWAY)$`  
**Returns:** `0`

**Example:** `$OPTION(LIST, QUERY)$`  
**Returns:** `$OPTION(LIST, QUERY)$` ... Syntax error

### $REFRESH$

_Forces terminal to Transmit Refresh_

_Added in RIPscrip v2.A1._

**Format:** `$REFRESH$`

**Syntax:** `$REFRESH$`

This text variable instructs the terminal to transmit the refresh host command to the remote host system if the refresh expression is non-NULL. The host has the ability to set a refresh expression that will, when sent, redisplay the current screen.

**Example:** `$REFRESH$`  
**Returns:** `^m`

### $SBAROFF$

_Turn OFF the Status Bar_

**Format:** `$SBAROFF$`

**Syntax:** `$SBAROFF$`

This Active Text Variable turns OFF the Status Bar in the terminal.

**Example:** `$SBAROFF$`  
**Returns:** nothing

### $SBARON$

_Turn ON the Status Bar_

**Format:** `$SBARON$`

**Syntax:** `$SBARON$`

This Active Text Variable turns ON the Status Bar in the terminal.

**Example:** `$SBARON$`  
**Returns:** nothing

### $STATBAR$

_Status Bar Status_

**Format:** `$STATBAR$`

**Syntax:** `$STATBAR$`

This Text Variable returns YES if the Status Bar is visible in the terminal. If the Status Bar is not visible, then NO is returned.

**Example:** `$STATBAR$`  
**Returns:** `YES`

### $TABOFF$

_Disable TAB key Mouse Field select_

**Format:** `$TABOFF$`

**Syntax:** `$TABOFF$`

This Active Text Variable turns off the use of the TAB key to jump from one defined Mouse or Button Field to another. If this command is received when a field is highlighted, it is deselected. This should be done when entering a full-screen editor so that the user can use the TAB key as a TAB, not a Mouse Field selector.

**Example:** `$TABOFF$`  
**Returns:** nothing

### $TABON$

_Enable TAB key Mouse Field select_

**Format:** `$TABON$`

**Syntax:** `$TABON$`

This Active Text Variable turns on the use of the TAB key to jump from one defined Mouse or Button Field to another.

**Example:** `$TABON$`  
**Returns:** nothing

### $VT102OFF$

_Turn VT-102 keyboard mode OFF_

_Added in RIPscrip v2.A4._

**Format:** `$VT102OFF$`

**Syntax:** `$VT102OFF$`

This Active Text Variable disables the VT-102 terminal emulation mode, returning your text windows to standard ANSI operation.

**Example:** `$VT102OFF$`  
**Returns:** nothing

### $VT102ON$

_Turn VT-102 keyboard mode ON_

_Added in RIPscrip v2.A4._

**Format:** `$VT102ON$`

**Syntax:** `$VT102ON$`

This Active Text Variable enables the VT-102 terminal emulation option of the RIPscrip software. This affects character placement and formatting of text in a text window, and also the way that the keyboard operates.

**Example:** `$VT102ON$`  
**Returns:** nothing

## RIPscrip Reset Related Text Variables

These text variables reset particular aspects of the RIPscrip environment. _(v2.A4)_

### $RESET$

_Perform a reset operation_

_Revised in RIPscrip v2.A4._

**Format:** `$RESET(option,element,sub_element)$`

**Syntax:**

```text
$RESET(opt:OPTION, opt:ELEMENT, opt:SUB_ELEMENT)$
           ──────      ───────
           SOFT        CUR:OPTION=TW,STYLE,BUT,PAL,PORT,ENV,
                                  VIEW
           HARD        0-35:OPTION=TW,STYLE,BUT,PAL,PORT,
                                   ENV,VIEW
           MCURSOR     TBL:OPTION=TW,STYLE,BUT,PAL,PORT,ENV,
                                  MOUSE,VIEW
           KEYBOARD    BASE:OPTION=TW,STYLE,BUT,PAL,PORT,
                                   ENV,MOUSE,SCREEN
           SOUND       S0-S9:OPTION=TW,STYLE,BUT,PAL,PORT,
                                    ENV,MOUSE,SCREEN
           TW          SLOTS:OPTION=TW,STYLE,BUT,PAL,PORT,
                                    ENV,MOUSE,SCREEN
           STYLE       STACK:OPTION=TW,STYLE,BUT,PAL,PORT,
                                    ENV,MOUSE,SCREEN
           BUT         BACKUP:OPTION=TW,STYLE,BUT,PAL,PORT,
                                     ENV,MOUSE,SCREEN
           PAL         ALL:OPTION=TW,STYLE,BUT,PAL,PORT,
                                  ENV,MOUSE,SCREEN,QUERY
           PORT        TW:OPTION=QUERY
           ENV                   VIEW:OPTION=QUERY
           MOUSE       ───────
           SCREEN  def=ALL:OPTION=QUERY
           VIEW        <none>:OPTION=HARD,MCURSOR,KEYBOARD,
           QUERY                     SOUND,SCREEN
           ──────      CUR:OPTION=TW,STYLE,BUT,PAL,PORT,ENV,
       def=SOFT                   VIEW
                       TBL:OPTION=MOUSE

       opt:SUB_ELEMENT
           ───────────
           ALL:ELEMENT=TW,VIEW
           CUR:ELEMENT=TW,VIEW
           TBL:ELEMENT=TW,TBL
           ───────────
       def=ALL:OPTION=QUERY
           <none>:OPTION<>QUERY
```

The reset text variable is a general purpose reset command. Without any parameters, the text variable adheres to the original 1.54 RIPscrip command which performs the exact same operations as a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) command. Some parameters used with the reset command require additional parameters to further specify what section is to be reset (eg, if you specify "TW" to reset a text window, you need to specify what text window [data table](03-data-tables.md) entry is to be reset). The possible reset "types" are as follows:

| Reset type | Uses Parms | Description                                    |
| ---------- | ---------- | ---------------------------------------------- |
| SOFT       | No         | Performs a soft reset (like RIP_RESET_WINDOWS) |
| HARD       | No         | Performs a hard reset (like RIP_HEADER)        |
| MCURSOR    | No         | Reset mouse cursor to standard cursor shape    |
| KEYBOARD   | No         | Reset keyboard state back to input status      |
| SOUND      | No         | Stops any currently playing digitized sound    |
| TW         | Yes        | Reset aspects of the text window system        |
| STYLE      | Yes        | Reset aspects of the graphical style system    |
| BUT        | Yes        | Reset aspects of the button style system       |
| PAL        | Yes        | Reset aspects of the color palette system      |
| PORT       | Yes        | Reset aspects of the drawing port system       |
| ENV        | Yes        | Reset aspects of the environment system        |
| MOUSE      | Yes        | Resets the mouse table (clears all entries)    |
| SCREEN     | Yes        | Resets aspects of the screen system            |
| VIEW       | Yes        | Resets aspects drawing port viewports          |
| QUERY      | Yes        | Resets resident queries (clears them)          |

The following paragraphs describe the purpose of each parameter and how each parameter operations:

**SOFT** - This makes the reset command perform a soft reset. This is identical in nature to the `RIP_RESET_WINDOWS` command. See that RIPscrip command for a complete, detailed description of what a soft reset does.

**HARD** - This performs a hard reset command. This is as if you issued the `RIP_HEADER` command with the "hard reset" flag enabled. See the `RIP_HEADER` command for more details about a hard reset.

**MCURSOR** - When this parameter is specified then the mouse cursor on the terminal is switched to its default, arrow shape. If the mouse input is currently disabled (via a `RIP_HEADER` command), then it is once again re-enabled as if a `RIP_NO_MORE` command were received.

**KEYBOARD** - When this parameter is specified, then keyboard input is once again enabled. Keyboard input can only be disabled via the `RIP_HEADER` command, and typically is re-enabled by a `RIP_NO_MORE` sequence. This allows you to explicitly re-enable the keyboard without a `RIP_NO_MORE` command being specified.

**SOUND** - This reset command stops any playing digitized sound (if any). If the sound is currently in the process of playing, it is aborted immediately to where there's no sound playing anymore.

**TW** - This parameter allows you to reset certain aspects of the text the text window system. The exact operation depends on what text window is being acted upon. If text window #0 is being reset, then it is set to full screen (no window clearing is performed). If it refers to a text window other than text window #0, then that window is formally deleted. If the current text window is a window other than window #0 and it is reset, then the text window is automatically switched to text window #0. If the reset affects a [data backup area](04-data-backup-areas.md), then the data backup area in question is deleted entirely. If you specify no parameters, then the current text window is reset. You may specify one or more parameters with this command to specifically alter particular aspects of the text window system. You may specify the following parameters:

| Parameter | Description                                                     |
| --------- | --------------------------------------------------------------- |
| CUR       | Reset the current text window.                                  |
| 0-35      | Reset a specific text window data table entry.                  |
| TBL       | Reset all text window data table entries.                       |
| BASE      | Reset (clear) the text window base save area                    |
| S0-S9     | Reset (clear) a specific text window data save slot             |
| SLOTS     | Reset (clears) all text window data save slots                  |
| STACK     | Reset (clears) the text window data save stack.                 |
| BACKUP    | Reset all text window backup areas (base area, slots and stack. |
| ALL       | Reset all text window data tables and backup areas              |

**Example:** `$RESET(TW, CUR, 30, BASE, S5)$`

**STYLE** - This parameter resets a graphical style back to its default settings. If you specify no parameters, then the current graphical style is reset to bootup defaults. You may specify one or more parameters with this command to specifically alter particular aspects of the graphics style system. You may specify the following parameters:

| Parameter | Description |
| --- | --- |
| CUR | Reset the current graphical style to default values. |
| 0-35 | Reset a specific graphical style data table to defaults. |
| TBL | Reset all graphical style data table entries to defaults. |
| BASE | Reset (clear) the graphical style base save area |
| S0-S9 | Reset (clear) a specific graphical style data save slot |
| SLOTS | Reset (clears) all graphical style data save slots. |
| STACK | Clears the graphical style data save stack. |
| BACKUP | Reset (clears) all graphical style backup areas (base area, slots and stack. |
| ALL | Reset all graphical style data tables and backup areas |

**Example:** `$RESET(STYLE, CUR, 30, BASE, S5)$`

**BUT** - This parameter resets a button style to basic button default values (see the `RIP_BUTTON_STYLE` command for more details). If you specify no parameters, then the current button style is reset to bootup defaults. You may specify one or more parameters with this command to specifically alter particular aspects of the button style system. You may specify the following parameters:

| Parameter | Description |
| --- | --- |
| CUR | Reset the current button style to default values. |
| 0-35 | Reset a specific button style data table to defaults. |
| TBL | Reset all button style data table entries to defaults. |
| BASE | Reset (clear) the button style base save area |
| S0-S9 | Reset (clear) a specific button style data save slot |
| SLOTS | Reset (clears) all button style data save slots. |
| STACK | Clears the button style data save stack. |
| BACKUP | Reset (clears) all button style backup areas (base area, slots and stack. |
| ALL | Reset all button style data tables and backup areas |

**Example:** `$RESET(BUT, CUR, 30, BASE, S5)$`

**PAL** - Resets a color palette back to the default color palette. If you specify no parameters, then the current color palette is reset to bootup defaults. You may specify one or more parameters with this command to specifically alter particular aspects of the color palette system. You may specify the following parameters:

| Parameter | Description |
| --- | --- |
| CUR | Reset the current color palette to default values. |
| 0-35 | Reset a specific color palette data table to defaults. |
| TBL | Reset all color palette data table entries to defaults. |
| BASE | Reset (clear) the color palette base save area. |
| S0-S9 | Reset (clear) a specific color palette data save slot. |
| SLOTS | Reset (clears) all color palette data save slots. |
| STACK | Clears the color palette data save stack. |
| BACKUP | Reset (clears) all color palette backup areas (base area, slots and stack. |
| ALL | reset all color palette data tables and backup areas |

**Example:** `$RESET(PAL, CUR, 30, BASE, S5)$`

**PORT** - This variation on the reset command resets one or more ports in some way. The exact way that one is reset varies depending on the type of port specified. If no additional parameters are specified, then the current port is reset. Otherwise, you may reset a specific port number in the port data table, or one (or all) of the port data backup areas. If you reset a specific port in the port data table, what happens varies depending on what kind of port it is. If it is port #0 (which cannot be deleted), then the only thing that happens is that any resident queries for that port/viewport are deleted, the viewport is made full screen and the viewport is erased to the background color (in that order). If the port number (from 1-35) represents a screen port, then any resident query attached to that port/viewport is deleted and the port itself is deleted. If it is an offscreen port, then that port is also deleted (offscreen ports cannot have resident queries). In any event, if the port being deleted happens to be the current port, then the port is automatically switched to port #0 (the screen's port). The following parameters are permitted with this reset port). Whenever the current port is reset, the port is automatically switched to port #0. Beware of this when chaining multiple port resets together at the same time. For example, if port #5 is current and you issue the following command `$RESET(PORT, 5, CUR)$`, then port number 5 will be deleted, then it would switch back to port #0 which is the screen port and cannot be deleted, so only its viewport will be reset. The following parameters are permitted with this reset command to control what type of reset operation is to be performed:

| Parameter | Description |
| --- | --- |
| CUR | Reset (delete) the current port. |
| 0-35 | Reset (delete) a specific port data table entry. |
| TBL | Reset (delete) all port data table entries. |
| BASE | Reset (clear) the port base save area. |
| S0-S9 | Reset (clear) a specific port data save slot. |
| SLOTS | Reset (clears) all port data save slots. |
| STACK | Reset (clears) the port data save stack. |
| BACKUP | Reset (clears) all port backup areas (base area, slots and stack. |
| ALL | Reset all port data tables and backup areas |

**Example:** `$RESET(PORT, CUR, 30, BASE, S5)$`

**ENV** - This command resets an environment to either default values or to a status of "erased". If you specify no parameters, then the current environment data table entry is reset to default bootup values. If you do specify any parameters, then the following ones may be used:

| Parameter | Description |
| --- | --- |
| CUR | Reset the current environment to default values. |
| 0-35 | Reset a specific environment data table entry to defaults. |
| TBL | Reset all environment data table entries to defaults. |
| BASE | Reset (clear) the environment base save area |
| S0-S9 | Reset (clear) a specific environment data save slot. |
| SLOTS | Reset (clears) all environment data save slots. |
| STACK | Clears the environment data save stack. |
| BACKUP | Reset (clears) all environment backup areas (base area, slots and stack. |
| ALL | Reset all environment data tables and backup areas |

**Example:** `$RESET(ENV, CUR, 30, BASE, S5)$`

**MOUSE** - Resets (deletes) all mouse fields defined in the specified specified destination. If no parameter is defined, then all current mouse definitions are reset. If you specify one or more parameters, then they may be any of the following:

| Parameter | Description                                               |
| --------- | --------------------------------------------------------- |
| TBL       | Reset (clear) all existing mouse definitions in use       |
| BASE      | Reset (clear) the mouse field base save area              |
| S0-S9     | Reset a specific mouse field data save slot               |
| SLOTS     | Reset all mouse field data save slots                     |
| STACK     | Clears the data save stack pointer, but don't reset slots |
| BACKUP    | Reset all mouse field backup areas                        |
| ALL       | Reset all mouse field data tables and backup areas        |

**Example:** `$RESET(MOUSE, BASE, S5)$`

**SCREEN** - When no parameters are specified, the entire video screen is cleared to color #0 (usually black). No viewports are modified, nor are any drawing ports, text windows, mouse fields or anything else. The following parameters are allowed, providing you with the ability to reset particular screen aspects:

| Parameter | Description                                               |
| --------- | --------------------------------------------------------- |
| BASE      | Reset (clear) the screen base save area                   |
| S0-S9     | Reset a specific screen data save slot area               |
| SLOTS     | Reset all screen data save slots                          |
| STACK     | Clears the data save stack pointer, but don't reset slots |
| BACKUP    | Reset all screen backup areas                             |
| ALL       | Reset the screen (erase) and all backup areas             |

**VIEW** - When no parameters are specified, then the viewport of the current drawing port is reset to the full dimensions of the port itself, and activated. Other possible parameters allow you to modify other viewports of other ports. The possible parameters are:

| Parameter | Description                                                    |
| --------- | -------------------------------------------------------------- |
| CUR       | Reset the current viewport to full port size                   |
| 0-35      | Reset the viewport of specific port to full port size          |
| TBL       | Resets all viewports of all data table ports to full port size |

**QUERY** - If no parameters are specified or a single parameter of "ALL" is specified then all resident queries are reset (deleted). If you specify any parameters, you may specify more than one. You may specify the following as the first parameters:

| Parameter | Description                                  |
| --------- | -------------------------------------------- |
| TW        | Reset the text window based resident queries |
| VIEW      | Reset the viewport based resident queries    |

The next parameter(s) (if any) indicate the data table entry number that you wish to reset. If no parameters are specified, then all queries for the given data type (text window or viewport) will be reset. If you specify an actual data table entry number, then you are deleting the query for a specific text window or viewport (a fixed resident query). You are allowed to use the following parameters after the TW or VIEW directives:

| Parameter | Description |
| --- | --- |
| ALL | Reset the current window query and all fixed queries |
| CUR | Reset the "current window" query for the given reset type (ie, TW or VIEW) |
| TBL | Reset all entry-specific queries (ie, reset all resident queries for all specific text windows or viewports. The "current window" query isn't reset. |
| 0-35 | Reset the resident query for the specified data table entry for the given TW or VIEW query type. |

See the `RIP_QUERY` command for more detailed information about the various possible resident query types.

**Example:**

```text
$RESET(QUERY, TW, CUR)$ ... Reset current text window's query
$RESET(QUERY, TW, TBL)$ ... Reset all queries for entries 0-35
$RESET(QUERY, TW, 5)$ ..... Reset resident query for Text window #5
```

It should be noted that if you attempt to reset something that cannot be reset (eg, drawing port #0, a protected data table entry, a stack that's empty, etc), then this command does nothing and does not generate a syntax error. A syntax error can only be generated if an invalid parameter is encountered. If even so much as a single parameter is invalid then the entire command is discarded as a syntax error without any of the parameters being processed.

---

[◀ Prev: Text Variables: Mouse, Text Window & Ports](18-text-variables-mouse-window.md) · [Contents](README.md) · [Next: Text Variables: Environment, Clipboard, Screen & Tables ▶](20-text-variables-environment.md)
