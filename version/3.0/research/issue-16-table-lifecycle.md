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

## Native palette checkpoint

New **run/riptel-table-probe DLL** verifies the original DLL hash, builds a scoped i386 executable and removes it on EXIT with core dumps disabled. FreeBSD 15.1 amd64 / clang 19.1.7, `-m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`, passes **77,948 table lifecycle checks**. It preserves original palette reset/select/query/copy and macro COPY/RESET routines. Palette RGB defaults are a controlled 768-byte fixture reached through the original defaults pointer; this establishes copy/reset behavior, not the boot palette's exact colors. Actual upload is counted, not rasterized.

The matrix covers all 36 current indices against all 36 reset slots and each valid definition/protection combination; all 36×36 copy pairs and source/destination flag combinations; current and all-table resets; undefined selection; macro reset/TBL; multi-destination protection and syntax permutations; protected source copying; destination zero; undefined source and forbidden port live-copy syntax. It compares the complete record array against expected changes and checks surrounding canaries. Native results confirm the palette reset/copy leads above.

The macro's source-INUSE and destination-protection prevalidation is decisive: **undefined source, a protected destination (even undefined-but-protected), self-copy or invalid destination syntax rejects the entire live-entry COPY before any write**. A protected source is allowed. Palette copy includes protection to nonzero writable destinations and clears it at zero. Lower direct-copy acceptance of an undefined-but-protected destination does not describe macro permission: the macro rejects it earlier.

Controlled hooks at this checkpoint: report **0x38e47**, memcpy **0x69e60**, memset **0x68d70**, strcpy/strcat/strlen **0x65431 / 0x65442 / 0x654a8**, case comparison **0x655e2**, decimal conversion **0x65604**, formatting **0x68440**, palette synchronization **0x447c0 / 0x44802**, counted upload **0x3e2e7**, ASCII class pointer **0x87908**. No original lifecycle/validation routine is replaced, and source artifacts stay unchanged. No fixture failure occurred in this checkpoint.

Additional static family tracing establishes why the remaining matrices matter. Live-copy dispatcher **0x18f8c** selects BUT **0x457dc**, STYLE **0x39629**, TW **0xe17e**, PAL **0x446eb** or ENV **0x3db94**. STYLE/TW/ENV copy their record flags and clear copied protection for destination zero. BUT keeps definition/protection outside its 0x1c-byte record: after copying it explicitly sets destination defined and **clears destination protection for every destination** at **0x4585f / 0x45867**. Therefore a universal “source protection is copied” rule would be wrong. Confirm each family natively before publication. TW copies its resident-query string separately at **0xe219–0xe298**; a failed query allocation can still leave a successful record copy with no query, so permission prevalidation must not be called a general transactional guarantee.

## Final native family, reset and backup checkpoint

Expanded **run/riptel-table-probe DLL** passes **80,982 checks**. Original per-family copy consumers and common macro prevalidation now confirm the protection distinction for BUT/STYLE/TW/PAL/ENV, including slot zero, undefined source and protected destination combinations. Obsolete destination resource cleanup and unrelated activation/UI work are controlled; the original acceptance checks, record copying, per-family flag writes and TW query duplication execute unchanged. BUT always clears destination protection; STYLE/TW/PAL/ENV carry source protection to nonzero destinations. Every macro family rejects a protected destination before any copy, including a protected-but-undefined destination.

Original CUR **0x34f7d**, port selection **0x3393c** and screen-port construction **0x3326f** run for all 36 port selections. Undefined nonzero slots become screen ports with (0,0)-(640,480) extents and full-port viewports in this fixture; queries return the selected decimal index. The shared COPY callback nevertheless rejects individual PORT entries. Whole-port backup consumers **0x219a9 / 0x22226**, with bounded in-memory stream services, save and restore both the global query at port-system +0x2a and each screen port's query at record +0x64. The restored strings are independently allocated. Saved protection and selection are restored even over an existing protected live target. This establishes the correct scope of the draft's query-copy sentence; it is not evidence for a live-port copy operation.

