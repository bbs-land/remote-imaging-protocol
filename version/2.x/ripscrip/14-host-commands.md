# Host Commands & Text Variable Basics

[← Level-3 & Level-9 Commands](13-level-3-9-commands.md) · [Contents](README.md) · [Local File Playback & Pop-Up Lists →](15-local-playback-popup-lists.md)

---

With Mouse regions, Buttons and Text Variable Query ability, you can control the Terminal/Paint programs and how they react with the BBS in many ways. To accomplish this, there are several features of RIPscrip that permit you to do special actions based on different circumstances. In effect, an "action language" of sorts. The following sections go into the available "action language" features in more detail.

Among the various abilities are:

- Control-Character specification
- Pre-defined Text Variables & User-defined text variables
- [Pop-up pick-lists](15-local-playback-popup-lists.md)
- Query text variable contents (pre-defined & user variables)
- A Host Command ["Template" system](16-templates.md) for added intelligence.

## Control Characters

Not all BBS'es will allow you to use control characters on their Service. Regardless of that, the capability to send any Control Character exists for your Host Commands. The most commonly used Control Characters are:

**Individual Control Characters**

| Character | Meaning                    |
| --------- | -------------------------- |
| `^@`      | Null (ASCII 0)             |
| `^G`      | Beep                       |
| `^L`      | Clear Screen (Top of Form) |
| `^M`      | Carriage Return            |
| `^C`      | Break (sometimes)          |
| `^H`      | Backspace                  |
| `^[`      | Escape character           |
| `^S`      | Pause data transmission    |
| `^Q`      | Resume data transmission   |

**Special Keystrokes**

| Sequence | Meaning      |
| -------- | ------------ |
| `^[[A`   | Up Arrow     |
| `^[[B`   | Down Arrow   |
| `^[[C`   | Right Arrow  |
| `^[[D`   | Left Arrow   |
| `^[[H`   | Home Key     |
| `^[[K`   | End Key      |
| `^[[L`   | Control Home |

