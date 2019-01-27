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
unsigned long starting, finish, elapsed, starting2, finish2, elapsed2, timeInCorridor, timer1, timer2, finaltimer;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
void manualControls() //manual control of the zumo
 {
    Serial.println("Manual control has been assigned!");
    while(autoMode == false)
    {
  incomingByte = Serial.read(); //reads inputs from the processing application
  if (incomingByte == 'R') //checks the right room
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
    delay(800);
    motors.setSpeeds(100,100); //go into room
    delay(500);
      for (int i = 0; i < 8; i++)
      {
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //start searching room. 800 ms
      delay(100);
      CheckIfPerson();
      }
      for (int i = 0; i < 16; i++)
      {
      motors.setSpeeds(-TURN_SPEED,TURN_SPEED); //start searching other side of room 1600
      delay(100);
      CheckIfPerson();
      }
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //turn back to leave room
      delay(850);
      motors.setSpeeds(-100,-100); //reverse out of room
      delay(500);
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //turn back to direction of corridoor
      delay(800);
      if (found > 0) //if the found value is higher than 0 that means an object has been found
      {
        Serial.print("We have found an object in room "); 
        Serial.println(roomnumber);
        found = 0;
      }
      else
      {
        Serial.print("We have not found an object in room ");
        Serial.println(roomnumber);
      }
    finish2 = millis();
    Stop();
    }
  }
  if (incomingByte == 'L')
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
    delay(800);
    motors.setSpeeds(100,100); //go into room
    delay(500);
      for (int i = 0; i < 8; i++)
      {
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //start searching room. 800 ms
      delay(100);
      CheckIfPerson();
      }
      for (int i = 0; i < 16; i++)
      {
      motors.setSpeeds(TURN_SPEED,-TURN_SPEED); //start searching other side of room 1700
      delay(100);
      CheckIfPerson();
      }
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //turn back to leave room
      delay(850);
      motors.setSpeeds(-100,-100); //reverse out of room
      delay(500);
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //turn back to direction of corridoor
      delay(800);
      if (found > 0)
      {
        Serial.print("We have found an object in room ");
        Serial.println(roomnumber);
        found = 0;
      }
      else
      {
        Serial.print("We have not found an object in room ");
        Serial.println(roomnumber);
      }
    finish2 = millis();
    Stop();
    }

  }
  
    if (incomingByte == 'W') { //forward
     motors.setSpeeds(100, 100);
    }
    if (incomingByte == 'S') { //backwards
      motors.setSpeeds(-100, -100);
    }
     if (incomingByte == 'F') { //stop
      motors.setSpeeds(0, 0);
    }
    if (incomingByte == 'E') // end of 1 direction of T junction
    {
      timer1 = millis();
      elapsed = finish - starting;
      elapsed2 = finish2 - starting2;
      timeInCorridor = elapsed - elapsed2;
      finaltimer = timer1 + timeInCorridor; //setting so the timer starts from 0
      Serial.println("returning from end of T junction");
      do
      {
        sensors.read(sensor_values); //reading values from sensor
        if (sensor_values[0] > QTR_THRESHOLD) //if the left most sensor detects a black line
       {
         motors.setSpeeds(100,100);
         delay(100);
         sensors.read(sensor_values);
         Serial.println("Checking for wall!");
        if(sensor_values[5] > QTR_THRESHOLD) //if the right most sensor detects a black line 
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
        else //now it knows that it has hit a wall in a corridoor and reverses to adjust
        {
          motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
          delay(REVERSE_DURATION);
          motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
          delay(TURN_DURATION);
          motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        }
       }
        else if (sensor_values[5] > QTR_THRESHOLD) //right most sensor detects black line
         {
         motors.setSpeeds(100,100);
         delay(100);
         sensors.read(sensor_values);
         Serial.println("Checking for wall!");
         if(sensor_values[0] > QTR_THRESHOLD) //left most sensor detects black line
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
        else   //now it knows that it has hit a wall in a corridoor and reverses to adjust
        {
          //if rightmost sensor detects line, reverse and turn to the left
          motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
          delay(REVERSE_DURATION);
          motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
          delay(TURN_DURATION);
          motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        }
       }
        else //if no black lines detected, just go straight
        {
          motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        }
      }while (millis() < finaltimer); //timer that makes sure no commands can be accepted until the junction has been reached
      Serial.println("Junction reached");
      automaticControls();
    }
     if (incomingByte == 'A') { //left
    motors.setSpeeds(-100, 100);
    delay(100);
    }
     if (incomingByte == 'D') { //right
    motors.setSpeeds(100, -100);
    delay(100);
     }
    if (incomingByte == 'C') //continue
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
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15); //starting noise
  delay(1000);
}

