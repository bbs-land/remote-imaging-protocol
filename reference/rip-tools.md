# Reference repositories & artifacts (`~/src/rip-tools/`)

External reference material used for reverse-engineering RIPscrip behavior, formats, and rendering details lives **outside this repository**, under **`~/src/rip-tools/`** - always use this directory so paths are consistent across sessions and machines. `~/` means the user's home (profile) directory; on Windows that is not literally `~/` (use the profile directory, e.g. `%USERPROFILE%`) unless you are in an environment like MSYS2 bash (aka Git Bash) that resolves it. This page catalogues what is there and where each piece came from.

## Reference repositories

Peer repositories cloned for reference:

| Directory | Upstream | Language | Relevance |
| --- | --- | --- | --- |
| `~/src/rip-tools/sbbs/` | https://gitlab.synchro.net/main/sbbs.git | C | Synchronet + **SyncTERM** - `src/syncterm/ripper.c` is the most complete open RIP implementation (claims RIP 3.0 compatibility). |
| `~/src/rip-tools/RIPtermJS/` | https://github.com/cgorringe/RIPtermJS | JavaScript | RIPscrip 1.54 renderer for HTML canvas. |
| `~/src/rip-tools/icy_tools/` | https://github.com/mkrueger/icy_tools | Rust | BBS tool collection; `icy_term` includes RIPscrip emulation - useful prior art for the planned Rust library. |
| `~/src/rip-tools/pablodraw/` | https://github.com/cwensley/pablodraw | C# | ANSI/RIPscrip art editor/viewer (maintained fork of blocktronics/pablodraw). |
| `~/src/rip-tools/fTelnet/` | https://github.com/rickparrish/fTelnet | TypeScript | HTML5 WebSocket BBS client with experimental RIP support. |
| `~/src/rip-tools/qodem/` | https://codeberg.org/AutumnMeowMeow/qodem | C | Qmodem clone; detects and discards the RIP auto-sense (`CSI !`) - reference for the detection handshake only. |

Key entry points into the RIP code:

- `sbbs/src/syncterm/ripper.c` - the entire SyncTERM RIP implementation (~19,000 lines).
- `icy_tools/crates/icy_parser_core/src/rip/` - dedicated Rust RIP parser, with test scripts in `tests/rip/` and sample data in `benches/rip_data/`.
- `pablodraw/Source/Pablo/Formats/Rip/` - C# RIP format implementation (commands, rendering, editing).

Clones are shallow (`git clone --depth 1`); run `git fetch --unshallow` in a repo if its history is needed. When adding a new reference repo, clone it into `~/src/rip-tools/` and add a row to this table.

## Original binaries & documents (`~/src/rip-tools/artifacts/`)

