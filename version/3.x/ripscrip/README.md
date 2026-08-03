# RIPscrip 3.0 Technical White Paper

**RIPscrip 3.0** — Technical White Paper, December 6th, 1996. Written by Jeff
Reeder. Copyright © 1992–1997 TeleGrafix Communications, Inc.

Unlike the [1.5x](../../1.5x/ripscrip/README.md) and
[2.x](../../2.x/ripscrip/README.md) specifications, no full RIPscrip 3.0
language reference was ever published — the white paper itself defers to a
"RIPscrip 3.0 Language Reference" that never shipped. This edition therefore
has two parts: a faithful conversion of the white paper, and a
clearly-marked **editorial reconstruction** of the 3.x-era protocol from
implementation evidence.

## Contents

### White Paper (faithful conversion)

1. [Introduction](01-introduction.md) — scope, the case for an open standard, and available platforms
2. [Applications of RIPscrip](02-applications.md) — online services, media presentation, publications, Internet applications, RIPweb, and browser plug-ins
3. [Language Structure](03-language-structure.md) — what RIPscrip is and is not, its text nature, MegaNum/UltraNum encoding, protocol structure, command levels and parameters
4. [Design Goals & Graphical Primitives](04-design-goals-and-primitives.md) — resolution and color-palette independence, font systems, vector primitives, fills, dash styles, and raster operators
5. [User Interface & Display](05-interface-and-display.md) — mouse fields, buttons, query expressions, viewports, text windows, and supported image formats (BMP, JPEG, GIF, PNG)
6. [Data Tables & Backup System](06-data-tables-and-backup.md) — the eight data-table types, context swapping, protection, and the data backup system
7. [Host Command Language](07-host-command-language.md) — control characters, pop-up picklists, local file playback, templates, and text variables
8. [Future Goals & Conclusion](08-future-goals-and-conclusion.md) — internationalization, video, streaming audio, GUI environment, forms, documents, and HTML

### Reconstructed Reference (editorial)

9. [Implementations & Versioning](09-implementations-and-versioning.md) — evidence sources and methodology, what "RIP 3.0 compatible" means in practice, version identification, and an implementation survey
10. [Reconstructed Command Set](10-reconstructed-command-set.md) — the 3.x-era command inventory as implemented by SyncTERM, parser byte-level rules, and known-but-unimplemented commands
11. [Reconstructed Text Variables & HCL](11-reconstructed-text-variables-and-hcl.md) — the built-in `$…$` text variables, user-defined variables, and host-command-language extensions beyond 1.54

The reconstruction pages are **not TeleGrafix documentation**: they are
derived from implementation evidence (primarily SyncTERM's `ripper.c`) with
per-claim citations, and are updated as further sources (the RIP 2 C Library
manual, the RIPtel 3.10 binary) are analyzed.
