# Project Ruby — Secure IoT Switch

ESP8266-based acoustic switch (double-clap detection) that controls a 
Sonoff DUAL R3 relay over the LAN — bypassing the manufacturer cloud 
infrastructure entirely by implementing the Sonoff LAN API protocol 
directly.

## What it does

A double clap triggers a relay toggle on a Sonoff DUAL R3 smart switch, 
without routing through eWeLink's cloud servers. All communication stays 
on the local network and is AES-128-CBC encrypted end-to-end.

## How it works

**1. Sonoff LAN API reverse engineering.** Sonoff devices accept 
encrypted commands over LAN on port 8081, but the API isn't publicly 
documented. I extracted the AES session key by inspecting the eWeLink 
web client's network traffic in browser DevTools and identifying the 
key derivation function (MD5 of the device key).

**2. Cryptographic implementation.** Custom C++ HTTP client running on 
ESP8266 (NodeMCU) using BearSSL:
- AES-128-CBC encryption of JSON payloads
- PKCS7 padding to align to 16-byte blocks
- Dynamic IV generation per request (16 random alphanumeric bytes)
- Base64 encoding of ciphertext and IV
- POST to `http://<device-ip>:8081/zeroconf/switches`

**3. Acoustic detection.** A KY-038 microphone sampled on ADC pin A0. 
Finite-state machine (`WAIT_FOR_SILENCE → LISTENING → WAIT_FOR_CLAP_2 
→ TRIGGERED`) gates the detection. Each candidate sound is profiled for 
attack time, duration, and peak amplitude in `profileSoundEvent()` and 
filtered in `isGenuineClap()`.

**4. Resilience.** OTA firmware updates with password protection, 
periodic WiFi health checks every 30s with automatic reconnection, 
runtime calibration of the ambient noise floor at boot.

## Hardware

- ESP8266 NodeMCU
- KY-038 sound sensor (analog out to A0)
- Sonoff DUAL R3 smart switch on the same LAN

Photos pending.

## Build & flash

1. Copy `config.h.example` to `config.h` and fill in your network and 
   Sonoff credentials.
2. Open `ruby.ino` in Arduino IDE with ESP8266 board support installed.
3. Required libraries: `ESP8266WiFi`, `ESP8266mDNS`, `ArduinoOTA`, 
   `ESP8266HTTPClient`, `bearssl/bearssl_block.h` (bundled with ESP8266 
   core).
4. Flash over USB the first time, OTA thereafter.

## Current state and known issues

v1 of the detection algorithm achieved 10/10 recall on intentional 
double-claps but suffered from false positives on door slams (sharp 
impulsive noise with similar envelope characteristics) and group 
laughter (voiced bursts with clap-like attack profiles).

v2 tightened the attack-time, duration, and amplitude filters in 
`isGenuineClap()` and eliminated most false positives, but at the cost 
of recall — current performance is approximately 2/10 on intentional 
double-claps. This is a precision-recall tradeoff inherent to 
envelope-based detection in the time domain.

## Planned improvements

- Add a peak-to-duration ratio filter (`peakAmplitude / durationMs`) 
  to distinguish claps (high ratio) from doors and laughter (lower 
  ratio) without further tightening primary thresholds.
- Derive thresholds dynamically from the runtime-measured ambient 
  noise floor rather than using compile-time constants.
- Long-term: replace envelope-based detection with FFT spectral 
  analysis to discriminate based on the frequency signature of a clap 
  (broadband impulse, ~2–4 kHz peak) versus doors (low-frequency 
  dominant) and voiced sound (harmonic structure). ESP8266 constraints 
  make this nontrivial — likely needs fixed-point FFT (e.g., 
  `arduinoFFT` library in int16 mode) on a 512-sample window.

## Why this project

Most "smart" home devices route everything through manufacturer cloud 
infrastructure even on the local network. This project demonstrates 
that with the right primitives — a working understanding of the 
device's protocol and a correct implementation of AES-128-CBC — you 
can keep everything local while preserving end-to-end encryption.