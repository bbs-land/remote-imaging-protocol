# Issue #277 — Bookmark variable ownership and approval

[Issue #277](https://github.com/bbs-land/remote-imaging-protocol/issues/277) · [Bookmark credential profile](../ripscrip/5.1-text-variables.md#bookmark-credential-lifetime-and-approval) · [Research backlog](issue-backlog.md)

RealDeuce filed #277 at 23:35:13 UTC on 2026-11-24. The 23:39 UTC in-thread check found it with no comments. Branch `issue-277-bookmark-variable-policy` starts at fork stable `162ec45d4feae99273e3747aab253b53b35f8af9`. The [help extraction](riptel-help-extraction.md) identifies both names. The [registration map](issue-6-macro-evaluation.md#registration-map-and-first-native-checks) and [live-store derivation](issue-175-rip-define.md#editor-and-store) distinguish a client-defined live record from a built-in value.

The [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/riptel-unpacked.exe`, SHA-256 `953a4d08042e6ddef7d581341faefde0e1c868fe00dd57ad83be1557c2524d13`, and `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. The image bases are `0x00400000` and `0x10000000`. Addresses below are RVAs.

## Observed record lifecycle

The client calls its connection setup routine at `0x179c2` from the successful socket-connect event path at `0x083e3–0x08404`. This path tests event 16 and a zero error word. [Winsock notification fields](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsaasyncselect).

Within that setup, the names are `USERNAME` at EXE RVA `0x4a064` and `PASSWORD` at `0x4a058`. The imported `RIP_DeleteTextVariable` calls at `0x17a9b` and `0x17ab4` precede either definition. The byte tests at `0x17aee` and `0x17b0e` skip an empty field. Calls at `0x17afe` and `0x17b21` define the corresponding nonempty value with fourth argument 1. Thus a new setup replaces the previous live values independently, and an empty field leaves no client-installed live record after setup.

The driver's `RIP_DefineTextVariable` export at `0x7463` calls the ordinary live-store helper at `0xe7a8`. Its word at record offset `+0x7a` is cleared for the nonzero fourth argument. It represents a confirmation request in the ordinary record policy. Neither name is a registered driver intrinsic. A present record participates in the existing live lookup and mutation rules. When the client record is absent, ordinary same-name live or persistent values may still exist. The source-backed facts here are record installation and ownership.

## Portable policy and checks

The portable profile reserves both names before ordinary variable lookup. The credential provider belongs to the client. It associates only nonempty fields from the selected bookmark with an established connection and clears the association on disconnect or before a reconnect. A direct connection without a bookmark has no associated fields. This makes the lifecycle explicit for portable implementations.

Every host-controlled evaluation sees zero replacement bytes and unavailable status for either name. This includes existence checks and nested or deferred consumption. A directive, redefinition, deletion or restore from host-controlled input cannot expose or alter the credentials. A client can support local submission in trusted UI for the verified destination, authorized per submission. Denial, cancellation, missing values and a connection change all terminate that submission with no credential bytes. No general variable transparency switch grants access.

The validation matrix uses synthetic field-presence booleans and session identities. It checks both fields empty or populated, switching to a bookmark with one empty field, a manual connection, disconnect, approval denial, stale approval, and host-controlled lookups and mutations. The source check fingerprints both artifacts and confirms the ordinary define/delete call identities and independent empty-field branches.

```sh
sha256sum "$client" "$dll"
objdump -d --start-address=0x417a84 --stop-address=0x417b29 "$client"
objdump -d --start-address=0x10007463 --stop-address=0x100074a8 "$dll"
```
