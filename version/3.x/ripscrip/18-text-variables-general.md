# Text Variables: General, Date/Time & Sound

[◀ Prev: Column Text System](17-column-text-system.md) · [Contents](README.md) · [Next: Text Variables: Mouse, Text Window & Ports ▶](19-text-variables-mouse-window.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

This page begins the four-part text-variable reference, grouped the same way as the [2.x edition](../../2.x/ripscrip/17-text-variables-general.md): general/date/time/sound here, then [mouse, text window & ports](19-text-variables-mouse-window.md), [terminal & reset](20-text-variables-terminal.md), and [environment, clipboard, screen & tables](21-text-variables-environment.md).

The 3.0 inventory is known from three overlapping sources: the RIPtel 3.1 help files document roughly 60 data-returning and 60 command-performing variables (`HLP`); the demo corpus shows **269 distinct `$…$` forms** actually used in shipping scripts (`corpus`); and SyncTERM's open-source implementation carries a **103-entry built-in table** (`SyncTERM`). Where the sources disagree, the divergence is called out on the entry.

## Variable Syntax & Resolution

A text variable is written `$NAME$` — or `$NAME(param,param,…)$` for the parameterized forms — and may be embedded in host commands, button commands, queries, and templates. The 2.00a4 `req:`/`opt:` parameter-notation conventions are inherited unchanged; see the [2.x syntax-description primer](../../2.x/ripscrip/17-text-variables-general.md#text-variable-syntax-descriptions). _(2.00a4)_

SyncTERM's `get_text_variable` resolves a reference in three steps:

1. **Built-ins** — a case-insensitive `bsearch` over the sorted `builtins[]` table.
2. **User-defined variables** on a miss (see [below](#user-defined-variables)).
3. **Empty string** if neither matches — an unknown variable expands to nothing; it is never an error.

Case-insensitivity is not just an implementation detail: TeleGrafix's own demo scripts freely mix case (`$dtw$` in CURVES.RIP, `$overflow(1,cur)$` in the .COL scenes).

Several **prefix forms** are recognized _before_ name lookup and are documented elsewhere:

| Form | Meaning | Reference |
| --- | --- | --- |
| `$>FILE$` `$)FILE$` `$<FILE$` `$(FILE$` | Local playback of a RIP scene, WAV, BMP, or JPEG | [Local File Playback & Pop-Up Lists](15-local-playback-popup-lists.md) |
| `$-=NAME=value$` | Set a user variable silently | [below](#silent-set--dereference) |
| `$&NAME$` | Dereference a user variable silently | [below](#silent-set--dereference) |
| `$?N$` | Expand template slot N | [below](#template-slot-dereference-n) and [Templates & Conditionals](16-templates-and-conditionals.md) |
| `$*…$` `$+…$` `$%…$` `$#…$` | User-variable prompt directives | [below](#directive-characters) |

_Evidence: SyncTERM (ripper.c:7963–7979) resolution order; SyncTERM (ripper.c:7633–7738) sorted built-in table; corpus (CURVES.RIP, DEMO-01.COL) case-insensitive usage; 2.00a4 syntax conventions._

## Version & Identity Variables

| Variable | Behavior | Evidence |
| --- | --- | --- |
| `$RIPVER$` | Returns the terminal's RIPscrip version string. The HLP shows the `RIPSCRIP015300`-style format (`RIPSCRIP0` + major + minor×100, zero-padded); SyncTERM returns `RIPSCRIP015410` or `RIPSCRIP030001` depending on its emulation mode. Distinct from the shorter `RIPSCRIP03000` auto-detect handshake reply (see [Protocol Definition](07-protocol-definition.md)). | 2.00a4; HLP; SyncTERM (ripper.c:8055–8058) |
| `$TERMINFO(keyword)$` | Terminal software identification. 2.00a4 mandates keywords `NAME`, `VENDOR`, `VERSION`, `LIST`; unknown keywords return `NONE`. Present in the RIPSCRIP.DLL processor table (`tvarProcTERMINFO`). | 2.00a4; HLP |
| `$IFS(keyword,category)$` | "Is Feature Supported" capability query, with `LIST` and `_CATEGORY` directives as documented for 2.00a4. Present in the DLL processor table ("IFS (with LIST/category keywords)"). | 2.00a4; HLP |
| `$NULL$` | Expands to nothing; place-holder for commands that require a text parameter. Heavily used by the demo scripts as the empty branch of `<<IF>>` conditionals (41 uses). | 2.00a4; HLP; corpus (BUTTONS.RIP, MENU.RET) |
| `$RAND$` | Random value. Listed in the HLP data-variable table; parameterization (range) unconfirmed _(hypothesis)_. Used by the demo menus to pick a random screen wipe. | HLP; corpus (TELCMDS.RET "Choose a random wipe") |
| `$USERNAME$` | User-ID stored in the current dialing-directory bookmark. New in the 3.x client environment. | HLP |
| `$PASSWORD$` | Password stored in the current bookmark ("Data Security" prompts guard host access to protected data). | HLP |

_Evidence: HLP (RIPTEL.HLP text-variable chapter; RIPSCRIP.HLP tvarProc string table); 2.00a4; SyncTERM (ripper.c:8055)._

## Date Variables

The full 1.54/2.00a4 date group survives intact in 3.0; all thirteen are in the HLP table and in SyncTERM's `rv_date` handler group.

| Variable | Behavior | Evidence |
| --- | --- | --- |
| `$ADOW$` | Abbreviated day-of-week name (`Sun`–`Sat`) | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$DATE$` | Date in short `MM/DD/YY` format | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$DATETIME$` | Combined date and time, Unix `ctime` style (`Sat Dec 19 14:38:50 1993`) | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$DAY$` | Day of month, `01`–`31` | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$DOW$` | Day of week, fully spelled out | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$DOY$` | Day of year, `001`–`366` | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$FYEAR$` | Four-digit year | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$MONTH$` | Month name, unabbreviated | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$MONTHNUM$` | Month number, `01`–`12` | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$WDAY$` | Day of week as a digit, `0`–`6` (0 = Sunday) | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$WOY$` | Week of year `00`–`53`, Sunday-first | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$WOYM$` | Week of year `00`–`53`, Monday-first | 2.00a4; HLP; SyncTERM (ripper.c:8061) |
| `$YEAR$` | Two-digit year | 2.00a4; HLP; SyncTERM (ripper.c:8061) |

_Evidence: 2.00a4; HLP; SyncTERM (ripper.c:8061 `rv_date`)._

## Time Variables

| Variable | Behavior | Evidence |
| --- | --- | --- |
| `$AMPM$` | `AM` or `PM` | 2.00a4; HLP; SyncTERM (ripper.c:8148) |
| `$HOUR$` | Hour, 12-hour style, `01`–`12` | 2.00a4; HLP; SyncTERM (ripper.c:8148) |
| `$MHOUR$` | Hour, 24-hour ("military") style, `00`–`23` | 2.00a4; HLP; SyncTERM (ripper.c:8148) |
| `$MIN$` | Minutes, `00`–`59` | 2.00a4; HLP; SyncTERM (ripper.c:8148) |
| `$SEC$` | Seconds, `00`–`59` | 2.00a4; HLP; SyncTERM (ripper.c:8148) |
| `$TIME$` | `HH:MM:SS`, 24-hour format | 2.00a4; HLP; SyncTERM (ripper.c:8148) |
| `$TIMEZONE$` | Time-zone abbreviation, or `NONE` if not configured | 2.00a4; HLP; SyncTERM (ripper.c:8148) |

_Evidence: 2.00a4; HLP; SyncTERM (ripper.c:8148 `rv_time`)._

## Sound Variables

All sound variables are _actions_: they expand to an empty string and play a PC-speaker effect. 2.00a4 defines optional parameters for every one of them (repeat counts, frequency/duration pairs, sweep parameters — see the [2.x reference](../../2.x/ripscrip/17-text-variables-general.md#sound-related-text-variables)); the HLP confirms the DLL validates start/stop frequency ordering and rejects zero increments/durations. SyncTERM implements the default (parameterless) behaviors, two of them with source comments reading `// Literally in the spec.`:

| Variable | Behavior | Evidence |
| --- | --- | --- |
| `$ALARM(count)$` | Failure/warning sound: `count`× (320 Hz 200 ms + 160 Hz 425 ms), default 3 repeats | 2.00a4; HLP; SyncTERM (ripper.c:8244–8253) |
| `$BEEP(freq,len)$` | Ctrl-G beep: default 1000 Hz for 75 ms, then a 75 ms silent gap (`SLEEP(75)` — "Literally in the spec.") | 2.00a4; HLP; SyncTERM (ripper.c:8257–8258) |
| `$BLIP(freq,len)$` | Barrier "bump": default 50 Hz for 25 ms, then a 10 ms gap (`SLEEP(10)` — "Literally in the spec.") | 2.00a4; HLP; SyncTERM (ripper.c:8261–8262) |
| `$MUSIC(count)$` | Success jingle: SyncTERM realizes it as a composite of three swept segments (1300→700, 700→850, 850→1300 Hz) repeated 4×, matching the 2.00a4 nine-step tone table | 2.00a4; HLP; SyncTERM (ripper.c:8266–8275) |
| `$PHASER(start,stop,inc,time)$` | Descending frequency sweep, default 2500→50 Hz | 2.00a4; HLP; SyncTERM (ripper.c:8276–8279) |
| `$REVPHASER(start,stop,inc,time)$` | Ascending sweep, default 50→2500 Hz | 2.00a4; HLP; SyncTERM (ripper.c:8280–8283) |
| `$T(freq,len)$` | Single raw tone with **no** trailing gap — chainable into melodies; default 1000 Hz / 75 ms | 2.00a4; HLP (`tvarProcT`) |

_Evidence: 2.00a4; HLP (tone-command error strings); SyncTERM (ripper.c:8235 `rv_sound`)._

## User-Defined Variables

The user-variable system is 3.0's client-side data store: the host defines named variables, the terminal prompts the user (or not), and later `$NAME$` references expand to the stored value — including as arguments _inside RIPscrip commands_ (the demo corpus passes `$&FONT_NAME$` as a font name and `$&MAIN_STORY$` as a filename).

### Definition via RIP_DEFINE (`!|1D`)

The wire command [RIP_DEFINE](11-level-1-commands.md) creates a variable. SyncTERM's handler quotes what appears to be original TeleGrafix specification prose in its comments, giving the definition-text grammar:

```text
variable-identifier[,field-width]:[?question-text?][default-value]
```

Example (from the in-source spec quote): `FULL_NAME,30:?What is your full name?John Doe`. Field width defaults to 60 when omitted. The command's numeric flags are `001` save to database (persistent), `002` a blank response is not allowed, `004` non-interactive query. When a definition is interactive — or a non-interactive one finds no stored value — the terminal raises an input dialog honoring the field width and no-blank flag.

_Evidence: SyncTERM (ripper.c:15899–16089) handler with spec-quoted comments; SyncTERM (ripper.c:7843–7895) `parse_define_text`; SyncTERM (ripper.c:15924–15928) flags; SyncTERM (ripper.c:11325–11354) input dialog._

### Inline prompt syntax (`$…NAME…$`)

The HLP documents a second, richer definition/prompt syntax usable directly in the macro language:

```text
$[directives][x,y:]NAME[(MODE[,CONV])][,width][@question][=default]$
```

An empty user response inserts the literal text `NULL`. Examples straight from the help file:

```text
$*+20,50:NAME(ToName),30@What's your name?=John Doe$
$*#20,10:PASSWORD,10@Please enter your password$
```

_Evidence: HLP (RIPTEL.HLP "Text Variables" chapter, complete grammar)._

### Directive characters

Any combination, in any order, between the opening `$` and the name; each may appear once:

| Directive | Meaning | Evidence |
| --- | --- | --- |
| `*` | Answer required — the user cannot cancel the prompt | HLP |
| `+` | Save the variable permanently to the [RIPSCRIP.DB database](22-file-formats.md#ripscripdb--text-variable-database) | HLP |
| `%` | Save to the internal memory table only (lost when the session hangs up) | HLP |
| `#` | Don't echo keystrokes — display `#` characters (password entry) | HLP |
| `-` | Set the variable without prompting the user | HLP; corpus (122 distinct `$-=…$` forms) |
| `&` | Retrieve the variable's contents without prompting the user | HLP; corpus (9 distinct `$&…$` forms) |

### Placement (`x,y:`)

An optional `x,y:` prefix positions the prompt dialog in world coordinates. Either value may be omitted (keeping the comma) to center on that axis; `$,:NAME$` centers on both. _(HLP)_

### Format classes and conversions

`(MODE[,CONV])` after the name constrains and transforms input:

| Keyword               | Meaning                              | Evidence |
| --------------------- | ------------------------------------ | -------- |
| `ANY`                 | Any characters accepted (default)    | HLP      |
| `ALPHA`               | Alphabetic characters only           | HLP      |
| `NUMBER`              | Numeric input only                   | HLP      |
| `ALPHANUM`            | Letters and digits only              | HLP      |
| `TONAME`              | Convert to name-style capitalization | HLP      |
| `TOUPPER` / `TOLOWER` | Force case                           | HLP      |

A conversion-only form such as `(TONAME)` is legal; writing the conversion _before_ the mode is a syntax error. _(HLP)_

### Width, question, default

- `,width` — data-entry field width in columns.
- `@question` — custom prompt text (no `$` allowed inside).
- `=default` — default response.

_(HLP)_

### Limits

| Item | Limit | Evidence |
| --- | --- | --- |
| Variable name | **20** characters — letters, digits, underscore; must start with a letter; auto-uppercased | HLP |
| Question / default text | **100** characters each | HLP |
| Variable content (value) | **255** characters | HLP |

> **Divergence:** SyncTERM caps variable names at **12** characters (`name[13]` storage; validation additionally forbids a leading digit _or underscore_). Scripts targeting both should stay within 12 characters. _(SyncTERM (ripper.c:103–109, 7774–7789))_

### Persistence

Variables defined with the `+` directive (or RIP_DEFINE flag `001`) are written to `RIPSCRIP.DB`, the client-side [text-variable database](22-file-formats.md#ripscripdb--text-variable-database), and survive across sessions. SyncTERM mirrors this with `save_persistent_var`/`load_persistent_vars`, reloading persistent variables during interpreter startup.

_Evidence: HLP (RIPSCRIP.DB header "RIPscrip Text Variable Database"); SyncTERM (ripper.c:7899–7961, 19008)._

### Silent set & dereference

- **`$-=NAME=value$`** assigns a value without prompting. This is the workhorse of the demo corpus — 122 distinct set-forms observed, wiring menu state: `$-=RETURN=>NEWSPAPR.RIP$`, `$-=WEBURL=http://…$`, `$-=TITLE=Telnet Site Listings$`.
- **`$&NAME$`** expands the stored value without any prompt, usable inline in command arguments: `$&FONT_NAME$` as the font-name field of an extended-font-style command, `$&MAIN_STORY$` as a read-scene filename, even computed names via macro expansion (`$&MSG<<FIELDID>>$`).

_Evidence: HLP (`-` and `&` directives); corpus (TELLISTS.MSE, SHOWFONT.FN, NEWSPAPR.RIP)._

### Template-slot dereference (`$?N$`)

`$?N$` — where `N` is one of the 36 slot characters `0`–`9`/`A`–`Z` — expands to the stored [template](16-templates-and-conditionals.md) text for that slot. _(SyncTERM (ripper.c:11918–11923))_

### Host deletion and data queries

The host may delete a variable; RIPtel asks the user to confirm ("The host wants to delete the variable %s. Proceed?"). A _data query_ is simply a host-sent template of variable references — e.g. `$FULL_NAME$^m$ST_ADDR$^m$CITY$, $STATE$ $ZIP$^m` — which the terminal returns filled in; protected variables trigger a "Data Security Activated" notice. _(HLP)_

---

[◀ Prev: Column Text System](17-column-text-system.md) · [Contents](README.md) · [Next: Text Variables: Mouse, Text Window & Ports ▶](19-text-variables-mouse-window.md)
