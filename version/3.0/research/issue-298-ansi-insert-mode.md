# Issue #298 — ANSI mode 4 insert and replace

[Issue #298](https://github.com/bbs-land/remote-imaging-protocol/issues/298) · [ANSI mode 4](../ripscrip/1.4-terminal-ansi-view.md#ansi-mode-4-insert-and-replace) · [Research backlog](issue-backlog.md)

RealDeuce filed #298 on 2026-11-25 at 16:27:38 UTC. The 16:37 UTC check found it with no comments. Branch `issue-298-ansi-insert-mode` starts at fork stable `2ca7c7a42485a72f52a659a67512e0403f6c160e`.

ECMA-48 fifth edition §7.2.10 identifies ordinary mode 4 as Insertion Replacement Mode. §8.3.64 assigns final `@` to Insert Character (ICH), with an omitted count default of one. RIP's ordinary ANSI profile uses the single-mode arity recovered for mode 2 and the local TW row extent established below. [ECMA-48 fifth edition](https://www.ecma-international.org/wp-content/uploads/ECMA-48_5th_edition_june_1991.pdf) · [Mode 2 arity](issue-289-ansi-keyboard-action-mode.md).

## Original receive and rendering path

Static inspection used the PC Micro RIPtel 3.1 package's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses below are RVAs at image base `0x10000000`. [Artifact catalogue](../../../reference/rip-tools.md).

- The five-byte records at `0x761d5–0x76247` hold the ordinary CSI final bytes and callbacks. Final `@` maps to `0x35681`, `J` to `0x35d64`, and `K` to `0x361e6`. There is no final `b` for REP in this range. The separate private records begin at `0x76248`.
- Ordinary `h` and `l` dispatch to `0x36cba` and `0x36d33`. Both require parser parameter count 1 at parser +0x14. For mode 4, `0x36cfe` sets mask `0x04` and `0x36d77` clears it at selected TW record +0x27.
- The ordinary character writer tests that bit at `0x4d914`, writes zero to the positional count at `0x4d91f`, and calls the same `0x35681` helper before drawing. The helper reads the selected TW column, then requires its active bit at `0x356b1`. It takes the first count if present or one if omitted. The extent helper at `0x09c27` reads the selected TW's full column count at TW +0x24. The comparison at `0x356e9–0x356fb` returns without shifting when no column remains.
- The helper reads the current row via `0x09bc1`, calculates pixel bounds from the cell metrics and TW origin, and chooses between erasing the row suffix and shifting part of it at `0x35764–0x3576c`. The erase branch uses SetRect and FillRect at `0x357a9` and `0x357c1`. The shift calls `0x094e5` at `0x35841`. This helper copies pixels with the `SRCCOPY` BitBlt raster operation and fills the vacated interval. The rectangle spans the cursor through the TW edge and has the height of one text cell.

## Portable boundary and checks

The reference defines insertion by its visible result so implementations can use their own text and graphics representations. Existing pixels within the shifted part move together. The operation clips at the TW's own right edge and may remove the old contents of its last cell. Vertical scrolling still uses the ordinary ANSI scroll rules.

The native character writer calls the helper before its ordinary right-edge processing at `0x4d959` and later. For the portable profile, resolve the glyph's wrap or chop before applying mode 4. This makes insert mode operate once on the row where the glyph is drawn. Explicit ICH stays a direct row edit regardless of mode, with no implicit wrap and no edit for count zero or a cursor already past the last column. ANSI erase is independent of both character writing and ICH. REP support belongs to the profile of the surrounding ANSI implementation. Mode 4 itself adds no new repeat control.

The literal six-column vector checks both shift and overwrite with one cursor. For a two-row edge vector, use width five with rows `ABCDE` and `QRSTU`. Position the cursor in row 1, column 5 and enable insert and wrap. Writing `X` gives `ABCDX` and leaves wrap pending. Writing `Y` next gives row 2 `YQRST` and leaves its cursor in column 2. For explicit ICH, start a five-column row `ABCDE` with the cursor in column 2. In either mode, `CSI 2 @` gives `A··BC` with cursor still in column 2. A subsequent `CSI K` gives `A····`.

```sh
sha256sum "$dll"
objdump -d --start-address=0x10036cba --stop-address=0x10036dac "$dll"
objdump -d --start-address=0x10035681 --stop-address=0x1003585b "$dll"
objdump -d --start-address=0x100094e5 --stop-address=0x100095dc "$dll"
objdump -d --start-address=0x1004d914 --stop-address=0x1004d9ae "$dll"
```
