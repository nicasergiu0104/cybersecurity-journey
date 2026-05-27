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

Scanning target: 10.10.10.2
Started at: 2026-03-29 13:00:58
[+] Port 135: OPEN
[+] Port 139: OPEN
[+] Port 445: OPEN
Total open ports found: 3
Open ports: [135, 139, 445]


## Key Concepts

- **TCP Connect scan**: completes full 3-way handshake (SYN → SYN-ACK → ACK)
- **Threading**: `ThreadPoolExecutor` manages 100 concurrent connections
- **Race condition prevention**: `threading.Lock()` protects shared `open_ports` list
- **Validated against**: Nmap on Windows 7 target — confirmed MS17-010 
  (EternalBlue) vulnerability on port 445
