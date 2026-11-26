# Issue #269 — PLAY_AUDIO flags and filename expansion

[Issue #269](https://github.com/bbs-land/remote-imaging-protocol/issues/269) · [RIP_PLAY_AUDIO](../ripscrip/4.5-audio-files.md#rip_play_audio) · [Research backlog](issue-backlog.md)

RealDeuce filed #269 at 22:49:04 UTC on 2026-11-24. The 22:58 UTC in-thread check found it ahead of #270. Branch `issue-269-play-audio-repeat` starts at fork stable `b553d21cd36ed33713e93edd6d2d632202cff310`. The [HCL media derivation](issue-161-playback-filenames.md) establishes the separate macro spelling and the shared waveform helper. The [READ_SCENE derivation](issue-170-local-scene-boundaries.md) establishes the wire filename expansion stage reused here.

The catalogued [PC Micro package](../../../reference/rip-tools.md) supplies `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses below are RVAs at image base `0x10000000`. The 40-byte physical slot 109 at `0x81928` identifies opcode `1w`, handler `0xd24e`, widths 1 and 3, and flags `0x000f`.

The low flag bits select active Base Math. The [shared reserve path](issue-263-resource-query-text-boundaries.md) validates three digits or a leading period, supplies zero, and enters text collection. The preceding flags field also accepts the one-period zero spelling. No fields can be omitted at the delimiter. The literal filename can start with a digit after both fields have completed.

## Handler order

| RVA | Operation |
| --- | --- |
| `0xd25a` | Read the first numeric field. |
| `0xd26b–0xd275` | Check for absent or empty literal text. The failure path at `0xd315` reports a local error. |
| `0xd27b–0xd29d` | Compare the unexpanded tail with `$OFF$` from `0x7ab08`, case-insensitively. Stop through `0x37e41` and clear the current sound marker on a match. |
| `0xd2bf–0xd2d7` | Copy text and call `0x4b0e4` with output bound 4095, feature mask `0x83` and angle preprocessing enabled, the same parameters as wire `1R`. |
| `0xd2df–0xd2f1` | Pass only flag bit 0 as the repeat argument to `0x37e56`. |
| `0x37e8c–0x37ebb` | Prepare the resource name, use the default `.wav` literal from `0x803f8` when the extension is absent, then request lookup. |
| `0x37edd–0x37f08` | On the eligible play path, stop the prior sound and call WinMM `PlaySoundA` with `0x00020001` for one shot or `0x00020009` for repeat. |

The Windows flags are asynchronous filename playback, with `SND_LOOP` added for repeat. Microsoft documents that looped playback continues until another playback or stop request. The shared HCL waveform wrapper calls `0x37e56` with repeat zero. [Microsoft PlaySound loop semantics](https://learn.microsoft.com/en-us/windows/win32/multimedia/using-playsound-to-loop-sounds).

## Staged captures

The original report feeds `!|`, body and CR separately. No handler is selected before CR. The results hold with Base Math 36 and 64. The handler observations come from the report's controlled services.

| Body | Parsed flags, reserve and text | Handler request |
| --- | --- | --- |
| `1w0000tone.wav` | (0, 0), `tone.wav` | Expand and play once. |
| `1w1.$&NAME$` | (1, 0), `$&NAME$` | Expand and repeat the resolved file. |
| `1w.77Ztone` | (0, 0), `tone` | Expand, default to `.wav` and play once. |
| `1w.0.name.wav` | No dispatch | — |
| `1w..$oFf$` | (0, 0), `$oFf$` | Stop before expansion. |
| `1w2000tone` | (2, 0), `tone` | Play once. |
| `1w3000tone` | (3, 0), `tone` | Repeat. |
| `1w1000` | (1, 0), empty tail | Local error. |
| `1w1` | No dispatch | — |

An expanded or unquoted `$OFF$` can only reach the ordinary filename helper because the stop comparison already ran on the supplied text. That result takes the default extension if needed.

```sh
sha256sum "$dll"
run/riptel-inspect "$dll" --slot 109
objdump -d --start-address=0x1000d24e --stop-address=0x1000d32f "$dll"
objdump -d --start-address=0x10037e56 --stop-address=0x10037f20 "$dll"
```
