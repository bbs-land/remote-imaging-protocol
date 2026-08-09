# Audio — RIPscrip 2.x

| File | Size (bytes) | Format | Notes |
| --- | --: | --- | --- |
| `RIPTERM.WAV` | 75,034 | RIFF WAVE, 8-bit mono PCM, 11,127 Hz | RIPterm's own sound file, shipped in the program directory of every 2.x release — byte-identical (md5) across the recovered 2.0, 2.20.01, and 2.30 installs (2.0 copy dated 1995-02-01) |

Digitized `.WAV` audio was a headline feature of **RIPterm Professional 2.0** (January 1995) — "A new feature in RIPterm is the support for digitized sound, in the form of .WAV files" (RIPTERM.DOC §4.3), with an Audio Setup screen supporting 19 sound boards (Sound Blaster through Gravis UltraSound) via HMI "Sound Operating System" drivers (`HMIDRV.386`/`HMIDET.386`, "Digitized Audio code copyright (c) 1993–1995 SOS/Human Machine Interfaces, Inc."). `RIPTERM.WAV` is the only WAV in the distribution — the client's own sound, not BBS content; host-supplied sounds were expected in `ICONS\` ("Sound Files — .WAV … must also be in the ICONS directory", RIPTERM.DOC §2.1.4).

On the wire, RIPscrip 2.2+ plays background WAV audio via [RIP_PLAY_AUDIO](../../../3.x/ripscrip/11-level-1-commands.md#rip_play_audio) (specified since 2.0 ALPHA 3) and the [`$)FILE.WAV$` playback prefix](../../../3.x/ripscrip/15-local-playback-popup-lists.md). RIPtel likewise had **no dedicated audio directory**: per the recovered help text, local playback searches the host directory first, then `ICONS\` — audio files lived alongside the icons, and the RIPtel install enables audio via `AUDIO=TRUE` in `RIPscrip.ini`. See the [research notes](../../../3.x/research/riptel-help-extraction.md) for the audio-related strings recovered from the RIPtel binaries.
