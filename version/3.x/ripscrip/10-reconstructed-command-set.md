# Reconstructed Command Set

[◀ Prev: Implementations & Versioning](09-implementations-and-versioning.md) · [Contents](README.md) · [Next: Reconstructed Text Variables & HCL ▶](11-reconstructed-text-variables-and-hcl.md)

> **Editorial reconstruction.** TeleGrafix never published a RIPscrip 3.0 Language Reference. This page reconstructs the 3.x-era command set from implementation evidence — primarily SyncTERM's `ripper.c` (`~/src/rip-tools/sbbs/src/syncterm/ripper.c`, Synchronet project, GPL), which answers the RIP version query with `RIPSCRIP030001`. Every claim cites its source line. This is NOT TeleGrafix documentation.

SyncTERM's version-reply table (`ripper.c:7619`) is the anchor for the whole reconstruction:

```c
static const char *ripver[] = {"", "RIPSCRIP015410", "RIPSCRIP030001"};
```

When negotiated into RIP mode (other than legacy 1.54), SyncTERM identifies itself as RIPscrip **3.00.01**. What follows is the complete command set that identification actually covers.

## Parser: Byte-Level Rules

The 3.x-era parser in `ripper.c` is a byte-fed state machine (`cb_feed`, 18516–18608) that is stricter and better specified than the 1.54-era line parser. Its observable rules:

### Introducers (18189–18202)

Three bytes can begin a RIP command sequence: `!`, SOH (`0x01`), and STX (`0x02`).

- **SOH / STX** are accepted *anywhere* in the stream (18196–18197). These are the binary introducers.
- **`!`** requires beginning-of-line: the previously emitted byte must be NUL, CR, LF, or VT (18198–18200), **or** — a deliberate fallback — the terminal's text cursor must already sit at column 1, e.g. having arrived there via an ANSI CUP sequence like `\x1b[5;1H` (18201, rationale comment 18180–18188).
- An introducer immediately preceded by `\` is rejected (18194–18195) — the backslash escapes it into literal text.

Every introducer must be followed by `|` to form a command header (18256–18263); an introducer followed by anything else takes the "un-rip" path (18540–18554): both the bogus introducer and the following byte are re-emitted to the text window.

### Backslash escaping (18204–18227)

A backslash preprocessor runs over the buffered command before code lookup (`cb_unescape`):

- `\|` becomes a **literal pipe** in the arguments (internally a `0xFF` placeholder, restored to `|` after argument slicing at 18291–18298).
- `\X` (any other X) becomes X.
- At byte-feed time, an odd number of trailing backslashes before a `|` means that pipe is data, not a command boundary (18583–18596).

### Level and sublevel decode (18275–18281)

After the `!|` / `\x01|` / `\x02|` header (or a chaining `|`):

```c
if (p < cb.len && cb.bytes[p] >= '1' && cb.bytes[p] <= '9') {
    level = cb.bytes[p++] - '0';
    if (p < cb.len && cb.bytes[p] >= '1' && cb.bytes[p] <= '9') {
        sublevel = cb.bytes[p++] - '0';
    }
}
```

Consequences worth spelling out:

- **Only digits `1`–`9` are level digits. `0` is never consumed as a level.** `!|0X` is therefore a *level 0 command whose command character is `0`* — not "level 0, command X". Level 0 is the implicit default reached by writing no level digit at all.
- Two consecutive digits `1`–`9` decode as level + sublevel. There is no third digit; nesting stops at two.
- The command character is whatever single byte follows (18289). It **need not be printable** — ESC (`0x1B`) is a real command character, used by `!|1<ESC>` (QUERY) and `!|9<ESC>` (ENTER_BLOCK_MODE).

### Terminators and chaining (18556–18607)

- **`|`** dispatches the buffered command and starts the next one — the classic RIPscrip command chain (18583–18602).
- **CR** dispatches the command and returns to idle (18561–18580), **unless** the buffer ends in an *odd* number of backslashes, in which case the trailing `\` is stripped and the command continues on the next line (18563–18576) — RIPscrip's line-continuation rule, implemented bit-exactly.
- **LF** is unconditionally swallowed mid-command (18557–18559).

### The "un-rip" fallback (18540–18554)

If a buffered introducer turns out not to head a command (next byte is not `|`), both bytes are flushed back to the text layer and parsing resets — so stray `!` characters in ordinary BBS text render normally.

## Argument Descriptor Mini-Language

Argument parsing is table-driven. A doc comment (9133–9139) defines a small descriptor language, and `rip_cmd_descs[]` (9141–9213) maps `(level, cmd)` to a descriptor string:

| Descriptor token | Meaning |
|---|---|
| `N#` | Fixed N-digit base-36 ("MegaNum") numeric field |
| `+N#` | Variable-length list of N-digit fields, repeated to end of arguments |
| `++` | Freeform text — the rest of the arguments verbatim |
| *(NULL)* | Handler parses its arguments manually |

