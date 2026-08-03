# Protocol Definition & Syntax

[◀ Prev: Color, Audio & Text Windows](06-color-audio-text.md) · [Contents](README.md) · [Next: Level-0 Commands (A–F) ▶](08-level-0-commands-a-f.md)

## RIPscrip Protocol Definition

The following sub-sections define the actual structure of the RIPscrip graphical language. The topic of Host Commands and Text Variables is covered in a section devoted specifically to that topic. *(v2.A4)*

## ANSI Sequences (Auto-Sensing)

RIPscrip predominantly uses non-ANSI command sequences. In a couple of situations though, an ANSI-like sequence is allowed to perform a specific function. There are currently four separate escape sequences defined in the RIPscrip protocol to perform various actions. They are as follows:

`ESC[!` — Query RIPscrip version number. RIPterm will respond with `RIPSCRIPxxyyvs` where "xx" is equal to the major version number (zero padded), "yy" is equal to the minor version number (zero padded), "v" is the vendor code of the terminal program (see below), and "s" is the vendor's sub-version code for their software. For v1.54, the returned sequence for RIPterm (Vendor Code "1") would be `RIPSCRIP015410`. Another example, v1.23 with a Vendor Code of "2" and a sub-revision code of "5" would return `RIPSCRIP012325`. *(v1.54)*

Valid Vendor Codes are:

| CODE | VENDOR |
|------|--------|
| 0 | Generic RIPscrip terminal (vendor unknown) |
| 1 | RIPterm (from TeleGrafix Communications) |
| 2 | Qmodem Pro (from Mustang Software, Inc) |
| 3 | deltaComm Development (Telix) *(v2.A1)* |
| 4 | Qmodem Pro for Windows (Mustang Software) *(v2.A1)* |

This ANSI sequence is often used for "Auto-Sensing" if a RIPscrip terminal exists on the remote end of the connection. If a non-RIPscrip terminal receives this ANSI sequence, it will ignore it. *(v1.54)*

> **NOTE:** This method of vendor determination is obsolete in RIPscrip 2.0. See the section Pre-Defined Text variable for Vendor specific text variables which can be used more generically to determine a specific terminal vendor (ie, `$TERMINFO$()`). *(v2.A1, revised v2.A4)*
>
> You still use this escape sequences to detect RIPscrip capability, but to determine the specific terminal program or manufacturer, use the `$TERMINFO()$` text variable. To determine specific capabilities of the terminal, use the `$IFS()$` text variable (Is Function Supported). To determine the current language that the terminal is running in, use the `$LANGUAGE$` text variable (see the text variable section later for more details). *(v2.A3)*

`ESC[0!` — Same as `ESC [ !` (see above)

`ESC[1!` — Disables all RIPscrip processing. Any RIPscrip sequences are interpreted as raw text.

`ESC[2!` — Enabled RIPscrip processing. Any RIPscrip sequences will be parsed and processed.

## RIPscrip Protocol - Syntax and General Structure

RIPscrip is organized into 10 levels of graphical commands (low Level-0 to high Level-9). Level-0 commands are the building blocks of RIPscrip. The basic graphics primitives of the system are all Level-0, including the commands Line, Rectangle, Circle, Color, Font, etc. Each level of RIPscrip gets progressively higher-level in concept. For example, Level-1 commands use Mouse Regions, Icons, and Formatted Text Regions.

The basic syntax rules are as follows:

1. A RIPscrip command line starts at the beginning of a line of text. A RIPscrip command line moved to the middle of a line of text is treated as literal text. This prevents people inserting mischievous things in teleconference messages, or similar pranks. The only exceptions to this rule is stated below under item 6, "continuation of long lines", and item 12 "alternate RIPscrip starting sequences".

2. A RIPscrip command line begins with an exclamation mark `!` (ASCII code 33 decimal).

3. Every RIPscrip command is preceded by the universal RIPscrip delimiter, vertical-bar `|` (ASCII code 124 decimal).

