#ifndef EEPROM_CONTROLLER_H
#define EEPROM_CONTROLLER_H

#include <Arduino.h>
#include <EEPROM.h>   

#include "Usuarios.h"
#include "Log.h"
#include "Cajas.h"
#include "cifrado.h"

const int EEPROM_SIZE = 4095;

const int LOG_COUNT_ADDRESS = 0;
const int CURRENT_LOG_ADDRESS = 1 * sizeof(int);
const int LOG_BLOCK_START_ADDRESS = 2 * sizeof(int);
const int MAX_LOG = 99;

const int BOX_COUNT_ADDRESS = MAX_LOG * sizeof(Log) + LOG_BLOCK_START_ADDRESS;
const int CURRENT_BOX_ADDRESS = 1 * sizeof(int) + BOX_COUNT_ADDRESS;
const int BOX_BLOCK_START_ADDRESS = 2 * sizeof(int) + BOX_COUNT_ADDRESS;
const int MAX_BOX = 9;

const int USER_COUNT_ADDRESS = MAX_BOX * sizeof(Cajas) + LOG_BLOCK_START_ADDRESS;
const int CURRENT_USER_ADDRESS = 1 * sizeof(int) + USER_COUNT_ADDRESS;
const int USER_BLOCK_START_ADDRESS = 2 * sizeof(int) + USER_COUNT_ADDRESS;

// Funciones de Cajas
int get_box_count();
void set_box_count(int count);
int get_current_box_address();
void write_box(Cajas box);
Cajas get_box(int index);
int get_boxes_ocuped();

//Funciones de Log
int get_log_count();
void set_log_count(int count);
int get_current_log_address();
void write_log(Log log);
Log get_log(int index);

int get_user_count();
void set_user_count(int count);
int get_current_user_address();
void write_user (Usuarios user);
Usuarios get_user(int index);
void update_user(Usuarios user);
void update_user_state(String name);
bool is_user_registered(String nombre);





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

    if(box_count >= MAX_BOX) {
        Serial.println("No hay espacio en el Sistema 9/9");
        return;
    }

    set_box_count(box_count + 1);
    box.address = get_current_box_address();
    EEPROM.put(get_current_box_address(), box);
    EEPROM.put(CURRENT_BOX_ADDRESS, box.address + sizeof(Cajas));
}

Cajas get_box(int index) {
   Cajas box;
    
    if(index < 0 || index >= get_box_count()){
        Serial.println("Index out of bounds");
        box = Cajas();
        return box;
    }

    int address = BOX_BLOCK_START_ADDRESS + (index * sizeof(Cajas));
    EEPROM.get(address, box);
    return box;
}


void update_box_state(Cajas boxs){
    EEPROM.put(boxs.address, boxs);  //PUEDEN USAR ESTA PARA ACTUALIZAR LOS DATOS DE UN USUARIO A 0000000000
}


void update_box_state(int id, bool estado, String name) {
    int box_count = get_box_count();
    
    // Buscar el compartimento con el ID correspondiente
    for (int i = 0; i < box_count; i++) {
        Cajas box = get_box(i);
        if (box.id == id) {
            // Actualizar el estado del compartimento
            box.estado = estado;
            strcpy(box.propietario, name.c_str());
            
            // Escribir los cambios en la EEPROM
            EEPROM.put(box.address, box);
            return;  // Salir del método después de actualizar el compartimento
        }
    }
    
    // El compartimento con el ID especificado no fue encontrado
    Serial.println("Compartimento no encontrado");
}


void mostrar_box(){
    for(int i = 0; i < get_box_count(); i++){
        Cajas box = get_box(i);
        Serial.print("Caja: ");
        Serial.println(box.id);
        Serial.print("Estado: ");
        Serial.println(box.estado);
        Serial.print("Propiertario: ");
        Serial.println(box.propietario);

    }
}

//      USUARIOS
int get_user_count() {
    int count;
    EEPROM.get(USER_COUNT_ADDRESS, count);
    return count;
}

void set_user_count(int count) {
    EEPROM.put(USER_COUNT_ADDRESS, count);
}

int get_current_user_address() {
    int address;
    EEPROM.get(CURRENT_USER_ADDRESS, address);
    return address;
}

