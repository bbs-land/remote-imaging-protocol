# Local RIPscrip File Playback

[◀ Prev: Text Variables](15-text-variables.md) · [Contents](README.md) · [Next: Pop-Up Lists ▶](17-popup-lists.md)

You can re-play a .RIP file that you have locally on your hard disk from anyplace that allows [text variables](15-text-variables.md). The format of the variable is somewhat different than user variables, or pre-defined text variables. After the initial dollar sign (`$`), enter the greater-than symbol (`>`) followed by the filename (with or without the .RIP extension), then ending in another dollar sign (`$`). Several examples of this are as follows:

```text
$>MYFILE.RIP$
$>FILE1$
$>FILE1.RIP$$>FILE2.RIP$$>FILE3$
```

Note in the last example, a file extension other than .RIP was used. You are not limited to playing back local .RIP files. In fact, you can play-back any file you want. You could load any simple text file, ANSI picture image, or other such thing. When loaded, the data is not sent to the host; it is strictly echoed on your local screen. If the file is a .RIP file, it will replay any graphics that were in the file and if any Mouse Regions are defined, it will create those fields for you as well, thus allowing you to pop-up dialog screens or other such things that are not built-in to RIPterm normally.

Each "local RIP playback" variable you enter will search for the .RIP file in the current host's icon directory. If it cannot find the file in that directory, it will check the `ICONS\` directory.

---

[◀ Prev: Text Variables](15-text-variables.md) · [Contents](README.md) · [Next: Pop-Up Lists ▶](17-popup-lists.md)
