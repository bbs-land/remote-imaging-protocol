# Issue #153: HEADER port black fill and bounds-reset gates

[Research backlog](issue-backlog.md) · [HEADER port flags](../ripscrip/1.1-protocol-and-command-hierarchy.md#drawing-port-flags)

## Scope and checkpoint

Started 2026-11-23T00:56:30Z on `issue-153-header-port-clipping` from fork stable **04d1637b6e6af424a7e8e7b5ac43de9669112c5b**, after #152 / PR #154 actual head 48171ad was merged with an identical source tree. [#153](https://github.com/bbs-land/remote-imaging-protocol/issues/153) was created/updated 00:33:53Z, zero comments, body SHA-256 `5b1ceaa9c73ed0bc8c7a97a7f3bc814c3fdd849978a9647028803e736e9abe76`, unchanged at start. Latest issue check 00:58:21Z found no newer report; next due about 01:08:21Z.

Complete type-2 black-fill bounds/color/screen-versus-bitmap semantics, independent clipping state and full-bounds reset eligibility. Bounded read-only protocol-source inspection is permitted here; **no original instructions/client/probe/UI execution, pixel parity or bug investigation**. Preserve #21 image state, #144/#146 pair ownership and existing viewport/drawing rules. Separate completed PR, actual-head stable ancestry, co-author trailers and temporary cleanup remain required.

## Starting reference hashes

Reproduce with `git show 04d1637:<path>`:

- `version/3.0/ripscrip/1.1-protocol-and-command-hierarchy.md`: SHA-256 `b5c1d979e62879a5c3e68bb821a6fe690b0448eb67552e2ed191133d53c21ee7`.
- `version/3.0/ripscrip/7.0-drawing-ports.md`: SHA-256 `5bb26f2b7002e7908b06e22f9ded9359cab875c6c2d1d65c153c244246e658c4`.
- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `d2327efd1b5628a7e1346043dcec85daaafbad783820ad9f053d99523a4334f8`.
- `version/3.0/ripscrip/1.3-world-view-virtual-canvas.md`: SHA-256 `ac68cd504e0ccd1701b5ba7df3f67a944233023b463757d8c14844c02166cfc8`.
- `version/3.0/ripscrip/4.1-jpeg-images.md`: SHA-256 `dcddb3f8374207d96787d2d828384ccc84ef4611719d40f863ed5d793eaf3022`.
- `version/3.0/research/issue-5-scene-headers.md`: SHA-256 `678fee87426ccf7a67e67bd1317f7afc5aec26ce2132b618e3f666c64580c0a2`.
- `version/3.0/research/issue-10-port-geometry.md`: SHA-256 `799617764268f8b2ada3eebe9cbe26e3e8621a812dd1620a971048b369497ee3`.
- `version/3.0/research/issue-21-image-style.md`: SHA-256 `868317ba2ffa02a20637160716360fd85281377dfcaf6c292788489a11eea4b9`.
- `version/3.0/research/issue-144-remembered-port-copy-pairs.md`: SHA-256 `7d993df415dab411c9d30b0ef80bd17cecc240d6b94a68473ee1ac4776532656`.
- `version/3.0/research/issue-146-physical-clip-pairs.md`: SHA-256 `eadc618631d855e82ead80ab7148ad45caefcdb4309dbdef9355d3a224f169c8`.
- `version/3.0/research/issue-152-header-tw-geometry.md`: SHA-256 `fe39da439e407ab456e9c1c043e5a409dc103b067493eb807c176800ed3463b0`.
- `reference/rip-tools.md`: SHA-256 `212df2c90c1af9f061f375709d7c763ade4d82a7e72c0d974d320136ac307d5c`.

## Original-source evidence and bounded inspection

The catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` has **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred image base `0x10000000`. `run/riptel-inspect DLL --verify` independently passes; it is a read-only PE/byte inspector, not a driver runner. LLVM objdump **19.1.7** reads these exact instruction ranges. Addresses below are **RVAs**; objdump commands use image-base-plus-RVA VAs.

```sh
run/riptel-inspect "$rip153_dll" --verify
llvm-objdump19 --disassemble --x86-asm-syntax=intel --start-address=0x1003400d --stop-address=0x10034113 "$rip153_dll"
llvm-objdump19 --disassemble --x86-asm-syntax=intel --start-address=0x10034113 --stop-address=0x10034231 "$rip153_dll"
run/riptel-inspect "$rip153_dll" --rva 0x34489 --length 1
```

Set `rip153_dll` to the catalogued original in the persistent artifact mirror. No dump files or temporary executables were created.

- **0x340bd–0x34112:** current sentinel -1 resolves the logical current port at port-system +8; all sentinel -2 iterates indices 0..35. This is not the separate physical-target deletion path used by header bit 4.
- **0x34113–0x34148:** require defined (`0x338bc`) and active (`0x33fb0`). There is no protection predicate. Target record stride is 0x78.
- **0x34155–0x34201:** screen/type-byte-1 branch constructs/selects a region from the target full rectangle **record+0x03**, selects it on shared screen DC, obtains stock brush **4**, fills that same rectangle and invalidates it. It removes the temporary region handle, not the selected DC copy; current text-caret hide/show is presentation cleanup, not a stored text reset.
- **0x34203–0x34230:** bitmap branch fills the same full record rectangle with stock brush 4 on the target record's DC, invalidates and returns. It has no corresponding clip-region selection in this leaf. Therefore byte inspection alone does not define all inherited offscreen-DC clip history or exact native pixel coverage.
- **0x3400d–0x34066:** bounds-reset leaf requires defined, active and unprotected (`0x33821`), copies a 16-byte system-held rectangle at port-system+0x32 into target viewport +0x3a, and reaches **0x34489**, a single-byte **C3 RET**. It does not write activation. The existing reference's **full-port-bounds** rectangle rule is retained as protocol; this bounded gate review does not newly claim that an undecoded private source buffer equals every target's full rectangle or investigate its provenance as a software defect.

Microsoft's primary API references explain the inspected boundary: [SelectClipRgn](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectcliprgn) keeps a DC copy after the region handle is deleted; [FillRect](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-fillrect) uses the supplied brush and excludes right/bottom edges; [GetStockObject](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getstockobject) identifies BLACK_BRUSH; [clipping regions](https://learn.microsoft.com/en-us/windows/win32/gdi/clipping-regions) describes DC clip state. These primary API contracts explain implementation mechanics, not vintage palette-device parity or a requirement to expose private DC state.

## Explicit portable completion and adopted inputs

For flags 8192/16384, take each eligible **defined, active** port's full DEVICE bounds B (record full rectangle, not its narrower stored viewport V), intersect B with its physical backing surface and clear that region to **RGB black (0,0,0)**. Protection does not block this content operation. Both screen and bitmap ports use that full-bounds operation; overriding inherited bitmap-DC clip history is an explicit portable completion, not an unmeasured native bitmap coverage claim. Screen ports clear their corresponding physical screen region; bitmap ports change only their own storage and do not implicitly copy to the screen. Black follows the adopted fixed-black clear model, not stored current PAL index 0, STYLE/ANSI background or W/fill, and no palette record is modified to manufacture it.

Fill retains logical current selection, port geometry/V/activation/protection, graphic/text cursors, queries, stored image-style fields, remembered copy pair and unrelated tables/backups. It is ordinary erasure, not a named copy-pair producer. There is **no extra independently observable protocol clipping state**: temporary/backend regions are implementation details; subsequent operations use their own port's existing stored viewport and physical bounds. A point outside unchanged V is still clipped after this full-B clear. This explicit scope avoids adding a hidden DC-history variable, mask, scene buffer or backup family. Do not investigate where a historical clip leak might be visible.

Flags 2048/4096 retain the existing full-port-bounds geometry rule but only for **defined, active, unprotected** targets at that stage. They do not activate an inactive target or paint. A prior activation bit may make the later reset eligible. Live RESET(VIEW) separately activates its target under its established contract; do not alias it to the gated HEADER leaf. Stored image-style state is not reset as by SOFT; normal viewport-fallback interpretation still follows any newly applied V. Pair offsets survive ordinary viewport edits under #144/#146.

The requested literal `!|h01000001204000` is revision 1/global flags 0/one type-2 segment/flags 16384/current selector with slot field 0 unused for selection. Use the same bytes with a current screen port B=(10,20;18,24), V=(12,21;15,23), and a current 8×4 bitmap B=(0,0;8,4), V=(2,1;5,3). With nonblack PAL entry 0, every cell of B∩physical backing becomes black even outside V; metadata remains unchanged. Screen clear affects only its screen B, bitmap clear leaves the screen alone. These are portable logical color/extent vectors, not original-client executions.

## Next action

Publish complete field/gate/color/clipping and paired screen/bitmap vectors, explicitly label the portable completion versus static facts, verify wire/geometry/eligibility/state invariants and source bytes, format/lint/link/diff and refresh #153. Publish separately, normal-merge actual source head with identical tree, audit all feedback/coverage/ancestry and resume ten-minute monitoring.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
