#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // si no te sale con esta direccion  puedes usar (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2)
Servo servo1, servo2;

const int botonInicioPin = 2;
const int botonDetenerPin = 3;
const int servoPlataformaPin = 9;
const int servoCartaPin = 10;
volatile bool contadorIniciado = false;
volatile bool detenerContador = false;
volatile unsigned int contador = 0;
volatile unsigned int posServo1 = 0;

int valoresSeleccionados[7];
int contadorValores = 0;

void displayStart();
void iniciarContador();
void pararContador();
bool validarNumeroEnArray(int numero, const int *arreglo, int longitud);
void tirarCarta();

void setup()
{
  servo1.attach(servoPlataformaPin);
  servo2.attach(servoCartaPin);

  pinMode(botonInicioPin, INPUT_PULLUP);
  pinMode(botonDetenerPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(botonInicioPin), iniciarContador, RISING);
  attachInterrupt(digitalPinToInterrupt(botonDetenerPin), pararContador, RISING);

  lcd.init();
  lcd.backlight();

  displayStart();
}

void loop()
{
  if (contadorIniciado)
  {
    while (contadorValores < 7)
    {
      while (!detenerContador)
      {
        contador++;
        if (contador > 18)
        {
          contador = 1;
          posServo1 = 0;
          servo1.write(posServo1);
        }

        lcd.clear();
        lcd.print("Carta actual: " + String(contador));
        servo1.write(posServo1);
        delay(1000);
        posServo1 += 10;
      }

      lcd.clear();
      if (!validarNumeroEnArray(contador, valoresSeleccionados, 7))
      {
        lcd.print("Carta elegida: " + String(contador));
        valoresSeleccionados[contadorValores] = contador;
        contadorValores++;
        tirarCarta();
        posServo1 = 0;
      }
      else
      {
        lcd.print("Carta " + String(contador));
        lcd.setCursor(0, 1);
        lcd.print("Ya seleccionada!");
        posServo1 = 0;
      }
      delay(500);

      // delay(1000);
      displayStart();
      // Reiniciar variables
      contador = 0;
      contadorIniciado = false;
      detenerContador = false;
    }
    lcd.clear();
    lcd.print("Cartas elegidas:");
    lcd.setCursor(0, 1);
    for (int i = 0; i < contadorValores; i++)
    {
      lcd.print(String(valoresSeleccionados[i]) + ' ');
      delay(500);
    }

    contadorValores = 0;
  }
}

void tirarCarta()
{
  servo2.write(0);
  delay(500);
  servo2.write(90);
  delay(500);
}

void displayStart()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pulse el boton");
  lcd.setCursor(0, 1);
  lcd.print("para iniciar");
}

void iniciarContador()
{
  contadorIniciado = true;
}

void pararContador()
{
  detenerContador = true;
}

bool validarNumeroEnArray(int numero, const int *arreglo, int longitud)
{
  for (int i = 0; i < longitud; i++)
  {
    if (arreglo[i] == numero)
    {
      return true;
    }
  }
  return false;
}