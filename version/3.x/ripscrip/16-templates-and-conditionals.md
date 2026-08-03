# Templates & Conditionals

[◀ Prev: Local File Playback & Pop-Up Lists](15-local-playback-popup-lists.md) · [Contents](README.md) · [Next: Column Text System ▶](17-column-text-system.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

Templates are the Host Command Language's mechanism for *building* host commands out of pieces — the White Paper calls them "a cookie-cutter approach to host commands," constructed from whichever buttons are selected and which are not. The 2.x template system carries into 3.0 intact; layered on top of it, the 3.0 era adds something no published specification describes: a genuine **macro and conditional layer** (`<<NAME>>`, `<<IF>>…<<ELSE>>…<<ENDIF>>`) evaluated before command execution. This page covers both.

*Evidence: WP; 2.00a4; corpus.*

## Command Blocks and the Three Host-Command Types

*Evidence: 2.00a4.*

Templates exist only in **button** (`1U`) host commands — not in mouse fields or queries. A button's host-command segment may be sub-divided into multiple **command blocks** with the two-character delimiter `[]`, and each block is one of three types:

1. **Direct host command** — raw text (plus HCL directives) transmitted immediately when the button is clicked.
2. **Template definition** — `[N:]text` stores text into a template slot; nothing is transmitted.
3. **Template execution** — `[ABC]text` feeds text through one or more slots and transmits the result.

A single host command can do all three at once:

```text
[3:]Template Definition[4]Template chaining[]BBS TEXT
```

A command with no block delimiters at all is simply a direct host command. If two template definitions in one command string target the same slot, the **last one wins** — earlier definitions in the same string are discarded, even for check-box buttons.

## Defining Templates — `[N:]`

*Evidence: 2.00a4; SyncTERM (ripper.c:11949–11982, 11637–11882).*

There are **36 template slots**, identified by the single meganum characters `0`–`9` and `A`–`Z`, one per button group. SyncTERM implements the full store/apply machinery (`ripper.c:11637–11882`).

```text
[5:]This is template #5's definition
[G:]This is template #16's definition
```

Crucially, a definition is *inert on receipt*: it becomes the **active** template for its group only when its button is clicked (or drawn pre-selected). Several buttons in one group may each carry a definition for the same slot; whichever was clicked most recently owns the slot.

## Radio Buttons and Check-Box Aggregation

*Evidence: 2.00a4; SyncTERM (ripper.c:11736–11800).*

- **Radio groups** — exactly one button active at a time, so exactly one definition active per slot.
- **Check-box groups** — zero or more buttons active. On every click the group's template is *recalculated*: the definitions of all currently-selected buttons are concatenated **in button-creation order**, not click order. SyncTERM's `process_template_definition` (`ripper.c:11736–11800`) implements exactly this — when the defining button belongs to a radio or check-box group, it concatenates the `template_cmd` of every selected button in the group, in creation order.

Seven check-box fruits defined as `[2:]APPLES^m`, `[2:]ORANGES^m`, … clicked in the order 2, 1, 4 still yield the template `Apples^mOranges^mGrapes^m` — definition order, always.

## Template Embedding — `$?N$`

*Evidence: 2.00a4; SyncTERM (ripper.c:11918–11923).*

`$?N$` — where `N` is a slot character `0`–`9` / `A`–`Z` — expands, inside a direct host command, to the stored text of that slot (`ripper.c:11918–11923`). The classic order form:

```text
Submit button:  I wish to order $?2$ right now^m
Check boxes:    [2:]APPLES^m   [2:]CHERRIES^m   ...
```

Clicking Submit with Apples and Cherries checked transmits `I wish to order APPLES^mCHERRIES^m right now^m`. An empty check-box template embeds as nothing; an empty **radio** template instead makes the terminal highlight the group and insist the user pick one — radio groups must always resolve to a selection.

## Template Chaining — `[ABC]`

*Evidence: 2.00a4; SyncTERM (ripper.c:11949–11982).*

Chaining is embedding's inverse: instead of inserting a template into a host command, it feeds a host command *into* templates. The generic insertion code `$?$` (no slot character) inside a template definition marks where fed text lands:

```text
[4:]This template inserts $?$ here!
[4]SOMETHING              →  This template inserts SOMETHING here!
```

Multiple identifiers chain left to right — `[1E3]text` feeds `text` through slot 1, that result through slot E, and that through slot 3 before transmission. Up to 36 chain levels are allowed in one operation.

Embedding and chaining combine freely — `[3:]Here is a $?$ and an $?5$` mixes both — with one guard against runaway expansion: embedded templates may nest only **two levels**; at the second level the text is raw, and insertion codes, text variables, control characters and pick lists in it are not processed. Directives in a template are processed when the template is **used**, not when it becomes active — a `$TIME$` in a template reads the clock at transmission time.

The final host command after all template processing is limited to **4096 bytes**; anything beyond is truncated.

## The 3.0 Macro Layer — `<<NAME>>`

*Evidence: corpus (MENU.DEF, MENU.MNU, MENU.MSE, MENU.ENT); SyncTERM absent.*

Beyond the slot templates, the RIPtel corpus reveals a second, entirely undocumented expansion mechanism: `<<NAME>>` expands to the value of the text variable `NAME` — anywhere in a command's argument text, including *inside* a `$…$` reference. No surveyed non-TeleGrafix implementation supports it.

The demo's menu system is built on it. A `.DEF` configuration file sets plain variables with the [set-syntax](14-host-commands.md):

```text
!|1^[0000$-=LAB2=Demos$
!|1^[0000$-=CMD2=>TELDEMOS.FN$
!|1^[0000$=-MSG2=Demonstrations of RIPscrip graphics$
```

and the *generic* menu scenes consume them by macro:

```text
!|1b3Y7YCCAY0000000000TELBUT.BMP|@809U<<LAB1>>       (button label text)
!|1M003W7YCEAW1000000ID=1:$<<CMD1>>$                 (host command, composed inside $...$)
!|@HSKA$&MSG<<FIELDID>>$                             (macro composing a variable NAME)
```

The second line is the striking one: `$<<CMD1>>$` expands `<<CMD1>>` to `>TELDEMOS.FN` *first*, yielding `$>TELDEMOS.FN$` — a [local playback directive](15-local-playback-popup-lists.md) assembled at run time. The third composes a variable *name* from the built-in `$FIELDID$`, then dereferences it with `$&…$`. One `.MSE` overlay and one `.MNU` screen thus serve every menu in the product; only the tiny `.DEF` files differ. Expansion evidently happens before `$…$` parsing *(hypothesis on the exact evaluation order)*.

## Inline Conditionals — `<<IF>> … <<ELSEIF>> … <<ELSE>> … <<ENDIF>>`

*Evidence: corpus (BUTTONS.RIP, TELDEMOS.RET, MENU.RET, SPECLEFX.RIP, FXSHWIMG.FN); SyncTERM absent.*

The same `<<…>>` bracket introduces conditionals, evaluated before the containing command executes. Keywords are case-insensitive (`<<IF>>` and `<<if>>` both appear). The observed grammar:

```text
<<IF expr>> text <<ELSEIF expr>> text <<ELSE>> text <<ENDIF>>
```

with expressions built from `$…$` values, quoted string literals, the comparison operators `=` `!=` `<`, the connectives `AND` / `OR`, and parentheses. All from shipping TeleGrafix scripts:

**Choosing a background by color depth** — the most common idiom, appearing as the *filename argument* of a `1R` (RIP_READ_SCENE):

```text
!|1R00000000<<IF $COLORS$<"256">>BLUEBACK.FN<<ELSE>>BLUEFADE.FN<<ENDIF>>
```

**A computed "Back" button** — as a mouse-field host command:

```text
!|1M000M0B37121000000ID=1:<<if $RETURN$!="">>$<<RETURN>>$<<else>>$NULL$<<endif>>
```

If a return scene was recorded (`$-=RETURN=>NEWSPAPR.RIP$`), play it; otherwise do nothing (`$NULL$`).

**Compound conditions** — connectives and parentheses (TELDEMOS.RET, MENU.RET):

```text
<<IF $NO_WIPES$="" OR $NO_WIPES$="NONE">>$>TELPORT.FN$<<ELSE>>$RESET$<<ENDIF>>
<<IF $TGMENU_WIPES$="1" and ($NO_WIPES$="" or $NO_WIPES$="NONE")>>$>WIPE01.FN$<<else>>$NULL$<<ENDIF>>
```

**The slideshow idiom** — the Special Effects demo's image cycler, rotating the same JPEGs that N2_PHOTO.RIP's newspaper gallery displays. The display subroutine FXSHWIMG.FN seeds the rotation on first use, testing whether a variable exists at all via `$INUSE(TV,name)$` (`TV` = the text-variable object class; returns `0`/`1`):

```text
!|1p0000<<IF $INUSE(TV,NEXT_IMG)$="0">>$-=NEXT_IMG=ASTRO.JPG$<<ENDIF>>$&NEXT_IMG$
```

and the "Next Image" button (SPECLEFX.RIP) advances it — a multi-branch `<<ELSEIF>>` spread over continued lines (`\` at line end continues the command):

```text
!|1U4GCUA2E60000<>Next Image<>\
<<IF $NEXT_IMG$="ASTRO.JPG">>$-=NEXT_IMG=JUPITER.JPG$\
<<ELSEIF $NEXT_IMG$="JUPITER.JPG">>$-=NEXT_IMG=BEACH2.JPG$\
<<ELSE>>$-=NEXT_IMG=ASTRO.JPG$\
<<ENDIF>>\
$>FXSHWIMG.FN$
```

State machine, existence test, silent assignment, computed playback: a complete client-side program in five lines of button command. The matching cleanup is `$RESET(TV,NO_WIPES)$`, deleting a variable by name.

### Comparison Semantics

*Evidence: corpus; editorial analysis.*

Operands are string-valued, and `$COLORS$<"256"` is the only ordering comparison observed. For the values that occur in practice (`16` vs `256` vs `65536`) lexical and numeric comparison happen to agree, so the corpus cannot distinguish which the driver performs — treat `<` on numbers other than these as unverified *(hypothesis)*. Equality against `""` tests emptiness/undefinedness and is used interchangeably with `="NONE"` sentinels by TeleGrafix's own scripts.

---

[◀ Prev: Local File Playback & Pop-Up Lists](15-local-playback-popup-lists.md) · [Contents](README.md) · [Next: Column Text System ▶](17-column-text-system.md)
