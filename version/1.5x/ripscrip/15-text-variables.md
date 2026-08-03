# Text Variables

[◀ Prev: Host Commands & Control Characters](14-host-commands.md) · [Contents](README.md) · [Next: Local RIPscrip File Playback ▶](16-local-playback.md)

---

A special feature of RIPterm allows it to understand what a Text Variable is.  A text variable is a piece of text that both RIPaint and RIPterm know something about.  For example, the Text Variable `$DATE$` is known to represent the current Date on your PC.  The host may ask your system what the values of one or more of these variables are, and if your terminal knows these particular Text Variables, it will tell the host.

There are three types of Text Variables.

- Built-In Text Variables that RIPscrip products will ALWAYS know about.  These include Text Variables like date and time that return a value

- Another type of built-in Text Variable are Active Text Variables, which perform an action, but return nothing to the host.  These include turning the status bar on/off, clearing the graphics screen, and playing some simple sounds, and many more.

- Then there are also User Text Variables that can contain a variety of information depending on what the user entered at the time the variable was created.  For example, the host might ask you what the contents of the `$FULL_NAME$` variable is, and if RIPterm doesn't know, it could pop-up a field on the screen and ask you about it.  From then on, RIPterm will remember that piece of information for the next time it is needed by a host.  (See [Text Variable Creation & Querying](19-text-variable-creation.md) for details on User Text Variables.)

You may use either the pre-defined Text Variables, or the User Text Variables at any place that allows Text Variables.

The following is a reference of all Built-In and Active Text Variables.

## Listing of the Pre-Defined Text Variables

### $RIPVER$

*RIPscrip version (e.g., "RIPSCRIP015300")*

This Text Variable returns a phrase which will identify a RIPscrip-compatible software package.  It is designed to be used by a host to detect what version of RIPscrip graphics your terminal can support as well as the type (brand) of RIPscrip terminal that is in use.  When this Text Variable is used, it will respond back with "RIPSCRIP" followed by the Version Number (e.g., "01.54"), followed by two digits identifying the Vendor of the terminal.  The first digit of the Vendor ID field is the Vendor Code (1=RIPterm).  The second digit is the Vendor's sub-version code identifying sub-versions of the software that still support the same RIPscrip software version.  Valid Vendor Codes are: *(v1.54)*

| Code | Vendor |
|------|--------|
| 0 | Generic RIPscrip terminal (vendor unknown) |
| 1 | RIPterm (from TeleGrafix Communications) |
| 2 | Qmodem Pro (from Mustang Software, Inc) |

See the section earlier in this document on ANSI sequences for a more robust description of the Vendor Codes and Auto-Sensing. *(v1.54)*

- **Example:** `$RIPVER$`
- **Returns:** `RIPSCRIP015300`

### $DATE$

*Date in short format*

This Text Variable returns the current date. in the format MM/DD/YY.

- **Example:** `$DATE$`
- **Returns:** `12/19/93`

### $MONTH$

*Month Name*

This Text Variable returns the full name of the current month.  It is not abbreviated (e.g., "November" instead of "Nov")

- **Example:** `$MONTH$`
- **Returns:** `December`

### $MONTHNUM$

*Month Number*

This Text Variable returns the number of the current month.  January=01 and December=12.

- **Example:** `$MONTHNUM$`
- **Returns:** `12`

### $DAY$

*Day of Month Number*

This Text Variable returns the current day of the month.  Possible values for this Variable are from 01-31.

- **Example:** `$DAY$`
- **Returns:** `05`

### $DOY$

*Day of year*

This Text Variable returns the number of days so far in the year.  A year has 365 days (except leap years which have 366).  `$DOY$` can return 001 - 366.

- **Example:** `$DOY$`
- **Returns:** `214`

### $YEAR$

*2 digit year*

This Text Variable returns the two-digit number of the current year.

- **Example:** `$YEAR$`
- **Returns:** `93`

### $FYEAR$

*4 digit year*

This Text Variable returns the four-digit number of the current year.

- **Example:** `$FYEAR$`
- **Returns:** `1993`

### $TIME$

*Time in standard format*

This Text Variable returns the time in military format (hours from 00 - 23).  The format is hours, minutes, and seconds separated by colons.  HH:MM:SS

