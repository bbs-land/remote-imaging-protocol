# Issue #3: shipped extended font style derivation

[Research backlog](issue-backlog.md) · [Current command reference](../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style)

## Status and questions

Investigation started 2026-11-16 for [issue #3](https://github.com/bbs-land/remote-imaging-protocol/issues/3). No field semantics have yet been established by this investigation. The existing page contains hypotheses; do not promote them to facts without tracing the original driver.

Required answers: each fixed field’s offset, width, radix, role, units, validation, defaults and effects; all style and justification flags; string and character rotation; both `CC` fields; `BB`, `WW` and `1a1a`; reserved fields; font selection/fallback; exact decode of `!|y0000BW1Q080000001a1a000000Marin`; and independently checked rotation cases. Investigate reset/state interactions and invalid-command behavior as needed to make the contract complete.

## Sources and initial observations

- Specification baseline: repository commit `2fb17724b6122a5ad5cd1df38b69b8cce3a7079f`.
- [Issue #2](https://github.com/bbs-land/remote-imaging-protocol/issues/2) offers disassembly leads from a 592,896-byte `RIPSCRIP.DLL`, MD5 `bade8b1f4e467ac7ad4edb2639738d4c`. These are unverified leads, including the claim that `y` always uses base 64.
- The catalogue’s extracted RIPtel tree is absent on this machine. The repository README links a PCMicro-patched MSI; obtain and fingerprint its driver, distinguishing it from original releases.
- Initial validation: `run/check-links` passed (4,477 links across 181 files); `run/lint` passed. Host: FreeBSD 15.1 amd64, Wine 11.0; `7z`, `objdump`, `llvm-objdump`, GDB and `i686-w64-mingw32-gcc` are available. Driver execution has not been verified.

## Next step

Download the installer to the external artifact mirror, extract without running the installer, identify its driver format and hashes, and locate the `y` handler and argument descriptor. Record exact commands and findings below before drawing conclusions.

---

[Research backlog](issue-backlog.md) · [Current command reference](../ripscrip/3.0-text-output-and-fonts.md#rip_extended_font_style)