void setup()
{
  pinMode(LED, HIGH);
  Serial.begin(9600); //start reading inputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  waitForButtonAndCountDown(); //waits for button to be pressed before starting 
}

void loop() //constant loop
{
  incomingByte = Serial.read();
  if (autoMode == true) //checks if automode is true
  {
    automaticControls();
  }
  if (incomingByte == 'F') //stop 
  {
    Stop();
  }
  if (incomingByte == 'C') //continue
  {
    //autoMode = true;
    automaticControls();
  }
  if (incomingByte == 'L') //checks left room
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
    delay(800);
    motors.setSpeeds(100,100); //go into room
    delay(500);
      for (int i = 0; i < 8; i++)
      {
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //start searching room. 800 ms
      delay(100);
      CheckIfPerson();
      }
      for (int i = 0; i < 16; i++)
      {
      motors.setSpeeds(TURN_SPEED,-TURN_SPEED); //start searching other side of room 1700
      delay(100);
      CheckIfPerson();
      }
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //turn back to leave room
      delay(850);
      motors.setSpeeds(-100,-100); //reverse out of room
      delay(500);
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //turn back to direction of corridoor
      delay(800);
      if (found > 0)
      {
        Serial.print("We have found an object in room ");
        Serial.println(roomnumber);
        found = 0;
      }
      else
      {
        Serial.print("We have not found an object in room ");
        Serial.println(roomnumber);
      }
    finish2 = millis();
    Stop();
    }

  }
  if (incomingByte == 'R')//checks right room 
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
    delay(800);
    motors.setSpeeds(100,100); //go into room
    delay(500);
      for (int i = 0; i < 8; i++)
      {
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //start searching room. 800 ms
      delay(100);
      CheckIfPerson();
      }
      for (int i = 0; i < 16; i++)
      {
      motors.setSpeeds(-TURN_SPEED,TURN_SPEED); //start searching other side of room 1600
      delay(100);
      CheckIfPerson();
      }
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED); //turn back to leave room
    delay(850);
    motors.setSpeeds(-100,-100); //reverse out of room
    delay(500);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED); //turn back to direction of corridoor
    delay(800);
      if (found > 0)
      {
        Serial.print("We have found an object in room ");
        Serial.println(roomnumber);
        found = 0;
      }
      else
      {
        Serial.print("We have not found an object in room ");
        Serial.println(roomnumber);
      }
    finish2 = millis();
    Stop();
    }
  }
}

 void automaticControls() //auto mode
 {
  starting = millis(); //starts counting from now
  autoMode = true; //enables auto mode
  Serial.println("Automatic Control started!");
  while (autoMode == true)
  {
  sensors.read(sensor_values); //read sensor values
  incomingByte = Serial.read(); //read input from processing GUI
  if (incomingByte == 'F') //stop if F is pressed
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
 void Stop() //stops the arduino
 {
  motors.setSpeeds(0,0);  
  checkingRoom = false;
  autoMode = false;
 }

 void CheckIfPerson() //uses sensor to check if something is found
 {
  for(int i = 0; i < 3;i ++){ //repeats 3 times to make sure that sensor is correct 
  digitalWrite(trigPin, LOW); //sends a low pulse before a high pulse to give a more accurate reading
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH); //stores the distance of the closest object  into duration
  cm = (duration/2) / 29.1;
  if(cm < 30) //if it is closer than 30cm it has found an object
  {
    found++; //stores a value in found variable.
  }
  }
 }
