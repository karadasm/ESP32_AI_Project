#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// OLED ayarları
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// WiFi bilgileri
const char* ssid = "karadas_internet_hayrati";
const char* password = "mkmk1249";
// OpenRouter API bilgileri
const char* openai_api_key = "Bearer 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";
const char* api_url = "https://openrouter.ai/api/v1/chat/completions";
// HTTP sunucu port 80
WebServer server(80);
// LED pini
#define LED_PIN 5
// Fonksiyon prototipleri
String sendMessageToGPT(String mesaj);
void yavasYaz(String yazi);
void handlePostMessage();
String toLowerTurkish(String str);
String replaceTurkishChars(String str);
void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED başlatılamadı");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("WiFi baglaniyor...");
  display.display();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi baglandi! IP: ");
  Serial.println(WiFi.localIP());
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi BAGLANDI!");
  display.println(WiFi.localIP().toString());
  display.display();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  server.on("/mesaj", HTTP_POST, handlePostMessage);
  server.begin();
  Serial.println("HTTP Sunucu başlatıldı");
}
void loop() {
  server.handleClient();
}
void handlePostMessage() {
  if (server.hasArg("plain")) {
    String mesaj = server.arg("plain");
    mesaj = toLowerTurkish(mesaj);
    Serial.println("Gelen mesaj: " + mesaj);
    if (mesaj.indexOf("ışığı aç") >= 0) {
      digitalWrite(LED_PIN, HIGH);
      yavasYaz("Isigi aciyorum");
      server.send(200, "text/plain", "Isik acildi");
    } else if (mesaj.indexOf("ışığı kapat") >= 0) {
      digitalWrite(LED_PIN, LOW);
      yavasYaz("Isigi kapatiyorum");
      server.send(200, "text/plain", "Isik kapandi");
    } else {
      String yanit = sendMessageToGPT(mesaj);
      yavasYaz(yanit);
      server.send(200, "text/plain", "Mesaj alındı ve yanıtlandı");
    }
  } else {
    server.send(400, "text/plain", "Hata: Mesaj yok");
  }
}
String sendMessageToGPT(String mesaj) {
  HTTPClient http;
  http.begin(api_url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", openai_api_key);
  String jsonData = "{\"model\":\"gpt-3.5-turbo\",\"messages\":[{\"role\":\"user\",\"content\":\"" + mesaj + "\"}]}";
  int httpResponseCode = http.POST(jsonData);
  Serial.println("HTTP kodu: " + String(httpResponseCode));
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Yanıt:\n" + response);
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
      Serial.println("JSON hatası: ");
      Serial.println(error.c_str());
      return "JSON hatasi!";
    }
    String gptYaniti = doc["choices"][0]["message"]["content"].as<String>();
    return gptYaniti;
  } else {
    Serial.println("HTTP hatasi: " + http.errorToString(httpResponseCode));
    return "API hatasi!";
  }
}
// OLED'de düzgün satırlarla yazı yazma
void yavasYaz(String yazi) {
  yazi = replaceTurkishChars(yazi); // Türkçe sadeleştirme
  String ekranda = "";
  int currentLine = 0;
  for (int i = 0; i < yazi.length(); i++) {
    ekranda += yazi[i];
    display.clearDisplay();
    // Her satır için ayrı yaz
    for (int line = 0; line <= currentLine; line++) {
      int startIdx = line * 21;
      int endIdx = startIdx + 21;
      if (endIdx > ekranda.length()) endIdx = ekranda.length();
      display.setCursor(0, line * 8);
      display.print(ekranda.substring(startIdx, endIdx));
    }
    display.display();
    delay(50);
    // Yeni satıra geç
    if ((ekranda.length() % 21) == 0 && i != 0) {
      currentLine++;
    }
    // 7 satırdan sonra sıfırla
    if (currentLine >= 7) {
      ekranda = "";
      currentLine = 0;
    }
  }
}
// Türkçe küçük harf düzeltme
String toLowerTurkish(String str) {
  str.replace("İ", "i");
  str.replace("I", "ı");
  str.replace("Ş", "ş");
  str.replace("Ğ", "ğ");
  str.replace("Ü", "ü");
  str.replace("Ö", "ö");
  str.replace("Ç", "ç");
  str.toLowerCase();
  return str;
}
// OLED uyumlu sade Türkçe karakterler
String replaceTurkishChars(String str) {
  str.replace("ç", "c");
  str.replace("ğ", "g");
  str.replace("ş", "s");
  str.replace("ı", "i");
  str.replace("ö", "o");
  str.replace("ü", "u");
  str.replace("Ç", "C");
  str.replace("Ğ", "G");
  str.replace("Ş", "S");
  str.replace("İ", "I");
  str.replace("Ö", "O");
  str.replace("Ü", "U");
  return str;
}
