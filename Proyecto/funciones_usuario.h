#ifndef FUNCIONES_USUARIO_H
#define FUNCIONES_USUARIO_H

#include <Arduino.h>
#include "Usuarios.h"
#include "eeprom_controller.h"


Usuarios login_user(String nickname, String password);

Usuarios login_user(String nombre, String password){

    for(int i = 0; i < get_user_count(); i++){
        Usuarios user = get_user(i);
        if(user.validate_credentials(nombre, password)){
            return user;
        }
    }

    return Usuarios();
}


#endif