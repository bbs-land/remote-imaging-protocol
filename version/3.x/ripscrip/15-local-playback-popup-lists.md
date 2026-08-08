# Local File Playback & Pop-Up Lists

[◀ Prev: Host Commands](14-host-commands.md) · [Contents](README.md) · [Next: Templates & Conditionals ▶](16-templates-and-conditionals.md)

_Reconstructed edition — see [Contents](README.md) for the evidence legend._

Local playback directives are the client/server heart of the Host Command Language: they instruct the terminal to process a file _already on the user's disk_ instead of pulling content over the modem. The White Paper singles them out as the way "to simulate menus on an online service locally, thus making it so that you don't have to transmit hardly anything over the modem." RIPscrip 3.0 defines four playback prefixes — one per media type — plus the pop-up pick list, an inline menu whose result feeds back into the host command.

_Evidence: WP; HLP._

All four playback forms share one shape: `$`, a one-character prefix, the filename, `$`. RIPtel searches for the named file in the current host's directory first, then in `ICONS\`, then along the user-configured Search Path (which allowed CD-ROM-resident resources for games). _Evidence: HLP._

| Directive     | Media                             | Example           |
| ------------- | --------------------------------- | ----------------- |
| `$>FILE.RIP$` | RIPscrip scene (or any text file) | `$>NEWSPAPR.RIP$` |
| `$)FILE.WAV$` | Digitized audio                   | `$)TRAIN.WAV$`    |
| `$<FILE.BMP$` | Bitmap image                      | `$<BRICK.BMP$`    |
| `$(FILE.JPG$` | JPEG photograph                   | `$(ASTRO.JPG$`    |

The bracket mnemonic is deliberate, and inherited from 2.x: angle brackets (`>`, `<`) mark the _low-level_ playback primitives (raw RIPscrip, raw bitmap), while the parentheses (`)`, `(`) mark the _higher-level_ media objects (audio, compressed photos). _Evidence: 2.00a4._

## Local Scene Playback — `$>FILE.RIP$`

_Evidence: 2.00a4; HLP; SyncTERM (ripper.c:7967–7970); corpus (DEMO-01.COL, TELCMDS.FN)._

`$>FILENAME$` replays a local file as if it had arrived from the host — with one critical difference: **the data is echoed locally only, never sent to the host**. If the file is a RIPscrip scene, its graphics draw and any mouse fields it defines are created, which is how pop-up dialogs and entire menu systems run without host traffic. The file need not be RIPscrip at all — any text file, ANSI screen, or (in 3.0) flowed article text is legal; see the [Column Text System](17-column-text-system.md) for the non-RIPscrip case.

SyncTERM implements the form as a special case checked _before_ text-variable lookup: a leading `>` routes the remainder to `rip_play_scene` (`ripper.c:7967–7970`).

The RIPtel corpus leans on this directive harder than on any other: 34 distinct `$>file$` forms chain the whole demo together — `$>WIPE00.FN$` transition "subroutines", `$>newspapr.rip$` navigation, and the `$<<RETURN>>$` computed-return idiom described under [Templates & Conditionals](16-templates-and-conditionals.md). Filenames are case-insensitive in practice (`$>newspapr.rip$` and `$>NEWSPAPR.RIP$` both appear).

## Local Audio Playback — `$)FILE.WAV$`

_Evidence: 2.00a4; HLP._

`$)FILENAME$` plays a local digitized audio file — Microsoft WAVE format as far as the wire is concerned; a terminal using another native format must translate. The macro form is the HCL twin of the `1w` (RIP_PLAY_AUDIO) command, and the same playback model applies:

- Audio plays **in the background, asynchronously** — graphics keep drawing and host data keeps arriving while sound plays.
- There is a **single playback channel**: starting a new file terminates whatever is currently playing.
- The reserved filename `$OFF$` (i.e. `1w` with filename `$OFF$`) silences the current sound.
- If the file does not exist locally, the directive does nothing; hosts deliver audio files ahead of time via block transfer.

