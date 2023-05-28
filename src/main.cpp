#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // si no te sale con esta direccion  puedes usar (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2)
Servo servo1, servo2;

const int botonInicioPin = 2;
const int botonDetenerPin = 3;
volatile bool contadorIniciado = false;
volatile bool detenerContador = false;
volatile unsigned long contador = 0;

int valoresSeleccionados[7];
int contadorValores = 0;

void displayStart();
void iniciarContador();
void pararContador();
bool validarNumeroEnArray(int numero, const int *arreglo, int longitud);

    void setup()
{
  servo1.attach(9);
  servo2.attach(10);

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
          contador = 1;

        lcd.clear();
        lcd.print("Carta actual: " + String(contador));
        delay(1000);

      }
      
      lcd.clear();
      if (!validarNumeroEnArray(contador, valoresSeleccionados, 7)){
        lcd.print("Carta elegida: " + String(contador));
        valoresSeleccionados[contadorValores] = contador;
        contadorValores++;
      }else{
        lcd.print("Carta " + String(contador));
        lcd.setCursor(0,1);
        lcd.print("Ya seleccionada!");
      }

      delay(1000);
      displayStart();
      // Reiniciar variables
      contador = 0;
      contadorIniciado = false;
      detenerContador = false;

      
    }
    lcd.clear();
    lcd.print("Cartas elegidas:");
    lcd.setCursor(0,1);
    for (int i = 0; i < contadorValores; i++)
    {
      lcd.print(valoresSeleccionados[i]);
      delay(500);
    }

    contadorValores = 0;
    
  }
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