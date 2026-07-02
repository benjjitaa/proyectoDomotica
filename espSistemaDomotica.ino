#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "benjjitaa";
const char* password = "nicoweko";

String serverName = "http://10.195.36.86:5000/api/estado"; 

const int PIN_SALA = 25;
const int PIN_PATIO = 26;
const int PIN_CLIMA =27;

unsigned long ultimoTiempo = 0;
const long intervalo = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_SALA, OUTPUT);
  pinMode(PIN_PATIO, OUTPUT);
  pinMode(PIN_CLIMA, OUTPUT);

  digitalWrite(PIN_SALA, LOW);
  digitalWrite(PIN_PATIO, LOW);
  digitalWrite(PIN_CLIMA, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a la red WiFi");
  Serial.print("IP del ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if ((millis() - ultimoTiempo > intervalo) && (WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    http.begin(serverName);
    
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.print("Estado recibido: ");
      Serial.println(payload);

      int estadoSala, estadoPatio, estadoClima;
      if (sscanf(payload.c_str(), "%d,%d,%d", &estadoSala, &estadoPatio, &estadoClima) == 3) {

        digitalWrite(PIN_SALA, estadoSala);
        digitalWrite(PIN_PATIO, estadoPatio);
        digitalWrite(PIN_CLIMA, estadoClima);
      }
    } else {
      Serial.print("Error conectando al servidor Flask. Código: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
    ultimoTiempo = millis();
  }
}
