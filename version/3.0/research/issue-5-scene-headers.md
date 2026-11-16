# Issue #5: scene headers and wire entry points

[Research backlog](issue-backlog.md) · [Header reference](../ripscrip/1.1-protocol-and-command-hierarchy.md#scene-header-segments)

## Status and questions

Started 2026-11-16 for [issue #5](https://github.com/bbs-land/remote-imaging-protocol/issues/5), branch `issue-5-scene-headers`, starting from `996b1a3` (issue #4’s delivered branch, PR #30; itself dependent on PR #29). No resolution PR yet; investigation incomplete. Queue refresh confirms #5 is the oldest issue without a complete resolution PR.

Required: shipped header byte grammar, framing/termination, field widths/radices, reset/protection behavior and a complete example; identify or explicitly exclude RIP_TextMetric, RIP_FontAttrib, RIP_SwitchDirectory, SyncTERM-only 1N/1O/1S and level-0 ESC from the executable 3.0 contract; distinguish an audio table from a single headerable state object. Article selection’s wire-layout reconciliation remains with issue #2.

## Primary artifact and initial leads

Use the exact [issue #3 driver](issue-3-extended-font-style.md#artifact-recovery-2026-11-16), SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. All code addresses are RVAs relative to preferred image base 0x10000000. Artifacts remain under the external mirror. No GUI work is required for dispatch/state probes; any GUI research still requires mandatory isolation in AGENTS.md.

The existing reference carries the Alpha 4 header flag list forward and hypothesizes per-object segments from error strings. RIPlib supplies unverified leads: multiple `h` dispatch records share handler `0x1cae1`; FontAttrib may be `q`, handler `0x1c799`; SwitchDirectory may reach `0x46f66`. RIPlib’s own header implementation explicitly leaves selection between equal-length forms unresolved; do not adopt it as a contract. Verify record discrimination and primary consumer paths.

## Header framing and primary parser evidence

Header slot 32 has handler `0x1cae1`, widths `[2,4,2]`, flags 0x0012 (always base 64 and segmented). Slots 33..37 share its handler but have NUL opcode bytes; they are segment descriptors, not alternate h signatures selected by payload length. Their widths are `[1,2,1]`, `[1,4,1]`, `[1,4,1,1,1]`, `[1,2]`, `[1,2]`. This corrects the external implementation lead.

Parser `0x39e90` uses multi-record map RVA `0x81ccc`: root 32, followed by descriptor slots for types 0..9 `[33,33,34,34,33,35,36,36,36,37]`. After the base fields, flag 0x10 places the parser in state 4. A selector character 0..9 picks the corresponding descriptor and is reprocessed as its first numeric field (`0x3a223–0x3a3c4`). Fields append into the same argument array; there are no pipes between segments. Pipe/CR/LF/form-feed transitions to final dispatch. The count does not terminate byte parsing; final validator `0x1774f` checks declared segment count and decoded argument count. A following command separator thus terminates the whole header, not an individual segment. All header descriptors use base 64, independently of global radix.

Header fields are revision, global flags, segment count (not reserved). Revision accepts 0 or 1, flags 0..31. The old Alpha 4 flag list is not the shipped contract. Apply routine `0x1799e` first invokes the validator, then bit 1 performs hard reset (`0x16665(context,0)`), otherwise bit 2 performs drawing reset (`0x16220(context,0,1,1)`). It stores revision in viewport/environment-system +0x1a, releases prior input lock (`0x16b51`), then flag 4 locks input (`0x16b24`, cursor 6, timeout state), flag 8 selects slot zero for text-window/button/style/port/palette/environment tables, and flag 16 clears refresh state through `0x3e41a`. The exact downstream reset/protection semantics are still being traced.

Types and consumers: 0 button style `0x16bf4`; 1 graphics style `0x16d58`; 2 drawing port `0x16ec1`; 3 text window `0x170c8`; 4 palette `0x172c5`; 5 environment `0x173f5`; 6 mouse fields `0x17658`; 7 audio `0x176dd`; 8 screen `0x176ef`; 9 character set `0x17b7a`. Common table segments pass flags then slot; do not reverse these because the consumer pushes them in reverse order.

Segment 7 consists only of type plus two base-64 flag characters. Validation allows 0..1, and flag 1 calls `0x37e41(1)` to stop audio. There is no audio entry number or table selection in this segment. Segment 8 allows flags 0..15; segment 6 allows 0..31. Type 9’s descriptor supplies only two arguments while the validator’s stride table (`0x761b0`, bytes `03 03 03 03 03 05 02 02 02 03`) expects three: a lone wire type-9 segment fails exact argument-count validation. Preserve this shipped mismatch rather than inventing an extra reserved wire field.

Environment validation contains another shipped defect: for flag 0x400 (coordinate width), it checks segment argument +8 (environment slot) against 2..5, rather than +12 (coordinate width). For 0x1000 (RGB bits), it again checks the slot against 1..8 instead of the RGB field at +16. Original downstream setters still need boundary/effect probes. Native tests confirm slot 0 fails despite valid width/RGB, and an in-range slot passes the validator despite field value 63.

## Preliminary native parser probe

`run/riptel-header-probe <dll>` currently passes 74 preliminary checks against the original byte parser and validator, with only the client framing-policy callbacks intercepted and a capture callback replacing final dispatch. It verifies all ten segment selectors, mixed descriptors in one header, delimiter/count behavior, environment validation defects and missing opcode dispatch. No header state handler is yet executed in this probe; full effect/protection coverage remains required.

Mixed parse oracle: `!|h01000003001070150002028` plus CRLF produces argument array `[1,0,3,0,1,0,7,1,5,2,0,2,8]` and validates. It is revision 1, flags 0, three segments: button flags 1/slot 0; audio flag 1; environment flags 2/slot 0/coordinate 2/RGB 8. It is constructed from the primary parser grammar, not found in a corpus script.

Rejected probe setup: memset-to-zero parser state lacked the original constructor’s prior-CR bytes at +0x9f/+0xa0, so an initial exclamation was not accepted as a line-boundary introducer. Setting both to CR reproduces `0x39cf1–0x39cfb`; this fixed the harness without changing parser instructions. An early hand-concatenated mixed example used type 6 for environment, misframing its fields; it was replaced by the independently checked argument-array oracle above.

Factored shared preferred-base PE mapping and observation helpers into `run/lib/riptel-probe-image.h`. Font and column probe logic/hooks are unchanged; both pass after refactoring (31,713 and 23,013 checks). Header-only helper functions are inline where unused in another probe. The header includes its own system dependencies so clang-format’s include sorting does not break compilation.

## Recovered entry-point leads

Original byte-parser callbacks independently identify:

- FontAttrib: level-0 `q`, slot 55, width 2, current radix; handler `0x1c799`. Accepts 0..15, requires unprotected graphics style and extended font mode. It **ORs** bits into extended-style flags +0x53; zero does not clear flags. They are bold/italic/underline/strike-through per the complete #3 renderer trace, not shadow. Native effect tests still pending.
- TextMetric: level-0 `r`, slot 57, widths 1,1,4 and string tail; handler `0x20371`. Initial validation is mode <4, domain <2; complete semantics and outputs require tracing `0x20371–0x209c8`.
- SwitchDirectory: level-2 ESC, physical slot 110 with callback command index 121, width 4 and string tail, flags 0x000f/current radix; handler `0x46f66–0x47125`. A null host-service callback at context +0x5e suppresses it. Requires a nonempty string; scratch copy is trimmed/normalized, literal $OFF$ clears host directory before macro expansion, otherwise expands variables, checks length <=12, rejects characters in `\:*?+/",;><=[]|`, rejects special dot combinations, calls host-directory setter `0x6b36`, then `0x3f5ff`. Full normalization/path behavior and callbacks still need effect tests.
- The complete table has no level-1 N/O/S or level-0 ESC; the original parser emits no callback for test records with those opcodes. They may be classified outside this driver’s executable command contract after finishing the dispatch audit; do not import SyncTERM-only descriptors.

## Entry-point effect probes and client reply trace

The expanded native probe passes 8,315 preliminary checks. It now executes the complete original q/r/directory handlers with controlled host calls, identity macro expansion (or one explicit test replacement), synthetic primitive/font metrics and captured directory creation. q is exhaustively tested over 0..4095; it only accepts 0..15, ORs those bits, rejects system-font mode/protected style and preserves other flags.

TextMetric r’s third four-digit argument is consumed but not read. Modes 0/1 return whole-string width/height, modes 2/3 return per-byte width/height values separated by colons, all with one final CR. Domain 0 converts device values back to world units via the actual helper `0x310f2` (signed 16-bit value times world dimension divided by device dimension, truncated); domain 1 leaves device units. The handler copies/expands the input into retained `$TEXTDATA$` (at most 1023 bytes) or uses that buffer for the case-insensitive `$TEXTDATA$` sentinel. The original outline branch makes a zero-position/font-initialization call to the renderer, then queries metrics. Synthetic width 3 per byte, height 7, world/device ratio 2 yields for `ab`: domain-1 responses `6\r`, `7\r`, `3:3\r`, `7:7\r`, and domain-0 `12\r`, `14\r`, `6:6\r`, `14:14\r`. Both system and extended font paths are checked. Mode >=4 and domain >=2 reject before output. The metric call does not draw glyphs in this path; the controlled renderer call still needs a final argument-effect review.

Reply helper `0x6947` validates the instance then sends client window message 0x4c8 with the result string as lParam. To trace its wire consequence, unpacked a separate copy of original `riptel.exe` using UPX 5.2.1 (no execution):

```sh
upx -d -o artifacts/riptel-3.1-pcmicro/riptel-unpacked.exe artifacts/riptel-3.1-pcmicro/extracted/riptel.exe
```

Unpacked file: 605696 bytes, SHA-256 `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. Original EXE/hash remain those recorded in issue #3. EXE addresses here are RVAs relative to image base **0x400000**, not the DLL’s base. Window dispatcher `0x74ee` routes message 0x4c8 to `0x844e`; with a connection active it calls `0x63ee`, which places the string in the outgoing buffer. That function adds LF after CR when global 0x452528 is zero (`0x6421–0x6430`); therefore distinguish the engine’s CR-terminated response from client-configured line-ending translation. Its message send at `0x647c` requests connection output. Source may be reproduced with objdump or the external annotated disassembler after setting its IB to 0x400000.

Directory handler trims then uppercases literal input (`0x65424` -> `0x75820`), checks literal $OFF$ before expansion, expands other text, validates <=12 bytes, rejects the listed separators and exact `.` / `..`, stores the name with original `RIP_SetHostDirectory` (`0x6b36`), then calls `0x3f5ff`. The latter constructs install-root + backslash + host name and calls `0x6c430`, which wraps CreateDirectoryA. Creation failure is ignored and does not roll back the selected name. Expanded `$OFF$` is a literal directory name, not the clear sentinel. No host-service callback at context +0x5e means a silent no-op. The native probe executes the setter and path-construction routines unchanged, captures the creation call (always failing), and checks uppercase/trim/store/root confinement, invalid names, pre-expansion OFF behavior and callback gating.

A failed metric harness attempt lacked the active viewport-entry pointer used by the outline branch at `0x2078c`. GDB located the null dereference; supplying the original viewport structure shape fixed the harness. No GUI or original-client process was launched.

## Additional header-effect observations (still incomplete)

The audio segment’s flag 1 directly reaches PlaySoundA(NULL,NULL,0x20001) through `0x37e41`; it stops waveform playback. The argument to that wrapper is ignored. Do not describe it as an audio table or assume it stops independent MIDI playback without a separate trace.

Common table-segment backup calls are `0x23d6e(context,type,-1)` to delete base, `…,type,-2` to delete a numbered saved area, and `0x24648(context,type)` to pop/delete stack entries until failure. **Important shipped defect:** `0x23d6e(...,-2)` returns after the first successfully deleted unprotected numbered save slot; header flags invoke it only once. It does not clear every numbered save slot in one call. Verify this with a controlled backup-record probe before publishing the flag table.

Environment setter `0x3e989` silently rejects widths outside 2..5; `0x3e943` accepts RGB bit count 0..8, silently rejecting >8. These downstream checks differ from the earlier misindexed validator. Both write parser and current environment state directly without a protection check; the complete header/protection sequence still needs probing.

## Next action

Extend the native header probe to execute global and segment effect routines, recording underlying reset/switch/protection/backup calls and actual state changes. Trace the remaining table-segment functions and their backup/reset helpers; test environment setters and character-set mismatch. Then fully derive TextMetric outputs and SwitchDirectory behavior. Keep #5 incomplete and do not file a partial resolution PR.

---

[Research backlog](issue-backlog.md) · [Header reference](../ripscrip/1.1-protocol-and-command-hierarchy.md#scene-header-segments)
