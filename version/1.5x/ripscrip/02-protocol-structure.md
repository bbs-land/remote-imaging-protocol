# Protocol Structure & Syntax

[◀ Prev: Introduction](01-introduction.md) · [Contents](README.md) · [Next: Command Reference Overview ▶](03-command-reference.md)

## RIPscrip PROTOCOL - GENERAL STRUCTURE

This document describes RIPscrip commands up through version 1.54 of the RIPscrip Protocol Specification.

RIPscrip is organized into 10 levels of graphical commands (low Level-0 to high Level-9). Level-0 commands are the building blocks of RIPscrip. The basic graphics primitives of the system are all Level-0, including the commands Line, Rectangle, Circle, Color, Font, etc. Each level of RIPscrip gets progressively higher-level in concept. For example, Level-1 commands use Mouse Regions, Icons, and Formatted Text Regions.

The basic syntax rules are as follows:

1.  A RIPscrip command line starts at the beginning of a line of text. A RIPscrip command line moved to the middle of a line of text is treated as literal text. This prevents people inserting mischievous things in teleconference messages, or similar pranks. The only exceptions to this rule is stated below under item 6, "continuation of long lines", and item 12 "alternate RIPscrip starting sequences".

2.  A RIPscrip command line begins with an exclamation mark (`!`).

3.  Every RIPscrip command is preceded by the universal RIPscrip delimiter, vertical-bar (`|`)

4.  Individual RIPscrip commands may be combined on the same line providing they are separated by the vertical bar delimiter.

