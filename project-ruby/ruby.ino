#include "config.h"

const int MIC_PIN = A0;
const int LED_PIN = LED_BUILTIN;

const int NOISE_FLOOR       = 576;
const int SILENCE_THRESHOLD = 70;
const int CLAP_THRESHOLD    = 200;

const unsigned long ATTACK_MAX_MS   = 10;
const unsigned long DURATION_MAX_MS = 80;
const unsigned long DURATION_MIN_MS = 2;

const unsigned long PRE_SILENCE_MS    = 120;
const unsigned long INTER_CLAP_MIN    = 150;
const unsigned long INTER_CLAP_MAX    = 900;
const unsigned long POST_CLAP_WAIT    = 1500;
const unsigned long STARTUP_IGNORE_MS = 2000;


const unsigned long WIFI_CHECK_INTERVAL = 30000;  
unsigned long lastWifiCheck = 0;


enum ClapFSMState {
  WAIT_FOR_SILENCE,
  LISTENING,
  WAIT_FOR_CLAP_2,
  TRIGGERED
};

ClapFSMState  fsmState       = WAIT_FOR_SILENCE;
unsigned long silenceStartMs = 0;
unsigned long clap1EndMs     = 0;


bool light_state = false;


struct EventProfile {
  int           peakAmplitude;
  unsigned long durationMs;
  unsigned long attackMs;
  bool          valid;
};


void          blinkFeedback();
void          toggleRelay();
int           readAmplitude();
EventProfile  profileSoundEvent();
bool          isGenuineClap(const EventProfile& ep);
void          yieldDelay(unsigned long ms);
bool          ensureWiFi();


inline int readAmplitude() {
  return abs(analogRead(MIC_PIN) - NOISE_FLOOR);
}


void yieldDelay(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    ArduinoOTA.handle();
    yield();
    delay(10);
  }
}


bool ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return true;

  Serial.println("[WiFi] Deconectat — incerc reconectare...");
  WiFi.disconnect();
  delay(100);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 8000) {
    delay(200);
    yield();
    Serial.print('.');
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n[WiFi] Reconectat! IP: %s\n", WiFi.localIP().toString().c_str());
    return true;
  }
  Serial.println("\n[WiFi] Reconectare esuata.");
  return false;
}


EventProfile profileSoundEvent() {
  EventProfile ep = {0, 0, 0, false};

  unsigned long startMs = millis();
  unsigned long peakMs  = startMs;
  int localPeak = 0;

  while (true) {
    delayMicroseconds(200);
    int amp = readAmplitude();

    if (amp > localPeak) {
      localPeak = amp;
      peakMs    = millis();
    }

    if (amp < CLAP_THRESHOLD) {
      delayMicroseconds(400);
      if (readAmplitude() < CLAP_THRESHOLD) {
        ep.durationMs    = millis() - startMs;
        ep.peakAmplitude = localPeak;
        ep.attackMs      = peakMs - startMs;
        ep.valid         = true;
        return ep;
      }
    }

    if ((millis() - startMs) > DURATION_MAX_MS * 3) {
      ep.durationMs    = millis() - startMs;
      ep.peakAmplitude = localPeak;
      ep.attackMs      = peakMs - startMs;
      ep.valid         = false;
      return ep;
    }
  }
}

bool isGenuineClap(const EventProfile& ep) {

  if (!ep.valid) {
    Serial.printf("[REJECT] Zgomot sustinut — durata: %lums\n", ep.durationMs);
    return false;
  }
  if (ep.attackMs > ATTACK_MAX_MS) {
    Serial.printf("[REJECT] Atac prea lent: %lums → usa/bufnitura\n", ep.attackMs);
    return false;
  }
  if (ep.durationMs > DURATION_MAX_MS) {
    Serial.printf("[REJECT] Durata prea lunga: %lums → ras/vorbit\n", ep.durationMs);
    return false;
  }
  if (ep.durationMs < DURATION_MIN_MS) {
    Serial.printf("[REJECT] Durata prea scurta: %lums → glitch\n", ep.durationMs);
    return false;
  }
  if (ep.peakAmplitude < CLAP_THRESHOLD) {
    Serial.println("[REJECT] Amplitudine sub prag");
    return false;
  }

  Serial.printf("[CLAP] VALID ✓ — atac: %lums | durata: %lums | varf: %d\n",
                ep.attackMs, ep.durationMs, ep.peakAmplitude);
  return true;
}

// ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);              
  WiFi.setAutoReconnect(true);         
  WiFi.setSleepMode(WIFI_NONE_SLEEP);  
  WiFi.config(IPAddress(192,168,1,20),
              IPAddress(192,168,1,1),
              IPAddress(255,255,255,0));
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("[WiFi] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print('.');
  }
  Serial.println();
  Serial.printf("[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("[WiFi] RSSI: %d dBm\n", WiFi.RSSI());

  
  ArduinoOTA.setHostname("ruby-clapswitch");
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.onStart([]()   { Serial.println("[OTA] Starting..."); });
  ArduinoOTA.onEnd([]()     { Serial.println("\n[OTA] Done!"); });
  ArduinoOTA.onProgress([](unsigned int p, unsigned int t) {
    Serial.printf("[OTA] %u%%\r", p / (t / 100));
  });
  ArduinoOTA.onError([](ota_error_t e) {
    Serial.printf("[OTA] Error[%u]\n", e);
  });
  ArduinoOTA.begin();

  
  Serial.println("[RUBY] Calibrare — fii linistit 2 secunde...");
  long sum = 0;
  int maxDev = 0;
  const int samples = 1000;
  for (int i = 0; i < samples; i++) {
    int raw = analogRead(MIC_PIN);
    sum += raw;
    int dev = abs(raw - NOISE_FLOOR);
    if (dev > maxDev) maxDev = dev;
    delayMicroseconds(2000);
  }
  int measuredFloor = sum / samples;
  Serial.printf("[RUBY] Floor masurat: %d | Deviatie ambient: %d\n", measuredFloor, maxDev);

  if (maxDev > SILENCE_THRESHOLD) {
    Serial.println("[RUBY] ⚠ Deviatie ambientala mare — verifica microfonul.");
  }
  if (abs(measuredFloor - NOISE_FLOOR) > 50) {
    Serial.printf("[RUBY] ⚠ Floor masurat (%d) ≠ NOISE_FLOOR (%d)\n",
                  measuredFloor, NOISE_FLOOR);
  }

  delay(STARTUP_IGNORE_MS);
  silenceStartMs = millis();
  lastWifiCheck  = millis();
  Serial.println("[RUBY] Ruby este treaza si asculta batai duble!");
}


void loop() {
  ArduinoOTA.handle();
  yield();

  unsigned long now = millis();

  
  if (now - lastWifiCheck > WIFI_CHECK_INTERVAL) {
    lastWifiCheck = now;
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("[WiFi] Health check: deconectat, reconectez...");
      ensureWiFi();
    }
  }

  int amplitude = readAmplitude();

  switch (fsmState) {

    case WAIT_FOR_SILENCE:
      if (amplitude < SILENCE_THRESHOLD) {
        if ((now - silenceStartMs) >= PRE_SILENCE_MS) {
          fsmState = LISTENING;
        }
      } else {
        silenceStartMs = now;
      }
      break;

    case LISTENING:
      if (amplitude >= CLAP_THRESHOLD) {
        EventProfile ep = profileSoundEvent();
        if (isGenuineClap(ep)) {
          Serial.println("[CLAP] count = 1");
          clap1EndMs = millis();
          fsmState   = WAIT_FOR_CLAP_2;
        } else {
          silenceStartMs = millis();
          fsmState = WAIT_FOR_SILENCE;
        }
      }
      break;

    case WAIT_FOR_CLAP_2: {
      unsigned long elapsed = now - clap1EndMs;

      if (elapsed > INTER_CLAP_MAX) {
        Serial.printf("[FSM] Timeout clap 2 (%lums) — reset\n", elapsed);
        silenceStartMs = now;
        fsmState = WAIT_FOR_SILENCE;
        break;
      }

      if (elapsed < INTER_CLAP_MIN) break;

      if (amplitude >= CLAP_THRESHOLD) {
        EventProfile ep = profileSoundEvent();
        if (isGenuineClap(ep)) {
          Serial.println("[CLAP] count = 2");
          fsmState = TRIGGERED;
        } else {
          Serial.println("[FSM] Al doilea eveniment nu e bataie — reset");
          silenceStartMs = now;
          fsmState = WAIT_FOR_SILENCE;
        }
      }
      break;
    }

    case TRIGGERED:
      Serial.println("[CLAP] DOUBLE-CLAP! Toggling...");
      toggleRelay();
      blinkFeedback();
      silenceStartMs = millis();
      fsmState       = WAIT_FOR_SILENCE;
      yieldDelay(POST_CLAP_WAIT);
      break;

    default:
      fsmState = WAIT_FOR_SILENCE;
      silenceStartMs = millis();
      break;
  }
}

