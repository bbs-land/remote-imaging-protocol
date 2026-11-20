# Issue #105: Outer SOFT drawing reset

[Research backlog](issue-backlog.md) · [Reset command](../ripscrip/1.3-world-view-virtual-canvas.md#rip_reset_windows)

## Scope and checkpoint

Started 2026-11-20 on `issue-105-outer-soft-reset` from stable ba97c88, which merges #104 / PR #107 actual head 16164ae with an identical source tree. [Issue #105](https://github.com/bbs-land/remote-imaging-protocol/issues/105) requests the complete outer `*` contract: family indices/protection, screen-clear source/order, TW and graphics/parser defaults, input/mouse/audio and unaffected variables/backup ownership. #106 selection follows separately. Retain #16/#21/#24 and #100/#101/#104 answers; source evidence supplements rather than overstates earlier controlled fixtures.

## Sources and verification boundary

Original RIPtel 3.1 / driver 3.0.7 DLL from [the catalogued mirror](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred base `0x10000000`. Addresses below are RVAs; `.text` file offset = RVA minus `0xc00`, `.rdata` minus `0x1000`, `.data` minus `0x1a00`. LLVM objdump 19.1.7 and Python perform bounded instruction/data reads only. No original instructions or client/GUI/audio/probe behavior is executed.

Primary documentary sources: immutable [ALPHA 4](../../2.0/text/RIPScrip-2.0-alpha-4.txt) SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, §3.4.1.44 (line 7156 onward), protection rules at lines 2022–2046, §5 RESET (16640 onward) and the explicit preserved-backup note. The old command bullets are incomplete/conflicting with the individually recovered PAL lifecycle and HEADER's separate zero-selection action. Correct only the reconstructed 3.0 reference.

Existing [#16 family/palette](issue-16-table-lifecycle.md), [#21 outer image/port](issue-21-image-style.md#original-outer-resetheader-checkpoint), [#24 defaults/ownership](issue-24-drawing-state-ownership.md), [#26 backup ownership](issue-26-screen-backups.md), [#42 clipboard](issue-42-clipboard-capture-state.md), [#86 metric-cache policy](issue-86-outline-advance-policy.md), [#100/#101 definitions](issue-101-extended-text-window-contract.md) and [#104 charsets](issue-104-terminal-character-sets.md) remain adopted. Their earlier native fixtures prove only the boundaries recorded there; this note supplies new read-only outer/callee evidence, not invented end-to-end native results.

## Outer call and selection ledger

Wire `*` handler `0x1f2f9` flushes pending drawing bookkeeping, skips when graphics is disabled, and calls `0x16220(context, keep_ports=0, clear=1, update=1)` at `0x1f31b`, then marks font metrics dirty through `0x3c37e`. There is no following all-zero selector or input-unlock call. SOFT's relevant sequence is:

| Caller RVA | Reached consumer / effect |
| --- | --- |
| 0x16263 / 0x16266 | Reset active drawing bookkeeping and scene-header revision marker at ENV-system +0x1a to zero |
| 0x16277 | Native old-current STYLE border word +0x22 = 1; portable protection disposition below |
| 0x16287 / 0x16290 | Clear mouse/button instances via 0x27f8f; clear refresh expression via 0x3e41a |
| 0x16298–0x162eb | Old-current PORT image rectangle all -1 and image flags zero, the adopted #21 exception |
| 0x162f0 | Reset outline/font engine state through 0x3c1cf; metric lifetime remains the separately adopted policy |
| 0x16309 / 0x16315 / 0x1632b | Select PORT 0; delete unprotected nonzero ports through 0x33112(-2,0); rebuild physical PORT 0 |
| 0x1633b / 0x1634d | Literal-color solid brush, fill physical screen rectangle; clear details below |
| 0x1636f / 0x1637e | 0x440f0(-2) resets unprotected PAL records; 0x3e2e7 uploads current palette, with no PAL selection change |
| 0x16389 | Preliminary ANSI reset 0x35320 on old-current TW |
| 0x16391–0x163f0 | Release floating PORT/TW query storage and transient mouse action strings |
| 0x163fb / 0x16406 | 0x391fd(-2) defaults unprotected STYLE records, then 0x394fe(0) selects STYLE 0 |
| 0x1648b / 0x16497 | Classic font 0/direction 0/size 1 and left/top engine justification, through 0x10f0/0x13d0 |
| 0x164a2 / 0x164ad | 0x4558d(-2) defaults unprotected BUT records; explicit current BUT index = 0 |
| 0x164be | Establish default viewport before later physical PORT-0 initialization |
| 0x164c9 / 0x164d3 / 0x164e8 | Discard unprotected nonzero TW records; select TW 0; ordinary full-screen initializer 0x8577 using the user's default font |
| 0x164fe / 0x16508 | 0x3d7d1(-2) defaults unprotected ENV records, then 0x3dac0(0) selects ENV 0 |
| 0x16512 / 0x1651b | Pointer 0 (arrow) through 0x3468a; 0x16b97 marks pointer/input presentation state without clearing lock flag |
| 0x16524 | 0x33d82 initializes PORT-0 graphics cursor (0,0), image fallback and full viewport |
| 0x16538 / 0x1654c / 0x16560 / 0x16573 | Reapply ENV-0 base, RGB precision, coordinate width and stored WORLD 640×350; 0x31052 updates active WORLD scaling |

STYLE `0x391fd(-2)` and BUT `0x4558d(-2)` copy their fixed default records into unprotected entries, clear nonzero in-use flags and ensure zero defined. ENV `0x3d7d1(-2)` calls default initializer `0x3d5fa` for each unprotected record then clears its in-use flag, retaining defined zero. PAL's already adopted `0x440f0(-2)` similarly defaults RGB/clears nonzero definition while preserving current index. TW `0x39ac8(-2)` instead discards nonzero unprotected records rather than defining defaults in every slot; it skips protected records. PORT deletion preserves protected nonzero screen/offscreen entries. Protection is not a prohibition on selecting another current entry.

Thus final current TW/BUT/STYLE/PORT/ENV are 0; PAL is retained even if reset made that selected slot undefined. HEADER global bit 8 has an independent six-family zero selection, as already recovered in #5; do not append that action to bare wire `*`.

## Screen-clear source and explicit portable color result

At `0x16339` the source pushes **0** into IAT `0x96418` (CreateSolidBrush). FillRect at `0x1634d` targets PORT-0 screen DC and physical rectangle at PORT-system `+0x32`, independently of the previous offscreen target or viewport. Palette reset/upload occurs later at `0x1636f/0x1637e`. The raw source is neither STYLE's background nor a palette-index color token. Microsoft's primary [CreateSolidBrush](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createsolidbrush) and [COLORREF](https://learn.microsoft.com/en-us/windows/win32/gdi/colorref) definitions corroborate that literal `0x00000000` is RGB black. These API references explain the arguments, not a promise about old palette-device raster realization.

The draft also intends a black clear, but its explanation says the palette was just reset. That causal/order claim is false, and protection can retain a nonblack palette entry 0. The **explicit portable display rule** is a full physical-canvas RGB-black result that is not later recolored from PAL entry 0. PAL protection/selection remains independent; subsequent index-0 drawing can use its retained nonblack RGB. This is not a claim of exact GDI palette-index quantization, dithering or subsequent native hardware recoloring. A strict indexed backend must implement the black canvas result without rewriting a protected palette. This choice differs from the separately adopted fixed-palette-index-0 `e`/ETW/b-flag-16 contract and is linked there to prevent conflation.

## Protection, text and environment details

The new family table distinguishes whole-record family reset from unconditional preparation. The adopted #21 old-current PORT image mutation and #24 departing-ENV activation write-back remain recorded, without investigating them again. ENV selector writes base/RGB/coordinate setters before changing index, so a protected old-current ENV receives 36/0/2 in those fields; stored WORLD and pointer number remain its own. Protected noncurrent ENV records are not affected.

Original old-current STYLE +0x22 is the **border-enable word**, not font size: the default/filled-shape consumers and #24 default record corroborate that identity. The native early write `0x16277` bypasses protection. Unlike the previously adopted image/ENV boundaries, no protocol rule authorizes a new exception for protected STYLE configuration. The portable reference therefore **preserves protected STYLE configuration**, explicitly records the native limit, and does not require reproducing this write. This is a protocol-first reconciliation against the protection rule, not a new client bug investigation or an unmarked native-equivalence claim.

Preliminary ANSI reset `0x35320` runs on the old current TW before TW-family cleanup. It changes ANSI colors to 7/0, clears rendition/insert flags using mask `0xfffffb0f` at record +0x26, homes active live cursor through active-gated setters, zeros saved coordinate bytes +0/+1, restores full scrolling margins and default tab map (`0x80` in each of thirteen bytes). It does not clear all other per-window mode bits or charset fields. This remains permitted ANSI state manipulation on a protected definition. An inactive protected old-current TW therefore retains its live cursor/activation while colors/saved/margins/tabs can change; a protected noncurrent TW receives none of this preliminary reset.

Final TW 0 is selected and redefined via the ordinary initializer, giving full nominal grid/default user font, activation, cursor-on, normal ANSI colors/rendition, full scrolling margins, live/saved origin and user wrap. Ordinary definition retains TW-0 tab/charset fields; if zero was old-current, its tab map already received the preliminary ANSI reset, otherwise its old tab map survives. Character sets/global emulation and partial parser state remain as #104/#100 specify. The full physical clear is separate from the nominal text display's possible unused screen edge space.

PORT-0 graphics cursor is zeroed by `0x33d82`; other protected ports retain their cursor. STYLE-0 defaults and classic engine initialization are those already established by #24. The ordinary outline width snapshot is invalid after a full drawing reset by the explicit #86 policy; no native measurement lifetime is newly claimed. Scene-header revision marker becomes 0; ENV-0 active WORLD/base/width/RGB defaults are 640×350 / 36 / 2 / 0.

## Input, audio, ownership and unaffected state

`0x27f8f` obtains the live mouse system at context +0x1e, releases field objects and clears its 0x5180-byte live table. This deletes buttons/mouse instances, not protected button-style records or independently saved mouse backups. `0x3e41a` releases the refresh expression. PORT/TW floating query strings are separately released by outer code; resource deletion of a live entry also discards its owned strings, but independent backup copies survive.

Mouse pointer setter `0x3468a(0)` changes ENV-0 pointer/presentation. `0x16b97` merely writes ENV-system `+0x1e = 1`; it **does not clear input lock +0x1c**. Explicit unlock `0x16b51` clears +0x1c/+0x1e, and the bare `*` handler/outer path does not call it. HEADER invokes unlock separately after its SOFT step; bare `*` does not. Thus arrow presentation is not evidence that keyboard/mouse lock was released.

SOFT has no audio-stop request. The explicit RESET(SOUND)/audio consumers are separate; normal playing audio can continue. Variables and template stores are separately owned (variable context +0x26 is not reset). All eight backup families retain base areas, numbered slots, stacks and stored protection/content under the draft's explicit preserved-backup rule; HARD and explicit backup reset have separate deletion calls. Do not conflate logical font/cache resources freed during SOFT with named/template stores or saved tables/screens.

Live clipboard is an offscreen PORT: the previously proved #42 rule applies unchanged. Unprotected clipboard deletion clears its assigned pointer; protected clipboard/pointer and pixels survive. Separate PORT/SCREEN backup pixel copies remain owned by their backups regardless of live deletion or screen clear.

## Reference changes, vectors and remaining work

Replace the contradictory RESET_WINDOWS bullets with family selections/protection, clear order/color and explicit source/policy limits, complete state effects, unaffected list and eight before/after rows. Correct the related all-TW-discard sentence in SWITCH_TEXT_WINDOW, link RESET(SOFT) to the full contract and cross-link the PAL lifecycle. #106 remains responsible for first-use selection copy/parser details.

The eight matrix rows are documentary/instruction-derived expectations. They cover all nonzero current selections, protected PAL 5 with red entry 0, protected current offscreen PORT 5 versus protected noncurrent 6/unprotected 7, active/inactive protected TW 5, protected STYLE border, departing ENV write-back, lock and independent clipboard/variable/backup lifetime. They are not end-to-end native tests; prior probes' counts are not enlarged here.

Reproduce bounded reads from the repository root, substituting the original mirror path for `DLL`:

```sh
objdump -d --start-address=0x1001f2f9 --stop-address=0x1001f32e DLL
objdump -d --start-address=0x10016220 --stop-address=0x10016665 DLL
objdump -d --start-address=0x100391fd --stop-address=0x1003935b DLL
objdump -d --start-address=0x1004558d --stop-address=0x10045674 DLL
objdump -d --start-address=0x1003d7d1 --stop-address=0x1003d8cc DLL
objdump -d --start-address=0x10016b51 --stop-address=0x10016ba5 DLL
objdump -d --start-address=0x10033d82 --stop-address=0x10033df3 DLL
```

Checks pass: sixteen original direct-call targets, SOFT arguments, raw-black brush and input-lock-marker bytes, eight manually reviewed matrix rows, Markdown lint, 5,898 links across 241 files and diff checks. [PR #109](https://github.com/bbs-land/remote-imaging-protocol/pull/109) is open/ready and closes exactly #105; specification d821419. Actual-head stable consolidation follows. Refresh 2026-11-20T21:55:41Z found unchanged #104–#106 bodies and no comments. No temporary payloads/artifacts or original/client/UI/probe execution.

---

[Research backlog](issue-backlog.md) · [Reset command](../ripscrip/1.3-world-view-virtual-canvas.md#rip_reset_windows)
