# Issue #205 — Block-mode requests and completion

[Research backlog](issue-backlog.md) · [RIP_ENTER_BLOCK_MODE](../ripscrip/5.5-file-transfer-and-queries.md#rip_enter_block_mode)

## Scope and checkpoint

[RealDeuce report #205](https://github.com/bbs-land/remote-imaging-protocol/issues/205), created 2026-11-23 23:49:02 UTC, body SHA-256 `824dac9896a1ee2885b90cdd4afd2d097aa146063a9e920e729331ef2caad411`, zero comments on intake. The 2026-11-24 00:04:19Z GitHub audit found only #205 uncovered, with all 100 open PR heads ancestral. Started from fork stable `34515c5d1ad7d457344150aa4ae485f31f79454b`.

## Primary sources

- [2.00 Alpha 4, §3.4.5.1](../../2.0/text/RIPScrip-2.0-alpha-4.txt) defines direction, protocol names, type meanings, `<>` plus CR, and the historical timed refusal rule. Its receive table permits a nameless YMODEM batch, assigns Super Kermit to 9, and ends its file types at 5.
- The unchanged [catalogued PCMicro driver](../../../reference/rip-tools.md) is `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred base `0x10000000`. Driver addresses below are RVAs; .text is at file offset RVA−0xc00 and the dispatch table is in .data.
- The matching [already unpacked client](issue-5-scene-headers.md#entry-point-effect-probes-and-client-reply-trace) `riptel-unpacked.exe` is 605,696 bytes, SHA-256 `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`, preferred base `0x400000`. Client addresses below are VAs. LLVM objdump 19.1.7 and read-only PE data were used for both artifacts. No connection, native code or client GUI was run.

## Driver request boundary

1. Physical slot 124, at RVA `0x81b80` in the 40-byte table beginning at `0x80820`, names level 9 Escape, points to `0x24b4e`, and carries five numeric widths `1,1,2,2,2` followed by the filename tail. This consumes the draft’s four reserved digits as two fields. The handler reads offsets 0 through 12 of the decoded array; the fifth field is unused.
2. After the general processing gate, unsigned mode >1, protocol =9 or >10, and type >6 branch to the invalid-parameter path `0x25157`. At `0x24bdb–0x24c0e` the handler copies the tail, uppercases ASCII, applies the existing HCL evaluator and searches the prepared tail for the `<>` delimiter. For a well-formed prepared tail with the delimiter at the start, receive with code <6 takes `0x24c36`, reports “Invalid Filename string”, and posts nothing. The send path has no corresponding nonempty check. It terminates the prepared buffer at the delimiter. No behavior for a missing required delimiter is inferred.
3. The send switch uses table `0x25173`, and receive uses `0x2519f`. Both have one target per code 0–10 even though the common precheck rules out 9. Their valid branches store the protocol at options +0x100, copy the prepared filename text to the engine buffer, and call PostMessageA at `0x25143`. The command posts `0x4ca` for send or `0x4c9` for receive, with the protocol code as wParam. lParam consists of `type | (first_reserved << 16)` after taking the low 16 bits of both fields. Neither mode's transport switch varies by type. The exports `0x6984` and `0x69ab` expose the stored filename buffer and protocol to the client.
4. The [separate XFER source and probes](issue-6-macro-evaluation.md#xfer-confirmed) reach the same handler at `0x29612`. The macro builds an empty filename tail for receive codes 5–8, including YMODEM even if a filename argument was supplied. Its acceptance is therefore not shared-handler acceptance. The previous keyword map, including the code-8 name, is retained.

## Connected client dispatch

The first-level message dispatch at VA `0x4074fa` and `0x407506` selects receive and send. Both handlers first exclude a disconnected socket. Receive uses the `RIP_GetBlockModeTransferType` thunk at `0x41c072`, then 11 pointers at `0x408ab4`.

| Code | Target VA | Result after ordinary connected dispatch |
| --- | --- | --- |
| 0–6 | 0x408499, 0x4084c3, 0x4084ed, 0x408517, 0x408541, 0x40856b, 0x408595 | Each reads the filename and formats a local request string using the corresponding protocol label, then returns. No transfer engine call. |
| 7 | 0x4085bf | With socket handoff successful, calls the receive wrapper at 0x40867b |
| 8 | 0x408725 | With socket handoff successful, calls that wrapper at 0x4087e7 |
| 9 | 0x408897 | Formats the Super_Kermit label and returns. The original driver gate prevents this request. |
| 10 | 0x4088c1 | Formats a local request string labeled Binary_Encoding (0x43f030) and returns. No receive engine call. |

Both active receive branches get the host directory and the DLL install path through `0x41c06c` and `0x41c066`, construct the connected resource path with `0x4057d9`, and pass it to `0x4203e0`. The driver's host-directory getter uses `ICONS` if none is selected. Both call `0x424020` with the same two receiver arguments `rz` and `-abeuy` in the table at `0x44c2d8`. Neither loads the requested filename for these paths. The standard receive wrapper uses the transfer's own filenames and the resolved resource directory. Thus names on the control command and verbatim crash-recovery labels do not imply different active receive dispatch.

The send path at `0x4088eb` sets up its connection state and checks wParam against 7 at `0x408985`. Only code 7 calls `0x42031f` at `0x408991`, which enters the `sz` sender at `0x420361`. This shows dispatch into the sender, not successful upload, actual selection UI, a confirmed authorization prompt or guaranteed refusal bytes. Other codes return following the connection housekeeping and do not start another transfer engine. The compatible specification requires authorization and a filename through the supported upload facility. Do not reproduce or infer further client failure paths from the historical message format.

## Completed-file routing

`0x4203e0` sets the transfer flag word from the high half of its request and empties the per-transfer list at entry. It calls the receiver at `0x420470` and examines the list at `0x42051b` after return. The normal asset receive path constructs each path from the destination directory and received name. This is distinct from the user-transfer sentinel `0xffff`, which takes the separate branch at `0x42048b`.

- First the client queries `RIP_IsFileArchiveFile` at `0x420589`. A recognized archive calls `RIP_DecompressArchive` at `0x4205c5` with the received path and two option tests, then advances to the next list node.
- For non-archives, types 3 and 5 run the JFIF test at `0x4205e5`. It opens the received file and reads exactly 11 bytes at `0x42076e`, checking `FF D8 FF E0` at offsets 0–3 and `JFIF\0` at 6–10. A hit calls `RIP_ShowImageWithCurrentStyle` for the received name. Merely giving a filename a JPEG extension does not select this branch.
- Type 0 calls `RIP_PlaybackLocalRIPFile` at `0x4206e3` for a non-archive without an extension check. Type 5 reaches that same playback route if the image check did not select presentation and the path ends in `.RIP` under the case-folded suffix test at `0x420618–0x420679`.
- Remaining non-archives use the common legacy-icon tests at `0x42067b–0x4206bf`, independent of the type. The constants are `.ICN`, `.HIC`, `.MSK`; the converter import call at `0x4206cf` is already identified in the [legacy icon trace](issue-19-legacy-icon-conversion.md#completed-client-and-playback-trace). Type 6 has no distinct case. No GIF path is implied by the active-dynamic label.
- For these non-archive files, the wrapper can register the completed name for later purge at `0x4206fe` if the first reserved word has bit value 1. It does not purge the file on return. Archive handling has its own service and bypasses this shared tail. The independent [image-style and temporary-file derivation](issue-21-image-style.md#received-file-cleanup-and-encoded-stream-checkpoint) explains why this is unrelated to an image-style DELETE flag.

## Completion, failure and cancellation

The ZMODEM file path writes data before the separate EOF dispatch. Frame 11 goes through `0x426f5b` via the tables at `0x427348` and `0x427328`. It compares the returned offset to the current offset before entering the close/record path `0x4272c3–0x42731d`. The wrapper has no RIP playback or image calls while the receive engine is active. File-name recording itself is a list append at `0x4203c7`, and processing happens after the engine returns.

The wrapper does not gate that subsequent list walk on a single whole-batch success result. The receive failure cleanup at `0x4248ba–0x4249bf` closes the current streams and can attempt `DeleteFileA` via `0x4249b1 → 0x437030 → 0x437000`. This normal cleanup trace does not promise that every interrupted path removes every partial file or that a recorded name by itself proves a successful file. Previously complete files can still be in the list. The portable rule uses per-file completion as the commit point and allows private resume staging only. Partial data never publishes a resource or triggers a content action in that profile.

The receiver's failure path at `0x42436d` uses the NUL-terminated abort string at `0x44c890`, ten CAN bytes followed by ten backspaces. The inspected loop can stop early on failed output and has no per-byte delay for those bytes. This is not an observed denied-upload reply or a measured network capture. The Alpha 4 rule and the RIPSCRIP.HLP confirmation string remain historical evidence, independent of that receive-abort path.

## Reproduction

```sh
mirror=../rip-tools/artifacts/riptel-3.1-pcmicro
dll="$mirror/extracted/ripscrip.dll"
exe="$mirror/riptel-unpacked.exe"
sha256 "$dll" "$exe"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10024b4e --stop-address=0x10025173 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x40846b --stop-address=0x408a30 "$exe"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x4203e0 --stop-address=0x420745 "$exe"
```

Map each RVA through its PE section table and read the numeric-switch pointers and strings separately. The posted-request examples stop before any native host service or transport operation.

## Verification

Verified both original artifact hashes, the five-width dispatch record, three jump tables, four control-flow signatures, the original receiver arguments and cancellation string, one XFER call and 14 client calls. Independently decoded seven request lines and checked 352 driver/client combinations across direction, protocol, type and empty/nonempty names. The publication and failure boundaries were reviewed statically. Link, formatting and diff checks passed. No native transfer, client or graphical probe was run.

## Resume

Verify the matrix and literal request bytes, publish the PR, then consolidate its source head into fork stable. The 01:50:57Z refresh found #205 through #212 uncovered in order, with no competing PR heads or new feedback. Continue with #206 after delivery.

---

[Research backlog](issue-backlog.md) · [RIP_ENTER_BLOCK_MODE](../ripscrip/5.5-file-transfer-and-queries.md#rip_enter_block_mode)

## Delivery

[PR #216](https://github.com/bbs-land/remote-imaging-protocol/pull/216) is open for upstream review. Specification commit c1e2b71b199917244512ab7ea67596729678f6ae.
