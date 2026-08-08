# Level-3 & Level-9 Commands

[◀ Prev: Level-2 Commands](12-level-2-commands.md) · [Contents](README.md) · [Next: Host Commands ▶](14-host-commands.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

Level-3 commands are time-based; level-9 commands handle communication with the host — binary transfers and encoded data streams. **None of the four commands on this page appears anywhere in the RIPtel demo corpus**: the demos are local scene files whose resources (bitmaps, JPEGs, fonts) already sit on disk, so nothing ever needs transferring or rate-limiting. All four are nonetheless attested for 3.0 — every one is named in the RIPSCRIP.HLP inventory, and SyncTERM implements the block-mode transfer with a full protocol table.

Commands: [RIP_BAUD_EMULATION](#rip_baud_emulation) (`3e`), [RIP_DELAY](#rip_delay) (`3D`), [RIP_ENTER_BLOCK_MODE](#rip_enter_block_mode) (`9<ESC>`), [RIP_BEGIN_UUENCODE_BLOCK](#rip_begin_uuencode_block) (`9U`).

## RIP_BAUD_EMULATION

_Set Baud Rate Emulation for Local RIP Playback_

|               |              |
| ------------- | ------------ |
| **Level**     | 3            |
| **Command**   | `e`          |
| **Arguments** | `rate_val:4` |

**Format:** `!|3e <rate_val>` **Example:** `!|3e01UN`

**Attributes used:** Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/13-level-3-9-commands.md#rip_baud_emulation)) · HLP (`RIP_BaudEmulation`; `$BAUDEMUL$` variable) — not observed in the RIPtel demo corpus, not in SyncTERM

Throttles local RIP file playback to approximate a baud rate (0 = full speed); the setting survives [RIP_RESET_WINDOWS](08-level-0-commands-symbols-a-f.md#rip_reset_windows) so a reset inside a played file cannot change the playback speed mid-stride. The 3.0 driver keeps the feature (its `$BAUDEMUL$` variable reads the setting back), but the demos run at full speed and pace their animations with `$D(n)$` delays instead.

## RIP_DELAY

_Causes the Client Terminal to Pause_

|               |                  |
| ------------- | ---------------- |
| **Level**     | 3                |
| **Command**   | `D`              |
| **Arguments** | `delay_period:4` |

**Format:** `!|3D <delay_period>` **Example:** `!|3D002D`

**Attributes used:** Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/13-level-3-9-commands.md#rip_delay)) — not in the RIPSCRIP.HLP name inventory, not observed in the RIPtel demo corpus, not in SyncTERM

Pauses the terminal for a number of 1/60-second timer ticks, buffering incoming data meanwhile. The 3.0 evidence points to this command having been superseded by the text-variable form: the corpus performs all of its pacing — 197 occurrences — with `$D(1)$` inside [RIP_QUERY](11-level-1-commands.md#rip_query) commands (`!|1<ESC>0000$D(1)$`, the heartbeat of every wipe animation), and RIPSCRIP.HLP carries a `D` _variable_ processor (`tvarProc` list) but no `RIP_Delay` command name.

## RIP_ENTER_BLOCK_MODE

_Enter block transfer mode with host_

|               |                                                  |
| ------------- | ------------------------------------------------ |
| **Level**     | 9                                                |
| **Command**   | `Σ` (the Escape character, ASCII 27)             |
| **Arguments** | `mode:1 proto:1 file_type:2 res:4 [filename] <>` |

**Format:** `!|9<escape> <mode> <proto> <file_type> <res> [filename] <>` **Example:** `!|9<escape>00010000ICONFILE.BMP<>`

**Attributes used:** Port (in image or local-playback file types), Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/13-level-3-9-commands.md#rip_enter_block_mode)) · HLP (`RIP_EnterBlockMode`; "Invalid file transfer protocol number", upload confirmation "Upload file(s) '%s'?", DLL exports `RIP_GetBlockModeFilename`/`RIP_GetBlockModeTransferType`; RIPtel itself uses Zmodem for actual transfers) · SyncTERM (ripper.c:17069; manual parse from 17150) — not observed in the RIPtel demo corpus

Auto-initiates a file transfer in either direction; the received files are typed (RIP scene to play, generic file, icon/bitmap, image to display, or batch DYNAMIC modes) and filed into the terminal's resource directories — see the 2.x entry for the full file-type semantics and the terminating-`<>`/carriage-return rules. It must be the last command on its line.

SyncTERM's 3.0-era implementation dispatches these protocol codes (ripper.c:17187–17300):

| Code | Protocol                              | SyncTERM     |
| ---- | ------------------------------------- | ------------ |
| `0`  | Xmodem (checksum)                     | supported    |
| `1`  | Xmodem-CRC                            | supported    |
| `2`  | Xmodem-1K                             | supported    |
| `3`  | Xmodem-1K-G                           | supported    |
| `4`  | Kermit                                | **rejected** |
| `5`  | Ymodem (batch)                        | supported    |
| `6`  | Ymodem-G                              | supported    |
| `7`  | Zmodem                                | supported    |
| `8`  | Zmodem (no crash recovery) _(2.00a4)_ | —            |
| `9`  | Super Kermit _(2.00a4)_               | —            |

SyncTERM also sandboxes the command: filenames containing `\` are rejected, and received files land in a per-BBS `RIP` cache subdirectory rather than RIPterm's global icon directory.

## RIP_BEGIN_UUENCODE_BLOCK

_Begin a generic UU-Encoded data block_

|               |                     |
| ------------- | ------------------- |
| **Level**     | 9                   |
| **Command**   | `U`                 |
| **Arguments** | `file_type:2 res:8` |

**Format:** `!|9U <file_type> <res>` **Example:** `!|9U0200000000`

**Attributes used:** Port (for displayable graphics sequences), Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/13-level-3-9-commands.md#rip_begin_uuencode_block)) · HLP (as **`RIP_BeginEncodedStream`** — the 3.0 name; "encoded stream" file types in the error strings) — not observed in the RIPtel demo corpus, not in SyncTERM

Declares that the next text lines are a UU-encoded data block — a way to move binary resources (JPEGs, bitmaps, sounds) over text-only links, with per-line CRC checking when the block's header requests it. The command must end its line; the `<file_type>` values match [RIP_ENTER_BLOCK_MODE](#rip_enter_block_mode). The 3.0 driver renamed the mechanism: RIPSCRIP.HLP's inventory lists it as `RIP_BeginEncodedStream`, and its error strings reference encoded-stream file types, confirming the feature survived into 3.0 under the new name even though no shipped demo exercises it.

---

[◀ Prev: Level-2 Commands](12-level-2-commands.md) · [Contents](README.md) · [Next: Host Commands ▶](14-host-commands.md)
