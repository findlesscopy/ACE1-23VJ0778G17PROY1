/*
    Código fuente - Práctica 1 - Arquitectura de Computadores y Ensambladores 1
    Grupo:  17
    Integrantes:
        - Roberto Carlos Gómez Donis 202000544
        - Edgardo Andrés Nil Guzmán 201801119
        - César André Ramírez Dávila 202010816
        - José Manuel Ibarra Pirir 202001800
        - Ángel Francisco Sique Santos 202012039
*/

#include "main.h"

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
    // reset_eeprom(); // ! Just for the first time
    menu_setup();
}

void loop() {
    menu_loop();
}