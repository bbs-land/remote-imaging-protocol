# Issue #164: RIP_SCROLL coordinates, clipping and source fill

[Research backlog](issue-backlog.md) · [RIP_SCROLL](../ripscrip/4.0-icons-and-bitmaps.md#rip_scroll)

## Scope and checkpoint

Started 2026-11-23T03:08Z on `issue-164-scroll-contract` from fork stable **d7b49fcb929b98ec8efa792496f985d6d55971fd**, after #161 / PR #163 was merged with an identical source tree. [#164](https://github.com/bbs-land/remote-imaging-protocol/issues/164) created/updated **2026-11-23T03:01:22Z**, zero comments, body SHA-256 `288cc05edd2e5608a930c724e2b4d460027caefd67525fe4268060673962306c`. Complete coordinate decoding/origin, inclusive normalization/alignment, same-line timing, coupled port clipping, exposed-source fill/corner sampling, physical ANSI/blink-plane ownership and pattern phase. Preserve the RIP 1.54 contract and accepted port/pattern/copy-pair rules. Protocol-only; no original instruction/client/UI/provider execution or bug investigation. Last all-author check 03:07:43Z, next about 03:17:43Z.

## Starting reference hashes

Reproduce with `git show d7b49fcb929b98ec8efa792496f985d6d55971fd:<path>`:

- `version/3.0/ripscrip/4.0-icons-and-bitmaps.md`: SHA-256 `0fbc0bbd84611e1e83e8f62d90590f5fb8d7754d10917e7a678b5be39f77df3f`.
- `version/3.0/ripscrip/9.0-command-reference.md`: SHA-256 `48b6edf08667da5857acf1b606ebba6d29377b8f44f996d40249a159014a0b0e`.
- `version/3.0/ripscrip/7.0-drawing-ports.md`: SHA-256 `bb4b9cb71da592d89def383efbbce2f096b43556e0a366f1bc04bab2c538db1a`.
- `version/3.0/ripscrip/2.3-shapes-and-fills.md`: SHA-256 `31ff9ee4d72339e03c438be93c9155542385b0a3fd9fd8e594ad1db9dd3b311b`.
- `version/3.0/research/issue-12-fill-masks.md`: SHA-256 `54e785f1b6270024e4cc7b0bed0343591f49412d54fdb0716daa8cc147763f7c`.
- `version/3.0/research/issue-144-remembered-port-copy-pairs.md`: SHA-256 `7d993df415dab411c9d30b0ef80bd17cecc240d6b94a68473ee1ac4776532656`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

## Fingerprinted source and initial observations

Catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred image base **0x10000000**. LLVM objdump **19.1.7**, static disassembly only; `run/riptel-inspect` fingerprints and maps original bytes without execution. Addresses are **RVAs**, not file offsets. Reproduce disassembly with numeric VA = image base + RVA and `llvm-objdump19 -d --start-address=... --stop-address=...`.

The driver table at **0x80820**, 129 records of 40 bytes, has level 1 G at **slot 95**: handler **0xd7e0**, seven values, types **[255,255,255,255,1,1,255]**, flags **0x3**. Extract with `run/riptel-inspect <original DLL> --rva 0x80820 --length 5160`. Type 255 is current XY width, including the destination; the inherited fixed `dest_line:2` is inconsistent with this record.

Handler **0xd7e0–0xdc57** normalizes X and Y independently via **0x3112e**, converts both source corners and destination Y with **0x31084**, with no viewport-origin addition, and compares the converted destination with normalized converted source top before drawing. The zero reserved field takes the inclusive-to-half-open SetRect path through **0xe72a**. No eight-pixel alignment appears in this handler. Bounded helper/clip/consumer reads remain to establish exact geometry, sampled corner and mode 4 state; physical ANSI/blink-plane completion must be distinguished from the original single-DC evidence.

The ALPHA 4 entry at preserved text lines 10199–10263 documents vertical overlap, no horizontal movement, viewport bypass but drawing-port clipping, and the seven fill meanings. It does not settle the requested clipping/corner edge cases. Do not silently replace its original fixed-width draft record.

## Remaining work

Trace the bounded rectangle/copy/clip helpers and original fill consumers; reconcile the existing adopted fill-phase and physical-plane rules. Publish source-supported rules separately from explicit completion choices, with independently checked unaligned/overlap/port-edge/wire vectors. Update current reference and command index, validate, publish one PR closing #164, normal-merge its actual head into stable with identical-tree proof, audit feedback/coverage/ancestry and resume the ten-minute watch.
