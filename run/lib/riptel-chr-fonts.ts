import { readFileSync } from "node:fs";
import { join } from "node:path";
import { createHash } from "node:crypto";

// Original RIPtel installer payloads, also pinned by version/3.0 asset LFS OIDs.
const fonts: [string, string][] = [
  [
    "trip.chr",
    "831963618a5dbf3562e12c3b4b8bab3ed3879143de3c7cd9293be171a44282db",
  ],
  [
    "litt.chr",
    "5eb19d13fef5a79833f114fa02fc1aaabd4b049abbc0a44ccfc83e866f93fcdf",
  ],
  [
    "sans.chr",
    "edd4db0e5735ce0872d60137819e2b74a590d069be7585269b7b6d675e319bbc",
  ],
  [
    "goth.chr",
    "8620ae9db9dd874b842f67f7153721a5681fef3cd9e99af0fde41f3b1194f183",
  ],
  [
    "scri.chr",
    "04fde2931ba5392c55791b0c99c0c6790cfae97e9792b5833e40e87e19730bf7",
  ],
  [
    "simp.chr",
    "8d51af7680da29886481166fbc352141e6c615dd97927521b06cb2548f940acf",
  ],
  [
    "tscr.chr",
    "305e2a9657902fef2a52b25e398a5e2fca4cd0ace0fc0208704b2cb5c360b187",
  ],
  [
    "lcom.chr",
    "52c7667877584dada19f2b9100699e6946cc0701ea7c69de15628e3f9408c1a0",
  ],
  [
    "euro.chr",
    "783392b9fd0414a9c768db7741808811de63c599b34f92acb2d967e74993c6b1",
  ],
  [
    "bold.chr",
    "ee8b45d7491e1bc61936923f8642f1766c58dee7c528fa542e727092a914d07e",
  ],
];

export function checkCHRFonts(directory: string) {
  return fonts.map(([name, expected]) => {
    const b = readFileSync(join(directory, name));
    const sha256 = createHash("sha256").update(b).digest("hex");
    if (sha256 !== expected) {
      throw new Error(`Wrong original CHR payload: ${name}`);
    }
    const eof = b.indexOf(0x1a);
    const header = b.readUInt16LE(eof + 1);
    const count = b.readUInt16LE(header + 1), first = b[header + 4];
    const definitions = header + b.readUInt16LE(header + 5);
    const scans: { code: number; offset: number; withinGlyph: number }[] = [];
    for (let g = 0; g < count; ++g) {
      const start = definitions + b.readUInt16LE(header + 16 + 2 * g);
      let offset = start;
      for (;;) {
        if (offset + 2 > b.length) {
          throw new Error(`Unterminated glyph: ${name}:${g}`);
        }
        const opcode = (b[offset] >> 7) * 2 + (b[offset + 1] >> 7);
        if (opcode === 0) break;
        if (opcode === 1) {
          scans.push({ code: first + g, offset, withinGlyph: offset - start });
        }
        offset += 2;
      }
    }
    return {
      name,
      bytes: b.length,
      sha256,
      count,
      first,
      scanFlag: b[header + 7],
      unusedName: b.subarray(header + 11, header + 15).toString("hex"),
      scans,
    };
  });
}
