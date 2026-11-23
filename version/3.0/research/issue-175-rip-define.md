# Issue #175 - RIP_DEFINE grammar and editor policy

[Backlog](issue-backlog.md) · [Specification](../ripscrip/5.5-file-transfer-and-queries.md#rip_define) · [Issue #175](https://github.com/bbs-land/remote-imaging-protocol/issues/175)

RealDeuce filed the report at **2026-11-23T16:32:59Z**. The body SHA-256 is `b6e9ef09d4112b741178e56a552fd6d860282e9932ff687402e7744ac9197213`. It describes controlled editor, lookup and writer calls. The pinned [wire entry](https://github.com/RealDeuce/remote-imaging-protocol/blob/2900e843f911b05183620c110e43f381d90abc38/version/3.0/ripscrip/5.5-file-transfer-and-queries.md#rip_define) and [variable summary](https://github.com/RealDeuce/remote-imaging-protocol/blob/2900e843f911b05183620c110e43f381d90abc38/version/3.0/ripscrip/5.1-text-variables.md#rip_define-in-3x) inherit the ALPHA 4 and SyncTERM descriptions. Work starts on `issue-175-rip-define` from fork stable **8197107797e11618ad0f4b2759ad49c84da48443**.

## Artifact and method

Use the unmodified RIPtel 3.1 PC Micro `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` from the [external catalogue](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses below are PE RVAs. The preferred image base is `0x10000000`. With LLVM 19.1.7 and `DLL` set to the artifact path, the following slices begin at function entry points.

```sh
sha256 -q "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1003f1c9 --stop-address=0x1003f620 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1000e7a8 --stop-address=0x1000ea05 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10018698 --stop-address=0x10018a98 "$DLL"
llvm-objdump19 -p "$DLL"
```

Translate an RVA within a PE section to `PointerToRawData + RVA - VirtualAddress` to inspect data bytes. Start the descriptor walk at `0x80820` with 40-byte rows. Follow the PE import directory and compare `FirstThunk + 4 * index` to the dialog handler's indirect call addresses. Follow the resource directory `RT_DIALOG` (5), then IDs 600–603 and language 1033. Walk the little-endian dialog template with DWORD-aligned control entries and UTF-16LE strings. The template data RVAs are `0x989cc`, `0x98a44`, `0x98930` and `0x98b38` respectively. Static inspection corroborates the controlled report and extends it to the checkbox resources and both store paths.

## Wire and value selection

- The parser at `0x39e90` dispatches descriptor 91 (level 1, `D`) to `0xbd39`. The descriptor specifies numeric widths 3 and 2 followed by text. The handler checks the RIP enable gate, rejects missing text and calls `0x3f1c9` with a writable copy. It frees the copy after dispatch. Only the first numeric value reaches definition processing.
- `0x3f1d9` splits the first colon, and `0x3f1f2` splits the first comma in its head. A comma demands an immediate ASCII decimal digit. `0x3f238` calls the decimal conversion at `0x68cc0` and `0x3f244` retains the low word. `0x3f38b–0x3f3ab` requires a nonempty name and raw definition. `0x15fdc` gives the identifier syntax `[A-Za-z][A-Za-z0-9_]{0,18}` when combined with that check. `0x3f3b1–0x3f3c4` makes widths outside 1–60 use 60.
- `0x3f5c2` splits a leading question at its next question mark. With no second question mark, there is no supplied default. Without a leading question mark, the body supplies a literal default. An empty raw body fails before this split. No macro expansion occurs in this path.
- At `0x3f444` the parsed default is terminated at index width − 1 before the call to `0xe99c` from `0x3f44c`. At `0x3f4a5–0x3f4bc`, an empty clipped default falls back to the existing record value, copying at most 60 bytes into a zeroed 61-byte buffer. That fallback is not clipped to width − 1 again before a silent write. The interactive readback is subject to its own width bound. These are `1D` limits, independent of generic macro storage limits.
- `0x3f385` initializes the confirmation word to zero. `0x3f458–0x3f46f` assigns zero for a found record with bit 0 of the word at `+0x7a` set, and one when it is clear. `0x3f4bf–0x3f4cb` skips the editor only with flag `004` and a zero confirmation word. Thus a new name can be written silently and a protected name can still prompt. `0x3f576` selects the persistent writer with flag `001`. Flag `002` only reaches the editor's acceptance check.

## Editor and store

| Resource ID | Empty input | Confirmation control | Accept and cancel buttons |
| ----------- | ----------- | -------------------- | ------------------------- |
| 600         | Rejected    | Absent               | Close                     |
| 601         | Rejected    | Present              | Close                     |
| 602         | Allowed     | Absent               | OK, Cancel                |
| 603         | Allowed     | Present              | OK, Cancel                |

`0x3f54f` calls `0x18698` with the width, selected value, confirmation word pointer, caption and no-empty flag. `0x186f0–0x1873a` selects the resource using the no-empty flag and the client option for the protection control. Dialog control 112 has style `0x50010003` (`BS_AUTOCHECKBOX`) and text `&Prevent unauthorized access to this information` in both resources that include it. The generic caption text at `0x8212c` is `Please Enter Information for "%s"`. For a nonempty custom caption, `0x3f516` appends the question mark literal at `0x82128`.

The initialization branch at `0x189c4–0x189e3` calls the USER32 import `CheckDlgButton` at IAT RVA `0x966b4` with control 112 and literal argument 8 when the option and incoming confirmation word are nonzero. The call records the linkage between the incoming protection word and the checkbox. The static evidence does not establish the displayed check state produced by that literal on Windows. A portable editor initializes its equivalent control to the incoming protection state. On acceptance, `0x18a1f` calls `GetDlgItemTextA` (IAT `0x966ac`) with width as the buffer length including NUL. `0x18a2c–0x18a35` rejects an empty buffer when required. `0x18a54–0x18a5a` uses `IsDlgButtonChecked` (IAT `0x966b0`) to write the returned word back when the control option is enabled. With the option off, the incoming word is retained.

`0xe99c` tries a case-insensitive live lookup (`0xe962`) before looking for an uppercased persistent key (`0x147e5`). The persistent read at `0x14875–0x14881` reads the complete 256-byte record into the caller's output, including the flags. `0xe802–0xe814` sets the live word at `+0x7a` to 1 for approval-free updates and clears it for confirmation-required updates. The persistent writer at `0xe897` constructs the same word at `0xe8ec–0xe8f8`, then inserts or updates the record. The database write path at `0x14b76–0x14b89` passes all `0x100` record bytes. The persistent writer refreshes the live entry at `0xe953` after a successful backing-store write. The live-only writer leaves the persistent record available for a later session. This establishes persistence of the confirmation choice without requiring a particular database layout.

`0x3f557–0x3f55d` clears the result when the editor returns a status other than 1 and continues to storage. That observed result differs from the portable cancellation rule. A canceled edit leaves both records and their confirmation metadata as they were. An accepted empty value is a real value. Use this distinction in clients with any equivalent editor, including an editor with a cancellation affordance when nonempty input is required. No Windows dialog was run and no database layout or platform-specific handling of the initialization argument was inferred.

## Resume

The static trace covers the reported parsing, prompt and storage behavior. Apply it to the wire entry and the variable summaries, then check literal examples and the documentation. Open one PR closing #175 and consolidate its actual head into fork stable before starting #179.
