#include <FS.h>
#include <WiFi.h>
#include "apis.h"

// Configuración WiFi
const char* ssid = "TP-LINK_A217";
const char* password = "z9yv7tE44v";

const int lamPrin = 16;
const int lapSec = 5;
const int ilumR = 4;
const int ilumG = 0;
const int ilumB = 2;

void setup() {
  Serial.begin(9600);

  //inicializacion de pines
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(buzPin, OUTPUT);
  digitalWrite(buzPin, LOW);

  pinMode(flashPin, OUTPUT);
  digitalWrite(flashPin, LOW);

  pinMode(indiPin, OUTPUT);
  digitalWrite(indiPin, HIGH);
  
  delay(200);
  digitalWrite(indiPin, LOW);
  delay(500);
  digitalWrite(indiPin, HIGH);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite(indiPin, LOW);
  delay(500);
  digitalWrite(indiPin, HIGH);

  //inicializacion de SPIFFS
  if (!SPIFFS.begin()){   // 'true' fuerza formateo si está corrupto
    Serial.println("error al montar SPIFFS");
    return;
  }else{
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while(file){
      Serial.print("Archivo en SPIFFS: ");
      Serial.println(file.name());
      file = root.openNextFile();
    }
    Serial.println("montado: SPIFFS");
  }

  // Servidor de APIs (puerto 80)
  startApiServer();

  digitalWrite(flashPin, HIGH);
  delay(500);
  digitalWrite(flashPin, LOW);
}

void loop() {
  // El WebServer clásico necesita manejar clientes en loop
  handleApiServer();
}
