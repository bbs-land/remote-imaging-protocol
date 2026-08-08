# Host Commands & Control Characters

[◀ Prev: Advanced Commands](13-advanced-commands.md) · [Contents](README.md) · [Next: Text Variables ▶](15-text-variables.md)

With Mouse regions, Buttons and Text Variable Query ability, you can control the Terminal/Paint programs and how they react with the BBS in many ways. To accomplish this, there are several features of RIPscrip that permit you to do special actions based on different circumstances. In effect, an "action language" of sorts. The following sections go into the available "action language" features in more detail.

Among the various abilities are:

1. [Control-Character specification](#control-characters)
2. [Pre-defined Text Variables & User-defined text variables](15-text-variables.md)
3. [Pop-up pick-lists](17-popup-lists.md)
4. A [Host Command "Template" system](18-host-command-templates.md) for added intelligence.
5. [Query text variable contents](19-text-variable-creation.md) (pre-defined & user variables)

## Control Characters

Not all BBS'es will allow you to use control characters on their Service. Regardless of that, the capability to send any Control Character exists for your Host Commands. The most commonly used Control Characters are:

| Individual Control Characters       | Special Keystrokes      |
| ----------------------------------- | ----------------------- |
| `^@` ... Null (ASCII 0)             | `^[[A` ... Up Arrow     |
| `^G` ... Beep                       | `^[[B` ... Down Arrow   |
| `^L` ... Clear Screen (Top of Form) | `^[[C` ... Right Arrow  |
| `^M` ... Carriage Return            | `^[[D` ... Left Arrow   |
| `^C` ... Break (sometimes)          | `^[[H` ... Home Key     |
| `^H` ... Backspace                  | `^[[K` ... End Key      |
| `^[` ... Escape character           | `^[[L` ... Control Home |
| `^S` ... Pause data transmission    |                         |
| `^Q` ... Resume data transmission   |                         |

Some hosts use the `^` (caret) for their own purposes. In these cases, you can use the `` ` `` (backquote) character instead of the caret. Some systems allow you to specify the caret symbol as two carets (`^^`). Consult your Host Software documentation to determine the best method for your needs.

NOTE: RIPterm uses `^` or `` ` `` and a character to represent a control character. IT IS NOT A CONTROL CHARACTER BY ITSELF, IT IS TRANSLATED BY RIPterm. In other words, `^M` does not send a `^` and then an `M`, it sends a carriage return (ASCII 13). Likewise, RIPscrip commands like Query do not use an `^[`, an actual escape character (ASCII 27) is used.

---

[◀ Prev: Advanced Commands](13-advanced-commands.md) · [Contents](README.md) · [Next: Text Variables ▶](15-text-variables.md)
