# Color, Audio & Text Windows

[◀ Prev: Numbers, Coordinates & Math](05-coordinates-and-math.md) · [Contents](README.md) · [Next: Protocol Definition & Syntax ▶](07-protocol-definition.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

## The Color System

_Evidence: 2.00a4; HLP._

RIPscrip 1.x colors were 16 out of a 64-color master palette, with an EGA-derived bit-swapped encoding. The 2.x/3.0 engine replaces this with a **256-entry drawing palette** (color lookup table): each entry maps a color number 0–255 to an arbitrary RGB combination, set by `RIP_ONE_DRAWING_PALETTE` and `RIP_SET_DRAWING_PALETTE` with an explicit bits-per-component precision. Up to 36 complete palettes live in the [color palette table](03-data-tables.md), switchable with `RIP_SwitchPalette`.

The 3.0.7 driver's error strings pin the concrete limits of the shipping engine:

| Limit | Error-string evidence |
| --- | --- |
| Palette indices 0–255 | "Color palette base is out of range (>255)" |
| Max **256 colors** per palette command | "Too many colors in color palette command (>256)" |
| Legacy system-palette color values 0–63 | "Invalid system palette color value (>63)" |
| Direct RGB: **8 bits per channel only** | "Invalid number of bits for RGB value (must be 8)", "RGB color mode only supports 8-bit color currently" |
| Palette animation supported | "Unable to animate palette" |
| Fill pattern values ≤255 | "Pattern value exceeds 255" |

The 0–63 system-palette range is the 1.54 desktop palette living on: the old `RIP_SET_PALETTE`/`RIP_ONE_PALETTE` commands still address 64 EGA master colors with the bit-swapped 2-bit RGB encoding, and remain supported for backward compatibility — the corpus's 1.54-style artwork still uses them (`a` in LANDSCPE.RIP). The full legacy encoding and the 2-bit→N-bit translation math are preserved in the [2.x edition's chapter](../../2.x/ripscrip/06-color-audio-text.md).

### Palette Mapping vs. Direct RGB

_Evidence: 2.00a4; HLP; corpus._

Two color modes exist, selected by **RIP_SET_COLOR_MODE** (wire opcode `M`) or `$COLORMODE$`:

- **Palette mapping mode** (the default, restored by any reset): color parameters are indices 0–255 into the current drawing palette.
- **Direct RGB mode**: color parameters are raw RGB values, blue in the lowest N bits, then green, then red. The 2.00a4 design allowed arbitrary per-component precision; the 3.0.7 driver accepts **only 8 bits per channel** (24-bit values), per the error strings above.

Commands whose color parameters obey the current mode are marked `:CM` in the command pages. The RIPtel demo prologue sets `M08` — palette-mode operation declared with 8-bit components — in 90 of 116 scripts. On palette hardware a direct-RGB color is matched to the closest available palette color; on 24-bit hardware it is used exactly. Palette-less (24-bit) environments treat the drawing palette purely as a lookup table for index-mode colors.

## Audio

_Evidence: 2.00a4; HLP._

### Background WAV Playback

