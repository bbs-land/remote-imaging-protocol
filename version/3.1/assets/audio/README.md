# Audio — RIPscrip 3.x (RIPtel 3.1)

Empty — RIPtel 3.1 shipped no audio files.

RIPscrip 3.0 plays background WAV audio via [RIP_PLAY_AUDIO](../../ripscrip/4.5-audio-files.md#rip_play_audio) and the [`$)FILE.WAV$` playback prefix](../../ripscrip/5.3-local-playback.md). RIPtel had **no dedicated audio directory**: per the recovered help text, local playback searches the host directory first, then `ICONS\` — audio files lived alongside the icons. The install creates only `Files`, `ICONS`, and `FONTS` directories (per `INSTALL.LOG`), and enables audio via `AUDIO=TRUE` in `RIPscrip.ini`.

This directory mirrors that reality: it exists to hold any 3.x-era audio files recovered in the future, and is empty because none were distributed. See the [research notes](../../research/riptel-help-extraction.md) for the audio-related strings recovered from the RIPtel binaries.
