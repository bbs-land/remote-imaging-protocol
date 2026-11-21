import { readFileSync } from "node:fs";
import { createHash } from "node:crypto";

export const DRIVER_SHA256 =
  "83d71b358492b15bcfd3773c30a52784881a3a49a5b6e9029d986b3cba1fd5e8";

export function loadDriver(path: string) {
  const bytes = readFileSync(path);
  const sha256 = createHash("sha256").update(bytes).digest("hex");
  if (sha256 !== DRIVER_SHA256) {
    throw new Error(`Unsupported driver SHA-256: ${sha256}`);
  }
  const pe = bytes.readUInt32LE(0x3c);
  const count = bytes.readUInt16LE(pe + 6);
  const optional = pe + 24;
  const sections = Array.from({ length: count }, (_, i) => {
    const at = optional + bytes.readUInt16LE(pe + 20) + i * 40;
    return {
      name: bytes.subarray(at, at + 8).toString("ascii").replace(/\0.*$/, ""),
      rva: bytes.readUInt32LE(at + 12),
      raw: bytes.readUInt32LE(at + 20),
      size: bytes.readUInt32LE(at + 16),
    };
  });
  function offset(rva: number, length = 1) {
    const section = sections.find((s) =>
      rva >= s.rva && rva + length <= s.rva + s.size
    );
    if (!section) {
      throw new Error(
        `RVA range is not file-backed: ${rva.toString(16)} + ${length}`,
      );
    }
    return section.raw + rva - section.rva;
  }
  return {
    bytes,
    sha256,
    sections,
    offset,
    imageBase: bytes.readUInt32LE(optional + 28),
  };
}
