#ifndef API_H
#define API_H

#include <Arduino.h> // para funciones Arduino pines etc.

extern const int lamPrin;
extern const int lamSec;
extern const int ilumR;
extern const int ilumG;
extern const int ilumB;

void handleFileRequest(String path, String contentType);
void endenderLuces();
void handleRGB();
void startApiServer();
void handleApiServer();

#endif
