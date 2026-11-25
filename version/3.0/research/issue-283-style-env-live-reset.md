# Issue #283 — Live STYLE and ENV reset

[Issue #283](https://github.com/bbs-land/remote-imaging-protocol/issues/283) · [Reset reference](../ripscrip/5.1-text-variables.md#live-style-and-env-reset-contract) · [Research backlog](issue-backlog.md)

RealDeuce filed #283 at 04:36:00 UTC on 2026-11-25. The 04:41 UTC in-thread check found it with no comments. Branch `issue-283-style-env-live-reset` starts at fork stable `c30640fad4969ec2e3187fb57fe680f11c8d9ec5`. The report asks for live reset defaults, protection, definition and selection, and the portable treatment of ENV activation and late syntax errors.

[ALPHA 4 §5.9.1](../../2.0/text/RIPScrip-2.0-alpha-4.txt) specifies boot defaults for bare STYLE/ENV, named live and backup selectors, whole-list validation and a no-op for an unresettable target. The verbatim source SHA-256 is `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. The [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The report executed the original macro callback with the existing [table fixture](issue-16-table-lifecycle.md), including controlled UI services. Addresses below are RVAs at image base `0x10000000`. Verification here used the report and read-only instruction and table checks.

## Dispatch and live reset

RESET starts at `0x43d38`. STYLE `0x43056` and ENV `0x43570` pass −1 to the live helper when given only the family. Explicit CUR also passes −1, numeric forwards the decoded slot, and TBL passes −2. ALL calls the live table reset first, then the backup bundle at `0x24660`. The bundle calls BASE, SLOTS and STACK in that order. The numbered backup deletion wrapper checks occupied and protected state before releasing a slot. The two families address their own backup records through IDs 1 and 7.

| Family and operation | Original store and gate |
| --- | --- |
| STYLE CUR | `0x39218–0x39275` reads current index, tests protection bit 0 at record +0x21, copies the fixed 97-byte record from `0x762e8` and sets bit 1 (defined). |
| STYLE numeric | `0x392e1–0x39330` tests target protection, copies the same defaults and sets defined only for target 0. The read-only default record has both flags clear. |
| STYLE TBL | `0x3927a–0x392df` visits 0 through 35, skips protected entries with a report, clears definition for reset entries and sets slot 0 defined. The current index is retained. |
| ENV CUR | `0x3d7ec–0x3d846` reads current index, tests protection bit 1 in the first record byte, initializes the addressed record and sets bit 0 (defined). |
| ENV numeric and TBL | `0x3d848–0x3d8bb` initializes unprotected targets and leaves only slot 0 defined among those reset. The table loop silently skips protected entries. The current index is retained. |

The STYLE defaults were already recovered in [#24](issue-24-drawing-state-ownership.md#defaults-rejected-bootstrap-lead). STYLE's selected entry continues to supply drawing attributes even when undefined. The report's numeric reset followed by `c04` changed the selected foreground to index 4 and left definition clear. The level-0 color handler at `0x1abf0` checks active color precision and current STYLE protection before the attribute store. It uses the selected record independently of the definition flag.

ENV initializer `0x3d5fa` clears the 16-byte record. The stores at `0x3d646–0x3d67b` set the inherited DCS byte to −1, WORLD words at +1 and +3 to 640 and 350, and bytes +5 through +8 to Base Math 36, XY width 2, RGB precision 0 and pointer 0. Activation through `0x3dac0` and the separate active setters belongs to other callers. In the report, active parser config at context+2 → +0x38 through +0x3a remained 64/5/8, and active WORLD at context+0x0e → +0x24/+0x28 remained 1285 × 965 after both CUR and numeric 5. Bare WORLD and COLORMODE read stored defaults. With definition cleared by numeric 5, their numbered queries returned `-1`. The [outer SOFT path](issue-105-outer-soft-reset.md#outer-call-and-selection-ledger) calls ENV selection and explicitly reapplies active settings after the same table reset helper.

## Syntax, protection and portable activation

The report checked numeric slot 7 (protected) followed by CUR (unprotected). Both families report the protected numeric operation and continue to successful CUR without a macro variable error. The case with a late `INVALID` performed the earlier numeric reset and left the variable-error flag clear. In the explicit selector loops, the dispatch at `0x430c8–0x430d0` and `0x435e2–0x435ea` processes each recognized selector. After the call, `0x43137–0x43145` and `0x43651–0x4365f` advance through the list independently of the operation's result.

The portable contract uses ALPHA 4's whole-list validation before writes, matching the rule adopted for [live TW reset](issue-148-live-tw-reset.md). CUR remains symbolic until execution. Live reset and backup deletion keep their own protection checks. The family tables and selector sentinels account for the observed difference between CUR and numeric current reset, so the portable profile retains that difference and its defined/undefined query results.

The draft promises boot defaults for the current ENV. Under the portable completion, apply them immediately whenever this reset reaches the selected entry, including numeric, TBL and the live part of ALL. Preserve its selected index and the selector's definition result. The reset installs new active values directly and does not invoke the selecting operation, which could define an undefined target. This also gives the next command a single consistent Base Math, XY width, color mode and WORLD. The DCS and SDCS metadata continue to follow their established scope and remain separate from ANSI and connection encoding. [ENV ownership and activation](../ripscrip/7.1-data-tables.md#environment-table) · [Character-set lifetime](../ripscrip/5.1-text-variables.md#character-set-metadata-lifetime-and-example).

```sh
sha256sum "$dll" version/2.0/text/RIPScrip-2.0-alpha-4.txt
run/riptel-inspect "$dll" --rva 0x762e8 --length 97
run/riptel-inspect "$dll" --slot 22
objdump -d --start-address=0x100391fd --stop-address=0x1003935b "$dll"
objdump -d --start-address=0x1003d5fa --stop-address=0x1003d6a8 "$dll"
objdump -d --start-address=0x1003d7d1 --stop-address=0x1003d8d0 "$dll"
objdump -d --start-address=0x10043056 --stop-address=0x10043151 "$dll"
objdump -d --start-address=0x10043570 --stop-address=0x1004366b "$dll"
```
