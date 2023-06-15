#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <LedControl.h>
#include <Keypad.h>

#include "button.h"
#include "matriz.h"

#include "letras.h"

//  MENUS
const int SECUENCIA_INICIAL = 0, MENU_PRINCIPAL = 1, LOGIN = 2, REGISTER = 3;
int menu_actual = SECUENCIA_INICIAL;

//  PINES
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int din = 10, cs = 9, clk = 8;
LedControl matriz(din, clk, cs, 1);

// BOTONES
Button Btn_Ok(23);
Button Btn_Cancel(24);

// ------------------ Keypad ------------------ //
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'X', '0', '#'}};

byte rowPins[ROWS] = {26, 27, 28, 29};
byte colPins[COLS] = {30, 31, 32};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// FUNCIONES
void menu_setup();
void menu_loop();

// *MENUS
void secuencia_inicial();
void menu_principal();
void login();
void registro();

void letras_matriz();
void menu_administrar();

void menu_setup()
{
    lcd.begin(16, 4); // Inicializa una pantalla LCD con 16 columnas y 4 filas.

    matriz.shutdown(0, false); // Desactiva el modo de apagado de una matriz de LED en el índice 0.
    matriz.setIntensity(0, 8); // Configura la intensidad de la matriz de LED en el índice 0 a 8.
    matriz.clearDisplay(0);    // Borra la pantalla de la matriz de LED en el índice 0.

    Btn_Ok.setup();     // Configura un botón llamado "ok_button".
    Btn_Cancel.setup(); // Configura un botón llamado "cancel_button".
}

void menu_loop()
{
    switch (menu_actual)
    {
    case SECUENCIA_INICIAL:
        secuencia_inicial();
        break;

    case MENU_PRINCIPAL:
        menu_principal();
        break;

    case LOGIN:
        login();
        break;

    case REGISTER:
        registro();
        break;
        // TODO:
        break;

    
    default:
        break;
    }
}

void secuencia_inicial()
{
    lcd.clear(); // Borra la pantalla LCD.

    lcd.setCursor(0, 0);             // Establece el cursor en la posición (1, 0) de la pantalla LCD.
    lcd.print("Bienvenido");         // Imprime el mensaje "Bienvenido" en la pantalla LCD.

    // Imprime diferentes nombres en la pantalla LCD.
    delay(300);          // Pausa de 0.3 segundos
    lcd.setCursor(0, 1); // Establece el cursor en la posición (0, 1) de la pantalla LCD.
    lcd.print("Roberto202000544");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 2);
    lcd.print("Edgardo201801119");

    delay(300); // Pausa de 0.9 segundos
    lcd.setCursor(0, 3);
    lcd.print("Cesar202010816");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 4);
    lcd.print("Jose202001800");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 5);
    lcd.print("Angel202012039");

    while (true)
    {
        if (Btn_Ok.is_pressed())
        {
            menu_actual = MENU_PRINCIPAL;
            break;
        }
    }
}

void menu_principal()
{
    lcd.clear(); // Borra la pantalla LCD.

    lcd.setCursor(0, 0);         // Establece el cursor en la posición (1, 0) de la pantalla LCD.
    lcd.print("Menu Principal"); // Imprime el mensaje "Bienvenido" en la pantalla LCD.

    // Imprime diferentes nombres en la pantalla LCD.
    delay(300);          // Pausa de 0.3 segundos
    lcd.setCursor(0, 1); // Establece el cursor en la posición (0, 1) de la pantalla LCD.
    lcd.print("1. Login");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 2);
    lcd.print("2. Registro");

    delay(300); // Pausa de 0.3 segundos
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Opcion:");
    String opcion = "";

    while (true)
    {
        char key = keypad.getKey();
        if (key != NO_KEY)
        {

            if (opcion.length() < 2)
            {
                opcion += key;
                lcd.setCursor(0, 1);
                lcd.print(opcion);
            }
            else
            {
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print("MAX");
                delay(1000);
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print(opcion);
            }
        }
        if (Btn_Ok.is_pressed())
        {
            int num;
            if (opcion == "1")
                num = 1;
            else if (opcion == "2")
                num = 2;
            else if (opcion == "0")
                num = 0;
            else 
                break;

            switch (num)
            {

            case 0:
                letras_matriz();
                break;
            case 1:
                login();
                break;

            case 2:
                menu_principal();
                break;
            default:
                break;
            }
        }

        if (Btn_Cancel.is_pressed())
        {
            if (opcion.length() > 0)
            {
                opcion.remove(opcion.length() - 1);
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print(opcion);
            }
        }
    }
}

