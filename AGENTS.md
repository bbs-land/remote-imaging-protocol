# Agent notes

This repository archives and extends the RIPscrip (Remote Imaging Protocol)
specifications: faithful Markdown conversions of the original TeleGrafix
documents under `version/<v>/ripscrip/`, original technical documentation
under `version/<v>/techspecs/`, and (planned) a VitePress documentation site
and in-repo Rust libraries.

**See [CONTRIBUTING.md](CONTRIBUTING.md) for all working conventions** —
Markdown/conversion style, encoding and line-ending rules, techspecs
placement, and the list of reference repositories.

Key points:

- Peer repositories used for reference are always cloned under
  `~/src/rip-tools/` (see the table in CONTRIBUTING.md).
- Conversions are faithful: never "fix" the original spec text; typos are
  preserved verbatim. Corrections and inferred details belong in `techspecs/`
  or editor's notes, with sources cited.
- LF line endings everywhere; 2.x original text is CP437.
- Renderer/implementation details (canvas sizes, 4:3 aspect policy) belong
  in [IMPLEMENTATION.md](IMPLEMENTATION.md), never in the `version/`
  language docs.
