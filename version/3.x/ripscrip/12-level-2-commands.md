# Level-2 Commands

[◀ Prev: Level-1 Commands](11-level-1-commands.md) · [Contents](README.md) · [Next: Level-3 & Level-9 Commands ▶](13-level-3-9-commands.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

Level-2 commands are "context" commands: they switch between and manipulate the 36-slot data tables (ports, styles, palettes, environments, text windows). The 3.0 evidence here is lopsided in an instructive way. SyncTERM's `RIPSCRIP030001` mode implements **no level-2 commands at all** — yet the RIPtel corpus proves the port subsystem was the beating heart of shipped 3.0 content: 9,104 uses of [`2C` RIP_PORT_COPY](#rip_port_copy) power the entire WIPE00–24 transition library, backed by [`2P`](#rip_define_port)/[`2p`](#rip_delete_port)/[`2s`](#rip_switch_port) define/delete/switch. The other seven documented commands remain 2.00a4-only, though the RIPSCRIP.HLP inventory names every one of them.

Commands: [RIP_DEFINE_PORT](#rip_define_port) (`P`), [RIP_DELETE_PORT](#rip_delete_port) (`p`), [RIP_PORT_COPY](#rip_port_copy) (`C`), [RIP_PORT_WRITE](#rip_port_write) (`W`), [RIP_SET_REFRESH](#rip_set_refresh) (`R`), [RIP_SWITCH_BUTTON_STYLE](#rip_switch_button_style) (`B`), [RIP_SWITCH_ENVIRONMENT](#rip_switch_environment) (`E`), [RIP_SWITCH_PALETTE](#rip_switch_palette) (`A`), [RIP_SWITCH_PORT](#rip_switch_port) (`s`), [RIP_SWITCH_STYLE](#rip_switch_style) (`Y`), [RIP_SWITCH_TEXT_WINDOW](#rip_switch_text_window) (`T`).

## RIP_DEFINE_PORT

_Define a drawing port_

|               |                                                    |
| ------------- | -------------------------------------------------- |
| **Level**     | 2                                                  |
| **Command**   | `P`                                                |
| **Arguments** | `port-num:1 x0:XY y0:XY x1:XY y1:XY flags:4 res:4` |

**Format:** `!|2P <port-num> <x0> <y0> <x1> <y1> <flags> <res>` **Example:** `!|2P10000ZKQO00030000`

**Attributes used:** Viewport, Port, Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_define_port)) · HLP (`RIP_PortDefine`; "Can't create drawing port #0", ports 1–35 only) · corpus (14 uses in 12 files) — not in SyncTERM

Creates an onscreen (video) or offscreen (clipboard) drawing port in one of the 36 port slots (see the 2.x entry for the flag table: 1 = offscreen, 2 = make active, 4 = deactivate viewport, 8 = protect). The example — the corpus's standard form from TELPORT.FN/FONTS.RIP — defines port #1 as a full-world **1280×960** offscreen bitmap (`ZKQO`) with flags `0003` (offscreen + switch to it immediately), where background scenes are composed before being wiped onto the screen. Scene comments narrate the pattern: "Make sure port #1 is deleted / Define port #1 / Copy the screen image to the port". HLP confirms the 3.0 limits: port numbers 1–35, port #0 is the screen and cannot be created or deleted.

## RIP_DELETE_PORT

_Deletes a specific port definition_

|               |                                |
| ------------- | ------------------------------ |
| **Level**     | 2                              |
| **Command**   | `p`                            |
| **Arguments** | `port_num:1 dest_port:1 res:2` |

**Format:** `!|2p <port_num> <dest_port> <res>` **Example:** `!|2p1000`

**Attributes used:** Port, Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_delete_port)) · HLP (`RIP_PortDelete`; "Can't delete graphics port #0") · corpus (12 uses in 12 files) — not in SyncTERM

Deletes an unprotected port and switches to `<dest_port>`; deleting port #0 is the garbage-collection special case that deletes _all_ unprotected ports 1–35 (MAKEPORT.FN's `!|2p00` — "Delete all ports (screen is unaffected)"). The example, from NEWPORT.FN ("Make sure port #1 is deleted before continuing"), removes port 1 and returns to the screen; corpus uses also appear with trailing reserved digits omitted (`!|2p00`), tolerated by the shipping parser.

## RIP_PORT_COPY

_Copies graphics data from one port to another_

