#ifndef USUARIOS_H
#define USUARIOS_H

struct Usuarios {
    char nombre[12];
    char numero[8];
    char contrasenia[12];
    unsigned int address;
    bool isAdmin;

    bool validate_credentials(String nombre, String contrasenia) {
        return (strcmp(this->nombre, nombre.c_str()) == 0) && (strcmp(this->contrasenia, contrasenia.c_str()) == 0);
    }

    bool is_valid() {
        return strcmp(this->nombre, "") != 0;
    }
};

#endif