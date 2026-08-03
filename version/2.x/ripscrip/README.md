# RIPscrip Graphics Protocol Language Technical Reference

**Version 2.00 — Revision ALPHA 4** (proposed enhancements), December 13th, 1994

## Contents

### Introduction & Fundamentals

1. [Introduction](01-introduction.md) — what RIPscrip is, licensing, revision history (2.A0–2.A4), differences from 1.54, and backward compatibility
2. [Drawing Ports](02-drawing-ports.md) — screen and offscreen (clipboard) drawing ports, coordinates, viewports, and port-to-port copying
3. [Data Tables](03-data-tables.md) — the seven data table types (drawing port, color palette, graphical style, button style, text window, environment, mouse field) and protected entries
4. [Data Backup Areas](04-data-backup-areas.md) — protection and restoration of table state, and the individual save areas
5. [Numbers, Coordinates & Math](05-coordinates-and-math.md) — numerical parameter formats, base-math variations, world coordinate systems, and graphics mathematics
6. [Color, Audio & Text Windows](06-color-audio-text.md) — color translation, drawing palettes, direct RGB mode, default color lookup table, audio formats, terminal emulation, and overlapping windows

### Protocol & Command Reference

7. [Protocol Definition & Syntax](07-protocol-definition.md) — ANSI auto-sensing sequences, syntax rules, command levels, and how command entries are documented
8. [Level-0 Commands (A–F)](08-level-0-commands-a-f.md) — RIP_ARC through RIP_FONT_STYLE
9. [Level-0 Commands (G–R)](09-level-0-commands-g-r.md) — RIP_GOTOXY through RIP_ROUNDED_RECT
10. [Level-0 Commands (S–W)](10-level-0-commands-s-w.md) — RIP_SET_BASE_MATH through RIP_WRITE_MODE
11. [Level-1 Commands](11-level-1-commands.md) — buttons, mouse fields, images, icons, audio, queries, and text regions
12. [Level-2 Commands](12-level-2-commands.md) — port definition/copying and table switching commands
13. [Level-3 & Level-9 Commands](13-level-3-9-commands.md) — baud emulation, delay, block transfer mode, and uuencoded blocks

### Host Commands & Scripting

14. [Host Commands & Text Variable Basics](14-host-commands.md) — the "action language" tutorial, control characters, and text variable creation/query
15. [Local File Playback & Pop-Up Lists](15-local-playback-popup-lists.md) — local RIPscrip, audio, bitmap, and image playback, plus pop-up pick lists
16. [Templates](16-templates.md) — the host command template system, radio/check-box templates, chaining, and command blocks

### Text Variable Reference

17. [Text Variables: General, Date/Time & Sound](17-text-variables-general.md) — syntax descriptions, version/vendor, date/time, and sound variables
18. [Text Variables: Mouse, Text Window & Ports](18-text-variables-mouse-window.md) — mouse, text window, and port/viewport variables
19. [Text Variables: Terminal & Reset](19-text-variables-terminal.md) — terminal operation variables and `$RESET$`
20. [Text Variables: Environment, Clipboard, Screen & Tables](20-text-variables-environment.md) — environment configuration, clipboard, mouse field, screen, and data table/backup variables

### Appendix

21. [Icon/DIB File Format](21-icon-file-format.md) — the RIPscrip icon file format and Device Independent Bitmap (DIB) specification
