#include <Arduino.h>

#define rows 10
#define cols 7
#define digit1 2
#define digit2 3
#define time 10

int encoderPinA = A2; // CLK pin
int encoderPinB = A4; // DT pin
int encoderBtn = 12; // SW pin
int buzzerPin = A1;
int count = 0;
int encoderPinA_prev;
int encoderPinA_value;
boolean bool_CW;

bool numbers[rows][cols] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}, // 9
};

int pins[cols] = {4, 5, 6, 7, 8, 9, 10};

void setNumber(int number)
{
  for (int i = 0; i < cols; i++)
  {
    digitalWrite(pins[i], numbers[number / 10][i]);
  }

  digitalWrite(digit1, LOW);
  delay(time);
  digitalWrite(digit1, HIGH);

// DRUHA CIFRA

  for (int i = 0; i < cols; i++)
  {
    digitalWrite(pins[i], numbers[number % 10][i]);
  }

  digitalWrite(digit2, LOW);
  delay(time);
  digitalWrite(digit2, HIGH);
}

void setup()
{

  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(encoderBtn, INPUT_PULLUP);
  encoderPinA_prev = digitalRead(encoderPinA);

  for (int i = 0; i < cols; i++)
  {
    pinMode(pins[i], OUTPUT);
  }
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  digitalWrite(digit1, LOW);
  digitalWrite(digit2, LOW);
}

void loop()
{
  encoderPinA_value = digitalRead(encoderPinA);
  if (encoderPinA_value != encoderPinA_prev) { // check if knob is rotating
    // if pin A state changed before pin B, rotation is clockwise
    encoderPinA_prev = encoderPinA_value;
    if (digitalRead(encoderPinB) != encoderPinA_value) {
      count ++;
      bool_CW = true;
    } else {
      // if pin B state changed before pin A, rotation is counter-clockwise
      bool_CW = false;
      count--;
    }

    if (bool_CW) {
      Serial.print("Clockwise | ");
    } else {
      Serial.print("Counter-Clockwise | ");
    }
      Serial.print(count);
      Serial.print(" | ");
    }

  if (count < 0)
    count = 99;

  if (count > 99)
    count = 0;

  if (digitalRead(encoderBtn) == LOW) {
    tone(buzzerPin, count * 100);
  }
  else {
    noTone(buzzerPin);
  }

  setNumber(count);
}
