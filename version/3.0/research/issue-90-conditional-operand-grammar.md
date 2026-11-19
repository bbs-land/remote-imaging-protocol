# Issue #90: conditional operand byte grammar

[Research backlog](issue-backlog.md) · [Conditionals](../ripscrip/5.2-templates-and-conditionals.md#comparison-semantics)

## Scope and checkpoint

Resolve [#90](https://github.com/bbs-land/remote-imaging-protocol/issues/90) on branch `issue-90-conditional-operand-grammar` from fork stable **ab37e09**. #91 ordinary outline anchors/justification follows, with a separate complete PR. Refresh at 2026-11-19 19:01:38 UTC finds 46 open RealDeuce issues and 43 open PRs; the two new reports have no comments and no newer discussion/inline feedback was returned since the previous checkpoint.

Preserve #6 comparison/reduction/evaluation rules, #84 dollar-argument grammar and #87 user-variable policies. Specify conditional reference/literal bytes and normalization, local escape stages, terminator/quote precedence, empty operands, connector boundaries and safe source/decoded limits. Primary evidence is the preserved protocol text plus read-only original RIPtel 3.1 parser instructions, not execution of client/UI/callbacks or unsafe paths. Clearly label explicit interoperability choices where source behavior is incomplete or unsafe.

## Next action

Trace original operand parser RVA `0x4b6e3–0x4bc9c` and its caller/provider boundaries; publish complete pseudocode and distinguishing byte examples. Record exact source fingerprints/offsets, facts/inference/policy boundaries and validation. Publish one complete PR closing #90, consolidate its actual head into fork stable, then start #91. Keep Codex co-author trailers and scoped temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Conditionals](../ripscrip/5.2-templates-and-conditionals.md#comparison-semantics)
