# Issue #215 — WRITE_ICON export flags and temporary assets

[GitHub issue #215](https://github.com/bbs-land/remote-imaging-protocol/issues/215) · [RIP_WRITE_ICON](../ripscrip/4.0-icons-and-bitmaps.md#rip_write_icon) · [shared temporary-file trace](issue-21-image-style.md#received-file-cleanup-and-encoded-stream-checkpoint)

## Scope and checkpoint

RealDeuce filed this at 2026-11-24 01:57:36 UTC. No comments were present at intake. Use RIPtel 3.1's PC Micro DLL SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8` and unpacked EXE SHA-256 `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`. Read the wire handler at `0xdd67`, name writer at `0x27930` and export at `0x27a90` statically.

## Filename and pixel source

Dispatch slot 108 selects `0xdd67`, with one current-base digit followed by text. The argument mask at `0xddd2–0xdde5` yields registration bit 0 and the inverse of bit 1 for the normalization selector. The entry has empty-name and drawing-state guards. At `0x27944/0x27971`, an absent clipboard or zero source-type record returns before lookup/export. Source protection is not read, and no remembered rectangle pair is consumed.

`0x3f776` prepares the name using the active host resource location or its `ICONS` default. `0x278e0` uppercases and substitutes the three legacy aliases, in the order ICN/HIC/MSK. `0x27a0c` tests for the first period in the prepared name and the absent-period branch appends the default BMP suffix. The portable document uses the bare name's final suffix and preserves its basename spelling, with case-insensitive resource identity.

`0x27a90` subtracts the stored viewport near edges from the far edges. Source type above 1 is offscreen. The direct offscreen lane at `0x27c8d` converts the complete backing bitmap with `0x4de5f`. The normalized lane uses GetClientRect at `0x27aef` and positive dimensions, multiplies by 350 and 640 at `0x27b65–0x27b90`, and reads from the offscreen backing DC at `0x27c03`. It issues a COPY StretchBlt at `0x27c12`, with the source origin and dimensions taken from the stored viewport. The screen lane at `0x27ce7` allocates the viewport size and COPY blits from the screen backing, independent of the normalization selector. All successful branches converge on BMP conversion/writing, retaining the source geometry and pixels. The portable pixel-center sampling rule completes the StretchBlt filter choice without assuming a particular GDI implementation.

## Temporary registration and cleanup

With bit 0, the export calls `0x18e92` at `0x27ab0` before source copy or BMP writing. The same function receives registered transfer paths from `RIP_AddTempFile` at DLL `0x767b` via the EXE's completed-transfer call at `0x206fe`. It uses the instance number from context `+0x7a` in `RIPSC%03hu.TFL`, opens with `a+t`, and writes one resolved path and newline. The list append is independent of success of later raster export. The prior [controlled registration/purge trace](issue-21-image-style.md#received-file-cleanup-and-encoded-stream-checkpoint) observes pathname removal followed by removal of the registry itself. Those storage details need not be reproduced to provide session lifetime.

The unpacked EXE imports `RIP_PurgeTempFiles` at VA `0x45b854`, through thunk RVA `0x1c00c`. It has exactly two direct calls, the disconnect transition `0x14d0e` and teardown `0x5fc3`. The former is gated by the cleanup argument in connection-state routine `0x14bfb`. Active-close/connection-ending paths call it with true at `0x17221/0x17354`; callers using false bypass registry purge. The optional drawing reset follows the purge and has its own client setting. Teardown calls `RIP_InstanceUnInit` and `RIP_InstanceDestroy` afterward. DLL direct purge calls are the explicit export `0x76ab` and instance uninitialization `0x6610`. Neither ordinary SOFT/HARD drawing reset nor file/clipboard presentation owns this registry.

The portable contract scopes cleanup to a completed disconnect or destruction of the owning interpreter and records only successful exports in managed storage. A later write to the same resource within that session retains its registration state. Directory changes do not retarget old entries. The source path scans and name tests used benign intended resource names and read-only evidence.

Read-only verification checked slot 108, eleven call sites, two import references, six aliases and three registry literals. Modeled checks covered all 128 flag/source combinations, five sample maps, five representative names and scoped cleanup across failed export, directory changes and same-session replacement.

## Verification

Verified dispatch and flag producer, alias and registry literals, all three pixel-source lanes, client/DLL purge ancestry, and portable output-size and lifetime vectors. Checked Markdown, links and whitespace.

## Resume

Read the export and temporary-registration call graphs. Specify the filename, pixel-source, resampling and cleanup contract for supported bare-name requests, with positive geometry vectors.

## Delivery

[PR #226](https://github.com/bbs-land/remote-imaging-protocol/pull/226) is open for upstream review. Specification commit 89fcc96ae8c277a8a8f6de15513e304730e3dd96.
