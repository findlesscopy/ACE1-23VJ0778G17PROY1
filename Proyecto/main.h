#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <LedControl.h>
#include <Keypad.h>

#include "button.h"
#include "matriz.h"

#include "letras.h"
#include "Usuarios.h"
#include "eeprom_controller.h"
#include "funciones_usuario.h"

#define LOOP while(true)

//  MENUS
const int SECUENCIA_INICIAL = 0, MENU_PRINCIPAL = 1, LOGIN = 2, REGISTER = 3, ADMIN = 4, CLIENTE = 5, ESCOGER_TECLADO = 6;
int menu_actual = SECUENCIA_INICIAL;

//  PINES
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int din = 10, cs = 9, clk = 8;
LedControl matriz(din, clk, cs, 1);

// BOTONES
Button Btn_Ok(23);
Button Btn_Cancel(24);

//Estado menus ingresos

int menu_ingresos = 0; // 0 = login ;1 = register; 2 = ingreso telefono 

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
bool APLICACION_ACTIVA = false;
bool KEYPAD_ACTIVO = false;

// ----------- VARIABLES APP ---------- //
char nombre_temp[11];
boolean entradaAceptada() {
    LOOP {
        if (digitalRead(4)) {
	    delay(210);
	    return true;
	}
        if (digitalRead(3)) {
	    delay(210);
	    return false;
	}
    }
} 
#define LINEA_VACIA "                "
String recibir_texto_app(char* mensaje, char* titulo);

/* Usuario */
Usuarios authenticated_user;

int letra_actual_index = 0;
// FUNCIONES
void menu_setup();
void menu_loop();
void conectar_dispositivo();
void limpiarBuffer();

// *MENUS
void secuencia_inicial();
void menu_principal();
void login();
void registro();

void letras_matriz();
void menu_administrar();
void menu_cliente();
void menu_seleccionar_teclado(int menu_ingresos);

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

    case ESCOGER_TECLADO:
        menu_seleccionar_teclado(menu_ingresos);
        break;

    case LOGIN:
        login();
        break;

    case REGISTER:
        registro();
        break;

    case ADMIN:
        menu_administrar();
        break;

    case CLIENTE:
        menu_cliente();
        break;

    default:
        break;
    }
}


void enviarConfirmar(char* cadena) {
    Serial1.println(cadena);
    bool hayAlgo = false;
    char recibidos[3];
    LOOP {
        while(Serial1.available()) {
	    Serial1.readBytes(recibidos, 2);
            hayAlgo = true;
        }
        if (hayAlgo && !Serial1.available()) break;
    }
}

void secuencia_inicial()
{
    lcd.clear(); // Borra la pantalla LCD.

    lcd.setCursor(0, 0);     // Establece el cursor en la posición (1, 0) de la pantalla LCD.
    lcd.print("Bienvenido"); // Imprime el mensaje "Bienvenido" en la pantalla LCD.

    // Imprime diferentes nombres en la pantalla LCD.
    delay(300);          // Pausa de 0.3 segundos
    lcd.setCursor(0, 1); // Establece el cursor en la posición (0, 1) de la pantalla LCD.
    lcd.print("Roberto202000544");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 2);
    lcd.print("Edgardo201801119");

    delay(300); // Pausa de 0.9 segundos
    lcd.setCursor(0, 3);
    lcd.print("Cesar 202010816");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 4);
    lcd.print("Jose 202001800");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 5);
    lcd.print("Angel 202012039");

    while (true)
    {
        if (Btn_Ok.is_pressed())
        {
            menu_actual = MENU_PRINCIPAL;
            break;
        }
    }
}

void limpiarBuffer() {
    int t0 = millis();
    int t1 = millis();
    LOOP {
        t1 = millis();
        while(Serial1.available()) {
	    Serial1.read();
        }
        if ((t1 - t0 >= 1000) && !Serial1.available()) break;
    }
}

void conectar_dispositivo()
{
  limpiarBuffer();
  lcd.clear();
  lcd.print(" Esperando  una ");
  lcd.setCursor(0, 1);
  lcd.print("   conexion...  ");
  bool alguienPorAhi = false;
        char recibidos[3];
   LOOP {
	        while(Serial1.available()) {
		    Serial1.readBytes(recibidos, 2);
		    alguienPorAhi = true;
		}
		if (alguienPorAhi && !Serial1.available()) break;
  }     
  limpiarBuffer();
  lcd.clear();
  lcd.print("Conectado con exito");
  delay(1000);
  lcd.clear();
  Serial.println("Final de conectar");
}

