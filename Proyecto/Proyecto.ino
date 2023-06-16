/*
    Código fuente - Práctica 1 - Arquitectura de Computadores y Ensambladores 1
    Grupo:  17
    Integrantes:
        - Edgardo Andrés Nil Guzmán 201801119
        - Roberto Carlos Gómez Donis 202000544
        - José Manuel Ibarra Pirir 202001800
        - César André Ramírez Dávila 202010816
        - Ángel Francisco Sique Santos 202012039
*/

#include "main.h"
#include "eeprom_controller.h"
#include "cifrado.h"

void setup() {
    /*Serial.begin(9600);
    Serial1.begin(9600);
    reset_eeprom(); // ! Just for the first time
    menu_setup();*/
    Serial.begin(9600);

  char mensajeOriginal[] = "49507431";  // Mensaje original

  // Cifrado
  char mensajeCifrado[sizeof(mensajeOriginal)];
  strcpy(mensajeCifrado, mensajeOriginal);
  dobleCifradoXOR(mensajeCifrado);

  Serial.print("Mensaje cifrado: ");
  Serial.println(mensajeCifrado);

  // Descifrado
  char mensajeDescifrado[sizeof(mensajeCifrado)];
  strcpy(mensajeDescifrado, mensajeCifrado);
  dobleDescifradoXOR(mensajeDescifrado);

  Serial.print("Mensaje descifrado: ");
  Serial.println(mensajeDescifrado);
}

void loop() {
    menu_loop();
}