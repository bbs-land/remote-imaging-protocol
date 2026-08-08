# Text Variables: General, Date/Time & Sound

[◀ Prev: Templates](16-templates.md) · [Contents](README.md) · [Next: Text Variables: Mouse, Text Window & Ports ▶](18-text-variables-mouse-window.md)

This section details all of the pre-defined text variables in the RIPscrip language. Each variable is described thoroughly, and where applicable, simple ANSI-C source code extracts are provided to show how to implement the variable under the C programming language. _(v2.A4)_

Additional groups of text variables are described in [Text Variables: Mouse, Text Window & Ports](18-text-variables-mouse-window.md), [Text Variables: Terminal](19-text-variables-terminal.md) and [Text Variables: Environment, Clipboard, Screen & Tables](20-text-variables-environment.md).

## Text Variable Syntax Descriptions

Each text variable's description details the exact syntax of that variable. The syntax is described in concise detail so that you can easily spot at the glance of an eye exactly what parameters are allowed, and if omitted, what the default values are for those parameters. You can also determine what text variables have optional parameters, and what parameters are required.

There are two basic text variables - those that take parameters, and those that don't. Those that don't use any parameters are the simplest of all to describe syntactically. If a text variable does not require any parameters, then its syntax description would be nearly identical to the following:

```text
Syntax:  $TEXTVAR$
```

This simply states that in order use this text variable in an expression, you insert the text `$TEXTVAR$` in your host command.

For text variables that require parameters though, more detailed descriptions are necessary. Here is an example listing of a text variable requiring one parameter and having an optional second parameter:

```text
Syntax:  $TEXTVAR(req:PARAM1, opt:PARAM2)$
```

You should notice the text "req:" and "opt:". The "req:PARAM1" text indicates that the parameter named "PARAM1" is required. The second piece of text is "opt:PARAM2", which means that the parameter named "PARAM2" is optional.

But what values can PARAM1 or PARAM2 be? It's not mentioned at all what values they can obtain. This is where the parameter value notation comes in. Suppose that PARAM1 can be set to "TRUE" or "FALSE", and that PARAM2 can be set to "BLUE" and "RED". To describe these values our new syntax description would be:

```text
Syntax:  $TEXTVAR(req:PARAM1, opt:PARAM2)$
                      ──────      ──────
                      TRUE        BLUE
                      FALSE       RED
```

From the above description, it is quite simple to determine what parameters are required, which ones are optional, and which ones can be set to what values. Now, let's add a new color to PARAM2, called "GREEN", and let's say that it can only be used as PARAM2 when PARAM1 is equal to "TRUE". We denote this like the following:

```text
Syntax:  $TEXTVAR(req:PARAM1, opt:PARAM2)$
                      ──────      ──────
                      TRUE        BLUE
                      FALSE       RED
                                  GREEN:PARAM1=TRUE
```

Notice the "GREEN:PARAM1=TRUE". This means that PARAM2 can be set to "GREEN" only if PARAM1 is equal to TRUE. What if we added another value to PARAM1 that could also be green, called "MAYBE". Then our example would look like this:

```text
Syntax:  $TEXTVAR(req:PARAM1, opt:PARAM2)$
                      ──────      ──────
                      TRUE        BLUE
                      FALSE       RED
                      MAYBE       GREEN:PARAM1=TRUE,MAYBE
```

Now the description of GREEN has "PARAM1=TRUE,MAYBE". This means that PARAM2 can be set to "GREEN" only if PARAM1 is equal to TRUE or MAYBE. Pretty simple. This same situation could be written more shortly (in this case) with the "not equal" expression <> as in the following:

```text
Syntax:  $TEXTVAR(req:PARAM1, opt:PARAM2)$
                      ──────      ──────
                      TRUE        BLUE
                      FALSE       RED
                      MAYBE       GREEN:PARAM1<>FALSE
```

This means that PARAM2 can be set to GREEN only if PARAM1 is not equal to FALSE. If PARAM2 could only be set to GREEN if PARAM1 was equal to TRUE, we could list this (using a previously described notation):

```text
Syntax:  $TEXTVAR(req:PARAM1, opt:PARAM2)$
                      ──────      ──────
                      TRUE        BLUE
                      FALSE       RED
                      MAYBE       GREEN:PARAM1<>FALSE,MAYBE
```

