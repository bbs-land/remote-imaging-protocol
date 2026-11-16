# Issue #28: tone defaults, sweep validation and MUSIC sequence

[Research backlog](issue-backlog.md) · [Tone variables](../ripscrip/4.4-tone-and-sound-generation.md)

Started after [PR #61](https://github.com/bbs-land/remote-imaging-protocol/pull/61) delivered #27. Branch `issue-28-tone-variable-contract` starts at **b0827a4**. [Issue #28](https://github.com/bbs-land/remote-imaging-protocol/issues/28) asks for PHASER/REVPHASER ordering and invalid-input/default fallback including zero TIME, T omission/default/trailing-silence behavior, BLIP's 25-versus-10-ms conflict and the exact MUSIC sequence. Preserve historical text and earlier-version behavior; use an independent original-driver oracle, not SyncTERM as specification.

Primary artifact: RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`; original extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base 0x10000000. Addresses are RVAs unless marked file offsets. Original artifacts remain outside checkout in the authorized mirror. Native/static only; capture tone requests without audio playback or desktop GUI. Scoped temporary builds/request files must be removed promptly.

Initial conflict inventory: PHASER syntax says ascending while its recipe descends 2500→50 step20/time2. REVPHASER syntax/prose says descending while defaults/recipe ascend 50→2500. T syntax allows 0..2 arguments but prose requires two and incorrectly equates it with BEEP despite no trailing gap. BLIP default recipe 25 ms conflicts with example 10 ms. MUSIC's nine frequencies 1300,1200,1100,1000,900,800,700,850,950 do not equal SyncTERM's swept segments. No answer published yet.

Next: identify actual registered callbacks via original 0x25c02 initialization; trace their numeric/default validation, queued sound service and reset. Capture full parameter matrices and MUSIC records with controlled audio services, then update the reconstructed reference and file one complete PR.
