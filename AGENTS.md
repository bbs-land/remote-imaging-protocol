# Agent notes

This repository archives and extends the RIPscrip (Remote Imaging Protocol) specifications: the original TeleGrafix `.txt` documents preserved verbatim under `version/<v>/text/`, Markdown reference editions under `version/<v>/ripscrip/`, original technical documentation under `version/<v>/techspecs/`, and (planned) a VitePress documentation site and in-repo Rust libraries.

**See [CONTRIBUTING.md](CONTRIBUTING.md) for all working conventions** - Markdown style, encoding and line-ending rules, techspecs placement, and the list of reference repositories.

Key points:

- Peer repositories used for reference are always cloned under `~/src/rip-tools/` (see the table in CONTRIBUTING.md).
- Only the original `.txt` documents under `version/<v>/text/` are verbatim history - never "fix" them. The Markdown editions are correct reference material, not 1:1 translations; corrections and reconstructions are welcome when clearly marked (editor's notes, evidence tags) and cited.
- Markdown style: hyphen (`-`) bullets, no hard word-wraps inside paragraphs or bullets (editors use soft wrapping), format with Prettier (`.prettierrc` sets `proseWrap: never`).
- Renderer/implementation details (canvas sizes, 4:3 aspect policy) belong in [version/IMPLEMENTATION.md](version/IMPLEMENTATION.md), never in the `version/` language docs.
