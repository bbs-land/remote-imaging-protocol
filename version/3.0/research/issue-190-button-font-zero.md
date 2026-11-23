# Issue #190 - Classic font zero in button labels

[Backlog](issue-backlog.md) · [Button metrics](../ripscrip/4.3-buttons.md#font-metrics-and-descenders) · [Issue #190](https://github.com/bbs-land/remote-imaging-protocol/issues/190)

RealDeuce filed #190 at **2026-11-23T21:01:24Z**, body SHA-256 `36cd0788f4c8a7a8c49e696630d6cf23d918313bf4db4b08bc6aaf1d288bebae`. The **21:05:00Z** audit found #190 then [#191](https://github.com/bbs-land/remote-imaging-protocol/issues/191), filed **21:01:29Z**, body SHA-256 `56f8b49c6f54948d5140c4dd76f9e53e16209b2e1aa267dd136366b034f63251`. Both had zero comments. [#192 and its closure comment](https://github.com/bbs-land/remote-imaging-protocol/issues/192#issuecomment-7113914226) supply the combined source leads and mark the two individual reports as the active work. No new review or inline feedback appeared. The branch `issue-190-button-font-zero` starts at **588782f47aad0d71cc4186796f3d91ce46b15b92**. The previous TODO checkpoint retained a completed #187 line, removed here.

## Sources

Use the fingerprinted original PC Micro RIPtel 3.1 `ripscrip.dll` (592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`) and `default.rff` (31,596 bytes, SHA-256 `c734c387ae1902335de818249025c6fcf6d28322eb13dfc82737f2503827706c`). [#187](issue-187-classic-font-zero.md) identifies the artifact path, face record, advances and point/spacing tables. [#27](issue-27-button-style-contract.md) establishes captured creation WORLD and original-face geometry. Original ALPHA 4 `3.4.2 and the [button style reference](../ripscrip/4.3-buttons.md#flags-field-1) prescribe the button-specific placement and effect controls. RIPaint 2 `Rpaint2/RIPaint2/RIP/ACC.RIP`matches the report SHA-256`742015fad9e69b8f0ffd189e1650ccce16720b8e6f20b87f40475c055e23b2a4`. The original client and graphics providers were not run.

All addresses below are RVAs. For LLVM 19.1.7 add the preferred base `0x10000000`. With `DLL` set to the original artifact:

```sh
sha256 -q "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x1002b0a9 --stop-address=0x1002ba85 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10040d70 --stop-address=0x10040ec1 "$DLL"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10042044 --stop-address=0x10042231 "$DLL"
```

## Measurement and placement

- The captured font descriptor is DWORD `field+0x2e`. Classic bit `0x800` and a **nonzero** low font nibble enter the stroke measurement at `0x2b0bd`. Font zero instead sets the creation WORLD in the outline engine via `0x2b159`, looks up spacing with `0x2b18a → 0x1000`, and reads nominal points at `0x2b19f` using captured size. `0x2b1ad` passes `DEFAULT EX` to `0x2b1be → 0x40ed0` with a NULL string for metric preparation.
- `0x2b1c9 → 0x3fc30` reads the prepared font’s baseline-height metric (`+0xca`) into frame local `-0xc`. `0x2b1d8 → 0x3fc40` reads its descender metric (`+0xcc`) into `-0x10`. `0x2b237 → 0x3fcd0` measures the expanded text using the prepared outline X advances and spacing. `0x2b256–0x2b278` convert width, height and descender values into creation WORLD. `0x2b280–0x2b2df` convert face bounds and face spans separately. `0x2b30b–0x2b37f` resolve inside or outside X, with left-flag precedence and 8-WORLD-unit side spacing. The center branch truncates half the signed difference between face width and label width.
- `0x2b3a3` and `0x2b3e9` test `0x65a39` for descenders, gated on flags-1 bit 8192. Its ASCII table bytes at `0x86ea0` match the existing `low_char` table: Q, g, j, p, q, y. For font zero the descender metric, when selected, adds to the measured height. Native center-case handling shares this test for orientations 1/2/3. The portable rule keeps the published exclusion for side labels. Above uses top minus height minus 5 WORLD, below uses bottom plus 5 WORLD, and center/side use top plus half the height residual. `0x2b936` and `0x2b949` convert the baseline-height offset and placement back to DEVICE independently.
- At `0x2b466–0x2b471` the three classic split/ordinary calls to `0x21a0` at `0x2b56b/0x2b5ce/0x2b621` are reached only for **classic fonts 1–10**. `0x2b57f` measures the CHR prefix for that split. Code inside that branch that mentions a zero font does not prove an eligible font-zero path. Font zero goes to `0x2b91c` and `0x2ba32` uses the same spacing row/size as measurement. `0x2ba5a` supplies the face again. The `0x2ba7b → 0x40ed0` call supplies one complete label at the baseline position, with string and character rotations zero and left baseline alignment. Style `Y` direction, justifications and shadow are not the button placement/effect controls.
- For a portable profile use the shared per-byte DEFAULT EX width including the final gap. Use design references 750 above the baseline and 250 below it, each scaled with nominal points and effective Y DPI and truncated in DEVICE. This fixes deterministic height, centering and pen positions without requiring hinted metric rounding. On a requested full redraw expand the source label once under the existing provider rules and measure the result with the instance’s captured font size and WORLD. This does not use whatever font happens to be selected at redraw time.

## Decoration

For the direct FastFont call, `0x2b96e–0x2b98c` maps underline/highlight bits to renderer arg 23: none 0, underline 1, highlight 2, both 3. The key byte is arg 22, underline/highlight color arg 24. `0x42044–0x42076` takes a nonzero hotkey and `0x40e70` matches case-insensitively, latching the first match. `0x420b1` and `0x4213f` retain the logical run and use `0x40d70` for underline. The upright helper at `0x40d9b` draws at baseline +3 DEVICE, from the matched glyph origin to that origin plus its **unspaced** device advance. Highlight recolors only the matched glyph. It does not alter its size or spacing. Prefix, selected byte and suffix come from the one measured run. Outline underline uses the baseline offset even when the older CHR descender test would lower its own mark.

For the original `ACC.RIP` world/device 1024×768 and size 2, Q=22, P=109, both effective DPI values 72. The face rectangle is `[32,300)×[176,204)`, and each of the 17 bytes has 500 font-unit advance. The stated portable policy gives 15 unspaced pixels, one gap, total 272, X=30, top Y=182, baseline Y=198. The nominal run is slightly wider than the face and remains centered. The inherited bitmap calculation gives 408. Hotkey N first matches the lowercase n in Phone. The eight-byte prefix advances 128, the glyph is at X=158 and its 15-pixel underline spans `[158,173)` at Y=201. These are protocol arithmetic, independent of ink bounds and the report's downstream capture.

## Verification

Verified 18 original call targets, branch and renderer arguments, DLL and RFF fingerprints, the ASCII descender table, and the ACC.RIP sample hash and wire fields. Independent arithmetic reproduced the centered label, descent-adjusted height, and first match, absent match and same-letter hotkey vectors. Formatting, links and diff checks passed.

## Resume

Apply the button-specific scope, placement, width and hotkey rules to `4.3`. Validate the reproduction and decoration vector, deliver #190 and consolidate before #191.

## Delivery

[PR #194](https://github.com/bbs-land/remote-imaging-protocol/pull/194) is open for upstream review. Specification commit **938f6e13daafe683b314ee66247166a40d863cc1**.
