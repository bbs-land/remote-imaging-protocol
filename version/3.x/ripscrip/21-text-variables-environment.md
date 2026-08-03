# Text Variables: Environment, Clipboard, Screen & Tables

[◀ Prev: Text Variables: Terminal & Reset](20-text-variables-terminal.md) · [Contents](README.md) · [Next: File Formats ▶](22-file-formats.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

This page covers state save/restore, the clipboard, screen and device queries, and the data-table introspection variables, mirroring the [2.x grouping](../../2.x/ripscrip/20-text-variables-environment.md). The save/restore families operate on the [data backup areas](04-data-backup-areas.md) (base area, slots 0–9, and the PUSH/POP stack — all confirmed for 3.0 by the DLL error strings "No slots available for PUSH" / "No stack to pop").

## Graphics-Screen Save & Restore

| Variable | Behavior | Evidence |
|---|---|---|
| `$SAVE(dest)$` | Save the graphics screen (plus viewport settings) — slot-less, or to `0`–`9`, `PUSH`, `BASE` per 2.00a4 | 2.00a4; HLP; SyncTERM (ripper.c:8483 `rv_save`) |
| `$SAVE0$`–`$SAVE9$` | Legacy numbered-slot save forms (obsolete in favor of `$SAVE(n)$`) | 2.00a4; HLP; SyncTERM (ripper.c:8483) |
| `$RESTORE(src)$` | Restore the screen — slot-less, or from `0`–`9`, `POP`, `BASE` | 2.00a4; HLP; SyncTERM (ripper.c:8530 `rv_restore`) |
| `$RESTORE0$`–`$RESTORE9$` | Legacy numbered-slot restore forms | 2.00a4; HLP |
| `$SAVEALL(dest)$` | Composite save: text windows, clipboard, mouse fields, styles, screen, palette, environment | 2.00a4; HLP |
| `$RESTOREALL(src)$` | Composite restore of everything `$SAVEALL$` records | 2.00a4; HLP |

SyncTERM backs these with eleven screen slots (`screen_saves[11]`: 0–9 numbered plus a default). Note an implementation asymmetry: SyncTERM's built-in table contains **`SAVE0` but no `RESTORE0`** (`RESTORE1`–`RESTORE9` only), whereas the HLP documents the symmetric `$RESTORE0$`–`$RESTORE9$` range — scripts should prefer the parameterized `$RESTORE(0)$` form.

*Evidence: 2.00a4; HLP; SyncTERM (ripper.c:320, 8483, 8530).*

## Data-Table Backup Save & Restore

Each pair snapshots one [data table](03-data-tables.md) to its backup area (`BASE` default, `0`–`9`, `PUSH`/`POP`):

| Variable | Behavior | Evidence |
|---|---|---|
| `$SBS$` / `$RBS$` | Save / restore the button-style table | 2.00a4; HLP |
| `$SCP$` / `$RCP$` | Save / restore the color-palette table | 2.00a4; HLP |
| `$SGS$` / `$RGS$` | Save / restore the graphics-style table | 2.00a4; HLP |
| `$SENV$` / `$RENV$` | Snapshot / re-activate the environment configuration (world frame, base math, coordinate size, color mode, current-entry numbers, baud emulation…) | 2.00a4; HLP |

*Evidence: 2.00a4; HLP (RIPTEL.HLP command-variable table; backup-system error strings).*

## Clipboard Variables

| Variable | Behavior | Evidence |
|---|---|---|
| `$SCB(dest)$` | Save the clipboard (contents *and* last location) | 2.00a4; HLP; SyncTERM (ripper.c:8483 group) |
| `$RCB(src)$` | Restore a saved clipboard | 2.00a4; HLP; SyncTERM (ripper.c:8530 group) |
| `$PCB(port)$` | Paste the clipboard at its last location — the standard "dismiss dialog, restore what was underneath" idiom; the demo entry/exit scripts comment it "Paste original screen image back" | 2.00a4; SyncTERM (ripper.c:9048 `rv_paste`); corpus (MENU.ENT, TEL3X2.ENT) |

The RIPTEL.HLP command table lists the paste operation as "`$P$` paste clipboard" while the DLL string table names the processor `PCB` — both spellings likely resolve to the same handler, with `$P$` a shorthand *(hypothesis)*.

*Evidence: 2.00a4; HLP; SyncTERM (ripper.c:9048); corpus.*

## Mouse-Field and Text-Window Save & Restore

| Variable | Behavior | Evidence |
|---|---|---|
| `$SMF(dest)$` / `$RMF(src)$` | Save / restore all defined mouse fields and buttons | 2.00a4; HLP; SyncTERM (ripper.c:8483/8530 groups) |
| `$STW(dest)$` / `$RTW(src)$` | Save / restore text-window definitions (positions, cursor, attributes, fonts — not window *contents*) | 2.00a4; HLP; SyncTERM (ripper.c:8483/8530 groups) |

*Evidence: 2.00a4; HLP; SyncTERM.*

## Erase Variables

| Variable | Behavior | Evidence |
|---|---|---|
| `$EGW(port,…)$` | Erase graphics viewport(s) (`ALL`/`CUR`/0–35) | 2.00a4; HLP; SyncTERM (ripper.c:8696 `rv_erase`) |
| `$ETW(w,…)$` | Erase text window(s), including the full bounding rectangle | 2.00a4; HLP; SyncTERM (ripper.c:8696) |

*Evidence: 2.00a4; HLP; SyncTERM (ripper.c:8696).*

## Environment & Device Queries

| Variable | Behavior | Evidence |
|---|---|---|
| `$COLORS$` | Total colors of the video device (2/16/256/…/16777216). The demo corpus branches on it constantly — `<<IF $COLORS$<"256">>BLUEBACK.FN<<ELSE>>BLUEFADE.FN<<ENDIF>>` — making it the standard color-capability adaptive test (15 uses) | 2.00a4; HLP; corpus (BUTTONS.RIP, MENU.RET) |
| `$RESX$` / `$RESY$` | Horizontal / vertical device resolution in pixels | 2.00a4; HLP |
| `$COLORMODE(env,mode,bits)$` | Query/set palette-mapping vs direct-RGB [color mode](06-color-audio-text.md); 3.0's DLL accepts only 8 bits per channel ("RGB color mode only supports 8-bit color currently") | 2.00a4; HLP |
| `$BASEMATH(env,setting)$` | Query/set the [base math](05-coordinates-and-math.md) (36 = MegaNums, 64 = UltraNums) | 2.00a4; HLP |
| `$ISPALETTE$` | `1` if the video device has a real color palette, `0` in direct-color modes | 2.00a4; HLP |
| `$PALENTRY(pal,start,stop)$` | Return RGB values from a palette (`bits;r:g:b,…` format) | 2.00a4; HLP (`tvarProcPALENTRY`) |
| `$CLS$` | Clear the screen to the background color (no reset) | 2.00a4; HLP |

*Evidence: 2.00a4; HLP.*

## Data-Table Introspection

The 2.00a4 table-management family survives in the 3.0 DLL — every processor is named in the RIPSCRIP.HLP string table. Full parameter grammars are in the [2.x reference](../../2.x/ripscrip/20-text-variables-environment.md#data-table-and-backup-area-text-variables).

| Variable | Behavior | Evidence |
|---|---|---|
| `$INUSE(object,element)$` | Is a data object in use? 2.00a4 defines objects `TW`/`PORT`/`STYLE`/`BUT`/`PAL`/`ENV`/`MOUSE`/`SCREEN`. **3.0 extension:** the corpus tests *text variables* with a first argument of `TV` — `<<IF $INUSE(TV,NEXT_IMG)$="0">>$-=NEXT_IMG=ASTRO.JPG$<<ENDIF>>` (first-run initialization of the photo-cycling demo) | 2.00a4; HLP; corpus (SPECLEFX.RIP, N2_PHOTO.RIP) |
| `$ISPROT(object,element)$` | Is an element protected? | 2.00a4; HLP |
| `$PROT(object,element,…)$` / `$UNPROT(…)$` | Protect / unprotect table entries and save slots (the DLL string table names a `PROTECT` processor) | 2.00a4; HLP |
| `$CUR(type,which)$` | Query or select the current entry of a data table | 2.00a4; HLP (`tvarProcCUR`) |
| `$COPY(type,src,dest,…)$` | Copy data tables/entries between the table, base area, slots, and stack | 2.00a4; HLP (`tvarProcCOPY`) |
| `$BACKSTAT(type,mode)$` | Backup-area status report (`base:stack:slots:free:s0…s9`) | 2.00a4; HLP (`tvarProcBACKSTAT`) |

*Evidence: 2.00a4; HLP (tvarProc string table).*

## Column-System Paging — `$OVERFLOW$`

**Format:** `$OVERFLOW(stream, cur|next|prev|page [,setverbose])$`

New in 3.0, and paired with the [column text system](17-column-text-system.md): flowed text that exceeds its columns paginates into numbered *overflow buffers*, and `$OVERFLOW$` navigates them. `page` yields the current page number ("Page `$overflow(1,page)$`" counters); `cur`/`next`/`prev` return the overflow content or file reference for use as a read-scene target — the demo pagers re-invoke their own scene: `ID=6:$overflow(1,prev,setverbose)$$>demo-01.col$`. `$RESET(OVERFLOW)$` deletes all overflow buffers (see the [previous page](20-text-variables-terminal.md#the-reset-family)).

*Evidence: corpus (DEMO-01.COL, DEMO-02.COL, NEWSPAPR.RIP); observed lowercase and uppercase spellings.*

## External Applications — `$APP$`

| Variable | Behavior | Evidence |
|---|---|---|
| `$APP(appno,argument)$` | Run external application 0–9 with an optional command-line argument (DLL validates "Application number…") | 2.00a4; HLP (`tvarProcAPP`) |
| `$APP0$`–`$APP9$` | Legacy fixed-slot forms. SyncTERM carries all ten in its built-in table as stubs — the handler only prints a TODO message | 2.00a4; SyncTERM (ripper.c:9041–9046 `rv_exploit`) |

*Evidence: 2.00a4; HLP; SyncTERM (ripper.c:9041–9046).*

## File-Playback Prefixes

The four playback prefixes — `$>file.RIP$` (scene), `$)file.WAV$` (audio), `$<file.BMP$` (bitmap), `$(file.JPG$` (JPEG) — are syntactically text variables but form their own subsystem; they are documented with search-path and image-style rules in [Local File Playback & Pop-Up Lists](15-local-playback-popup-lists.md). The corpus uses `$>…$` 34 distinct ways (scene chaining is how the entire demo menu system navigates).

*Evidence: HLP; corpus; SyncTERM (ripper.c:7967–7970 implements `$>…$` only).*

---

[◀ Prev: Text Variables: Terminal & Reset](20-text-variables-terminal.md) · [Contents](README.md) · [Next: File Formats ▶](22-file-formats.md)
