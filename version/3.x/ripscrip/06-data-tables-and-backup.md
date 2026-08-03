# Data Tables & Backup System

[◀ Prev: User Interface & Display](05-interface-and-display.md) · [Contents](README.md) · [Next: Host Command Language ▶](07-host-command-language.md)

## 3.8 Data Tables

A fundamental concept in RIPscrip 3.0 is the idea of [data tables](../../2.x/ripscrip/03-data-tables.md). A data table is a list of data objects. There are a eight different types of data tables in RIPscrip, each serving a unique purpose. To the left, is a listing of the available data tables:

| Port Type | Entries |
| --- | --- |
| Graphics style | 36 |
| Button style | 36 |
| Drawing port | 36 |
| Text window | 36 |
| Color palette | 36 |
| Environment | 36 |
| Mouse field | 128 |
| Graphics screen | 1 |

One such data table, the graphics style table, is for the storage of drawing attribute information. Parts of a graphics style are the settings for current drawing color, fill pattern, font style, raster-op, line pattern and more. In effect, it stores all of the graphical drawing attributes, or the "style" for graphics drawing in one central location.

The mouse field and graphics screen data tables in the preceding list are special and require further discussion. The mouse field data table is a collection of up to 128 separate mouse field definitions, where each mouse field is considered an individual data table entry. There is no such thing as a "current mouse field", and as such, there is no current Mouse Field data table entry. You can't switch from one mouse field to another either.

The other special data table is the graphics screen data table. The graphics screen data table is not actually a real data table. You cannot switch from one graphics screen to another like you can with other data table entries, because you only have one computer monitor, and as such, only one graphics screen. The graphics screen is formally defined as a data table so that it can interface with the data backup system (see below).

## 3.8.1 Switching Data Tables - Context Swapping

As you may notice, most of the data tables have more than one entry, allowing you to have multiple defined data table entries simultaneously. Using these multiple entries, you can quickly move from one drawing environment to another with a simple data table "switch context" command. There are commands to switch from one data table to another for each type of table in the preceding list (e.g., [`RIP_SWITCH_STYLE`](../../2.x/ripscrip/12-level-2-commands.md), [`RIP_SWITCH_TEXT_WINDOW`](../../2.x/ripscrip/12-level-2-commands.md), etc.).

Switching data tables can be extremely useful in the efficient use of transmission speeds. For example, a graphics style data table could be used to store different kinds of drawing environments for different menus, and all you need to do when you go to another menu is switch to the desired graphics style - you don't need to re-transmit all of the RIPscrip commands to configure all of the correct drawing attributes before you actually start drawing. This can result in substantial space savings in RIPscrip data files.

## 3.8.2 Protected Data Tables

Having data tables at your disposal has quite a bit of usefulness in the overall design of efficient RIPscrip content. However, for the service provider who is designing a large-scale online service, using data table entries effectively requires the ability of defining data tables that can stay defined permanently throughout the duration of a user's online session.

This is where "protection" comes into play. Each data table entry can be protected, so that it cannot be deleted or altered by some kind of environment reset operation. This is perfectly suited for an online service that needs to use the same data table configurations throughout the service, and only needs to define them once. By protecting the entries immediately after they're defined, you are guarding them, making them so that they will remain defined until you want them removed.

Nearly all data table entries can be protected except for the first one, entry number 0. Entry 0 in every data table is always considered a "scratch pad" entry, and is always modifiable. This ensures that you will always have a valid, usable entry that you can change to suit your needs. In this manner, entry 0 is typically referred to as the "common table entry".

It should be noted that graphics screen data tables and mouse field data tables cannot be protected.

## 3.9 Data Backup System

Data tables, by themselves, are quite powerful in nature. But in larger scale online services, where many areas of the system may be made by different manufacturers (as in the case of BBS systems), the operator of the service doesn't necessarily have complete control over each module. This can cause problems with data tables that you may have defined.

