#include <Arduino.h>
#include <M5Stack.h>
#include "Samples.h"
#include "SamplesGenerator.h"
#include "Screen.h"


void setup()
{
  M5.begin();
  M5.Power.begin();
  Serial.println("Hello from Taras");

  Screen screen;
  SamplesGenerator generator;
  Samples samples(512);

  while (true)
  {
    Serial.println("while (true)");
    delay(10);
    M5.update();

    generator.writeTo(samples);
    screen.readFrom(samples);
  }
}

void loop()
{
  Serial.println("loop");
}