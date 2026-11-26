# Issue #193 - Correct the DEFAULT EX placement example

[Backlog](issue-backlog.md) · [Placement examples](../ripscrip/3.0-text-output-and-fonts.md#placement-examples) · [Issue #193](https://github.com/bbs-land/remote-imaging-protocol/issues/193)

RealDeuce filed #193 at **2026-11-23T21:05:08Z**, body SHA-256 `b8df05f1ecf76141fb7efd0b6dfe928a813d2428e8cdfecad2558dade2f16bd0`, zero comments. The **21:15:23Z** audit identified it as the only open RealDeuce issue without a resolution PR. Start at fork stable **de707194a9ea8887797be85d9dfd52e93461960d** on `issue-193-font-zero-example`.

## Wire and placement check

Read the [adopted classic flag map](../ripscrip/3.0-text-output-and-fonts.md#justification-and-special-effect-flags) and its [original DLL trace](issue-186-font-style-flags.md). The reference example in PR #189 specified `Y00000521`. MegaNum `21` is 2×36+1 = 73, or `0x49`. That includes left (1), top (8) and middle (64), so the original-style validation sees two vertical bits. It rejects the whole command while retaining the previous style. The earlier layout check decoded the numeric fields but supplied the expected alignment separately, allowing the bad flag combination to pass.

For left/middle, use 1+64 = 65, MegaNum `1T`. The accepted command is `Y0000051T`. The [DEFAULT EX metrics and shared arithmetic](issue-187-classic-font-zero.md) give Q=56 and P=104 at 800×600 DEVICE and WORLD. `@2S28AB` decodes anchor (100,80). Both bytes have 500 design-unit advance. With 140% face scale and 72 device DPI, each has 784/1440 inch before the DEVICE conversion, then width 39 and gap 1. Total 80. The middle reference at −250 design units contributes −14 DEVICE pixels, so the first origin is (100,94). Neither raster coverage nor metric scope changes.

Reproduce by decoding the four two-digit MegaNums, rejecting more than one flag from either mask `0x07` or `0x78`, then applying font 0 defaults and the selected reference. Use the decoded flags to drive the placement calculation. Check the other two published vectors in the same pass. No original client or renderer is needed.

## Verification

The original DLL and DEFAULT.RFF hashes match. Rejected the old 73 value and independently decoded and applied the flags in all three actual published wire examples. All three expected logical advances and origins match. Verified the one-line specification diff, formatting and link checks.

## Resume

Correct the wire field and decimal value together, verify all three examples and publish a focused PR for #193.

## Delivery

[PR #196](https://github.com/bbs-land/remote-imaging-protocol/pull/196) is open for upstream review. Specification commit **ee1ddd853ccb41e4a9efb4c5765b407f556f998f**.

Fork stable normal-merged the actual head **d4629fa8ed68e377a4549383e6eed5c8e743897c** through **ceb61a4f09eab6e07692a3e6c962f8438435acf6**, with an identical tree. At **21:18:29Z**, overlapping **21:14:23Z**, the audit checked 97 open RealDeuce issues and 96 open PR heads, all ancestral. No new review, discussion or inline feedback appeared. The verified queue is empty.