For example, let's say you defined all of your graphics style data table entries the way that you need for your basic service, then your customer links from your system to another server. In this situation, the other server could entirely redefine the graphics styles in RIPscrip, overwriting yours. When the customer returns to your system, the graphics styles aren't defined to what you expected them to be.

This is where the RIPscrip [data backup system](../../2.x/ripscrip/04-data-backup-areas.md) comes in. The data backup system allows you to effectively "backup" an entire data table to some alternate storage area, making a "safe copy" of it that you can restore from. It is much like making a tape backup of a hard disk. You make backup copies of your data before you go to an area that will change them, then restore the information when you need it.

There are multiple data backup areas - one for each type of data table. This is why the graphics screen and mouse fields are formally defined as data tables - so that they can be backed up. Each data backup area (for each type of data table), has multiple storage sub-areas. This allows you to have multiple copies of each data table stored separately. Using data backup areas effectively can open up an enormous amount of flexibility when it comes to working with online software packages.

The data table system can be visually broken down into three distinct areas: The Base Data Save Area, the Data Save Slots and the Stack Save Area. Visually, it can be described as follows:

*[Editor's note: The original document included a diagram of the data backup system areas here; the image was lost in the HTML-to-text conversion of this white paper. An equivalent diagram appears in the RIPscrip 2.x edition under [Data Backup Areas](../../2.x/ripscrip/04-data-backup-areas.md).]*

## 3.9.1 Base Data Save Area

The [Base Save Area](../../2.x/ripscrip/04-data-backup-areas.md) is perhaps one of the more commonly used save areas. It is a common "scratch pad" storage area, designated for temporary storage only. It cannot be protected, and should not be used to hold long-term backup information. It can hold one entire data table, and can hold it indefinitely (until deleted, or overwritten).

## 3.9.2 Data Save Slots

The [Data Save Slots](../../2.x/ripscrip/04-data-backup-areas.md) are one of the more useful, and most flexible storage mechanisms in a data backup area. You have up to 10 separate slots available to you, each one of which can store an entire data table. You access Save Slots by slot number (0-9), and these slots can be protected - this means that can put some information in a backup slot and make sure that it can't be overwritten or deleted by some other application!

Unlike the Base Area, Save Slots do not hold their information indefinitely. Save Slots are deleted (i.e., cleared) of their data immediately after you restore from them (unless they're protected). In this manner, the data backup system automatically cleans up after itself when you restore information.

You may use up to 10 of the Save Slots, but keep in mind that the 10 Save Slots are "shared" with the Stack Save Area (see below), so you may not have 10 actual slots to work with - some of them might be in use by the stack!

## 3.9.3 Stack Save Area

The last and most useful area of the data backup system is the [Stack Save Area](../../2.x/ripscrip/04-data-backup-areas.md). The Stack Area "borrows" slots from the Save Slot system when information is stored on the stack, and those slots are returned when a data table is removed from the top of the stack. In this manner, the stack can be considered a chronological storage mechanism. When you store a data table onto the stack, you are "pushing" it onto the stack, making any other data tables already on the stack appear below the one you're storing. When you restore from the stack (i.e., "pop"), you are removing the top-most data table from the stack. Subsequent restore operations from the stack will remove the top entries, thus reducing the size of the stack by one with each restore operation.

The Save Stack Area is ideally suited for an overlapping dialog box/windowing system. If you want to display a dialog box on the screen, overlaying an existing menu, simply push all of the data tables in use onto the stack, then when you want to close the dialog box (and return to the previous menu), you pop the data tables off of the stack, thus making the old settings active again. The same method can also apply to the menu that was in the background - there could have been another menu behind that one which could have been stored on the stack, which can be restored just as easily.

Since the Save Stack borrows "slots" from the Save Slot system, you can only have a maximum of 10 data tables stored on the stack at any one point in time (assuming that no slots are actually in use in the Save Slot system).

---

[◀ Prev: User Interface & Display](05-interface-and-display.md) · [Contents](README.md) · [Next: Host Command Language ▶](07-host-command-language.md)
