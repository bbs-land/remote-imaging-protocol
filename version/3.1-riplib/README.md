# RIPscrip 3.1 (RIPlib §A2G.1-7) - unofficial extensions

**Third-party, post-TeleGrafix extensions to RIPscrip 3.x, defined and implemented by [RIPlib](https://github.com/BradHawthorne/riplib).** RIPlib is a parallel, actively developed effort to build a portable, canonical RIPscrip rendering core in C99 (MIT, © 2026 SimVU / Brad Hawthorne), extracted from the A2GSPU RP2350 firmware. Its authors advertise a protocol revision **v3.1** carrying seven extension sections numbered **§A2G.1** through **§A2G.7**, identified on the wire as `RIPSCRIP031001`.

_Additions and differences only._ This is deliberately **not** a spec clone in the manner of [2.0](../2.0/ripscrip/README.md) and [3.0](../3.0/ripscrip/README.md): everything RIPlib inherits unchanged from the TeleGrafix record is documented there, and these pages cover only what §A2G.1-7 adds to or changes about it. The companion revision is [3.2-riplib](../3.2-riplib/README.md) (§A2G.8-13).

## Status and provenance

|  |  |
| --- | --- |
| **Origin** | RIPlib, not TeleGrafix - authored 2026, roughly three decades after the company's last release |
| **Standing** | **Unofficial.** No TeleGrafix document, client, or artifact contains any of this. It is one implementation's extension proposal, shipped in that implementation |
| **Deployment** | RIPlib and the A2GSPU firmware it was extracted from. No other client is known to accept `RIPSCRIP031001` traffic |
| **Wire identification** | `RIPSCRIP031001` - via `$RIPVER$` and the `ESC[!` auto-sense response |
| **Upstream source** | `~/src/rip-tools/riplib/docs/spec/06-v31-extensions.md`, cross-referenced through segments 01, 02, 04, 05, 07, 08, 10 and 11 |
| **Reconciled against** | riplib `main` @ `3e05ecb` (2026-06-30). **RIPlib is a moving target** - see [reference/rip-tools.md](../../reference/rip-tools.md#riplib-is-a-moving-target---pull-main-regularly) for the refresh procedure before editing these pages |
| **Licensing** | RIPlib's specification segments carry `(c) 2026 SimVU (Brad Hawthorne)` alongside the TeleGrafix copyright on the inherited material. The pages here are original summaries and comparisons, [CC0](../../LICENSE) like the rest of this repository - see [rights.md](../../reference/rights.md) |

### Why the directory is named `3.1-riplib`

Version directories in this repository are named for the **RIPscrip language version**, never for a client ([CONTRIBUTING.md](../../CONTRIBUTING.md#repository-conventions)) - and a bare `3.1/` would collide badly. **RIPtel 3.1** is a TeleGrafix _client_ that speaks language version **3.0** (driver 3.0.7), documented in [version/3.0/](../3.0/ripscrip/README.md) and cited by product name throughout this repository. RIPlib's "v3.1" is an unrelated _language_ revision from 2026. The `-riplib` suffix keeps the two apart and marks the tree as vendor-specific rather than part of the TeleGrafix record.

## Alignment with this repository

RIPlib's documentation and this repository's were reconstructed independently and **do not yet align**. Where §A2G.1-7 describes something as new or as a correction, that claim is stated relative to RIPlib's own v3.0 baseline - which is derived from disassembly of RIPSCRIP.DLL 3.0.7 and the published 1.54 specification, not from the fuller record reconstructed here. Reading these pages, keep three things in mind:

- **"New in v3.1" sometimes means "new to RIPlib".** Several §A2G items restore or complete behavior the TeleGrafix specifications already describe; they are extensions of the implementation more than of the language.
- **The two baselines themselves disagree.** Before any extension enters the picture, the projects differ on what RIPscrip 3.0 already was - opcode assignments, write-mode numbering, escape rules. Those conflicts are **not** documented here: they live in **[3.0-riplib](../3.0-riplib/README.md)**, so they can be discussed as questions of fact rather than as objections to someone's extension. Pages here point there where an extension depends on a contested baseline.
- **Convergence is plausible but has not happened.** Nothing here is a joint standard; both efforts may move.

Divergences are called out inline with a **⚠ Divergence** note. Aligning the two records is tracked in [TODO.md](../../TODO.md).

## Layout

```
3.1-riplib/
  ripscrip/    Language-visible additions - wire syntax and semantics
  techspecs/   Implementation-level additions - rasterization, parsing, palette mapping
```

There is no `text/` (RIPlib's segments are living documents in an active repository, not a fixed historical artifact - read them upstream) and no `assets/` (RIPlib ships the same ten Borland `.CHR` stroke fonts already preserved under [`version/2.0/assets/fonts/`](../2.0/assets/fonts/README.md)).

## Contents

**[Language additions (`ripscrip/`)](ripscrip/README.md)**

- **[2.0 Write Modes - AND and NOT](ripscrip/2.0-write-modes.md)** _(§A2G.1)_ - two new write-mode values, and the mode-numbering discrepancy behind them
- **[3.0 Text Direction & Font Attributes](ripscrip/3.0-text-direction-and-font-attributes.md)** _(§A2G.2, §A2G.3, §A2G.7)_ - a third text direction, the reversed-vertical-text correction, and rendered font attributes
- **[7.0 Port Flags](ripscrip/7.0-port-flags.md)** _(new command)_ - `|2F`, giving drawing ports opacity, compositing mode and z-order
- **[9.0 Additions Reference](ripscrip/9.0-additions-reference.md)** - consolidated command, variable and version-string delta for v3.1

**[Implementation additions (`techspecs/`)](techspecs/README.md)**

- **[1.0 Stream Parsing Delta](techspecs/1.0-stream-parsing-delta.md)** - the relaxed `!` trigger rule after ANSI CSI terminators
- **[2.0 Fill Patterns & FPU Rendering](techspecs/2.0-fill-patterns-and-rendering.md)** _(§A2G.4, §A2G.5)_ - the complete 8×8 pattern table and the floating-point curve/trig/pie work
- **[2.1 Palette Index Mapping](techspecs/2.1-palette-index-mapping.md)** _(§A2G.6)_ - relocating the EGA palette to indices 240-255 to coexist with xterm-256 text

## Further reading

- [3.0-riplib](../3.0-riplib/README.md) - where the two projects' accounts of RIPscrip 3.0 itself conflict, kept separate from the extensions
- [3.2-riplib](../3.2-riplib/README.md) - the follow-on revision (§A2G.8-13)
- [version/3.0/](../3.0/ripscrip/README.md) - the TeleGrafix 3.x record these extensions build on
- [version/next/](../next/README.md) - this repository's own forward-looking proposals, an independent effort
- [reference/rip-tools.md](../../reference/rip-tools.md) - the RIPlib clone, and how to keep it current
