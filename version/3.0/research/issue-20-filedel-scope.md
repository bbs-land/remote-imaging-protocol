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

## Original lookup and ownership: native checkpoint

Extended `run/riptel-macro-probe` now passes **388,884 macro/variable checks**. New FILEDEL coverage preserves original registrations, callback **0x28434**, deletion helper **0x4524c**, tail/predicate **0x3f71a / 0x65c44**, split/compose **0x65660 / 0x65766**, full ordinary lookup **0x3f80e**, delivery-name construction **0x3f776**, install-root concatenation **0x3f662**, search-path provider wrapper **0x3160b**, ordered search **0x276e9** and unlink **0x6b480 / 0x6b450**. Original code uses context+0x32 file state, +0x36 options, +0x72 install-root object and +6 client-method object. The fixture supplies these layouts, root **C:\RIP**, optional HOST and **D:\LIB;EXTRA** user search path.

Controlled boundaries are existence-only access **0x758e0**, DeleteFileA IAT **0x965dc**, GetLastError **0x96600** and the user-configured search-path callback at client-methods+0x10. Existing macro probe memory/string/format/ctype and registration capture controls remain as recorded in [issue #6](issue-6-macro-evaluation.md). Neither lookup nor final unlink is replaced with a host-only rule. Access/DeleteFileA only record paths and return modeled status; no host filesystem file is touched, Windows canonicalization is not emulated, and GUI never runs. FreeBSD 15.1 amd64, clang 19.1.7, SHA-verified original driver; the existing wrapper disables core dumps and removes its build directory through its EXIT trap.

Configured host: access order is **C:\RIP\HOST\X.BMP**, **C:\RIP\ICONS\X.BMP**, **D:\LIB\X.BMP**, **C:\RIP\EXTRA\X.BMP**. With no host, the first candidate is omitted and the other three remain. Each possible first-hit position and no-match case is exercised. All candidates existing deletes **only the host hit**; if deletion fails it does not try shared/search-path duplicates. Empty/no-match inputs are ignored. No creation/session metadata is supplied or requested: any first existing file is eligible, including shared files predating the host connection. The host-created requirement is a **caller precondition**, not an enforced provenance check in this shipped path. User search paths are not read-only for this operation, contrary to the earlier inference.

The callback requires at least one argument, sets variable error for zero, ignores each helper’s result and returns no text. A failed delete does not set variable error. A later argument is still attempted; direct empty argument slots are skipped (the actual dollar parser instead omits empty comma fields). Supplied filename case and extension are retained, no default extension is added and legacy ICN/HIC/MSK are not rewritten to BMP/BMH/BMM for FILEDEL.

## Parser and helper are different layers

The initial direct-helper cases are **not** the wire grammar. Full original dollar expansion exposed this before publication. The general parameter predicate **0x155bf** rejects forward slash, wildcard and several punctuation characters before FILEDEL is called; parameter parser **0x15602**, argument state **0x15ad1–0x15b8c**, handles a single backslash as an escape marker and overwrites it with the following accepted character (**0x15ea7–0x15ebd**). Consecutive backslashes reject the expression. Original callback/helper paths do not remove those backslashes because they never receive them in a successfully parsed ordinary expression.

Native full-pipeline cases run original registration lookup, parser and active-variable insertion through **0x4b0e4**, both with full ordinary flags **0xff** and host-first-pass mask **0xff5f** (which defers ordinary backslash/control processing, per issue #6). For supported inputs the callback receives the same resulting filename under both flags. Rejected expressions remain literal with no deletion or variable-error flag, except the full pass can subsequently remove a trailing escape after the parser already rejected that expression; that does not execute FILEDEL in the same pass.

| Literal parameter text in `$FILEDEL(…)` | Full pipeline result | Direct callback/helper observation (not a wire promise) |
| --- | --- | --- |
| `X.BMP` | Delete first resolved X.BMP | Same |
| `C:X.BMP` | Lookup receives C:X.BMP under the chosen directory prefix | Same; no drive stripping |
| `C:\DIR\X.BMP` | Filename becomes C:DIRX.BMP | Original C:\DIR\X.BMP is retained after the directory prefix |
| `DIR\X.BMP` | Filename becomes DIRX.BMP | Directory component retained |
| `..\X.BMP` | Filename becomes ..X.BMP | Parent component retained |
| `\X.BMP` | Filename becomes X.BMP | Leading backslash retained after prefix |
| `DIR/X.BMP`, `../X.BMP`, `/X.BMP` | Expression rejected; no lookup/delete | Tail becomes X.BMP |
| UNC-style consecutive backslashes | Expression rejected; no lookup/delete | Raw backslashes retained after prefix |
| `*.BMP`, `X?.BMP`, `[X].BMP` | Expression rejected; no lookup/delete | Tail becomes .BMP |
| `X*Y.BMP`, `X;Y.BMP`, `X\|Y.BMP` | Expression rejected; no lookup/delete | Tail becomes Y.BMP |
| `X.BMP/` | Expression rejected; no lookup/delete | Empty tail, no operation |
| Trailing backslash | Expression rejected; no lookup/delete | Passed through to lookup |
| `.`, `..` | Passed as literal names; OS deletion success is not asserted | Same |
| Leading/interior spaces | Preserved in filename | Same |
| Empty parentheses | Zero parsed arguments, callback syntax error | Empty direct slot in a nonempty list is skipped |

The direct tail predicate **0x65c44** is independently checked for all **256 input bytes**. It accepts 0x20–0x7f except byte values **34,42,43,44,47,59,60,61,62,63,91,93,124**; high bytes fail the initial signed-byte comparison. Direct tail stripping chooses the suffix after the last rejected byte, but returns empty/no operation when the last byte is rejected. The split-path wrapper’s wildcard flag is checked only after that tail selection. The ordinary argument predicate uses an unsigned-byte lower bound, so do not substitute one character predicate for the other.

Microsoft’s [Win32 filename/path rules](https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file) identify backslash-separated components and `..` parent components; its [path-format documentation](https://learn.microsoft.com/en-us/dotnet/standard/io/file-path-formats) distinguishes drive-relative from absolute names. These describe the controlled OS boundary, not this client’s parser. A captured prefixed path containing a colon, directory name, dot component or trailing separator is **not** proof that Windows accepts it as an existing regular file. In particular, the raw-helper parent-component observations must not be presented as a verified ordinary FILEDEL wire traversal: the parser removes/rejects backslashes as above.

## Next action

Checkpoint this complete native evidence, verify the remaining generic argument escape rule if needed, then replace the storage hypothesis with first-hit all-tier deletion, no-host fallback and caller-only provenance semantics. Correct FILEDEL’s malformed Format line, document actual parser/path behavior and qualify inherited reject-all-filenames guidance. Add AGENTS.md parser/helper cautions, run required checks and file the complete #20 PR after review of the exact wire cases.

---

[Research backlog](issue-backlog.md) · [FILEDEL](../ripscrip/5.1-text-variables.md#filedel)
