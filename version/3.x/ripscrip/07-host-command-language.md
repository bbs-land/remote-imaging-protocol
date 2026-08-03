# Host Command Language

[◀ Prev: Data Tables & Backup System](06-data-tables-and-backup.md) · [Contents](README.md) · [Next: Future Goals & Conclusion ▶](08-future-goals-and-conclusion.md)

## 3.10 Host Command Language

Up until now, we've been covering the basic architecture of RIPscrip in general - discussing the types of commands available, data areas, and structure of the language. In this section, we'll discuss another, extremely powerful area of RIPscrip: the [Host Command Language](../../2.x/ripscrip/14-host-commands.md).

The Host Command Language (HCL) is used in mouse fields, buttons and query expressions to control what information is sent to the host, and also to control different kinds of processing that may occur. In fact, some areas of the HCL may even be used in other RIPscrip commands like graphical text operations, loading bitmap files, etc., but the descriptions of their usefulness goes beyond the scope of this document.

Normally a host command contains raw text that is to be sent to the host system. This information is usually in the form of a command that the host understands, but can also be just about anything you want. Using raw commands like this makes up the bulk of RIPscrip host commands. In order to take accommodate the needs of most host systems, some portions of the HCL will inevitably be needed. For example, to go into Email on a particular BBS, you might have to send the letter `E` followed by a carriage return. The `E` would be considered the raw host command, but the carriage return would have to be handled by a control character directive in the HCL (e.g., `^M`).

There are five different areas of the HCL: control characters, pop-up picklists, local playback directives, template directives and text variables. Each of these are distinctly different in purpose, but when used in combination, give you a great deal of flexibility in whatever you want to do in a host command.

## 3.10.1 Control Characters

