import socket
import sys
from datetime import datetime
from concurrent.futures import ThreadPoolExecutor
import threading

target = input("Enter target IP: ")
timeout = float(input("Timeout in seconds (0.5 recommended): "))

print(f"Scanning target: {target}")
print(f"Started at: {datetime.now()}")
print("-" * 50)

open_ports = []
lock = threading.Lock()

def scan_port(port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(timeout)
    result = sock.connect_ex((target, port))
    if result == 0:
        with lock:
            open_ports.append(port)
            print(f"[+] Port {port}: OPEN")
    sock.close()

# Scan using 100 threads simultaneously
with ThreadPoolExecutor(max_workers=100) as executor:
    executor.map(scan_port, range(1, 1025))

print("-" * 50)
open_ports.sort()
print(f"Total open ports found: {len(open_ports)}")
print(f"Open ports: {open_ports}")
print(f"Completed at: {datetime.now()}")
