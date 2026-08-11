# RIPscrip 3.1 (RIPlib §A2G.1-7) - Language Additions

_Language-visible additions and changes only - what a content creator writing for a `RIPSCRIP031001` terminal can do that a 3.0 terminal cannot. Everything else is unchanged from the [3.0 reference](../../3.0/ripscrip/README.md); implementation-level detail is in the companion [technical specifications](../techspecs/README.md), shared terminology in the [glossary](../../glossary.md)._

These pages are **deltas, not a self-contained reference.** The `ripscrip/` trees for 1.54, 2.0 and 3.0 are self-contained by design (a creator reads only the version they target); this one deliberately is not, because §A2G.1-7 is an extension layer on top of the TeleGrafix record rather than a generation of the language. Read [3.0](../../3.0/ripscrip/README.md) first, then these pages for what changes. See the [tree README](../README.md) for standing, provenance and the alignment caveats.

## Source tags

Every claim on these pages names where it comes from:

| Tag | Meaning |
| --- | --- |
| `A2G.n` | Stated in RIPlib's v3.1 extension segment, `docs/spec/06-v31-extensions.md` |
| `spec (seg-N §M)` | Stated elsewhere in RIPlib's specification segments (e.g. `spec (05 §5.6)`) |
| `riplib (path:N)` | Observed in the RIPlib source tree |
| ⚠ **Divergence** | Conflicts with the record reconstructed in this repository |

Section numbers on these pages follow this repository's chapter scheme (`2.x` drawing, `3.x` text, `7.x` ports, `9.x` reference), **not** RIPlib's segment numbering; each page names the §A2G section it covers.

## Contents

- **2. Drawing**
  - **[2.0 Write Modes - AND and NOT](2.0-write-modes.md)** _(§A2G.1)_ - modes 2 and 4 added to `|W`, and the COPY/OR/XOR numbering discrepancy that surrounds them
- **3. Text**
  - **[3.0 Text Direction & Font Attributes](3.0-text-direction-and-font-attributes.md)** _(§A2G.2, §A2G.3, §A2G.7)_ - direction 2 (CCW) added to `|Y`, direction 1 redefined top-to-bottom, and the four font attribute bits given rendering behavior
- **7. Ports**
  - **[7.0 Port Flags](7.0-port-flags.md)** _(new command)_ - `|2F` sets per-port alpha, compositing mode and z-order
- **9. Reference**
  - **[9.0 Additions Reference](9.0-additions-reference.md)** - every v3.1 command, parameter value, variable and version string in one table, each marked as an addition, a redefinition, or a divergence

Rendering-only §A2G items - the complete fill-pattern set (§A2G.4), floating-point curves and pie fill (§A2G.5), and the palette index relocation (§A2G.6) - change no wire syntax and live in [techspecs](../techspecs/README.md).
