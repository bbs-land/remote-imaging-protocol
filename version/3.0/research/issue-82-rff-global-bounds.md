# Issue #82: FastFont global bounding-box fields

[Research backlog](issue-backlog.md) · [FastFont format](../../2.0/techspecs/3.2-fastfont-fonts.md)

## Scope and checkpoint

Resolve [#82](https://github.com/bbs-land/remote-imaging-protocol/issues/82) on branch `issue-82-rff-global-bounds` from fork stable **a529850**. The report identifies metrics words +0x16/+0x18/+0x1a/+0x1c as signed xmin/ymin/xmax/ymax, with reordered copies in RFF/FF1 headers. Confirm field identity/signedness through original metric consumers and framed glyph-record unions, correct the 2.0/3.0 labels, and explain the horizontal reference used with flag 0x400 without changing resolved font rendering behavior.

Refresh 2026-11-19 01:42:05 UTC finds no #82 comments or later repository issue/inline comments after the preceding delivery. All prior issue PRs remain separate. Use original byte inspection and read-only instruction tracing; do not execute fonts/client code, launch GUI probes or investigate obsolete client bugs. Record original artifact hashes, offsets/RVAs, reproducible comparisons, derivation and any limits incrementally.

## Original sources and byte verification

RIPtel 3.1 / RIPscrip driver 3.0.7 original DLL: `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` under the [catalogued mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The eight RFF payloads came from the same unchanged extracted distribution; each hash was compared with its committed 3.0 asset LFS OID. The five FF1 payloads were selectively fetched from the repository's existing pinned LFS objects and read from the local LFS cache, verifying their OIDs. No asset was smudged or rewritten and no original code/font was executed.

| Original file | Bytes | SHA-256 / committed asset OID |
| --- | --- | --- |
| BRUSH.RFF | 87,155 | `8abacec6fc4c0d9d5f89cf2d546c6155aa71c0d289409ee2d2f04583133e54c2` |
| COBB.RFF | 62,725 | `fecffc9273a5d201092c019a3d856a708b9d5ca6438b7c89028903d630ef6014` |
| DEFAULT.RFF | 31,596 | `c734c387ae1902335de818249025c6fcf6d28322eb13dfc82737f2503827706c` |
| DIXON.RFF | 42,071 | `cdd71d2683ee7784b140816df69ff9080044485c92159e716a87693e985969be` |
| EUREKA.RFF | 70,789 | `fb0abb6dad57865cdb2356e3a99c9f54a4d77dc68d3b8bef0404488a0e2bae2e` |
| MARIN.RFF | 56,526 | `4b0a392baa74a1849b9fabedacc39c1434784370175f7708ea151b851255ed68` |
| OAKLAND.RFF | 64,213 | `cf70f1c6388b6221554a614cf79097dd07ed2333b3dd153a0886be959b99966e` |
| SYMBOL.RFF | 43,601 | `beb264463aec8b9008f2497acdd9edaacdaf5f07b0aea4c0d736e0036e3e000b` |
| COBB.FF1 | 62,311 | `d92e92070ff909745d7658e50b35237bf8b8618a1f8f2834f3dffb4b694224ec` |
| DEFAULT.FF1 | 31,228 | `33558f13dbf220fc8be27c5c861ce624aafc106521fe22df29a6d3574f9c52fa` |
| DIXON.FF1 | 41,657 | `f5d6759743086742c3705c94c7e45f133134e74532e5c6e5525f93c799510efc` |
| MARIN.FF1 | 56,112 | `e143925fc3b45c132d568114fb542c3276f39e1138a96da8af7f0b7e31ac404a` |
| SYMBOL.FF1 | 43,187 | `f96c48afb6ffe8fe8e7ccb2fb1592456affae6f5b3990674a60ea9221b112914` |

The original 4,470-byte `atf.cfg` (SHA-256 `32117068ab604419dbde9d319ee5e948d329eaccaac21fba1e0800ff52f2d9be`) also contains exact 54-byte copies of all eight RFF headers at byte offsets 30, 108, 186, 264, 342, 420, 498, 576 in family order BRUSH/COBB/DEFAULT/DIXON/EUREKA/MARIN/OAKLAND/SYMBOL. Its copied fields inherit the same meanings.

The five FF1 objects total 234,495 bytes. Selective retrieval used Git LFS 3.8.1:

```sh
git lfs fetch --include='version/2.0/assets/fonts/*.FF1' --exclude='' fork stable
```

These are pinned original source assets retained in the normal LFS cache, not temporary probe output. RFF metrics base is 0x212 in all eight files; FF1 metrics base is 0x74 except DEFAULT at 0xa2. Across all thirteen files, the four signed metrics words, the corresponding signed header copies and the framed-glyph bbox union agree exactly.

Each non-SYMBOL file has 224 framed records. Each SYMBOL version contributes 219 framed records; slots 251–255 fail the bounded tag/length framing predicate. The union comparison is limited to framed records and is **not** a replacement strict path/opcode/raster validation count. Previously recorded malformed-tail behavior is outside this request; no defect investigation was performed.

The exact bounded comparison is reproducible from a regular checkout root, after the selective LFS fetch above. Supply the original extracted RFF directory as the first script argument. Its read-only Python is:

```python
import hashlib, struct, sys
from pathlib import Path

paths = sorted(Path(sys.argv[1]).glob("*.rff"))
paths += sorted(Path("version/2.0/assets/fonts").glob("*.FF1"))
for path in paths:
    data = path.read_bytes()
    if data.startswith(b"version https://git-lfs"):
        oid = next(line.split(":", 1)[1] for line in data.decode().splitlines()
                   if line.startswith("oid sha256:"))
        data = (Path(".git/lfs/objects") / oid[:2] / oid[2:4] / oid).read_bytes()
        assert hashlib.sha256(data).hexdigest() == oid
    u16 = lambda at: struct.unpack_from("<H", data, at)[0]
    ff1 = data[:4] == bytes.fromhex("bd7a0001")
    section = u16(0x22) if ff1 else u16(0x16) + u16(0x14) * u16(0x2a)
    base = u16(section + 2)
    assert u16(base) == 0xf31e
    bbox = struct.unpack_from("<hhhh", data, base + 0x16)
    header = (struct.unpack_from("<hhhh", data, 0x12) if ff1 else
              tuple(struct.unpack_from("<h", data, at)[0]
                    for at in (0x22, 0x20, 0x1e, 0x1c)))
    boxes, skipped = [], []
    for slot in range(u16(base + 0x10)):
        offset = struct.unpack_from("<I", data,
                                    base + u16(base + 0x2e) + slot * 4)[0]
        record = base + offset
        if (record + 14 > len(data) or u16(record) != 1 or
            record + 14 + u16(record + 10) + u16(record + 12) > len(data)):
            skipped.append(slot + u16(base + 0x0e))
            continue
        boxes.append(struct.unpack_from("<hhhh", data, record + 2))
    assert boxes
    union = tuple((min if i < 2 else max)(box[i] for box in boxes)
                  for i in range(4))
    assert bbox == header == union
    print(path.name, hashlib.sha256(data).hexdigest(), bbox,
          "framed", len(boxes), "skipped", skipped)
```

## Confirmed field mapping

All four coordinates are **signed little-endian int16** in y-down font units; the thirteen shipped files use 1000 units/em. Negative ymin is above the baseline; positive ymax is below it. The four values form one global coordinate rectangle, rather than four independent typographic metrics.

| Coordinate | Metrics-relative | FF1 header absolute | RFF header absolute |
| ---------- | ---------------- | ------------------- | ------------------- |
| xmin       | +0x16            | 0x12                | 0x22                |
| ymin       | +0x18            | 0x14                | 0x20                |
| xmax       | +0x1a            | 0x16                | 0x1e                |
| ymax       | +0x1c            | 0x18                | 0x1c                |

This confirms the report's eight RFF tuples and extends the field-copy verification to all five FF1s. The [public table](../../2.0/techspecs/3.2-fastfont-fonts.md#global-bounding-box-and-header-copies) lists each tuple. Earlier labels “ascender / bbox ymax” for metrics +0x1a and “width metric” for +0x1c are rejected. “Descender” at ymin is also misleading in the stated y-down convention: ymin is a coordinate above the baseline, not the positive descender depth. Do not assign a typographic ascender line just by negating a bbox coordinate; the alignment-zone/advance fields are separate.

For DIXON: width 1000−(−44)=1044, height 250−(−921)=1171, separately stored maximum-advance metric 1014. For MARIN: width 1059−(−78)=1137 and height 250−(−916)=1166. Positive xmax/ymax values in the corpus alone do not prove unsigned types; signedness comes from the consumers below.

## Original consumer and signedness trace

LLVM objdump 19.1.7 displays virtual addresses with base 0x10000000. All addresses below are RVAs; .text file offsets equal RVA−0xc00. The existing `run/riptel-inspect` fingerprints the DLL and reports PE mappings without executing it. Export labels that cover unrelated internal routines are not function identities.

| Original RVA | Read-only instruction evidence | Meaning |
| --- | --- | --- |
| 0x51844–0x51857 | Reach the embedded metrics header through the cached file/metrics block | The following displacements are metrics-relative, not absolute RFF header offsets |
| 0x5185b, 0x51860, 0x51865, 0x5186a | Load WORDs +0x1c, +0x1a, +0x18, +0x16, push in reverse argument order | Pass xmin, ymin, xmax, ymax to rectangle-transform helper 0x50fc0 |
| 0x50fc0–0x5102b | Form the four corner combinations and call 0x50460 for each | The values are rectangle edges, not ascender/advance quantities |
| 0x50460 and 0x5046b | `movswl` from the Y and X arguments before multiplication | Both axes sign-extend their 16-bit coordinates, including upper bounds |
| 0x51030–0x51149 | Signed minima/maxima of transformed corners and coordinate output | Realized bbox comes from transformed geometry |
| 0x40303 and 0x40325–0x40343 | Obtain realized metrics; sign-extend bounds at vector-state +0xd0/+0xd2/+0xd4/+0xd6; subtract X and Y extents separately | Output rectangle has separate horizontal and vertical spans |
| 0x3fc50–0x3fc65 | Subtract realized xmin at +0xd0 from xmax at +0xd4 | Horizontal reference returned to the spacing helper |
| 0x3fd99–0x3fdd8 | Negative spacing control selects 0x3fc50, then computes the percentage adjustment | Flag 0x400 uses realized horizontal span, not metrics +0x1c as a standalone width |

The primary sign proof is the actual transform's sign extension, not the fact that shipped upper bounds happen to be positive. Header copies reproduce the same signed coordinate values. This establishes the shared file interpretation without asserting that an earlier RIPterm rasterizer performs every realization step identically.

Read-only reproduction (`artifact_root` is the persistent mirror):

```sh
run/riptel-inspect "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll" --rva 0x5182d --length 78
objdump -d --start-address=0x1005182d --stop-address=0x1005187b "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x10050fc0 --stop-address=0x10051150 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x10050460 --stop-address=0x100504a7 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x1003fc50 --stop-address=0x1003fc66 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
objdump -d --start-address=0x1003fd90 --stop-address=0x1003fde3 "$artifact_root/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll"
```

## Distinguishing geometry and spacing example

The previously recorded [unmodified Marin oracle](issue-3-extended-font-style.md#unmodified-font-engine-rotation-oracle) already gives upright realized bounds (-7,-22)..(95,82) at 90 points/72 DPI. This task did not rerun it. Scaling the corrected raw rectangle (-78,-916)..(1059,250) ideally by 90/1000 gives X -7.02..95.31 and raw Y -82.44..22.5. The engine's Y-axis conversion yields -22.5..82.44, consistent with that recorded rectangle. Treating 1059 as a vertical ascender fails this axis comparison.

For the recorded instance, horizontal width is 95−(−7)=102 device units; the separately converted ordinary F advance is 50. Flag 0x400 selects that realized horizontal span (and, under other orientations, the span of the realized rotated bbox), not raw xmax alone, raw ymax, design height or maximum advance. The existing spacing formula and behavior stay intact. This arithmetic explains field identity and scale only; it claims no new exact hinting, integer-rounding or glyph-raster oracle.

## Specification changes and remaining delivery

Corrected the canonical 2.0 FF1 header, metrics block and RFF header to signed bbox coordinates; added one cross-format field map and the eight RFF tuples. The 3.0 family table, metric/spacing explanation and text-command cross-reference now use those labels. The earlier binary-triage header hypotheses are explicitly corrected too, so a secondary reference no longer contradicts the canonical table. Prior #3/#22 derivations and all font files remain unchanged; no client bug or rendering investigation was added.

Validation passes the published read-only Python comparison for all thirteen fonts, pinned font/DLL fingerprints, manual field/numeric/prose/table review, repository Markdown lint, 0 bad of 5,573 links across 228 files and `git diff --check`. Original text/assets and RIPscrip 1.54 are unchanged. The 2026-11-19 01:52:22 UTC refresh confirms #82 is an open RealDeuce issue with no comments, no new repository discussion/inline comments and no newer issue.

Next: publish one complete PR closing #82, then consolidate into fork stable. Record delivery identifiers here. Co-author every commit and remove temporary PR payloads automatically.

---

[Research backlog](issue-backlog.md) · [FastFont format](../../2.0/techspecs/3.2-fastfont-fonts.md)
