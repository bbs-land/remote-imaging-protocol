# Issue #100: Ordinary text-window redefinition

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md#rip_text_window)

## Scope and checkpoint

Started 2026-11-20 from fork stable b8c70aa on `issue-100-ordinary-text-window-state`. [Issue #100](https://github.com/bbs-land/remote-imaging-protocol/issues/100) asks for ordinary `w` redefinition state, pixel effects, deactivation/reactivation and a corrected font-2 example. #101 follows separately. Preserve the adopted #16 table/backup and #51 query-dispatch contracts and RIPscrip 1.54.

## Primary sources and reproduction

Use the unmodified DLL under the [catalogued artifact mirror](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred base `0x10000000`. Addresses below are RVAs. LLVM objdump 19.1.7 reads instructions only; no original instructions execute. Section mapping is file offset = RVA minus `0xc00` for `.text`, minus `0x1000` for `.rdata`, and minus `0x1a00` for `.data`. Inspect PE section headers before applying a mapping. An initial attempt to read the ANSI table at RVA `0x762c0` with the `.data` bias yielded an impossible count and was discarded; section-aware decoding gives 24 entries for group 0.

The immutable [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, lines 1875–1910 describes per-window ANSI colors, cursor, margins, visibility and activation but no saved text contents. Command §3.4.1.54, lines 7750 onward, establishes nominal font grids, placement, protection and the identical-definition exception. Its `(20,25)` font-2 example contradicts its own 0–24 table. Correct only the 3.0 reconstructed Markdown, leaving the source and 1.54 frozen.

Reproduce the bounded reads from the repository root, replacing `DLL` with that mirror path:

```sh
run/riptel-inspect DLL --slot 69
objdump -h DLL
objdump -d --start-address=0x100209cf --stop-address=0x10020cbc DLL
objdump -d --start-address=0x10008034 --stop-address=0x100083a4 DLL
objdump -d --start-address=0x1002bf9e --stop-address=0x1002c0df DLL
objdump -d --start-address=0x10037177 --stop-address=0x100372cd DLL
objdump -d --start-address=0x10037b4b --stop-address=0x10037c4a DLL
```

## Original control and state derivation

Physical dispatch slot 69 is `w`, handler `0x209cf`, six arguments, descriptor `[2,2,2,2,1,1]`, flags 3. It rejects font >= 5 at `0x20a40`, checks current-slot protection through `0x27642(-1)` at `0x20a61`, orders X/Y pairs through `0x3112e` at `0x20a87/0x20a97`, and clamps all four coordinates at `0x20acd–0x20b11`. Extent helpers `0xe3b1/0xe3ec` provide the five nominal grid sizes; their mode input `0x3dc6e` is an original constant-zero return. A warning is reported for clamping, then processing continues. The all-zero bounds/wrap branch at `0x20b47–0x20b6d` invokes activation `0x2bf9e(-1,0)` without overwriting the definition.

TW system = context `+0x12`, selected index `+0x0a`, record stride `0xf1`. Stored near cell coordinates `+0x22/+0x23` and width/height `+0x24/+0x25`, plus font bits `+0x2e & 0x700`, establish identity at `0x20b72–0x20bd0`. Activation is not part of that comparison. The matching branch updates wrap bit 8 at `+0x26`, handles additional historical wrap-field bits outside this report, then activates an inactive slot at `0x20c3e–0x20c58`. It does not redefine or reset ANSI state. The normative documented wrap values remain 0/1.

A mismatch calls `0x8034` at `0x20c8d`. That routine writes new geometry, full-height scroll margins `+0x2c = 0`, `+0x2d = rows - 1` (`0x8148–0x8170`), saved cursor bytes `+0/+1 = 0` (`0x8289–0x82a5`), ANSI color byte `+0x2e = 7` preserving the higher packed bits (`0x82aa–0x82d1`), and per-window flags dword `+0x26 = 7` (`0x82e2`). It applies wrap, marks the slot active/defined, and homes the live cursor via `0x9cf3/0x9d9a` (`0x8331/0x833d`), which write live cursor `+0x2a/+0x2b`. Distinguish these from saved coordinates: ANSI save `0x37177` copies the live positions into `+0/+1`, and restore `0x371c1` reads those saved bytes.

ANSI mode consumers corroborate the flag meanings: mode 2/4/20 at `0x36cba/0x36d33` writes keyboard-lock/insert/newline bits in `+0x27`; private mode 1/6/7 at `0x372cd/0x37333` writes cursor-key/origin/wrap bits in `+0x26/+0x27`. SGR `0x36dac` writes rendition bits in `+0x26` and the foreground/background nibbles in `+0x2e`. Definition therefore clears these flags and supplies normal defaults rather than retaining them. Character-set selection consumers `0x37a3a–0x37ab8` use higher bits in `+0x2f`, which the definition preserves; tab-stop bytes `+0x32` onward are also untouched. Global client emulation/communication options are not window state and are not changed by `w`.

Incomplete ANSI syntax lives at context `+0x46`, parser state `+0x0c`, counts/parameters `+0x0e/+0x12/+0x14/+0x18..+0x1a`. Explicit parser reset `0x37b4b` clears these, and parser `0x37b7c` consumes them. Neither ordinary definition nor activation calls that reset or writes those fields. Font selection `0xddfc -> 0x4876d` updates font/cache resources; color brush refresh `0x35577` updates ANSI brush resources; neither resets parser state. The handler's final `0x39c0a` is a one-byte return. This is positive separation of producers/consumers, not inference from a missing help string. UTF-8 is absent from this byte-oriented source and is not specified as a protocol extension here.

Deactivation `0x2bffd` checks existence/protection and hides the cursor before clearing only active mask `0x01` at `+0x26`. Reactivation sets that bit and refreshes the cursor; it preserves other fields. Definition/activation have cursor/font/brush resource operations but no region erase, text-store allocation, pixel copy or background paint. The protocol's existing pixel-overlap model also explicitly excludes saved cell contents. Newly included pixels must therefore remain as they were; a modern backing store cannot silently turn redefinition into erase/repaint.

## Dispositions and review vectors

The reference now states every requested boundary without extending the `b` reset sentence by analogy. Font-2 bounds `(10,15)..(20,25)` order/clamp to `(10,15)..(20,24)`; inclusive extents are 11 × 10, while the upper-left reference origin remains `(80,210)`. Changing only wrap retains live/saved cursor, SGR, margins and per-window modes. Changing bounds or font homes both cursors, sets full margins, 7/0 colors and default per-window flags while retaining tabs, character-set selections and unfinished ANSI syntax. Deactivate and repeat the stored definition restores activation and preserves the same state. Protected commands do nothing. Pixel preservation is independent of the ANSI defaults.

No new interoperability policy is needed for the inherited byte protocol. Optional UTF-8 decoder state remains explicitly outside this language's contract, rather than a fictitious RIPtel guarantee. Resource failure behavior is not newly defined. Existing #16/#51 table and deferred-query answers, ordinary placement/cell-size formulas and RIPscrip 1.54 are preserved. These examples are documentary/instruction-derived, not native execution results.

## Validation and next action

`run/lint`, `run/check-links` (0 bad of 5,823 links across 238 files), `git diff --check`, artifact hashes and manual state/placement review pass. Refresh started 2026-11-20T14:29:51Z: #100/#101 bodies unchanged, no issue or repository discussion/inline feedback since the previous watermark. [PR #102](https://github.com/bbs-land/remote-imaging-protocol/pull/102) is open and ready with exactly #100 as closing reference. Specification commit `862c986`; initial fork stable merge 93fd0c3 contains delivered head dfbca09 with an identical tree. Reproduction-note correction b23a943 is included in #101 actual head ed0f290 and stable merge cb38c28. The final query-start refresh 2026-11-20T14:43:01Z verifies all 50 open PR heads ancestral, all 51 open RealDeuce issues covered and no new feedback. No task-owned temporary files, client/UI/native probes, source assets or original text were created or changed.

---

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md)
