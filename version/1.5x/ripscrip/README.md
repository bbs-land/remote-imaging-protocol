# RIPscrip Graphics Protocol Specification

**"Remote Imaging Protocol" — Revision 1.54** (July 19th, 1993)

## Contents

### General

1. [Introduction](01-introduction.md) — what RIPscrip is, revision notation, licensing, and how it works
2. [Protocol Structure & Syntax](02-protocol-structure.md) — general structure, syntax rules, ANSI auto-sensing sequences, and miscellaneous notes
3. [Command Reference Overview](03-command-reference.md) — how command entries are documented, plus a full command index

### Command Reference

4. [Window Commands](04-window-commands.md) — text window, graphics viewport, reset/erase, and cursor positioning
5. [Colors & Attributes](05-colors-and-attributes.md) — drawing color, palettes, and write mode
6. [Text Output & Fonts](06-text-output.md) — drawing position, text output, and font styles
7. [Drawing Primitives](07-drawing-primitives.md) — pixels, lines, rectangles, circles, ovals, arcs, pie slices, béziers, polygons, and flood fill
8. [Line & Fill Styles](08-line-fill-styles.md) — line styles/thickness, fill styles, and custom fill patterns
9. [Mouse Fields](09-mouse-fields.md) — rectangular hot mouse regions
10. [Text Regions](10-text-regions.md) — formatted rectangular text regions
11. [Images & Icons](11-images-icons.md) — clipboard image copy/paste and disk-based icons
12. [Buttons](12-buttons.md) — button styles and mouse buttons
13. [Advanced Commands](13-advanced-commands.md) — text variable definition/query, region copy, scene playback, file query, and block transfer mode

### Host Commands & Scripting

14. [Host Commands & Control Characters](14-host-commands.md) — the "action language" tutorial and control-character specification
15. [Text Variables](15-text-variables.md) — pre-defined text variables reference
16. [Local RIPscrip File Playback](16-local-playback.md) — replaying local `.RIP` files via text variables
17. [Pop-Up Lists](17-popup-lists.md) — pop-up pick-list definitions
18. [Host Command Templates](18-host-command-templates.md) — the host command "template" system
19. [Text Variable Creation & Query](19-text-variable-creation.md) — user variables and querying variable contents

### Appendix

20. [Icon File Format](20-icon-file-format.md) — the RIPscrip icon file format specification _(v1.54)_
