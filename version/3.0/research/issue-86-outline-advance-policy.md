# Issue #86: outline advances and cached cursor width

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)

## Scope and checkpoint

Resolve [#86](https://github.com/bbs-land/remote-imaging-protocol/issues/86) on branch `issue-86-outline-advance-policy` from fork stable **5224b70**. #87 user-variable prefix/repetition/scope/transparency follows. Both are open new reports without comments at the 2026-11-19 13:12:10 UTC refresh; no later repository discussion/inline comments were found.

Preserve adopted #24 cursor stores, #3 spacing, #22 unsigned renderer mapping and #82 global bbox definitions. Specify ordinary scalar advances and DPI/face/rounding/kerning, an independent finite unsigned cached-width contract, and cache lifecycle/cursor examples. Use protocol documentation and read-only original metric producers/callers. Explicit protocol rules control; identify genuine gaps and label any proposed interoperability policy instead of requiring native narrowing, stale-memory behavior or unsafe byte-index loops. No original client/UI/unsafe loop is to be executed or debugged.

## Sources and evidence boundary

The original RIPtel 3.1 driver is `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [external artifact catalogue](../../../reference/rip-tools.md): 592,896 bytes, SHA256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. It reports RIPscrip driver 3.0.7, not language 3.1. LLVM objdump 19.1.7 disassembled the unchanged PE32 image, base `0x10000000`; addresses below are RVAs, not file offsets. `.text` file offsets are RVA minus `0xc00`; use `run/riptel-inspect` rather than assuming that mapping applies to other sections.

The preserved [ALPHA 4 record](../../2.0/text/RIPScrip-2.0-alpha-4.txt), 1,202,490 bytes, SHA256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, and current [extended-font reference](../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style) establish nominal points, independent string/character rotation and percentage spacing. Neither specifies every integer-rounding/cache-lifetime case asked here. The adopted [#3](issue-3-extended-font-style.md), [#22](issue-22-fastfont-rendering.md), [#24](issue-24-drawing-state-ownership.md) and [#82](issue-82-rff-global-bounds.md) derivations remain unchanged. Their previous native fixtures are evidence for their stated branches/services, not proof of this new policy's entire metric producer.

No DLL instructions, parser callbacks, client/UI, Wine process or unsafe loop were executed in this investigation. This is read-only source/data analysis followed by an explicitly labeled protocol proposal, as the issue requests. Do not continue by debugging the old loop or attempting pixel parity.

## Read-only trace

| Original RVA | Observation | What it establishes / does not establish |
| --- | --- | --- |
| `0x41101–0x4112f` | Client dimensions times 72 divided by world dimensions stored at vector state `+0x6e/+0x70` | Effective X/Y DPI, distinct from monitor DPI |
| `0x41133–0x4113f` | X copied to `+0x72`, Y to `+0x74` | Initial cached scales; not a complete lifetime guarantee |
| `0x41485`, `0x4149a–0x414ca` | Renderer zero-extends each byte, gets one advance, converts to device units, calls spacing helper and adds per-code result | Unsigned direct mapping and per-code layout; no previous/next code is passed |
| `0x3fcd0–0x3fd8c` | Cached helper reads unsigned WORD `+0x72`; per-code engine result is unsigned AX; float scale uses `1/1440` at RVA `0x76958`; `_ftol` at `0x69fb0` converts each value, then `0x3fd90` adds spacing | Independent cached X-scale/per-byte structure; not whole-string raw scaling |
| `0x3fd2d–0x3fd31` | Index is BL, code is sign-extended into AX | A historical discrepancy from renderer mapping, not a requirement to change adopted unsigned codes |
| `0x3fd6a–0x3fd7e` | Full strlen compared with zero-extended incremented BL | Apparent unsafe long-input behavior; no upstream limit or native long-input execution is claimed |
| `0x4efd0`, `0x527d0–0x5286a` | Per-code advance service; subtract first code WORD at `+0x26`, unsigned comparison with count `+0x28`, cached table lookup | Missing metric returns zero; ordinary scalar service itself does not accept a kerning pair |
| `0x52520–0x52661` | Kind-1 metric producer reads advance-table offsets, can fall back to base metrics; has a synthetic-bold addition; multiplies by descriptor `+4` fixed-point scale, adds `0x8000`, takes high WORD | Native 16.16 and effect arithmetic exist; the complete scale/effect producer was not established as an exact portable formula |
| `0x52680` | Kind-0 producer uses a different layout and `0x54340` | Do not mislabel its offsets as the RFF metrics block |
| `0x54340–0x543a0` | Unsigned WORD product plus floor(WORD divisor/2), unsigned divide, AX return | Half-up integer scaler; not proof that every RFF metric traverses this helper |

Reproduce a bounded disassembly, substituting the catalogue's local artifact root:

```sh
objdump -d --start-address=0x1003fcd0 --stop-address=0x1003fd90 ripscrip.dll
objdump -d --start-address=0x10041470 --stop-address=0x100414d0 ripscrip.dll
objdump -d --start-address=0x10052520 --stop-address=0x10052680 ripscrip.dll
objdump -d --start-address=0x100527d0 --stop-address=0x1005286a ripscrip.dll
objdump -d --start-address=0x10054340 --stop-address=0x100543a1 ripscrip.dll
```

Start at actual instruction boundaries when drawing conclusions; an arbitrary range start can decode a tail byte as a spurious instruction. The renderer's string-direction dispatch and character-orientation comparisons do not, by themselves, prove a complete all-sixteen axis matrix. No further native bug investigation is necessary: the protocol policy states that matrix explicitly.

## Original font inputs

Use MARIN.RFF from the same original extraction: SHA256 `4b0a392baa74a1849b9fabedacc39c1434784370175f7708ea151b851255ed68`. DIXON.RFF, cross-checked independently, has SHA256 `cdd71d2683ee7784b140816df69ff9080044485c92159e716a87693e985969be`. These are original data, not synthetic glyphs or patched binaries.

A bounded Python extraction (little-endian unsigned words) reproduces the inputs:

```python
from pathlib import Path
from struct import unpack_from

b = Path("marin.rff").read_bytes()
u16 = lambda offset: unpack_from("<H", b, offset)[0]
section = u16(0x16) + u16(0x14) * u16(0x2a)
base = u16(section + 2)  # absolute file offset; do not add section again
units, first, count = u16(base + 0x0c), u16(base + 0x0e), u16(base + 0x10)
table = base + u16(base + 0x26)
assert base == 0x212 and units == 1000 and first == 32 and count == 224
for code in (0x41, 0x46, 0x80, 0xff):
    assert first <= code < first + count
    print(hex(code), u16(table + 2 * (code - first)))
```

MARIN yields `722, 556, 167, 1000`; DIXON yields `666, 610, 540, 675`. A first scratch extraction accidentally added the absolute `base` to the section again, yielding impossible units/first-code values; the documented format and the `U=1000/first=32/count=224` invariants exposed it. The corrected extraction above is the source for the published vectors. No font bytes were modified.

## Protocol choices and rejected interpretations

The [new contract](../techspecs/2.4-outline-advance-and-cache.md) states **explicit interoperability policy**, not a falsely exact emulation of all native arithmetic:

1. Convert unsigned font advances using face percentage and whole point size to integer 1/1440-inch units by exact rational half-up rounding, then truncate device conversion **per byte**. Select the string axis DPI for ordinary layout for all sixteen rotations. Character rotation affects ink and the existing horizontal bbox reference, not the base scalar. Synthetic effects are ink-only unless an actual face supplies different metrics. Do not infer a universally exact formula for native bold additions or 16.16 quantization.
2. No automatic pair kerning. The font trailer's possession of pairs is insufficient evidence of ordinary text applying them; the ordinary caller asks for one character at a time. This is also an explicit rule for new implementations.
3. Cached width independently uses unsigned bytes and frozen X DPI, per-byte conversion and the adopted spacing formula. Use wide arithmetic/full-length finite iteration. Do not adopt sign extension, byte-index wrap, native WORD accumulation or a conjectural 255-byte caller cap. Preserve only the already adopted **final signed low-word cursor displacement**. Intermediate modular addition would not justify unsafe per-byte narrowing or rendering wraps.
4. Cache ownership is interpreter-wide, separate from ports/styles and from backend raster storage. Choose an atomic last-successful metric snapshot, cold/reset width zero, failure retention, and refresh on successful accepted outline text preparation even if subsequent output has no ink. Freeze spacing/bbox/DPI together. Selection/world changes alone do not mutate it; classic zero reads it. These lifetime rules are a protocol choice, not a recovered historical promise.

Rejected shortcuts: scaling a whole raw string once; replacing advance with bbox width; rotating cursor storage with the string; treating the controlled 13/37 fixture as a real font oracle; adding kerning because a table exists; claiming native behavior above 255 bytes from static instructions; and using an unspecified native failure cache as the new protocol's definition.

## Verification and completeness

Independent integer/rational calculations check the published MARIN vectors: 90-point `F` gives `r=1001`, 50 device units at 72 DPI and 25 at 36 DPI; condensed gives `r=751`/37; 10-point `FF` at 36 DPI gives 4, while converting the integer engine sum once gives 5. High byte `0x80` gives `r=301`/15, `0xff` gives 1800/90. Spacing at 125 percent gives 62 with normal reference, 75 with supplied realized `B=102`. The `B=102` example is the previously realized horizontal-span fixture; it is not a new guarantee of cross-renderer raster rounding. 260/700 characters produce finite totals 13000/35000, with only the final 35000 cursor handoff becoming -30536.

Cursor vectors distinguish cold failure, last-good width after failed reinitialization/world/style changes, successful reinitialization at new DPI, classic-zero raw anchors and accepted no-ink calls. The ordinary axis table enumerates every string rotation and all four character rotations. All issue subquestions are answered; no unsafe reproduction is needed to establish the proposed protocol policy. Repository Markdown/link/diff checks and delivery status follow below.

## Next action

Specification and derivation are complete. Original MARIN fingerprint/input checks and independent arithmetic vectors pass; `run/lint`, `run/check-links` (0 bad of 5,628 links across 231 files) and `git diff --check` pass. Refresh 2026-11-19 13:28:05 UTC finds no comments on #86/#87 and no newer repository discussion/inline feedback. Publish a complete PR closing only #86, consolidate its actual head into fork stable with ancestry preserved, then start #87. Retain co-author trailers and scoped temporary cleanup.

---

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)
