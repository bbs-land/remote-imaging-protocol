# Issue #5: scene headers and wire entry points

[Research backlog](issue-backlog.md) · [Header reference](../ripscrip/1.1-protocol-and-command-hierarchy.md#scene-header-segments)

## Status and questions

Started 2026-11-16 for [issue #5](https://github.com/bbs-land/remote-imaging-protocol/issues/5), branch `issue-5-scene-headers`, starting from `996b1a3` (issue #4’s delivered branch, PR #30; itself dependent on PR #29). No resolution PR yet; investigation incomplete. Queue refresh confirms #5 is the oldest issue without a complete resolution PR.

Required: shipped header byte grammar, framing/termination, field widths/radices, reset/protection behavior and a complete example; identify or explicitly exclude RIP_TextMetric, RIP_FontAttrib, RIP_SwitchDirectory, SyncTERM-only 1N/1O/1S and level-0 ESC from the executable 3.0 contract; distinguish an audio table from a single headerable state object. Article selection’s wire-layout reconciliation remains with issue #2.

## Primary artifact and initial leads

Use the exact [issue #3 driver](issue-3-extended-font-style.md#artifact-recovery-2026-11-16), SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. All code addresses are RVAs relative to preferred image base 0x10000000. Artifacts remain under the external mirror. No GUI work is required for dispatch/state probes; any GUI research still requires mandatory isolation in AGENTS.md.

The existing reference carries the Alpha 4 header flag list forward and hypothesizes per-object segments from error strings. RIPlib supplies unverified leads: multiple `h` dispatch records share handler `0x1cae1`; FontAttrib may be `q`, handler `0x1c799`; SwitchDirectory may reach `0x46f66`. RIPlib’s own header implementation explicitly leaves selection between equal-length forms unresolved; do not adopt it as a contract. Verify record discrimination and primary consumer paths.

## Next action

Dump all header/target dispatch records, trace `0x1cae1` and parser record selection, then map segment effects. Preserve field/radix hypotheses here until original instructions and state probes resolve them.

---

[Research backlog](issue-backlog.md) · [Header reference](../ripscrip/1.1-protocol-and-command-hierarchy.md#scene-header-segments)
