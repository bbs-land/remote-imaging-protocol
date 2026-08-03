# Implementations & Versioning

[◀ Prev: Future Goals & Conclusion](08-future-goals-and-conclusion.md) · [Contents](README.md) · [Next: Reconstructed Command Set ▶](10-reconstructed-command-set.md)

> **Editorial reconstruction.** TeleGrafix never published a RIPscrip 3.0 Language Reference. This page reconstructs the versioning and implementation landscape of the 3.x era from implementation evidence — primarily SyncTERM's `ripper.c` (Synchronet project, GPL). Every claim cites its source. This is NOT TeleGrafix documentation.

## Sources & Methodology

No official RIPscrip 3.0 language reference is known to exist. What we have instead:

- **The RIPscrip 3.0 Technical White Paper** — marketing/architecture prose only; it describes goals and features but contains no command reference. See [Introduction](01-introduction.md).
- **SyncTERM's implementation** — `ripper.c` in the Synchronet source tree is the only known open-source implementation that *claims* RIP 3.0 compatibility. It is the primary evidence base for the reconstructed pages in this section.
- **The RIPscrip 2.00 Alpha 4 specification** — the last documented TeleGrafix language reference, used as the documented baseline that 3.0-era behavior is measured against. See [the 2.x specification](../../2.x/ripscrip/README.md).
- **Unexplored artifacts** — the project's reference collection preserves a TeleGrafix "RIP 2 C Library Manual" PDF and Win16 installers for RIPtel 3.10 and RIPterm 2.30. These are the closest surviving things to first-party 3.x-era software, but they await extraction and analysis; see the repo's CONTRIBUTING.md for their locations. Findings from them may revise these pages.

Methodology: claims below cite `file:line` locations in the surveyed sources as plain text (e.g. `ripper.c:7619`). Line numbers refer to the source revisions surveyed in mid-2026 and will drift as upstream projects change.

## What "RIP 3.0 Compatible" Means in Practice

SyncTERM's "RIPv3" mode is, concretely:

> the RIPscrip 1.54 command set, **plus** the 2.0 Alpha additions it implements, **plus** a large family of terminal-control text variables, **answering the version query with a 3.0 version string.**

There is no separate "RIP 2.x" mode at all. The per-BBS emulation enum has exactly three states — none, 1, and 3:

```c
enum {
    RIP_VERSION_NONE,
    RIP_VERSION_1,
    RIP_VERSION_3
};
```

(`sbbs/src/syncterm/bbslist.h:121–127`; the corresponding UI strings are `"Off"`, `"RIPv1"`, `"RIPv3"` at `bbslist.c:269`.)

Two consequences worth stating plainly:

1. **The version is configuration, not negotiation.** In SyncTERM the RIP version is a per-BBS directory setting chosen by the user, not something auto-sensed from the wire. The terminal *answers* version queries (below) but does not change modes based on what the host sends.
2. **"3.0" is a self-assigned label.** SyncTERM reports `RIPSCRIP030001` because its feature set exceeds the documented 2.0 Alpha 4 spec, not because it implements a known TeleGrafix 3.0 document — none exists to implement.

## Version Identification

### Reply strings

`ripper.c:7619` defines the version reply strings:

```c
static const char *ripver[] = {"", "RIPSCRIP015410", "RIPSCRIP030001"};
```

Each reply is exactly **14 bytes**, sent with no CR/LF (`conn_send(ripver[rip.version], 14, 1000)` at `ripper.c:18447`). The format is the literal text `RIPSCRIP` followed by six version digits, `MMmmrr` + `pp` style:

| Mode | Reply | Decodes as |
|---|---|---|
| RIPv1 | `RIPSCRIP015410` | major 01, minor 54, revision 10 → v1.54.10 |
| RIPv3 | `RIPSCRIP030001` | major 03, minor 00, revision 01 → v3.00.01 |

### The `ESC[!` query family

The query dispatcher lives in `cb_esc_feed` (`ripper.c:18437–18470`):

| Sequence | Effect | Cite |
|---|---|---|
| `ESC[!` | Send the 14-byte version reply (if RIP enabled) | `ripper.c:18444–18448` |
| `ESC[0!` | Same — send the version reply | `ripper.c:18449–18452` |
| `ESC[1!` | **Disable** RIP processing — a one-way door; there is no code path that re-enables it afterward | `ripper.c:18453–18456` |
| `ESC[2!` | Enable RIP — a no-op in practice, since a disabled parser never sees it | `ripper.c:18457–18459` |
| other `ESC[…!` | Falls through to the SkyPix handler | `ripper.c:18460–18463` |

The same version string is also exposed as the `$RIPVER$` text variable — `rv_version` simply returns `strdup(ripver[rip.version])` (`ripper.c:8055–8058`).

Compare the 1.54-era auto-detection convention, where hosts sent the query and sniffed the reply to decide whether to serve RIP menus at all — documented in [the 1.5x specification](../../1.5x/ripscrip/README.md). SyncTERM honors the query side of that handshake but, as noted above, its own mode never changes in response.

## Implementation Survey

RIP version coverage across surveyed open implementations (from a prior repo survey; paths are the authoritative cite):

| Implementation | Language | RIP coverage | Notes / cite |
|---|---|---|---|
| SyncTERM (`sbbs`) | C | 1.54 + 2.0 additions; claims 3.0 | The only implementation answering `RIPSCRIP030001`. `~/src/rip-tools/sbbs/src/syncterm/ripper.c` |
| icy_tools | Rust | Clean 1.54 only | 54-variant command enum in `icy_parser_core/src/rip/command.rs` |
| RIPtermJS | JavaScript | 1.54 code | But ships the 2.00 A4 spec and 3.0 white-paper HTML in `docs/`, and real RIP 2.0 samples in `rips/v2.0/` — opcodes `\|!` (comment) and `\|1w` (play audio) observed in the wild there |
| fTelnet | TypeScript | 1.54, levels 0/1/9 | Opcode table at `source/graph/rip/RIP.ts:375–385` |
| PabloDraw | C# | 46-command 1.54 subset | Rendering-oriented subset |
| Qodem | C | Detection only | Detects and discards RIP sequences |

The practical takeaway: **1.54 is the interoperable floor**; SyncTERM is the sole living reference for anything beyond it, which is why these reconstruction pages lean on it.

## Behavioral Notes

- **RIPv3 mode is idealized, not bug-compatible.** SyncTERM's own developer documentation states the RIPv3 implementation is "idealized, NOT bug-compatible with v1.54," and that intentional compatibility bugs are confined to the v1 path — it notes three such bugs preserved in `broken_flood_fill()` (`sbbs/src/syncterm/HACKING.md:956`).
- **A 1.54 quirk retained under v1 only:** after a line-style sweep, original RIPterm restored the line style with a stale register value, clobbering custom pattern 4 with `0x8000`; SyncTERM reproduces this only when `rip.version == RIP_VERSION_1` (`ripper.c:12800`).
- **Default font selection differs by version:** `init_rip_ver` forces the 8×8 font under RIPv1, but under RIPv3 adopts whatever font the terminal currently has active (`ripper.c:18970–18989`).
- **Empirical validation:** SyncTERM's RIP rendering was validated against real DOS RIPterm output using a pixel-diff test harness — see `sbbs/src/syncterm/rip_test/rip_harness.md`.

---

[◀ Prev: Future Goals & Conclusion](08-future-goals-and-conclusion.md) · [Contents](README.md) · [Next: Reconstructed Command Set ▶](10-reconstructed-command-set.md)
