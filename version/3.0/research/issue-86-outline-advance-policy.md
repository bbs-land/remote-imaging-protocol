# Issue #86: outline advances and cached cursor width

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)

## Scope and checkpoint

Resolve [#86](https://github.com/bbs-land/remote-imaging-protocol/issues/86) on branch `issue-86-outline-advance-policy` from fork stable **5224b70**. #87 user-variable prefix/repetition/scope/transparency follows. Both are open new reports without comments at the 2026-11-19 13:12:10 UTC refresh; no later repository discussion/inline comments were found.

Preserve adopted #24 cursor stores, #3 spacing, #22 unsigned renderer mapping and #82 global bbox definitions. Specify ordinary scalar advances and DPI/face/rounding/kerning, an independent finite unsigned cached-width contract, and cache lifecycle/cursor examples. Use protocol documentation and read-only original metric producers/callers. Explicit protocol rules control; identify genuine gaps and label any proposed interoperability policy instead of requiring native narrowing, stale-memory behavior or unsafe byte-index loops. No original client/UI/unsafe loop is to be executed or debugged.

## Next action

Trace per-code metric producers, axis/face scaling, renderer and cached-width call sites, and documented cursor/font semantics. Record exact equations, source/inference/policy distinctions and independent vectors before publishing a complete PR closing #86 and consolidating into fork stable. Then start #87. Retain co-author trailers and temporary-file cleanup.

---

[Research backlog](issue-backlog.md) · [Graphics text](../ripscrip/3.0-text-output-and-fonts.md)
