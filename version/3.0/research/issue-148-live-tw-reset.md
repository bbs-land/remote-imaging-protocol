# Issue #148: Live text-window reset state and ordering

[Research backlog](issue-backlog.md) · [RESET](../ripscrip/5.1-text-variables.md#reset) · [Text windows](../ripscrip/1.4-terminal-ansi-view.md)

## Scope and checkpoint

Started 2026-11-22T17:39:24Z on `issue-148-live-tw-reset-contract` from fork stable **4c8672d935e9faccdb74eb702ab7c056cf24071e**. The scheduled issue check at **17:36:09Z** detected [#148](https://github.com/bbs-land/remote-imaging-protocol/issues/148), created/updated 17:35:54Z, zero comments, body SHA-256 `a8f99157aeef3387dd8ca3f5caa67bc5968ad6f4a9d86056ba21c8c6844f7460`. Next check due about 17:46:09Z. A separate queued connectivity-test message requested no protocol work; this report is processed under the user's standing research-and-PR authorization, not the test.

Complete live RESET(TW) definition, query, selection/protection and prevalidation/runtime continuation. Adopt #16/#100/#101/#105/#106 and the connection parser/screen ownership boundary. **No new original-client/probe/bug investigation or execution**, no pixel parity or 1.54 change.

## Starting sources and documentary limits

Hashes, reproducible with `git show 4c8672d:<path>`:

- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `8b4eaf03875a00a9e8261188784a8a5d978ce8f2f7aa5e23f652ad7b16c235fd`.
- `version/3.0/ripscrip/1.4-terminal-ansi-view.md`: SHA-256 `bf4eb5adc3b0134a2c77687f1667a4db99219b83a0067316e7ce30f4b721ccef`.
- `version/3.0/ripscrip/1.3-world-view-virtual-canvas.md`: SHA-256 `11a01cdf019368d24deee857404a8ae3b8302438aed095ec64b0424f08eb40c9`.
- `version/3.0/ripscrip/7.1-data-tables.md`: SHA-256 `b402c0b7d2336ef3af52911843fb716b5670d8ba9f84712baec60d30cad15ca9`.
- `version/3.0/research/issue-100-ordinary-text-window-state.md`: SHA-256 `25ed5048bb063e4a257eca5b129da0b87419b234be83355bd3d7b47e2d758d26`.
- `version/3.0/research/issue-101-extended-text-window-contract.md`: SHA-256 `467df2fb7f4800f68838082628b3ae839f6856c6f04f421c3322840b17b7e8b9`.
- `version/3.0/research/issue-105-outer-soft-reset.md`: SHA-256 `c3c63d2e4de8d6464a1165656fdc07312ab03c9b9e852c2e843ef0e4ddd6a60a`.
- `version/3.0/research/issue-106-text-window-selection.md`: SHA-256 `db4d51fe2df2808cf1d3b037c272f1a0ffc7f199e900cac73897bea5f9ffefd5`.
- `version/3.0/research/issue-51-deferred-query-order.md`: SHA-256 `1f031f1eb2608720ad60402744f9e3f67128ecf0a03b52247044cff1f529fa7a`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

ALPHA 4 §5.9.1 (RESET, line 16598 onward; TW prose/selector table at 16701–16732) establishes slot-zero fullscreen/no clear, nonzero deletion, fallback only when the current nonzero target is reset, current default, and independent backup deletion. Its final paragraph at 16930–16937 establishes unresettable/protected no-ops and whole-request rejection for any invalid parameter. It does not enumerate the new TW 0 record, query ownership or runtime/resource continuation. §5.6.10 MTW (14759 onward) instead expressly retains the window font/kind, ANSI colors and disabled cursor; it must not silently supply all RESET defaults. CUR's unused-slot table at 18545–18547 expressly uses the user default font.

Accepted ordinary `w` (#100) has an identical-definition retention branch and retains tabs/charset even after changed geometry; accepted extended `b` (#101) has its own reinitialization boundary. Accepted CUR first use (#106) already enumerates fresh logical defaults: active ordinary full-screen default-font definition, cursors zero, visible/default cursor request, 7/0 normal rendition/modes, full margins, empty explicit tab map, US/US/G0, no resident query. Outer SOFT (#105) separately resets the departing current window, clears the physical screen and floating queries and then initializes/selects TW 0. None of those broader effects follows from a live target reset. The larger ESC-7/8 snapshot and partial ANSI parser are interpreter-owned, distinct from CSI-saved per-window coordinates.

## Explicit portable completion to publish

A successful live reset of slot 0 installs the fresh **CUR-first-use logical defaults**, regardless of previous kind/geometry/inactivity and without ordinary `w`'s identical-definition bypass. It uses the user's current default font, full nominal grid/ordinary placement and user wrap preference, not the retained font or fixed font 2. It clears TW 0's fixed query. Requested cursor shape resets to the ordinary fresh-window default request, not the old request; exact cursor raster remains renderer-defined. This reuse is a specified state recipe, not selecting/creating a different slot or executing a CUR macro.

Deleting a defined unprotected nonzero window drops its fixed query; if current, select **existing TW 0 without resetting or activating it**. Protected/undefined targets are no-ops, including query/selection. Numeric 0 applied while another window is current resets 0 only. TBL visits 0 then 1..35, skipping protected nonzero records: protected current nonzero remains selected; deleted current falls back to the newly reset 0. ALL performs TBL then TW BACKUP. Live numeric/CUR/TBL operations retain independent backups. Backup-only operations retain live selection/definitions/queries.

The floating TW query is separately owned and **retained by live TW reset, including TBL/ALL**. This explicitly chooses entry reset scope rather than the separate outer SOFT/shared QUERY reset effect. Protected survivors retain their fixed queries. No stored expression executes during reset; a saved backup query is independently owned under the existing family rules.

Validate the whole argument list structurally before any live/backup write. Keep CUR symbolic through validation and resolve it at each left-to-right operation. Unresettable no-ops do not abort later selectors. Portable runtime completion: prepare a live target's replacement resources before logical commit; an actual target-operation failure leaves that target's logical state/query and selection unchanged, retains earlier completed operations and continues later selectors. TBL uses the same per-entry rule; ALL still reaches BACKUP after live failures. This adds no global rollback or Boolean/session termination and makes no claim about native failure recovery.

Reject MTW aliasing, conditional ordinary-`w` redefinition, source cloning and outer SOFT equivalence: each retains/resets different fields. Reject pre-resolving CUR or applying a valid prefix before a late syntax error. Fresh zero state is an explicitly chosen reset recipe, not a newly observed native initializer result. Separate entry-local versus shared query ownership preserves protected survivors without introducing a global query deletion side effect.

## Independent state validation

An inline Python logical model checked all ten baseline SHA-256 values against `git show 4c8672d:<path>`, then verified the published deletion-versus-fresh-zero, protected-current TBL/ALL, sequential CUR, late-invalid-selector and first-attempt/zero-replacement failure examples. An exhaustive current/target/status matrix checked **3,781 valid numeric cases** (all 36 indices, writable/protected/undefined states excluding impossible protected/undefined slot 0 and undefined current targets), preserving unrelated records, backups, floating query, parser/ESC snapshot, entry pixels, graphics selections, global lock and emulation. Five default-font grid/margin bounds were checked against the nominal table. These are pure policy/state arithmetic checks, not native execution or a terminal raster/parser oracle.

The reference centralizes the live recipe and target/selector/failure tables under RESET, with crosslinks from the terminal lifecycle/charset table, outer SOFT, TW table and variable index. It explicitly distinguishes requested cursor defaults from pixel shape realization, dynamic CUR from prevalidation, protected current retention from outer SOFT's zero selection, and fixed-entry queries from the separate floating TW query. Forced zero reset bypasses ordinary `w`'s identical-definition path while preserving connection-owned parser and larger ESC-7/8 snapshot.

Full `run/lint`, `run/check-links` and `git diff --check` pass, with **0 bad of 6,349 links across 262 files**. A focused prose-spacing scan passes. Before publication, #148 retains its original body/update time and zero comments. The 17:46:28Z all-author issue check found no newer report; next due about 17:56:28Z.

## Delivery and next action

[PR #149](https://github.com/bbs-land/remote-imaging-protocol/pull/149) is open/ready, specification **79e92a8846a696efac0a12fb80341615c4d8094a**, with exactly #148 as closing reference and no pagination overflow. The report retains its initial body/update time and zero comments. Scoped `TemporaryDirectory(prefix='rip-148-pr-')` request files were removed automatically. No upstream merge/manual closure or original execution.

Normal-merge the actual completed source head into fork stable with an identical source tree before workflow notes, audit all coverage/feedback/ancestry, preserve FastFont separation and Codex trailers, then resume active-session ten-minute monitoring. No persistent scheduler.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
