# Issue #87: user-variable prefixes, scope and transparency

[Research backlog](issue-backlog.md) · [Text variables](../ripscrip/5.1-text-variables.md)

## Scope and checkpoint

Resolve [#87](https://github.com/bbs-land/remote-imaging-protocol/issues/87), created after #86, on `issue-87-user-variable-directives` from fork stable **94513e5**. #86 is delivered in PR #88 and consolidated. The latest issue/comment refresh (2026-11-19 13:28:05 UTC) finds no comments on either report and no newer discussion/inline feedback.

Preserve adopted #6 evaluation/argument/built-in answers and #84/PR #85 byte/backslash grammar. Address prefix `=` versus `%`, repeated directives, persistent/session precedence, and a complete existing/missing × define/retrieve × default matrix. Distinguish accepted empty input, cancellation and transparent no output. The original #6 user-variable service was controlled, not independent evidence of actual storage/prompts. Use the protocol draft, original RIPtel help and bounded read-only parser/provider instructions; do not execute client/UI/prompt paths or investigate implementation bugs.

## Original sources

- Original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` from the [artifact catalogue](../../../reference/rip-tools.md): 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. PE32 image base `0x10000000`; instruction addresses below are RVAs. LLVM objdump 19.1.7 was used read-only. For `.text`, file offset = RVA minus `0xc00`; the data section uses a different mapping (RVA minus `0x1a00`). Verify with `run/riptel-inspect` rather than interchanging offsets and RVAs.
- Original `riptel.hlp` from the same extraction: **347,178 bytes**, SHA256 `ebf1e58682aa65177101fe1ab7acd4e29a6e14e3ca8e3d58a0417e06e2de6f2c`. The offsets below are **file offsets in the help file**, not DLL RVAs. Read bounded printable runs, including one-character runs: the directive column consists of isolated single characters. A minimum `strings` length greater than one loses that column.
- Preserved [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt): 1,202,490 bytes, SHA256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`; four transparent-mode cases around lines 11698 onward, repeated-directive and persistent/session prose immediately before them. This is protocol source material, not an implementation bug report.
- The [existing corpus evidence](../ripscrip/5.0-host-commands.md#the-set-syntax--namevalue) includes both `$-=NAME=value$` and `$=-NAME=value$`; it supports decomposition into define + memory + default. The 122 distinct assignment forms and earlier #6/#85 evaluation/argument work are retained, not reclassified as new native prompt checks.

No original instruction, client/UI, prompt, Wine probe, external transmission or persistence write was executed. The earlier #6 provider at `0xf16b` was replaced by a controlled name/value fixture; its test count cannot prove the real provider's storage/prompt behavior. Its wrapper parses format options before calling the underlying engine at `0xeb3c`; read-only instructions now supply additional bounded evidence, not new native outcomes.

## Help bytes correct the former transcription

| HLP file offset | Original content / significance |
| --- | --- |
| `0x467dd`, `0x467f9`, **`0x46825`**, `0x46867`, `0x468cb`, `0x468fd` | Directive column is respectively `*`, `+`, **`=`**, `#`, `-`, `&` |
| `0x46829` | Memory-table description immediately follows the `=` code |
| `0x469a7`, `0x46b44` | Explanatory prose repeats **`=`** as the storage directive |
| `0x46d12–0x46dbb` | Order is arbitrary; additional directive occurrences are ignored |
| `0x476b4–0x477df` | Cancellation/ignoring the request inserts **`NONE`**; `NONE` itself is at `0x477aa` |
| `0x44b8a` | `$NULL$` is the built-in returning nothing; this is not evidence that an empty user response inserts literal `NULL` |

Thus the current 3.0 `%` row and 5.0 synonym hypothesis were **errors in our reconstruction**, not a documented draft-versus-help change. The raw help table agrees with the draft, parser and corpus on `=`. The earlier help summary's “empty response inserts NULL” attribution is not supported by the located user-variable passage; the cancellation passage says NONE. The canonical reference now separates the documented cancellation rule from an explicit successful-empty policy. Corrected [curated help notes](riptel-help-extraction.md) retain the correction/provenance; original HLP and verbatim text/assets are unchanged.

Reproduce the actual one-character column without a WinHelp viewer:

```python
from hashlib import sha256
from pathlib import Path
import re

b = Path("riptel.hlp").read_bytes()
assert len(b) == 347178
assert sha256(b).hexdigest() == "ebf1e58682aa65177101fe1ab7acd4e29a6e14e3ca8e3d58a0417e06e2de6f2c"
for offset in (0x467dd, 0x467f9, 0x46825, 0x46867, 0x468cb, 0x468fd):
    print(hex(offset), chr(b[offset]))
for lo, hi in ((0x4697e, 0x46b90), (0x46d12, 0x46dc2), (0x476b4, 0x477e7)):
    for match in re.finditer(rb"[\x20-\x7e]{1,}", b[lo:hi]):
        print(hex(lo + match.start()), match.group().decode("ascii"))
```

This decodes only bounded plaintext runs. It is not a full WinHelp topic reconstruction or proof about arbitrary compressed fragments elsewhere.

## Read-only parser and destination evidence

The initial prefix classifier at `0x15749–0x15785` explicitly accepts `=` and the other five directives; it does not accept `%`. State 1 dispatches through the byte table at `0x15fc0` and pointer table at `0x15f90`. `%` maps to default class 11 (`0x157b1`), whose non-alphanumeric/non-digit path goes to syntax failure state 11. Prepending another directive does not turn `%` into an alias.

| Directive | Dispatch RVA | Operation on parsed record `+0x35f` |
| --------- | ------------ | ----------------------------------- |
| `#`       | `0x15bc8`    | OR `0x08`                           |
| `&`       | `0x15bd4`    | OR `0x20`                           |
| `*`       | `0x15c0a`    | OR `0x01`                           |
| `+`       | `0x15c16`    | OR `0x02`                           |
| `-`       | `0x15c9e`    | OR `0x10`                           |
| `=`       | `0x15cbf`    | OR `0x04`                           |

All six handlers accumulate bits without a duplicate-rejection guard. They return through the same prefix continuation at `0x15f11`. This supports idempotence of all six directives, rather than extrapolating a one-occurrence recommendation into rejection.

At `0xebcd–0xec01`, the engine tests `+` bit 2 first, then memory bit 4 only when persistent is absent; this selects persistent-only, memory-only or no destination. Lookup `0xe99c` first calls memory lookup `0xe962`, otherwise persistent lookup `0x147e5`, independently of destination flags. Conditional writes at `0xf015–0xf04f` and `0xf116–0xf150` select persistent helper `0xe897`, otherwise memory helper `0xe7a8`; no flag does not request either write. The memory helper replaces an existing memory entry or creates one. This supports destination independence and the selected protocol's cross-store copy/shadow rule; it is not a database-transaction/persistence-failure guarantee.

The engine's original decision tree is **not** used to override the explicit protocol four-case prompting rules. For example, the recovered no-default define/retrieve branches are not a substitute for the draft's stated prompt exceptions. Native prompt/status handling and optional data-security callbacks can differ; some return paths use literal `DENIED`, others `NONE`. No GUI outcome was observed, and investigating those implementation differences is outside this task. The published table explicitly adopts the complete documentary rule and labels the remaining choices.

Bounded reproduction:

```sh
objdump -d --start-address=0x10015749 --stop-address=0x1001583b ripscrip.dll
objdump -d --start-address=0x10015bc8 --stop-address=0x10015ccb ripscrip.dll
objdump -d --start-address=0x1000e99c --stop-address=0x1000ea02 ripscrip.dll
objdump -d --start-address=0x1000ebbf --stop-address=0x1000ec35 ripscrip.dll
objdump -d --start-address=0x1000f015 --stop-address=0x1000f063 ripscrip.dll
```

Read the dispatch bytes with PE-aware `run/riptel-inspect`, or map these `.text` RVAs to file offsets. For byte `c`, the target is `u32(0x15f90 + 4 * u8(0x15fc0 + c - 0x23))`, for `0x23 <= c <= 0x3e`; these are RVAs before conversion. The six target entries above and `%`'s default were independently read from the original file.

## Adopted protocol reconciliation

1. Exactly six user directives; prefix `=` is session storage, `%` is invalid (alone or in a mix). Repetitions are idempotent. `+` wins over `=` for destination, in either order, without changing independent memory-first lookup. Prefix and post-name `=` have separate roles.
2. Retain all sixteen cases from the draft's four-mode rules. Neither flag always prompts; define with a supplied default does not prompt; define without a default prompts; retrieve alone prefers an existing value, then a default, and prompts only when both are absent. Both flags combine define behavior with replacement output. These are documentary protocol rules, even where the short help row/native decision tree differs.
3. Specify successful storage separately from replacement. A temporary operation does not mutate an existing record, `-` alone requests no storage, and retrieval with a storage flag can copy into the requested store. Preserve another store's same-name record. The exact shadow/copy handling is labeled interoperability policy; source intent/lifetime/priority and read-only lookup/destination support are recorded above.
4. Cancellation: ordinary prompt inserts documented literal `NONE`, transparent prompts insert zero bytes under the inherited skipped-operation rule; no storage change. Successful empty values are **not cancellation**: keep an actual empty stored value and use literal `NULL` only when emitting a result. This successful-empty normalization, its extension to stored/default empty values, and an empty default counting as present are **explicit choices**, not recovered help/native facts. `*` bars cancellation rather than imposing a no-blank rule; `#` only masks entry. The separate RIP_DEFINE numeric no-blank flag and #6/#85 argument/evaluation rules are unchanged.

Rejected interpretations: `%`/`=` synonyms based solely on the old transcription; duplicate rejection based on “once”; `-` implicitly selecting memory; a storage flag suppressing prompts; treating `&` as always noninteractive; inserting/storing NULL for cancellation; replacing zero output with a NUL byte; and requiring native recovery branches or the controlled provider's behavior as protocol.

## Verification and delivery

The original HLP fingerprint and isolated code bytes reproduce exactly. Original dispatch-table decoding matches all six OR handlers and `%`'s default. Manual review checks the matrix covers exactly all 16 existing/default/mode combinations, and every row names prompt, replacement and destination write. Prefix examples distinguish all forms requested, `+` precedence, repeat idempotence and mixed invalid `%`. Empty acceptance, cancellation and successful suppression are separate; examples specify whether the stored value is empty, unchanged or nonempty. Both host-command and text-variable references now point at the same authoritative matrix; the erroneous curated help attribution is corrected.

Original help/DLL fingerprint and directive-dispatch checks, the independently audited sixteen-row matrix, `run/lint`, `run/check-links` (0 bad of 5,659 links across 232 files) and `git diff --check` pass. Refresh at 2026-11-19 13:38:17 UTC finds no #86/#87 comments and no newer repository discussion/inline/review feedback across the 42 open PRs. Delivered in [PR #89](https://github.com/bbs-land/remote-imaging-protocol/pull/89), specification **f80f413**, branch `issue-87-user-variable-directives`. GitHub confirms open/ready/mergeable and exactly #87 as closing reference. The issue refresh finds 44 open RealDeuce issues, newest #87; no newer report. Consolidate this actual branch head into fork stable. Retain co-author trailers and cleanup of scoped temporary API payloads. No client/UI or unsafe paths need execution.

---

[Research backlog](issue-backlog.md) · [Text variables](../ripscrip/5.1-text-variables.md)
