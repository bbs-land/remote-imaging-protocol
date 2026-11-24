# Issue #210 — Character-set and language providers

[GitHub issue #210](https://github.com/bbs-land/remote-imaging-protocol/issues/210) · [ANSI terminal profile](../ripscrip/1.4-terminal-ansi-view.md#byte-character-sets-in-riptel-31)

## Scope and checkpoint

RealDeuce filed this issue at 2026-11-24 01:20:19 UTC. The issue had no comments at intake. Use RIPtel 3.1's PC Micro DLL, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, static addresses as RVAs. Callback entries `0x2bb73`, `0x2bb93`, `0x2bc98`, `0x2be34` and `0x2bebe` establish CDCS, CHARSET, DCS, LANG and SDCS respectively.

The shared table access at context offset `0x0e`, 16-byte stride and index offset `0x0a` identify the [environment subsystem](issue-172-local-playback-baud.md#live-rate-and-setter-grammar). These are ENV references. Text windows have a separate subsystem with its own byte-to-glyph state.

## Selectors and the two state layers

`0x98fc` selects current on CUR or an omitted argument and accepts an unsigned decimal 0–35. The callbacks synthesize CUR and QUERY where needed by copying the actual literals at `0x7a104` and `0x7eaf4`. Each text argument record is 21 bytes wide. Valid explicit nonzero targets run the ENV definition test at `0x3d6f9`. In-range undefined entries return literal `-1` before reaching DCS keyword dispatch. CDCS has only the zero-argument branch and constant `0x7ee8c`. LANG compares three keywords and reads the six literals beginning at `0x7ee9c`. Case-insensitive argument comparison uses `0x655e2` throughout.

ENV record offset `0x09` is 0 for USASCII, 1 for KANJI and 255 for inherited. CHARSET reads the override at `0x2bc42` and consults subsystem byte `0x10` at `0x2bc77` only when no explicit override is set. DCS QUERY reads the record at `0x2bd7a`; its inherited response is ASCII `0` from `0x79714`. The name setters store 0 and 1 at `0x2bdeb` and `0x2be04` without a protection gate. DCS OFF tests ENV flags mask 2 at `0x2bdcd`; only the set/protected branch writes 255. The success string at `0x79594` is empty. SDCS accesses only the fallback byte, with query at `0x2bef6` and three stores at `0x2bf3e/0x2bf5f/0x2bf80`.

## Reset, backup and rendering

`0x3d54f` zeroes the ENV subsystem, so the initial fallback is 0 (USASCII). The 16-byte record initializer writes the override 255 at `0x3d646`. Selection at `0x3dac0` can use this initializer for missing slots and does not copy the incoming override to the departing slot. SOFT and HARD default ENV records via `0x164fe` and `0x16946` respectively. Their ENV initializer and activation leave the shared byte in place. For the full distinction between protected SOFT survival and HARD ownership use the linked [shared reset profile](../ripscrip/1.3-world-view-virtual-canvas.md#hard-reset-state-and-lifetime).

RESET action table `0x82430/0x82434` associates CHARSET with handler `0x43293`. In the explicit three-argument form the second argument selects DCS or SDCS. DCS uses the third argument as CUR, numeric ENV or TBL and clears defined unprotected overrides at `0x433ce` or `0x4343a`. SDCS clears the shared byte at `0x43393` or `0x43406`. The HEADER type 9 consumer `0x17b7a` also writes this byte; its existing framing qualification is unchanged. The ENV backup writer `0x234d2` writes the 36 complete records, index and separate live baud value, so DCS overrides participate and the shared fallback is outside the payload.

The [#104 ANSI byte trace](issue-104-terminal-character-sets.md#designation-mapping-and-invocation) already follows the text path through window designation/invocation and the current emulation hook. These use the TW state, with no ENV override/fallback or LANG input. The two environment lock hooks called by these providers both return immediately. The specified compatibility profile preserves that separation and uses the interpreter's ordinary byte rules for ANSI output.

The read-only verification checked twelve literals, six direct targets, initialization and protection instruction bytes, the RESET action table and 90 modeled override/fallback/protection/operation combinations.

## Verification

Cross-checked original selectors and output literals, record layout and initializers, RESET and HEADER dispatch and ENV backups. Checked read-only state vectors, Markdown, links and whitespace.

## Resume

Finish the selector, protection, initialization, preservation and ANSI-output traces. Update the built-in chapter, index, ENV state description and terminal cross-reference, then verify and deliver the PR.
