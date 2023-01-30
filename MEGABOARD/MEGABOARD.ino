#include <Wire.h>

int proximity = 0;
int sensorRight = 22;
int sensorLeft = 52;

int speedA = 75;
int speedB = 75;


//Motors AB Right side

int enA = 2;
int in1 = 3;
int in2 = 4;

//Motors CD Left side

int enB = 5;
int in3 = 6;
int in4 = 7;

int redLed = A0;



void setup() {
  // put your setup code here, to run once:

Wire.begin(9);
Serial.begin(9600);
pinMode(enA, OUTPUT);
pinMode(enB, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
pinMode(sensorRight, INPUT);
pinMode(sensorLeft, INPUT);
pinMode(redLed,OUTPUT);
//Wire.onRecieve(recieveEvent);


}  


void loop() {
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  if(digitalRead(sensorRight) == 1)
{
  digitalWrite(redLed,HIGH);
  Serial.println("right very close");
  analogWrite(enB, 0);
  analogWrite(enA,speedA*1.25);
  proximity = 1;
  delay(300);
}
else


  {
    if(digitalRead(sensorLeft) == 1)
{
  digitalWrite(redLed,HIGH);
  Serial.println("left very close");
  analogWrite(enA, 0);
  analogWrite(enB,speedB*1.25);
  proximity = -1;
  delay(300);
}



else 
  digitalWrite(redLed,LOW);
  proximity = 0;
  analogWrite(enA, speedA);
  analogWrite(enB, speedB);
  }


}