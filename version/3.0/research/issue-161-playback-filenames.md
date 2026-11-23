# Issue #161: Shared HCL playback filenames

[Research backlog](issue-backlog.md) · [Local playback](../ripscrip/5.3-local-playback.md)

## Scope and checkpoint

Started 2026-11-23T02:22Z on `issue-161-playback-filename-contract` from fork stable **1d46feda7d82001ee6f8b7c614defaa439facb43**, after #159 / PR #162 actual head 1549601 was merged with an identical tree. [#161](https://github.com/bbs-land/remote-imaging-protocol/issues/161) created/updated **2026-11-23T02:14:16Z**, zero comments, body SHA-256 `05d72d4ce7dd36a6e831364f9387d8c4afaa75c6e6051dbc7422e415ddf8c2ce`, unchanged at start. Complete shared playback-name grammar, state substitutions, decoded bound/recovery, extension/stop and ownership. Keep #6/#53/#84/#93/#113/#159 and JPEG presentation/lifetime rules. Last issue check 02:16:25Z, next about 02:26:25Z. No original instructions/client/dialog/UI/audio/file-provider execution or bug investigation.

## Starting reference hashes

Reproduce with `git show 1d46feda7d82001ee6f8b7c614defaa439facb43:<path>`:

- `version/3.0/ripscrip/5.3-local-playback.md`: SHA-256 `b21039c6f5ddfe40106f455014af0e377c415e01239a00d3ee8aad2b355f6fc5`.
- `version/3.0/ripscrip/5.2-templates-and-conditionals.md`: SHA-256 `9294fe31e294c5ea384ccd8d24587513167e998b281386b48722a864b0cd9670`.
- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `bc472bc63e09711da67c75a8f497c6656f936e6697343c42df7ce6e5d1660c53`.
- `version/3.0/ripscrip/5.0-host-commands.md`: SHA-256 `e11281b5a27edf93b755fb83eaccfdda0f2a58f9f07585e10a454d0b0dd1d57b`.
- `version/3.0/ripscrip/4.5-audio-files.md`: SHA-256 `38ab6732325bb1e56ec59d663cecd4d35460552e3da29f85a0b3fd815e0818aa`.
- `version/3.0/research/issue-84-dollar-argument-grammar.md`: SHA-256 `716029d580fc54fb83cf51d5458e7713609fc1fcca39e7bac07be9d7f654778f`.
- `version/3.0/research/issue-113-mouse-gesture-identity.md`: SHA-256 `b93ae44ac92e7d14c2d3ad79276094b2fc7525f5239d44dd5146d13bde747880`.
- `version/3.0/research/issue-21-image-style.md`: SHA-256 `868317ba2ffa02a20637160716360fd85281377dfcaf6c292788489a11eea4b9`.
- `version/3.0/research/issue-159-pick-list-contract.md`: SHA-256 `b4fce44df34cd31e6d16a47f0cd10b91251eff48f18ed17d6d2d13ac9dc52c4b`.

## Fingerprinted source and bounded reads

