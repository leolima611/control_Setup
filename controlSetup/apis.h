#ifndef API_H
#define API_H

#include <Arduino.h> // para funciones Arduino pines etc.

extern const int ledPin; // declaración del ino
extern const int buzPin;

void handleFileRequest(String path, String contentType);
void handleBrightness();
void handleBuzzer();
void handleMov();
void startApiServer();
void handleApiServer();

#endif
