# Reconstructed Text Variables & HCL

[◀ Prev: Reconstructed Command Set](10-reconstructed-command-set.md) · [Contents](README.md)

> **Editorial reconstruction.** TeleGrafix never published a RIPscrip 3.0 Language Reference. This page reconstructs the text-variable and host-command language of the 3.x era from implementation evidence — primarily SyncTERM's `ripper.c` (Synchronet project, GPL). Every claim cites its source. This is NOT TeleGrafix documentation.

Text variables (`$NAME$`) and the Host Command Language (HCL) are where the 3.0-era feature set most visibly outgrows the documented 1.54/2.0 specifications. This page inventories what SyncTERM actually implements. Line numbers refer to the `ripper.c` revision surveyed in mid-2026. Baseline for comparison: [1.54 text variables](../../1.5x/ripscrip/15-text-variables.md), [2.x host commands](../../2.x/ripscrip/14-host-commands.md), and [2.x templates](../../2.x/ripscrip/16-templates.md).

## Resolution Model

`get_text_variable` (`ripper.c:7963–7979`) resolves a `$NAME$` reference in three steps:

1. **Built-ins**: a case-insensitive `bsearch` over the `builtins[]` table (`ripper.c:7633–7738` — the table must therefore stay alphabetically sorted).
2. **User-defined variables** on a miss (see below).
3. **Empty string** if neither matches — an unknown variable expands to nothing, never an error.

One special form is checked before either lookup: a leading `>` character. `$>FILENAME$` plays the named local `.RIP` scene file (`rip_play_scene(&var[1])`, `ripper.c:7967–7970`) and expands to empty. This form appears in no 1.54 documentation — it is an undocumented extension.

## Built-in Variable Inventory

The `builtins[]` table contains **103 entries** (`ripper.c:7633–7738`), grouped below by handler function. Many entries are not "variables" at all in the 1.54 sense — expanding them *performs a terminal-control action* and yields an empty string. These action-variables have no 1.54 equivalent and constitute the bulk of the practical "3.0 additions."

### Version — `rv_version` (`ripper.c:8055`)

| Variable | Behavior | Cite |
|---|---|---|
| `RIPVER` | Expands to the 14-byte version string (`RIPSCRIP015410` / `RIPSCRIP030001`) | `ripper.c:8055–8058` |

### Date — `rv_date` (`ripper.c:8061`)

| Variable | Behavior |
|---|---|
| `ADOW` | Abbreviated day-of-week name |
| `DATE` | Date, short format `MM/DD/YY` |
| `DATETIME` | Date and time combined |
| `DAY` | Day-of-month number |
| `DOW` | Day of week, fully spelled out |
| `DOY` | Day of year |
| `FYEAR` | Four-digit year |
| `MONTH` | Month name |
| `MONTHNUM` | Month number (1–12) |
| `WDAY` | Day of week (0–6) |
| `WOY` | Week of year 00–53, Sunday-first |
| `WOYM` | Week of year 00–53, Monday-first |
| `YEAR` | Two-digit year |

### Time — `rv_time` (`ripper.c:8148`)

| Variable | Behavior |
|---|---|
| `AMPM` | `AM` or `PM` |
| `HOUR` | Hour, 12-hour style |
| `MHOUR` | Hour, 24-hour ("military") style |
| `MIN` | Minutes |
| `SEC` | Seconds |
| `TIME` | Time in `HH:MM:SS` format |
| `TIMEZONE` | Time zone, or `NONE` if unknown |

### Sound — `rv_sound` (`ripper.c:8235`)

All sound variables are actions (expand to empty, play a sound):

| Variable | Behavior | Cite |
|---|---|---|
| `ALARM` | 3 × (320 Hz 200 ms + 160 Hz 425 ms) tone pairs | `ripper.c:8244–8253` |
| `BEEP` | 1000 Hz for 75 ms, then `SLEEP(75)` — annotated `// Literally in the spec.` | `ripper.c:8257–8258` |
| `BLIP` | 50 Hz for 25 ms, then `SLEEP(10)` — annotated `// Literally in the spec.` | `ripper.c:8261–8262` |
| `MUSIC` | Composite of three swept segments (1300→700, 700→850, 850→1300 Hz), repeated 4× | `ripper.c:8266–8275` |
| `PHASER` | Single downward sweep, 2500→50 Hz | `ripper.c:8276–8279` |
| `REVPHASER` | Single upward sweep, 50→2500 Hz | `ripper.c:8280–8283` |

