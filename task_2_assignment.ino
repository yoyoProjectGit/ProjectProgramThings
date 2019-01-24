#include <Wire.h>
#include <ZumoShield.h>
#define LED 13
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  500 // microseconds
// these might need to be tuned for different motor types
#define REVERSE_SPEED     100 // 0 is stopped, 400 is full speed
#define TURN_SPEED        100
#define FORWARD_SPEED     100
#define REVERSE_DURATION  250 // ms
#define TURN_DURATION     100 // ms
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
bool autoMode = true;
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
int incomingByte;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
void manualControls()
 {
  Serial.println("Manual control has been assigned!");
    while(autoMode == false)
    {
    incomingByte = Serial.read();
    if (incomingByte == 'W') {
     motors.setSpeeds(100, 100);
    }
    if (incomingByte == 'S') {
      motors.setSpeeds(-100, -100);
    }
     if (incomingByte == 'F') {
      motors.setSpeeds(0, 0);
    }
     if (incomingByte == 'A') {
    motors.setSpeeds(-100, 100);
    delay(100);
    }
     if (incomingByte == 'D') {
    motors.setSpeeds(100, -100);
    delay(100);
     }
    if (incomingByte == 'C')
    {
      motors.setSpeeds(0,0);
      autoMode = true;
    } 
  }
 }
void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);
//  for (int i = 0; i < 3; i++)
//  {
//    delay(1000);
//    buzzer.playNote(NOTE_G(3), 200, 15);
//  }
//  delay(1000);
//  buzzer.playNote(NOTE_G(4), 500, 15);
//  delay(1000);
}

void setup()
{
  // uncomment if necessary to correct motor directions
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
  pinMode(LED, HIGH);
  Serial.begin(9600);
  waitForButtonAndCountDown();
}

void loop()
{
  incomingByte = Serial.read();
  if (autoMode == true)
  {
    automaticControls();
  }
  if (incomingByte == 'F')
  {
    Stop();
  }
  if (incomingByte == 'C')
  {
    automaticControls();
  }
  if (incomingByte == 'A')
  {
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(1320);
    Stop();
  }
  if (incomingByte == 'D')
  {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(1320);
    Stop();
  }
}

 void automaticControls()
 {
  Serial.println("Automatic Control started!");
  while (autoMode == true)
  {
  sensors.read(sensor_values);
  incomingByte = Serial.read();
  if (sensor_values[0] > QTR_THRESHOLD)
 {
   motors.setSpeeds(100,100);
   delay(100);
   sensors.read(sensor_values);
   Serial.println("Checking for wall!");
  if(sensor_values[5] > QTR_THRESHOLD)
  {
    motors.setSpeeds(0,0);
    Serial.println("I have reached a wall");
    motors.setSpeeds(-100,-100);
    delay(500);
    motors.setSpeeds(0,0);
    autoMode = false;
    manualControls();
  }
  else
  {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
 }
  else if (sensor_values[5] > QTR_THRESHOLD)
   {
   motors.setSpeeds(100,100);
   delay(100);
   sensors.read(sensor_values);
   Serial.println("Checking for wall!");
   if(sensor_values[0] > QTR_THRESHOLD)
   {
   motors.setSpeeds(0,0);
   Serial.println("I have reached a wall");
   motors.setSpeeds(-100,-100);
   delay(500);
   motors.setSpeeds(0,0);
   autoMode = false;
   manualControls();
   }
  else
  {
    //if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
 }
  else 
  {
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  if (incomingByte == 'F')
  {
    autoMode = false;
  }
  }
 }
 void Stop()
 {
  motors.setSpeeds(0,0);
 }

  
