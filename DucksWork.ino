/*************************************************************

  Exercise: Blynk + Integromat // DucksWork – an Employer-Employee communication aid.

  Have you ever had a friend or a family member (or even yourselves!) who can get 
  drowned by hybrid work and won’t take breaks because it requires too much hustle 
  by reaching to the boss and hoping he’s available right now?
  THEN THIS IS THE APP FOR YOU!

  In essence, the management tool consists of two parties: 
  the manager and the employee. 
  To make work more efficient, this is a smart and advanced tool 
  that can send automatic messages between two users.
  
  An employee notices a flicker of light when his manager assigns him a task 
  under his responsibility under the task management tool, ClickUp. 
  Then, by another tap after he saw the new task, 
  he will send the task to ClickUp's "in progress" column.
  
  An employee who wishes to go out for a break (such as lunch) clicks on the 
  availability button, which changes to red, and the manager is notified. 
  Managers receive return emails stating that the employee is currently 
  unavailable when trying to assign new tasks.
  Upon returning from lunch, the employee releases his availability, 
  and his manager is notified that he is available for more tasks.

  Created By Inbar Lahat 315976647

*************************************************************/

#define BLYNK_PRINT SerialUSB
// #define BLYNK_DEBUG SerialUSB

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Adafruit_CircuitPlayground.h>

#define BLYNK_TEMPLATE_ID "TMPLi6h_Q5cp"
#define BLYNK_DEVICE_NAME "DucksWork"
#define BLYNK_AUTH_TOKEN "JB_cwtLNBVxduTbLfPZr8G64zLDr6ZRf"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Omer";
char pass[] = "hashla109";

#define EspSerial Serial1
#define ESP8266_BAUD 115200
ESP8266 wifi(&EspSerial);

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(*arr))

// modes
bool isOnABreak = false;
bool newTask = false;
bool approveButton = false;

int buttons[] = {V1,V2,V3};

// resets all the modes 
void resetAll() 
{
  isOnABreak = false;
  newTask = false;
  approveButton = false;
  CircuitPlayground.clearPixels();
   // Blynk.virtualWrite(V5, 2);
  
  for (int i = 0; i < ARRAY_LENGTH(buttons); i++) 
  {
    Blynk.virtualWrite(buttons[i], 0);
    }
   
   CircuitPlayground.playTone(0, 0);  
}

// check if there is a new task
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  Serial.println(V1);
  if (pinValue == 1) 
  {
    newTask = true;
  }
}

void setup()
{
  CircuitPlayground.begin();
  SerialUSB.begin(115200);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);
}

void loop()
{
  Blynk.run();

  // check modes by importance
  if (newTask) {
    newTaskFunction();
  } 

  // pressed when wish to go on a break
   bool rightButtonBreak = CircuitPlayground.rightButton();
  if (rightButtonBreak){
     isOnABreak = (!isOnABreak);
     isOnABreakFunction();
   }  
    
  // pressed when confirms that the saw new task is incoming
  bool leftButtonPressed = CircuitPlayground.leftButton();
  if (leftButtonPressed) 
  {
    approveButton = true;
    Blynk.virtualWrite(V5, 2);
    delay(2000);
    resetAll();
  } 
} 

// Employee is on a break - Set red color
void isOnABreakFunction() {
    if(isOnABreak)
    {
      Blynk.virtualWrite(V2, 1);
      for (int i = 0; i < 10; i++) 
      {
       CircuitPlayground.setPixelColor(i, 255,   0,   0);
       }
     }
    else { 
      CircuitPlayground.clearPixels();
      Blynk.virtualWrite(V2, 1);
    }
}

// New task arrived - Play sound with pink lights until key is pressed
void newTaskFunction() 
{
  for (int i = 0; i < 10; i++) 
  {
    CircuitPlayground.setPixelColor(i, 255,   0,   200);
    CircuitPlayground.playTone(500, 0);
    }
}
