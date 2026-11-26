# Issue #333 — Exact nested template example bytes

[Issue #333](https://github.com/bbs-land/remote-imaging-protocol/issues/333) · [Example 4](../ripscrip/5.2-templates-and-conditionals.md#host-command-language-and-templates) · [Research backlog](issue-backlog.md)

RealDeuce filed #333 on 2026-11-26 at 05:11:36 UTC. The 05:16 UTC check found it with no comments. Branch `issue-333-template-example-bytes` starts at fork stable `8875443b50eb40f9bac243019cdacfe55438b32a`.

The ALPHA 4 example defines the time template in slot 2 and refers to slot 4 in the response. The guide keeps the reported input, including the empty slot 4, and corrects its expected response. Assume a fresh session with no group requiring a selection for slot 4. At ordinary-scan entry, each parenthesis in the source is an ordinary byte. [Original text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) · [Existing processing order](../ripscrip/5.2-templates-and-conditionals.md#shipped-processing-order).

At DLL `0x4b1b9–0x4b1fd` the ordinary scanner recognizes a valid four-byte template marker, deletes it, and looks up the slot through `0x489f8`. For an absent slot and no required selection, `0x4b209–0x4b216` continues at the deletion point. For a present slot, `0x4b253–0x4b266` invokes insertion helper `0x65987` and advances over the inserted content. The calls at `0x4acfa` and `0x4ad16` apply the two ordinary passes. The first masks caret/backquote control decoding, and the second converts source `^m` to CR where reached. [Two-pass source and existing fixtures](issue-6-macro-evaluation.md#original-preprocessing-and-host-command-pipeline-probes) · [Control-byte derivation](issue-53-host-control-brackets.md).

After pass one, the exact working text is:

```text
HELLO THERE^m(The date is $DATE$^m$?1$^m)()(The day is $DOW$^m)
```

For the fixed local day and date, pass two reaches four source `^m` pairs and converts them. The insertion for the remaining `$?1$` is skipped in that pass and preserves its own `$DOW$^m`. The resulting sequence has four bytes `0D` and one literal `^m`:

```text
HELLO THERE<CR>(The date is 07/18/93<CR>The day is $DOW$^m<CR>)()(The day is Sunday<CR>)
```

The last byte is `29` hex, the final close parenthesis. This records the response presented for sending, before any transport-specific handling. [Date and day formatting](issue-18-date-time-format.md).
