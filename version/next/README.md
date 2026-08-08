# Future

Near futre, may want to formalize soem additional image and audio formats as an unofficial 3.50 or 4.x enhancement. Additional file formats for icons/images and audio. It would also be useful to add/improve support for propper/common font formats as well as UTF-8 rendering support.

## Fonts and UTF-8

Should add a command to switch/swap between **CP-437** DOS text and **UTF-8** Unicade.

- For non-rip terminal region rendering, a default, fixed-width font should be set and rendered/scopped per cell, with wide-characters and emoji mapped to two cells. These should be rendered over-sized and scaled with anti-aliasing to best match against the cell size including room for the hanging characters.
- For text rendering, the addition of **.WOFF**, **.WOFF2**, **.TTF** and **.OTF** typefaces should be added as well as offset definitions for rendering sizes within the canvas. This will require additional research and work.
- It would probably be beneficial to include a base livrary of open fonts to include, or possibly utilize an online library to download as needed, such as Google Fonts, or the Github releases for said fonts.

## Internal Storage

Similar to the 1.5x to 2.x migration of icons to a new format, it may be beneficial to internally migrate and default to different formats for audio and image files. Requests for the old extension, transparently replaced with stored files in the format below, or searching through supported formats in a hierarchal search.

- **ICONS** - It may be beneficial to internally use **.PNG** format and translate all icons on load to said format, with \*_zipfli_ cpmpression at 2-5 iterations in order to improve the file sizes and storage. Transparently replacing a request for an icon/bmp file with the same named .png -- For .BMP files that are using a reduced palette, the .png should match the palette to further save on compressed size, and the palette should be treated as it is with the .bmp file. More details to be expanded on.
- **AUDIO** - Similarly to the icons, it may be beneficial to do similar using **.MP2** in place of **.WAV** files for audio. Maybe defaulting to the typical **44.1khz**, **128kbps** that is most typical. This will similarly lead to significant storage savings.

## Additional Formats

These changes can be a path to a future 3.5x or 4.x enhancement to the specification which should add a few additional formats to be supported.

Itermediately, implementations in default or current playback modes can replace the default representations, with a search of the given file with the extensions in the format order listed below.

### Icons/Images

- **.png** - Icons - new default automatically replaces .ICO/.BMP usage.
- **.apng** - Icons - additional support for animaged .png images
- **.webp** - Icons - addition of webp format support
- **.gif** - Icons, re-add .gif including animation support.

### Audio

- **.mp3** - Audio, add .mp3 support.
- **.ogg** - Audio, Ogg Vorbis
- **.opus** - Audio, Ogg Opus
- **.oga** - Audio, Ogg + FLAC
