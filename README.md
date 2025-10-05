# 🚀 ESP32 + OpenRouter API ile Yapay Zeka ve IoT Kontrolü

Bu depo, **ESP32** mikrodenetleyici kullanılarak gerçekleştirilen üç farklı projeyi içerir.  
Projeler, hem yapay zeka yanıtlarını OLED ekranda göstermek hem de HTTP POST üzerinden mesaj alma ve LED kontrolü gibi IoT işlevlerini kapsamaktadır.

---

## 📘 Projeler

### 1️⃣ Proje 1: ESP32 ile Yapay Zeka Yanıt Gösterimi (OLED)
**Amaç:**  
ESP32 ile OpenRouter API üzerinden yapay zekaya mesaj göndermek ve yanıtını OLED ekranda harf harf göstermek.

**Özellikler:**  
- 🌐 WiFi üzerinden internet bağlantısı  
- 🤖 OpenRouter API entegrasyonu  
- 🖥️ OLED ekranda harf harf yanıt gösterimi  
- 🔤 Türkçe karakterleri sadeleştirme  

---

### 2️⃣ Proje 2: ESP32 HTTP Sunucusu ile Yapay Zeka Yanıtı
**Amaç:**  
Kullanıcının telefon veya bilgisayardan ESP32’ye mesaj göndererek, gelen mesajın GPT modelinden yanıt alıp OLED ekranda gösterilmesi.

**Özellikler:**  
- 📨 HTTP POST ile mesaj alma (`/mesaj` endpoint)  
- 🤖 OpenRouter API üzerinden yanıt alma  
- 🖥️ OLED ekranda satır satır ve harf harf yazdırma  
- 🔤 Türkçe karakterlerin OLED uyumlu hale getirilmesi  

---

### 3️⃣ Proje 3: ESP32 HTTP Sunucusu ile Mesaj Alma ve LED Kontrolü
**Amaç:**  
ESP32 üzerinde HTTP sunucusu ile mesaj alıp, belirli komutlara göre LED kontrolü yapmak ve diğer mesajları GPT modeline gönderip yanıtı OLED ekranda göstermek.

**Özellikler:**  
- 🌐 WiFi ağı üzerinden HTTP sunucu  
- 💡 "ışığı aç" / "ışığı kapat" komutlarıyla LED kontrolü  
- 🤖 OpenRouter API ile GPT-3.5-turbo yanıtı  
- 🖥️ OLED ekran üzerinde satır satır yanıt gösterimi  
- 🔤 Türkçe karakterleri sadeleştirme ve küçük harfe çevirme  

---

## 🛠️ Gerekli Malzemeler
- ESP32 Geliştirme Kartı  
- SSD1306 OLED Ekran (128x64, I2C)  
- Bağlantı kabloları  
- WiFi ağı  
- Arduino IDE ve kütüphaneler:  
  - `WiFi.h`  
  - `WebServer.h`  
  - `HTTPClient.h`  
  - `ArduinoJson.h`  
  - `Adafruit_GFX.h`  
  - `Adafruit_SSD1306.h`  

---

## 🔌 Bağlantılar

### OLED (I2C)
| OLED | ESP32 |
|------|-------|
| VCC  | 3.3V  |
| GND  | GND   |
| SCL  | GPIO 22 |
| SDA  | GPIO 21 |

### LED (Proje 3)
| LED | ESP32 |
|-----|-------|
| +   | GPIO 5 |

---

## ⚡ Çalışma Prensibi
1. ESP32 açılır ve WiFi ağına bağlanır  
2. HTTP sunucusu başlatılır ve `/mesaj` endpoint’i dinlenir  
3. Kullanıcı HTTP POST ile mesaj gönderir  
4. Mesaj kontrol edilir:  
   - 💡 "ışığı aç" → LED açılır ve OLED’de mesaj gösterilir  
   - 💡 "ışığı kapat" → LED kapanır ve OLED’de mesaj gösterilir  
   - 🤖 Diğer mesajlar → GPT modeline gönderilir, yanıt OLED ekranda harf harf gösterilir  
5. 🔤 OLED ekran Türkçe karakterleri sadeleştirerek gösterir  

---

## 💻 Kurulum ve Kullanım
```plaintext
1. Arduino IDE’ye ESP32 kart desteğini ekleyin.
2. Gerekli kütüphaneleri yükleyin:
   - Adafruit_SSD1306
   - ArduinoJson
   - WiFi
   - WebServer
   - HTTPClient
3. 'ssid' ve 'password' değişkenlerini kendi WiFi bilgilerinize göre güncelleyin.
4. OpenRouter API anahtarınızı 'openai_api_key' değişkenine girin.
5. Proje kodunu ESP32’ye yükleyin.
6. ESP32’nin IP adresini seri monitörden öğrenin.
7. Postman veya RestClient gibi bir uygulama ile '/mesaj' endpoint’ine POST isteği gönderin.
```

---

## 💡 Geliştirme Fikirleri
- 🌐 HTML tabanlı web arayüzü eklemek  
- 🎤 Sesli komutlar ile LED ve cihaz kontrolü  
- 🖥️ OLED yerine TFT ekran veya sesli çıktı eklemek  
- 🔒 API güvenliği için HTTPS desteği ve mesaj sınırlandırmaları  

---

## 📂 Kaynak Kodlar
- [Proje 1 Kodu](./Proje1/main.ino)  
- [Proje 2 Kodu](./Proje2/main.ino)  
- [Proje 3 Kodu](./Proje3/main.ino)  

---

## 📖 Kaynaklar
- [ESP32 WiFi Documentation](https://www.espressif.com/en/products/socs/esp32/resources)  
- [Adafruit SSD1306 OLED](https://github.com/adafruit/Adafruit_SSD1306)  
- [OpenRouter API](https://openrouter.ai)
