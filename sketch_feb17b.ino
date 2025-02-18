#include "WiFi.h"
#include "HTTPClient.h"

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* audio_url = "http://yourserver.com/hello.wav";  // Update with your correct URL

#define AUDIO_PIN 25  // GPIO25 (Use GPIO26 if needed)

void setup() {
    Serial.begin(115200);
    pinMode(AUDIO_PIN, OUTPUT);

    // Connect to Wi-Fi
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ Wi-Fi Connected!");

    // Start streaming
    playAudioFromWeb(audio_url);
}

void loop() {
    // Nothing in loop
}

void playAudioFromWeb(const char* url) {
    HTTPClient http;
    http.begin(url);

    int httpCode = http.GET();
    Serial.print("HTTP Response Code: ");
    Serial.println(httpCode);

    if (httpCode == HTTP_CODE_OK) {
        WiFiClient* stream = http.getStreamPtr();

        Serial.println("🎵 Playing Audio...");
        while (stream->available()) {
            int audioSample = stream->read();
            if (audioSample < 0) continue;  // Skip invalid data
            dacWrite(AUDIO_PIN, audioSample);  // Send sample to DAC
            delayMicroseconds(125);  // Adjust timing if needed
        }
        Serial.println("✅ Audio playback complete.");
    } else {
        Serial.println("❌ Error: Could not fetch audio file.");
    }

    http.end();
}
