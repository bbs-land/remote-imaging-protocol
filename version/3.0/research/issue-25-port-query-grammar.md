# Issue #25: port-query argument grammar

[Research backlog](issue-backlog.md) · [PORTW/PORTH](../ripscrip/5.1-text-variables.md#portw) · [PORTY1](../ripscrip/5.1-text-variables.md#porty1)

## Scope and source

Started 2026-11-16 after [PR #57](https://github.com/bbs-land/remote-imaging-protocol/pull/57) delivered #24. Branch `issue-25-port-query-grammar` starts at `4be4e3a`. [Issue #25](https://github.com/bbs-land/remote-imaging-protocol/issues/25) asks whether PORTW accepts a two-field port/domain shorthand, which three-field grammar PORTH uses, and whether PORTY1's viewno/coordtype summary is real. Geometry changes are not requested.

Primary artifact: original RIPtel 3.1 extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, from [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), installer SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`. PE base `0x10000000`; addresses are RVAs. Original files remain unchanged; native Unix probes only, no GUI. Capstone 5.0.7 / clang 19.1.7 / FreeBSD 15.1. The [earlier #10 derivation](issue-10-port-geometry.md#unexpected-original-portwporth-query-defect) established the original width/height callback inversion but deferred this grammar question.

## Initial primary trace

- Shared decoder `0x2f743–0x2f862` accepts 0..3 arguments in fixed order **port, type, domain**. Defaults are current port / PORT / WORLD. Nonempty optional fields can only be omitted from the end. Keywords use case-insensitive comparisons. First field is CUR or a decimal slot accepted by `0x987b` (digits only, 0..35). Second is exactly PORT or VIEW. Third is exactly WORLD or DEVICE. No branch interprets DEVICE/WORLD in the second field.
- Original registration gives PORTH `0x2f642`, PORTW `0x2f863`, PORTX0 `0x2f95e`, PORTX1 `0x2fa45`, PORTY0 `0x2fb2a`, PORTY1 `0x2fc11`. They all call the same decoder; no alternate PORTY1 parser exists. Original registration name pushes are `0x26157/0x2616f/0x26186/0x2619e/0x261b6/0x261ce` respectively.
- All six callbacks invert the decoder-success gate, not only width/height. PORTY1 `0x2fc37–0x2fc4a` rejects AX=1, sets variable error and returns NULL. Invalid decoder input can fall through to calculation with partially uninitialized locals. Do not execute invalid forms through those callbacks as a grammar oracle; test the original decoder directly. Confirm valid forms through the complete macro pipeline.

Next: expand native decoder matrices for all selectors/optional prefixes/casing, explicitly reject shorthand/misordered/extra fields, assert all six callback identities and valid-form expansion failures, then correct the summaries/syntax and clearly retain the shipped callback defect.

## Completed native grammar matrix

Extended existing `run/riptel-port-probe DLL` to run original decoder `0x2f743` over **7,488 valid combinations**: three current indices 0/1/35 × 39 selectors (all decimal 0..35, CUR, cur and leading-zero 035) × PORT/VIEW in upper/lower case × WORLD/DEVICE in upper/lower case × counts 0/1/2/3. It asserts AX success, decoded slot WORD, type DWORD at +2, domain DWORD at +6 and an untouched two-byte trailing canary. The repeated prefixes explicitly verify that unprovided fields take defaults rather than reading supplied buffer suffixes. For every valid combination all six **original callbacks** return NULL and set variable error +0x436, whether the selected slot is defined or not.

Eighteen direct-decoder rejection cases cover empty/36/negative/base-36-letter/VIEW/WORLD/DEVICE/nondigit selector, the two alleged domain shorthands, empty/unknown type, wrong/empty/unknown domain, misordered fields and a fourth argument. Invalid decoder outputs are never passed into the inverted callbacks. The parser has no alternate PORTY1 signature or domain-in-second-position dispatch. The ordinary macro parser's separate empty-comma collapse is already established in [#6](issue-6-macro-evaluation.md); an empty type placeholder cannot repair a shorthand.

The port probe now passes **53,625 allocation/query/copy checks**. It preserves the original geometry tests. Hook additions for this matrix are CRT atoi at `0x65604` and a fixed 256-entry digit classification table via `0x87908`; original slot validator `0x987b` and numeric predicate `0x65589` execute. Existing hooks are `0x45038/0x343e4/0x342ad/0x34319/0x18cf6/0x18bf7/0x3988f/0x398ee/0x13c69` host/viewport synchronization, `0x33821` controlled protection, `0x3393c` captured port selection, `0x339e4` area accounting, `0x38e47` error capture and `0x65431/0x655e2/0x68440` CRT operations; the prior geometry fixture supplies bounded GDI/rectangle/allocation import stand-ins. Those geometry paths are not reached by the new decoder-success callback cases, which fail before looking up port geometry. No decoder, query callback, variable-error store or macro-dispatch instruction is patched.

Expanded `run/riptel-macro-probe DLL` asserts all six original callback registrations, then passes **60 valid ordinary expansion cases**: six names × bare/CUR/1/35/CUR-PORT/CUR-VIEW/CUR-PORT-WORLD/CUR-PORT-DEVICE/lowercase-CUR-VIEW-DEVICE/leading-zero-035-VIEW-WORLD. Each expression is surrounded by A/B; expansion returns status 1, preserves the **entire expression**, sets the variable error, and reports no host error. It never emits a numeric sentinel or an empty successful value. Total macro probe: **390,071 checks**, preserving earlier coverage; the full ordinary parser, original 179-name registry and dispatcher run within the existing controlled host/memory fixture described in #6/#10. No invalid query expression is executed through expansion.

Reproduction (original DLL only):

```sh
run/riptel-port-probe /path/to/extracted/ripscrip.dll
run/riptel-macro-probe /path/to/extracted/ripscrip.dll
```

Both wrappers verify SHA-256, compile with native `cc -m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`, disable core dumps and delete scoped temporary builds on EXIT. No original bytes are modified on disk; no Wine/GUI; no new persistent captures or temporary files. There were no failed expectations during this issue's native expansion.

## Reference changes and next action

Corrected PORTW Format, PORTH's name and missing TYPE column, and PORTY1 Format to the same three-field form. Added a shared accepted-prefix/default/shorthand contract and an all-six callback warning, with links from every coordinate entry. Existing geometry and verbatim text/assets remain unchanged; numeric prose is explicitly labeled intended semantics. AGENTS.md records the distinction between parser rejection and unsafe inverted-callback fallthrough.

Every #25 subquestion is answered. `run/lint`, `run/check-links` (**0 bad of 5,160 links across 210 files**) and `git diff --check` pass. Delivered in [PR #59](https://github.com/bbs-land/remote-imaging-protocol/pull/59), specification commit `0db4298`, dependent on #57; issue-only comparison starts at `4be4e3a`.
