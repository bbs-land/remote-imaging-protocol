# Text Variables: Terminal & Reset

[◀ Prev: Text Variables: Mouse, Text Window & Ports](19-text-variables-mouse-window.md) · [Contents](README.md) · [Next: Text Variables: Environment, Clipboard, Screen & Tables ▶](21-text-variables-environment.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

This page covers the variables that switch terminal operating modes and the parameterized `$RESET(…)$` family, mirroring the [2.x grouping](../../2.x/ripscrip/19-text-variables-terminal.md).

## Terminal Operation Variables

All are action variables (expand to empty) unless noted as returning a value.

| Variable | Behavior | Evidence |
| --- | --- | --- |
| `$SBARON$` / `$SBAROFF$` | Turn the terminal status bar on / off; the demo screens routinely hide it (`$SBAROFF$`, 7 uses) | 2.00a4; HLP; SyncTERM (ripper.c:8801 group); corpus (DRAGON.RIP) |
| `$STATBAR$` | Returns `YES`/`NO` — status-bar visibility | 2.00a4; HLP; SyncTERM (ripper.c:8734 group) |
| `$DWAYON$` / `$DWAYOFF$` | Doorway keyboard mode on / off | 2.00a4; HLP; SyncTERM (ripper.c:8801 group) |
| `$VT102ON$` / `$VT102OFF$` | VT-102 emulation mode on / off. SyncTERM implements these as stubs (effectively no-ops); RIPtel advertises real built-in VT-102 emulation | 2.00a4; HLP; WP; SyncTERM (ripper.c:8895–8903) |
| `$COFF$` / `$CON$` | Text cursor off / on (window list `ALL`/`CUR`/0–35 per 2.00a4); `$COFF$$DTW$` is the standard demo-scene prologue | 2.00a4; HLP; SyncTERM (ripper.c:8801 group); corpus (BLUEBACK.FN) |
| `$DTW(w,…)$` | Deactivate text window(s) — received raw text is discarded | 2.00a4; HLP; SyncTERM (ripper.c:8726 `rv_disable`); corpus (14 uses) |
| `$HKEYON$` / `$HKEYOFF$` | Enable / disable button hotkeys | 2.00a4; HLP; SyncTERM (ripper.c:9016–9024) |
| `$TABON$` / `$TABOFF$` | Enable / disable Tab-key navigation between mouse/button fields; SyncTERM implements full `tab_next_field` highlight tracking | 2.00a4; HLP; SyncTERM (ripper.c:8974–9014, 9025–9035) |
| `$COMPAT(env)$` | Set an environment to RIPscrip 1.54 compatibility: 640×350 world frame, palette color mode, 2-byte coordinates, base-36 math, full-speed playback. SyncTERM additionally clears its coordinate remap tables. The single most common way 3.0 scenes drop into legacy mode (21 corpus uses) | 2.00a4; HLP; SyncTERM (ripper.c:8814–8840); corpus (BLUEBACK.FN) |
| `$OPTION(name,mode)$` | Enable/disable/query a named software option (`DOORWAY`, `HOTKEY`, `STATBAR`, `TAB`, `LIST` per 2.00a4) | 2.00a4; HLP (`tvarProcOPTION`) |
| `$D(duration)$` | Delay, in 60ths of a second; `$D(1)$` is the single most-used variable in the demo corpus (197 uses, pacing the wipe effects) | 2.00a4; HLP (`tvarProcD`); corpus (SHADOW.FN, WIPE*.FN) |
| `$REFRESH$` / `$NOREFRESH$` | Transmit the refresh expression to the host / disable the refresh expression (see RIP_SetRefresh) | 2.00a4; HLP |
| `$BAUDEMUL(env,rate)$` | Query or set the baud-rate emulation for local playback (0 = full speed) | 2.00a4; HLP |
| `$FILEDEL(file,…)$` | Delete host-created files on the terminal (no wildcards, no paths) | 2.00a4; HLP |

### HLP-attested processor names with unrecovered semantics

The RIPSCRIP.DLL string table names additional variable processors — `SHIFT`, `CTRL`, `HOSTDIR`, `XFER` — that appear in no help-file table and nowhere in the corpus. Plausibly key-modifier injection, host-directory control (cf. the DLL API's `RIP_SetHostDirectory`), and file-transfer triggering _(hypothesis)_.

_Evidence: HLP (RIPSCRIP.HLP tvarProc string table)._

## The `$RESET(…)$` Family

Bare `$RESET$` is inherited from 1.54: it performs the same operation as [RIP_RESET_WINDOWS](08-level-0-commands-symbols-a-f.md) (a "soft" reset). SyncTERM implements it as its full-reset handler.

3.0 keeps 2.00a4's _parameterized_ reset but the HLP documents a flat keyword list rather than 2.00a4's full `(option, element, sub-element)` grammar (documented in the [2.x reference](../../2.x/ripscrip/19-text-variables-terminal.md#reset)). The DLL string table names one handler per keyword (`resetKeywordSOFT`, `resetKeywordHARD`, `resetKeywordMCURSOR`, `resetKeywordKEYBOARD`, `resetKeywordSOUND`, `resetKeywordQUERY`, `resetKeywordTV`), which suggests the keyword set below is the complete shipping surface:

| Keyword | Behavior | Evidence |
| --- | --- | --- |
| `$RESET(SOFT)$` | Soft reset — as RIP_RESET_WINDOWS | HLP; 2.00a4 |
| `$RESET(HARD)$` | Hard reset — as a RIP_HEADER hard-reset | HLP; 2.00a4 |
| `$RESET(MCURSOR)$` | Restore the default mouse cursor; re-enables mouse input if disabled | HLP; 2.00a4 |
| `$RESET(KEYBOARD)$` | Re-enable keyboard input | HLP; 2.00a4 |
| `$RESET(SOUND)$` | Stop any playing digitized sound | HLP; 2.00a4 |
| `$RESET(TV)$` | Reset text-variable state _(hypothesis: 2.00a4's `TW` text-window reset, renamed/retargeted — the DLL handler is `resetKeywordTV` and 3.0 elsewhere uses `TV` to mean "text variable", cf. `$INUSE(TV,…)$`)_ | HLP |
| `$RESET(QUERY)$` | Clear resident queries | HLP; 2.00a4 |
| `$RESET(PAL)$` | Reset the color palette; called out by name in the RIPtel readme | HLP; corpus-adjacent (readme) |
| `$RESET(OVERFLOW)$` | Reset (delete) all overflow paging buffers of the [column text system](17-column-text-system.md); issued by NEWSPAPR.RIP before building a fresh article ("Reset all overflow files") | corpus (NEWSPAPR.RIP) |

The `OVERFLOW` keyword appears only in the corpus — it is a 3.0 addition tied to the column system and is absent from both the 2.00a4 grammar and the HLP keyword list.

_Evidence: HLP (keyword list + resetKeyword_ handler names); 2.00a4 (full reset grammar); SyncTERM (ripper.c:8381 `rv_reset`); corpus (NEWSPAPR.RIP `$RESET(OVERFLOW)$`).*

## `$OFF$` — Deferred-Command Sentinel

`$OFF$` is not a mode toggle but a sentinel value: sent as the text of a deferred query (RIP_QUERY modes 1/2) it _cancels_ the stored deferred command, and 2.00a4 uses it likewise to disable refresh expressions (`RIP_SET_REFRESH` with `$OFF$`).

_Evidence: 2.00a4; SyncTERM (ripper.c:14996–15013)._

## `$MKILL$` — Kill Mouse Fields

**Format:** `$MKILL(x0,y0,x1,y1,inout)$`

With no parameters, deletes all defined mouse fields (as RIP_KILL_MOUSE_FIELDS) while leaving the graphics on screen. The five-parameter 2.00a4 form restricts deletion to fields inside (`IN`) or outside (`OUT`) a world-coordinate box.

_Evidence: 2.00a4; HLP; SyncTERM (ripper.c:8719 `rv_mouse_kill`)._

## `$GOTOURL(var)$` — Web Launch

**Format:** `$GOTOURL(variable_name)$`

Launches the terminal's web browser on the URL stored in a user variable — 1997 web integration, absent from every earlier specification. The demo corpus wires it through mouse fields in the telnet-listings screens:

```text
ID=1:$-=WEBURL=http://duke.usask.ca/~scottp/free.html$$GOTOURL(WEBURL)$
```

Note that the parameter is a _variable name_, not a literal URL. See also [Host Commands](14-host-commands.md).

_Evidence: corpus (TELLISTS.MSE / TELLISTS.MNU, 8 uses)._

---

[◀ Prev: Text Variables: Mouse, Text Window & Ports](19-text-variables-mouse-window.md) · [Contents](README.md) · [Next: Text Variables: Environment, Clipboard, Screen & Tables ▶](21-text-variables-environment.md)
