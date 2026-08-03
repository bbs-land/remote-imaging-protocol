# Implementation Notes

Practical guidance for implementations (renderers, terminals, libraries)
built from the specifications in this repository. These are **implementation
details, not language details** — the RIPscrip language docs under
`version/` intentionally do not prescribe them.

## Canvas sizes

Limit the virtual canvas to the era-appropriate resolutions:

| Version | Virtual canvas | Notes |
|---|---|---|
| 1.5x | **640×350** (fixed) | EGA, 16 colors from the EGA palette. The language hardwires this geometry. |
| 2.x / 3.x | **640×480**, **800×600**, or **1024×768** | Matches the resolutions RIPtel itself supported — its MicroANSI font file (`RIPscrip.maf`) carries per-resolution records for exactly these three modes. |

The 2.x/3.x wire protocol is resolution-independent (world coordinate
frames, `RIP_SET_WORLD_FRAME`); the canvas limit applies to the *device*
side an implementation renders into, not to world coordinates. World frames
map onto the chosen canvas — e.g. the RIPtel demos' standard 1280×960 world
frame is itself 4:3 and scales cleanly onto any of the three canvases.

## Aspect ratio

- **2.x / 3.x:** fix the displayed aspect ratio to **4:3**. All three
  canvas sizes are 4:3 with square pixels; when scaling to a modern display,
  preserve 4:3 (letterbox/pillarbox as needed) rather than stretching.
- **1.5x:** 640×350 has non-square pixels — on period hardware it filled a
  4:3 screen, so pixels are ~1.37× taller than wide. Render scaled to 4:3
  (e.g. 640×350 → 640×480 or an integer multiple) for period-correct
  proportions.

## Future

Widescreen and arbitrary-resolution support are deliberately out of scope
for now; future revisions of these notes can clarify wide support, scaling
policy, and high-DPI handling.
