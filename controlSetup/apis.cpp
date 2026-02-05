#include "apis.h"
#include <WebServer.h>
#include "SPIFFS.h"


WebServer apiServer(80);

void handleFileRequest(String path, String contentType){
  if (!path.startsWith("/")) path = "/" + path;
  
  File file = SPIFFS.open(path, "r");
  if(!file) {
    apiServer.send(404, "text/plain", "Archivo no encontrado");
    return;
  }
  apiServer.streamFile(file, contentType);
  file.close();
}

void handleBrightness(){
  if(apiServer.hasArg("value")){
    int estado = apiServer.arg("value").toInt();
    if(estado == 1){
      digitalWrite(ledPin, HIGH);
    }else if(estado == 0){
      digitalWrite(ledPin, LOW);
    }
    /*no recomendado para RC, solo debug
    //Serial.println("estado: "+String(estado));*/
    apiServer.send(201, "text/plain", "estado actual: " + String(estado));
  } else {
    apiServer.send(400, "text/plain", "Falta el valor");
    /*no recomendado para RC, solo debug
    Serial.println("no recibido");*/
  }
}

void handleBuzzer(){
  if(apiServer.hasArg("value")){
    int estado = apiServer.arg("value").toInt();
    if(estado == 1){
      digitalWrite(buzPin, HIGH);
    }else if(estado == 0){
      digitalWrite(buzPin, LOW);
    }
    /*no recomendado para RC, solo debug
    //Serial.println("estado: "+String(estado));*/
    apiServer.send(201, "text/plain", "estado actual: " + String(estado));
  } else {
    apiServer.send(400, "text/plain", "Falta el valor");
    /*no recomendado para RC, solo debug
    Serial.println("no recibido");*/
  }
}

void handleMov(){
  if(apiServer.hasArg("value")){
    String estado = apiServer.arg("value");
    /*no recomendado para RC, solo debug
    //Serial.println("estado: "+String(estado));*/
    Serial.print(String(estado));
    apiServer.send(201, "text/plain", "estado actual: " + String(estado));
  } else {
    apiServer.send(400, "text/plain", "Falta el valor");
    /*no recomendado para RC, solo debug
    Serial.println("no recibido");*/
  }
}

void startApiServer() {
  apiServer.on("/status", [](){
    apiServer.send(200, "application/json", "{\"ok\":true,\"msg\":\"ESP32-CAM funcionando\"}");
  });

  apiServer.on("/luz", HTTP_POST, handleBrightness);
  apiServer.on("/buz", HTTP_POST, handleBuzzer);
  apiServer.on("/mov", HTTP_POST, handleMov);

  apiServer.onNotFound([]{
    String path = apiServer.uri();
    if(path.endsWith("/")) path += "index.html";

    String contentType = "text/plain";
    if(path.endsWith(".html")) contentType = "text/html";
    else if(path.endsWith(".css")) contentType = "text/css";
    else if(path.endsWith(".js")) contentType = "application/javascript";

    handleFileRequest(path, contentType);
  });

  apiServer.begin();
  Serial.println("Servidor API iniciado en puerto 80");
}

void handleApiServer() {
  apiServer.handleClient();
}
