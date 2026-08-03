# Text Variable Creation & Query

[◀ Prev: Host Command Templates](18-host-command-templates.md) · [Contents](README.md) · [Next: Icon File Format ▶](20-icon-file-format.md)

As mentioned in preceding sections, [Text Variables](15-text-variables.md) were described as either pre-defined variables, or as User Variables. Pre-defined variables are variables that RIPscrip products know things about "out of the box". They will always know what the variables mean, from the day you install the software. User Variables are variables that the user of RIPscrip products defines, and teach it new things it doesn't already know.

## What Are User Variables?

A User Variable is a Text Variable that RIPscrip doesn't know exists. They are custom-defined text variables that contain information that the terminal user will fill in. If a variable already contains information, a host will be automatically told (if told to do so) what that variable contains without the user having to intervene (i.e., transparent information exchange).

Examples of Text Variables might be:

```text
$FULL_NAME$    ... What is your full name?
$COMPANY_NAME$ ... What company do you work for?
$AGE$          ... How old are you?
$DATEOFBIRTH$  ... What is your Date of Birth?
$PHONENUMBER$  ... What is your Day-time phone number?
```

User Variables will "keep track" of these responses for you, in the terminal program database. You can tell the terminal to store these values permanently, or they may be active only during the current session, or they may be defined as temporary where they are not stored for more than a brief moment.

> **NOTE:** This ability is configurable so that information exchange can be either interactive, or automatic. Automatic transfer of information does NOT prompt the user with the information unless the variable has not yet been defined. If it has not been defined, a pop-up question will appear asking the user a particular question, thus defining the text variable.

If the exchange is interactive, the data is displayed in a pop-up editor box, asking you if the information is correct. If it is, press ENTER and the retrieved information is sent to the host for you. If it is not correct, or it has not been created yet, just type it in and press ENTER and it will be saved automatically, and sent to the host all at once.

## How Can User Variables Be Important?

Lets take an example. You are the system operator of a large RIPscrip host. As you have read, RIPscrip can take advantage of database-like ability on the terminal end. If you can alter your host to ask questions with RIPscrip Text Variables built in, you can have the terminal calling your host automatically fill in questionnaires. Imagine if a user could sign-up on your host without having to type more than a single keystroke (i.e., "YES, this information is correct"). With User Text Variables, you can do this very thing.

## Creating User Variables

There are two ways of defining User Text Variables in RIPaint. You can use either the Define Text Variable command, or you can use Text Variable Queries, as described in the next section.

## Defining Text Variables

The RIPscrip command Define Text Variable ([RIP_DEFINE](13-advanced-commands.md#rip_define)) is by definition, an interactive command with the user. The RIPscrip command will attempt to define a User Variable. This variable is some piece of information that the system operator deems important. You may specify a question, a default response, and how many characters long the response may be.

Once the terminal has received a define command, the terminal pops up an appropriate question box on the user's screen, asking him the desired question that should be saved to a particular Text Variable. If you did not specify a question, a default question is used (i.e., "Enter `<name of text variable>`").

Once the user has entered his response, it is recorded and saved. How long it is stored depends on what the host told the terminal. The host can tell the terminal "save this on your hard disk forever". The host may also tell the terminal "don't save this to disk, but remember this value until you exit RIPterm". You also have the option of saying "don't remember this value at all, just pop up a question, and send the value to me NOW" - i.e., don't save it at all, just enter it and send it to the host).

## Querying Text Variables

Now that you know how to define information on the terminal, you need to know the last method of asking the terminal about text variables. This feature is called "Data Query" ([RIP_QUERY](13-advanced-commands.md#rip_query)).

Data Query is a generic query command that can ask the terminal one or more questions, and tell it how to transmit the information back to the host. This command is for use in non-button situations where you do not want to wait until the user clicks on a button to get your data back.

Data Query is a special RIPscrip command that can be used to ask the contents of one or more Text Variables.

## Examples of Text Variable Query

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

---

[◀ Prev: Host Command Templates](18-host-command-templates.md) · [Contents](README.md) · [Next: Icon File Format ▶](20-icon-file-format.md)