- **Example:** `$TIME$`
- **Returns:** `18:09:33`

### $HOUR$

*Hour (format HH) - normal style*

This Text Variable returns the two digit number of the current hour.  This variable range from 01 - 12.  This does not use military format.

- **Example:** `$HOUR$`
- **Returns:** `11`

### $MHOUR$

*Hour (format HH) - military style*

This Text Variable returns a two-digit number of the current hour in military format.  This variable may range from 00 - 23.

- **Example:** `$MHOUR$`
- **Returns:** `17`

### $MIN$

*Minutes*

This Text Variable returns the two-digit number representing the current minutes in the hour.  Possible values for this variable are 00-59.

- **Example:** `$MIN$`
- **Returns:** `45`

### $SEC$

*Seconds*

This Text Variable returns a two-digit number representing the current seconds of the minute.  Possible values for this variable are 00-59.

- **Example:** `$SEC$`
- **Returns:** `59`

### $AMPM$

*Returns AM or PM depending on time*

This Text Variable returns a two-character value of either "AM" or "PM" depending on what time it is.

- **Example:** `$AMPM$`
- **Returns:** `PM`

### $DATETIME$

*Date and Time*

This Text Variable returns a combination date and time.  The format is somewhat different than standard time/date notation.  It is:

```text
DAY-OF-WEEK   MONTH   DAY-OF-MONTH  HH:MM:SS  YEAR
```

- **Example:** `$DATETIME$`
- **Returns:** `Sat Dec 19 14:38:50 1993`

> **NOTE:** This is the standard Unix date/time notation.

### $TIMEZONE$

*Time Zone or "NONE" if unknown*

This Text Variable returns a word/phrase that describes the time-zone the terminal is in.  This may be returned as anything like "PST" for Pacific Standard Time, "EST" for Eastern Standard Time, etc.  If the time zone is not set on your PC, this variable will respond with "NONE".

- **Example:** `$TIMEZONE$`
- **Returns:** `PST`

### $DOW$

*Day of week fully spelled out*

This Text Variable returns the current day of the week.  The name is fully spelled out.  Possible values are: Sunday, Monday, Tuesday, Wednesday, Thursday, Friday and Saturday.

- **Example:** `$DOW$`
- **Returns:** `Saturday`

### $ADOW$

*Abbreviated Day of Week*

This Text Variable returns the current day of the week in abbreviated form.  Possible values are: Sun, Mon, Tue, Wed, Thu, Fri and Sat.

- **Example:** `$ADOW$`
- **Returns:** `Mon`

### $WDAY$

*Day of Week*

This Text Variable returns a one-digit number representing the day of the week.  Possible values are 0-6, where 0=Sunday (the first day in the week).

- **Example:** `$WDAY$`
- **Returns:** `2`

### $WOY$

*Week of current year 00-53; Sunday=1st Day of Week*

This Text Variable returns a number from 00-53, representing the week in the year.  Even though there are 52 weeks in a year, a week might not begin exactly on the first day of the year, so a maximum value for this variable can be 53 under these circumstances.  For this variable, Sunday is considered to be the first day of the week.

- **Example:** `$WOY$`
- **Returns:** `32`

### $WOYM$

*Week of current year 00-53; Monday=1st Day of Week*

This Text Variable returns a number from 00-53, representing the week in the current year.  Even though there are 52 weeks in a year, a week might not begin exactly on the first day of the year, so a maximum value for this variable can be 53 under these circumstances.  For this variable, Monday is considered to be the first day of the week.

- **Example:** `$WOYM$`
- **Returns:** `32`

### $BEEP$

*Beep Sound (ala Ctrl-G)*

This Active Text Variable beeps the terminal, producing a `Ctrl-G` sound.

The C source code to play this sound is:

```c
sound(1000);   // the Hertz frequency to play
delay(75);     // millisecond delay
nosound();     // turn the sound off
delay(75);     // millisecond delay
```

- **Example:** `$BEEP$`
- **Returns:** nothing

### $BLIP$

*Blipping Sound (like a hitting a barrier)*

This Active Text Variable is like `$BEEP$`, except the sound is different.  It produces a barrier sound; like you're running into a wall.

