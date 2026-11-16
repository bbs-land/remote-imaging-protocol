# Issue #3: shipped extended font style derivation

[Research backlog](issue-backlog.md) · [Current command reference](../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style)

## Status and questions

Investigation started 2026-11-16 for [issue #3](https://github.com/bbs-land/remote-imaging-protocol/issues/3). No field semantics have yet been established by this investigation. The existing page contains hypotheses; do not promote them to facts without tracing the original driver.

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

The probe currently passes 30,820 checks: exhaustive encoded rotation domains, full two-digit size/shadow/baseline/nonzero-spacing storage, all one-digit s/f values, all low eleven flag combinations, the requested example, ignored WW/reserved values, protected/invalid commands, all sixteen rotation pairs through the actual ordinary text consumer, and deferred renderer checks using controlled font/OS callbacks. This is not end-to-end client execution or pixel validation. The renderer’s font-loading callback is forced to fail after recording its actual parameters; no inference about successful glyph rasterization comes from that stub.

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

The renderer’s body is `0x40ed0–0x42718`; switch tables follow at `0x4271c–0x4280b`. Inspection of all its stack-argument reads finds no read of original arguments 5, 8 or 9. Consequently s and f are stored compatibility bytes with no rendering effect, not recoverable active selectors. Their original intended meanings are unnecessary to implement this driver. They accept all 64 one-digit values. A temporary Capstone control-flow walk cross-checked this conclusion (13,547 states, all return stack deltas zero); it followed direct branches and each switch-table target, tracked pushes/pops, cdecl versus stdcall cleanup, and excluded reads of overwritten argument slots. The reproducible primary evidence is the complete renderer instruction range, available via `objdump` and `run/riptel-inspect --rva 0x40ed0 --length 6217`; the temporary audit is not a repository dependency.

At `0x41069`, size <=0 fails at render time (“Font size cannot be 0”), although y itself accepts zero. Positive size is multiplied by ten at `0x41097` and passed into font realization (`0x40170`) as tenths of a point. Font DPI is computed at `0x41101–0x4112f` as integer `72 * client dimension / world-frame dimension`, separately for X and Y. Thus nominal size is in world-frame points (one unit per world coordinate at the 72-DPI reference), not physical monitor points or a fixed device-pixel height. Shadow distance is separately scaled at `0x40efc–0x40f23` by drawing-canvas width/world width, truncating and clamping a positive result to at least one.

Typefaces are resolved case-insensitively by `0x3fb50` via the font catalogue. Failure returns -1. At `0x41023–0x41068`, a negative lookup result restores GDI state and returns; it does not select the legacy font at size one. The existing prose’s fallback claim is not the behavior of this shipped driver. Font-loading failure is also a render failure, logged as “Failed to load vector font”; the stored extended-style record remains selected.

## Next step

Download the installer to the external artifact mirror, extract without running the installer, identify its driver format and hashes, and locate the `y` handler and argument descriptor. Record exact commands and findings below before drawing conclusions.

---

[Research backlog](issue-backlog.md) · [Current command reference](../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style)
