# Issue #87: user-variable prefixes, scope and transparency

[Research backlog](issue-backlog.md) · [Text variables](../ripscrip/5.1-text-variables.md)

## Scope and checkpoint

Resolve [#87](https://github.com/bbs-land/remote-imaging-protocol/issues/87), created after #86, on `issue-87-user-variable-directives` from fork stable **94513e5**. #86 is delivered in PR #88 and consolidated. The latest issue/comment refresh (2026-11-19 13:28:05 UTC) finds no comments on either report and no newer discussion/inline feedback.

Preserve adopted #6 evaluation/argument/built-in answers and #84/PR #85 byte/backslash grammar. Address prefix `=` versus `%`, repeated directives, persistent/session precedence, and a complete existing/missing × define/retrieve × default matrix. Distinguish accepted empty input, cancellation and transparent no output. The original #6 user-variable service was controlled, not independent evidence of actual storage/prompts. Use the protocol draft, original RIPtel help and bounded read-only parser/provider instructions; do not execute client/UI/prompt paths or investigate implementation bugs.

## Initial evidence and next action

The current 3.0 directive table says `%` and “each once” and unconditionally says `-`/`&` do not prompt; its inherited description and endorsed `$-=NAME=value$` corpus disagree. Original parser initial classification accepts prefix `=` at RVA `0x15756–0x15758`. Read the following directive dispatch and the user-variable provider only as necessary to distinguish source facts from explicit protocol choices. Reconcile the normative reference in one place, retain documentary provenance, then publish a complete separate PR and consolidate its actual head into fork stable. Keep co-author trailers and scoped temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Text variables](../ripscrip/5.1-text-variables.md)
