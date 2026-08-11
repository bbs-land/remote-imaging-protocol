/**
 * Single declaration point for third-party dependencies used by `run/` scripts.
 *
 * Import from `jsr:`/`npm:` here and re-export what the scripts need, so every
 * external package and its version is visible in one file rather than scattered
 * through the entry points. Prefer `jsr:` over `npm:`. See CONTRIBUTING.md,
 * "Project scripts (`run/`)".
 *
 *   export { $ } from "npm:@xec-sh/core@^0.11.1";
 *
 * Nothing is declared yet - `run/check-links` uses `node:*` built-ins only.
 */
export {};
