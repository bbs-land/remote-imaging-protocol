import { readFileSync } from "node:fs";
import { join } from "node:path";

function glyph(path: string) {
  const bytes = readFileSync(path);
  const header = /^P4\n(\d+) (\d+)\n/.exec(
    bytes.subarray(0, 64).toString("ascii"),
  );
  if (!header) throw new Error(`Unexpected probe PBM header: ${path}`);
  const width = Number(header[1]), height = Number(header[2]);
  const stride = Math.ceil(width / 8), start = header[0].length;
  if (bytes.length !== start + stride * height) {
    throw new Error(`Unexpected PBM length: ${path}`);
  }
  const points: [number, number][] = [];
  for (let y = 0; y < height; ++y) {
    for (let x = 0; x < width; ++x) {
      if (bytes[start + y * stride + (x >> 3)] & (128 >> (x & 7))) {
        points.push([x, y]);
      }
    }
  }
  if (points.length === 0) throw new Error(`Empty glyph: ${path}`);
  const x0 = Math.min(...points.map(([x]) => x));
  const y0 = Math.min(...points.map(([, y]) => y));
  const cropped = points.map(([x, y]) => [x - x0, y - y0] as [number, number]);
  return {
    width: Math.max(...cropped.map(([x]) => x)) + 1,
    height: Math.max(...cropped.map(([, y]) => y)) + 1,
    points: cropped,
  };
}

export function checkGlyphs(directory: string) {
  const glyphs = [0, 1, 2, 3].map((i) =>
    glyph(join(directory, `glyph-${i}.pbm`))
  );
  const baseline = glyphs[0];
  return glyphs.map((actual, turns) => {
    let expected = baseline.points,
      width = baseline.width,
      height = baseline.height;
    for (let i = 0; i < turns; ++i) {
      expected = expected.map(([x, y]) => [y, width - 1 - x]);
      [width, height] = [height, width];
    }
    if (actual.width !== width || actual.height !== height) {
      throw new Error(`Rotation ${turns} has the wrong dimensions`);
    }
    const expectedSet = new Set(expected.map(([x, y]) => `${x},${y}`));
    let intersection = 0;
    for (const [x, y] of actual.points) {
      if (expectedSet.has(`${x},${y}`)) ++intersection;
    }
    const union = expectedSet.size + actual.points.length - intersection;
    const overlap = intersection / union;
    // The original engine rounds two orientations by one pixel. Its actual
    // quarter-turn glyphs agree >99%; the other orientations agree <22%.
    if (overlap < 0.99) {
      throw new Error(
        `Rotation ${turns} disagrees with counterclockwise rotation: ${overlap}`,
      );
    }
    return {
      wireValue: turns * 900,
      width,
      height,
      overlap,
      counterclockwiseTurns: turns,
    };
  });
}
