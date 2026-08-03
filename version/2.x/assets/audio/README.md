# Audio — RIPscrip 2.x

Empty — TODO

RIPscrip 2.2+ plays background WAV audio via
[RIP_PLAY_AUDIO](../../../3.x/ripscrip/11-level-1-commands.md#rip_play_audio) and the
[`$)FILE.WAV$` playback prefix](../../../3.x/ripscrip/15-local-playback-popup-lists.md).
RIPtel had **no dedicated audio directory**: per the recovered help text, local
playback searches the host directory first, then `ICONS\` — audio files lived
alongside the icons. The install creates only `Files`, `ICONS`, and `FONTS`
directories (per `INSTALL.LOG`), and enables audio via `AUDIO=TRUE` in
`RIPscrip.ini`.

This directory mirrors that reality: it exists to hold any 2.x-era audio
files recovered in the future, and is empty because none were distributed.
See the [research notes](../../../3.x/research/riptel-help-extraction.md) for the
audio-related strings recovered from the RIPtel binaries.
