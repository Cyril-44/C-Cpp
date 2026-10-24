import * as std from 'std';

function optimum(n) {
    const square = n * n;
    if (n & 1) return (square - 1) / 2;
    if (n % 4 === 2) return (square + n - 2) / 2;
    if (n === 4 || n === 8) return (square + n - 4) / 2;
    return (square + n - 6) / 2;
}

function reject(message) {
    std.err.puts(`Wrong Answer: ${message}\n`);
    return 1;
}

const INT_MIN = -2147483648n;
const INT_MAX = 2147483647n;
const LL_MIN = -9223372036854775808n;
const LL_MAX = 9223372036854775807n;

class Reader {
    constructor(data) {
        this.data = data;
        this.pos = 0;
    }

    isSpace(c) {
        return c === 32 || (c >= 9 && c <= 13);
    }

    skipSpace() {
        const data = this.data;
        while (
            this.pos < data.length &&
            this.isSpace(data[this.pos])
        ) {
            ++this.pos;
        }
    }

    readWord() {
        this.skipSpace();

        if (this.pos >= this.data.length) {
            return null;
        }

        const start = this.pos;
        const data = this.data;

        while (
            this.pos < data.length &&
            !this.isSpace(data[this.pos])
        ) {
            ++this.pos;
        }

        return data.subarray(start, this.pos);
    }

    readInteger(min, max) {
        this.skipSpace();

        const data = this.data;
        if (this.pos >= data.length) {
            return null;
        }

        let sign = 1n;

        if (data[this.pos] === 43) {
            ++this.pos;
        } else if (data[this.pos] === 45) {
            sign = -1n;
            ++this.pos;
        }

        const digitStart = this.pos;
        const limit = sign < 0n ? -min : max;

        let magnitude = 0n;
        let overflow = false;

        while (this.pos < data.length) {
            const c = data[this.pos];

            if (c < 48 || c > 57) {
                break;
            }

            if (!overflow) {
                const next =
                    magnitude * 10n + BigInt(c - 48);

                if (next > limit) {
                    overflow = true;
                } else {
                    magnitude = next;
                }
            }

            ++this.pos;
        }

        if (this.pos === digitStart || overflow) {
            return null;
        }

        const value = sign * magnitude;

        if (value < min || value > max) {
            return null;
        }

        return value;
    }
}

function main() {
    // scriptArgs[0] 是脚本名或可执行程序名。
    const args = scriptArgs;

    if (args.length !== 3) {
        std.err.puts(
            `Usage: ${args[0] || 'checker'} <input-file> <output-file>\n`
        );
        return 2;
    }

    // binary 模式返回 Uint8Array，保留文件的原始字节。
    const inputData = std.loadFile(args[1], {
        binary: true
    });

    const outputData = std.loadFile(args[2], {
        binary: true
    });

    if (inputData === null) {
        return reject('cannot open input file');
    }

    if (outputData === null) {
        return reject('cannot open output file');
    }

    const input = new Reader(inputData);
    const output = new Reader(outputData);

    const tv = input.readInteger(INT_MIN, INT_MAX);

    if (tv === null || tv < 1n || tv > 5n) {
        return reject('invalid T in input');
    }

    const t = Number(tv);
    const ns = [];

    for (let i = 0; i < t; ++i) {
        const nv = input.readInteger(INT_MIN, INT_MAX);

        if (nv === null || nv < 2n || nv > 1000n) {
            return reject('invalid N in input');
        }

        ns.push(Number(nv));
    }

    if (input.readWord() !== null) {
        return reject('extra tokens in input');
    }

    const symbols = 'QWEADZXC';
    const dr = [-1, -1, -1, 0, 0, 1, 1, 1];
    const dc = [-1, 0, 1, -1, 1, -1, 0, 1];

    const directionOf = new Int8Array(256);
    directionOf.fill(-1);

    for (let i = 0; i < symbols.length; ++i) {
        directionOf[symbols.charCodeAt(i)] = i;
    }

    for (let tc = 0; tc < t; ++tc) {
        const n = ns[tc];
        const caseNo = tc + 1;

        const claimed = output.readInteger(LL_MIN, LL_MAX);

        if (claimed === null) {
            return reject(
                `missing claimed maximum in case ${caseNo}`
            );
        }

        const path = output.readWord();

        if (path === null) {
            return reject(`missing path in case ${caseNo}`);
        }

        if (path.length !== n * n - 1) {
            return reject(
                `case ${caseNo}: path length must be N^2-1`
            );
        }

        const seen = new Uint8Array(n * n);
        seen[0] = 1;

        let row = 0;
        let col = 0;
        let right = 0;

        for (let step = 0; step < path.length; ++step) {
            const direction = directionOf[path[step]];

            if (direction === -1) {
                return reject(
                    `case ${caseNo}: invalid direction at step ${step + 1}`
                );
            }

            row += dr[direction];
            col += dc[direction];

            if (
                row < 0 || row >= n ||
                col < 0 || col >= n
            ) {
                return reject(
                    `case ${caseNo}: path leaves the grid at step ${step + 1}`
                );
            }

            const pos = row * n + col;

            if (seen[pos] !== 0) {
                return reject(
                    `case ${caseNo}: a cell is visited more than once`
                );
            }

            seen[pos] = 1;

            // ASCII 'D' = 68。
            if (path[step] === 68) {
                ++right;
            }
        }

        if (row !== n - 1 || col !== n - 1) {
            return reject(
                `case ${caseNo}: path does not end at (N,N)`
            );
        }

        if (BigInt(right) !== claimed) {
            return reject(
                `case ${caseNo}: D count differs from the claimed value`
            );
        }

        if (claimed !== BigInt(optimum(n))) {
            return reject(
                `case ${caseNo}: claimed value is not optimal`
            );
        }
    }

    if (output.readWord() !== null) {
        return reject('extra tokens in output');
    }

    std.out.puts(
        'Accepted: all paths are valid and optimal\n'
    );

    return 0;
}

std.exit(main());