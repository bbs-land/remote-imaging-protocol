# Issue #309 — User-variable case conversion and TONAME words

[Issue #309](https://github.com/bbs-land/remote-imaging-protocol/issues/309) · [User-variable format options](../ripscrip/5.1-text-variables.md#user-defined-data-variable-format-options) · [Research backlog](issue-backlog.md)

RealDeuce filed #309 on 2026-11-25 at 19:37:38 UTC. The 19:46 UTC check found it with no comments. Branch `issue-309-user-variable-case-conversion` starts at fork stable `0b6805ee419dad26f64cd8be378e11383b3ff949`.

ALPHA 4 §4.2.1.10 supplies the ASCII editor modes and the three case conversions. It describes TONAME as capitalizing the first letter of each word and gives `TOM SMITH` as an example. The PC Micro RIPtel 3.1 help repeats the conversion entries at file offsets `0x47e60`, `0x47fbb` and `0x47fe6`. The help and draft leave the byte-level character class and boundary implicit. [ALPHA 4 text](../../2.0/text/RIPScrip-2.0-alpha-4.txt) · [Help extraction](riptel-help-extraction.md).

## Original converter and classification

Static inspection used PC Micro RIPtel 3.1's `artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll`, SHA-256 `83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8`. Driver addresses below are RVAs at image base `0x10000000`. The matching help SHA-256 is `ebf1e58682aa65177101fe1ab7acd4e29a6e14e3ca8e3d58a0417e06e2de6f2c`. [Artifact catalogue](../../../reference/rip-tools.md).

- The option decoder at `0xf16b` recognizes TOUPPER at `0xf23e`, TOLOWER at `0xf261` and TONAME at `0xf29c`. It assigns conversion selectors 1, 2 and 3 respectively. The call at `0xf2f2` passes the selected conversion to the value provider at `0xeb3c`.
- The provider dispatches conversion on a selected buffer, including the copied default and retrieved-value paths at `0xeda6–0xedde`, `0xeef4–0xef2c` and `0xefd5–0xf00d`. A successful prompt has the same conversion dispatch at `0xeb01–0xeb24`. Conversion changes the value that subsequent existing storage and expansion routes receive. [Directive and storage derivation](issue-87-user-variable-directives.md).
- TOUPPER goes through `0x65424` to `0x75820`. The default runtime-locale path compares each byte with `61` and `7A` and subtracts `20` in range at `0x7583b–0x75848`. TOLOWER calls `0x65935` and `0x759f0`, with the corresponding range `41`–`5A` and addition at `0x75a0b–0x75a18`. These default paths preserve bytes above ASCII.
- TONAME at `0x657bf` first calls the uppercaser at `0x657ce`. It then scans consecutive byte positions. At `0x6581e–0x6589d` it skips bytes classified as space, punctuation or digit. It tests the current byte against the runtime letter mask `0x103` at `0x658ce` and, after position zero, tests the immediately preceding byte with the same mask at `0x65908`. A hit on both calls the single-character lowercase helper at `0x65913`. Its default runtime path at `0x6bb71–0x6bb7b` lowercases only ASCII capitals.
- The initial narrow classification table pointer at RVA `0x87908` is VA `0x10087912`. For this image the table and pointer use .data file offsets equal to RVA minus `0x1a00`. The table gives ASCII space, punctuation and digits their respective flags `0x08`, `0x10` and `0x04`. All ASCII letters pass the mask `0x103` test. In particular, apostrophe and hyphen fail the previous-letter test, as does `2`. This gives `Anne-Marie`, `O'Neill` and `42Wallaby` for the report's inputs.

## Portable byte rule

Apply conversion to ASCII letters only. TONAME treats maximal ASCII letter runs as words. This preserves the original ASCII distinction at punctuation and digits and defines a boundary for every high byte. The native helpers have locale-dependent branches and TONAME uses the runtime classification table. The portable rule makes the treatment of high bytes independent of those services and of the host's encoding. A high byte is kept and begins a boundary before the next ASCII letter. For example, `TOUPPER` maps `caf\x82` to `CAF\x82`, and the portable TONAME mapping of `AB\x82CD` is `Ab\x82Cd`. This evidence concerns the transformation of value bytes under the existing variable lifecycle.

Validated all 16,129 ordered pairs of nonzero ASCII bytes by comparing an ASCII letter-run implementation with the two-step transformation using the original ASCII classification entries. Verified that each of the 128 high bytes passes through unchanged and separates the ASCII runs in the portable rule.

```sh
sha256sum "$dll" "$hlp"
strings -a -t x "$hlp"
objdump -d --start-address=0x1000f23e --stop-address=0x1000f2f7 "$dll"
objdump -d --start-address=0x100657bf --stop-address=0x10065935 "$dll"
objdump -d --start-address=0x10075820 --stop-address=0x100758d5 "$dll"
objdump -d --start-address=0x100759f0 --stop-address=0x10075aab "$dll"
```
