# Text Variables: Mouse, Text Window & Ports

[◀ Prev: Text Variables: General, Date/Time & Sound](18-text-variables-general.md) · [Contents](README.md) · [Next: Text Variables: Terminal & Reset ▶](20-text-variables-terminal.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

This page covers the pointer-and-layout half of the text-variable language: mouse position/status, text-window introspection and management, drawing-port and world-frame queries, and the image-placement variable `$IMGSTYLE$`. The grouping mirrors the [2.x edition](../../2.x/ripscrip/18-text-variables-mouse-window.md).

## Mouse Variables

SyncTERM formats coordinate fields as **4 digits in graphics mode and 2 digits in text mode** — matching 2.00a4's rule that a text-window query returns text-cell coordinates in `XX`/`YY` form while graphics queries return `XXXX`/`YYYY`.

| Variable | Behavior | Evidence |
| --- | --- | --- |
| `$M$` | Three-character `LMR` mouse-button status (`1` = pressed): `100` = left button only | 2.00a4; HLP; SyncTERM (ripper.c:8340) |
| `$MSTAT$` | `YES` if a mouse is installed, else `NO` | 2.00a4; HLP; SyncTERM (ripper.c:8290 group) |
| `$MCURSOR(n)$` | Sets the mouse-cursor style (0–6, as in [RIP_SET_MOUSE_CURSOR](11-level-1-commands.md)); the demo scripts switch cursors on field entry/exit (`$MCURSOR(4)$` busy, `$MCURSOR(6)$` hand, `$MCURSOR(0)$` arrow) | 2.00a4; HLP (`tvarProcMCURSOR`); corpus (DEMO-01.COL, MENU.FN) |
| `$X(domain)$` | Mouse X coordinate, zero-padded decimal; optional `WORLD`/`DEVICE` domain per 2.00a4 | 2.00a4; HLP; SyncTERM (ripper.c:8305–8309) |
| `$Y(domain)$` | Mouse Y coordinate, as `$X$` | 2.00a4; HLP; SyncTERM (ripper.c:8337–8339) |
| `$XY(domain)$` | Both coordinates, colon-separated. **Divergence:** 2.00a4 documents zero-padded _decimal_ (`0297:0321`); SyncTERM emits **hexadecimal** (`%0*x:%0*x`). Which the shipping 3.0 driver used is unconfirmed _(hypothesis: decimal, per the documentation lineage)_ | 2.00a4; HLP; SyncTERM (ripper.c:8312–8320) |
| `$XYM$` | As `$XY$` plus `:` and the three button bits `LMR` (`%d%d%d`) — e.g. `0123:0297:110` | 2.00a4; HLP; SyncTERM (ripper.c:8321–8332) |
| `$FIELDID$` | ID number of the current mouse field — the counterpart of the `ID=n:` prefix on 3.0 mouse-field host commands (see [Host Commands](14-host-commands.md)). New in 3.0; used by the demos for computed variable names (`$&MSG<<FIELDID>>$`) | HLP; corpus (MENU.MSE) |

_Evidence: SyncTERM (ripper.c:8290 `rv_mouse`; 8293, 8300 field widths); 2.00a4; HLP._

## Text-Window Introspection Variables

All window parameters accept `CUR` or a data-table entry number 0–35, per 2.00a4; the parameter grammar (window, type `BOUND`/`TEXT`/`CELL`, domain `WORLD`/`DEVICE`/`COLS`/`ROWS`) is documented in full in the [2.x reference](../../2.x/ripscrip/18-text-variables-mouse-window.md#text-window-related-text-variables) and carries into 3.0 unchanged as far as the evidence shows.

| Variable | Behavior | Evidence |
| --- | --- | --- |
| `$TWX0$` / `$TWY0$` | Upper-left X / Y coordinate of a text window (text coordinates by default; `BOUND`/`TEXT` + domain forms for graphical coordinates); `-1` on an undefined/deactivated window | 2.00a4; HLP; SyncTERM (ripper.c:8734 group) |
| `$TWX1$` / `$TWY1$` | Lower-right X / Y coordinate, same parameter grammar | 2.00a4; HLP; SyncTERM (ripper.c:8734 group) |
| `$TWW$` / `$TWH$` | Text-window width / height — text cells by default, or `BOUND`/`CELL`/`TEXT` pixel forms | 2.00a4; HLP; SyncTERM (ripper.c:8734 group) |
| `$TWIN$` | `YES` if the specified text window is activated, else `NO` | 2.00a4; HLP; SyncTERM (ripper.c:8734 group) |
| `$TWFONT$` | Active text-window font: 0 = none, 1 = 80×43, 2 = 91×43 MicroANSI, 3 = 80×25, 4 = 91×25 MicroANSI, 5 = 40×25 (font number + 1) — matching the five terminal text modes listed in RIPtel's MESSAGES.HLP | 2.00a4; HLP; SyncTERM (ripper.c:8734 group) |
| `$CURX$` / `$CURY$` | Text-cursor X / Y within the window, one-based; `0` when unavailable | 2.00a4; HLP |
| `$CURSOR$` | `YES`/`NO` — text-cursor enabled state for the window | 2.00a4; HLP |
| `$ISEXTWIN$` | `1` if the window is an extended text window, `0` if standard, `-1` if undefined | 2.00a4; HLP |
| `$TEXTXY$` | Location of the last graphical text output (X/Y). New in the 3.0 HLP inventory; return format unconfirmed _(hypothesis: `X:Y` pair like `$XY$`)_ | HLP |

_Evidence: 2.00a4; HLP; SyncTERM (ripper.c:8734 `rv_termstat`)._

## Text-Window Management Variables

Action variables (expand to empty). Window list parameters accept `ALL`, `CUR`, or 0–35.

| Variable | Behavior | Evidence |
| --- | --- | --- |
| `$ATW(w,…)$` | Activate one or more text windows | 2.00a4; HLP |
| `$MTW(w,…)$` | Maximize text window(s) to full screen | 2.00a4; HLP |
| `$TWGOTO(w,x,y)$` | Move the cursor in a window to X/Y (`CUR` keeps an axis unchanged) | 2.00a4; HLP |
| `$TWHOME(w)$` | Move the cursor to the window's home (upper-left) position | 2.00a4; HLP |
| `$TWERASEEOL(w,…)$` | Erase from the cursor to end of line in current ANSI attributes | 2.00a4; HLP |

Deactivation (`$DTW$`) is grouped with the terminal-control variables on the [next page](20-text-variables-terminal.md); erase (`$ETW$`) and save/restore (`$STW$`/`$RTW$`) are on the [environment page](21-text-variables-environment.md).

_Evidence: 2.00a4; HLP (RIPTEL.HLP command-variable table)._

## Port, Viewport & World-Frame Variables

The port group is a 3.0-era expansion: the `$PORT…$` query family appears in the HLP data-variable inventory but not in the 2.00a4 text-variable chapter.

| Variable | Behavior | Evidence |
| --- | --- | --- |
| `$PORTX0$` / `$PORTY0$` | Upper-left X / Y coordinate of a [drawing port](02-drawing-ports.md) | HLP |
| `$PORTX1$` / `$PORTY1$` | Lower-right X / Y coordinate of a drawing port | HLP |
| `$PORTW$` / `$PORTH$` | Port width / height | HLP |
| `$OFFSCREEN$` | Offscreen bitmap port pixel data — the HLP one-liner suggests a host-side readback of offscreen port contents; wire format unrecovered _(hypothesis)_ | HLP |
| `$AVP(p,…)$` | Activate viewport(s) (`ALL`/`CUR`/0–35) | 2.00a4; HLP (`tvarProcAVP`) |
| `$DVP(p,…)$` | Deactivate viewport(s) | 2.00a4; HLP (`tvarProcDVP`) |
| `$MVP(p,…)$` | Maximize viewport(s) to full port size | 2.00a4; HLP (`tvarProcMVP`) |
| `$WORLD(env,w,h)$` | Query or set an environment's [world coordinate frame](05-coordinates-and-math.md); query returns `width:height` (e.g. `640:350`); `DEFAULT` selects 1280×960 (MegaNums) or 4096×3072 (UltraNums) | 2.00a4; HLP |
| `$WORLDW$` / `$WORLDH$` | Query/set the world frame's X / Y dimension individually | 2.00a4; HLP |
| `$COORDSIZE(env,size)$` | Query or set the byte-width (2–5) of X/Y coordinate parameters in raw RIPscrip code for an environment; querying an unused environment returns `-1` | 2.00a4; HLP |

Graphics-viewport erasure (`$EGW$`) is on the [environment page](21-text-variables-environment.md) with the other erase/save/restore variables.

_Evidence: HLP (RIPTEL.HLP data-variable table; RIPSCRIP.HLP tvarProc names); 2.00a4 for the inherited `$AVP$`/`$DVP$`/`$MVP$`/`$WORLD…$`/`$COORDSIZE$` grammar._

## Image Placement — `$IMGSTYLE$`

**Format:** `$IMGSTYLE(cur,x0,y0,x1,y1)$`

Sets the image style — destination port and bounding rectangle — for subsequently displayed images and bitmaps, including those shown via the `$<file$` / `$(file$` [playback prefixes](15-local-playback-popup-lists.md). The first parameter names the target port; the HLP documents `cur` as "the current screen port". This is the macro-language counterpart of the [RIP_IMAGE_STYLE](11-level-1-commands.md) wire command.

_Evidence: HLP (RIPTEL.HLP macro chapter; `tvarProcIMGSTYLE`); corpus (RIP_IMAGE_STYLE used in 6 files, e.g. N2_PHOTO.RIP, IMAGES.RIP)._

---

[◀ Prev: Text Variables: General, Date/Time & Sound](18-text-variables-general.md) · [Contents](README.md) · [Next: Text Variables: Terminal & Reset ▶](20-text-variables-terminal.md)
