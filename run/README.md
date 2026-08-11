# Repository scripts (`run/`)

Every script this repository ships - checkers, formatters, generators, one-off utilities - lives here and is invoked from the repository root:

```sh
run/check-links          # validate every Markdown file/anchor link
run/lint                 # report Markdown files that are off-style
run/format               # rewrite Markdown files to the repository style
```

**Deno** is the canonical runtime. Nothing here needs `npm install` or a `node_modules/` directory - Deno fetches and caches whatever a script declares, and [`deno.lock`](../deno.lock) pins the resolved versions. Content, naming, and formatting conventions for the documentation itself live in [CONTRIBUTING.md](../CONTRIBUTING.md); this page covers the scripts.

## Available scripts

| Script | What it does | `deno task` equivalent |
| --- | --- | --- |
| [`check-links`](check-links) | Validates every `[text](path#anchor)` link across the doc trees - the target file must exist and the `#anchor` must match a heading slug. Skips fenced and inline code. Exits 1 on any broken link. | `deno task check-links` |
| [`lint`](lint) | Reports Markdown files that do not match [`.prettierrc`](../.prettierrc). Reports only, never modifies. Exits 1 if any file is off-style. | `deno task lint` |
| [`format`](format) | Rewrites Markdown files in place to the same style. The fixing counterpart to `lint`. | `deno task format` |

All three accept explicit targets (`run/lint README.md`, `run/check-links version`) and default to the whole repository. All three take `-h`/`--help`. Run them from anywhere - each resolves the repository root itself.

**Prefer these scripts over ad-hoc equivalents.** When a task is already covered here - checking links after a rename or relocation, reformatting after a bulk edit - use the script rather than hand-rolling a `find`/`grep`/`npx` pipeline. They encode the repository's configuration (ignore lists, pinned versions, slug rules) and stay correct as that configuration changes. If a recurring task has no script yet, add one here rather than repeating the incantation.

## Conventions

- **The files directly under `run/` are the entry points.** They are **extensionless**, marked **executable**, and carry a shebang: `#!/usr/bin/env bash` for shell scripts, `#!/usr/bin/env -S deno run <permissions>` for TypeScript. Grant the narrowest permission set the script actually needs - `check-links` asks only for `--allow-read`. Git tracks the executable bit; if a new script lands without it, `git update-index --chmod=+x run/<name>`.
- **Bash or TypeScript only**, and prefer TypeScript unless the task is genuinely shell plumbing (`lint` and `format` are little more than a pinned Prettier invocation, so they are Bash).
- **Prefer `node:*` APIs over `Deno.*`** in TypeScript scripts - `node:fs`, `node:path`, `node:process`, `node:util`. Deno is the expected runtime for now, but portability to Node and Bun is worth more than the ergonomic edge of the Deno globals; reach for `Deno.*` only where no `node:` equivalent exists.
- **Supporting modules live under `run/lib/`** _with_ their extensions ([`lib/dependencies.ts`](lib/dependencies.ts), [`lib/prettier.sh`](lib/prettier.sh)); entry points import or source them. Nothing under `run/lib/` needs a shebang or the executable bit, and nothing there is meant to be run directly.
- **Third-party dependencies are declared in one place per language.** TypeScript imports go through [`lib/dependencies.ts`](lib/dependencies.ts), which imports from `jsr:`/`npm:` and re-exports what the scripts use; command-line tools invoked as `deno run npm:<tool>` pin their version in a shared `run/lib/` helper the same way ([`lib/prettier.sh`](lib/prettier.sh)). Prefer `jsr:` over `npm:`, and do not scatter `jsr:`/`npm:` specifiers through the entry points. Popular, well-maintained packages are welcome - there is no need to hand-roll what a library does well.
- When a TypeScript script is heavy on **operational workflow** - spawning processes, pipes, file shuffling - use [Xec](https://xec.sh) (`npm:@xec-sh/core`) rather than hand-rolling `node:child_process` plumbing.
- **Every entry point gets a `deno task` wrapper** in [`deno.json`](../deno.json), so scripts are also runnable where a shebang is not available.
- **Windows:** the entry points assume a POSIX-like shell - run them from **Git Bash / MSYS2** (or WSL), not `cmd.exe` or PowerShell.

## Adding a script

1. Write it as `run/<name>` - no extension, shebang on line 1, a short comment block saying what it does and what it exits with.
2. `chmod +x run/<name>`, and confirm the mode is staged (`git ls-files -s run/`).
3. Put anything shared or importable in `run/lib/<name>.ts` (or `.sh`), with its extension.
4. Support `-h`/`--help`, accept optional explicit targets, and exit non-zero on failure so the script can gate CI.
5. Add the `deno task` wrapper to [`deno.json`](../deno.json) and a row to the table above.

## Formatting the scripts themselves

`deno fmt` and `deno lint` do not auto-detect extensionless files - pass the language explicitly:

```sh
deno fmt --ext=ts run/check-links
deno lint --ext=ts run/check-links
deno check run/check-links      # type-check; no --ext needed
```

Files under `run/lib/` are picked up normally (`deno fmt run/lib`). Markdown - including this page - goes through `run/format`, not `deno fmt`.
