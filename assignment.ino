#include <Wire.h>
#include <ZumoShield.h>
#define LED 13
#define QTR_THRESHOLD  500 // microseconds
#define REVERSE_SPEED     100 // 0 is stopped
#define TURN_SPEED        100
#define FORWARD_SPEED     100
#define REVERSE_DURATION  250 // ms
#define TURN_DURATION     100 // ms
#define NUM_SENSORS 6

int roomnumber = 0;
int found = 0;
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
bool autoMode = true;
unsigned int sensor_values[NUM_SENSORS];
int incomingByte;
int trigPin = 2;    // Trigger
int echoPin = 6;    // Echo
long duration, cm;
bool checkingRoom = false;
unsigned long starting, finish, elapsed, starting2, finish2, elapsed2, timeInCorridor;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
void manualControls()
 {
    Serial.println("Manual control has been assigned!");
    while(autoMode == false)
    {
      Serial.println("im in here");
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
    if (incomingByte == 'E')
    {
      elapsed = finish - starting;
      elapsed2 = finish2 - starting2;
      timeInCorridor = elapsed - elapsed2;
      Serial.println(elapsed);
      Serial.println(elapsed2);
      Serial.println(timeInCorridor);
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
      automaticControls();
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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
    //autoMode = true;
    automaticControls();
  }
  if (incomingByte == 'A')
  {
    roomnumber++;
    starting2 = millis();
    checkingRoom = true;
    Serial.print("We have entered room number: ");
    Serial.print(roomnumber);
    Serial.println(" Location: Left.");
    while (checkingRoom == true)
    {
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //turning left into the room
    delay(850);
    motors.setSpeeds(100,100); //go into room
    delay(500);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //start searching room. 850 ms
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //start searching room. 600 ms
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //start searching room. 350 ms
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //start searching room. 100 ms
    delay(100);
    CheckIfPerson();
    motors.setSpeeds(TURN_SPEED,-TURN_SPEED); //start searching other side of room 1700
    delay(1000);
    CheckIfPerson();
    motors.setSpeeds(TURN_SPEED,-TURN_SPEED); //start searching other side of room 700
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(TURN_SPEED,-TURN_SPEED); //start searching other side of room 450
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(TURN_SPEED,-TURN_SPEED); //start searching other side of room 200
    delay(200);
    CheckIfPerson();
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //turn back to leave room
    delay(850);
    motors.setSpeeds(-100,-100); //reverse out of room
    delay(500);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //turn back to direction of corridoor
    delay(850);
    if (found > 0)
    {
      Serial.print("We have found an object in room ");
      Serial.println(roomnumber);
      found = 0;
    }
    finish2 = millis();
    Stop();
    }

  }
  if (incomingByte == 'D')
  {
    roomnumber++;
    starting2 = millis();
    checkingRoom = true;
    Serial.print("We have entered room number: ");
    Serial.print(roomnumber);
    Serial.println(" Location: Right.");
    
    while (checkingRoom == true)
    {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //turning left into the room
    delay(850);
    motors.setSpeeds(100,100); //go into room
    delay(500);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //start searching room. 850 ms
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //start searching room. 600 ms
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //start searching room. 350 ms
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //start searching room. 100 ms
    delay(100);
    CheckIfPerson();
    motors.setSpeeds(-TURN_SPEED,TURN_SPEED); //start searching other side of room 1700
    delay(1000);
    CheckIfPerson();
    motors.setSpeeds(-TURN_SPEED,TURN_SPEED); //start searching other side of room 700
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(-TURN_SPEED,TURN_SPEED); //start searching other side of room 450
    delay(250);
    CheckIfPerson();
    motors.setSpeeds(-TURN_SPEED,TURN_SPEED); //start searching other side of room 200
    delay(200);
    CheckIfPerson();
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //turn back to leave room
    delay(850);
    motors.setSpeeds(-100,-100); //reverse out of room
    delay(500);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //turn back to direction of corridoor
    delay(850);
    if (found > 0)
    {
      Serial.print("We have found an object in room ");
      Serial.println(roomnumber);
      found = 0;
    }
    finish2 = millis();
    Stop();
    }
  }
}

 void automaticControls()
 {
  starting = millis();
  autoMode = true;
  Serial.println("Automatic Control started!");
  while (autoMode == true)
  {
  sensors.read(sensor_values);
  incomingByte = Serial.read();
  if (incomingByte == 'F')
  {
    Stop();
  }
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
    finish = millis();
    delay(300);
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
   finish = millis();
   delay(300);
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
 }
}
 void Stop()
 {
  motors.setSpeeds(0,0);
  checkingRoom = false;
  autoMode = false;
 }

 void CheckIfPerson()
 {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  if(cm < 20)
  {
    found++;
  }
 }
