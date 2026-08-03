# Icon/DIB File Format

[◀ Prev: Text Variables: Environment, Clipboard, Screen & Tables](20-text-variables-environment.md) · [Contents](README.md)

*Added in RIPscrip v2.A1.*

The following section describes the exact file format specification of the RIPscrip Icon File.  This format differs from the older RIP v1.54 icon file format which used the Borland getimage() and putimage() formats.  That format was completely inadequate to resolution independent and color independent environments.  With that in mind, we have changed to a Device Independent Bitmap (DIB).  A DIB is a file that can be shown at any resolution, in any color configuration.  There are no actual DIB files out there in the world that conform to the raw DIB format.  The Microsoft Windows BMP format is a DIB though, with a little more header information thrown in to give some added flexibility for future expandability.  In the interests of not inventing a completely new file format, we will be using BMP files for the icon format for future RIPscrip revisions.

## Device Independent Bitmap (DIB) File Format

The DIB file format, originally pioneered by Microsoft for their Windows product provides a device-independent method of storing bitmap data in a universally accessible way.

The file format has a more elaborate header structure than does the RIPscrip Icon file format.  In addition, color palette information is stored in the file so that color approximation or dithering methods may be used to make image appear correct no matter what color palette is in use.

The file format accomodates 1 bit per pixel (monochrome images), 4 bits per pixel (16 color images), 8 bits per pixel (256 color images) and 24 bit images.

## DIB File Header

At the beginning of the file (at offset 0) is the bitmap information header.  Immediately following this is a color table (for all formats except for 24 bit images).  The size of the color table is dependent on the number of bits per pixel and also on some other fields in the header.  After the color table is the actual raw bitmap data.  The format of the bitmap data varies depending on the number of bits per pixel.

The structure of the bitmap header is as follows:

```c
struct BitmapInfoHeader {
     long biSize;                  /* Size of this header (40 bytes)       */
     long biWidth;                 /* Image width  in pixels               */
     long biHeight;                /* Image height in pixels               */
     int  biPlanes;                /* Number of image planes (must be 1)   */
     int  biBitCount;              /* Bits per pixel (1, 4, 8 or 24)       */
     long biCompression;           /* Compression type (1=no compression)  */
     long biSizeImage;             /* Size in bytes of compressed image    */
     long biXPelsPerMeter;         /* Horizontal resolution in pixels/meter*/
     long biYPelsPerMeter;         /* Vertical resolution in pixels/meter  */
     long biClrUsed;               /* Number of colors used                */
     long biClrImportant;          /* Number of "Important" Colors         */
};
```

The biSize parameter is the size of the actual header in the file.  This should be set to 40 for this structure.  In the future, this value may change to accomodate larger headers.  To properly read in the header, you should examine the first four bytes of the header to find out the actual header's size then read in the remaining bytes of the header into your program.

The biWidth and biHeight parameter define the pixel dimensions of the actual image in the raw bitmap data block.

The biPlanes field defines how many "bit planes" are used in the bitmap data.  This value should be set to 1 - meaning one bit plane.

The biBitCount field determines the number of bits per pixel used in this image.  Valid values for this field are 1, 4, 8 and 24.

The biCompression field determines the type of image compression used on this image.  A value of 0 means no compression.  Microsoft defines several nonzero values for the BMP files (basically a DIB with a bit more header information) that use a byte-oriented RLE encoding scheme.  After some research, it was found that no applications actually use this mode and the one vendor that we discovered that supported this form of compression did not do it correctly. As a result of this, we will formally declare that this field should be set to zero to indicate no compression.  In the future, we may alter this for something like internal JPEG data compression, TIFF, GIF, etc.

The biSizeImage indicates the size of the compressed image in bytes.  This is actually the size of the file (including all header information).

The biXPelsPerMeter and biYPelsPerMeter values are used for dots per inch calculations.  The formal definition of these fields are the horizontal and vertical resolution in pixels per meter.  It does not appear that any applications that use DIB formats actually take advantage of these fields, so they could easily be set to zero, or the width and height of the image.

The biClrUsed field defines how many colors out of the maximum available are actually used by this image.  For example, if you have an 8 bit per pixel image (256 colors), but the image only uses 200 colors, you could set this value to 200 to indicate that the color table only has 200 entries in it. Set this field to 0 to indicate that the maximum number of colors are in the color table.  We will set this value to 0 for all RIP bitmaps.

The biClrImportant field is used to specify how many of the colors in the color table are actually important to the image.  For example, if you had a 256 color bitmap, but only 16 colors of the image were critically important to the reproduction of the image, you would set this field to 16 and make sure that the first 16 colors in the color table were those important colors. This makes conversion to a 16 color environment easier.  We formally will set this value to 0 to indicate that all colors are equally important.

