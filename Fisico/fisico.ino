/*#include <Wire.h>

void setup() {
  Serial.begin (9600);

  // Leonardo: wait for serial port to connect
  while (!Serial) 
    {
    }

  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  
  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      { 
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
      } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
}  // end of setup

void loop() {}*/

#include <LiquidCrystal_I2C.h>

/*
Esta línea de código está inicializando un nuevo objeto de la clase LiquidCrystal_I2C 
que se usa para controlar una pantalla LCD usando el protocolo I2C.
Los parámetros pasados al constructor son la dirección I2C de la pantalla LCD (39)
El número de columnas (16)
Y el número de filas (2).
*/
LiquidCrystal_I2C lcd(39, 16, 2);

/*
Estas líneas de código definen los números de pin para los sensores ultrasónicos (trigPin1, echoPin1, trigPin2, echoPin2) 
Y los sensores LDR (ldrPin1, ldrPin2). 
También están estableciendo el valor umbral para la oscuridad (umbral), 
la tolerancia para las medidas de distancia (tolerancia) 
y los valores de altura inicial para cada compartimento (alturaInicial1, alturaInicial2).
*/
const int trigPin1 = 2;
const int echoPin1 = 3;
const int trigPin2 = 4;
const int echoPin2 = 5;

const int ldrPin1 = A0;
const int ldrPin2 = A1;

const int umbral = 15;  // Umbral para determinar obscuridad

const int tolerancia = 1;

float alturaInicial1 = 7;
float alturaInicial2 = 7;