The number parser is BGI-exact (`parse_mega`, 9107–9131): digits `0-9`, `A-Z`, `a-z` are base-36 digits; `-` counts as **0** (9120–9121); early termination on `|` or NUL is accepted after at least one valid digit (9122–9123); any other character is an error.

Field widths are a fingerprint of the era. Classic 1.54 commands use 1-, 2-, and 4-digit fields (max values 35, 1,295, 1,679,615). The descriptor for `!|1N` is `2#2#5#5#+5#` (9181) — the **only 5-digit MegaNum fields in the table**, addressing a range (36⁵ − 1 = 60,466,175) that nothing in the published 1.54 reference needs. See [Known but Unimplemented](#known-but-unimplemented--the-reconstruction-leads) below.

## Level 0 Commands

All commands verified against both the dispatch `switch` (case-label line cited) and the descriptor table (descriptor line cited). Provenance "1.54" means the command appears in the published RIPscrip 1.54 reference; "2.x" entries carry SyncTERM's own doc-revision tags (`v2.A0` … `v2.A4` = RIPscrip 2.0 Alpha document revisions).

### 1.54-provenance commands

| Cmd | Name | Args (descriptor) | Case / Desc lines |
|---|---|---|---|
| `#` | RIP_NO_MORE | *(none)* | 13315 / 9191 |
| `*` | RIP_RESET_WINDOWS | *(none)* | 13348 / 9190 |
| `=` | RIP_LINE_STYLE | `2#4#2#` | 13369 / 9172 |
| `>` | RIP_ERASE_EOL | *(none)* | 13426 / 9147 |
| `@` | RIP_TEXT_XY | `2#2#++` | 13445 / 9176 |
| `A` | RIP_ARC | `2#2#2#2#2#` | 13470 / 9162 |
| `B` | RIP_BAR | `2#2#2#2#` | 13517 / 9165 |
| `C` | RIP_CIRCLE | `2#2#2#` | 13546 / 9161 |
| `E` | RIP_ERASE_VIEW | *(none)* | 13579 / 9148 |
| `F` | RIP_FILL | `2#2#2#` | 13593 / 9166 |
| `H` | RIP_HOME | *(none)* | 13687 / 9146 |
| `I` | RIP_PIE_SLICE | `2#2#2#2#2#` | 13695 / 9196 |
| `L` | RIP_LINE | `2#2#2#2#` | 13800 / 9159 |
| `O` | RIP_OVAL | `2#2#2#2#2#2#` | 13850 / 9163 |
| `P` | RIP_POLYGON | `2#2#2#+2#` | 13871 / 9164 |
| `Q` | RIP_SET_PALETTE | `2#` ×16 | 13921 / 9175 |
| `R` | RIP_RECTANGLE | `2#2#2#2#` | 13948 / 9160 |
| `S` | RIP_FILL_STYLE | `2#2#` | 13967 / 9169 |
| `T` | RIP_TEXT | `++` | 14026 / 9154 |
| `V` | RIP_OVAL_ARC | `2#2#2#2#2#2#` | 13848 (falls through to `O`) / 9195 |
| `W` | RIP_WRITE_MODE | `1#` | 14062 / 9167 |
| `X` | RIP_PIXEL | `2#2#` | 14084 / 9174 |
| `Y` | RIP_FONT_STYLE | `2#2#2#1#1#` | 14100 / 9155 |
| `Z` | RIP_BEZIER | `2#` ×8 + `2#` count | 14140 / 9194 |
| `a` | RIP_ONE_PALETTE | `2#2#` | 14221 / 9204 |
| `c` | RIP_COLOR | `2#` | 14366 / 9150 |
| `e` | RIP_ERASE_WINDOW | *(none)* | 14388 / 9149 |
| `g` | RIP_GOTOXY | `2#2#` | 14428 / 9153 |
| `i` | RIP_OVAL_PIE_SLICE | `2#2#2#2#2#2#` | 14445 / 9197 |
| `l` | RIP_POLYLINE | `2#2#2#+2#` | 14529 / 9207 |
| `m` | RIP_MOVE | `2#2#` | 14568 / 9152 |
| `o` | RIP_FILLED_OVAL | `2#2#2#2#` | 14589 / 9168 |
| `p` | RIP_FILL_POLYGON | `2#2#2#+2#` | 14596 / 9171 |
| `s` | RIP_FILL_PATTERN | `2#` ×9 | 14670 / 9170 |
| `v` | RIP_VIEWPORT | `2#2#2#2#` | 14725 / 9158 |
| `w` | RIP_TEXT_WINDOW | `2#2#2#2#1#1#` | 14783 / 9157 |

### Post-1.54 additions (the 2.x/3.x layer)

These carry SyncTERM's own provenance comments naming the 2.0-Alpha documentation revision that introduced each one.

| Cmd | Name | Provenance tag | Args (descriptor) | Case / Desc lines |
|---|---|---|---|---|
| `f` | RIP_SET_WORLD_FRAME | v2.A0 | `2#2#` | 14401 / 9210 |
| `z` | RIP_POLY_BEZIER | v2.A1 | manual (shared bezier parser) | 14870 |
| `t` | RIP_POLY_BEZIER_LINE | v2.A2 | manual (NULL desc) | 14721 / 9208 |
| `x` | RIP_FILLED_POLY_BEZIER | v2.A2 | manual (empty desc) | 14866 / 9192 |
| `K` | RIP_FILLED_RECTANGLE | v2.A2 | `2#2#2#2#` | 13812 / 9212 |
| `N` | RIP_SET_BORDER | v2.A3 | `2#` | 13842 / 9209 |
| `b` | RIP_EXTENDED_TEXT_WINDOW | v2.A4 | `2#2#2#2#2#2#1#4#` | 14255 / 9151 |
| `j` | RIP_POINT | "Not in Alpha docs" | `2#2#` | 14518 / 9211 |
| `$` | *(undocumented)* text-variable trigger | — | manual | 13336 |

Notes on the additions:

- **`f` RIP_SET_WORLD_FRAME is the single biggest semantic change of the 2.x/3.x era.** It redefines the logical coordinate space away from 1.54's fixed 640×350: the handler stores the two arguments as the new logical `x_dim`/`y_dim` and remaps all subsequent drawing to the physical screen (14401 ff., setting `rip.x_dim`/`rip.y_dim` and rescaling against `vstat.scrnwidth`). Every coordinate-taking command's meaning becomes frame-relative.
- **`b` RIP_EXTENDED_TEXT_WINDOW** is a pixel-addressed text window (coordinates pass through `map_rip_x`/`map_rip_y` at 14259–14262) with a font-id field (0–4) and a 4-digit flags field — replacing the cell-addressed 1.54 `w`.
- The three **poly-bezier** commands funnel into one shared `rip_poly_bezier()` implementation with line/fill/close flags (`t` → `(args, false, false)` at 14723, `x` → `(args, true, true)` at 14868, `z` → `(args, false, true)` at 14872).
- **`!|$NAME$`** (13336, source comment: "Undocument RIP_QUERY thing") evaluates a text variable purely for its side effect — a command-position use of the text-variable machinery with no 1.54 or 2.0-Alpha documentation at all.

## Level 1 Commands

All 1.54-provenance unless noted.

| Cmd | Name | Args (descriptor) | Case / Desc lines |
|---|---|---|---|
| `<ESC>` | RIP_QUERY | `1#3#++` | 14881 / 9200 |
| `B` | RIP_BUTTON_STYLE | `2#2#2#4#2#` + 9×`2#` + `6#` | 15015 / 9193 |
| `C` | RIP_GET_IMAGE | `2#2#2#2#1#` | 15847 / 9183 |
| `D` | RIP_DEFINE | `3#2#++` | 15899 / 9199 |
| `E` | RIP_END_TEXT | *(none)* | 16090 / 9188 |
| `F` | RIP_FILE_QUERY | `2#4#++` | 16104 / 9203 |
| `G` | RIP_COPY_REGION | `2#2#2#2#2#2#` | 16246 / 9201 — **stub, see below** |
| `I` | RIP_LOAD_ICON | `2#2#2#1#1#1#++` | 16302 / 9179 |
| `K` | RIP_KILL_MOUSE_FIELDS | *(none)* | 16370 / 9186 |
| `M` | RIP_MOUSE | `2#2#2#2#2#1#1#4#1#` | 16381 / 9180 |
| `P` | RIP_PUT_IMAGE | `2#2#2#1#` | 16481 / 9184 |
| `R` | RIP_READ_SCENE | `2#2#2#2#++` | 16534 / 9202 |
| `T` | RIP_BEGIN_TEXT | `2#2#2#2#1#1#` | 16581 / 9187 |
| `t` | RIP_REGION_TEXT | `1#++` | 16623 / 9189 |
| `U` | RIP_BUTTON | `2#2#2#2#2#1#1#++` | 16764 / 9198 |
| `W` | RIP_WRITE_ICON | `1#++` | 16969 / 9191 |

**`!|1G` RIP_COPY_REGION is a documentation-only stub.** The case label at 16246 carries the full 1.54 spec comment ("physically copies a rectangular region of the graphics screen up or down…"), followed by a bare `break;` at ~16301 with no implementation between them. The command is recognized and swallowed but does nothing. It is the one place where SyncTERM documents a command it does not implement.

## Level 9: File Transfer

One command exists at level 9.

| Cmd | Name | Args | Case / Desc lines |
|---|---|---|---|
| `<ESC>` | RIP_ENTER_BLOCK_MODE | manual parse (NULL desc) | 17069 / 9205 |

`!|9<ESC>` auto-initiates a file transfer. The manual argument parse (from 17150) reads: protocol (1 digit), file_type (1 digit), reserved (2 digits), reserved2 (4 digits), then a filename starting at argument offset 8, terminated by the literal sequence `<>` (17158 ff., `strstr(&args[8], "<>")`).

Protocol codes, dispatched at 17187–17212 (download direction) and 17263–17300 (upload direction):

| Code | Protocol |
|---|---|
| 0 | XMODEM (checksum) |
| 1 | XMODEM-CRC |
| 2 | XMODEM-1K |
| 3 | XMODEM-1K-G |
| 4 | *(rejected)* |
| 5 | YMODEM |
| 6 | YMODEM-G |
| 7 | ZMODEM |

Filenames containing `\` are rejected, and received files land in a per-BBS `RIP` cache subdirectory (17150–17190) — SyncTERM's sandboxing of a command originally designed to write into RIPterm's icon directories.

## Known but Unimplemented — the Reconstruction Leads

Seven descriptor-table entries have **no case label anywhere in the dispatch switch**. SyncTERM knows their argument shapes — meaning its authors had evidence (captured traffic, RIPterm behavior, or non-public documentation) of real commands — but never implemented them. These are the strongest leads for commands RIPterm 2.x/3.0 had beyond the 1.54 reference:

| Level | Cmd | Descriptor | Desc line | Plausible identity |
|---|---|---|---|---|
| 0 | `y` | `2#2#2#2#` | 9156 | Unknown; sits beside `Y` FONT_STYLE in the table |
| 0 | `D` | `2#2#2#2#2#2#` | 9173 | Unknown 6-field level-0 drawing/state command |
| 0 | `<ESC>` | *(NULL — manual)* | 9206 | A level-0 ESC command, counterpart to `1<ESC>`/`9<ESC>` |
| 1 | `A` | `2#2#2#2#2#2#2#` | 9177 | Shape matches a region/animation-class command |
| 1 | `O` | `2#2#2#2#` | 9178 | Four coordinates — a region command |
| 1 | `N` | `2#2#5#5#+5#` | 9181 | The only 5-digit fields in the table; the 60M+ range suggests file sizes or byte offsets |
| 1 | `S` | `4#4#` | 9182 | Two 4-digit fields |

To these add the `1G` COPY_REGION doc-stub above: eight commands total that SyncTERM acknowledges but does not perform.

Cross-referencing this repo's RIPscrip 2.00 A4 edition ([Level 1 commands](../../2.x/ripscrip/11-level-1-commands.md), [Level 2 commands](../../2.x/ripscrip/12-level-2-commands.md)): several of these argument shapes are consistent with commands documented there, and the descriptor table's own revision tags (`v2.A0`–`v2.A4`) show its authors worked directly from 2.0-Alpha documents. A match for individual unimplemented entries is *plausible* on that basis, but the descriptor strings alone do not prove which 2.00 command each one is — we note the correspondence as a lead, not a finding.

## What Is Absent

Equally important is what a terminal answering `RIPSCRIP030001` demonstrably does **not** contain, despite all of the following being advertised 2.x/3.0-white-paper features:

- **No level 2 or level 3 commands at all.** The dispatch switch has cases only for levels 0, 1, and 9. The entire 2.00 A4 level-2 command set (see [`../../2.x/ripscrip/12-level-2-commands.md`](../../2.x/ripscrip/12-level-2-commands.md)) is unrepresented.
- **No raster or media formats**: no JPEG, PNG, BMP, GIF, MIDI, or WAV support anywhere in the RIP path.
- **No outline fonts.** Text rendering uses only the 10 embedded BGI stroke fonts.
- **No RIPtel symbols** or any other 3.0-white-paper vocabulary.

One deliberate 1.54 **bug-compatibility quirk** is version-gated (≈12800): after an arc/sweep draw, when emulating version 1 (`RIP_VERSION_1`) with custom line style 4, the code intentionally clobbers the user-defined line pattern with a stale `0x8000` register value — reproducing a documented RIPterm bug. In 3.0 mode the bug is fixed; this is the clearest evidence that SyncTERM's authors treated 1.54 behavior and 3.0 behavior as distinct, versioned contracts.

Finally, `ripper.c` also implements **SkyPix** (17543–17950, `do_skypix`), an entirely separate Amiga-era graphics protocol that happens to share the CSI escape channel. It is not RIPscrip, uses none of the machinery above, and should not be conflated with the RIP 3.0 command set merely because it lives in the same source file.

## How to Read This Reconstruction

SyncTERM's command set is the **observable floor** of RIP 3.0, not its ceiling: it is what one careful open-source implementation, working from 2.0-Alpha documents and real-world traffic, found necessary and verifiable enough to implement under the `RIPSCRIP030001` banner. Commands TeleGrafix shipped but nobody scripted, and features that never left the white paper, are invisible to this method. The documented baseline that 3.0 built upon is the RIPscrip 2.00 A4 specification, preserved in this repo's [2.x edition](../../2.x/ripscrip/README.md) — read that for the language as TeleGrafix last described it, and this page for the subset a working 3.0-era terminal actually answers to.

---

[◀ Prev: Implementations & Versioning](09-implementations-and-versioning.md) · [Contents](README.md) · [Next: Reconstructed Text Variables & HCL ▶](11-reconstructed-text-variables-and-hcl.md)
