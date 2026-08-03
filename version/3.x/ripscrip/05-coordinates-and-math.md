# Numbers, Coordinates & Math

[◀ Prev: Data Backup Areas](04-data-backup-areas.md) · [Contents](README.md) · [Next: Color, Audio & Text Windows ▶](06-color-audio-text.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

## Numeric Formats: MegaNums and UltraNums

*Evidence: 2.00a4; WP §3.2.1.*

RIPscrip does not transmit decimal numbers. By default, numeric parameters are **base-36** ("Hexa-Tri-Decimal", or **MegaNums**), using the digits `0-9` followed by `A-Z` — the original 1.x numbering system.

The 2.x/3.0 language adds a second scheme: **base-64** ("Quadra-Hexa-Decimal", or **UltraNums**). Per the white paper, UltraNums use the same 36 characters for the lower part of the encoding spectrum, then continue with the lowercase letters and two symbols:

```text
0-9  A-Z  a-z  #  &
```

so `a` = 36, `z` = 61, `#` = 62, `&` = 63. Capacity by digit count:

| Digits | Decimal | Hexadecimal | MegaNums | UltraNums |
|---|---|---|---|---|
| 1 | 9 | 15 | 35 | 63 |
| 2 | 99 | 255 | 1,295 | 4,095 |
| 3 | 999 | 4,095 | 46,655 | 262,143 |
| 4 | 9,999 | 65,535 | 1,679,615 | 16,777,215 |
| 5 | 99,999 | 1,048,575 | 60,466,175 | 1,073,741,823 |

Most numeric fields are two digits wide, giving a fundamental range of 0–1295 (MegaNum) or 0–4095 (UltraNum) per field; the base-math setting is how a fixed-width field accommodates larger worlds without changing the command format. (The 2.00a4 caution stands: UltraNums beyond 5 digits exceed 32-bit arithmetic and should be avoided.) A handful of commands mandate a specific base regardless of the global setting; the command pages note these exceptions.

One wire-level quirk observed in the shipping driver: its MegaNum reader treats `-` as digit **0**, which is why stray `!|----` separator lines in TeleGrafix's own files parse harmlessly (see [Protocol Definition](07-protocol-definition.md)). SyncTERM reproduces this exactly (`parse_mega`, ripper.c:9120–9121). *Evidence: corpus; SyncTERM.*

## RIP_SET_BASE_MATH — the `J` Opcode

*Evidence: corpus (ONLINE.RIP and 89 other files); HLP; 2.00a4.*

The global base math is selected by **RIP_SET_BASE_MATH** — confirmed by name in the 3.0.7 driver's command inventory (`RIP_SetBaseMath`). Its actual wire opcode is level-0 **`J`**, taking one 2-digit argument giving the base *in base 36* (`10` = 36, i.e. MegaNums; `1S` = 64, UltraNums):

```text
!|J10                   Set base math to MegaNums (base 36)
```

That line — inline comment included — is TeleGrafix's own, from ONLINE.RIP; `J10` opens 90 of the 116 RIPtel demo scripts as part of the standard prologue.

This resolves a defect in the 2.00 ALPHA 4 draft, which assigned level-0 `b` to **both** RIP_SET_BASE_MATH and RIP_EXTENDED_TEXT_WINDOW — an unshippable collision. The 3.0 driver's corpus shows the resolution: SET_BASE_MATH moved to `J` on the wire, leaving `b` to the extended text window. This edition documents `J` as the 3.0 opcode throughout.

## RIP_SET_COORDINATE_SIZE

*Evidence: 2.00a4; HLP; corpus.*

By default all X/Y coordinates are 2-digit fields. **RIP_SET_COORDINATE_SIZE** (`RIP_SetCoordinateSize` in the 3.0.7 inventory; wire opcode `n`) widens coordinate parameters to 3, 4, or 5 digits for very large world coordinate systems — a 2-digit MegaNum tops out at 1,295, so worlds wider than that in a single axis need wider fields or UltraNums. The 3.0.7 driver validates the setting ("Invalid coordinate size", and "Invalid coordinate size in environment header segment" for the scene-header form), and the demo prologue `n2000` pins 2-digit coordinates explicitly in nearly every RIPtel scene. Commands whose coordinate fields obey this setting are marked `:XY` in the command pages.

## World Coordinate Systems

*Evidence: 2.00a4; WP §4.1; corpus.*

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

*Evidence: 2.00a4.*

Resolution independence forces a precise mathematical model of what a coordinate *is*. The 2.00a4 treatment carries into 3.0 unchanged; the essentials:

**Coordinates are the lines between pixels, not the pixels.** Think of the drawing surface as graph paper where the infinitely thin grid lines are the coordinates and pixels are the cells between them. This model is what makes shapes survive translation between resolutions without gaps or overlaps.

**Filled areas fill between the boundary lines.** A filled rectangle from (2,1) to (5,4) colors the cells strictly between those coordinate lines — so its right and bottom pixel edges are "inset" one pixel relative to naive pixel addressing. Two filled rectangles sharing an edge coordinate ((2,1)–(5,4) and (5,1)–(8,4)) tile perfectly with no gap and no overlap, at every resolution. This is why 2.x/3.0 filled objects appear one pixel narrower/shorter on their right/bottom edges than their 1.54 counterparts.

**Lines and points round up to the next pixel.** An outline or point at coordinate (4,2) lights the pixel cell between lines 4–5 and 2–3. Consequently an unfilled rectangle is one pixel wider and taller than the same-coordinate filled rectangle, and a border drawn around a filled region hugs its outside edge.

**Practical summary:** for fills, think "coordinates between pixels"; for lines, points, and curves, think "coordinates address pixels" — the models only diverge for filled interiors. Filled ovals centered on a point are slightly asymmetric (interior 8 pixels one side of center, 7 the other, for even radii); drawing the border in the fill color smooths this. Filled polygons and curved regions follow the same fill-between-lines rule, with their borders (optional in 2.x/3.0 via RIP_SET_BORDER) rounding up like lines.

Worked diagrams for all of these cases are preserved in the [2.x edition's chapter](../../2.x/ripscrip/05-coordinates-and-math.md), which applies to 3.0 verbatim.

## Limits Snapshot

*Evidence: WP; HLP.*

| Quantity | Limit | Source |
|---|---|---|
| Maximum coordinate value / world frame axis | 32,767 | WP |
| Default coordinate field width | 2 digits (`n2000` prologue) | corpus |
| Coordinate size range | 2–5 digits | 2.00a4; HLP validates |
| Polygon-class vertex count | 4,096 | HLP ("polygon-type objects to have 4096 vertices") |
| Base math values | 36 (MegaNum), 64 (UltraNum) | 2.00a4; WP |

---

[◀ Prev: Data Backup Areas](04-data-backup-areas.md) · [Contents](README.md) · [Next: Color, Audio & Text Windows ▶](06-color-audio-text.md)