The C source code to play this sound is:

```c
sound(50);     // the Hertz frequency to play
delay(25);     // millisecond delay
nosound();     // turn the sound off
delay(10);
```

- **Example:** `$BLIP$`
- **Returns:** nothing

### $MUSIC$

*Musical (cheerful) sound*

This Active Text Variable produces a cheerful sound, indicating success of an action.  This sound is used for successful downloads and dialed connections.

The C source code to play this sound is:

```c
for (i=0 ; i<4 ; i+=1)
{
     sound(1300);   delay(10);     // Hertz frequency to play
     sound(1200);   delay(10);     // millisecond delay
     sound(1100);   delay(10);
     sound(1000);   delay(10);
     sound(900);    delay(10);
     sound(800);    delay(10);
     sound(700);    delay(10);
     sound(850);    delay(10);
     sound(950);    delay(10);
}
nosound();                         // turn the sound off
```

- **Example:** `$MUSIC$`
- **Returns:** nothing

### $ALARM$

*Warning!  This sound indicates failure!*

This Active Text Variable produces a warning sound, indicating failure of an action.  This sound is used for aborted downloads.

The C source code to play this sound is:

```c
for (i=0 ; i<3 ; i+=1)
{
     sound(320);  delay(200);     // the Hertz frequency to play
     sound(160);  delay(425);     // millisecond delay
}
nosound();                        // turn the sound off
```

- **Example:** `$ALARM$`
- **Returns:** nothing

### $PHASER$

*Fire phasers!*

This Active Text Variable produces a sound like firing your energy weapons in a game.  Now you too can blast away with the best of them.  Trivia question:  What does phaser stand for?  See `$REVPHASERS$` for the answers.

The C source code to play this sound is:

```c
for (i=2500 ; i>=50 ; i-=20)
{
     sound(i);               // the Hertz frequency to play
     delay(2);               // millisecond delay
}
nosound();                   // turn the sound off
```

- **Example:** `$PHASER$`
- **Returns:** nothing

### $REVPHASER$

*Fire phasers!*

This Active Text Variable produces a sound like firing your energy weapons in a game.  Like `$PHASER$` makes an ascending tone, `$REVPHASER$` makes a descending tone.  Answer to trivia question in `$PHASER$`: Phaser stands for PHoton Amplification by Stimulated Emission of Radiation.  Sound familiar?  Laser is Light Amplification by Stimulated Emission of Radiation, and Maser is Microwave Amplification by Stimulated Emission of Radiation.

The C source code to play this sound is:

```c
for (i=50 ; i<=2500 ; i+=20)
{
     sound(i);               // the Hertz frequency to play
     delay(2);               // millisecond delay
}
nosound();                   // turn the sound off
```

- **Example:** `$REVPHASER$`
- **Returns:** nothing

### $X$

*X Mouse location*

This Text Variable returns the current X coordinate of the mouse pointer.  This can be used interactively (for example, by on-line games) to determine the location of the mouse pointer.  Only the X value of the mouse (X,Y) is returned.  The value is 0000-9999 depending on what the current position is.

- **Example:** `$X$`
- **Returns:** `0523`

### $Y$

*Y Mouse location*

This Text Variable returns the current Y coordinate of the mouse pointer.  This can be used interactively (for example, by on-line games) to determine the location of the mouse pointer.  Only the Y value of the Mouse (X,Y) is returned.  The value is 0000-9999 depending on what the current position is.

- **Example:** `$Y$`
- **Returns:** `0244`

### $XY$

*X/Y Mouse Location*

This Text Variable returns both the X and Y coordinates of the mouse pointer.  A colon (:) separates the two values.  The X and Y values may range from 0000-9999.  The format that this value uses is: `XXXX:YYYY`

- **Example:** `$XY$`
- **Returns:** `0297:0321`

### $XYM$

*X, Y & button status*

This Text Variable returns the X and Y coordinates of the mouse pointer, and which mouse buttons are pressed (if any).  A colon (:) separates the three values.  The X and Y values may range from 0000-9999.  LMR stands for Left/Middle/Right.  If any of these buttons are depressed (clicked), then the corresponding position will contain a 1.  If a button is NOT depressed, then it will contain a 0.  The format that this value uses is:  `XXXX:YYYY:LMR`

