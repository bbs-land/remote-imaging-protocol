# Issue #199 — Status-bar visibility and geometry

[Research backlog](issue-backlog.md) · [Status bar](../ripscrip/5.1-text-variables.md#statbar)

## Scope and checkpoint

[RealDeuce report #199](https://github.com/bbs-land/remote-imaging-protocol/issues/199), created 2026-11-23 22:57:18 UTC, body SHA-256 `fc9002e3d69dc6d3c7661b612fc6869e4a4b06a2d47bbe8fc806eac7759727cc`, zero comments at intake. The 23:00:09Z audit found only #199 uncovered and all 97 open PR heads ancestral. Started from fork stable `592402ea550e5b77073fb7eb83267eb3d924ee76`. Refine the 3.0 status option and presentation ownership while retaining the existing OPTION profile and SCREEN backup semantics.

## Primary evidence

- The [2.00 Alpha 4 record](../../2.0/text/RIPScrip-2.0-alpha-4.txt), §§5.8.11 and 5.8.13–15, defines the status bar switch and the YES/NO query. Its older save wording includes the status bar. The [existing RIPtel backup trace](issue-26-screen-backups.md#real-status-bar-owner-and-saveall-palette-phase) establishes the 3.0 backup boundary.
- RIPtel 3.1's PCMicro distribution provides the unchanged `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, driver 3.0.7, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. DLL addresses below are RVAs from `0x10000000`. Subtract `0xc00` to map the cited .text RVAs to file offsets and `0x1a00` for the .data table. [Installer source and mirror catalogue](../../../reference/rip-tools.md).
- The unchanged packed client `extracted/riptel.exe` has SHA-256 `648fc45524f9bdecb46499710d6e08fbc28958eabd6a36ec80110874699d08de`. Its existing separately unpacked copy `artifacts/riptel-3.1-pcmicro/riptel-unpacked.exe` has 605,696 bytes and SHA-256 `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. The executable addresses below are preferred VAs, using base `0x400000`. [UPX 5.2.1 provenance and prior static client inspection](issue-5-scene-headers.md#entry-point-effect-probes-and-client-reply-trace).
- The installed `dragon.rip` has 906 bytes and SHA-256 `fcb4b27e5adacffee9325f6d3bfaa63b461f3095830289a9f8f3ab98674d945b`. Its zero-based file offset 41 starts `!|1`, ESC, `0000$SBAROFF$` and CRLF. Its following `w0000000000` is a separate text-window command. The scene proves ordinary use of the directive. It does not independently determine the status row or any geometric effect.

## Argument, service and return trace

| Registered name | Registration call RVA | Handler RVA | Selector | Expansion |
| --- | --- | --- | --- | --- |
| SBAROFF | 0x269b4 | 0x3075a | 0 | Empty |
| SBARON | 0x269cb | 0x30782 | 1 | Empty |
| STATBAR | 0x2600c | 0x307aa | -1 | YES on nonzero, NO on zero |

All three handlers reject nonzero argument count before calling the shared service and set variable-context WORD +0x436. Normal calls from `0x30777`, `0x3079f` and `0x307c9` reach `ripStatbarStatus` at `0x31b37`. The query reads its result and returns the `NO` and `YES` strings at `0x7f244` and `0x7f248`. The generic [OPTION dispatcher](issue-197-tab-navigation.md#option-dispatch-and-list) has the STATBAR row at `0x7f4e0` and the same function pointer. Its query branch uses the numeric ASCII strings at `0x79714` and `0x7970c`, respectively.

The service loads the engine via instance +6 and its callback table via engine +4. At `0x31b50` it loads the callback pointer at +0x38, slot 14 of a four-byte-pointer table. When present, the call at `0x31b5b` receives the instance and selector. The return low byte goes to engine state +0x30c at `0x31b62`. With no callback, the branches at `0x31b6a–0x31b8e` store only this byte on set and return it for query. Initialization sets it to 1 at `0x31322`. The same initialization zero-fills 0x12c bytes of callback table starting at `0x3125e`. Registration at `0x3138b` stores the third argument at table base +4 times the second argument. The fallback touches no canvas or window geometry.

## Bounded host evidence

LLVM objdump 19.1.7 identifies the executable's import IAT VA `0x45b7d0` as `ripSetCallback`; thunk VA `0x41c096` jumps through that pointer. All 31 disassembled direct calls to this thunk occur from VA `0x408fe8` through `0x40925e`. Their slot argument is a literal push before the instance argument. In call order the slots are:

```text
6, 0, 8, 28, 24, 23, 29, 19, 20, 58, 43, 4, 61, 3, 33, 34,
35, 72, 71, 64, 65, 66, 67, 68, 69, 70, 13, 73, 45, 46, 47
```

The first of these comes after initialization at VA `0x408fc7`. Slot 14 is absent. This confirms the logical fallback for the examined normal registration sequence. It does not establish what appears on an original desktop, exhaust all dynamic paths or constrain a separate host's callback. No argument about resizing or backing pixels depends on reverse engineering undocumented UI events.

## Adopted portable behavior

Keep a session-wide option even when the frontend has no usable status indicator. An indicator that exists reflects the setting. Its visibility changes at the presentation layer while RIP DEVICE/WORLD, PORT0 pixels and bounds, and the active ANSI text window stay fixed. A shared-surface indicator can be composited over the canvas. Keep it out of backing pixels and exclude both overlay and option state from SCREEN and SAVEALL/RESTOREALL. This policy lets a terminal honor the inherited status directive without losing the host's screen geometry when the terminal shares canvas and chrome. It does not require a particular visual layout or imply that a native RIPtel screen was captured.

The published example uses the existing ordinary TW geometry and portable query rules. At matching 640×350 DEVICE/WORLD, `w0A0F0K0O12` selects inclusive cells 10..20 and 15..24 in font 2 (8×14), for bounds `[80,168) × [210,350)` and 11 columns by 10 rows. The sentinel independently measures restored backing pixels near a typical status row. It uses `X0M9P` at device (22,349) with origin zero and COPY mode; it does not depend on interpreting a screenshot of an overlaid indicator. Status queries preserve geometry and the screen snapshot restores the sentinel without changing the setting. Exact DEVICE PORT/TW query expectations describe the portable profile.

## Reproduction

```sh
mirror=../rip-tools/artifacts/riptel-3.1-pcmicro
dll="$mirror/extracted/ripscrip.dll"
exe="$mirror/riptel-unpacked.exe"
sha256 "$dll" "$exe"
llvm-objdump19 -p "$exe" | rg 'ripSetCallback'
llvm-objdump19 -d --no-show-raw-insn "$exe" | rg -B 4 'calll\s+0x41c096\b'
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1003075a --stop-address=0x100307e1 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10031b37 --stop-address=0x10031bb2 "$dll"
```

Read the PE section table for raw names and pointers. Test the call targets as call-site RVA +5 plus the signed `rel32` displacement. Decode the second argument from the literal push in each executable call sequence. The import descriptor maps each NUL-terminated name to its IAT slot. No original binary code, graphical probe or UI was run.

## Verification

Read-only checks verified four artifact hashes, three named DLL registrations and service calls, eight instruction signatures, the STATBAR option row, and all four result strings. The executable import and thunk were decoded from the PE file, and all 31 direct client registration sites and slot operands were checked. Independently decoded the setup bytes and checked both geometry replies and six option/pixel rows in the portable vector. The vector is for a live conforming implementation. The checks here were static and arithmetic, without running RIPtel or opening a graphical probe. Full repository lint, 6,817 link checks before delivery metadata, and git diff checks passed.

## Resume

Publish the [status-bar contract](../ripscrip/5.1-text-variables.md#statbar) and its literal vector, then merge the PR's actual source head into fork stable. Refresh the queue and feedback starting at 22:59:09Z or earlier.

---

[Research backlog](issue-backlog.md) · [Status bar](../ripscrip/5.1-text-variables.md#statbar)

## Delivery

[PR #202](https://github.com/bbs-land/remote-imaging-protocol/pull/202) is open for upstream review. Specification commit **dfbd9ba5c0f4dbd0baab7faccd4187abd7a73b9a**.
