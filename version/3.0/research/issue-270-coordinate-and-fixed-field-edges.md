# Issue #270 — Coordinate width and fixed-field receive cases

[Issue #270](https://github.com/bbs-land/remote-imaging-protocol/issues/270) · [Coordinate width](../ripscrip/1.2-math-and-coordinates.md#rip_set_coordinate_size) · [Encoded-stream stub](../ripscrip/5.5-file-transfer-and-queries.md#rip_begin_uuencode_block) · [Dispatcher](../ripscrip/1.1-protocol-and-command-hierarchy.md#numeric-collection-and-dispatch) · [Research backlog](issue-backlog.md)

RealDeuce filed #270 at 22:57:56 UTC on 2026-11-24. The 22:58 UTC in-thread check found it after #269. Branch `issue-270-coordinate-and-fixed-field-edges` starts at fork stable `b13d24a1ea53cb1efed2eff1fe943dc99a037844`. The [fixed-field framing derivation](issue-237-fixed-numeric-dispatch.md) identifies the delimiter wait. The [encoded-stream derivation](issue-21-image-style.md#received-file-cleanup-and-encoded-stream-checkpoint) establishes the scope of the `9U` stub.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The 40-byte descriptors start at RVA `0x80820`, signed widths at offset 20 and flags at offset 38. Addresses below are RVAs at image base `0x10000000`.

| Slot | Opcode | Widths | Flags    | Handler   |
| ---- | ------ | ------ | -------- | --------- |
| 49   | `n`    | 1, 3   | `0x000b` | `0x1f39f` |
| 128  | `9U`   | 2, 4   | `0x000b` | `0x252c0` |

Both descriptors use active Base Math and the no-text reserve flag `0x08`. As for the [article selector](issue-261-column-article-reserves.md), the delimiter can finalize a nonempty shortened preceding value with the reserve omitted. A full or shortened reserve validates its digits and supplies zero. A leading period completes the current field immediately. An interior period reaches invalid-digit handling. Once both fields finish, state 8 ignores surplus.

The `n` handler reads operand 0 at `0x1f3a9`, services pending work, and checks the graphics gate at `0x1f3b4–0x1f3bf`. It checks current ENV protection at `0x1f3c4–0x1f3e6` before evaluating the width. At `0x1f3e8–0x1f3f2` it maps zero to 2 and forwards every nonzero value to `0x3e989`. The helper checks 2–5 at `0x3e990–0x3e99a`, then writes active parser byte `+0x39` and current ENV record byte `+0x06`. Out-of-range values return without those writes or a width diagnostic. A protected current ENV takes the separate protection-error branch for valid and invalid values.

The `9U` stub loads only the type at `0x252c9`, services pending work, tests the unsigned value against 1 at `0x252d3–0x252d6` and reports an invalid-type error for greater values. Types 0 and 1 return after the administrative service without starting encoded-stream processing.

## Zero-argument commands

After resolving a descriptor, `0x3a676–0x3a67a` checks for zero fields. At `0x3a699–0x3a6a5`, text bit `0x04` selects state 5 and its absence selects state 8. Slots 2 (`#`), 4 (`(`), 5 (`)`), 6 (`*`), 15 (`>`) and 26 (`e`) have zero fields and flags zero. Appending `extra` to each leaves no arguments and waits for the delimiter. Slot 0 (`!`) also has zero numeric fields and carries flag `0x04`, so `!extra` passes the text `extra`.

## Staged captures

The original report feeds `!|`, a body and CR in separate calls. No command dispatches before CR. For width examples the report starts with XY width 4 active and saved. Unless indicated, the table uses Base Math 36 and an unprotected environment.

| Body | Arguments at CR and effect |
| --- | --- |
| `n3`, `n3.`, `n3A`, `n3001suffix` | (3, 0). Active and saved width become 3. |
| `n0`, `n.` | (0, 0). Active and saved width become 2. |
| `n1`, `n6`, `nZ001` | (1, 0), (6, 0), (35, 0). Width remains 4. |
| `n`, `n30.` | No dispatch. |
| `n3001`, `n6000` in a protected ENV | (3, 0) or (6, 0), with a protection error and neither width changed. |
| `9U0`, `9U.`, `9U..tail` | (0, 0), accepted by the stub. |
| `9U1`, `9U01`, `9U0101`, `9U01.ignored` | (1, 0), accepted by the stub. |
| `9U02` | (2, 0), invalid-type error. |
| `9U`, `9U010.`, `9U0.0000` | No dispatch. |
| `n3&`, `9U01&` | No dispatch in Base Math 36. In Base Math 64 the reserve digit is valid and supplies zero. |

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 49
run/riptel-inspect "$dll" --slot 128
objdump -d --start-address=0x1001f39f --stop-address=0x1001f3fd "$dll"
objdump -d --start-address=0x1003e989 --stop-address=0x1003e9d4 "$dll"
objdump -d --start-address=0x100252c0 --stop-address=0x100252f2 "$dll"
objdump -d --start-address=0x1003a673 --stop-address=0x1003a6b7 "$dll"
```