/*
Esta es la función de configuración que inicializa los pines para los sensores ultrasónicos (trigPin1, echoPin1, trigPin2, echoPin2) 
y sensores LDR (ldrPin1, ldrPin2). 
También inicializa la pantalla LCD utilizando la biblioteca LiquidCrystal_I2C
*/
void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  lcd.init();
  lcd.clear();
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {

  // Compartimento 1
  float distancia1 = medirDistancia(trigPin1, echoPin1);
  bool obscuridad1 = verificarObscuridad(ldrPin1);

  Serial.println(medirDistancia(trigPin1, echoPin1));
  Serial.println(medirDistancia(trigPin2, echoPin2));

  if (obscuridad1) {
    // La puerta del compartimento 1 está cerrada
    if (distancia1 >= 7) {
      alturaInicial1 = 7;
      //notificar vacío
      lcd.setCursor(0, 0);
      lcd.print("-Comp-1: vacio--");
      Serial.println("comp-1 vacio");
    }
    if (distancia1 < 7) {
      if (distancia1 > alturaInicial1 + tolerancia || distancia1 < alturaInicial1 - tolerancia) {
        //notificar anomalia
        lcd.setCursor(0, 0);
        lcd.print("Anomalia Comp-1-");
        Serial.println("anomalia comp-1");
      } else {
        //notificar
        lcd.setCursor(0, 0);
        lcd.print("-Comp-1: en uso-");
        Serial.println("comp-1 en uso");
      }
    }
  } else if (!obscuridad1) {
    // La puerta del compartimento 1 está abierta
    //notificar puerta abierta
    lcd.setCursor(0, 0);
    lcd.print("Comp-1: abierto-");
    Serial.println("comp-1 abierto");
    while (!verificarObscuridad(ldrPin1)) {
      delay(250);
    }
    delay(500);
    if (verificarObscuridad(ldrPin1)) {
      alturaInicial1 = medirDistancia(trigPin1, echoPin1);
      //notificar nueva altura
      if (medirDistancia(trigPin1, echoPin1) < 6) {
        lcd.setCursor(0, 0);
        lcd.print("Comp-1:Nuevo obj");
        Serial.println("Nuevo obj");
      }

      delay(500);
    }
  }

  // Compartimento 2
  float distancia2 = medirDistancia(trigPin2, echoPin2);
  bool obscuridad2 = verificarObscuridad(ldrPin2);

  if (obscuridad2) {
    // La puerta del compartimento 1 está cerrada
    if (distancia2 >= 7) {
      alturaInicial2 = 7;
      //notificar vacío
      lcd.setCursor(0, 1);
      lcd.print("-Comp-2: vacio--");
      Serial.println("comp-2 vacio");
    }
    if (distancia2 < 7) {
      if (distancia2 > alturaInicial2 + tolerancia || distancia2 < alturaInicial2 - tolerancia) {
        //notificar anomalia
        lcd.setCursor(0, 1);
        lcd.print("Anomalia Comp-2-");
        Serial.println("anomalia comp-2");
      } else {
        //notificar
        lcd.setCursor(0, 1);
        lcd.print("-Comp-2: en uso-");
        Serial.println("comp-2 en uso");
      }
    }
  } else if (!obscuridad2) {
    // La puerta del compartimento 1 está abierta
    //notificar puerta abierta
    lcd.setCursor(0, 1);
    lcd.print("Comp-2: abierto-");
    Serial.println("comp-2 abierto");
    while (!verificarObscuridad(ldrPin2)) {
      delay(250);
    }
    delay(500);
    if (verificarObscuridad(ldrPin2)) {
      alturaInicial2 = medirDistancia(trigPin2, echoPin2);
      //notificar nueva altura
      if (medirDistancia(trigPin2, echoPin2) < 6) {
        lcd.setCursor(0, 1);
        lcd.print("Comp-2:Nuevo obj");
        Serial.println("Nuevo obj");
      }
      delay(500);
    }
  }

  delay(800);
}
/*
`medirDistancia` es una función que toma dos parámetros enteros `trigPin` y `echoPin` que representan los pines conectados a un sensor ultrasónico. 
La función utiliza estos pines para enviar una señal de activación y medir la duración de la señal de eco. 
Luego calcula la distancia a un objeto utilizando la fórmula `distancia = duración * 0.034 / 2` 
donde `duración` es el tiempo que tarda la señal de eco en regresar. 
La función devuelve la distancia como un valor flotante.
*/
float medirDistancia(int trigPin, int echoPin) {
  //Este código envía una señal de disparo a un sensor ultrasónico conectado al pin `trigPin`.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  /*
 `float distancia = duracion * 0.034 / 2;` está calculando la distancia a un objeto utilizando la duración de la señal de eco de un sensor ultrasónico.
  La fórmula utilizada se basa en la velocidad del sonido en el aire (aproximadamente 0,034 cm/microsegundo) 
  y en el hecho de que el eco viaja el doble de la distancia hasta el objeto y de regreso al sensor. 
  El resultado se almacena como un valor de coma flotante en la variable `distancia`.
  */
  float distancia = duracion * 0.034 / 2; 

  return distancia;
}

/*
`verificarObscuridad` es una función que toma un parámetro entero `ldrPin` que representa el pin conectado a una resistencia dependiente de la luz (LDR). 
 La función lee el valor analógico del pin LDR usando `analogRead` y lo almacena en la variable entera `valor`. 
 Luego verifica si el valor de `valor` es menor que el valor de umbral `umbral`. 
 Si es así, la función devuelve `verdadero` indicando que está oscuro, de lo contrario devuelve `falso` indicando que no está oscuro. 
 Esta función se utiliza para determinar si la puerta de un compartimento está cerrada o abierta en función de la cantidad de luz que detecta el LDR.
*/
bool verificarObscuridad(int ldrPin) {
  int valor = analogRead(ldrPin);

  if (valor < umbral) {
    return true;  // Está oscuro
  } else {
    return false;  // No está oscuro
  }
}
/*
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,16,2);

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();
  
  lcd.setCursor(2,0);
  lcd.print("Hello world!");
  
  lcd.setCursor(2,1);
  lcd.print("LCD Tutorial");
}

void loop() {}*/