Original-era binaries and documents for reverse-engineering reference, with provenance. Additional preservation clone: `~/src/rip-tools/RIPterm154/` (https://github.com/cgorringe/RIPterm154) holds the original `RIPTM154.ZIP` **RIPterm 1.54** plus an unpacked DOS install and a ready-to-run DOSBox setup.

| Path | What | Provenance |
| --- | --- | --- |
| `artifacts/ripterm-1.52/RIPTERM.ZIP` | RIPterm v1.52 (DOS) - `RIPTERM.EXE`, `RIPTERM.FNT`, `.ICN` icons, docs | https://archive.org/details/RIPTERM_ZIP |
| `artifacts/ripterm-2.30/RIPT2300.zip` | RIPterm v2.30 shareware distribution (complete, 1,960,399 bytes): `INSTALL.BAT`, PKZIP self-extracting `SHAREWAR.EXE` (DOS installer), `FILE_ID.DIZ`, `LICENSE.DOC`, `README.DOC` | [VOGONS "Looking for RIPTerm" thread attachment](https://www.vogons.org/viewtopic.php?t=67912) (recovered 2026-08-08); mirrored at [files.bbs.land](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTerm%202.30/RIPT2300.zip) |
| `artifacts/ripterm-2.30/rtrm2300.exe` | RIPterm v2.30 shareware installer - **truncated/damaged** (1,048,309 of ~2,000,000 bytes; install aborts partway; kept for provenance only) | Wayback: `web.archive.org/web/20000819065943/http://www.telegrafix.com/products/ripterm/rtrm2300.exe` |
| `artifacts/riptel-3.10/rtel3100.exe` | RIPtel Visual Telnet v3.10 installer (Win16 NE) - the RIPscrip-3 client | Wayback: `web.archive.org/web/20010411110821/http://www.telegrafix.com/products/riptel/rtel3100.exe` |
| `artifacts/ripaint-1.52/RIPAINT.ZIP` | RIPaint v1.52 (DOS) - includes sample `.RIP` scripts | https://archive.org/details/RIPAINT_ZIP |
| `artifacts/docs/RIP2C-Library-Manual.pdf` | TeleGrafix RIP 2 C Library manual (SDK-era 2.x documentation) | https://archive.org/details/rip-2-c-library-manual |
| `artifacts/tools/ICUPD263.ZIP` | IconUpDater v2.63 (RIP icon updater door) | https://archive.org/details/ICUPD263.ZIP |
| `artifacts/tools/sysmon.zip` | TeleGrafix SysMon RIP tool (1997) | Wayback: `telegrafix.com/asr/tools/sysmon.zip` (1997-07-03 capture) |
| `artifacts/proboard/pb_220d.zip` | ProBoard BBS v2.20d (TeleGrafix-era release) | Wayback: `telegrafix.com/products/proboard/pb_220d.zip` (2000-10-10 capture) |
| `artifacts/searchlight/SLBBS510.EXE` | Searchlight BBS v5.10 (LHa SFX; TeleGrafix-era, RIP-supporting BBS) | Wayback: `telegrafix.com/products/searchlight/SLBBS510.EXE` (2001-09-05 capture) |
| `artifacts/RIPtel/` | **Extracted RIPtel 3.1 install** (driver 3.0.7, Oct 1997): `RIPTEL.EXE`, WinHelp docs (`RIPTEL.HLP`, `RIPSCRIP.HLP`), ~110 authentic RIPscrip 3.0 demo scripts in `ICONS/` (.RIP/.FN/.DEF/.MNU/.MSE/.RET/.ENT/.EXT/.COL), 10 `.CHR` stroke fonts + 8 `.RFF` outline fonts in `FONTS/`, demo BMP/JPG images | Extracted from `artifacts/riptel-3.10/rtel3100.exe` install |
| `RIPTerm2.0/extracted/` | **Installed RIPterm Professional 2.0** (released Jan 23-24, 1995; files dated 1995-01-27 to 1995-02-01): `RIPTERM.EXE` (32-bit DOS4GW protected mode), the full `RIPTERM.DOC` manual, `README.DOC`/`THANKYOU.DOC`, RIPtech 1.0 diagnostics (`RIPTECH.EXE`/`.DOC`), HMI sound drivers (`HMIDRV.386`/`HMIDET.386`), `RIPTERM.WAV`, MicroANSI `RIPTERM.FNT`, splash `RIPTERM.BMP`, keyboard macros (`.MAC`), phone directory (`.PHO`), video-mode database (`.VID`), `FONTS\` (10 `.CHR`, 5 Atech `.FF1`, per-resolution `.FNT` + `.IMG` widgets, `DEMO_ONE.EXE` FF1 viewer), `ICONS\` (91 `.BMP`, 3 `.BMH`, `SHADOW.RIP`). Fonts/icons/audio are preserved in-repo under `version/2.0/assets/`. | Installed from the original two-disk commercial release |
| `artifacts/ripterm-2.30/extracted/` | **Complete RIPterm 2.30 file set** (253 files: `RIPTERM.EXE`/`.DOC`/`.HLP` dated 1997-10-26 and `RIPTERM.FAQ`/`README.DOC`/`ARTWORK.DOC`/`RIPTECH.DOC` dated 1997-10-25 are the actual 2.30 binaries and docs; 5 `.RFF` outline fonts are dated 1997-04; the other 236 files, dated 1995-11-28, are support files carried forward from the 2.20.01 vintage): DIALCONV/RIPTECH/REGISTER executables, HMI sound drivers, splash bitmaps, keyboard macros, `RIPterm.app` config, Windows DOS-box conveniences (`.PIF`/`.ICO`), `FONTS\` and `ICONS\` directories | Unpacked from `RIPT2300.zip`'s `SHAREWAR.EXE` (PKZIP self-extractor) |
| `RIPTerm2.22/` | **Installed RIPterm v2.20.01** (366 files; the release date 1995-11-28 covers the bulk of the tree, with some `FONTS\` library components carrying earlier 1993-94 dates; the directory name misstates the release - its own `README.DOC` reads "RIPterm v2.20.01 Unregistered Shareware"): the last `.FF1`-era client, and the only recovered install still shipping the 1.x-format legacy icon art (107 `.ICN`, 4 `.MSK`, 3 `.HIC`) that 2.30 dropped. Cited by the MicroANSI, icon-inventory and versions pages. | Installed from the original 2.20.01 shareware release |
| `ripterm-2.3/` | Redundant unpack of `artifacts/ripterm-2.30/RIPT2300.zip` - all 251 files under `extracted/` are byte-identical to `artifacts/ripterm-2.30/extracted/`, and its top-level files match the zip entries exactly. No unique content; no in-repo page cites it. **Recommended for removal.** | Unpacked from `artifacts/ripterm-2.30/RIPT2300.zip` |

When adding a new artifact, keep this provenance table up to date. RIPdraw has not yet been located in any archive.
