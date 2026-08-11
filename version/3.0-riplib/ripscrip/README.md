# RIPscrip 3.0 - RIPlib baseline comparison (language level)

_Conflicts between [RIPlib](https://github.com/BradHawthorne/riplib)'s account of RIPscrip 3.0 and the [3.0 reconstruction](../../3.0/ripscrip/README.md) here, at the language level: which opcode means what, and what its parameters carry. Implementation-level conflicts are in the companion [technical specifications](../techspecs/README.md); shared terminology is in the [glossary](../../glossary.md)._

Extensions are out of scope - RIPlib's own additions live in [3.1-riplib](../../3.1-riplib/ripscrip/README.md) and [3.2-riplib](../../3.2-riplib/ripscrip/README.md). See the [tree README](../README.md) for why the two records differ and how each side's evidence should be weighed.

## How items are stated

Each conflict gives both readings, the evidence behind each, and a disposition:

| Disposition | Meaning |
| --- | --- |
| **Callable** | The evidence here is strong enough to state which reading is correct, and the page shows why |
| **Open** | Both readings are defensible on the evidence available; naming the check that would settle it is the useful output |
| **Compatible** | The two differ in form but not in effect - worth recording so neither side "fixes" it into a real conflict |

## Contents

- **2. Drawing**
  - **[2.0 Write Mode Numbering](2.0-write-modes.md)** _(callable)_ - `|W` values for XOR, OR and AND. The single highest-impact disagreement between the two projects: it silently mis-renders existing content in both directions
- **9. Reference**
  - **[9.0 Command Inventory Comparison](9.0-command-inventory-comparison.md)** - the full opcode-by-opcode comparison. Around thirty assignments differ, most of them clustered in the punctuation range where RIPlib's extended-command block and this repository's 3.x skewed-oval family both live

## Not in scope here

- **RIPlib's `§A2G` extensions** - [3.1-riplib](../../3.1-riplib/README.md), [3.2-riplib](../../3.2-riplib/README.md)
- **Text-variable conflicts** - RIPlib tags every one of its divergent variables as a v3.1 or v3.2 addition, so they are recorded with those revisions ([v3.1](../../3.1-riplib/ripscrip/9.0-additions-reference.md#text-variables), [v3.2](../../3.2-riplib/ripscrip/9.1-text-variables.md)) rather than duplicated here
- **Defects RIPlib found and fixed correctly** - folded into this repository's own techspecs; see [2.0 fill defects](../../2.0/techspecs/2.1-fill-defects.md)