Optional parameters do not need to be specified. If they are not, then some "suitable" default value will be used for that parameter. Let's take a variation of our previous example:

```text
Syntax:  $TEXTVAR(req:PARAM1, opt:PARAM2)$
                      ──────      ──────
                      TRUE        BLUE
                      FALSE       RED
                      MAYBE       GREEN
                                  ──────
                              def=BLUE
```

This says that if PARAM2 is omitted, that BLUE will be the default. What if BLUE is only the default when PARAM1 is set to TRUE, and RED is default all the other times? Then you would have the following syntax description:

```text
Syntax:  $TEXTVAR(req:PARAM1, opt:PARAM2)$
                      ──────      ──────
                      TRUE        BLUE
                      FALSE       RED
                      MAYBE       GREEN
                                  ──────
                              def=BLUE:PARAM1=TRUE
                                  RED:PARAM1<>TRUE
```

Lastly, a number of text variables have a variable number of parameters. This means that one of the parameters can be repeated more than once, but it must be specified at least once. This is represented in the actual text variable "short description" with an ellipse "..." shown in the parameter list like the following:

```text
$TEXTVAR(param1,param2,...)$ - Perform some kind of operation
```

In the syntax description, it is also shown as an ellipse "...". The parameter immediately preceding the ellipse is the one that is repeated and all subsequent instances of that parameter use the same syntax as the first one repeated. Here is an example if we allowed PARAM2 to be repeated multiple times:

```text
Syntax:  $TEXTVAR(req:PARAM1, req:PARAM2, ...)$
                      ──────      ──────
                      TRUE        BLUE
                      FALSE       RED
                      MAYBE       GREEN
                                  ──────
                              def:BLUE
```

Notice how PARAM2 is set to "req:". This means that PARAM2 must be specified at least once, but can be repeated. It is also possible that the repeated parameter itself may be optional (eg, "opt:"). If this is the case then there can be zero or more occurrences of that parameter.

If no specific default value is allowed for an omitted parameter (ie, it has special significance if it is omitted), then the default value would be listed as "\<none\>".

## Version Number and Vendor Text Variables

These text variables are for version number, vendor determination, and software features of a RIPscrip package. These are used heavily for "detecting" what kind of RIPscrip software a host is connected with. _(v2.A4)_

### $IFS$

_Is Feature Supported_

**Format:** `$IFS(keyword,function)$`

**Syntax:**

```text
$IFS(req:KEYWORD,    opt:CATEGORY)$
         ───────         ────────
         LIST            ALL:KEYWORD=LIST
         _AUDIO          _AUDIO:KEYWORD=LIST
         _EMULATIONS     _EMULATIONS:KEYWORD=LIST
         _IMAGE          _IMAGE:KEYWORD=LIST
         _LANGUAGES      _LANGUAGES:KEYWORD=LIST
         _MISC           _MISC:KEYWORD=LIST
         _PROTOCOLS      _PROTOCOLS:KEYWORD=LIST
         ANSI            ────────
         BMP         def=<none>
         CISQUICKB
         DOORWAY
         ENG
         EXTAPPS
         JPEG
         KERMIT
         RIPSCRIP
         SUPERKERMIT
         VT102
         WAV
         XMODEM
         XMODEMCRC
         XMODEM1K
         XMODEM1KG
         YMODEM
         YMODEMG
         ZMODEM
         ZMODEMCR
```

This is a unique and powerful text variable. This is the mechanism by which the host can find out what capabilities are supported in the remote terminal. For example, if the host needs to know if the terminal supports JPEG files, the host could send a query: _(v2.A2)_

```text
!|1Σ0000$IFS(JPEG)$
```

Note that the character "Σ" is actually an escape character (ASCII value 27). _(v2.A4)_

The terminal would respond with `1` if it has JPEG display ability, or "0" if it doesn't. _(v2.A4)_

