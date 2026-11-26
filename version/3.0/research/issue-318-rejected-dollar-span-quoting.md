# Issue #318 — Pass-two quoting and rejected dollar spans

[Issue #318](https://github.com/bbs-land/remote-imaging-protocol/issues/318) · [Host response quoting](../ripscrip/5.0-host-commands.md#quoting-and-incomplete-candidates) · [Research backlog](issue-backlog.md)

RealDeuce filed #318 on 2026-11-26 at 00:05:56 UTC. The 00:06 UTC check found it with no comments. Branch `issue-318-rejected-dollar-span-quoting` starts at fork stable `bf2e2bbb8d48501baccfc810ebacc38b120cb6ce`.

The #315 derivation establishes full-span retention for explicit argument syntax rejection and ordinary built-in rejection. The earlier FILEDEL probe records removal of a backslash before the sole closing parenthesis under the full ordinary scan. The parser reports different advances even when delimiter lookahead finds a later dollar in both cases. [Ordinary rejection derivation](issue-315-host-argument-rejection.md) · [Original FILEDEL matrix](issue-20-filedel-scope.md#parser-and-helper-are-different-layers).

## Original advance and quoting paths

Static inspection used PC Micro RIPtel 3.1's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Driver addresses are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md).

- Dollar parser lookahead locates the next eligible dollar at `0x15626–0x15653`, before checking argument syntax. In argument state 8, a backslash saves a pending quote and the next admitted byte overwrites it at `0x15ea7–0x15ebd`. The predicate at `0x155bf` admits byte `0x29`, so `\)` supplies data. The branch that ends the argument list at `0x15ee7` needs an unquoted parenthesis.
- For `$FILEDEL(X.bmp\)$`, the only parenthesis after the argument data is quoted. The parser reaches the lookahead bound without completing the reference. At `0x15696–0x156a0` exhaustion branches to `0x15f1a`, which returns status 0 and increments the reported advance by one. The scanner sets that advance to zero before parsing at `0x4b186`, so its `0x4b2f6` advance skips just the opening dollar.
- An explicit syntax rejection such as a double quote in argument data returns -1 through `0x15f44–0x15f54`, with the complete span length. The scanner advances by that length at `0x4b2ef`. A parsed callback rejection likewise advances by the parsed length. In both cases the next outer scan position follows the closing dollar. The scanner never invokes outer backslash or caret processing on bytes within the skipped span.
- Outer backslash processing at `0x4b18e–0x4b1ac` deletes the backslash and advances past the following byte. Caret control processing is at `0x4b394–0x4b445`. The first ordinary pass masks both features at `0x4ace3`, and the second starts from the beginning with the full flags. Neither advance branch rescans the earlier opening dollar in the same pass. [Control-byte derivation](issue-53-host-control-brackets.md).

## Discriminating results

The existing probe covers the same argument suffix `X.BMP\` in `A$FILEDEL(X.BMP\)$B`. It separately checks the first-pass flag set and the full flag set and observes no callback lookup on either attempt. The full flag set removes the backslash. The report's `pre` and `post` strings replace those outer literals without changing the scanner path. The second pass sees the original candidate before it removes the backslash and has no later pass in which to invoke the newly spelled reference. [Existing controlled probe](../../../run/lib/riptel-macro-probe.c).

A double quote before a backslash or caret pair can explicitly reject a complete span. That span is skipped intact on both passes. An argument with no unquoted closing parenthesis can exhaust the candidate while a caret pair remains in its text. The second pass sees the pair after the one-byte advance and can turn `^M` into CR. These are ordinary response bytes prior to any transport-specific output handling.

```sh
sha256sum "$dll"
objdump -d --start-address=0x10015696 --stop-address=0x100156ba "$dll"
objdump -d --start-address=0x10015ea7 --stop-address=0x10015f60 "$dll"
objdump -d --start-address=0x1004b17d --stop-address=0x1004b1b3 "$dll"
objdump -d --start-address=0x1004b2de --stop-address=0x1004b2fd "$dll"
```
