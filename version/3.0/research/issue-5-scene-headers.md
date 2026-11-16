# Issue #5: scene headers and wire entry points

[Research backlog](issue-backlog.md) · [Header reference](../ripscrip/1.1-protocol-and-command-hierarchy.md#scene-header-segments)

## Status and questions

Started 2026-11-16 for [issue #5](https://github.com/bbs-land/remote-imaging-protocol/issues/5), branch `issue-5-scene-headers`, starting from `996b1a3` (issue #4’s delivered branch, PR #30; itself dependent on PR #29). Delivered in [PR #31](https://github.com/bbs-land/remote-imaging-protocol/pull/31), with complete specification, reproducible native probes and verification. It depends on unmerged PR #30 and provides an issue-only comparison from `996b1a3`. The incremental observations below preserve the investigation history; the final findings supersede earlier pending items. Queue refresh after delivery selects #6 as the next issue without a complete resolution PR.

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

## Final header effects and protection findings

The expanded native probe now executes the original global header handler and each segment consumer. Leaf reset/switch/delete/UI routines are explicitly intercepted to record calls, while environment setters, style default copying, global lock/revision storage, original byte parsing and a separate backup-deletion helper run unchanged. This is controlled routine execution, not end-to-end client behavior. No Wine or desktop process is involved. The published flag tables come from the full consumer instruction ranges plus their downstream helper traces, not inferred bit names.

Global order: validator `0x1774f`; hard reset `0x16665(context,0)` when bit 1, otherwise drawing reset `0x16220(context,0,1,1)` when bit 2; revision stored at environment-system +0x1a; previous input lock released by `0x16b51`; bit 4 calls `0x16b24` (wait cursor 6, lock +0x1c=1, timeout state +0x1e=0); bit 8 selects zero in all six tables; bit 16 calls refresh-clear `0x3e41a`; then segments in wire order. The wrapper `0x1cae1` skips application when driver state +0x104 is nonzero and marks metrics dirty through `0x3c37e` even after failed validation. It has no blanket drawing-port protection guard. `#` releases input lock. Zero flags still remember revision and release an existing lock. No new cursor is forced when there was no prior lock.

Hard reset’s `0x16665` sequence clears refresh/audio state, removes table and saved-area protection, deletes backup areas/stacks and rebuilds table defaults. The drawing-reset path preserves the usual protected table entries; individual segment routines retain their own protection exceptions below. Do not treat a segment selection as an automatic reset of the selected entry.

| Segment | Consumer / key downstream routines | Derived behavior |
| --- | --- | --- |
| Button 0 | `0x16bf4`; delete `0x4558d`, protect `0x454c4`, switch `0x4576f` | Flags 1/2/4 delete unprotected nonzero/select supplied/delete others preserving current; 8/16/32 base/one-numbered/stack backup deletion; 64 current unprotected defaults; 128 all unprotected defaults |
| Graphics style 1 | `0x16d58`; delete `0x391fd`, protect `0x390e4`, switch `0x394fe` | Same common low bits and current/all default split; actual default record copying is checked with protected and unprotected current style |
| Port 2 | `0x16ec1`; delete `0x33112`, screen geometry `0x33d82`, switch `0x3393c` | Additional flag 4 deletes current physical target; common delete-others and backup bits shift left one position. Viewport helpers `0x33f55`, `0x34067`, `0x340bd` implement disable/enable, full-bounds rectangle reset and clipping application; query helpers `0x33e20`/`0x33e40` clear global/per-port queries |
| Text window 3 | `0x170c8`; delete `0x39ac8`, reset geometry/cursor `0x83a5`, switch `0xe0dc` | Common low six bits; active-state helper `0x2bf9e` toggles bit 0; erase/home `0x4597b`; query helpers `0x2d663`/`0x2d677`; cursor helper `0x18e33` toggles cursor bit 1 and `0x18bf7` handles active/visible caret state |
| Palette 4 | `0x172c5`; defaults `0x3e2e7` | Common low six bits. Both 64 and 128 reset only the currently selected defined unprotected palette: the 64 branch loops but checks loop index against current before acting. Native capture sees exactly one reset |
| Environment 5 | `0x173f5`; base `0x3e8eb`, coordinate width `0x3e989`, RGB `0x3e943` | Common low six bits. Flag 64 scans defined unprotected environments and restores selection; 128 directly defaults current, including protected current. Additional setters execute in ascending bit order; 256 takes precedence over 512, 2048 is followed/overridden by 4096, and 16384 follows/overrides 8192 |
| Mouse 6 | `0x17658`; field deletion `0x12d63` | Flag 1 uses mask 15 (button fields), 2 uses mask 23 (simple fields), both use mask 7; flags 4/8/16 delete base/one-numbered/stack saves |
| Audio 7 | `0x176dd` -> `0x37e41` | Flag 1 invokes PlaySoundA(NULL,NULL,0x20001). Wrapper argument is ignored. No table slot or separate MIDI-stop action |
| Screen 8 | `0x176ef` | Flag 1 clears with stock BLACK_BRUSH (4); 2/4/8 delete base/one-numbered/stack saves |
| Charset 9 | `0x17b7a` | Stores charset byte at environment-system +0x10, but its two-argument descriptor conflicts with the three-argument stride; see accepted misalignment below |

Backup family numbers are button 0, style 1, port 2, text window 3, palette 4, mouse 5, screen 6, environment 7. All numbered-save flags invoke `0x23d6e(context,family,-2)` once. A controlled test restores that helper’s original instructions, supplies three present saves with slot 0 protected, and verifies that only slot 1 is deleted and the count decrements once; slot 2 survives. Stack-clear helper `0x24648` instead repeats `-4` until failure.

Port bit 4 checks current physical target (+0xa), definition and protection before calling `0x33112(context,physical,0)`, then clears physical selection even if deletion failed. That helper explicitly refuses deliberate deletion of port 0 (`0x33137–0x33155`); protected physical targets skip the call and retain selection. Delete-others bit 8 also resets screen-port geometry when a nonzero logical port remains current (`0x16fb7–0x16fc7`). Logical selection (+8) and physical target (+0xa) must not be conflated.

Environment’s misindexed validator does not replace setter range checks. Width setter accepts 2..5; RGB setter accepts **0..8**, not the validator’s misapplied 1..8 slot constraint. Both silently ignore out-of-range fields, update current state/parser when valid, and do not consult protection. Native checks exercise protected direct setters and invalid fields. Flag 128’s default-setting path has the same direct behavior; flag 64’s scan checks protection first. Palette 64’s current-only action is a separate shipped defect. Button definition/protection bitmaps are distinct from record flags; an early suspicion that default copying corrupted protection was rejected after resolving the table layout.

### Framing and integration oracles

The final probe checks all ten selectors, mixed descriptors, ten accepted versus eleven rejected declared segments, backslash-CRLF continuation both inside and between fields, all global flag combinations 0..63, and each defined segment flag’s original control path. Declared count zero ignores parsed extras; well-formed headers should still contain exactly the declared segments. Validation runs before global effects and does not provide runtime rollback.

Constructed `!|h01000201502O2220` plus CRLF invokes the original byte parser, header consumer and environment setters. `02O2` base 64 is 9730 (2+512+1024+8192): drawing reset, environment slot 2, base 36, width 2 and WCF 1280×960. This is a checked generated example, not recovered corpus content.

Two additional original-parser integration cases prove settings affect the next chained command: `!|h01000001500G0230|L001002003004` changes coordinate width to 3 and decodes the following line as 1,2,3,4; `!|h0100000150040220|q0a` changes radix to 64 and decodes q’s argument as 36. An initially failed radix test was a **harness defect**: the replacement CRT character-type table had digit/whitespace bits but omitted uppercase/lowercase bits used by the original numeric parser. Adding those bits fixed it without changing driver instructions.

The type-9 mismatch is not equivalent to “charset can never execute.” `!|h01000002900200060` plus CRLF parses to `[1,0,2,9,0,2,6,0]`: charset 0 followed by port flags 6/slot 0. Original validation/application advances three arguments after type 9, so it accepts and executes charset 0 then mouse flags 0, with no port switch. This accepted misalignment is checked natively. Other combinations can read outside the intended argument structure. Publish no invented extra wire field and no independently consistent charset grammar.

### Final entry-point details

q’s exhaustive 0..4095 tests confirm only 0..15 are accepted, flags accumulate, zero does not clear, and protected/system-font styles reject it. r’s numeric reserved field is not read; invalid mode/domain return before TEXTDATA processing. The complete routine shows a 1023-byte input copy, NUL at scratch +0x3ff and expansion call with limit 0x3fe. Its renderer call at `0x2080d` supplies zero coordinates and a NULL text argument; it prepares the outline font without drawing the supplied string. It ignores the renderer’s return before querying width/height. Primitive results are narrowed to signed 16 bits; world conversion `0x310f2` sign-extends that value, multiplies by the appropriate world dimension, signed-divides by the corresponding device dimension and stores the low 16 bits. The reply uses `%hd` decimal text and final CR, with colon separators for per-byte results.

Native tests check literal-empty input in both font modes: the outline branch returns without reply (`0x20660–0x2066b` checks original input); system-font width is zero, system-font height follows its metric, and list modes return CR. Nonempty `$VAR$` expanded to empty still produces the outline reply, confirming the early test’s operand. Font/OS metric providers are controlled, so these tests establish protocol control/formatting and not real-font pixel dimensions.

Directory probe confirms the complete original setter/path-construction behavior, whitespace-only accepted as empty after trim, exact `.`/`..` rejected, literal empty rejected and host callback absence suppressing all work. Trimming/uppercasing occur **before expansion**, not a second time afterward. Literal `$OFF$` clears without creation; expanded `$OFF$` is an ordinary name. Creation failure leaves the selected name changed. Host directory is a single name, not a switchable drawing table.

The missing SyncTERM-only entries are now explicitly excluded: the **complete original dispatch table** lacks 1N, 1O, 1S and level-0 ESC, and the original byte parser yields no callback for each constructed test record. Level-0 ESC as a RIP opcode is distinct from ANSI autodetection and from actual level-1/2/9 ESC commands. This classification resolves the issue’s request without importing guessed field semantics. Article selection remains with issue #2.

## Verification and next action

Run the SHA-guarded native probes against the unchanged source DLL:

```sh
run/riptel-header-probe /path/to/ripscrip.dll
run/riptel-font-probe /path/to/ripscrip.dll
run/riptel-column-probe /path/to/ripscrip.dll
run/lint
run/check-links
git diff --check
```

Final native verification passes: **9,176 header/parser/entry checks**, **31,713 font checks** and **23,013 column checks** against the unchanged SHA-verified DLL. Repository link validation passes **4,529 links across 187 files**, and Markdown lint and `git diff --check` pass. Documentation review also removed stale Alpha 4 audio flags and blanket header-reset assumptions from related reference pages. All issue subquestions are resolved by the published header contract, q/r/level-2 ESC entries, explicit four-opcode exclusion and single audio-action classification. Delivered as PR #31 (primary specification commit `ac4c168`). Review/merge remains pending; do not merge or close the issue manually. Continue serial research with issue #6 on its own branch.

---

[Research backlog](issue-backlog.md) · [Header reference](../ripscrip/1.1-protocol-and-command-hierarchy.md#scene-header-segments)
