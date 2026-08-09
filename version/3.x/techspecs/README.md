# RIPscrip 3.x Technical Specifications

[Contents](README.md) · [Next: RIPtel FastFont Additions ▶](rff-additions.md)

Original byte-level documentation of the binary formats shipped with **RIPtel Visual Telnet 3.1** (TeleGrafix, October 1997; RIPscrip driver 3.0.7) — the only shipping RIPscrip 3.0 client. These pages are original techspecs, not spec conversions: every reverse-engineered claim cites an artifact path with observed bytes, a recovered document, or a repo file, and details that could not be verified are marked as such.

**These pages are deltas.** Formats are documented in the earliest version family where they appear (see [CONTRIBUTING.md](../../../CONTRIBUTING.md)), and the RIPscrip 3.0 engine is the renamed RIPterm 2.x engine — its font and resource containers even carry "RIPterm v2.0" magic strings — so the full format documentation lives in [`version/2.x/techspecs/`](../../2.x/techspecs/README.md) (and `version/1.5x/techspecs/` for the 1.x-era formats RIPtel still reads). This directory covers **only what RIPtel 3.1 adds or changes**: the three new FastFont families and the regenerated font catalog, and the revised MicroANSI container. (RIPtel's own resource/database container files are packaging details of the original client, not specification surface — they are documented in the [research notes](../research/riptel-resource-containers.md).)

Primary sources: the extracted RIPtel 3.1 install (`~/src/rip-tools/artifacts/RIPtel/`), the byte-exact copies preserved in-repo under [`version/3.x/assets/`](../assets/fonts/README.md), the prior triage notes in [`version/3.x/research/`](../research/riptel-binary-formats.md), and the RIPterm 2.30 distribution (`~/src/rip-tools/artifacts/ripterm-2.30/extracted/`) for 2.x-side comparison.

## Contents

1. [RIPtel FastFont Additions](rff-additions.md) — the three `.RFF` families RIPtel adds (BRUSH, EUREKA, OAKLAND) and the regenerated `atf.cfg` catalog, as a delta from the [2.x FastFont format](../../2.x/techspecs/fastfont-fonts.md)
2. [MicroANSI Fonts — RIPscrip.maf](maf-fonts.md) — the 3.x MicroANSI container versus [2.x `RIPTERM.FNT`/`RIPTERM.MAF`](../../2.x/techspecs/microansi-fonts.md): new resolution set, cleaned-up layout, revised artwork

---

[Contents](README.md) · [Next: RIPtel FastFont Additions ▶](rff-additions.md)
