# Issue #206 — ISPALETTE return policy

[Research backlog](issue-backlog.md) · [ISPALETTE](../ripscrip/5.1-text-variables.md#ispalette)

## Scope and checkpoint

[RealDeuce report #206](https://github.com/bbs-land/remote-imaging-protocol/issues/206), created 2026-11-24 00:41:58 UTC, body SHA-256 `3985483a5276ebba5aa5313b4213ffb405ac690e745bcd4f2821eedd30cc99c3`, zero comments at intake. Started from fork stable `c72a3ec219fef525a000e11d44ae49a1e9429e33` after the 01:59:32Z audit. The complete source is a short argument gate and literal return. Next verify the raw sequence, relate it to the immutable draft and adopt a renderer-independent portable result for the RIPtel profile.

## Original-source trace

The [2.00 Alpha 4 description](../../2.0/text/RIPScrip-2.0-alpha-4.txt) specifies a physical-capability answer. The matching driver in the [PCMicro artifact mirror](../../../reference/rip-tools.md) is `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses are RVAs from `0x10000000`. LLVM objdump 19.1.7 and raw PE reads establish the full routine.

- Registration pushes handler `0x182fc` at `0x25e81`, the `ISPALETTE\0` string at `0x7e5f8` from `0x25e86`, and calls the registrar `0x2556d` at `0x25e8c`.
- The routine at `0x182fc–0x1831d` reads the decoded argument count and the text-variable context. Nonzero count sets WORD +0x436 to 1 at `0x1830b`, then returns NULL. The zero branch at `0x18318` returns the literal at `0x7970c`, bytes `31 00`. The NUL ends the storage string, and the replacement byte is `31`. The routine has no ENV, port, palette, color-mode or backend reads.
- The adopted RIPtel-compatible profile keeps the constant even for an RGB framebuffer. The promised logical palette is a RIP drawing feature. No inference about which historic Windows display modes the client opened is needed. Direct-RGB selection and precision already have their own COLORMODE entry.

```sh
dll=../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
sha256 "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x100182fc --stop-address=0x1001831e "$dll"
llvm-objdump19 -d --no-show-raw-insn --start-address=0x10025e81 --stop-address=0x10025e91 "$dll"
```

Map raw strings through the PE section table. The callback's .text file offset is RVA−0xc00. No client or display probe ran.

## Verification

Verified the original DLL SHA-256, registrar arguments and call target, the argument-count and error instructions, the immediate pointer and terminated 1 bytes, and the four documented fixtures. This is a read-only source check. The client and native display code were not executed.

## Resume

Verify and publish the portable rule and reconcile the index.

## Delivery

[PR #217](https://github.com/bbs-land/remote-imaging-protocol/pull/217) is open for upstream review. Specification commit e86b3c34f7cc70fb9f14ea370505cccdbd34bb82.
