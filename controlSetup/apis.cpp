#include <ESP8266WebServer.h>
#include "apis.h"
#include <FS.h>
#include <ESP8266WiFi.h>

ESP8266WebServer apiServer(80);

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

void endenderLuces(){
  if(apiServer.hasArg("lampara")){
    String lampara = apiServer.arg("lampara");
    if (apiServer.hasArg("estado")){
      int estado = apiServer.arg("estado").toInt();

      if(lampara == "lamP"){
        if(estado == 1){
          digitalWrite(lamPrin, HIGH);
        }else if(estado == 0){
          digitalWrite(lamPrin, LOW);
        }
      }else if(lampara == "lamS"){
        if(estado == 1){
          digitalWrite(lamSec, HIGH);
        }else if(estado == 0){
          digitalWrite(lamSec, LOW);
        }
      }else if(lampara == "lamRGB"){
        if(estado == 1){
          analogWrite(ilumR, 1023);
          analogWrite(ilumG, 1023);
          analogWrite(ilumB, 1023);
        }else if(estado == 0){
          analogWrite(ilumR, 0);
          analogWrite(ilumR, 0);
          analogWrite(ilumR, 0);
        }
      }
      Serial.println("lampara"+String(lampara)+" - "+"estado: "+String(estado));
      apiServer.send(201, "text/plain", "Lampara"+String(lampara)+" estado actual: " + String(estado));
    }else{
      apiServer.send(400, "text/plain", "Falta el valor: estado");
      Serial.println("no recibido: estado");
    }
  } else {
    apiServer.send(400, "text/plain", "Falta el valor");
    Serial.println("no recibido");
  }
}

void handleRGB(){
  if(apiServer.hasArg("r") && apiServer.hasArg("g") && apiServer.hasArg("b")){
    int r = apiServer.arg("r").toInt();
    int g = apiServer.arg("g").toInt();
    int b = apiServer.arg("b").toInt();
    
    Serial.println("r: "+String(r)+" g: "+String(g)+" b: "+String(b));

    String msg = String("estado actual: r: ") + String(r) + " g: " + String(g) + " b: " + String(b);
    apiServer.send(201, "text/plain", msg);

  } else {
    apiServer.send(400, "text/plain", "Falta el valor");
    Serial.println("RGB no recibido");
  }
}

void startApiServer() {
  apiServer.on("/status", [](){
    apiServer.send(200, "application/json", "{\"ok\":true,\"msg\":\"ESP8266 funcionando\"}");
  });

  apiServer.on("/encendido", HTTP_POST, endenderLuces);
  apiServer.on("/RGB", HTTP_POST, handleRGB);

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
