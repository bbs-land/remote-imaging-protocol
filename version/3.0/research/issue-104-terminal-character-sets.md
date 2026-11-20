# Issue #104: Terminal character sets

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md) · [ANSI baseline](../../baseline/techspecs/ansi-vt-support.md)

## Scope and checkpoint

Started 2026-11-20 on `issue-104-terminal-character-sets` from fork stable ff2043e. [Issue #104](https://github.com/bbs-land/remote-imaging-protocol/issues/104) requests ANSI versus VT-102 designation/invocation, byte-to-glyph mapping and reset/save/definition state. #105 outer reset and #106 selection follow separately. Separate FastFont research branch `research-fastfont-fields` is preserved and excluded from this issue branch. Preserve RIPscrip 1.54 and prior #100/#101 answers unless exact new evidence requires a documented correction.

## Remaining work

Read original protocol/emulation documentation, then bounded read-only RIPtel 3.1 instruction/data consumers. Establish designation/invocation gating and state ownership, exact CP437 mappings and explicit choices for missing glyph identities, reset versus save/restore/definition boundaries, and literal vectors. Do not execute original code or investigate obsolete-client bugs. Update the reference, validate, publish a separate PR, consolidate its actual head into fork stable, then start #105. No task temporary files created.

---

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md)
