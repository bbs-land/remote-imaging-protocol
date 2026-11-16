# Issue #27: button-style defaults, validation, geometry and transparency

[Research backlog](issue-backlog.md) · [Buttons](../ripscrip/4.3-buttons.md) · [Button tables](../ripscrip/7.1-data-tables.md#button-style-table)

## Scope and original artifact

Started 2026-11-16 after [PR #60](https://github.com/bbs-land/remote-imaging-protocol/pull/60) delivered #26. Branch `issue-27-button-style-contract` starts at `54e46e2`. [Issue #27](https://github.com/bbs-land/remote-imaging-protocol/issues/27) asks for complete initial/reset values/current/in-use status, one-zero sizing, effect/indent coordinate units, invalid flag combinations and precedence, protected auto-clip record side effects, flags2 bit 128 transparency/fallback/palette semantics, and SBS/RBS PUSH/POP copy/paste correction. Investigate every part; an explicitly bounded unresolved bit contract is permitted by the question only after primary evidence is exhausted.

Primary source: RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`; extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Original artifacts remain byte-exact in the external mirror. PE base `0x10000000`, addresses below are RVAs. Capstone 5.0.7 / clang 19.1.7 / FreeBSD 15.1. Native/static probes only unless an explicitly isolated Xvfb tool becomes essential; never user-desktop Wine. Clean scoped task temporary files promptly.

## Initial primary traces, not yet a completed contract

Dispatch slot 87 is level 1 B, handler **0xb325**, 16 fields `[XY,XY,mega2,mega4,mega2×10,mega1,res5]`. Slot 107 is level 1 U (button creation), handler **0xa952**, seven fields `[XY,XY,XY,XY,mega2,mega1,mega1]`. Slot 112 is level 2 B (button-style selection), handler **0x46d08**. Button system is context +0x16, current index system +0xa, entries pointer +0, stride **0x1c**, in-use bytes system +0xc, protection bytes +0x30.

The fixed default record is `.data` **RVA 0x828e8 / file 0x80ee8**, 28 bytes:

```text
00 00 00 00 02 00 00 03 03 00 0f 00 00 00 0f 00
08 00 07 00 00 00 00 00 0f 00 07 00
```

Interpreting stores in original style handler: width/height WORDs +0/+2 = 0/0; label orientation WORD +4 = 2 (center), whose high byte +5 is separately used as other-port state by old descriptions; flags WORD +6 = **0x0300** (clipboard + bevel, not PLAIN); bevel WORD +8 = 3; label foreground +0xa = 15, background +0xc = 0, bright +0xe = 15, dark +0x10 = 8, surface +0x12 = 7, group WORD +0x14 = 0, flags2 WORD +0x16 = 0, underline +0x18 = 15, corner +0x1a = 7. Verify initialization/reset/default selection and consumers before publishing.

Selector **0x4576f** defaults undefined entries from that record, sets in-use byte 1 and protection 0, then selects. Reset/delete **0x4558d** has distinct branches: table reset copies defaults into unprotected entries and leaves only zero in use; specific nonzero reset copies defaults and clears its in-use byte. **Suspicious current (-1) branch** copies 28 bytes to **system +0xa**, rather than a current entry address, at `0x455cd–0x455d6`; this would overwrite current selection/in-use/protection metadata. Existing #16 table fixtures deliberately controlled this helper and do not establish the defaults or this branch. Execute it in an owned bounded fixture before deciding whether it is an original defect.

Style definition **0xb325–0xb7a3** validates orientation ≤4; colors through original `0x44385`; group <36; flags2 ≤255; at least one type bit in **0x0181**; and current button-style protection via **0x45674**. No exclusive type-bit check is present in this definition. It converts width/height with **0x31084**, bevel with the same helper's X-only form, translates colors with **0x16140**, clears/copies the current 28-byte record and stores low words. If BEVEL (0x0200) is absent it normalizes stored bevel to zero. Flags are not otherwise masked in the visible store path. The handler reads only the first fourteen argument DWORDs and writes orientation as a WORD at +4; the supplied other_port argument is not visibly read. Trace later auto-clip consumers rather than assuming this wire field works. Radio/checkbox and incompatible rendering flags still need consumer validation; source help strings alone do not settle them.

Prior [#8 bitmap derivation](issue-8-bmm-masks.md) establishes keyed loader behavior: explicit transparent-key route supports 1/4/8/24-bit, rejects 16-bit with no draw, and has a 32-bit classifier defect. It does not establish this button flags2 bit's key/source/palette selection or normal/hot fallback. Reuse that exact downstream evidence only after tracing the button caller and testing it.

Next: native default/initializer/reset/style-validator matrices, then trace original button creation geometry, auto-clip and icon/hot-icon transparency consumers. No #27 specification edits or completed answer yet.
