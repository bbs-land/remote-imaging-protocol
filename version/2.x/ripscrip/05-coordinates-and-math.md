# Numbers, Coordinates & Math

[◀ Prev: Data Backup Areas](04-data-backup-areas.md) · [Contents](README.md) · [Next: Color, Audio & Text Windows ▶](06-color-audio-text.md)

## Numerical Parameters - Formats and Base-Math Variations

_Added in RIPscrip v2.A0._

Throughout the RIPscrip specification, numeric parameters are used extensively to indicate where to draw things, how to draw things and various other pieces of information. For compactness, we do not use numbers that are normal decimal numbers (base-10). By default, RIPscrip uses base-36 numbers (Hexa-Tri-Decimal or more fondly, MegaNums) which use the digits 0-9 followed by A-Z. This was the original numbering system used in the 1.xx specification.

In the 2.xx specification, the base math can be altered to something other than base-36! With v2.xx, we are introducing a new numbering scheme that is base-64, otherwise known as Quadra-Hexa-Decimal, or more fondly known as UltraNums. UltraNums use the following digits:

`0-9  A-Z  a-z  #  &`

With this numbering scheme, you can squeeze larger numbers into an even smaller space than MegaNums! Here is a table showing how big of a number can be contained with X number of digits:

| Total Digits (X) | Decimal | Hexa-Decimal | MegaNums      | UltraNums      |
| ---------------- | ------- | ------------ | ------------- | -------------- |
| 1                | 9       | 15           | 35            | 63             |
| 2                | 99      | 255          | 1,295         | 4,095          |
| 3                | 999     | 4,095        | 46,655        | 262,143        |
| 4                | 9,999   | 65,535       | 1,679,615     | 16,777,215     |
| 5                | 99,999  | 1,048,575    | 60,466,175    | 1,073,741,823  |
| 6                | 999,999 | 16,777,215   | 2,176,782,335 | 68,719,476,735 |

