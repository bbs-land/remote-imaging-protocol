# Issue #18: YEAR syntax and exact DATETIME bytes

[Research backlog](issue-backlog.md) · [DATETIME](../ripscrip/5.1-text-variables.md#datetime) · [YEAR](../ripscrip/5.1-text-variables.md#year)

## Scope and source

Started 2026-11-16 after [PR #47](https://github.com/bbs-land/remote-imaging-protocol/pull/47) delivered #17. Branch **issue-18-date-time-format** starts at **314fae6**. [Issue #18](https://github.com/bbs-land/remote-imaging-protocol/issues/18) asks to correct YEAR's DOY syntax typo, DATETIME's weekday/date example and exact day padding/separators/newline. Do not choose the host platform's asctime/strftime as the oracle.

Primary artifact: unchanged RIPtel 3.1 / driver 3.0.7 `extracted/ripscrip.dll`, SHA-256 **83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8**, base **0x10000000**, from the [catalogued mirror](../../../reference/rip-tools.md), `artifacts/riptel-3.1-pcmicro/`. All code addresses are RVAs; .text file offsets are RVA-0xc00 and the .data string/table offsets below are RVA-0x1a00. Preserve original text/assets, use native/static work without GUI and remove task-owned temporary files.

## Original callbacks and first observation

Original registration initializer records DATETIME at **0x38f2a** (registration **0x25d91–0x25d9c**) and YEAR at **0x390ca** (**0x262b9–0x262c9**). DATETIME supplies literal **`%c`** at **0x79658**, YEAR supplies **`%y`** at **0x80534**, to shared callback **0x38e9a**. The shared callback accepts no arguments; nonzero count marks variable syntax error and returns NULL before reading the clock.

The callback initializes timezone data through **0x6f230**, reads current time through **0x68ff0**, converts it to local broken-down time through **0x69c90**, and passes its format string, result buffer and capacity **0x4f = 79** to wrapper **0x65b86**. That wrapper calls the original linked formatter **0x69210**, whose conversion dispatcher is **0x692e0**. This is not an asctime call.

The formatter's built-in time-locale pointer **0x87cf4** initially points at **0x87c48**. Its short-date template at +0xa0 / **0x87ce8** points to **0x87b40**, literal **M/d/yy**; its time template at +0xa8 / **0x87cf0** points to **0x87b24**, literal **H:mm:ss**. The `%c` branch **0x695d5–0x6961b** formats the short date via **0x69970**, appends exactly one ASCII space, then formats the time. A static reference search finds the locale pointer read in the dispatcher and no original text reference assigning a new value; preserve these shipped templates rather than substituting the host runtime's `%c`.

The first native checkpoint extends `run/riptel-macro-probe`: original registrations, callback/wrapper/formatter execute unchanged, with timezone/time/localtime replaced by controlled services returning nine original 32-bit tm fields. A Sunday 1993-12-05 14:38:50 fixture produces exactly **`12/05/93 14:38:50`**, length **17**, and YEAR produces **`93`**. The probe passes **66,224 macro/variable checks**. This disproves the inherited asctime-style example and all three proposed December-5 textual layouts. No GUI or clock setting occurs; the wrapper now also disables core dumps before building its automatically cleaned native executable.

## Exact padding and terminators

The template parser **0x69970** selects `%m`, `%d`, `%y`, `%H`, `%M`, `%S` for the shipped templates. Single-letter M/d/H initially set unpadded-number global **0x87cfc**, but the conversion dispatcher overwrites that global from percent-hash global **0x87cf8**, which the outer formatter cleared for plain `%c`. Thus even these single-letter template fields take the two-digit path: month **0x6961c–0x6963f**, day through the dispatcher’s analogous two-digit arm, hour **0x69366–0x69389**, minute **0x693ca–0x693ed**, second **0x693f5–0x69418**. YEAR's `%y` arm **0x6975f–0x6978a** takes tm_year modulo 100 and width 2. Numerical output **0x69880** repeatedly divides by ten and writes exactly the requested width with ASCII zeroes. Do not infer unpadded output from the literal M/d/yy or H:mm:ss alone.

Original formatter **0x692b0–0x692cb** appends a C-string NUL after the produced characters; no template or callback adds CR/LF. Returned DATETIME text is **17 ASCII bytes**, `MM/DD/YY HH:MM:SS`, followed by its storage NUL. There are two slash bytes (0x2f), exactly one space (0x20), two colons (0x3a), and six zero-padded two-digit fields. Hour is 24-hour 00–23. Neither weekday, month name nor four-digit year appears. YEAR is exactly two digits 00–99 followed by a storage NUL. Insertion copies the characters into surrounding macro text without inserting the NUL or a line ending.

## Native coverage and reproduction

Run from the checkout root, substituting the catalogued artifact root:

```sh
run/riptel-macro-probe /path/to/rip-tools/artifacts/riptel-3.1-pcmicro/extracted/ripscrip.dll
```

Observed with FreeBSD 15.1 amd64, clang 19.1.7 (`-m32 -O0 -Wall -Wextra -Werror -fno-pie -Wl,-no-pie`), and the SHA-verified original DLL. The script builds in a task-owned temporary directory, removes it through its EXIT trap, and disables core dumps. The existing [macro probe derivation](issue-6-macro-evaluation.md) describes registration, parser/insertion and supporting memory/string hooks; this extension replaces only timezone initialization **0x6f230** with a no-op, current-time **0x68ff0** with a counter/sentinel producer, and localtime **0x69c90** with controlled nine-32-bit-field tm data. Original callbacks **0x38f2a / 0x390ca**, shared callback **0x38e9a**, wrapper **0x65b86**, formatter **0x69210**, dispatcher **0x692e0**, template parser **0x69970** and numerical helper **0x69880** execute unchanged. No formatter or asctime/strftime output is supplied by the host. Expected strings use explicit decimal formatting of fixture fields.

The expanded probe passes **388,202 macro/variable checks**, including existing issue coverage:

- Every **24,837 valid Gregorian dates from 1970 through 2037**, at 00:00:00 and 23:59:59, with consistent weekday/year-day fields; leap dates and 1999/2000 rollover included.
- Every **86,400 time-of-day values** for Sunday 1993-12-05; exact 17 characters, NUL at offset 17 and an unchanged canary at offset 18.
- YEAR for each year 1970–2037: exact two digits and NUL, unchanged next-byte canary; 1999 → `99`, 2000 → `00`, 2005 → `05`, 2026 → `26`.
- Both original callbacks with one/two arguments: NULL and syntax-error flag, with no clock service call.
- Original dollar parsing and insertion of `A$DATETIME$B/$YEAR$C` and empty-parenthesis equivalents: exactly `A12/05/93 14:38:50B/93C`, no newline/truncation/error.

These are formatting tests after controlled local-time conversion, not tests of Win32 timezone setup, actual clock acquisition, daylight-saving transitions or supported timestamp range. The chosen fixture range does not assert behavior beyond it. The original disk artifact and built-in locale tables are unchanged; only execution-copy service hooks are patched. Earlier clients’ output has not been tested and is not generalized from this driver.

## Issue disposition

All requested bytes are settled. Correct YEAR Syntax to `$YEAR$`; document no parameters, two-digit zero padding and rollover. Replace the inherited DATETIME asctime assertion/example in the 3.0 reconstruction and 9.1 row with original-driver numeric output. December 5 produces `12/05/93 14:38:50`; December 19 produces `12/19/93 14:38:50`. December 19, 1993 was Sunday (native fixture calendar starts Thursday 1970-01-01), so the old Sat was independently wrong, but changing it to Sun would still leave the wrong output format. None of the issue’s three proposed textual layouts matches this original driver. Preserve verbatim source text and earlier client references.

## Next action

Update the 3.0 reference/9.1 rows, agent guidance and active TODO, run required checks, then file the complete #18 PR with an issue-only comparison from 314fae6.

---

[Research backlog](issue-backlog.md) · [Date/time reference](../ripscrip/5.1-text-variables.md#date-and-time-text-variables)
