# Issue #23: COLORMODE sentinel, WORLD names and environment ranges

[Research backlog](issue-backlog.md) · [COLORMODE](../ripscrip/5.1-text-variables.md#colormode) · [WORLD](../ripscrip/5.1-text-variables.md#world)

## Scope and artifact

Started 2026-11-16 after [PR #55](https://github.com/bbs-land/remote-imaging-protocol/pull/55) delivered #22. Branch **issue-23-environment-query-names** starts at **e5ed876**. [Issue #23](https://github.com/bbs-land/remote-imaging-protocol/issues/23) asks for the exact undefined COLORMODE reply; whether WORLDX/WORLDY are shipping aliases; corrected WORLDH/WORLDW syntax/examples; and consistent presentation of world-coordinate and RGB-bit restrictions. No resolution PR yet.

Use unchanged RIPtel 3.1 / driver 3.0.7 `extracted/ripscrip.dll`, 592896 bytes, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, base **0x10000000**, from the [catalogued mirror](../../../reference/rip-tools.md), [MSI source](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi) SHA-256 **04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c**. All addresses below are DLL RVAs; .text file offset = RVA−0xc00, .data = RVA−0x1a00. Prefer original native callbacks/registration and owned environment records; no GUI or real file mutation is needed.

## Original registration and first callback traces

COLORMODE registration **0x26bd6–0x26be1** names **0x17eac**; WORLD **0x26b30–0x26b3b** names **0x8d5e**; WORLDH **0x261f9–0x26204** names **0x8fe0**; WORLDW **0x26211–0x2621c** names **0x91e8**. WORLDX and WORLDY NUL-terminated strings are absent from the DLL. Absence alone is not full alias proof; check the entire original registration output and ordinary lookup behavior with the existing macro probe.

COLORMODE's one-argument numeric query checks original environment definition helper **0x3d6f9**; undefined branch **0x17fb8–0x17fc6** returns constant **0x7a100**, previously byte-verified as **-1**. Zero arguments / CUR query reads current environment's byte +7 directly. Records have **stride 16**, definition byte +0, width word +1, height word +3, base math byte +5, RGB bits byte +7; environment system current index is +0xa. Validate full outputs and no state/definition mutation natively.

## Range/policy discrepancy requiring investigation

Do **not** merely replace the inherited macro tables with the wire-setter restrictions before following actual consumers. Original COLORMODE **0x180af–0x180c5** validates **1..8**, stores the requested byte in the target environment, and only for a current target calls **0x3e943**. The latter may reject non-eight-bit active modes, but that would not automatically undo the earlier stored state. Query and active-mode restrictions may differ.

WORLD **0x8f6a–0x8f9b** compares dimensions against **65535**, with an AND-shaped rejection path (rejects only if both exceed), stores low words, then calls **0x3e9d4** only for the current target. The macro's explicit range is not evidently 1..32767. WORLDH/WORLDW have analogous 65535 validation and an apparently inverted CUR/set-target branch (**0x90e8–0x914d / 0x92f0–0x9355**) that needs native tests before documenting. These are unverified static leads, not published contracts. Investigate zero, 32767/32768, 65535/65536, asymmetric oversized WORLD values, current versus noncurrent storage and active-setter failures.

## Native callback, activation and wire checkpoint

Expanded `run/riptel-macro-probe DLL` passes **389,954 checks** on FreeBSD 15.1 / clang 19.1.7. It captures all **179 original variable registrations**: the four callback identities above match, WORLDX/WORLDY are absent. Ordinary macro expansion of `$COLORMODE(6)$` produces exact **`-1`**, no variable-error flag, leaving slot six undefined. Canonical `$WORLDH(5)$:$WORLDW(5)$` produces **350:640** for a 640×350 target. Custom user variables named WORLDX/WORLDY expand to their custom values rather than built-in world queries, proving these names occupy the ordinary user namespace and are not shipping aliases.

Owned fixtures include all 36 sixteen-byte environment records, active state/configuration, options, variable state/result and original definition/selector/parser routines. Preserve original **0x3e943** color activation, **0x3e9d4** world activation and **0x31052** active world cache updates. Only environment-refresh notification **0x3c37e** and wire host-flush **0x45038** are controlled as no-ops, in addition to previously catalogued macro host/CRT services. Adjacent **0x3dc6c / 0x3dc6d** synchronization RETs remain original. Initial wire entry faulted in an unprovided host-flush context at 0x45044; controlling that unrelated service fixed the fixture. TemporaryDirectory debug build and wrapper builds were removed; core dumps are disabled.

**The static active-rejection lead above was false:** original **0x3e943** accepts **0..8** and updates the current record and active byte context+2→+0x3a. COLORMODE RGB setters accept all **1..8**, for current and noncurrent defined targets; queries can therefore return 1..7 as well as 0/8. Wire **RIP_SET_COLOR_MODE 0x1f3fd** is the distinct gate: when mode=1 it only calls activation if bits=8. Native tests execute bits 0..9 through both paths, proving that the eight-only wire rule cannot be applied to the macro or shared active setter. Renderer color-conversion fidelity at 1..7 bits is outside these state/reply checks.

**World dimensions have no 32767 setter gate in this driver.** Original wire **RIP_SET_WORLD_FRAME 0x1f874** accepts both dimensions in **0..65535** and rejects either value above 65535 before activation. Original WORLD macro accepts zero and 32768..65535 as well; its incorrect rejection condition rejects only when **both** integers exceed 65535, otherwise stores each low word. Current-target activation zero-extends those low words into active world DWORDs; noncurrent updates leave active cache unchanged. The first fixture expected full oversized input values in active cache; original **0x3e9d4** reads its arguments as words and corrected assertions now check low-word values. Native 7×7 matrices use 0,1,32767,32768,65535,65536,65537 for each dimension, on current/noncurrent WORLD and original wire handlers. Thus `$WORLD(CUR,65536,1)$` stores/activates **0:1**, whereas the wire handler rejects (65536,1).

**WORLDH/WORLDW setters have an inverted target test.** With two arguments, a first argument other than case-insensitive CUR writes the **current** environment without validating the supplied target. Literal CUR reaches the numeric-only environment decoder and errors. Native matrices cover current slots zero/five; axis H/W; targets CUR,0,5,6,BOGUS; the same seven dimensions. Values 0..65535 succeed for non-CUR targets and update the current record and active cache, including when the named slot is undefined or invalid text. Values >65535 reject. CUR rejects unchanged. Getters still honor numeric targets and undefined-query -1. These are shipped defects, not aliases or a reason to publish the intended setter semantics as working behavior.

The white paper's **32767×32767** advertised world limit remains a historical statement, not an executable validation bound. The canonical page's inference “the shipping figure is the signed-16-bit bound” must be corrected rather than copied into macro tables. The original wire f handler compares unsigned values against **0xffff** at **0x1f8df–0x1f8ea** and calls the preserved active setter; the macro checks above independently confirm the storage range. An implementor may choose a stricter geometric profile, but it must be labeled a policy rather than RIPtel's setter gate.

Ordinary expansion also executes the setters: `$WORLDH(CUR,1000)$` errors unchanged, `$WORLDH(5,1000)$` changes current height while slot five stays at 350, `$WORLDW(BOGUS,1234)$` changes current width without error, and `$COLORMODE(CUR,RGB,1)$` followed by `$COLORMODE$` activates/replies **1**. These parser-reachable cases agree with the callback fixtures; the findings are not confined to a direct helper API.

Next update all affected canonical examples, syntax and restriction claims, with explicit wire/macro distinctions and the H/W setter defect, then required checks and one complete resolution PR. Original text editions stay unchanged; no PR yet.

---

[Research backlog](issue-backlog.md) · [WORLD](../ripscrip/5.1-text-variables.md#world)
