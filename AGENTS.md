# Agents

Guidance for **automated (AI) agents** working in this repository. Conventions that apply to any author - human or AI - live in [CONTRIBUTING.md](CONTRIBUTING.md); read and follow it first. This file covers only what is specific to agent workflows.

## Ground rules

- Follow [CONTRIBUTING.md](CONTRIBUTING.md) for all content, naming, and formatting conventions - especially the version naming (language vs client) rule, the verbatim `version/<v>/text/` record, and the Markdown style section.
- External reference repositories and original-era artifacts are catalogued in [reference/rip-tools.md](reference/rip-tools.md). The user authorized `/usr/home/admin/RIP/rip-tools/` as this workspace’s persistent mirror of `~/src/rip-tools/`; preserve the catalogue’s relative paths under that root. Keep artifacts outside the specification checkout and identify them by provenance and hashes, not the machine-specific root.
- After edits, verify with the repository's own scripts: `run/check-links` and `run/lint` (add `run/format` to fix what `run/lint` reports).

## Use the `run/` scripts

Repository scripts live under `run/`; their catalogue and conventions are in [run/README.md](run/README.md). Read it before writing a new one, and do not create new top-level script directories.

- **Reach for an existing `run/` script before hand-rolling a command.** They encode the repository's configuration - ignore lists, pinned tool versions, slug rules - and stay correct as that configuration changes, which an improvised `find`/`grep`/`npx` pipeline will not.
- **After renames, relocations, or any bulk edit, run `run/check-links`** - moving a file breaks inbound links and anchors silently, and this is the only thing that catches it.
- **Format with `run/format`, not by hand** - never hand-wrap prose or hand-tune Markdown spacing to match the style; let the formatter do it, then confirm with `run/lint`.
- **If you find yourself repeating an incantation, add it as a `run/` script** rather than repeating it across sessions - following the conventions in [run/README.md](run/README.md), including the `deno task` wrapper and the table row.

## Commit attribution

The user requests Codex co-authorship for this work. Add the trailer `Co-authored-by: Codex <codex@openai.com>` to every new commit.

## Session work log (`DONE.md`)