Catalogued original `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, **592,896 bytes**, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, preferred image base **0x10000000**. LLVM objdump **19.1.7**, Intel static disassembly only; `run/riptel-inspect` fingerprints/maps bytes without original execution. All addresses below are **RVAs**, not file offsets. Reproduce with `llvm-objdump19 --disassemble --x86-asm-syntax=intel` and numeric start/stop **VA = image base + RVA**, using the catalogued original; no dump/temp files required.

## Shared substitution and ownership ledger

Four callbacks **0x25971/0x259b3/0x259f7/0x25a39**, bounded through 0x25a7b, copy the parsed filename into an owned 256-byte local buffer and call **0x26dd2** at **0x25996/0x259d8/0x25a1c/0x25a5e**, then dispatch scene **0x324a(...,0)**, WAV **0x37e56(...,0,0)**, bitmap **0x38910(...,1)** and JPEG **0x3880e(...,0)** respectively. This filename pass does not mutate stored HCL/template/user-variable source bytes, or introduce another HCL ordinary pass.

The exact helper **0x26dd2–0x26ebf** repeatedly finds a byte in **RVA 0x7e73c = `*?[]`**. Each occurrence is replaced, then search restarts; inserted digits contain no new substitution symbols.

- `*`: delete the star first, read mouse-system context+0x1e temporary current callback slot +0x18. If **-1**, continue with star removed/no text. Otherwise read field stride 0xa3 WORD **+0xa0** (identity, not slot), format with **RVA 0x7cf88 = `%hu`** and insert. Thus an unidentified active field WORD -1 prints **65535**, unlike signed `$FIELDID$` = -1; absent activation removes the star instead of retaining the last field/defaulting to zero. Use the already adopted safe captured FIELDID scope across nested/deleted handlers; do not investigate stale native slot reads.
- `?`: **0x26e57 → 0x2814d(context,0)**, bit 0 → ASCII 1/0. The helper **0x2814d–0x2818e** reads the held-left-button state at mouse-system+0x0a, with other bits from separate right/middle state; only bit 0 is relevant. This is neither field selection nor the presence of an activation/ID.
- `[`: **VK_SHIFT 0x10** through GetKeyState IAT **0x966d0**, high byte → ASCII 1/0; `]`: **VK_CONTROL 0x11**, same. There is no ALT placeholder or ordinary template-bracket parser here.

No preceding-backslash test exists in this helper: quoting cannot suppress `*?[]` substitution. Those bytes are not glob wildcards, conditional syntax or literal filename escapes. Repetition/mixing works in every prefix. Explicit coherent-state completion: capture the current handler identity scope and mouse/modifier state once per reached filename operation, so repeated placeholders share one source snapshot; later separate playback tokens can observe later state. A filename transformation does not execute an old field, select it or acquire FIELDID outside activation.

## Filename parser, bound and fallback ledger

The original dollar parser **0x15602–0x15f60** has state 4 **0x15945**, entered by all four prefix branches. Prescan **0x15626–0x15653** finds a dollar not immediately preceded by backslash; it is not an odd/even backslash-pair rule. No eligible terminator returns parse failure with one-byte advance. State-4 storage is a NUL-terminated filename beginning at the parsed name field, not ordinary 21-byte argument slots.

State 4 copies arbitrary nonterminating bytes while stored count < **20**. At a dollar, if the prior stored byte is backslash, it **overwrites that byte with `$`** without increasing the count and continues; otherwise it accepts the terminator. Other backslashes/characters are retained literally. Therefore `\$` at this parser stage decodes to one dollar counted toward **20 decoded working bytes**; it does not implement general filename backslash unescaping or literalize a star. Escaping a dollar can complete a 20-byte name; a 21st ordinary stored byte rejects the reference, not a truncated playback.

Accepted state 10 **0x15f24** uppercases the parsed name via **0x75820** (ASCII a..z → A..Z in original default-locale fast path). The separate ordinary callback-argument slots are not what gets uppercased. State 11 **0x15f44** returns rejected-reference status with the complete prescanned terminator count; outer scanner **0x4b287–0x4b2f8** leaves that original reference literal and advances past it for the current pass. Missing terminator instead advances one byte and continues ordinary scanning, so later eligible codes are not quarantined. Normal two-pass/backslash/control rules still apply; no recursive parser or connection/program abort is added.

Evaluator **0x25b21–0x25c01** gates playback on **0x40** plus recognized prefix flags. On the enabled prefix route it invokes the media wrapper and returns success with empty inserted text. A media return/failure is not a filename echoed to host or an automatic HCL abort; following text proceeds. Disabled recognized-prefix handling must follow the existing consumer reserved-feature gate, not fall through into a user-variable prompt. Determine/publish the exact retained-literal behavior without executing providers. Preserve the independent 4096-byte HCL working prefix; the 20-byte parser bound is before state substitution and default extension, not a cap on the expanded name.

## Media-specific extension and platform boundary

Scene **0x324a–0x3373** uses name preparation **0x3f71a**, appends literal **`.RIP` at RVA 0x794e8** if no dot in its returned filename component, and requests normal resource lookup with retrieval off. WAV **0x37e56–0x37f20** prepares name then calls **0x65b40** with literal **`.wav` at RVA 0x803f8**, normal lookup/retrieval off; this helper splits filename components and only supplies a default extension if absent. There is **no `$OFF$` stop sentinel comparison** in this WAV macro path. An escaped parsed filename `$OFF$` becomes a resource filename with `.wav`, not a stop request; missing resource does nothing, while a real file with that name is an ordinary playback. Wire `1w` has its separately documented stop sentinel; use RESET(SOUND) for unambiguous macro stop.

Bitmap **0x38910**, name part **0x3896d–0x389f1**, supplies **`.BMP` at RVA 0x7a90c** without a dot and retains the established legacy extension normalization `.ICN→.BMP`, `.HIC→.BMH`, `.MSK→.BMM` (literals at 0x7a904/0x7a8f4/0x7a8fc/0x7a8e4/0x7a8ec). JPEG common presentation **0x3815f**, extension call **0x381af → 0x65b40**, defaults to **`.JPG` at RVA 0x8042c**. Preserve existing JPEG style/lifetime rules and media gates, not another transfer/palette/deletion contract.

Name preparation **0x3f71a–0x3f775** and extension split/make helpers are platform path machinery, not another HCL variable/escape pass. Publish normal bare resource-name/extension and connection→shared ICONS→search-path lookup rules; do not generalize DOS drive/path punctuation, normalization/recovery or unsafe/private component buffers into portable path permission rules. No real file lookup, playback, sound-stop or platform path probe is required. Empty expanded resource name should be an explicit no-action completion rather than requiring backend-specific `.EXT` fallback.

## Completed reference and validation checkpoint

The shared chapter section now specifies parsed versus transformed names, ASCII uppercase and source ownership; all four `*?[]` substitutions, unsigned/unidentified/outside-handler differences, coherent per-name snapshot and safe callback identity; the decoded 20-byte bound and quoted-dollar rule; literal/missing/overlong/disabled/empty recovery; extension defaults/bitmap aliases and WAV macro OFF versus wire stop. Audio/variable/processing summaries are reconciled. Normal case-insensitive bare-resource lookup stays independent from platform component handling and JPEG presentation/lifetime.

The release path at **0x539d** clears held-left state **before** action dispatch **0x53cc**, confirming the normal `?=0` release-action vector without inferring field selection from button state. Native GetKeyState IAT 0x966d0 takes VK_SHIFT 0x10 / VK_CONTROL 0x11 and tests AH; no ALT placeholder is added. No UI or key-event probe runs. The accepted press path sets +0x0a only after finding a field (0x5345), so distinguish this tracked MSTAT/capture bit from an independent physical-button poll outside all fields.

Independent inline Python checks pass **nine baseline hashes**, **192 four-prefix/identity/left/Shift/Control combinations**, requested ordinary/unidentified/outside/mixed/repeated names, 19/20/21 decoded lengths and quoted-dollar boundary, double-backslash/immediate-dollar semantics, 100-byte expansion plus extension, empty/disabled/missing/overlong/unclosed literal outcomes, explicit/default extensions and all bitmap aliases, WAV `$OFF$` filename, source immutability and following reply. Pure models do not perform lookup/playback or claim native parser/provider execution.

The fingerprinted inspector independently rereads RVA **0x26dd2**, length **238**, SHA-256 `4d943205c295206466b5d466006a81e0a39e1c22b845694adac59e6161f20950`; RVA **0x25971**, length **266**, SHA-256 `d709decb38bcf4d297639fb8bf58c7e8cd92061622347dc596d794987445bf4d`; RVA **0x15602**, length **2398**, SHA-256 `3ac5ab74fa21075d93ec0bdfcc443f9fec0ecc5ac7b109d92f32bc2289df259d`; RVA **0x25b21**, length **225**, SHA-256 `87db3495450586e9e7ee1ee8d9caf46927a1f5182acdbe69edc07d833bf3ae9d`. Eleven direct call targets, seven exact bound/quote/gate/state/key/release sequences and six literal C strings (substitution alphabet, unsigned format, four default extensions) pass independent assertions. No original instructions, event/audio/file providers, dialog or client execute; no test/dump/temporary file persists.

Full repository `run/lint`, `run/check-links` and `git diff --check` pass: **0 bad of 6,476 links across 269 files**. Targeted prose spacing and exact dollar-quoting literal review pass; #161 body/update/zero comments remain unchanged.

Latest all-author issue check **2026-11-23T02:26:59Z** found no newer report; next about **02:36:59Z**. Publish separately, normal-merge actual source head with identical-tree proof, audit feedback/coverage/ancestry and resume ten-minute monitoring. Keep protocol/source limits, Codex trailers, original text/assets, FastFont separation and temporary cleanup.

## Delivery checkpoint

[PR #163](https://github.com/bbs-land/remote-imaging-protocol/pull/163) is OPEN/non-draft and closes exactly #161. Specification **395eb9d26c9b6f3924b47d2f10dcb3d4f5fd1991**. Push this complete delivery checkpoint, normal-merge its actual source head into fork stable with identical-tree proof, audit feedback/coverage/ancestry and resume the standing ten-minute watch. No upstream merge/manual closure; temporary API payload directory removed automatically.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
