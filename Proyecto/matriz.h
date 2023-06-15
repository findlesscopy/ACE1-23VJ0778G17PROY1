#ifndef MATRIZ_H
#define MATRIZ_H

#include <Arduino.h>

void print_number(LedControl matriz, int number);


//  CARACTERES   [A-Z0-9*#$!]
const bool caracteres [31][5][3] {
    {
        //A
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {1,0,1},
        {1,0,1},
    },
    {
        //B
        {1,1,1},
        {1,0,1},
        {1,1,0},
        {1,0,1},
        {1,1,1},
    },
    {
        //C
        {1,1,1},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,1,1},
    },
    {
        //D
        {1,1,0},
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {1,1,0},
    },
    {
        //E
        {1,1,1},
        {1,0,0},
        {1,1,1},
        {1,0,0},
        {1,1,1},
    },
    {
        //F
        {1,1,1},
        {1,0,0},
        {1,1,1},
        {1,0,0},
        {1,0,0},
    },
    {
        //G
        {0,1,1},
        {1,0,0},
        {1,0,1},
        {1,0,1},
        {0,1,1},
    },
    {
        //H
        {1,0,1},
        {1,0,1},
        {1,1,1},
        {1,0,1},
        {1,0,1},
    },
    {
        //I
        {1,1,1},
        {0,1,0},
        {0,1,0},
        {0,1,0},
        {1,1,1},
    },
    {
        //J
        {1,1,1},
        {0,1,0},
        {0,1,0},
        {0,1,0},
        {1,1,0},
    },
    {
        //K
        {1,0,1},
        {1,0,1},
        {1,1,0},
        {1,0,1},
        {1,0,1},
    },
    {
        //L
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,1,1},
    },
    {
        //M
        {1,0,1},
        {1,1,1},
        {1,0,1},
        {1,0,1},
        {1,0,1},
    },
    {
        //N
        {0,0,},
        {0,0,0},
        {1,1,1},
        {1,0,1},
        {1,0,1},
    },
    {
        //Ñ
        {1,1,1},
        {0,0,0},
        {1,1,1},
        {1,0,1},
        {1,0,1},
    },
    {
        //O
        {0,1,0},
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {0,1,0},
    },
    {
        //P
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {1,0,0},
        {1,0,0},
    },
    {
        //Q
        {1,0,1},
        {1,0,1},
        {1,1,1},
        {1,0,1},
        {1,0,1},
    },
    {
        //R
        {1,1,1},
        {1,0,1},
        {1,1,0},
        {1,0,1},
        {1,0,1},
    },
    {
        //S
        {0,1,1},
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {1,1,0},
    },
    {
        //T
        {1,1,1},
        {0,1,0},
        {0,1,0},
        {0,1,0},
        {0,1,0},
    },
    {
        //U
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {1,1,1},
    },
    {
        //V
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {0,1,0},
    },
    {
        //W
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {1,1,1},
        {1,0,1},
    },
    {
        //X
        {1,0,1},
        {0,1,0},
        {0,1,0},
        {0,1,0},
        {1,0,1},
    },
    {
        //Y
        {1,0,1},
        {1,0,1},
        {0,1,0},
        {0,1,0},
        {0,1,0},
    },
    {
        //Z
        {1,1,1},
        {0,0,1},
        {0,1,0},
        {1,0,0},
        {1,1,1},
    },
    {
        //*
        {0,1,0},
        {1,1,1},
        {0,1,0},
        {1,1,1},
        {0,1,0},
    },
    {
        //#
        {1,0,1},
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {1,0,1},
    },
    {
        //$
        {0,1,0},
        {1,1,1},
        {0,1,1},
        {1,1,0},
        {0,1,0}
    },
    {
        //!
        {0,1,0},
        {0,1,0},
        {0,1,0},
        {0,0,0},
        {0,1,0},
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
    int unidades = numero % 32;
    int decenas = numero / 32;

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