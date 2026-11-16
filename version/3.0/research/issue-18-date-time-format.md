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

## Next action

Retain original formatting, expand native checks across valid dates/single-digit fields/year boundaries and exact terminators, verify invalid arity, and correct the reference plus 9.1 rows to the actual numeric DATETIME format. Record the erroneous historical weekday only as a withdrawn example, not as the intended output format. Run required checks and file the complete #18 PR once all bytes are settled.

---

[Research backlog](issue-backlog.md) · [Date/time reference](../ripscrip/5.1-text-variables.md#date-time-information-text-variables)