`DONE.md` is the agent-facing work log for a session or workflow - it records what actually changed, with dates and short result summaries, so work can be resumed or reviewed mid-session. It affects agents only; it is in `.gitignore`, is never committed, and is cleared regularly as changes are committed to git. Treat it as scratch: **check that the file exists before trying to edit existing content in it** - if it is missing, start a fresh one rather than assuming prior entries survive. The committed backlog stays in [TODO.md](TODO.md), which is updated as tasks start and finish (see [CONTRIBUTING.md](CONTRIBUTING.md#repository-conventions)); do not link to `DONE.md` from committed documentation, since it will not exist in a clean checkout.

## Serial primary-source issue research

- Resolve open issues filed by RealDeuce in creation order (tie-break by issue number), beginning with #3. Refresh GitHub state before choosing the next issue; skip issues already covered by a complete resolution PR. Research one issue at a time and stay on it if evidence remains incomplete.
- Read the committed [research backlog](version/3.0/research/issue-backlog.md) and its active issue note before continuing. Record the branch, PR, current evidence gaps and exact next action there. Keep TODO.md active-only; DONE.md is disposable and must never be the only derivation record.
- Write derivation notes incrementally under `version/3.0/research/issue-<number>-<topic>.md`, checkpointing them in local commits. Record source URLs, hashes, original versus patched artifact identity, file offsets versus RVAs, tool versions, reproducible commands and inputs, observations, reasoning, rejected interpretations and remaining gaps. Link the specification to these notes.
- Never run Wine or another GUI research probe on the user’s desktop. Use a dedicated Xvfb display and a task-specific Wine prefix, explicitly set `DISPLAY` for every related process, redirect diagnostics to the artifact mirror, and clean up only the probe’s own processes. Font-error dialogs interrupted the user on 2026-11-16; a separate Wine prefix alone does not isolate its windows.
- Verify leads from other implementations against primary evidence. Trace parsing, validation, state changes and observable effects; help strings and corpus silence alone do not establish a complete contract. Clearly distinguish facts from hypotheses, and shipped defects from intended behavior. Do not generalize findings to earlier clients without evidence from those clients.
- For native i386 driver probes, preserve the original ABI: some routines return status only in AX, adjacent one-byte RET synchronization stubs cannot both receive five-byte jump hooks, and original string insertion temporarily reverses its source buffer. Keep fixture return buffers mutable and record harness failures separately from driver behavior. Exact hook lists and controlled-service limits belong in each issue note.
- For bitmap native probes, preserve signed low-word argument semantics where original helpers read only 16 bits; upper argument-slot bits may be stale. DC fixtures must track SelectObject’s previous-object return. Keep original decoding/compositing separate from controlled GDI RGB operations and never claim Windows stretch/palette parity from a fixture. See the [bitmap derivation](version/3.0/research/issue-8-bmm-masks.md) for exact boundaries.
- For port geometry, distinguish stored rectangle extent boundaries from last addressable pixel positions and from unrelated command conventions. Preserve the original copy compositor’s scale decision before clipping. A query that fails natively must be traced through its success/error branches before replacing the expected result; see the [port derivation](version/3.0/research/issue-10-port-geometry.md), including the PORTW/PORTH inverted gate.
- For point/pen research, distinguish the rectangle used for invalidation from the coordinates sent to the actual drawing call. Shared world conversion does not establish aliases: X omits its actual origin offset, POINT expands by the stored thickness, and the viewport producer itself omits the screen-port-origin addition. Trace producers and consumers; the [lines derivation](version/3.0/research/issue-11-pixel-pen-units.md) records exact original instructions and GDI-fixture limits.
- For fill-pattern probes, track brush, pen and bitmap selections independently in each DC. Distinguish original brush construction/drawing arguments from the documented GDI phase contract: native stand-ins do not execute Windows rasterization. Screen ports share a context, offscreen bitmaps have independent origins, and bitmap copies preserve already painted pixels. See the [fill-mask derivation](version/3.0/research/issue-12-fill-masks.md).
- For polygon parsing, distinguish fixed count widths from repeated coordinate widths and aggregate numeric capacity. The original multi-contour descriptor makes later contour counts XY-wide; the parser truncates after 8193 values but still dispatches. Single-contour handlers convert N+1 pairs while passing N to GDI. Keep readable sentinel slots and never invoke a handler on a deliberately truncated oversized fixture. See the [polygon derivation](version/3.0/research/issue-13-polygon-counts.md).
- For reply contracts, distinguish the producer’s exact bytes from client-configured output translation and C-string storage terminators. Identify an explicit generic-implementation interoperability policy separately from historical client identity bytes.
- Before filing a resolution PR, answer every issue subquestion, preserve reproduction cases, format changed Markdown with `run/format`, and run `run/lint`, `run/check-links` and `git diff --check`. New reusable tools follow `run/README.md`.
- The user authorized the RealDeuce fork and issue-specific branches. Keep `origin` pointing to bbs-land and `fork` pointing to RealDeuce. File one complete resolution PR per issue, with evidence, validation and `Closes #N`; do not merge or manually close issues. Continue after filing. Stack dependent PRs on earlier unmerged branches when necessary and retarget after merge.

## Parallel worker agents

When running parallel worker agents, limit the number of parallel agents to 3 unless told otherwise, letting them work on batches. Use a `WORKING.md` file in the project directory to coordinate such work - tracking a work summary, testing details (if any), the immediate task list, and the list of agents as they start, progress, and finish a given task. This should allow for relatively easy continuation of broken or incomplete work due to a session window limit. Ask agents to list their batch tasks and progress in `./temp/agent-X.md` while working, and remove the file when done.

`WORKING.md` and `temp/` are in .gitignore and will not be committed or checked in, they will also be regularly deleted, so ensure the file exists before making asumptions when starting new work tasks.

- Post-PR #13 lower-count follow-up: zero direct count parses then diagnoses Insufficient parameters before drawing; one vertex is passed ascount1 to GDI, which requires2 and draws no point; two-vertex P is an ordinary accepted degenerate closed outline. Persistent RIP pen/style/cursor records are unchanged. The [small-count note](version/3.0/research/issue-13-polygon-counts.md#post-pr-small-count-results) records223,367 native checks and official GDI contracts. Update PR #41 and propagate to the latest stack; no Windows raster parity is claimed.
