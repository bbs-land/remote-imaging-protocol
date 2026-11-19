# Issue #84: dollar-reference argument byte grammar

[Research backlog](issue-backlog.md) · [Variable resolution](../ripscrip/5.1-text-variables.md#variable-resolution-in-3x)

## Scope and checkpoint

The final #82 delivery refresh discovered new [#84](https://github.com/bbs-land/remote-imaging-protocol/issues/84), created 2026-11-19 01:54:56 UTC with no comments. Continue the standing oldest-first workflow on branch `issue-84-dollar-argument-grammar` from fork stable **f77f918**, after #82 was delivered in PR #83 and consolidated at 122be1e. Preserve the completed #6 count/size/empty-field answers and its derivation.

Recover accepted/rejected argument bytes, nonempty quote behavior, commas/parentheses/dollar/backslash representation, stored-versus-source length accounting and ordinary versus host two-pass escape stages. Use explicit protocol rules and read-only original parser/caller instructions. No native client/UI or unsafe-behavior investigation is authorized. Record exact state transitions and stage-specific examples, then publish a complete PR closing #84 and consolidate into fork stable. Every commit needs Codex co-authorship; scoped temporary PR files must be removed.

## Next action

Read the existing stage/argument contract and protocol sources. Trace byte predicate 0x155bf, parser states around 0x15afe/0x15b1a/0x15ea7, and ordinary expansion's backslash modes in both host passes. Distinguish delimiters from accepted payload bytes and describe limits after argument decoding without inventing modern quoting rules.

---

[Research backlog](issue-backlog.md) · [Variable resolution](../ripscrip/5.1-text-variables.md#variable-resolution-in-3x)
