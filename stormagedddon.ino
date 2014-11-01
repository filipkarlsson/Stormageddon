/*
Stormageddon - Dark Lord of All

Sorce code for sumo robot using the Arduino Pro Mini ATmega328. 

Copyright Filip Karlsson, Axel Tdierhorn 2014.

The code is avalible under the GNU Public License version 3. 
*/

#include <SoftwareSerial.h>
#include <PololuQik.h>
#include <QTRSensors.h>
 
//Edge sensors------------
#define NUM_SENSORS 4 // number of sensors used
#define TIMEOUT 1500 // waits for 1500 microseconds for sensor outputs to go low
#define EMITTER_PIN QTR_NO_EMITTER_PIN // emitter is controlled by digital pin 2

QTRSensorsRC qtrrc((unsigned char[]) {6, 7, 8, 9},
NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
boolean out_front_right = false;
boolean out_front_left = false;
boolean out_back_right = false;
boolean out_back_left = false;
//-----------------------

PololuQik2s9v1 qik(17, 2, 16); //create object for motor driver lib. 

int start_module_pin = 10;

int M1Speed; //holds speed for motors 
int M2Speed;

//constants for predfiend motorspeeds (-127 to 127) 
int backward = 30;
int fast_backward = 60;
int forward = -30;
int fast_forward = -60; 

int back_delay = 350; //Period of time the robot will reverse after crossing edge of dhoyo

void front_check(); //checks if the front of robot is over the edge of the dhoyo
void back_check(); //checks if the back of robot is over the edge of the dhoyo

// Sensor pins 
int front_right= 11;
int front_mid  = 12;
int front_left = 13;

int back_right = 5;
int back_mid   = 4;
int back_left  = 3; 


void setup()
{
  Serial.begin(115200);
  
  qik.init();

  //set start module pin to input
  pinMode(start_module_pin, INPUT);
  
  //set sensor pins to input
  pinMode(front_right, INPUT);
  pinMode(front_mid, INPUT);
  pinMode(front_left, INPUT);

  pinMode(back_right, INPUT);
  pinMode(back_mid, INPUT);
  pinMode(back_left, INPUT);


  
}

void loop()
{

  while(!digitalRead(start_module_pin)){ //wait for start signal

    delay(10); 
  }


   while(digitalRead(start_module_pin)){ // main loop, go as long as start signal is active. 
   
   if(!digitalRead(front_mid)){ //senor pins go low in case of detection
    M1Speed = forward;
    M2Speed = forward;

   }

   else if(!digitalRead(front_right)){
    M1Speed = 0;
    M2Speed = forward;
   }

    else if(!digitalRead(front_left)){
    M1Speed = forward;
    M2Speed = 0;
   }


   else if(!digitalRead(back_mid)){
    M1Speed = backward;
    M2Speed = backward;
   }

   else if(!digitalRead(back_right)){
    M1Speed = 0;
    M2Speed = backward;
   }

   else if(!digitalRead(back_left)){
    M1Speed = backward;
    M2Speed = 0;
   }

   
   else{
    M1Speed = forward;
    M2Speed = backward;
   }
   

   qik.setM0Speed(M1Speed);
   qik.setM1Speed(M2Speed);

   front_check();
   back_check();

   /*qtrrc.read(sensorValues);

  Serial.print(sensorValues[0]);
  Serial.print("\t");
  Serial.print(sensorValues[1]);
  Serial.print("\t");
  Serial.print(sensorValues[2]);
  Serial.print("\t");
  Serial.print(sensorValues[3]);
  Serial.print("\n");
  
  delay(1000);*/

  }

}


void front_check(){
  //check for white line in front
  qtrrc.read(sensorValues);
  if(sensorValues[0] < 800){ //if over the line front-left go back for 350 ms
    qik.setM0Speed(backward);
    qik.setM1Speed(fast_backward);

    delay(back_delay);
  }


  else if(sensorValues[1] < 800){
   qik.setM0Speed(fast_backward);
   qik.setM1Speed(backward);

   delay(back_delay);

  }

return;
}

void back_check(){
  //check for white line in front
  qtrrc.read(sensorValues);
  if(sensorValues[2] < 800){
    qik.setM0Speed(forward);
    qik.setM1Speed(fast_forward);

    delay(back_delay);

  }


  else if(sensorValues[3] < 800){
    qik.setM0Speed(fast_forward);
    qik.setM1Speed(forward);

    delay(back_delay);

  }

return;
}