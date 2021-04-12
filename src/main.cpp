#include <Arduino.h>
#include <M5Stack.h>
#include <Generator.h>
#include <GeneratorTask.h>
#include <Screen.h>
#include <ScreenTask.h>
#include <BufferQueue.h>

void setup()
{
  M5.begin();
  M5.Power.begin();
  Serial.println("Hello from Taras");

  GeneratorParameters params = {
      .samplingFrequency = 100000,
      .signalFrequency = 1000,
      .waveform = Waveform::Sawtooth,
      .amplitude = 100};

  Generator generator(params);
  Screen screen;
  BufferQueue generatorQueue(64, 8);
  ScreenTask screenTask(screen, generatorQueue);
  GeneratorTask generatorTask(generator, generatorQueue);

  screenTask.start();
  generatorTask.start();

  while (true)
  {
    loop();
  }
}

void loop()
{
  Serial.println("loop");
  delay(10000);
}