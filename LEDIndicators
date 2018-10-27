#include <TimerThree.h>

#define shiftData 11  // Pin connected to DS of 74HC595 (P.14)
#define shiftCLK 12 // Pin connected to SH_CP of 74HC595 (P.11)
#define shiftLatch 8  // Pin connected to ST_CP of 74HC595 (P.12)
#define RPMReadPin 0  // Connected to the output of the inductive RPM reader
#define gasReadPin 1  // Arbitrary value assigned for testing the indicators, (will not be using analog reading for final DAQ)
#define batteryReadPin 2  // Connected to the + terminal of the battery
#define batteryIndicator 22 //  Connected to the battery indicator LED

int RPMVoltage; // 0-1023 (0-5V), avg voltage of RPM pulses
int encodedRPM; // Encoded so it can be pushed to the shift reg.
int gasVoltage; // Arbitrary value assigned for testing the indicators, (will not be using analog reading for final DAQ)
int encodedGas; // ^
int batteryVoltage; // 0-1023 (0-5V), voltage of battery, will have to use voltage divider to step down 12V battery
int batteryIndicatorState = 0;  // Holds the state of the battery light so it can blink


void setup() {
  pinMode(shiftData, OUTPUT);
  pinMode(shiftCLK, OUTPUT);
  pinMode(shiftLatch, OUTPUT);
  
  pinMode(batteryIndicator, OUTPUT);

  Timer3.initialize(100000);
  Timer3.attachInterrupt(updateDAQ); // Run every 0.1 seconds
  Serial.begin(9600);
}

void loop() {
}

void updateDAQ(){
  tachometer(); // Find the RPM value
  gasLevel(); // Find the gas level
  batteryLevel(); // Find the battery level

  shiftOut(shiftData, shiftCLK, MSBFIRST, encodedGas); // Load first 8 bits to shift reg.
  shiftOut(shiftData, shiftCLK, MSBFIRST, encodedRPM); // Load second 8 bits to shift reg.

  digitalWrite(shiftLatch, LOW);  // Take the latchPin low so the LEDs don't change while you're sending in bits:
  digitalWrite(shiftLatch, HIGH); // Take the latch pin high so the LEDs will light up
}


void tachometer(){
  RPMVoltage = analogRead(RPMReadPin); // Returns 0-1023 (0-5V)
  // Set the encoded RPM to the respective LEDs in the tachometer
  if (RPMVoltage <= 100){ encodedRPM = 0b00000000; }
  else if (RPMVoltage > 100 & RPMVoltage <= 200){ encodedRPM = 0b00000001; }
  else if (RPMVoltage > 200 & RPMVoltage <= 300){ encodedRPM = 0b00000011; }
  else if (RPMVoltage > 300 & RPMVoltage <= 400){ encodedRPM = 0b00000111; }
  else if (RPMVoltage > 400 & RPMVoltage <= 500){ encodedRPM = 0b00001111; }
  else if (RPMVoltage > 500 & RPMVoltage <= 600){ encodedRPM = 0b00011111; }
  else if (RPMVoltage > 600 & RPMVoltage <= 700){ encodedRPM = 0b00111111; }
  else if (RPMVoltage > 700 & RPMVoltage <= 800){ encodedRPM = 0b01111111; }
  else if (RPMVoltage > 800){ encodedRPM = 0b11111111; }
}

void gasLevel(){
  // Set the encoded gas level to the respective LEDs in the gas gauge
  gasVoltage = analogRead(gasReadPin); // Returns 0-1023 (0-5V)
  if (gasVoltage < 200){ encodedGas = 0b00000001; }
  else if (gasVoltage > 200 & gasVoltage <= 400){ encodedGas = 0b00000011; }
  else if (gasVoltage > 400 & gasVoltage <= 600){ encodedGas = 0b00000111; }
  else if (gasVoltage > 600 & gasVoltage <= 800){ encodedGas = 0b00001111; }
}

void batteryLevel(){
  batteryVoltage = analogRead(batteryReadPin);  // Returns 0-1023 (0-5V)
  // Check if LED is off, if it is, turn the LED on and update status to on
  if (batteryVoltage < 800 && batteryIndicatorState == 0){
    digitalWrite(batteryIndicator, HIGH);
    batteryIndicatorState = 1;
  }
  // Check if the LED is on, if it is, turn the LED off and update status to off
  else if (batteryVoltage < 800 && batteryIndicatorState == 1){
    digitalWrite(batteryIndicator, LOW);
    batteryIndicatorState = 0;
  }
  // Always keep off otherwise
  else {digitalWrite(batteryIndicator, LOW);}
}
