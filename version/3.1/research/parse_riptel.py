#!/usr/bin/env python3
"""Census parser for RIPtel 3.1 demo corpus (authentic RIPscrip 3.0 scripts)."""
import os, re, sys, json
from collections import defaultdict, Counter

CORPUS = "/home/tracker1/src/rip-tools/artifacts/RIPtel/ICONS"
EXTS = {".RIP", ".FN", ".DEF", ".MNU", ".MSE", ".RET", ".ENT", ".EXT", ".COL"}
ESC = "\x1b"
SOH = "\x01"

# ---------- known command tables ----------
K154_L0 = set("#*=>@ABCEFHILOPQRSTVWXYZacegilmopsvw")
K154_L1 = set("BCDEFGIKMPRTtUW") | {ESC}
K2X_L0 = set("fztxKNbj$") | {'"'} - {'"'}  # 2.x additions per prompt (j, $, f, z, t, x, K, N, b)
K2X_L0 = set("fztxKNbj$")
K2X_L1 = set("gpikbwGc")  # COPY_BLIT g, IMAGE p, IMAGE_STYLE i, KILL_ENCL k, LOAD_BITMAP b, PLAY_AUDIO w, SCROLL G, SET_MOUSE_CURSOR c
K2X_L2 = set("PpCWRBEAsTY")
K2X_L3 = set("eD")
SYNC_DESC = {(0, "y"), (0, "D"), (0, ESC), (1, "A"), (1, "O"), (1, "N"), (1, "S")}

SPEC_NAMES = {
    (0, "A"): "RIP_ARC", (0, "k"): "RIP_BACK_COLOR*", (0, "B"): "RIP_BAR", (0, "Z"): "RIP_BEZIER",
    (0, "C"): "RIP_CIRCLE", (0, "c"): "RIP_COLOR", (0, "!"): "RIP_COMMENT", (0, ">"): "RIP_ERASE_EOL",
    (0, "E"): "RIP_ERASE_VIEW", (0, "e"): "RIP_ERASE_WINDOW", (0, "y"): "RIP_EXTENDED_FONT_STYLE",
    (0, "b"): "RIP_EXTENDED_TEXT_WINDOW / RIP_SET_BASE_MATH (spec collision)",
    (0, "F"): "RIP_FILL", (0, "s"): "RIP_FILL_PATTERN", (0, "S"): "RIP_FILL_STYLE",
    (0, "G"): "RIP_FILLED_CIRCLE*", (0, "o"): "RIP_FILLED_OVAL", (0, "p"): "RIP_FILLED_POLYGON",
    (0, "x"): "RIP_FILLED_POLY_BEZIER", (0, "K"): "RIP_FILLED_RECTANGLE", (0, "u"): "RIP_FILLED_ROUNDED_RECT*",
    (0, "Y"): "RIP_FONT_STYLE", (0, "g"): "RIP_GOTOXY", (0, "("): "RIP_GROUP_BEGIN*", (0, ")"): "RIP_GROUP_END*",
    (0, "h"): "RIP_HEADER*", (0, "H"): "RIP_HOME", (0, "L"): "RIP_LINE", (0, "="): "RIP_LINE_STYLE",
    (0, "m"): "RIP_MOVE", (0, "#"): "RIP_NO_MORE", (0, "d"): "RIP_ONE_DRAWING_PALETTE*",
    (0, "a"): "RIP_ONE_PALETTE", (0, "O"): "RIP_OVAL", (0, "V"): "RIP_OVAL_ARC", (0, "i"): "RIP_OVAL_PIE_SLICE",
    (0, "I"): "RIP_PIE_SLICE", (0, "X"): "RIP_PIXEL", (0, "P"): "RIP_POLYGON", (0, "l"): "RIP_POLYLINE",
    (0, "z"): "RIP_POLY_BEZIER", (0, "t"): "RIP_POLY_BEZIER_LINE", (0, "R"): "RIP_RECTANGLE",
    (0, "*"): "RIP_RESET_WINDOWS", (0, "U"): "RIP_ROUNDED_RECT*", (0, "N"): "RIP_SET_BORDER",
    (0, "M"): "RIP_SET_COLOR_MODE*", (0, "n"): "RIP_SET_COORDINATE_SIZE*", (0, "D"): "RIP_SET_DRAWING_PALETTE",
    (0, "Q"): "RIP_SET_PALETTE", (0, "f"): "RIP_SET_WORLD_FRAME", (0, "T"): "RIP_TEXT",
    (0, "w"): "RIP_TEXT_WINDOW", (0, "@"): "RIP_TEXT_XY", (0, "v"): "RIP_VIEWPORT", (0, "W"): "RIP_WRITE_MODE",
    (0, "j"): "RIP_POINT (2.x)", (0, "$"): "VAR_TRIGGER (2.x)",
    (1, "T"): "RIP_BEGIN_TEXT", (1, "U"): "RIP_BUTTON", (1, "B"): "RIP_BUTTON_STYLE",
    (1, "g"): "RIP_COPY_BLIT", (1, "D"): "RIP_DEFINE", (1, "E"): "RIP_END_TEXT", (1, "F"): "RIP_FILE_QUERY",
    (1, "C"): "RIP_GET_IMAGE", (1, "p"): "RIP_IMAGE", (1, "i"): "RIP_IMAGE_STYLE",
    (1, "k"): "RIP_KILL_ENCLOSED_MOUSE_FIELDS", (1, "K"): "RIP_KILL_MOUSE_FIELDS", (1, "b"): "RIP_LOAD_BITMAP",
    (1, "I"): "RIP_LOAD_ICON", (1, "M"): "RIP_MOUSE", (1, "w"): "RIP_PLAY_AUDIO", (1, "P"): "RIP_PUT_IMAGE",
    (1, ESC): "RIP_QUERY", (1, "R"): "RIP_READ_SCENE", (1, "t"): "RIP_REGION_TEXT", (1, "G"): "RIP_SCROLL",
    (1, "c"): "RIP_SET_MOUSE_CURSOR", (1, "W"): "RIP_WRITE_ICON",
    (2, "P"): "RIP_DEFINE_PORT", (2, "p"): "RIP_DELETE_PORT", (2, "C"): "RIP_PORT_COPY",
    (2, "W"): "RIP_PORT_WRITE", (2, "R"): "RIP_SET_REFRESH", (2, "B"): "RIP_SWITCH_BUTTON_STYLE",
    (2, "E"): "RIP_SWITCH_ENVIRONMENT", (2, "A"): "RIP_SWITCH_PALETTE", (2, "s"): "RIP_SWITCH_PORT",
    (2, "T"): "RIP_SWITCH_TEXT_WINDOW", (2, "Y"): "RIP_SWITCH_STYLE",
    (3, "e"): "RIP_BAUD_EMULATION", (3, "D"): "RIP_DELAY",
    (9, ESC): "RIP_ENTER_BLOCK_MODE", (9, "U"): "RIP_BEGIN_UUENCODE_BLOCK",
}

