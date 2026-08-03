# Data Backup Areas

[◀ Prev: Data Tables](03-data-tables.md) · [Contents](README.md) · [Next: Numbers, Coordinates & Math ▶](05-coordinates-and-math.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

## The Backup Architecture

*Evidence: 2.00a4; HLP.*

A data backup area is out-of-band storage for one type of [data table](03-data-tables.md) (or object, in the case of screens and mouse fields). Each backup area is composed of:

- **One base save area** — a slot-less temporary parking place. Restoring from it does *not* clear it, so it can be re-read any number of times. It cannot be protected.
- **Ten data save slots**, numbered **0–9** — individually addressable storage. Restoring from a slot clears that slot (unless it is protected).
- **A data save stack** — push/pop (LIFO) storage sharing capacity with the slots.

The slots and the stack draw from a shared pool of ten: any mix — four slots occupied and six tables pushed — fills the system, and further saves or pushes are ignored as errors. Overwriting an already-occupied, unprotected slot is still allowed when the pool is full. Each backup area maintains its own stack pointer, independent of every other backup area.

All told, a backup area holds up to eleven complete tables of its type — and since a table itself has 36 entries, one backup area can shelter 396 table entries.

The 3.0.7 driver confirms all three mechanisms directly: the `$SAVE0$`–`$SAVE9$` / `$RESTORE0$`–`$RESTORE9$` slot variables and the slot-less `$SAVE$`/`$RESTORE$` pair in RIPTEL.HLP's variable tables, stack PUSH/POP via the RIPSCRIP.HLP error strings **"No slots available for PUSH"** and **"No stack to pop"**, and per-object save/restore of button styles, graphics styles, text windows, palettes, mouse fields, environments, ports, and screens.

### The Fourteen Copy Paths

*Evidence: 2.00a4.*

Data moves between the live table, the base area, the slots, and the stack along fourteen distinct paths:

```text
      Data Table
    ╔════════════╗ 2         ╔═════════════════════════════════╗
    ║            ║  ◀────── ║         Base Save Area          ║
    ╟────────────╢  ──────▶ ╚═════════════════════════════════╝
    ║            ║ 1                               8 ▲   │ 11
    ╟────────────╢                                 9 │   │
    ║            ║           ╔═══╤═══╤═══╤═══╤═══╤═══╗   │
    ╟────────────╢ 4         ║   │   │  14 ──▶  │   ║   │
    ║    7 ⟳    ║  ◀────── ║   │   │   │   │   │   ║   │
    ╟────────────╢  ──────▶ ╚═══╧═══╧═══╧═══╧═══╧═══╝   │
    ║            ║ 3          Data Save Slots  13 ▲   │  │
    ╟────────────╢                                │   │12│10
    ║            ║ 6         ╔═════════════════════════════════╗
    ╟────────────╢  ◀────── ║         Data Save Stack         ║
    ║            ║  ──────▶ ╚═════════════════════════════════╝
    ╚════════════╝ 5
```

1. Copy a data table to the base save area
2. Copy the base save area to the data table
3. Copy a data table to a data save slot
4. Copy a data save slot to a data table
5. Push a data table onto the stack
6. Pop a data table from the stack into a data table
7. Copy one entry of a data table over another entry in the same table
8. Copy a data save slot to the base save area
9. Copy the base save area to a data save slot
10. Push the base save area onto the stack
11. Pop from the stack into the base save area
12. Push a data save slot onto the stack
13. Pop from the stack into a data save slot
14. Copy a data save slot to another data save slot

Push/pop is what makes overlapping dialogs work: opening a window pushes the environment it covers; closing it pops the covered environment back — nesting to any depth the pool allows.

## Protection and Restoration

*Evidence: 2.00a4.*

Data save slots (only those actually in use) can be individually protected. A protected slot survives restore-clearing and overwrites; it can only be changed by explicitly unprotecting it first, or by a hard reset. The base save area and the stack cannot be protected — protection contradicts their purposes.

Entries *inside* a saved table retain their own protection status: restoring a table from backup restores each entry's protection along with its data. A whole table cannot be protected, so a backup-area restore is the one operation that can overwrite protected entries in the live table (with the saved copies).

## The `$SAVE$` / `$RESTORE$` Variable Family

*Evidence: HLP.*

RIPTEL.HLP documents the uniform save/restore vocabulary the 3.0 engine exposes through [text variables](../../2.x/ripscrip/17-text-variables-general.md), pairing S-/R- forms per object type:

| Save | Restore | Object |
|---|---|---|
| `$SAVE$`, `$SAVE0$`–`$SAVE9$`, `$SAVEALL$` | `$RESTORE$`, `$RESTORE0$`–`$RESTORE9$`, `$RESTOREALL$` | Graphics screen (base area, slots 0–9, or everything at once) |
| `$SGS$` | `$RGS$` | Graphics style table |
| `$SBS$` | `$RBS$` | Button style table |
| `$STW$` | `$RTW$` | Text window table |
| `$SCP$` | `$RCP$` | Color palette table |
| `$SMF$` | `$RMF$` | Mouse field table |
| `$SCB$` | `$RCB$` | Clipboard |
| `$SENV$` | `$RENV$` | Environment ("record" / "activate snapshotted" environment) |

## Individual Backup Areas

*Evidence: 2.00a4; HLP.*

Backup areas exist for each of the following; each stores the current entry number plus the full 36-entry table unless noted:

- **Button style table** and **graphical style table** — table plus current entry number.
- **Drawing port table** — the 36 port definitions, the actual graphical contents of all offscreen/clipboard ports, the floating viewport query expression, and the clipboard port pointer. The 3.0.7 driver's strings confirm port backups carry **strip-based screen bitmap data** for the stored pixels.
- **Text window table** — the 36 definitions plus the floating text-window query expression; window *contents* are not saved.
- **Color palette table** — all 36 palettes plus the current entry number.
- **Mouse field table** — the count and up to 128 field definitions; restoring button fields restores the click regions, not the button graphics.
- **Screen** — a special backup area with no underlying table: each slot or base area holds a complete bitmap of the graphical screen with its associated palette and the status-bar state. This is the area behind `$SAVE0$`–`$SAVE9$` and `$SAVEALL$`.
- **Environment table** — all 36 environments plus the current entry number.

TeleGrafix's own demo scenes exercise the system constantly — the `.ENT`/`.EXT` transition stubs "Paste original screen image back" (their comment) around every menu excursion. *Evidence: corpus (MENU.ENT, TEL3X2.ENT).*

---

[◀ Prev: Data Tables](03-data-tables.md) · [Contents](README.md) · [Next: Numbers, Coordinates & Math ▶](05-coordinates-and-math.md)
