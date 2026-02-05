#include <ESP8266WiFi.h>
#include <FS.h>
#include "apis.h"


// Configuración WiFi
const char* ssid = "TP-LINK_A217";
const char* password = "z9yv7tE44v";

const int lamPrin = 16;
const int lamSec = 5;
const int ilumR = 4;
const int ilumG = 0;
const int ilumB = 2;

void statusSistem(){
  Serial.println("Probando conecciones");
  delay(500);
  digitalWrite(lamPrin, HIGH);
  delay(500);
  digitalWrite(lamPrin, LOW);
  delay(500);
  digitalWrite(lamSec, HIGH);
  delay(500);
  digitalWrite(lamSec, LOW);
  delay(500);
  analogWrite(ilumR, 1023);
  delay(500);
  analogWrite(ilumR, 0);
  delay(500);
  analogWrite(ilumG, 1023);
  delay(500);
  analogWrite(ilumG, 0);
  delay(500);
  analogWrite(ilumB, 1023);
  delay(500);
  analogWrite(ilumB, 0);
}

void setup() {
  Serial.begin(115200);

  //inicializacion de pines
  pinMode(lamPrin, OUTPUT);
  digitalWrite(lamPrin, LOW);

  pinMode(lamSec, OUTPUT);
  digitalWrite(lamSec, LOW);

  pinMode(ilumR, OUTPUT);
  analogWrite(ilumR, 0);

  pinMode(ilumG, OUTPUT);
  analogWrite(ilumG, 0);

  pinMode(ilumB, OUTPUT);
  analogWrite(ilumB, 0);

  statusSistem();

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

  //inicializacion de SPIFFS
  if (!SPIFFS.begin()){   // 'true' fuerza formateo si está corrupto
    Serial.println("error al montar SPIFFS");
    return;
  }else{
    Dir dir = SPIFFS.openDir("/"); 
    Serial.print("Archivo en SPIFFS: "); 
    while (dir.next()) {
      Serial.println(dir.fileName()); 
    }
    Serial.println("montado: SPIFFS");
  }
  
  // Servidor de APIs (puerto 80)
  startApiServer();
}

void loop() {
  // El WebServer clásico necesita manejar clientes en loop
  handleApiServer();
}
