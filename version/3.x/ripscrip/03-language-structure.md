# Language Structure

[◀ Prev: Applications of RIPscrip](02-applications.md) · [Contents](README.md) · [Next: Design Goals & Graphical Primitives ▶](04-design-goals-and-primitives.md)

## 3. Overview of the Technology

RIPscrip is a very comprehensive scripting language. It contains many hundreds of commands and features that give you the flexibility to do just about whatever your imagination would like. The actual protocol definition is a document approximately 300 printed pages in length, covering every aspect of the language including its syntax and capabilities.

Earlier versions of this language specification have been publicized widely, via freely distributable documents. The current RIPscrip 3.0 protocol definition is in the final stages of finalization and will be published very shortly.

## 3.1 What RIPscrip Is and Is Not

With all of this discussion about online services, graphical presentation technologies and graphical user interfaces, it would be useful to explain what RIPscrip is, and what it is not. In the past, much confusion has existed about what the technology really is, and as such, a better explanation is needed.

### 3.1.1 What It Is - A Graphical Presentation Environment

RIPscrip is a graphical presentation language. It contains many commands and features for the display of fast, high-quality online graphics. It allows you to mix graphics and text, from a variety of sources, into one integrated presentation medium (i.e., the computer screen). You can mix vector graphics, bitmapped graphics, photos and text in a variety of fonts and colors on the screen to create just about any style of presentation you could imagine. You can even integrate ANSI text into your presentation so that you can interface with older online applications. With RIPscrip, you get the best of both worlds.

### 3.1.2 What It Is Not - A Graphical User Interface

RIPscrip is not a graphical user interface (GUI), per se. Yes, it contains many of the necessary features to display a GUI, but it is not, in and of itself, a GUI. You can display dialog boxes, menus and other window-like objects that are commonly available in existing GUI's, but you draw them!

For example, if you want to display a dialog box on the screen, RIPscrip provides you with the necessary tools to construct your own dialog box - showing clickable buttons, radio buttons, checkboxes, and other such controls. In order to do this though, you have to draw what you want on the screen. This is because RIPscrip is a graphical presentation language. You have to present your users with exactly what you want to show them. This gives you the flexibility to show them whatever you want, in whatever appearance you want. Again, flexibility is the goal with RIPscrip.

RIPscrip's fundamental design and philosophy is geared toward allowing you, the service provider, the ability to display information in any manner that you want. This is simply not possible in most traditional GUI environments. For example, Microsoft Windows applications typically look very similar, and are quite different than Macintosh applications. Using RIPscrip, you can make your applications look like Windows, Macintosh, X-Windows, OS/2 Warp, or just about anything else you can imagine. The choice is entirely yours!

## 3.2 Text Nature of the Language

As previously described, RIPscrip is a text-based graphical language. It is designed around a parameterized command structure, where each command in the language accepts zero or more parameters to alter the functionality of the command. Each command is a set of tightly compacted sequence of character, numeric and text parameters.

In order to make RIPscrip as fast as possible over slower network connections, some sacrifices naturally had to be made. The first was that the language was not readable in normal English. English representation of commands is too bulky for any kind of realistic encoding of graphics data. Encoding the information in English or some other suitably readable language would have made the efficiency of the language terrible, and never would have gained any kind of industry support. Again, it had to be fast, therefore readability had to be thrown out the window.

The second aspect of the language is that it had to be transmittable over normal 7-bit ASCII text connections. This requires that the language be printable text. Transmitting binary information over a modem connection requires special binary protocols and other special environments, which makes it all that much harder to implement on various software and hardware environments. Since everything in the online world can transmit text, using a text-based encoding scheme makes sense, since it is supported by all systems.

An additional level of efficiency could have been gained by making the protocol binary in nature, but it would have been at the sacrifice of significantly reduced compatibility with online environments, and that custom designed software would be required for just about every operating system and platform used in the online world. For these reasons, binary encoding in RIPscrip was out of the question.

## 3.2.1 Numeric Encoding

Unlike many other script languages, RIPscrip doesn't use decimal values for its numbering system. Decimal representations of numbers are too bulky for efficient transmission of information. Since transmission efficiency is of paramount importance in the structure of the language, the encoding of numbers in as efficient a manner as possible became the most fundamental concept.

RIPscrip uses two different numbering systems to encode its numeric values. The first method, adopted in the 1.0 edition of RIPscrip, utilizes a base-36 encoding scheme. In this system, numbers are stored as sequences of numeric digits '0' through '9', and capital letters 'A' through 'Z'. Using this encoding scheme, a single character can store a much larger number than could normally be represented in decimal. This system of numeric encoding has been called [MegaNum](../../2.x/ripscrip/05-coordinates-and-math.md) in RIPscrip.

The second method of numeric encoding was introduced in RIPscrip 2.0. It has been called [UltraNum](../../2.x/ripscrip/05-coordinates-and-math.md), and is based around a base-64 encoding scheme. Similar to MegaNums, UltraNums use the same 36 characters for the lower part of the encoding spectrum. The next sequences of encoded values are the lowercase letters 'a' through 'z' and the symbols '#' and '&'. This provides for 64 separate values that can be encoded into a single byte of data. This allows you to store even larger numbers in a smaller space than MegaNums permitted.

Here is a table detailing the sizes of numbers that can be stored in various numbers of bytes, showing comparisons to decimal and hexadecimal encoding schemes:

| Total Digits | Decimal | Hexa-Decimal | MegaNums      | UltraNums      |
|--------------|---------|--------------|---------------|----------------|
| 1            | 9       | 15           | 35            | 63             |
| 2            | 99      | 255          | 1,295         | 4,095          |
| 3            | 999     | 4,095        | 46,655        | 262,143        |
| 4            | 9,999   | 65,535       | 1,679,615     | 16,777,215     |
| 5            | 99,999  | 1,048,575    | 60,466,175    | 1,073,741,823  |
| 6            | 999,999 | 16,777,215   | 2,176,782,335 | 68,719,476,735 |