> **Editor's note — provenance:** WAV audio is not a 3.0 innovation. RIP_PLAY_AUDIO entered the 2.0 specification at ALPHA 3 ("Added the RIP_PLAY_AUDIO command to playback a WAVE file" — 2.00a4 changelog) and **shipped** in the DOS RIPterm 2.20.00 (Nov 19, 1995): its manual documents `.WAV` digitized-sound support with an Audio Setup screen (Sound Blaster, Pro Audio Spectrum, Gravis, etc.), and the install carries HMI sound drivers dated to the 2.20.01 release. TeleGrafix's December 1995 "RIP-2 Internet Multimedia" press release already advertised "digital WAV sound". Like several other "3.x" features, it belongs to the 2.x product era that 3.0 inherits (see the [Introduction's revision history](01-introduction.md#revision-history)).

Digitized audio is Microsoft **WAVE** format, played in the background while graphics and command processing continue. The 2.00a4 command `RIP_PLAY_AUDIO` (level 1, `w`) carries into the 3.0 inventory (`RIP_PlayAudio` in RIPSCRIP.HLP), with the same macro form `$)FILE.WAV$` in the host command language. Playback is effectively **single-channel**:

- If a sound is already playing when a new RIP_PLAY_AUDIO arrives, the old file is **terminated** and replaced by the new one.
- Passing `$OFF$` as the filename stops the currently playing sound.

No evidence in any recovered source — spec, help strings, or corpus — shows mixing of simultaneous sounds or any way for the host to monitor playback status; the model is fire-and-forget, last-command-wins.

The terminal-side master switch is the **`AUDIO=TRUE`** toggle in the `[CONFIG]` section of `RIPscrip.ini` (created by the RIPtel installer); the driver also exposes a `RIP_AudioSupport` client-API entry point. Audio files reach the user's disk via [RIP_ENTER_BLOCK_MODE](../../2.x/ripscrip/13-level-3-9-commands.md#rip_enter_block_mode) file transfer and are looked up alongside the icon files.

### Tones and Sweeps

Beyond WAV playback, the 3.0.7 driver contains a tone-generation command family: its error strings validate tone commands with **frequency, duration, and increment sweep** parameters ("start frequency greater than stop", zero increment/duration errors). These generators sit behind the named-sound text variables — `$BEEP$`, `$MUSIC$`, `$ALARM$`, `$BLIP$`, `$PHASER$`, `$REVPHASER$` — documented in RIPTEL.HLP's variable tables. `$RESET(SOUND)$` resets the sound subsystem. No wire-level opcode for direct tone commands has been recovered from the corpus _(the demos ship no audio at all)_.

## Text Windows and Terminal Emulation

_Evidence: 2.00a4; HLP; corpus._

A text window is a screen region where raw (non-RIPscrip) text is routed, with ANSI and VT-102 emulation: colors, cursor movement, scrolling margins, wrap/chop. Up to 36 text windows are defined in the [text window table](03-data-tables.md); the default is full-screen 80×25. A deactivated current window silently discards incoming text — the demos lean on `$DTW$` (deactivate text window) constantly to keep stray host text off their graphics.

### The Extended Text Window (`0b`)

_Evidence: 2.00a4; HLP; corpus._

3.0 keeps the classic `RIP_TEXT_WINDOW` (`w`, text-cell coordinates and MicroANSI font numbers) and adds **RIP_EXTENDED_TEXT_WINDOW** at level-0 **`b`** (`RIP_ExtendedTextWindow` in the 3.0.7 inventory; the opcode is unambiguous now that SET_BASE_MATH lives at `J` — see [Numbers, Coordinates & Math](05-coordinates-and-math.md)). The extended form defines text windows in world coordinates for resolution independence, with font-ID selection, and is queryable via `$ISEXTWIN$`. RIPTEL.HLP's strings confirm the richer text-window model: row/column positions, wrap/chop designator, domain designator, text metric mode (`RIP_TextMetric` is in the inventory), write mode, and article/column bindings into the flowed-text system ("Invalid text article number", "Invalid text column number").

### MicroANSI Fonts (`.maf`)

_Evidence: HLP (file headers)._

Terminal text is rendered from **MicroANSI** bitmap fonts shipped as `RIPscrip.maf` — header `RIPterm v2.0 MicroANSI Font File`. The file carries per-resolution tables for **three display resolutions** — 640×480, 800×600, and 1024×768 — each with about five font-size subtables, so text windows keep proportion as the device resolution changes. (Text-mode geometries offered by RIPtel: 80×43, 91×43, 80×25, 91×25, 40×25.) Graphical text uses the separate font systems: BGI stroked `.CHR` vector fonts and the `.RFF` outline fonts of RIP_EXTENDED_FONT_STYLE, covered in the command pages.

### Overlapping Windows and Viewports

_Evidence: 2.00a4._

Multiple text windows and viewports may overlap freely, and no compositing occurs — everything is just pixels on the screen. Draw a circle across a text window and then scroll the window, and part of the circle scrolls with it; text written where two windows overlap is simply graphics on top of the other window. RIPscrip preserves no record of _what_ drew a pixel; the final screen state is the only truth. Mouse regions are the one place ordering matters: overlapping regions are scanned most-recent-first on a click.

---

[◀ Prev: Numbers, Coordinates & Math](05-coordinates-and-math.md) · [Contents](README.md) · [Next: Protocol Definition & Syntax ▶](07-protocol-definition.md)
