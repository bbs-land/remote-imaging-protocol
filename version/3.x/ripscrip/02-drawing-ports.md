# Drawing Ports

[◀ Prev: Introduction](01-introduction.md) · [Contents](README.md) · [Next: Data Tables ▶](03-data-tables.md)

*Reconstructed edition — see [Contents](README.md) for the evidence legend.*

## Drawing Ports — What They Are

*Evidence: 2.00a4; HLP.*

A drawing port is an area where graphics can be drawn. At any moment there is exactly one **current drawing port** — the port that receives graphical drawing operations. Ports come in two types:

1. **Screen (visual) drawing ports** — rectangular regions of the user's actual screen. Drawing to a screen port appears immediately. Screen ports divide the display into separate drawing areas that can be switched between at will.
2. **Offscreen bitmap drawing ports** (also called **clipboard ports**) — drawing surfaces that are not part of the visible screen. Graphics drawn to an offscreen port are invisible until copied to a screen port. Offscreen ports are used to compose scenes out of sight and reveal them whole, to cache bitmaps loaded from disk, to save screen regions under dialog boxes, and to drive transition animation (see the observed usage below).

The 3.0 driver additionally distinguishes **snapshot ports** — offscreen ports created by capturing screen contents — in its error strings ("No mouse/button fields on offscreen drawing ports"). *Evidence: HLP.*

### The Port Table

*Evidence: 2.00a4; HLP.*

Ports live in a 36-slot [data table](03-data-tables.md), entries numbered **0–35**:

- **Port 0 is always the screen.** It is the full size of the display, exists at all times, and cannot be created, deleted, or redefined — confirmed verbatim by the 3.0.7 driver's error strings "Can't delete graphics port #0" and "Can't create drawing port #0". You can always switch to port 0 to address the entire screen (its viewport may still be altered).
- **Ports 1–35** are user-definable — the driver rejects out-of-range numbers with "The given port number is invalid (1-35 only)". Each may be a screen sub-region or an offscreen bitmap.

Some things cannot be done to offscreen ports: mouse fields and clickable buttons only have meaning on the screen, so they may not be placed on an offscreen port (a 2.00a4 rule the 3.0.7 driver enforces with its own error string). Text windows and resident query expressions are likewise screen-only.

## Ports and Coordinates

*Evidence: 2.00a4.*

Each port has its own coordinate origin: (0,0) is the upper-left corner of the port, and a port of width W and height H runs to (W−1,H−1). Drawing a circle centered at (50,50) into a port whose upper-left sits at (10,10) on the screen actually paints it centered at (60,60) in screen terms — the port supplies "port relative" coordinates, so hosts can lay out graphs, game maps, and panels without recomputing absolute positions.

Screen ports are defined by their upper-left and lower-right corners on the screen (in the current [world coordinate system](05-coordinates-and-math.md)); from these the pixel dimensions are derived and remembered. Offscreen ports have no screen location — their upper-left is always (0,0).

Each port remembers its own drawing state, including:

- the current X/Y location (used by `RIP_TEXT`-family commands),
- the current image style settings,
- its viewport location and dimensions,
- any fixed viewport "resident query" expression,
- the current source X/Y location for `RIP_PORT_COPY`, `RIP_GET_IMAGE`, `RIP_PUT_IMAGE`, and `$PCB$` operations.

## Ports and Viewports

*Evidence: 2.00a4.*

Every drawing port has an associated **viewport** — a clipping rectangle inside the port that defines its drawing limits. Anything drawn beyond the viewport's edge is truncated (clipped), like coloring inside invisible lines. By default a port's viewport is the full size of the port; it can be redefined to any rectangle within the port. A viewport definition extending beyond the port is adjusted to fit; one completely outside the port is ignored as an error.

The viewport also shifts the coordinate origin: (0,0) for drawing operations is the upper-left corner of the *viewport*, not the port. All drawing operations are relative to, and clipped by, the current port's viewport. So a point at (50,50) inside a viewport that starts at (40,40) inside a port that starts at (25,25) on the screen lands at screen position (25+40+50, 25+40+50) = (115,115).

```text
╔═════════════════════════════ Screen ══╗
║ (0,0)                                 ║
║   ┌────────── Port (25,25 screen) ─┐  ║
║   │ (0,0) port                     │  ║
║   │    ┌──── Viewport (40,40) ──┐  │  ║
║   │    │ (0,0) viewport         │  │  ║
║   │    │        drawing here    │  │  ║
║   │    └────────────────────────┘  │  ║
║   └────────────────────────────────┘  ║
╚═══════════════════════════════════════╝
```

## Copying Data Between Ports

*Evidence: 2.00a4.*

Graphics data is copied between ports (or within one port) in rectangular blocks, via [RIP_PORT_COPY](../../2.x/ripscrip/12-level-2-commands.md#rip_port_copy) and related commands. The rules:

- **Both viewports apply.** The copy honors the viewport of the source *and* the destination port; pixels outside either viewport are not transferred. If the source or destination rectangle lies completely outside its port's viewport, the copy is not performed.
- **Mismatched rectangles scale.** The source and destination rectangles need not be the same size; if they differ in width or height, the source image is scaled to fit the destination rectangle. Only the data remaining after viewport truncation is scaled — no blank zones are ever introduced.
- Copies to and from **protected** ports are allowed: copying alters a port's *contents*, not its protected *configuration* (see [Data Tables](03-data-tables.md)).

## Observed Usage in the 3.0 Era

*Evidence: corpus (WIPE00.FN–WIPE24.FN); HLP.*

The port system is the workhorse of TeleGrafix's own 3.0 demo material. The RIPtel demo corpus contains **9,104 uses of `2C` (RIP_PORT_COPY)** — by far the most frequent command after `RIP_LINE` — almost all of them in the WIPE00–WIPE24 "wipe" transition library: each wipe composes the next scene in an offscreen port, then reveals it with a sequence of narrow port-to-screen copies to animate slides, splits, and dissolves. The supporting pattern (define port 1 full-screen, copy the screen into it, switch, draw, copy back) appears throughout the `.FN` function scenes, with TeleGrafix's own comments narrating it: "Create a port for the status line backup area, and copy the area where the status line will go into that drawing port."

The corpus uses only four level-2 port commands — `2P` (define), `2p` (delete), `2s` (switch), `2C` (copy) — confirming these as the practical core of the port system on the wire.

---

[◀ Prev: Introduction](01-introduction.md) · [Contents](README.md) · [Next: Data Tables ▶](03-data-tables.md)
