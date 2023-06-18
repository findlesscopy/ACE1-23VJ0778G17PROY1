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

//Funciones de Usuarios
int get_user_count();
void set_user_count(int count);
int get_current_user_address();
void write_user (Usuarios user);
Usuarios get_user(int index);
void update_user(Usuarios user);
bool is_user_registered(String nombre);

//Funciones de Log
int get_log_count();
void set_log_count(int count);
int get_current_log_address();
void write_log(Log log);
Log get_log(int index);

// Funciones de Cajas
int get_box_count();
void set_box_count(int count);
int get_current_box_address();
void write_box(Cajas box);
Cajas get_box(int index);

//      CAJAS
int get_box_count() {
    int count;
    EEPROM.get(BOX_COUNT_ADDRESS, count);
    return count;
}

void set_box_count(int count) {
    EEPROM.put(BOX_COUNT_ADDRESS, count);
}

int get_current_box_address() {
    int address;
    EEPROM.get(CURRENT_BOX_ADDRESS, address);
    return address;
}

void write_box(Cajas box) {

    int box_count = get_box_count();

    if(get_current_box_address() >= EEPROM_SIZE) {
        Serial.println("No hay espacio en el Sistema 9/9");
        return;
    }

    set_box_count(box_count + 1);
    box.address = get_current_box_address();
    EEPROM.put(get_current_box_address(), box);
    EEPROM.put(CURRENT_BOX_ADDRESS, box.address + sizeof(Cajas));
}

Cajas get_box(int index) {
    // TODO : No se que parametros van a necesitar
}


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
    EEPROM.put(user.address, user);  //PUEDEN USAR ESTA PARA ACTUALIZAR LOS DATOS DE UN USUARIO A 0000000000
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

// LOGS

int get_log_count(){
    int count;
    EEPROM.get(LOG_COUNT_ADDRESS, count);
    return count;
}

void set_log_count(int count){
    EEPROM.put(LOG_COUNT_ADDRESS, count);
}

int get_current_log_address(){
    int address;
    EEPROM.get(CURRENT_LOG_ADDRESS, address);
    return address;
}

void write_log(Log log){
    int log_count = get_log_count();

    if(get_current_log_address() >= EEPROM_SIZE){
        Serial.println("EEPROM FULL");
        return;
    }

    set_log_count(log_count + 1);
    log.address = get_current_log_address();
    EEPROM.put(get_current_log_address(), log);
    EEPROM.put(CURRENT_LOG_ADDRESS, log.address + sizeof(Log));
}

Log get_log(int index){
    Log log;
    
    if(index < 0 || index >= get_log_count()){
        Serial.println("Index out of bounds");
        log = Log();
        return log;
    }

    int address = LOG_BLOCK_START_ADDRESS + (index * sizeof(Log));
    EEPROM.get(address, log);
    return log;
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

    Usuarios user1 = Usuarios();
    user1.isAdmin = false;
    char nombre1[] = "b";// Pruebas solamente "ADMIN*94069"
    char nombre_cifrado1[sizeof(nombre1)];
    strcpy(nombre_cifrado1, nombre1);
    dobleCifradoXOR(nombre_cifrado1);

    char numero1[] = "94069";
    char numero_cifrado1[sizeof(numero1)];
    strcpy(numero_cifrado1, numero1);
    dobleCifradoXOR(numero_cifrado1);

    char contrasenia1[] = "2";// Pruebas solamente "GRUPO17"
    char contrasenia_cifrada1[sizeof(contrasenia1)];
    strcpy(contrasenia_cifrada1, contrasenia1);
    dobleCifradoXOR(contrasenia_cifrada1);

    strcpy(user1.nombre, nombre_cifrado1);
    strcpy(user1.numero, numero_cifrado1);
    strcpy(user1.contrasenia, contrasenia_cifrada1);
    Serial.println(nombre_cifrado1);
    Serial.println(contrasenia_cifrada1);
    write_user(user1);
}

#endif