void letras_matriz()
{
    lcd.clear();
    bool exit = false;
    int letra_actual_index = 0;  // Índice de la letra actual en el arreglo letras[]
    String cadena = "";

    while (!exit)
    {
        // Obtener la letra actual
        Letras letra_actual = letras[letra_actual_index];

        // Imprimir la letra actual
        matriz_imprime_caracteres(matriz, letra_actual_index);

        // Esperar a que se presione el botón para cambiar la letra o imprimir la letra actual
        char key = keypad.getKey();
        if (key != NO_KEY)
        {
            switch (key)
            {
            case 'X':
                letra_actual_index--;
                if (letra_actual_index < 0)
                    letra_actual_index = sizeof(letras) / sizeof(letras[0]) - 1;
                break;

            case '#':
                letra_actual_index++;
                if (letra_actual_index >= sizeof(letras) / sizeof(letras[0]))
                    letra_actual_index = 0;
                break;

            case '0':
                cadena += letra_actual.name;
                lcd.clear();  // Limpiar el LCD
                lcd.setCursor(0, 0);
                lcd.print(cadena);
                //delay(1000);  // Esperar un segundo para que se pueda leer la letra en el LCD
                break;

            case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                cadena += key;
                lcd.clear();  // Limpiar el LCD
                lcd.setCursor(0, 0);
                lcd.print(cadena);
                break;

            default:
                break;
            }

            // Esperar un breve tiempo para evitar cambios rápidos de letra
            delay(50);
        }

        // Verificar si se alcanzó el final del arreglo letras[]
        if (letra_actual_index >= sizeof(letras) / sizeof(letras[0]))
        {
            exit = true;  // Salir del bucle si se alcanzó el final
        }
    }

    limpiar_matriz(matriz);
}




void login()
{
    menu_administrar();
}

void registro()
{
    //TODO: 
}

void menu_administrar()
{
    lcd.clear(); // Borra la pantalla LCD.

    lcd.setCursor(0, 0);         // Establece el cursor en la posición (1, 0) de la pantalla LCD.
    lcd.print("Menu ADMIN"); // Imprime el mensaje "Bienvenido" en la pantalla LCD.

    // Imprime diferentes nombres en la pantalla LCD.
    delay(300);  
    lcd.clear(); // Borra la pantalla LCD.        // Pausa de 0.3 segundos
    lcd.setCursor(0, 0); // Establece el cursor en la posición (0, 1) de la pantalla LCD.
    lcd.print("1. display de");
    lcd.setCursor(0, 1); // Establece el cursor en la posición (0, 1) de la pantalla LCD.
    lcd.print("logs");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 2);
    lcd.print("2. check del");
    lcd.setCursor(0, 3);
    lcd.print("system status");

    delay(300); // Pausa de 0.3 segundos
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Opcion:");
    String opcion = "";

    while (true)
    {
        char key = keypad.getKey();
        if (key != NO_KEY)
        {

            if (opcion.length() < 2)
            {
                opcion += key;
                lcd.setCursor(0, 1);
                lcd.print(opcion);
            }
            else
            {
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print("MAX");
                delay(1000);
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print(opcion);
            }
        }
        if (Btn_Ok.is_pressed())
        {
            int num;
            if (opcion == "1")
                num = 1;
            else if (opcion == "2")
                num = 2;
            else 
                break;

            switch (num)
            {
            case 1:
                secuencia_inicial();
                break;

            case 2:
                menu_principal();
                break;
            default:
                break;
            }
        }

        if (Btn_Cancel.is_pressed())
        {
            if (opcion.length() > 0)
            {
                opcion.remove(opcion.length() - 1);
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print(opcion);
            }
        }
    }
}
#endif