# Issue #20: FILEDEL directory scope, ownership and path handling

[Research backlog](issue-backlog.md) · [FILEDEL](../ripscrip/5.1-text-variables.md#filedel) · [Storage](../ripscrip/6.2-asset-delivery-and-storage.md)

## Scope and artifact

Started 2026-11-16 after [PR #49](https://github.com/bbs-land/remote-imaging-protocol/pull/49) delivered #19. Branch **issue-20-filedel-scope** starts at **a3dc068**. [Issue #20](https://github.com/bbs-land/remote-imaging-protocol/issues/20) asks for configured/no-host deletion scope and duplicate matches, enforcement of the host-created restriction across sessions/hosts, and the exact FILEDEL exception to inherited reject-path guidance. Preserve older text and distinguish normative caller restrictions from executable checks.

Primary unchanged RIPtel 3.1 / driver 3.0.7 DLL, 592896 bytes, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, base **0x10000000**, catalogued as `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [external mirror](../../../reference/rip-tools.md). [Original MSI source](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 **04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c**. All addresses below are DLL RVAs. Use native/static analysis, trap GUI calls and actual filesystem mutation; remove task-owned temporary files immediately.

## First exact code trace

Original variable registration at **0x26587–0x26592** binds FILEDEL to callback **0x28434**. It rejects zero arguments by setting variables+0x436, otherwise loops over the original 21-byte parameter slots and calls helper **0x4524c** once per argument. It ignores helper status and returns NULL (active variable, no returned text).

Helper **0x4524c** copies each nonempty input to a 256-byte local buffer, calls filename-tail helper **0x3f71a**, parses that tail with **0x65660**, rejects only wildcard flag bit 1, then passes the **local buffer** (not the tail pointer) to ordinary lookup **0x3f80e** with mode/retrieval **0,0**. On lookup success it passes the resulting path to remove/unlink **0x6b480**, which calls DeleteFileA through **0x965dc**. No creation/session/host identity or ownership table is consulted anywhere in this callback/helper. File lookup uses current host selection, shared ICONS and configured search path; follow the full original helper in native fixtures rather than substituting a host-only lookup.

Lookup **0x3f80e** first normalizes its name with **0x3f776**, saving/restoring scratch at file-state+0x4e2. It constructs a candidate under selected file-state+0x462 host directory, probes it with access **0x758e0**, then probes shared ICONS, then calls search-path provider **0x3160b** and ordered-path lookup **0x276e9**. Zero retrieval prevents network requests. Exactly one successful lookup path reaches deletion; deletion failure does not return to lookup to try lower tiers.

Path handling needs exact native cases. Tail helper **0x3f71a** scans backward while predicate **0x65c44** accepts a byte; the predicate rejects many punctuation/control characters but **accepts backslash and period**. Thus its notion of filename tail is not a general Windows basename or a reject-all-paths rule. The split-path wrapper sets bit 1 for wildcard characters in name/extension, while helper 0x4524c does not reject its directory/drive flags. Original lookup repeats tail parsing and constructs paths; do not infer from the prose that all supplied path forms are stripped or rejected. Keep any traversal probes entirely in memory with captured paths.

## Next action

Build a memory-only native FILEDEL fixture preserving callback, tail/character predicates, split/compose, full lookup and final unlink. Supply context’s file state, install-root path and controlled search-path callback; capture access candidates and DeleteFileA arguments. Test host/shared/search-path duplicates and fallbacks, no configured host, deletion failure and no match, ownership-independent preexisting files, all path/wildcard forms and multiargument status. Record actual raw paths separately from Win32 normalization. Update the reference/storage guidance only after this matrix settles all subquestions.

---

[Research backlog](issue-backlog.md) · [FILEDEL](../ripscrip/5.1-text-variables.md#filedel)
