# RIPscrip 3.0 - Technical Specifications

Original byte-level documentation of the binary formats shipped with **RIPtel Visual Telnet 3.1** (TeleGrafix, October 1997; RIPscrip driver 3.0.7) - the only shipping RIPscrip 3.0 client. These are original techspecs, not spec conversions: every reverse-engineered claim cites an artifact path with observed bytes, a recovered document, or a repo file, and details that could not be verified are marked as such.

**These pages are deltas only.** Formats are documented in the earliest version where they appear (see [CONTRIBUTING.md](../../../CONTRIBUTING.md#technical-specifications-techspecs)), and the 3.x engine is the renamed 2.x engine - its font and resource containers even carry "RIPterm v2.0" magic strings - so the full format documentation lives in the [v2.0 techspecs](../../2.0/techspecs/README.md) (and the [v1.54 techspecs](../../1.54/techspecs/README.md) for the 1.x-era formats RIPtel still reads). Chapter and section numbers mirror v2.0 so the deltas line up. Language semantics live in the companion [language reference](../ripscrip/README.md); shared terminology is defined in the [glossary](../../glossary.md).

## Contents

- **1. Parsing & wire protocol** - _no 3.x-specific pages; the v2.0 chapter applies (a 3.x corpus-conventions delta would slot in at `1.0` if evidence warrants one)_
- **2. Rendering semantics** _(delta from v2.0)_
  - **[2.0 Fill Defects (delta)](2.0-fill-defects-delta.md)** - legacy flood-fill evidence and the withdrawal of the unsupported manual pie/chord flood-leak attribution
  - **[2.1 Arc Consumer Contract](2.1-arc-consumer-contract.md)** - original integer endpoint rays, command-specific empty/full decisions and skewed contour closure, verified through native consumers
  - **[2.2 Bézier Consumer Contract](2.2-bezier-consumer-contract.md)** - original integer interpolation, missing sample, device conversion order and independent 4094-point expansion budget
  - **[2.3 Drawing Composition (delta)](2.3-drawing-composition.md)** - point/stroke/fill/border roles, reconstructed skewed-contour rules and historical backend boundaries
- **3. File formats** _(deltas from v2.0)_
  - **[3.0 Bitmaps, Masks and Legacy Conversion (delta)](3.0-bitmap-mask-delta.md)** - explicit BMM stamping, accepted depth profile, RGB put-mode algebra, missing/dimension behavior, separate color-key selection and shipped 16/32-bit limitations, with a four-pixel oracle; fixed EGA legacy-conversion RGBQUAD table and packed output layout
  - **[3.2 FastFont Additions (.RFF / atf.cfg)](3.2-fastfont-additions.md)** - the three added families (BRUSH, EUREKA, OAKLAND), style-table observations, the regenerated atf.cfg
  - **[3.3 MicroANSI Delta (RIPscrip.maf)](3.3-microansi-maf-delta.md)** - new resolution set, cleaned directory, revised artwork vs the RIPterm 2.30 containers
- **4. Storage & asset delivery** _(delta from v2.0)_
  - **[4.0 Connection Directory Model (delta)](4.0-connection-directory-model-delta.md)** - the connection-directory-first model as carried into RIPtel's bookmark settings; verification against the help strings found real differences (the per-bookmark "host directory" rename, a third lookup tier via the icon search path, the install-tree confinement rule), so this is a delta page rather than a bare pointer

Client packaging (RIPtel's own resource and database containers) is deliberately **not** part of these techspecs - those are packaging details of the original client, not specification surface, and their decodes live in [`riptel-resource-containers.md`](../../3.0/research/riptel-resource-containers.md) as a research record.

## Primary sources

- The extracted RIPtel 3.1 install (`~/src/rip-tools/artifacts/RIPtel/`) and the byte-exact copies preserved in-repo under [`version/3.0/assets/`](../../3.0/assets/fonts/README.md)
- The prior triage notes in [`version/3.0/research/`](../../3.0/research/riptel-binary-formats.md), incl. the [help-file extraction](../../3.0/research/riptel-help-extraction.md) behind the storage delta
- The RIPterm 2.30 distribution (`~/src/rip-tools/artifacts/ripterm-2.30/extracted/`), for 2.x-side comparison
