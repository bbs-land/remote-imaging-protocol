# Agents

Guidance for **automated (AI) agents** working in this repository. Conventions that apply to any author - human or AI - live in [CONTRIBUTING.md](CONTRIBUTING.md); read and follow it first. This file covers only what is specific to agent workflows.

## Ground rules

- Follow [CONTRIBUTING.md](CONTRIBUTING.md) for all content, naming, and formatting conventions - especially the version naming (language vs client) rule, the verbatim `version/<v>/text/` record, and the Markdown style section.
- External reference repositories and original-era artifacts are catalogued in [reference/rip-tools.md](reference/rip-tools.md) - use the `~/src/rip-tools/` paths exactly as listed there.
- After edits, verify: `python3 tools/check-links.py` and `npx prettier --check` on the touched Markdown.

## Parallel worker agents

When running parallel worker agents, limit the number of parallel agents to 3 unless told otherwise, letting them work on batches. Use a `WORKING.md` file in the project directory to coordinate such work - tracking a work summary, testing details (if any), the immediate task list, and the list of agents as they start, progress, and finish a given task. This should allow for relatively easy continuation of broken or incomplete work due to a session window limit. Ask agents to list their batch tasks and progress in `./temp/agent-X.md` while working, and remove the file when done.

`WORKING.md` and `temp/` are in .gitignore and will not be committed or checked in, they will also be regularly deleted, so ensure the file exists before making asumptions when starting new work tasks.