Because the number of features supported by a software package can get quite extensive, they are categorized for the purposes of this command. This allows the host to be able to query particular sub-sets of information (eg, what file transfer protocols are supported, etc). These categories are simple keyword names just like the keyword names used to identify particular features, but with one slight difference. Categories begin with an underscore character (_) to differentiate them from actual feature keywords. These categories are used in conjunction with a special `$IFS$` directive called LIST. This will list out all keywords for a specific category, or if no category is specified, it will list out all categories. The basic categories and their corresponding sub-items are as follows: _(v2.A4)_

| Keyword/category  | Description                                      |
| ----------------- | ------------------------------------------------ |
| _IMAGE            | Category: what image formats are supported       |
| &emsp;JPEG        | JPEG photographic image file format supported    |
| &emsp;BMP         | BMP bitmap image file format supported           |
| _AUDIO            | Category: what audio formats are supported       |
| &emsp;WAV         | WAV digitized audio file format supported        |
| _PROTOCOLS        | Category: file transfer protocols                |
| &emsp;KERMIT      | Kermit file transfer protocol supported          |
| &emsp;CISQUICKB   | Compuserve(TM) QuickB protocol supported         |
| &emsp;SUPERKERMIT | Super Kermit protocol supported                  |
| &emsp;XMODEM      | X-Modem protocol supported                       |
| &emsp;XMODEMCRC   | X-Modem protocol with CRC checking supported     |
| &emsp;XMODEM1K    | X-Modem protocol with 1K blocks supported        |
| &emsp;XMODEM1KG   | X-Modem protocol with 1K blocks / G supported    |
| &emsp;YMODEM      | Y-Modem Batch protocol supported                 |
| &emsp;YMODEMG     | Y-Modem protocol with G supported                |
| &emsp;ZMODEM      | Z-Modem protocol supported                       |
| &emsp;ZMODEMCR    | Z-Modem protocol with Crash Recovery supported   |
| _LANGUAGES        | Category: what languages are supported           |
| &emsp;ENG         | English language is supported                    |
| _EMULATIONS       | Category: what terminal emulations are supported |
| &emsp;RIPSCRIP    | RIPscrip terminal emulation is supported         |
| &emsp;DOORWAY     | Doorway (tm) mode is supported                   |
| &emsp;ANSI        | ANSI terminal emulation supported                |
| &emsp;VT102       | DEC VT-102 terminal emulation supported          |
| _MISC             | Category: miscellaneous features                 |
| &emsp;EXTAPPS     | External applications are supported              |

If you specify the LIST directive all by itself as in the text variable `$IFS(LIST)$`, then the categories will be returned in an alphabetical, comma-delimited list like this: _(v2.A4)_

**Example:** `$IFS(LIST)$` **Returns:** `_AUDIO,_EMULATIONS,_IMAGE,_LANGUAGES,_MISC,_PROTOCOLS`

You may list out a particular category by issuing an `$IFS$` variable with the LIST keyword and the category as the second parameter as in the following example: _(v2.A4)_

**Example:** `$IFS(LIST, _EMULATIONS)$` **Returns:** `ANSI,DOORWAY,RIPSCRIP,VT102`

