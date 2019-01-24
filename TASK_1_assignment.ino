#include <Wire.h>
#include <ZumoShield.h>
#include <NewPing.h>
#define LED 13

#define QTR_THRESHOLD  1500 // microseconds

#define REVERSE_SPEED     50 // 0 is stopped, 400 is full speed
#define TURN_SPEED        100
#define FORWARD_SPEED     50
#define REVERSE_DURATION  50 // ms
#define TURN_DURATION     50 // ms
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
int incomingByte;
void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);

  // play audible countdown
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
}

void setup() {
  // uncomment if necessary to correct motor directions
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
  pinMode(LED, HIGH);
  Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'W') {
     motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'S') {
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    }
     if (incomingByte == 'F') {
      motors.setSpeeds(0, 0);
    }
     if (incomingByte == 'A') {
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    }
     if (incomingByte == 'D') {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    }
  }
  
  // put your main code here, to run repeatedly:

}
