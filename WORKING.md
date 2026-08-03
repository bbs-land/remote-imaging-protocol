# WORKING — RIPscrip 3.x documentation

## Session 3: COMPLETE (2026-08-03) — 3.x edition restructured to the 2.x layout

- `version/3.x/ripscrip/` rebuilt: 22 pages + README (evidence legend),
  mirroring the 2.x structure; 111 command entries; ~142 text variables;
  new Column Text System page; File Formats appendix
- Whitepaper conversion relocated to `version/3.x/whitepaper/` (own README)
- Old reconstruction pages 09–12 absorbed and retired
- `temp/syncterm-missing-feature-rip3.md` written (SyncTERM gap checklist
  with GitHub links; all link targets/anchors verified)
- Verification: 23-file inventory, uniform H1/nav/legend structure,
  703 links across ripscrip+whitepaper+research — 0 broken
- Top-level README + TODO updated

Goal (user direction): a NEW `version/3.x/ripscrip/` structured like the 2.x
edition, with 3.x additions/removals flattened into their proper sections.
Evidence sources: 2.00a4 spec (inherited baseline), white paper, RIPSCRIP.HLP
command inventory + error-string limits, the 116-script demo corpus,
SyncTERM. Every page carries provenance tiers.

### Proposed file map (2.x → new 3.x)

| New 3.x file | Mirrors 2.x | 3.x deltas flattened in |
|---|---|---|
| `README.md` | README | contents + evidence-tier legend (documented / observed / reconstructed) |
| `01-introduction.md` | 01 | 3.0 identity (engine self-id "RIPterm v2.0", driver 3.0.7), revision history 2.00a4→3.0, differences from 2.00a4 and 1.54, `$COMPAT$`, GIF dropped |
| `02-drawing-ports.md` | 02 | ports 0–35 (0 = screen) confirmed; wipe-library usage patterns |
| `03-data-tables.md` | 03 | 36-slot confirmation via error strings; audio table entry in header segments |
| `04-data-backup-areas.md` | 04 | save slots 0–9, stack PUSH/POP, `$SAVE$`/`$RESTORE$` family |
| `05-coordinates-and-math.md` | 05 | MegaNum + UltraNum, `J` = SET_BASE_MATH wire opcode, SET_COORDINATE_SIZE, world frames (`fZKQO` = 1280×960 example) |
| `06-color-audio-text.md` | 06 | 256-color palettes, 8-bit-only direct RGB, palette animation; WAV background audio + tone family; extended text windows, MicroANSI `.maf` fonts |
| `07-protocol-definition.md` | 07 | introducers `!`/SOH/STX, level+sublevel decode, escapes/continuation/chaining, scene header segments, auto-sense `RIPSCRIP03000`, error tolerance |
| `08-level-0-commands-a-f.md` | 08 | new symbol commands inserted alphabetically-adjusted: skewed ovals `& - ] [ + _`, bounded text `"`, markers `;`, poly-polygon `<` |
| `09-level-0-commands-g-r.md` | 09 | `J` SET_BASE_MATH, `j` POINT, `K` FILLED_RECT, `N` SET_BORDER |
| `10-level-0-commands-s-z.md` | 10 | `y` extended font style (RFF font names; rotation crib), `b` extended text window |
| `11-level-1-commands.md` | 11 | IMAGE / IMAGE_STYLE / LOAD_BITMAP / PLAY_AUDIO with real corpus examples; `1e`/`1E` column regions; `1g`; bitmap-skin buttons (.BMH); `ID=` mouse-field identities |
| `12-level-2-commands.md` | 12 | port define/copy/delete/write + switches, with wipe-library observed usage |
| `13-level-3-9-commands.md` | 13 | baud emulation, delay, block mode, BeginEncodedStream (HLP); note: none observed in corpus |
| `14-host-commands.md` | 14 | control chars + backtick, `$GOTOURL$`, `ID=` |
| `15-local-playback-popup-lists.md` | 15 | `$>rip$ $)wav$ $<bmp$ $(jpg$` prefixes; 64-entry popup lists with `@desc` and `x,y:` placement |
| `16-templates-and-conditionals.md` | 16 | templates + `<<NAME>>` expansion + `<<IF/ELSE/ENDIF>>` conditional layer |
| `17-column-text-system.md` | *(new)* | `1e` columns, stream chaining, `1R` text flow, `$OVERFLOW$` paging, drop caps — flagship new 3.0 feature |
| `18…21-text-variables-*.md` | 17–20 | merged inventory: ~120 documented (HLP) + 269 observed; parameterized `$RESET(kw)$`, `$INUSE$`, `$COLORS$`, full user-var directive syntax + 20/100/255 limits |
| `22-file-formats.md` | 21 | ICN legacy (+ built-in ICN→BMP conversion), BMP/JPEG, RFF outline fonts, `.maf`, `.BMH`, RIPSCRIP.RES/.DB |

