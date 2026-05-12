# Cybersecurity & Engineering Journey

> Computer Engineering student transitioning into offensive and defensive security.
> Documenting practical work — tools built, labs run, systems broken, and hardware hacked.

---

## 🗺️ Master Roadmap

| Timeline | Focus | Core Curriculum / Target | Status |
|----------|-------|--------------------------|--------|
| **Month 1** | Networking Basics & OS Internals | Cisco CCNA 1 | 🟡 In Progress |
| **Month 2** | IoT Security & Applied Crypto | Custom Hardware Projects | 🟡 In Progress |
| **Month 3** | Web App Sec & Active Directory | eJPT / CompTIA Security+ | ⏳ Upcoming |
| **Month 4+** | Advanced Offensive Operations | OSCP Preparation | ⏳ Upcoming |

---

## 🛠️ Tools & Stack

| Category | Technologies Used |
|----------|-------------------|
| **Languages** | Python, C++, Bash |
| **Networking** | Wireshark, Nmap, Custom Sockets |
| **Hardware / IoT** | ESP8266 (NodeMCU), Sonoff DUAL R3, Arduino IDE |
| **Cryptography** | BearSSL, AES-128-CBC, MD5, Hashcat |
| **OS Internals** | `/proc` filesystem, `strace`, `ltrace`, GTFOBins |

---

## 🚀 Progress Log

### Month 2 — IoT Security & Applied Cryptography
**Highlight: Project Ruby (Secure IoT Switch)**
* Built an ESP8266-based acoustic switch (double-clap detection) to control a Sonoff DUAL R3 relay over the local network, bypassing manufacturer cloud infrastructure.
* **Cryptographic Implementation:** Crafted a custom C++ HTTP client implementing AES-128-CBC encryption, PKCS7 padding, dynamic IV generation, and Base64 encoding using `BearSSL` to securely communicate with the Sonoff LAN API. 
* **Hardware:** Handled logic debouncing, hardware interrupts, and secure OTA (Over-The-Air) firmware updates.

### Month 1 — Reconnaissance & Linux Internals
**Highlight: Custom Tooling & OS Exploitation**
* **TCP Port Scanners:** Wrote raw socket connect scanners in Python (v1 basic banner grabbing; v2 multi-threaded for speed and cleaner CLI output).
* **/proc Filesystem & Tracing:** Mapped live processes, file descriptor tables, and UID/EUID transitions. Used `strace` to capture and annotate full userspace-to-kernel transitions.
* **Privilege Escalation Foundations:** Mapped root-owned SUID binaries on a test machine, cross-referenced exploitation paths via GTFOBins, and verified a PAM (Pluggable Authentication Module) `nullok` configuration bypass on a default Kali installation.

---
*Last updated: May 2026*