Some hosts use the `^` (caret) for their own purposes. In these cases, you can use the `` ` `` (backquote) character instead of the caret. Some systems allow you to specify the caret symbol as two carets (`^^`). Consult your Host Software documentation to determine the best method for your needs.

> **NOTE:** RIPterm uses `^` or `` ` `` and a character to represent a control character. IT IS NOT A CONTROL CHARACTER BY ITSELF, IT IS TRANSLATED BY RIPterm. In other words, `^M` does not send a `^` and then an `M`, it sends a carriage return (ASCII 13). Likewise, RIPscrip commands like [Query](11-level-1-commands.md#rip_query) do not use an `^[`, an actual escape character (ASCII 27) is used.

## Text Variables

A special feature of RIPscrip allows it to understand what a Text Variable is. A text variable is a piece of text that both RIPaint and RIPterm know something about. For example, the Text Variable `$DATE$` is known to represent the current Date on your PC. The host may ask your system what the values of one or more of these variables are, and if your terminal knows these particular Text Variables, it will tell the host.

There are three types of Text Variables.

- Built-In Text Variables that RIPscrip products will ALWAYS know about. These include Text Variables like date and time that return a value

- Another type of built-in Text Variables are Active Text Variables, which perform an action, but return nothing to the host. These include turning the status bar on/off, clearing the graphics screen, and playing some simple sounds, and many more. These variables are a very powerful aspect of RIPscrip, providing mechanisms for doing dialog boxes and interactive GUI applications. _(v2.A1)_

- Then there are also User Text Variables that can contain a variety of information depending on what the user entered at the time the variable was created. For example, the host might ask you what the contents of the `$FULL_NAME$` variable is, and if RIPterm doesn't know, it could pop-up a field on the screen and ask you about it. From then on, RIPterm will remember that piece of information for the next time it is needed by a host.

You may use either the pre-defined Text Variables, or the User Text Variables at any place that allows Text Variables.

Some built-in text variables have been extended in RIPscrip 2.0 to allow for parameters. This extends text variables functionality in many ways. If a text variable has a parameter, it is enclosed in paranthesis immediately after the text variable name as in the following example: _(v2.A1)_

```text
$SAVE(8)$
```

This would be basically identical in nature to the older `$SAVE8$` text variable which saves the screen to the eighth slot. The new method is more universal in design than having separate text variables for basically identical operations. The older forms of these commands will remain in the specification but their use is not recommended because the new method is far superior. _(v2.A1)_

Any text variables which take parameters don't necessarily need them. In any case, these text variables will have their parameters described in the appropiate sections below. _(v2.A1)_

If a variable takes more than one parameter, then they are separated by commas between the paranthesis (eg, `$ETW(0,1)$`). _(v2.A1)_

A [complete listing of all pre-defined text variables](17-text-variables-general.md) (both data and active) is near the end of this document. _(v2.A3)_

## Text Variable Creation and Query

As previously mentioned, Text Variables were described as either pre-defined variables, or as User Variables. Pre-defined variables are variables that RIPscrip products know things about "out of the box". They will always know what the variables mean, from the day you install the software. User Variables are variables that the user of RIPscrip products defines, and teaches the software new things it doesn't already know.

### Pre-Defined Text Variables

A pre-defined text variable is either a data text variable, or an active text variable. A data text variable is a text variable that inserts a piece of text wherever the text variable is used. For example, the sequence `$DATE$` might get replaced with 09/19/94. This is a simple example of a data text variable. An active text variable on the other hand does something (usually). They normally don't get replaced with other text information. For example, the text variable `$SAVE$` saves the contents of the screen to a disk file that can later be restored with `$RESTORE$`. In these situations, active text variables are removed from whatever text message they are present in (they are still activated though).

### What Are User Variables?

A User Variable is a Text Variable that RIPscrip doesn't know exists. They are custom-defined text variables that contain information that the terminal user will fill in. If a variable already contains information, a host will be automatically told (if told to do so) what that variable contains without the user having to intervene (i.e., transparent information exchange).

Examples of Text Variables might be:

| Variable         | Question                            |
| ---------------- | ----------------------------------- |
| `$FULL_NAME$`    | What is your full name?             |
| `$COMPANY_NAME$` | What company do you work for?       |
| `$AGE$`          | How old are you?                    |
| `$DATEOFBIRTH$`  | What is your Date of Birth?         |
| `$PHONENUMBER$`  | What is your Day-time phone number? |

User Variables will "keep track" of these responses for you, in the terminal program database. You can tell the terminal to store these values permanently, or they may be active only during the current session, or they may be defined as temporary where they are not stored for more than a brief moment.

> **NOTE:** This ability is configurable so that information exchange can be either interactive, or automatic. Automatic transfer of information does NOT prompt the user with the information unless the variable has not yet been defined. If it has not been defined, a pop-up question will appear asking the user a particular question, thus defining the text variable.

If the exchange is interactive, the data is displayed in a pop-up editor box, asking you if the information is correct. If it is, press ENTER and the retrieved information is sent to the host for you. If it is not correct, or it has not been created yet, just type it in and press ENTER and it will be saved automatically, and sent to the host all at once.

### How Can User Variables Be Important?

Lets take an example. You are the system operator of a large RIPscrip host. As you have read, RIPscrip can take advantage of database-like ability on the terminal end. If you can alter your host to ask questions with RIPscrip Text Variables built in, you can have the terminal calling your host automatically fill in questionnaires. Imagine if a user could sign-up on your host without having to type more than a single keystroke (i.e., "YES, this information is correct"). With User Text Variables, you can do this very thing.

### Creating User Variables

There are two ways of defining User Text Variables in RIPscrip. You can use either the [Define Text Variable command](11-level-1-commands.md#rip_define), or you can use Text Variable Queries, as described in the next section.

### Defining Text Variables

The RIPscrip command Define Text Variable is by definition, an interactive command with the user. The RIPscrip command will attempt to define a User Variable. This variable is some piece of information that the system operator deems important. You may specify a question, a default response, and how many characters long the response may be.

Once the terminal has received a define command, the terminal pops up an appropriate question box on the user's screen, asking him the desired question that should be saved to a particular Text Variable. If you did not specify a question, a default question is used (i.e., "Enter <name of text variable>").

Once the user has entered his response, it is recorded and saved. How long it is stored depends on what the host told the terminal. The host can tell the terminal "save this on your hard disk forever". The host may also tell the terminal "don't save this to disk, but remember this value until you exit RIPterm". You also have the option of saying "don't remember this value at all, just pop up a question, and send the value to me NOW" - i.e., don't save it at all, just enter it and send it to the host).

### Querying Text Variables

Now that you know how to define information on the terminal, you need to know the last method of asking the terminal about text variables. This feature is called "Data Query". Data Query is a generic query command that can ask the terminal one or more questions, and tell it how to transmit the information back to the host. This command is for use in non-button situations where you do not want to wait until the user clicks on a button to get your data back.

[Data Query](11-level-1-commands.md#rip_query) is a special RIPscrip command that can be used to ask the contents of one or more Text Variables.

### Examples of Text Variable Query

Lets take a simple example. You wanted to ask the terminal program some address information. You could do this with the following query (remember, the query also tells the terminal HOW to send the data back to the host):

```text
$FULL_NAME$^m$COMPANY$^m$ST_ADDR$^m$CITY$, $STATE$ $ZIP^m
```

This would query the terminal the contents of 6 text variables, and format them in a manner similar to any normal address on an envelope. The results of this query might send the following back to the host :

```text
Joe Sixpack
ACME Corporation
13631 Palindrome Parkway
Surf City, CA 92649
```

If a text variable is queried, and it has not been defined yet, a pop-up question will appear asking the user to fill in the information.

### Defining Permanent Variables

Under normal situations you reference text variables by simply placing dollar signs (`$`) around the variable name. Anywhere where that text variable occurs in a Host Command, Query, or other related place that can contain text variables will have its contents replaced with the associated information. A RIPscrip command exists in the specification to define text variables for permanent storage. Since this is an actual RIPscrip command, it is not well suited for using in a Host Command directly. In fact, you couldn't use that in the Host Command's return string definition.

If you reference a Text Variable in a Host Command that hasn't been defined yet (eg, `$FIRST_NAME$`), then the user will be presented with a pop-up dialog box asking them to:

```text
Please enter FIRST_NAME:
```

When the user enters some information, this data is inserted where the `$FIRST_NAME$` variable was located, and the contents of that variable are lost after that moment. What is truly needed is an ability to preserve that information (either on Disk, or in memory) so that it can be used later on. To accomplish this, we will form some variations on the text variable syntax.

There are six basically different text variable references, each of them take a single command character added between the first dollar sign and the beginning of the variable name. Those characters and their significance is:

- `*` ... An answer is required
- `+` ... Save variable to database permanently
- `=` ... Save to internal memory table (lost when RIPterm hangs up)
- `#` ... Do not echo keystrokes (show #'s instead). This is useful for things like entering passwords.
- `-` ... Used in conjunction with a default response (see below). When this option is used, the value of the variable is set to the default value and the user is not prompted for any data entry (transparent data variable define). Nothing is returned to the host in this mode (unless transparent retrieval mode is used - see below). _(v2.A1)_
- `&` ... Transparent data variable retrieval. This allows the host to retrieve a text variable from the terminal and the user is not prompted to modify the information. _(v2.A1)_

> **DEVELOPER NOTE:** For database variables that are retrieved you might want to implement a "data security" option in your software that overides this "transparent" mode of operation for permanent variables. For temporary variables (in memory) though, the system should be allowed to retrieve them transparently since it created them itself. _(v2.A1)_

If you do not specify the "+" or the "=" directives to actually save the text variable's contents, then the data will be passed on as part of whatever host command the text variable expression was a part of, and will not be saved. To save the text variable permanently, you must specify the "+" command which will store the variable in some kind of internal database file for permanent storage. If you wish to only save the variable temporarily (eg, for the duration of the current session), use the "=" directive instead - this saves the variable in an internal memory table. _(v2.A4)_

To ask for the FIRST_NAME text variable that must be filled in, and to instruct RIPterm to save the variable to the local database, you would use the following text variable command syntax:

```text
$*+FIRST_NAME$
```

The four command characters (`*`, `+`, `=` and `#`) can be in any order, but can only appear once in the text variable statement - additional occurences of them are ignored.

You may specify how wide the data entry field for the text variable is. To do this, simply put the number of columns after the variable name with a comma (,) in between (eg, `$NAME,10$`). _(v2.A1)_

As you may have noticed earlier, if a text variable is referenced without being previously defined, it will display a generic question to prompt the user. You have the option to specify a custom question. The syntax is similar in nature to the syntax of the host command/text labels of the pop-up pick lists described below. In order to prompt with a particular question, after the variable name place an at-sign (`@`) followed by the question, then the final dollar sign as in the following example:

```text
$FIRST_NAME,20@What's your first name?$
```

In the question text, you are not allowed to use dollar signs at all.

You also have the ability to X/Y location of the pop-up window that asks for the text variable. This gives you control over the location of the window. The way you do this is by adding some coordinate information before the variable name followed by a colon. An example of this would be as follows: _(v2.A1)_

```text
$10,10:FIRST_NAME,20@What's your first name?$
```

These coordinates are specified in normal decimal format. If one or either of the number are omitted then the dialog is centered either horizontally, vertically or both. Some examples of this are as follows: _(v2.A1)_

| Example      | Result                                    |
| ------------ | ----------------------------------------- |
| `$,50:NAME$` | Centered horizontally                     |
| `$50,:NAME$` | Centered vertically                       |
| `$,:NAME$`   | Centered both horizontally and vertically |

If you omit the X/Y specification codes entirely (eg, `$NAME$`), then the actual location of the popup dialog is up to the discretion of the RIPscrip software. _(v2.A1)_

X/Y coordinates are in current "world coordinates". _(v2.A4)_

You may also supply a default value for the text variable. This default value is used if the variable doesn't already exist. When the default value is used it is displayed in the data field's edit region and you have the option of changing it. _(v2.A1)_

To supply a default response, you specify the default contents after an equal sign (`=`) which must be placed after the variable name (and width parameter and question parameter if applicable). Some examples are: _(v2.A1)_

```text
$STATE=Ca$
$STATE@What state do you live in?=Ca$
```

User defined text variables do not always require a specific response (unless the "*" flag is specified indicating that a response is required). If the user chooses to ignore the request (ie, hitting CANCEL or whatever), then a value of "NONE" is inserted in place of the user data text variable. _(v2.A4)_

Some discussion needs to be made about the six various text variable referencing modes described earlier and how they interact. The two modes "=" and "+" are used to actually store (preserve) the text variable's contents for a period of time (how long depends on if its a permanent variable or a memory variable). If either of these modes are both present, then it is considered to be a permanent variable.

The transparent data define mode ("-") and transparent data retrieval mode (&) are used by the host to interact with the user without having to be prompt the user for any data (eg, the operation is "transparent" to the user).

There are four basic possibilities with transparent modes:

1. **Transparent Define and Retrieve modes are set.** One of the following situations will occur in this mode:

   - a\) Default response specified and variable exists. The text variable is redefined with its contents set to the default response.
   - b\) Default response not specified and variable exists. The user is prompted to see if he wants to change the contents of the text variable as it stands.
   - c\) Default response specified and variable doesn't exist. The variable is defined using the default response.
   - d\) Default response not specified and variable doesn't exist. The user is prompted to enter a value to define the text variable.

   After the variable is defined (if the user didn't say to skip this operation - ie, he was prompted), then it is sent to the host system.

2. **Transparent Define is set but retrieaval isn't set.** One of the following situations will occur:

   - a\) Default response specified and variable exists. The text variable is redefined with its contents set to the default response.
   - b\) Default response not specified and variable exists. The user is prompted to see if he wants to change the contents of the text variable as it stands.
   - c\) Default response specified and variable doesn't exist. The variable is defined using the default response.
   - d\) Default response not specified and variable doesn't exist. The user is prompted to enter a value to define the text variable.

   Nothing is sent to the host system.

