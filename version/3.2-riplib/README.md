# RIPscrip 3.2 (RIPlib §A2G.8-13) - unofficial extensions

**Third-party, post-TeleGrafix extensions to RIPscrip 3.x, defined and implemented by [RIPlib](https://github.com/BradHawthorne/riplib).** The follow-on revision to [3.1-riplib](../3.1-riplib/README.md): six further extension sections, **§A2G.8** through **§A2G.13**, identified on the wire as `RIPSCRIP032001`. RIPlib describes them as _"small refinements that build on v3.1 without changing any existing wire-format command"_ - quality-of-life additions rather than architecture.

_Additions and differences only._ Deliberately **not** a spec clone; everything inherited unchanged from the TeleGrafix record is documented in [3.0](../3.0/ripscrip/README.md), and everything §A2G.1-7 changes in [3.1-riplib](../3.1-riplib/README.md). These pages cover only what v3.2 adds on top of both.

## Status and provenance

|  |  |
| --- | --- |
| **Origin** | RIPlib, not TeleGrafix - authored 2026 |
| **Standing** | **Unofficial.** One implementation's extension proposal, shipped in that implementation |
| **Builds on** | [3.1-riplib](../3.1-riplib/README.md) (§A2G.1-7). v3.2 is additive to v3.1, not a replacement |
| **Wire identification** | `RIPSCRIP032001` - via `$RIPVER$` and the `ESC[!` auto-sense response |
| **Upstream source** | `~/src/rip-tools/riplib/docs/spec/06a-v32-extensions.md`, cross-referenced through segments 02 and 07 |
| **Reconciled against** | riplib `main` @ `3e05ecb` (2026-06-30). **RIPlib is a moving target** - see [reference/rip-tools.md](../../reference/rip-tools.md#riplib-is-a-moving-target---pull-main-regularly) for the refresh procedure before editing these pages |
| **Licensing** | The pages here are original summaries and comparisons, [CC0](../../LICENSE); RIPlib's own segments carry its copyright. See [rights.md](../../reference/rights.md) |

The `-riplib` suffix on the directory name keeps this tree clear of the TeleGrafix client version numbers used throughout the repository; the reasoning is in the [3.1-riplib README](../3.1-riplib/README.md#why-the-directory-is-named-31-riplib).

## Compatibility design

RIPlib states the compatibility contract for v3.2 explicitly, and it holds up on inspection: every addition is **a new command letter unused in v3.0/v3.1, a new `$VARIABLE$` name, a new preprocessor directive, or a new value for an already-validated parameter field.** A v3.0 or v3.1 client sees the new content as either a no-op (unknown command letters pass through the parser's accept list) or as literal text (an unrecognized `$XYZ$` falls through unexpanded).

Against the fuller record reconstructed in this repository, that contract survives for the commands - `|^`, `|~` and gradient mode `2` are all clean - but **not for the variables.** Several of the §A2G.9-11 names collide with real, corpus-attested RIPscrip variables, and one of the new _behaviors_ silently replaces a documented one. The details are in [9.1 Text Variables](ripscrip/9.1-text-variables.md); this is the same pattern already seen in v3.1 and the main thing to resolve if the two efforts converge.

## Layout

```
3.2-riplib/
  ripscrip/    Language-visible additions - wire syntax and semantics
```

There is no `techspecs/` here: unlike §A2G.1-7, every §A2G.8-13 item is language-visible - new commands, new variables, a new directive, a new parameter value - so nothing falls to the implementer-only layer. The implementation-level v3.1 material (parser relaxations, fill patterns, floating-point rasterization, palette mapping) is in [3.1-riplib/techspecs/](../3.1-riplib/techspecs/README.md) and applies unchanged.

## Contents

**[Language additions (`ripscrip/`)](ripscrip/README.md)**

- **[2.0 Drawing State Stack](ripscrip/2.0-state-stack.md)** _(§A2G.8)_ - `|^` and `|~`, a bounded LIFO of the drawing prelude
- **[2.1 Radial Gradient](ripscrip/2.1-radial-gradient.md)** _(§A2G.13)_ - gradient mode `2`, and the gradient command's own standing in the record
- **[5.0 `<<DEBUG>>` Directive](ripscrip/5.0-debug-directive.md)** _(§A2G.12)_ - a host-side log line joining the 3.x `<<IF>>` macro layer
- **[9.0 Additions Reference](ripscrip/9.0-additions-reference.md)** - consolidated command, directive and version-string delta for v3.2
- **[9.1 Text Variables](ripscrip/9.1-text-variables.md)** _(§A2G.9, §A2G.10, §A2G.11)_ - layout/introspection, time-component and EGA color-name variables, each checked against the canonical inventory

## Further reading

- [3.0-riplib](../3.0-riplib/README.md) - where the two projects' accounts of RIPscrip 3.0 itself conflict, kept separate from the extensions
- [3.1-riplib](../3.1-riplib/README.md) - the revision this builds on (§A2G.1-7)
- [version/3.0/](../3.0/ripscrip/README.md) - the TeleGrafix 3.x record both extend
- [version/next/](../next/README.md) - this repository's own forward-looking proposals, an independent effort
- [reference/rip-tools.md](../../reference/rip-tools.md) - the RIPlib clone, and how to keep it current