### Mouse — `rv_mouse` (`ripper.c:8290`)

Field width is **4 digits** in graphics mode, **2 digits** in text mode (`ripper.c:8293`, `ripper.c:8300`).

| Variable | Behavior | Cite |
|---|---|---|
| `X` | Mouse X, zero-padded decimal | `ripper.c:8305–8309` |
| `Y` | Mouse Y, zero-padded decimal | `ripper.c:8337–8339` |
| `XY` | Both coordinates in **hex**, format `%0*x:%0*x` | `ripper.c:8312–8320` |
| `XYM` | As `XY` plus `:` and three button bits (left, middle, right as `%d%d%d`) | `ripper.c:8321–8332` |
| `M` | Mouse status/coordinates form | `ripper.c:8340` |
| `MSTAT` | Mouse status | `ripper.c:8290` group |

### Screen save/restore — `rv_save` (`ripper.c:8483`) and `rv_restore` (`ripper.c:8530`)

Eleven screen slots back these: `struct saved_screen screen_saves[11]` — slots 0–9 numbered, slot 10 the default (`ripper.c:320`).

| Variable | Behavior |
|---|---|
| `SAVE`, `SAVE0`–`SAVE9`, `SAVEALL` | Save screen to the default slot, a numbered slot, or all state |
| `SCB` | Save clipboard |
| `SMF` | Save mouse fields |
| `STW` | Save text window |
| `RESTORE`, `RESTORE1`–`RESTORE9`, `RESTOREALL` | Restore from the default slot, a numbered slot, or all state (note the observed asymmetry: `SAVE0` exists, `RESTORE0` does not) |
| `RCB` | Restore clipboard |
| `RMF` | Restore mouse fields |
| `RTW` | Restore text window |

### Reset, erase, disable, kill — `rv_reset` (`ripper.c:8381`), `rv_erase` (`ripper.c:8696`), `rv_mouse_kill` (`ripper.c:8719`), `rv_disable` (`ripper.c:8726`)

| Variable | Behavior |
|---|---|
| `RESET` | Full terminal reset |
| `EGW` | Erase graphics window |
| `ETW` | Erase text window |
| `MKILL` | Kill (remove) mouse fields |
| `DTW` | Disable text window |

### Terminal status queries — `rv_termstat` (`ripper.c:8734`)

| Variable | Behavior |
|---|---|
| `STATBAR` | Status-bar state |
| `TWIN` | Text-window state |
| `TWFONT` | Text-window font |
| `TWW` / `TWH` | Text-window width / height |
| `TWX0` / `TWY0` / `TWX1` / `TWY1` | Text-window corner coordinates |

### Terminal settings — `rv_termset` (`ripper.c:8801`)

| Variable | Behavior | Cite |
|---|---|---|
| `COFF` / `CON` | Cursor off / on (normal) | `ripper.c:8801` group |
| `COMPAT` | Reset to the 640×350 compatibility world frame, clearing coordinate remap tables | `ripper.c:8814–8840` |
| `CURSOR` | Cursor control | |
| `CURX` / `CURY` | Cursor position | |
| `DWAYON` / `DWAYOFF` | "Doorway" mode on / off | |
| `SBARON` / `SBAROFF` | Status bar on / off | |
| `VT102ON` / `VT102OFF` | VT102 mode — stubs, effectively no-ops | `ripper.c:8895–8903` |

### Hotkeys and tab navigation — `rv_hotkey` (`ripper.c:9016`)

| Variable | Behavior | Cite |
|---|---|---|
| `HKEYON` / `HKEYOFF` | Enable / disable hotkey mouse fields | `ripper.c:9016–9024` |
| `TABON` / `TABOFF` | Enable / disable Tab-key navigation between mouse fields (`tab_next_field`, with highlight tracking) | `ripper.c:8974–9014`, `9025–9035` |

Tab-key field navigation has no 1.54 counterpart; it is a keyboard-accessibility extension.

### Application hooks and paste — `rv_exploit` (`ripper.c:9041`), `rv_paste` (`ripper.c:9048`)

