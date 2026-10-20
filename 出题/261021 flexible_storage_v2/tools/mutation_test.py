#!/usr/bin/env python3
"""Build deliberately wrong versions of std and demonstrate wrong output.

Usage: python tools/mutation_test.py --build-dir PATH [--skip-compile]
Optional: --official-dir PATH --official-limit 20 --report PATH --cxx g++
Generated cases use a fixed seed and independent brute/recompute answers.
The original standard solution and official test data are never modified.
"""

import argparse
import concurrent.futures
import hashlib
import importlib.util
import json
import os
import random
import subprocess
import sys
import time
from pathlib import Path

sys.dont_write_bytecode = True
PACKAGE = Path(__file__).resolve().parents[1]
SEED = 1756420261021


def replace_one(source, old, new):
    if source.count(old) != 1:
        raise ValueError(f"mutation anchor must occur exactly once: {old!r}")
    return source.replace(old, new, 1)


def remove_point_push(source):
    start = source.index("    void pointRemake(")
    end = source.index("    Matrix query(", start)
    part = replace_one(source[start:end], "        push(p, a, b);\n", "        // BUG: skipped pending lazy tags.\n")
    return source[:start] + part + source[end:]


def wrong_boundaries(source):
    source = replace_one(source, "boundary(a, z, 1, delta);", "boundary(a, z, 0, delta);")
    return replace_one(source, "boundary(b, z, 0, delta);", "boundary(b, z, 1, delta);")


MUTATIONS = [
    ("ways_always_one", "Always output one optimal storage set", lambda s: replace_one(s, "<< ans.ways <<", "<< 1 <<")),
    ("drop_equal_cost_alternative", "Do not add ways when Value alternatives tie", lambda s: replace_one(s, "return {a.cost, w};", "return a;")),
    ("heavy_counter_is_boolean", "Forget shared references to a heavy child", lambda s: replace_one(s, "heavyCount[p] += delta;", "heavyCount[p] = delta > 0 ? 1 : 0;")),
    ("reverse_boundary_directions", "Use outside siblings instead of inside siblings", wrong_boundaries),
    ("do_not_lift_endpoints", "Start at raw leaves instead of maximal contained nodes", lambda s: replace_one(s, "int a = maximal(leafId[l], l, r), b = maximal(leafId[r], l, r);", "int a = leafId[l], b = leafId[r];")),
    ("ignore_root_requirement", "Ignore the root's direct requirement counter", lambda s: replace_one(s, "Value ans = rootCount ? B[1] : X[1];", "Value ans = X[1];")),
    ("ignore_right_zero_minimum", "Always treat right-light-child counters as positive in query", lambda s: replace_one(s, "int mask = (seg[p].minimum[0] == 0) | ((seg[p].minimum[1] == 0) << 1);", "int mask = (seg[p].minimum[0] == 0);")),
    ("matrix_ways_add_instead_multiply", "Add ways when concatenating independent matrix choices", lambda s: replace_one(s, "int count = int(1LL * a.w[ai] * b.w[bi] % MOD);", "int count = (a.w[ai] + b.w[bi]) % MOD;")),
    ("truncate_cost_to_int32", "Truncate the final minimum cost to signed 32 bits", lambda s: replace_one(s, "cout << ans.cost <<", "cout << int(ans.cost) <<")),
    ("point_remake_without_push", "Rebuild a point before pushing pending counter changes", remove_point_push),
    ("propagate_chains_top_down", "Propagate parent chains before changed child chains", lambda s: replace_one(s, "return depth[a] > depth[b];", "return depth[a] < depth[b];")),
    ("ignore_child_minimum_eligibility", "Treat a child's larger minima as global minima", lambda s: replace_one(s, "seg[right].product[mask & rightEligible]", "seg[right].product[mask]")),
]


def import_helpers():
    specification = importlib.util.spec_from_file_location("storage_verify_helpers", PACKAGE / "tools" / "verify_tools.py")
    module = importlib.util.module_from_spec(specification)
    specification.loader.exec_module(module)
    return module


