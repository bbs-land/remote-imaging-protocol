# Color, Audio & Text Windows

[◀ Prev: Numbers, Coordinates & Math](05-coordinates-and-math.md) · [Contents](README.md) · [Next: Protocol Definition & Syntax ▶](07-protocol-definition.md)

## Color Palettes and Hardware - Color Translation

*Added in RIPscrip v2.A0; revised in v2.A4.*

With versions of RIPscrip prior to v2.0, the color palette was limited to 16 colors maximum out of a total of 64 colors in the Master Color Palette. This was due to the hardware origins of the language in its initial releases. With the popularity of higher color video sub-systems, RIPscrip needed to grow to accomodate these advancing trends. In addition, it needed to be open-ended enough to accomodate yet unknown hardware environments that might provide for even higher color resolution.

The 1.xx RIPscrip specification provided for 16 colors out of a palette of 64 colors. This 64 color palette corresponded to 2-bits of Red, Green and Blue information each, giving you four levels of color saturation in each of the three categories. Unlike typical RGB implementations the color palette entry numbers are not sequentially ordered based on the R, G and B components. The bit-layouts of the color values as follows:

```text
                       Primary    Secondary
                     ├─────────┤ ├─────────┤
            ╔═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗
            ║▒▒▒│▒▒▒║   │   │   ║   │   │   ║
            ║▒▒▒│▒▒▒║ R │ G │ B ║ r │ g │ b ║
            ║▒▒▒│▒▒▒║   │   │   ║   │   │   ║
            ╚═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝
              80  40  20  10  8   4   2   1  (hex)
              7   6   5   4   3   2   1   0  (position)
```

Notice that each of the R, G and B sections are broken up into two separate bit sections in the color palette entry number. Also, the bits are reversed when they are encoded. Let's look at four separate colors of Red, Green and Blue to see how the bit patterns correspond to the actual Palette entries:

| Level | RED xxRGBrgb | RED Value | GREEN xxRGBrgb | GREEN Value | BLUE xxRGBrgb | BLUE Value |
|-------|--------------|-----------|----------------|-------------|---------------|------------|
| 00    | 00000000     | 0         | 00000000       | 0           | 00000000      | 0          |
| 01    | 00100000     | 32        | 00010000       | 16          | 00001000      | 8          |
| 10    | 00000100     | 4         | 00000010       | 2           | 00000001      | 1          |
| 11    | 00100100     | 36        | 00010010       | 18          | 00001001      | 9          |

