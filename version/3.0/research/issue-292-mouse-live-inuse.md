# Issue #292 — Live MOUSE occupancy queries

[Issue #292](https://github.com/bbs-land/remote-imaging-protocol/issues/292) · [Live MOUSE aggregates](../ripscrip/5.1-text-variables.md#live-mouse-aggregates) · [Research backlog](issue-backlog.md)

RealDeuce filed #292 at 07:21:14 UTC on 2026-11-25. The 07:25 UTC audit found it, and the issue had no comments at intake. Branch `issue-292-mouse-live-inuse` starts at fork stable `c593e9582f58837fee59ad0bbf6fa7862f15f1bd`.

## Source and portable predicate

[ALPHA 4 §5.14.4](../../2.0/text/RIPScrip-2.0-alpha-4.txt) reserves the direct numeric live selector for numbered families. Its ALL and ANY grammar excludes SCREEN, so it admits MOUSE. The prose describes scanning 1–35, a range belonging to the numbered families. The [mouse table and creation rules](../ripscrip/7.1-data-tables.md#mouse-field-table) instead describe a shared capacity of 128 resident simple fields and mouse-active buttons. The verbatim draft SHA-256 is `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

The portable profile retains the admitted aggregate names and tests the MOUSE pool directly. ANY means one or more live members and ALL means the 128-entry pool is full. This uses the existing resident ownership boundary. An independently saved field contributes only after it becomes live through restore. A style or drawn decoration alone never occupies a mouse slot. Neither visibility nor availability of keyboard or pointer delivery changes residence. The [backup aggregates](issue-207-backup-aggregates.md) remain independent of these queries.

## Original branches

The [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses are RVAs at image base `0x10000000`. Subtract `0xc00` for file offsets in .text and `0x1a00` in .data, which holds these keywords and reply strings. This is a static check of the issue's instruction observations.

- `0x09b0c–0x09b23` compares MOUSE and returns family 5.
- `0x13102–0x1311e` handles that family and rejects a decimal live element through the numeric parser at `0x0987b`. ALL and ANY proceed to the shared branches.
- `0x13147–0x1316c` tests entries 1 through 35 for ALL and stops at the first absent one. `0x131ab–0x131cb` searches the same range for ANY. Each branch calls the shared presence helper `0x45fb1` with mode 1 for live entries. The dispatch at `0x45fe6–0x46003` supports live numbered families 0, 1, 2, 3, 4 and 7. Family 5 falls through to zero at `0x460a2`.
- Because every attempted element reports absent, the ALL branch copies the literal ASCII `0` at `0x79714` on its first test. ANY reaches the false result after testing 1 through 35. The result comes from those probes of numbered slots, independent of the mouse-field count.

```sh
sha256sum "$dll" version/2.0/text/RIPScrip-2.0-alpha-4.txt
objdump -d --start-address=0x10009b0c --stop-address=0x10009b25 "$dll"
objdump -d --start-address=0x10013102 --stop-address=0x100131ec "$dll"
objdump -d --start-address=0x10045fb1 --stop-address=0x100460a7 "$dll"
```