def generate_cases(helper):
    rng = random.Random(SEED)
    cases = []

    def add(name, rows, operations):
        cases.append({"name": name, "n": len(rows) // 2, "q": len(operations), "input": helper.encode(rows, operations)})

    six = ["+ 1 2", "+ 1 1", "+ 2 2", "- 1 1", "- 1 2", "- 2 2"]
    add("unit_two_leaf", helper.make_tree(2), six)
    add("weighted_root_tie", [None, (2, 3, 2), (0, 0, 1), (0, 0, 1)], six)
    add("shared_heavy_references", helper.make_tree(4), ["+ 3 4", "+ 2 4", "- 3 4", "- 2 4"])
    add("int64_costs", helper.make_tree(5, weights="large", rng=rng, shuffle=True),
        [f"+ {x} {x}" for x in range(1, 6)] + [f"- {x} {x}" for x in range(1, 6)])
    for rightward in (True, False):
        operations = ["+ 1 31", "+ 8 32", "+ 9 32", "- 8 32", "- 1 31", "+ 2 31", "+ 1 30", "- 9 32", "- 2 31", "- 1 30"]
        if not rightward:
            mirrored = []
            for operation in operations:
                sign, l, r = operation.split()
                mirrored.append(f"{sign} {33-int(r)} {33-int(l)}")
            operations = mirrored
        add("lazy_then_point_" + ("right_comb" if rightward else "left_comb"), helper.comb_tree(32, rightward), operations)
    for shape, n in (("random", 50), ("mid", 49)):
        for weights in ("unit", "size", "random", "large"):
            add(f"{shape}_n{n}_{weights}", helper.make_tree(n, shape, rng, weights, shuffle=True), helper.random_operations(n, 600, rng))
    for weights in ("unit", "size", "random", "large"):
        add(f"brute_n8_{weights}", helper.make_tree(8, "random", rng, weights, shuffle=True), helper.random_operations(8, 400, rng))
    add("odd_random_n7", helper.make_tree(7, "random", rng, "size", shuffle=True), helper.random_operations(7, 400, rng))
    add("single_leaf_root", helper.make_tree(1, weights="large"), ["+ 1 1", "- 1 1"] * 8)
    assert len(cases) == 20
    return cases


def first_difference(expected, actual):
    wanted, found = expected.split(), actual.split()
    for index in range(max(len(wanted), len(found))):
        a = wanted[index] if index < len(wanted) else None
        b = found[index] if index < len(found) else None
        if a != b:
            operation = index // 2 + 1
            start = (operation - 1) * 2
            return {"operation": operation, "field": "cost" if index % 2 == 0 else "ways", "expected": wanted[start:start+2], "actual": found[start:start+2]}
    return None


def digest(text):
    return hashlib.sha256(text.encode("utf-8")).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, default=Path.cwd() / "work" / "v2_mutations")
    parser.add_argument("--report", type=Path, default=PACKAGE / "reports" / "mutations.json")
    parser.add_argument("--official-dir", type=Path, default=PACKAGE / "data")
    parser.add_argument("--official-limit", type=int, default=20)
    parser.add_argument("--skip-compile", action="store_true")
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    args = parser.parse_args()
    started = time.perf_counter()
    directory = args.build_dir.resolve()
    directory.mkdir(parents=True, exist_ok=True)
    case_directory = directory / "cases"
    case_directory.mkdir(exist_ok=True)
    suffix = ".exe" if os.name == "nt" else ""
    standard_path = PACKAGE / "solutions" / "std.cpp"
    original_bytes = standard_path.read_bytes()
    source = original_bytes.decode("utf-8").replace("\r\n", "\n")
    original_hash = hashlib.sha256(original_bytes).hexdigest()
    source_paths = {"baseline": directory / "baseline.cpp"}
    source_paths["baseline"].write_text(source, encoding="utf-8")
    for name, _, change in MUTATIONS:
        mutated = change(source)
        assert mutated != source
        source_paths[name] = directory / (name + ".cpp")
        source_paths[name].write_text(mutated, encoding="utf-8")
    for name in ("brute", "recompute"):
        source_paths[name] = PACKAGE / "solutions" / (name + ".cpp")
    executables = {name: directory / (name + suffix) for name in source_paths}

    def compile_one(item):
        name, path = item
        process = subprocess.run([args.cxx, "-std=c++17", "-O2", str(path), "-o", str(executables[name])], text=True, capture_output=True)
        if process.returncode:
            raise RuntimeError(f"compile failed for {name}: {process.stderr}")

    if not args.skip_compile:
        with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
            list(pool.map(compile_one, source_paths.items()))

    def execute(name, data):
        return subprocess.run([str(executables[name])], input=data, text=True, capture_output=True, timeout=90)

    helper = import_helpers()
    cases = generate_cases(helper)
    for case in cases:
        oracle = "brute" if case["n"] <= 8 else "recompute"
        expected = execute(oracle, case["input"])
        if expected.returncode:
            raise RuntimeError(f"oracle failed on {case['name']}: {expected.stderr}")
        case["answer"] = expected.stdout
        case["oracle"] = oracle
        (case_directory / (case["name"] + ".in")).write_text(case["input"], encoding="ascii")
        (case_directory / (case["name"] + ".ans")).write_text(case["answer"], encoding="ascii")
        baseline = execute("baseline", case["input"])
        difference = first_difference(case["answer"], baseline.stdout)
        if baseline.returncode or difference:
            raise AssertionError(f"unmodified std failed {case['name']}: {difference}; {baseline.stderr}")

    official = []
    if args.official_dir.exists():
        for input_path in sorted(args.official_dir.rglob("*.in")):
            answer_path = input_path.with_suffix(".ans")
            if not answer_path.exists():
                answer_path = input_path.with_suffix(".out")
            if not answer_path.exists():
                continue
            official.append({"name": str(input_path.relative_to(args.official_dir)), "input": input_path.read_text(encoding="ascii"), "answer": answer_path.read_text(encoding="ascii")})
            if len(official) >= args.official_limit:
                break

    results = []
    for name, description, _ in MUTATIONS:
        record = {"name": name, "description": description, "generated": {"status": "survived"}, "official": {"status": "not_available" if not official else "survived"}}
        for category, selected_cases in (("generated", cases), ("official", official)):
            for case in selected_cases:
                observed = execute(name, case["input"])
                difference = first_difference(case["answer"], observed.stdout)
                if observed.returncode or difference:
                    record[category] = {
                        "status": "killed_by_wrong_output" if observed.returncode == 0 else "runtime_error",
                        "case": case["name"], "return_code": observed.returncode,
                        "first_difference": difference,
                        "expected_stdout_sha256": digest(case["answer"]),
                        "actual_stdout_sha256": digest(observed.stdout),
                    }
                    if category == "generated":
                        (case_directory / (name + ".out")).write_text(observed.stdout, encoding="ascii")
                    break
        results.append(record)

    unchanged = hashlib.sha256(standard_path.read_bytes()).hexdigest() == original_hash
    generated_kills = sum(result["generated"]["status"] == "killed_by_wrong_output" for result in results)
    official_kills = sum(result["official"]["status"] == "killed_by_wrong_output" for result in results)
    report = {
        "status": "passed" if generated_kills == len(results) and unchanged else "needs_attention",
        "seed": SEED, "standard_sha256": original_hash, "standard_unchanged_during_test": unchanged,
        "generated_cases": [{key: value for key, value in case.items() if key not in ("input", "answer")} for case in cases],
        "official_cases_available": len(official), "mutants": len(results),
        "generated_wrong_output_kills": generated_kills, "official_wrong_output_kills": official_kills,
        "elapsed_seconds": round(time.perf_counter() - started, 3),
        "review": {
            "findings": "No correctness defect identified in the reviewed standard solution.",
            "checked_invariants": [
                "Maximal contained endpoint ancestors and exclusion of the LCA edge",
                "Heavy-child reference counts versus light-child lazily maintained counts",
                "Two orientation minima and eligibility-dependent four-product merge",
                "Lazy propagation before coefficient point remakes",
                "Bottom-up chain propagation using X/B states independent of own root count",
                "Root requirement handled separately",
                "Compact segment-node indices and iterative input-tree traversals",
                "Finite matrix costs remain safely below INF under input bounds",
            ],
        },
        "results": results,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"Generated cases: {generated_kills}/{len(results)} mutants killed by actual wrong stdout")
    print(f"Official cases: {official_kills}/{len(results)} mutants killed; {len(official)} cases available")
    print(f"Report: {args.report}")
    if report["status"] != "passed":
        raise SystemExit(1)


if __name__ == "__main__":
    main()
