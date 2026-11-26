# Issue #315 — Reached dollar-argument errors in ordinary host responses

[Issue #315](https://github.com/bbs-land/remote-imaging-protocol/issues/315) · [Ordinary response rule](../ripscrip/5.0-host-commands.md#rejected-dollar-arguments-in-ordinary-responses) · [Research backlog](issue-backlog.md)

RealDeuce filed #315 on 2026-11-25 at 23:33:42 UTC. The 23:36 UTC check found it with no comments. Branch `issue-315-host-argument-rejection` starts at fork stable `6bbd7dc1384e9c822a87f14132f272b9e80f4b93`.

The accepted #116 contract defers host validation until a stored button action is reached. #84 provides the argument-byte predicate. Original pipeline observations for FILEDEL show a complete parser-rejected span remaining literal. This issue follows the two-pass sender and contrasts parser rejection with RAND's callback rejection. [Button validation](issue-116-button-payload-hit-geometry.md#validation-versus-deferred-execution) · [Argument grammar](issue-84-dollar-argument-grammar.md#outer-ordinary-scanner-and-host-passes) · [Original FILEDEL pipeline](issue-20-filedel-scope.md#parser-and-helper-are-different-layers).

This note traces explicit rejection of complete spans. For candidates exhausted before an unquoted closing parenthesis, the parser reports a one-byte advance and ordinary scanning can reach the remaining bytes. [Incomplete candidate derivation](issue-318-rejected-dollar-span-quoting.md).

## Original scanner and send path

Static inspection used PC Micro RIPtel 3.1's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Driver addresses are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md).

- Dollar parser `0x15602` first seeks the closing delimiter at `0x15626–0x15653`. It skips dollar candidates immediately preceded by a backslash. With no eligible close it returns 0 and advances one byte. For a complete reference, the closing pointer is retained separately from the argument/state cursor. The argument state calls predicate `0x155bf` at `0x15b60`. A double quote fails via `0x155d5` and `0x155f6`, then state 11 returns -1 at `0x15f44–0x15f54` with the full source span length, from the opening dollar through the located close.
- The ordinary scanner calls that parser at `0x4b28f` and invokes `0x25b21` only for parser status 1. Parser rejection takes the advance-only route `0x4b2e6–0x4b2f8` with no deletion, insertion or provider call. The scanning cursor moves beyond the retained span. It continues the pass at `0x4b268`.
- A valid `$RAND(bad)$` reaches the evaluator. RAND `0x28e96` tests decimal syntax through `0x65589` at `0x28ec5`. The nondecimal branch reaches the local error at `0x29017`, sets variable error at `0x2901c` and returns NULL at `0x29028`. The built-in evaluator `0x25a7b` distinguishes that from a successful NULL at `0x25b03–0x25b17`. The wrapper `0x25b21` bypasses user lookup for that recognized error and returns ordinary refusal. The scanner advances over the retained parsed span via `0x4b2de–0x4b2e4`. Only ordinary success reaches source deletion at `0x4b2c3`. The separate abort result takes the `0x4b450` exit, where the scan returns failure.
- Successful completion of an ordinary scan returns 1 at `0x4b279`. Host execution uses the response routine at `0x4acd9`, which calls the scanner at `0x4acfa` with the first-pass mask, then at `0x4ad16` with full flags. Successful nonempty output reaches send at `0x4ad41`. The mode-0 query entry follows `0xd484 → 0x13ed0` and calls the same general host entry `0x4ab3e` at `0x13ef2`. [Immediate query caller](issue-51-deferred-query-order.md).

## Derived response

In pass one the two transparent session assignments succeed and disappear from the working string. The quoted argument rejects before provider lookup and remains in place, so the working string becomes `pre$RAND("bad")$post`. In pass two its argument rejects again and the same bytes reach send. Assignments already consumed in pass one have no second invocation. For the unquoted `bad` vector the full span also survives each pass, with a reached RAND callback error on each attempt. No numeric value is inserted. These derivations assume ordinary source after the earlier framing, preprocessing and HCL stages, an eligible host send, and no truncation or unrelated abort. [Assignment rules](issue-87-user-variable-directives.md) · [Prior two-pass capture](issue-6-macro-evaluation.md).

```sh
sha256sum "$dll"
objdump -d --start-address=0x10015602 --stop-address=0x10015658 "$dll"
objdump -d --start-address=0x10015f24 --stop-address=0x10015f60 "$dll"
objdump -d --start-address=0x1004b287 --stop-address=0x1004b2fd "$dll"
objdump -d --start-address=0x10025a7b --stop-address=0x10025bb2 "$dll"
objdump -d --start-address=0x1004acd9 --stop-address=0x1004ad51 "$dll"
```
