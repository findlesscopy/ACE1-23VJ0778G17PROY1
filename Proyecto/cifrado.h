#ifndef CIFRADO_H
#define CIFRADO_H

#include <Arduino.h>

void dobleCifradoXOR(char* mensaje);
void dobleDescifradoXOR(char mensaje[]);

const int clave1 = 9;
const int clave2 = 1;

void dobleCifradoXOR(char* mensaje) {
    int longitud = strlen(mensaje);

  
  for (int i = 0; i < longitud; i++) {
    mensaje[i] = mensaje[i] ^ clave1;  // Primer cifrado con la primera clave
    mensaje[i] = mensaje[i] ^ clave2;  // Segundo cifrado con la segunda clave
  }
}

void dobleDescifradoXOR(char mensaje[]) {
  int longitud = strlen(mensaje);
  
  for (int i = 0; i < longitud; i++) {
    mensaje[i] = mensaje[i] ^ clave2;  // Descifrado con la segunda clave
    mensaje[i] = mensaje[i] ^ clave1;  // Descifrado con la primera clave
  }
}

#endif