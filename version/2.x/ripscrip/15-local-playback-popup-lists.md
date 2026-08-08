# Local File Playback & Pop-Up Lists

[◀ Prev: Host Commands & Text Variable Basics](14-host-commands.md) · [Contents](README.md) · [Next: Templates ▶](16-templates.md)

## Local RIPscrip File Playback

You can re-play a .RIP file that you have locally on your hard disk from anyplace that allows [text variables](17-text-variables-general.md). The format of the variable is somewhat different than user variables, or pre-defined text variables. After the initial dollar sign (`$`), enter the greater-than symbol (`>`) followed by the filename (with or without the .RIP extension), then ending in another dollar sign (`$`). Several examples of this are as follows:

```text
$>MYFILE.RIP$
$>FILE1$
$>FILE1.RIP$$>FILE2.RIP$$>FILE3$
```

Note in the last example, a file extension other than .RIP was used. You are not limited to playing back local .RIP files. In fact, you can play-back any file you want. You could load any simple text file, ANSI picture image, or other such thing. When loaded, the data is not sent to the host; it is strictly echoed on your local screen. If the file is a .RIP file, it will replay any graphics that were in the file and if any Mouse Regions are defined, it will create those fields for you as well, thus allowing you to pop-up dialog screens or other such things that are not built-in to RIPterm normally.

Each "local RIP playback" variable you enter will search for the .RIP file in the current host's icon directory. If it cannot find the file in that directory, it will check the ICONS\ directory.

## Local Audio File Playback

_Added in RIPscrip v2.A1._

In 2.0, we are introducing audio capability (both digitized and sequenced music). To further enhance our host command language, we are now offering an extension of local RIP file playback for audio files. This is "Local Audio File Playback". This command is nearly identical in syntax to the local RIP file playback command with a simple alteration. Instead of using the `>` character to signify RIPscrip file, you use the close paranthesis `)` as in the following example:

```text
$)AUDIOFILE.FIL$
$)TRAIN.WAV$
```

The file extension is unimportant. See the section of this document on [audio commands](06-color-audio-text.md) for more detailed information on audio.

## Local Bitmap Playback (Display)

_Added in RIPscrip v2.A1._

This command is much like the local RIPscrip playback or the local audio playback, but it intended to place a local bitmap (.BMP) file onto the screen. The bitmap will be displayed inside the current image settings as defined by the `RIP_IMAGE_STYLE` command and will adhere to the settings of that command. This mode of displaying a BMP file is the only method for showing a BMP via the image style settings.

For the purposes of universality, the bitmap is shown to the screen using the current screen's color palette and "auto-dithering" mode is used for the viewing of the image. If you need support for some of the other modes for viewing a bitmap image (ie, the [RIP_LOAD_BITMAP](11-level-1-commands.md#rip_load_bitmap)'s flexibility), then you will have to use that command instead. This command is provided as a simple method of showing a bitmapped image from within a host command.

Note, if no image style definition has been recorded then the bitmap is shown in the maximum size of the current viewport. In other words, it will be scaled to fill up the entire viewport.

To instruct the terminal to playback a local bitmap file, you would execute a host command with the following in it:

```text
$<FILENAME.BMP$
```

Note that this uses the less-than sign unlike the greater than sign used for local RIPscrip file playback. This is intentional. In fact, we use the opposite angle-bracket to indicate that this is a low-level primitive type playback like local RIPscrip file playback. The audio playback uses the parenthesis because it is not a low-level primitive playback, but a higher level concept. We use the parenthesis as a distinction from the lower level playback commands.

## Local Image File Playback (Display)

_Added in RIPscrip v2.A1._

This command is similar to the local bitmap playback except that dealing with raw bitmapped images, this command displays compressed photo-like images. Currently, only JPEG compressed image files are supported. This command takes a JPEG file and displays it based on the defined image style (set with a `RIP_IMAGE_STYLE`) command. If no image style is recorded, then the JPEG file is displayed inside the current viewport and will occupy the entire viewport's dimensions.