[Control characters](../../2.x/ripscrip/14-host-commands.md) are perhaps one of the most basic aspects of the HCL. In fact, they are the most commonly used feature of the HCL as well. With control characters, you can insert a code into a host command that, when processed, will be converted into a single-byte ASCII control character sequence (e.g., ASCII values 0-31). You specify control characters as a caret symbol (`^`) followed by the character code designated for the control character sequence. For example, `^M` is used for carriage return and `^G` is used for a beep. You may also use the backquote (`` ` ``) character instead of the caret if your host software uses the caret for its own purposes.

## 3.10.2 Pop-Up Picklists

[Pop-up picklists](../../2.x/ripscrip/15-local-playback-popup-lists.md) are a very useful way of presenting the user with a list of choices. Typically used in a host command of a mouse field or button, picklists are used to display a listing of choices to the user. When the user makes a choice, the result of that choice is inserted into the host command in place of the picklist command itself.

For example, the host command `order ((apples,oranges,cherries))` will send the sequence `order ` to the host system, then display a listing of three different kinds of fruits for the user to choose. Once the user chooses one of them, the appropriate value is transmitted to the host system. If the user chose "apples", then the sequence `order apples` will be sent to the host system.

There are a number of formatting options and extra features available for picklists to control the placement, prompt, and display of each item of the list, giving you considerable flexibility in how your list should be displayed.

## 3.10.3 Local File Playback Directives

Another commonly used feature of the HCL are [local file playback directives](../../2.x/ripscrip/15-local-playback-popup-lists.md). A local playback directive instructs the RIPscrip software to process a file located on its local hard disk in a client/server fashion. An example of this might be to play the file `BEEP.WAV` when the user clicks on a particular button. The file `BEEP.WAV` would have to be located on the user's hard disk for the option to work.

There are four basic playback directives: display a JPEG image, play an audio file, display a RIPscrip file, and show a BMP image. Using local playback directives, you can make quite a number of things happen directly from inside a mouse field or button command, activated when the user clicks on the object. In fact, it is entirely possible that local playback directives be used to simulate menus on an online service locally, thus making it so that you don't have to transmit hardly anything over the modem (if at all). Many software packages use this feature extensively to provide the customer with lightning fast menu displays at the expense of a little bit of disk space on the user's hard disk.

## 3.10.4 Template Directives

[Template directives](../../2.x/ripscrip/16-templates.md) constitute perhaps one of the more complex areas of the HCL, but also one of the most powerful. Templates are used in the RIPscrip HCL as a way of "building" more complex host commands based on specific conditions. In this manner, you can think of templates as a cookie-cutter approach to host commands, using templates to construct host commands based on which buttons are selected and which ones are not.

Typically, templates are used with radio buttons and checkbox buttons which have a distinct on/off status. When one of these buttons is turned on, it defines a template sequence. That template sequence is used by some other host command to "import" the currently defined template value.

Taking the fruit ordering example described previously, you could make the selection of fruits a list of three radio buttons instead of a picklist. Each radio button, when clicked, will define a particular template number to a specific text sequence (e.g., defines template #1 to the values "apples", "oranges" or "cherries"). Depending on which radio button is clicked by the user, the selected fruit can be referenced in an "order" button by the host command sequence `order $?1$`. When the user clicks on the order button, it sends `order ` to the host system, then sends template number 1 to the host system, whichever value it happens to be set to at the moment (e.g., apples, oranges or cherries).

Templates can perform many complex tasks well beyond the one described in this example. The methods that templates can be defined are diverse, and the ways that they can be called upon to perform operations are equally diverse.

## 3.10.5 Text Variables

Probably the most common use of the HCL in RIPscrip, is the usage of [text variables](../../2.x/ripscrip/17-text-variables-general.md). Text variables are, as the name implies, a variable containing some piece of text information. In actuality, text variables are much more extensive than simple data variables. Some text variables can store a piece of text information, whereas other text variables don't store any text information, but rather perform some kind of action or processing.

There are two distinctly different kinds of text variables in RIPscrip. There are user-defined text variables, which are designed to store user-defined text information that the user (or the host system) specifies. The other kind of text variables are pre-defined RIPscrip text variables. Pre-defined variables typically store a particular type of text information, or perform a specific action that the host specifies.

### 3.10.5.1 User-Defined Text Variables

User-defined text variables are variables that store a piece of information that the host system, or the user specifies. There are two types of user-defined text variables: memory variables or database variables.

Memory text variables are temporary in nature, and store the designated information until the RIPscrip software is shutdown, they are deleted or overwritten with new information.

Database text variables are essentially permanent. They are stored in a database file on the user's hard disk and can be retrieved at any time. Database variables can be deleted or overwritten just like memory variables can.

User-defined text variables can be used for a variety of purposes. A typical example of a user variable might be `$FIRST_NAME$` which contains the customer's first name. Another application of user variables could be address information, allowing for automated signup to new online services.

User-defined variables can also be created by the host system, without the user's intervention. These variables are defined "transparently" to the user, and are typically used by the host system for various configuration information that needs to be stored on the client terminal. An example of this is the RIPmaster software package running under TBBS which can be installed on many different online services. This package implements a complete RIPscrip 3.0 multimedia presentation system for your BBS, and as such, uses a great many files on the user's local hard disk. To distinguish the files of one online service from another, a user-defined text variable named `$EXTENSION$` is used to define a file extension for the system (e.g., `.TG` for TeleGrafix's BBS), and whenever a filename parameter in RIPscrip is entered, the text variable `$EXTENSION$` is used where the file's extension would normally appear, forcing RIPscrip to lookup the variable and replace it with `.TG`. In this example, the sequence `FILENAME$EXTENSION$` becomes `FILENAME.TG`.

### 3.10.5.2 Pre-Defined Text Variables

Pre-defined text variables in RIPscrip are a very powerful, very broad subject. Essentially there are two types of pre-defined text variables: data variables, and action variables. There are over 160 different pre-defined text variables in RIPscrip 3.0, some of which are data variables, and others are action variables.

Data variables are basic text variables that contain a specific piece of information. An example of one of these variables is `$DATE$` which inserts the current date into the host command (e.g., "03/07/96"). Other variables return configuration information about various aspects of RIPscrip in the form of text information (e.g., what is the current graphics style number, etc.).

Action text variables perform some kind of action on the RIPscrip software package. An example of this might be the `$CLS$` text variable which clears the screen. There are a great many action variables in RIPscrip which can do things ranging from simple beeps and sounds, to complex data copying operations. Just about all operations with switching data table entries and data backup slots can be handled efficiently with action variables. Effective use of RIPscrip to provide some kind of GUI interface to the user can only be accomplished with the use of action text variables.

Some pre-defined text variables are a combination of both data variables and action variables. For example, the text variable sequence `$CUR(TW)$` by itself, reports the current Text Window data table entry number as a number from 0-35. In this configuration, this text variable acts like a data text variable, inserting a piece of text into the host command. The sequence `$CUR(TW,5)$` on the other hand, changes the current text window to text window number 5. In this example, this text variable performs an action and hence, is an action variable. When a variable performs an action, it never contains any data and always evaluates to nothing (e.g., the sequence `Hello $CUR(TW,5)$world` would send `Hello ` to the host system, switch to text window number 5, then send `world` to the host system.

## 3.11 Putting It All Together

With RIPscrip 3.0, you can use as little or as much of the language in the creation of your online service as you want. At the very minimum, you might opt to use just the drawing capabilities to show nice pictures and images to your customers. On the other side, you could build a full-fledged graphical online service, utilizing the full range of RIPscrip's capabilities to present a robust, feature-rich service similar to the largest online services. With RIPscrip, the capability is there to do just about whatever you want. The more you want to do though, the more aspects of RIPscrip you'll naturally need to use.

For example, if you want to present dialog boxes and menus which can overlap each other, that can be removed with a click of a button, you'll need to get a pretty good grasp of the basics of action text variables and the [data backup system](../../2.x/ripscrip/04-data-backup-areas.md).

To make your online connection as fast as possible, you would need to take advantage of [data tables](../../2.x/ripscrip/03-data-tables.md) and [local playback directives](../../2.x/ripscrip/15-local-playback-popup-lists.md) to create a truly client/server architecture.

If you wish to display high-quality documents like those you would normally find in magazines or newspapers, then you'd need to familiarize yourself with the capabilities of the font systems of RIPscrip, and the multi-column system for extensive layout control.

---

[◀ Prev: Data Tables & Backup System](06-data-tables-and-backup.md) · [Contents](README.md) · [Next: Future Goals & Conclusion ▶](08-future-goals-and-conclusion.md)
