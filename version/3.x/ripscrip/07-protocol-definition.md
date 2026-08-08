# Protocol Definition & Syntax

[◀ Prev: Color, Audio & Text Windows](06-color-audio-text.md) · [Contents](README.md) · [Next: Level-0 Commands (Symbols & A–F) ▶](08-level-0-commands-symbols-a-f.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

## Auto-Sensing and Version Identification

*Evidence: 2.00a4; HLP (RIPTEL.HLP FAQ); SyncTERM (ripper.c:7619, 18437–18470).*

RIPscrip is predominantly non-ANSI, but four ANSI-style escape sequences control detection and enablement:

| Sequence | Effect |
|---|---|
| `ESC[!` | Query RIPscrip version — the terminal replies with its version string (below) |
| `ESC[0!` | Same as `ESC[!` |
| `ESC[1!` | Disable all RIPscrip processing (sequences render as raw text) |
| `ESC[2!` | Enable RIPscrip processing |

Hosts auto-sense a RIP terminal by sending the query at login and sniffing for the reply; a non-RIP terminal ignores the sequence. The reply format is the literal text `RIPSCRIP` followed by version digits in **`MMmmrr`** form (2-digit major, minor, revision), optionally followed by a 2-character **`pp`** vendor/patch suffix (vendor code + sub-version, per the 1.54 convention).

The 3.0-era replies on record:

- **`RIPSCRIP03000`** — the reply RIPtel's own FAQ documents (RIPTEL.HLP: the string appears on the login line when MajorBBS/WorldGroup hosts time out waiting for it; servers allow only ~1–1.5 s). Read as major 03, minor 00, revision 0 — 13 bytes, without the vendor suffix.
- **`RIPSCRIP030001`** — SyncTERM's 14-byte reply (`ripver[]` at ripper.c:7619, sent without CR/LF via `conn_send(…, 14, 1000)` at ripper.c:18447): major 03, minor 00, revision 01. For comparison, its RIPv1 reply is `RIPSCRIP015410` (v1.54, vendor 1, sub-rev 0).

Host software probing for "any RIP 3.0" should therefore match on the prefix `RIPSCRIP03` rather than a fixed length. The same string is available to host scripts as the **`$RIPVER$`** text variable (RIPTEL.HLP's own example for it is `RIPSCRIP015300`).

Two behavioral notes from SyncTERM. First, its `ESC[1!` handling is a one-way door — once disabled, its parser never sees the `ESC[2!` that should re-enable it (ripper.c:18453–18459). **This is a SyncTERM bug, not a protocol delineation**: the language defines disable (`ESC[1!`) and enable (`ESC[2!`) as a reversible pair, and a conforming terminal must resume RIPscrip processing when it receives `ESC[2!`. Do not treat SyncTERM's inability to restart RIP processing as license to drop the feature — implementations should keep watching for `ESC[2!` while disabled. Second, the RIP version SyncTERM answers with is per-BBS *configuration*, not negotiated from the wire. The 2.00a4 note also stands: for identifying the specific terminal product, the query's vendor code is obsolete in favor of `$TERMINFO()$`/`$IFS()$`.

## Command Syntax — General Structure

*Evidence: 2.00a4; SyncTERM; corpus.*

RIPscrip statements are lines of 7-bit printable ASCII intermixed with ordinary text. The rules below merge the 2.00a4 language definition with the byte-exact behavior of SyncTERM's 3.x parser (`cb_feed`, ripper.c:18516–18608) and the shipping driver's observed tolerance in the RIPtel corpus.

### Introducers

*Evidence: 2.00a4 rules 1–2, 12; SyncTERM (ripper.c:18189–18202).*

Three bytes can begin a RIP command sequence:

- **`!`** (exclamation mark, 33) — the standard introducer, valid **only at the beginning of a line**. SyncTERM implements "beginning of line" precisely: the previously emitted byte must be NUL, CR, LF, or VT (ripper.c:18198–18200), *or* the text cursor must already sit at column 1 — e.g. placed there by an ANSI cursor-position sequence (ripper.c:18201, rationale comment 18180–18188). The mid-line restriction exists so users can't inject RIP sequences into teleconference text.
- **SOH** (Ctrl-A, `0x01`) and **STX** (Ctrl-B, `0x02`) — accepted **anywhere** in the stream (ripper.c:18196–18197). Since BBSes strip control characters from user input, only the host can start a mid-line sequence.

An introducer immediately preceded by `\` is escaped into literal text (ripper.c:18194–18195). Every introducer must be followed by **`|`** (vertical bar, 124) to form a command header; if the next byte is anything else, the parser takes the **"un-rip" fallback** (ripper.c:18540–18554): both the bogus introducer and the following byte are re-emitted to the text window and parsing resets — stray `!` characters in ordinary text render normally.

### Levels and Sublevels

*Evidence: 2.00a4 rule 8; SyncTERM (ripper.c:18275–18281).*

After the header `|` (or a chaining `|`), an optional level digit `1`–`9` selects the command level, and an optional second digit `1`–`9` a sublevel. Three consequences the code makes exact:

- **`0` is never a level digit.** `!|0X` is a *level-0 command whose command character is `0`* — not "level 0, command X". Level 0 is reached by writing no level digit at all.
- Nesting stops at two digits in SyncTERM's parser (level + sublevel), although the 2.00a4 grammar reserves up to nine level digits (`!|123456789<cmd>`).
- The command character is whatever single byte follows the level digits, and **need not be printable**: ESC (`0x1B`) is a real command character (`!|1<ESC>` RIP_QUERY, `!|9<ESC>` RIP_ENTER_BLOCK_MODE) (ripper.c:18289).

Level-0 commands are the graphics primitives and protocol settings; level 1 covers user-interface objects (buttons, mouse fields, images, text regions); level 2, context switching (ports, styles); level 3, time-related commands; level 9, binary transfer modes; levels 4–8 are open. Each level/character pair is a unique command: `!|L`, `!|1L`, and `!|15L` are three different commands.

### Parameters

*Evidence: 2.00a4 rules 9–10.*

Zero or more parameters follow the command character, with no delimiters between numeric fields. Numeric parameters are fixed-width [MegaNums or UltraNums](05-coordinates-and-math.md) per the current base math. A text parameter, if present, is always last. The last numeric parameter may be shortened by dropping leading zeros (`!|c01` → `!|c1`; at least one digit must remain), and a trailing *reserved* parameter may be omitted entirely (assumed `0`).

### Chaining, Continuation, and Line Endings

*Evidence: 2.00a4 rules 4–5, 13; SyncTERM (ripper.c:18556–18607); corpus.*

- **`|` chains commands**: a bare `|` dispatches the buffered command and starts the next on the same line (ripper.c:18583–18602). Some commands (RIP_ENTER_BLOCK_MODE and a few others) must stand alone.
- **CR dispatches** the pending command and returns the parser to idle (ripper.c:18561–18580) — *unless* the buffer ends in an **odd** number of backslashes, in which case the final `\` is stripped and the command continues on the next physical line (ripper.c:18563–18576). This is the classic line-continuation rule for keeping long commands inside right margins; a literal backslash at end-of-line must be doubled (`\\`), and continuation after a literal backslash needs three (`\\\`).
- **LF is unconditionally swallowed** mid-command (ripper.c:18557–18559) — which is why the CRLF line endings throughout the RIPtel corpus (the 1.54 spec said bare CR) parse identically.

### Escaping in Text Parameters

*Evidence: 2.00a4 rules 11, 14; SyncTERM (ripper.c:18204–18227).*

`!` and `|` appear literally in text parameters by quoting with backslash (`\!`, `\|`); a literal backslash is `\\`. SyncTERM's preprocessor (`cb_unescape`) makes `\|` a literal pipe in arguments (held as a placeholder through argument slicing, ripper.c:18291–18298) and collapses `\X` to `X` for any other character; at feed time an odd number of trailing backslashes before a `|` marks that pipe as data, not a command boundary (ripper.c:18583–18596). Host-command and text-variable metacharacters inside text parameters — `$`, `[`, `]`, `(`, `)` — must likewise be quoted (`\$Name\$`) to avoid interpretation.

### Raw Text and Error Handling

*Evidence: 2.00a4 rules 6, 15; corpus.*

A line that does not begin with an introducer is raw text, routed to the current [text window](06-color-audio-text.md) (or consumed as column content inside a flowed-text region). Unrecognized text *after* a complete command on the same line is ignored — the command still executes. A command with too much or too little data is "hopelessly corrupted" and discarded; the parser resynchronizes at the next `|`.

## Scene Header Segments

*Evidence: HLP.*

The 3.0 engine formalizes the 2.A1 RIP_HEADER idea into **scene header segments** — a structured header at the start of a scene that declares the environment the scene expects, so the terminal can validate and configure before drawing. RIPSCRIP.HLP's error-string family maps its contents:

- **Per-table segments** carrying flags and entry numbers for each object class: button style, graphics style, drawing port, text window, color palette, environment, mouse field, **audio**, and graphics screen (see [Data Tables](03-data-tables.md)).
- An **environment segment** carrying the coordinate size ("Invalid coordinate size in environment header segment") and the direct-RGB bit count ("Invalid direct RGB bit count in environment header segment").
- **General header flags** ("Invalid general header flags").
- A **revision code** identifying the RIPscrip revision of the code that follows ("Invalid RIPscrip revision code").

No wire-format example of a header segment has been recovered — the RIPtel demos use the explicit prologue commands instead (below) — so the segment encoding itself remains undocumented *(hypothesis: a parameterized RIP_HEADER-style command, per the 2.A1 design)*.

## Wire-Format Observations from the Corpus

*Evidence: corpus.*

The 116 TeleGrafix-authored RIPtel demo scripts are the best available picture of real 3.0 on the wire:

- **SOH marks scene start.** 111 of 116 files begin their *first* command line with SOH + `|`, then use `!|` for every subsequent line — SOH apparently flags start-of-scene to the 3.0 stream parser, while `!|` remains the 1.54-compatible introducer. (Three files invert the pattern, nearly all-SOH.)
- **The standard prologue** opens 90+ files: `J10|n2000|M08|fZKQO` — base math 36, 2-digit coordinates, 8-bit palette color mode, 1280×960 world frame (see [Numbers, Coordinates & Math](05-coordinates-and-math.md)).
- **Trailing text after fixed-length arguments is tolerated** and used as inline commentary — the driver stops reading once the fixed fields are consumed: `!|fZKQO                 Set world coordinats to 1280x960`. Chained `!|command|! comment` forms (the `!` comment opcode) are ubiquitous.
- **Typo tolerance is high.** A missing `!` introducer (`|1<ESC>0000$COMPAT$`, CURVES.RIP), separator lines `!|----` that accidentally parse as the `-` opcode with dash arguments (`-` reads as MegaNum 0), and bare `!|` no-op lines all pass through the shipping driver without visible failure. Robust parsers should degrade as gracefully.
- **Line endings are CRLF**, with `\` continuation across physical lines for long vertex lists (POLYPOLY.RIP).

## Annotated Wire Examples

### The standard prologue, decoded

*Evidence: corpus (90+ files).*

Nearly every TeleGrafix scene opens with the same chained sequence:

```text
!|J10|n2000|M08|fZKQO
```

| Piece | Command | Meaning |
|---|---|---|
| `!\|` | — | line introducer + command delimiter |
| `J10` | [RIP_SET_BASE_MATH](09-level-0-commands-g-r.md#rip_set_base_math) | base math ← 36 (`10` base-36 = 36): MegaNums |
| `\|n2000` | [RIP_SET_COORDINATE_SIZE](09-level-0-commands-g-r.md#rip_set_coordinate_size) | coordinates ← 2-digit fields |
| `\|M08` | [RIP_SET_COLOR_MODE](09-level-0-commands-g-r.md#rip_set_color_mode) | 8-bit palette color mode |
| `\|fZKQO` | [RIP_SET_WORLD_FRAME](08-level-0-commands-symbols-a-f.md#rip_set_world_frame) | world frame ← 1280×960 (`ZK`=1280, `QO`=960) |

The order matters: the scene pins its numeric conventions *before* any
coordinate-bearing command arrives.

### A real scene opening (DRAGON.RIP)

*Evidence: corpus (DRAGON.RIP; annotations editorial).*

```text
\x01|!                                    SOH scene-start + comment (no text)
!|*                                       RIP_RESET_WINDOWS - clean slate
!|1\x1b0000$-=NO_WIPES=Disabled$          query cmd sets variable NO_WIPES=Disabled
!|1\x1b0000$SBAROFF$                      hide the status bar
!|w0000000000                             RIP_TEXT_WINDOW - zero-size (off)
\x01|1\x1b0000$DTW$                       disable text window
\x01|1\x1b0000$COFF$                      cursor off
\x01|1\x1b0000$RESET(OVERFLOW)$           clear column-overflow state
\x01|n2000                                2-digit coordinates
\x01|M08                                  8-bit palette mode
\x01|W00                                  RIP_WRITE_MODE - COPY
\x01|fZKQO                                world frame 1280x960
\x01|2P10000ZKQO00030000                  RIP_DEFINE_PORT - offscreen port 1, 1280x960
\x01|J10                                  base math 36 (MegaNums)
\x01|1b0000HS0Y000G000000STRIP6.BMP       RIP_LOAD_BITMAP at (0,0), 640x34 region
\x01|c0F                                  draw color 15
\x01|y0000BW0X040000001a1a000000dixon     extended font style - outline font "dixon"
```

(`\x01` = SOH, `\x1b` = ESC; both are literal single bytes on the wire.
Lines end CRLF. Note the mixed introducers — SOH mid-scene is legal — and
`$…$` terminal-control variables doing setup work alongside drawing
commands.)

### Chaining, escaping, and continuation

*Evidence: 2.00a4; corpus; SyncTERM (ripper.c:18204–18232, 18563–18576).*

```text
!|c0F|=00000001|S010F                     three commands, one line: color,
                                          line style, fill style
!|@0A0ASay \| for a pipe                  \| = literal pipe inside a text
                                          parameter (not a delimiter)
!|p04000A000A0M0M2S0M2S000A00\            RIP_FILL_POLYGON with the vertex
0A                                        list continued across lines: odd
                                          trailing backslash before CR
!|! any prose after the bang-pipe-bang    comment command - ignored by the
                                          terminal, ubiquitous in the corpus
```

### A minimal constructed scene

*(Constructed example — not from the corpus; commands and field widths
follow the entries in pages 08–11.)*

```text
!|*|J10|n2000|M08|fZKQO                   reset + standard prologue
!|c0F|=00000001|S010F                     white pen, solid 1px line, solid fill
!|K05052S1U                               filled rectangle (5,5)-(100,66)
!|Y02000909                               font style: small stroked font, 9x9
!|@0F0PHello, RIPscrip 3.0                text at (15,25)
!|1b000000000000000000LOGO.BMP            bitmap at (0,0), unscaled
!|#                                       RIP_NO_MORE - scene complete
```

## Command Documentation Legend

*Evidence: editorial; format follows 2.00a4.*

The remaining pages of this edition ([08](08-level-0-commands-symbols-a-f.md)–13) document the reconstructed 3.0 command set. Each command entry uses the same two-column table style as the [2.x edition](../../2.x/ripscrip/07-protocol-definition.md):

| | |
|---|---|
| **Level** | Command level (sublevels shown with dots, e.g. 1.5 — the dots never appear on the wire) |
| **Command** | The command type character (with its level digits where helpful, e.g. `2C`) |
| **Arguments** | The parameter list in order. `name:2` = fixed 2-digit numeric field; `:1`/`:4` etc. likewise; `:XY` = variable-width coordinate governed by [RIP_SET_COORDINATE_SIZE](05-coordinates-and-math.md); `:CM` = color parameter governed by the current [color mode](06-color-audio-text.md); a bare name = text parameter (always last); `…` = the preceding field group repeats |

followed by **Format** (the full command with the `!|` header, level digits, command character, and spaced-out parameter names — the spaces never appear in real commands), an **Example** from the corpus where one exists, the attributes the command uses or affects (draw color, line style, fill, write mode, font, viewport/port adherence), the **base math** the command obeys (MegaNums only, UltraNums only, or the current setting), and an *Evidence* line naming the sources for that command — `2.00a4`, `HLP` (the RIP_* name in the 3.0.7 inventory), `corpus` (with file names), and/or `SyncTERM` (with ripper.c line cites). Commands attested only by a single source, and argument layouts inferred rather than documented, are flagged in the entry.

---

[◀ Prev: Color, Audio & Text Windows](06-color-audio-text.md) · [Contents](README.md) · [Next: Level-0 Commands (Symbols & A–F) ▶](08-level-0-commands-symbols-a-f.md)