|  |  |
| --- | --- |
| **Level** | 2 |
| **Command** | `C` |
| **Arguments** | `source_port:1 sx0:XY sy0:XY sx1:XY sy1:XY dest_port:1 dx0:XY dy0:XY dx1:XY dy1:XY write_mode:1 res:5` |

**Format:** `!|2C <source_port> <sx0> <sy0> <sx1> <sy1> <dest_port> <dx0> <dy0> <dx1> <dy1> <write_mode> <res>` **Example:** `!|2C1XC00ZKQO0000028QO0`

**Attributes used:** Viewport, Port, Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_port_copy)) · HLP (`RIP_PortCopy`) · corpus (**9,104 uses in 46 files** — the most-used command in the entire corpus) — not in SyncTERM

Copies (and optionally scales) a rectangle from one port to another with a raster write mode. By use count this is _the_ RIPscrip 3.0 command: the WIPE00–24.FN transition library is almost nothing but port-copy animation, sliding successive slices of a pre-rendered offscreen scene onto the screen. The example line, from a wipe, copies the slice of offscreen port 1 from x = `XC` (1200) to `ZK` (1280), full height `QO` (960), onto screen port 0 at (0,0)–(72,960) in COPY mode — one frame of a right-to-left push (reserved digits omitted, as the shipping driver tolerates). Between the wipes and the `.MSE` status-line backups ("Copy the screen image to the port"), 46 of the 116 corpus files depend on it, making SyncTERM's complete lack of level-2 support the single largest gap between the open-source reconstruction and shipped 3.0 content.

## RIP_PORT_WRITE

_Writes port image to a disk-based bitmap file_

|               |                                                     |
| ------------- | --------------------------------------------------- |
| **Level**     | 2                                                   |
| **Command**   | `W`                                                 |
| **Arguments** | `port_num:1 x0:XY y0:XY x1:XY y1:XY res:4 filename` |

**Format:** `!|2W <port_num> <x0> <y0> <x1> <y1> <res> <filename>` **Example:** `!|2W5000020200000FILENAME.BMP`

**Attributes used:** Viewport, Port, Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_port_write)) · HLP (`RIP_PortWrite`) — not observed in the RIPtel demo corpus, not in SyncTERM

Saves all or part of a port's image to a BMP file — the port-generation counterpart of [RIP_WRITE_ICON](11-level-1-commands.md#rip_write_icon). Documented in 2.00a4 and named in the 3.0 driver's string table, but the demos never write to disk.

## RIP_SET_REFRESH

_Sets a sequence to send host to refresh display_

|               |                        |
| ------------- | ---------------------- |
| **Level**     | 2                      |
| **Command**   | `R`                    |
| **Arguments** | `res:4 refresh_string` |

**Format:** `!|2R <res> <refresh_string>` **Example:** `!|2R0000^m`

**Attributes used:** Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_set_refresh)) · HLP (`RIP_SetRefresh`; `$REFRESH$`/`$NOREFRESH$` variables, "refresh expressions", DLL export `RIP_RefreshSend`) — not observed in the RIPtel demo corpus, not in SyncTERM

Stores a host command the user can trigger to redraw a noise-corrupted screen; cleared with `$OFF$`/`$NOREFRESH$`. Local demo scenes have no host to refresh from, but the 3.0 driver's refresh machinery is well attested in HLP.

## RIP_SWITCH_BUTTON_STYLE

_Switches to a new button style_

|               |                |
| ------------- | -------------- |
| **Level**     | 2              |
| **Command**   | `B`            |
| **Arguments** | `bstyle_num:2` |

**Format:** `!|2B <bstyle_num>` **Example:** `!|2B04`

**Attributes used:** Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_switch_button_style)) · HLP (`RIP_SwitchButtonStyle`; button-style slots 0–35, `$SBS$`/`$RBS$` backup variables) — not observed in the RIPtel demo corpus, not in SyncTERM