A [RIPscrip Header command](10-level-0-commands-s-w.md#rip_set_base_math) can be used to specify the global base math used throughout the command sets. By altering the base math, you can make a single numeric parameter accept larger values without expanding the parameter in the specification. A couple of the RIPscrip commands utilize specific base math values (eg, UltraNums, etc) under certain circumstances. Any commands that are exceptions like this will be clearly documented as being exceptions to the rule.

> **NOTE:** 6 digit UltraNums cannot be easily represented under most personal computer compilers (beyond 32 bits). Therefore, UltraNums beyond 5 digits probably should not be used in a real world situation. _(v2.A3)_

The following is a basic table of Decimal, MegaNum and UltraNum values:

| Decimal | MegaNum | UltraNum | Decimal | MegaNum | UltraNum |
| ------- | ------- | -------- | ------- | ------- | -------- |
| 00      | 0       | 0        | 32      | 0W      | W        |
| 01      | 01      | 1        | 33      | 0X      | X        |
| 02      | 02      | 2        | 34      | 0Y      | Y        |
| 03      | 03      | 3        | 35      | 0Z      | Z        |
| 04      | 04      | 4        | 36      | 10      | a        |
| 05      | 05      | 5        | 37      | 11      | b        |
| 06      | 06      | 6        | 38      | 12      | c        |
| 07      | 07      | 7        | 39      | 13      | d        |
| 08      | 08      | 8        | 40      | 14      | e        |
| 09      | 09      | 9        | 41      | 15      | f        |
| 10      | 0A      | A        | 42      | 16      | g        |
| 11      | 0B      | B        | 43      | 17      | h        |
| 12      | 0C      | C        | 44      | 18      | i        |
| 13      | 0D      | D        | 45      | 19      | j        |
| 14      | 0E      | E        | 46      | 1A      | k        |
| 15      | 0F      | F        | 47      | 1B      | l        |
| 16      | 0G      | G        | 48      | 1C      | m        |
| 17      | 0H      | H        | 49      | 1D      | n        |
| 18      | 0I      | I        | 50      | 1E      | o        |
| 19      | 0J      | J        | 51      | 1F      | p        |
| 20      | 0K      | K        | 52      | 1G      | q        |
| 21      | 0L      | L        | 53      | 1H      | r        |
| 22      | 0M      | M        | 54      | 1I      | s        |
| 23      | 0N      | N        | 55      | 1J      | t        |
| 24      | 0O      | O        | 56      | 1K      | u        |
| 25      | 0P      | P        | 57      | 1L      | v        |
| 26      | 0Q      | Q        | 58      | 1M      | w        |
| 27      | 0R      | R        | 59      | 1N      | x        |
| 28      | 0S      | S        | 60      | 1O      | y        |
| 29      | 0T      | T        | 61      | 1P      | z        |
| 30      | 0U      | U        | 62      | 1Q      | #        |
| 31      | 0V      | V        | 63      | 1R      | &        |

The following C code fragment provides you with a way of converting decimal numbers to an UltraNum format:

```c
char ultra_num_table[64] = {
     '0', '1', '2', '3', '4', '5', '6', '7',
     '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
     'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
     'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
     'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
     'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
     'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
     'u', 'v', 'w', 'x', 'y', 'z', '#', '&'
};

char *Dec2Ultra(long value)
{
     static char buf[8];
     int    part, pos=0;

     setmem(buf, 0, 8);

     while (value >= 64L) {
          part       = value & 0x003Fl;
          value      = value >> 6;
          buf[pos++] = ultra_num_table[part];
     }

     if (value) {
          buf[pos++] = ultra_num_table[value];
     }

     strrev(buf);
     return(buf);
}
```

## World Coordinate Systems

_Added in RIPscrip v2.A0._

Starting with the v2.0 specification of RIPscrip, we have introduced a "world coordinate system". This means that you can alter the base coordinate system used to address the video's physical device coordinate system. This is the first step in achieving device independence. Each coordinate system is called a "Coordinate Frame". There are actually two distinct levels of coordinate systems. From the lowest (least abstract) to the highest level (the drawing board) you have the following coordinate systems:

1. **DEVICE COORDINATE FRAME** - the resolution of the actual display device. This is not determined as part of the RIPscrip language itself, it is actually determined by the terminal - based upon whatever type of video hardware is present (and what mode the user has chosen).

2. **[WORLD COORDINATE FRAME](10-level-0-commands-s-w.md#rip_set_world_frame)** - This is the master coordinate system. This is the global set of coordinates. Ideally, it should be higher in dimensions than the Device Frame so that you do not lose resolution when "mapping" X/Y coordinates from the world coordinate system to the device coordinate system. If the dimensions of the World Frame are the same as the Device Frame then the mapping operation between World and Device Frames are identical and can be skipped entirely since an X/Y coordinate in the World Frame directly corresponds to a pixel location in the Device Frame.

   The maximum dimensions of the world coordinate frame is 65535x65535. _(v2.A4)_

Since versions of the RIPscrip specification prior to v2.0 were based on a 640x350 resolution without any World Frame, a suitable set of defaults need to be assumed for the World Frame and the Logical Frame. The World Frame would be set to a dimension of 640 across and 350 high.

## The Mathematics of Graphics and Coordinates

_Added in RIPscrip v2.A0._

Mathematics, unlike graphics, is abstract in nature. Graphics on the other hand, is very discrete in nature. A pixel in graphics is the smallest displayable element on a graphics screen (Picture Element). The corresponding concept in mathematics is a single point. A pixel has a discrete size and shape in graphics whereas in mathematics, a point is infinitely small and occupies no physical space in the cartesian coordinate system in which it exists.

In graphics we use the concept of coordinates and points to specify where a pixel is located. If graphics were like mathematics where a pixel was infinitely small, a graphics monitor wouldn't show anything other than black because a pixel would be infinitely small surrounded by blackness. But this is not the case. Graphics hardware has to have pixels defined as discrete areas of the screen - addressable areas of the screen that can be set to a particular color. This is how we see things on the screen, as individual pixels of data used together to represent some kind of image.

Just like the point in mathematics, a line is infinitely thin, but has length. A line is composed of an infinite number of points between the beginning and ending locations of the line.

Relating these two distinctly different concepts to each other though doesn't always lend itself to a simple situation to understand. On the surface, a pixel's coordinate location appears to be the same thing as it is in mathematics. But, when you get into deeper issues of graphics theory this isn't truly the case because of the difference in sizes of a point compared to a pixel. In math, points don't have to be on even integer boundaries - they can be fractional. A point can be at (2.53,1.295), whereas in graphics, a pixel cannot be at a fractional location - there's no such thing as a part of a pixel!

A graphics screen is designed so that every pixel location on the screen has a unique location specified (in human terms) as an X/Y coordinate pair. This is called the Cartesian Coordinate System. It is by far one of the easiest ways of representing a coordinate in a two-dimensional world like a monitor. If you zoom in closely on a monitor and look at the layout of pixels and their relationships to coordinates, you might see something like this:

```text
                        (X DIMENSION)

                           1│1│1│1│1│1│1│1│1│1│2│2│2│2│
      │0│1│2│3│4│5│6│7│8│9│0│1│2│3│4│5│6│7│8│9│0│1│2│3│
    ──╔═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╗
     0║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
     1║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
Y    2║ │ │ │ │ │ │ │ │ │ │ │█│█│ │ │ │ │ │ │ │ │ │ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
D    3║ │ │ │ │ │ │ │ │ │ │ │█│█│ │ │ │ │ │ │ │ │ │ │ ║
I   ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
M    4║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
E   ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
N    5║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
S   ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
I    6║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
O   ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
N    7║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
     8║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
    ──╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝
```

If you look at the previous diagram, you can see that there is a grid of pixels that is 24 pixels wide (the X direction) and 10 pixels high (the Y direction). Each of the squares is a black pixel. The pixels contain a "#" are considered white pixels. There are four pixels that are ON in this diagram, at locations (11,2), (12,2), (11,3) and (12,3). This is how graphics hardware addresses graphics pixels, by specifying an X/Y coordinate location for a pixel to turn it on or off (or more generally, to set it to a particular color).

If you look closely at the locations of the coordinates in the previous diagram, the X/Y coordinates address the physical pixel cells themselves. Each pixel cell is separated by infinitely thin lines - the pixel cells' borders so to speak. Now, each pixel is physically adjacent to the ones next to it and there are no gaps between them. This is the way graphics hardware works.

Now that we've described the way graphics hardware addresses pixel locations, let's look at why it's not really the best way of describing graphics mathematically. For the hardware, this is the best way of handling things, but mathematically, it's not.

The world of graphics hardware has many different facets. One graphics display device can often times display graphics at different resolutions. A typical video card on the IBM-PC can display graphics at 320x200, 640x350, 640x400, 640x480, 800x600, 1024x768 and even as high as 1280x1024 and higher. That's quite a bit of difference in pixel grids (like the one shown earlier). Not only can the number of pixels change horizontally and vertically, but when the resolutions get higher, the pixels get smaller and harder to see. When graphics hardware can achieve a resolution of infinity by infinity, then we will have achieved perfect sight (like what we see around us). But obviously, this will probably never happen in our lifetimes (or our great granchildren's for that matter).

What would happen if you had a line drawn on a graphics screen from coordinates (0,0) to (639,349) at 640x350 resolution? You would have a nice little line on the screen that stretches from the upper-left corner to the lower-right corner. Now, if you drew the same line at 800x600 resolution? Your line would no longer go all the way to the bottom of the screen or to the far right of the screen. It would stop about 2/3'rds of the way down and over.

The problems start to come up with mathematics and graphical representations when we try to make something look the same at one resolution as it does in another (resolution independence). Sure you can translate a coordinate in one resolution to some other coordinate in a different resolution. All you need to know are the dimensions of each resolution and a bit of algebra. But pixels aren't the same thickness! The line described previously could still be drawn properly to the lower-right of the screen, but it would appear a lot thinner. You could try drawing two lines offset by one pixel location and you would come "close" to the original thickness of the line at 640x350 resolution, but it wouldn't be a perfect match. A perfect match is a rare thing when dealing with different resolutions.

Now, on to the real issue at hand here: translation of coordinates. We won't worry about the size of pixels, but will concentrate on getting the locations of points correct at different resolutions. Let's say you have a screen at 640x350 resolution with two filled rectangles drawn on it. The first rectangle is drawn from (2,1) to (4,3) and the second one is drawn from (5,1) to (7,3) like this:

```text
      │0│1│2│3│4│5│6│7│8│9│
    ──╔═╤═╤═╤═╤═╤═╤═╤═╤═╤═╗
     0║ │ │ │ │ │ │ │ │ │ ║     1st: (2,1) - (4,3)
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢     2nd: (5,1) - (7,3)
     1║ │ │█│█│█│░│░│░│ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
     2║ │ │█│█│█│░│░│░│ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
     3║ │ │█│█│█│░│░│░│ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
     4║ │ │ │ │ │ │ │ │ │ ║
    ──╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝
```

Now, you notice that both rectangles are right next to each other. There are no gaps between them. Now, if you tried to draw these same two rectangles on a graphics screen that was 1280x700 pixels in size, and tried to make it appear in the exact same location and size on the monitor, you would have to translate the corners of the rectangles to the new resolution and re-plot the rectangles. Now, 640x2 is 1280 and 350x2 is 700 so our new resolution is exactly twice as large in both the X and Y directions (this is RARELY the case!). So, in order to translate our original coordinates to this resolution we simply multiply the numbers by 2. Pretty simple. Our previous rectangles in their old untranslated state and the newly translated ones would be:

|             | Untranslated  | Translated      |
| ----------- | ------------- | --------------- |
| Rectangle 1 | (2,1) - (4,3) | (4,2) - (8,6)   |
| Rectangle 2 | (5,1) - (7,3) | (10,2) - (14,6) |

Now, let's plot these two rectangles on our new graphics screen at the newly translated coordinates:

```text
                 (X DIMENSION)

                          │1│1│1│1│1│1│1│
      │0│1│2│3│4│5│6│7│8│9│0│1│2│3│4│5│6│
    ──╔═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╗
     0║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
     1║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
Y    2║ │ │ │ │█│█│█│█│█│ │░│░│░│░│░│ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
D    3║ │ │ │ │█│█│█│█│█│ │░│░│░│░│░│ │ ║
I   ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
M    4║ │ │ │ │█│█│█│█│█│ │░│░│░│░│░│ │ ║
E   ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
N    5║ │ │ │ │█│█│█│█│█│ │░│░│░│░│░│ │ ║
S   ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
I    6║ │ │ │ │█│█│█│█│█│ │░│░│░│░│░│ │ ║
O   ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
N    7║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
    ──╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
     8║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
    ──╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝
```

What happened? We now have a black line in between the two rectangles. That's definitely not what we wanted to happen. Why did this happen? It's partly because of the nature of graphics hardware in the way they address pixels, and partly the way we humans think of pixels mathematically from a programming standpoint. What we need is a better mathematical representation of pixels and graphics coordinates so that when we translate coordinates these things don't happen.

As we mentioned earlier, the lines on the above diagram represent the borders around each pixel. Each of these lines, like their mathematical counterparts, are infinitely thin. The only thing in the above diagram that has any size (or area) physically are the pixels themselves. Since the boundary lines around the pixels are defining the area that the pixel will occupy, it makes sense that we should think of those lines as the actual coordinates. If we think of the areas in between the pixels as the actual coordinates, we can think of pixels as the areas in between coordinates that are filled in with color. So, if we said to draw a filled in rectangle like we did earlier, we would actually be saying "fill in this rectangle's interior". Just like coloring books as a child, you have to stay inside the lines (or at least we're supposed to). Think about pixels as spots between the lines that get filled in with color just like a coloring book - the only difference is, a pixel cannot go outside the lines (don't you wish you were a pixel when you were younger?).

Using this new way of thinking of graphics is not very difficult, it just seems a bit odd -especially if you've been working with graphics for awhile. To draw the previous two rectangles properly the coordinates for them would have to be changed somewhat - like this:

**(old method)**

|             | Untranslated  | Translated      |
| ----------- | ------------- | --------------- |
| Rectangle 1 | (2,1) - (4,3) | (4,2) - (8,6)   |
| Rectangle 2 | (5,1) - (7,3) | (10,2) - (14,6) |

**(new method)**

|             | UNTRANSLATED  | TRANSLATED      |
| ----------- | ------------- | --------------- |
| RECTANGLE 1 | (2,1) - (5,4) | (4,2) - (10,8)  |
| RECTANGLE 2 | (5,1) - (8,4) | (10,2) - (16,8) |

Notice that the untranslated rectangles' coordinates are almost the same. In fact, the upper-left coordinates haven't changed at all. The only coordinates that have changed are the lower-right coordinates, and only by one pixel location. Now, if we draw these two rectangles using our new mathematical model for graphics, we would get the following (in both resolution examples:

```text
                                               1 1 1 1 1 1 1 1
                           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
                          0╔═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╗
                    1      ║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
0 1 2 3 4 5 6 7 8 9 0     1╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
0╔═╪═╪═╪═╪═╪═╪═╪═╪═╪═╗      ║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
 ║ │ │ │ │ │ │ │ │ │ ║     2╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
1╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢      ║ │ │ │ │█│█│█│█│█│█│░│░│░│░│░│ │ ║
 ║ │ │█│█│█│░│░│░│ │ ║     3╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
2╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢      ║ │ │ │ │█│█│█│█│█│█│░│░│░│░│░│ │ ║
 ║ │ │█│█│█│░│░│░│ │ ║     4╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
3╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢      ║ │ │ │ │█│█│█│█│█│█│░│░│░│░│░│ │ ║
 ║ │ │█│█│█│░│░│░│ │ ║     5╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
4╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢      ║ │ │ │ │█│█│█│█│█│█│░│░│░│░│░│ │ ║
 ║ │ │ │ │ │ │ │ │ │ ║     6╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
5╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝      ║ │ │ │ │█│█│█│█│█│█│░│░│░│░│░│ │ ║
                          7╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
                           ║ │ │ │ │█│█│█│█│█│█│░│░│░│░│░│ │ ║
                          8╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
                           ║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
                          9╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝
```

If you look closely at the two graphs above you may notice a subtle difference in the numbering of the coordinates. The numbers are all there, but instead of labeling the cells themselves with coordinates, we have labeled the lines in between as the coordinates. When we say to draw a rectangle from (2,1) to (5,4) we are saying to draw a filled in area "between" coordinate lines 2 and 5 in the X direction and 1 and 4 in the Y direction. If you look closely at the upper left diagram, you will notice that is exactly what we have done. Now look closely at the upper right diagram. There are no gaps between the rectangles now. Also if you look at the translated coordinates, the right edge of the first rectangle is at X coordinate 10, and the left edge of the second rectangle is also at X coordinate 10. But they don't overlap each other. That's the part about this new way of thinking about things that gives people the hardest time.

So far so good. Let's look at another example - one which draws the same two rectangles but this time, let's not fill them in. The diagram below shows the two at both example resolutions:

```text
                                               1 1 1 1 1 1 1 1
                           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
                          0╔═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╗
                    1      ║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
0 1 2 3 4 5 6 7 8 9 0     1╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
0╔═╪═╪═╪═╪═╪═╪═╪═╪═╪═╗      ║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
 ║ │ │ │ │ │ │ │ │ │ ║     2╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
1╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢      ║ │ │ │ │█│█│█│█│█│█│░│░│░│░│░│ │ ║
 ║ │ │█│█│█│░│░│░│ │ ║     3╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
2╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢      ║ │ │ │ │█│ │ │ │ │ │░│ │ │ │░│ │ ║
 ║ │ │█│ │ │░│ │░│ │ ║     2╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
3╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢      ║ │ │ │ │█│ │ │ │ │ │░│ │ │ │░│ │ ║
 ║ │ │█│█│█│░│░│░│ │ ║     5╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
4╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢      ║ │ │ │ │█│ │ │ │ │ │░│ │ │ │░│ │ ║
 ║ │ │ │ │ │ │ │ │ │ ║     6╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
5╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝      ║ │ │ │ │█│ │ │ │ │ │░│ │ │ │░│ │ ║
                          7╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
                           ║ │ │ │ │█│█│█│█│█│█│░│░│░│░│░│ │ ║
                          8╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
                           ║ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ ║
                          9╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝
```

Not what you thought would happen, was it? This example illustrates a key point in our mathematical model of graphics: that there is a difference between filled objects and outlined objects. Filled objects fill everything inside an enclosed area whereas an outline (one or more lines) uses pixels to approximate an infinitely thin line. When lines and points are plotted on the video device, they are "rounded up" to the next highest pixel location. So, a point at (4,2) would not be on the intersecting lines at (4,2), it would be a pixel activated between 4 and 5 on the X axis and 2 and 3 on the Y axis. Look at the upper right diagram to convince yourself this is the case. Filled-in areas on the other hand, do not round up to the next highest pixel locations. They always fill in an area in-between the boundary lines. This means that in a filled-in rectangle, the right and bottom edges are "off by one" so to speak.

To better illustrate these two different situations, we will draw a filled-in rectangle and a non-filled rectangle both using the same dimensions at the same resolution, then we will superimpose them on top of each other to better show the differences;

```text
  0 1 2 3 4 5 6 7 8 9    0 1 2 3 4 5 6 7 8 9    0 1 2 3 4 5 6 7 8 9
 0╔═╪═╪═╪═╪═╪═╪═╪═╪═╗   0╔═╪═╪═╪═╪═╪═╪═╪═╪═╗   0╔═╪═╪═╪═╪═╪═╪═╪═╪═╗
  ║ │ │ │ │ │ │ │ │ ║    ║ │ │ │ │ │ │ │ │ ║    ║ │ │ │ │ │ │ │ │ ║
 1╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   1╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   1╫─┼─┼─┼─┼─┼─┼─┼─┼─╢
  ║ │░│░│░│░│░│░│ │ ║    ║ │█│█│█│█│█│█│█│ ║    ║ │▒│▒│▒│▒│▒│▒│█│ ║
 2╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   2╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   2╫─┼─┼─┼─┼─┼─┼─┼─┼─╢
  ║ │░│░│░│░│░│░│ │ ║    ║ │█│ │ │ │ │ │█│ ║    ║ │▒│░│░│░│░│░│█│ ║
 3╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   3╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   3╫─┼─┼─┼─┼─┼─┼─┼─┼─╢
  ║ │░│░│░│░│░│░│ │ ║    ║ │█│ │ │ │ │ │█│ ║    ║ │▒│░│░│░│░│░│█│ ║
 4╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   4╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   4╫─┼─┼─┼─┼─┼─┼─┼─┼─╢
  ║ │░│░│░│░│░│░│ │ ║    ║ │█│ │ │ │ │ │█│ ║    ║ │▒│░│░│░│░│░│█│ ║
 5╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   5╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   5╫─┼─┼─┼─┼─┼─┼─┼─┼─╢
  ║ │ │ │ │ │ │ │ │ ║    ║ │█│█│█│█│█│█│█│ ║    ║ │█│█│█│█│█│█│█│ ║
 6╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   6╫─┼─┼─┼─┼─┼─┼─┼─┼─╢   6╫─┼─┼─┼─┼─┼─┼─┼─┼─╢
  ║ │ │ │ │ │ │ │ │ ║    ║ │ │ │ │ │ │ │ │ ║    ║ │ │ │ │ │ │ │ │ ║
 7╚═╧═╧═╧═╧═╧═╧═╧═╧═╝   7╚═╧═╧═╧═╧═╧═╧═╧═╧═╝   7╚═╧═╧═╧═╧═╧═╧═╧═╧═╝
    Filled rectangle      Unfilled rectangle      Both rectangles
     (1,1) - (7,5)          (1,1) - (7,5)          (1,1) - (7,5)
```

Notice that the two rectangles are not the exact same size. Both the left and top edges are at the exact same locations, but in the case of the unfilled rectangle, the right and bottom edges are one pixel over and down. This shows that lines and points are rounded up to the next highest pixel location (the first pixel after the coordinate), but filled areas fill between the boundary lines. This seemingly odd way of doing things becomes extremely important when you deal with one image at different resolutions - as you saw with the case of translating two filled-in rectangles earlier from one resolution to another.

In a very real sense, you could easily think about coordinates and graphics in two different ways. The way to think about them depends on the type of graphics operation being performed. If you are dealing with a fill operation of some kind, then you think about the drawing surface with coordinate lines "in between" the physical pixel cells. If you are dealing with a line drawing situation (or drawing points, circles, etc), then you can think of coordinates directly addressing the pixel cells themselves (just like when we began this conversation at the beginning of this section). When dealing with line drawing operations, there is no difference in mathematical models - there's only a difference when you get into filling areas in. Whichever method of thinking of things works best for you, use.

Other filled objects work the same way as do filled rectangles. For example, consider an unfilled polygon with the following vertices: _(v2.A4)_

|          | X Pos | Y Pos |
| -------- | ----- | ----- |
| Vertex 0 | 0     | 0     |
| Vertex 1 | 8     | 0     |
| Vertex 2 | 14    | 3     |
| Vertex 3 | 14    | 4     |
| Vertex 4 | 12    | 6     |
| Vertex 5 | 4     | 6     |
| Vertex 6 | 0     | 2     |
| Vertex 7 | 0     | 0     |

With this polygon we would have a polygon that looks like the left diagram below. The right diagram is what it would look like if it were filled and had a border drawn at the same time (the different shaded blocks indicates which is border, fill-color, or both): _(v2.A4)_

```text
                     1 1 1 1 1 1                       1 1 1 1 1 1
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
0╔═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╗  0╔═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╗
 ║█│█│█│█│█│█│█│█│█│ │ │ │ │ │ ║   ║▒│▒│▒│▒│▒│▒│▒│▒│█│ │ │ │ │ │ ║
1╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  1╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║█│ │ │ │ │ │ │ │ │█│█│ │ │ │ ║   ║▒│░│░│░│░│░│░│░│░│█│█│ │ │ │ ║
2╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  2╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║█│ │ │ │ │ │ │ │ │ │ │█│█│ │ ║   ║▒│░│░│░│░│░│░│░│░│░│░│█│█│ │ ║
3╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  3╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║ │█│ │ │ │ │ │ │ │ │ │ │ │█│█║   ║ │▒│░│░│░│░│░│░│░│░│░│░│░│█│█║
4╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  4╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║ │ │█│ │ │ │ │ │ │ │ │ │ │ │█║   ║ │ │▒│░│░│░│░│░│░│░│░│░│░│░│█║
5╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  5╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║ │ │ │█│ │ │ │ │ │ │ │ │ │█│ ║   ║ │ │ │▒│░│░│░│░│░│░│░│░│░│█│ ║
6╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  6╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║ │ │ │ │█│█│█│█│█│█│█│█│█│ │ ║   ║ │ │ │ │█│█│█│█│█│█│█│█│█│ │ ║
7╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝  7╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝
        Unfilled polygon             Filled and unfilled polygon
```

Taking a second example. Let's consider an unfilled oval centered at (7,3) with a horizontal radius of 8 and a vertical radius of 4. The unfilled oval is depicted in the left diagram below. The diagram on the right is the same oval with the border drawn and also filled in. Notice how the border is drawn in relation to the filled-in interior and how it adheres to resolution independence. _(v2.A4)_

```text
                     1 1 1 1 1 1                       1 1 1 1 1 1
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
0╔═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╗  0╔═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╪═╗
 ║ │ │ │ │█│█│█│█│█│█│█│ │ │ │ ║   ║ │ │ │ │▒│▒│▒│▒│▒│▒│█│ │ │ │ ║
1╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  1╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║ │ │█│█│ │ │ │ │ │ │ │█│█│ │ ║   ║ │ │▒│▒│░│░│░│░│░│░│░│█│█│ │ ║
2╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  2╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║ │█│ │ │ │ │ │ │ │ │ │ │ │█│ ║   ║ │▒│░│░│░│░│░│░│░│░│░│░│░│█│ ║
3╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  3╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║█│ │ │ │ │ │ │■│ │ │ │ │ │ │█║   ║▒│░│░│░│░│░│░│■│░│░│░│░│░│░│█║
4╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  4╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║█│ │ │ │ │ │ │ │ │ │ │ │ │ │█║   ║▒│░│░│░│░│░│░│░│░│░│░│░│░│░│█║
5╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  5╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║ │█│ │ │ │ │ │ │ │ │ │ │ │█│ ║   ║ │▒│░│░│░│░│░│░│░│░│░│░│░│█│ ║
6╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  6╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║ │ │█│█│ │ │ │ │ │ │ │█│█│ │ ║   ║ │ │▒│▒│░│░│░│░│░│░│░│█│█│ │ ║
7╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢  7╫─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
 ║ │ │ │ │█│█│█│█│█│█│█│ │ │ │ ║   ║ │ │ │ │█│█│█│█│█│█│█│ │ │ │ ║
8╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝  8╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝
```

If you look closely at the right diagram, you will notice that the oval isn't "balanced". This is because in this situation, the oval is centered about a particular point, but the filled oval is partially offset on an odd-boundary - the filled interior is 8 pixels to the left of the center point (inclusive), and 7 pixels to the right of the center point (inclusive). This is due to the resolution independent nature of filled regions. If you need to draw a perfectly smooth filled oval, draw one with a solid fill and with borders enabled, both with the same drawing and fill color. _(v2.A4)_

---

[◀ Prev: Data Backup Areas](04-data-backup-areas.md) · [Contents](README.md) · [Next: Color, Audio & Text Windows ▶](06-color-audio-text.md)
