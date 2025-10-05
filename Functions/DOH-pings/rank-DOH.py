import sys
import re
import socket
import subprocess
from typing import Optional, Tuple, List

#!/usr/bin/env python3
"""
Fetch a DoH list from GitHub, ping each server (multi-ping), rank by latency,
and write the top 5 entries into doh-list.txt.

Usage:
    python3 rank_doh.py [COUNT]
where COUNT is optional number of pings per host (default 4).
"""

import urllib.request
import urllib.parse
import concurrent.futures

RAW_URL = "https://raw.githubusercontent.com/MohamedElashri/doh-list/main/doh-list.txt"
OUTFILE = "doh-list.txt"
DEFAULT_PING_COUNT = 4
TOP_N = 5

def fetch_lines_from_file(file: str) -> List[str]:
    with open(file, "r", encoding="utf-8") as f:
        lines = [line.strip() for line in f.readlines()]
    # keep non-empty, non-comment lines
    return [line for line in lines if line and not line.startswith("#")]

def fetch_lines(url: str) -> List[str]:
    with urllib.request.urlopen(url, timeout=20) as resp:
        data = resp.read().decode(errors="ignore")
    lines = [line.strip() for line in data.splitlines()]
    # keep non-empty, non-comment lines
    return [line for line in lines if line and not line.startswith("#")]

def extract_host(url: str) -> Optional[str]:
    try:
        p = urllib.parse.urlparse(url)
        host = p.hostname or url  # fallback if not a normal URL
        return host
    except Exception:
        return None

def parse_ping_output(output: str) -> Optional[float]:
    # Windows: "Average = 23ms"
    m = re.search(r"Average = (\d+)(?:ms)?", output)
    if m:
        return float(m.group(1))
    # Unix-like: "rtt min/avg/max/mdev = 23.456/45.678/67.890/1.234 ms"
    m = re.search(r"=\s*([\d\.]+)/([\d\.]+)/([\d\.]+)", output)
    if m:
        try:
            return float(m.group(2))
        except Exception:
            pass
    # Another possible format
    m = re.search(r"round-trip.*=\s*([\d\.]+)/([\d\.]+)/([\d\.]+)", output)
    if m:
        try:
            return float(m.group(2))
        except Exception:
            pass
    return None

def ping_host(host: str, count: int = DEFAULT_PING_COUNT) -> Optional[float]:
    # Resolve host first to fail fast on unknown hosts
    try:
        socket.getaddrinfo(host, None)
    except Exception:
        return None

    is_windows = sys.platform.startswith("win")
    cmd = ["ping", "-n" if is_windows else "-c", str(count), host]
    try:
        # timeout: give a little buffer per ping
        timeout = max(10, count * 3)
        proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, timeout=timeout)
        out = proc.stdout or ""
        avg = parse_ping_output(out)
        return avg
    except Exception:
        return None

def main():
    count = DEFAULT_PING_COUNT
    if len(sys.argv) >= 2:
        try:
            count = int(sys.argv[1])
            if count <= 0:
                count = DEFAULT_PING_COUNT
        except Exception:
            pass

    # lines = fetch_lines(RAW_URL)
    lines = fetch_lines_from_file("doh-list-source.txt")
    # map original line -> host
    entries = []
    for line in lines:
        host = extract_host(line)
        if host:
            entries.append((line, host))

    # ping unique hosts in parallel
    host_to_latency = {}
    unique_hosts = sorted({host for _, host in entries})

    with concurrent.futures.ThreadPoolExecutor(max_workers=20) as ex:
        futures = {ex.submit(ping_host, host, count): host for host in unique_hosts}
        for fut in concurrent.futures.as_completed(futures):
            host = futures[fut]
            try:
                latency = fut.result()
            except Exception:
                latency = None
            host_to_latency[host] = latency
            print(f"Finished {(len(host_to_latency) / len(unique_hosts)) * 100:.1f}% - {host}: {latency if latency is not None else 'failed'} ms")

    # build list of (original_line, host, latency)
    scored = []
    for line, host in entries:
        latency = host_to_latency.get(host)
        if latency is not None:
            scored.append((line, host, latency))

    # sort by latency ascending
    scored.sort(key=lambda t: t[2])

    # take top N unique hosts (preserve original lines for the host; if multiple lines share same host, keep them all but we only need unique hosts)
    seen_hosts = set()
    top_lines = []
    for line, host, latency in scored:
        if host in seen_hosts:
            continue
        seen_hosts.add(host)
        top_lines.append((line, host, latency))
        # if len(top_lines) >= TOP_N:
        #     break

    # write output file
    with open(OUTFILE, "w", encoding="utf-8") as f:
        for line, host, latency in top_lines:
            f.write(f"{line}  # {latency:.2f} ms\n")

    # also print summary to stdout
    for line, host, latency in top_lines:
        print(f"{latency:.2f} ms  {host}  ->  {line}")

if __name__ == "__main__":
    main()