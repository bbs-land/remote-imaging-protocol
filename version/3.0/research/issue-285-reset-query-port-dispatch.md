# Issue #285 — RESET PORT and QUERY dispatch

[Issue #285](https://github.com/bbs-land/remote-imaging-protocol/issues/285) · [RESET reference](../ripscrip/5.1-text-variables.md#reset) · [Research backlog](issue-backlog.md)

RealDeuce filed #285 on 2026-11-25 at 06:06:52 UTC. The 06:18 UTC in-thread check found it with no comments. Branch `issue-285-reset-query-port-dispatch` starts at fork stable `beae79de28ecc4ebdebc9dbfb0c47957d2ff8abc`.

## Protocol source

[ALPHA 4 §5.9.1](../../2.0/text/RIPScrip-2.0-alpha-4.txt) supplies both RESET families, PORT at lines 16789–16826 and QUERY at lines 16893–16927. The final paragraph at 16930–16937 specifies prevalidation of every parameter and a no-op for a valid protected target. The verbatim file's SHA-256 is `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

The QUERY forms follow that draft. A bare QUERY or QUERY,ALL addresses both resident families. After TW or VIEW an omitted selector has the same scope as family ALL. CUR is the shared floating expression, TBL visits fixed-entry expressions, and a decimal number addresses any fixed slot 0 through 35. The distinction follows the recovered [fixed and floating storage](issue-51-deferred-query-order.md) and the separate [live TW reset contract](issue-148-live-tw-reset.md). The direct query action preserves records and the independently saved copies. The named families limit the expression set to TW and VIEW, leaving hover expressions to their own lifetime rules.

The draft describes a family keyword followed by a list of selectors. The portable grammar uses a single family per list and rejects a second family keyword. A global ALL is a complete form. Each family ALL removes its floating expression before the fixed entries. The two-family operation visits TW before VIEW. Neither order runs expressions. Protected records and absent targets follow the draft's valid-target no-op rule.

## Original callback boundary

The [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The issue report used the [table probe](issue-16-table-lifecycle.md) and the original ordinary RESET callback. This note cross-checks that report against the table and original instructions. Addresses are RVAs at image base `0x10000000`.

RESET `0x43d38` calls the lookup at `0x43da2` using the keyword table at `0x82428`. Each row contains a name pointer and a callback pointer. The order from table index 8 is PAL, SCREEN, QUERY, PORT, SOFT. QUERY points to `0x43bb6` and PORT to `0x4368f`. At `0x43dd5–0x43dda` the lookup stops when the stored keyword compares greater than the input. SCREEN therefore stops both searches. The RESET caller sets the variable-error flag at `0x43d90`. The report checked QUERY, QUERY/ALL, QUERY/TW/CUR, QUERY/VIEW/TBL and PORT/5 through this public entry. All failed before the corresponding leaf.

The direct QUERY leaf captures below used zero-initialized argument slots and intercepted the four final deletion helpers. The capture therefore shows argument acceptance and requested operations. It gives no native target-level deletion result.

| Callback arguments | Original leaf result |
| --- | --- |
| QUERY, QUERY/ALL, QUERY/TW, QUERY/VIEW | Variable error, no deletion call. |
| QUERY/TW/ALL or QUERY/VIEW/ALL | Variable error, no deletion call. |
| QUERY/TW/CUR or QUERY/VIEW/CUR | Request deletion of the respective floating expression. |
| QUERY/TW/TBL or QUERY/VIEW/TBL | Request deletion of fixed expressions in the respective family. |
| QUERY/TW/5/CUR | Request deletion of fixed TW 5, then floating TW. |
| QUERY/TW/9/10/35 | Request deletion of fixed TW 9 only. Variable error remains clear. |
| QUERY/TW/5/VIEW/CUR or QUERY/TW/5/INVALID | Variable error, no deletion call. |

The dispatch for CUR at `0x43cb2–0x43cc9` calls `0x2d663` for TW or `0x33e20` for VIEW. The fixed helpers `0x2d677` and `0x33e40` receive the numeric target or −2 for TBL. Only the report's tested direct-leaf cases are asserted. The portable profile enables both keyword paths and follows the protocol's documented selectors and protection rules.

```sh
sha256sum "$dll" version/2.0/text/RIPScrip-2.0-alpha-4.txt
run/riptel-inspect "$dll" --rva 0x82428 --length 104
objdump -d --start-address=0x10043bb6 --stop-address=0x10043e0f "$dll"
```
