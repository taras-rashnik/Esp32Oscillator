#include <Arduino.h>
#include <M5Stack.h>
#include <Generator.h>
#include <GeneratorTask.h>
#include <Screen.h>
#include <ScreenTask.h>
#include <BufferQueue.h>
#include <DacTask.h>
#include <AdcTask.h>
#include <BleTask.h>
#include <BleAdapter.h>

void setup()
{
  // M5.begin();
  // M5.Power.begin();
  Serial.begin(115200);
  Serial.println("Hello from Taras");

  GeneratorParameters params = {
      .samplingFrequency = 44100,
      .signalFrequency = 2000,
      .waveform = Waveform::Sawtooth,
      .amplitude = 128};

  Generator generator(params);
  Screen screen;
  BleAdapter bleAdapter;

  BufferQueue generatorQueue(64, 8);
  BufferQueue adcQueue(64, 8);

  AdcTask adcTask(adcQueue);
  ScreenTask screenTask(screen, adcQueue);

  GeneratorTask generatorTask(generator, generatorQueue);
  DacTask dacTask(generatorQueue);
  BleTask bleTask(bleAdapter, generatorQueue);

  // screenTask.start();
  // adcTask.start();

  generatorTask.start();
  // dacTask.start();
  bleTask.start();

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