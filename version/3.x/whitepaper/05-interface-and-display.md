# User Interface & Display

[◀ Prev: Design Goals & Graphical Primitives](04-design-goals-and-primitives.md) · [Contents](README.md) · [Next: Data Tables & Backup System ▶](06-data-tables-and-backup.md)

## 3.5 User-Interface Objects

Up until now, we've only been discussing the drawing, or presentation capabilities of RIPscrip. RIPscrip offers much more than just simple drawing capabilities. It has many built-in user-interface objects which can be used to control the sequence of events in an online environment.

For example, on a traditional online service, you have a text menu that instructs the user what command to type to activate a particular operation. Under RIPscrip, this scheme is still possible, but you probably wouldn't want to use it. What would be better would be to display a menu with buttons on the screen that the user can click. Clicking on one of the buttons will activate that function, causing the host system to perform the desired operation. In this kind of environment, the user doesn't have to know what the command is in order to activate an option; all he has to know is that he clicks on a picture of an envelope to activate the Email option. This is the essence of RIPscrip user-interface objects.

There are three user-interface objects in RIPscrip: the mouse field, the button and the query expression. We will cover each of these separately.

## 3.5.1 Mouse Fields

Mouse fields are the most generic of user-interface objects in RIPscrip. They allow you to perform hyper-links from one area of an online service to another. You can even perform a large number of operations that don't even affect the host system with mouse fields. Suffice to say, that mouse fields are a versatile mechanism to perform numerous functions when the user clicks his mouse on the screen.

A mouse field is an invisible rectangle that you can place on the screen. When the user clicks the left mouse button inside this screen area, it activates a particular host command sequence defined for that mouse field.

For example, let's say that you have the image of a mailbox on the screen, and that your host system expects the user to type the letter `E` followed by a carriage return to enter the Email system. You could make the mailbox image "clickable", forcing the user into the Email system by placing a mouse field over the mailbox. Define the mouse field on top of the mail box (remember, a mouse field is invisible so it won't obstruct the image of the mailbox), and define a host command of `E^M` for that mouse field. This transmits the character `E` to the host system, followed by a CTRL-M sequence (i.e., a carriage return). When the user clicks on the mailbox, the user will be placed into Email on the host system.

You can do many more things with mouse fields beyond this example. You can assign hotkeys to mouse fields, change what happens to the screen when the user clicks on them, and even utilize the extensive [host command language](../../2.x/ripscrip/14-host-commands.md) of RIPscrip from inside the host command of the mouse field (see below).

## 3.5.2 Buttons

Buttons are perhaps the most common user-interface object in RIPscrip. They combine two distinctly different concepts into a more powerful command. Specifically a button combines the concepts of a mouse field, with a visible image. This not only creates a clickable object, but also gives it a specific image in one basic command.

There are three types of button objects. The first is the plain button. A plain button is a button that uses built-in drawing effects in RIPscrip to display the button. An example of a plain button might be a gray beveled button with the phrase "Email" embossed in dropshadowed text.

The second, more flexible style of button is the bitmapped button, or icon button as it's commonly known. This allows you to design a button to look like anything you want. You use one, or possibly two pictures for the image of your button. One picture is the image that is displayed for the button in its normal, un-selected state. The optional second image is used to display the button when it is selected, or clicked.

The third, and perhaps least frequently used style of buttons is called a snapshot button. A snapshot button uses an image contained on the RIPscrip image snapshot to display the button. A snapshot is a rectangular image "snapshot" of a piece of graphical information from the screen the can be used to "stamp" onto the screen in one or more locations to save graphical operations. For example, you could take twenty or so RIPscrip commands to draw the image of a brick on the screen, take a snapshot of that brick, then paste the snapshot image on the screen to make a brick wall.

You have the ability to display bevels, chisels, recessed and sunken special effects on all types of buttons, giving them the appearance of being 3D. TeleGrafix uses the special effects of buttons to achieve a "chiseled steel" appearance to its menus, but this is only one particular color scheme and it can be changed to achieve a wide variety of 3D special effects for button objects.

