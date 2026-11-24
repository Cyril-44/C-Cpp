import * as std from 'std';

// Run with qjs: qjs checker.mjs <infile> <outfile>

function fail(message, exitCode = 1) {
  std.err.puts(`WA: ${message}\n`);
  std.exit(exitCode);
}

// QuickJS scriptArgs[0] is the script name; the next two arguments are paths.
if (scriptArgs.length !== 3) {
  fail('Usage: ./checker <infile> <outfile>', 2);
}

const inputPath = scriptArgs[1];
const outputPath = scriptArgs[2];
const inputText = std.loadFile(inputPath);
if (inputText === null) fail(`cannot read input file: ${inputPath}`);
const outputText = std.loadFile(outputPath);
if (outputText === null) fail(`cannot read output file: ${outputPath}`);

const inputTokens = inputText.trim().split(/\s+/);
if (inputTokens.length !== 2 || !/^\d+$/.test(inputTokens[0]) || !/^\d+$/.test(inputTokens[1])) {
  fail('input must contain exactly two integers: T K');
}

const task = Number(inputTokens[0]);
const K = BigInt(inputTokens[1]);
if (task < 1 || task > 7 || K < 1n || K > 1000000000000000000n) {
  fail('input is outside the stated range');
}
if ((task === 1 && K > 5n) || (task === 2 && K > 10n) ||
    (task === 5 && K > 1000n)) {
  fail(`input violates Subtask ${task}'s K bound`);
}
if (task === 6) {
  let x = K;
  let bits = 0;
  while (x > 0n) {
    x &= x - 1n;
    bits++;
  }
  if (bits !== 2) fail('Subtask 6 requires popcount(K) = 2');
}

const outputTokens = outputText.trim().split(/\s+/).filter(Boolean);
if (outputTokens.length < 2 || !/^\d+$/.test(outputTokens[0]) || !/^\d+$/.test(outputTokens[1])) {
  fail('output must start with N M');
}

const n = Number(outputTokens[0]);
const m = Number(outputTokens[1]);
if (!Number.isSafeInteger(n) || n < 1 || n > 10 ||
    !Number.isSafeInteger(m) || m < 1 || m > 10000) {
  fail('N or M is outside the allowed range');
}
if (outputTokens.length !== n + 2) {
  fail(`expected ${n} grid rows, found ${Math.max(0, outputTokens.length - 2)}`);
}

const grid = outputTokens.slice(2);
for (let i = 0; i < n; i++) {
  if (grid[i].length !== m) fail(`row ${i + 1} has length ${grid[i].length}; expected ${m}`);
  if (!/^[01]+$/.test(grid[i])) fail(`row ${i + 1} contains a character other than 0 or 1`);
}

// Count tilings exactly up to K+1. BigInt avoids loss of precision above 2^53.
const CAP = K + 1n;
const stateCount = 1 << n;
let current = new Array(stateCount).fill(0n);
current[0] = 1n;

function addSaturated(array, index, value) {
  const sum = array[index] + value;
  array[index] = sum > CAP ? CAP : sum;
}

for (let j = 0; j < m; j++) {
  for (let i = 0; i < n; i++) {
    const next = new Array(stateCount).fill(0n);
    const white = grid[i][j] === '0';
    const canHorizontal = white && j + 1 < m && grid[i][j + 1] === '0';
    const canVertical = white && i + 1 < n && grid[i + 1][j] === '0';

    for (let mask = 0; mask < stateCount; mask++) {
      const ways = current[mask];
      if (ways === 0n) continue;

      if ((mask >> i) & 1) {
        addSaturated(next, mask ^ (1 << i), ways);
      } else if (!white) {
        addSaturated(next, mask, ways);
      } else {
        if (canHorizontal) addSaturated(next, mask | (1 << i), ways);
        if (canVertical && !((mask >> (i + 1)) & 1)) {
          addSaturated(next, mask | (1 << (i + 1)), ways);
        }
      }
    }
    current = next;
  }
}

const count = current[0];
if (count !== K) {
  fail(count > K ? `tiling count is greater than K=${K}` : `tiling count is ${count}; expected ${K}`);
}

if (task <= 5) {
  std.puts(`OK: N=${n}, M=${m}, tiling count=${K}, full Subtask ${task} score\n`);
  std.exit(0);
}

const totalPoints = task === 6 ? 20 : 50;
const t = Math.log(Math.max(m, 180) / 180) / Math.log(10000 / 180);
const fPercent = 100 - 84 * t - 15 * Math.sqrt(t) + 4 * t * t;
const g = Math.pow(0.8, Math.max(n - 4, 0));
const rate = Math.max(0, Math.min(1, (fPercent / 100) * g));
const earned = Math.floor(totalPoints * rate + 1e-10);
std.puts(`OK: N=${n}, M=${m}, tiling count=${K}, Subtask ${task} score=${earned}/${totalPoints}\n`);
