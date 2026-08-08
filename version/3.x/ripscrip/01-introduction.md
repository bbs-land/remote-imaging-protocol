# Introduction

[Contents](README.md) · [Next: Drawing Ports ▶](02-drawing-ports.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

## What RIPscrip 3.0 Is

_Evidence: WP; 2.00a4._

RIPscrip ("Remote Imaging Protocol script") is TeleGrafix Communications' text-based graphical scripting language for online services. It encodes vector graphics, bitmapped images, mouse-driven user interfaces, fonts, audio, and terminal text into a stream of 7-bit printable ASCII that can be transmitted over any connection capable of carrying text, and intermixed freely with ordinary TTY and ANSI/VT-102 output. The language is object-oriented rather than raster-oriented: a scene is a sequence of compact drawing commands, not a bitmap.

RIPscrip 3.0 is the third generation of the language — the completed form of the 2.0 design that TeleGrafix circulated as the ALPHA specification drafts of 1993–1994, shipped in RIPterm 2.0 and RIPtel 3.x, and described publicly in the December 1996 [RIPscrip 3.0 Technical White Paper](../whitepaper/README.md). It adds to the 1.54 baseline: drawing ports and offscreen bitmaps, world coordinates and resolution independence, 256-color and direct-RGB color, data tables with protection, a save/restore backup system, outline fonts, JPEG images, WAV audio, a flowed-text column system, and a greatly expanded text-variable/host-command language.

## Evidence Basis for This Edition

Unlike the [1.5x](../../1.5x/ripscrip/README.md) and [2.x](../../2.x/ripscrip/README.md) editions, this edition is **not** a conversion of an official document. TeleGrafix's own product list advertised a "RIPscrip 3.0 protocol specification," and the white paper promises a "RIPscrip 3.0 Language Reference," but no such document has ever been located. This edition therefore reconstructs the 3.0 language from five converging sources:

1. **The RIPscrip 2.00 ALPHA 4 specification** (December 1994) — the last published TeleGrafix language reference, and the documented baseline that 3.0 completes. Where 3.0 evidence shows a behavior unchanged from 2.00a4, this edition adapts the [2.x edition's](../../2.x/ripscrip/README.md) text.
2. **The RIPscrip 3.0 Technical White Paper** (December 1996) — architecture and feature prose, converted in full in the [white paper edition](../whitepaper/README.md).
3. **RIPSCRIP.HLP recovered strings** — the complete error-message and command-name string table of the RIPSCRIP.DLL 3.0.7 parser, extracted from the RIPtel 3.1 install. It fixes the 3.0 command inventory (~90 `RIP_*` names) and many hard limits. See the [help extraction report](../research/riptel-help-extraction.md).
4. **The 116-script RIPtel demo corpus** — TeleGrafix's own demo scenes (22,921 parsed commands, self-documented with 305 prose comments), the primary source for on-the-wire opcodes and syntax. See the [script census](../research/riptel-script-census.md).
5. **SyncTERM's implementation** — `ripper.c` in the Synchronet source tree, the only open-source implementation that answers the version query as RIP 3.0. Cited by line number throughout.

Every section in this edition carries an _Evidence:_ line naming which of these sources supports it (`2.00a4`, `WP`, `HLP`, `corpus`, `SyncTERM`); editorial inferences are marked _(hypothesis)_. The full legend is in the [Contents](README.md).

## The 3.0 Identity

_Evidence: HLP; corpus; SyncTERM (ripper.c:7619)._

"RIPscrip 3.0" is a renaming, not a redesign. The evidence from RIPtel 3.1 (October 1997) is unambiguous:

- RIPtel 3.1 ships **driver version 3.0.7** of RIPSCRIP.DLL, per its readme.
- The engine's internal file headers self-identify as **"RIPterm v2.0"** — the resource file, the MicroANSI font file, and the help resources all carry `RIPterm v2.0` signatures.
- The RIPTEL.HLP FAQ gives the auto-detect reply as **`RIPSCRIP03000`** (see [Protocol Definition](07-protocol-definition.md) for the handshake; SyncTERM answers with the 14-byte `RIPSCRIP030001`).

In other words, RIPscrip 3.0 **is** the completed RIPscrip 2.x engine: the 2.00 ALPHA feature set, finished, debugged, and shipped under a new major version number. This is why the 2.00a4 specification remains the structural backbone of this edition.

## Revision History

_Evidence: 2.00a4; WP; HLP._

| Revision | Date | Notes |
| --- | --- | --- |
| 1.54 | 1993 | Last published 1.x specification; the universal interoperability baseline. See the [1.5x edition](../../1.5x/ripscrip/README.md). |
| 2.00 A0–A4 | 1993–Dec 1994 | The five ALPHA drafts of the 2.0 specification, introducing ports, tables, world coordinates, UltraNums, and the rest of the modern architecture. Per-revision change logs are preserved in the [2.x Introduction](../../2.x/ripscrip/01-introduction.md). |
| 2.1 / 2.2 | 1995–1997 | The undocumented "RIPscrip-2" product era: the engine as shipped in the DOS RIPterm 2.20.00 (Nov 19, 1995), 2.20.01 (Nov 28, 1995), and 2.3 (Oct 25, 1997), with **no specification published after ALPHA 4** (no product labeled "2.1" has surfaced). The shipping engine moved past the draft — e.g. v2.20.01 added the icon-transparency flag (`<flags2>` bit 128) to RIP_BUTTON_STYLE, absent from the ALPHA 4 table. Features often credited to "3.x" were already shipping here — notably background **.WAV audio playback** (specified since ALPHA 3; RIPterm 2.2 shipped it with HMI sound drivers) and JPEG image display. See [Color, Audio & Text Windows](06-color-audio-text.md#background-wav-playback). |
| 3.0 | 1995–1996 | The 2.x design finalized and renamed. Publicly described by the Dec 1996 white paper; no language reference published. |
| 3.0.7 | Oct 1997 | Driver shipped with RIPtel 3.1 "Visual Telnet" — the concrete engine this edition documents. |

One notable change between the 2.00a4 draft and the shipping 3.0: **GIF support was dropped.** The 2.A4 draft added GIF alongside JPEG for compressed images, but the white paper explains it had to be removed "due to outrageous licensing conditions for the compression algorithm LZW," leaving JPEG as the sole compressed image format (with PNG named as the intended successor). See [the white paper on image formats](../whitepaper/05-interface-and-display.md).

## Differences from 2.00 ALPHA 4

_Evidence: corpus; HLP; WP._

Relative to the last published specification draft, the shipping 3.0 engine differs as listed below. Note that these differences accumulated **across the 2.1/2.2 product releases as well as the 3.0 renaming** — with no specifications published after ALPHA 4, the exact release where each feature landed is mostly unrecorded; where evidence exists (e.g. WAV audio shipping in the 2.1/2.2 era), the relevant section says so.

- **Adds new drawing primitives** never documented in any spec: skewed ovals (`&`, `-`, `]`, `[`, `+`), the filled oval chord (`_`), markers (`;`), poly-polygons with even-odd fill (`<`), and bounded text (`"`). All are named or demonstrated in TeleGrafix's own NEWCMDS.RIP and companion demos — see [the script census](../research/riptel-script-census.md) and the command pages.
- **Resolves the 2.00a4 opcode collision**: RIP_SET_BASE_MATH is level-0 `J` on the wire, leaving `b` to RIP_EXTENDED_TEXT_WINDOW. See [Numbers, Coordinates & Math](05-coordinates-and-math.md).
- **Adds the flowed-text column system** — `1e` column regions, linked multi-column layouts, overflow paging, and RIP_SelectArticle — the "powerful column system" advertised for newspaper-style presentation.
- **Adds a conditional/macro layer**: `<<IF …>> <<ELSE>> <<ENDIF>>` inline conditionals, `<<NAME>>` macro expansion, the `$&VAR$` dereference form, and `ID=n:` mouse-field identities.
- **Adds web integration**: `$GOTOURL(var)$` launches a URL held in a text variable — 1997-era web/BBS convergence.
- **Formalizes scene header segments** — a per-scene header mechanism carrying data-table flags and entries, coordinate size, direct-RGB bit count, general flags, and a revision code. See [Protocol Definition](07-protocol-definition.md).

## Differences from 1.54

_Evidence: 2.00a4; HLP; corpus._

Everything the 2.00 ALPHAs introduced applies: ports, tables, backup areas, world coordinates, 256/direct-RGB color, base-math variation, and the expanded command levels — the fundamentals chapters of this edition cover each. For interoperating with 1.54-era content, 3.0 provides the **`$COMPAT$`** text variable, which sets the environment to RIPscrip 1.54 settings — the classic 640×350 EGA coordinate space and defaults. TeleGrafix's own demo scenes issue `$COMPAT$` before playing 1.54-style artwork (21 uses in the corpus), making it the standard bridge between the two coordinate worlds. The driver also retains 1.54 `.ICN` icon handling, converting old icons to BMP on the fly ("Can't convert RIP 1.54 Icon %s to 2.0 BMP format!").

## Implementation Status

_Evidence: SyncTERM; editorial survey._

No modern implementation covers RIPscrip 3.0 in full:

| Implementation | Coverage |
| --- | --- |
| SyncTERM | 1.54 plus a substantial subset of 2.x/3.0 additions; the only implementation answering the version query as 3.0 (`RIPSCRIP030001`). Its RIPv3 mode is "idealized, not bug-compatible" with 1.54. |
| icy_tools, fTelnet, PabloDraw | RIPscrip 1.54 only (varying subsets). |
| Qodem | Detects and discards RIP sequences. |

The practical takeaway: **1.54 is the interoperable floor**, SyncTERM is the only living implementation of anything beyond it, and the original RIPSCRIP.DLL 3.0.7 driver (via RIPtel under emulation) remains the reference behavior for the full language. See [Protocol Definition](07-protocol-definition.md) for version identification details.

---

[Contents](README.md) · [Next: Drawing Ports ▶](02-drawing-ports.md)
