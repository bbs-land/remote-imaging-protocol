# Issue #108: Text-window geometry-query grammar

[Research backlog](issue-backlog.md) · [Geometry variables](../ripscrip/5.1-text-variables.md#twx0)

## Scope and checkpoint

Started 2026-11-20 on `issue-108-text-window-query-grammar` from stable 568d806 after #106 / PR #110 actual head 368fe26 was merged with an identical tree. [Issue #108](https://github.com/bbs-land/remote-imaging-protocol/issues/108) requests one common accepted/default/result contract for TWX0/TWX1/TWY0/TWY1/TWW/TWH, especially omitted TYPE versus explicit TEXT, COLS/ROWS and extended-window cell origins. Preserve existing inactive/undefined sentinels, graphical far edges, first-use/charset/reset contracts, 1.54 and separate FastFont research.

## Sources and method

Original RIPtel 3.1 / RIPscrip driver 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Image base is `0x10000000`; all addresses below are RVAs. LLVM objdump 19.1.7 reads original instructions; section file biases are `.text` minus `0xc00`, `.rdata` minus `0x1000`, `.data` minus `0x1a00`. No original code, callback, ANSI interpreter, GUI or probe is executed.

The immutable [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, §§5.6.16/19–23 (lines 15025, 15228, 15380, 15514, 15658, 15677) is the primary intended query contract. Original `extracted/ripscrip.maf`, 270,945 bytes, SHA-256 `8c0be38eee89b6935ed6f93885c14350f4a6db0a10a72a38d126811bbd090cef`, supplies the example's 800×600 font-2 metrics, 10×24. Preserve all original text/assets and RIPscrip 1.54.

## Contradictions and normative reconciliation

The original TWX1 syntax block includes COLS and defaults explicit TEXT to COLS. Its own following prose allows only WORLD/DEVICE for graphical queries, defaults omitted DOMAIN to WORLD and reserves legacy cell coordinates for omitted TYPE. TWY1 prints WORLD and refers to TWX1. TWW/TWH say the third argument is only useful for BOUND/CELL, then expressly define TEXT,WORLD/DEVICE and matching COLS/ROWS. The shared specification table chooses the coordinate-query prose and all explicit dimension-domain descriptions as one coherent contract. **This is an explicit normative reconciliation of contradictory documentation**, not a claim that the obsolete decoder successfully accepts that whole grammar.

Coordinate queries accept BOUND/TEXT and WORLD/DEVICE, defaulting explicit TYPE to WORLD; none accepts CELL/COLS/ROWS. Omitted TYPE alone requests ordinary cell coordinates; extended windows return −1. Dimension queries accept BOUND/TEXT/CELL, with WORLD/DEVICE for all three; TEXT also permits only the matching count domain (TWW,COLS or TWH,ROWS). TEXT defaults to that count; BOUND/CELL default to WORLD. The early BOUND/CELL-only sentence and the TWX1 COLS/default block are rejected as contradictory draft descriptions.

Original helper `0x2e855–0x2ea0f` corroborates the intended keyword vocabulary and axis capability split, but does **not** prove the reconciled explicit-TEXT position default: it sets TEXT/domain 2 both when TYPE is omitted and when explicit TEXT has no DOMAIN, for all its callers. Its CELL equality branch and caller status gates are also inverted relative to the documented grammar. These bounded observations are sufficient to establish a source limit; investigating or reproducing those client paths is outside this protocol task. No native acceptance or output equivalence is claimed. In particular, do not silently change the public default to reproduce this shared-helper behavior or accept arbitrary words as CELL.

## Registration, argument records and read-only consumers

Registration `0x25c02–0x26c00` maps these names to callbacks. Each calls the argument helper with the capability word below:

| Query | Callback RVA | Capabilities    |
| ----- | ------------ | --------------- |
| TWH   | `0x2e719`    | 3 = CELL + ROWS |
| TWW   | `0x2eaf4`    | 5 = CELL + COLS |
| TWX0  | `0x2ec30`    | 0               |
| TWX1  | `0x2ed32`    | 0               |
| TWY0  | `0x2ee3e`    | 0               |
| TWY1  | `0x2ef4b`    | 0               |

The helper's result record has window u16 at +0, TYPE i32 at +2 (TEXT=0, BOUND=1, CELL=2), DOMAIN i32 at +6 (WORLD=0, DEVICE=1, legacy/count=2). ROWS requires capability bit 2 and TEXT; COLS requires bit 4 and TEXT. All four position callers supply 0, hence expose neither explicit cell-domain keyword. Literal pointers: BOUND `0x7f274`, TEXT `0x7f26c`, CELL `0x7f264`, DEVICE `0x7f25c`, ROWS `0x7f254`, COLS `0x7f24c`, WORLD `0x7e1e4`. These are data RVAs, not file offsets. Helper `0x2ea0f–0x2ea7e` is the adjacent TWHOME wrapper, not this argument parser.

TW metadata: ordinary origin bytes +0x22/+0x23, column/row count +0x24/+0x25; extended marker bit 1 at +0x28; bounding edges +0x02/+0x06/+0x0a/+0x0e; display edges +0x12/+0x16/+0x1a/+0x1e. Position callbacks' legacy path reads the ordinary origin, or origin + count **− 1** for the far coordinate, and rejects extended kind. X1 has `addw` at `0x2edcf` followed by `decw` at `0x2edd2`; Y1 has `addw` at `0x2efe8` followed by `decw` at `0x2efeb`. These are inclusive cell corners. An intermediate reading missed the decrement and incorrectly called the legacy far edge exclusive; rereading both exact ranges corrected that before any specification edit. Do not confuse it with the already documented exclusive graphical far edge.

Graphical position paths read the corresponding stored edge without increment. TWW/TWH subtract right−left or bottom−top for TEXT/BOUND. CELL uses the selected font's actual TW-system width words at +0x4c or height words at +0x56, consistent with the existing MAF producer/consumer derivation in [#101](issue-101-extended-text-window-contract.md). WORLD helper `0x310f2–0x3112e` applies current context ENV values: multiply the queried device value by WORLD extent and divide by device extent, using integer division toward zero. Length is converted once, after device-edge subtraction, not by subtracting separately rounded WORLD positions. `%d` format string RVA `0x7e74c` gives decimal output. The callbacks inspect the requested record without calling the current-window selector; they do not select/create/activate a noncurrent target or move its cursor. Existing inactive/undefined sentinels remain −1 for positions and 0 for dimensions, from the explicit primary protocol text; malformed-call recovery is not a new sentinel.

## Reproduction and remaining work

Read only the original artifact, using for example:

```sh
objdump -d --start-address=0x1002e855 --stop-address=0x1002ea0f ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002edc5 --stop-address=0x1002edda ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002efde --stop-address=0x1002eff4 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x100310f2 --stop-address=0x1003112e ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

Publish the common table and both literal ordinary/extended examples, verify original source fingerprints/field arithmetic/default/domain matrix, run repository Markdown/link checks and review normal prose spacing. Refresh feedback, publish separately, consolidate actual head and audit all coverage. No original/client/UI/probe execution or task temporary files.

---

[Research backlog](issue-backlog.md) · [Geometry variables](../ripscrip/5.1-text-variables.md#twx0)
