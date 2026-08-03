# Design Goals & Graphical Primitives

[◀ Prev: Language Structure](03-language-structure.md) · [Contents](README.md) · [Next: User Interface & Display ▶](05-interface-and-display.md)

## 3.3 Design Goals

RIPscrip 3.0 differs substantially from its earlier 1.0 version in a number of areas. The older editions of RIPscrip were designed around a fixed environment of 16 color displays at a resolution of 640x350. RIPscrip 3.0 is not like that - it is designed around a hardware independent display environment, capable of being implemented on a broad variety of display environments and operating systems.

To accomplish this task, the language became considerably more complex. Not only did the number of basic commands in the language grow, but the structure and relationships between the commands grew as well. What resulted is a highly-defined, broad-reaching graphics presentation technology that can be used in just about any display environment.

The two fundamental precepts of RIPscrip 3.0 are the concepts of resolution independence, and also color independence. These are the two areas where graphical environments vary greatly, and consequently, where the largest source of compatibility problems arises. This is why so much time and effort was put into RIPscrip 3.0 in these areas, making sure that the language encompassed as many environments as possible. This was to ensure its future growth and compatibility with new equipment and environments.

## 3.3.1 Resolution Independence - The World Coordinate System

One of the most important improvements in RIPscrip, is the concept of a resolution independent drawing environment. Without a resolution independent environment, a drawing on one computer monitor, might appear tiny when viewed on another monitor running at a higher resolution. RIPscrip 3.0 overcomes this with the addition of a [world coordinate system](../../2.x/ripscrip/05-coordinates-and-math.md).

The world coordinate system in RIPscrip can be considered a "virtual coordinate system", superimposed on the actual drawing surface of the monitor. For example, consider that your monitor is running at 640x480 resolution right now. If you drew graphics at this resolution, then took that graphical information over to another computer running say, at 1280x1024, your image would only appear on the upper-left portion of the monitor. With the diverse kinds of video hardware in the computer industry, resolution independence is critically important to preserving the look and feel of information from one computer environment to another.

The world coordinate system adds a new coordinate system on-top of your video display, logically mapping physical pixels on the screen to new coordinates in the world coordinate system. For example, if you were running at 640x480 resolution, and your world coordinate system were defined to a size of 6400x4800, then every pixel on your screen would correspond to 10 logical pixels wide and tall. When working with a high-scale coordinate system like this, viewing graphics on another video device is simply a matter of "re-mapping" the logical coordinates in the world coordinate system, to the actual physical device pixel coordinates used by your computer monitor.

RIPscrip 3.0 allows you to define a world coordinate system to a maximum size of 32,767 by 32,767, giving you plenty of room to accommodate video hardware over the next several decades.

## 3.3.2 Color-Palette Independence

The second area of hardware independence is color palette independence. This is an important issue, because the number of colors available on one video device can vary widely from those of another device. It is not uncommon to have people in the same office using video displays running at 16 colors, while other people in the same office are using 256 color display modes. With new video hardware, video devices supporting 32,767 colors, 65,636 colors, 16.7 million colors, and even higher are becoming affordable, and quite commonplace. With this in mind, proper thought needed to be put into a color system for RIPscrip which accommodates the needs of yesterday's technology, and tomorrow's.

RIPscrip handles color palette independence in two ways - it provides for an indexed color palette system, where you have a table of 256 colors all referred to by a particular index number. The RIPscrip engine itself is in charge of mapping these individual color numbers to actual hardware colors available in the video device. The developer has the ability to change these colors at will, forcing the RIPscrip engine to re-map the colors at the low-level, to the proper display colors. Using color palette indexes allows for fast, efficient specification of color values with only a single number. Color palettes can also be used to change all occurrences of a particular color on the screen to another actual color in the blink of an eye, allowing for color palette animation techniques commonly used in video games, and other special effects.

The second area of color palette independence used in RIPscrip, is the concept of RGB encoding. With RGB encoding, you can specify color values as a set of Red, Green and Blue values. These values are combined by the RIPscrip display engine, and the closest available color in the video hardware is used to display the graphics. In this mode, the RIPscrip system is entirely in charge of determining which color is the closest available color, and should be used for the requested RGB value. Leaving RIPscrip in charge of mapping the colors, makes your graphics flexible and capable of being displayed on just about any kind of video hardware.

## 3.4 Broad Range of Graphical Primitives

RIPscrip possesses a comprehensive set of built-in graphical primitive commands. With them, you can visually display just about anything you can imagine. With nearly 100 predefined commands, and over 150 separate "text variable" commands (see below), RIPscrip gives you a serious arsenal to design your online service.

## 3.4.1 Font Systems

Probably one of the most important aspects of RIPscrip graphics, is the availability of a flexible font system. RIPscrip actually gives you access to two different kinds of font systems!

The original font system, based around a vector font technology, is called the System Font. There are currently 11 separate fonts in this system of RIPscrip. New fonts may not be added to this system, as it is an older font engine, with moderate appearance. These fonts are however, very fast and are quite suitable for many environments. These fonts are designed around line-based vectors, which when assembled together, create a complete font glyph, or character. Microsoft Windows and many other GUI environments provided this kind of vector font technology in their early stages of development. RIPscrip is no different in this regard. The System Font engine of RIPscrip was part of the 1.x edition of RIPscrip.

