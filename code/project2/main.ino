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
// Yazı pozisyonu OLED için
int currentLine = 0;
String ekranda = "";
// Fonksiyon prototipleri
String sendMessageToGPT(String mesaj);
void yavasYaz(String yazi);
void handlePostMessage();
String replaceTurkishChars(String str);
void setup() {
  Serial.begin(115200);
  // OLED başlat
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
  // WiFi'ye bağlan
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
  // HTTP POST endpoint ayarla
  server.on("/mesaj", HTTP_POST, handlePostMessage);
  // Sunucuyu başlat
  server.begin();
  Serial.println("HTTP Sunucu başlatıldı");
}
void loop() {
  server.handleClient();
}
// === Türkçe karakterleri sadeleştirme ===
String replaceTurkishChars(String str) {
  str.replace("ç", "c");  str.replace("Ç", "C");
  str.replace("ğ", "g");  str.replace("Ğ", "G");
  str.replace("ı", "i");  str.replace("İ", "I");
  str.replace("ö", "o");  str.replace("Ö", "O");
  str.replace("ş", "s");  str.replace("Ş", "S");
  str.replace("ü", "u");  str.replace("Ü", "U");
  return str;
}
// === POST isteği ile gelen mesajı işleyen fonksiyon ===
void handlePostMessage() {
  if (server.hasArg("plain")) {
    String mesaj = server.arg("plain");
    Serial.println("Gelen mesaj: " + mesaj);
    String yanit = sendMessageToGPT(mesaj);
    yavasYaz(yanit);
    server.send(200, "text/plain", "Mesaj alındı");
  } else {
    server.send(400, "text/plain", "Hata: Mesaj yok");
  }
}
// === OpenRouter API'ye mesaj gönderip cevap alan fonksiyon ===
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
// === OLED ekranda yavaş yazdırma fonksiyonu ===
void yavasYaz(String yazi) {
  yazi = replaceTurkishChars(yazi);  // Türkçeyi sadeleştir
  display.clearDisplay();
  display.setCursor(0, 0);
  currentLine = 0;
  ekranda = "";
  for (int i = 0; i < yazi.length(); i++) {
    ekranda += yazi[i];
    String satir = ekranda.substring(currentLine * 21, (currentLine + 1) * 21);
    display.setCursor(0, currentLine * 8);
    display.print(satir);
    display.display();
    delay(50);
    if (satir.length() >= 21 || i == yazi.length() - 1) {
      currentLine++;
      if (currentLine >= 7) {
        delay(1000); // Biraz bekle
        display.clearDisplay();
        currentLine = 0;
        ekranda = "";
      }
    }
  }
}

