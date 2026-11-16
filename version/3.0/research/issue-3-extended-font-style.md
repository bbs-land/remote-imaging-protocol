# Issue #3: shipped extended font style derivation

[Research backlog](issue-backlog.md) · [Current command reference](../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style)

## Status and questions

Investigation started 2026-11-16 for [issue #3](https://github.com/bbs-land/remote-imaging-protocol/issues/3). The field meanings, validation, ordinary text-consumer mapping and renderer control flow have now been established. The complete answer is delivered in [PR #29](https://github.com/bbs-land/remote-imaging-protocol/pull/29), from branch `issue-3-extended-font-style`. Validation passed; review/merge remains with the maintainers. Continue with issue #4 as directed by the research backlog.

Required answers: each fixed field’s offset, width, radix, role, units, validation, defaults and effects; all style and justification flags; string and character rotation; both `CC` fields; `BB`, `WW` and `1a1a`; reserved fields; font selection/fallback; exact decode of `!|y0000BW1Q080000001a1a000000Marin`; and independently checked rotation cases. Investigate reset/state interactions and invalid-command behavior as needed to make the contract complete.

## Sources and initial observations

- Specification baseline: repository commit `2fb17724b6122a5ad5cd1df38b69b8cce3a7079f`.
- [Issue #2](https://github.com/bbs-land/remote-imaging-protocol/issues/2) offers disassembly leads from a 592,896-byte `RIPSCRIP.DLL`, MD5 `bade8b1f4e467ac7ad4edb2639738d4c`. These are unverified leads, including the claim that `y` always uses base 64.
- The catalogue’s extracted RIPtel tree is absent on this machine. The repository README links a PCMicro-patched MSI; obtain and fingerprint its driver, distinguishing it from original releases.
- Initial validation: `run/check-links` passed (4,477 links across 181 files); `run/lint` passed. Host: FreeBSD 15.1 amd64, Wine 11.0; `7z`, `objdump`, `llvm-objdump`, GDB and `i686-w64-mingw32-gcc` are available. Driver execution has not been verified.

## Artifact recovery (2026-11-16)

Downloaded `https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi` to `artifacts/riptel-3.1-pcmicro/riptel.msi` under the mirror. Extracted `Data1.cab` with `7z e`, then its 358 files with `7z x` into `artifacts/riptel-3.1-pcmicro/extracted/` (flat filenames). No installer was executed.

| Artifact | Bytes | SHA-256 |
| --- | --- | --- |
| `riptel.msi` | 17949184 | `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c` |
| `extracted/ripscrip.dll` | 592896 | `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8` |
| `extracted/riptel.exe` | 206336 | `648fc45524f9bdecb46499710d6e08fbc28958eabd6a36ec80110874699d08de` |

The DLL is PE32 i386, image base `0x10000000`, timestamp 1997-10-16. Its MD5 is exactly `bade8b1f4e467ac7ad4edb2639738d4c`, matching issue #2. This identifies that issue’s artifact; it does not yet establish byte identity with a separately recovered original installer.

## First binary findings (incomplete)

All code addresses below are RVAs, not raw file offsets. `.text` maps RVA to file offset by subtracting `0xc00`; `.data` by subtracting `0x1a00`. Export labels printed by objdump cover unrelated internal functions and must not be used as function identities.

- Dispatch slot 75: table RVA `0x80820`, 40-byte records, entry RVA `0x813d8`; handler RVA `0x1adc0`; 11 numeric fields with widths `1,1,4,2,2,2,2,2,2,2,6`, summing to 26. Record flag word at +38 is `0x000e`; the radix selector still needs independent parser verification.
- The handler reads decoded arguments 0–8; argument 9 (wire offset 18, crib `WW`) and argument 10 (wire offset 20, reserved six characters) are not read in this handler.
- At `0x1ae29–0x1ae75`, arguments 5 and 6 must equal 0, 900, 1800 or 2700. Base-64 `E4`, `S8`, `gC` decode to 900, 1800, 2700; these are tenths of a degree. RIPlib’s current implementation comment suggests scaling after decoding, but this handler performs no such scaling: do not adopt that comment as evidence.
- At `0x1ae75`, argument 8 must be nonzero; error string: “Character spacing percentage cannot be zero!”. `1a` in base 64 is 100. The two adjacent `1a` fields are therefore not proved to be x/y scale; the second is ignored by this handler.
- Successful installation (`0x1af3c–0x1aff6`) writes the current graphics-style record (stride `0x61`): extended mode +0x24=1; arg0 byte +0x51; arg1 byte +0x52; arg2 dword +0x53; arg3 word +0x4f; arg4 word +0x57; arg5 word +0x59; arg6 word +0x5b; arg7 word +0x5d; arg8 word +0x5f. Typeface name is copied to +0x27 with a count of 39, and +0x4e is terminated explicitly.
- Style reset code (`0x67ac–0x6835`) initializes these values to 0,0,0,25,1,0,0,0,100 respectively. Its caller and full-reset applicability remain to be verified.
- The two alignment error checks mask flags with `0x30` and `0xc0` but compare against `0x200` and `0x3000`; both tests are therefore incapable of rejecting an encoded alignment. Exact consumption/effects still require tracing.
- Text-metric consumer (`0x20689–0x2080d`) extracts horizontal alignment `(flags >> 4) & 3`, vertical alignment `((flags >> 6) & 3)+1` when arg4 is nonzero, underline/strike flags 4/8, baseline displacement from arg7, displacement sign flag `0x100`, coordinate-conversion flag `0x200`, and negation of character-spacing percentage under flag `0x400`. **Correction after tracing the callee:** the `0xc0` selector is shadow direction, not vertical justification. The preliminary field-name interpretation here is superseded by the consumer trace below.

## Reproduction tools used so far

`objdump -d --x86-asm-syntax=intel --start-address=0x1001adc0 --stop-address=0x1001b075 <dll>` reproduces the complete handler. `objdump -h <dll>` gives the section mappings.

Cloned `BradHawthorne/riplib` to the external mirror at commit `024032a263649cbd842bcf855ac1fe9238606e6f`. Its `scripts/dll-dispatch-table.py` and Capstone-based `scripts/dll-disasm.py` provide convenient independent table extraction and annotated assembly; each claim above was checked against the actual instruction bytes/disassembly. These external Python helpers are investigative leads; repository-owned repeatable tools will follow the `run/` conventions.

## Original-instruction probe (2026-11-16)

Added `run/riptel-inspect` (Deno; SHA-256, sections, dispatch record, RVA bytes and font-field decoder) and `run/riptel-font-probe` (Bash compilation wrapper with native i386 C routine harness). The latter maps the identified image at its preferred address and executes its original instructions. It changes only in-memory host-call targets, never the artifact on disk. Its C source lists every hook; numeric validation, decoding, radix selection and the ordinary text consumer are original code.

Reproduce from the repository root:

```sh
run/riptel-inspect /usr/home/admin/RIP/rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll --decode 0000BW1Q080000001a1a000000Marin
run/riptel-font-probe /usr/home/admin/RIP/rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

The probe currently passes 31,713 checks: exhaustive encoded rotation domains, full two-digit size/shadow/baseline/nonzero-spacing storage, all one-digit s/f values, all low eleven flag combinations, the requested example, ignored WW/reserved values, protected/invalid commands, all sixteen rotation pairs through the actual ordinary text consumer, and renderer checks using controlled font/OS callbacks, including all sixteen baseline-displacement branches under three justification settings, shadow offsets/colors and s/f independence. This is not end-to-end client execution or pixel validation. The preflight phase forces the font-loading callback to fail after recording its parameters. A separate geometry phase supplies synthetic realized-font metrics and intercepts bitmap output; it verifies placement but not original glyph rasterization. The isolated glyph oracle below separately exercises the unmodified font engine and original Marin font.

## Consumer trace and corrected field interpretation

The ordinary outline-text placement routine is RVA `0x42870`; `RIP_Text` (`0x20169`) and the outline branch of `RIP_TextXY` (`0x20cbc`, call at `0x20efc`) reach it. Its final call at `0x42a30` passes 26 arguments to `0x40ed0`. Capturing that call independently verifies:

| Renderer argument (zero-based) | Value derived from y |
| --- | --- |
| 5, 9 | s and f, respectively; carried through as stored bytes |
| 10 | stored typeface name |
| 11 | face: normal 0, bold 1, italic 2, combined 3, from flags bits 0–1 |
| 12 | flags & 12: underline 4, strike-through 8 |
| 13 | size (whole nominal points) |
| 14 | shadow direction 0 when distance is zero, otherwise 1 + ((flags >> 6) & 3) |
| 15 | shadow distance |
| 16 | string direction: encoded 0,900,1800,2700 → internal 0,3,2,1 |
| 17 | character orientation: encoded 0,900,1800,2700 → internal 0,1,2,3 |
| 18 | zero; no shipped y vertical-justification field |
| 19 | baseline shift, negated by flag 0x100 |
| 20 | spacing percentage, negated by flag 0x400 |
| 21 | (flags >> 4) & 3 |

Flag `0x200` converts the unsigned baseline shift from world to device Y units **when y executes**, using viewport device-height/world-height and integer truncation (helper `0x31084`). The ordinary placement routine uses the stored result, and does not convert it a second time. The text-metric path’s separate conversion must not be confused with ordinary drawing.

The renderer’s body is `0x40ed0–0x42718`; switch tables follow at `0x4271c–0x4280b`. Inspection of all its stack-argument reads finds no read of original arguments 5, 8 or 9. Consequently s and f are stored compatibility bytes with no rendering effect, not recoverable active selectors. Their original intended meanings are unnecessary to implement this driver. They accept all 64 one-digit values. A temporary Capstone control-flow walk cross-checked this conclusion (13,547 states, all return stack deltas zero); it followed direct branches and each switch-table target, tracked pushes/pops, cdecl versus stdcall cleanup, and excluded reads of overwritten argument slots. The reproducible primary evidence is the complete renderer instruction range, available via `objdump` and `run/riptel-inspect <dll> --rva 0x40ed0 --length 6217`; the temporary audit is not a repository dependency.

At `0x41069`, size <=0 fails at render time (“Font size cannot be 0”), although y itself accepts zero. Positive size is multiplied by ten at `0x41097` and passed into font realization (`0x40170`) as tenths of a point. Font DPI is computed at `0x41101–0x4112f` as integer `72 * client dimension / world-frame dimension`, separately for X and Y. Thus nominal size is in world-frame points (one unit per world coordinate at the 72-DPI reference), not physical monitor points or a fixed device-pixel height. Shadow distance is separately scaled at `0x40efc–0x40f23` by drawing-canvas width/world width, truncating and clamping a positive result to at least one.

Typefaces are resolved case-insensitively by `0x3fb50` via the font catalogue. Failure returns -1. At `0x41023–0x41068`, a negative lookup result restores GDI state and returns; it does not select the legacy font at size one. The existing prose’s fallback claim is not the behavior of this shipped driver. Font-loading failure is also a render failure, logged as “Failed to load vector font”; the stored extended-style record remains selected.

## GUI isolation incident and continuation

A prototype Wine font-engine probe loaded the real DLL and successfully initialized its font engine against a reconstructed `runtime/FONTS/` directory. An earlier attempt used the wrong root and displayed font-error dialogs on the user’s desktop. The user reported that these interrupted their work. Both Wine probe sessions have exited, and a process/window check found no remaining probe process or visible dialog. **All future GUI probes must use an explicitly isolated Xvfb display**, as recorded in AGENTS.md; a separate Wine prefix alone does not isolate windows.

Prototype source and diagnostics are preserved outside the checkout at `artifacts/riptel-3.1-pcmicro/font-engine-probe.c` and `.log`. The successful call was `RIPSCRIP.DLL` RVA `0x3ff90` with a synthetic context whose +0x72 configuration points to a buffer containing the runtime root at +0x16; the original routine appends `\FONTS\`. The original catalogue and `.RFF` files were copied byte-exact into that directory. The probe is not yet a repository-owned reproduction tool.

## Baseline, shadow and spacing findings

The geometry probe executes the complete original renderer against a controlled synthetic font and captures calls to its bitmap-output routine. All sixteen baseline cases were repeated under left, right and center justification. The [reference table](../ripscrip/3.0-text-output-and-fonts.md#size-rotation-baseline-and-spacing) records the observed additive offsets; several cases ignore B, so a general perpendicular-displacement rule is false. The relevant branch tree is `0x415b5–0x41e79`, with baseline reads at `0x415fa`, `0x41682`, `0x417d5`, `0x41884`, `0x41985`, `0x41a17` and `0x41c08` (including non-y vertical-alignment paths).

Shadow mode 1–4 produces respectively `(-d,-d)`, `(+d,-d)`, `(-d,+d)`, `(+d,+d)` bitmap offsets. Each glyph gets one shadow copy in the background color before the foreground copy. The glyph branch is `0x41ee8–0x42016`; line decorations use the same signs at `0x40880–0x40a71`. No intermediate extrusion occurs.

The spacing helper at `0x3fd90–0x3fde2` returns `trunc((abs(P)-100)*reference/100)`. Positive P uses the caller’s glyph advance; negative P (flag 0x400) uses the font bounding-box width returned by `0x3fc50`. The normal advance is still added, so this is a constant extra-gap adjustment rather than monospacing. It uses the bounding-box width already returned in device units; an initial suspicion of mixed units was disproved by the unmodified font engine (see below). Ordinary advance values returned by `0x4efd0` are in 1/1440-inch units and are separately converted by DPI/1440 in `0x414a9–0x414c4`.

## Unmodified font-engine rotation oracle

`run/riptel-font-glyphs` now reproduces the four actual glyphs using the unmodified DLL and the shipping Marin font. Its wrapper verifies the DLL hash, copies the catalogue/fonts byte-exact into a temporary FONTS directory, allocates a private Xvfb display with `-displayfd`, creates a private Wine prefix, and invokes Wine only after setting that display explicitly. GUI diagnostics stay isolated; the wrapper tears down its own Wine server and Xvfb on exit. The output directory receives PBM glyphs and `probe.log`.

Additional artifact fingerprints:

| File | Bytes | SHA-256 |
| --- | --- | --- |
| `marin.rff` | 56526 | `4b0a392baa74a1849b9fabedacc39c1434784370175f7708ea151b851255ed68` |
| `atf.cfg` | 4470 | `32117068ab604419dbde9d319ee5e948d329eaccaac21fba1e0800ff52f2d9be` |

```sh
MINGW_CC=/usr/home/admin/mingw-w32/bin/i686-w64-mingw32-gcc run/riptel-font-glyphs /usr/home/admin/RIP/rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll /usr/home/admin/RIP/rip-tools/artifacts/riptel-3.1-pcmicro/extracted /usr/home/admin/RIP/rip-tools/artifacts/riptel-3.1-pcmicro/verified-glyphs
run/riptel-inspect /usr/home/admin/RIP/rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll --glyphs /usr/home/admin/RIP/rip-tools/artifacts/riptel-3.1-pcmicro/verified-glyphs
```

The C probe calls the original initializer `0x3ff90`, resolves Marin through `0x3fb50` (catalogue index 51), selects the renderer’s orientation-to-font-instance mapping 0,3,2,1, realizes 90-point fonts through `0x40170`, and calls the original glyph routine `0x523f0` for `F`. The descriptor argument is the pointer stored at vector-state +0x86, not the adjacent metadata at +0x8a; the bitmap target must supply both stride and height. Early prototype calls using the wrong descriptor or zero height were rejected as failed experiments, not evidence.

| Wire character rotation | Font-instance orientation | Glyph bitmap bounds | Cropped ink dimensions | Overlap with counterclockwise rotation of the upright glyph |
| --- | --- | --- | --- | --- |
| 0 | 0 | `(-7,-22)..(95,82)` | 47×60 | 100% |
| 900 | 3 | `(-82,-7)..(23,96)` | 60×47 | 99.758% at one quarter turn |
| 1800 | 2 | `(-95,-82)..(8,23)` | 47×60 | 99.758% at two quarter turns |
| 2700 | 1 | `(-22,-95)..(83,8)` | 60×47 | 100% at three quarter turns |

The glyph checker crops foreground and applies exact 90-degree lattice rotations, requiring at least 99% intersection-over-union. The two one-pixel rounding differences are properties of the original engine; alternative orientations have less than 22% agreement. Visual inspection of all four F glyphs independently confirms the direction. This establishes counterclockwise character rotations directly, rather than inferring them from engine enumeration names.

The original width query `0x4efd0` returns 1001 for this F. At 72 DPI its ordinary device advance is `trunc(1001 * 72 / 1440) = 50`, while the realized font bounding-box width is already `95 - (-7) = 102` device units. This distinguishes the two reference widths used by the spacing helper and disproves the earlier mixed-unit suspicion.

## Next step

Complete the final source/documentation consistency review, run both repository checks and the original-instruction probes, then file the #3 resolution PR. Do not advance to #4 until the complete PR is filed.

---

[Research backlog](issue-backlog.md) · [Current command reference](../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style)
