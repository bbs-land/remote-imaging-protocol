# Issue #171: FILEEXISTS text variable

[Research backlog](issue-backlog.md) · [Original report](https://github.com/bbs-land/remote-imaging-protocol/issues/171)

## Source and scope

Started **2026-11-23T16:39Z** on `issue-171-fileexists` from fork stable **b5b860850fd077fcdcafec62017b2647f317ad83**. The RealDeuce report was created and last updated at **2026-11-23T07:39:05Z** with no comments. Original body SHA-256 `3a04ac4c2ea4e7929ad16112438edf04b9b947e00e1d97c8947e19e1149db401`. The [local scene derivation](issue-170-local-scene-boundaries.md#catch-up-queue-and-watch-status) records the later queued reports #172 through #175.

Read the catalogued original RIPtel 3.1 `ripscrip.dll`, 592,896 bytes, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Addresses are RVAs from image base 0x10000000. Use LLVM objdump 19.1.7 with image-base-plus-RVA start/stop addresses, or `run/riptel-inspect` for byte verification. The [BMM routing audit](issue-8-bmm-masks.md#completed-routing-audit) gives the mask alias call. The [FILE_QUERY derivation](issue-168-file-query-replies.md) supplies the common tail predicate and lookup helpers used for comparison. No original client execution.

## Callback trace

- **0x25e06–0x25e19** registers name literal **0x7e620** with flag **1** (data-capable) and callback **0x28481**. The [ordinary and graphical gates](../ripscrip/5.2-templates-and-conditionals.md#eligible-providers-and-rejected-codes) admit that class. No separate active operation or send callback.
- **0x28490** requires one parsed argument. Other counts reach the `No filename parameter specified` diagnostic at **0x7e95c**, including counts above one. **0x284a7** calls tail helper **0x3f71a**. Null or empty tail reaches `Invalid filename parameter` at **0x7e928**. Both diagnostic paths return null and set the shared callback error word **+0x436** at **0x2858d**.
- **0x25a7b** clears the callback error word before dispatch. A null result with the error word set returns reject status 2 at **0x25b17**. The ordinary wrapper at **0x25b88** blocks same-name user fallback. Its scanner preserves and advances past the rejected reference. A malformed argument that fails the dollar parser never invokes the callback. This is distinct from a valid missing file's substituted ASCII `0`.
- **0x284d2** calls the DOS path splitter **0x65660** with null drive/directory outputs. **0x284f1** reconstructs filename plus extension only through **0x65766**, and **0x28500** uppercases it. A drive such as `C:` provides no search root. There is no extension default, wildcard interpretation, playback substitution, second argument expansion or automatic bitmap companion query.
- Calls **0x28519**, **0x28532**, **0x2854b** to **0x65635** run three ordered substring rewrites, unconditionally in sequence. Each finds the first old marker and overwrites it and the remaining bytes. Constants at **0x7a904/0x7a90c**, **0x7a8f4/0x7a8fc**, **0x7a8e4/0x7a8ec** give `.ICN→.BMP`, `.HIC→.BMH` and `.MSK→.BMM`. Unlike the handler for `1F`, one rewrite does not skip later rules. For example, `A.MSK.ICN` becomes `A.MSK.BMP` and then `A.BMM`.
- **0x2855d** calls `0x3f80e(context,name,0,0)`. The final zero disables retrieval. Resource resolution uses host directory, shared ICONS and configured search path, in that order. The callback treats missing result 1 as literal **0x79714** (`0`), successful lookup as literal **0x7970c** (`1`). It performs no open, raster interpretation, length/date calculation or use of FILE_QUERY reply formats. The enclosing consumer substitutes the byte and continues.

## Next action

Publish the variable entry with ordinary/graphical vectors for hit, miss, explicit mask alias and rejected arguments. Check the processed name stages, distinction from FILE_QUERY and exact result bytes, validate the reference, and deliver one PR closing #171.
