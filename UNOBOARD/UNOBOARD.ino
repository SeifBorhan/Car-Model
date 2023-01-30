#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Arduino_FreeRTOS.h"
#include "semphr.h"
#include "stdio.h"
#include "time.h"
//#include "timer.h"
#include "task.h"



void vmp3Task();
void vheadLights();
void vgearSelect();


int g = 2;
int f = 3;
int a = 4;
int b = 5;
int e = 6;
int d = 7;
int c = 8;
int x_key = A1;                                               
int y_key = A0;  
int x_pos=1;
int y_pos=1;

int t = 1000;
int i = 0;
char  gears [] = {'p','r','n','d'};

TaskHandle_t xmp3TaskHandle = NULL;
TaskHandle_t xheadlightshandle = NULL;
TaskHandle_t xgearselecthandle = NULL;



int LDR = A5;
int led1 = 9;
int led2 = 10;
char command = 'q';
int pause = 0;
SoftwareSerial myserial(11, 12); // RX, TX
SoftwareSerial speaker(A3,A4);

DFRobotDFPlayerMini myDFPlayer;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myserial.begin(115200);
  speaker.begin(9600);
  pinMode(g, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode (x_key, INPUT) ;                     
  pinMode (y_key, INPUT) ;  

  pinMode(A5,INPUT);
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);

digitalWrite(led1,HIGH);
digitalWrite(led2,HIGH);


  

 // myDFPlayer.volume(20); //Volume 5


 Serial.println();
Serial.println(F("DFRobot DFPlayer Mini"));
Serial.println(F("Initializing DFPlayer module ... Wait!"));

if (!myDFPlayer.begin(speaker))
{
Serial.println(F("Not initialized:"));
Serial.println(F("1. Check the DFPlayer Mini connections"));
Serial.println(F("2. Insert an SD card"));
//while (true);
}

Serial.println();
Serial.println(F("DFPlayer Mini module initialized!"));

//Definicoes iniciais
myDFPlayer.setTimeOut(500); //Timeout serial 500ms
myDFPlayer.volume(15); //Volume 5
myDFPlayer.EQ(0); //Equalizacao normal
delay(5000);
xTaskCreate(vheadLights,"vheadlights",128,NULL,2,&xheadlightshandle);
xTaskCreate(vmp3Task,"vmp3Task",128,NULL,1,&xmp3TaskHandle);
xTaskCreate(vgearSelect,"vgearSelect",128,NULL,3,&xgearselecthandle);

vTaskStartScheduler();


}






void vgearSelect()
{
  UBaseType_t uxPriority;
  uxPriority = uxTaskPriorityGet(NULL);

  

  while(1)
  {
    if(analogRead (y_key) < 50)
  {
    if(i == 0)
    {
      
      i = 0;
    }
      
    else if(i==3)
    {
      i=2;
      
    }
     else if(i==2)
    {
      i=1;
      
    }
     else if(i==1)
    {
      i=0;
      
    }
  }

  if(analogRead (y_key) >950)
  {
    if(i == 3)
    {
      
      i = 3;
    }
      
     else if(i==0)
    {
      i=1;
      
    }
     else if(i==1)
    {
      i=2;
      
    }
     else if(i==2)
    {
      i=3;
      
    }
  }



delay(250);


  

   switch(gears[i])
{
  case 'p':
  digitalWrite(g,1);
  digitalWrite(f,1);
  digitalWrite(a,1);
  digitalWrite(b,1);
  digitalWrite(e,1);
  digitalWrite(d,0);
  digitalWrite(c,0);
  break;

    case 'r':
digitalWrite(g,1);
digitalWrite(f,0);
digitalWrite(a,0);
digitalWrite(b,0);
digitalWrite(e,1);
digitalWrite(d,0);
digitalWrite(c,0);

  break;

      case 'n':
digitalWrite(g,1);
digitalWrite(f,0);
digitalWrite(a,0);
digitalWrite(b,0);
digitalWrite(e,1);
digitalWrite(d,0);
digitalWrite(c,1);

  break;

      case 'd':
digitalWrite(g,1);
digitalWrite(f,0);
digitalWrite(a,0);
digitalWrite(b,1);
digitalWrite(e,1);
digitalWrite(d,1);
digitalWrite(c,1);

  break;
  
  
}



  //Serial.println(i);
  
  //delay(2000);
  vTaskPrioritySet(xheadlightshandle,(uxPriority +1));

  }
  
}





void vmp3Task()
{
  Serial.print("mp3 running");
  UBaseType_t uxPriority;
  uxPriority = uxTaskPriorityGet(NULL);
  myserial.listen();
while(1)
{
if(myserial.isListening() && myserial.available())
  {
  delay(200);
  command =   char(myserial.read());
  Serial.println(command);
  }
  
  delay(200);

  if ((command >= '1') && (command <= '9'))
{
Serial.print("Music reproduction");
Serial.println(command);
command = command - 48;
myDFPlayer.play(command);
}

if (command == '+')
{
myDFPlayer.volumeUp();

}

if (command == '<')
{
myDFPlayer.previous();

}

if (command == '>')
{
myDFPlayer.next();

}

//Decreases volume
if (command == '-')
{
myDFPlayer.volumeDown();
}

if (command == 'p')
{
pause = !pause;
if (pause == 0)
{
myDFPlayer.start();
}

if (pause == 1)
{
myDFPlayer.pause();
}

}
command = "f";
 menu_opcoes();
delay(200);
vTaskPrioritySet(NULL,(uxPriority - 3));

}
  
}



void vheadLights()
{
  Serial.println("headlights are working");
  UBaseType_t uxPriority;
  uxPriority = uxTaskPriorityGet(NULL);
  while(1)
  {

  if(analogRead(A5) < 500)
{
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);

}
else
{
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
}
// vTaskDelay(5000 / portTICK_PERIOD_MS);
vTaskPrioritySet(xmp3TaskHandle,(uxPriority));

vTaskPrioritySet(NULL,(uxPriority-2));


  }
}



void menu_opcoes()
{
Serial.println();
Serial.println(F("=================================================================================================================================="));
Serial.println(F("Commands:"));
Serial.println(F(" [1-3] To select the MP3 file"));
Serial.println(F(" [s] stopping reproduction"));
Serial.println(F(" [p] pause/continue music"));
Serial.println(F(" [+ or -] increases or decreases the volume"));
Serial.println(F(" [< or >] forwards or backwards the track"));
Serial.println();
Serial.println(F("================================================================================================================================="));
}

void loop()
{
  return;

}