String recibir_texto_app(char* mensaje, char* titulo)
{
  bool termino = false;
  LOOP {
            
    Serial.println("Entro a loop nombre");
    if(termino)
    {
      break;
    }
    termino = true;
    limpiarBuffer();
    enviarConfirmar(mensaje);
    memset(nombre_temp, 0, 11);    
    lcd.clear();
    lcd.print(titulo);
    lcd.setCursor(0, 1);
    lcd.print(" - ");
    lcd.print(mensaje);
    lcd.print(":");
    lcd.println("");
    lcd.setCursor(0, 2);
    // OBTENER CADENA DE APLICACIÓN -- Nombre
    bool seEnvioAlgo = false;
    int indiceNombre = 0;
    long int t0 = millis();
    long int t1 = millis();
    limpiarBuffer();

    LOOP {
        // SI YA SE ENVIO ALGO DESDE LA APLICACION
        while (Serial1.available()) {
            seEnvioAlgo = true;
        //   RECIBIRLO
            nombre_temp[indiceNombre++] = Serial1.read();

            Serial.println(nombre_temp);
            Serial.println("Entro a en enviado algo");

        }
        // CONTROLAR CUANTO HA PASADO DESDE QUE COME...
        if (seEnvioAlgo) {
            t1 = millis();
            if (t1 - t0 >= 500) break;
        } else {
            t0 = millis();
            t1 = millis();
        }
    }
  }
  Serial.print("Dato obtenido de app");
  return nombre_temp;
}


