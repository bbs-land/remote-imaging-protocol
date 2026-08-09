# Terminal Behavior

[◀ Prev: MegaNum Encoding](meganum-encoding.md) · [Contents](README.md)

The 1.54 specification describes commands but often not their pixel-level semantics: what exactly XOR mode XORs, where fill patterns are anchored, how line patterns advance, what happens when an icon crosses the viewport edge. This page collects those under-specified behaviors as reconstructed from the reference implementations — chiefly SyncTERM's `ripper.c`, whose code is annotated with behaviors "verified against RIPterm" (`sbbs:src/syncterm/ripper.c`), and RIPtermJS (`RIPtermJS:src/BGI.js`), a from-scratch BGI reimplementation tested against RIPterm 1.54 output. Where a behavior is one implementation's choice rather than verified RIPterm behavior, it is marked as such. Renderer policy questions for modern implementations (canvas sizes, aspect ratio) live in [version/IMPLEMENTATION.md](../../IMPLEMENTATION.md), not here.

## Screen model

- The drawing surface is 640x350, 16 colors out of the EGA 64-color master palette (spec, [RIP_VIEWPORT](../ripscrip/04-window-commands.md#rip_viewport): "Coordinates are specified based on a 640x350 pixel resolution"; RIPterm links Borland's EGAVGA driver — see [BGI Stroked Fonts](bgi-stroked-fonts.md#egavgabgi-and-the-rendering-pipeline)).
- EGA pixels are non-square (640x350 on a 4:3 display); handling of that is a renderer policy decision, documented in [IMPLEMENTATION.md](../../IMPLEMENTATION.md).
- Colors are palette **indices**; all raster-op semantics below operate on the 4-bit index, never on RGB values.

## Viewport clipping and coordinates

- All graphics output is clipped ("truncated") to the RIP_VIEWPORT rectangle; the spec states this only in the RIP_VIEWPORT entry, but it applies to every drawing primitive, including text and image stamps.
- Drawing coordinates are **viewport-relative**: pixel (x, y) lands at screen (viewport.x0 + x, viewport.y0 + y) (`sbbs:src/syncterm/ripper.c` `draw_pixel()`/`set_pixel()` add `rip.viewport.sx/sy` and clip against the viewport extent).
- An all-zero viewport (`!|v00000000`) disables graphics output entirely — commands are parsed but draw nothing (spec: "The viewport may be disabled ... by setting all parameters to zero"; `sbbs:src/syncterm/ripper.c` `no_viewport()` guards the drawing paths).
- Bitmap-font text has a per-character clipping quirk: rather than clipping a glyph pixel-by-pixel at the right/bottom viewport edge, RIPterm skips the **entire character** when its cell would cross the edge, while still advancing the text position — SyncTERM reproduces this with a clip extent of `8·size − 2` (`sbbs:src/syncterm/ripper.c` `write_char()`, "Per-character viewport clipping" comment).

## Write modes (RIP_WRITE_MODE)

[RIP_WRITE_MODE](../ripscrip/05-colors-and-attributes.md#rip_write_mode) `!|W <mode:2>`: mode 00 = copy (overwrite), mode 01 = XOR. Real files often send just one digit (`!|W0` opens `RIPtermJS:rips/set1/11FV.RIP`), relying on [meganum early termination](meganum-encoding.md#edge-cases-as-implemented) at the `|`; SyncTERM's descriptor table even declares the field 1-digit (`sbbs:src/syncterm/ripper.c` `rip_cmd_descs[]` entry `{0, 'W', "1#"}`). Under-specified detail:

- XOR combines the current drawing color's **palette index** with the **palette index of the pixel already on screen**: `new_index = old_index ^ draw_color_index` (`sbbs:src/syncterm/ripper.c` `draw_pixel()`: reads the pixel back, maps it to its palette slot via `pixel2color()`, XORs, rewrites). With a modified palette this is very different from XORing RGB.
- Write mode affects lines, shapes, and graphics text; drawing the same figure twice in XOR mode restores the original screen — the spec relies on this for animation (its Bezier notes describe erase-by-redraw, `RIPScrip-1.54.txt` RIP_BEZIER section; SyncTERM's comments around the mouse-region inverter make the same point).
- Icon/clipboard stamping ignores RIP_WRITE_MODE — it has its own mode parameter (below).

## Image put modes (RIP_PUT_IMAGE / RIP_LOAD_ICON)

Modes 00–04 = COPY, XOR, OR, AND, NOT ([RIP_PUT_IMAGE](../ripscrip/11-images-icons.md#rip_put_image)). Semantics per pixel, on 4-bit palette indices (`sbbs:src/syncterm/ripper.c` `stamp_icon_mode()`):

| Mode    | Result index                                         |
| ------- | ---------------------------------------------------- |
| 00 COPY | `icon`                                               |
| 01 XOR  | `screen ^ icon`                                      |
| 02 OR   | `screen \| icon`                                     |
| 03 AND  | `screen & icon`                                      |
| 04 NOT  | `icon ^ 0xF` (screen not read; inverted icon copied) |

- AND+OR pairs implement transparency via `.MSK` files — see [Icon File Format](icon-format.md#msk-mask-files) for the observed mask convention.
- **Off-edge icons are refused, not clipped.** If any part of the icon would extend past the viewport's right or bottom edge, RIP_LOAD_ICON draws nothing at all (spec: "If the width or height of the Icon would make it go off the right or left edge of the screen, the Icon will not be displayed"; `sbbs:src/syncterm/ripper.c` `load_stamp_icon()` rejects before drawing; `RIPtermJS:src/BGI.js` `putimage()` logs "ignored: right edge past canvas edge"). This differs from every other primitive, which clips.
- **No icon scaling exists in 1.5x.** Icons and clipboard images are stamped 1:1 device pixels; there is no zoom/stretch command in the 1.54 command set, and the only "magnification" anywhere in the system is font-size pixel replication ([RIP_FONT_STYLE](../ripscrip/06-text-output.md#rip_font_style)). Modern renderers that scale output do so on the whole 640x350 framebuffer, not per icon (SyncTERM's `scale_setpixel`; see [IMPLEMENTATION.md](../../IMPLEMENTATION.md)).

### "Image too large" side effects

A BGI image buffer is limited to 65,535 bytes (16-bit `imagesize()`), i.e. `⌈w/8⌉·4·h + 6 ≤ 65535`. RIPterm reacts to an over-limit RIP_GET_IMAGE/RIP_LOAD_ICON with an "Image too large" error popup — and the popup itself **clobbers graphics state**: afterward, draw color = 7, fill color = 8, fill style = solid, line width = 1, line style = solid, write mode = copy. SyncTERM replicates these side effects, noting they were "empirically verified by setting known state before triggering the popup and checking each variable after" (`sbbs:src/syncterm/ripper.c` `bgi_imagesize_ok()` / `rip_image_too_large()`). Scenes that trip the limit therefore corrupt subsequent drawing state on a real RIPterm.

## Line styles and patterns

[RIP_LINE_STYLE](../ripscrip/08-line-fill-styles.md#rip_line_style) `!|= <style:2> <user_pat:4> <thick:2>`:

- The four predefined styles are 16-bit patterns: solid `FFFF`, dotted `CCCC`, centered `FC78`, dashed `F8F8`; style 04 uses the 4-digit user pattern verbatim (`sbbs:src/syncterm/ripper.c` `rip_line_patterns[4]`; pattern values match the spec's style descriptions).
- **One pattern bit is consumed per pixel** along the line, cycling through the 16 bits; a set bit draws the pixel, a clear bit skips it (leaves the screen untouched — patterned lines are transparent in the gaps, not background-colored). SyncTERM advances the bit position `pp = (pp + 1) & 0x0F` and tests `pattern & (1 << pp)`, i.e. LSB first (`sbbs:src/syncterm/ripper.c` `draw_line_to()`).
- The pattern position **carries across the segments of a connected figure** (polyline/polygon/Bezier joints continue the cycle rather than restarting it) — `draw_line_to()` threads the position through by pointer.
- Thickness is 1 or 3 only (spec). A thick line is drawn as three 1-pixel lines: the center line plus offsets of ±1 perpendicular to the line's major axis, each independently clipped, matching BGI (`sbbs:src/syncterm/ripper.c` `draw_line_to()` width-3 branch and its comment).
- Line patterns apply to lines, polylines, polygons, and Beziers. Rectangle outlines also honor them; circles/ellipses/arcs in BGI ignore the line _pattern_ (they draw solid) though the thickness applies — 1.5x scenes rarely exercise this corner; treat it as BGI-inherited behavior. The spec explicitly recommends patterned Beziers for special effects and warns about XOR redraw for animation (`RIPScrip-1.54.txt` RIP_BEZIER notes).
- **RIP_RESET_WINDOWS does not reset line pattern or line width** — "(verified against RIPterm reference implementation)" (`sbbs:src/syncterm/ripper.c`, comment in the reset path). See [Reset semantics](#reset-semantics).

## Fill styles and patterns

[RIP_FILL_STYLE](../ripscrip/08-line-fill-styles.md#rip_fill_style) selects one of 12 patterns (00 background … 11 close-dot) plus a fill color; [RIP_FILL_PATTERN](../ripscrip/08-line-fill-styles.md#rip_fill_pattern) sets a user-defined 8x8 pattern (eight 2-digit rows, top row first, bit 7 = leftmost pixel) and the fill color.

- **Two-color rendering, not transparency:** for every filled pixel, a set pattern bit paints the current **fill color** and a clear bit paints **color 0 (black)** — not the "background color" state and not transparent (`sbbs:src/syncterm/ripper.c` `native_fill_pixel()`: `fill_pattern[y & 7] & (0x80 >> (x & 7)) ? fill_color : color 0`).
- **Patterns are anchored to absolute screen coordinates** (`x & 7`, `y & 7` of the device pixel), so adjacent filled shapes tile seamlessly and a shape refilled at the same position aligns with itself (`native_fill_pixel()` operates on post-viewport screen coordinates).
- Fill style 00 ("background fill"): SyncTERM's scanline polygon filler simply skips filling entirely when style 0 is active (`scanline_poly_fill()` step 1: "Skip if fill_style == 0"). Since the pattern for style 0 is all-zero bits, painting it would produce solid black; skipping produces nothing. Which of the two RIPterm 1.54 actually did for filled _polygons_ is unverified; for bars/flood fills an all-zero pattern paints solid color-0 by the rule above.
- Filled primitives (RIP_BAR, RIP_FILLED_POLYGON, pie slices, flood fill) fill with the pattern; the _outline_, where the primitive has one, is drawn separately with the current drawing color and line state.
- Filled-polygon rasterization is scanline even-odd with **inclusive** spans between sorted edge intersections, edges treated as half-open in y (`y_min ≤ y < y_max`) so shared vertices don't double-count (`sbbs:src/syncterm/ripper.c` `scanline_poly_fill()` — algorithm annotated as "matching RIPterm's algorithm").
- The spec's own pattern table contains an error RIPterm faithfully implements: the "Light Backslash" style's documented bytes (`A5 D2 69 B4 5A 2D 96 4B`) do not form a backslash pattern (the plausible intended bytes `80 40 20 10 08 04 02 01` do). SyncTERM ships the documented bytes, labeling the alternative "as expected" and the shipped one "as documented" (`sbbs:src/syncterm/ripper.c` `rip_fill_patterns[12]`). Match the documented bytes for fidelity.
- [RIP_FILL](../ripscrip/07-drawing-primitives.md#rip_fill) (flood fill) fills the connected region at (x, y) bounded by pixels of the given **border color**, using the current fill pattern/color; the border pixels themselves are untouched, and if the border does not fully enclose the seed point the fill spreads to the viewport edges (spec, RIP_FILL notes). In Borland's `floodfill()`, seeding on a pixel that is already the border color does nothing; this is inherited BGI behavior (unverified against RIPterm itself).

## Fonts and text

- Stroked-font sizes are **not** linear magnifications: RIP size 4 is 1:1, size 1 is 60% (full ratio table and citations in [BGI Stroked Fonts](bgi-stroked-fonts.md#scaling-rip-size-vs-actual-magnification)). The spec's "01 for the normal default size, 02 for x2..." sentence is accurate only for bitmap font 0, whose magnification is N× pixel replication ([Bitmap Fonts](bitmap-fonts.md#rip_font_style-font-0--the-default-8x8-graphics-font)).
- Graphics text (all fonts) is drawn with the current drawing color and honors XOR write mode; stroked glyphs are stroked as solid 1-px vectors regardless of line style ([BGI Stroked Fonts](bgi-stroked-fonts.md#egavgabgi-and-the-rendering-pipeline)).
- Vertical direction (01) rotates glyphs 90° counter-clockwise; the current position advances only along the writing direction (`sbbs:src/syncterm/ripper.c` `write_char()`; `RIPtermJS:src/BGI.js` `drawChar()` direction handling).
- Text width must be computed as sum-of-unscaled-widths, then scaled once (not per-glyph), to match RIPterm's rounding (`sbbs:src/syncterm/ripper.c` `textwidth()` "sum-then-scale" comment).

## Reset semantics

What [RIP_RESET_WINDOWS](../ripscrip/04-window-commands.md#rip_reset_windows) `!|*` does (spec + implementation-verified additions):

- Per spec: full-screen text window (80x43) and graphics viewport (640x350), clear screen, home cursor, kill all mouse regions/buttons, erase clipboard, restore the default 16-color palette.
- Verified additions and omissions (`sbbs:src/syncterm/ripper.c` reset path): the border/cursor/hotkey states reset, **but line pattern and line width survive** the reset (comment: "verified against RIPterm reference implementation"). Well-behaved scenes therefore re-send `!|=…` after `!|*` — and the shipped TeleGrafix scenes do exactly that (see [scene prologue conventions](rip-file-format.md#scene-prologue-and-epilogue-conventions)).
- Draw color, fill state, write mode, and font state also persist from scene to scene unless explicitly reset; there is no "reset all attributes" command in 1.5x.

## Mouse regions

Not strictly drawing, but commonly mis-implemented (spec, [miscellaneous notes](../ripscrip/02-protocol-structure.md#miscellaneous-notesinformation)):

- At most 128 mouse fields + buttons combined may exist; hit-testing scans **most recent first**, so overlapping regions resolve to the newest.
- Region/button inversion feedback ("visibly inverted while the mouse button is down", spec [RIP_MOUSE](../ripscrip/09-mouse-fields.md#rip_mouse) `<clk>` flag) complements each on-screen pixel's palette index: `index ^ 0x0F` (`sbbs:src/syncterm/ripper.c` `invert_rect()`; button color inversion likewise uses `0x0f ^ color` throughout `draw_button()`).

---

[◀ Prev: MegaNum Encoding](meganum-encoding.md) · [Contents](README.md)