3. **Transparent Retrieval is set but define isn't set.** One of the following situations will occur:

   - a\) Default response specified and variable exists. The default response is ignored and the contents of the text variable are sent to the host.
   - b\) Default response not specified and variable exists. The contents of the text variable are sent to the host system.
   - c\) Default response specified and variable doesn't exist. The text variable is defined with its contents set to the default response and those contents are sent to the host system.
   - d\) Default response not specified and variable doesn't exist. The user is prompted to enter data for the text variable. If the user enters some data, then that variable is defined with its new contents and those contents are sent to the host. If the user decided to abort the prompt then nothing is sent to the host3.

4. **Neither the transparent define or retrieval flags are set.** If this situation occurs, one of the following things will happen:

   - a\) Default response specified and variable exists. If this is the case, then the user is prompted with the actual contents of the text variable, giving them the option to modify it, then the final response is sent to the host system.
   - b\) Default response not specified and variable exists. Same as (a).
   - c\) Default response specified and variable doesn't exist. The user is prompted to enter data for the variable. The default response is the value prompted to the user giving him the option to accept it, or enter a new value.
   - d\) Default response not specified and variable doesn't exist. The user must be prompted for some data. The data entry field will be blank forcing the user to enter some information.

If nothing is sent to the host and the text variable isn't to be saved to the database or to memory, then the text variable operation can be omitted entirely - since it would produce no functional results.