This command uses a syntax similar to the local bitmap playback operation but instead uses the `(` character instead of the `<` one. This uses the same idealogy as was noted between local RIPscrip file playback and local audio file playback. The JPEG file is considered a higher level object then a raw bitmap, so it uses the parenthesis command character instead of the angle brackets. To playback the local JPEG file MYFILE.JPG, issue the following command:

```text
$(MYFILE.JPG$
```

## Pop-Up Lists

Any place that you can use a Text Variable (Queries, Button and Mouse Field return strings, and Keystroke Macros), you can take advantage of a unique feature of RIPscrip - Pop-Up Pick Lists. A Pop-Up Pick List is simply a list that pops up allowing you to choose from one of several available values. Whichever entry in the list you choose will insert it's associated command in the Host Command returned back to your host.

A list is created by putting the special list instructions inside two sets of parenthesis like this: `((` and `))`. The list consists of an optional question followed by two colons (`::`), followed by one or more list entries. For example, `((Send Email to?::Sysop,Cosysop,Joe))` says to pop-up a list asking you "Send Email to?", giving you the choices of "Sysop", "Cosysop", and "Joe".

By default, if you press ESC instead of picking an entry in the list, then nothing will be inserted into the text of your Command. You can indicate that the user must pick an entry by putting an asterisk (`*`) at the beginning of the question. For example, `((*Send Mail to?::Sysop,Joe))`. This would make it so that the user must choose either Sysop or Joe.

In the previous examples, Sysop and Joe are the text responses that are inserted into your Host Command. These commands are also the same things that are displayed in the listing. If you want to use something else in the listing instead of the return text, you can. When you make the list entry, add an `@description` to the end of it. For example:

```text
((Send Mail To?::Sysop@Head Honcho,Cosysop,Joe))
```

...would display a pop-up pick list of Head Honcho, Cosysop, and Joe.

One final feature of Pop-Up Pick Lists allows you to specify a hotkey for each entry in the list. For example, if you wanted the first character of each entry to be highlighted (thus allowing you to select that character to activate the entry), simply put a tilde (`~`) or an underline (`_`) before and after the keystroke. For example `_S_ysop` would highlight the "S" in "Sysop" appearing like this:

```text
Sysop
```

You can highlight more than one character, but only the first one will be the active hotkey. If you omit the second tilde or underline, then the remainder of the description will be highlighted.

**NOTE:** If you use a tilde or an underline in the Text Response (not the description), then those characters are inserted into your Host Command when it is transmitted to the host. You probably don't want to do this. Recommendation: only use hotkey features on list entries where you specify a description!

If you do not specify a question, then the list default to the question:

```text
Choose one of the following:
```

You may specify up to 64 entries for any one list. _(v2.A1)_

In RIPterm version 1.52 and earlier, the total length of a pick list was 256 bytes. In version 1.53 and later, this limit has been increased to 1024 bytes. _(v1.53)_

In earlier revisions of RIPscrip, a maximum number of 20 entries in a picklist were allowed. This has been expanded to 64 for version 2.0. If the total number of entries makes the picklist too tall to fit on the screen then it should handle some form of scrolling mechanism. _(v2.A1)_

Some characters have special significance in the RIPscrip language. These characters are `!` (exclamation mark, or for you Unix-heads, bang), `\` (backslash), and `|` (vertical bar). To use these characters in a Text Response, they must be preceded by a backslash (`!` becomes `\!`, `\` becomes `\\`, and `|` becomes `\|`). RIPaint automatically adds these when creating Text Responses. You need to be aware of this only if you are editing RIPscrip files with a text editor. The `_` (underline) and `~` (tilde) characters used to indicate the hotkey in a Text Response are not able to be preceded by a backslash to be used by themselves. They will be returned to the host if they exist in a Text Response (not in the description), however everything after the underline or tilde will be underlined, and the first character will be considered the hotkey.

Examples:

```text
((Send E-Mail to?::Sysop,Joe,Mike))
((*Send E-Mail to?::Sysop@The Head Honcho,Joe,Mike@My Brother))
((::Sysop@_T_he Head Honcho,Joe,Mike@My _B_rother))
```

---

[◀ Prev: Host Commands & Text Variable Basics](14-host-commands.md) · [Contents](README.md) · [Next: Templates ▶](16-templates.md)
