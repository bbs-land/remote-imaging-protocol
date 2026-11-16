# Issue #21: image-style flags, target defaults, reset and clear modes

[Research backlog](issue-backlog.md) · [JPEG/image style](../ripscrip/4.1-jpeg-images.md) · [IMGSTYLE](../ripscrip/5.1-text-variables.md#imgstyle)

## Scope and artifact

Started 2026-11-16 after [PR #50](https://github.com/bbs-land/remote-imaging-protocol/pull/50) delivered #20. Branch **issue-21-image-style** starts at **508ceea**. [Issue #21](https://github.com/bbs-land/remote-imaging-protocol/issues/21) asks: which image flags delete when viewing is disabled and whether local displays also delete; IMGSTYLE default flags and undefined/offscreen targets; image-area/flag reset on protected and surviving ports; and COPY versus current write mode for initial clearing and aspect-ratio margins. Derive complete state/side effects without requiring exact JPEG pixel reproduction.

Primary unchanged RIPtel 3.1 / driver 3.0.7 `extracted/ripscrip.dll`, 592896 bytes, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, base **0x10000000**, from [catalogued mirror](../../../reference/rip-tools.md) `artifacts/riptel-3.1-pcmicro/`. [MSI source](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 **04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c**. All addresses below are DLL RVAs. Preserve originals and use native/static probes with captured file/GDI operations, no desktop GUI and scoped temporary cleanup.

## First original entry-point trace

Dispatch physical slot **98**, level-1 i, is IMAGE_STYLE **0xc39a**, arguments four XY fields, mega4 flags and 12 reserved characters. Physical slot **103**, level-1 p, is IMAGE **0xc2c6**, one mega4 reserved field and filename. IMGSTYLE original registration **0x26c77–0x26c82** binds callback **0x2f1be**.

IMAGE_STYLE accepts flags **0..63**, normalizes coordinate order through **0x3112e**, converts both corners through **0x31084**, and stores a SetRect at current 0x78-byte port record **+0x4a** with both far coordinates **incremented by one**. Flags are a word at **+0x5a**. Adjacent synchronization functions **0x34488 / 0x34489** are one-byte RETs; never apply five-byte hooks to both. Verify all-zero area behavior after this +1 storage conversion rather than assuming the inherited disable description is literally implemented.

IMGSTYLE has more surface than the HLP synopsis: zero/one argument **queries** image rectangle and flags, formatting **`%hd:%hd:%hd:%hd:%hd`** after reverse coordinate conversion **0x310f2**. First target argument is parsed by **0x98fc**, with CUR (-1) resolving current; **0x338bc** checks defined and port+0x17 bit 2 rejects a type that still needs identification. Undefined/invalid-type branch returns string constant **0x7a100** (previous issues identify `-1`) rather than defining a port. Invalid selector -5 sets variable error.

Set form requires at least **five** arguments. The local flag accumulator is initialized to **zero** at **0x2f1d1**; additional arguments starting at index 5 recognize case-insensitive **ASPECT (1), DELETE (2), NOCLEAR (4), USEPAL (8)** via **0x2f37a–0x2f3e8**; an unknown extra sets variable error before setting. It checks target protection **0x33821**, temporarily selects the target with **0x3393c**, invokes original IMAGE_STYLE, then restores previous current port. Thus the premise that IMGSTYLE has no flags facility is incomplete; native default/target/protection tests are required before documenting this.

IMAGE handler expands its filename then invokes **0x3880e(context,filename,1)**. The latter checks current port state, tests stored image right/bottom at +0x52/+0x56 for zero and normally calls **0x3815f(context,filename,1,retrieve)**. A zero stored right or bottom instead reaches **0x383b0** placeholder/drawing path without ordinary JPEG display. Because IMAGE_STYLE increments endpoints, an all-zero command initially appears to store right=bottom=1; this is a lead to verify, not a published conclusion.

Common image presentation **0x3815f** looks up JPG through **0x3f80e**, calls **0xf363** to classify the file, uses image area or viewport according to **0x34385**, then reaches **0x37fb5** for actual JPEG processing. Its success tail tests image flag **0x10** (16) and calls **0x3f12f**; the effect remains to trace. No image deletion policy is established merely by this entry-point walk. Trace final file operation and actual clear/margin consumers.

## Next action

Trace **0x37fb5** and its JPEG/display helpers, actual delete calls, image-state default/reset producers and clear/margin GDI calls. Reuse table/port geometry fixtures where useful but preserve the relevant original operations. Native coverage must include every flag combination, area zero/one storage, local versus delivered invocation, IMGSTYLE default/keywords/getter/invalid/undefined/offscreen targets, protected surviving ports under reset/header, and captured background/margin write modes. Record controlled decoder/GDI boundaries; do not assert Windows pixel parity.

---

[Research backlog](issue-backlog.md) · [JPEG/image style](../ripscrip/4.1-jpeg-images.md)
