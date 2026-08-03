# RIPscrip 3.0 Protocol Reference (Reconstructed Edition)

**RIPscrip 3.0** — the third-generation Remote Imaging Protocol scripting
language (TeleGrafix Communications, Inc., 1996–1997), as shipped in RIPtel
Visual Telnet 3.1 (RIPscrip driver 3.0.7).

TeleGrafix never published a RIPscrip 3.0 language reference. This edition is
an **editorial reconstruction**, structured to mirror the
[2.x specification](../../2.x/ripscrip/README.md) that 3.0 directly evolved
from, with the 3.x-era changes flattened into their proper sections. The
official [3.0 Technical White Paper](../whitepaper/README.md) (prose only) is
preserved separately as a faithful conversion.

## Evidence legend

Every section and command entry carries an evidence tag:

| Tag | Meaning |
|---|---|
| `2.00a4` | Documented in the RIPscrip 2.00 alpha 4 specification, which 3.0 inherits (see the [2.x edition](../../2.x/ripscrip/README.md)) |
| `WP` | Stated in the official 3.0 Technical White Paper |
| `HLP` | Recovered from the RIPtel 3.1 help files / RIPSCRIP.DLL string table |
| `corpus (FILE)` | Observed in the RIPtel 3.1 demo scripts (116 authentic TeleGrafix RIPscrip 3.0 files) |
| `SyncTERM (ripper.c:N)` | Behavior of SyncTERM's open-source RIP 3.0 implementation |
| *(hypothesis)* | Editorial inference — plausible but unconfirmed |

Underlying research (full data, byte layouts, opcode census):
[script census](../research/riptel-script-census.md) ·
[help-file extraction](../research/riptel-help-extraction.md) ·
[binary formats](../research/riptel-binary-formats.md)

## Contents

### Introduction & Fundamentals

1. [Introduction](01-introduction.md) — what RIPscrip 3.0 is, its evidence basis, lineage from 2.00a4, and differences from earlier versions
2. [Drawing Ports](02-drawing-ports.md) — screen and offscreen ports (0–35), viewports, and port-to-port copying
3. [Data Tables](03-data-tables.md) — the data-table types, 36-entry slots, protection, and header-segment table state
4. [Data Backup Areas](04-data-backup-areas.md) — save slots, the backup stack, and save/restore semantics
5. [Numbers, Coordinates & Math](05-coordinates-and-math.md) — MegaNums, UltraNums, base math, coordinate size, and world frames
6. [Color, Audio & Text Windows](06-color-audio-text.md) — palettes and direct RGB, digitized audio and tones, and the text-window system
7. [Protocol Definition & Syntax](07-protocol-definition.md) — introducers, command levels, escaping, chaining, auto-sensing, and scene header segments

### Protocol & Command Reference

8. [Level-0 Commands (Symbols & A–F)](08-level-0-commands-symbols-a-f.md) — including the newly identified skewed-oval, bounded-text, marker, and poly-polygon commands
9. [Level-0 Commands (G–R)](09-level-0-commands-g-r.md) — including `J` RIP_SET_BASE_MATH and the scene-header command
10. [Level-0 Commands (S–Z)](10-level-0-commands-s-z.md) — including the extended (outline) font style command
11. [Level-1 Commands](11-level-1-commands.md) — UI objects, images (BMP/JPEG), audio playback, and text-column regions
12. [Level-2 Commands](12-level-2-commands.md) — port definition/copying and table switching
13. [Level-3 & Level-9 Commands](13-level-3-9-commands.md) — baud emulation, delay, and block transfer

### Host Commands & Scripting

14. [Host Commands](14-host-commands.md) — the action language, control characters, mouse-field identities, and web integration
15. [Local File Playback & Pop-Up Lists](15-local-playback-popup-lists.md) — the `$>` `$)` `$<` `$(` playback prefixes and pick lists
16. [Templates & Conditionals](16-templates-and-conditionals.md) — the template system and the 3.0 `<<IF>>` macro/conditional layer
17. [Column Text System](17-column-text-system.md) — flowed-text columns, streams, overflow pagination — 3.0's flagship addition

### Text Variable Reference

18. [Text Variables: General, Date/Time & Sound](18-text-variables-general.md) — fundamentals, user-defined variables, and the general/date/time/sound groups
19. [Text Variables: Mouse, Text Window & Ports](19-text-variables-mouse-window.md) — mouse, text-window, port, and image-style variables
20. [Text Variables: Terminal & Reset](20-text-variables-terminal.md) — terminal-control variables and the parameterized `$RESET(…)$` family
21. [Text Variables: Environment, Clipboard, Screen & Tables](21-text-variables-environment.md) — save/restore, clipboard, environment queries
22. [File Formats](22-file-formats.md) — ICN, BMP/JPEG, RFF outline fonts, MicroANSI fonts, and the RIPtel resource formats
