# Issue #79: SWITCH_STYLE width and protection flags

[Research backlog](issue-backlog.md) · [SWITCH_STYLE](../ripscrip/7.1-data-tables.md#rip_switch_style)

## Scope and checkpoint

Resolve [#79](https://github.com/bbs-land/remote-imaging-protocol/issues/79) after #78 was delivered in PR #80. Branch `issue-79-switch-style-flags` starts from fork stable **48767dc**, which merges #80 head acc1cb1 without changing its tree. Verify the one-plus-two numeric fields, current-base/fixed-base boundary, final-field shortening, protection order/conflicts, same-slot case, destination domain, slot-zero diagnostics and graphics-disable scope. Existing #24 defaults/ownership and other completed issue notes remain intact.

Read the original protocol first; use original dispatch/handler instructions for undocumented extension semantics. No native/client bug or GUI investigation is authorized. Record documentary rules, recovered extension behavior and genuinely unspecified states separately.

## Primary sources and authority

Preserved ALPHA 4 `version/2.0/text/RIPScrip-2.0-alpha-4.txt`, 1,202,490 bytes, SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`: §3.4.3.11 begins at line 10915, declares `style_num:1 res:1`, gives `!|2YG0` and permits 36 styles. It does not describe protection flags. Its current-base rule and syntax rule 10 remain applicable. Recovered 3.0 flags are a later command-field reconstruction, not a silent alteration of the preserved 2.x record. Earlier [#24 ownership/default findings](issue-24-drawing-state-ownership.md) remain unchanged.

Original artifact `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, PE base 0x10000000. LLVM objdump 19.1.7 and the existing read-only `run/riptel-inspect` were used. All addresses below are RVAs unless labeled file offsets. No client instructions, new native parser/handler fixture, Wine or GUI were executed.

## Dispatch, radix and shortening

`run/riptel-inspect DLL --slot 121` fingerprints the original and returns opcode Y, handler **0x46e41**, signed count **2**, width descriptors **1,2**, flags **3**. The 40-byte record is RVA **0x81b08**, file offset **0x80108**, in the table beginning at RVA 0x80820. Handler file offset is **0x46241**. The radix selector 0x39d70 with descriptor flags 3 falls through to the current base at parser +0x38; unlike N's fixed-base native descriptor, 2Y's descriptor agrees with the documented current-base rule.

The flags value therefore has ordinary two-digit domain 0–1295 in MegaNum or 0–4095 in UltraNum. Only bits with decimal values 1/2/4/8 are tested. Higher bits are ignored, with no `flags > 15` rejection. Target is one current-base digit; 0–35 is the STYLE domain, so UltraNum can encode out-of-domain targets 36–63.

Full `!|2YG00` means style 16, flags 0, in either base. Syntax rule 10 directly permits shortening its final numeric field to `!|2YG0`; the old one-character final example is valid and does not prove a one-character descriptor. This protocol guarantee does not rely on a new native fixture. The reconstructed flags field is not reserved: its entire omission is outside the valid grammar. There is no third field.

## Verified instruction sequence

| RVA | Original path | Consequence |
| --- | --- | --- |
| 0x46e4c/0x46e4f | Load decoded flags/target as full DWORDs | Separate fields, no low-byte substitution |
| 0x46e51–0x46e57 | Pending-input service 0x45038 | Do not describe later early exits as no possible host effect |
| 0x46e5a–0x46e65 | Read options via context +0x36; nonzero WORD +0x104 exits | Global disabled-graphics gate precedes STYLE changes |
| 0x46e67–0x46e6a | Unsigned target comparison with 36; at least 36 exits | No source flags, selection, destination flags or command range diagnostic |
| 0x46e6c–0x46e93 | Test bit 4 then 8; call 0x390e4 with current-slot sentinel -1 and protect 1 then 0 | Source protect, then unprotect |
| 0x46e96–0x46e9d | Call selector 0x394fe with decoded destination | Destination selected/defaulted before its flags |
| 0x46ea0–0x46ec7 | Test bit 1 then 2; call 0x390e4 with current-slot sentinel -1 and protect 1 then 0 | Destination protect, then unprotect |

None of the four protection calls checks the helper return. There is no handler read of the current viewport's active flag in this sequence. Suppression is the global graphics-processing option, not a claim that an inactive viewport prohibits table switching. Range diagnostics in lower selector 0x394fe are not reachable for a rejected wire target because the handler returns before calling it. This distinguishes wire behavior from independent CUR/selector entry points.

For source equal to destination, the same defined record receives source pair then destination pair. For distinct records the source pair cannot be displaced onto the selected destination. An unused destination receives the previously established fixed defaults during selection, before its destination protection requests. A defined protected destination remains selectable; general table protection does not block selection or unprotecting it. No new resource-failure or backend-recovery contract is inferred.

## Slot zero and diagnostics

Protection helper **0x390e4**, file offset **0x384e4**, resolves -1 to current STYLE at 0x39102. If current is zero and protect is requested, 0x39112–0x39133 reports error through 0x38e47 and returns failure without setting its protection bit. Original strings at RVA 0x80574/0x80560 are `Cannot protect style slot #0` / `styleSlotProtect()`. For zero/unprotect it falls through to 0x39151–0x39159 and clears the bit successfully. For nonzero entries protect sets bit 0 at record +0x21; unprotect clears it. No drawing-attribute overwrite occurs in these bit operations.

The wire handler ignores those returns, so a protect-zero diagnostic is local to its request. Bit 8 still runs after a failed source protect; selection still occurs; bit 2 still runs after a failed destination protect. Source and destination each produce their own protect-zero error if both requests name zero, even when their paired unprotect requests later leave it clear. Thus:

- Source 0 → destination 5, flags 4: one preselection diagnostic, then current becomes 5.
- Source 3 → destination 0, flags 5: source 3 becomes protected, zero is selected, then its protect request diagnoses. Source protection is not rolled back.
- Source/destination 0, flags 5 or 15: two diagnostics in source-then-destination order, with zero current/unprotected at completion.
- Disabled graphics or target at least 36: neither pair is attempted, so no protect-zero diagnostic is produced by those skipped requests.

These are state-transition/control-flow deductions from original instructions, not claims from a newly executed fixture or OS diagnostic display. They fill previously unspecified flag semantics without investigating a client defect.

## Independent state examples and boundaries

The public reference publishes decimal flag cases 12, 3, 6, 9, 4, 5, 15 and 16, covering both pair conflicts, both same-slot precedence directions, source/destination zero, repeated zero diagnostics and ignored higher bits. For an ordinary defined nonzero source/destination with initial protection clear:

```text
source_pair = flags & 12
4 -> protected; 8 or 12 -> unprotected; 0 -> unchanged
select/default target
destination_pair = flags & 3
1 -> protected; 2 or 3 -> unprotected; 0 -> unchanged
```

This compact final-state mapping does not erase the intermediate protect request or its diagnostic. For wire examples, `!|2Y501` selects/protects style 5; `!|2Y503` selects style 5 then protects and unprotects it. In both bases the final two digits 01/03 have the same values. `!|2Ya01` is an UltraNum destination 36 followed by flags 1: ignore it entirely for STYLE selection/protection and issue no command range diagnostic. Do not interpret the ignored command's bit 1 as protecting the previous current STYLE.

Read-only reproduction, with `artifact_root` set to the persistent mirror:

```sh
run/riptel-inspect "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll" --slot 121
objdump -d --start-address=0x10046e41 --stop-address=0x10046ece "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x100390e4 --stop-address=0x100391fd "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x100394fe --stop-address=0x10039574 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
```

## Validation and next action

The 3.0 language entry and inventory now agree on one-plus-two current-base fields and active flags. The entry retains `!|2YG0`, fixed defaults, style ownership and protected-selection behavior while adding source/destination precedence, ignored high bits, domain/suppression and zero diagnostics. The 2.x Markdown reference, original text/assets, RIPscrip 1.54 and #24 derivation remain unchanged. The small #78 dispatch-label correction was made on its own PR #80 branch (ca75dd4) and merged here with ancestry preserved; normalization/radix conclusions are unchanged.

Validation passes Markdown formatting/lint, 0 bad of 5,546 links across 227 files, `git diff --check` and manual numeric/prose/table review. The 2026-11-18 15:56:35 UTC refresh finds no comments on #78/#79 and no newer issue. Next: file a complete PR closing only #79, then consolidate into fork stable. No new native/client tests are needed for this documentation change. Keep API payloads in scoped temporary directories and retain Codex co-author trailers.

---

[Research backlog](issue-backlog.md) · [SWITCH_STYLE](../ripscrip/7.1-data-tables.md#rip_switch_style)