| Variable | Behavior | Cite |
|---|---|---|
| `APP0`–`APP9` | Reserved application hooks — stubs; handler only prints a TODO message | `ripper.c:9041–9046` |
| `PCB` | Paste clipboard | `ripper.c:9048` |

## User-Defined Variables (`!\|1D` RIP_DEFINE)

The RIP_DEFINE command handler (`ripper.c:15899–16089`) creates client-side variables, and quotes what appears to be original TeleGrafix specification prose verbatim in its comments (`ripper.c:15971–16000`).

- **Storage**: `struct rip_user_var` (`ripper.c:103–109`) — `name[13]` (1–12 characters plus NUL), string value, field width, persistence flag, linked list.
- **Name validation** (`ripper.c:7774–7789`): alphanumerics and `_` only; the first character may be neither a digit nor an underscore; max 12 characters.
- **Definition text syntax** (`parse_define_text`, `ripper.c:7843–7895`):

  ```
  variable-identifier[,field-width]:[?question-text?][default-value]
  ```

  Example from the in-source spec quote: `FULL_NAME,30:?What is your full name?John Doe`. Field width defaults to 60 when omitted.
- **Flags** (`ripper.c:15924–15928`, spec-quoted values):

  | Value | Meaning |
  |---|---|
  | `001` | Save variable to database (persistent) |
  | `002` | Cannot specify a blank response |
  | `004` | Non-interactive query |

- **Persistence**: `save_persistent_var` / `load_persistent_vars` (`ripper.c:7899–7961`); persistent variables are reloaded during `init_rip` (`ripper.c:19008`).
- **Interactive prompting**: when a definition is interactive (or a non-interactive one finds no existing value), the user is asked via an input dialog (`rip_input_dialog`, `ripper.c:11325–11354`), honoring field width and the no-blank flag.

## Host Command Language Extensions Beyond 1.54

`handle_command_str` (`ripper.c:11884–12001`) processes the command string attached to buttons, hotkeys, and queries. Beyond the documented 1.54 behavior it implements:

### Control-character escapes — `^X` and `` `X ``

Both caret **and backtick** prefixes produce control characters: `^M` or `` `M `` emits CR (`char - '@'`), with `^^` and ` `` ` emitting a literal caret/backtick (`ripper.c:11901–11915`). The backtick form is absent from the 1.54 specification. A comment at `ripper.c:11900` records a notable gap: *RIPterm has no escape for `$` or `[` in host commands* — those characters cannot be sent literally.

### Template-slot dereference — `$?N$`

`$?N$` where `N` is one of the 36 slot characters `0`–`9` / `A`–`Z` expands to the stored template text for that slot (`ripper.c:11918–11923`).

### Inline pick-lists — `((question::opt1,opt2,...))`

A `((...))` construct pops up a menu (`do_popup`, `ripper.c:11356–11635`); an option prefixed `*` forces that answer without user interaction. The selected result is **recursively re-fed** through `handle_command_str` (`ripper.c:11936–11948`), with recursion depth capped at 64 (`ripper.c:11890–11896`).

### Templates — `[N:]...` definition and `[ABC...]` application

- `[N:text]` stores `text` into template slot `N` (36 slots); `[ABC]` applies slots in sequence, expanding the result and re-feeding it (`ripper.c:11949–11982`; template machinery `ripper.c:11637–11882`).
- **Radio-group / checkbox aggregation**: when the defining button belongs to a radio or checkbox group, `process_template_definition` (`ripper.c:11736–11800`) instead concatenates the `template_cmd` of every *selected* button in the group, in button-creation order — turning a form of checkboxes into a single composed host command.

### Query deferral modes

The `!\|1<ESC>` query command takes a mode parameter (`ripper.c:14996–15013`):

| Mode | Effect |
|---|---|
| `0` | Execute the command text immediately |
| `1` | Store as the deferred graphics-click command |
| `2` | Store as the deferred text-click command |

Sending the text `$OFF$` in mode 1 or 2 cancels the stored deferred command.

For the documented ancestors of these mechanisms, see the 2.x pages on [host commands](../../2.x/ripscrip/14-host-commands.md) and [templates](../../2.x/ripscrip/16-templates.md), and the [1.54 text-variable baseline](../../1.5x/ripscrip/15-text-variables.md).

---

[◀ Prev: Reconstructed Command Set](10-reconstructed-command-set.md) · [Contents](README.md)
