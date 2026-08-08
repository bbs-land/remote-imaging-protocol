# Text Variables: Mouse, Text Window & Ports

[◀ Prev: Text Variables: General, Date/Time & Sound](17-text-variables-general.md) · [Contents](README.md) · [Next: Text Variables: Terminal & Reset ▶](19-text-variables-terminal.md)

## Mouse Related Text Variables

These text variables return information on, or manipulate the mouse in some fashion. _(v2.A4)_

### $M$

_Mouse Button Status: LMR_

**Format:** `$M$` **Syntax:** `$M$`

This Text Variable returns a 3-character code representing the status of each mouse button. This variable works with two button and three button mice. The format of the code is LMR where L=Left, M=Middle (if any), and R=Right. If any button is clicked, the code for that button is "1". If the button is not depressed, it is "0". "100" would mean the left mouse button is depressed, but none of the others are.

**Example:** `$M$` **Returns:** `001`

### $MCURSOR$

_Set the mouse cursor style number_

_Added in RIPscrip v2.A1._

**Format:** `$MCURSOR(cursor_no)$`

**Syntax:**

```text
$MCURSOR(opt:CURSOR_NO)$
             ─────────
             0-6
             ─────────
         def=0
```

This command changes the current mouse cursor style to one of the pre-defined mouse cursor styles as defined in the [RIP_SET_MOUSE_CURSOR](11-level-1-commands.md#rip_set_mouse_cursor) command. The cursor_no parameter is identical to the cursor style numbers permitted for the RIPscrip command to alter the cursor. If you omit the CURSOR_NO parameter, the default "pointer" cursor is activated.

### $MSTAT$

_Mouse Status_

**Format:** `$MSTAT$` **Syntax:** `$MSTAT$`

This Text Variable returns a "YES" if there is a mouse installed on the RIPterm computer. If no mouse is installed, this variable returns "NO".

**Example:** `$MSTAT$` **Returns:** `YES`

### $X$

_X Mouse location_

**Format:** `$X(domain)$`

**Syntax:**

```text
$X(opt:DOMAIN)$
       ──────
       WORLD
       DEVICE
       ──────
   def=WORLD (except under "text mode query - see below)
```

This Text Variable returns the current X coordinate of the mouse pointer. This can be used interactively (for example, by on-line games) to determine the location of the mouse pointer. Only the X value of the mouse (X,Y) is returned. The value is 0000-9999 depending on what the current position is.

If this text variable is used inside a "text window query", then the X coordinate is returned in text cell coordinates based on the internal dimensions of the text window. This kind of result cannot happen unless the user clicks inside a text window's display area that has a text window query command in it, so there's no possibility for the coordinate to be out of bounds for that text window's dimensions. _(v2.A4)_

The results are in current world coordinates if no parameter is specified. If TYPE is "DEVICE" then the result is in raw video device coordinates. If TYPE is "WORLD" then the result is in desktop world coordinates. If the domain parameter is specified when this text variable is used in a text mode query, then it is explicitly overiding the "text coordinate" defaults of this command and returning the mouse coordinate information in the desired coordinate system. _(v2.A4)_

**Example:** `$X(WORLD)$` — equivalent to `$X$` **Returns:** `0523`

### $XY$

_X/Y Mouse Location_

**Format:** `$XY(domain)$`

**Syntax:**

```text
$XY(opt:DOMAIN)$
        ──────
        WORLD
        DEVICE
        ──────
    def=WORLD (except under text mode query - see below)
```

This Text Variable returns both the X and Y coordinates of the mouse pointer. A colon (:) separates the two values. The X and Y values may range from 0000-9999. The format that this value uses is: XXXX:YYYY

If this text variable is used inside a "text window query", then the X/Y coordinates are returned in text cell coordinates based on the internal dimensions of the text window. This kind of result cannot happen unless the user clicks inside a text window's display area that has a text window query command in it, so there's no possibility for the coordinate to be out of bounds for that text window's dimensions. The format that the coordinates are returned in is XX:YY where XX and YY can range from 00-99. _(v2.A4)_

The results are in current world coordinates if no parameter is specified. If TYPE is "DEVICE" then the results are in raw video device coordinates. If TYPE is "WORLD" then the results are in desktop world coordinates. If the domain parameter is specified when this text variable is used in a text mode query, then it is explicitly overiding the "text coordinate" defaults of this command and returning the mouse coordinate information in the desired coordinate system. _(v2.A4)_

**Example:** `$XY(WORLD)$` — equivalent to `$XY$` **Returns:** `0297:0321`

### $XYM$

_X, Y & button status_

**Format:** `$XYM(domain)$`

**Syntax:**

```text
$XYM(opt:DOMAIN)$
         ──────
         WORLD
         DEVICE
         ──────
     def=WORLD (except under text mode query - see below)
```

This Text Variable returns the X and Y coordinates of the mouse pointer, and which mouse buttons are pressed (if any). A colon (:) separates the three values. The X and Y values may range from 0000-9999. LMR stands for Left/Middle/Right. If any of these buttons are depressed (clicked), then the corresponding position will contain a 1. If a button is NOT depressed, then it will contain a 0. The format that this value uses is: XXXX:YYYY:LMR

If this text variable is used inside a "text window query", then the X/Y coordinates are returned in text cell coordinates based on the internal dimensions of the text window. This kind of result cannot happen unless the user clicks inside a text window's display area that has a text window query command in it, so there's no possibility for the coordinate to be out of bounds for that text window's dimensions. The format that the coordinates are returned in is XX:YY:LMR where XX and YY can range from 00-99. _(v2.A4)_

This means that the (X,Y) location of the cursor is (0123,0297), and that the Left and Middle buttons are depressed, but that the Right Mouse Button is not depressed.

The results are in current world coordinates if no parameter is specified. If TYPE is "DEVICE" then the results are in raw video device coordinates. If TYPE is "WORLD" then the results are in desktop world coordinates. If the domain parameter is specified when this text variable is used in a text mode query, then it is explicitly overiding the "text coordinate" defaults of this command and returning the mouse coordinate information in the desired coordinate system. _(v2.A4)_

**Example:** `$XYM(WORLD)$` — Equivalent to `$XYM$` **Returns:** `0123:0297:110`

### $Y$

_Y Mouse location_

**Format:** `$Y(domain)$`

**Syntax:**

```text
$Y(opt:DOMAIN)$
       ──────
       WORLD
       DEVICE
       ──────
   def=WORLD (except under text mode query - see below)
```

This Text Variable returns the current Y coordinate of the mouse pointer. This can be used interactively (for example, by on-line games) to determine the location of the mouse pointer. Only the Y value of the Mouse (X,Y) is returned. The value is 0000-9999 depending on what the current position is.

If this text variable is used inside a "text window query", then the Y coordinate is returned in text cell coordinates based on the internal dimensions of the text window. This kind of result cannot happen unless the user clicks inside a text window's display area that has a text window query command in it, so there's no possibility for the coordinate to be out of bounds for that text window's dimensions. _(v2.A4)_

The results are in current world coordinates if no parameter is specified. If TYPE is "DEVICE" then the result is in raw video device coordinates. If TYPE is "WORLD" then the result is in desktop world coordinates. If the domain parameter is specified when this text variable is used in a text mode query, then it is explicitly overiding the "text coordinate" defaults of this command and returning the mouse coordinate information in the desired coordinate system. _(v2.A4)_

**Example:** `$Y(WORLD)$` — equivalent to `$Y$` **Returns:** `0244`

## Text Window Related Text Variables

These text variables return information on, or manipulate the text window. _(v2.A4)_

### $ATW$

_Activates a text window definition_

_Added in RIPscrip v2.A1._

**Format:** `$ATW(window1,window2,...)$`

**Syntax:**

```text
$ATW(opt:WINDOW1, ...)$
         ───────
         ALL
         CUR
         0-35
         ───────
     def=CUR
```

This command does the exact opposite of the [`$DTW$`](#dtw) text variable which deactivates a text window. When a window is activated, it no longer discards raw ANSI text that is sent to it. This does not make any visual changes on the screen immediately unless the current text window is the one activated, whereby the cursor might appear all of a sudden.

You may specify one or more parameters to indicate that you wish to activate more than one text window in the same `$ATW$` statement. If any one of them fails to match the proper parameters, then the entire command is considered a syntax error and none of the parameters are processed.

If the text window being activated is protected, then it is not changed (ie, activated).

You may activate more then one window by specifying multiple text window slot numbers (0-35). You may also specify the following:

| Keyword | Description                           |
| ------- | ------------------------------------- |
| `ALL`   | Activate all text windows at once     |
| `CUR`   | Activate only the current text window |

**Example:** `$ATW(CUR)$` **Returns:** nothing

### $COFF$

_Disable the Text Cursor_

**Format:** `$COFF(window1,window2,...)$`

**Syntax:**

```text
$COFF(opt:WINDOW1, ...)$
          ───────
          ALL
          CUR
          0-35
          ───────
      def=CUR
```

This Active Text Variable turns off the text cursor in the specified text window(s). If that text window(s) is undefined or disabled, this this command does nothing for that parameter. If the window parameters are omitted, then the cursor in the current text window is displayed. If the window parameter(s) are specified, then they can be a value from 0-35 to indicate a specific text window data table entry, the value "CUR" to indicate the current text window, or the value of "ALL" to indicate that you wish to turn off the cursor in all text windows simultaneously. _(v2.A4)_

If you attempt to turn off a cursor in a text window that isn't defined, or that is deactivated, then that parameter does nothing. _(v2.A4)_

If any one of the parameters is invalid, then the entire command is considered a syntax error and none of the parameters are processed. _(v2.A4)_

If you turn off the cursor in a text window other than the current text window, then only the internal cursor status of that text window is altered. Since the cursor wouldn't be visible in that text window anyway, you wouldn't expect anything to visually happen on the screen. If you switch to that text window though, the cursor would not be displayed upon switching to it (normally it would). _(v2.A4)_

**Example:** `$COFF$` **Returns:** nothing

### $CON$

_Enable the Text Cursor_

**Format:** `$CON(window1,window2,...)$`

**Syntax:**

```text
$CON(opt:WINDOW1, ...)$
         ───────
         ALL
         CUR
         0-35
         ───────
     def=CUR
```

This Active Text Variable turns on the text cursor in the specified text window(s). If that text window is undefined or deactivated, this command does nothing. If the window parameter(s) are omitted, then the cursor in the current text window is displayed. If the window parameter is specified, then it can be a value from 0-35 to indicate a specific text window data table entry, the value "CUR" to indicate the current text window, or the value "ALL" to indicate all defined text windows (ie, in use). _(v2.A4)_

If you attempt to turn on a cursor in a text window that isn't defined, or that is deactivated, then that parameter does nothing. _(v2.A4)_

If any one of the parameters is invalid, then the entire command is considered a syntax error and none of the parameters are processed. _(v2.A4)_

If you turn on the cursor in a text window other than the current text window, then only the internal cursor status of that text window is altered - it will not be turned on until you switch to that text window. In this respect, only altering the current text window's cursor will actually make any immediate visible effect on the screen. _(v2.A4)_

**Example:** `$CON$` **Returns:** nothing

### $CURSOR$

_Text Cursor Status_

**Format:** `$CURSOR(window)$`

**Syntax:**

```text
$CURSOR(opt:WINDOW)$
            ──────
            CUR
            0-35
            ──────
        def=CUR
```

This Text Variable returns "YES" if the Text Cursor is enabled in the specified text window, and "NO" if the Text Cursor is deactivated. If the specified text window doesn't exist, or is currently deactivated, then this command returns a value of "NO". _(v2.A4)_

If you do not specify a window parameter, then this command refers to the current text window. If you specify the window number parameter, then you can specify a value from 0-35 to indicate a specific text window data table entry, or a value of "CUR" to indicate the current text window. _(v2.A4)_

**Example:** `$CURSOR$` **Returns:** `YES`

### $CURX$

_Text Cursor X Coordinate_

**Format:** `$CURX(window)$`

**Syntax:**

```text
$CURX(opt:WINDOW)$
          ──────
          CUR
          0-35
          ──────
      def=CUR
```

This Text Variable returns the X coordinate of the text cursor in the specified text window, relative to the upper left of the Text Window. The location is "one based", so the first column of data is a value of "1", not "0". Typical return values for valid cursor locations could easily range from 1-91. If the specified text window is not defined, or is currently deactivated (via the [`$DTW$`](#dtw) command for example), then this text variable returns a value of "0" to indicate that the information isn't available. _(v2.A4)_

If you do not specify a window parameter, then this command refers to the current text window. If you specify the window number parameter, then you can specify a value from 0-35 to indicate a specific text window data table entry, or a value of "CUR" to indicate the current text window. _(v2.A4)_

**Example:** `$CURX(4)$` **Returns:** `2`

### $CURY$

_Text Cursor Y Coordinate_

**Format:** `$CURY(window)$`

**Syntax:**

```text
$CURY(opt:WINDOW)$
          ──────
          CUR
          0-35
          ──────
      def=CUR
```

This Text Variable returns the Y coordinate of the text cursor in the specified text window, relative to the upper left of the Text Window. The location is "one based", so the first column of data is a value of "1", not "0". Typical return values for valid cursor locations could easily range from 1-43. If the specified text window is not defined, or is currently deactivated (via the [`$DTW$`](#dtw) command for example), then this text variable returns a value of "0" to indicate that the information isn't available. _(v2.A4)_

If you do not specify a window parameter, then this command refers to the current text window. If you specify the window number parameter, then you can specify a value from 0-35 to indicate a specific text window data table entry, or a value of "CUR" to indicate the current text window. _(v2.A4)_

**Example:** `$CURY(4)$` **Returns:** `5`

### $DTW$

_Deactivate Text Window_

**Format:** `$DTW(window1,window2,...)$`

**Syntax:**

```text
$DTW(opt:WINDOW1, ...)$
         ───────
         ALL
         CUR
         0-35
         ───────
     def=CUR
```

If no slot parameter is specified then the current text window is deactivated, preventing any received raw text from being displayed in that window. Switching to another text window data table entry can re-enable text displaying if that other window is "activated". _(v2.A4)_

You may specify one or more parameters to indicate that you wish to disable more than one text window in the same `$DTW$` statement. If any one of them fails to match the proper parameters, then the entire command is considered a syntax error and none of the parameters are processed. _(v2.A4)_

If a slot (0-35) is specified then that text window slot is deactivated. This may or may not affect the current status of received text. If the slot specified just happens to be the current slot then all received text is deactivated. If the text window slot isn't the current text window then that text window is deactivated, but what is done with received raw text depends on the status of the current text window's status. _(v2.A4)_

You may specify "CUR" to explicitly deactivate the current text window. _(v2.A4)_

You may also specify a slot of "ALL" to deactivate all defined text window slots. Text window data table entries that are not defined, or that are protected are not affected by this operation. This would also deactivate any raw text that is received because the act of deactivating all windows will consequently deactivate the current one too. _(v2.A4)_

If the text window being deactivated is protected, then it is not changed (ie, deactivated). _(v2.A4)_

This command is useful in Host Commands when you click on a Mouse Field, it would halt any further output to the text window.

**Example:** `$DTW$` **Returns:** nothing

### $ETW$

_Erase Text Window_

**Format:** `$ETW(window1,window2,...)$`

**Syntax:**

```text
$ETW(opt:WINDOW1, ...)$
         ───────
         ALL
         CUR
         0-35
         ───────
     def=CUR
```

If no parameter(s) are specified then the current text window in use is cleared. If you specify a slot from 0-35 then the corresponding text window slot is erased. You may also specify "ALL" to erase all currently defined text window slots. Finally, you may specify the value of "CUR" to also indicate the current text window. _(v2.A4)_

It should be noted that the entire text window's bounding rectangle is erased, not just the display region inside the bounding rectangle. _(v2.A4)_

You may specify one or more parameters to indicate that you wish to erase more than one text window in the same `$ETW$` statement. If any one of them fails to match the proper parameters, then the entire command is considered a syntax error and none of the parameters are processed. _(v2.A4)_

If any text window specified aren't defined, or if the viewport is deactivated, then this command does nothing for that viewport parameter. _(v2.A4)_

This command is useful in Host Commands when you click on a Mouse Field, it could erase the text window THEN transmit the remainder of the Host Command (if any).

**Example:** `$ETW(ALL)$` **Returns:** nothing

### $ISEXTWIN$

_Is text window an extended text window?_

_Added in RIPscrip v2.A4._

**Format:** `$ISEXTWIN(windowno)$`

**Syntax:**

```text
$ISEXTWIN(opt:WINDOWNO)$
              ────────
              CUR
              0-35
              ────────
          def=CUR
```

This text variable determines if the specified text window is an extended text window or not. If the WINDOWNO parameter is omitted, then it will inquire about the current text window. If WINDOWNO is specified, then it must be the value "CUR" for the current text window, or a window data table entry number from 0-35 to inquire about a specific text window data table entry.

If the specified text window is not defined, or deactivated, then this text variable returns a value of "-1". If it is an extended text window, then a "1" is returned, otherwise a "0" is returned to indicate that it is a standard (non-extended) text window.

**Example:** `$ISEXTWIN(5)$` **Returns:** `1`

### $MTW$

_Maximize text window to full size_

_Added in RIPscrip v2.A1._

**Format:** `$MTW(window1,window2,...)$`

**Syntax:**

```text
$MTW(opt:WINDOW1, ...)$
         ───────
         ALL
         CUR
         0-35
         ───────
     def=CUR
```

This command maximizes a particular text window to full screen using whatever text window font is associated with that text window. This does not affect any text that is already on the screen, only the internal definition of the text window (any scroll margins are reset). The cursor is moved to the home position, but is not re-enabled if it was previously disabled (use [`$CON$`](#con) for this). ANSI color attributes are not changed. The possible settings for the WHICH parameter are:

| Keyword  | Description                                   |
| -------- | --------------------------------------------- |
| `ALL`    | Maximize all text windows at once             |
| `CUR`    | Maximize only the current text window         |
| `number` | Maximize a particular text window slot number |

You may specify one or more parameters to indicate that you wish to maximize more than one text window in the same `$MTW$` statement. If any one of them fails to match the proper parameters, then the entire command is considered a syntax error and none of the parameters are processed.

It should be noted that if a text window being maximized is a standard text window created with the [RIP_TEXT_WINDOW](10-level-0-commands-s-w.md#rip_text_window) command, or if you just switched to a previously "unused" text window without explicitly defining it with a [RIP_EXTENDED_TEXT_WINDOW](08-level-0-commands-a-f.md#rip_extended_text_window) command, then this command will treat the text window still as a "standard" text window. The window is moved to the very upper-left corner of the display. If it was an extended text window, then it is still moved to the upper-left corner of the screen, and the bounding box is made to exactly fit the actual text window display region. Remember, extended text windows don't know anything about text coordinate base locations of the upper-left corner, but standard text windows do. This command maintains this nature of the text window being maximized.

If a text window being maximized is currently protected, this command also does nothing to that text window.

**Example:** `$MTW(ALL)$` **Returns:** nothing

### $RTW$

_Restore Text Window information_

**Format:** `$RTW(source)$`

**Syntax:**

```text
$RTW(opt:SOURCE)$
         ──────
         BASE
         POP
         0-9
         ──────
     def=BASE
```

If no slot parameter is specified then this command restores all text window definitions from the slot-less definition file saved with a [`$STW$`](#stw) command (with no parameters). The file is not deleted and you can restore many times. _(v2.A1)_

If you specify a slot parameter then that identifies which of the ten different slots you wish to restore from (0-9). Once the slot file is read it is deleted. _(v2.A1)_

If you specify a slot number of "POP" then you are performing a stack-based pop operation (eg, `$RTW(POP)$`). _(v2.A1)_

If you specify "BASE" instead of a slot number than the text window table will be restored from the text window backup area's base save area. _(v2.A4)_

In any case, the text window settings active when a `$STW$` (Save Text Window) was executed are saved. The current cursor location, window location, ANSI attributes, cursor ON/OFF status, vertical scrolling margins, and the System Font are restored. _(v2.A1)_

> **NOTE:** The text contents of the window are not restored.

**Example:** `$RTW(5)$` **Returns:** nothing

### $STW$

_Save Text Window information_

**Format:** `$STW(destination)$`

**Syntax:**

```text
$STW(opt:DESTINATION)$
         ───────────
         BASE
         PUSH
         0-9
         ───────────
      def=BASE
```

If no slot parameter is specified, then this command stores all currently defined text windows' settings base save area. The windows' X/Y dimensions are preserved, as are the current cursor location, ANSI attributes, cursor ON/OFF status and the vertical scrolling margins. Even the current System Fonts used for all windows are saved (if necessary). _(v2.A4)_

If you specify a slot parameter, then you are specifying to save the text window definitions to one of up to ten different slots (0-9). These allow you to have up to ten different text window configurations saved at the same time. If you save to a slot then when you restore that slot the saved definitions are deleted from the disk. If you don't specify a slot parameter (a slot-less save) then you can restore that definition file multiple times without the file being deleted. _(v2.A1)_

If you specify a slot number of "PUSH" then you are performing a stack-based save operation (eg, `$STW(PUSH)$`). _(v2.A1)_

If you specify "BASE" instead of a slot number than the text window table will be stored into the text window backup area's base save area. _(v2.A4)_

> **NOTE:** The contents of the Text Window are not saved.

**Example:** `$STW(5)$` **Returns:** nothing

### $TWERASEEOL$

_Erase text window to line end_

_Added in RIPscrip v2.A1._

**Format:** `$TWERASEEOL(window1,window2`

**Syntax:**

```text
$TWERASEEOL(opt:WINDOW1, ...)$
                ───────
                ALL
                CUR
                0-35
                ───────
            def=CUR
```

This variable clears the current line in the specified text window from the cursor (inclusive) to the end of the line in the current ANSI color attributes. If the you specify no parameters, then the current text window is used for the destination window. If you do specify any parameters, they must be set to "CUR" for the current text window, "ALL" for all currently defined text windows, or 0-35 to indicate a specific text window slot number.

You may specify one or more parameters to indicate that you wish to erase more than one text window's current lines in the same `$TWERASEEOL$` statement. If any one of them fails to match the proper parameters, then the entire command is considered a syntax error and none of the parameters are processed.

If the specified text window is deactivated or undefined then nothing happens for that parameter.

Unlike many other text window commands, this one can make an actual visual effect on the screen even if the specified text window isn't the current text window.

**Example:** `$TWERASEEOL(5)$` **Returns:** nothing

### $TWFONT$

_Active Text Font_

**Format:** `$TWFONT(window)$`

**Syntax:**

```text
$TWFONT(opt:WINDOW)$
            ──────
            CUR
            0-35
            ──────
        def=CUR
```

This Text Variable returns which of the five Text Window Fonts is active, or 0 (zero) if specified text window is not defined, or is deactivated. The following values are returned: _(v2.A4)_

| Value | Description          |
| ----- | -------------------- |
| 0     | No Text Window       |
| 1     | 80x43 font           |
| 2     | 91x43 MicroANSI font |
| 3     | 80x25 font           |
| 4     | 91x25 MicroANSI font |
| 5     | 40x25 font           |

Note that this command returns the actual text window font number plus one. So to match the returned value to actual RIPscrip text window font numbers, subtract one from this result (providing that it is greater than 0). _(v2.A4)_

If you do not specify a window parameter, then this command refers to the current text window. If you do specify the window number parameter, then you may specify a value of 0-35 for a specific text window data table entry, or the value "CUR" to indicate the current text window (the default). _(v2.A4)_

**Example:** `$TWFONT(4)$` **Returns:** `1`

### $TWGOTO$

_Move cursor to X,Y in text window_

_Added in RIPscrip v2.A1._

**Format:** `$TWGOTO(winno,x_pos,y_pos)$`

**Syntax:**

```text
$TWGOTO(req:WINNO, opt:X_POS, opt:Y_POS)$
            ─────      ─────      ─────
            CUR        CUR        CUR
            0-35       0-90       0-42
                       ─────      ─────
                   def=CUR    def=CUR
```

This variable allows you to move the cursor position in a given text window to a specified X/Y position. If no parameters are given or no X/Y parameters are given then nothing happens when this is executed.

If the specified text window doesn't exist, or is deactivated, then this command does nothing.

The WINNO parameter specifies a text window data table entry to move the cursor in. Valid settings for WINNO are 0-35 for a specific text window data table entry, or "CUR" to indicate the current text window. If the specified window is not the current text window then the cursor in that window's definition is moved but no visible things occur on the screen. If the specified window is "CUR" or the window number happens to be the current text window, then the cursor that is (potentially) on the screen is moved to the new location.

The X/Y coordinates are text coordinates (1 based). Specifying both coordinates allows you to place the cursor anywhere in the chosen window. You may omit the Y parameter entirely to move the cursor horizontally only. Either the X or Y parameter (or both) may be specified as "CUR" to indicate the the row or column is not to change. In other words, if you wanted to move the cursor to line 2 in the text window 5, but leave its column unchanged, you would use `$TWGOTO(5,CUR,2)$`.

If the goto sequence would put the cursor outside the dimensions of the text window then the cursor is placed at the closest point to the given location at the windows border.

Text locations in this command are zero based (eg, the first column is specified as "0"). If the no X or Y parameters are specified, then this command does nothing because there is no X/Y information to move to.

**Example:** `$TWGOTO(CUR,10,2)$` **Returns:** nothing

### $TWH$

_Text Window Height_

**Format:** `$TWH(window,type,domain)$`

**Syntax:**

```text
$TWH(opt:WINDOW, opt:TYPE, opt:DOMAIN)$
         ──────      ────      ──────
         CUR         BOUND     WORLD
         0-35        CELL      DEVICE
         ──────      TEXT      ROWS:TYPE=TEXT
     def=CUR         ────      ──────
                 def=TEXT  def=ROWS:TYPE=TEXT
                               WORLD:TYPE<>TEXT
```

If no parameters are specified, then this command returns the height of the current text window in text cells (lines). You may specify a window number to indicate which text window you are inquiring about. The text window number parameters may be a value from 0-35 to reference a specific text window data table entry, or you may specify a keyword of "CUR" to indicate the current text window. If the specified text window doesn't exist, or is deactivated, then a value of "0" is returned. _(v2.A4)_

If you specify any parameters then they must be in a specific order. The first (already discussed) is the text window number, and if it is omitted, then the current text window is assumed. If you specify two parameters, then you must specify the window number parameter and the type parameter. The type parameter determines what type of information you are inquiring about. If you omit the type parameter then you are inquiring about the text window height itself, in actual text window cell sizes (eg, the window might respond with "5" to indicate that it is 5 lines of text tall. _(v2.A4)_

If you specify the type parameter, then you are directing this command to respond with information about a specific text window piece of information. The available type parameters are: _(v2.A4)_

| Keyword | Description                                         |
| ------- | --------------------------------------------------- |
| `BOUND` | Return the height of the bounding box in pixels     |
| `TEXT`  | Return the height of the display region             |
| `CELL`  | Return the height of a text cell in the text window |

The third and final parameter is only of use if the BOUND or CELL parameter is used, and it determines the domain of the pixel size returned by the text variable. The possible values of domain are: _(v2.A4)_

| Keyword | Description |
| --- | --- |
| `WORLD` | Return the value in world coordinates |
| `DEVICE` | Return the value in raw device pixel coordinates |
| `ROWS` | Return height of area in text lines (valid only for a type of "TEXT". |

If the domain parameter is omitted, then it automatically defaults to WORLD (except for the TEXT type, which defaults to the height of the text window in lines of text). When a world coordinate value is returned, the actual "raw" dimensions are converted to the current environment's world coordinate system then returned to the host. If the domain is set to "DEVICE" then the value is returned in raw, device-level pixel coordinates. _(v2.A4)_

The BOUND parameter refers to the text window's bounding rectangle. It is the absolute exterior that the text window may extend to. It can also be thought of as a margin outside the actual text window where nothing can be displayed from the text window. See the RIP_EXTENDED_TEXT_WINDOW for more details about this bounding rectangle. _(v2.A4)_

The CELL parameter indicates that you are inquiring about the height of a character cell used in the text window. The height of a cell is based on the font number and resolution currently in use on the actual destination RIPscrip software package. Under 640x350, this was typically 8x8 for font #0 (ie, in RIPscrip 1.54). This may be different now at varying resolutions. So, if you are running at a resolution of 640x350 and you inquire about a text window's cell height for a text window using font #0, then it most likely will return a result of 8 pixels (in device coordinates). The actual value in world coordinates could vary well be different based on the size of the world coordinate resolution frame. _(v2.A4)_

The "TEXT" type indicates that you are requesting information about the text window sub-section of the text window definition. If the domain parameter is specified, then you are requesting the actual height of the text window display area in "lines of text". This can also be retrieved if the domain parameter is "ROWS". This particular domain parameter known as "ROWS" is only valid for a type parameter of "TEXT". Using it in any other context is considered a syntax error. If the domain parameter is specified, and it is set to "WORLD", then the pixel height of the display region is being requested in world coordinates. If domain is set to DEVICE, then you are requesting the height of the display region in actual hardware pixel coordinates. _(v2.A4)_

It should be noted that the bounding box of a text window can be obtained regardless of whether it is an old-style 1.54 text window created with RIP_TEXT_WINDOW or a new "resolution independent" text window defined via the RIP_EXTENDED_TEXT_WINDOW command. This can be because under the older text window concept, the bounding box is exactly as large as the text window itself, while under the extended text window, the text window will typically be inside the bounding rectangle. _(v2.A4)_

If you completely omit the type parameter, then you must omit the domain parameter. In this case, you are asking for the height of the text window in "lines of text", not in pixels. _(v2.A4)_

**Example:** `$TWH$` — Height of current text window in lines of text. **Returns:** `5`

**Example:** `$TWH(CUR)$` — Height of current text window in lines of text. **Returns:** `5`

**Example:** `$TWH(5,BOUND)$` — Height of text window 5's bounding box in world coordinates. **Returns:** `800`

**Example:** `$TWH(5,BOUND,DEVICE)$` — Height of text window 5's bounding box in device pixel coordinates **Returns:** `400`

**Example:** `$TWH(CUR,CELL)` — Height of current text window's character cell in world coordinates. **Returns:** `16`

**Example:** `$TWH(5,CELL,DEVICE)$` — Height of text window 5's character cell in device pixel coordinates.

### $TWHOME$

_Move cursor to home position in text window_

_Added in RIPscrip v2.A1._

**Format:** `$TWHOME(winno)$`

**Syntax:**

```text
$TWHOME(opt:WINNO)$
            ─────
            CUR
            0-35
            ─────
        def=CUR
```

This command moves the cursor in the specified text window data table entry to the upper-left corner of the window. WINNO may be any value from 0-35 to indicate the desired data table entry, or the value "CUR" to indicate the current text window. If the WINNO slot number happens to correspond to the current text window or the WINNO parameter is specified as "CUR" then the cursor that is on the screen (potentially) will be visibly moved. If the WINNO parameter specifies a window that is not the current text window then the cursor position definition for that non-current window is altered only - no visible thing would happen on the screen.

If the specified window is deactivated or is undefined then nothing happens.

If the WINNO parameter is omitted then the current text window is assumed.

**Example:** `$TWHOME(5)$` **Returns:** nothing

### $TWIN$

_Text Window Status_

**Format:** `$TWIN(window)$`

**Syntax:**

```text
$TWIN(opt:WINDOW)$
          ──────
          CUR
          0-35
          ──────
      def=CUR
```

This Text Variable checks to see if the specified Text Window is activated, and returns "YES" if the specified text window is activated or returns "NO" if it is deactivated (eg, with the [`$DTW$`](#dtw) command for example). If the specified text window isn't defined, then this command returns a value of "NO". _(v2.A4)_

If you do not specify a window parameter, then this command refers to the current text window. If you do specify a window parameter then it can be a value from 0-35 to indicate a specific text window data table entry, or the value "CUR" to indicate the current text window. _(v2.A4)_

**Example:** `$TWIN(4)$` **Returns:** `YES`

### $TWW$

_Text Window Width_

**Format:** `$TWW(window,type,domain)$`

**Syntax:**

```text
$TWW(opt:WINDOW, opt:TYPE, opt:DOMAIN)$
         ──────      ────      ──────
         CUR         BOUND     WORLD
         0-35        CELL      DEVICE
         ──────      TEXT      COLS:TYPE=TEXT
     def=CUR         ────      ──────
                 def=TEXT  def=COLS:TYPE=TEXT
                               WORLD:TYPE<>TEXT
```

If no parameters are specified, then this command returns the width of the current text window in text cells (columns). You may specify a window number to indicate which text window you are inquiring about. The text window number parameters may be a value from 0-35 to reference a specific text window data table entry, or you may specify a keyword of "CUR" to indicate the current text window. If the specified text window doesn't exist, or is deactivated, then a value of "0" is returned. _(v2.A4)_

If you specify any parameters then they must be in a specific order. The first (already discussed) is the text window number, and if it is omitted, then the current text window is assumed. If you specify two parameters, then you must specify the window number parameter and the type parameter. The type parameter determines what type of information you are inquiring about. If you omit the type parameter then you are inquiring about the text window width itself, in actual text window cell sizes (eg, the window might respond with "5" to indicate that it is 5 columns wide. _(v2.A4)_

If you specify the type parameter, then you are directing this command to respond with information about a specific text window piece of information. The available type parameters are: _(v2.A4)_

| Keyword | Description                                        |
| ------- | -------------------------------------------------- |
| `BOUND` | Return the width of the bounding box in pixels     |
| `TEXT`  | Return the width of the display region             |
| `CELL`  | Return the width of a text cell in the text window |

The third and final parameter is only of use if the BOUND or CELL parameter is used, and it determines the domain of the pixel size returned by the text variable. The possible values of domain are: _(v2.A4)_

| Keyword | Description |
| --- | --- |
| `WORLD` | Return the value in world coordinates |
| `DEVICE` | Return the value in raw device pixel coordinates |
| `COLS` | Return width of area in text columns (valid only for a type of "TEXT". |

If the domain parameter is omitted, then it automatically defaults to WORLD (except for the TEXT type, which defaults to the height of the text window in lines of text). When a world coordinate value is returned, the actual "raw" dimensions are converted to the current environment's world coordinate system then returned to the host. If the domain is set to "DEVICE" then the value is returned in raw, device-level pixel coordinates. _(v2.A4)_

If the domain parameter is omitted, then it automatically defaults to WORLD (except for the TEXT type, which defaults to the width of the text window in columns of text). When a world coordinate value is returned, the actual "raw" dimensions are converted to the current environment's world coordinate system then returned to the host. If the domain is set to "DEVICE" then the value is returned in raw, device-level pixel coordinates. _(v2.A4)_

The BOUND parameter refers to the text window's bounding rectangle. It is the absolute exterior that the text window may extend to. It can also be thought of as a margin outside the actual text window where nothing can be displayed from the text window. See the RIP_EXTENDED_TEXT_WINDOW for more details about this bounding rectangle. _(v2.A4)_

The CELL parameter indicates that you are inquiring about the width of a character cell used in the text window. The width of a cell is based on the font number and resolution currently in use on the actual destination RIPscrip software package. Under 640x350, this was typically 8x8 for font #0 (ie, in RIPscrip 1.54). This may be different now at varying resolutions. So, if you are running at a resolution of 640x350 and you inquire about a text window's cell width for a text window using font #0, then it most likely will return a result of 8 pixels (in device coordinates). The actual value in world coordinates could vary well be different based on the size of the world coordinate resolution frame. _(v2.A4)_

The "TEXT" type indicates that you are requesting information about the text window sub-section of the text window definition. If the domain parameter is specified, then you are requesting the actual width of the text window display area in "columns of text". This can also be retrieved if the domain parameter is "COLS". This particular domain parameter known as "COLS" is only valid for a type parameter of "TEXT". Using it in any other context is considered a syntax error. If the domain parameter is specified, and it is set to "WORLD", then the pixel width of the display region is being requested in world coordinates. If domain is set to DEVICE, then you are requesting the width of the display region in actual hardware pixel coordinates. _(v2.A4)_

It should be noted that the bounding box of a text window can be obtained regardless of whether it is an old-style 1.54 text window created with RIP_TEXT_WINDOW or a new "resolution independent" text window defined via the RIP_EXTENDED_TEXT_WINDOW command. This can be because under the older text window concept, the bounding box is exactly as large as the text window itself, while under the extended text window, the text window will typically be inside the bounding rectangle. _(v2.A4)_

If you completely omit the type parameter, then you must omit the domain parameter. In this case, you are asking for the width of the text window in "text columns", not in pixels. _(v2.A4)_

**Example:** `$TWW$` — Width of current text window in columns of text. **Returns:** `5`

**Example:** `$TWW(CUR)$` — Width of current text window in columns of text. **Returns:** `5`

**Example:** `$TWW(5,BOUND)$` — Width of text window 5's bounding box in world coordinates. **Returns:** `800`

**Example:** `$TWW(5,BOUND,DEVICE)$` — Width of text window 5's bounding box in device pixel coordinates **Returns:** `400`

**Example:** `$TWW(CUR,CELL)` — Width of current text window's character cell in world coordinates. **Returns:** `16`

**Example:** `$TWW(5,CELL,DEVICE)$` — Width of text window 5's character cell in device pixel coordinates.

### $TWX0$

_Text Win Upper Left X Coordinates_

**Format:** `$TWX0(window,type,domain)$`

**Syntax:**

```text
$TWX0(opt:WINDOW, opt:TYPE, opt:DOMAIN)$
          ──────      ────      ──────
          CUR         BOUND     WORLD
          0-35        TEXT      DEVICE
          ──────      ────      ──────
      def=CUR     def=TEXT  def=WORLD
```

This variable returns upper-left X coordinate information about a text window. The exact type and nature of the returned information is determined by the parameters provided (if any). Without any parameters, this command returns the upper-left X coordinate of the text window in text coordinates (see below for a more detailed explanation). _(v2.A4)_

If the first parameter is provided, then it indicates which text window you requesting information on. It may be set to a numeric value from 0-35 to indicate a specific text window data table entry, or it may be "CUR" to indicate the current text window. _(v2.A4)_

If the specified text window doesn't exist, or is deactivated, then this command returns a value of "-1" to indicate failure. _(v2.A4)_

> **NOTE:** Under 1.54, this text variable used to return "0" to indicate that a text window was deactivated. This was inaccurately documented in previous RIPscrip releases because "0" could be a valid upper-left X coordinate.

If the second parameter, type, is provided, then the window number parameter must be present. The type parameter defines what type of information you are requesting about the text window. The avaiable keyword values for the type parameter are as follows: _(v2.A4)_

| Keyword | Description                                              |
| ------- | -------------------------------------------------------- |
| `BOUND` | Upper left X coordinate of the bounding rectangle        |
| `TEXT`  | Upper left X coordinate of text window display rectangle |

The BOUND parameter indicates that you are inquiring about the text window's bounding rectangle's upper left X coordinate. The result is returned in either world coordinates or in physical device coordinates based on the domain parameter (see below). _(v2.A4)_

The TEXT parameter indicates that you are inquiring about the actual text window's display area somewhere inside the bounding rectangle. The information returned is the upper-left X graphical coordinate of the text window display rectangle. It is returned either in world coordinates or in device pixel coordinates based on the domain parameter (see below). _(v2.A4)_

The third parameter is only valid if the type parameter is provided. It indicates the domain under which the type parameter data is returned to the host. The possible values for domain are: _(v2.A4)_

| Keyword  | Description                                       |
| -------- | ------------------------------------------------- |
| `WORLD`  | Return information in current world coordinates   |
| `DEVICE` | Return information in physical device coordinates |

The domain parameter determines what type of numbers are returned for the bounding box or the text window display rectangle. If this parameter is omitted, then the coordinates are returned in world coordinates. If the parameter is specified, then the result is in either world or device coordinates depending on the value of the parameter. _(v2.A4)_

If no type parameter is specified (as previously described), then the text window's upper-left X coordinate is returned in text coordinates. This is to maintain backward compability with older RIPscrip v1.54 related commands. In fact, this is not the best way of determining where the text window is on the screen - graphical coordinates are a much better method. If a text window is defined with the RIP_TEXT_WINDOW command where you specify the location of the text window solely on the basis of text coordinate X/Y data, then this form of this command will return a number indicating which X character cell the text window starts at. If the text window is defined using the RIP_EXTENDED_TEXT_WINDOW command, where the upper-left corner of the text window might not start on an even multiple of the window's cell size, then this command returns a value of "-1" to indicate that the desired request cannot be processed because the text window isn't the right kind of text window. In this manner, the value "-1" is used to indicate that an error has occurred with this command. _(v2.A4)_

**Example:** `$TWX0$` — Upper left X coordinate of the current text window in text coordinates **Returns:** `5`

**Example:** `$TWX0(CUR)$` — Upper left X coordinate of the current text window in text coordinates **Returns:** `5`

**Example:** `$TWX0(CUR,BOUND)$` — Upper left X coordinate of the current text window's bounding box in world coordinates. **Returns:** `100`

**Example:** `$TWX0(CUR,BOUND,WORLD)$` — Upper left X coordinate of the current text window's bounding box in world coordinates. **Returns:** `100`

**Example:** `$TWX0(CUR,BOUND,DEVICE)$` — Upper left X coordinate of the current text window's bounding box in device pixel coordinates. **Returns:** `50`

**Example:** `$TWX0(CUR,TEXT,DEVICE)$` — Upper left X coordinate of the text window's display box in device pixel coordinates. **Returns:** `55`

### $TWX1$

_Text Win Lower Right X Coordinate_

**Format:** `$TWX1(window,type,domain)$`

**Syntax:**

```text
$TWX1(opt:WINDOW, opt:TYPE, opt:DOMAIN)$
          ──────      ────      ──────
          CUR         BOUND     WORLD
          0-35        TEXT      DEVICE
          ──────      ────      COLS
      def=CUR     def=TEXT      ──────
                            def=COLS:TYPE=TEXT
                                WORLD:TYPE<>TEXT
```

This variable returns lower-right X coordinate information about a text window. The exact type and nature of the returned information is determined by the parameters provided (if any). Without any parameters, this command returns the lower-right X coordinate of the text window in text coordinates (see below for a more detailed explanation). _(v2.A4)_

If the first parameter is provided, then it indicates which text window you requesting information on. It may be set to a numeric value from 0-35 to indicate a specific text window data table entry, or it may be "CUR" to indicate the current text window. _(v2.A4)_

If the specified text window doesn't exist, or is deactivated, then this command returns a value of "-1" to indicate failure. _(v2.A4)_

> **NOTE:** Under 1.54, this text variable used to return "0" to indicate that a text window was deactivated. This was inaccurately documented in previous RIPscrip releases because "0" could be a valid lower-right X coordinate.

If the second parameter, type, is provided, then the window number parameter must be present. The type parameter defines what type of information you are requesting about the text window. The avaiable keyword values for the type parameter are as follows: _(v2.A4)_

| Keyword | Description                                               |
| ------- | --------------------------------------------------------- |
| `BOUND` | Lower right X coordinate of the bounding rectangle        |
| `TEXT`  | Lower right X coordinate of text window display rectangle |

The BOUND parameter indicates that you are inquiring about the text window's bounding rectangle's lower right X coordinate. The result is returned in either world coordinates or in physical device coordinates based on the domain parameter (see below). _(v2.A4)_

The TEXT parameter indicates that you are inquiring about the actual text window's display area somewhere inside the bounding rectangle. The information returned is the lower-right X graphical coordinate of the text window display rectangle. It is returned either in world coordinates or in device pixel coordinates based on the domain parameter (see below). _(v2.A4)_

The third parameter is only valid if the type parameter is provided. It indicates the domain under which the type parameter data is returned to the host. The possible values for domain are: _(v2.A4)_

| Keyword  | Description                                       |
| -------- | ------------------------------------------------- |
| `WORLD`  | Return information in current world coordinates   |
| `DEVICE` | Return information in physical device coordinates |

The domain parameter determines what type of numbers are returned for the bounding box or the text window display rectangle. If this parameter is omitted, then the coordinates are returned in world coordinates. If the parameter is specified, then the result is in either world or device coordinates depending on the value of the parameter. _(v2.A4)_

If no type parameter is specified (as previously described), then the text window's lower-right X coordinate is returned in text coordinates. This is to maintain backward compability with older RIPscrip v1.54 related commands. In fact, this is not the best way of determining where the text window is on the screen - graphical coordinates are a much better method. If a text window is defined with the RIP_TEXT_WINDOW command where you specify the location of the text window solely on the basis of text coordinate X/Y data, then this form of this command will return a number indicating which X character cell the text window starts at. If the text window is defined using the RIP_EXTENDED_TEXT_WINDOW command, where the lower-right corner of the text window might not start on an even multiple of the window's cell size, then this command returns a value of "-1" to indicate that the desired request cannot be processed because the text window isn't the right kind of text window. In this manner, the value "-1" is used to indicate that an error has occurred with this command. _(v2.A4)_

When graphical coordinate information is returned on the lower right X location, it is returned based on the resolution independent nature of rectangles in RIPscrip. This means that if the very last most device pixel of text window data is at X coordinate 299, then this command would return a value of 300 for the lower right X coordinate value. See the section on the THE MATHEMATICS OF GRAPHICS AND COORDINATES" for more details about why we use this coordinate convention. _(v2.A4)_

**Example:** `$TWX1$` — Lower right X coordinate of the current text window in text coordinates **Returns:** `5`

**Example:** `$TWX1(CUR)$` — Lower right X coordinate of the current text window in text coordinates **Returns:** `5`

**Example:** `$TWX1(CUR,BOUND)$` — Lower right X coordinate of the current text window's bounding box in world coordinates. **Returns:** `100`

**Example:** `$TWX1(CUR,BOUND,WORLD)$` — Lower right X coordinate of the current text window's bounding box in world coordinates. **Returns:** `100`

**Example:** `$TWX1(CUR,BOUND,DEVICE)$` — Lower right X coordinate of the current text window's bounding box in device pixel coordinates. **Returns:** `50`

**Example:** `$TWX1(CUR,TEXT,DEVICE)$` — Lower right X coordinate of the text window's display box in device pixel coordinates. **Returns:** `55`

### $TWY0$

_Text Win Upper Left Y Coordinate_

**Format:** `$TWY0(window,type,domain)$`

**Syntax:**

```text
$TWY0(opt:WINDOW, opt:TYPE, opt:DOMAIN)$
          ──────      ────      ──────
          CUR         BOUND     WORLD
          0-35        TEXT      DEVICE
          ──────      ────      ──────
      def=CUR     def=TEXT  def=WORLD
```

This command is identical in every way to the [`$TWX0$`](#twx0) text variable except that it returns information on the upper-left Y coordinate of the text window. The exact same parameters apply as in the `$TWX0$` text variable (see that command for more details). _(v2.A4)_

### $TWY1$

_Text Win Lower Right Y Coordinate_

**Format:** `$TWY1(window,type,domain)$`

**Syntax:**

```text
$TWY1(opt:WINDOW, opt:TYPE, opt:DOMAIN)$
          ──────      ────      ──────
          CUR         BOUND     WORLD
          0-35        TEXT      DEVICE
          ──────      ────      ──────
      def=CUR     def=TEXT  def=WORLD
```

This command is identical in every way to the [`$TWX1$`](#twx1) text variable except that it returns information on the lower-right Y coordinate of the text window. The exact same parameters apply as in the `$TWX1$` text variable, as do conventions for coordinates (see that command for more details). _(v2.A4)_

## Port/Viewport Related Text Variables

These text variables return information on, or manipulate ports or viewports. _(v2.A4)_

### $AVP$

_Activates a viewport definition_

_Added in RIPscrip v2.A1._

**Format:** `$AVP(port1,port2,...)$`

**Syntax:**

```text
$AVP(opt:PORT1, ...)$
         ─────
         ALL
         CUR
         0-35
         ─────
     def=CUR
```

This command does the exact opposite of the [`$DVP$`](#dvp) text variable which deactivates a viewport. When a viewport is activated, it no longer discards graphical commands that are sent to it. This does not make any visual changes on the screen immediately - they only affect subsequent graphical RIPscrip operations that are received when that viewport is selected as the current viewport. You may activate more then one viewport by specifying multiple viewport slot numbers.

You may specify one or more parameters to indicate that you wish to activate more than one viewport in the same `$AVP$` statement. If any one of them fails to match the proper parameters, then the entire command is considered a syntax error and none of the parameters are processed.

If the port you are trying to activate is protected, then it is not changed (ie, activated).

You may activate more then one viewport by specifying multiple viewport slot numbers (0-35). You may also specify the following:

| Keyword | Description                        |
| ------- | ---------------------------------- |
| `ALL`   | Activate all viewports at once     |
| `CUR`   | Activate only the current viewport |

**Example:** `$AVP(CUR)$` **Returns:** nothing

### $DVP$

_Deactivate a viewport definition_

_Added in RIPscrip v2.A4._

**Format:** `$DVP(port1,port2,...)$`

**Syntax:**

```text
$DVP(opt:PORT1, ...)$
         ─────
         ALL
         CUR
         0-35
         ─────
     def=CUR
```

This command deactivates a viewport, making no more RIPscrip graphics commands displayable in that viewport. When a viewport is deactivated, it no longer accepts graphical commands that are sent to it. This does not make any visual changes on the screen immediately; they only affect subsequent graphical RIPscrip operations that are received when that viewport is selected as the current viewport. You may deactivate more then one viewport by specifying multiple viewport slot numbers.

You may specify one or more parameters to indicate that you wish to deactivate more than one viewport in the same `$DVP$` statement. If any one of them fails to match the proper parameters, then the entire command is considered a syntax error and none of the parameters are processed.

If the port you are trying to deactivate is protected, then it is not changed (ie, deactivated).

You may deactivate more then one viewport by specifying multiple viewport slot numbers (0-35). You may also specify the following:

| Keyword | Description                          |
| ------- | ------------------------------------ |
| `ALL`   | Deactivate all viewports at once     |
| `CUR`   | Deactivate only the current viewport |

**Example:** `$DVP(CUR)$` **Returns:** nothing

### $EGW$

_Erase Graphics viewport_

**Format:** `$EGW(port1,port2,...)$`

**Syntax:**

```text
$EGW(opt:PORTNO, ...)$
         ──────
         ALL
         CUR
         0-35
         ──────
     def=CUR
```

This Active Text Variable erases the graphics viewport (much like a Reset Windows command does). This command is useful in Host Commands. When you click on a Mouse Field, it could erase the viewport window THEN transmit the remainder of the return string (if any) to the host. _(v2.A1)_

Remember, this may not clear the entire screen (although it will quite often since the Graphical Viewport is often full-screen).

This command does not require the slot parameter. If it is omitted, then the current viewport is cleared. If you specify a number (0-35) then the corresponding viewport slot number is erased. A special value of "ALL" may be used to erase all currently defined viewports. _(v2.A1)_

If any viewports specified belong to a port that isn't defined, or if the viewport is deactivated, then this command does nothing for that viewport parameter. _(v2.A4)_

You can specify mulitple viewport/port slots to erase to erase more than one withine one command if you wish. _(v2.A4)_

**Example:**

```text
$EGW(ALL)$ ... Erase all viewports
$EGW$      ... Erase current viewport
$EGW(5)$   ... Erase viewport slot #5
```

**Returns:** nothing

### $MVP$

_Maximizes viewport to full screen_

_Added in RIPscrip v2.A1._

**Format:** `$MVP(port1,port2,...)$`

**Syntax:**

```text
$MVP(opt:PORT1, ...)$
         ─────
         ALL
         CUR
         0-35
         ─────
     def=CUR
```

This command will take the specified viewport and make it full-screen regardless of its current settings. This does not affect any current graphics on the screen, only the internal viewport definition. The possible settings for the WHICH parameter are:

| Keyword  | Description                                       |
| -------- | ------------------------------------------------- |
| `ALL`    | Maximize all viewports at once                    |
| `CUR`    | Maximize only the current viewport                |
| `number` | Maximize a particular viewport slot number (0-35) |

To maximize more then one slot at the same time without maximizing all slots, simply specify more then one slot number parameter.

If any one of the parameters fails to match the proper parameters, then the entire command is considered a syntax error and none of the parameters are processed. If the underlying port of any viewport specified is not in use (ie, defined), deactivated or protected, then this command does nothing for that viewport.

**Example:** `$MVP(5)$` **Returns:** nothing

### $OFFSCREEN$

_Get offscreen bitmap port pixel data_

_Added in RIPscrip v2.A4._

**Format:** `$OFFSCREEN(mode)$`

**Syntax:**

```text
$OFFSCREEN(opt:MODE)$
               ────
               FREE
               USED
               TOTAL
               ────
           def=FREE
```

This command determines offscreen bitmap port pixel data. As described earlier on in this document, the [offscreen bitmap ports](02-drawing-ports.md) that you may defined cannot exceed the total number of device pixels used on your screen. In other words, if you have a screen that is 1000 pixels wide, by 500 tall, then you would have a total number of pixels available for offscreen bitmap ports of 1000x500 (or 500,000).

When a new port is defined as an offscreen bitmap port, it calculates the width and height of that port in hardware pixels and reduces the remaining offscreen pixels by that amount.

If you specify the MODE parameter of this command as "FREE", then you are requesting the total number of unused pixels available for use in offscreen bitmap ports. If MODE is set to "USED", then you are requesting the total number of offscreen pixels current "in use" by offscreen bitmap ports. Finally, if MODE is set to "TOTAL", then you are requesting the total number of offscreen pixels for all bitmap ports at once (FREE plus USED should equal TOTAL). If you omit the MODE parameter, then "FREE" is assumed.

**Example:** `$OFFSCREEN(TOTAL)$` **Returns:** `500000`

**Example:** `$OFFSCREEN(USED)$` **Returns:** `200000`

**Example:** `$OFFSCREEN(FREE)$` **Returns:** `300000`

### $PORTH$

_Height of port_

_Added in RIPscrip v2.A4._

**Format:** `$PORTH(portno,type,domain)$`

**Syntax:**

```text
$PORTW(opt:PORTNO, opt:DOMAIN)$
           ──────      ──────
           CUR         WORLD
           0-35        DEVICE
           ──────      ──────
       def=CUR     def=WORLD
```

This text variable is identical in nature to the [`$PORTW$`](#portw) text variable, except that it returns height information about the port instead of width.

### $PORTW$

_Width of port_

_Added in RIPscrip v2.A4._

**Format:** `$PORTW(portno,domain)$`

**Syntax:**

```text
$PORTW(opt:PORTNO, opt:DOMAIN)$
           ──────      ──────
           CUR         WORLD
           0-35        DEVICE
           ──────      ──────
       def=CUR     def=WORLD
```

This text variable returns the a piece of width information about the specified port.

If no parameters are specified, then you a request is being made for the width of the current drawing port itself.

If you specify the portno parameter, then you are indicating which port you are requesting information on. You may indicate "CUR" for the current port, or a value from 0-35 to indicate a specific port number.

The type parameter defines what type of port information you are requesting. If it is omitted then you can only specify the portno parameter and this will return the width of the port itself. If you specify the type parameter, then you may set this to "PORT" to explicitly indicate that you want information specifically on the port, or a value of "VIEW" to return information on the viewport.

If the type parameter is "VIEW", then you are specifically requesting the width of the viewport itself.

The third and final parameter is the domain parameter. This specifies what type of coordinate information you are requesting. By default, if this parameter is omitted then you are requesting your coordinate information in world coordinate values. Possible domain values are "WORLD" to explicitly request world coordinate values, or "DEVICE" for physical hardware device pixel coordinates.

If you request information on a port (or the viewport belonging to a port) where the port isn't defined (ie, not in use), or it is deactivated, then this variable returns a value of "0" to indicate this error condition.

**Example:** `$PORTW$` **Returns:** `100`

**Example:** `$PORTW(CUR)$` **Returns:** `100`

**Example:** `$PORTW(CUR,PORT)$` **Returns:** `100`

**Example:** `$PORTW(CUR,PORT,WORLD)$` **Returns:** `100`

**Example:** `$PORTW(CUR,PORT,DEVICE)$` **Returns:** `50`

### $PORTX0$

_Port's upper left X coordinate_

_Added in RIPscrip v2.A4._

**Format:** `$PORTX0(portno,type,domain)$`

**Syntax:**

```text
$PORTX0(opt:PORTNO, opt:TYPE, opt:DOMAIN)$
            ──────      ────      ──────
            CUR         PORT      WORLD
            0-35        VIEW      DEVICE
            ──────      ────      ──────
        def=CUR     def=PORT  def=WORLD
```

This text variable returns the a piece of upper-left X coordinate information about the specified port.

If no parameters are specified, then you a request is being made for the upper-left X coordinate of the current drawing port itself related to the current screen. This is only meaningful for "screen ports" where this X coordinate specifies an offset from the left border of the screen in pixels. For offscreen bitmapped ports, there is no upper-left X coordinate (or more precisely, an offset from some screen), so for offscreen ports this variation returns a value of "0". Port number 0 would always yield a value of "0" because its port is set to the full dimensions of the screen so there's no offset information for the upper-left X coordinate.

If you specify the portno parameter, then you are indicating which port you are requesting information on. You may indicate "CUR" for the current port, or a value from 0-35 to indicate a specific port number.

The type parameter defines what type of port information you are requesting. If it is omitted then you can only specify the portno parameter and this will return the upper-left X coordinate of the port itself. If you specify the type parameter, then you may set this to "PORT" to explicitly indicate that you want information specifically on the port, or a value of "VIEW" to return information on the viewport.

If the type parameter is "VIEW", then you are specifically requesting the upper-left X coordinate of the viewport in relation to the port's actual origin (the upper-left corner of the port).

The third and final parameter is the domain parameter. This specifies what type of coordinate information you are requesting. By default, if this parameter is omitted then you are requesting your coordinate information in world coordinate values. Possible domain values are "WORLD" to explicitly request world coordinate values, or "DEVICE" for physical hardware device pixel coordinates.

If you request information on a port (or the viewport belonging to a port) where the port isn't defined (ie, not in use), or it is deactivated, then this variable returns a value of "-1" to indicate this error condition.

**Example:** `$PORTX0$` **Returns:** `100`

**Example:** `$PORTX0(CUR)$` **Returns:** `100`

**Example:** `$PORTX0(CUR,PORT)$` **Returns:** `100`

**Example:** `$PORTX0(CUR,PORT,WORLD)$` **Returns:** `100`

**Example:** `$PORTX0(CUR,PORT,DEVICE)$` **Returns:** `50`

### $PORTX1$

_Port's lower right X coordinate_

_Added in RIPscrip v2.A4._

**Format:** `$PORTX1(portno,type,domain)$`

**Syntax:**

```text
$PORTX1(opt:PORTNO, opt:TYPE, opt:DOMAIN)$
            ──────      ────      ──────
            CUR         PORT      WORLD
            0-35        VIEW      DEVICE
            ──────      ────      ──────
        def=CUR     def=PORT  def=WORLD
```

This text variable returns the a piece of lower-right X coordinate information about the specified port.

If no parameters are specified, then you a request is being made for the lower-right X coordinate of the current drawing port itself related to the current screen. This is only meaningful for "screen ports" where this X coordinate specifies an offset from the left border of the screen in pixels. For offscreen bitmapped ports, there is no upper-left X coordinate (or more precisely, an offset from some screen), so for offscreen ports this variation returns a value that is equivalent to the width of the port. Port number 0 would always yield a value that is the width of the actual screen because its port is set to the full dimensions of the screen so there's no offset information for the upper-left X coordinate.

If you specify the portno parameter, then you are indicating which port you are requesting information on. You may indicate "CUR" for the current port, or a value from 0-35 to indicate a specific port number.

The type parameter defines what type of port information you are requesting. If it is omitted then you can only specify the portno parameter and this will return the lower-right X coordinate of the port itself. If you specify the type parameter, then you may set this to "PORT" to explicitly indicate that you want information specifically on the port, or a value of "VIEW" to return information on the viewport.

If the type parameter is "VIEW", then you are specifically requesting the lower-right X coordinate of the viewport in relation to the port's actual origin (the lower-right corner of the port).

The third and final parameter is the domain parameter. This specifies what type of coordinate information you are requesting. By default, if this parameter is omitted then you are requesting your coordinate information in world coordinate values. Possible domain values are "WORLD" to explicitly request world coordinate values, or "DEVICE" for physical hardware device pixel coordinates.

If you request information on a port (or the viewport belonging to a port) where the port isn't defined (ie, not in use), or it is deactivated, then this variable returns a value of "-1" to indicate this error condition.

Note that the lower-right X coordinate is non-inclusive. This means that the lower-right X coordinate is not actually part of the port's drawing area. It works exactly like the rectangle defining a filled rectangle.

**Example:** `$PORTX1$` **Returns:** `100`

**Example:** `$PORTX1(CUR)$` **Returns:** `100`

**Example:** `$PORTX1(CUR,PORT)$` **Returns:** `100`

**Example:** `$PORTX1(CUR,PORT,WORLD)$` **Returns:** `100`

**Example:** `$PORTX1(CUR,PORT,DEVICE)$` **Returns:** `50`

### $PORTY0$

_Port's upper left Y coordinate_

_Added in RIPscrip v2.A4._

**Syntax:**

```text
$PORTY0(opt:PORTNO, opt:TYPE, opt:DOMAIN)$
            ──────      ────      ──────
            CUR         PORT      WORLD
            0-35        VIEW      DEVICE
            ──────      ────      ──────
        def=CUR     def=PORT  def=WORLD
```

This command is identical in nature to the [`$PORTX0$`](#portx0) text variable, except that it returns upper-left Y coordinate information on the port.

### $PORTY1$

_Port's lower right Y coordinate_

_Added in RIPscrip v2.A4._

**Format:** `$PORTY1(viewno,coordtype)$`

**Syntax:**

```text
$PORTY1(opt:PORTNO, opt:TYPE, opt:DOMAIN)$
            ──────      ────      ──────
            CUR         PORT      WORLD
            0-35        VIEW      DEVICE
            ──────      ────      ──────
        def=CUR     def=PORT  def=WORLD
```

This command is identical in nature to the [`$PORTX1$`](#portx1) text variable, except that it returns lower-right Y coordinate information on the port.

---

[◀ Prev: Text Variables: General, Date/Time & Sound](17-text-variables-general.md) · [Contents](README.md) · [Next: Text Variables: Terminal & Reset ▶](19-text-variables-terminal.md)
