# Issue #168: FILE_QUERY reply bytes and alias metadata

[Research backlog](issue-backlog.md) · [FILE_QUERY](../ripscrip/5.5-file-transfer-and-queries.md#rip_file_query)

## Scope and checkpoint

Started 2026-11-23T06:19Z on `issue-168-file-query-replies` from fork stable **9716850d93ba720097af2a3e100e633c6797ac38**, after #166 / PR #167 actual full head e93ccd3 was normal-merged with an identical tree. [#168](https://github.com/bbs-land/remote-imaging-protocol/issues/168) created/updated **2026-11-23T06:19:17Z**, zero comments, body SHA-256 `2e0141726291e86200728c6aaa2ccb487136be9df584d667ca230c48e40a48a8`. Complete exact modes 00..04 success/miss/error replies and engine/client line endings, prepared reply name versus alias lookup, substitutions/disabled/rejection, translated bitmap alias size/fallback and direct BMP comparison. Preserve source originals/1.54 and prior filename/search/metadata contracts. Protocol only; no original/client/UI/provider execution or bug investigation. Last issue check 06:19:39Z, next about 06:29:39Z; last full feedback audit 06:09:31Z, overlap at least one minute earlier.

## Starting reference hashes

Reproduce with `git show 9716850d93ba720097af2a3e100e633c6797ac38:<path>`:

- `version/3.0/ripscrip/5.5-file-transfer-and-queries.md`: SHA-256 `8799eef4defc1450b644b1cb4bf54cf1d24e13dde7c25b784effcd9dfb9a59fb`.
- `version/3.0/ripscrip/9.3-host-command-reference.md`: SHA-256 `acad974aac2ea19501588954560494694f71828ea8153f28527b4dfe653db766`.
- `version/3.0/ripscrip/6.2-asset-delivery-and-storage.md`: SHA-256 `87df1126e0cb63cc913f3b1723976db57a50379c808bfae3c9033b57b08a108f`.
- `version/3.0/ripscrip/9.0-command-reference.md`: SHA-256 `78cec23f8cd5e3b1f4c4deb5e27d1401547404e62288b83584adc304ba20b381`.
- `version/3.0/research/issue-161-playback-filenames.md`: SHA-256 `e168c6ff42c5206debdb0cf9e28515ce7279eb542244b2ad1a75a64771c43b1a`.
- `version/3.0/research/issue-8-bmm-masks.md`: SHA-256 `534d10842be1500203ccd541d0d8ec6ba2f9cf76e73fc2b270432df4c94d8875`.

## Fingerprinted primary source and initial evidence

Catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred image base **0x10000000**. LLVM objdump/readobj **19.1.7** and `run/riptel-inspect` read only. All addresses are **RVAs**, not file offsets. Reproduce disassembly with `llvm-objdump19 -d --start-address=<base+RVA> --stop-address=<base+end> <original DLL>`. No temporary dump files or original execution.

Handler **0xbde4** checks option word **context+0x36, +0x104** before validating mode 0..4/name/scratch. It copies the input, calls **0x3f71a** at **0xbe6c**, then **0x65424** at **0xbe88**; helper meanings must be confirmed before publishing substitutions/spelling. It splits/reconstructs a separate lookup buffer and applies **.ICN→.BMP, .HIC→.BMH, .MSK→.BMM**. The prepared original name is separate and supplied to mode 04 reply formatting.

Lookup miss has its own reply branch **0xbf45–0xbf9e**; found-file open/metadata failures follow later branches and cannot be flattened into a lookup miss without tracing them. Alias queries read exactly 14 then 40 bytes and require initial BM; helper **0x30491** consumes signed low-word width/height and calculates **6 + 4 × ceil(width/8) × height** for ordinary positive dimensions. Direct BMP has no alias flag and retains file length. Need define positive-domain/fallback without unsafe native edge investigation.

## Initial next action

Read the complete handler tail/format strings, name helpers and engine/client send boundary; establish every reply/error/disabled/name/size case and portable limits. Document exact vectors and source/policy distinctions, validate, publish one PR closing #168, normal-merge its actual full head to fork stable with identical-tree proof, audit all feedback/coverage/ancestry and continue the authorized ten-minute watch.

## Complete source ledger

Original DLL disassembly **0xbde4–0xc1ce** is read in full. Descriptor table at 0x80820, physical **slot 94**, count 2, numeric types **[2,4]**, flags **15**, confirms two fixed fields followed by the filename. No original instruction, client, provider or unsafe file probe was executed.

| RVA | Observation and derivation |
| --- | --- |
| 0xbe13–0xbe28 | Nonzero options+0x104 globally suppresses before mode/name validation. Modes >4 reject locally without reply |
| 0xbe32–0xbe88 | Missing/empty raw name and scratch failure have local diagnostic/no reply; copy, **0x3f71a** filename-tail selection, then **0x65424→0x75820** ASCII uppercase. No call to playback substitution 0x26dd2 or HCL expansion 0x4b0e4 |
| 0x3f71a–0x3f775 / 0x65c44–0x65c8f | Final byte must satisfy predicate; retain longest accepted suffix, or all input if all accepted. Signed-byte ASCII predicate accepts 0x20..0x7f except 0x22,0x2a..0x2c,0x2f,0x3b..0x3f,0x5b,0x5d,0x7c. Space/dollar/colon/backslash/period accepted; no trim or placeholder expansion |
| 0xbe90–0xbec9 / 0x65660 / 0x65766 | DOS split/make receives null drive/directory output/input arguments; reconstruct only filename+extension into separate Q. Prepared N pointer is retained for mode 04 |
| 0xbed2–0xbf2c / 0x65635–0x6565f / 0x65965 / 0x69050–0x69085 | Ordered ICN/BMP, HIC/BMH, MSK/BMM helpers use **first substring occurrence**, then strcpy the new marker over that point and everything after it, stopping at first matching alias rule. This is not a final-extension comparison. A.ICN.EXTRA→A.BMP is a source-derived string vector, not a path/file probe |
| 0xbf2c–0xbf9e | Ordinary lookup 0x3f80e with mode/retrieval 0/0. Return 1 enters miss branch: mode 00 uses one-byte 0, 01..03 use 0 CR, 04 uses full prepared-name zero record |
| 0xbfa3–0xbfcc / 0xc197–0xc1a3 | Open local binary file via 0x6b560; failure -1 skips all send calls. File length 0x75790 supplies initial signed L |
| 0xbfcf–0xc01f / 0x30491–0x304c4 | Alias flag alone enables 14-byte then 40-byte reads. Exact counts and initial BM are required before replacing L. Ordinary positive W/H give **6+4×ceil(W/8)×H**; all three aliases use the same helper. Raw helper reads signed low words; it does not validate arbitrary BMP dimensions/layouts |
| 0xc01f–0xc059 | Opened modes 00/01 send 1 / 1 CR without fstat; no file-metadata failure branch in those modes |
| 0xc059–0xc110 | Modes 02/03 require fstat 0x6bf40. Success formats selected S, and mode 03 localtime 0x69c90 at stat buffer+28 (mtime), then strftime 0x69210. Fstat failure uses 1.0/zero date placeholders |
| 0xc110–0xc179 | Mode 04 fstat success formats **N:S**, no resolved path or alias name; failure still uses prefix 1/N but zero size/date/time |
| 0xc17c–0xc194 / 0x6947–0x6983 | Send result through message 0x4c8 and close opened descriptor; string storage NUL is not a reply byte |

Source format bytes, extracted with `run/riptel-inspect DLL --rva 0x7a830 --length 208` plus one-byte strings at 0x7970c/0x79714:

| RVA               | Exact ASCII content (CR written as \r) |
| ----------------- | -------------------------------------- |
| 0x7970c / 0x79714 | `1` / `0`                              |
| 0x7a8c0 / 0x7a8e0 | `1\r` / `0\r`                          |
| 0x7a8b8           | `1.%ld\r`                              |
| 0x7a8b0           | `1.0\r`                                |
| 0x7a890           | `1.%ld.%s\r`                           |
| 0x7a878           | `1.0.00/00/00.00:00:00\r`              |
| 0x7a868           | `1.%s:%ld.%s\r`                        |
| 0x7a84c           | `1.%s:0.00/00/00.00:00:00\r`           |
| 0x7a8c4           | `0.%s:0.00/00/00.00:00:00\r`           |
| 0x7a89c           | `%m/%d/%y.%H:%M:%S`                    |

The current 3.0 reference now separates raw-name rejection, tail preparation and uppercasing, split/reconstruction, marker rewrite, lookup miss, open failure, length/header selection and metadata failure. No filename/playback/HCL substitution behavior is inferred from another command.

## Engine versus original client

The existing [#5 primary-source trace](issue-5-scene-headers.md#entry-point-effect-probes-and-client-reply-trace) identifies this same send boundary. Rechecked the catalogued original **riptel-unpacked.exe**, **605,696 bytes**, SHA-256 `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`, image base **0x400000**, using static bytes only. EXE RVAs are distinct from DLL RVAs.

DLL 0x6947 sends message **0x4c8**, lParam reply string. Client dispatcher branch **0x844e–0x8466** requires an active connection then calls **0x63ee**. Client **0x6415–0x6435** copies non-NUL bytes, adding LF after CR only when global **VA 0x452528** equals zero. Source ends are one engine CR for modes 01..04, no ending for 00; connection settings can yield CRLF. No decoder/client/UI or actual transmit ran.

## Protocol completion and limits

Adopt source-derived ordinary filename and reply bytes, including the bounded ASCII tail/string-rewrite behavior, without granting input drive/path prefixes new resource access; Q strips drive/directory and follows the existing lookup namespace. No current graphics port/viewport/protection gate is added. Storage must be safely owned/bounded by the implementation; the vintage private 256-byte temporary buffers are not new wire limits or requirements to reproduce unsafe writes.

**Explicit safe bitmap completion:** use the ordinary file length for unsupported headers or dimensions outside positive **1..32767**, rather than requiring signed-low-word wrap/negative icon-equivalent lengths. For ordinary positive Windows headers, exact 14+40 read/BM gate and 6+4×ceil(W/8)×H apply independent of actual BMP bit depth. Short/failed header reads keep L; direct BMP never converts. The source's unvalidated private out-of-domain arithmetic is recorded, not investigated or required as a bug compatibility feature.

**Explicit time completion:** if an otherwise obtained modification timestamp cannot be converted/represented in modes 03/04, use that mode's metadata-unavailable record; mode 02 does not convert timestamps. This supplies a safe boundary rather than investigating a vintage null-localtime path. Ordinary fstat failure zero placeholders and open-failure silence are direct source facts. Initial file-length failure remains its signed L=-1 result if formatting proceeds and no successful alias conversion replaced it; do not flatten this into lookup-miss 0.

Completed independent validation below: extract every literal/critical branch/call, construct the 9×2/70-byte BMP and 22-byte legacy calculation, compare all per-mode outcomes and CR/CRLF/no-NUL bytes, test safe ASCII preparation/alias strings without executing original code, review the docs and refresh #168 before PR publication/stable delivery.

## Independent validation

At 2026-11-23T06:34Z an inline Python check passes **six starting-file hashes**, both exact original artifact fingerprints, descriptor 94's handler/count/types, **12 exact format literals with storage NUL**, **23 direct DLL call targets**, **13 DLL instruction sequences** and **four original-client sequences** proving CR detection, conditional LF insertion and connection gating. File offsets are mapped from each PE's own section table; no original instruction is executed.

An independent abstract formatter compares all five ordinary successes against literal expected ASCII bytes, all five lookup misses and all metadata-failure forms, then covers **560 mode/status/length/CR-versus-CRLF combinations**. Status cases include open failure, suppression/rejection/unusable name, fstat failure and the explicit time-conversion fallback limited to modes 03/04. Mode 02 time-unavailable with successful metadata still reports S. Sizes include -2147483648,-1,0,22,70,20345,2147483647; every result has no embedded NUL/LF at the engine boundary and the expected one/no ending. Client LF insertion changes only CR-terminated cases; mode 00 stays one byte.

Pure byte preparation checks cover **255 non-NUL candidate final bytes**, invalid separators before a valid tail, retained spaces/literal dollar text, mixed case, the Ab*tail.ICN suffix, drive/backslash split, no extension default, ordered alias rewrites and the A.ICN.EXTRA first-marker behavior. These are owned string models with no filesystem query, original helper execution or path probe.

A separately constructed valid **70-byte 9×2, 1-bit BI_RGB BMP** has a 14-byte header, 40-byte info header, two RGBQUADs and two four-byte raster rows. Header extraction and independent bit-plane row packing give **22** legacy bytes. **99 small packed-plane vectors** (widths 1..33; heights 1/2/17) independently count packed bytes; all **32,767 positive signed-word widths** agree with ceil division. Short/non-BM header fallbacks, six out-of-domain dimension completions, all six usual BMP depth fields with fixed W/H, direct-BMP ordinary size and exact alias/direct/miss reply vectors pass without executing a decoder or creating fixture files.

The full literal `!|1F040000MiXeD.RiP` independently decodes [mode 4,res 0,name MiXeD.RiP]. Exact original chapter prefix before FILE_QUERY and suffix beginning RIP_DELAY are unchanged; the update is confined to current 3.0 prose and workflow notes. Original text/assets/1.54 are untouched. Full `run/lint`, link checking and diff checks pass before final metadata; links report **0 bad of 6,523 across 272 files**.

Source date/size/name/line-ending facts and explicit bitmap-domain/time-conversion completions remain distinct. The 9.3 blanket statement about host strings now refers to authored HCL responses; already formatted FILE_QUERY replies are not re-expanded. No original/client/UI/provider execution or obsolete bug investigation, and no task temporary files.

## Publication checkpoint

Content/independent checks, final report refresh, PR publication and actual-head stable consolidation are complete below. Last all-author poll **06:30:21Z** found none newer; next about **06:40:21Z**.

Final pre-publication review confirms normal prose spacing, exact full wire literals outside GFM tables, all reply separators/endings and unchanged neighboring commands. The reserved numeric field is consumed/ignored; alias timestamps remain those of the opened translated resource. #168 retains original body/update/zero comments. Full lint plus changed-file checks, 6,523 links and diff checks pass; source-byte and independent arithmetic/string/transport checks pass. No task-owned temporary paths were created.

## PR delivery

[PR #169](https://github.com/bbs-land/remote-imaging-protocol/pull/169), specification **4e8678ceb796d871a8d1f0d216dc61093009935c**, is OPEN/non-draft and closes exactly #168. Source branch `issue-168-file-query-replies` starts at stable 9716850. The actual complete head, including this delivery checkpoint, is consolidated and audited below. The scoped `rip-168-pr-` request directory was automatically removed. No upstream merge or manual closure.

## Stable consolidation and final audit

[PR #169](https://github.com/bbs-land/remote-imaging-protocol/pull/169), specification **4e8678ceb796d871a8d1f0d216dc61093009935c**, is OPEN/non-draft and closes exactly #168. Actual full source head **e9f962eb66419b5a8b812f159b5728e70d8efe5a**, including delivery metadata, is normal-merged through fork stable **2900e843f911b05183620c110e43f381d90abc38**. Its merge tree exactly matches the actual source tree before the final workflow checkpoint.

Audit started **2026-11-23T06:40:27Z**, overlapping **2026-11-23T06:08:31Z**: **84 open RealDeuce issues covered and all 83 open PR heads ancestral**, unchanged #168 original body/update/zero comments and no other report update, discussion/inline comment or review feedback. GraphQL issue/PR/closing/review pagination and REST comment pagination checked. Review updates use updatedAt as well as submission time. No upstream merge or manual issue/PR closure.

Four commits through merge carry Codex trailers; the final workflow checkpoint retains attribution. Eight allowed Markdown paths changed, originals/text/assets/1.54 and adjacent query-chapter commands are unchanged, FastFont 5966e1b remains unchanged/nonancestral and no task-owned temporary directory remains. Full lint/link/diff and source/independent checks pass; content link count is 0 bad of 6,523 across 272 files. Abstract vectors establish protocol string/arithmetic/transport expectations, not native execution or platform metadata parity. No original/client/UI/provider execution or obsolete bug investigation.

No active investigation remains. Latest all-author poll **06:40:42Z** found no newer issue; next about **06:50:42Z**. Continue the explicitly authorized active-session ten-minute research/PR/stable watch, overlapping future feedback queries at least one minute before this audit. No persistent scheduler.
