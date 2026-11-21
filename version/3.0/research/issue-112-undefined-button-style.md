# Issue #112: Undefined current button-style state

[Research backlog](issue-backlog.md) · [Button styles](../ripscrip/4.3-buttons.md#initial-and-reset-style)

## Scope and checkpoint

Started 2026-11-21 on `issue-112-undefined-button-style` from fork stable fc9588f. [Issue #112](https://github.com/bbs-land/remote-imaging-protocol/issues/112) follows #27 / PR #61 and #16 / PR #45: specify specific/table reset selection, explicit `1B` writes while the current slot is undefined, subsequent first-use defaulting and protection/query behavior when the already documented current-reset metadata overwrite hides a protected record. Preserve those earlier default/flag/reset answers, outer SOFT's separate selection of zero, 1.54 and separate FastFont research.

## Primary sources and boundaries

Original RIPtel 3.1 / RIPscrip driver 3.0.7 `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll` in the [catalogued mirror](../../../reference/rip-tools.md), 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Image base `0x10000000`; all addresses below are RVAs. LLVM objdump 19.1.7 reads instructions only; file biases are `.text` minus `0xc00`, `.rdata` minus `0x1000`, `.data` minus `0x1a00`.

The immutable [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt), SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`, §3.4.2.3 (line 8317 onward) defines `1B` as altering the current button style; §3.4.3.6 (10710) says outer reset selects zero. §2.2.2 (1981) protects defined configuration and says an undefined entry cannot be protected (2056). CUR/ISPROT descriptions and the earlier [#16](issue-16-table-lifecycle.md) and [#27](issue-27-button-style-contract.md) derivations supply the already adopted state/first-use/reset/query distinctions. Do not reopen default fields, flag applicability, current-reset behavior, outer reset or 1.54.

No original code, GUI, probe, button renderer or invalid memory operation is executed. Existing probes are historical evidence only; new work reads bounded consumers and checks bytes/state arithmetic. This is not a new investigation of the already documented obsolete-client reset defect.

## Four independent state components

Button system is context +0x16; current index WORD +0x0a; record pointer +0; records have stride 0x1c; in-use bytes +0x0c; protection bytes +0x30. Record edits and in-use transitions must not be inferred from each other. In-use helper `0x456fb` returns true for nonzero bytes (the numeric path compares against 1 at `0x45761` and normalizes its result); protection helper `0x45674` reads the current or numbered protection byte without an in-use check. Slot zero is treated as defined and unprotectable.

### Reset versus outer selection

Original reset `0x4558d` has separate current, table and numeric branches, already established by #27. The numeric branch `0x45626–0x45662` rejects protected targets, copies fixed defaults into the requested record and clears its nonzero in-use byte; it does not store current +0x0a. Table branch `0x455e9–0x45626` defaults only unprotected records, clears their nonzero in-use flags, preserves protected records/flags and restores in-use[0]=1; it likewise never selects zero. Outer SOFT calls table reset at `0x164a2`, then separately writes current BUT=0 at `0x164ad`, as already documented in [#105](issue-105-outer-soft-reset.md). This is distinct from RESET(BUT,CUR)'s already adopted metadata overwrite.

### Successful explicit definition establishes in-use

Complete style handler `0xb325–0xb7a4` caches the button-system pointer in local -0x20 at `0xb386–0xb38b`. It validates fields and calls current protection helper `0x45674(-1)` at `0xb558` before mutation; a protected current slot rejects the definition even when its in-use byte is false. No initial in-use gate or first-use selector is called. The accepted path clears the actual current 28-byte record at `0xb632–0xb646`, writes all style fields and applies the already documented bevel normalization. Its decisive tail is:

```text
RVA 0xb77a: load cached button-system pointer
RVA 0xb77e: read current index WORD at +0x0a
RVA 0xb782: store byte 1 at system +0x0c + current
```

Exact store bytes at `0xb782` are `c6 44 08 0c 01`. Thus a valid accepted `1B` on an unprotected undefined current slot **defines it**, retaining the current index and protection byte. It does not merely edit an undefined record, reject because it is unused or default/select another slot. A later selection sees the now-defined record and retains it. The question's conditional “if it edits without defining” does not apply to successful `1B`. A rejected definition leaves the four target-state components unchanged.

### Selection and stale protection

CUR setter calls selector `0x4576f` at `0x3500a`; query-only CUR reads current metadata without calling it. Selector calls in-use helper at `0x45795`. If false, `0x457a2–0x457c3` copies the fixed default record from RVA `0x828e8` (file `0x80ee8`), writes in-use=1 at `0x457b9`, protection=0 at `0x457be`, then sets current index at `0x457c6`. There is no protection rejection in that undefined branch. If true, it skips all three writes and only selects, even for the already current/protected target.

The normal documentary invariant says undefined entries cannot be protected. The inherited current-reset overwrite can leave just such inconsistent metadata. **Explicit completion for that already adopted state:** apply the same first-use rule to every logically undefined nonzero slot, regardless of a surviving protection byte: fixed defaults, in-use true, protection false, selected. This is supported by the exact original selector, but is explicitly scoped as completion of the inconsistent state rather than a new general permission to overwrite protected defined styles. If the overwritten in-use byte remains nonzero, selection treats the slot as defined and retains both record and protection. Do not implicitly select to inspect the flag or repair it ahead of an explicit selection.

### Query gates and creation

ISPROT's CUR branch at `0x2dbc5–0x2dbe7` directly dispatches protection (`0x460a7`), whereas numeric `0x2dddf–0x2de0d` first asks INUSE (`0x45fb1`) and returns −1 if false. BUT dispatch targets are `0x4600c -> 0x456fb` for INUSE and `0x46172 -> 0x45674` for protection; current lookup is BUT system +0x0a. Therefore for an undefined current nonzero slot: INUSE(BUT,n)=0, numeric ISPROT(BUT,n)=−1, but ISPROT(BUT,CUR) reads its retained protection (0 or 1). No definition/defaulting follows a query. The ordinary numeric-reset sequence yields current protection 0; the general CUR query does not acquire the numeric definition gate. No newly reachable wire sequence for every inconsistent metadata combination is implied.

Original `1U` handler `0xa952` reads the actual current record (e.g. `0xaa1c–0xaa4c`, current index multiplied by 0x1c) and captures its style into the created field as already traced by #27. In the requested accepted-definition/reselection sequence this is style B, not defaults. First selecting a truly undefined stale-protected slot instead installs defaults, so subsequent `1U` uses defaults. Existing renderer/auto-clip and protection rules remain adopted; exact pixels are not part of this answer.

## Reproduction and remaining work

Read original instructions, for example:

```sh
objdump -d --start-address=0x1000b325 --stop-address=0x1000b7a4 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1004558d --stop-address=0x100457dc ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002dbc5 --stop-address=0x1002dbe8 ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
objdump -d --start-address=0x1002dddf --stop-address=0x1002de0d ../rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

Use section-aware file offsets for byte checks. Add the complete transition table, literal accepted reset/redefinition sequence and protected-slot example using the already published metadata bytes. Verify source hashes/decisive opcodes and state arithmetic, run Markdown/link/diff checks, refresh report/feedback, publish one PR and consolidate its actual head. Initial 2026-11-21T15:53:50Z scan finds this single uncovered report and zero issue comments. Refresh feedback overlapping 2026-11-20T22:17:25Z. Keep co-author trailers and scoped temporary cleanup; no original/client/UI/probe execution.

---

[Research backlog](issue-backlog.md) · [Button styles](../ripscrip/4.3-buttons.md#initial-and-reset-style)
