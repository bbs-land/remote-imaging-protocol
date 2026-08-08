# Host Commands

[◀ Prev: Level-3 & Level-9 Commands](13-level-3-9-commands.md) · [Contents](README.md) · [Next: Local File Playback & Pop-Up Lists ▶](15-local-playback-popup-lists.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

With mouse regions, buttons, hotkeys and query expressions, RIPscrip gives the host far more than a way to draw pictures: it gives it an **action language**. The RIPscrip 3.0 White Paper names this layer the **Host Command Language (HCL)** and divides it into five areas — control characters, pop-up picklists, local playback directives, template directives, and text variables. A host command is, at its simplest, raw text transmitted to the host when a button or mouse field is clicked; every HCL directive is something the terminal interprets _before_ (or instead of) transmitting.

_Evidence: WP; 2.00a4._

This reference splits the HCL across five pages:

- **This page** — where host commands run, control characters, query deferral, mouse-field identities, and reading/writing user variables from a command string.
- [Local File Playback & Pop-Up Lists](15-local-playback-popup-lists.md) — the `$>` / `$)` / `$<` / `$(` playback directives and `((…))` picklists.
- [Templates & Conditionals](16-templates-and-conditionals.md) — the `[N:]` template system and the 3.0 `<<IF>>` / `<<NAME>>` macro layer.
- [Column Text System](17-column-text-system.md) — the flowed-text/overflow machinery the HCL pagination variables drive.
- [Text Variables](18-text-variables-general.md) — the full inventory of pre-defined `$…$` variables (this and following pages).

## Where Host Commands Run

_Evidence: 2.00a4; SyncTERM (ripper.c:11884–12001); HLP._

A host-command string may be attached to:

- **Buttons** (`1U`) — the command block of the Text Parameter segment, executed when the button is clicked. Only buttons may contain [template directives](16-templates-and-conditionals.md).
- **Mouse fields** (`1M`) — the host command executed when the field is clicked.
- **Queries** (`1` + ESC) — the query text itself is a host-command string, executed or deferred per the mode digit (below).
- **Keystroke macros / hotkeys** — hotkey-triggered buttons execute their command block exactly as if clicked.

The 2.00a4 "what can go where" chart carries forward unchanged in the evidence:

| Context | Templates | Data Text Vars | Active Text Vars | Pick Lists | Ctrl Chars | Local Playback |
| --- | :-: | :-: | :-: | :-: | :-: | :-: |
| Button host commands | Y | Y | Y | Y | Y | Y |
| Simple mouse fields | N | Y | Y | Y | Y | Y |
| Query commands | N | Y | Y | Y | Y | Y |
| Button labels | N | Y | N | N | N | N |
| `RIP_TEXT_XY` / `RIP_TEXT` | N | Y | N | N | N | N |

Host-command processing is **recursive**: the result of a pick-list selection, a template application, or a played-back local file is fed back through the command interpreter. SyncTERM's `handle_command_str` caps this recursion at a depth of 64 (`ripper.c:11890–11896`).

RIPtel additionally exposes a **Command Mode** menu option that executes any typed HCL string directly — TeleGrafix's own recommended way to test host commands interactively. _Evidence: HLP._

## Control Characters

_Evidence: 2.00a4; HLP; SyncTERM (ripper.c:11901–11915)._

A control character is written as a caret (`^`) followed by a character code; the terminal translates the pair into the single ASCII control byte (value = character − `@`). The commonly used codes:

| Sequence    | Meaning                          |
| ----------- | -------------------------------- |
| `^@`        | Null (ASCII 0)                   |
| `^G`        | Beep                             |
| `^H`        | Backspace                        |
| `^L`        | Clear screen (top of form)       |
| `^M`        | Carriage return                  |
| `^C`        | Break (host-dependent)           |
| `^[`        | Escape (ASCII 27)                |
| `^S` / `^Q` | Pause / resume data transmission |

Multi-byte special keystrokes are built from an escape plus a key code:

| Sequence        | Meaning            |
| --------------- | ------------------ |
| `^[[A` / `^[[B` | Up / Down arrow    |
| `^[[C` / `^[[D` | Right / Left arrow |
| `^[[H` / `^[[K` | Home / End         |
| `^[[L`          | Control-Home       |

Because some host software claims the caret for its own purposes, the backquote (`` ` ``) is accepted as a fully interchangeable prefix: `` `M `` is a carriage return exactly as `^M` is. Doubling the prefix character produces a literal: `^^` sends a caret, ` `` ` sends a backquote. Both prefixes are implemented in SyncTERM's command interpreter (`ripper.c:11901–11915`) and both are documented in RIPtel's own help.

> **NOTE:** `^M` is a _notation_, not two transmitted bytes — the terminal translates it before anything is sent. RIPscrip commands themselves (e.g. [queries](11-level-1-commands.md)) use a real ESC byte, never the `^[` notation.

### No Escape for `$` and `[`

_Evidence: SyncTERM (ripper.c:11900)._

The HCL has escape mechanisms for its prefix characters (`^^`, doubled backquote, and `\!` / `\\` / `\|` inside pick-list responses) — but **none for `$` or `[`**. A SyncTERM source comment at `ripper.c:11900` records the finding directly: RIPterm provides no way to place a literal dollar sign or open bracket in a host command; those characters always begin a text-variable reference or a template directive. Authors must simply avoid them in raw host text.

## Query Deferral Modes

_Evidence: SyncTERM (ripper.c:14996–15013); 2.00a4; corpus (MENU.MSE, DEMO-01.COL); HLP._

The query command `1` + ESC (RIP_QUERY) takes a leading mode digit (followed by a window number and two reserved digits) that controls _when_ its host-command text runs:

| Mode | Effect |
| --- | --- |
| `0` | Execute the command text immediately upon receipt |
| `1` | Store the text as the **deferred graphics-click command** — executed when the user clicks graphics outside any mouse field |
| `2` | Store the text as the **deferred text-click command** — executed on a click in the text window |

Sending the text `$OFF$` in mode 1 or 2 cancels the stored deferred command (`ripper.c:14996–15013`).

The RIPtel corpus makes constant use of mode 0 as an "execute now" carrier — nearly every `.DEF` configuration file is a stack of `!|1<ESC>0000$-=NAME=value$` lines that silently set variables. It also uses two further mode digits that no published spec describes:

```text
!|1^[5000$MCURSOR(4)$$>menu.ent$
!|1^[6000$MCURSOR(0)$$>menu.ext$
```

These appear under TeleGrafix's own comment _"Setup our mouse entry/exit queries to do our status line, and mouse cursor changing"_ (MENU.MSE): mode `5` stores a command executed when the pointer **enters** any mouse field, mode `6` when it **exits** _(hypothesis on the exact numbering; the entry/exit behavior itself is confirmed by the comments and by RIPSCRIP.HLP's "mouse-field entry/exit queries")_. RIPSCRIP.HLP strings also reveal _resident queries_, _per-port_ and _per-text-window_ queries beyond these.

## Mouse-Field Identities — `ID=n:`

_Evidence: corpus (DEMO-01.COL, MENU.MSE, MENU.ENT); HLP._

RIPscrip 3.0 lets a mouse field's host command begin with an identity prefix, `ID=` followed by a number and a colon:

```text
!|1M000G7Y8ICC1000000ID=2:$-=RETURN=>NEWSPAPR.RIP$$>TWEATHER.RIP$
!|1M000GCG8GGU1000000ID=3:$>N2_HORO.RIP$
```

The prefix is not transmitted; it assigns the field a numeric identity that the pre-defined data variable `$FIELDID$` ("current mouse field ID" — RIPTEL.HLP) reports back. Combined with entry/exit queries this produces per-field behavior from a _single_ shared handler. RIPtel's main menu is the canonical example: nine fields carry `ID=1:` through `ID=9:`, the entry query plays MENU.ENT, and MENU.ENT draws a status line with:

```text
!|@HSKA$&MSG<<FIELDID>>$
```

— the `<<FIELDID>>` [macro expansion](16-templates-and-conditionals.md) composes the variable name `MSG1`…`MSG9`, and the `$&…$` dereference (below) prints whichever hover text the `.DEF` file assigned to that field.

## Setting & Reading User Variables from Host Commands

_Evidence: HLP; corpus (NEWSPAPR.RIP, MENU.DEF, SHOWFONT.FN); 2.00a4; SyncTERM (ripper.c:7774–7789)._

The full reference syntax for a user-defined text variable inside a host command is unchanged in shape from 2.x, and RIPTEL.HLP documents it completely:

```text
$[directives][x,y:]NAME[(MODE[,CONV])][,width][@question][=default]$
```

| Element | Meaning |
| --- | --- |
| directives | Prefix characters, any order, each at most once (table below) |
| `x,y:` | Pop-up dialog position in world coordinates; omit either value (keep the comma) to center on that axis — `$,:NAME$` centers both |
| `(MODE[,CONV])` | Data mode `ANY` / `ALPHA` / `NUMBER` / `ALPHANUM`, optional conversion `TONAME` / `TOUPPER` / `TOLOWER`; conversion-only form (`(TONAME)`) is legal, conversion _before_ mode is a syntax error |
| `,width` | Data-entry field width in columns |
| `@question` | Custom prompt (no `$` allowed inside) |
| `=default` | Default response |

The directive characters, per RIPTEL.HLP:

| Char | Meaning                                                         |
| ---- | --------------------------------------------------------------- |
| `*`  | Answer required — the prompt cannot be cancelled                |
| `+`  | Save the variable permanently to the database (`RIPSCRIP.DB`)   |
| `%`  | Save to the internal memory table (lost at hang-up)             |
| `#`  | Don't echo keystrokes — show `#`s (passwords)                   |
| `-`  | **Set** the variable without prompting the user                 |
| `&`  | **Retrieve** the variable's contents without prompting the user |

If the user cancels a non-required prompt, the value `NULL` is inserted. Limits per RIPTEL.HLP: variable names up to **20 characters** (letters, digits, underscore; must start with a letter; auto-uppercased), questions and defaults up to **100 characters**, values up to **255 characters**. SyncTERM's reconstruction diverges on the name limit — its `rip_user_var` struct allows only 12 characters (`ripper.c:7774–7789`) — and RIPtel's help lists `%` where the 2.00a4 draft used `=` for the memory-table directive. The corpus shows `=` still accepted (below), so `%` and `=` are best read as synonyms for the memory directive _(hypothesis)_.

### The Set-Syntax: `$-=NAME=value$`

_Evidence: corpus (122 distinct occurrences; NEWSPAPR.RIP, MENU.DEF, TELLISTS.MSE)._

The single most common HCL construct in the RIPtel corpus — 122 distinct forms — is the silent assignment:

```text
!|1^[0000$-=MAIN_STORY=story01.txt$|!     Filename containing main story
!|1^[0000$-=MAIN_AUTHOR=Jeff Reeder$|!    Author of main story
!|1^[0000$-=RETURN=>TELDEMOS.RET$
```

This is not new syntax but a _composition_ of the directive characters above: `-` (set without prompting) + `=` (memory table) + `=value` (default supplying the value). Nothing is prompted, nothing is transmitted — the variable is simply defined. MENU.DEF confirms the decomposition by writing the same two directives in the opposite order: `$=-MSG1=Visit your favorite Telnet sites$`. Whole `.DEF` files are nothing but these lines, turning scene scripts into configurable applications.

### The Dereference: `$&NAME$`

_Evidence: corpus (SHOWFONT.FN, NEWSPAPR.RIP, MENU.ENT); 2.00a4._

The `&` directive was defined in 2.x as _transparent retrieval to the host_. The 3.0 corpus shows its far more powerful role: **inline interpolation into command arguments**. A `$&NAME$` expands to the variable's value wherever it appears — including inside the argument block of drawing commands:

```text
!|@I064$&FONT_NAME$:                    (text argument of RIP_TEXT_XY)
!|y0000010X000000001S1a000000Marin
!|@4L2L$&MAIN_TITLE$                    (headline drawn from a variable)
!|1R00000000$&MAIN_STORY$               (RIP_READ_SCENE filename from a variable)
```

Even the outline-font _name_ argument of `y` (RIP_EXTENDED_FONT_STYLE) can be `$&FONT_NAME$`. Together with the set-syntax this is the parameter-passing mechanism of the whole RIPtel demo: a scene sets variables, then calls a generic `.FN` "function" scene that dereferences them.

### Defining Variables from the Wire — RIP_DEFINE (`1D`)

_Evidence: SyncTERM (ripper.c:15899–16089, 7843–7895); HLP._

The host can also create variables with the level-1 `D` command rather than through a host command. SyncTERM's handler quotes what appears to be original TeleGrafix specification prose: the definition text is

```text
variable-identifier[,field-width]:[?question-text?][default-value]
```

e.g. `FULL_NAME,30:?What is your full name?John Doe`, with field width defaulting to 60, and a flags word of `001` (save to database), `002` (blank response not allowed), `004` (non-interactive). Persistent variables live in `RIPSCRIP.DB` — RIPtel ships the file with header string "RIPscrip Text Variable Database". The host may also _delete_ variables; RIPtel confirms with the prompt _"The host wants to delete the variable %s. Proceed?"_, and protected variables trigger a "Data Security Activated" message — the data-security concern the 2.x specification urged implementors to address.

## Launching Web URLs — `$GOTOURL(NAME)$`

_Evidence: corpus (TELLISTS.MSE); HLP._

New to the 3.0 era — and pure 1997 — is web integration from a mouse field. `$GOTOURL(NAME)$` launches the URL held in user variable `NAME` in the system browser. RIPtel's Telnet Site Listings screen wires six fields this way:

```text
!|1M000ZACBSE41000000ID=1:$-=WEBURL=http://duke.usask.ca/~scottp/free.html$$GOTOURL(WEBURL)$
!|1M00CDACNAE41000000ID=2:$-=WEBURL=http://library.usask.ca/hytelnet$$GOTOURL(WEBURL)$
```

Each click silently sets `WEBURL` with the set-syntax, then invokes the launcher — a two-step idiom that keeps `$GOTOURL$` itself generic.

---

[◀ Prev: Level-3 & Level-9 Commands](13-level-3-9-commands.md) · [Contents](README.md) · [Next: Local File Playback & Pop-Up Lists ▶](15-local-playback-popup-lists.md)
