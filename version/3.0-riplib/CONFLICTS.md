# Conflict summary - RIPlib vs this repository

[Contents](README.md)

_A single-page index of every clear conflict recorded across [3.0-riplib](README.md), [3.1-riplib](../3.1-riplib/README.md) and [3.2-riplib](../3.2-riplib/README.md), for citing in discussion upstream. Each item states both readings in a line or two and links to the page that carries the evidence; nothing here is new material._

Items are numbered for reference: **B** for baseline conflicts (what RIPscrip 3.0 already was - questions of fact, exactly one answer is right), **X** for extension-layer collisions (RIPlib's v3.1/v3.2 taking a name or field the record already uses), **N** for non-conflicts worth correcting anyway. Dispositions follow the [legend](ripscrip/README.md#how-items-are-stated): **callable** (the evidence settles it), **open** (both readings defensible), **compatible** (differs in form, not effect).

Reconciled against riplib `main` @ `3e05ecb` (2026-06-30). See the [tree README](README.md#why-the-two-records-differ) for how the two evidence bases compare, and [where RIPlib is right](README.md#where-riplib-is-right) for the other side of the ledger.

## At a glance

| ID | Item | Disposition | Why it matters |
| --- | --- | --- | --- |
| [B1](#b1---w-write-mode-numbering) | `\|W` write-mode numbering | **callable** | Silently mis-renders existing content in both directions |
| [B2](#b2---j---base-math-vs-save_icon) | `\|J` - base math vs SAVE_ICON | **callable** | In 90 of 116 corpus prologues |
| [B3](#b3---f---world-frame-vs-font_attrib) | `\|f` - world frame vs FONT_ATTRIB | **callable** | Same prologue; §A2G.3 rides on it |
| [B4](#b4---the-punctuation-block) | The punctuation block (`&`, `-`, `+`, `[`, `]`, `_`, `;`, `<`) | **callable** | Eight opcodes, corpus-attested with author comments |
| [B5](#b5---k---filled-rectangle-vs-kill_mouse_ext) | `\|K` - filled rectangle vs KILL_MOUSE_EXT | **callable** | The corpus's standard background primitive |
| [B6](#b6---d-and-d---palette-vs-pattern) | `\|D` / `\|d` - palette vs pattern/font | **open** | Drawing-palette command absent upstream |
| [B7](#b7---2r---defines-a-refresh-vs-performs-one) | `\|2R` - define refresh vs perform refresh | **open** | Same letter, opposite roles |
| [B8](#b8---level-1-and-level-2-letter-swaps) | Level 1 / Level 2 letter swaps | **open** | Six commands on different letters |
| [B9](#b9---fill-pattern-00) | Fill pattern `00` - background fill vs no fill | **open** | `!\|S0000` + bar is a no-op upstream |
| [B10](#b10---the-built-in-pattern-bitmaps) | Built-in pattern bitmaps | **open** | Fills tile with different pixels; `05`/`06` collapse |
| [B11](#b11---the--escape-attributed-as-an-extension) | `\!` attributed as a RIPlib extension | **callable** | Bookkeeping only |
| [B12](#b12---soh--stx-alternate-introducers-not-implemented) | SOH / STX introducers (rule 12) missing | **callable** | Shipped 2.x content will not start |
| [X1](#x1---v31-text-variables) | v3.1 text variables (`$COMPAT$`, `$COPY$`, `$PROT$`, `$YEAR$`) | **conflict** | Silent wrong output at the host |
| [X2](#x2---v32-time-variables) | v3.2 time variables (`$HOUR$`, `$DOW$`, `$MONTH$`) | **conflict** | Conditionals silently evaluate false |
| [X3](#x3---y-direction-01-redefined) | `\|Y` direction `01` redefined | **redefinition** | Vertical text reads upside-down across implementations |
| [X4](#x4---font-attribute-bit-assignment) | Font attribute bit assignment | **conflict** | `04`/`08` differ from the documented style flags |
| [X5](#x5---the-csi-relaxed--trigger) | The CSI-relaxed `!` trigger | **divergence** | Turns ordinary text into commands |
| [X6](#x6---28-gradient-fill-is-unattested) | `\|28` gradient fill unattested in the record | **open (baseline)** | v3.2 extends a command we cannot corroborate |
| [X7](#x7---debug-directive) | `<<DEBUG>>` directive | **caution** | Shadows a macro; unsolicited host traffic |
| [N1](#n1---four-commands-misfiled-as-riplib-extensions) | `\|!`, `\|(`, `\|)`, `\|1R` misfiled as extensions | _(free)_ | Four rows leave the deviation register |
| [N2](#n2---x-missing-from-a1) | `\|x` missing from §A.1 | _(free)_ | Table omission, already right in §11.2 |
| [N3](#n3---and-and-not-are-not-new) | AND and NOT are not v3.1 additions | _(free)_ | §A2G.1 reduces to a completeness fix |

---

## Baseline conflicts

### B1 - `|W` write-mode numbering

- **RIPlib:** `01` = OR, `02` = AND, `03` = XOR (`include/drawing.h:81-84`, `docs/spec/02-level0-drawing.md` §2.3), recorded as a bug fix in `§BUG.7`.
- **Here:** `01` = XOR, `02` = OR, `03` = AND.
- **Evidence:** four independent sources put XOR at `01` - the 1.54 specification text, the 1.54 `RIP_PUT_IMAGE` mode table, the 2.00a4 five-value table (44 corpus uses, SyncTERM `ripper.c:14062`), and **RIPlib's own `§DEAD.3`**, which records the DLL implementing COPY 0 / XOR 1 / OR 2. Borland BGI, which the language borrows from, also puts XOR at 1. No source has been identified for the renumbering.
- **Effect:** `|W01` is the historically common non-copy mode - draw-twice-to-erase animation and rubber-banding. Under OR it never erases, so those effects smear. `|W03` inverts the error against the corpus's AND masking. Nothing detects it: both sides parse, both render, neither errors.
- **Fix:** four lines in `include/drawing.h`, the §2.3 table, and retiring `§BUG.7`.
- **Detail:** [2.0 Write Mode Numbering](ripscrip/2.0-write-modes.md)

### B2 - `|J` - base math vs SAVE_ICON

- **RIPlib:** `SAVE_ICON`, `!|J<slot>`, 2 arguments.
- **Here:** [RIP_SET_BASE_MATH](../3.0/ripscrip/1.2-math-and-coordinates.md#rip_set_base_math) - `base_math:2`, selects base 36 or 64 for every subsequent numeric field.
- **Evidence:** 90 of the 116 shipping demo scenes open `!|J10|n2000|M08|fZKQO`, and ONLINE.RIP carries TeleGrafix's own inline comment - _"Set base math to MegaNums"_ - next to `J10`. SyncTERM's descriptor table agrees. RIPlib's §11.2 Erratum 1 is right that the 2.00a4 draft collides `b` between base math and extended text window; the shipping driver resolved that by moving base math to `J`.
- **Effect:** a terminal reading `!|J10` as `SAVE_ICON` skips the declaration that governs how it parses every numeric field in the rest of the scene.
- **Detail:** [9.0 Command Inventory Comparison](ripscrip/9.0-command-inventory-comparison.md#prologue-critical---j-and-f)

### B3 - `|f` - world frame vs FONT_ATTRIB

- **RIPlib:** `FONT_ATTRIB`, `attrib:2 res:2`; §11.1 lists RIP_WORLD_FRAME as defined in v2.A4 but absent from the production DLL.
- **Here:** [RIP_SET_WORLD_FRAME](../3.0/ripscrip/1.3-world-view-virtual-canvas.md#rip_set_world_frame) - `x_dim:XY y_dim:XY`, corpus standard `ZKQO` = 1280×960.
- **Evidence:** `|fZKQO` is on the wire in the same 90 prologues as B2. §11.1 may be accurate about what the DLL _computes_ - but "parsed but not acted on" is a category RIPlib documents extensively (`§DEAD.1`-`§DEAD.8`); reassigning the letter is the step that does not follow.
- **Effect:** a `RIPSCRIP031001` terminal reads a live prologue field as a font-attribute command. §A2G.3 is the feature that rides on the assignment, so this is a precondition for it.
- **Detail:** [9.0 Command Inventory Comparison](ripscrip/9.0-command-inventory-comparison.md#prologue-critical---j-and-f)

### B4 - the punctuation block

Eight Level-0 opcodes assigned differently. RIPlib labels its names "Extended Commands (v2.0+)"; they do not appear in the 2.00 Alpha 4 draft.

| Opcode | Here                         | RIPlib             |
| ------ | ---------------------------- | ------------------ |
| `\|&`  | RIP_SKEWED_OVAL              | `ICON_STYLE`       |
| `\|-`  | RIP_FILLED_SKEWED_OVAL       | `TEXT_XY_EXT`      |
| `\|+`  | RIP_SKEWED_OVAL_CHORD        | `SCROLL`           |
| `\|[`  | RIP_SKEWED_OVAL_PIE_SLICE    | `FILL_POLYGON_EXT` |
| `\|]`  | RIP_SKEWED_OVAL_ARC          | `POLYLINE_EXT`     |
| `\|_`  | RIP_FILLED_OVAL_CHORD        | `DRAW_TO`          |
| `\|;`  | RIP_MARKER (361 corpus uses) | `BUTTON_EXT`       |
| `\|<`  | RIP_POLY_POLYGON             | `GET_IMAGE_EXT`    |

- **Evidence:** the six skewed-oval names come **verbatim from TeleGrafix's own comments in NEWCMDS.RIP**, a demo script written to introduce them, shipped inside the same product as the DLL. `;` and `<` are corpus-observed with editorially reconstructed names.
- **Ask:** establish the provenance of RIPlib's names. If they came from DLL strings or dispatch analysis, both sides hold real evidence and the task is matching names to opcodes; if inferred, the corpus decides.
- **Detail:** [9.0 Command Inventory Comparison](ripscrip/9.0-command-inventory-comparison.md#the-3x-punctuation-block)

### B5 - `|K` - filled rectangle vs KILL_MOUSE_EXT

- **RIPlib:** `KILL_MOUSE_EXT`; §11.1 additionally lists RIP_FILLED_RECTANGLE as dropped and absent from the DLL.
- **Here:** [RIP_FILLED_RECTANGLE](../3.0/ripscrip/2.3-shapes-and-fills.md#rip_filled_rectangle) (v2.A2) - the corpus's standard full-screen background primitive, used heavily.
- **Effect:** scenes lose their backgrounds and gain spurious mouse-field kills. Resolvable on the same terms as B4.
- **Detail:** [9.0 Command Inventory Comparison](ripscrip/9.0-command-inventory-comparison.md#level-0-letters)

### B6 - `|D` and `|d` - palette vs pattern

- **RIPlib:** `|D` = `FILL_PATTERN_EXT` (18 args), `|d` = `EXT_FONT_STYLE` (`fid attr size`). No drawing-palette command exists; `|20 SET_VGA_PALETTE` covers adjacent ground with different arguments.
- **Here:** `|D` = [RIP_SET_DRAWING_PALETTE](../3.0/ripscrip/2.0-color-modes-and-palettes.md#rip_set_drawing_palette) - blocks of 256-entry RGB in UltraNums, what the corpus uses to build its dithered fade backgrounds; `|d` = [RIP_ONE_DRAWING_PALETTE](../3.0/ripscrip/2.0-color-modes-and-palettes.md#rip_one_drawing_palette). Extended font style is `|y`, a 26-digit layout recovered from FONTS.RIP and already driven the same way by RIPterm Professional 2.0.
- **Disposition:** open - worth checking which form the DLL's palette code actually parses. `|d` and `|y` look like different commands rather than a naming dispute.
- **Detail:** [9.0 Command Inventory Comparison](ripscrip/9.0-command-inventory-comparison.md#palette-versus-pattern---d-and-d)

### B7 - `|2R` - defines a refresh vs performs one

- **RIPlib:** `REFRESH`, 0 arguments, performs a refresh.
- **Here:** [RIP_SET_REFRESH](../3.0/ripscrip/5.0-host-commands.md#rip_set_refresh) - `res:4 refresh_string`, **defines** the host command sent to refresh the display (v2.A1).
- **Effect:** a stray `!|2R` followed by text is a definition on one side and a no-op plus stray text on the other.
- **Detail:** [9.0 Command Inventory Comparison](ripscrip/9.0-command-inventory-comparison.md#level-2)

### B8 - Level 1 and Level 2 letter swaps

| Opcode | Here | RIPlib | Note |
| --- | --- | --- | --- |
| `\|1i` / `\|1S` | `1i` = RIP_IMAGE_STYLE - image area + display flags (v2.A0) | `1S` = IMAGE_STYLE - `mode:2` | Same concept, different letter **and** arguments |
| `\|1w` / `\|1A` | `1w` = RIP_PLAY_AUDIO (v2.A3), well attested in HLP | `1A` = PLAY_AUDIO | Corpus cannot arbitrate; DLL analysis can |
| `\|1G` / `\|1g` | `1G` = RIP_SCROLL, `1g` = RIP_COPY_BLIT | `1G` = COPY_REGION | Names swapped across the case pair |
| `\|1Q` | RIP_QUERY is introduced by a literal Escape (0x1B) - the corpus's universal command executor | `1Q` = QUERY_EXT | **Callable** for the Escape form; whether `1Q` also exists is separate |
| `\|1D` | RIP_DEFINE - `flags:3 res:2 text`, obsolete since v2.A1 | `DEFINE_VARIABLE` - `!\|1D<name>=<value>` | RIPlib already tracks this as open question **U-025** |
| `\|t` | RIP_POLY_BEZIER_LINE (v2.A2) | `REGION_TEXT` | Region text is `\|1t` here, which RIPlib also has |

- **Detail:** [9.0 Command Inventory Comparison](ripscrip/9.0-command-inventory-comparison.md#level-1)

### B9 - fill pattern `00`

- **RIPlib:** skips the fill entirely, for every primitive.
- **Here:** the specification defines `00` as _"Fill with background color"_ - an all-zero pattern painting solid color 0. The 1.54 entry is explicit: _"Fill pattern 00 will set the entire fill area to the background color."_
- **Nuance:** implementations already differ. SyncTERM's scanline **polygon** filler skips style 0, while bars and flood fills paint color 0 by the general rule.
- **Effect:** `!|S0000` followed by a bar - a scene blanking a region - is a no-op upstream. Worth aligning at least for bars and rectangles; the polygon case is genuinely uncertain everywhere.
- **Detail:** [2.0 Fill Pattern Mapping](techspecs/2.0-fill-pattern-mapping.md#pattern-00---background-fill-versus-no-fill)

### B10 - the built-in pattern bitmaps

- **RIPlib:** eleven independently chosen 8×8 bitmaps (`src/drawing.c:52-65`) plus a user slot, reached through an explicit wire-value mapping (`src/ripscrip.c:693-710`).
- **Here:** the specification prints eight byte values per pattern, and RIPterm implemented them faithfully - including the famously wrong "Light Backslash" bytes `A5 D2 69 B4 5A 2D 96 4B`, which SyncTERM still ships as documented.
- **Two effects:** fills render with the right _character_ but not the same pixels; and four of the thirteen wire values resolve to approximations, with `05`/`06` collapsing onto one bitmap - the specific defect `§DEAD.6` set out to fix, relocated rather than removed, which qualifies §A2G.4's claim to provide _"all 13 patterns natively"_.
- **Disposition:** open, and a real decision rather than an error - but it should be a recorded choice. Byte-exact fidelity matters when a fill tiles against era artwork.
- **Detail:** [2.0 Fill Pattern Mapping](techspecs/2.0-fill-pattern-mapping.md#the-pattern-bitmaps-are-an-independent-set)

### B11 - the `\!` escape attributed as an extension

- **RIPlib:** `§DEV.1` records `\!` as _"a deliberate, backward-compatible extension"_ beyond spec §1.6 / §7.1.
- **Here:** `\!` is **syntax rule 11**, unchanged from 1.54 through 2.00a4 and into 3.x, named in the same sentence as `\|`. Conversely `\n` is not in the record at all, and `\^` is defensible but unnamed by any rule.
- **Effect:** none - nothing renders differently. Pure bookkeeping: `\!` moves out of `§DEV.1` and the deviation register loses an entry.
- **Also flagged:** since v2.A3 the specification requires host-command metacharacters (`$`, `[`, `]`, `(`, `)`) to be quoted in text parameters. Not a conflict, but a gap that bites - the corpus leans on `$…$` inside host strings.
- **Detail:** [1.0 Stream Parsing & Escapes](techspecs/1.0-stream-parsing-and-escapes.md#the-escape-set-and-its-attribution)

### B12 - SOH / STX alternate introducers not implemented

- **RIPlib:** recognizes `!` at four positions - stream start, or after CR/LF/FF - which matches the specification as far as it goes.
- **Here:** syntax rule 12 also allows `!` to be replaced by **SOH (`0x01`)** or **STX (`0x02`)**, accepted _anywhere_ in a line - deliberately host-only, since BBS users could not readily type control characters. SyncTERM implements exactly this split, and the shipped 2.x corpus opens with the SOH form, `\x01|*`.
- **Effect:** a `RIPSCRIP03`-class terminal that does not honor SOH/STX fails to start scenes shipping content already uses. It also makes X5 unnecessary.
- **Detail:** [1.0 Stream Parsing & Escapes](techspecs/1.0-stream-parsing-and-escapes.md#alternate-command-introducers)

---

## Extension-layer collisions

These are not questions of fact - they are RIPlib's own additions landing on names the TeleGrafix record already uses. Each is cheap to resolve by renaming upstream.

### X1 - v3.1 text variables

Four of the seven variables §A2G.1-7 introduces take a name that in RIPscrip 2.x/3.x denotes a _parameterized action_, and re-use it as a bare value or an unrelated action:

| Variable | RIPlib v3.1 | The record |
| --- | --- | --- |
| `$COMPAT$` | Returns `"1"`, a fixed compatibility level | `$COMPAT(env)$` sets an environment to 1.54 settings - the standard legacy-mode drop, **21 corpus uses** |
| `$COPY$` | Sets write mode to COPY | `$COPY(type,source,dest,…)$` copies data tables, entries, save slots and stack |
| `$PROT$` | Returns a resolution-mode index | `$PROT(object,element,…)$` protects table entries and save slots |
| `$YEAR$` | 4-digit year | `$YEAR$` is the **2-digit** year; `$FYEAR$` is the 4-digit one |

- **Effect:** a host sending `$COMPAT(0)$` to drop a `RIPSCRIP031001` terminal into legacy mode instead gets the literal string `1` rendered - a silent, visible failure rather than a caught error, and not cheap to work around at the host.
- **Also:** `$COFF$` and `$MKILL$` are reduced to unparameterized forms of existing variables rather than redefined.
- **Detail:** [9.0 Additions Reference](../3.1-riplib/ripscrip/9.0-additions-reference.md#text-variables)

### X2 - v3.2 time variables

| Variable | RIPlib v3.2 | The record | The record's name for RIPlib's meaning |
| --- | --- | --- | --- |
| `$HOUR$` | `HH`, **00-23** | `HH`, **01-12** (non-military) | `$MHOUR$` |
| `$DOW$` | Digit **0-6, Monday = 0** | Day of week **spelled out** (`Monday`) | `$WDAY$` - and it is **0 = Sunday** |
| `$MONTH$` | `MM`, 01-12 | **Full month name** (`December`) | `$MONTHNUM$` |

- **Effect:** quiet. `<<IF $DOW$=4>>Happy Friday!<<ENDIF>>` - RIPlib's own example - is false on every conforming 3.x terminal, where `$DOW$` expands to `Friday`. Write it the other way and it is false on RIPlib. Neither errors. `$MONTH$` displays `December` on one and `12` on the other.
- **Fix:** the record already names every value RIPlib wants. Adopting `$MHOUR$`, `$WDAY$`, `$MONTHNUM$` (and `$DAY$` for `$DOM$`) clears the whole group at no cost to the feature.
- **Detail:** [9.1 Text Variables](../3.2-riplib/ripscrip/9.1-text-variables.md)

### X3 - `|Y` direction `01` redefined

- **RIPlib (§A2G.2):** `01` is vertical **top to bottom**, glyphs rotated CW; `02` adds CCW.
- **Here:** 1.54 states it explicitly - _"Vertical text is drawn with the base-line to the right, and is read from bottom to the top"_ - and 3.x carries the same reading.
- **Effect:** content authored against either reads upside-down on the other. Practical risk is small (no corpus scene uses vertical `|Y` text, which is what motivated the change), but the two are not interchangeable. `02` itself is a clean addition.
- **Detail:** [3.0 Text Direction & Font Attributes](../3.1-riplib/ripscrip/3.0-text-direction-and-font-attributes.md#text-direction---a-third-value-and-a-redefined-second)

### X4 - font attribute bit assignment

- **RIPlib (§A2G.3):** `0x01` bold, `0x02` italic, `0x04` underline, `0x08` shadow.
- **Here:** `RIP_EXTENDED_FONT_STYLE`'s `<style>` field - the canonical place facings live - defines `01` bold, `02` italic, `04` **strike-out**, `08` **underline**. Shadow is not absent from the record either; it lives in the extended-font mechanism (TELLISTS.MNU narrates _"Marin, centered, bold w/ dropshadow"_).
- **Compounded by B3:** the feature is carried on `|f`, a contested opcode.
- **Detail:** [Two divergences worth flagging](../3.1-riplib/ripscrip/3.0-text-direction-and-font-attributes.md#two-divergences-worth-flagging)

### X5 - the CSI-relaxed `!` trigger

- **RIPlib (v3.1):** additionally accepts `!` immediately after an ANSI CSI terminator, so `ESC[2J!|*|` starts a scene on one line.
- **Here:** that is exactly what rule 12's SOH/STX introducers already do (see B12), portably and back to 1.54 - `ESC[2J\x01|*|` works on any conforming terminal, and the shipped 2.x corpus already opens that way.
- **Cost of the relaxation:** ordinary prose containing an ANSI sequence followed by `!` - an exclamation mark opening a sentence right after a color change - parses as a command. The line-boundary rule exists to make that impossible.
- **Detail:** [1.0 Stream Parsing Delta](../3.1-riplib/techspecs/1.0-stream-parsing-delta.md#this-problem-already-has-a-documented-solution)

### X6 - `|28` gradient fill is unattested

- **RIPlib:** `RIP_GRADIENT_FILL` (`|28`) is part of its **v3.0 baseline**, attributed to RIPSCRIP.DLL 3.0.7; §A2G.13 adds mode `2` (radial) to it.
- **Here:** no gradient command appears in the 1.54 specification, the 2.00 Alpha 4 draft, the RIPtel 3.1 help inventory, or the 116-file corpus. Shipping 3.x content produces gradients through `RIP_FILL_PATTERN` with alternating dither patterns across bands of a 256-color fade - the BLUEFADE.FN idiom.
- **Standing:** the 3.2 delta is sound; the baseline is the open question, and a good candidate for joint verification against the DLL. Note also that `|20`/`|28` occupy Level-2 **digit** slots, which the published tables do not use.
- **Detail:** [2.1 Radial Gradient](../3.2-riplib/ripscrip/2.1-radial-gradient.md#relationship-to-the-telegrafix-record)

### X7 - `<<DEBUG>>` directive

Not a naming conflict, but two properties worth raising before it is described as safe to leave in production:

- **It can shadow a macro.** In the 3.x layer `<<NAME>>` expands to the text variable `NAME` anywhere in a command's argument text, so `<<DEBUG msg>>` is syntactically indistinguishable from a macro reference to a variable named `DEBUG`. `<<ELSEIF>>` escapes this only because it appears solely between `<<IF>>` and `<<ENDIF>>`.
- **Unsolicited terminal-to-host traffic has no precedent.** Everything a 2.x/3.x terminal sends is a _response_ - auto-sense, mouse-field or button host command, `RIP_QUERY` result, file-query answer. A BBS at a prompt treats inbound bytes as keystrokes: `>DEBUG: entering menu render` + CR is a menu selection, not a dropped log line. Safe only for hosts written to expect it.
- **Detail:** [5.0 `<<DEBUG>>` Directive](../3.2-riplib/ripscrip/5.0-debug-directive.md#relationship-to-the-telegrafix-record)

---

## Free wins

### N1 - four commands misfiled as RIPlib extensions

`§DEV.4` marks `|!`, `|(`, `|)` and `|1R` as _"RIPlib extensions beyond the published TeleGrafix tables"_. All four are documented commands carried from 1.54 - RIP_COMMENT, RIP_GROUP_BEGIN / RIP_GROUP_END, and [RIP_READ_SCENE](../3.0/ripscrip/5.5-file-transfer-and-queries.md#rip_read_scene). Behavior matches in every case; only the standing is wrong. Reclassifying them removes four rows from the deviation register at zero behavioral cost. (`|1V`, `|1X` and the backtick composite-icon command have no counterpart here and are genuine additions.)

### N2 - `|x` missing from §A.1

§11.2 Erratum 2 gets the letter right - `x` filled poly-bezier, `z` unfilled - but §A.1 lists only `z`. Likely a table omission.

### N3 - AND and NOT are not new

§A2G.1 presents AND and NOT as v3.1 additions to `|W`. Both have been documented modes since **2.00 Alpha 1**, both appear in the RIPtel 3.1 help inventory, and AND is used in the shipping corpus for masking. What §A2G.1 genuinely contributes is _rendering_ them - `§DEAD.3` records the DLL accepting and silently ignoring both. If B1 resolves in favor of the documented table, §A2G.1 reduces to a completeness fix and one fewer extension to negotiate.

---

## Suggested resolution order

1. **B1** - highest impact, cheapest fix, and both projects' evidence already agrees.
2. **B2 / B3** - they govern how every corpus scene opens. One `grep` of any demo script settles the wire question; one look at the DLL dispatch table settles the other half.
3. **N1, N2, B11** - bookkeeping, no behavioral change, three fewer deviation-register entries.
4. **B4 / B5** - establish the provenance of RIPlib's punctuation-block and `|K` names. If they came from DLL strings, both sides hold real evidence and the task is matching names to opcodes.
5. **X1 / X2** - rename upstream; the record supplies every value under a name of its own.
6. **B12** - implement rule 12, which also retires X5.
7. **B6 / B7 / B8** - lower stakes, mostly resolvable by DLL dispatch-table inspection.

Items 2, 4 and part of 7 would likely fall out of a single pass: dump the DLL's dispatch table and compare it against the corpus opcode census. Either project can run one half of that.

## What is not in dispute

RIPlib's analysis of the shipping fill code is correct, it caught defects this repository had not documented, and in one place it corrected the record here - `§DEAD.7` establishes that `RIP_FILL` outlived its removal from the language, disproving an inference drawn here from the driver's silence. The pie/chord fill leak (`§BUG.6`), the never-applied patterned-flood brush (`§DEAD.7`), and RIPlib's replacements for both have been folded into this repository's own techspecs rather than treated as conflicts: see [2.0 fill defects](../2.0/techspecs/2.1-fill-defects.md) and the [3.0 delta](../3.0/techspecs/2.0-fill-defects-delta.md). The same goes for `§BUG.3`, `§BUG.4`, `§BUG.5` and `§BUG.9` - implementation defects no amount of corpus analysis would have surfaced.

---

[Contents](README.md)