4. Individual RIPscrip commands may be combined on the same line providing they are separated by the vertical bar delimiter. There are some exceptions to this ([RIP_ENTER_BLOCK_MODE](13-level-3-9-commands.md#rip_enter_block_mode), and a few others). See individual command descriptions for exceptions. *(v2.A1)*

5. RIPscrip commands or command lines may be split across multiple lines with a backslash `\` (ASCII code 92 decimal) just before each split. This helps RIPscrip commands conform to right margins and escape word wrapping.

   An example:

   ```text
   !|c02|L02030405|P0901020102010201020102\
   0102010201020102
   ```

6. RIPscrip must allow for normal text to be intermixed with RIPscrip commands. If unrecognized text appears after a RIPscrip command, on the same line, the text is ignored (the command is not ignored). A line that does not begin with `!|` is considered raw text and is routed to the TTY text window (see "8" below).

7. RIPscrip makes provisions for multiple Graphical Windows and multiple Text Windows. A Graphical Window is where all RIPscrip graphics appear. A Text Window is where raw text appears. Raw Text includes ANSI color and cursor movement codes (a subset of VT-100 terminal emulation), and possibly other terminal emulations like VT-100, VT-102, VT-220, etc.

8. The vertical bar (`|`) of a RIPscrip command can be followed by a level number. If the 1st character after (`|`) is a numeric digit (1-9), then that's the RIPscrip Command Level. If the very 1st character is NOT a digit 1-9, then it is the command type character and the command is a Level-0 command. If the 1st character is a digit 1-9, and the second character is also a digit, then that defines a sub-level of a RIPscrip level. For example:

   ```text
         !|L     RIPscrip Level-0 Command "L"
        !|1L     RIPscrip Level-1 Command "L"
       !|15L     RIPscrip Level-1, sub-level 5 Command "L"
   ```

   Each of the above examples are unique commands not to be confused with each other. You may continue the sub-levels up to a maximum level of 9 (e.g., `!|123456789<cmd>`").

9. Every RIPscrip command includes a command type character. In Level-0 commands, this character immediately follows the vertical bar. At all other levels, it follows the level digits. The command type character may be any of the following characters:

   ```text
   ABCDEFGHIJKLMNOPQRSTUVWXYZ
   abcdefghijklmnopqrstuvwxyz
   @#$&*()[]{}<>:;'",.?-_=+!
   ```

10. Following the command type character are 0 or more parameters. If the command requires a text-string, it is always the LAST parameter. Numeric parameters DO NOT have any delimiters (commas, dashes, spaces, etc.). A variable width numeric parameter may be used as the last parameter. This allows for maximum efficiency.

    If a RIPscrip command does not have a string parameter, but does have numeric parameters, the last parameter may be shortened to get a bit more efficiency out of it. If the last parameter is more than one digit long and the left-most digits are zeros, they may be omitted. At least one digit must remain though after all of the shortening is done (eg, "0001" can be shortened to "1" but "0000" cannot be shortened to "", it can only be shortened to "0"). *(v2.A3)*

    For example, the [RIP_COLOR](08-level-0-commands-a-f.md#rip_color) command, used to set the current drawing color, requires one parameter - a color number. If we were going to set color 1 (usually dark blue), then the command would appear formally as follows: *(v2.A3)*

    ```text
    !|c01
    ```

    Under this shortening clause, you could omit the "0" and make the command appear like this: *(v2.A3)*

    ```text
    !|c1
    ```

    This allows you to shave one byte off of the command. If you were trying to set color number zero (normally black), then you could still shorten the parameter, but you still have to leave at least one digit there (0), so the command would appear like this: *(v2.A3)*

    ```text
    !|c0
    ```

    There is an additional optimization that can be used for commands that have no string parameter, and have a reserved parameter as the LAST parameter for that command. In this case, the reserved parameter may be omitted completely, and the value should be assumed to be zero (`0'). Please take note that the previous paragraphs still apply! You can omit the reserved parameter AND shorten the last transmitted parameter so as to squeeze out as many unnecessary characters from the command as possible. *(v2.A2)*

    Numbers are normally represented in base-36 (unless altered by a RIPscrip command). This compacts numbers down to roughly 3/5 of their decimal form. This numbering system, technically called "Hexa-Tri-Decimal" or "MegaNums" is unlike Hexadecimal which uses 0-9 and A-F. MegaNums take advantage of the entire alphabet, using characters 0-9 and A-Z. *(v2.A0)*

    See the next section about Base Math variations. *(v2.A0)*

11. An exclamation mark (`!`) or vertical bar (`|`) character can appear in a RIPscrip text parameter by preceding it with a backslash (`\`). This is known as "quoting" a special character. A literal backslash is represented with a double-backslash (`\\`). Also, in some situations you need to quote specific characters so that they are not misinterpretted. For example, in a button host string, the characters `[`, `]`, `(`, and `$` are used as special host command characters and will need to be quoted (eg, `\[`, `\]`, `\(` and `\$` respectively). See rule #14 below for more details on quoting these special sequences. *(v2.A3)*

12. A RIPscrip sequence CAN begin in a column other than column #0, if the exclamation mark prefix is replaced with a Ctrl-A (Start Of Header [SOH] - ASCII character 1) character, or Ctrl-B (STX - ASCII character 2) character. Since 99.9% of all BBS' do not allow users to enter most control characters, users will be unable to begin RIPscrip sequences in the middle of a command line. Only the host should be able to do this. This prevents people from cluttering teleconference, or other areas of a host with spurious RIPscrip sequences.

    Let's take an example, where we have a piece of raw text and then a RIPscrip command immediately following the text before the carriage return (in this example `<STX>` stands for the STX character): *(v2.A3)*

    ```text
    This is raw text<STX>|c01|@1010hello world
    ```

    In the above example, the text "This is raw text" would be sent to the active text window, then the RIPscrip sequences would be processed - set the color to blue (01), then print the text "hello world" on the screen at (10,10) in meganums or (36,36) in decimal. *(v2.A3)*

13. If the last couple of bytes on a RIPscrip text line are backslashes, special care must be taken to make sure that they are not interpreted as a line-continuation. If a literal backslash is desired as the last position on the line, it must be specified as a double-backslash (eg, `\\`). If a line-continuation is used then there would have to be three backslashes used on the line as in the following example: *(v1.54)*

    ```text
    !|@2233this is a text line with a literal \\\
    used in the message
    ```

    This would text output at (22,33) [meganum] the message: *(v1.54)*

    ```text
    this is a text line with a literal \used in the message
    ```

14. Some character sequences in text parameter strings can be misinterpreted as RIPscrip command sequences (eg, `$`, `<>`, `((`, `))`, `[`, `]`, `[]`, etc). To avoid syntax confusion these sequences can be "escaped" to indicate that they are actually literal sequences, not to be confused with a RIPscrip command sequence identifier. For example, the sequence: *(v2.A1)*

    ```text
    $Name$
    ```

    would normally be interpreted as a RIPscrip text variable that requires the user's response. If however, you wanted this sequence to be interpretted as literal text, you should perform the following escape sequences:

    ```text
    \$Name\$
    ```

    This makes it so a RIPscrip terminal doesn't misinterpret the dollar signs as text variable delimiters.

15. Extraneous information. If too much data is encountered for a particular RIPscrip command, or too little information is found (aside from a short last parameter) then the command is considered "hopelessly corrupted" and discarded. The RIPscrip parser should synchronize to the next vertical bar delimiter to begin the next command. *(v2.A1)*

## Description of RIPscrip Command Levels

*Added in RIPscrip v2.A1.*

As previously described, RIPscrip is composed of ten different levels of commands. The distribution of commands over different levels is done in a rather organized fashion based on command type. Each level of command in general, is progressively higher level in concept. For example, level 0 commands are the most basic building blocks of RIPscrip (eg, graphical primitives, protocol setup, etc). Level one commands are simple user interface objects. Level 2 commands are higher level still in that they deal with context swapping issues. At the highest level, level-9, we deal with binary transfer modes like file transfer protocols, etc. A more formal definition of the currently used levels are:

| Level | Description |
|-------|-------------|
| 0 | Lowest-level RIPscrip primitives. Simple graphical drawing primitives, low-level RIPscrip protocol mode settings, etc. |
| 1 | Simple user interface objects (buttons, mouse fields, clipboard slots, formatted text regions). This level consolidates one or more simple building blocks from level 0 and also introduces more abstract user interface objects (eg, mouse fields). |
| 2 | Basic context switching information (switching styles, and other window or context senstive commands. |
| 3 | Basic time-related commands (baud rate emulation and delay commands). |
| 4 | Open for future development |
| 5 | Open for future development |
| 6 | Open for future development |
| 7 | Open for future development |
| 8 | Open for future development |
| 9 | Binary related data transfer functions (ie, block protocol modes, UU-Encoding, etc). |

## RIPscrip Command Reference

The remaing bulk of this document details the RIPscrip command set. Each command has these aspects:

- **SYMBOL** — the symbolic constant that is referenced in RIPscrip documents. This is the universal name for the command. *(v2.A3)*

- **FUNCTION** — A short description of the command.

- **LEVEL** — The Command Level. Sub-levels (if any) are represented with decimal points (eg, 1.3.5 for Level-1, Sub-level 3, Sub-Sub-level 5). This is for discussion purposes only. The decimal points are never part of the actual command.

- **COMMAND** — The character identifying the command

- **ARGUMENTS** — The arguments or parameters for the command. Commands that do not require any parameters after the command type character are shown here as "\<none\>". Each parameter is shown in the order it appears in the command, and is represented by a name. If a parameter is numeric, it is followed by a width specifier indicating how many meganum or ultranum digits the parameter consists of (e.g., ":2" means a 2-digit number, or a value between 0 and 1295 for meganums, or 0 and 4095 for ultranums). If a parameter does not have a width specifier, it is by default a text parameter, and should be the last parameter of the command.

  If a command is variable length (see [POLYGON](09-level-0-commands-g-r.md#rip_polygon)), then it will appear with an ellipe (...), meaning that the command repeats the specific parameters a variable number of times. *(v2.A2)*

  If the parameter is a variable-width coordinate that can be set by the [RIP_SET_COORDINATE_SIZE](10-level-0-commands-s-w.md#rip_set_coordinate_size), or via the [RIP_HEADER](09-level-0-commands-g-r.md#rip_header) commands, then the parameter will be followed by a coordinate specifier (e.g., ":XY" means that the parameter is a variable-width coordinate). *(v2.A2, revised v2.A3)*

  If the parameter is a color parameter whose format is determined by the [RIP_SET_COLOR_MODE](10-level-0-commands-s-w.md#rip_set_color_mode) command, (ie, the parameter switches between "color mapping mode" or "direct RGB encoding" mode), then a color mode specifier will come after the parameter name (e.g., ":CM" indicates this parameter uses the current color mode). *(v2.A3)*

  Some commands do not allow color parameters to switch modes based on RIP_SET_COLOR_MODE. These exceptional commands document these exceptions and the :CM specifier will not be present. *(v2.A3)*

- **FORMAT** — This represents the format of the command, with the starting `!|`, the level digits, the command type character, and the parameter list, with the parameter names in angle brackets. (These parameters are spaced apart, but these spaces never appear in the physical commands.)

- **EXAMPLE** — An actual example of the RIPscrip command.

- **DRAW COLOR** — If YES, then this command uses or affects the current Drawing Color.

- **BACK COLOR** — If YES, then this command uses (or affects) the current background pen drawing color. *(v2.A1)*

- **LINE STYLE** — If YES, then this command uses or affects the current Line Style Pattern.

- **FILL COLOR** — If YES, then this command uses or affects the current Fill Color.

- **FILL PATRN** — If YES, then this command uses or affects the current Fill Pattern.

- **WRITE MODE** — If YES, then this command will take advantage of the current Write Mode (eg, COPY, or XOR).

- **FONT STYLE** — If YES, then this command uses or affects the current Font Style/size/orientation. *(v2.A3)*

- **VIEWPORT** — If YES, then this command adheres to the graphical viewport (draws inside it). Any RIPscrip command that adheres to the graphical viewport will be drawn (when received) only if the viewport is activated. If the viewport is deactivated, then the command is parsed, but completely ignored. *(v1.54, revised v2.A4)*

- **USES PORT** — If YES, then this command adheres to, is affected by, or modifies the definition of a [port](02-drawing-ports.md). *(v2.A3)*

- **BASE MATH** — Indicates which base math this command responds to. The possible settings are N/A (does not apply), Meganums (uses meganums ONLY), Ultranums (uses Ultranums ONLY), and Current (uses current base math setting as set by either RIP_HEADER or [RIP_SET_BASE_MATH](10-level-0-commands-s-w.md#rip_set_base_math)). *(v2.A2)*

---

[◀ Prev: Color, Audio & Text Windows](06-color-audio-text.md) · [Contents](README.md) · [Next: Level-0 Commands (A–F) ▶](08-level-0-commands-a-f.md)