## DIB Color Table Format

For 1, 4 and 8 bits per pixel images, a color table immediately follows the bitmap file header.  If the biClrUsed field is set to zero in the header, then the number of entries is 2^bits where "bits" is the number of bits per pixel (1, 4, or 8).  This yields color table sizes of 2, 16 or 256 entries.  If the biClrUsed field is non-zero, then it defines the actual number of color table entries actually present.

Each entry in the color table consists of an RgbQuad structure.  This structure stores information for the Red, Green and Blue components of the corresponding color table entry.  All values are full 8-bit unsigned characters, representing values from 0-255.  If displaying colors in video sub-systems that are not 8 bits for these components, some bit shifting may be necessary to convert to the proper target color system.

Here is the structure definition for the RgbQuad structure:

```c
struct RgbQuad {
     unsigned char rgbBlue;        /* Blue  value for color map entry      */
     unsigned char rgbGreen;       /* Green value for color map entry      */
     unsigned char rgbRed;         /* Red   value for color map entry      */
     unsigned char rgbReserved;    /* Reserved - set to zero               */
};
```

Notice that the values are in reverse order (Blue, Green then Red).  For some obscure reason, this was the way that it was originally designed.  Also note that there is a reserved parameter.  This should be set to zero for future compatibility.

For 24 bit images, there is no color table.  Color table values are actually stored in the raw bitmap data block itself (see below).

## DIB Bitmap Data Block Format

The format for the bitmap data block varies depending on the number of bits per pixel actually defined in the header.  The reason for this is optimal data storage without compression.

All bitmap image data blocks are stored in horizontal rows of data.  Each row is padded to an even four byte boundary with zeros.  The first row of raw data is actually the bottom-most scan line of the bitmap.  Every subsequent row is progressively closer to the top of the bitmap in one line increments.

The four storage methods are described in the next sub-sections.

**1 BIT PER PIXEL**

Information is stored as one pixel per bit.  The high-order bit in a byte is the left-most pixel in a group of eight pixels.  For bits that represent pixels beyond the right side of the image, zero bits are used as padding.

For example, if you had the following pixel values:

```text
1 0 0 1 1 1 0 1 1 0 1 0 1 1 1 1 0 1 1 1
```

They would be encoded as the following:

```text
  In Binary: 10011101 10101111 01110000
Hexadecimal: 9D AF 70 00
```

Note how four zero bits are added to the last byte, and another zero byte is added to pad the row out to an even four-byte boundary.  If image data stops exactly on a four byte boundary, no padding is necessary.

**4 BITS PER PIXEL**

Information is stored as two pixels per byte.  Each pixel's color value is stored in its own nibble inside the byte.  The left-most pixel is stored in the high-order nibble, and the right-most pixel is stored in the low-order nibble.

For example, if you had the following pixel values:

```text
 In decimal:  12 0 7 8 4 15 11 10 9
Hexadecimal:  C  0 7 8 4 F  B  A  9
```

This would result in the following 4 bit encoding values:

```text
Hexadecimal:  C0 78 4F BA 90 00 00 00
```

Note that an extra 0 nibble was added to the last valid dat byte and three zero bytes were added to round the row out to an even four byte boundary.

**8 BITS PER PIXEL**

This is the most straightforward encoding scheme.  Each pixel is stored as exactly one pixel per byte.  All eight bits of the byte are used to store the pixel color values.  No bit shifting or color encoding is necessary.

The above example (4 bits per pixel) would be encoded as follows:

```text
Hexadecimal: 0C 00 07 08 04 0F 0B 0A 09 00 00 00
```

Notice that three extra zero bytes were added to round the row out to an even four byte boundary.

**24 BITS PER PIXEL**

This format does not use a color table.  The actual RGB values used for colors is actually stored in the raw bitmap data blocks.  Each pixel's worth of data in the data block consists of three bytes.  The first byte represents the BLUE component, then the GREEN component, then the RED component in the third byte.  Each one of these values can range from 0 to 255.  Some bit shifting may be necessary depending on your target video sub-system.  Again, every row of information is padded to an even four byte boundary like all the above methods do.

## DIB Miscellaneous Notes

Any of the fields in the header that are LONGs or INTs use Intel byte ordering of its values.  For IBM-PC based software, this should pose no problems at all.  On Motorola based processors though, the data in these fields will have to have some bit shifting done on them to ensure that they are represented correctly.  In no cases will these fields in the header represent negative values.

---

[◀ Prev: Text Variables: Environment, Clipboard, Screen & Tables](20-text-variables-environment.md) · [Contents](README.md)
