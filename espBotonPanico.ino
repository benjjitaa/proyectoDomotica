#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "benjjitaa";
const char* password = "nicoweko";

String serverName = "http://10.195.36.86:5000/api/alerta"; 

const int PIN_BOTON = 0; 
int estadoBoton = 0;
int ultimoEstado = HIGH;

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_BOTON, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi (ESP32 Sensor)");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a la red WiFi");
}

void loop() {

  estadoBoton = digitalRead(PIN_BOTON);
  
  if (estadoBoton == LOW && ultimoEstado == HIGH) {
    Serial.println("¡Botón presionado! Enviando alerta a Flask...");
    
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode > 0) {
        Serial.print("Alerta enviada correctamente. Código: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Error enviando alerta. Código: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    
    delay(1000); 
  }
  
  ultimoEstado = estadoBoton;
  delay(50);
}
