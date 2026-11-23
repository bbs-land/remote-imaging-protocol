# Issue #201 — Selective MKILL arity and selectors

[Research backlog](issue-backlog.md) · [MKILL](../ripscrip/5.1-text-variables.md#mkill)

## Scope and checkpoint

[RealDeuce report #201](https://github.com/bbs-land/remote-imaging-protocol/issues/201), created 2026-11-23 23:03:47 UTC, body SHA-256 `4611c33010d6a2a888d69d45c89773d70abcdec1625f29f322c9f20e5edbf152`, zero comments on intake. Started from fork stable `02aaf631f448c68e5bfc1931063949e5c2a37410`. The 23:09:16Z audit found #201 the only uncovered report and all 99 open PR heads ancestral. Preserve the existing argument, geometry, field interaction and no-paint contracts.

## Sources

The [2.00 Alpha 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), §5.12.1, gives the whole-table form, 0..65535 WORLD coordinates, `IN` and `OUT` semantics and the explicit default of IN. The four-coordinate form is a portable adoption of that default. The [1k derivation](issue-120-selective-field-deletion.md) supplies the adopted three-class half-open geometry. [Tab focus](issue-197-tab-navigation.md) and [mouse commitment](issue-113-mouse-gesture-identity.md) supply the surviving interaction state rules.

The unchanged [PCMicro distribution driver](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, is 592,896 bytes with SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. These DLL addresses are RVAs from `0x10000000`. For the cited .text/.data bytes subtract `0xc00`/`0x1a00`, respectively. Static reads use LLVM objdump 19.1.7.

## Original callback

1. `0x265e3–0x265ee` registers `MKILL` (string RVA `0x7e3b8`) with handler `0x347d6`. At zero arguments the direct call at `0x347ef` reaches `0x27f8f`, also used by the `1K` entry. Nonzero first passes `0x347fc`'s equality test against 5. Other arities reach variable error +0x436 at `0x3495e`.
2. The four arguments pass the decimal-digit predicate `0x65589` at `0x3480a, 0x3481f, 0x34834, 0x34849`. Each calls conversion `0x65b29` with base 10. Calls at `0x348ab` and `0x348bb` order the axis pairs using `0x3112e`. Calls at `0x348cc` and `0x348dd` use `0x31084` to convert each corner. The rectangle setup at `0x348e5–0x34902` sends all four coordinates unchanged to SetRect and then calls the existing rectangle preparation helper. There is no viewport-origin addition or increment of the converted far edge in this callback.
3. `0x3490a` references the string `IN` at `0x8025c`. Comparison `0x655e2 → 0x6f540` returns zero on equality. At `0x3491b` a zero result goes to `0x34923`, which compares to `OUT` at `0x80258`. This result is nonzero and loads selector flag 4 at `0x34936`. A nonzero result against IN instead falls through to flag 1 at `0x3491d`; this includes OUT and any other nonempty token. The ordinary selector calls `0x12d63` at `0x34949` with exactly that region flag and no paint bits. None of these flag observations redefines the underlying native rectangle predicates. Successful action returns NULL at `0x34967`.
4. Native selection suspends and resumes the caret around the removal helper at `0x3493b` and `0x34954`. Portable deletion preserves underlying screen pixels and removes affected transient feedback using the shared stable-instance rules.

## Adopted protocol behavior

Use the ordinary decoded argument rules, including omission of empty comma fields, before applying the effective arity. The selective form accepts four unsigned decimal coordinates (independent of Base Math) and the optional ASCII-case-insensitive selector. A bad argument never runs the selector. For a valid call use current WORLD, absolute half-open device coordinates and class C for IN, class O for OUT. For a zero-width or zero-height converted rectangle, no field is removed. No painting is requested, and crossing P survives both. Once the removal set has been established, retain surviving definitions, pointer captures and hover memberships under the existing rules. Clear Tab focus when a live field is removed, with no synthetic field input or changes to existing artwork.

The literal case uses identity conversion, selector device `[20,60) × [20,60)`, A `[30,40) × [30,40)`, B `[50,70) × [30,50)`, and C `[60,80) × [30,50)`. The sentinel pixels belong to the backing screen. No action can recolor or erase them by deleting the definitions.

## Reproduction

```sh
dll=../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
sha256 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x100347d6 --stop-address=0x10034970 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x100265e3 --stop-address=0x100265f3 "$dll"
```

Map raw RVA reads through the PE sections, read the terminated literal strings, and check direct-call targets as call site +5 plus signed displacement. Static evidence is limited to the documented callback boundary. No old client, graphical probe or handler was executed.

## Verification

Verified the DLL and draft SHA-256 fingerprints, 23 direct calls, five instruction signatures, registration and selector bytes, and the native arity/selector branch outcomes. Decoded each field and marker pixel in the published setup. Independently checked all seven portable rows and additional arity, range and equality cases. Formatting, 6,866 links before delivery metadata, and diff checks passed. No original handler, client or graphical probe was executed.

## Resume

Publish [MKILL](../ripscrip/5.1-text-variables.md#mkill), merge the actual PR head into fork stable, and refresh GitHub from 23:08:16Z or earlier.

---

[Research backlog](issue-backlog.md) · [MKILL](../ripscrip/5.1-text-variables.md#mkill)

## Delivery

[PR #204](https://github.com/bbs-land/remote-imaging-protocol/pull/204) is open for upstream review. Specification commit **d0311ca228598ecc766d25ce1c79544b33a4c40f**.