def classify(level, ch):
    if level == 0 and ch in K154_L0: return "known-1.54"
    if level == 1 and ch in K154_L1: return "known-1.54"
    if level == 0 and ch in K2X_L0: return "known-2.x"
    if level == 1 and ch in K2X_L1: return "known-2.x"
    if level == 2 and ch in K2X_L2: return "known-2.x"
    if level == 3 and ch in K2X_L3: return "known-2.x"
    if (level, ch) in SYNC_DESC: return "SyncTERM-descriptor-only"
    if (level, ch) in SPEC_NAMES: return "2.00a4-documented"
    return "COMPLETELY NEW"

# ---------- parsing ----------
def logical_lines(raw: bytes):
    """Yield logical lines: split on CR(LF), join trailing-backslash continuations."""
    text = raw.decode("latin-1")
    # normalize CRLF / CR / stray LF
    phys = re.split(r"\r\n|\r|\n", text)
    buf = None
    for ln in phys:
        if buf is not None:
            ln = buf + ln
            buf = None
        # trailing backslash continuation: odd number of trailing backslashes
        m = re.search(r"\\+$", ln)
        if m and len(m.group(0)) % 2 == 1:
            buf = ln[:-1]
            continue
        yield ln
    if buf is not None:
        yield buf

def split_chain(body: str):
    """Split command chain on unescaped |; keep escapes inside args."""
    parts, cur, i = [], [], 0
    while i < len(body):
        ch = body[i]
        if ch == "\\" and i + 1 < len(body):
            cur.append(body[i:i+2]); i += 2; continue
        if ch == "|":
            parts.append("".join(cur)); cur = []; i += 1; continue
        cur.append(ch); i += 1
    parts.append("".join(cur))
    return parts

def parse_cmd(tok: str):
    """Return (level, cmdchar, args) or None."""
    if not tok:
        return None
    i, level = 0, 0
    digits = ""
    while i < len(tok) and tok[i] in "123456789" and len(digits) < 2:
        digits += tok[i]; i += 1
    if digits:
        level = int(digits)
    if i >= len(tok):
        return None
    return level, tok[i], tok[i+1:]

