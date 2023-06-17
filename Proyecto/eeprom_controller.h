#ifndef EEPROM_CONTROLLER_H
#define EEPROM_CONTROLLER_H

#include <Arduino.h>
#include <EEPROM.h>

#include "Usuarios.h"
#include "Log.h"
#include "Cajas.h"
#include "cifrado.h"

const int EEPROM_SIZE = 4095;

const int BOX_COUNT_ADDRESS = 0;
const int CURRENT_BOX_ADDRESS = 1 * sizeof(int);
const int BOX_BLOCK_START_ADDRESS = 2 * sizeof(int);
const int MAX_BOX = 9;

const int LOG_COUNT_ADDRESS = MAX_BOX * sizeof(Cajas) + BOX_BLOCK_START_ADDRESS;
const int CURRENT_LOG_ADDRESS = 1 * sizeof(int) + LOG_COUNT_ADDRESS;
const int LOG_BLOCK_START_ADDRESS = 2 * sizeof(int) + LOG_COUNT_ADDRESS;
const int MAX_LOG = 99;

const int USER_COUNT_ADDRES = MAX_LOG * sizeof(Log) + LOG_BLOCK_START_ADDRESS;
const int CURRENT_USER_ADDRESS = 1 * sizeof(int) + USER_COUNT_ADDRES;
const int USER_BLOCK_START_ADDRESS = 2 * sizeof(int) + USER_COUNT_ADDRES;

int get_user_count();
void set_user_count(int count);
int get_current_user_address();
void write_user (Usuarios user);
Usuarios get_user(int index);
void update_user(Usuarios user);
bool is_user_registered(String nombre);



//      USUARIOS
int get_user_count() {
    int count;
    EEPROM.get(USER_COUNT_ADDRES, count);
    return count;
}

void set_user_count(int count) {
    EEPROM.put(USER_COUNT_ADDRES, count);
}

int get_current_user_address() {
    int address;
    EEPROM.get(CURRENT_USER_ADDRESS, address);
    return address;
}

void write_user(Usuarios user) {
    if(get_current_user_address() >= EEPROM_SIZE) {
        Serial.println("EEPROM FULL");
        return;
    }

    int user_count = get_user_count();

    set_user_count(user_count + 1);
    user.address = get_current_user_address();
    EEPROM.put(get_current_user_address(), user);
    EEPROM.put(CURRENT_USER_ADDRESS, user.address + sizeof(Usuarios));
}

Usuarios get_user(int index) {
    Usuarios user;
    
    if(index < 0 || index >= get_user_count()){
        Serial.println("Index out of bounds");
        user = Usuarios();
        return user;
    }

    int address = USER_BLOCK_START_ADDRESS + (index * sizeof(Usuarios));
    EEPROM.get(address, user);
    return user;
}

void update_user(Usuarios user){
    EEPROM.put(user.address, user);
}

bool is_user_registered(String nombre){
    for(int i = 0; i < get_user_count(); i++){
        Usuarios user = get_user(i);
        if(strcmp(user.nombre, nombre.c_str()) == 0){
            return true;
        }
    }
    return false;
}

void reset_eeprom(){
    EEPROM.put(USER_COUNT_ADDRES, 0);
    EEPROM.put(CURRENT_USER_ADDRESS, USER_BLOCK_START_ADDRESS);

    EEPROM.put(LOG_COUNT_ADDRESS, 0);
    EEPROM.put(CURRENT_LOG_ADDRESS, LOG_BLOCK_START_ADDRESS);

    Serial.println("EEPROM RESETED");

    // INIT ADMIN USER

    Usuarios admin = Usuarios();
    admin.isAdmin = true;

    char nombre[] = "a";// Pruebas solamente "ADMIN*94069"
    char nombre_cifrado[sizeof(nombre)];
    strcpy(nombre_cifrado, nombre);
    dobleCifradoXOR(nombre_cifrado);

    char numero[] = "94069";
    char numero_cifrado[sizeof(numero)];
    strcpy(numero_cifrado, numero);
    dobleCifradoXOR(numero_cifrado);


    char contrasenia[] = "3";// Pruebas solamente "GRUPO17"
    char contrasenia_cifrada[sizeof(contrasenia)];
    strcpy(contrasenia_cifrada, contrasenia);
    dobleCifradoXOR(contrasenia_cifrada);

    strcpy(admin.nombre, nombre_cifrado);
    strcpy(admin.numero, numero_cifrado);
    strcpy(admin.contrasenia, contrasenia_cifrada);
    Serial.println(nombre_cifrado);
    Serial.println(contrasenia_cifrada);
    write_user(admin);





}

#endif