# Remote Imaging Protocol (RIPscrip)

Documentation and extended information around RIPScrip/RIPTerm/RIPtel specifications.

## RIPscrip

RIPscrip ("Remote Imaging Protocol Script language") is a text-based script
language for displaying online graphics, created by TeleGrafix Communications,
Inc. in 1992. It was designed to bring a Graphical User Interface to Bulletin
Board Systems (BBSes) over the limited bandwidth of dial-up modems, at a time
when most online services were plain ASCII/ANSI text.

Rather than transmitting raster images, RIPscrip is an **object-oriented
(vector) drawing language**: scenes are composed of hundreds or thousands of
compact drawing operations — lines, circles, polygons, fills, fonts, buttons,
and mouse regions — streamed as printable text. Key design points:

- **7-bit ASCII** — commands use only printable ASCII characters, so the
  protocol works over X.25 networks and other carriers that cannot pass 8-bit
  binary data, and can be layered onto any existing text-based host without
  modification.
- **Compact command syntax** — a command line starts with `!` and each command
  is introduced by the universal delimiter `|`, e.g. `!|c0A|L00010A0E`.
  Commands are organized into levels (Level-0 graphics primitives, Level-1
  user-interface objects such as icons, buttons and mouse fields, and higher
  levels for ports, tables, and system functions in later versions).
- **"MegaNum" base-36 numbers** — numeric parameters are encoded in
  hexa-tri-decimal (digits `0-9` and `A-Z`), compacting values to roughly 3/5
  of their decimal width.
- **Mixed graphics and text** — RIPscrip statements coexist with ordinary
  ASCII text and ANSI/VT-100 directives on the same connection; the terminal
  routes each to a graphics window (viewport) or a TTY text window.
- **Interactivity** — mouse fields, styled buttons, text variables, pop-up
  lists, and host command templates let the host build clickable, form-driven
  interfaces; the terminal sends back host commands when the user interacts.
- **Auto-sensing** — a host emits the ANSI query `ESC[!` and a RIPscrip-capable
  terminal (such as TeleGrafix's RIPterm) responds with its version, while
  ordinary terminals silently ignore it.

The language was freely licensed for implementation (though not public
domain — TeleGrafix retained the copyright), and was adopted by BBS packages
and terminal programs beyond TeleGrafix's own RIPterm, RIPaint, and RIPdraw
tools.

### Version history

| Version | Date | Notes |
|---|---|---|
| **[1.5x](version/1.5x/ripscrip/README.md)** | July 1993 | The widely deployed classic standard: EGA 640×350×16 graphics, drawing primitives, fills, fonts, icons, mouse fields, buttons, and text variables. [RIPScrip-1.54.txt](version/1.5x/text/RIPScrip-1.54.txt) |
| **[2.x](version/2.x/ripscrip/README.md)** | December 1994 | A major proposed redesign (never finalized): drawing ports, data tables and backup areas, world coordinates, 256-color and direct-RGB modes, audio playback, DIB bitmaps, and an expanded multi-level command set. [RIPScrip-2.0-alpha-4.txt](version/2.x/text/RIPScrip-2.0-alpha-4.txt) |
| **[3.x](version/3.x/ripscrip/README.md)** | December 1996 | A technical white paper (by Jeff Reeder) summarizing the next-generation architecture and goals — resolution independence, Internet/WWW applications, and multimedia — rather than a full command reference. [White paper](version/3.x/whitepaper/README.md) |

The Markdown editions are split into linked sections suitable for browsing and for generating a documentation website.


## Repository layout

```
version/
  1.5x/
    assets/     Fonts and icons distributed with RIPterm 1.54
    ripscrip/   Markdown edition of the RIPscrip 1.54 specification
    text/       Original specification text
  2.x/
    assets/     2.x-era distribution assets (recovery in progress)
    ripscrip/   Markdown edition of the RIPscrip 2.00 alpha 4 specification
    text/       Original specification text
  3.x/
    assets/     Fonts, icons, and demo content distributed with RIPtel 3.1
    research/   Research notes from reverse-engineering 3.0-era artifacts
    ripscrip/   Reconstructed 3.0 protocol reference (mirrors the 2.x layout)
    text/       RIPscrip 3.0 technical white paper (original text)
    whitepaper/ Markdown edition of the white paper (faithful conversion)
```

## Trademarks & copyright

RIPscrip, RIPterm, RIPaint, and RIPdraw are trademarks of TeleGrafix
Communications, Inc. The RIPscrip specification documents reproduced here are
Copyright © 1992–1997 TeleGrafix Communications, Inc. They are preserved for
historical and interoperability purposes.
