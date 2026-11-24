# Issue #207 — Backup query aggregates

[Research backlog](issue-backlog.md) · [INUSE](../ripscrip/5.1-text-variables.md#inuse) · [ISPROT](../ripscrip/5.1-text-variables.md#isprot)

## Scope and checkpoint

[RealDeuce report #207](https://github.com/bbs-land/remote-imaging-protocol/issues/207), created 2026-11-24 00:50:29 UTC, body SHA-256 `b80a80f0266f58b73b365e514e50fe683a4b2f928f0eda1f6bbc347f69749aa0`, zero comments at intake. Started from fork stable `c6651034555a49ba7f1ab704412cbb240b6ab256` after the 02:01:14Z audit. The native callbacks confirm generation of literal Boolean digits, numbered lookup through S9, and aggregate scans ending before S8. Next reconcile the intended ten-slot profile, BACKUP and live protection predicates, then validate fixture vectors.

## Primary-source trace

The [Alpha 4 record](../../2.0/text/RIPScrip-2.0-alpha-4.txt) describes ten individually addressable backup slots and the all/any predicates. The [previous table-query derivation](issue-16-table-lifecycle.md) verifies the separate CUR/numeric protection behavior. Use the unchanged [PCMicro driver](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses are RVAs from base `0x10000000`, with .text at file offset RVA−0xc00.

- The numbered parser `0x09828–0x0987a` checks the S prefix and the decimal suffix below 10. Shared presence route `0x45fb1` dispatches backup selector mode 0 to `0x24020` at `0x46081`. The helper checks a base sentinel −1 or the numbered range 0–9. It tests the presence byte of the selected record.
- INUSE ALLSLOTS at `0x1320a–0x13227` stops on the first missing slot and reports true when its counter reaches 8. ANYSLOTS at `0x1326d–0x1328a` stops on the first present slot and reports false after S7. BACKUP is matched against `0x7cb90`. It tests BASE at `0x13355`, stack depth at `0x13370 → 0x2409e`, then visits S0–S7 at `0x13387–0x133a4`. The stack-depth helper reads the selected family's WORD at +0xb13. Direct BASE uses the same presence helper at `0x132d3`.
- ISPROT ALLSLOTS and ANYSLOTS at `0x2dcf6–0x2dd23` and `0x2dd6a–0x2dd97` have the same numbered limit. They check existence through `0x45fb1` before reading protection through `0x460a7`. ALL requires both per slot, and ANY skips a missing or unprotected slot. Direct ISPROT(Sn) uses the same two calls at `0x2de34/0x2de50` and returns the literal at `0x7a100` when missing.
- For numbered live families, INUSE ALL and ANY start at index 1 and stop at 36 exclusive (`0x13147–0x131c9`). ISPROT ALL and ANY do the same (`0x2dc11–0x2dcb4`) and additionally test protection after presence. Numeric live ISPROT uses the missing sentinel. All aggregate replies are numeric Boolean ASCII strings, `1\0` at `0x7970c` and `0\0` at `0x79714`; missing direct ISPROT uses `-1\0`. The inherited example spelling FALSE is not the replacement returned by the driver.

## Portable boundary

Count all ten documented numbered slots for the portable aggregate profile, including the numbered branch of BACKUP. Keep the base and stack independent of numbered-only queries, and retain the numeric/live/current distinctions. In the low-eight-only native profile, S8 and S9 still have individual meanings; their omission is specific to the five aggregate loops. No additional native edge path or client behavior was investigated.

```sh
dll=../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
sha256 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10013147 --stop-address=0x10013450 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1002dc11 --stop-address=0x1002de80 "$dll"
```

Map the literal strings through the PE section table. Decode direct targets as call-site RVA +5 plus signed rel32 displacement. No original callback ran.

## Verification

Verified the original DLL fingerprint, five numbered-loop and four live-loop comparisons, ten direct calls, direct slot bounds and literal output bytes. Evaluated both profiles for the published fixtures and all 59,049 modeled numbered-slot presence/protection combinations. Original instructions and client were not executed.

## Resume

Publish the aggregate contract and update the backup-page cross-reference.