The actual SOFT/HARD RESET callbacks and outer routines **0x16220 / 0x16665** execute with unrelated subsystems controlled and all palette operations preserved. SOFT reaches **0x440f0(-2)**, defaulting all unprotected RGB records and clearing nonzero definitions while leaving their protection untouched. HARD additionally calls palette unprotect-all **0x441ed(-2,0)**, resets all records and selects palette zero through **0x442c3(0)**. All nonzero palette slots become undefined/unprotected. SOFT preserves the palette selection index even when the selected palette becomes undefined; HARD selects zero. Neither a specific reset nor TBL defines an unused unprotected slot. A protected specific reset reports an error and leaves the record unchanged; table/soft reset skips protected records.

An original callback sequence selects palette 5, changes a fixture RGB byte, resets 5 and queries it: **INUSE(PAL,5)=0**, **ISPROT(PAL,5)=-1**, **CUR(PAL)=5**. Reselecting 5 defines it with defaults again. Numeric ISPROT first checks INUSE at **0x2dddf–0x2ddee**; it does not expose protection on an undefined slot. Its separate CUR branch **0x2dbc5–0x2dbe2** directly tests protection and can return 0 for the same undefined-but-current slot. Native status combinations confirm that distinction.

## Header correction and failed hypotheses

The probe restores the original bytes of **0x3e2e7** and captures actual SetPaletteEntries/SelectPalette/RealizePalette imports. Header flag 64 uploads the current defined, unprotected palette; all RGB records and flags remain byte-identical. It is not a default reset. Header flag 128 has a second defect: it invokes **0x4460b(-1)**, but that helper does not implement the current-slot sentinel. Its path **0x446b1–0x446c3** sign-extends -1, indexes a record stride of 0x302 and reads the flag word at offset +0x300, hence **two bytes before the record array**. The subsequent protection check does correctly use the current slot.

Two controlled preceding-canary values, **0x1234** and **0x1236**, demonstrate flag 128's upload guard changing with the bit-1 value of that preceding word, independently of the current palette's definition flag; a protected current palette still skips the upload. The read stays within the fixture's owned canary. The earlier issue #5 probe replaced INUSE with an always-true service and could not establish this guard. Its “defaults” label and the current-defined claim are withdrawn by this direct-helper evidence. Correct the header reference and earlier note explicitly in this resolution.

Two harness expectations were corrected during expansion: original backup stream helpers read the **low 16 bits** of the length argument, with stale upper bits possible; matching that ABI fixed the stream-bound assertion. Numeric ISPROT on an undefined slot returns **-1**, as its existing reference says, rather than exposing the raw protection bit. Neither failed assertion was an original crash. The header-128 mismatch was different: tracing it revealed the actual out-of-bounds original read above. Every build directory was cleaned automatically, with core dumps disabled and no GUI.

## Controlled service boundary

In addition to the first checkpoint hooks, per-family fixtures control synchronization/cleanup/activation at **0x3988f / 0x398ee / 0x2badd / 0x2bb24 / 0x391fd / 0x4558d / 0x39ac8 / 0x3c37e / 0xe6e9 / 0x18cf6 / 0x18bf7 / 0x3dac0** and query allocation at **0x65ca3**. The source lists the complete unrelated-reset hook set in `whole_reset_checks`; none of the outer reset or palette operations is replaced. Whole-port stream services are **0x2148a / 0x215a5 / 0x21777 / 0x21543**, plus controlled display activation **0x343e4**. Query allocations are freed explicitly. Original adjacent one-byte synchronization returns are left alone.

GDI imports are controlled and never draw: SetRect, palette uploads/selection/realization, brush/pen creation, object selection/deletion, FillRect and background/text state setters. They establish API arguments and lifecycle/control decisions, not Windows pixel behavior. Actual source bytes on disk remain unchanged; hooks only modify the in-memory mapped service entries.

## Next action

Every issue subquestion is now answered from original consumers. Publish the corrected COPY/current-port/query scope, family-specific protection, palette reset/status truth table and header correction; format and run required checks, then file the complete #16 PR. No PR yet.

---

[Research backlog](issue-backlog.md) · [Tables](../ripscrip/7.1-data-tables.md)
