# Issue #179 - TXT playback in an active article

[Backlog](issue-backlog.md) · [Column text](../ripscrip/3.1-column-text-system.md#flowing-whole-files---1r-on-plain-text) · [Scene input](../ripscrip/5.5-file-transfer-and-queries.md#file-boundaries-and-nested-scenes) · [Issue #179](https://github.com/bbs-land/remote-imaging-protocol/issues/179)

RealDeuce filed the report at **2026-11-23T16:59:59Z**. The body SHA-256 is `ab5efc99014d08b2282b3d45e0ab5b1d38d9eb38fbc5ea688fd5c962788d61fe`. The pinned problem is a local `STORY.TXT` with an active article and the bytes `21 7c 63 30 34 0d 0a 41`, followed by article end and a point. The corpus’s ordinary prose files cannot distinguish direct prose from text that has passed through RIP framing. The report identifies a differing SyncTERM rule. Work starts on `issue-179-txt-playback` from fork stable **367e11a32f60e9d90494f81ba3dc28d862d54c4c** after #175.

## Original source

Use the unmodified RIPtel 3.1 PC Micro `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` from the [external catalogue](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The addresses are RVAs. The preferred image base is `0x10000000`. The [local scene boundaries trace](issue-170-local-scene-boundaries.md) establishes resolver and text reader semantics. The [framing trace](issue-7-framing-identification.md) establishes the normal introducer and delimiter policy. The [column trace](issue-4-column-overflow.md) establishes the nonzero article and raw-flow gates.

With `DLL` set to the artifact, LLVM 19.1.7 reads the source and destination addresses of the calls. Use image-base-plus-RVA for the disassembler's ranges.

```sh
sha256 -q "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10002b1e --stop-address=0x10003079 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1000324a --stop-address=0x10003372 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1004d2d0 --stop-address=0x1004d338 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1003ce24 --stop-address=0x1003ce70 "$DLL"
```

## Route and decision

1. The wire handler at `0xd64d` calls resolver `0x324a` at `0xd6c4`. It checks that RIP graphics processing is enabled. The HCL scene handler at `0x25971` calls the same resolver at `0x259aa`. The caller flag is passed to resource lookup, and the successful lookup for either caller goes through `0x3344` to `0x2b1e` with identical playback flags. The resolver tests for a dot when adding the default suffix. Neither it nor the successful file-open path selects a decoder by extension.
2. Local engine `0x2b7a` calls `0x31420(context,-1)`. The helper returns the effective RIP-enabled setting at `0x31476`. The engine stores the value in `[ebp-0x2c]` at `0x2b89` and tests that same slot at `0x2ed6`. Playback therefore decides whether to invoke RIP framing from the enable state at entry. The path at `0x2cf0` clears the parser and at `0x2d1d–0x2d23` sets its previous-byte fields to CR. The CRT read at `0x2dfc` supplies normalized text bytes. Neither the byte reader nor the parser gate tests the article state, the filename suffix, or the file contents.
3. The byte loop skips NUL at `0x2ed0–0x2ed4`. With RIP enabled, `0x2ee0` calls `0x39e90` on the next byte. A zero result routes the current ordinary byte through `0x2f30` to `0x4d2d0`. Result 1 first routes the parser's saved byte at `0x2f03`, then the current byte. If the current ordinary byte is LF, `0x2f24` supplies CR before it. Other results skip that text path at `0x2f12–0x2f1b`. With RIP disabled at entry, the direct ordinary-text branch at `0x2f1d` bypasses the RIP parser and uses the same CR-before-LF and text path. Wire `1R` itself cannot initiate playback with processing disabled. The bypass applies to a file opened through an otherwise permitted route.
4. The terminal-text handler at `0x4d2e9` invokes the raw article feeder at `0x3ce24` before ordinary ANSI handling. The gate at `0x3ce32–0x3ce50` checks the active-text state and positive article number along with its existing enclosing-state check. When the feeder accepts the byte, the handler takes its early return after the echo/bookkeeping call at `0x4d2f8`. There is no path from that check back to RIP byte classification. Geometry and overflow affect accepted prose but cannot turn the command bytes into prose.
5. The normalized input is `!|c04\nA`. At the fresh line boundary, `!|` opens RIP command parsing. The LF dispatches descriptor 22 (`c`, one two-byte numeric argument, callback `0x1abf0`) and is consumed as part of the RIP line. The successful color path stores the new drawing color at `0x1acc7`. Then `A` reaches the open article as ordinary text. EOF preserves the completed color effect. Caller `1E` reaches `0x3d488` via `0xbdda` and `0x3bf4b`, flushing and deactivating article flow. A subsequent `X` samples the live draw color, index 4. In the default mapped palette it is red. Use ordinary unprotected drawing state, no search highlighting, and an in-view point, so the point witnesses command execution independently of the rendered article.

The source supports the same framing and article routing for `.TXT`, `.FN`, `.RIP` and other explicitly named scene files through either entry point. A name suffix identifies the resource only. Normal framing can classify certain apparent introducers as ordinary text under its byte and line rules. An active article does not select a literal input mode. This conclusion concerns the protocol and the source routing; the original client and downstream regression test were not run.

## Verification

The DLL fingerprint, five opcode descriptors, 15 relative call targets and eight gate and state byte sequences match the trace. A separate model checked five fixtures for framed and bypassed input, article selection, plain prose and an unfinished command at EOF. The documented caller bytes decode to article 1, column 0 and rectangle (15,130)–(147,160) with overflow and highlighting disabled. Formatting and link checks passed.

## Resume

The specification and both playback summaries now describe the shared route. Validate the byte example and submit a PR closing #179. Consolidate its actual head and refresh the queue.

## Delivery

[PR #183](https://github.com/bbs-land/remote-imaging-protocol/pull/183) targets upstream main from the RealDeuce issue branch. Specification commit **e10d9bdd8df677b147dcc35ff449e5b2da25fbf8**. Lint, diff checks and 6,625 Markdown links passed.

Fork stable normal-merged the actual source **54e8d10f86c12b715cbd88ccd1c3472a1fa293fd** through **75cd7bc21eacd7034f59151cf7ca24643e8901cc**, with an identical tree. The audit at **17:19:47Z**, overlapping **17:15:08Z**, verified 90 open PR heads as ancestors and 91 open RealDeuce issues. All open RealDeuce issues have a resolution PR. No new review, issue discussion or inline feedback appeared during that audit.