During transparent define or retrieval modes, it is possible for the user to have to be prompted for some information (as you have already read). If these "special case" situations occur, then the transparency is effectively overidden.

The "answer is required" setting forces the user (if prompted) to make some kind of selection (ie, they can't abort the text variable operation).

### User Defined Variables and Data Security

When working with user-defined text variables, a topic arises which may or may not be addressed by the RIPscrip software developer: security! What if you had credit card information stored in a permanent text variable named `$CredCardNo$` and the host system asked your terminal for that information? Obviously the user should be informed about this request. Situations might arise where security is not a concern - like in a closed environment like an internal office system using RIPscrip. With these situations in mind, the designer of a RIPscrip terminal might wish to include some kind of "data security" feature into the software package.

Data security can be in many forms. You could implement complete "tight" security which would overide all transparent forms of text variable operations - in effect, always making the user approve the request. A lighter security might be desirable - one where any action working on permanent database text variables requires the user's approval, but memory variables don't have to be approved. This kind of a situation would let allow a host system to have control over memory variables (which it presumably created anyway), but let access to permanent variables be under strict control. The exact form of data security that you offer your software's users is unimportant - but knowing that security is a concern is. You do not have to implement data security on your system - it is not an actual part of RIPscrip (per se), but it could be useful to your customers if security is an issue.

### User Defined Data Variable Format Options

Just like text variable parameters defined earlier for built-in text variables, user defined text variables have the same luxury. These parameters define the format of the entered data. You are allowed up to two parameters for a user-defined text variable. The first one is the "mode" parameter which designates what type of data the user can enter. The possible settings for mode are:

| Mode     | Description                                          |
| -------- | ---------------------------------------------------- |
| ANY      | Any character is allowed                             |
| ALPHA    | User can enter alphabetic characters only (A-Z, a-z) |
| NUMBER   | User can enter numeric characters only (0-9)         |
| ALPHANUM | User can enter alphanumeric only (A-Z, a-z, 0-9)     |

The second parameter is an optional conversion designator. The possible settings for this parameter are: _(v2.A4)_

| Conversion | Description |
| --- | --- |
| TONAME | Convert the data to "name" format. The first letter is capitalized and all subsequent characters in a word are set to lower case automatically (eg, "TOM SMITH" becomes "Tom Smith". No special processing is performed on names like "McDonald", etc. If you wish to tackle this particular complexity of name conversion, feel free. |
| TOUPPER | Convert the data to upper case. |
| TOLOWER | Convert the data to lower case |

You may omit one or both of the parameters. If both are omitted, then it will default to "ANY" data with no conversion of any kind. You may omit the mode parameter if you wish, and specify only the conversion parameter. Under no circumstances should a conversion parameter be allowed before a mode parameter - this is considered a syntax error. Some examples of legal and illegal user variable queries might be: _(v2.A4)_

|         |                              |
| ------- | ---------------------------- |
| Legal   | `$USERDATA$`                 |
| Legal   | `$USERDATA()$`               |
| Legal   | `$USERDATA(ANY)$`            |
| Legal   | `$USERDATA(ALPHA,TOUPPER)$`  |
| Legal   | `$USERDATA(TONAME)$`         |
| Illegal | `$USERDATA(TONAME,ANY)$`     |
| Illegal | `$USERDATA(ANY,ALPHA)$`      |
| Illegal | `$USERDATA(TONAME,TOUPPER)$` |

By default, a format option of "ANY" is assumed when no parameter are specified. _(v2.A1)_

### Limits on Length of Variable Names, Etc.

_Added in RIPscrip v2.A2._

The maximum length of a text variable name is 20 characters.

Valid text variable names may contain alpha characters, numbers, or an underscore ("_"). The first character MUST be an aplhabetic character. Only uppercase letters are recognized ("A"-"Z"). Any lowercase letters should be capitilized automatically.

The maximum number of text vaiable parameters (the ones appearing between parenthesis, not the text string) is 40.

The maximum size of a text variable parameter is 12 characters. These are only available to text variables that are part of the RIPscrip specification.

The maximum size of a question or default response is 100 characters. Anything longer than this length is truncated.

The maximum length of a text variable text field is 255 characters. Anything longer than this length is truncated.

### Examples of User-Defined Text Variables

_Added in RIPscrip v2.A1._

Below are some examples of user defined text variables as might be used in a real world situation:

```text
$*+20,50:NAME(ToName),30@What's your name?=John Doe$
```

_(example as of v2.A4)_

This is about as complex as they get. Going from left to right, lets look at what all the codes mean. First, the "*" means that a response to this request is required (they cannot hit ESC or CANCEL to get out of it). The "+" means to save the response to the internal database permanently. The sequence "20,50:" means to place the dialog box's upper left corner at location (20,50) on the screen. The "NAME" is the name of the data variable. The format option "(Name)" means that this should be a formatted name field where the first letter of any word should be capitolized and the remaining characters should be set to lower case. The value ",30" after the variable name/format indicates that this field is up to 30 characters in length. "@What's your name?" indicates that the question "What's your name?" should be displayed when the user is prompted for the information. Finally, the "=John Doe" indicates that if the field doesn't exist, it should be filled in with "John Doe" before any editing is allowed. This gives the user the ability to choose a default name if they wish.

```text
$*#20,10:PASSWORD,10@Please enter your password$
```

This example is a required data variable that echos #'s in place of the keystrokes you entered. The field is placed at (20,10) on the screen with a variable name of PASSWORD. The field is 10 characters wide and the prompt is "Please enter your password".

---

[← Level-3 & Level-9 Commands](13-level-3-9-commands.md) · [Contents](README.md) · [Local File Playback & Pop-Up Lists →](15-local-playback-popup-lists.md)
