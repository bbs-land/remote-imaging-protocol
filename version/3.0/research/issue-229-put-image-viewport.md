# Issue #229 — PUT_IMAGE source viewport and reserved digit

[Issue #229](https://github.com/bbs-land/remote-imaging-protocol/issues/229) · [PUT_IMAGE](../ripscrip/4.0-icons-and-bitmaps.md#rip_put_image) · [Research backlog](issue-backlog.md)

## Primary record

RealDeuce filed #229 on 2026-11-24 at 14:22:16 UTC. The 14:23 UTC in-thread check found it without comments or a resolution PR. Branch `issue-229-put-image-viewport` starts at fork stable `fbedadb`. The report follows the separate [GET_IMAGE capture contract](issue-42-clipboard-capture-state.md) and [remembered copy pair contract](issue-144-remembered-port-copy-pairs.md).

The [1.54 specification](../../1.54/text/RIPScrip-1.54.txt) at lines 2028–2066 defines the one-digit reserved field and asks authors to send zero. The [2.0 ALPHA 4 specification](../../2.0/text/RIPScrip-2.0-alpha-4.txt) at lines 9887–9946 keeps that requirement, defines the current-base widths and modes 00 through 04, and uses the assigned port's viewport to select the subimage. It defines no receive-side meaning for any reserved value. The deactivation question applies to the retained rectangle after the [viewport setter](issue-21-image-style.md#integration-review-inactive-viewport-wire-setter).

The catalogued [PC Micro RIPtel 3.1 package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. `run/riptel-inspect` reports slot 102 at RVA `0x81810` (file offset `0x7fe10`), opcode `P`, handler RVA `0x0d32f` and operand types `255,255,2,1` (XY, XY, two digits and one digit). LLVM objdump 19.1.7 read the original PE statically. Addresses below are RVAs at image base `0x10000000`. For `.text`, subtract `0xc00` from the RVA to obtain the file offset.

## Source selection and clipping

- All-zero `v` reaches `0x33cf9–0x33d1d` and sets mask `0x02` at current port record offset `0x17`. The jump skips the write to the stored viewport rectangle at offset `0x3a` in `0x33d55–0x33d6a`.
- In the paste handler, `0x0d367 → 0x3445b` calculates the record from the current port selector at port-system offset `0x08` and tests that same mask. A set bit takes the exit at `0x0d3d3` before the paste helper. The handler converts the destination position at `0x0d382` and calls `0x02a00` at `0x0d3b7`, with zero explicit width and height.
- The helper loads the clipboard selector from port-system offset `0x0a` and the current destination from `0x08`. It checks the absent selector `-1` and source definition byte at `0x02a2b–0x02a3c`. It derives extents at `0x02a42–0x02a58` and offsets the source rectangle by the stored viewport origin at `0x02a6d–0x02a77`. For zero explicit dimensions it requests the same extents at the destination anchor at `0x02a7d–0x02ab0`.
- Shared compositor `0x134d0` classifies the equal extents before clipping. Its coupled clipping calls use the destination and source rectangles stored at port offset `0x3a` (`0x13573–0x135b9` and `0x135eb–0x1362b`). These calls use the rectangle values even when the source's viewport flag is inactive. The equal-size branch enters `0x136a9` and calls BitBlt at `0x13749`. This establishes the ordinary in-bounds subrectangle case with no scaling and no source reactivation.

## Reserved-field completion

The report's instruction summary attributes the comparison at `0x0d374` to the third operand. The original instruction compares EBX, loaded from operand four at `0x0d343`. Operand three is loaded into EDI at `0x0d34a` and forwarded at `0x0d3a8–0x0d3b7`. The earlier sender instruction to emit zero avoids the fourth-operand gate in the shipped build. The portable receive rule consumes the complete fourth operand, then ignores its numeric value. This fulfills the requested protocol behavior. Senders emit zero for interoperability. The existing mode table supplies the defined operations independently of the reserved digit.

The [state and wire vector](../ripscrip/4.0-icons-and-bitmaps.md#inactive-clipboard-source-example) uses a known tile in the captured port after capture. This keeps the expected paste independent of the capture resampling described in #42. Its center rectangle has edges `[1,3) × [1,3)` and four distinct samples. With a full active destination, copy to `[10,12) × [20,22)` and publish the associated pair. After deactivating the destination, the second paste leaves the old pair and pixels intact. The nonzero reserved-digit variant is an explicit portable expectation.

Reproduce the instruction reads using the original DLL:

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 102
objdump -d --start-address=0x1000d32f --stop-address=0x1000d3df "$dll"
objdump -d --start-address=0x10002a00 --stop-address=0x10002b19 "$dll"
objdump -d --start-address=0x1003445b --stop-address=0x10034488 "$dll"
objdump -d --start-address=0x10033cf9 --stop-address=0x10033d7b "$dll"
objdump -d --start-address=0x10013456 --stop-address=0x10013763 "$dll"
```

---

[Research backlog](issue-backlog.md) · [PUT_IMAGE](../ripscrip/4.0-icons-and-bitmaps.md#rip_put_image)
