# Cybersecurity Journey 🛡️

Computer Engineering student transitioning into cybersecurity.
Documenting every step of a structured 12-month roadmap.

## Goal
Master's-ready and Junior Pentester/SOC Analyst 
competitive by March 2027.

---

## Week 1 — Completed ✅

### Theoretical Foundations
- **EternalBlue (CVE-2017-0144)** — full exploit chain analysis
  (integer overflow → heap grooming → kernel shellcode execution)
- **Heap spray, grooming, nonpaged pool** — kernel exploitation mechanics
- **SMB Protocol** — history, v1/v2/v3 security implications
- **TCP/IP internals** — 3-way handshake, sequence numbers,
  SYN flood, session hijacking (blind vs non-blind)
- **DNS** — record types, resolution flow, TTL attacks, Fast Flux
- **HTTP** — methods, status codes, cookie security flags
- **SQL Injection** — tautology attack, query manipulation
- **ARP** — poisoning mechanics, MitM enablement

### Practical Work
- Wireshark: captured HTTP traffic, TLS encryption,
  TCP 3-way handshake — annotated every packet field
- Identified Apache version via banner grabbing
- Observed SNI leakage in encrypted TLS traffic
- Built Python TCP port scanner from scratch (v1 + v2 threaded)
- Nmap: 8 exercises covering all scan types, NSE scripts,
  timing, evasion, and full pentest methodology
- Confirmed EternalBlue vulnerability on Win7 target
  using both Nmap and custom Python scanner

### Tools Used
| Tool | Purpose |
|---|---|
| Wireshark | Traffic capture and packet analysis |
| Nmap | Network enumeration and vulnerability scanning |
| Python (socket) | Raw TCP port scanner built from scratch |
| Git | Version control and portfolio management |

### Key Finding
```
nmap --script smb-vuln-ms17-010 -p 445 10.10.10.2
→ VULNERABLE (Risk: HIGH)

python3 port_scanner_v2.py → 10.10.10.2
→ Port 135: OPEN (RPC)
→ Port 139: OPEN (NetBIOS)  
→ Port 445: OPEN (SMB ← EternalBlue lives here)
```

---

## Roadmap Progress

| Month | Focus | Status |
|---|---|---|
| 1 | Networking Internals & OS Fundamentals | 🔄 In Progress |
| 2 | Cryptography & Vulnerability Theory | ⬜ Upcoming |
| 3 | Penetration Testing Methodology | ⬜ Upcoming |
| 4 | Exploitation & eJPT Certification | ⬜ Upcoming |
| 5 | Web Application Security | ⬜ Upcoming |
| 6 | Active Directory Attacks | ⬜ Upcoming |
| 7 | Privilege Escalation & Pivoting | ⬜ Upcoming |
| 8 | Blue Team & SIEM | ⬜ Upcoming |
| 9 | Incident Response & Malware Analysis | ⬜ Upcoming |
| 10 | Cloud Security & Evasion | ⬜ Upcoming |
| 11 | Certifications Sprint | ⬜ Upcoming |
| 12 | Master's Application & Portfolio | ⬜ Upcoming |

---

## Certifications Target
- [ ] eJPT — target: Month 4
- [ ] CompTIA Security+ — target: Month 11
- [ ] OSCP — target: 2027

---

## Tools & Technologies (Cumulative)
`Wireshark` `Nmap` `Python` `Git` `Kali Linux` 
`VirtualBox` `Obsidian`

---

*Started: March 2026 | Updated weekly*
```

---


```