The base 16 color palette in RIPscrip 1.54 and earlier was modified by the [RIP_SET_PALETTE](10-level-0-commands-s-w.md#rip_set_palette) and [RIP_ONE_PALETTE](09-level-0-commands-g-r.md#rip_one_palette) commands. These commands are limited to a maximum of 16 colors and only accomodate 2-bits for each color component. These commands are here on out referred to as the "desktop palette" alteration commands and are only designed to make "simple" changes to the lowest 16 colors with a limited set of color saturation in each of the red, green and blue components.

Under RIPscrip 2.0 and later, we deal with the concept of a 256-entry color lookup table (otherwise known as the drawing palette). This palette of RGB colors is used to cross-reference a particular color number to some arbitrary RGB color combination much like the earlier 1.xx palette counterparts, but with some more important differences. The newer set palette commands allow you to specify the number of bits for each red, green and blue component, and in addition, the internal layout of each RGB value isn't encoded in a seemingly haphazard fashion. A color value is a raw binary number without any frills or complications. If the bit value were 00100100 then the actual color saturation level would be 36, not encoded in a strange bit swapped fashion as in the older color palette commands. It is prophesized that the older color palette commands will not be used much (if at all) in the newer 2.0 environments due to their complexity - about the only way that the older commands will be used is via software packages that only output colors in the older syntax.

Since RIPscrip 1.54 and earlier didn't know about the 256-entry color lookup table, we have introduced two new color palette commands:

- [RIP_ONE_DRAWING_PALETTE](09-level-0-commands-g-r.md#rip_one_drawing_palette) — Set one color in the drawing palette to an arbitrary RGB color combination.
- [RIP_SET_DRAWING_PALETTE](10-level-0-commands-s-w.md#rip_set_drawing_palette) — Set a block of colors in the drawing palette to some set of arbitrary RGB color values.

These commands supercede the older 1.54 commands and are much more flexible. They not only allow you to access the entire 256-entry color lookup table, but they allow you to specify how many bits of precision of RGB component data are used in the RGB encoded data.

No matter what hardware configuration your actual RIPscrip software is running under, there is some low-level inherent bit-precision for each RGB color component (whether it be 2 bits each, 4, 6, 8 or more bits per color component). With this in mind, the actual "set palette" commands need to internally convert any RGB color values to the "native" values of the target operating system's graphics environment. The actual discussion of this goes beyond the scope of this document, however the actual conversion should be rather trivial.

Supporting the older 1.54 "set palette" commands that modify the lower 16 color of the drawing palette are also just as trivial. All you need to do is map the 4 levels of RGB data to target values in the native graphics environment and you're done. For example, if your target environment has 6 bits of precision (0-63), then your 2-bit to 6-bit conversions might be something like this (a simple bit shift operation):

| 2-bit | 6-bit |
|-------|-------|
| 0     | 0     |
| 1     | 16    |
| 2     | 32    |
| 3     | 48    |

This is an overly simplistic example, and if you notice that the 6-bit range goes from 0-63, not 0-48! With this in mind, a simple bit shift operation (eg, shifting left 4 bits or multiplying by 16) in this example doesn't get the job done just right - a full intensity BLUE component (value 3 in 2-bit components) maps to a value of 48 in 6-bit components which is not a full-intensity blue). What you need to do is take the maximum target value (63 in this example), and divide it by the maximum value of the source range (3). This yields a value of 21. So, our range would evaluate to the following based on this new algorithm:

| 2-bit | 6-bit (x21) |
|-------|-------------|
| 0     | 0           |
| 1     | 21          |
| 2     | 42          |
| 3     | 63          |

This gives a perfectly proportioned range where each division in the target color range is evenly spaced and covers the base (black) and the top (purest color) perfectly.

If you considered another example of 8-bit target colors, this rule still applies - 255 / 3 = 85, which is our division as in the following 2 to 8 bit table:

| 2-bit | 8-bit (x85) |
|-------|-------------|
| 0     | 0           |
| 1     | 85          |
| 2     | 170         |
| 3     | 255         |

This same algorithm should work relatively well for any color component mapping operation from any bit precision to another where the source bit precision is less than the target's precision. Some "fractional" component operations may need to be performed to compensate for non-integral divisions, but this shouldn't pose any problems in a real-world scenario. In fact, this same algorithm can equally be used by the 2.0 extended "set palette" commands to map the RGB color component values to the actual "bit precision" of the target graphics environment's video hardware.

If the source's bit precision is greater than the target's, then a simple right bit shift operation would accomplish the job without yielding any "uneven" divisions in the target environment's color components (eg, if you had 3-bit color values and a 2-bit target environment, shifting the values right by one bit (dividing by 2) would give you the perfect results. Here's the translation table so that you can see this alternative algorithm at work:

| 3-bit | 2-bit (÷2) |
|-------|------------|
| 0     | 0          |
| 1     | 0          |
| 2     | 1          |
| 3     | 1          |
| 4     | 2          |
| 5     | 2          |
| 6     | 3          |
| 7     | 3          |

### The Drawing Palette

*Added in RIPscrip v2.A4.*

Now that we've discussed how to translate an RGB color from one environment to another, now let's discuss the larger picture as far as RIPscrip is concerned: The Drawing Palette.

The Drawing Palette in RIPscrip is a table of 256 entries, where each entry is a set of RGB color component values. This table is a mechansim to "map" color codes (from 0-255) to arbitrary RGB color values.

It may be distinctly possible that the RIPscrip software is running in an environment that allows for an actual "color palette". If this is the case then this color lookup table will match the color palette of the video hardware entry-for-entry. Color 5 in the lookup table would correspond with color 5 in the video system's hardware color palette. This works for environments where the color palette is "completely redefinable". What this means is that the RIPscrip software can completely control the entire video hardware color palette.

It is also very possible that the RIPscrip software could be running on a video system that cannot support 256 simultaneous colors, and only has a color palette of smaller size (typically 16 colors). In this case, the lowest 16 colors (the desktop palette) would typically map one-for-one with the actual hardware color palette, although the programmer might opt to choose a more intelligent approach of color selection based on color distribution. This rather complex issue goes beyond the scope of this discussion and it is recommended that for simplicity, the lowest 16 colors of the drawing palette be mapped to the target palette.. This again, assumes that the RIPscrip software has complete control over the video color palette system and can change any or all of the entries at its discretion.

There are two other important situations that can occur:

1. The video hardware doesn't have a color palette (eg, its a 24-bit color system and doesn't have an actual palette), or
2. The operating system is running in an environment with only a certain number of colors (eg, 256, 16, etc), but you cannot redefine any or them, or you can only redefine some of them (eg, Microsoft Windows under many configurations, etc).

Situation #1 is the easiest to work with. You don't have an actual color palette to modify, so any color values (from 0-255) simply lookup the RGB data in the color lookup table and use that raw color component data to set the color of the current drawing operation. In this manner, the lookup table is "pretending" to be a hardware video palette. This is a situation where having the lookup table is very important. It isn't extremely important when you have a one-to-one mapping of lookup colors to actual hardware color palette colors because the lookup table does the exact same thing as the hardware color palette. But, in modes where you have no palette, this information becomes vital. In practice, this local lookup table aids in color translation of image files (JPEG, GIF, BMP, etc), and also aids in color translation in other situations.

Situation #2, where you don't have complete control over the target video palette is much more complicated. For example, under Microsoft Windows, you typically cannot redefine the colors black or white. This gives you only a sub-set of the actual palette to work with. Also, other applications in the environment may have "locked" several entries in the color palette for their own exclusive use. This can reduce your target color palette even further. How you handle which colors in the lookup table to "activate" (if any), and in what order you activate them is entirely up the discretion of the programmer(s) of the RIPscrip package dealing with this kind of environment. The topic of algorithms for this situation goes completely beyond the scope of this RIPscrip specification. Suffice it to say that the programmer would have to determine some way of mapping the lookup table to a suitable set of colors in the actual destination color environment. If he uses strange methods of dithering (like Windows does), or other mechanisms to achieve the desired color to the user, so be it.

### Palette Mapping and Direct RGB Mode

*Added in RIPscrip v2.A4.*

Under RIPscrip 2.0, you have two methods of specifying color values. You have already seen how colors can be specified by a color index number into the color lookup table (from 0-255). What RGB color that actually maps to is based on the contents of that entry in the actual color lookup table. This is called "Palette mapping mode". In palette mapping mode, most locations in RIPscrip commands that allow for color values take a color number (from 0-255) which references some RGB value in the color lookup table.

We also allow for another mode in RIPscrip, called "Direct RGB Encoding". This mode allows you to specify actual "raw RGB" values instead of color lookup table indices. When a direct RGB value is found, it is decoded (ie, breaking up the red, green and blue components) and is used for the actual color. What happens at this particular moment depends on the environment that the RIPscrip software is running under.

If the software is running in an environment with a color palette, where the hardware video palette is set to some combination of the colors used in the color lookup table, then the RGB encoded data is matched to the "closest" color in the actual target hardware video palette and that color number is actually used. Under many situations, this color value will be the same one as used in the actual color lookup table (for situations where the software is running on a 256-color palette environment with complete control over the hardware palette). Under modes that are more limited, but where a hardware palette is still being used, the same algorithm can still be applied - but there won't be a one-to-one mapping of color lookup table values to the hardware palette. This is unavoidable and in the case of direct RGB encoding, irrelevant.

If the software is running in an environment like 24-bit mode, then life couldn't be simpler. Simply take the color value and use its RGB components (possibly with some color component remapping), and and activate that color in the environment.

Direct RGB encoding mode, unlike palette mapping mode, allows you to specify the bit-precision of RGB color codes used subsequently in RIPscrip code (where direct RGB colors are permitted - not all RIPscrip commands permit direct RGB encoded color codes). This allows you to have color values with the same flexibility as "set color palette" commands, where you can specify arbitrary precisions of RGB data to potentially accomodate high-end color systems like 24-bit, 16-bit, 15-bit, etc).

By default, RIPscrip operates in palette mapping mode, Whenever a reset operation occurs (any type of reset), palette mapping mode is once again re-enabled. In order to go to RGB encoding mode, you must explicitly specify a [RIP_COLOR_MODE](10-level-0-commands-s-w.md#rip_set_color_mode) command, or a `$COLORMODE()$` text variable to activate direct RGB encoding mode. When you specify direct RGB encoding mode, you indicate how many bits of precisions to allow for each red, green and blue component. This value is used for all three components. For example, if you specify 8 bits of precision for each component, then combined they will amount to 24 bits of RGB data (eg, 24-bit mode). This is unlike some unusual environments on the IBM-PC where 16-bit modes (5 red, 6 green and 5 blue bits) are allowed on some VGA environments.

When a direct RGB encoded color value is received, it is decoded uniformly based on the N bits of precision. Blue is always in the lowest N bits of the RGB value, then green, then red in the highest bits. Let's say we have an 8-bit RGB encoded value of 04140D hexadecimal. Given this hexadecimal number, we would have the bit pattern of "000001000001010000001100". Breaking this up into 8-bit groups, we would have the following in binary, hexadecimal and decimal:

|             | RED      | GREEN    | BLUE     |
|-------------|----------|----------|----------|
| binary      | 00000100 | 00010100 | 00001100 |
| hexadecimal | 04h      | 14h      | 0Dh      |
| decimal     | 4        | 20       | 12       |

Given that we are working with 8-bit components (from 0-255), this would yield a very dark shade of cyan (approximately).

### Default RGB Values of Color Lookup Table

*Added in RIPscrip v2.A1; revised in v2.A4.*

When a reset command is executed, all unprotected color palette data table entries are reset to some suitable default values. The exact RGB values used in this default color palette table are listed below. In this table, we list the values with six bits of precision for red, green and blue. This is a carefully calculated color palette designed to be of optimal use both in 16 color modes, and in 256 color modes.

*Each row lists the RGB values for six consecutive palette entries, beginning at the entry number in the Num column.*

| Num | R | G | B | R | G | B | R | G | B | R | G | B | R | G | B | R | G | B |
|-----|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 0   | 0 | 0 | 0 | 0 | 0 | 42 | 0 | 42 | 0 | 0 | 42 | 42 | 42 | 0 | 0 | 42 | 0 | 42 |
| 6   | 42 | 21 | 0 | 42 | 42 | 42 | 21 | 21 | 21 | 21 | 21 | 63 | 21 | 63 | 21 | 21 | 63 | 63 |
| 12  | 63 | 21 | 21 | 63 | 21 | 63 | 63 | 63 | 21 | 63 | 63 | 63 | 0 | 0 | 0 | 4 | 4 | 4 |
| 18  | 8 | 8 | 8 | 13 | 13 | 13 | 17 | 17 | 17 | 21 | 21 | 21 | 25 | 25 | 25 | 29 | 29 | 29 |
| 24  | 34 | 34 | 34 | 38 | 38 | 38 | 42 | 42 | 42 | 46 | 46 | 46 | 50 | 50 | 50 | 55 | 55 | 55 |
| 30  | 59 | 59 | 59 | 63 | 63 | 63 | 0 | 0 | 0 | 0 | 0 | 21 | 0 | 0 | 42 | 0 | 0 | 63 |
| 36  | 0 | 9 | 0 | 0 | 9 | 21 | 0 | 9 | 42 | 0 | 9 | 63 | 0 | 18 | 0 | 0 | 18 | 21 |
| 42  | 0 | 18 | 42 | 0 | 18 | 63 | 0 | 27 | 0 | 0 | 27 | 21 | 0 | 27 | 42 | 0 | 27 | 63 |
| 48  | 0 | 36 | 0 | 0 | 36 | 21 | 0 | 36 | 42 | 0 | 36 | 63 | 0 | 45 | 0 | 0 | 45 | 21 |
| 54  | 0 | 45 | 42 | 0 | 45 | 63 | 0 | 54 | 0 | 0 | 54 | 21 | 0 | 54 | 42 | 0 | 54 | 63 |
| 60  | 0 | 63 | 0 | 0 | 63 | 21 | 0 | 63 | 42 | 0 | 63 | 63 | 10 | 0 | 0 | 10 | 0 | 21 |
| 66  | 10 | 0 | 42 | 10 | 0 | 63 | 10 | 9 | 0 | 10 | 9 | 21 | 10 | 9 | 42 | 10 | 9 | 63 |
| 72  | 10 | 18 | 0 | 10 | 18 | 21 | 10 | 18 | 42 | 10 | 18 | 63 | 10 | 27 | 0 | 10 | 27 | 21 |
| 78  | 10 | 27 | 42 | 10 | 27 | 63 | 10 | 36 | 0 | 10 | 36 | 21 | 10 | 36 | 42 | 10 | 36 | 63 |
| 84  | 10 | 45 | 0 | 10 | 45 | 21 | 10 | 45 | 42 | 10 | 45 | 63 | 10 | 54 | 0 | 10 | 54 | 21 |
| 90  | 10 | 54 | 42 | 10 | 54 | 63 | 10 | 63 | 0 | 10 | 63 | 21 | 10 | 63 | 42 | 10 | 63 | 63 |
| 96  | 21 | 0 | 0 | 21 | 0 | 21 | 21 | 0 | 42 | 21 | 0 | 63 | 21 | 9 | 0 | 21 | 9 | 21 |
| 102 | 21 | 9 | 42 | 21 | 9 | 63 | 21 | 18 | 0 | 21 | 18 | 21 | 21 | 18 | 42 | 21 | 18 | 63 |
| 108 | 21 | 27 | 0 | 21 | 27 | 21 | 21 | 27 | 42 | 21 | 27 | 63 | 21 | 36 | 0 | 21 | 36 | 21 |
| 114 | 21 | 36 | 42 | 21 | 36 | 63 | 21 | 45 | 0 | 21 | 45 | 21 | 21 | 45 | 42 | 21 | 45 | 63 |
| 120 | 21 | 54 | 0 | 21 | 54 | 21 | 21 | 54 | 42 | 21 | 54 | 63 | 21 | 63 | 0 | 21 | 63 | 21 |
| 126 | 21 | 63 | 42 | 21 | 63 | 63 | 31 | 0 | 0 | 31 | 0 | 21 | 31 | 0 | 42 | 31 | 0 | 63 |
| 132 | 31 | 9 | 0 | 31 | 9 | 21 | 31 | 9 | 42 | 31 | 9 | 63 | 31 | 18 | 0 | 31 | 18 | 21 |
| 138 | 31 | 18 | 42 | 31 | 18 | 63 | 31 | 27 | 0 | 31 | 27 | 21 | 31 | 27 | 42 | 31 | 27 | 63 |
| 144 | 31 | 36 | 0 | 31 | 36 | 21 | 31 | 36 | 42 | 31 | 36 | 63 | 31 | 45 | 0 | 31 | 45 | 21 |
| 150 | 31 | 45 | 42 | 31 | 45 | 63 | 31 | 54 | 0 | 31 | 54 | 21 | 31 | 54 | 42 | 31 | 54 | 63 |
| 156 | 31 | 63 | 0 | 31 | 63 | 21 | 31 | 63 | 42 | 31 | 63 | 63 | 42 | 0 | 0 | 42 | 0 | 21 |
| 162 | 42 | 0 | 42 | 42 | 0 | 63 | 42 | 9 | 0 | 42 | 9 | 21 | 42 | 9 | 42 | 42 | 9 | 63 |
| 168 | 42 | 18 | 0 | 42 | 18 | 21 | 42 | 18 | 42 | 42 | 18 | 63 | 42 | 27 | 0 | 42 | 27 | 21 |
| 174 | 42 | 27 | 42 | 42 | 27 | 63 | 42 | 36 | 0 | 42 | 36 | 21 | 42 | 36 | 42 | 42 | 36 | 63 |
| 180 | 42 | 45 | 0 | 42 | 45 | 21 | 42 | 45 | 42 | 42 | 45 | 63 | 42 | 54 | 0 | 42 | 54 | 21 |
| 186 | 42 | 54 | 42 | 42 | 54 | 63 | 42 | 63 | 0 | 42 | 63 | 21 | 42 | 63 | 42 | 42 | 63 | 63 |
| 192 | 52 | 0 | 0 | 52 | 0 | 21 | 52 | 0 | 42 | 52 | 0 | 63 | 52 | 9 | 0 | 52 | 9 | 21 |
| 198 | 52 | 9 | 42 | 52 | 9 | 63 | 52 | 18 | 0 | 52 | 18 | 21 | 52 | 18 | 42 | 52 | 18 | 63 |
| 204 | 52 | 27 | 0 | 52 | 27 | 21 | 52 | 27 | 42 | 52 | 27 | 63 | 52 | 36 | 0 | 52 | 36 | 21 |
| 210 | 52 | 36 | 42 | 52 | 36 | 63 | 52 | 45 | 0 | 52 | 45 | 21 | 52 | 45 | 42 | 52 | 45 | 63 |
| 216 | 52 | 54 | 0 | 52 | 54 | 21 | 52 | 54 | 42 | 52 | 54 | 63 | 52 | 63 | 0 | 52 | 63 | 21 |
| 222 | 52 | 63 | 42 | 52 | 63 | 63 | 63 | 0 | 0 | 63 | 0 | 21 | 63 | 0 | 42 | 63 | 0 | 63 |
| 228 | 63 | 9 | 0 | 63 | 9 | 21 | 63 | 9 | 42 | 63 | 9 | 63 | 63 | 18 | 0 | 63 | 18 | 21 |
| 234 | 63 | 18 | 42 | 63 | 18 | 63 | 63 | 27 | 0 | 63 | 27 | 21 | 63 | 27 | 42 | 63 | 27 | 63 |
| 240 | 63 | 36 | 0 | 63 | 36 | 21 | 63 | 36 | 42 | 63 | 36 | 63 | 63 | 45 | 0 | 63 | 45 | 21 |
| 246 | 63 | 45 | 42 | 63 | 45 | 63 | 63 | 54 | 0 | 63 | 54 | 21 | 63 | 54 | 42 | 63 | 54 | 63 |
| 252 | 63 | 63 | 0 | 63 | 63 | 21 | 63 | 63 | 42 | 63 | 63 | 63 | ░░ | ░░ | ░░ | ░░ | ░░ | ░░ |

The first 16 entries in the above color table correspond to the default color palette used in v1.xx of RIPscrip and corresponds directly to the color palette used for 16 color ANSI text. The next 16 colors in the color table above is a 16 level gray-scale used for gray-scale image output and to provide a basic spectrum of grays.

The remaining 224 entries is a "uniform distribution" of RGB colors. The actual organization of the colors is mathematically based such that an arbitrary RGB color value can be mapped to a color value in the default palette with a simple calculation (instead of searching through the entire palette for the closest entry). This block tries to cover as many colors in the color spectrum as possible.

There are four levels of Blue, eight levels of green and seven levels of red. Each block is organized with Blue the color that changes every entry, green the next most changing, and red the least frequently changing. If you have a RED value from 0-6, a GREEN value from 0-7 and a BLUE value from 0-3, you can easily calculate the correct palette index entry with the following equation:

```text
INDEX = 32 + BLUE + GREEN*4 + RED*32
```

Or in C with bit-shifting, you could do it like this:

```text
INDEX = 32 + BLUE + GREEN<<2 + RED<<5
```

The main reason for choosing this color palette was to facilitate the ability to display many color images onto the screen at the same time in 256 color mode without having to alter the color palette for each image. The color representation may not be 100% exact, but it would be close enough with dithering to accurately represent the original image.

The exact determination of this color palette was very carefully chosen. The lowest 16 colors, which happen to default to the basic colors of ANSI color codes (universally used throughout the computer world seemingly), are a good sub-set of colors for basic 16 color operations. The next 16 colors are defined as a dedicated gray-scale color palete (only of use in 256 color modes), and gives a very good breakdown of the gray-scale monotone color palette. The remaining 224 entries are broken down mathematically (as you've seen above) with 7 levels of red, 8 levels of green and 4 levels of blue. This seemingly odd configuration of color distrubution was very scientifcally chosen. *(v2.A4)*

Extensive experimental research has determined that the human eye is more susceptible to particular "hues" of light than others. Specifcally, the human eye is most susceptible to subtle changes in the shade of green than any other color. Next comes red, then finally blue at the very lowest end of the perceptivity scale. If you recall, red is at the lower end of the light spectrum (eg, near infrared), and blue is at the upper end of the spectrum (near ultra-violet). Green is in the middle. This tells us that our eye is most responsive to light in the middle-to-lower bands of the color spectrum. After scientific analysis, our eye was determined to be responsive to the following levels of each color band (comparitively based on an 8 level scale, 1 being lowest): *(v2.A4)*

```text
8 - green
7 - red
4 - blue
```

Taking this information into account, we constructed a default palette which reflected the best distribution of color that the human eye can perceive. By no means is it perfectly suited for all environments, but it is a "best case" situation for most images and environments. As you can undoubtedly guess, a photograph with a lot of blues and violets would be significantly degraded in quality in this color environment, but many typical images where a broad range of colors is used works very well, and when dithering is used, the situation improves even more. *(v2.A4)*

## Audio (Sound & Music) Formats

*Added in RIPscrip v2.A4.*

RIPscrip 2.0 permits the support of digitized audio to be played in the background whild graphics and other operations are being performed. This includes playing sounds while graphical commands are being received from the host (without interruption).

The format of audio data used by RIPscrip 2.0 is formally defined as straight audio data in the Microsoft WAVE file format. When dealing with host systems and downloading digitized audio data, or being told to play back audio data, RIPscrip will assume that you are working with WAVE files. For example, you might be told to playback a file named SOUND.WAV. If you are not internally supporting the actual WAVE format, but some other format, then you need to translate the name of the file to the correct name in your environment (if necessary) and perform the playback.

When dealing with the [RIP_FILE_QUERY](11-level-1-commands.md#rip_file_query) command, you need to be careful if you're not dealing with true WAVE files. The RIP_FILE_QUERY command is used to "detect" if a particular file is present on the target system and if so, if it is the right date/time and file size as the file on the host. The file on the host will be in WAVE format so if you're translating the files as they're received, you need to translate the RIP_FILE_QUERY information returned to the host so that it thinks that the file you have on your local hard disk is actually a WAVE file (ie, translate the file size to a WAVE equivalent). If the sizes and, or the time/date don't match, the host could download a new file to you (via the [RIP_ENTER_BLOCK_MODE](13-level-3-9-commands.md#rip_enter_block_mode) command) thinking that your's is out of date.

If you receive a WAVE file via the RIP_ENTER_BLOCK_MODE command, or by some other meanss, and you're dealing with anther digitized audio file format, it is up to you to translate that WAVE file to your destination format. Consequently, if you are requested to send a WAVE file to the host system, and the host system is requesting a .WAV file, then you must translate your file to WAVE format before sending it.

A Microsoft WAVE file is actually a normal "Pulse Code Modulation" digitized audio file stored with a bit of header information to identify whether it is stereo, 16-bit, 8-bit, etc, and other pieces of critical information for the decoding of the data. See the "Microsoft Windows Programmer's Reference Guide" for more specific information on the internal format of WAVE files.

## Text Windows and Terminal Emulation Protocols

*This section is an unfinished placeholder in the original ALPHA 4 specification; it appears verbatim below.*

```text
[ BEGIN REWORD ]

Discuss text windows and terminal emulations here

[ END REWORD ]
```

## Viewports and Text Windows - Overlapping Issues

*Added in RIPscrip v2.A1; revised in v2.A3.*

Since there are multiple text windows and [ports](02-drawing-ports.md) (with their viewports) allowed in the RIPscrip specification, some elaboration needs to be made on what happens if any overlap each other. Very simple, they do what they have always done - draw text, or draw graphics. For example, if a viewport overlaps a text window and you draw some graphics (say a circle) over the top of some text in a text window, and the text window subsequently scrolls, all or part of the circle could scroll with it! Now, of course, from the viewport's standpoint, the graphics are no longer what you originally sent to the viewport, but that doesn't matter - you don't preserve any of the commands you used to create the graphics - you simply draw the graphics and that's it. So with this in mind, even if multiple viewports overlap each other it doesn't matter because the final result is what's on the screen - doing things in one viewport might do some overlapping graphics in another viewport but that is alright.

If a text window overlaps another text window, the same thing happens. To a RIPscrip terminal, text is simply just a piece of graphical information that is being placed on the screen in a formatted fashion. If two text windows overlap and text is placed on an area that overlap the other window, then you will be drawing graphics (essentially) on top of another text window. The same thing as the circle example above would happen if that text window scrolled.

## Miscellaneous Notes/Information

Later in this document, references are made to Mouse Fields and Mouse Buttons. Specifically, it is noted that up to 128 of these types of commands may exist simultaneously on-screen. This means that you can have 128 mouse fields, 128 mouse buttons, or any combination of the above, but combined, their total number cannot exceed 128. *(v1.54)*

When the user clicks his/her mouse on the screen, all mouse regions (whether mouse fields or mouse buttons) are scanned from most recent to the least recent. This means that if a mouse region is received that overlaps another (previously received) mouse region, the newest one would be selected if the user clicked in that region. *(v1.54, revised v2.A3)*

If you are implementing a client terminal to support RIPscrip graphics and you do not intend on supporting 100% of all pre-defined text variables, you SHOULD at least recognize them and do nothing. This makes it so that if a particular text variable is used to make a sound (for example), then if you don't support it, you just ignore it instead of popping up a dialog box on your user's screen asking them to enter data for the variable `$MUSIC$` for example! *(v1.54)*

> **NOTE:** Many of the text variables like `$PCB$` and other key variables are very important to GUI design and should be implemented! If you are going to omit any of the text variable, do not omit any of the active text variables - these are CRITICAL to implementing a full GUI system using RIPscrip 2.0!!! *(v2.A1)*

---

[◀ Prev: Numbers, Coordinates & Math](05-coordinates-and-math.md) · [Contents](README.md) · [Next: Protocol Definition & Syntax ▶](07-protocol-definition.md)
