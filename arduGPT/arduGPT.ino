#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi101.h>
#include <ArduinoBearSSL.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char ssid[] = ""; // tu SSID de red (nombre)
char pass[] = ""; // tu contraseña de red (usa para WPA, o como clave para WEP)
int keyIndex = 0; // tu número de índice de clave de red (solo necesario para WEP)
char server[] = "api.openai.com"; // dirección del servidor OpenAI API (usando DNS)


WiFiClient client;
BearSSLClient sslClient(client);

unsigned long getTime() {
  return WiFi.getTime();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display(); // Mostrar el logotipo de Adafruit (opcional)
  delay(2000); // Esperar 2 segundos
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("HEY, I am : )");
  display.setCursor(16, 16);
  display.println("arduGPT");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(1000);
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Accessing Wi-Fi...");
    display.display();
    Serial.println("Accessing Wi-Fi...");
  }
  delay(1000);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Connected to Wi-Fi:");
  display.setCursor(16,16);
  display.println(ssid);
  display.display();
  Serial.println("Connected to Wi-Fi:");
  Serial.println(ssid);
  delay(10000);

  ArduinoBearSSL.onGetTime(getTime);


display.clearDisplay(); // limpia la pantalla
  display.setCursor(0, 0); // establece el cursor para escribir en la pantalla
  display.println("I have"); // escribe en la pantalla OLED
  display.setCursor(16, 16);
  display.println("super powers!!!");
  display.display(); // actualiza la pantalla
  delay(3000);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Give me an");
  display.setCursor(16, 16);
  display.println("phrase");
  display.display();
  delay(3000);

  if (sslClient.connect("api.openai.com", 443)) {
    Serial.println("Connected to the Server: api.openai.com");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Connected to the Server");
    display.setCursor(16,16);
    display.println("api.openai.com");
    display.display();
    Serial.println("Connected to the Server");
    Serial.println("api.openai.com");
    delay(1000);

    // Send HTTP POST request and change the prompt to your own
    String requestBody = "{\"model\":\"text-davinci-003\",\"prompt\":\"make a quote\",\"max_tokens\":150}";
    String request = "POST /v1/completions HTTP/1.1\r\n";
    request += "Host: api.openai.com\r\n";
    // Replace with your own API Key Here
    request += "Authorization: Bearer apiKey\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + String(requestBody.length()) + "\r\n";
    request += "Connection: close\r\n\r\n";
    request += requestBody;

    sslClient.print(request);

     // Read response
    while (sslClient.connected()) {
      String line = sslClient.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }


    String response = sslClient.readString();
    Serial.println("Response:");
    Serial.println(response);

     // Parse JSON
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.println("Failed to parse JSON");
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("ChatGPT Failed to parse JSON:");
      display.display();
    } else {
      String completionText = doc["choices"][0]["text"];
      Serial.println("ChatGPT response: "+ completionText);
      // Change the size here to whatever you want (1-4) 2 is lower
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("ChatGPT Response" + completionText);
      display.display();
      }
  } else {
    Serial.println("Connection failed");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("ChatGPT Connection failed");
    display.display();
  }
}

void loop() {
  // Do nothing
}