void write_user(Usuarios user) {
    int user_count = get_user_count();

    if(user_count >= EEPROM_SIZE) {
        Serial.println("EEPROM FULL");
        return;
    }

   

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

void mostrar_usuarios(){
    for(int i = 0; i < get_user_count(); i++){
        Usuarios box = get_user(i);
        Serial.print("Nombre: ");
        Serial.println(box.nombre);
        Serial.print("Contrasenia: ");
        Serial.println(box.contrasenia);

    }
}

void update_user_state(String name) {
    int user_count = get_user_count();
    
    // Buscar el usuario con el nombre correspondiente
    for (int i = 0; i < user_count; i++) {
        Usuarios user = get_user(i);
        if (strcmp(user.nombre, name.c_str()) == 0) {
            // Actualizar el estado del usuario
            memset(user.nombre, 0, sizeof(user.nombre));
            memset(user.numero, 0, sizeof(user.numero));
            memset(user.contrasenia, 0, sizeof(user.contrasenia));

            // Escribir los cambios en la EEPROM
            EEPROM.put(user.address, user);
            return;  // Salir del método después de actualizar el usuario
        }
    }
    
    // El usuario con el nombre especificado no fue encontrado
    Serial.println("Usuario no encontrado");
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

    if(log_count >= MAX_LOG){
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

int get_boxes_ocuped(){
    int count = 0;
    for(int i = 0; i < get_box_count(); i++){
        Cajas box = get_box(i);
        if(box.estado){
            count++;
        }
    }
    return count;
}

void mostrar_logs(){
    for(int i = 0; i < get_log_count(); i++){
        Log box = get_log(i);
        Serial.print("ID: ");
        Serial.println(box.id);
        Serial.print("Desc: ");
        Serial.println(box.descripcion);

    }
}

void reset_eeprom(){


    EEPROM.put(USER_COUNT_ADDRESS, 0);
    EEPROM.put(CURRENT_USER_ADDRESS, USER_BLOCK_START_ADDRESS);

    EEPROM.put(LOG_COUNT_ADDRESS, 0);
    EEPROM.put(CURRENT_LOG_ADDRESS, LOG_BLOCK_START_ADDRESS);

    EEPROM.put(BOX_COUNT_ADDRESS, 0);
    EEPROM.put(CURRENT_BOX_ADDRESS, BOX_BLOCK_START_ADDRESS);

    Serial.println("SE RESETEO LA EEPROM");

    // INIT ADMIN USER

    Usuarios admin = Usuarios();
    admin.isAdmin = true;
    char nombre[] = "admin*94869";// Pruebas solamente "ADMIN*94869"
    char nombre_cifrado[sizeof(nombre)];
    strcpy(nombre_cifrado, nombre);
    dobleCifradoXOR(nombre_cifrado);

    char numero[] = "94069";
    char numero_cifrado[sizeof(numero)];
    strcpy(numero_cifrado, numero);
    dobleCifradoXOR(numero_cifrado);

    char contrasenia[] = "grupo17";// Pruebas solamente "GRUPO17"
    char contrasenia_cifrada[sizeof(contrasenia)];
    strcpy(contrasenia_cifrada, contrasenia);
    dobleCifradoXOR(contrasenia_cifrada);

    strcpy(admin.nombre, nombre_cifrado);
    strcpy(admin.numero, numero_cifrado);
    strcpy(admin.contrasenia, contrasenia_cifrada);
    //Serial.println(nombre_cifrado);
    //Serial.println(contrasenia_cifrada);
    write_user(admin);

    
    // Usuarios user1 = Usuarios();
    // user1.isAdmin = false;
    // char nombre1[] = "a";// Pruebas solamente "ADMIN*94069"
    // char nombre_cifrado1[sizeof(nombre1)];
    // strcpy(nombre_cifrado1, nombre1);
    // dobleCifradoXOR(nombre_cifrado1);

    // char numero1[] = "94069";
    // char numero_cifrado1[sizeof(numero1)];
    // strcpy(numero_cifrado1, numero1);
    // dobleCifradoXOR(numero_cifrado1);

    // char contrasenia1[] = "1";// Pruebas solamente "GRUPO17"
    // char contrasenia_cifrada1[sizeof(contrasenia1)];
    // strcpy(contrasenia_cifrada1, contrasenia1);
    // dobleCifradoXOR(contrasenia_cifrada1);

    // strcpy(user1.nombre, nombre_cifrado1);
    // strcpy(user1.numero, numero_cifrado1);
    // strcpy(user1.contrasenia, contrasenia_cifrada1);
    // Serial.println(nombre_cifrado1);
    // Serial.println(contrasenia_cifrada1);
    // write_user(user1);
    
    
    Cajas box ;

    box = Cajas();
    box.id = 0;
    box.estado = false;
    strcpy(box.propietario, "");
    write_box(box);

    box = Cajas();
    box.id = 1;
    box.estado = false;
    strcpy(box.propietario, "");
    write_box(box);

    box = Cajas();
    box.id = 2;
    box.estado = false;
    strcpy(box.propietario, "");
    write_box(box);
    
    box = Cajas();
    box.id = 3;
    box.estado = false;
    strcpy(box.propietario, "");
    write_box(box);

    box = Cajas();
    box.id = 4;
    box.estado = false;
    strcpy(box.propietario, "");
    write_box(box);

    box = Cajas();
    box.id = 5;
    box.estado = false;
    strcpy(box.propietario, "");
    write_box(box);
    
    box = Cajas();
    box.id = 6;
    box.estado = false;
    strcpy(box.propietario, "");
    write_box(box);
    
    box = Cajas();
    box.id = 7;
    box.estado = false;
    strcpy(box.propietario, "");
    write_box(box);

    box = Cajas();
    box.id = 8;
    box.estado = false;
    strcpy(box.propietario, "");
    write_box(box);


}

#endif