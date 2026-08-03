# Level-3 & Level-9 Commands

[← Level-2 Commands](12-level-2-commands.md) · [Contents](README.md) · [Host Commands & Text Variable Basics →](14-host-commands.md)

---

## Level-3 RIPscrip Commands

Level-3 commands are time-based and affect operations of RIPscrip. *(v2.A4)*

## RIP_BAUD_EMULATION

*Set Baud Rate Emulation for Local RIP Playback*

*Added in RIPscrip v2.A0.*

| | |
|---|---|
| **Level** | 3 |
| **Command** | `e` |
| **Arguments** | `rate_val:4` |

**Format:** `!|3e <rate_val>`

**Example:** `!|3e01UN`

**Attributes used:** base math (current setting)

This command will set the rate at which [local RIP file playback](15-local-playback-popup-lists.md) is performed. By default, local RIP playback is done at full speed which is dependent on your CPU, hard disk speed and many other factors. With this command, you can set the speed at which the playback is performed so that it can approximate a particular baud rate. This setting will remain in effect even after a [RIP_RESET_WINDOWS](09-level-0-commands-g-r.md#rip_reset_windows) command so that a Reset command inside a local RIP file will not alter the playback speed in mid-stride. The setting remains in effect until changed explicitly by the user (if possible under the terminal software), or by the Host via another RIP_BAUD_EMULATION command.

Typical settins for `<rate_val>` might be 300, 2400, 4800, 19200, etc. A value of 0 indicates that local playback should be performed at maximum speed (no baud rate emulation). *(v2.A3)*

## RIP_DELAY

*Causes the Client Terminal to Pause*

*Added in RIPscrip v2.A0.*

| | |
|---|---|
| **Level** | 3 |
| **Command** | `D` |
| **Arguments** | `delay_period:4` |

**Format:** `!|3D <delay_period>`

**Example:** `!|3D002D`

**Attributes used:** base math (current setting)

This command will cause the remote terminal to Pause for `<delay_period>` number of "timer ticks". A Timer Tick is defined as being 1/60th of a second. So, a delay period of 60 would be equal to one full second of delay.

During the time period of the delay, the remote terminal should be queueing up any incoming RIPscrip code or raw text (if any) into some form of internal buffer to prevent character loss. After the delay period is complete, the data in that buffer (if any) should be processed immediately and the terminal should resume processing the incoming stream of data.

## Level-9 RIPscrip Commands

Level-9 commands deal with communication to the host system in some fashion. These embody binary transfers and encoded data transfers. *(v2.A4)*

## RIP_ENTER_BLOCK_MODE

*Enter block transfer mode with host*

| | |
|---|---|
| **Level** | 9 |
| **Command** | `Σ` |
| **Arguments** | `mode:1 proto:1 file_type:2 res:4 [filename] <>` |

**Format:** `!|9<escape> <proto> <file_type> <res> [filename] <>`

**Example:** `!|9<escape>00010000ICONFILE.BMP<>`

**Attributes used:** port (in JPEG mode, or local RIP playback mode), base math (current setting)

> **NOTE:** `Σ` is used to indicate the Escape character (ASCII 27 or ESC).

This command is used to auto-initiate any desired File Transfer Protocol. The `<filename>` parameter is optional on downloads, required for uploads, and if omitted must be replaced with a `<>` parameter (end of string).

The `<mode>` parameter is to specify upload or download. Use "1" for upload mode, or "0" (zero) for download mode. A filename is required for uploads. If the user has Data Security enabled on the terminal, they are prompted to OK the upload before it proceeds. If the user does not authorize the upload, ten Ctrl-X's (ASCII 24 or CAN) are sent at one-tenth second intervals. The `<file_type>` parameter is ignored for uploads.

### Transfer Protocols

The `<proto>` parameter is the file transfer protocol specifier. Possible values, and the protocols they refer to are:

| Value | Protocol | Filename Required? |
|---|---|---|
| 0 | Xmodem (checksum) | Yes |
| 1 | Xmodem (CRC) | Yes |
| 2 | Xmodem-1K | Yes |
| 3 | Xmodem-1K (G) | Yes |
| 4 | Kermit | Yes |
| 5 | Ymodem (batch) | No |
| 6 | Ymodem-G | No |
| 7 | Zmodem (crash recovery) | No |
| 8 | Zmodem (no crash recovery) *(v2.A0)* | No |
| 9 | Super Kermit *(v2.A4)* | No |

### File Types

The `<file_type>` parameter determines what type of files are to be received during the block transfer. These are the valid parameters:

| Value | Description of Block Transfer Contents |
|---|---|
| 0 | RIP file sequence (display it) |
| 1 | Generic file sequence (store files for future use) *(v2.A0)* |
| 2 | BMP file sequence (store them in proper directories) *(v2.A1)* |
| 3 | Image file sequence (display it) *(v2.A0)* |
| 4 | COMPOSITE DYNAMIC file sequence (batch protocols only) |
| 5 | ACTIVE DYNAMIC file sequence (batch protocols only) |

Whether the `<filename>` is specified or not, this command must have a "<>" sequence after the filename (even if there is none). Here are examples of how it would look with and without a filename:

With a filename, using X-Modem/CRC:

```text
!|9<escape>01020000filename.icn<>
```

Without a filename, using Z-Modem

```text
!|9<escape>07040000<>
```

A File Type of 0 is intended for RIP files. The received RIP file is placed in the appropriate Host System directory and is played back like any other Local RIP file. This gives you the ability to transmit a RIP scene and play it back without having to send two different commands. *(v2.A0)*

A File Type of 1 is intended for generic files. The files are placed in the appropriate Host System directory and are left there for future use. No processing is performed on any file(s) received. *(v2.A0)*

A File Type of 2 is identical in nature to File Type 1. It is designated for Icon Files only (.BMP, .BMH and .BMM). You should use the Generic Mode 1 instead of this mode as they are identical and in future revisions, this File Type (2) might change. *(v2.A0)*

A File Type of 3 is intended for various types of received bitmap images. These image files (JPEG) are controlled with the [RIP_IMAGE_STYLE](11-level-1-commands.md#rip_image_style) command, which must be set prior to the image being received. When the image file is received, it is displayed according to the given Image Mode settings. If no previous image settings were received, the image is displayed at the full size of the current graphics viewport. JPEG files have a file extension of .JPG. *(v2.A0)*

A File Type of 4 is for COMPOSITE DYNAMIC mode. This is only useful for Batch File Transfers (ie, Zmodem and Ymodem). When files are received by this mode, they are processed separately depending on the files' extensions. In this mode, the file(s) are simply stored in the proper location. No further processing is done on the files after this point. The recognized file extensions are: *(v2.A0)*

| Extension | Action |
|---|---|
| RIP | Store the RIP file locally in the proper directory |
| BMP | Place icon in proper directory |
| BMH | Place hot icon in proper directory |
| BMM | Place mask icon in proper directory |
| JPG | Store JPEG file in proper directory |
| GIF | Store GIF file in proper directory *(v2.A4)* |

A File Type of 5 is for ACTIVE DYNAMIC mode. This is only useful for Batch File Transfers (ie, Zmodem and Ymodem). When files are received by this mode, they are processed separately depending on the files' extensions. Files received while in this mode are stored in their proper locations and then extra processing (if any) is done on the file based on file extension. The recognized file extensions are: *(v2.A0)*

| Extension | Action |
|---|---|
| RIP | Store and playback RIP file locally |
| BMP | Place icon in proper directory |
| BMH | Place hot icon in proper directory |
| BMM | Place mask icon in proper directory |
| JPG | Store and display JPEG file based on image settings |
| GIF | Store and display GIF file based on image settings *(v2.A4)* |

If you receive file(s) with extensions other than the ones listed above, then no processing will be done on the files other than placing them in the appropriate Host Directory. *(v2.A0)*

> **NOTE:** This command must be terminated with a carriage return. A vertical bar (`|`) command delimiter cannot be used to separate this command from a subsequent one on the same line. In other words, this command must be the last command on a line of text. The protocol must begin on the very next line.

## RIP_BEGIN_UUENCODE_BLOCK

*Begin a generic UU-Encoded data block*

*Added in RIPscrip v2.A1.*

| | |
|---|---|
| **Level** | 9 |
| **Command** | `U` |
| **Arguments** | `file_type:2 res:8` |

**Format:** `!|9U <file_type> <res>`

**Example:** `!|9U0200000000`

**Attributes used:** port (for displayable graphics sequences), base math (current setting)

This command must be the last RIPscrip command on the line of text. The very next line of text will be the beginning (header) information of a UU-Encoded data file block. In this header is contained the filename associated with the binary UU-Encoded data. This information is decoded by the receiving terminal and the resulting filename is stored on the terminal's hard disk to be processed depending on the `<file_type>` parameter. The `<file_type>` parameter is identical in nature and values to the parameter by the same name in the [RIP_ENTER_BLOCK_MODE](#rip_enter_block_mode) command. The file that results from the UU-Encoded data block might be a JPEG file, BMP file, sound file, or could be just about anything.

Immediately after the last line of UU-Encoded data, either raw text or RIPscrip code should begin. Some UU-Encoded data blocks have line-by-line CRC checking. If this mode is specified in the header of the UU-Encoded data block, then CRC checking on the lines should be performed as defined in the UU-Encoding specification. If a single CRC fails, the terminal should kill the partial file and ignore remaining UU-Encoded data until a RIPscrip or raw text block is encountered that it can then subsequently process.

---

[← Level-2 Commands](12-level-2-commands.md) · [Contents](README.md) · [Host Commands & Text Variable Basics →](14-host-commands.md)