One final LIST directive is the "ALL" directive. This returns a list of all feature keywords (omitting category keywords) in one very long alphabetical, comma-delimited list. This would be like asking for each category's listing separately then sorting the list of keywords and stringing them all together. Here's an example of the above keywords being queried in ALL mode (note, we use an ellipse (...) at the end of the line to indicate continuation to the next line. _(v2.A4)_

```text
Example:  $IFS(LIST, ALL)$
Returns:  ANSI,BMP,CISQUICKB,DOORWAY,ENG,EXTAPPS,JPEG, ...
          KERMIT,RIPSCRIP,SUPERKERMIT,VT102,WAV,XMODEM, ...
          XMODEM1K,XMODEM1KG,XMODEMCRC,YMODEM,YMODEMG, ...
          ZMODEM,ZMODEMCR
```

If you omit all parameters from the `$IFS$` variable, then it should be considered a text variable syntax error. _(v2.A4)_

In the future, more categories will probably be added for different purposes like hardcopy support, network support and many other things. That is why this command has been designed with such flexibility in mind. _(v2.A4)_

Note that the comma-delimited list of keywords returned from a LIST directive are alphatbetically sorted and have no spaces in them. In addition, there are no carriage returns or any other form of delimiter after the last keyword returned. If you wish to have a carriage return after the list, place a ^M control character directive in the query command that you used to work with this text variable. _(v2.A4)_

Note, a category must have at least one keyword defined underneath it in order for it to be considered "defined". _(v2.A4)_

If a LIST directive is specified on a category that doesn't exist, then nothing is returned to the host (a null string). If a specific keyword is inquired about and it doesn't exist, a "0" is returned to indicate that the feature isn't supported.

### $NULL$

_A null text variable (returns nothing)_

_Added in RIPscrip v2.A1._

**Format:** `$NULL$` **Syntax:** `$NULL$`

This text variable is a special variable. It always returns nothing to the host system. It doesn't prompt the user or any information and it doesn't set anything. It is intended to be a place-holder for commands that require a text parameter ([RIP_MOUSE](11-level-1-commands.md#rip_mouse), [RIP_BUTTON](11-level-1-commands.md#rip_button) and [RIP_QUERY](11-level-1-commands.md#rip_query)). When you have this text variable all by itself in a host command, it makes the host command do absolutely nothing, but has something defined for the host command to satisfy the RIPscrip interpreter (which expects something to be defined in host command text parameters).

**Example:** `$NULL$` **Returns:** nothing

### $RIPVER$

_RIPscrip version (e.g., "RIPSCRIP015300")_

**Format:** `$RIPVER$` **Syntax:** `$RIPVER$`

This Text Variable returns a phrase which will identify a RIPscrip-compatible software package. It is designed to be used by a host to detect what version of RIPscrip graphics your terminal can support as well as the type (brand) of RIPscrip terminal that is in use. When this Text Variable is used, it will respond back with "RIPSCRIP" followed by the Version Number (e.g., "01.54"), followed by two digits identifying the Vendor of the terminal. The first digit of the Vendor ID field is the Vendor Code (1=RIPterm). The second digit is the Vendor's sub-version code identifying sub-versions of the software that still support the same RIPscrip software version. Valid Vendor Codes are: _(v1.54)_

| Code | Vendor                                     |
| ---- | ------------------------------------------ |
| 0    | Generic RIPscrip terminal (vendor unknown) |
| 1    | RIPterm (from TeleGrafix Communications)   |
| 2    | Qmodem Pro (from Mustang Software, Inc)    |

See the section earlier in this document on ANSI sequences for a more robust description of the Vendor Codes and Auto-Sensing. _(v1.54)_

**Example:** `$RIPVER$` **Returns:** `RIPSCRIP015300`

### $TERMINFO$

_Returns vendor specific data_

_Added in RIPscrip v2.A4._

**Format:** `$TERMINFO(keyword)$`

**Syntax:**

```text
$TERMINFO(opt:KEYWORD)$
              ───────
              NAME
              VENDOR
              VERSION
              LIST
              ───────
          def=NAME
```

This text variable returns specific information about the RIPscrip software package in use by the terminal (remote) user. If no parameter is specified, for example, `$TERMINFO$` or `$TERMINFO()$`, then the sequence returned to the host is the name of the terminal (see the "NAME" keyword below). Otherwise, you may specify a particular terminal information keyword to request information about. If the specific keyword is undefined (ie, not used by the terminal), a value of "NONE" will be returned.

The following keywords are prerequisites for any generic RIPscrip 2.0 terminal system and must be implemented. You may add as many more as you wish, but these following keywords must be defined:

| Keyword | Description |
| --- | --- |
| NAME | Name of the software (default with no parameter). Example "RIPterm Professional". |
| VENDOR | Name of the company who wrote the software. Example, "TeleGrafix Communications, Inc." |
| VERSION | The Version number of the software. For example, "2.00.00". |
| LIST | List all allowable keywords (see below). |

The text returned to the host is not terminated with any carriage returns or anything like that. It's up to you to provide that kind of information in a button's host string or in a query string. The LIST directive though returns a list of all recognized keywords for that terminal. For example, RIPterm Pro returns the following for the `$TERMINFO(LIST)$` expression:

```text
LIST,NAME,VENDOR,VERSION
```

Note, the list is comma (,) delimited between keywords, but not after the last keyword. In addition, the keywords are returned in alphabetical order, converted to all capitals.

**Example:** `$TERMINFO$` **Returns:** `RIPterm Professional`

**Example:** `$TERMINFO(NAME)$` **Returns:** `RIPterm Professional`

**Example:** `$TERMINFO(VERSION)$` **Returns:** `2.00.00`

**Example:** `$TERMINFO(VENDOR)$` **Returns:** `TeleGrafix Communications, Inc.`

**Example:** `$TERMINFO(LIST)$` **Returns:** `LIST,NAME,VENDOR,VERSION`

**Example:** `$TERMINFO(GOOSE)$` **Returns:** `NONE`

## Date and Time Text Variables

These text variables return information on the current date and/or the current time. _(v2.A4)_

### $ADOW$

_Abbreviated Day of Week_

**Format:** `$ADOW$` **Syntax:** `$ADOW$`

This Text Variable returns the current day of the week in abbreviated form. Possible values are: Sun, Mon, Tue, Wed, Thu, Fri and Sat.

**Example:** `$ADOW$` **Returns:** `Mon`

### $AMPM$

_Returns AM or PM depending on time_

**Format:** `$AMPM$` **Syntax:** `$AMPM$`

This Text Variable returns a two-character value of either "AM" or "PM" depending on what time it is.

**Example:** `$AMPM$` **Returns:** `PM`

### $DATE$

_Date in short format_

**Format:** `$DATE$` **Syntax:** `$DATE$`

This Text Variable returns the current date. in the format MM/DD/YY.

**Example:** `$DATE$` **Returns:** `12/19/93`

### $DATETIME$

_Date and Time_

**Format:** `$DATETIME$` **Syntax:** `$DATETIME$`

This Text Variable returns a combination date and time. The format is somewhat different than standard time/date notation. It is:

```text
DAY-OF-WEEK   MONTH   DAY-OF-MONTH  HH:MM:SS  YEAR
```

**Example:** `$DATETIME$` **Returns:** `Sat Dec 19 14:38:50 1993`

> NOTE: This is the standard Unix date/time notation.

### $DAY$

_Day of Month Number_

**Format:** `$DAY$` **Syntax:** `$DAY$`

This Text Variable returns the current day of the month. Possible values for this Variable are from 01-31.

**Example:** `$DAY$` **Returns:** `05`

### $DOW$

_Day of week fully spelled out_

**Format:** `$DOW$` **Syntax:** `$DOW$`

This Text Variable returns the current day of the week. The name is fully spelled out. Possible values are: Sunday, Monday, Tuesday, Wednesday, Thursday, Friday and Saturday.

**Example:** `$DOW$` **Returns:** `Saturday`

### $DOY$

_Day of year_

**Format:** `$DOY$` **Syntax:** `$DOY$`

This Text Variable returns the number of days so far in the year. A year has 365 days (except leap years which have 366). `$DOY$` can return 001 - 366.

**Example:** `$DOY$` **Returns:** `214`

### $FYEAR$

_4 digit year_

**Format:** `$FYEAR$` **Syntax:** `$FYEAR$`

This Text Variable returns the four-digit number of the current year.

**Example:** `$FYEAR$` **Returns:** `1993`

### $HOUR$

_Hour (format HH) - normal style_

**Format:** `$HOUR$` **Syntax:** `$HOUR$`

This Text Variable returns the two digit number of the current hour. This variable range from 01 - 12. This does not use military format.

**Example:** `$HOUR$` **Returns:** `11`

### $MHOUR$

_Hour (format HH) - military style_

**Format:** `$MHOUR$` **Syntax:** `$MHOUR$`

This Text Variable returns a two-digit number of the current hour in military format. This variable may range from 00 - 23.

**Example:** `$MHOUR$` **Returns:** `17`

### $MIN$

_Minutes_

**Format:** `$MIN$` **Syntax:** `$MIN$`

This Text Variable returns the two-digit number representing the current minutes in the hour. Possible values for this variable are 00-59.

**Example:** `$MIN$` **Returns:** `45`

### $MONTH$

_Month Name_

**Format:** `$MONTH$` **Syntax:** `$MONTH$`

This Text Variable returns the full name of the current month. It is not abbreviated (e.g., "November" instead of "Nov")

**Example:** `$MONTH$` **Returns:** `December`

### $MONTHNUM$

_Month Number_

**Format:** `$MONTHNUM$` **Syntax:** `$MONTHNUM$`

This Text Variable returns the number of the current month. January=01 and December=12.

**Example:** `$MONTHNUM$` **Returns:** `12`

### $SEC$

_Seconds_

**Format:** `$SEC$` **Syntax:** `$SEC$`

This Text Variable returns a 2-digit number representing the current seconds of the minute. Possible values for this variable are 00-59.

**Example:** `$SEC$` **Returns:** `59`

### $TIME$

_Time in standard format_

**Format:** `$TIME$` **Syntax:** `$TIME$`

This Text Variable returns the time in military format (hours from 00 - 23). The format is hours, minutes, and seconds separated by colons. HH:MM:SS

**Example:** `$TIME$` **Returns:** `18:09:33`

### $TIMEZONE$

_Time Zone or "NONE" if unknown_

**Format:** `$TIMEZONE$` **Syntax:** `$TIMEZONE$`

This Text Variable returns a word/phrase that describes the time-zone the terminal is in. This may be returned as anything like "PST" for Pacific Standard Time, "EST" for Eastern Standard Time, etc. If the time zone is not set on your PC, this variable will respond with NONE

**Example:** `$TIMEZONE$` **Returns:** `PST`

### $WDAY$

_Day of Week_

**Format:** `$WDAY$` **Syntax:** `$WDAY$`

This Text Variable returns a one-digit number representing the day of the week. Possible values are 0-6, where 0=Sunday (the first day in the week).

**Example:** `$WDAY$` **Returns:** `2`

### $WOY$

_Week of current year 00-53; Sunday=1st Day of Week_

**Format:** `$WOY$` **Syntax:** `$WOY$`

This Text Variable returns a number from 00-53, representing the week in the year. Even though there are 52 weeks in a year, a week might not begin exactly on the first day of the year, so a maximum value for this variable can be 53 under these circumstances. For this variable, Sunday is considered to be the first day of the week.

**Example:** `$WOY$` **Returns:** `32`

### $WOYM$

_Week of current year 00-53; Monday=1st Day of Week_

**Format:** `$WOYM$` **Syntax:** `$WOYM$`

This Text Variable returns a number from 00-53, representing the week in the current year. Even though there are 52 weeks in a year, a week might not begin exactly on the first day of the year, so a maximum value for this variable can be 53 under these circumstances. For this variable, Monday is considered to be the first day of the week.

**Example:** `$WOYM$` **Returns:** `32`

### $YEAR$

_2 digit year_

**Format:** `$YEAR$` **Syntax:** `$DOY$`

This Text Variable returns the two-digit number of the current year.

**Example:** `$YEAR$` **Returns:** `93`

## Sound Related Text Variables

These text variables generate different kinds of sounds. _(v2.A4)_

### $ALARM$

_Warning! This sound indicates failure!_

**Format:** `$ALARM(count)$`

**Syntax:**

```text
$ALARM(opt:COUNT)$
           ─────
           1-65535
           ─────
       def=3
```

This Active Text Variable produces a warning sound, indicating failure of an action. This sound is used for aborted downloads.

This command doesn't require any parameters. If none are specified, then the count is assumed to be 3. The count parameter is the number of times that the warning sound is repeated. _(v2.A1)_

The C source code to play this sound is:

```c
for (i=0 ; i<count ; i+=1)
{
     sound(320);  delay(200);     // the Hertz frequency to play
     sound(160);  delay(425);     // millisecond delay
}
nosound();                        // turn the sound off
```

**Example:** `$ALARM(3)$` ... equivalent to `$ALARM$` **Returns:** nothing

### $BEEP$

_Beep Sound (ala Ctrl-G)_

**Format:** `$BEEP(frequency,length)$`

**Syntax:**

```text
$BEEP(opt:FREQUENCY, opt:LENGTH)$
          ─────────      ──────
          1-65535        1-65535
          ─────────      ──────
      def=1000       def=75
```

This Active Text Variable beeps the terminal, producing a Ctrl-G sound. No parameters are required. If none are provided then the frequency is assumed to be 1000 Hertz and the length of time that it should play is 75 milliseconds. _(v2.A1)_

This command allows you to specify no parameters (default settings), only one parameter (the frequency), or both parameters (frequency and length/duration). Under no circumstances will values for any of the two parameters above 65535 be permitted. If values above these limits are encountered then the variable is not processed. _(v2.A4)_

Note that this text variable has a 75 millisecond delay after the beep is complete where no sound is playing. Stringing multiple beeps together will have a noticable gap between the sounds. To play continuous tones at different frequencies, use multiple [`$T$`](#t) variables (see below). _(v2.A4)_

The C source code to play this sound is:

```c
sound(freq);     // the Hertz frequency to play
delay(length);   // millisecond delay
nosound();       // turn the sound off
delay(75);       // millisecond delay
```

**Example:** `$BEEP(1000,75)$` ... equivalent to `$BEEP$` **Returns:** nothing

### $BLIP$

_Blipping Sound (like a hitting a barrier)_

**Format:** `$BLIP(freq,length)$`

**Syntax:**

```text
$BLIP(opt:FREQUENCY, opt:LENGTH)$
          ─────────      ──────
          1-65535        1-65535
          ─────────      ──────
      def=50         def=25
```

This Active Text Variable is like [`$BEEP$`](#beep), except the sound is different. It produces a barrier sound; like you're running into a wall.

No parameters are required. If none are provided then the frequency is assumed to be 50 Hertz and the length of time that it should play is 25 milliseconds. _(v2.A1)_

This command allows you to specify no parameters (default settings), only one parameter (the frequency), or both parameters (frequency and length/duration). Under no circumstances will values for any of the two parameters above 65535 be permitted. If values above these limits are encountered then the variable is not processed. _(v2.A4)_

The C source code to play this sound is:

```c
sound(freq);     // the Hertz frequency to play
delay(length);   // millisecond delay
nosound();       // turn the sound off
delay(10);
```

**Example:** `$BLIP(50,10)$` ... equivalent to `$BLIP$` **Returns:** nothing

### $MUSIC$

_Musical (cheerful) sound_

**Format:** `$MUSIC(count)$`

**Syntax:**

```text
$MUSIC(opt:COUNT)$
           ─────
           1-65535
           ─────
       def=4
```

This Active Text Variable produces a cheerful sound, indicating success of an action. This sound is used for successful downloads and dialed connections.

This command doesn't require any parameters. If none are provided, then the count is assumed to be 4. The count parameter determines how many times the musical sound is repeated. _(v2.A1)_

The C source code to play this sound is:

```c
for (i=0 ; i<count; i+=1)
{
     sound(1300);   delay(10);     // Hertz frequency to play
     sound(1200);   delay(10);     // millisecond delay
     sound(1100);   delay(10);
     sound(1000);   delay(10);
     sound(900);    delay(10);
     sound(800);    delay(10);
     sound(700);    delay(10);
     sound(850);    delay(10);
     sound(950);    delay(10);
}
nosound();                         // turn the sound off
```

**Example:** `$MUSIC(4)$` ... equivalent to `$MUSIC$` **Returns:** nothing

### $PHASER$

_Fire phasers!_

**Format:** `$PHASER(start,stop,inc,time)$`

**Syntax:**

```text
$PHASER(opt:START, opt:STOP, opt:INCREMENT, opt:TIME)$

        opt:START      opt:STOP
            ─────          ────
            1-65535        START-65535
            ───────        ────
        def=2500       def=50

        opt:INCREMENT  opt:TIME
            ─────────      ────
            1-65535        1-65535
            ─────────      ────
        def=20         def=2
```

This Active Text Variable produces a sound like firing your energy weapons in a game. Now you too can blast away with the best of them. Trivia question: What does phaser stand for? See [`$REVPHASERS$`](#revphaser) for the answers.

This command doesn't require any parameters. If none are specified then the START is assumed to be 2500 Hertz. STOP is assumed to be 50 Hertz, INC is assumed to be 20 Hertz increments and TIME is assumed to be 2 milliseconds. START must be greater than STOP and INC must be greater than zero. If none of these conditions are met then the defaults are used for the sound effect. _(v2.A1)_

This command allows you to specify no parameters (default settings), only one parameter (the starting frequency), two parameters (start and end frequency), three parmaters (start and end frequency as well as the increment frequency), or finally all four parameters which correspond to the start and stop frequencies, the increment frequency and lastly the increment time delay (in milliseconds). Under no circumstances will values for any of the four parameters above 65535 be permitted. If values above these limits are encountered then the variable is not processed. _(v2.A4)_

The C source code to play this sound is:

```c
for (i=start ; i>=stop ; i-=inc)
{
     sound(i);               // the Hertz frequency to play
     delay(time);            // millisecond delay
}
nosound();                   // turn the sound off
```

**Example:** `$PHASER(2500,50,20,2)$` ... equivalent to `$PHASER$` **Returns:** nothing

### $REVPHASER$

_Fire phasers!_

**Format:** `$REVPHASER(start,stop,inc.time)$`

**Syntax:**

```text
$REVPHASER(opt:START, opt:STOP,   opt:INCREMENT, opt:TIME)$

           opt:START      opt:STOP
               ─────          ────
               1-65535        1-START
               ─────          ────
           def=50         def=2500

           opt:INCREMENT  opt:TIME
               ─────────      ────
               1-65535        1-65535
               ─────────      ────
           def=20         def=2
```

This Active Text Variable produces a sound like firing your energy weapons in a game. Like [`$PHASER$`](#phaser) makes an ascending tone, `$REVPHASER$` makes a descending tone. Answer to trivia question in `$PHASER$`: Phaser stands for PHoton Amplification by Stimulated Emission of Radiation. Sound familiar? Laser is Light Amplification by Stimulated Emission of Radiation, and Maser is Microwave Amplification by Stimulated Emission of Radiation.

This command doesn't require any parameters. If none are specified then the START is assumed to be 50 Hertz. STOP is assumed to be 2500 Hertz, INC is assumed to be 20 Hertz increments and TIME is assumed to be 2 milliseconds. START must be greater than STOP and INC must be greater than zero. If none of these conditions are met then the defaults are used for the sound effect. _(v2.A1)_

This command allows you to specify no parameters (default settings), only one parameter (the starting frequency), two parameters (start and end frequency), three parmaters (start and end frequency as well as the increment frequency), or finally all four parameters which correspond to the start and stop frequencies, the increment frequency and lastly the increment time delay (in milliseconds). Under no circumstances will values for any of the four parameters above 65535 be permitted. If values above these limits are encountered then the variable is not processed. _(v2.A4)_

The C source code to play this sound is:

```c
for (i=start ; i<=stop ; i+=inc)
{
     sound(i);               // the Hertz frequency to play
     delay(time);            // millisecond delay
}
nosound();                   // turn the sound off
```

**Example:** `$REVPHASER(50,2500,20,2)$` ... Same as `$REVPHASER$` **Returns:** nothing

### $T$

_Play a simple audio tone_

_Added in RIPscrip v2.A4._

**Format:** `$T(freq,length)$`

**Syntax:**

```text
$T(opt:FREQUENCY, opt:LENGTH)$
       ─────────      ──────
       1-65535        1-65535
       ─────────      ──────
   def=1000       def=75
```

This Active Text Variable produces an audible sound. Both parameters are required. The \<freq\> parameter determines the frequency in Hertz and the \<length\> parameter determines the duration of the tone in milliseconds. frequency is assumed to be 1000 Hertz and the length of time that it should play is 75 milliseconds.

Under no circumstances will values for any of the two parameters above 65535 be permitted. If values above these limits are encountered then the variable is not processed.

Unlike the [`$BEEP$`](#beep) command, this variable has no pause after the sound stops playing, thus allowing you to string multiple `$T$` variables together in rapid succession to produce musical notes.

The C source code to play this sound is:

```c
sound(freq);     // the Hertz frequency to play
delay(length);   // millisecond delay
nosound();       // turn the sound off
```

**Example:** `$T(1000,75)$` ... equivalent to `$BEEP$` **Returns:** nothing

---

[◀ Prev: Templates](16-templates.md) · [Contents](README.md) · [Next: Text Variables: Mouse, Text Window & Ports ▶](18-text-variables-mouse-window.md)
