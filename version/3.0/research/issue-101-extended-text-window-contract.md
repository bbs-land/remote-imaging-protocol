# Issue #101: Extended text windows and terminal controls

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md) · [Variables](../ripscrip/5.1-text-variables.md)

## Scope and checkpoint

Started 2026-11-20 on `issue-101-extended-text-window-contract` from fork stable 93fd0c3 after #100 / [PR #102](https://github.com/bbs-land/remote-imaging-protocol/pull/102). That stable merge includes the actual PR head dfbca09 and has an identical source tree. [Issue #101](https://github.com/bbs-land/remote-imaging-protocol/issues/101) requests automatic fitting, MAF versus nominal metrics, coordinate/undersize/validation rules, erase sources/gates, TWGOTO and COFF corrections. Preserve #16/#51 and 1.54. Separate primary facts from explicit completions of unspecified cases; no obsolete-client bug investigation or original/client/UI/probe execution.

## Sources, identity and read-only boundary

Use [the catalogued artifact mirror](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Preferred image base is `0x10000000`; addresses below are RVAs, not file offsets. LLVM objdump 19.1.7 reads instructions without executing them. Section biases are `.text` RVA minus `0xc00`, `.rdata` minus `0x1000`, `.data` minus `0x1a00`; use the PE directory rather than extrapolating a section's mapping. An exploratory table read using the wrong `.rdata` bias returned nonsensical transitions and was discarded; section-aware bytes produce the documented transitions below. The earlier #100 note's prose mapping was corrected in PR #102 head b23a943, merged into this branch before delivery; its section-aware ANSI-table reading and state conclusions are unchanged.

The immutable [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, is primary for command syntax, flags and normative color/protection rules: §3.4.1.8 (line 4722 onward), §3.4.1.10 (4778), §3.4.1.12 (4998), §5.6.2 (14467), §5.6.8 (14690), §5.6.13 (14897), §5.6.15 (14974), general shortening rule 10 and protection discussion at lines 2022–2046. Do not change the original text or RIPscrip 1.54.

Original `extracted/ripscrip.maf` is 270,945 bytes, SHA-256 `8c0be38eee89b6935ed6f93885c14350f4a6db0a10a72a38d126811bbd090cef`. Its six 60-byte directory records and thirty 50-byte font headers reproduce the existing [MAF metric table](../techspecs/3.3-microansi-maf-delta.md#the-new-resolution-set), including 1024×768 font 4 at 20×24. No client, ANSI parser, font loader, callback, pixel renderer or UI was executed; all new cases below are documentary/byte/arithmetic review.

## Extended definition and fitting

Original `b` dispatch slot 20 -> RVA `0x1b075`, nine arguments, flags `0x0b`. Its first four types are XY; width/height are fixed two characters, font one, flags four, reserved three. The printed repeated fourth `y0` is a documentary typo: the descriptor and consumer read the fourth value as Y1. Requested dimension guards are `0x1b117/0x1b137`; font guard `0x1b0f3–0x1b112` ignores the font when flag 8 is present. Current-TW protection is checked at `0x1b15a`. The handler never reads its ninth reserved argument. General rule 10 expressly permits omitting a final reserved field and then shortening the last transmitted numeric field; do not invent a requirement for an explicit `000` suffix.

Both endpoints pass WORLD/device helper `0x31084` at `0x1b18a/0x1b19b`, then ordering helper `0x3112e` at `0x1b1ab/0x1b1bb`; SetRect and IsRectEmpty at `0x1b1db/0x1b1e5` preserve exclusive far edges, with no increment. Under the six documented flags there is no PORT/viewport origin addition or pre-fit clipping. The native handler accepts additional flags up to `0x3ff`, including a separate PORT-relative branch under `0x80`, outside the documentary protocol. Do not turn these into baseline guarantees or investigate their defects. Rejection of unsupported bits outside 0–63 is an explicitly chosen portable validation boundary, not a claim that every original driver rejects bit 64.

Dispatcher `0x7988` chooses explicit fitting `0x7ae8` or automatic wrapper `0x8008`. The wrapper supplies **font 4**, then uses the same fitter; it does not enumerate/scoring-sort all fonts. Each candidate multiplies requested columns/rows by the font's device cell metrics, compares both dimensions, selects a width/height/both transition, and retests the full original request. Exact-fit uses <=. The first fitting candidate wins. Section-aware signed-word tables at `.rdata` RVAs `0x760e8/0x760f8/0x76108` are:

| Font | Width | Height | Both |
| ---- | ----- | ------ | ---- |
| 0    | 1     | -1     | -1   |
| 1    | -1    | -1     | -1   |
| 2    | 3     | 0      | 1    |
| 3    | -1    | 1      | -1   |
| 4    | 2     | 0      | 0    |

Here -1 is the draft's `n/a`. There are no cycles. At the terminal candidate, the draft requires reducing columns/rows independently until they fit, with a one-cell minimum and adjusted bounding box. The reference gives the equivalent wide-integer floor/min formula. It explicitly chooses to retain each near edge and extend only an undersized far edge; the draft supplies no exact anchor. Original tiny-box/recovery writes and internal 16-bit multiplication are not a portable geometry contract and were not executed or debugged. Wide arithmetic and failure for nonpositive/unavailable font metrics are explicit safe completion rules.

Metrics are **not merely glyph bitmap trivia**: fitter `0x7b18–0x7b44` and subsequent candidates `0x7bbc–0x7bf3` read TW-system cell-width words at `+0x4c + 2*font` and heights at `+0x56 + 2*font`. Initialization `0x473d8–0x4745d` first computes nominal screen/grid floor metrics. On successful MAF load `0x47536 -> 0x2d744`, the returned dimensions overwrite those arrays at `0x4754b/0x47553`. Loader `0x2d957` reads a 50-byte font header; `0x2d9ef–0x2da0d` returns its first two words as dimensions. Thus a loaded 1024×768 font 4 uses 20×24, not nominal 25×30. No rewrite of ordinary `w` placement is implied by documenting this extended consumer.

Placement `0x7d85–0x7e49` copies the bounding rectangle, computes display dimensions from final counts/metrics, and centers on both axes, truncating nonnegative spare pixels by half. The portable formulas center after any explicit one-cell box extension and leave an odd pixel at the far edge. Graphics drawing clip state does not define the TW bounding rectangle; addressable screen pixels bound actual display/erase.

## Definition state and clear order

Unlike `w`, extended definition `0x7c8f` has no identical-definition bypass. It preserves old cursor-visibility bits (`+0x26 & 6`) at `0x7d5e–0x7d67`, then calls record initialization `0x39935`. That initializer saves bytes `+0x43..+0xee`, clears the entire `0xf1`-byte record and restores that saved range. Consequently the tab-stop map and character-set selectors are reset here; do not generalize ordinary `w`'s retention rule. The subsequent writes set full margins (`0x7eb3–0x7ed9`), saved cursor zero (`0x7edc–0x7ef6`), normal 7/0 ANSI colors (`0x7f1b–0x7f3f`), default flags plus old cursor visibility (`0x7f44–0x7f68`), and live cursor zero (`0x7f86/0x7f95`). Separate ANSI syntax state at context `+0x46` and global client options are not reset.

Outer `0x7988` then applies wrap, clear flag 16 via `0x4597b(-1)` at `0x7a40`, cursor-off flag 2, deactivate flag 4 and protection flag 32. The documented clear-before-final-cursor/deactivate order controls the protocol; no transient Windows cursor-paint equivalence is claimed. Without flag 16, defining geometry/state does not erase the screen. A repeated accepted `b` always resets its ANSI window state; a protected target blocks the whole definition.

## Erase reconciliation: protocol before implementation

The draft explicitly declares **Uses Back Color: NO**, **Uses Port: NO**, **Uses Viewport: NO** for `e`, and its body says color #0. Extended flag 16 also expressly says color #0. These stronger statements resolve the ambiguous phrase “current graphics background color” and generic function title: use **index 0 of the current graphics palette**, not STYLE's mutable background, fixed RGB black, or a prior ANSI background. `$ETW$` defines the same whole-bounding-box operation but does not repeat the color; this reference explicitly adopts `e`'s index-0 contract for it. This is a documentary reconciliation/completion, not a native-color identity claim.

Original `e` path `0x1ad98 -> 0x4597b` and ETW callback `0x2e20d -> 0x4597b` share a target iterator. Its `0x459ca` checks defined/active **TW** through `0xe028/0x2c075`, not graphics PORT activation or definition protection, then erases the stored bounding rectangle and homes the live cursor. The literal native paint call at `0x45a17–0x45a2e` uses the current ANSI brush, which can disagree with the documentary index-0 rule after ANSI color changes. Record that limit instead of allowing an implementation discrepancy to override the specification; no obsolete-client bug was investigated. Extended flag 16 runs after reinitializing ANSI colors to 7/0, so its normal branch agrees on zero.

The draft separately declares **Uses Back Color: YES** for wire `>`, and expressly distinguishes it from ANSI `ESC[K`. §5.6.13 instead specifies ANSI color attributes for `$TWERASEEOL$`; original callback `0x2e4fe -> 0x45b7f` and target consumer `0x45bd6` read the target TW background nibble at `+0x2e`, map it through ANSI color helper `0x3dce0` and create its erase brush at `0x45c4f–0x45c66`. Keep the documented distinction: wire `>` uses STYLE background, the macro uses target ANSI background. Both cover the cursor cell to the display row's final cell, leave bounding margins and cursor unchanged, and do not use graphics W/fill. No assertion of identical native wire/macro rendering is made.

The ETW sentence mentioning a deactivated viewport contradicts its subject, target grammar and actual TW predicates; correct both nouns to text window. Draft protection rules expressly allow output/ANSI/erase on protected TW definitions. Undefined/inactive targets are no-ops, unrelated active/inactive graphics PORT state does not enable/suppress them, and overlapping graphics may be overwritten. Syntax prevalidation of target lists remains unchanged. No table-copy, backup or deferred-query contract is changed.

## Cursor controls and related corrections

Original registrations in `0x25c02` map TWGOTO to `0x2e624`, COFF to `0x2df01`, ETW to `0x2e20d` and TWERASEEOL to `0x2e4fe`. The immutable TWGOTO text contradicts itself, first saying 1-based and then zero-based with first column zero; its allowed range also starts at zero, and wire `g` is explicitly zero-based. Original TWGOTO passes parsed positions without subtracting one through `0x9f4f`; live setters `0x9cf3/0x9d9a` use window-relative positions, clamp against the selected TW dimensions and do not read origin mode or scroll margins. Correct the macro to zero-based column/row, and limit to actual chosen-window bounds. Numeric data is decimal; malformed arguments retain the macro grammar's syntax failure behavior.

The documentary defaults are retained: no arguments or no coordinates does nothing, WINNO is otherwise required, omitted Y preserves the chosen target's row, and `CUR` preserves the chosen target's coordinate. The original `CUR`-X branch at `0x2e688` queries current slot -1 even when a different target was selected; this does not override the explicit target-relative documentary promise. Likewise internal byte/word narrowing does not define nearest-border positioning. The reference explicitly completes wide-coordinate handling by clamping before narrowing, without bug probes. This is the intended protocol, not a promise to reproduce original edge paths.

COFF's omitted-target branch `0x2df15–0x2df1a` supplies cursor-off (0), while the corresponding CON branch supplies on (1). Shared `0x18dbc` tests target existence and TW activation before changing cursor-enabled bit 2. Thus omitted COFF hides the current active cursor, and a deactivated target does not acquire a deferred visibility change. The inherited “displayed” sentence is a copy error. Definition protection does not gate this operation.

The same positioning boundary clarifies `g`, `H` and `$TWHOME$`: full-window absolute coordinates, independent of ANSI origin/margins, no graphics PORT gate. Fix `g`'s inherited ANSI example to **row then column**, and qualify ANSI-home equivalence by origin mode off; this prevents a related contradiction in this family. The TWERASEEOL format's missing `,...)$` is also corrected. No earlier-version page is changed.

## Reproducible byte and arithmetic review

Bounded instruction reads from the repository root, replacing `DLL` with the original mirror path:

```sh
run/riptel-inspect DLL --slot 20
objdump -d --start-address=0x1001b075 --stop-address=0x1001b2e6 DLL
objdump -d --start-address=0x10007988 --stop-address=0x10008034 DLL
objdump -d --start-address=0x100473d8 --stop-address=0x10047561 DLL
objdump -d --start-address=0x10039935 --stop-address=0x10039985 DLL
objdump -d --start-address=0x100459ca --stop-address=0x10045a78 DLL
objdump -d --start-address=0x1002e624 --stop-address=0x1002e719 DLL
objdump -d --start-address=0x10018dbc --stop-address=0x10018e33 DLL
```

A section-aware byte review reads five signed little-endian words at each transition RVA and compares the table above. A separate MAF directory/header read checks all thirty metrics against the existing table and independently reads the 1024×768 profile. The final specification's three fitting examples use that profile: automatic 40×25 in a 1000×600 box selects 4 and centers an 800×600 display; in an 800×500 box it follows 4→0 and centers 480×425; a 1×1-pixel box follows 4→0 then uses the chosen one-cell extension to 12×17. Eight fitting vectors check exact-fit, each overflow category, explicit-font versus automatic preference, dimension reduction and odd centering; separate arithmetic checks cover reversed/scaled bounds and zero-device-extent cases. A first expected vector incorrectly followed font 0→1 while both axes overflowed; the actual table says `n/a` in that case. Correcting that expectation gives 8×5 at font 0 in a 100×100 box, centered at `(2,7)`, rather than 9×5 at font 1. All corrected checks pass; this was a calculation-fixture error, not original execution. These are calculations, not native probe counts.

## Status and remaining work

Specification complete. Markdown lint, 5,853 links across 239 files, diff checks, original transition/MAF data review and eight corrected fitting vectors pass. Refresh started 2026-11-20T14:40:48Z finds unchanged #100/#101 bodies, no new discussion/inline/review feedback, all 49 open PR heads ancestral and only #101 still uncovered. [PR #103](https://github.com/bbs-land/remote-imaging-protocol/pull/103) is open/ready and closes exactly #101; specification commit 2c22866. Fork stable merge cb38c28 includes actual PR head ed0f290, with an identical source tree before final workflow notes, and preserves corrected #102 head b23a943. Refresh started 2026-11-20T14:43:01Z confirms all 50 open PR heads ancestral and all 51 open RealDeuce issues covered, with no later feedback. Delivery checks pass with 5,861 links; no active work remains. Explicit choices are unsupported-flag rejection, wide arithmetic, unavailable/nonpositive metric failure, near-edge one-cell expansion and wide macro clamping; erase color reconciliation follows documentary attributes and explicit index 0. No temporary artifacts created, original files changed or original/client/UI/probe instructions executed.

---

[Research backlog](issue-backlog.md) · [Terminal reference](../ripscrip/1.4-terminal-ansi-view.md)
