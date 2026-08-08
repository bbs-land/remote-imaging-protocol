# Numbers, Coordinates & Math

[◀ Prev: Data Backup Areas](04-data-backup-areas.md) · [Contents](README.md) · [Next: Color, Audio & Text Windows ▶](06-color-audio-text.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

## Numeric Formats: MegaNums and UltraNums

_Evidence: 2.00a4; WP §3.2.1._

RIPscrip does not transmit decimal numbers. By default, numeric parameters are **base-36** ("Hexa-Tri-Decimal", or **MegaNums**), using the digits `0-9` followed by `A-Z` — the original 1.x numbering system.

The 2.x/3.0 language adds a second scheme: **base-64** ("Quadra-Hexa-Decimal", or **UltraNums**). Per the white paper, UltraNums use the same 36 characters for the lower part of the encoding spectrum, then continue with the lowercase letters and two symbols:

```text
0-9  A-Z  a-z  #  &
```

so `a` = 36, `z` = 61, `#` = 62, `&` = 63. Capacity by digit count:

| Digits | Decimal | Hexadecimal | MegaNums   | UltraNums     |
| ------ | ------- | ----------- | ---------- | ------------- |
| 1      | 9       | 15          | 35         | 63            |
| 2      | 99      | 255         | 1,295      | 4,095         |
| 3      | 999     | 4,095       | 46,655     | 262,143       |
| 4      | 9,999   | 65,535      | 1,679,615  | 16,777,215    |
| 5      | 99,999  | 1,048,575   | 60,466,175 | 1,073,741,823 |

Most numeric fields are two digits wide, giving a fundamental range of 0–1295 (MegaNum) or 0–4095 (UltraNum) per field; the base-math setting is how a fixed-width field accommodates larger worlds without changing the command format. (The 2.00a4 caution stands: UltraNums beyond 5 digits exceed 32-bit arithmetic and should be avoided.) A handful of commands mandate a specific base regardless of the global setting; the command pages note these exceptions.

One wire-level quirk observed in the shipping driver: its MegaNum reader treats `-` as digit **0**, which is why stray `!|----` separator lines in TeleGrafix's own files parse harmlessly (see [Protocol Definition](07-protocol-definition.md)). SyncTERM reproduces this exactly (`parse_mega`, ripper.c:9120–9121). _Evidence: corpus; SyncTERM._

## RIP_SET_BASE_MATH — the `J` Opcode

_Evidence: corpus (ONLINE.RIP and 89 other files); HLP; 2.00a4._

The global base math is selected by **RIP_SET_BASE_MATH** — confirmed by name in the 3.0.7 driver's command inventory (`RIP_SetBaseMath`). Its actual wire opcode is level-0 **`J`**, taking one 2-digit argument giving the base _in base 36_ (`10` = 36, i.e. MegaNums; `1S` = 64, UltraNums):

```text
!|J10                   Set base math to MegaNums (base 36)
```

That line — inline comment included — is TeleGrafix's own, from ONLINE.RIP; `J10` opens 90 of the 116 RIPtel demo scripts as part of the standard prologue.

This resolves a defect in the 2.00 ALPHA 4 draft, which assigned level-0 `b` to **both** RIP_SET_BASE_MATH and RIP_EXTENDED_TEXT_WINDOW — an unshippable collision. The 3.0 driver's corpus shows the resolution: SET_BASE_MATH moved to `J` on the wire, leaving `b` to the extended text window. This edition documents `J` as the 3.0 opcode throughout.

## Choosing MegaNums vs UltraNums

_Evidence: 2.00a4; corpus; SyncTERM (ripper.c:9107–9131)._

Which base a command's parameters use is a **global mode**, not a per-command choice: after `!|J10` every base-math-governed field in every subsequent command reads as base-36; after `!|J1S`, base-64. Individual commands differ only in whether they _obey_ the mode — each entry in the command pages carries a base-math line, which is one of:

- **current setting** — the common case: coordinates, dimensions, radii, vertex counts, and most numeric fields.
- **MegaNums only** — commands that must parse before the mode is knowable or that reset it: `RIP_SET_BASE_MATH` itself (its `10`/`1S` argument is always base-36), and anything that follows a [RIP_RESET_WINDOWS](08-level-0-commands-symbols-a-f.md#rip_reset_windows) (a reset silently returns the mode to base 36 — re-send `J` immediately if the scene needs otherwise).
- **not numeric** — text parameters (always last in a command) ignore base math entirely.

### Worked encodings

A field's value is `first_digit × base + second_digit` (and so on for wider fields). Digit values: `0`–`9` = 0–9, `A`–`Z` = 10–35, then (UltraNums only) `a`–`z` = 36–61, `#` = 62, `&` = 63.

| Value | 2-digit MegaNum | 2-digit UltraNum |
| ----- | --------------- | ---------------- |
| 14    | `0E`            | `0E`             |
| 64    | `1S`            | `10`             |
| 960   | `QO` (26×36+24) | `F0` (15×64+0)   |
| 1280  | `ZK` (35×36+20) | `K0` (20×64+0)   |
| 1295  | `ZZ` (max)      | `KF`             |
| 4095  | — overflows     | `&&` (max)       |

So the ubiquitous corpus line `!|fZKQO` is a 1280×960 world frame in MegaNums — and note that TeleGrafix's chosen frame fits 2-digit MegaNums with 15 to spare (1295 max). A constructed UltraNum equivalent would be `!|J1S|fK0F0` _(constructed example — see the caution below)_.

### Decision guide

| Largest coordinate in either axis | Recommended encoding |
| --- | --- |
| ≤ 1,295 | 2-digit MegaNums (the default; what every TeleGrafix scene does) |
| 1,296 – 4,095 | 2-digit UltraNums (`J1S`) — same wire size, larger range — **or** 3-digit MegaNums via [RIP_SET_COORDINATE_SIZE](#rip_set_coordinate_size) |
| 4,096 – 46,655 | 3-digit MegaNums, or 3-digit UltraNums (max 262,143, capped by the 32,767 world limit) |
| larger | wider coordinate size; keep total field width ≤ 5 digits (32-bit arithmetic limit per 2.00a4) |

UltraNums earn one byte per coordinate over 3-digit MegaNums in the 1,296–4,095 band — meaningful at 2,400 baud when a scene is thousands of drawing operations.

### Interoperability caution

_Evidence: corpus; SyncTERM._

**No UltraNum usage has been observed in the wild.** All 116 TeleGrafix demo scripts run `J10` MegaNums, and SyncTERM cannot parse UltraNums at all — its number reader (`parse_mega`, ripper.c:9107–9131) treats `a`–`z` as _case-insensitive base-36 digits_, so a base-64 scene would silently mis-decode rather than fail. MegaNums are the only wire-proven encoding; treat UltraNums as documented-but-unexercised.

## RIP_SET_COORDINATE_SIZE

_Evidence: 2.00a4; HLP; corpus._

By default all X/Y coordinates are 2-digit fields. **RIP_SET_COORDINATE_SIZE** (`RIP_SetCoordinateSize` in the 3.0.7 inventory; wire opcode `n`) widens coordinate parameters to 3, 4, or 5 digits for very large world coordinate systems — a 2-digit MegaNum tops out at 1,295, so worlds wider than that in a single axis need wider fields or UltraNums. The 3.0.7 driver validates the setting ("Invalid coordinate size", and "Invalid coordinate size in environment header segment" for the scene-header form), and the demo prologue `n2000` pins 2-digit coordinates explicitly in nearly every RIPtel scene. Commands whose coordinate fields obey this setting are marked `:XY` in the command pages.

## World Coordinate Systems

_Evidence: 2.00a4; WP §4.1; corpus._

RIPscrip 2.x/3.0 is resolution independent. Two coordinate frames exist:

1. **Device coordinate frame** — the physical resolution of the user's display, chosen by the terminal, invisible to the language.
2. **World coordinate frame** — the master coordinate system all RIPscrip coordinates are expressed in, set by **RIP_SET_WORLD_FRAME** (wire opcode `f`). The terminal maps world coordinates to device pixels; ideally the world frame is at least as large as the device frame so no resolution is lost in mapping. When the two match, mapping is the identity.

Per the white paper, the world coordinate system may be defined up to a maximum of **32,767 × 32,767** — "plenty of room to accommodate video hardware over the next several decades." (The 2.00a4 draft had said 65535×65535; the shipping figure is the signed-16-bit bound.) For 1.54 compatibility the world frame defaults to 640×350; the `$COMPAT$` variable restores that environment (see [Introduction](01-introduction.md)).

A real 3.0 example, from the RIPtel demo prologue:

```text
!|fZKQO                 Set world coordinats to 1280x960
```

(comment and spelling TeleGrafix's own, ONLINE.RIP): `ZK` = 35×36+20 = **1280**, `QO` = 26×36+24 = **960**. The demos thus run in a 1280×960 world regardless of the user's actual screen mode. Alternate frames observed in the corpus: `HSDC` = 640×480, `HR9S` = 639×352, `HRDC` = 639×480.

## The Mathematics of Graphics and Coordinates

_Evidence: 2.00a4._

Resolution independence forces a precise mathematical model of what a coordinate _is_. The 2.00a4 treatment carries into 3.0 unchanged; the essentials:

**Coordinates are the lines between pixels, not the pixels.** Think of the drawing surface as graph paper where the infinitely thin grid lines are the coordinates and pixels are the cells between them. This model is what makes shapes survive translation between resolutions without gaps or overlaps.

**Filled areas fill between the boundary lines.** A filled rectangle from (2,1) to (5,4) colors the cells strictly between those coordinate lines — so its right and bottom pixel edges are "inset" one pixel relative to naive pixel addressing. Two filled rectangles sharing an edge coordinate ((2,1)–(5,4) and (5,1)–(8,4)) tile perfectly with no gap and no overlap, at every resolution. This is why 2.x/3.0 filled objects appear one pixel narrower/shorter on their right/bottom edges than their 1.54 counterparts.

**Lines and points round up to the next pixel.** An outline or point at coordinate (4,2) lights the pixel cell between lines 4–5 and 2–3. Consequently an unfilled rectangle is one pixel wider and taller than the same-coordinate filled rectangle, and a border drawn around a filled region hugs its outside edge.

**Practical summary:** for fills, think "coordinates between pixels"; for lines, points, and curves, think "coordinates address pixels" — the models only diverge for filled interiors. Filled ovals centered on a point are slightly asymmetric (interior 8 pixels one side of center, 7 the other, for even radii); drawing the border in the fill color smooths this. Filled polygons and curved regions follow the same fill-between-lines rule, with their borders (optional in 2.x/3.0 via RIP_SET_BORDER) rounding up like lines.

Worked diagrams for all of these cases are preserved in the [2.x edition's chapter](../../2.x/ripscrip/05-coordinates-and-math.md), which applies to 3.0 verbatim.

## Limits Snapshot

_Evidence: WP; HLP._

| Quantity | Limit | Source |
| --- | --- | --- |
| Maximum coordinate value / world frame axis | 32,767 | WP |
| Default coordinate field width | 2 digits (`n2000` prologue) | corpus |
| Coordinate size range | 2–5 digits | 2.00a4; HLP validates |
| Polygon-class vertex count | 4,096 | HLP ("polygon-type objects to have 4096 vertices") |
| Base math values | 36 (MegaNum), 64 (UltraNum) | 2.00a4; WP |

---

[◀ Prev: Data Backup Areas](04-data-backup-areas.md) · [Contents](README.md) · [Next: Color, Audio & Text Windows ▶](06-color-audio-text.md)