### Relocations

- Current faithful white-paper conversion (files 01–08) → `version/3.x/whitepaper/`
  (preserved as the historical document, nav intact, own README)
- Current reconstruction pages 09–12 → content absorbed into the new
  structure; pages retired
- `version/3.x/research/` unchanged (cited throughout)

### Execution shape (pending user approval)

4 writer agents (≤4 parallel per standing cap): (1) files 01–07, (2) command
references 08–13, (3) 14–17 + HCL/columns, (4) 18–22 text variables + formats.
Coordinator: README, relocation, nav chain, link/fidelity verification.

## Session 2 (COMPLETE): RIPtel 3.1 demo-corpus analysis

In-flight coordination. Prior session (3.x whitepaper conversion +
reconstruction, 12 files) completed and its log was removed; open follow-ups
live in TODO.md.

## Context

The extracted RIPtel 3.1 install (`~/src/rip-tools/artifacts/RIPtel/`,
driver 3.0.7, Oct 1997) contains ~110 authentic RIPscrip 3.0 script files
(.RIP/.FN/.DEF/.MNU/.MSE/.RET/.ENT/.EXT/.COL — self-documented via `|!`
comments), 8 undocumented `.RFF` outline fonts, 10 `.CHR` stroke fonts,
WinHelp documentation (RIPTEL.HLP 347KB, RIPSCRIP.HLP 38KB), palettes, and
demo images. First triage already shows: level-2 port commands in the wild
(`2P 2p 2C 2s`), new level-0/1 opcodes (`n J k M y`, `1b 1e 1g`),
parameterized `$RESET(OVERFLOW)$`/`$RESET(PAL)$`, `$-=NAME=value$`
set-syntax, 1280×960 world frame (`fZKQO`), SOH introducer in use.

## Task board (max 4 parallel agents)

| Task | Owner | Status |
|---|---|---|
| Opcode census of all ~110 script files → scratchpad report | D1 | done — 116 files, 22,921 commands, 72 opcodes, 11 completely new (skewed-oval family named by TeleGrafix's own comments; `J`=SET_BASE_MATH; `1e` columns) |
| Help-file / config text extraction → scratchpad report | D2 | done — RIPSCRIP.HLP = DLL string table: full ~90-command 3.0 inventory, error-string limits, complete text-variable language, `RIPSCRIP03000` handshake; engine self-identifies as "RIPterm v2.0" |
| Binary format triage (.RFF/.CHR/.BMH/.COL/.RES) → scratchpad report | D3 | done — .RFF = Atech FastFont outlines; .COL = column-system scripts (`1e`/`1R`, `$overflow$`, `<<if>>` conditionals); .BMH = button highlight BMPs; .CHR = classic BGI set |
| Integrate findings into `version/3.x/` docs | integrator | done — research reports preserved in `version/3.x/research/`; new page `ripscrip/12-riptel-demo-corpus.md`; pages 09/10/11 + README amended; 0 bad links across 16 files |

## STATUS: COMPLETE (2026-08-03)
| Update CONTRIBUTING artifacts table (RIPtel dir) | coordinator | done |
| Record restructure direction in TODO.md | coordinator | done |

## Direction change (user, 2026-08-03)

Future documentation need not preserve the whitepaper's structure; align
docs consistently across versions/features/subfeatures so each version
represents the whole feature space comparably.
