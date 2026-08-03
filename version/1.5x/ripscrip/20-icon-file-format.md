# Icon File Format

*Added in RIPscrip v1.54.*

[◀ Prev: Text Variable Creation & Query](19-text-variable-creation.md) · [Contents](README.md)

The following section describes the exact file format specification of the [RIPscrip Icon File](11-images-icons.md#rip_load_icon). It is identical in design to the Borland BGI graphics `putimage()` and `getimage()` buffer format (the same buffer format used by the [RIP_PUT_IMAGE](11-images-icons.md#rip_put_image) and [RIP_GET_IMAGE](11-images-icons.md#rip_get_image) commands).

The beginning of the file contains a header structure at file offset zero. It is exactly four bytes in length. Here is the "C" structure definition for this header:

```c
struct iconfile_header {
       int width;        /* Width  of image in pixels (minus 1) */
       int height;       /* Height of image in lines  (minus 1) */
};
```

After the header follows each scan-line of the bitmap. Each scan-line is segmented into four chunks in 16-color mode. In 256 color mode it is segmented into 8 chunks. All chunks of the scan-line are monochrome bitmaps of a horizontal line. Each chunk is padded to an even 8-pixels (bytes). Each of the scan-line chunks are merged together to create one full-scan-line bitmap.

After the first scan-line's segments follows the next scan-line's four segments (or 8), and so on. After all the scan-lines is 1-byte of "trash" data which is never used and its value is undefined (who knows why?)...

The general format of an iconfile buffer is:

```text
 _______________________________
|                               |
|            HEADER             |  4 bytes
|_______________________________|
|                               |
|  Scan line #1 (bit plane 3)   |
|  Scan line #1 (bit plane 2)   |
|  Scan line #1 (bit plane 1)   |
|  Scan line #1 (bit plane 0)   |
|_______________________________|
|                               |
|  Scan line #2 (bit plane 3)   |
|  Scan line #2 (bit plane 2)   |
|  Scan line #2 (bit plane 1)   |
|  Scan line #2 (bit plane 0)   |
|_______________________________|
|                               |
|  Scan line #n (bit plane 3)   |
|  Scan line #n (bit plane 2)   |
|  Scan line #n (bit plane 1)   |
|  Scan line #n (bit plane 0)   |
|_______________________________|
|                               |
|           TRASH BYTE          |   1 byte
|_______________________________|
```

Example:

If you had a bitmap image (2 scan lines high) containing the following pixel colors:

```text
               < X pos >
           0  1  2  3  4  5
         -------------------
<Y>   0 | 00 01 02 04 08 0F
      1 | 03 05 08 03 02 01
```

IN BINARY:

```text
0000 0001 0010 0100 1000 1111
0011 0101 1000 0011 0010 0001
||| \
|| \ \
| \ \ \
 \ \ \ \ Bit plane #0
  \ \ \
   \ \ \ Bit plane #1
    \ \
     \ \ Bit plane #2
      \
       \ Bit plane #3
```

Breaking this up into bit-planes, you have the following four monochrome patterns (in reverse order):

```text
000011    Bit plane #3   (bit #3 in each pixel)
001000

000101    Bit plane #2   (bit #2 in each pixel)
010000

001001    Bit plane #1   (bit #1 in each pixel)
100110

010001    Bit plane #0   (bit #0 in each pixel)
110101
```

Since these bit patterns are not an even byte in size, pad remaining bits on the right with 0 bits (these are "don't care bits"):

```text
         vv--- don't care bits
A: 00001100 = 0Ch - Bit plane #3               WIDTH  = 6 pixels
B: 00100000 = 20h                              HEIGHT = 2 lines

C: 00010100 = 14h - Bit plane #2
D: 01000000 = 40h

E: 00100100 = 24h - Bit plane #1
F: 10011000 = 98h

G: 01000100 = 44h - Bit plane #0
H: 11010100 = C4h
```

The final structure of the entire iconfile buffer would be as follows in this example:

```text
Offset  VALUE             Ref    Description
==============================================================
  00:   05 00          /* ---    Width  6-1=5               */
  02:   01 00          /* ---    Height 2-1=1               */
  04:   0C             /* (A)    Scan line #0, bit plane #3 */
  05:   14             /* (C)    Scan line #0, bit plane #2 */
  06:   24             /* (E)    Scan line #0, bit plane #1 */
  07:   44             /* (G)    Scan line #0, bit plane #0 */

  08:   20             /* (B)    Scan line #1, bit plane #3 */
  09:   40             /* (D)    Scan line #1, bit plane #2 */
  0A:   98             /* (F)    Scan line #1, bit plane #1 */
  0B:   C4             /* (H)    Scan line #1, bit plane #0 */

  0C:   00             /* ---    TRASH BYTE - UNKNOWN VALUE */
```

---

[◀ Prev: Text Variable Creation & Query](19-text-variable-creation.md) · [Contents](README.md)
