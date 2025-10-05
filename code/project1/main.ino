Proje1:
#include <WiFi.h>  // ESP32 WiFi kütüphanesi
// Wi-Fi bilgilerini buraya yaz
const char* ssid = "karadas_internet_hayrati";         // Wi-Fi SSID (ağ adı)
const char* password = "mkmk1249";   // Wi-Fi şifresi
void setup() {
  Serial.begin(115200); // Seri haberleşmeyi başlat
  WiFi.begin(ssid, password); // Wi-Fi ağına bağlanmayı dene
  Serial.print("Wi-Fi'ye bağlanıyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("✅ Bağlantı başarılı!");
  Serial.print("💻 IP Adresi: ");
  Serial.println(WiFi.localIP()); // IP adresini yazdır
}
void loop() {
  // Wi-Fi bağlantısı kontrolü
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ Bağlantı koptu! Yeniden bağlanılıyor...");
    WiFi.begin(ssid, password);
  }
  delay(10000); // Her 10 saniyede bir kontrol et
}
