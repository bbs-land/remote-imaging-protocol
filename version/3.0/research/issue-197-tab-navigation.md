# Issue #197 — Tab navigation and OPTION(LIST)

[Research backlog](issue-backlog.md) · [Tab navigation](../ripscrip/4.2-mouse-fields-and-pointer.md#tab-navigation) · [OPTION](../ripscrip/5.1-text-variables.md#option)

## Scope

[RealDeuce report #197](https://github.com/bbs-land/remote-imaging-protocol/issues/197), created 2026-11-23 22:37:52 UTC, body SHA-256 `afd5cca4d1cc954d2b2381e7a49b5fe2a1f327698cd45f3b68d902f5e29acdbb`, zero comments at intake. The 22:40:08Z audit found only #197 uncovered, with all 96 open PR heads ancestral. Started from fork stable `b0aada2e7d5053aee596b2f6f76efa86ab189a31`. Keep the adopted keyboard, pointer and field lifetime contracts. No original client or GUI execution.

## Initial source

Use the [catalogued](../../../reference/rip-tools.md) RIPtel 3.1 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` (592,896 bytes; SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`). Addresses are RVAs from image base `0x10000000`. LLVM objdump 19.1.7 confirms the shared setter, Tab dispatch, focus draw/clear, traversal comparison and five-entry list loop.

## Tab setting and events

The [1.54 record](../../1.54/text/RIPScrip-1.54.txt), TABON and TABOFF, and [2.00 Alpha 4](../../2.0/text/RIPScrip-2.0-alpha-4.txt), §§5.8.11/16/17 (SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`), establish traversal of mouse fields and button fields and deselection on TABOFF. They do not define reverse order, event priority, geometry or mutation lifetime. The registration references at `0x26a8f`, `0x26aa6` and `0x26cea` point to TABOFF, TABON and OPTION. The zero-argument handlers at `0x307e1` and `0x30809` set the same `ripTabStatus` helper with 0 and 1. The config initializer loads 1 at `0x312e5` and stores CL at configuration +0x30b at `0x3131a`. The setter's local and callback routes clear current focus on disabling (`0x31ac4`, `0x31afe`). The getter uses selector -1 and returns the current state. Initialization of the driver fallback is the source for the portable session default.

- Normal `RIP_ProcessEvent` character-message dispatch `0x44a1–0x44a6` branches to `0x4fb6` for 0x102. This checks Tab status and positive shared field count. Character HT passes when Ctrl is clear, and the Shift test sets the reverse argument at `0x4ffc`. The handler consumes the navigation event even for a selected singleton. With focus, CR saves the index, clears the marker and calls the keyboard action dispatcher `0x2c424` from `0x5031`. Escape clears and consumes at `0x504b` without calling the action. Fallthrough later uses the separate HOTKEY check at `0x50df`. This is the common route for both field types, ahead of ordinary default-Enter, explicit hotkey and general input. [Earlier keyboard evidence](issue-118-button-keyboard-activation.md) was delivered in [PR #121](https://github.com/bbs-land/remote-imaging-protocol/pull/121).
- Mouse system is context +0x1e. Its +0x16 WORD is live count, +0x50 is focus-active, and +0x52 is focused index. Records are 0xa3 bytes. Traversal `0x45056–0x450d6` uses no other selection filter. No focus starts at zero for either direction. Forward increments and wraps at count. The reverse branch at `0x450a6` decrements, and `JG` at `0x450af` leaves only results greater than zero. Otherwise it sets index to count−1. With two records, native reverse from B returns B. With three, native reverse from B returns C. A selected singleton returns without switching.
- Focus draw `0x44fda` takes the current field rectangle at +0x10 and calls `InflateRect(4,4)`, `DrawFocusRect`, invalidation, and `InflateRect(-4,-4)`. The IAT uses `0x96698` and `0x966e0` for those two Win32 functions. Clear `0x45038` first clears +0x50, then erases the previous marker using the same draw helper. The native request establishes geometry. Independent rendering uses a device-space marker clipped to the screen, with raster details left to the client.

## Field order and clearing

1. The `1M` handler `0xcef8` clears at `0xcf3a` and appends using current count at `0xd0ba` before incrementing it at `0xd1d6`. The `1U` handler `0xa952` clears at `0xa996` and uses count at `0xad09` for a live button, then increments it at `0xb175`. Both clear before completion of field admission.
2. Selective `1k` clears at `0xc49e`, calls selector `0x12d63` at `0xc52a`, and reaches per-field remove `0x27eee` at `0x12edd`. Remove compacts following 0xa3-byte records at `0x27f48–0x27f6b`. The `1K` handler clears at `0xc549` and calls whole-field removal `0x27f8f` at `0xc55f`. The same removal helper is called when applying the MOUSE backup at `0x23285`, during drawing reset at `0x16287`, and during HARD at `0x166b8` and `0x168b7`. Whole-field removal itself is a field-table operation. Its call sites establish loss of the old resident definitions, without requiring any inference about native live indices after restoration. See [selective deletion](issue-120-selective-field-deletion.md) and [mouse restore](issue-136-grouped-mouse-backups.md).
3. A different command can also call the native clear helper during normal execution, for example style definition `1B` at `0xb38e` and copy blit `1g` at `0xb7db`. Focus clearing is broader than actual table mutation in the original. The adopted field lifecycle below defines when portable focus survives. Pointer capture and hover membership follow their [existing stable-instance contract](issue-113-mouse-gesture-identity.md).

## OPTION dispatch and list

`0x304fc` splits one argument from two. With one, a case-insensitive LIST match branches to `0x305c4`, otherwise the name goes to the query dispatcher. With two, the mode comparisons accept ON, OFF or QUERY. Lookup `0x304c5` uses exactly the five records starting at `0x7f4c8`, each a pair of little-endian preferred-address pointers. Keyword comparison `0x655e2 → 0x6f540` compares whole strings case-insensitively. The flags for invalid arguments and unrecognized names are stored to variable context +0x436. A two-argument LIST form cannot match an option row.

| Row RVA | Name    | Setter/getter RVA |
| ------- | ------- | ----------------- |
| 0x7f4c8 | DOORWAY | 0x3189f           |
| 0x7f4d0 | HOTKEY  | 0x31a10           |
| 0x7f4d8 | TAB     | 0x31a8b           |
| 0x7f4e0 | STATBAR | 0x31b37           |
| 0x7f4e8 | VT102   | 0x317f3           |

The list loop at `0x305e3–0x30645` appends names in this order, without reading the option states. Its comma string at `0x796a8` is inserted before entries after the first. The result storage terminator is outside the expansion. Query uses selector -1 at `0x3068a`; return pointers `0x7970c` and `0x79714` are ASCII `1` and `0`. Set uses selectors 1 and 0 at `0x306a4` and `0x306af` and expands to no text. The draft's older RIPterm Pro list is the advertised alphabetized example `DOORWAY,HOTKEY,LIST,STATBAR,TAB`; the original RIPtel 3.1 result is `DOORWAY,HOTKEY,TAB,STATBAR,VT102`.

## Older and portable behavior

The former reference pointed to SyncTERM's separate implementation. The catalogued clone at commit `564edbb0eb55cc84e6e213f0f3852b5d69055281`, `src/syncterm/ripper.c:8974–9011,19188–19210`, traverses its own eligible linked-list entries from first or last according to direction when there is no focus. Its predecessor step does wrap across index zero, and its focused ordinary-input fallthrough clears the marker and forwards the key. This is third-party behavior and does not establish TeleGrafix behavior or portable key handling.

The reference retains RIPtel's slot-zero start in both directions, but uses a complete cycle once focused. It ties focus to successful live mutations, input boundaries, and the applying MOUSE restore boundary. Rejected/no-op mutations, ordinary drawing, and ordinary keyboard or pointer delivery by themselves preserve focus. This keeps a selected field reachable after every cycle without coupling its focus to pointer capture, hover or current port. An accepted keyboard activation still snapshots the source and identity, clears focus and cancels the held gesture before running the action. TAB and HOTKEY are independent. Portable listing follows the original RIPtel order filtered to supported options, following the draft's feature-discovery purpose while preserving the original byte order for a full implementation. Unsupported names take the normal recognized-macro evaluation error. No original binary instructions, native probe, client or GUI were executed.

## Reproduction

```sh
dll=../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
sha256 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10044fda --stop-address=0x100450d7 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10004fb6 --stop-address=0x10005123 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x100304c5 --stop-address=0x100306ba "$dll"
llvm-objdump19 -d --no-show-raw-insn "$dll" | rg 'calll\s+0x10045038'
```

For raw reads, map the RVA through the PE section table. The file offset is RVA−0xc00 for these .text addresses and RVA−0x1a00 for the .data option table. Decode five pairs of 32-bit little-endian pointers and subtract `0x10000000` before following name or callback addresses. A direct near call's target is call site +5 plus its signed 32-bit displacement.

## Verification

Read-only checks verified both source hashes, 26 direct calls, three registration addresses, eight instruction signatures and three imported names. The exact option list and every one of the 32 supported-name subsets were checked. Decoded the published style, two 1M fields, the 1U CR binding and 1k selector, confirming that only A is deleted. Independently checked native and portable examples and both portable directions for all 128 positive field counts. Full repository lint, link checks (6,789 links before delivery metadata) and git diff checks passed. No original client or graphical probe was run.

## Resume

Publish the [traversal and focus contract](../ripscrip/4.2-mouse-fields-and-pointer.md#tab-navigation) and [option profile](../ripscrip/5.1-text-variables.md#option), then consolidate the actual PR head into fork stable. Refresh the old-to-new queue and review overlap from 22:39:08Z or earlier.

---

[Research backlog](issue-backlog.md) · [Tab navigation](../ripscrip/4.2-mouse-fields-and-pointer.md#tab-navigation) · [OPTION](../ripscrip/5.1-text-variables.md#option)

## Delivery

[PR #198](https://github.com/bbs-land/remote-imaging-protocol/pull/198) is open for upstream review. Specification commit **b7580dd28987363a8536a095db1993fb10893898**.

Fork stable normal-merged the actual head **741394e664db11df7fd10e87f9f78e1fca108a37** through **78af40f04da66012587b2f10f66899d95471350e**, with an identical tree. At **22:47:26Z**, overlapping **22:39:08Z**, the audit checked 98 open RealDeuce issues and 97 open PR heads, all ancestral. No new review, discussion or inline feedback appeared. The verified queue is empty.
