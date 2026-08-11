# Baseline Terminal Support (non-RIP)

[Contents](README.md) · [Next: ANSI-BBS / VT-x Emulation in RIPterm and RIPtel ▶](ansi-vt-support.md)

The **non-RIP baseline** that RIPscrip layers on top of: the ANSI/VT-x text terminal emulation that the _text_ side of a RIP-capable terminal supports. RIPscrip was explicitly designed to be "mixed with printable ASCII text and [de facto standard] ANSI/VT-100 directives" ([RIPscrip 1.54 spec, "WHAT IS RIPscrip?"](../../1.54/text/RIPScrip-1.54.txt)), so every RIPscrip terminal is _also_ an ANSI-BBS terminal.

The primary evidence here is **TeleGrafix's own products and documentation**: RIPterm 1.54, RIPterm 2.x for DOS (2.30, the final release, stands as the definitive 2.x reference) and RIPtel Visual Telnet 3.1 for Windows - their manuals, help files and the shipped binaries themselves. Modern open-source implementations (SyncTERM, icy_term) are treated strictly as **secondary, modern reference points**, never as evidence of what the historical products did.

## Contents

1. [ANSI-BBS / VT-x Emulation in RIPterm and RIPtel](ansi-vt-support.md) - the emulation modes TeleGrafix's clients actually shipped, the documented ESC/CSI sequence set, the `ESC[!` auto-sense query and `RIPSCRIPxxyyvs` response, text-window vs graphics-window routing, character-set notes (CP437), and per-version differences (RIPterm 1.54, RIPterm 2.x as of 2.30 final, RIPtel 3.1).
2. [Modern Terminal Baseline: SyncTERM and icy_term](modern-terminal-reference.md) - what a _modern_ ANSI-BBS terminal implements (ANSI music, sixel, xterm extensions, extended color, …), with an explicit per-feature statement of whether any evidence for the feature exists in the RIPterm/RIPtel materials (for most of these features: none).

## Sources

- RIPterm 1.54 install: `~/src/rip-tools/RIPterm154/DOS/RIPTERM/` (`RIPTERM.DOC`, `WHATSNEW.DOC`, `RIPTERM.EXE`)
- RIPterm Professional 2.0 install: `~/src/rip-tools/RIPTerm2.0/extracted/` (`RIPTERM.DOC`, `README.DOC`, `ANSI.MAC`, `VT100.MAC`, `RIPTERM.EXE`)
- RIPterm 2.30 shareware install (with the 2.20.00/2.20.01 change logs): `~/src/rip-tools/artifacts/ripterm-2.30/extracted/` (`RIPTERM.DOC`, `README.DOC`, `RIPTERM.FAQ`, `RIPTERM.EXE`)
- RIPtel Visual Telnet 3.1 install: `~/src/rip-tools/artifacts/RIPtel/` (`readme.txt`, `RIPTEL.EXE`, `RIPTEL.HLP`, `MESSAGES.HLP`), plus the in-repo help-string research in [version/3.0/research/](../../3.0/research/riptel-help-extraction.md)
- Specification texts: [RIPscrip 1.54](../../1.54/text/RIPScrip-1.54.txt), [RIPscrip 2.0 alpha 4](../../2.0/text/RIPScrip-2.0-alpha-4.txt), [RIPscrip 3.x technical whitepaper](../../3.0/text/RIPScrip-3.x-technical-whitepaper.txt)
- Modern references: `~/src/rip-tools/sbbs/` (SyncTERM + cterm, incl. the `sbbs:src/conio/cterm.adoc` sequence manual), `~/src/rip-tools/icy_tools/` (icy_term + icy_parser_core), `~/src/rip-tools/qodem/` (auto-sense handling only)

Every capability claim about RIPterm/RIPtel on these pages is cited to a TeleGrafix document (with section), to direct binary evidence (`strings` on a shipped executable, cited by artifact path), or to a specification section. Where a feature left _no_ trace in any TeleGrafix material examined, that absence is stated explicitly - it is a finding, not an omission.

---

[Contents](README.md) · [Next: ANSI-BBS / VT-x Emulation in RIPterm and RIPtel ▶](ansi-vt-support.md)
