#!/usr/bin/env python3
"""Link and anchor checker for the Markdown docs.

Validates every [text](path#anchor) link in the given directories (or the
repo's standard doc trees when run with no arguments): the target file must
exist and the #anchor must match a heading slug in the target (GitHub slug
rules: lowercase, spaces to '-', punctuation stripped, underscores kept,
'$' stripped — em dashes produce double hyphens). Fenced code blocks are
skipped. Exit code 1 if any link is broken.

Interim tooling: TODO.md tracks porting this to a Deno script for the
VitePress site build.
"""
import re, os, sys, glob

DEFAULT_DIRS = [
    'version/1.5x/ripscrip', 'version/2.x/ripscrip',
    'version/3.x/ripscrip', 'version/3.x/whitepaper', 'version/3.x/research',
    'version/1.5x/assets', 'version/2.x/assets', 'version/3.x/assets',
]

def slugify(h):
    h = re.sub(r'[*`\[\]()]', '', h).strip().lower()
    h = re.sub(r'[^\w\s-]', '', h, flags=re.UNICODE)
    return h.replace(' ', '-')

def anchors_of(path, cache={}):
    if path in cache:
        return cache[path]
    anc, fenced = set(), False
    for line in open(path, encoding='utf-8'):
        if line.startswith('```'):
            fenced = not fenced
            continue
        if fenced:
            continue
        m = re.match(r'^(#{1,6})\s+(.*)', line)
        if m:
            anc.add(slugify(m.group(2)))
    cache[path] = anc
    return anc

def check(files):
    bad = total = 0
    for f in files:
        fenced = False
        for i, line in enumerate(open(f, encoding='utf-8'), 1):
            if line.startswith('```'):
                fenced = not fenced
                continue
            if fenced:
                continue
            line = re.sub(r'`[^`]*`', '', line)  # ignore links inside inline code
            for m in re.finditer(r'\[[^\]]*\]\(([^)]+)\)', line):
                t = m.group(1)
                total += 1
                if t.startswith(('http://', 'https://', 'mailto:')):
                    continue
                path, _, frag = t.partition('#')
                target = os.path.normpath(os.path.join(os.path.dirname(f), path)) if path else f
                if not os.path.exists(target):
                    print(f'{f}:{i} MISSING FILE -> {t}')
                    bad += 1
                    continue
                if frag and os.path.isfile(target):
                    if frag not in anchors_of(target):
                        print(f'{f}:{i} MISSING ANCHOR -> {t}')
                        bad += 1
    print(f'{bad} bad of {total} links across {len(files)} files')
    return bad

if __name__ == '__main__':
    dirs = sys.argv[1:] or [d for d in DEFAULT_DIRS if os.path.isdir(d)]
    files = sorted(f for d in dirs for f in glob.glob(os.path.join(d, '**', '*.md'), recursive=True))
    files += sorted(glob.glob('*.md'))
    sys.exit(1 if check(files) else 0)