void menu_seleccionar_teclado(int estado_menu_ingresos)
{
    lcd.clear(); // Borrar pantalla LCD

    lcd.setCursor(0, 0);
    lcd.print("Entrada:"); // Imprime el mensaje 

    // Imprime diferentes nombres en la pantalla LCD.
    delay(300);          // Pausa de 0.3 segundos
    lcd.setCursor(0, 1); // Establece el cursor en la posición (0, 1) de la pantalla LCD.
    lcd.print("1. Aplicacion");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 2);
    lcd.print("2. KeyPad");

    delay(300); // Pausa de 0.3 segundos
    //lcd.clear();

    lcd.setCursor(0, 3);
    lcd.print("Opcion:");
    String opcion = "";

    bool salir = false; // Se completo el ciclo y tiene que salir
    while (true)
    {
        char key = keypad.getKey();
        if (key != NO_KEY)
        {

            if (opcion.length() < 2)
            {
                opcion += key;
                lcd.setCursor(12, 3);
                lcd.print(opcion);
            }
            else
            {
                lcd.setCursor(12, 3);
                lcd.print("                ");
                lcd.setCursor(12, 3);
                lcd.print("MAX");
                delay(1000);
                lcd.setCursor(12, 3);
                lcd.print("                ");
                lcd.setCursor(12, 3);
                lcd.print(opcion);
            }
        }
        if (Btn_Ok.is_pressed())
        {
            int num;
            if (opcion == "1"){
                APLICACION_ACTIVA = true;
                KEYPAD_ACTIVO = false;
                Serial.println("1");
                Serial.print("APP");
                Serial.println(APLICACION_ACTIVA);
                Serial.print("Key");
                Serial.println(KEYPAD_ACTIVO);
                conectar_dispositivo();
                salir = true;
            }
            else if (opcion == "2")
            {
                APLICACION_ACTIVA = false;
                KEYPAD_ACTIVO = true;
                Serial.println("2");
                Serial.print("APP");
                Serial.println(APLICACION_ACTIVA);
                Serial.print("Key");
                Serial.println(KEYPAD_ACTIVO);
                switch (estado_menu_ingresos)
                {
                case 0:
                    menu_actual = LOGIN;
                    break;
                case 1:
                    menu_actual = REGISTER;
                    break;
                case 2:
                    menu_actual = MENU_PRINCIPAL;
                    //TODO INGRESO CELULAR
                    break;
                default:
                    break;
                }
                salir = true;
            }
            else{
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

        if(salir)
        {
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
    delay(200);          // Pausa de 0.3 segundos
    lcd.setCursor(0, 1); // Establece el cursor en la posición (0, 1) de la pantalla LCD.
    lcd.print("1. Login");

    delay(200); // Pausa de 0.3 segundos
    lcd.setCursor(0, 2);
    lcd.print("2. Registro");

    delay(200); // Pausa de 0.3 segundos
                //    lcd.clear();

    lcd.setCursor(0, 3);
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
                lcd.setCursor(12, 3);
                lcd.print(opcion);
            }
            else
            {
                lcd.setCursor(12, 3);
                lcd.print("      ");
                lcd.setCursor(12, 3);
                lcd.print("MAX");
                delay(100);
                lcd.setCursor(12, 3);
                lcd.print("      ");
                lcd.setCursor(12, 3);
                lcd.print(opcion);
            }
        }
        if (Btn_Ok.is_pressed())
        {
            int num;
            if (opcion == "1")
            {
                menu_actual = ESCOGER_TECLADO;
                menu_ingresos = 0;
                //menu_actual = LOGIN;
                break;
            }
            else if (opcion == "2")
            {
                menu_actual = ESCOGER_TECLADO;
                menu_ingresos = 1;
                //menu_actual = REGISTER;
                break;
            }
            else if (opcion == "0")
            {
                letras_matriz();
                break;
            }
            else
            {
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
    // lcd.clear();
    lcd.setCursor(0, 0);
    bool exit = false;
    int letra_actual_index = 0; // Índice de la letra actual en el arreglo letras[]
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
                // lcd.clear(); // Limpiar el LCD
                lcd.setCursor(0, 1);
                lcd.print(cadena);
                // delay(1000);  // Esperar un segundo para que se pueda leer la letra en el LCD
                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                cadena += key;
                // lcd.clear(); // Limpiar el LCD
                lcd.setCursor(0, 1);
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
            exit = true; // Salir del bucle si se alcanzó el final
        }
        if (Btn_Ok.is_pressed())
        {
            Serial.println(cadena);
            // return cadena;
            break;
        }
    }

    limpiar_matriz(matriz);
}

void login()
{
  //menu_seleccionar_teclado();
    lcd.clear(); // Borra la pantalla LCD.

    lcd.setCursor(0, 0);
    lcd.print("Nombre: ");
    bool ya_entro = false;
    String nombre = "";

    while (true)
    {
        if(KEYPAD_ACTIVO && !APLICACION_ACTIVA) 
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
                nombre += letra_actual.name;

                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                nombre += key;
                break;

            default:
                break;
            }
            if (nombre.length() < 12)
            {
                lcd.setCursor(0, 1);
                lcd.print(nombre);
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
                lcd.print(nombre);
            }
        }
      } 

      else if(!KEYPAD_ACTIVO && APLICACION_ACTIVA && !ya_entro) 
        {
          Serial.println("App");
          nombre = recibir_texto_app("Nombre", "LOGIN");
          lcd.setCursor(0,3);
          lcd.print(nombre);
          ya_entro = true;
        }

        if (Btn_Ok.is_pressed())
        {
            Serial.println(nombre);
            break;
        }
        if (Btn_Cancel.is_pressed())
        {
            if (nombre.length() > 0)
            {
                nombre.remove(nombre.length() - 1);
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print(nombre);
            }
        }
    }
    Serial.println("Sali del nombre");
    //lcd.clear();

    lcd.setCursor(0, 2);
    lcd.print("Password: ");
    //lcd.setCursor(0, 0);
    String password = "";
  while (true) {

    if (KEYPAD_ACTIVO && !APLICACION_ACTIVA) {
      // Obtener la letra actual
      Letras letra_actual = letras[letra_actual_index];

      // Imprimir la letra actual
      matriz_imprime_caracteres(matriz, letra_actual_index);

      // Esperar a que se presione el botón para cambiar la letra o imprimir la letra actual
      char key = keypad.getKey();
      if (key != NO_KEY) {
        switch (key) {
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
            password += letra_actual.name;

            break;

          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            password += key;
            break;

          default:
            break;
        }

        if (password.length() < 8) {
          lcd.setCursor(0, 3);
          lcd.print(password);
        } else {
          lcd.setCursor(0, 3);
          lcd.print("                ");
          lcd.setCursor(0, 3);
          lcd.print("MAX");
          delay(1000);
          lcd.setCursor(0, 3);
          lcd.print("                ");
          lcd.setCursor(0, 3);
          lcd.print(password);
        }
      }
    } else if (!KEYPAD_ACTIVO && APLICACION_ACTIVA && !ya_entro) {
      Serial.println("App");
      password = recibir_texto_app("Password", "LOGIN");
      lcd.setCursor(0, 3);
      lcd.print(password);
      ya_entro = true;
    }
        if (Btn_Ok.is_pressed())
        {
            Serial.println(password);
            break;
        }
        if (Btn_Cancel.is_pressed())
        {
            if (password.length() > 0)
            {
                password.remove(password.length() - 1);
                lcd.setCursor(3, 1);
                lcd.print("                ");
                lcd.setCursor(3, 1);
                lcd.print(password);
            }
        }
    }
    char *nombre_char = nombre.c_str();
    int longitud_nombre = strlen(nombre_char);
    char nombre_cifrado[longitud_nombre + 1];
    strcpy(nombre_cifrado, nombre_char);
    dobleCifradoXOR(nombre_cifrado);

    char *password_char = password.c_str();
    int longitud_password = strlen(password_char);
    char password_cifrado[longitud_password + 1];
    strcpy(password_cifrado, password_char);
    dobleCifradoXOR(password_cifrado);

    Serial.print("Nombre cifrado: ");
    Serial.println(nombre_cifrado);
    Serial.print("Password cifrado: ");
    Serial.println(password_cifrado);

    Usuarios usuario = login_user(nombre_cifrado, password_cifrado);

    // Serial.println(usuario.is_valid());
    // Serial.println(usuario.isAdmin);
    if (usuario.is_valid())
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bienvenido");
        lcd.setCursor(0, 1);
        lcd.print(usuario.nombre);

        authenticated_user = usuario;

        if (usuario.isAdmin)
        {
            delay(1000);
            menu_actual = ADMIN;
        }
        else
        {
            delay(1000);
            menu_actual = CLIENTE;
        }
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Incorrect");
        lcd.setCursor(0, 1);
        lcd.print("Credentials");
        delay(1000);
    }
}

