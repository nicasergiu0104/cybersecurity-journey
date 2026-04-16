# Cybersecurity Journey

> Computer Engineering student transitioning into offensive and defensive security.
> Documenting practical work — tools built, labs run, systems broken.

---

## Roadmap

| Phase | Focus | Target Cert | Status |
|-------|-------|-------------|--------|
| 1 | Networking + Linux fundamentals | — | In progress |
| 2 | Web app security + scripting | eJPT | Upcoming |
| 3 | Active Directory + exploitation | CompTIA Security+ | Upcoming |
| 4 | Full offensive ops | OSCP | Upcoming |

---

## Progress

### Week 1 — Recon & Scanning Tools

| What | Output |
|------|--------|
| TCP port scanner v1 | Raw socket connect scan, banner grabbing |
| TCP port scanner v2 | Threaded version, cleaner output |

---

### Week 2 — Networking Protocols + Linux Internals

| What | Output |
|------|--------|
| /proc filesystem inspection | Live process map, fd table, UID/EUID from /proc/2526 |
| strace on ls | Full 96-syscall trace captured and summarized |
| strace annotated | Line-by-line syscall breakdown |
| SUID enumeration | 32 root-owned SUID binaries, GTFOBins cross-check |
| PAM stack analysis | Confirmed nullok bypass on Kali default config |

---

## Tools & Stack

| Tool | Used for |
|------|----------|
| Python (sockets) | Custom TCP port scanners |
| Nmap | Port scanning, NSE scripts, vuln detection |
| Wireshark | Packet capture and protocol analysis |
| strace | Syscall tracing |
| ltrace | Library call tracing |
| /proc | Live process inspection |
| GTFOBins | SUID binary exploitation reference |
| Hashcat | Hash cracking — dictionary and brute-force |

---

*Last updated: 16.04.2026 — Week 2*
