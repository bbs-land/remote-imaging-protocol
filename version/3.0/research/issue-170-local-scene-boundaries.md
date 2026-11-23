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

Descriptor table physical **slot 104** points to **0xd64d**, two numeric types **[2,6]**, flags 15, followed by filename. The documented eight reserved zero characters remain the supported wire form; do not reopen #2's general combined-field issue without need.

Complete wire handler **0xd64d–0xd70a** copies the literal filename, gates options+0x104, validates nonempty raw input, calls HCL expansion **0x4b0e4(context,scratch,0xfff,0x83,1)** at **0xd6b8**, then **0x324a(context,expanded,1)** at **0xd6c4**. A return 2 reaches **RIP_Suspend 0x6c01**. HCL scene callback **0x25971** instead uses the accepted #161 owned-name/substitution path then **0x324a(context,name,0)**.

Shared resolver **0x324a–0x3372** saves options+0x344, sets it to one, applies filename-tail helper 0x3f71a, appends .RIP if the returned component has no dot, then calls **0x3f80e(context,whole_name_buffer,0x32,caller_flag)**. Normal found case invokes local engine **0x2b1e(context,resolved_path,0,1)**. Wrapper restores options+0x344 on its observed exits. Need establish exact flag/retrieval meaning and all no-action/missing branches without assuming HCL and wire have identical reply settings.

Local engine **0x2b1e** begins by saving options+0x12a/+0x344 and prior input-source pointer at context+0x0e,+0x0c, setting local/source state and calling emulation/suspend helpers. It opens with CRT mode **0x4000**. Only the beginning through 0x2d30 has been read; source-loop/line framing, parser snapshot/restoration, reply gating and recursion guards remain to derive. Do not infer CR/LF/EOF behavior from open flags or the old prose alone.

## Next action

Read the remainder of 0x2b1e and its bounded input/parser/reply helpers; identify exact nesting and local reply state, resolver/wire expansion and missing behavior. Publish supported source-backed rules separately from explicit safe completions, with independent child-color/query/unterminated-command and parent-pipe/CR/HCL-suffix examples. Validate and publish one PR closing #170, normal-merge its actual complete head into fork stable with identical-tree proof and audit; then start #171 from updated stable. Keep Codex trailers, FastFont 5966e1b separation and scoped temporary cleanup. Continue ten-minute checks while working.
