
import socket
import sys
from datetime import datetime

target = input("Enter target IP: ")

print(f"Scanning target: {target}")
print(f"Started at: {datetime.now()}")
print("-" * 50)

open_ports = []

for port in range(1, 1025):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	socket.setdefaulttimeout(1)
	result = sock.connect_ex((target, port))
	if result == 0:
		print(f"[+] Port {port}: OPEN")
		open_ports.append(port)
	sock.close()

print("-" * 50)
print(f"Total open ports found: {len(open_ports)}")
print(f"Open ports: {open_ports}")
print(f"Scan completed at : {datetime.now()}")