5.  RIPscrip commands or command lines may be split across multiple lines with a backslash (`\`) just before each split. This helps RIPscrip commands conform to right margins and escape word wrapping.

    An example:

    ```text
    !|c02|L02030405|P0901020102010201020102\
    0102010201020102
    ```

6.  RIPscrip must allow for normal text to be intermixed with RIPscrip commands. If unrecognized text appears after a RIPscrip command, on the same line, the text is ignored (the command is not ignored). A line that does not begin with `!|` is considered raw text and is routed to the TTY text window (see "8" below).

7.  RIPscrip makes provisions for a Graphical Window and a Text Window. The Graphical Window is where all RIPscrip graphics appear. The Text Window is where raw text appears. Raw Text includes ANSI color and cursor movement codes (a subset of VT-100 terminal emulation).

8.  The vertical bar (`|`) of a RIPscrip command can be followed by a level number. If the 1st character after (`|`) is a numeric digit (1-9), then that's the RIPscrip Command Level. If the very 1st character is NOT a digit 1-9, then it is the command type character and the command is a Level-0 command. If the 1st character is a digit 1-9, and the second character is also a digit, then that defines a sub-level of a RIPscrip level. For example:

    ```text
      !|L     RIPscrip Level-0 Command "L"
     !|1L     RIPscrip Level-1 Command "L"
    !|15L     RIPscrip Level-1, sub-level 5 Command "L"
    ```

    Each of the above examples are unique commands not to be confused with each other. You may continue the sub-levels up to a maximum level of 9 (e.g., `!|123456789<cmd>`").

9.  Every RIPscrip command includes a command type character. In Level-0 commands, this character immediately follows the vertical bar. At all other levels, it follows the level digits. The command type character may be any printable non-decimal-digit character.

10. Following the command type character are 0 or more parameters. If the command requires a text-string, it is always the LAST parameter. Numeric parameters DO NOT have any delimiters (commas, dashes, spaces, etc.). A variable width numeric parameter may be used as the last parameter. This allows for maximum efficiency. Numbers are represented in base-36. This compacts numbers down to roughly 3/5 of their decimal form. This numbering system, technically called "Hexa-Tri-Decimal", has affectionately been dubbed "MegaNums". Unlike Hexadecimal which uses 0-9, A-F, MegaNums take advantage of the entire alphabet, using characters 0-9 and A-Z.

11. An exclamation mark (`!`) or vertical bar (`|`) character can appear in a RIPscrip text parameter by preceding it with a backslash (`\`). A literal backslash is represented with a double-backslash (`\\`).

12. A RIPscrip sequence CAN begin in a column other than column #0, if the exclamation mark prefix is replaced with a Ctrl-A (Start Of Header [SOH]) character, or Ctrl-B (STX) character. Since 99.9% of all BBS' do not allow users to enter most control characters, users will be unable to begin RIPscrip sequences in the middle of a command line. Only the host should be able to do this. This prevents people from cluttering teleconference, or other areas of a host with spurious RIPscrip sequences.

13. If the last couple of bytes on a RIPscrip text line are backslashes, special care must be taken to make sure that they are not interpretted as a line-continuation. If a literal backslash is desired as the last position on the line, it must be specified as a double-backslash (eg, `\\`). If a line-continuation is used then there would have to be three backslashes used on the line as in the following example: *(v1.54)*

    ```text
    !|@2233this is a text line with a literal \\\
    used in the message
    ```

    This would text output at (22,33) [meganum] the message:

    ```text
    this is a text line with a literal \used in the message
    ```

## ANSI SEQUENCES (AUTO-SENSING)

RIPscrip predominantly uses non-ANSI command sequences. In a couple of situations though, an ANSI sequence is allowed to perform a specific function. There are currently three separate ANSI sequences defined in the RIPscrip protocol to perform various actions. They are as follows:

`ESC[!` — Query RIPscrip version number. RIPterm will respond with `RIPSCRIPxxyyvs` where "xx" is equal to the major version number (zero padded), "yy" is equal to the minor version number (zero padded), "v" is the vendor code of the terminal program (see below), and "s" is the vendor's sub-version code for their software. For v1.54, the returned sequence for RIPterm (Vendor Code "1") would be `RIPSCRIP015410`. Another example, v1.23 with a Vendor Code of "2" and a sub-revision code of "5" would return `RIPSCRIP012325`. *(v1.54)*

Valid Vendor Codes are: *(v1.54)*

| CODE | VENDOR |
|------|--------|
| 0 | Generic RIPscrip terminal (vendor unknown) |
| 1 | RIPterm (from TeleGrafix Communications) |
| 2 | Qmodem Pro (from Mustang Software, Inc) |

This ANSI sequence is often used for "Auto-Sensing" if a RIPscrip terminal exists on the remote end of the connection. If a non-RIPscrip terminal receives this ANSI sequence, it will ignore it. *(v1.54)*

`ESC[0!` — Same as `ESC [ !` (see above)

`ESC[1!` — Disables all RIPscrip processing. Any RIPscrip sequences are interpreted as raw text.

`ESC[2!` — Enabled RIPscrip processing. Any RIPscrip sequences will be parsed and processed.

## MISCELLANEOUS NOTES/INFORMATION

*Added in RIPscrip v1.54.*

Later in this document, references are made to [Mouse Fields](09-mouse-fields.md) and [Mouse Buttons](12-buttons.md). Specifically, it is noted that up to 128 of these types of commands may exist simultaneously on-screen. This means that you can have 128 mouse fields, 128 mouse buttons, or any combination of the above, but combined, their total number cannot exceed 128.

When the user clicks his/her mouse on the screen, all mouse regions (whether mouse fields or mouse buttons) are scanned from most recent to the least recent. This means that if a mouse region is received that overlaps another mouse region, that one would be clicked first if the user clicked in that region.

If you are implementing a client terminal to support RIPscrip graphics and you do not intend on supporting 100% of all pre-defined [text variables](15-text-variables.md), you SHOULD at least recognize them and do nothing. This makes it so that if a particular text variable is used to make a sound (for example), then if you don't support it, you just ignore it instead of popping up a dialog box on your user's screen asking them to enter data for the variable `$MUSIC$` for example!

---

[◀ Prev: Introduction](01-introduction.md) · [Contents](README.md) · [Next: Command Reference Overview ▶](03-command-reference.md)
