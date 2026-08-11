#!/usr/bin/env bash
# Shared Prettier invocation for run/lint and run/format.
#
# Prettier is this repository's formatter of record for Markdown; Deno is the
# runtime, so there is no npm install and no node_modules - Deno fetches and
# caches the package on first use. The version is pinned here, the single
# declaration point for the Prettier CLI the way run/lib/dependencies.ts is for
# TypeScript imports. Keep it in step with the version the VS Code Prettier
# extension resolves (.vscode/extensions.json), so editor and CLI agree.
#
# Not executable and not meant to be run directly: source it, then call
# run_prettier with a mode (--check or --write) and any explicit targets.

PRETTIER_VERSION="3.9.6"

# Default target: every Markdown file in the repository. Quoted all the way
# through so Prettier expands the glob itself - bash without globstar would
# only reach one directory deep. Exclusions live in .prettierignore.
PRETTIER_DEFAULT_TARGETS=("**/*.md")

# run_prettier <--check|--write> [target ...]
run_prettier() {
  local mode="$1"
  shift

  local targets=("$@")
  if [[ ${#targets[@]} -eq 0 ]]; then
    targets=("${PRETTIER_DEFAULT_TARGETS[@]}")
  fi

  # Narrowest permission set that works: reading files and config, plus the
  # env/sys probes Prettier makes at startup. Writing is granted only to
  # --write, so run/lint cannot modify the tree even by accident.
  local perms=(--allow-read --allow-env --allow-sys)
  if [[ "$mode" == "--write" ]]; then
    perms+=(--allow-write)
  fi

  exec deno run "${perms[@]}" "npm:prettier@${PRETTIER_VERSION}" "$mode" "${targets[@]}"
}
