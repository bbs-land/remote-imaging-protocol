# Drawing Ports

[◀ Prev: Introduction](01-introduction.md) · [Contents](README.md) · [Next: Data Tables ▶](03-data-tables.md)

## How RIPscrip is Designed - Fundamental Topics

_Added in RIPscrip v2.A4._

The sections that following describe specific areas of RIPscrip that are fundamental aspects of the language and its design. Each section is designed to give you information on one basic area of RIPscrip.

## Drawing Ports - What Are They?

_Added in RIPscrip v2.A4._

A drawing port is an area where graphics can be drawn. This is much like having a tablet of paper, where each page can be drawn to individually. However, just like a tablet of paper, only the piece of paper that is on the top of the tablet can be drawn to (the current piece of paper). The same concept applies to drawing ports. At any one moment in time, there is a "current drawing port". This current port is the one that will display graphical drawing operations when they are received.

There are two types of drawing ports:

1. A screen (visual) drawing port, and
2. An offscreen bitmap drawing port (also known as a "Clipboard Port".

### Screen Drawing Ports

A screen drawing port is the most commonly used form of drawing ports. A screen drawing port is a port that is somewhere on top of the screen. What this means is when a graphical drawing operation occurs, it is displayed in that port and subsequently to the screen immediately. Screen drawing ports are methods of dividing up the user's screen into separate regions, of which each can be thought of as completely separate drawing areas. You can switch between them pretty much at will to draw your graphics.

Here is a typical example of one screen and how it can be divided up with multiple drawing ports:

```text
                                 ┌─── Screen boundary
                                 │
                                 ▼
     ╔══════════════════════════════════════════════════╗
     ║ Port 0                                           ║
     ║                                                  ║
     ║  ┌────────────────────┐  ┌────────────────────┐  ║
     ║  │ Port 1             │  │ Port 2             │  ║
     ║  │                    │  │                    │  ║
     ║  └────────────────────┘  └────────────────────┘  ║
     ║  ┌────────────────────────────────────────────┐  ║
     ║  │ Port 3                                     │  ║
     ║  │                                            │  ║
     ║  └────────────────────────────────────────────┘  ║
     ║  ┌────────────────────────────────────────────┐  ║
     ║  │ Port 4                                     │  ║
     ║  │                                            │  ║
     ║  └────────────────────────────────────────────┘  ║
     ╚══════════════════════════════════════════════════╝
```

As you can see, you have quite a bit of flexibility in placing your port's around on the screen. In this example, we have five ports defined on the screen. You might notice that port number 0 doesn't seem to have a rectangle associated with it. Actually, it does - it is the screen's boundary. Port number 0 is defined as the actual screen and cannot be redefined (you can alter its viewport though - see below for more details about viewports). Port 0 is always the full size of the screen. You can create your own screen ports, but port 0 is one that you cannot redefine the boundary of. This gives you the the ability to always switch to port 0 to address the entire screen, but have the luxury of other ports when you only want to work with a portion of the screen.

### Offscreen Drawing Ports (Clipboard Ports)

An offscreen bitmap port, otherwise known as a "clipboard port", is very much like a screen port, but it isn't actually a part of the screen. It is more like another screen that you cannot see, but one that you can still draw to. You might be asking yourself why something like this is part of RIPscrip? The answer is quite simple - they are extremely powerful! An offscreen port can be used for placing a piece of graphics data temporarily while you are showing a dialog box on the screen, only to be restore the original graphics when the user clicks the "OK" button on the dialog box. The graphical data that was overwritten by the dialog box isn't deleted - it's been saved temporarily on a screen that you cannot see, but is restored to the screen when necessary. Another use of this might be if you had a very complicated scene to display, but you didn't want the user to see each little graphics operation until the entire scene was complete; switch to an offscreen bitmap port, draw your scene, then switch back to the screen and copy the offscreen port's graphics to your screen. Voila, the user sees the scene appear on his screen complete - not piecemeal!

There are numerous reasons why you would use offscreen ports; things like storing a bitmap that you just loaded from the hard disk onto an offscreen port so that when you paste it to the screen a large number of times, there isn't a large amount of disk activity on the user's machine. Another good example is in the situation of an online game where the screen is some kind of map, and one or more offscreen ports are used to hold small "icons", or bitmaps of game pieces. Simply copy the images from the offscreen port(s) to appropriate locations on the map and you've accomplished a rather complicated situation without adversely affecting the user's system with intense hard drive activity due to constantly loading bitmaps off the disk - the side effect, the game moves much more swiftly and with a lot less "jerkiness" due to the hard disk accesses.

Just like with screen drawing ports, you can select an offscreen bitmap port as the one which will receive graphical drawing operations. Selecting an offscreen port as the current port let's you draw simple graphics objects, photos and other such things to it. But remember, the graphics you draw to an offscreen port are of little value unless at some point you actually copy it to the screen so that the user can see it.

Some things cannot be done to offscreen ports. For example, you cannot place a mouse field or a clickable button on an offscreen port. The reason for this is that the mouse only has any meaning to the screen - the environment that the user interacts with. How can the user click on a button that he can't see? He can't. You also cannot place [text windows](06-color-audio-text.md) or assign resident query expressions to offscreen ports (these topics are described more fully in later sections).

An offscreen port is most easily thought of as a graphics screen that you can't see. You can work with it to your heart's content, but the user can't see the contents of it until you copy the data to the screen. As long as you keep this one fundamental concept in mind, there should be no confusion about ports.

### Ports and Coordinates

A port is a specific drawing area. No matter whether the port is a screen port of an offscreen bitmap port, it is still a drawing area, and as such, it has a legal number of horizontal and vertical pixels that can be drawn to. Every graphical drawing operation uses graphical coordinates with which they draw their image to the current port. For example, a circle needs to know the center point, and the radius of the circle in order to draw itself.

Each port has what can be thought of as its own set of coordinates. Just like the screen where (0,0) is the upper-left corner of the screen, when a port is active (ie, current), (0,0) is the coordinate of the upper-left corner of the drawing port. If a port had a width of W and a height of H, then the lower-right coordinate would be (W-1,H-1). This holds true for all drawing ports, even offscreen bitmap ports.

For example, if you draw a circle at (50,50) with a radius of (25,25) to the current port (a screen port), then you might have something like the following diagram:

```text
                               ┌──── Screen Boundary
 (0,0)                         ▼
   ╔══════════════════════════════════════════════════╗
   ║                                                  ║
   ║    ┌───────────────────────────────────────┐     ║
   ║    │(10,10)            │◄─25─►│            │◄───────Port
   ║    │                 ▄▄▄▄▄                 │     ║
   ║    │              ▄▀▀     ▀▀▄              │     ║
   ║    │             █           █             │     ║
   ║    │            █      ■ ◄─── █ ──────────────────── (60,60) on
   ║    │             █  (50,50)  █             │     ║   the screen
   ║    │              ▀▄▄     ▄▄▀              │     ║
   ║    │                 ▀▀▀▀▀                 │     ║
   ║    │                              (124,124)│     ║
   ║    └───────────────────────────────────────┘     ║
   ║                                                  ║
   ╚══════════════════════════════════════════════════╝
```

In this example, the port starts at (10,10) on the screen and goes down to (124,124) on the screen (a width of 115 and a height of 115). Since you said to draw the circle centered at (50,50) in the current port, what actually happens on the screen is that the circle is physically drawn at (50+10,50+10) to the screen, or centered at the "absolute" coordinate of (60,60) in relation to the actual screen.

As you can see, each port can be thought of as its own little drawing universe, with its own set of coordinates. This makes drawing things like graphs, or showing game map windows easy - without having to make extensive calculations in the host software to figure out where things need to be placed. This is considered "port relative" coordinates.

Screen ports are defined as some location on the actual screen from the upper-left corner to the lower-right corner. From this information, the width and height of the port can be easily calculated.

Offscreen ports are a bit different though. Since they're not part of the user's actual screen, they don't have this upper-left location on the screen. In this manner, an offscreen port is always thought of as having an upper-left corner of (0,0) and its lower-right corner as (W-1,H-1).

### Ports and Viewports

Each drawing port has an associated viewport, or clipping rectangle. A viewport is a rectangle inside the port that defines the drawing limits inside that port. Its similar in concept to a coloring book with invisible lines. You can draw all you want, but you can't go outside the lines. If any operation would extend beyond the edge of this clipping rectangle, it will be truncated. For example, if you have a drawing port that is 100x100 pixels in size, and you define the viewport to be from (25,25) to (74,74), you would have a 50x50 drawing area right in the center of the drawing port. If you then draw a circle in the exact center of the drawing port with a radius of 60 pixels, you would have pieces of the circle that extend beyond the top, left, bottom and right borders of the viewport. What you would see would be four arcs in each corner of the viewport as in the following example:

```text
                           Viewport from (25,25) to (74,74)
                           ───────┬────────────────────────
         (0,0)                    │
            ╔═════════════════════│════╗
            ║          ░░░░░      │    ║
            ║       ░░░     ░░░   ▼    ║
            ║   ┌─██───────────██─┐    ║
            ║   │█               █│    ║
            ║   █                 █    ║
            ║  ░│                 │░   ║
            ║ ░ │                 │ ░  ║
            ║ ░ │                 │ ░  ║
            ║ ░ │                 │ ░  ║
            ║  ░│                 │░   ║
            ║   █                 █    ║
            ║   │█               █│    ║
            ║   └─██──────────-██─┘    ║
            ║       ░░░     ░░░        ║
            ║          ░░░░░           ║
            ╚══════════════════════════╝ (99,99)
```

If you notice in the above diagram, the rectangle inside the port defines the viewport. The solid squares (█) denote pixels inside the viewport that would actually be drawn, and the shaded squares (░) show pixels that will not be drawn because they are outside the viewport.

You can alter the location and size of the viewport inside any given drawing port. By default, the viewport is set to the full size of the drawing port when the port is created. If you attempt to make the viewport go outside the actual port, it will be adjusted to fit so that it is completely within the port. If an attempt is made to define a viewport that is completely outside the boundary of its port, then the definition is ignored as an error condition. If the lower-right corner is outside the boundary of the port, then the lower-right corner of the newly defined viewport is set to the lower-right corner of the port itself.

Just like drawing ports, a viewport also "adjusts" drawing coordinates. When you alter the location of a viewport inside of a drawing port, the origin (0,0) for drawing operations relates to the upper-left corner of the viewport itself, not the actual underlying drawing port. All drawing operations pertain to a port's viewport, not the port itself. The port is considered the maximum limits for the viewport inside - just like the screen, you can only draw to areas inside the screen; the same applies to viewports and ports.

As an example, let's say you have a screen port defined and you alter the viewport (remember, by default it is the full size of the port when the port is defined until you re-define it). The following diagram will give you an idea of our example:

```text
                          ╔════ Screen boundary
                          ▼
╔═════════════════════════════════════════════╗
║(0,0) screen                                 ║
║    ┌───────── (0,0) port                    ║
║    ▼          (25,25) screen                ║
║    ■───────────────────────────────────┐ ◄══════ Port boundary
║    │           │ (0,0) viewport        │    ║
║    │        ┌──┤ (40,40) port          │    ║
║    │        ▼  │ (65,65) screen        │    ║
║    │        ■─────────────────────┐    │    ║
║    │        │                     │    │    ║
║    │        │                     │ ◄═══════════ Viewport boundary
║    │        │          ■          │    │    ║
║    │        │          ▲          │    │    ║  │ (130,130) viewport
║    │        └──────────│──────────■ ◄──────────┤ (170,170) port
║    │                   │               │    ║  │ (195,195) screen
║    │                   │               │    ║
║    └───────────────────│───────────────■ ◄────── (200,200) port
║                        │                    ║    (225,225) screen
╚════════════════════════│════════════════════╝
                         │    │ (50,50) viewport
                         └────┤ (90,90) port
                              │ (125,125) screen
```

In this example, our port is defined from (25,25) to (225,225) on the screen. This makes our port 201 pixels wide and 201 pixels tall. Our viewport starts at (40,40) in the port's coordinate system and ends at (170,170). This makes our viewport 131 pixels wide and 131 pixels tall. The upper-left coordinate of the viewport would map to the screen coordinates (25+40,25+40) or (65,65), and the lower-right corner would map to (25+40+130,25+40+130) or (195,195).

As you can see, mapping a screen port's coordinates to those actually used on the screen can get quite involved. But when you get down to the benefits of coordinate systems, this approach provides quite a bit of flexibility in moving things around without having to change coordinate systems all the time.

### Ports, Viewports and Graphical Operations

We've already discussed how a viewport "truncates" a graphical operation if it extends beyond the border of the port's viewport.

What if you're copying graphical data from one port to another? Graphics data can only be copied from one port to another in rectangular portions, and just like all other graphical operations, this kind of situation adheres to the viewports of both the source and the destination ports' viewports! Let's take an example where you are copying a rectangle of graphics from port 1 to port 2. For sake of clarity, we'll copy the entire viewport over (not a sub-area of the viewport). The following diagram shows what would happen if the two viewports aren't the exact same size (specifically, the source viewport is larger than the destination viewport):

```text
      ┌─── Port 1's viewport             ┌─── Port 2's viewport
      │                                  │
╔═════│═══════════════════════╗     ╔════│══════════════════╗
║     ▼                       ║     ║    ▼                  ║
║   ┌───────────────────┐     ║     ║ ┌─────────────┐       ║
║   │███████████████████│     ║     ║ │█████████████│░░░░░  ║
║   │███████████████████│     ║     ║ │█████████████│░░░░░  ║
║   │███████████████████│     ║     ║ │█████████████│░░░░░  ║
║   │███████████████████│     ║     ║ └─────────────┘░░░░░  ║
║   │███████████████████│     ║     ║  ░░░░░░░░░░░░░░░░░░░  ║
║   └───────────────────┘     ║     ║                       ║
╚═════════════════════════════╝     ╚═══════════════════════╝
            Port 1                            Port 2
```

The squares that are shown as solid (█) are the graphics data that is to be copied (port 1), and the graphics data that is actually copied (port 2). The shaded squares (░) are graphics data from the source viewport that aren't copied to the destination. This shows how a viewport "truncates" graphical drawing operations, even when copying data from one port to another.

### Copying Data From One Port to Another

When you copy graphical data from one port to another (or to another location on the same port for that matter), you are duplicating the graphical contents of the source port onto the destination port. Whether the result is an exact replica of the original image or not is another matter. Whenever you copy a rectangle of data from one port to another, you need to specify a rectangle in the source port, and another one in the destination port. These two rectangles do not need to be the same pixel size. If they are different either in the width or height dimensions, then scaling of the source image will occur! For example, if the source image's rectangle doesn't have the same dimensions as the rectanalge in the destination port, you could have a situation similar to the following:

```text
╔══════════════════════════════╗   ╔══════════════════════════════╗
║   ┌────────────────────┐     ║   ║  ┌────────────────────────┐  ║
║   │██▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀██│     ║   ║  │██▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀██│  ║
║   │█ ▀▄            ▄▀ █│     ║   ║  │█ ▀▄                ▄▀ █│  ║
║   │█   ▀▄        ▄▀   █│     ║   ║  │█   █▀▀▀▀▀▀▀▀▀▀▀▀▀▀█   █│  ║
║   │█     █▀▀▀▀▀▀█     █│     ║   ║  │█   █▄▄▄▄▄▄▄▄▄▄▄▄▄▄█   █│  ║
║   │█     █      █     █│     ║   ║  │█ ▄▀                ▀▄ █│  ║
║   │█     █      █     █│     ║   ║  │██▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄██│  ║
║   │█     █▄▄▄▄▄▄█     █│     ║   ║  └────────────────────────┘  ║
║   │█   ▄▀        ▀▄   █│     ║   ║                              ║
║   │█ ▄▀            ▀▄ █│     ║   ║                              ║
║   │██▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄██│     ║   ║                              ║
║   └────────────────────┘     ║   ║                              ║
╚══════════════════════════════╝   ╚══════════════════════════════╝
        Source (Port 1)                  Destination (Port 2)
```

Notice that the image in the destination is shorter, but much wider than the original. This is because in this example we specified our destination rectangle to be shorter, but wider in size than the original image in the source port. If the two rectangles had the same width and height values, then our destination image would be pixel-for-pixel identical to the original (providing that the viewport of the destination port didn't truncate the image of course).

When copying rectangular pieces of data from one port to another, you have to be careful about viewports. If either the source or the destination rectangle are completely outside that port's viewport, then the copy operation isn't performed - nothing would be visible if it did happen, because either the source or the destination image wouldn't be visible inside the viewport.

Here is another example showing a straight copy operation without any scaling being performed. We are assuming that the source and destination rectangles are the same size. The port's boundary and it's viewport are shown in the following diagram:

```text
     ╔════════════════════╗       ╔═══════════════════════════╗
     ║  ┌────────────┐    ║       ║  ┌───────────────────┐    ║
     ║  │██  ██  ██  │░░░ ║       ║  │██  ██  ██  ░░░░   │    ║
     ║  │  ██  ██  ██│░░░ ║       ║  │  ██  ██  ██░░░░   │    ║
     ║  │██  ██  ██  │░░░ ║       ║  │██  ██  ██  ░░░░   │    ║
     ║  │  ██  ██  ██│░░░ ║       ║  │  ██  ██  ██░░░░   │    ║
     ║  │██  ██  ██  │░░░ ║       ║  │██  ██  ██  ░░░░   │    ║
     ║  │  ██  ██  ██│░░░ ║       ║  │  ██  ██  ██░░░░   │    ║
     ║  └────────────┘░░░ ║       ║  └───────────────────┘    ║
     ║   ░░░░░░░░░░░░░░░░ ║       ║   ░░░░░░░░░░░░░░░░        ║
     ║   ░░░░░░░░░░░░░░░░ ║       ║   ░░░░░░░░░░░░░░░░        ║
     ╚════════════════════╝       ╚═══════════════════════════╝
```

The shaded squares (░) are graphics that extend beyond the source's viewport. Those pixels aren't copied to the destination port, but they are shown in the upper-right diagram to show where the graphics would have been in the destination port if they were copied.

Let's take another example but this time we will show scaling because this situation may not be intuitive. Let's assume that our source rectangle is twice as large as our destination rectangle both in the width and height dimensions. This means that our image will be reduced by 1/2. But what happens if the source image needs to be "truncated" to fit in the viewport? The answer is simple - only the data that remains after truncation is scaled into the destination rectangle. If the destination rectangle also had to be truncated, then the scaling is still performed. Under no circumstances will a vertical or horizontal "blank" zone be created during scaling. When you say "scale to fit", it does exactly what you told it to - it makes the graphics fit in the given area. Here is the diagram that shows this situation:

```text
          ╔════════════════════╗       ╔══════════════╗
          ║  ┌────────────┐    ║       ║  ┌─────────┐ ║
          ║  │██  ██  ██  │░░░ ║       ║  │▀▄▀▄▀▄░░ │ ║
          ║  │  ██  ██  ██│░░░ ║       ║  │▀▄▀▄▀▄░░ │ ║
          ║  │██  ██  ██  │░░░ ║       ║  │▀▄▀▄▀▄░░ │ ║
          ║  │  ██  ██  ██│░░░ ║       ║  │░░░░░░░░ │ ║
          ║  │██  ██  ██  │░░░ ║       ║  │░░░░░░░░ │ ║
          ║  │  ██  ██  ██│░░░ ║       ║  └─────────┘ ║
          ║  └────────────┘░░░ ║       ╚══════════════╝
          ║   ░░░░░░░░░░░░░░░░ ║
          ║   ░░░░░░░░░░░░░░░░ ║
          ╚════════════════════╝
```

---

[◀ Prev: Introduction](01-introduction.md) · [Contents](README.md) · [Next: Data Tables ▶](03-data-tables.md)
