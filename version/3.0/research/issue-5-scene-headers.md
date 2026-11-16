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

## Next action

Extend the native header probe to execute global and segment effect routines, recording underlying reset/switch/protection/backup calls and actual state changes. Trace the remaining table-segment functions and their backup/reset helpers; test environment setters and character-set mismatch. Then fully derive TextMetric outputs and SwitchDirectory behavior. Keep #5 incomplete and do not file a partial resolution PR.

---

[Research backlog](issue-backlog.md) · [Header reference](../ripscrip/1.1-protocol-and-command-hierarchy.md#scene-header-segments)
