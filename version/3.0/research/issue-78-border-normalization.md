# Issue #78: SET_BORDER value normalization

[Research backlog](issue-backlog.md) · [SET_BORDER](../ripscrip/2.3-shapes-and-fills.md#rip_set_border)

## Scope and checkpoint

Resolve [#78](https://github.com/bbs-land/remote-imaging-protocol/issues/78) on branch `issue-78-border-normalization` from fork stable **72edbfc**. The issue asks for the two-digit current-base value domain, zero/nonzero normalization, final-field shortening and current STYLE protection. It is independent of the already resolved border drawing operations. Issue #79 (SWITCH_STYLE width/protection flags) is queued next; both new issues have no comments at the 2026-11-18 15:46:29 UTC refresh. No later issue comments appeared after the preceding delivery watermark.

Use the preserved ALPHA 4 protocol documentation and read-only original instruction paths. Explicit protocol rules control; implementation evidence fills missing accepted-domain details. Do not investigate client bugs or launch native/GUI probes. Record exact source identity, normalization/protection branches, independent decode examples, uncertainty and validation before filing a complete issue-specific PR.

## Primary sources and authority

Preserved ALPHA 4 `version/2.0/text/RIPScrip-2.0-alpha-4.txt` is 1,202,490 bytes, SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`. §3.4.1.47 begins at line 7296: `borders:2`, current base math, 01 enabled and 00 disabled. It does not prohibit larger values. Syntax rule 10 permits leading-zero removal from the last numeric field, retaining at least one digit. Only a reserved trailing field may be omitted completely; N's field is not reserved.

The unchanged RIPtel 3.1 / driver 3.0.7 artifact is `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. PE image base is 0x10000000. Addresses below are RVAs unless explicitly labeled file offsets. LLVM objdump 19.1.7 and Python standard-library byte/PE inspection were used; no original instructions were executed and no source/artifact bytes changed.

## Dispatch and radix conflict

Physical slot 48 is a 40-byte record at RVA **0x80fa0**, file offset **0x7f5a0**, in the original table at RVA 0x80820. Byte 0 is level `0`; the unaligned handler pointer at byte 1 is 0x1000dcf1; opcode at byte 15 is `N`; signed argument count at +0x10 is 1; width at +0x14 is 2. The width helper 0x39de0 reads that literal width.

The WORD at descriptor +0x26 is **1**, not the current-base flag combination 3. Radix selector 0x39d70, specifically 0x39da0–0x39dbd, returns MegaNum for bit 0 set with bit 1 clear. The prior line-style descriptor research establishes that return value 1 means MegaNum. Therefore the issue's inference that native N accepts `N&&` as an ordinary current-base UltraNum field is not supported by this dispatch path. There is no need to execute or debug that conflict: the explicit protocol current-base rule controls the public contract. The reference now labels UltraNum examples as protocol expectations and records the original parser's fixed-base choice separately. The nonzero handler result applies to a value already decoded; it does not establish the wire parser's radix.

## Original normalization and protection path

| RVA | Verified instruction/control flow | Contract consequence |
| --- | --- | --- |
| 0xdcfb | Load full parsed DWORD into EDI | Inspect decoded value, not one digit |
| 0xdd0f–0xdd1d | Call current STYLE protection predicate 0x3935b with -1; test AX | Protection gates the update |
| 0xdd1f–0xdd34 | Protected-data diagnostic through 0x38e47, then jump past both stores | Existing border state remains unchanged |
| 0xdd36–0xdd3f | Read current STYLE index; `test edi,edi` | Full zero/nonzero test; no range or low-bit test |
| 0xdd41–0xdd4d | Index 0x61-byte record and store WORD 1 at +0x22 | Every nonzero decoded value enables |
| 0xdd4f–0xdd54 | Same current record, store WORD 0 | Zero disables |

Handler RVA 0xdcf1 maps to file offset **0xd0f1**. The existing [STYLE ownership derivation](issue-24-drawing-state-ownership.md) identifies the style system at context +0x0a, current index at system +8 and record stride 0x61. Current-slot resolution in protection helper 0x3935b uses the -1 sentinel before testing the record flag; slot zero remains unprotectable. N does not switch style or reinterpret its argument as a write mode. These state-setting facts fill a documentary omission without changing the earlier drawing/border-operation contract.

Read-only reproduction, with `artifact_root` set to the persistent mirror:

```sh
objdump -d --start-address=0x1000dcf1 --stop-address=0x1000dd68 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x10039d70 --stop-address=0x10039e01 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x100393b1 --stop-address=0x10039435 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
```

## Independent protocol examples

The current-base rule plus ordinary numeric decoding gives this matrix for an unprotected current style. These are protocol examples, not a native parser/renderer test matrix.

| Input               | Base     | Decoded value   | Stored border |
| ------------------- | -------- | --------------- | ------------- |
| `!\|N00` or `!\|N0` | Either   | 0               | Disabled (0)  |
| `!\|N01` or `!\|N1` | Either   | 1               | Enabled (1)   |
| `!\|N02` or `!\|N2` | Either   | 2               | Enabled (1)   |
| `!\|NZZ`            | MegaNum  | 35×36+35 = 1295 | Enabled (1)   |
| `!\|NZZ`            | UltraNum | 35×64+35 = 2275 | Enabled (1)   |
| `!\|N&&`            | UltraNum | 63×64+63 = 4095 | Enabled (1)   |

The even value 2 independently distinguishes nonzero normalization from bit-0 masking. A protected current style retains its previous 0 or 1 for every row and reports the protection error. N without a numeric digit is outside syntax rule 10's valid shortening; no new missing-argument recovery guarantee is specified.

## Validation and next action

The public 3.0 N entry, command index and composition delta now state the value domain, current-base rule, Boolean storage, shortening and protection gate. No 2.x native value-domain assertion is added without that client's evidence; its preserved documentary 00/01 statements remain valid. No changes to earlier #24/#33 derivations, RIPscrip 1.54, original text or assets are required.

Validation passes repository Markdown lint, 0 bad of 5,531 links across 226 files, `git diff --check`, independent base-36/base-64 arithmetic and manual prose/table review. The 2026-11-18 15:49:37 UTC refresh confirms #78/#79 are open RealDeuce issues without comments. Next: file a complete PR closing only #78, consolidate into fork stable, then start #79 from that baseline. No native fixture or new test script is needed for this read-only documentation change. Temporary API payloads must be scoped and removed.

---

[Research backlog](issue-backlog.md) · [SET_BORDER](../ripscrip/2.3-shapes-and-fills.md#rip_set_border)
