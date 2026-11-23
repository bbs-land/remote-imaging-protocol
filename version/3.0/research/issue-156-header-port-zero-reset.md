# Issue #156: HEADER PORT 0 initializer fields

[Research backlog](issue-backlog.md) · [HEADER port flags](../ripscrip/1.1-protocol-and-command-hierarchy.md#drawing-port-flags)

## Scope and checkpoint

Started 2026-11-23T01:34Z on `issue-156-header-port-zero-reset` from fork stable **1635d61636220f3e46c3457790a9a7403fe73783**, which already includes #153 / PR #155 actual head a863f45 with identical-tree consolidation. [#156](https://github.com/bbs-land/remote-imaging-protocol/issues/156) was created/updated **2026-11-23T01:28:06Z**, zero comments, body SHA-256 `6815d86d0b407883b884ead4dd9610a17c5ec5d697246e8e00e592c9ffb61b78`. Resolve type-2 bits 1/8 PORT-0 field effects and their gates, keeping separate global SOFT, bounds-only and black-fill rules. No original instructions/probe/UI execution or obsolete-software bug investigation. Continue ten-minute issue monitoring; last check **01:33:57Z**, next about **01:43:57Z**.

## Starting reference hashes

Reproduce with `git show 1635d61636220f3e46c3457790a9a7403fe73783:<path>`:

- `version/3.0/ripscrip/1.1-protocol-and-command-hierarchy.md`: SHA-256 `1f9d72c442a34df77bde7fb0232e0e6daabdd9804d88b0295cd778d23cf6e2fa`.
- `version/3.0/ripscrip/7.0-drawing-ports.md`: SHA-256 `212295228868044a54959f1ff34b63ed55ff8a280e9e1d816f230d8d809ec9e5`.
- `version/3.0/ripscrip/4.1-jpeg-images.md`: SHA-256 `dcddb3f8374207d96787d2d828384ccc84ef4611719d40f863ed5d793eaf3022`.
- `version/3.0/ripscrip/1.3-world-view-virtual-canvas.md`: SHA-256 `9a27ec9015ac8423e19ca6d83094c5bbc1cedda7777ea49682817151ca9d5225`.
- `version/3.0/research/issue-5-scene-headers.md`: SHA-256 `2bc9e8020d8d3103537aa8c16377eee6da1706969fc537746e00150a454718e9`.
- `version/3.0/research/issue-105-outer-soft-reset.md`: SHA-256 `c3c63d2e4de8d6464a1165656fdc07312ab03c9b9e852c2e843ef0e4ddd6a60a`.
- `version/3.0/research/issue-21-image-style.md`: SHA-256 `868317ba2ffa02a20637160716360fd85281377dfcaf6c292788489a11eea4b9`.
- `version/3.0/research/issue-144-remembered-port-copy-pairs.md`: SHA-256 `7d993df415dab411c9d30b0ef80bd17cecc240d6b94a68473ee1ac4776532656`.
- `version/3.0/research/issue-153-header-port-clipping.md`: SHA-256 `94b275d720bc94b604158757e04216af3d3951d65a36defdf06a60fb2b38ddcc`.

## Bounded original-source evidence

The catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`: **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred image base **0x10000000**. LLVM objdump **19.1.7**, read-only disassembly. All addresses below are **RVAs**, not file offsets. No executable is loaded or run.

```sh
llvm-objdump19 --disassemble --x86-asm-syntax=intel --start-address=0x10016ec1 --stop-address=0x100170c8 "$rip156_dll"
llvm-objdump19 --disassemble --x86-asm-syntax=intel --start-address=0x10033d82 --stop-address=0x10033e20 "$rip156_dll"
llvm-objdump19 --disassemble --x86-asm-syntax=intel --start-address=0x10033e20 --stop-address=0x10033f55 "$rip156_dll"
```

Set `rip156_dll` to that catalogued mirror artifact. The first range establishes both actual calls to **0x33d82**: bit 1 after nonzero unprotected deletion and selection 0 (**0x16eee**), and bit 8 after delete-others, only when the logical current index at system+8 is nonzero (**0x16fba–0x16fc2**). Bit 2 selection precedes bit 8; bit 1 precedes both. Neither caller tests PORT-0 activation. Temporary protection preserves bit-8 current definition; existing protection is retained.

The initializer **0x33d82–0x33df2** directly addresses record 0, regardless of logical current. It zeros graphics cursor dwords **+0x5c/+0x60**, image flags word **+0x5a**, and stores image rectangle **+0x4a** as all -1 via SetRect. It also zeros the two transfer-rectangle fields **+0x1a/+0x2a**. The adopted portable pair model uses an absent value after this initializer; this is an explicit completion of #144 lifetime, not a claim that raw zero rectangles encode a public pair-validity bit.

The initializer calls **0x33df3(context,0)**, which copies record-0 full bounds **+0x03** into viewport **+0x3a**. Crucially, it does **not** alter the viewport-disabled flag in **+0x17**: full viewport geometry and activation are separate. Thus retain prior active/inactive state, even though full SOFT has other activation/selection preparation. It zeros a private word at +0x18; no additional public state meaning is invented for it. Its call **0x390e4(context,0,0)** reaches BUT-0 protection bookkeeping (stride 0x61 and flag +0x21), already unprotectable; it is not a pixel fill or PORT activation. Do not misidentify an unrelated table helper as a viewport operation.

The initializer does not touch fixed PORT query storage **+0x64**, backing/full bounds/type, or floating query. Fixed/floating deletion helpers are separately **0x33e40 / 0x33e20**, reached by type-2 flag 32768, not by these initializer calls. No fill/blit/invalidation/whole-table-reset call occurs in the initializer; effects are metadata only. Deleting other port definitions does not repaint their old screen rectangles under existing deletion rules.

## Adopted completion and next action

For either actually invoked stage: PORT 0 remains the existing defined unprotected full-screen port; V becomes B, activation is retained, graphics cursor becomes (0,0), image rectangle becomes raw all-minus-one viewport fallback with flags 0, fixed/floating queries are retained and pair becomes absent. No painting, TW cursor/ANSI reset, other-table reset, backup deletion, global snapshot reset or old-current image-style exception is imported from SOFT. Both invocations have the same PORT-0 field effect; bit 1 selects 0/deletes unprotected nonzero definitions, bit 8 retains logical current/deletes other unprotected nonzero definitions and invokes only if that current remains nonzero. Skipping bit-8 initializer retains every PORT-0 field, including its pair.

## Specification and validation checkpoint

The reference now has a complete PORT-0 initializer table and the three literal vectors: bit 1 with current 5, bit 8 with current 5, and bit 8 starting with current 0. It distinguishes full V from retained activation, graphics cursor from TW/ANSI cursor, raw image fallback from copied V, retained query from the separate query-clear bit, absent pair from a viewport-only edit, and unchanged screen pixels from later explicit black fill. Global-reset/image wording, image lifecycle, pair lifetime, SOFT and #5 notes are reconciled.

Independent inline Python verification passes **nine starting hashes**, **two exact UltraNum header decodes**, **16 current/activation/protection/stage state cases**, the three requested vectors, initializer idempotence and combined stage order (including bit-2 selection to nonzero versus 0 before bit 8). Pixels, floating query, unrelated tables/backup/global state and surviving definitions retain their values. These are abstract protocol checks, not native execution or raster measurements.

The existing fingerprinted inspector rereads source ranges: RVA **0x16ec1**, length **521**, SHA-256 `083ce5074dcabc57693a6d846ee635566ac1e723f027952c36a199ed8f2f258f`; RVA **0x33d82**, length **158**, SHA-256 `0176680fdaa8309cfe8a9910bb9ba192f3fbac07f702edb2a8e4fb57d9b00e1e`. Independent byte assertions decode both relative calls to 0x33d82 and verify cursor/image/private-word stores and four viewport MOVSD bytes. No original instructions execute, no private native validity layout is imposed and no test/dump/temporary file persists.

Full `run/lint`, `run/check-links` and `git diff --check` pass (**0 bad of 6,423 links across 266 files**); exact literals remain outside tables and prose spacing review passes. Pre-publication #156 body is unchanged with zero comments. Publish separately, merge the actual source head into fork stable with identical-tree proof, audit feedback/coverage/ancestry and resume ten-minute monitoring. No original-software bug investigation is required.

## Delivery checkpoint

[PR #157](https://github.com/bbs-land/remote-imaging-protocol/pull/157) is OPEN/non-draft and closes exactly #156. Specification **eb6149cb84e23e92608dd18028a9d903582c0e27**. This metadata checkpoint becomes the complete source head: push, normal-merge its actual commit to fork stable with identical-tree proof, audit all issue coverage/comments/reviews/ancestry and resume the ten-minute watch. No upstream merge/manual closure; temporary API payload directory was removed automatically.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
