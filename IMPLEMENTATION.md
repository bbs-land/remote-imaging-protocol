# Implementation Notes

Practical guidance for implementations (renderers, terminals, libraries)
built from the specifications in this repository. These are **implementation
details, not language details** — the RIPscrip language docs under
`version/` intentionally do not prescribe them.

## Canvas sizes

For content creators, limit the virtual canvas to the era-appropriate resolutions:

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

* Because era displays were mostly a **4:3** ratio, as well as the supported 2.x 
  display options, it's best to have the viewport in your application fixed to a 
  **4:3** aspect ratio.
* **1.3x** used non-square pixels with a resolution of 640x350 scaled to a 4:3 
  physical display.
- **2.x / 3.x:** fix the displayed aspect ratio to **4:3**. All three ANSI font 
  canvas sizes are 4:3 with square pixels; when scaling to a modern display,
  preserve 4:3 rather than stretching (letterbox/pillarbox as needed).

## Rendering

* **1.5x** scaling - For "classic" preservation of rendering, it's suggested 
  to render each pixel as a scaled 3px wide and 4px tall, which will correct for
  the aspect ratio.  You can then apply a mild blur/soften filter before bilinear 
  scaling to the native viewport.
* You may want to apply HQX for XBR filtering for display enhancement before 
  scaling to fit the actual render viewport.  This can be used with either 
  default (640x350) with an aspect correction or to higher virtual canvas sizes.
* Ideally, you would want to use a hardware accelerated shader in your rendering 
  engine or WebGL based on your implementation.  Note: too high a setting can 
  affect render performance, but realistically this type of technology is not 
  high performance gaming.  So should be relatively okay.

## Future

Near futre, may want to formalize soem additional image and audio formats as an
unofficial 3.50 or 4.x enhancement.  Such as PNG, APNG and GIF as well as MP3 
and Ogg (Vorbis and Opus) in order to support much better compression options.
Especially since MP3 and GUF are no longer patent encumbered.

Widescreen and arbitrary-resolution support are deliberately out of scope
for now; future revisions of these notes can clarify wide support, scaling
policy, and high-DPI handling.