Buttons utilize [host commands](../../2.x/ripscrip/14-host-commands.md) in exactly the same manner as mouse fields do (see above). You can even define hotkeys for a button, allowing the button to be selected when the user chooses a particular keystroke from the keyboard.

Buttons may also have a descriptive text label associated with them. Labels may be placed in a number of orientations around the button. Labels can be visually centered in the middle of the button, displayed above or below it, or even on the right or left sides of the button.

## 3.5.3 Query Expressions

A query expression is similar in concept to a mouse field or a button object, but only loosely. A query expression is designed to instruct the RIPscrip client software to perform some operation at a particular moment in time. In effect, it is similar to an alarm clock that triggers when a certain kind of event occurs. Mouse fields and buttons trigger whenever the user clicks the mouse inside the rectangular area defined for the object. Query expressions go beyond that - you can assign them to other kinds of objects like graphical viewports or text windows (see below), or even instruct the RIPscrip software to process them immediately.

Immediate-mode query expressions are probably the most common. When the RIPscrip software encounters an immediate-mode query expression, the host command portion of the command is immediately processed, executing every option defined in it right after the command is received from the host. This acts like a mouse field that is clicked the very instant that it is received from the host system, except that the query expression doesn't remain defined - it is typically deleted immediately after it's executed. In this manner, query expressions are temporary in nature, unlike mouse fields and buttons which can remain active on the screen for quite some time.

Some query expressions can be defined as "resident" query expressions. A resident query expression is a background operation that remains active until it's turned off, much like mouse fields. For example, an Email editor might define a text window on the screen so that the user can enter text information. You could define a resident query expression over that text window, that would report back to the host which (X,Y) location the user clicked on inside the text window if the user clicks his left mouse button in the text window. This could easily be used to re-position the cursor to a new location, mark text for deletion, or a variety of other operations.

There are also some new resident query expressions that allow certain events to occur if the mouse moves into our out of a mouse/button field. This doesn't mean that the user actually clicks inside the mouse field - but rather, the query expression activates when the mouse moves over a mouse field. These commands are frequently used to change the mouse cursor whenever the mouse moves over a particular region of the screen.

## 3.6 Integrated Graphical and TTY Interfaces

RIPscrip allows you to intermix graphical information with old-style TTY and ANSI information, giving you the best of both worlds. This lets you add graphics to your online system in stages, link-out to another text-based online service from your graphical system, or simply display lists of text information to your users. This is accomplished by mixing graphics inside graphical viewports, with text information shown in text windows.

## 3.6.1 Viewports

A [graphical viewport](../../2.x/ripscrip/02-drawing-ports.md), is a "window" inside which graphics may be drawn. In nearly all circumstances, graphical information can only be drawn inside viewports. Viewports are like a cookie-cutter window - anything that would extend beyond the boundary of the viewport will be clipped, or cut-off at the boundary. Viewports, otherwise known as clipping rectangles, allow you to place graphics at particular locations on the screen, making it so that the graphics cannot extend beyond that designated area.

RIPscrip even allows you to define multiple [drawing "ports"](../../2.x/ripscrip/02-drawing-ports.md). A drawing port is like a virtual canvas or video screen in that you can switch to it, and draw to it as you want. You are allowed up to 36 separate drawing ports, each one may have its own unique viewport drawing sub-area. Drawing ports may be on-screen video drawing ports, or offscreen memory drawing environments. Offscreen drawing ports are frequently used to prepare some graphics data off the screen, then copy them to the screen when they're finished. This gives the appearance of extremely fast drawing of graphics. Other uses of offscreen drawing ports might be in game environments - load all of the icons or images used in a game board onto an offscreen drawing port, and copy them to the screen as needed. This makes it so RIPscrip doesn't have to constantly access the hard disk every time a new image needs to be displayed. In this situation, the image(s) are already loaded in memory.

## 3.6.2 Text Windows

TTY and ANSI information will appear only in [text windows](../../2.x/ripscrip/02-drawing-ports.md). A text window is much like a normal MS-DOS screen, where you see rows and columns of ASCII text information displayed. In RIPscrip, all raw text data is displayed in a customizable text window object. In this manner, you can place a text window anywhere on the screen that you want. Subsequent TTY or ANSI text that is received by the software will be routed (i.e., displayed) to that text window.