def vis(s):
    return s.replace(ESC, "<ESC>").replace(SOH, "<SOH>")

def main():
    files = sorted(f for f in os.listdir(CORPUS) if os.path.splitext(f)[1].upper() in EXTS)
    census = defaultdict(lambda: {"count": 0, "files": Counter(), "examples": []})
    comments = defaultdict(list)          # file -> [comment text]
    varuse = defaultdict(lambda: {"count": 0, "examples": []})
    filerefs = defaultdict(set)           # script -> set of referenced files
    introducers = defaultdict(Counter)    # file -> Counter(SOH/! / other)
    nonrip = defaultdict(list)
    worldframes = defaultdict(list)
    parse_oddities = []

    ref_re = re.compile(r"[A-Za-z0-9_&$~!#%^\-]+\.(BMP|JPG|JPEG|GIF|RFF|COL|TXT|WAV|BMH|FN|MSE|MNU|DEF|RIP|ICN|HLP|PAL|ENT|EXT|RET|VOC|MID|FNT)\b", re.I)
    var_re = re.compile(r"\$[^$\r\n]*\$")

    for fn in files:
        path = os.path.join(CORPUS, fn)
        with open(path, "rb") as fh:
            raw = fh.read()
        for ln in logical_lines(raw):
            if not ln:
                continue
            if ln.startswith(SOH + "|"):
                introducers[fn]["SOH"] += 1; body = ln[2:]
            elif ln.startswith("!|"):
                introducers[fn]["!"] += 1; body = ln[2:]
            elif ln.startswith(SOH) or ln.startswith("!"):
                introducers[fn]["odd-" + vis(ln[:2])] += 1
                body = ln[1:].lstrip("|")
            else:
                nonrip[fn].append(ln[:80])
                continue
            for tok in split_chain(body):
                pc = parse_cmd(tok)
                if pc is None:
                    if tok.strip():
                        parse_oddities.append((fn, vis(tok[:60])))
                    continue
                level, ch, args = pc
                # variables in args
                for m in var_re.finditer(args):
                    v = m.group(0)
                    d = varuse[v]; d["count"] += 1
                    if len(d["examples"]) < 3:
                        d["examples"].append(f"{fn}: {vis(tok[:70])}")
                # file references
                for m in ref_re.finditer(args):
                    filerefs[fn].add(m.group(0).upper())
                if level == 0 and ch == "!":
                    txt = args.strip()
                    comments[fn].append(txt)
                    continue
                if level == 0 and ch == "f":
                    worldframes[fn].append(args)
                key = (level, ch)
                d = census[key]
                d["count"] += 1
                d["files"][fn] += 1
                if len(d["examples"]) < 3 and vis(args)[:60] not in d["examples"]:
                    d["examples"].append(vis(args)[:60])

    out = {
        "files": files,
        "census": {f"{lvl}|{vis(ch)}": {
            "level": lvl, "char": vis(ch), "count": d["count"],
            "classification": classify(lvl, ch),
            "spec_name": SPEC_NAMES.get((lvl, ch), ""),
            "files": [f for f, _ in d["files"].most_common(5)],
            "nfiles": len(d["files"]),
            "examples": d["examples"],
        } for (lvl, ch), d in sorted(census.items(), key=lambda kv: (kv[0][0], kv[0][1]))},
        "comments": comments,
        "vars": {k: v for k, v in sorted(varuse.items(), key=lambda kv: -kv[1]["count"])},
        "filerefs": {k: sorted(v) for k, v in sorted(filerefs.items())},
        "introducers": {k: dict(v) for k, v in sorted(introducers.items())},
        "nonrip": dict(nonrip),
        "worldframes": dict(worldframes),
        "oddities": parse_oddities[:80],
    }
    outp = os.path.join(os.path.dirname(os.path.abspath(__file__)), "census.json")
    with open(outp, "w") as fh:
        json.dump(out, fh, indent=1)
    print(f"files parsed: {len(files)}")
    print(f"distinct opcodes: {len(census)}")
    cls = Counter(classify(l, c) for (l, c) in census)
    for k, v in cls.most_common():
        print(f"  {k}: {v}")
    print("\n== CENSUS ==")
    for key, d in out["census"].items():
        print(f"{d['level']}|{d['char']!r:8} n={d['count']:5} files={d['nfiles']:3} [{d['classification']}] {d['spec_name']}")
        for e in d["examples"]:
            print(f"      ex: {e}")
    print(f"\nvars: {len(varuse)} distinct; oddities: {len(parse_oddities)}")
    print(f"nonrip lines in {len(nonrip)} files: {list(nonrip)[:10]}")

if __name__ == "__main__":
    main()
