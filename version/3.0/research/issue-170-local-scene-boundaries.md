# Issue #170: Local scene input and reply boundaries

[Research backlog](issue-backlog.md) · [READ_SCENE](../ripscrip/5.5-file-transfer-and-queries.md#rip_read_scene) · [HCL scene playback](../ripscrip/5.3-local-playback.md#local-scene-playback---filerip)

## Scope and checkpoint

Started 2026-11-23T07:42Z on `issue-170-local-scene-boundaries` from stable **9396d5577175e470c7b3dd6bd6613b3fe7c80221**, after #168 / PR #169 actual source e9f962e was normal-merged with an identical tree. [#170](https://github.com/bbs-land/remote-imaging-protocol/issues/170), created/updated **2026-11-23T07:38:56Z**, zero comments, body SHA-256 `800402f9a276cfe8fa2eb42660535bf773426b5e3d040b48bfc71f02f4f03e3f`. Resolve local CR/LF/EOF command dispatch, nested pipe-versus-CR, child reply suppression versus parent HCL suffix, wire filename preparation/missing/recursion and retained drawing versus interpreter/source state. Preserve #161 HCL naming, prior framing/terminal/column/query contracts and originals/1.54. No original/client/UI/provider execution or obsolete bug investigation.

**Queued next, separately:** [#171 FILEEXISTS](https://github.com/bbs-land/remote-imaging-protocol/issues/171), created/updated **2026-11-23T07:39:05Z**, zero comments, initial body SHA-256 `3a04ac4c2ea4e7929ad16112438edf04b9b947e00e1d97c8947e19e1149db401`. Its report is recorded only; no investigation started. Last all-author check **07:41:51Z**, next about **07:51:51Z**. Last full audit **07:31:44Z**, future feedback overlaps at least one minute earlier.

## Starting reference hashes

Reproduce with `git show 9396d5577175e470c7b3dd6bd6613b3fe7c80221:<path>`:

- `version/3.0/ripscrip/5.5-file-transfer-and-queries.md`: SHA-256 `b41ffa401e700bc7f3557600db7f867d73892c2ee549c35e0b2ab6dae71eb55c`.
- `version/3.0/ripscrip/5.3-local-playback.md`: SHA-256 `24c630615b43ad6f486683391e4a2f2bd0e77cf561ea392709f2ed7e4017f5c4`.
- `version/3.0/ripscrip/1.1-protocol-and-command-hierarchy.md`: SHA-256 `1bdd96ef2e3af28d433a1329b4949aa14b7b19d1ac3ad9b1e657e0b676291c2a`.
- `version/3.0/ripscrip/1.4-terminal-ansi-view.md`: SHA-256 `4039d8e0333b987dad76351bf373774767ffa390a7580912aad24cdddfdca24e`.
- `version/3.0/research/issue-161-playback-filenames.md`: SHA-256 `e168c6ff42c5206debdb0cf9e28515ce7279eb542244b2ad1a75a64771c43b1a`.
- `version/3.0/research/issue-168-file-query-replies.md`: SHA-256 `eab00a708624f742d8d170e223a2f6ad7296d647f8ebe98b69db14e73a48efb6`.
- `version/3.0/research/issue-7-framing-identification.md`: SHA-256 `7c79594f0e4d9242ad44854accdc227b7fe2e91852a8501cb6a9b7e698481e22`.

## Fingerprinted source and initial bounded observations

Catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, image base **0x10000000**. LLVM objdump **19.1.7** and `run/riptel-inspect` read only. Addresses are **RVAs**; objdump uses image-base-plus-RVA. No new probes/temp dump files or original execution.

Descriptor table physical **slot 104** points to **0xd64d**, two numeric types **[2,6]**, flags 15, followed by filename. Eight zero digits remain a full-width spelling. The [receive-field derivation](issue-263-resource-query-text-boundaries.md) gives the boundary for each field and the one-period spellings.

Complete wire handler **0xd64d–0xd70a** copies the literal filename, gates options+0x104, validates nonempty raw input, calls HCL expansion **0x4b0e4(context,scratch,0xfff,0x83,1)** at **0xd6b8**, then **0x324a(context,expanded,1)** at **0xd6c4**. A return 2 reaches **RIP_Suspend 0x6c01**. HCL scene callback **0x25971** instead uses the accepted #161 owned-name/substitution path then **0x324a(context,name,0)**.

Shared resolver **0x324a–0x3372** saves options+0x344, sets it to one, applies filename-tail helper 0x3f71a, appends .RIP if the returned component has no dot, then calls **0x3f80e(context,whole_name_buffer,0x32,caller_flag)**. Normal found case invokes local engine **0x2b1e(context,resolved_path,0,1)**. Wrapper restores options+0x344 on its observed exits. Need establish exact flag/retrieval meaning and all no-action/missing branches without assuming HCL and wire have identical reply settings.

Local engine **0x2b1e** begins by saving options+0x12a/+0x344 and reading the initial baud rate from the timing structure at context+0x0e followed by +0x0c, setting local/source state and calling emulation/suspend helpers. It opens with CRT mode **0x4000**. The CRT and byte-loop evidence below establish physical line handling and source completion.

## Catch-up queue and watch status

Issue and discussion refresh at **2026-11-23T16:25Z** verified unchanged bodies and zero issue comments for #170 and #171. The query from **07:30:44Z** found three additional reports. Their order and fingerprints are recorded here for later issue branches.

| Issue | Created and updated (UTC) | Initial body SHA-256 |
| --- | --- | --- |
| [#172 local playback baud and scene resets](https://github.com/bbs-land/remote-imaging-protocol/issues/172) | 2026-11-23T07:51:15Z | `a7e69f75e562872b47fb260af48708a5ed21678dca42f09f88b7c6582a5d9817` |
| [#173 prewrapped line placement and justification](https://github.com/bbs-land/remote-imaging-protocol/issues/173) | 2026-11-23T08:05:59Z | `89cb0562df0c8a5d2f7ea26aba89ead0a2c64b98401aa8b7d3c73f26549a88ee` |
| [#174 palette publication and PALENTRY precision](https://github.com/bbs-land/remote-imaging-protocol/issues/174) | 2026-11-23T16:18:49Z | `f8ccd9ddef74bfffb151a75ff566069fe74d252e5dc744a295ece422bafec144` |
| [#175 RIP_DEFINE grammar and editor policy](https://github.com/bbs-land/remote-imaging-protocol/issues/175) | 2026-11-23T16:32:59Z | `b6e9ef09d4112b741178e56a552fd6d860282e9932ff687402e7744ac9197213` |

Each had zero comments. #175 arrived after the initial refresh. Issue discussion and PR inline comment queries returned no items since **07:30:44Z**. The previous full audit watermark still applies to reviews and PR ancestry.

## Bounded source continuation

- The context pointer at **context+0x0e** addresses the playback and timing structure. Its dword at **+0x0c** is the baud rate, passed to **0x307f** and **0x322a**. Save/check helper **0x16be8** reads the structure's **+0x1c** word. **0x16ba5** updates a timing value when that flag is set. Its call after the byte parser is outside the command dispatch itself. #172 owns the general baud/reset contract.
- **0x3ab9c** dispatches parser state 9 to **0x3a4a6**, which calls the descriptor handler at **0x3a515** in the normal callback configuration. State 11 at **0x3aaec** selects state 2 for pipe or state 10 for CR, LF or FF. The normal string/error arms at **0x3a469** and **0x3a581** distinguish pipe from line endings protected by continuation backslashes. Local file loop **0x2ee0** uses this parser directly. Its EOF path closes the file without appending a delimiter.
- Local playback calls **0x39d20** at **0x2cf0** before copying 59 parser bytes. The helper clears 56 bytes of parser state and the argument buffers. Child previous-byte fields start at CR and a partial ANSI parse is reset. The post-child state restoration and preserved drawing state need to be stated separately. The original pipe-handling branch in state 11 has to be read with the inherited requirement that an authored nested `1R` end the calling line with CR.
- Shared scene resolver sets resource option **+0x344** before the lookup. After the configured host directory, the configured RIP directory and ordered search paths, a miss reaches **0x3f9ac**. The retrieval path requires a nonzero retrieval flag and a zero **+0x344** at **0x3f9b7**. The resolver's setting blocks retrieval for wire `1R` even though it passes flag 1. HCL passes flag 0. Neither miss enters a child file or sends a file-query reply.
- Wire `1R` calls **0x4b0e4(context,scratch,0xfff,0x83,1)** once. Its preprocessing flag enters the documented angle worker at **0x4b15f**. The shared resource name tail and default extension are selected after that expansion. The activator substitutions specific to HCL playback callbacks have no call on this wire path. Compare existing graphics-text mask **0x83** and the accepted single-pass replacement rule in the [macro derivation](issue-6-macro-evaluation.md).
- The mode 0 RIP query path **0xd484 → 0x13ed0 → 0x4ab3e** enters ordinary HCL execution. ANSI status report **0x37000–0x37066** accepts one argument 5 or 6 and calls common reply **0x6947** at **0x3705a**. The similarly located **0x36c10** call sends device attributes instead. Child ordinary text enters **0x4d2d0**, which routes to the same ANSI parser **0x37b7c** when ANSI handling is enabled. Confirm the HCL executor's output gates and exact independent reply fixtures before publishing.
- The nesting counter **0x79324** is incremented at **0x2dcf** and decremented at **0x3072**. The intervening comparison with 1 handles cancellation while nested. It supplies no numeric admission cap. Document the inherited line-final rule and leave an explicit result for refusing a call when the implementation cannot admit more playback.

## Adopted playback contract

- Text mode `0x4000` at the original open site feeds the internal CRT reader at **0x6b1c0**. Its text branch **0x6b2f5–0x6b3e2** folds CRLF, retains standalone CR and stops regular files at Ctrl+Z. In the scene loop, `0x2ed0` skips zero bytes. Only LF routed to the text interpreter gets a preceding CR at **0x2f24**. RIP line termination uses CR, LF or FF through the byte parser. A trailing parameter at EOF stays pending and is discarded when the caller resumes.
- Ordinary state 9 invokes the handler before it sets state 11 at **0x3a51a**. State 11 reconsiders the current delimiter, still in BL, through **0x3aaf5–0x3ab2e**. On normal return, pipe selects the opcode state and CR/LF/FF select the line-end state. A local caller continues reading its own file at **0x2f38**. The inherited rule still requires CR after a nested call. The example records the observed normal pipe continuation without extending the authored grammar.
- The local engine resets the parser prefix before its snapshot, so it does not preserve a pending child command or clone the caller's RIP command buffers. At **0x2d1d–0x2d4c** it sets previous-byte fields to CR and resets the ANSI parse state for the child. **0x2fda–0x2fe8** restores the caller's ANSI parse record. Ordinary completion resumes the active delimiter. The graphics and text objects remain shared.
- The wire name mask **0x83** admits data providers, user variables and backslash processing. The `0x4b0e4` checks at **0x4b1c3**, **0x4b305** and **0x4b3b1** exclude ordinary slot, list and control notation processing at their respective bits. There is no second ordinary pass. The **0x79db4** literal is `ICONS`. Default `.RIP` at **0x794e8** is appended only after the name-tail test, and `0x3f9b7` excludes retrieval. An absent local result has no wire reply. The prefix keeps its existing filename preparation.
- One immediate query uses **0xd484 → 0x13ed0 → 0x4ab3e → 0x4ae9d → 0x4acd9**. The unbracketed response path **0x4aeed–0x4af42** owns a scratch copy of its response at each invocation. The executor's two successful passes, then nonempty result at **0x4ad28**, lead to **0x4ad37** (optional echo) and **0x4ad41** (send). Thus child mode 0 execution can send while the separate parent buffer awaits the empty scene-token replacement. The parent then sends its surviving suffix. FILE_QUERY's existing direct writers and ANSI DSR **0x3705a** use the same **0x6947** service. The driver send checks the instance signature and calls message **0x4c8**, regardless of the local source flag. The active-client transport and output newline setting retain the established reply behavior. Status request argument 5 returns `1b 5b 30 6e` from **0x803e8**, and argument 6 uses the position format at **0x803dc**.
- No numeric scene admission limit follows from the counter. The portable completion adds one behavior for implementations that cannot admit another scene: decline the requested child as a local return, keeping active callers and their prior effects. The HCL token still returns an empty replacement. No claim about a numeric native cap or its failure response is made.

## Validation checkpoint

Rechecked the seven baseline hashes and original DLL fingerprint. Read 15 original relative call targets, three opcode descriptors, two jump table entries, four data literals and the retrieval check directly from the PE image. Independently evaluated twelve drawing/reply examples using explicit delimiter and command subsets, including all four line endings, pending EOF, compatible pipe resumption, parent suffix and missing scene. Checked the literal FILE_QUERY and ANSI status vectors against prior query evidence and the original DSR response constant. No native client execution was required.

## Delivery

[PR #176](https://github.com/bbs-land/remote-imaging-protocol/pull/176) opened from the RealDeuce `issue-170-local-scene-boundaries` branch targeting upstream `main`. The specification commit is **a44b81624c9b9bc13548e5ceb1cf27c2eab0ac3b**. GitHub reports OPEN, ready for review and exactly #170 as its closing issue. Full lint and diff checks passed, and the link checker reported 0 broken links among 6,547. The #170 report still matches its original hash and has no comments. The actual source head **126d65552b7db7f1aecff7b7fbc6af32b6aa8446** was normal-merged as **fc9638c192d7711d60b10eeeadb253ad14c5c18b** in fork stable. The two tree objects matched. The post-merge audit at **2026-11-23T16:37:59Z** verified ancestry for 84 open PR heads and coverage of 85 of 90 open RealDeuce issues. Only #171 through #175 remain queued. There was no discussion, inline PR or review feedback since **07:30:44Z**. Continue separately with #171.