With version 2.0 of RIPscrip, high-tech outline font technology was introduced as the second font engine in the graphical technology. This outline font technology, similar to TrueType or Adobe Postscript style fonts, gives you the ability to place fonts on the screen in a variety of orientations, at any point size you want. You have full control over bold, italic, underline and strikethrough attributes, and can even control the rotation and orientation of characters on even 90° increments. Unlike the System Font engine of RIPscrip, the Outline Font engine (otherwise known as the [extended font system](../../2.x/ripscrip/08-level-0-commands-a-f.md#rip_extended_font_style)), allows you to incorporate new fonts into RIPscrip to enhance your documents.

## 3.4.2 Vector Graphics Primitives

RIPscrip offers the developer a wide variety of vector graphics primitives to achieve just about any kind of geometric drawings you could ever want. Similar to many high-end drawing packages only available for many hundreds of dollars, RIPscrip gives you the same drawing flexibility as you would come to expect from a quality presentation graphics system.

Of the many drawing primitives in RIPscrip, you'll find commands to perform the following drawing functions:

| Command             | Borders only | Filled-only | Filled/Border |
|---------------------|--------------|-------------|---------------|
| Lines               | Yes          | No          | No            |
| Points and pixels   | Yes          | No          | No            |
| Graphical text      | Yes          | No          | No            |
| Bezier curves       | Yes          | No          | No            |
| Circular arcs       | Yes          | No          | No            |
| Oval arcs           | Yes          | No          | No            |
| Poly-lines          | Yes          | No          | No            |
| Circles             | Yes          | Yes         | Yes           |
| Ovals               | Yes          | Yes         | Yes           |
| Rectangles          | Yes          | Yes         | Yes           |
| Rounded rectangles  | Yes          | Yes         | Yes           |
| Poly-Bezier curves  | Yes          | Yes         | Yes           |
| Polygons            | Yes          | Yes         | Yes           |
| Circular pie slices | No           | Yes         | Yes           |
| Oval pie slices     | No           | Yes         | Yes           |

## 3.4.3 Color Palettes

RIPscrip offers a number of commands to alter the existing color palette. RIPscrip has two different methods of working with color information: using indexed color palettes, or with direct hard-coded RGB values. Using color palettes, allows you to quickly switch between one color set and another, giving you the ability to perform basic color palette animation techniques, or to change the color sets of a particular screen on-the-fly.

RGB encoding modes allow you to encode your graphical data in raw RGB values, making the RIPscrip engine do the hard part of locating the proper color that most closely resembles the desired color, and using that one to draw with.

RIPscrip provides the best of both worlds when it comes to color. You can use the flexibility of a color palette system, or the more robust, platform independent method of using RGB color values for your artwork.

## 3.4.4 Fill Patterns

RIPscrip provides a set of twelve predefined fill-patterns which you may use with filled-in objects like filled-rectangles, circles, or the like. The basic standard patterns give you a place to start from, but it doesn't stop there. You may define your own 8x8 fill patterns for use in your content. The choice of pattern selection is entirely up to you, and you can even use two-tone colored patterns to provide dithering effects, or halftoning patterns for your information. Using fill patterns gives your artwork "texture", and gives things a more appealing look.

Of the available predefined patterns, you have numerous cross-hatch patterns, dotted patterns and others to work with. If a pattern isn't in the predefined list, you can define your own.

Fill patterns only apply to filled-in objects like circles, rectangles, polygons, and ovals, etc. They do not apply for non-closed objects like lines, pixels, text or poly-lines.

## 3.4.5 Line Dash-Styles

It doesn't stop there. You also have both predefined and custom line dash styles to work with. If you want to change the appearance of a line from a solid line, to a dotted line, you can. There are five separately defined dash styles, ranging from a solid line, to dotted and dashed lines. If that's not enough, you can define your own dash styles using a 16-pixel dash style definition. This gives you the ability to define any kind of dash style you could want.

You also have control over the thickness of lines. The thickness of lines governs the weight of a line, or how heavy it appears. This allows you to define pencil-thin, or very fat lines. Line thickness is used for any outline object (e.g., lines, ovals, circles, pie-slices, Bezier curves, polygons, etc.).

Dash sequences may be used for any "straight-line" RIPscrip oriented commands. This covers lines, poly-lines and polygons because they're all based on straight-line segments. It also covers rectangles. It does not cover circles, ovals, rounded rectangles or the Bezier-curve family of commands because these are based on curves. These commands only utilize the thickness of lines.

## 3.4.6 Raster-Operators

One final aspect of the graphics primitive area of RIPscrip, is the use of raster operators. A raster operator, or raster-op, governs how a graphical object is drawn onto the screen. Typically, the standard Raster-Op mode of COPY is used. When COPY mode is used, graphics are drawn onto the screen "as-is", overwriting whatever graphics were underneath them. Other raster-ops exist which permit you to change this drawing mode. This lets you "merge" a graphical command with the information already on the screen. For example, one of the raster-ops is OR mode. This allows the RIPscrip engine to effectively merge the operation of drawing a circle, with the graphics already on the screen underneath the circle, giving a rather translucent effect. There are five separate Raster-Ops available which provide a number of Boolean drawing modes (i.e., COPY, AND, OR, NOT, and XOR).

---

[◀ Prev: Language Structure](03-language-structure.md) · [Contents](README.md) · [Next: User Interface & Display ▶](05-interface-and-display.md)
