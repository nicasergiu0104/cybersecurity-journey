# TCP Port Scanner

A TCP connect port scanner built from scratch in Python using raw sockets.
No external libraries — only the standard `socket` module.

## Versions

### v1 — Sequential Scanner (`port_scanner.py`)
Scans ports 1–1024 one at a time using TCP connect.
Includes 1-second timeout per port and basic banner grabbing.

**Scan type:** TCP Connect (-sT equivalent)  
**Speed:** ~17 minutes worst case at 1s timeout  
**Privileges required:** None (uses OS socket layer)

### v2 — Threaded Scanner (`port_scanner_v2.py`)
Same logic but uses `ThreadPoolExecutor` with 100 parallel workers.
100x faster than v1. Configurable timeout via user input.

**Scan type:** TCP Connect, multithreaded  
**Speed:** ~5–10 seconds for 1024 ports  
**Privileges required:** None

## Usage

```bash
python3 port_scanner.py
# Enter target IP when prompted

python3 port_scanner_v2.py
# Enter target IP and timeout when prompted
```

## Sample Output