This means that the (X,Y) location of the cursor is (0123,0297), and that the Left and Middle buttons are depressed, but that the Right Mouse Button is not depressed.

- **Example:** `$XYM$`
- **Returns:** `0123:0297:110`

### $M$

*Mouse Button Status: LMR*

This Text Variable returns a 3-character code representing the status of each mouse button.  This variable works with two button and three button mice.  The format of the code is LMR where L=Left, M=Middle (if any), and R=Right.  If any button is clicked, the code for that button is "1".  If the button is not depressed, it is "0".  "100" would mean the left mouse button is depressed, but none of the others are.

- **Example:** `$M$`
- **Returns:** `001`

### $MSTAT$

*Mouse Status*

This Text Variable returns a "YES" if there is a mouse installed on the RIPterm computer.  If no mouse is installed, this variable returns "NO".

- **Example:** `$MSTAT$`
- **Returns:** `YES`

### $RESET$

*Performs RIP_RESET_WINDOWS (Identical to `!|*`)*

This Active Text Variable resets and clears the graphics screen, resets the text window to full screen and clears it, resets the color palette, deletes all mouse fields, and clears the clipboard.  (See [RIP_RESET_WINDOWS](04-window-commands.md#rip_reset_windows).)

- **Example:** `$RESET$`
- **Returns:** nothing

### $SAVEALL$

*Save all screen attributes*

This Active Text Variable saves the Text Windows coordinates, save the contents of the clipboard, saves all mouse fields, and saves the contents of the entire screen.  It is the same as doing a "`$STW$ $SCB$ $SMF$ $SAVE$`".

- **Example:** `$SAVEALL$`
- **Returns:** nothing

### $RESTOREALL$

*Restore all screen attributes*

This Active Text Variable restores the Text Windows coordinates, restores the contents of the clipboard, restores all mouse fields, and restores the contents of the screen.  It is equal to "`$RTW$ $RCB$ $RMF$ $RESTORE$`".

- **Example:** `$RESTOREALL$`
- **Returns:** nothing

### $EGW$

*Erase Graphics Window*

This Active Text Variable erases the graphics window (much like a Reset Windows command does). This command is useful in [Host Commands](18-host-command-templates.md).  When you click on a [Mouse Field](09-mouse-fields.md), it could erase the graphics window THEN transmit the remainder of the return string (if any) to the host.

Remember, this may not clear the entire screen (although it will quite often since the Graphical Viewport is often full-screen).

- **Example:** `$EGW$`
- **Returns:** nothing

### $SAVE$ and $SAVEx$

*Save graphics screen*

The Active Text Variable `$SAVE$` saves the contents of the entire graphics screen to a disk file called RIPTERM.SAV.  No Mouse Fields, Text Window locations or Clipboard data are saved - just the graphics screen.  The entire 640x350 region is saved to disk.

If you choose the SAVE0 through SAVE9 variations, the filename that is saved to files RIPTERM0.SAV through RIPTERM9.SAV, allowing you to have multiple screens saved simultaneously.

In addition to the Graphical data that is currently on-screen, the current Graphical Viewport settings are saved as well so that when a restore is done, the viewport will be properly restored as well. *(v1.54)*

If you wish to save the entire state of the RIPterm system, use `$SAVEALL$`.

- **Example:** `$SAVE7$`
- **Returns:** nothing

### $RESTORE$ and $RESTOREx$

*Restore graphics screen*

The Active Text Variable `$RESTORE$` reads the saved file RIPTERM.SAV in from disk and restores the graphics as they were originally saved with the `$SAVE$` command.  Only the graphics screen is restored, not the Clipboard, Mouse Fields or Text Window settings.

If you choose the RESTORE0 through RESTORE9 variations, the filename that is restored are RIPTERM0.SAV through RIPTERM9.SAV, allowing you to restore from up to ten different saved files.  A slight difference from `$RESTORE$` is that `$RESTORE0$` - `$RESTORE9$` delete the file after the graphics screen is restored.

When the graphics screen is restored, the Graphics Viewport settings that were in effect when the screen was saved will be restored as well. *(v1.54)*

To restore the entire context of the graphics environment `$RESTALL$`.

- **Example:** `$RESTORE3$`
- **Returns:** nothing

### $SMF$

*Save Mouse Fields*

This Active Text Variable saves all defined Mouse Fields and Mouse Buttons to a temporary file for later retrieval.  This is designed especially for the graphical designer who wishes to pop-up a dialog box on the screen with one or more mouse fields, and when finished, to restore the screen and original mouse fields.  This command is intended to be used with the Restore Mouse Fields text variable `$RMF$`.

- **Example:** `$SMF$`
- **Returns:** nothing

### $RMF$

*Restore Mouse Fields*

This Active Text Variable restores any Mouse Fields saved with `$SMF$`.  You may have only one set of mouse fields saved at once.  If no mouse fields were saved, or if the number of fields saved is 0, then no mouse fields are  active.

> **NOTE:** You may restore Mouse Fields more than once is you wish. In other words, if you do a `$SMF$` command, you may execute `$RMF$` one or more times.

- **Example:** `$RMF$`
- **Returns:** nothing

### $MKILL$

*Kill Mouse Fields*

This Active Text Variable deletes all defined Mouse Fields exactly like [RIP_KILL_MOUSE_FIELDS](09-mouse-fields.md#rip_kill_mouse_fields) does.  The benefit is when the user clicks on a Mouse Fields or [Button](12-buttons.md), the Mouse Fields are removed, but the graphics remain on the screen.  The fields could be subsequently re-defined quickly and easily without having to re-transmit an identical menu over again.

- **Example:** `$MKILL$`
- **Returns:** nothing

### $ETW$

*Erase Text Window*

This Active Text Variable erases the Text Window (like a clear screen code does).  This command is useful in Host Commands when you click on a Mouse Field, it could erase the text window THEN transmit the remainder of the Host Command (if any).

- **Example:** `$ETW$`
- **Returns:** nothing

### $DTW$

*Disable Text Window*

This Active Text Variable disables the Text Window (preventing any received text from showing up on screen).  This command is useful in Host Commands when you click on a Mouse Field, it would halt any further output to the text window.

- **Example:** `$DTW$`
- **Returns:** nothing

### $STW$

*Save Text Window information*

This Active Text Variable stores all of the text window settings.  The window's X/Y dimensions are preserved, as is the current cursor location, ANSI attributes, cursor ON/OFF status and the vertical scrolling margins.  Even the current System Font is saved (if necessary).

> **NOTE:** The contents of the Text Window are NOT saved.

- **Example:** `$STW$`
- **Returns:** nothing

### $RTW$

*Restore Text Window information*

This Active Text Variable restores the Text Window to the settings active when `$STW$` (Save Text Window) was executed. The current cursor location, ANSI attributes, cursor ON/OFF status, vertical scrolling margins, and the System Font are restored.

> **NOTE:** The text contents of the window are not restored.

- **Example:** `$RTW$`
- **Returns:** nothing

### $TWIN$

*Text  Window Status*

This Text Variable checks to see if a Text Window exists, and returns YES if there is a Text Window, or returns NO if there is no Text Window or the Text Window has been disabled (with `$DTW$`).

- **Example:** `$TWIN$`
- **Returns:** `YES`

### $TWFONT$

*Active Text Font*

This Text Variable returns which of the five Text Window Fonts is active, or 0 (zero) if there is no Text Window.

| Value | Font |
|-------|------|
| 0 | No Text Window |
| 1 | 80x43 font |
| 2 | 91x43 MicroANSI font |
| 3 | 80x25 font |
| 4 | 91x25 MicroANSI font |
| 5 | 40x25 font |

- **Example:** `$TWFONT$`
- **Returns:** `1`

### $TWH$

*Text Window Height*

This Text Variable returns the height of the Text Window, or 0 (zero) if there is no Text Window.  If a text window exists, the minimum value is 1 and the maximum value is 43.  This may increase in the future.

- **Example:** `$TWH$`
- **Returns:** `25`

### $TWW$

*Text Window Width*

This Text Variable returns the width of the Text Window, or 0 (zero) if there is no Text Window.  If a text window exists, the minimum value is 1 and the maximum value is 91.  This may increase in the future.

- **Example:** `$TWW$`
- **Returns:** `80`

### $TWX0$

*Text Window Upper Left X Coordinate*

This Text Variable is the X coordinate of the upper left corner of the Text Window.  The coordinates given are relative to the upper left of the screen.  The values are given in cells, which is a block the size of one character in the currently selected font.  A good analogy is that a cell is equivalent to a square on a sheet of graph paper.  The cell size may change depending on the font used, but the relative position for that font remains constant.  If there is no Text Window, this returns 0 (zero).  However, note that 0 (zero) is also a valid coordinate.  Use `$TWIN$` to determine if there is a Text Window.

- **Example:** `$TWX0$`
- **Returns:** `0`

### $TWY0$

*Text Window Upper Left Y Coordinate*

This Text Variable is the Y coordinate of the upper left corner of the Text Window.  See `$TWX0$` for an explanation of the coordinate system.

- **Example:** `$TWY0$`
- **Returns:** `40`

### $TWX1$

*Text Window Lower Right X Coordinate*

This Text Variable is the X coordinate of the lower right corner of the Text Window.  See `$TWX0$` for an explanation of the coordinate system.

- **Example:** `$TWX1$`
- **Returns:** `80`

### $TWY1$

*Text Window Lower Right Y Coordinate*

This Text Variable is the Y coordinate of the lower right corner of the Text Window.  See `$TWX0$` for an explanation of the coordinate system.

- **Example:** `$TWY1$`
- **Returns:** `43`

### $CURX$

*Text Cursor X Coordinate*

This Text Variable is the X coordinate of the text cursor in the Text Window, relative to the upper left of the Text Window.  See `$TWX0$` for an explanation of the coordinate system.

- **Example:** `$CURX$`
- **Returns:** `2`

### $CURY$

*Text Cursor Y Coordinate*

This Text Variable is the Y coordinate of the text cursor in the Text Window, relative to the upper left of the Text Window.  See `$TWX0$` for an explanation of the coordinate system.

- **Example:** `$CURY$`
- **Returns:** `5`

### $CON$

*Enable the Text Cursor*

This Active Text Variable turns on the text cursor.

- **Example:** `$CON$`
- **Returns:** nothing

### $COFF$

*Disable the Text Cursor*

This Active Text Variable turns off the text cursor.  This is automatically reset when a Reset Windows command is received.

- **Example:** `$COFF$`
- **Returns:** nothing

### $CURSOR$

*Text Cursor Status*

This Text Variable returns YES if the Text Cursor is enabled, and NO if the Text Cursor is disabled.  If there no Text Window, it returns NO.

- **Example:** `$CURSOR$`
- **Returns:** `YES`

### $SCB$

*Save Clipboard*

This Active Text Variable saves the Clipboard to disk for later retrieval by a [Query](13-advanced-commands.md#rip_query) or Host Command.  If the clipboard is empty, the temporary file is deleted so Restore Clipboard knows there shouldn't be a clipboard active.

- **Example:** `$SCB$`
- **Returns:** nothing

### $RCB$

*Restore Clipboard*

This Active Text Variable restores the Clipboard from the temporary disk file called RIPCLIB.BRD.  This file is created by `$SCB$` (Save Clipboard).  Not only are the clipboard contents saved, but so is the last clipboard location, so Paste Clipboard (`$PCB$`) restores the clipboard's contents AND location.

- **Example:** `$RCB$`
- **Returns:** nothing

### $PCB$

*Paste Clipboard at last location*

This Active Text Variable pastes the clipboard at the last location it was clipped from. This also works with icons.  The last location taken used is the location the icon was stamped when it was first loaded.  This text variable is useful if you want to pop up a dialog box (saving the previous area behind the dialog onto the clipboard), and when the user clicks on the "OK" button, restoring the screen contents (by using a `$PCB$` in the host command string).

When the Clipboard is pasted back onto the screen, it adheres to the Graphical Viewport that was in effect when the clipboard was clipped initially.  In other words, the clipboard will be put in the same location regardless of where it came from. *(v1.54)*

- **Example:** `$PCB$`
- **Returns:** nothing

### $STATBAR$

*Status Bar Status*

This Text Variable returns YES if the Status Bar is visible in the terminal.  If the Status Bar is not visible, then NO is returned.

- **Example:** `$STATBAR$`
- **Returns:** `YES`

### $SBARON$

*Turn ON the Status Bar*

This Active Text Variable turns ON the Status Bar in the terminal.

- **Example:** `$SBARON$`
- **Returns:** nothing

### $SBAROFF$

*Turn OFF the Status Bar*

This Active Text Variable turns OFF the Status Bar in the terminal.

- **Example:** `$SBAROFF$`
- **Returns:** nothing

### $VT102ON$

*Turn VT-102 keyboard mode ON*

This Active Text Variable enables the VT-102 keystrokes ability.  This makes the following keystrokes send something to the host:

```text
          F1 - ESC [ M
          F2 - ESC [ N
          F3 - ESC [ O
          F4 - ESC [ P
          F5 - ESC [ Q
          F6 - ESC [ R
          F7 - ESC [ S
          F8 - ESC [ T
          F9 - ESC [ U
         F10 - ESC [ V
        PGUP - ESC [ I
        PGDN - ESC [ G
        HOME - ESC [ H
         END - ESC [ F
      INSERT - ESC [ L
   CURSOR UP - ESC [ A
   CURSOR DN - ESC [ B
 CURSOR LEFT - ESC [ C
CURSOR RIGHT - ESC [ D
```

This text variable puts the terminal in VT-102 mode automatically, making it simpler for the user.

- **Example:** `$VT102ON$`
- **Returns:** nothing

### $VT102OFF$

*Turn VT-102 keyboard mode OFF*

This Active Text Variable disables the VT-102 keystrokes mode, returning your keyboard to the standard keyboard operation.

- **Example:** `$VT102OFF$`
- **Returns:** nothing

### $DWAYON$

*Turn Doorway Mode ON*

This Active Text Variable enables Doorway Mode.  This is intended to be used by a Host system that wishes to take advantage of the Doorway mode available in Marshall Dudley's Doorway (tm) software package.

- **Example:** `$DWAYON$`
- **Returns:** nothing

### $DWAYOFF$

*Turn Doorway Mode OFF*

This Active Text Variable disables the Doorway keyboard mode.  This will return the keyboard to normal operation.

- **Example:** `$DWAYOFF$`
- **Returns:** nothing

### $HKEYON$

*Enable Button Hotkeys*

This Active Text Variable turns on use of Button Hotkeys.  When enabled, if the user presses a key associated with a button, it is selected just as if it were clicked.  The Scroll Lock light on the keyboard is turned on.

- **Example:** `$HKEYON$`
- **Returns:** nothing

### $HKEYOFF$

*Disable Button Hotkeys*

This Active Text Variable turns off Button Hotkeys.  This should be done when entering a full-screen editor, or any part of the system where the user is entering a string of text.  This is to prevent the user from accidentally selecting a button when typing in text.

- **Example:** `$HKEYOFF$`
- **Returns:** nothing

### $TABON$

*Enable TAB key Mouse Field select*

This Active Text Variable turns on the use of the TAB key to jump from one defined Mouse or Button Field to another.

- **Example:** `$TABON$`
- **Returns:** nothing

### $TABOFF$

*Disable TAB key Mouse Field select*

This Active Text Variable turns off the use of the TAB key to jump from one defined Mouse or Button Field to another.  If this command is received when a field is highlighted, it is deselected.  This should be done when entering a full-screen editor so that the user can use the TAB key as a TAB, not a Mouse Field selector.

- **Example:** `$TABOFF$`
- **Returns:** nothing

### $APP0$ - $APP9$

*External Application Call*

This Active Text Variable instructs the terminal to execute an external application.  By recommendation, `$APP0$` is the user's text editor.  There are ten external application slots available, numbered 0 - 9.  These are defined in the External menu in RIPterm.

- **Example:** `$APP1$`
- **Returns:** nothing

---

[◀ Prev: Host Commands & Control Characters](14-host-commands.md) · [Contents](README.md) · [Next: Local RIPscrip File Playback ▶](16-local-playback.md)
