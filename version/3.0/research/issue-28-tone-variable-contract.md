# Issue #28: tone defaults, sweep validation and MUSIC sequence

[Research backlog](issue-backlog.md) · [Tone variables](../ripscrip/4.4-tone-and-sound-generation.md)

Started after [PR #61](https://github.com/bbs-land/remote-imaging-protocol/pull/61) delivered #27. Branch `issue-28-tone-variable-contract` starts at **b0827a4**. [Issue #28](https://github.com/bbs-land/remote-imaging-protocol/issues/28) asks for PHASER/REVPHASER ordering and invalid-input/default fallback including zero TIME, T omission/default/trailing-silence behavior, BLIP's 25-versus-10-ms conflict and the exact MUSIC sequence. Preserve historical text and earlier-version behavior; use an independent original-driver oracle, not SyncTERM as specification.

Primary artifact: RIPtel 3.1 [riptel.msi](https://files.bbs.land/rip/RIPTerm%20Installers/RIPTel%203.1/riptel.msi), SHA-256 `04915952dcb04013a164eae1b6f6c5f5335798053d4d0e69b3f4da24f812dd2c`; original extracted `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`, base 0x10000000. Addresses are RVAs unless marked file offsets. Original artifacts remain outside checkout in the authorized mirror. Native/static only; capture tone requests without audio playback or desktop GUI. Scoped temporary builds/request files must be removed promptly.

Initial conflict inventory: PHASER syntax says ascending while its recipe descends 2500→50 step20/time2. REVPHASER syntax/prose says descending while defaults/recipe ascend 50→2500. T syntax allows 0..2 arguments but prose requires two and incorrectly equates it with BEEP despite no trailing gap. BLIP default recipe 25 ms conflicts with example 10 ms. MUSIC's nine frequencies 1300,1200,1100,1000,900,800,700,850,950 do not equal SyncTERM's swept segments. No answer published yet.

Next: identify actual registered callbacks via original 0x25c02 initialization; trace their numeric/default validation, queued sound service and reset. Capture full parameter matrices and MUSIC records with controlled audio services, then update the reconstructed reference and file one complete PR.

## Original callback and request checkpoint

Original variable registration identifies **ALARM 0x8600, BEEP 0x868e, BLIP 0x873a, MUSIC 0x87e6, PHASER 0x8892, REVPHASER 0x89d2 and T 0x8b0d**. Every audio operation is a direct synchronous **KERNEL32 Beep(frequency,duration)** call through IAT **0x964fc**. No call in these callbacks inserts the draft's extra BEEP/BLIP delays. The native fixture replaces only that stdcall IAT service, records arguments and returns controlled success/failure, so no sound/desktop process runs. Original numeric predicate **0x65589**, number conversion **0x68c10**, callback control flow and full variable expansion execute. No tone callback uses a separate HLP zero-duration error string as validation: callback state word variable-system +0x436 is the error oracle.

BEEP accepts 0/1/2 parameters with defaults **1000 Hz, 75 ms**. BLIP also accepts 0/1/2, but its actual defaults are **1000 Hz, 25 ms**, not the draft's 50 Hz. Both allow zero frequency/duration through to the OS request; >65535/nonnumeric/extra arguments set the variable error without Beep. **T requires exactly two parameters**: no omitted defaults are implemented. Explicit T and BEEP with the same pair produce identical requests, with no callback-added trailing gap. The OS may reject a request and its return is ignored by the macro. This establishes requested tone events, not waveform/device timing.

MUSIC defaults count 4 and sends **six** pairs per repeat, in order: **(1125,17), (775,14), (625,10), (725,14), (825,10), (600,25)**, Hz/ms. ALARM defaults 3 and sends (320,200), (160,225), not the inherited second duration 425. Both accept count zero as no requests; counts above 65535 and extra parameters error. Neither the draft's nine-note MUSIC recipe nor SyncTERM's three sweeps is the original RIPtel 3.1 sequence.

PHASER defaults (2500,50,20,2), requires start>stop and nonzero increment/time; REVPHASER defaults (50,2500,20,2), requires start<stop and nonzero increment/time. All fields must fit unsigned WORD. Omitted trailing arguments retain their individual defaults; invalid specified values set the variable error and generate **no default fallback**. PHASER accepts stop zero, REVPHASER accepts start zero. Default PHASER requests 123 tones 2500..60 by -20; REVPHASER requests 123 tones 50..2490 by +20, each 2 ms.

Both sweeps update their frequency in a **16-bit register**. On underflow/overflow it wraps; some otherwise valid inputs cycle rather than terminate (downward stop zero and upward stop 65535 cannot satisfy the exit test). The fixture uses a setjmp/longjmp guard at the 65,537th Beep request, after one complete WORD domain; it compares original count/hash/cycle behavior with an independently expressed modular model. No unbounded audio/wait is executed. This shipped defect must be described separately from ordinary ordered sweep semantics.

Expanded `run/riptel-macro-probe` passes **1,576,902 checks**: exhaustive 0..65536 frequency/duration values on both axes for BEEP/BLIP/T, arity/default/invalid forms, exact count/hash of MUSIC/ALARM through count 65535, 2×6⁴ sweep boundary combinations and every optional suffix, plus full expansion with OS success/failure. A first pipeline assertion expected expansion failure for invalid tone arguments; the original pipeline instead returns 1, retains the literal expression and sets its variable error word. Corrected the harness to assert that exact behavior. No primary observation required a fallback/default reinterpretation. Next: publish the complete requested contract and corrected original recipe, then required checks/PR.

## Publication scope and reproduction

The reconstructed tone chapter now states the original request contract for all seven registered tone variables. It corrects PHASER/REVPHASER endpoint direction, invalid-input rejection without fallback, sweep time-zero errors versus simple-tone zero forwarding, required T pair, missing BEEP/BLIP gaps, BLIP 1000 Hz / 25 ms default and exact six-note MUSIC. The adjacent ALARM second-duration discrepancy is also corrected from the same native source. The summary inventory and script catalogue point to this evidence; earlier version text/reference, WAV playback and SyncTERM code are unchanged. Shipped WORD wrap/cycling is explicitly separated from ordinary finite sweep semantics.

Reproduction uses FreeBSD 15.1 amd64, clang 19.1.7 with native -m32 and the original DLL; see the wrapper for build/service boundaries:

```sh
run/riptel-macro-probe /path/to/original/ripscrip.dll
run/format version/3.0/ripscrip/4.4-tone-and-sound-generation.md version/3.0/ripscrip/9.1-text-variable-reference.md
run/lint
run/check-links
git diff --check
```

All **1,576,902 native macro checks** pass. New tests execute no audio or desktop GUI and write no real sound files; their temporary build directory is removed on wrapper EXIT. PR/request payloads use scoped TemporaryDirectory. Initial cross-reference used a nonexistent summary anchor; corrected it to the actual `sounds-active` heading before publication. Final required check results and PR link follow at delivery.
