# Issue #16: Live table copy and reset lifecycle

[Research backlog](issue-backlog.md) · [Tables](../ripscrip/7.1-data-tables.md) · [COPY](../ripscrip/5.1-text-variables.md#copy) · [RESET](../ripscrip/5.1-text-variables.md#reset)

## Scope and source

Started 2026-11-16 after [PR #44](https://github.com/bbs-land/remote-imaging-protocol/pull/44) delivered #15. The refreshed creation-order list selects [issue #16](https://github.com/bbs-land/remote-imaging-protocol/issues/16) next. Branch **issue-16-table-lifecycle** starts at **6a4af0e**. Resolve current-port selection versus forbidden live port-entry copies, the contradictory resident-query copy sentence, palette default/reset definition state, protected/undefined sources, protection copied into destinations including slot zero, and semantic failures in multi-destination COPY. Whole-table backup behavior is related context; do not substitute it for live-entry evidence.

Primary source is unchanged RIPtel 3.1 / RIPscrip driver 3.0.7 `extracted/ripscrip.dll`, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, base **0x10000000**, in [the catalogued mirror](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/`. Addresses below are RVAs, not file offsets. Original text/assets remain byte-exact; use native/static probes and clean up scoped temporary files.

## Original callback registrations

Original initializer **0x25c02** calls registrar **0x2556d** with names and callbacks. A temporary native i386 bridge reused the existing macro probe's image mapping, `prepare`, controlled initializer allocations and registrar, and printed these original registrations; its directory was removed by Python TemporaryDirectory. The same addresses are now asserted reproducibly by **run/riptel-macro-probe DLL**, which passes **66,219 macro/variable checks** after this registration addition. This is a registration checkpoint, not native validation of the lifecycle leads below.

| Name   | Callback RVA |
| ------ | ------------ |
| COPY   | 0x194f5      |
| CUR    | 0x34f7d      |
| RESET  | 0x43d38      |
| INUSE  | 0x12fbd      |
| ISPROT | 0x2daaa      |
| PROT   | 0xe685       |
| UNPROT | 0xe69c       |

## Palette layout and reset/copy leads

Palette system pointer is context **+0x1a**. Its current slot is byte **+4**, record-array pointer **+5**, stride **0x302**. Each record contains **0x300 RGB bytes** then flags at **+0x300**: bit 0 protection and bit 1 defined/in-use, as tested by original helpers **0x44508** / **0x4460b**. Slot zero is treated as always in use by the query helper. These static identities need direct native lifecycle matrices before becoming a published contract.

Reset helper **0x440f0(context, slot)**: slot -2 loops all 36, skips protected records, copies 0x300 default RGB bytes from the pointer at **context+0x72 -> +0x216**, clears defined bit 1, then ensures slot zero is defined. A specific/current (-1) reset rejects a protected record, copies defaults and clears the defined bit for a nonzero record; it does not define an undefined nonzero record. It finishes by refreshing the current palette through **0x3e2e7**. Need native confirmation, plus RESET macro/TBL/SOFT/HARD routes and current selection effects.

Selection **0x442c3** checks INUSE, defaults an undefined record through 0x440f0, sets its defined bit, stores current byte and uploads colors. This distinguishes selecting an unused palette from resetting one.

Live copy helper **0x446eb(context,source,destination)** rejects same/out-of-range entries; an already-defined protected destination fails. It checks source INUSE and fails for an undefined source. The copy length is **0x302**, including flags, so a protected source appears to protect a writable nonzero destination. A destination zero explicitly clears protection after copying. Native confirmation and other table-family consumers are still required; do not generalize palette flags by analogy.

**Correction lead affecting earlier header research:** **0x3e2e7 is a current-palette upload/realization helper**, not a default-RGB initializer. Its body converts the current record's RGB values, calls SetPaletteEntries, SelectPalette and RealizePalette; it never copies bootup RGB or changes defined flags. Header palette flag 64 loops defined/unprotected slots but calls this helper only for the current slot; flag 128 also only uploads the current defined/unprotected slot. The earlier issue #5 note/reference described those calls as resetting defaults. Reconcile those published claims in #16 after the actual palette lifecycle is natively established; do not silently repeat the earlier helper label.

## COPY validation leads

COPY callback **0x194f5** maps table type through **0x19b16**. Numeric/CUR source copies explicitly reject families **2/5/6** (port/mouse/screen) at **0x195ee–0x195fb** before reaching live-entry path **0x19621**. CUR retrieval helper **0x1970a** does have a port branch reading context+0x22 -> current word +8. Thus having a current port is separate from being eligible for live entry copies.

The live path first parses/validates source, then scans destinations through **0x19774** before copying through **0x197df**. Destination prevalidation invokes **0x460a7** and rejects the whole expression if that returns true, alongside invalid destination/self-copy syntax. The actual loop invokes table copy dispatcher **0x18f8c** and ignores each individual return. Trace 0x45fb1/0x460a7 and their defined/protected distinctions before deciding whether semantic permission failures are all-or-nothing or partial; a lower helper's skip alone is not sufficient evidence.

## Next action

Build guarded native palette/table fixtures preserving original reset/select/copy and macro validation. Capture RGB/defined/protection/current changes for defined/undefined/protected slots, destination zero, current/noncurrent reset, TBL/SOFT/HARD, and multi-destination permutations with a protected entry. Trace corresponding live-entry helpers for all copyable general table families. Confirm current-port macro behavior and inspect whole-table port/query backup code to replace the contradictory query sentence exactly. Record harness failures separately from driver behavior, checkpoint results, and publish only after every issue subquestion is answered.

---

[Research backlog](issue-backlog.md) · [Tables](../ripscrip/7.1-data-tables.md)