void registro()
{

    lcd.clear(); // Borra la pantalla LCD.

    lcd.setCursor(0, 0);
    lcd.print("==Registro==");
    lcd.setCursor(0, 1);
    lcd.print("Nombre: ");
    String nombre_registro = "";

    while (true)
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
                nombre_registro += letra_actual.name;

                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                nombre_registro += key;
                break;

            default:
                break;
            }
            if (nombre_registro.length() <= 12)
            {
                lcd.setCursor(0, 2);
                lcd.print(nombre_registro);
            }
            else
            {
                lcd.setCursor(0, 2);
                lcd.print("         ");
                lcd.setCursor(0, 2);
                lcd.print("MAX");
                delay(1000);
                lcd.setCursor(0, 2);
                lcd.print("         ");
                lcd.setCursor(0, 2);
                lcd.print(nombre_registro);
            }
        }
        if (Btn_Ok.is_pressed())
        {
            Serial.println(nombre_registro);
            break;
        }
        if (Btn_Cancel.is_pressed())
        {
            if (nombre_registro.length() > 0)
            {
                nombre_registro.remove(nombre_registro.length() - 1);
                lcd.setCursor(0, 2);
                lcd.print("                ");
                lcd.setCursor(0, 2);
                lcd.print(nombre_registro);
            }
        }
    }

    lcd.clear(); // Borra la pantalla LCD.

    lcd.setCursor(0, 0);
    lcd.print("==Registro==");
    lcd.setCursor(0, 1);
    lcd.print("Celular: ");
    String celular_registro = "";

    while (true)
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
                celular_registro += letra_actual.name;

                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                celular_registro += key;
                break;

            default:
                break;
            }
            if (celular_registro.length() <= 8)
            {
                lcd.setCursor(0, 2);
                lcd.print(celular_registro);
            }
            else
            {
                lcd.setCursor(0, 2);
                lcd.print("         ");
                lcd.setCursor(0, 2);
                lcd.print("MAX");
                delay(1000);
                lcd.setCursor(0, 2);
                lcd.print("         ");
                lcd.setCursor(0, 2);
                lcd.print(celular_registro);
            }
        }
        if (Btn_Ok.is_pressed())
        {
            Serial.println(celular_registro);
            break;
        }
        if (Btn_Cancel.is_pressed())
        {
            if (celular_registro.length() > 0)
            {
                celular_registro.remove(celular_registro.length() - 1);
                lcd.setCursor(0, 2);
                lcd.print("                ");
                lcd.setCursor(0, 2);
                lcd.print(celular_registro);
            }
        }
    }

    lcd.clear(); // Borra la pantalla LCD.

    lcd.setCursor(0, 0);
    lcd.print("==Registro==");
    lcd.setCursor(0, 1);
    lcd.print("Contrasenia: ");
    String password_registro = "";

    while (true)
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
                password_registro += letra_actual.name;

                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                password_registro += key;
                break;

            default:
                break;
            }
            if (password_registro.length() <= 12)
            {
                lcd.setCursor(0, 2);
                lcd.print(password_registro);
            }
            else
            {
                lcd.setCursor(0, 2);
                lcd.print("         ");
                lcd.setCursor(0, 2);
                lcd.print("MAX");
                delay(1000);
                lcd.setCursor(0, 2);
                lcd.print("         ");
                lcd.setCursor(0, 2);
                lcd.print(password_registro);
            }
        }
        if (Btn_Ok.is_pressed())
        {
            Serial.println(password_registro);
            break;
        }
        if (Btn_Cancel.is_pressed())
        {
            if (password_registro.length() > 0)
            {
                password_registro.remove(password_registro.length() - 1);
                lcd.setCursor(0, 2);
                lcd.print("                ");
                lcd.setCursor(0, 2);
                lcd.print(password_registro);
            }
        }
    }

    lcd.clear();

    Usuarios usuario = Usuarios();

    char *nombre_char = nombre_registro.c_str();
    int longitud_nombre = strlen(nombre_char);
    char nombre_cifrado[longitud_nombre + 1];
    strcpy(nombre_cifrado, nombre_char);
    dobleCifradoXOR(nombre_cifrado);

    char *celular_char = celular_registro.c_str();
    int longitud_celular = strlen(celular_char);
    char celular_cifrado[longitud_celular + 1];
    strcpy(celular_cifrado, celular_char);
    dobleCifradoXOR(celular_cifrado);

    char *password_char = password_registro.c_str();
    int longitud_password = strlen(password_char);
    char password_cifrado[longitud_password + 1];
    strcpy(password_cifrado, password_char);
    dobleCifradoXOR(password_cifrado);

    strcpy(usuario.nombre, nombre_cifrado);
    strcpy(usuario.numero, celular_cifrado);
    strcpy(usuario.contrasenia, password_cifrado);
    bool error = false;

    if (is_user_registered(nombre_cifrado))
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("El usuario ya");
        lcd.setCursor(0, 1);
        lcd.print("existe :(");
        error = true;
        delay(2000);
    }
    if (strlen(nombre_cifrado) < 8 || strlen(nombre_cifrado) > 12)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("El nombre");
        lcd.setCursor(0, 1);
        lcd.print("debe ser de 8 digitos");
        lcd.setCursor(0, 2);
        lcd.print("minimo y 12 maximo");
        error = true;
        delay(2000);
    }
    if (strlen(celular_cifrado) != 8)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("El numero debe");
        lcd.setCursor(0, 1);
        lcd.print("ser de 8 digitos");
        error = true;
        delay(2000);
    }
    if (strlen(password_cifrado) < 8 || strlen(password_cifrado) > 12)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("La contrasenia");
        lcd.setCursor(0, 1);
        lcd.print("debe ser de 8 digitos");
        lcd.setCursor(0, 2);
        lcd.print("minimo y 12 maximo");
        error = true;
        delay(2000);
    }
    if (!error)
    {
        usuario.isAdmin = false;
        strcpy(usuario.nombre, nombre_cifrado);
        strcpy(usuario.numero, celular_cifrado);
        strcpy(usuario.contrasenia, password_cifrado);
        Serial.print("Usuario registrado: ");
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Usuario registrado");
        delay(1000);

        write_user(usuario);
        Serial.print("Usuario Registrado: ");
        Serial.println(get_user_count());
    }

    menu_actual = MENU_PRINCIPAL;
}