Activates one of the 36 button-style slots for subsequent [RIP_BUTTON](11-level-1-commands.md#rip_button) commands. The demos simply re-send [RIP_BUTTON_STYLE](11-level-1-commands.md#rip_button_style) before each button group instead of switching slots.

## RIP_SWITCH_ENVIRONMENT

_Switches to a new environment_

|               |             |
| ------------- | ----------- |
| **Level**     | 2           |
| **Command**   | `E`         |
| **Arguments** | `env_num:2` |

**Format:** `!|2E <env_num>` **Example:** `!|2E04`

**Attributes used:** Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_switch_environment)) · HLP (`RIP_SwitchEnvironment`; environment slots 0–35, slot #0 unprotectable, `$SENV$`/`$RENV$` variables, environment header segments) — not observed in the RIPtel demo corpus, not in SyncTERM

Switches among the 36 environment slots — whole-state bundles central to the 3.0 backup/dialog system (HLP: "Can't protect environment data table entry #0"; environment header segments carry coordinate-size and RGB-bit settings).

## RIP_SWITCH_PALETTE

_Switches to a new color palette_

|               |                 |
| ------------- | --------------- |
| **Level**     | 2               |
| **Command**   | `A`             |
| **Arguments** | `palette_num:2` |

**Format:** `!|2A <palette_num>` **Example:** `!|2A04`

**Attributes used:** Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_switch_palette)) · HLP (`RIP_SwitchPalette`; "Can't protect color palette zero", palette animation strings, `$SCP$`/`$RCP$` variables) — not observed in the RIPtel demo corpus, not in SyncTERM

Activates one of 36 color-palette slots, instantly recoloring the screen in palette-mapped video modes. The demos restyle single palettes in place ([`D`](08-level-0-commands-symbols-a-f.md#rip_set_drawing_palette)/[`d`](08-level-0-commands-symbols-a-f.md#rip_one_drawing_palette)) and reset with `$RESET(PAL)$` rather than switching slots.

## RIP_SWITCH_PORT

_Switches to a new port_

|               |                            |
| ------------- | -------------------------- |
| **Level**     | 2                          |
| **Command**   | `s`                        |
| **Arguments** | `port-num:1 flags:2 res:3` |

**Format:** `!|2s <port-num> <flags> <res>` **Example:** `!|2s002`

**Attributes used:** Port, Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_switch_port)) · HLP (`RIP_SwitchPort`) · corpus (35 uses in 31 files) — not in SyncTERM

Makes another port the active drawing target, with flags to protect/unprotect the source or destination port in passing. Every wipe function ends with a switch back to the screen (`WIPE00.FN` comment "Switch to port 0"; the example's flags `02` unprotect the destination), and the port-composition scenes switch to port 1 with `!|2s100` before drawing offscreen. Reserved digits are routinely omitted in the corpus (`!|2s002` is 3 of the documented 6 characters) — further evidence of the shipping parser's truncation tolerance.

## RIP_SWITCH_STYLE

_Switches to a new Drawing Style Context_

|               |                     |
| ------------- | ------------------- |
| **Level**     | 2                   |
| **Command**   | `Y`                 |
| **Arguments** | `style_num:1 res:1` |

**Format:** `!|2Y <style_num> <res>` **Example:** `!|2YG0`

**Attributes used:** Draw Color, Back Color, Line Style, Fill Style, Write Mode, Font Style, Viewport, Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_switch_style)) · HLP (`RIP_SwitchStyle`; "Can't protect graphics style data table entry zero", `$SGS$`/`$RGS$` variables) — not observed in the RIPtel demo corpus, not in SyncTERM

Switches among 36 saved drawing-style contexts (colors, patterns, fonts, write mode, image style — full list in the 2.x entry), avoiding re-transmission of state commands.

## RIP_SWITCH_TEXT_WINDOW

_Switch to another Text Window (activate)_

|               |                      |
| ------------- | -------------------- |
| **Level**     | 2                    |
| **Command**   | `T`                  |
| **Arguments** | `window_num:1 res:1` |

**Format:** `!|2T <window_num> <res>` **Example:** `!|2T30`

**Attributes used:** Base Math (current setting) **Evidence:** 2.00a4 ([2.x entry](../../2.x/ripscrip/12-level-2-commands.md#rip_switch_text_window)) · HLP (`RIP_SwitchTextWindow`; "Cannot protect current text window slot when it is #0", `$STW$`/`$RTW$`/`$TWIN$` variables) — not observed in the RIPtel demo corpus, not in SyncTERM

Switches among the 36 text-window slots, each keeping its own ANSI attributes, cursor, and activation state. The graphics-only demos deactivate their single text window and never switch.

---

**An HLP name without a documented letter.** The port/data-table group of the RIPSCRIP.HLP inventory contains one name matching no 2.00a4 command and no observed opcode: `RIP_SwitchDirectory` — presumably switching the terminal's host/resource directory context (the DLL API exports `RIP_SetHostDirectory`/`RIP_GetHostDirectory`, and a `HOSTDIR` text-variable processor exists). Its wire opcode is unknown.

---

[◀ Prev: Level-1 Commands](11-level-1-commands.md) · [Contents](README.md) · [Next: Level-3 & Level-9 Commands ▶](13-level-3-9-commands.md)
