# RIPscrip 3.0 - RIPlib baseline comparison

**Where [RIPlib](https://github.com/BradHawthorne/riplib)'s RIPscrip 3.0 differs from the [3.0 reconstruction](../3.0/ripscrip/README.md) in this repository - before either side's extensions enter the picture.**

RIPlib is a parallel, actively developed effort to build a portable, canonical RIPscrip rendering core in C99 (MIT, © 2026 SimVU / Brad Hawthorne). It also defines its own extensions on top of 3.0, documented separately under [3.1-riplib](../3.1-riplib/README.md) (§A2G.1-7) and [3.2-riplib](../3.2-riplib/README.md) (§A2G.8-13). **This tree is not about those.** It is about the layer beneath them: the two projects' accounts of what RIPscrip 3.0 already _was_, and the places where those accounts contradict each other.

That separation is the point. A disagreement about `|W01` or `|f` is not a question of whether to adopt someone's extension - it is a question of fact about a protocol both projects are trying to describe correctly, and exactly one answer can be right. Keeping those questions here, out of the extension trees, is meant to make them straightforward to discuss and settle on their own terms.

## Why the two records differ

Neither project has seen the other's primary evidence, and the two evidence bases barely overlap:

|  | This repository | RIPlib |
| --- | --- | --- |
| **Primary source** | The RIPtel Visual Telnet 3.1 install (RIPscrip driver 3.0.7): its `RIPSCRIP.HLP` / `RIPTEL.HLP` string tables and the 116-file demo-script corpus TeleGrafix shipped with it | Systematic disassembly of `RIPSCRIP.DLL` v3.0.7 (592,896 bytes, 32-bit PE) |
| **Corroborated by** | The 2.00 Alpha 4 draft, SyncTERM's `ripper.c`, the 1.54 published specification, the RIPterm 2.0/2.20.01/2.30 installs | The 1.54 published specification and the 2.00 Alpha 4 draft |
| **Answers well** | What the wire actually carried - which opcodes shipping content sends, with what arguments, how often | What the shipping code actually did - which functions exist, what they compute, where they are buggy or dead |
| **Answers poorly** | Internal behavior of code nobody has disassembled; commands the corpus never exercises | Which of the DLL's accepted opcodes real content used, and what TeleGrafix documented for authors |

Both sources are legitimate and mostly complementary - the same client, examined from opposite ends. Where they genuinely conflict, the resolution is usually cheap: one side checks the other's source. Several items below could be settled in an afternoon by anyone holding both `RIPSCRIP.DLL` and the demo corpus.

## Standing of these pages

|  |  |
| --- | --- |
| **Purpose** | A comparison and negotiation record, not a specification. Nothing here supersedes either project's own documentation |
| **Neutrality** | Each item states both readings and the evidence behind them. Where the evidence here is strong enough to call, it says so and shows why; where it is not, it stays open |
| **Reconciled against** | riplib `main` @ `3e05ecb` (2026-06-30), `docs/spec/` segments 01, 02, 04, 05, 07, 10 and 11 - especially **`11-dll-deviations.md`**, RIPlib's own register of DLL bugs, errata and deliberate deviations, which is the natural counterpart to these pages |
| **Refresh** | RIPlib is a moving target - see [reference/rip-tools.md](../../reference/rip-tools.md#riplib-is-a-moving-target---pull-main-regularly) before editing |
| **Licensing** | Original comparison material, [CC0](../../LICENSE); see [rights.md](../../reference/rights.md) |

The `-riplib` suffix follows the vendor-tree convention in [CONTRIBUTING.md](../../CONTRIBUTING.md#repository-conventions). This tree carries no `text/` or `assets/`.

## Layout

```
3.0-riplib/
  ripscrip/    Language-level conflicts - opcode assignments, parameter semantics
  techspecs/   Implementation-level conflicts - parsing rules, rasterization
```

## Contents

**[Language-level conflicts (`ripscrip/`)](ripscrip/README.md)**

- **[2.0 Write Mode Numbering](ripscrip/2.0-write-modes.md)** - **the priority item.** Which numbers `|W` assigns to XOR, OR and AND. RIPlib's own DLL analysis agrees with this repository's table; a separate reading of the specification led it to renumber anyway
- **[9.0 Command Inventory Comparison](ripscrip/9.0-command-inventory-comparison.md)** - all ~30 opcode assignments where the two inventories disagree, grouped by how they should be resolved, plus commands each side has and the other does not

**[Implementation-level conflicts (`techspecs/`)](techspecs/README.md)**

- **[1.0 Stream Parsing & Escapes](techspecs/1.0-stream-parsing-and-escapes.md)** - the text-escape set and its attribution, and the alternate command introducers
- **[2.0 Fill Pattern Mapping](techspecs/2.0-fill-pattern-mapping.md)** - wire pattern IDs to built-in patterns, pattern `00` semantics, and the pattern bytes themselves

## Where RIPlib is right

This tree is a conflict register, so it reads as a list of disagreements. It is worth recording the opposite finding just as plainly: **RIPlib's analysis of the shipping fill code is correct, it caught defects this repository had not documented, and in one place it corrected the record here.** The pie/chord flood-fill leak (`§BUG.6`) and the never-applied patterned-flood brush (`§DEAD.7`) are real defects in the TeleGrafix implementation, and RIPlib's replacements for both are the right fix. `§DEAD.7` also settles a question this repository had answered from negative evidence: the DLL still carries a live flood-fill command, so `RIP_FILL` outlived its removal from the language - an inference here that the 3.0 driver dropped it entirely has been corrected. All of it has been folded into this repository's own techspecs rather than left in this tree - see [2.0 fill defects](../2.0/techspecs/2.1-fill-defects.md) and the [3.0 delta](../3.0/techspecs/2.0-fill-defects-delta.md).

That case is also the template for how the two evidence bases should settle things: **positive evidence from the binary beats an absent name in the help inventory, and wire observation from the corpus beats an absent function in a disassembly.** Most items on the pages below fall to one or the other.

The same goes for `§BUG.9` (the CHR font parser's `'+'` marker assumption), `§BUG.3`, `§BUG.4` and `§BUG.5` - implementation defects found by disassembly that no amount of corpus analysis would have surfaced.

## Further reading

- [version/3.0/](../3.0/ripscrip/README.md) - this repository's 3.x reconstruction, with per-claim evidence tags
- [3.1-riplib](../3.1-riplib/README.md) · [3.2-riplib](../3.2-riplib/README.md) - RIPlib's extensions on top of the baseline
- [reference/rip-tools.md](../../reference/rip-tools.md) - the RIPlib clone, and how to keep it current