void menu_cliente()
{
   Serial.println("Soy un cliente :)");
   Serial.println(authenticated_user.nombre);
}

void menu_administrar()
{
    lcd.clear(); // Borra la pantalla LCD.

    lcd.setCursor(0, 0);     // Establece el cursor en la posición (1, 0) de la pantalla LCD.
    lcd.print("Menu ADMIN"); // Imprime el mensaje "Bienvenido" en la pantalla LCD.

    // Imprime diferentes nombres en la pantalla LCD.
    delay(300);
    // lcd.clear();         // Borra la pantalla LCD.        // Pausa de 0.3 segundos
    lcd.setCursor(0, 1); // Establece el cursor en la posición (0, 1) de la pantalla LCD.
    lcd.print("1. LOGS");

    delay(300); // Pausa de 0.3 segundos
    lcd.setCursor(0, 2);
    lcd.print("2. CHECK STATUS");

    delay(300); // Pausa de 0.3 segundos
    // lcd.clear();

    lcd.setCursor(0, 3);
    lcd.print("Opcion:");
    String opcion2 = "";

    while (true)
    {
        char key = keypad.getKey();
        if (key != NO_KEY)
        {

            if (opcion2.length() < 2)
            {
                opcion2 += key;
                lcd.setCursor(10, 3);
                lcd.print(opcion2);
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
                lcd.print(opcion2);
            }
        }
        if (Btn_Ok.is_pressed())
        {
            int num;
            if (opcion2 == "1")
                num = 1;
            else if (opcion2 == "2")
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
            if (opcion2.length() > 0)
            {
                opcion2.remove(opcion2.length() - 1);
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print(opcion2);
            }
        }
    }
}
#endif