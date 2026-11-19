# Issue #90: conditional operand byte grammar

[Research backlog](issue-backlog.md) · [Conditionals](../ripscrip/5.2-templates-and-conditionals.md#comparison-semantics)

## Scope and checkpoint

Resolve [#90](https://github.com/bbs-land/remote-imaging-protocol/issues/90) on branch `issue-90-conditional-operand-grammar` from fork stable **ab37e09**. #91 ordinary outline anchors/justification follows, with a separate complete PR. Refresh at 2026-11-19 19:01:38 UTC finds 46 open RealDeuce issues and 43 open PRs; the two new reports have no comments and no newer discussion/inline feedback was returned since the previous checkpoint.

Preserve #6 comparison/reduction/evaluation rules, #84 dollar-argument grammar and #87 user-variable policies. Specify conditional reference/literal bytes and normalization, local escape stages, terminator/quote precedence, empty operands, connector boundaries and safe source/decoded limits. Primary evidence is the preserved protocol text plus read-only original RIPtel 3.1 parser instructions, not execution of client/UI/callbacks or unsafe paths. Clearly label explicit interoperability choices where source behavior is incomplete or unsafe.

## Primary source identity and method

The original DLL in the [catalogued artifact mirror](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, is **592,896 bytes**, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. RIPtel 3.1 ships RIPscrip driver 3.0.7; this is a language 3.0 reconstruction, not a new language version 3.1. LLVM objdump 19.1.7 reads the unchanged PE32 image at base `0x10000000`. Instruction addresses below are RVAs; `.text` file offset is RVA minus `0xc00`, while the cited `.data` strings use RVA minus `0x1a00`. `run/riptel-inspect` verifies the mappings and fingerprint.

The preserved [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) is 1,202,490 bytes, SHA256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`; it supplies inherited macro/template context, not this later conditional byte lexer. Existing [#6 comparison/evaluation evidence](issue-6-macro-evaluation.md), [#84 argument grammar](issue-84-dollar-argument-grammar.md) and [#87 directives](issue-87-user-variable-directives.md) remain adopted. Neither an unrelated argument decoder nor a peer implementation supplies a missing conditional rule by analogy.

Only original instruction/data **reading** was performed. No original instruction, callback, prompt, UI, Wine process, unsafe input or transmission was executed. Earlier native counts belong to their earlier declared fixtures; the new examples are source-derived scanning/policy vectors, not new native outcomes. The work defines the protocol, not fixes for obsolete client code.

## Left scanner and provider boundary

| RVA | Original instruction evidence |
| --- | --- |
| `0x4b701–0x4b744` | Skip whitespace via ctype mask 8; reject exhausted input |
| `0x4b754–0x4b78b` | Optional connector reads compare four bytes with `AND ` or three with `OR ` |
| `0x4b7df–0x4b842` | A leading bare `?` reads one case-folded MegaNum digit and obtains its template; no opening-dollar removal precedes this test |
| `0x4b847–0x4b88e` | Optional opening dollar, then alphanumeric mask `0x103` on the first reference byte; raw directives/quote/backslash do not pass that initial predicate |
| `0x4b8bb–0x4b91e` | Later backslash removes itself and copies the next non-NUL byte before delimiter checks; no argument-byte predicate is used |
| `0x4b8ca–0x4b954` | Dollar ends an enclosed reference, errors if there was no opening dollar; bare whitespace/operators stop the token, enclosed ones are copied |
| `0x4b955–0x4ba36` | Skip separator whitespace; reject premature `>>`; recognize the six existing comparison operators |
| `0x4bd23–0x4bd48` | Format normalized body using original string at `0x83b34`: **`$&%s$`**; parse through `0x15602` |
| `0x4bd6d–0x4bdaa` | Evaluate the parsed reference through `0x25b21`; unsuccessful evaluation returns false |

The conditional lexical buffer contains a **reference body**, not an already validated name/argument list or an expanded value. Its case and non-delimiter bytes survive; the next parser imposes name rules, 40 nonempty arguments, 20 decoded bytes per argument and #84 payload/escape rules. A dollar-enclosed conditional reference can preserve spaces/quotes/operators which that provider may subsequently reject. A leading digit passes this scanner without establishing that it is a valid provider name. Prefix directives cannot be typed raw at the start, even after a dollar; synthetic `&` is introduced by the comparison service.

The contract explicitly distinguishes conditional syntax failure from provider-format/value rejection (false comparison), and successful empty data from lookup failure. **Portable validation choice:** require the synthesized provider input to be exactly one complete reference; reject embedded terminators/trailing material without evaluating an unintended prefix. This choice closes a boundary the source does not promise to validate completely. It is provider rejection/false, not a new recursive macro feature or fallback for a rejected known built-in. Existing caller permissions and user-variable prompt policy still apply.

## Right scanner, exact termination and trim

| RVA | Original instruction evidence |
| --- | --- |
| `0x4ba3f–0x4ba94` | Skip leading whitespace, require non-NUL input, consume optional initial double quote |
| `0x4baa5–0x4baf2` | Unquoted whitespace and closing parenthesis terminate the literal |
| `0x4baf8–0x4bb27` | Backslash quotes the next byte; `>>` after that prefix is copied as two data bytes |
| `0x4bb29–0x4bb3b` | Unescaped `>>` ends scanning before quote-state closure; unescaped quote closes quoted mode |
| `0x4bafc`, `0x4bb13`, `0x4bb3d` | 255-byte decoded capacity checks; special two-byte copy needs two slots |
| `0x4bb56–0x4bb84` | Quote must have closed; unescaped terminator while still quoted reports missing quotation mark |
| `0x4bb90–0x4bb9f` | Unquoted cleanup calls `0x654e9` and then `0x654a8` |
| `0x654e9–0x65548` | `0x654e9` removes trailing ctype-whitespace; it does not remove leading whitespace |
| `0x654a8–0x654b4` | Second helper calls known strlen `0x68dd0`; **it is not a left-trim operation** |
| `0x4bbaa–0x4bc57` | Skip whitespace, require `>>`, `AND `, `OR ` or a closing group parenthesis |
| `0x4c1ad–0x4c1c4` | NOT recognizes exact four-byte, case-insensitive `not ` from `0x83b8c` |

The original constants are `AND ` at RVA `0x83a88`, `OR ` at `0x83a84`, and `not ` at `0x83b8c`, each including an ASCII space. Generic whitespace skipping before/after a keyword does not replace that space. The report's proposed literal cases have different outcomes for one, two and three actual backslashes; copied JSON/host-language spellings must not be mistaken for parser bytes.

A useful correction to the earlier terse “unquoted text is trimmed” sentence: leading **unescaped** whitespace is skipped before scanning, but a quoted leading space survives in unquoted data. Only decoded trailing whitespace is stripped after scanning; an escaped trailing space does not survive. Quoted literals bypass that trim. No local C/JSON escape names exist. Right-side literal syntax allows doubled backslashes and double-quoted content; #84's argument restrictions apply only if a later ordinary argument parser is actually reached, not to literal data.

A missing unquoted value immediately before `>>` or a legal closing group parenthesis is empty, while NUL immediately after operator/whitespace is a failure. A quoted literal encountering unescaped `>>` before its closing quote fails. A protected `>>` pair is data; a doubled backslash before that pair protects only the backslash, leaving an unescaped terminator. A trailing backslash is explicitly rejected rather than following an unsafe source read.

## Portable completion choices and limits

The source establishes the normal scanning order and explicit RHS capacity. It does not establish a complete safe locale/left-buffer/provider-result contract. This reconstruction makes the following choices explicit:

- ASCII whitespace/alphanumeric and keyword folding; other non-NUL payload bytes are preserved, with provider validation still separate. No Unicode/code-page reinterpretation is introduced.
- Maximum **255 decoded bytes** in an ordinary left body and right literal, plus storage NUL. The original callers pass adjacent 256-byte stack buffers at `ebp-0x26c` and `ebp-0x16c` (`0x4c45b–0x4c471`, `0x4c5a8–0x4c5c0`), but the left append path has no demonstrated matching bound. The portable rule checks before writes; it is not a claim about all native upstream length limits.
- Capacity is checked before trimming; a two-byte escaped terminator needs two slots. Optional enclosing dollars/quotes do not count. No independent smaller source cap is introduced; prior command/expansion limits still apply. Maximum escaped left spelling is 509 body bytes, or 511 with dollars; 255 ordinary escaped RHS bytes can occupy 510 payload bytes plus quotes.
- A provider result over 255 bytes is false/evaluation failure, not truncation or a required unsafe copy. Ordinary reference rejection also follows the existing false result; lexical/structural errors reject preprocessing nonfatally.
- Reject dangling escapes without reading beyond NUL and do not evaluate a lexically rejected comparison. No whole-pass rollback of earlier completed side effects is promised.

These choices preserve the adopted comparison/reduction and inactive-parent rules. They neither reproduce unsafe behavior nor reclassify a client defect as a protocol requirement.

## Reproduction and review

Bounded original disassembly:

```sh
objdump -d --start-address=0x1004b6e3 --stop-address=0x1004bc9c ripscrip.dll
objdump -d --start-address=0x1004bd00 --stop-address=0x1004bdc1 ripscrip.dll
objdump -d --start-address=0x1004c142 --stop-address=0x1004c244 ripscrip.dll
objdump -d --start-address=0x100654a8 --stop-address=0x10065549 ripscrip.dll
```

A read-only byte check reproduces the fingerprint and keyword/provider strings (these particular string RVAs map to `.data`):

```python
from hashlib import sha256
from pathlib import Path

b = Path("ripscrip.dll").read_bytes()
assert len(b) == 592896
assert sha256(b).hexdigest() == "83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8"
for rva, expected in ((0x83a88, b"AND "), (0x83a84, b"OR "),
                      (0x83b8c, b"not "), (0x83b34, b"$&%s$")):
    offset = rva - 0x1a00
    assert b[offset:offset + len(expected) + 1] == expected + bytes([0])
```

The [published byte tables and stage examples](../ripscrip/5.2-templates-and-conditionals.md#conditional-operand-byte-grammar) answer each subquestion, including left provider normalization, one/two/three-backslash distinctions, escaped spaces/quotes/parentheses/dollars/terminators, empty operands, keyword separators and safe decoded bounds. Reviewed each example as actual bytes; used explicit hex for ambiguous quote/backslash results. An initial draft's command-string escaping dropped some literal backslashes; manual byte review caught this before commit/publication and the tables were rewritten with explicit byte-preserving construction. Do not copy prose shorthand or host-language escaped strings as parser inputs.

## Delivery

Original DLL fingerprint, keyword/provider strings, exact published backslash spellings and capacity arithmetic checks pass. `run/lint`, `run/check-links` (0 bad of 5,684 links across 233 files) and `git diff --check` pass. Refresh at 2026-11-19 19:10:05 UTC finds no #90/#91 comments or newer discussion/inline feedback. Delivered in [PR #92](https://github.com/bbs-land/remote-imaging-protocol/pull/92), specification **a1beea8**, branch `issue-90-conditional-operand-grammar`. GitHub confirms open/ready/mergeable and exactly #90 as closing reference. Consolidate its actual head into fork stable. Then start #91 from updated stable. Retain co-author trailers and scoped temporary API cleanup. No client execution is needed.

---

[Research backlog](issue-backlog.md) · [Conditionals](../ripscrip/5.2-templates-and-conditionals.md#comparison-semantics)
