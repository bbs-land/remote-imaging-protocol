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

## Remaining work

Read the complete handler tail/format strings, name helpers and engine/client send boundary; establish every reply/error/disabled/name/size case and portable limits. Document exact vectors and source/policy distinctions, validate, publish one PR closing #168, normal-merge its actual full head to fork stable with identical-tree proof, audit all feedback/coverage/ancestry and continue the authorized ten-minute watch.
