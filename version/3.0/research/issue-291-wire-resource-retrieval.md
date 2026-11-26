# Issue #291 — Level 9 resource retrieval

[Issue #291](https://github.com/bbs-land/remote-imaging-protocol/issues/291) · [Resource retrieval](../ripscrip/5.5-file-transfer-and-queries.md#resource-retrieval-9d) · [Research backlog](issue-backlog.md)

RealDeuce filed #291 at 07:20:48 UTC on 2026-11-25. The 07:25 UTC audit found it, and GitHub returned no comments at intake. Branch `issue-291-wire-resource-retrieval` starts at fork stable `9b362bf563a8b0d15ba669ca3f6d210641745e90`.

The [PC Micro package](../../../reference/rip-tools.md) supplies the original `extracted/ripscrip.dll` with SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8` and `extracted/riptel.exe` with SHA-256 `648fc45524f9bdecb46499710d6e08fbc28958eabd6a36ec80110874699d08de`. The previously unpacked matching `riptel-unpacked.exe` SHA-256 is `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. DLL addresses below are RVAs at base `0x10000000`. Client instructions use VAs at base `0x400000`. The checks are read-only; they use the report, original instructions and existing lookup/transfer derivations.

## Framing and name

Physical descriptor 125 at `0x81ba8` is level 9 `D`, handler `0x24af4`, with count 1, width 4 and flags `0x000f`. Low bits `11` select active Base Math. Bit `0x08` marks the last numeric field reserved, and `0x04` selects a text tail. The collector at `0x3a8ae–0x3a905` counts valid digits, stores zero and switches into text collection at completion. A leading period takes the whole-field zero path. The delimiter checks an unfinished prefix at `0x3a09a–0x3a0a1` before the trailing-reserve omission branches. The filename therefore begins after four valid digits or one period. [Existing reserve and text collector derivation](issue-245-image-reserved-fields.md).

The consumer copies the received text at `0x24b07` and checks for a nonempty filename tail through `0x3f71a` at `0x24b16`. It passes its filename buffer to `0x3f80e(context, name, 0, 1)` at `0x24b35`. Neither the numeric operand nor a macro expander is read or called. The resolver prepares the name using the common ASCII filename-tail helper and checks the host, shared ICONS and configured search directories in order. The 9D call path adds no alias conversion or extension. The portable profile uses a bare name in that namespace and bounds the tail at 255 bytes, leaving space for the private terminator in the handler's 256-byte name buffer. [Filename helper](issue-168-file-query-replies.md#complete-source-ledger) · [Local resource resolver](issue-171-fileexists.md).

## Callback results and resumption

The first local hit returns 0 before the retrieval branch. A miss reaches `0x3f9ac–0x3f9d5`, which checks the caller permission, options +0x344 inhibit and a comparison with the preceding lookup name. The preceding name comes from file state +0x5b2 and is updated at `0x3f891–0x3f8b6`. The original equal-name comparison suppresses that retrieval attempt. `0x3f9ed` calls the wrapper with context, an output pathname buffer initialized to the prepared destination candidate, the request name and mode 0. At `0x3f7ef–0x3f808`, an absent options +0x10c pointer returns 1. An installed callback supplies its result. On result 0 the resolver copies the returned pathname back at `0x3fa00–0x3fa06`.

9D tests only result 2 at `0x24b3d–0x24b45`, then calls `RIP_Suspend`. The export stores 1 at options +0x348. `RIP_ProcessBuffer` checks this before each byte at `0x5800–0x580a`; it returns the consumed count at `0x58ac`. The triggering delimiter is counted by the normal loop before the suspension check for the next byte. The unconsumed suffix remains with the caller. `RIP_Resume` at `0x6c30` clears that flag. The portable completion puts responsibility for releasing a pending request on the frontend on every terminal outcome. It resumes the saved stream suffix in order and completes the resource request once. See the distinct [explicit block-mode path](issue-205-block-mode.md).

## Standalone RIPtel client

Instance initialization calls `0x3e86a` at `0x5fdc`. It allocates and zeroes the options object at context +0x36, including the callback pointer at options +0x10c. Export `ripSetRetrieveFn` (ordinal 140, RVA `0x4276`) writes that pointer at `0x429e`. The connected EXE registers its ordinary client callbacks through `ripCallbackInit` and `ripSetCallback`, which use context +6 rather than this options field.

The complete unpacked EXE import table has no `ripSetRetrieveFn` name or ordinal, and no `RIP_Resume` import. The static call sites using the `GetProcAddress` IAT entry at VA `0x45b72c` resolve affinity/process/thread functions from KERNEL32 at `0x432118–0x432178` and MessageBoxA/GetActiveWindow/GetLastActivePopup from user32 at `0x434fee–0x43501a`. The RIPSCRIP loading path uses the imported driver-version function. These checks cover the apparent dynamic export-lookup alternative. The callback remains absent in the recovered standalone client. No standalone 9D remote request, transfer or completion reply follows a local miss. An embedding can provide its own transport with its optional callback.

```sh
sha256sum "$dll" "$exe"
run/riptel-inspect "$dll" --slot 125
objdump -d --start-address=0x10024af4 --stop-address=0x10024b4e "$dll"
objdump -d --start-address=0x1003f7e5 --stop-address=0x1003fa1e "$dll"
objdump -d --start-address=0x100057a2 --stop-address=0x100058e8 "$dll"
objdump -p "$exe"
objdump -d --start-address=0x4320fd --stop-address=0x432188 "$exe"
objdump -d --start-address=0x434fd0 --stop-address=0x43501f "$exe"
```
