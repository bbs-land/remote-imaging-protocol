# Issue #72: DEFINE_PORT flags, extents and replacement state

[Research backlog](issue-backlog.md) · [RIP_DEFINE_PORT](../ripscrip/7.0-drawing-ports.md#rip_define_port)

## Scope and source

Started 2026-11-17 for [issue #72](https://github.com/bbs-land/remote-imaging-protocol/issues/72). Branch `issue-72-define-port-contract` starts at **e800949**, the integration PR #71 checkpoint. The four required answers concern flag acceptance/actions, nonzero offscreen near coordinates and extent wrapping, protected redefinition selection, and actual replacement failure state including clipboard effects. Keep this separate from the earlier #10/#54 answers. No new resolution PR is filed yet.

Primary source: original RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`; extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Preferred image base is 0x10000000; all addresses below are RVAs. Original artifacts remain unchanged. Use the existing native port probe with controlled OS services; no desktop GUI, Wine or actual bitmap files.

## Original instruction checkpoint

Independently disassembled the complete wire handler **0x466ec–0x46861**. It saves the old current index at 0x46721, validates the target range 1..35 and tests protection at 0x46754. A protected target branches straight to diagnostic/return at 0x46761–0x46766 and0x4684b–0x46861; it does not call the selector. Both corners are converted with 0x31084, then their low-word differences are stored at 0x467a0–0x467b5. A defined unprotected target is deleted through entry helper 0x3302c. The constructor receives only `flags & 1`; success tests bit 2 for selection. No range check on flags or action on bits 4/8 appears in this handler. These static facts still require native state matrices before publication.

Entry deletion **0x3302c–0x33111** releases offscreen resources, clears the record, restores its numeric slot ID, sets image rectangle to all-1 and leaves the entry undefined. Unlike outer portDelete 0x33112, it does not touch current selection or the clipboard pointer. Constructor 0x3326f can call this entry helper again when a saved nonzero slot ID remains; the undefined-record early return makes that harmless.

**Do not stop at the saved-index store on failure.** Handler 0x467ee–0x467f6 restores the old current word and calls **0x343e4**. This rebinder tests the selected entry's definition and, if undefined, calls the original constructor with screen kind 0 and full-display dimensions at 0x34407–0x34426. A successful fallback defines that selected slot as a screen port; only fallback-construction failure selects 0. The earlier port-probe phase hooks this rebinder to a no-op, so it cannot establish the final outcome here. Restore its original bytes and the real selector/protection functions for the new phase.

Offscreen constructor 0x33502 onward checks its budget, creates DC/bitmap, then on success zeroes stored near origin at 0x3371f–0x33722 while retaining passed dimensions. Screen/offscreen initialization clear protection/disabled flags at 0x3334a/0x3375a. Need native zero/wrapped-width observations and success/failure captures, including old resources, complete unrelated records, selected DC and clipboard pointer.

## Next action

Extend `run/riptel-port-probe` with a phase retaining original handler, entry deletion, constructor, selector, rebinder, protection and coordinate conversion. Cover all low flag combinations plus reachable higher values, screen/offscreen targets, same/different current selection, absent/assigned clipboard, nonzero/reversed/wrapped coordinates and budget/DC/bitmap failure injections. Record any harness failures separately. Only then correct the reference, run required checks and file a complete separate PR closing #72. Preserve the draft claims as historical context.

---

[Research backlog](issue-backlog.md) · [RIP_DEFINE_PORT](../ripscrip/7.0-drawing-ports.md#rip_define_port)
