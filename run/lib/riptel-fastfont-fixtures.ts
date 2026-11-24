// Replace A-H only in a task-local copy of the fingerprinted Marin font.
import { readFileSync, writeFileSync } from "node:fs";
import { createHash } from "node:crypto";
import { argv } from "node:process";
import { Buffer } from "node:buffer";
const file = argv[2];
const bytes = readFileSync(file);
if (
  createHash("sha256").update(bytes).digest("hex") !==
    "4b0a392baa74a1849b9fabedacc39c1434784370175f7708ea151b851255ed68"
) {
  throw new Error("Unexpected original Marin font");
}
type Point = [number, number];
const rectangle = (
  x: number,
  y: number,
  w: number,
  h: number,
): Point[] => [[x, y], [x + w, y], [x + w, y + h], [x, y + h], [x, y]];
const outer = rectangle(0, -600, 600, 600);
const inner = rectangle(200, -400, 200, 200);
const fixtures: Point[][][] = [
  [outer, inner],
  [outer, inner.toReversed()],
  [outer, outer],
  [outer, rectangle(300, -600, 600, 600)],
  [outer],
  [outer.toReversed()],
  [outer.toReversed(), inner.toReversed()],
  [outer, outer.toReversed()],
];
const base = bytes.readUInt16LE(0x204);
for (let n = 0; n < fixtures.length; ++n) {
  const at = base + bytes.readUInt32LE(base + 0x20c + (65 + n - 32) * 4);
  const capacity = bytes.readUInt16LE(at + 10) + bytes.readUInt16LE(at + 12);
  const contours = fixtures[n];
  const points = contours.flat();
  const path = Buffer.alloc(points.length * 5);
  let cursor = 0;
  for (const contour of contours) {
    for (let i = 0; i < contour.length; ++i) {
      path[cursor++] = i ? 1 : 0;
      path.writeInt16LE(contour[i][0], cursor);
      cursor += 2;
      path.writeInt16LE(contour[i][1], cursor);
      cursor += 2;
    }
  }
  if (path.length > capacity) {
    throw new Error("Synthetic path exceeds owned record");
  }
  bytes.writeInt16LE(Math.min(...points.map((p) => p[0])), at + 2);
  bytes.writeInt16LE(Math.min(...points.map((p) => p[1])), at + 4);
  bytes.writeInt16LE(Math.max(...points.map((p) => p[0])), at + 6);
  bytes.writeInt16LE(Math.max(...points.map((p) => p[1])), at + 8);
  bytes.writeUInt16LE(path.length, at + 10);
  bytes.writeUInt16LE(0, at + 12);
  path.copy(bytes, at + 14);
}
writeFileSync(file, bytes);
console.log(
  "Synthetic Marin A-H SHA256",
  createHash("sha256").update(bytes).digest("hex"),
);
