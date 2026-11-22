# Issue #144: Remembered port-copy rectangles and PCB

[Research backlog](issue-backlog.md) · [PORT_COPY](../ripscrip/7.0-drawing-ports.md#rip_port_copy) · [PCB](../ripscrip/5.1-text-variables.md#pcb)

## Scope and checkpoint

Started 2026-11-22T08:30:11Z on `issue-144-remembered-port-copy-pairs` from fork stable **95ef6834d3cf8610cfb5940c8f998925be186213**. The in-session ten-minute check detected [#144](https://github.com/bbs-land/remote-imaging-protocol/issues/144) at 08:20:01Z, after creation at 08:19:40Z. Updated 2026-11-22T08:19:40Z, zero comments, body SHA-256 `b31b01bbc3cd8dfd4302605ace8433ae9191a254d8404931a537baca0217a884`. Next check due about 08:30:01Z. User explicitly confirmed research-and-PR processing. Keep the active-session monitor running and newer reports queued oldest first; no persistent scheduler.

Complete an explicit portable remembered-pair/PCB contract, preserving accepted #10 geometry, #42 capture/duplicate/failure rules, #128 logical auto-stamp footprint and #140 full PORT-family ownership. This is protocol completion, not native bug/pixel parity. **No new original-client/probe/bug investigation or execution**; use primary documentation and the existing accepted derivations only. No 1.54/text/assets changes.

## Sources and documentary boundaries

Read at starting stable 95ef683; reproduce with `git show 95ef683:<path>`:

- `version/3.0/ripscrip/5.1-text-variables.md`: SHA-256 `85114b09ba254a45ac245fbe2f0421d1872aa529f5e576cf50afc9539c956dca`.
- `version/3.0/ripscrip/7.0-drawing-ports.md`: SHA-256 `fa3d00d399189b9df3250dfd43a6aea56a72459e92dae2edfaadf3a58f4942ad`.
- `version/3.0/ripscrip/4.0-icons-and-bitmaps.md`: SHA-256 `383580f1e000240373e8c3dd7622698fc9e221d281a63362f94e8020d17d6a06`.
- `version/3.0/ripscrip/4.3-buttons.md`: SHA-256 `e57e621b97d7f607fec23cfa6512b48cef9beaedadb1e4821ecb4e0046c57d4a`.
- `version/3.0/ripscrip/7.2-data-backup-areas.md`: SHA-256 `54aac0782b99bfa09dcc1a457373acb990b903c640bebd8fd895a1c93e1345d3`.
- `version/3.0/ripscrip/1.3-world-view-virtual-canvas.md`: SHA-256 `e8207b9ed226c4e98551ae0ecdd9701a643f10045634277066872f66af9da223`.
- `version/3.0/research/issue-128-auto-stamp-redraw.md`: SHA-256 `96fbe0a30ed0f943cad986c84b8c1e00e4db55a10cd90fc32e5891d2c6ea9628`.
- `version/2.0/text/RIPScrip-2.0-alpha-4.txt`: SHA-256 `0da8a34644fa7c0ac7bf457ddfb6fe12288c1bff17c966f3d2b792af57c01982`.

ALPHA 4 §5.11.1 lines 17736–17790 establishes destination ownership of two rectangles after PORT_COPY, reversal into the currently selected port, viewport-relative placement, same-port eligibility and undefined/inactive no-op. Its `def=CUR` syntax conflicts with explicit prose specifying the assigned clipboard for omitted arguments: use the explicit prose, retaining explicit CUR/numeric selectors separately. The [shared dollar parser](../ripscrip/5.1-text-variables.md#variable-resolution-in-3x) already makes empty parentheses zero arguments. Primary port protection prose (ALPHA 4 lines 2028–2049 and accepted 7.0) permits copying contents **to/from protected ports**; do not invent a protection rejection for ordinary copy/paste merely because bookkeeping is stored in a port definition.

Existing [#10](issue-10-port-geometry.md) already supplies extent boundaries, sentinel precedence and pre-trim equal-versus-unequal classification/coupled versus independent trimming. Existing [#42](issue-42-clipboard-capture-state.md) supplies wire GET_IMAGE's 3×3→4×4 example, selection/redefinition/failure cases and LOAD duplication differences. Existing [#128](issue-128-auto-stamp-redraw.md) explicitly chooses logical effect footprint C, excluding overallocated capacity, and the documentary same-target capture guard. Existing [#140](issue-140-port-backup-entry-scope.md) and [#26](issue-26-screen-backups.md) establish whole-PORT backup/restore with definition/protection/cursor exceptions. These are adopted inputs, not new native observations.

The record does not fully define pair units/initialization, all producers, publication timing or reset/restore continuation. Mark those choices as portable completion, not recovered DLL layout/default bytes. In particular, no hidden buffer, private binary layout or new wire/query variable is introduced.

## Chosen completion and rationale

A live port owns an optional pair (Rs,Rd), initially absent. Both are positive half-open **DEVICE offsets relative to the corresponding producer viewport origins**, taken from the final rectangles actually transferred after WORLD conversion, sentinel expansion and existing clipping/fitting geometry. Store neither raw WORLD/sentinels nor uncopied pre-trim extents. WORLD changes do not rescale them. PCB adds the source port's current viewport origin to Rd and the current destination's origin to Rs, reversing them, then runs normal copy geometry in DEVICE units; it does not reinterpret them as WORLD or sentinels. PCB uses COPY mode 0, without changing global W or inheriting producer ROP. A stored scaled pair reverses fitting geometry, not a promise of inverse raster reconstruction.

Zero arguments (including PCB()) resolve only the assigned clipboard pointer. Empty/dangling pointer, valid selector of undefined/inactive source, inactive/undefined destination, absent pair or wholly clipped geometry is nonfatal no-op, with no implicit selection/creation/pointer repair/default pair. Explicit CUR means the current port; numeric decimal 0..35 selects that source. Callback-specific portable validation trims outer ASCII whitespace, accepts ASCII case-insensitive CUR or a complete decimal-digit string in range, and rejects malformed selectors/extra nonempty arguments under existing macro-error handling. Whitespace-only arguments are not omitted arguments. Existing dollar parsing/count/keyword rules govern malformed arguments below the void parser/session boundary.

Publish the final pair on the destination only after a nonempty successful named transfer: wire 2C, GET_IMAGE/LOAD duplicate/initial auto-stamp capture, PUT_IMAGE and PCB. Pure image-file drawing, wallpaper and button redraw copies are not separate publishers. Protected defined content targets permit publication as operational copy bookkeeping; protected **redefinition/capture replacement** keeps its existing rejection rules. New/replaced targets start absent; allocation/copy failure cannot invent a pair. Existing nontransactional target/pointer/pixel/current effects remain unchanged. No-op/rejection/failure on an existing definition retains its previous pair.

PCB snapshots its input pair before transfer, including same-owner case, then publishes its actual reversed transfer on the current destination. Repeated paste from a different source port retains that source's pair; PCB(CUR) publishes the reversed final geometry; with no further clipping the same-port example alternates pair orientation. Further trimming can change those extents, so unconditional alternation is not promised. Snapshot source pixels before overlapping writes, including logical screen ports sharing one backing surface; no exact scaling filter is prescribed. PUT_IMAGE retains its existing same-port no-op. Ordinary content drawing/erasure and viewport/metadata edits retain pair offsets; successful redefinition/backing replacement clears, except whole-PORT restore explicitly reinstates the saved optional pair. SOFT/drawing HEADER clears port-zero pair and removes deleted-port pairs, retaining protected surviving nonzero pairs; HARD initializes all pairs absent. Viewport-only reset and a header without definition reset preserve pairs. Save/restore owns pair state under existing whole-PORT exceptions; saving does not publish or draw.

Reject raw-WORLD/default-whole-viewport/pre-trim publication because it could replay uncaptured regions or change placement after WORLD changes. Keep one consistent destination-publication rule for named transfers rather than silently special-casing PCB; document same-port orientation explicitly. The pair stores geometry, not cached pixels, so later drawing changes what PCB reads without changing the pair. Auto-stamp C uses its logical viewport/crop, not extra capacity; wire GET_IMAGE retains its distinct adopted extent increments.

## Independent validation and completion

At 2026-11-22T08:42:33Z, an inline Python arithmetic/state check passed all eight starting-file SHA-256 values using `git show 95ef683:<path>`, rectangle origin subtraction/addition, the 0→5→0 and independently moved source/destination examples, WORLD-independent DEVICE offsets, equal-size coupled trim, unequal-size independent trim and a new PCB reclassification when the final extents become equal. It checked the 4×1→2×1 shorthand's final 1×1 pair, 4→2 and reversed 2→4 extents, GET_IMAGE's separate far/allocation increments, logical auto-stamp C versus backing capacity, PUT source-origin removal, the independent snapshot-overlap `ABCD`→`BCDD`→`BBCD` unit-cell model and deep-copy preservation of an optional PORT pair/absence. No native instructions, provider, GUI or renderer ran; these are abstract geometry/state checks rather than pixel sampling evidence.

The completed reference centralizes pair state under PORT_COPY, with crosslinks from PCB, GET_IMAGE, PUT_IMAGE, LOAD_ICON/LOAD_BITMAP, auto-stamp, reset, PORT backup and the variable index. The selector example starts every variant from an independent state so a preceding successful paste cannot silently initialize port zero. Lifecycle examples distinguish failed existing definition from destructive replacement, saved absence from missing backup, protected content copying from protected replacement, and viewport-only reset from full SOFT/HARD. The already adopted cursor exception is explicitly kept separate from the new saved pair rule.

The reason for destination publication after each named successful transfer is a uniform rule usable by capture and paste without a hidden clipboard buffer. Recording the actual final rectangles avoids replaying uncopied regions; storing DEVICE offsets preserves documentary viewport-relative movement without retroactive WORLD projection. Snapshot overlap and COPY mode make same-port paste definite while leaving raster sampling unspecified. Distinguishing protocol producers from internal rendering copies prevents button feedback/wallpaper implementation details from unexpectedly changing PCB's placement. These remain explicitly chosen portable rules, not claims of newly recovered native bytes.

Full `run/lint`, `run/check-links` and `git diff --check` pass; links report **0 bad of 6,313 across 260 files**. A focused prose-spacing scan found no missing word/number spaces. Before publication, #144 retained the original body hash, update time and zero comments.

## Delivery and next action

[PR #145](https://github.com/bbs-land/remote-imaging-protocol/pull/145) is open/ready with specification **e3aec0a914e5d3d9d4aac337b7a70abb4f4dd8a5** and exactly #144 as its closing reference; GraphQL verifies no closing-reference pagination overflow. The issue body/update time and zero comments are unchanged. Scoped `TemporaryDirectory(prefix='rip-144-pr-')` request payloads were removed automatically. No upstream merge or manual closure.

Normal-merge the actual completed source branch head into fork stable with an identical source tree before workflow notes, audit all coverage/feedback/ancestry and preserve the separate FastFont branch/co-author trailers. Then resume active-session ten-minute monitoring; no persistent scheduler is installed.

---

[Research backlog](issue-backlog.md) · [Agent workflow](../../../AGENTS.md)
