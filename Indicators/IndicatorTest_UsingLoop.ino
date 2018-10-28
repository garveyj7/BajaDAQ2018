#include <TimerThree.h>

#define hallEffectPin 2
#define shiftData 11  // Pin connected to DS of 74HC595 (P.14)
#define shiftCLK 12 // Pin connected to SH_CP of 74HC595 (P.11)
#define shiftLatch 8  // Pin connected to ST_CP of 74HC595 (P.12)
#define RPMReadPin 0  // Connected to the output of the inductive RPM reader
#define gasResetButton 2  // Pin connected to gas reset button
#define batteryReadPin 1  // Connected to the + terminal of the battery
#define batteryIndicator 23 //  Connected to the battery indicator LED

int RPMVoltage; // 0-1023 (0-5V), avg voltage of RPM pulses
int encodedRPM; // Encoded so it can be pushed to the shift reg.
int gasTimer = 0;
int gasResetButtonState;
int encodedGas;
int batteryVoltage; // 0-1023 (0-5V), voltage of battery, will have to use voltage divider to step down 12V battery
int updateDAQ = 0;

void setup() {
  pinMode(shiftData, OUTPUT);
  pinMode(shiftCLK, OUTPUT);
  pinMode(shiftLatch, OUTPUT);
  
  pinMode(batteryIndicator, OUTPUT);
  pinMode(gasResetButton, INPUT);
  
  attachInterrupt(0, resetGas, RISING); // Interrupts can be placed on pins 2, 3, 18, 19, 20, 21 of MEGA

  Timer3.initialize(100000);
  Timer3.attachInterrupt(updateStatus); // Run every 0.1 seconds
  Serial.begin(9600);
}

void loop(){
  if (updateDAQ == 1){
    tachometer(); // Find the RPM value
    gasLevel(); // Find the gas level
    batteryLevel(); // Find the battery level
    updateShiftRegisters();
  }
}

void updateStatus(){
  updateDAQ = 1;
  gasTimer++;
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
  if (gasTimer < 50){ encodedGas = 0b000001111; } // If the timer = 5s
  else if (gasTimer > 50 & gasTimer <= 100){ encodedGas = 0b00000111; } // If the timer is between 5 and 10s
  else if (gasTimer > 100 & gasTimer <= 150){ encodedGas = 0b00000011; }  // If the timer is between 10 and 15s
  else if (gasTimer > 150){ encodedGas = 0b00000001;} // If the timer is over between 15s
}

void batteryLevel(){
  batteryVoltage = analogRead(batteryReadPin);  // Returns 0-1023 (0-5V)
  // Check if LED is off, if it is, turn the LED on and update status to on
  if (batteryVoltage < 800){
    digitalWrite(batteryIndicator, HIGH);
  }
  // Always keep off otherwise
  else {digitalWrite(batteryIndicator, LOW);}
}

void resetGas(){
  gasTimer = 0; // Set timer back to 0 if the reset button is pushed
}

void updateShiftRegisters(){
    shiftOut(shiftData, shiftCLK, MSBFIRST, encodedGas); // Load first 8 bits to shift reg.
    shiftOut(shiftData, shiftCLK, MSBFIRST, encodedRPM); // Load second 8 bits to shift reg.

    digitalWrite(shiftLatch, LOW);  // Take the latchPin low so the LEDs don't change while you're sending in bits:
    digitalWrite(shiftLatch, HIGH); // Take the latch pin high so the LEDs will light up
}
