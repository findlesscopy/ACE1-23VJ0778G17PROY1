#ifndef MATRIZ_H
#define MATRIZ_H

#include <Arduino.h>

void print_number(LedControl matriz, int number);


//  CARACTERES   [A-Z0-9*#$!]
const bool caracteres [11][5][3] {
    {
        {1,1,1},
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {1,1,1},
    },
    {
        {0,0,1},
        {0,0,1},
        {0,0,1},
        {0,0,1},
        {0,0,1},
    },
    {
        {1,1,1},
        {0,0,1},
        {1,1,1},
        {1,0,0},
        {1,1,1},
    },
    {
        {1,1,1},
        {0,0,1},
        {1,1,1},
        {0,0,1},
        {1,1,1},
    },
    {
        {1,0,1},
        {1,0,1},
        {1,1,1},
        {0,0,1},
        {0,0,1},
    },
    {
        {1,1,1},
        {1,0,0},
        {1,1,1},
        {0,0,1},
        {1,1,1},
    },
    {
        {1,1,1},
        {1,0,0},
        {1,1,1},
        {1,0,1},
        {1,1,1},
    },
    {
        {1,1,1},
        {0,0,1},
        {0,0,1},
        {0,0,1},
        {0,0,1},
    },
    {
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {1,0,1},
        {1,1,1},
    },
    {
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {0,0,1},
        {0,0,1},
    },
    {
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {1,0,1},
        {1,0,1},
    }
};

void matriz_imprime_caracteres(LedControl matriz, int numero){

    // Representa el caracter en la matriz
     bool buffer [8][8] {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };

    // Estas líneas descomponen el número en unidades y decenas.
    int unidades = numero % 11;
    int decenas = numero / 11;

     for (int i = 0; i < 5; i++){
        for (int j = 0; j < 3; j++){
            buffer[i+2][j+1] = caracteres[decenas][i][j];
            buffer[i+2][j+5] = caracteres[unidades][i][j];
        }
    }

    // imprimir  la matriz
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            matriz.setLed(0, i, j, buffer[i][j]);
        }
    }
}

void limpiar_matriz(LedControl matriz){
    // settear la matriz con 0´s
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            matriz.setLed(0, i, j, 0);
        }
    }
}

#endif