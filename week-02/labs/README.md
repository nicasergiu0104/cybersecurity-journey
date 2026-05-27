# Linux OS Internals — Lab Notes

Practical exercises in Linux process tracing, filesystem analysis,
and privilege escalation foundations on Kali Linux.

## Files

### `strace_ls_tmp.txt`
Raw `strace` output from tracing the `ls /tmp` command.
Captures every syscall made during execution — `openat`, `getdents64`,
`write`, `close` — showing the full userspace-to-kernel transition path.

### `strace_summary.txt`
Annotated summary of the strace output.
Each syscall identified, explained, and mapped to its OS function.
Documents how a simple `ls` command results in 50+ kernel interactions.

### `suid_binaries.txt`
Output of SUID binary enumeration on a default Kali installation:
```bash
find / -perm -4000 -type f 2>/dev/null
```
Binaries cross-referenced against GTFOBins for privilege escalation paths.
Includes verification of a PAM `nullok` configuration bypass.

## Key Findings

- Mapped UID/EUID transitions during privileged binary execution
- Identified exploitable SUID binaries present in default Kali install
- Verified PAM `nullok` misconfiguration allows authentication bypass
  without password on affected accounts
