#include <TimerThree.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#define LED 13
#define hallEffectPin 2

int axleSpeed = 0;
int axleRevCount = 0;

int LEDstate = 0;
int speedometerTimer = 0;

Adafruit_7segment matrix = Adafruit_7segment();

void setup(void) {
  pinMode(LED, OUTPUT);
  
  Serial.begin(9600);
  matrix.begin(0x70);
  Timer3.initialize(100000);
  Timer3.attachInterrupt(updateCluster);

  attachInterrupt(0, hallEffect, CHANGE);
}

void loop(void) {
  matrix.print(axleSpeed, DEC);
  matrix.writeDisplay();
}

void updateCluster(){
  speedometerTimer++;
  if(speedometerTimer > 2){
    axleSpeed = axleRevCount;
    speedometerTimer = 0;
    axleRevCount = 0;
  }
}

void hallEffect() {
  axleRevCount++;
  LEDstate = digitalRead(hallEffectPin);
  digitalWrite(LED, LEDstate);
}