See [Color, Audio & Text](06-color-audio-text.md) for the audio format details. Note that RIPtel 3.1's install sets `AUDIO=TRUE` in `RIPSCRIP.INI` and its help advertises "background digitized audio," but the demo corpus itself ships no `.WAV` files and never issues an audio command — the demos are silent. _Evidence: corpus; HLP._

## Local Bitmap Playback — `$<FILE.BMP$`

_Evidence: 2.00a4; HLP._

`$<FILENAME.BMP$` displays a local bitmap through the current **image style** (`1i`, RIP_IMAGE_STYLE): the bitmap is placed and scaled per the recorded style, or fills the current viewport if no style is recorded. For universality the macro form always renders with the current screen palette in auto-dithering mode; RIPTEL.HLP is explicit that the full flexibility of `RIP_LoadBitmap` (palette modes, clipping options) is _not_ available from the macro form — use the wire command when you need it.

## Local JPEG Playback — `$(FILE.JPG$`

_Evidence: 2.00a4; HLP; corpus (N2_PHOTO.RIP)._

`$(FILENAME.JPG$` displays a compressed photographic image the same way — through the current image style, or full-viewport when none is recorded. JPEG is the only supported photo format in the 3.0 driver. A companion action variable positions the _next_ image without redefining a style on the wire:

```text
$IMGSTYLE(cur,x0,y0,x1,y1)$
```

where `cur` selects the current screen port. The demo corpus displays its sixteen JPEGs through the wire commands (`1i` + `1p`) rather than the macro form, but the macro and `$IMGSTYLE$` are documented in RIPtel's help as the host-command route to the same machinery.

## Pop-Up Pick Lists

_Evidence: 2.00a4; HLP; SyncTERM (ripper.c:11356–11635, 11936–11948)._

Anywhere a text variable is legal in a host command, a **pop-up pick list** may appear: an inline menu whose chosen entry is spliced into the host command in place of the list itself. The 3.0 syntax, with every extension in place:

```text
((*x,y:Question::response@Description,response@Description,...))
```

| Element | Meaning |
| --- | --- |
| `((` … `))` | List delimiters |
| `*` | Response required — the user cannot ESC out of the list |
| `x,y:` | Pop-up position in world coordinates; omit either value (keep the comma) to center on that axis _(new in 3.0 — HLP)_ |
| `Question` | Prompt text; if omitted, the default is `Choose one of the following:` |
| `::` | Separates the question from the entries |
| `response` | The text spliced into the host command when chosen |
| `@Description` | Optional display text shown in the list instead of the response |
| `~h~` or `_h_` | Hotkey markup around a character of the description; the first marked character is the active hotkey |

Examples, from simplest to fullest:

```text
((Send Email to?::Sysop,Cosysop,Joe))
((*Send Mail to?::Sysop@Head Honcho,Cosysop,Joe))
((*10,50:Order which fruit?::A@_A_pples,O@_O_ranges,C@_C_herries))
```

Rules carried forward from 2.x, all confirmed by RIPtel's help:

- Up to **64 entries** per list; a list too tall for the screen must scroll.
- The characters `!`, `\` and `|` in a _response_ must be escaped with a backslash (`\!`, `\\`, `\|`). Hotkey markers (`~`, `_`) cannot be escaped — put hotkeys in descriptions, not responses.
- If the user presses ESC on a non-required list, nothing is inserted.

### Recursion into the HCL

_Evidence: SyncTERM (ripper.c:11936–11948, 11890–11896)._

The chosen response is not merely transmitted — it is **re-fed through the host-command interpreter**. A pick-list entry may therefore itself contain text variables, control characters, playback directives, or another pick list; SyncTERM implements exactly this (`do_popup`'s result recursing through `handle_command_str`), with recursion depth capped at 64. An entry prefixed `*` inside the option list forces that answer without user interaction — the mechanism SyncTERM uses to auto-answer forced choices. A menu that plays local scenes, a scene that pops another menu: pick lists plus `$>` playback form a small but genuinely programmable UI layer, years before such things were commonplace.

---

[◀ Prev: Host Commands](14-host-commands.md) · [Contents](README.md) · [Next: Templates & Conditionals ▶](16-templates-and-conditionals.md)