For example, you might have a file library system on your online service. When the user chooses the "Display files" option, the host system could open up a text window and display a listing of all of the available files in it. You could still have buttons, display fields and graphical information visible outside the text window. This would give the user the ability to view additional information and even perform many operations while the list of files is being displayed.

You are allowed to have up to 36 separate text windows defined at any one moment in time. Although you can have a large number of them defined simultaneously, only one of them may be active at any particular moment in time. In order to display ANSI or TTY information to a text window that is not the current one, you would need to switch text windows to make the desired text window the current one. Multiple text windows are typically used to implement multi-field database entry forms, where each field is a different text window.

Note, text windows and graphical viewports may overlap.

## 3.7 Supported Image Formats

Not only does RIPscrip support a wide variety of vector graphics commands, but it also supports a number of [bitmapped image formats](../../2.x/ripscrip/06-color-audio-text.md) for the display of various kinds of images. Numerous options are available to control the appearance of images, like options to control aspect ratio, dithering, transparency and color palettes. Currently, two actual bitmapped file formats are supported: BMP and JPEG.

## 3.7.1 BMP

The high-speed bitmapped file format supported by RIPscrip is the device-independent [BMP format](../../2.x/ripscrip/06-color-audio-text.md) used commonly under Microsoft Windows. RIPscrip fully supports the monochrome, 16 color, 256 color and the 24-bit uncompressed variations of the BMP file format. Other features of the bitmap display system are dithering, color palette manipulation, transparency and wallpapering.

BMP files are uncompressed normally, and as such, are very fast to display. RIPscrip does not support compressed versions of BMP or OS/2-style BMP files that have a slightly different internal file format. RIPscrip has the ability to stretch a BMP to any desired size on the screen.

## 3.7.2 JPEG

RIPscrip 2.0 introduced [JPEG image format](../../2.x/ripscrip/06-color-audio-text.md) into the language to enhance the normal, uncompressed BMP file formats used for high-speed image display. While JPEG images are slower to display, the significantly smaller file sizes justify the tradeoffs in performance. JPEG images have an adjustable compression ratio that allows the artist to custom-tailor the compression versus quality issues for a given image by adjusting this compression factor.

JPEG compressed images obtain such a high-degree of compression because it is considered "lossey" compression. Lossey compressed images suffer minor image degradation as a result of the compression process. Under many situations, the loss in quality of the image is barely noticeable (if at all).

All JPEG images are 24-bit in nature, and RIPscrip supports even the GrayPEG variations of JPEG that display gray images (i.e., black and white with gray-scales).

## 3.7.3 GIF

Early on in the development of RIPscrip version 2.0, the Graphics Interchange Format (GIF) was incorporated into the design to supplement the compressed image format JPEG. It was added because JPEG is a "lossey" compression, that loses some image quality for higher compression ratios. GIF on the other hand, got lower compression ratios, but didn't lose any quality as a result of compression. As a result, GIF images decode considerably faster than JPEG does.

Unfortunately, due to outrageous licensing conditions for the compression algorithm LZW used with GIF formats, the technology had to be dropped from RIPscrip 2.0. This leaves only JPEG images available for compressed images.

## 3.7.4 PNG

As a result of the licensing problems that arose in 1995 with GIF, a new file format was born known as Portable Network Graphics (PNG). This format is not based on any patented compression technology and is designed to be freely distributable in all formats with no licensing restrictions of any kind. In addition, it supports all of the same functions and features as did the older GIF format, and even has some extra capabilities built-in that make it a superior technology.

Like GIF, PNG is a "loss-less" image compression method where image quality doesn't suffer from compression like it does with JPEG. What you get back after decompressing an image is exactly the same as the original image. PNG decompression is as fast as GIF is, making it comparable in both performance and features.

TeleGrafix is currently implementing PNG for its next release of the RIPscrip graphics technology. Soon, you will have both JPEG and PNG compressed images.

---

[◀ Prev: Design Goals & Graphical Primitives](04-design-goals-and-primitives.md) · [Contents](README.md) · [Next: Data Tables & Backup System ▶](06-data-tables-and-backup.md)