// ─────────────────────────────────────────────────────
void blinkFeedback() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_PIN, LOW);
    delay(80);
    digitalWrite(LED_PIN, HIGH);
    delay(80);
  }
}

// ─────────────────────────────────────────────────────
void toggleRelay() {
  
  if (!ensureWiFi()) {
    Serial.println("[SONOFF] WiFi indisponibil, omitem.");
    return;
  }

  light_state = !light_state;
  String sw = light_state ? "on" : "off";
  Serial.printf("[SONOFF] Încercare comutare pe: %s\n", sw.c_str());

  MD5Builder md5;
  md5.begin();
  md5.add(DEVICE_KEY);
  md5.calculate();
  uint8_t aes_key[16];
  md5.getBytes(aes_key);

  char iv[17];
  const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  for (int i = 0; i < 16; i++) {
    iv[i] = alphanum[random(0, 62)];
  }
  iv[16] = '\0';

  String payload = "{\"switches\": [{\"switch\": \"" + sw + "\", \"outlet\": 0}, {\"switch\": \"" + sw + "\", \"outlet\": 1}]}";

  size_t payload_len = payload.length();
  uint8_t pad_len    = 16 - (payload_len % 16);
  size_t padded_len  = payload_len + pad_len;
  uint8_t padded_payload[128];
  memcpy(padded_payload, payload.c_str(), payload_len);
  for (size_t i = payload_len; i < padded_len; i++) {
    padded_payload[i] = pad_len;
  }

  br_aes_ct_cbcenc_keys ctx;
  br_aes_ct_cbcenc_init(&ctx, aes_key, 16);
  uint8_t iv_copy[16];
  memcpy(iv_copy, iv, 16);
  uint8_t encrypted[128];
  memcpy(encrypted, padded_payload, padded_len);
  br_aes_ct_cbcenc_run(&ctx, iv_copy, encrypted, padded_len);

  String b64_data = base64::encode(encrypted, padded_len);
  String b64_iv   = base64::encode((uint8_t*)iv, 16);

  String sequence = String(millis());
  String body = "{\"deviceid\":\"" + String(DEVICE_ID) + "\","
              + "\"sequence\":\""   + sequence            + "\","
              + "\"selfApikey\":\"" + String(SELF_APIKEY) + "\","
              + "\"iv\":\""         + b64_iv              + "\","
              + "\"encrypt\":true,"
              + "\"data\":\""       + b64_data            + "\"}";

  WiFiClient client;
  HTTPClient http;
  http.setTimeout(3000);  
  String url = String("http://") + DEVICE_IP + ":8081/zeroconf/switches";
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  int code = http.POST(body);
  if (code > 0) {
    Serial.printf("[SONOFF] Raspuns %d: %s\n", code, http.getString().c_str());
  } else {
    Serial.printf("[SONOFF] Eroare HTTP: %s\n", http.errorToString(code).c_str());
  }
  http.end();
}