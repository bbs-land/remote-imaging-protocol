# Data Tables

[◀ Prev: Drawing Ports](02-drawing-ports.md) · [Contents](README.md) · [Next: Data Backup Areas ▶](04-data-backup-areas.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

## Data Tables — A Fundamental Concept

*Evidence: 2.00a4; HLP.*

A data table is a table of like-typed configuration entries maintained by the RIPscrip environment: color palettes, graphical styles, button styles, [drawing ports](02-drawing-ports.md), text window definitions, environments, and mouse fields. Most tables have one entry that is "currently active" (the current text window, the current palette, and so on); entries can be *in use* and can be *protected* from modification.

The 3.0.7 driver's error strings confirm the table architecture directly: every general table has **36 entries, numbered 0–35** ("Invalid … entry number (0-35)", "Invalid button group number (>35)"), entries can be PROTECTed and unprotected, and data can be copied from one entry to another. Entry contents survive until overwritten, cleared by a reset, or removed by a hard reset (a hard reset returns the entire RIPscrip environment to ground zero).

### Entry 0 — the Common Modifiable Entry

*Evidence: HLP.*

Entry 0 of each table is special: it is the always-present working entry that **cannot be deleted or protected**. The driver enforces this per table — "Cannot protect current text window slot when it is #0", "Can't protect environment data table entry #0", "Can't protect graphics style data table entry zero", "Can't protect color palette zero", and for ports, "Can't delete graphics port #0". Entry 0 is thus the common, freely modifiable entry every scene can rely on; protection is reserved for entries 1–35.

## The Table Inventory

*Evidence: 2.00a4; HLP.*

The 2.00a4 draft defined six general table groups plus the special mouse-field table. The 3.0 engine's [scene header segments](07-protocol-definition.md#scene-header-segments) enumerate per-table flags and entry numbers for **button style, graphics style, drawing port, text window, color palette, environment, mouse field, audio, and graphics screen** — adding an **audio** entry to the inventory, and treating the graphics screen as a headerable object class alongside the tables. The `RIP_Switch*` command family in the 3.0 command inventory covers the switchable tables (`RIP_SwitchButtonStyle`, `RIP_SwitchEnvironment`, `RIP_SwitchPalette`, `RIP_SwitchPort`, `RIP_SwitchStyle`, `RIP_SwitchTextWindow`, plus `RIP_SwitchDirectory`).

### Drawing Port Table

*Evidence: 2.00a4; HLP.*

Up to 36 ports defined simultaneously; port 0 is always the screen; the remaining 35 may be screen sub-regions or offscreen clipboard ports. Only one port is current at a time. Ports are defined in the current world coordinate system, from which physical pixel dimensions are derived and remembered. See [Drawing Ports](02-drawing-ports.md) for what each entry stores.

### Color Palette Table

*Evidence: 2.00a4; HLP.*

Each entry is a complete 256-color palette (color lookup table). Up to 36 palettes may be held at once, and switching entries retargets the whole color scheme in a couple of bytes — on palette-driven hardware the screen colors change instantly; in direct-color modes the switch affects subsequent drawing only. The 3.0.7 driver caps palette indices at 0–255 and enforces the per-command limits described in [Color, Audio & Text Windows](06-color-audio-text.md). A normal reset restores unprotected palette entries to the boot-up defaults; a hard reset resets the whole table.

### Graphical Style Table

*Evidence: 2.00a4.*

36 entries; entry 0 is current at startup with default values. Each entry captures the active drawing attributes: drawing color, background color, fill pattern and fill color, line pattern and odd-dash rule, mouse cursor style, font (number or extended font name, size, orientation, alignment), write mode, and color mode (palette vs. direct RGB). Pre-loading several styles and switching between them replaces long attribute sequences with a two-byte switch.

### Button Style Table

*Evidence: 2.00a4; HLP.*

36 entries, each a complete button style as defined by `RIP_BUTTON_STYLE`. The 3.0.7 driver additionally fixes button semantics via its error strings: button types are **plain, icon, or snapshot**; labels and hotkey codes are limited to 255; button groups are numbered 0–35.

### Text Window Table

*Evidence: 2.00a4.*

36 entries, each a text window definition: corner coordinates (in text coordinates), MicroANSI font number, current ANSI attributes, cursor position and visibility, vertical scrolling margins, and activation state. The window's *contents* are not part of the definition. The default window is full-screen 80×25. A deactivated current window discards incoming raw text. See [Color, Audio & Text Windows](06-color-audio-text.md) for the extended (resolution-independent) text window added in this era.

### Environment Table

*Evidence: 2.00a4; HLP.*

36 entries, each a full drawing context: the current entry numbers of the other tables (style, button style, port, text window, palette), the world coordinate dimensions, base math (36 or 64), [coordinate size](05-coordinates-and-math.md), color mode, mouse pointer, and baud-rate emulation value. Switching environments is a whole-context swap — an entire drawing world can be exchanged for another and back without destroying anything. The mouse-field table has no "current entry," so environment switches do not affect mouse fields.

### Mouse Field Table

*Evidence: 2.00a4; HLP.*

Unlike the others: up to **128** mouse field definitions, appended in order received, with no current-entry concept and no per-entry protection. Mouse fields pertain only to port 0 (the screen). The 3.0.7 driver adds the `ID=n:` prefix convention for assigning numbered identities to mouse fields (see the host-command pages).

### Audio (New in 3.0)

*Evidence: HLP.*

The scene header segment mechanism includes an **audio** entry alongside the classic tables ("…mouse field / audio / graphics screen" in the header-segment error-string family), indicating the 3.0 engine tracks audio state as a headerable object class. No slot-count error strings for audio have been recovered, so its table dimensions are unconfirmed *(hypothesis: state entry rather than a 36-slot table)*. Audio behavior itself is covered in [Color, Audio & Text Windows](06-color-audio-text.md).

## Protected Data Table Entries

*Evidence: 2.00a4; HLP.*

A protected entry cannot be cleared by a normal reset or a clear-entry operation, and cannot be modified, until it is unprotected or a hard reset is performed. Protection lets a host park expensive definitions (styles, palettes, windows, ports) in the terminal once and rely on them across doors and sub-systems that may perform their own resets, saving retransmission.

"Not modifiable" means the entry's *configuration* is frozen — you cannot redefine a protected port's boundaries or a protected style's colors — but ordinary operations against the object are still allowed: drawing into a protected port, writing text and ANSI sequences into a protected text window, copying pixel data to or from a protected port. (One 2.00a4 nicety carries over: the VT-102 wrap/chop escape sequence still works on protected text windows, since ANSI operations are permitted on them.)

The 3.0.7 driver's PROTECT machinery is confirmed by its error strings, including the entry-0 exclusions listed above, and by the `$PROTECT$`/`$ISPROT$`/`$UNPROT$`-family text-variable processors present in its string table.

---

[◀ Prev: Drawing Ports](02-drawing-ports.md) · [Contents](README.md) · [Next: Data Backup Areas ▶](04-data-backup-areas.md)
