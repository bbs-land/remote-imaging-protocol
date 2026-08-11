# Agents

Guidance for **automated (AI) agents** working in this repository. Conventions that apply to any author - human or AI - live in [CONTRIBUTING.md](CONTRIBUTING.md); read and follow it first. This file covers only what is specific to agent workflows.

## Ground rules

- Follow [CONTRIBUTING.md](CONTRIBUTING.md) for all content, naming, and formatting conventions - especially the version naming (language vs client) rule, the verbatim `version/<v>/text/` record, and the Markdown style section.
- External reference repositories and original-era artifacts are catalogued in [reference/rip-tools.md](reference/rip-tools.md) - use the `~/src/rip-tools/` paths exactly as listed there.
- After edits, verify with the repository's own scripts: `run/check-links` and `run/lint` (add `run/format` to fix what `run/lint` reports).

## Use the `run/` scripts

Repository scripts live under `run/`; their catalogue and conventions are in [run/README.md](run/README.md). Read it before writing a new one, and do not create new top-level script directories.

- **Reach for an existing `run/` script before hand-rolling a command.** They encode the repository's configuration - ignore lists, pinned tool versions, slug rules - and stay correct as that configuration changes, which an improvised `find`/`grep`/`npx` pipeline will not.
- **After renames, relocations, or any bulk edit, run `run/check-links`** - moving a file breaks inbound links and anchors silently, and this is the only thing that catches it.
- **Format with `run/format`, not by hand** - never hand-wrap prose or hand-tune Markdown spacing to match the style; let the formatter do it, then confirm with `run/lint`.
- **If you find yourself repeating an incantation, add it as a `run/` script** rather than repeating it across sessions - following the conventions in [run/README.md](run/README.md), including the `deno task` wrapper and the table row.

## Session work log (`DONE.md`)

`DONE.md` is the agent-facing work log for a session or workflow - it records what actually changed, with dates and short result summaries, so work can be resumed or reviewed mid-session. It affects agents only; it is in `.gitignore`, is never committed, and is cleared regularly as changes are committed to git. Treat it as scratch: **check that the file exists before trying to edit existing content in it** - if it is missing, start a fresh one rather than assuming prior entries survive. The committed backlog stays in [TODO.md](TODO.md), which is updated as tasks start and finish (see [CONTRIBUTING.md](CONTRIBUTING.md#repository-conventions)); do not link to `DONE.md` from committed documentation, since it will not exist in a clean checkout.

## Parallel worker agents

When running parallel worker agents, limit the number of parallel agents to 3 unless told otherwise, letting them work on batches. Use a `WORKING.md` file in the project directory to coordinate such work - tracking a work summary, testing details (if any), the immediate task list, and the list of agents as they start, progress, and finish a given task. This should allow for relatively easy continuation of broken or incomplete work due to a session window limit. Ask agents to list their batch tasks and progress in `./temp/agent-X.md` while working, and remove the file when done.

`WORKING.md` and `temp/` are in .gitignore and will not be committed or checked in, they will also be regularly deleted, so ensure the file exists before making asumptions when starting new work tasks.