In practice, using a higher-base numeric encoding scheme improves efficiency of transmission and provides for a higher-degree of expansion capability. For example, since many numeric parameters in RIPscrip are two-bytes in length (see below), this gives a fundamental limit of 1295 in MegaNum, or 4095 in UltraNums. When working with graphical coordinates, a value in the range of 0-640 is quite possible - typically it will be three digits in length, but if you can store that information in only two bytes of data, that gives you an overall savings of 50%. In a transmitted data stream, this kind of efficiency becomes extremely important.

## 3.2.2 Structure of the Protocol

RIPscrip is structured around a hierarchical, command-based architecture. Each command is part of a tree-like substructure of [command levels](../../2.x/ripscrip/07-protocol-definition.md), and is assigned a particular command-character. The command-level, and the command character uniquely identify the command as a particular RIPscrip command.

RIPscrip commands can be intermixed with normal, TTY or ANSI text information so that you can mix and match graphics and text information inside of a single document. To distinguish between RIPscrip commands, special RIPscrip introducer codes are necessary to indicate when a graphical command is about to start. Normally, this introducer sequence is the ASCII text sequence "!|" (exclamation mark, followed by a vertical bar character) starting in column one of a line of text. If this sequence doesn't appear at the beginning of a line of text, then the line is assumed to be raw, TTY/ANSI text and is displayed as such.

There are other ways to introduce a RIPscrip sequence in the middle of a line, but the details of which go beyond the scope of this discussion. Suffice to say that RIPscrip commands need not start at the beginning of a line of text, but are more often than not, used this way.

Each RIPscrip command is separated by a command delimiter character sequence. This sequence is the vertical bar character in ASCII, or the value 124. In this way, you can separate multiple RIPscrip commands on a single line of text following the RIPscrip introducer sequence.

## 3.2.3 Command Levels

RIPscrip is organized in a hierarchical system of commands. Each command is part of a particular ["level"](../../2.x/ripscrip/07-protocol-definition.md) of RIPscrip, where each level is a conceptual "type" of command. Currently, there are ten separate levels of commands, each one of which can be further subdivided into deeper and deeper levels of commands. This tree-like structure allows RIPscrip to utilize potentially billions of commands.

Command levels are organized into ten basic levels, which are assigned command-level digits. The lowest command level, level-0, has no command level digit associated with it. It is designated for the most primitive commands in the RIPscrip language (e.g., line, rectangle, circle, font, color, etc.). Each level of RIPscrip, may be divided into further sub-levels of commands in a tree-like arrangement, using more level digits to define the actual depth and position of the sequence.

For example, the RIPscrip command for a simple line is "!|L". This stands for level-0, command character "L". A hypothetical command defined at level 3, sub-level 2 would be defined as "!|32L". You may have commands up to maximum of 9 levels deep, where each command sub-level can accommodate up to 79 separate command characters. This gives the RIPscrip language the capacity to utilize literally billions of commands. This should give the language plenty of room for growth in the future.

## 3.2.4 Command Parameters

Command parameters always follow the command character. There are two kinds of parameters in RIPscrip - the numeric parameter, and the text parameter. If a text parameter is available for a particular RIPscrip command, it is always the very last parameter and has a maximum length of 1024 bytes.

Numerical parameters on the other hand can be varied in their number, size and encoding methods. Different RIPscrip commands allow for a different number of parameters to be specified. In fact, some RIPscrip commands allow for a variable number of numeric parameters (e.g., polygons, headers, palettes, etc.). Numeric parameters can also be encoded using different numeric encoding schemes (i.e., [MegaNum or UltraNum](../../2.x/ripscrip/05-coordinates-and-math.md)) based on the requirements of a particular command, or a global protocol setting.

An example of a basic RIPscrip command is the [RIP_LINE](../../2.x/ripscrip/09-level-0-commands-g-r.md#rip_line) command. This command utilizes four separate parameters which define the two end-points of a line in (X,Y) coordinates. The basic syntax of this command is:

```text
!|L x0:2 y0:2 x1:2 y1:2
```

Note that spaces are not permitted in this command, and are only provided for readability. The sequence ":2" indicate that the parameter is two bytes long. An example of a RIP_LINE command which draws a line from (5,4) to (130,99), encoded using MegaNums, would appear like this:

```text
!|L05043M2R
```

Another example, showing both numeric and text parameters, would be the RIPscrip command that outputs the phrase "Hello World" at the location (35,71) on the screen. This example, encoded in MegaNums would appear like this:

```text
!|@0Z1ZHello World
```

## 3.2.5 Command Structure Philosophy

The basic design goal of the command-level structure in RIPscrip, is for the possibility of third-party add-on products utilizing RIPscrip. With a large command capacity, ranges of commands and command-levels can be authorized for use by developers in their own products, and those developers can add their own commands to the language as they see fit. This philosophy has not yet been introduced to the developer community, but the capability is present should the need arise.

This kind of a system might allow a particular developer to implement an entire suite of 3D graphics primitives in RIPscrip, included as part of a RIPscrip add-on module. He can design graphics using his command-set, and customers using his add-on module with the basic RIPscrip engine, could view the 3D graphics the way that they were intended to be viewed. This has the potential to open up a vast third-party developer market for products and services.

---

[◀ Prev: Applications of RIPscrip](02-applications.md) · [Contents](README.md) · [Next: Design Goals & Graphical Primitives ▶](04-design-goals-and-primitives.md)
