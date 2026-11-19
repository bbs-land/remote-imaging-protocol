# Issue #84: dollar-reference argument byte grammar

[Research backlog](issue-backlog.md) · [Variable resolution](../ripscrip/5.1-text-variables.md#variable-resolution-in-3x)

## Scope and checkpoint

The final #82 delivery refresh discovered new [#84](https://github.com/bbs-land/remote-imaging-protocol/issues/84), created 2026-11-19 01:54:56 UTC with no comments. Continue the standing oldest-first workflow on branch `issue-84-dollar-argument-grammar` from fork stable **f77f918**, after #82 was delivered in PR #83 and consolidated at 122be1e. Preserve the completed #6 count/size/empty-field answers and its derivation.

Recover accepted/rejected argument bytes, nonempty quote behavior, commas/parentheses/dollar/backslash representation, stored-versus-source length accounting and ordinary versus host two-pass escape stages. Use explicit protocol rules and read-only original parser/caller instructions. No native client/UI or unsafe-behavior investigation is authorized. Record exact state transitions and stage-specific examples, then publish a complete PR closing #84 and consolidate into fork stable. Every commit needs Codex co-authorship; scoped temporary PR files must be removed.

## Sources and standing

Preserved ALPHA 4 `version/2.0/text/RIPScrip-2.0-alpha-4.txt`, SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, describes parameterized dollar references and comma separators. §4.2.1.11 documents the older 40-parameter/12-character limits; the already adopted #6 reconstruction establishes the 3.0 parser's 40 nonempty slots and 20 stored bytes, and the user expressly retains those answers. The source's general text-response escapes and conditional quoted literals are different stages, not an argument quoting grammar. No recovered protocol text grants double-quoted parameter strings or a general escape for arbitrary punctuation.

Original RIPtel 3.1 / driver 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The current bytes were fingerprinted before reading the predicate/state table. PE base is 0x10000000; below, addresses are RVAs and .text file offset is RVA−0xc00. LLVM objdump 19.1.7 and read-only byte inspection were used. No original instructions, callback, host transmission, unsafe path or UI were executed. New example outcomes are control-flow deductions, not new native fixture results.

## Predicate and argument state

The complete predicate at **0x155bf–0x15601** first rejects unsigned bytes below 0x20, then zero-extends the byte and excludes exactly:

```text
22        double quote
2A–2C     asterisk, plus, comma
2F        forward slash
3B–3F     semicolon, less-than, equals, greater-than, question mark
5B,5D     square brackets
7C        vertical bar
```

Every other byte 0x20–0xff passes this helper. That includes single quote, opening/closing parentheses, dollar, backslash, DEL and all high bytes. The helper alone is insufficient: the argument state treats comma/right-parenthesis as delimiters and backslash as a pending quote, while the whole-reference lookahead treats dollar specially.

The original 12-entry state jump table at **0x15f60**, file offset **0x15360**, contains:

```text
0:156ba  1:15790  2:1583b  3:158d5  4:15945  5:15990
6:15a29  7:15a7d  8:15ad1  9:15b91  10:15f24  11:15f44
```

The opening-parenthesis transition at **0x15da8–0x15db5** enters state 8 with stored length zero. Argument scanning begins at **0x15ad1**. Its exact ordering is:

| Path | Original RVAs | Result |
| --- | --- | --- |
| Current byte is backslash, last stored byte is also backslash | 0x15ad7–0x15af9 | Syntax failure; doubled backslash is not a literal-backslash escape |
| Current byte is backslash otherwise | 0x15afe–0x15b15, store 0x15e8d–0x15ea5 | Check slot/length capacity, append backslash and increment length |
| Previous stored byte is backslash and current byte is not | 0x15b1a–0x15b49 | Require the predicate, then overwrite the backslash at 0x15ea7–0x15ebd, without incrementing length |
| No pending backslash, ordinary accepted byte other than right parenthesis | 0x15b4e–0x15b6a, store 0x15ebf–0x15ed7 | Append byte, increment stored length |
| Unquoted comma | 0x15b70–0x15b78, 0x15ed9–0x15ee5 | Commit current argument only if nonempty, reset length |
| Unquoted right parenthesis | 0x15b7e–0x15b80, 0x15ee7–0x15ef6 | Commit only if nonempty, enter post-argument state 9 |
| Other excluded byte | 0x15b86 | Syntax failure |

The previous-byte test indexes argument slot base +0x17 plus 21×argument_index plus stored_length−1; each slot has 20 payload bytes and a terminator. Appending checks count<40 and length<20. The overwrite route instead preserves length, permitting a quoted byte to finish slot 20. Argument bytes are copied unchanged except removal of their quote prefix; the later uppercase helper 0x75820 operates on the NUL-terminated variable-name field, not its separate argument slots. Callbacks can perform their own later normalization.

## Composed lexical answers

- Nonempty and empty double-quoted arguments reject at the quote byte. There is no argument quote-open/quote-close state. Single quotes are ordinary retained payload.
- Comma separates fields, and backslash-comma rejects before it can be reinterpreted as a delimiter. There is no direct literal-comma spelling.
- Opening parenthesis is ordinary argument data; it does not add nesting depth. Unquoted right parenthesis ends the list; backslash-right-parenthesis stores a literal right parenthesis and stays in state 8.
- Backslash followed by another backslash rejects. Backslash followed by any other predicate-accepted byte overwrites the quote prefix. Therefore no valid direct dollar argument stores a literal backslash; a lone/pending backslash cannot close normally.
- Backslash does not admit the excluded quote, asterisk, plus, slash, comparison punctuation, brackets or pipe. Borrowing C/JSON quoting or a filesystem path parser would accept inputs this grammar rejects.
- The reference presearch at **0x1562c–0x15649** finds a dollar not immediately preceded by backslash. Thus payload dollars must be backslash-dollar. The quote-overwrite route then stores the literal dollar. An unescaped inner dollar is selected as the terminator too early, so raw nested dollar references are not a way to expand an argument.
- Spaces and accepted high bytes are retained. Limits count stored bytes after local quote removal, not source bytes or Unicode characters. Twenty backslash-A pairs occupy 40 source bytes but store 20 A bytes; a 21st stored byte rejects. No new slot or length rule replaces #6's adopted empty-field behavior.

The public [byte table and distinguishing references](../ripscrip/5.1-text-variables.md#argument-bytes-and-local-backslashes) cover these classes and outcomes. They use F as a syntactic placeholder, not as a predefined callback. The table's backslashes are actual input bytes at the dollar parser, after earlier enclosing stages. The argument contract ends at its first unquoted closing parenthesis; state 9's separate question/default/directive handling must not be mistaken for parenthesis nesting or a new argument payload phase.

As a static arithmetic cross-check of the class enumeration: 211 byte values pass the helper; removing dollar/right-parenthesis/backslash from the ordinary raw payload class leaves 208, while local quoting admits dollar/right-parenthesis but not a literal backslash, for 210 representable payload byte values. These counts are derived from the byte sets, not original predicate execution. NUL is a C-string boundary and cannot be embedded.

The earlier [FILEDEL derivation](issue-20-filedel-scope.md) independently records native reachable-input cases where single backslashes disappear and slash/wildcard forms are rejected by the original parser. That existing result corroborates the new lexical account; it was not rerun or extended, and no filesystem/unsafe helper behavior was investigated here.

## Outer ordinary scanner and host passes

The outer scanner **0x4b0e4** is separate from the argument parser **0x15602**. At **0x4b18e–0x4b1ac**, a backslash outside a consumed reference is removed only if caller flag 0x80 is enabled. Deletion calls 0x65942, then jumps to pointer increment **0x4b44a**, skipping the following byte for that scan. If flag 0x80 is disabled, the scanner leaves the backslash in place, advances normally and can process the next dollar. The parser call at **0x4b28f** has no outer-backslash-mode argument; state 8's local quote decoding still runs when the outer mode is disabled.

Host response routine **0x4acd9** calls the ordinary scanner at **0x4acfa** with flags masked by **0xff5f**, then at **0x4ad16** with full caller flags. The first clears outer control/backslash bits 0x20/0x80. This is a recovered internal implementation distinction, not a wire flag to expose to users. First-pass dollar arguments still decode; the outer leading backslash does not shield a following dollar. In the second pass, outer backslashes are removed and their following byte skipped.

A distinguishing stage derivation assumes an allowed data lookup N=`OK`:

```text
ordinary one-pass input: backslash + $N$
outer mode enabled -> remove backslash, skip opening dollar -> literal $N$

host ordinary-response entry: backslash + $N$
first pass (outer mode disabled) -> expand N -> backslash + OK
second pass (outer mode enabled) -> remove backslash, skip O -> OK
```

If a quoted dollar is already inside a parsed argument, that byte is passed literally to the callback. The scanner does not recursively scan an argument's decoded bytes. Insertion helper **0x65987** returns after inserted replacement text; the same ordinary pass skips it, as the prior #6 derivation already proved. Returned dollar text can instead be processed in a later host pass. The mere existence of two host passes is not a reason to unquote the original argument twice.

The plain example contains no angle/bracket directives. The angle worker checks backslash followed by `<<` at 0x4c2d1–0x4c319; a plain backslash-dollar outside discarded conditional text reaches its ordinary byte-advance path 0x4cc6f–0x4cc8f unchanged. Thus the example does not require an earlier generic unquote that would remove the source backslash.

Earlier wire escaping, angle preprocessing and bracket segmentation precede these ordinary scans; later callback-specific validation follows argument decoding. For example, wire quoting can deliver a pipe byte to a string, but it cannot make that pipe valid dollar-argument payload. These layers must remain explicit rather than applying one generic escape convention to the entire source.

## Read-only reproduction

With `artifact_root` set to the persistent mirror:

```sh
run/riptel-inspect "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll" --rva 0x155bf --length 67
run/riptel-inspect "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll" --rva 0x15f60 --length 48
objdump -d --start-address=0x100155bf --stop-address=0x10015602 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x10015ad1 --stop-address=0x10015b91 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x10015e8d --stop-address=0x10015efb "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x1004b18c --stop-address=0x1004b1b9 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x1004acd9 --stop-address=0x1004ad51 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
```

## Validation and remaining delivery

The 3.0 variable reference now gives the byte alphabet, quote/delimiter distinction, decoded-length rule and distinguishing examples; the processing-order page links the specific outer/local escape distinction. Existing #6 derivations, 2.x references, RIPscrip 1.54 and original text/assets remain unchanged. No new client/native tests or GUI were run. New grammar outcomes are proven by the original instruction paths and stage composition, not by executing the obsolete client or its callbacks.

Validation passes repository Markdown lint, 0 bad of 5,592 links across 229 files, `git diff --check`, fingerprinted read-only predicate/state inspection, byte-class arithmetic and manual example/prose/table review. The 2026-11-19 02:14:25 UTC refresh finds no #84 comments or new repository discussion/inline feedback. No native execution was used for validation.

Next: publish one complete PR closing #84, then consolidate into fork stable. Record delivery identifiers and continuation state here. Retain co-author trailers and remove scoped temporary PR files.

---

[Research backlog](issue-backlog.md) · [Variable resolution](../ripscrip/5.1-text-variables.md#variable-resolution-in-3x)
