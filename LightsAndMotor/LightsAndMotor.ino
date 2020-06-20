//www.elegoo.com
//2016.12.8

#include "SweepingLight.h"

// Define Pins
#define RESET 9
#define BLUE 6
#define GREEN 3
#define RED 5

int pinNums1[3];
int startColor1[3];
int endColor1[3];
int colorDeltas1[3];
float delayTime1 = 1;
int totalTime1 = 1;
float elapsedTime1 = 1;
bool lightLoopRunning = false;


SweepingLight* sweepingLight1;
    
void setup()
{
    Serial.begin(9600);
    //RESET switch
    pinMode(RESET, INPUT_PULLUP);

    // led 1
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);

    int pinNum1[3];
    pinNum1[0] = RED;
    pinNum1[1] = GREEN;
    pinNum1[2] = BLUE;
    sweepingLight1 = new SweepingLight(pinNum1);

}

// main loop
void loop()
{
  if (digitalRead(RESET) == LOW && !sweepingLight1->_lightLoopRunning)
  {
      //lightSweepInit(5000);
      startColor1[0] = 0;
    startColor1[1] = 0;
    startColor1[2] = 255;

    endColor1[0] = 255;
    endColor1[1] = 0;
    endColor1[2] = 0;
      sweepingLight1->init(startColor1, endColor1, 5000);
  }

  if(sweepingLight1->_lightLoopRunning)
  {
      
      //lightSweepStep();
      sweepingLight1->step();
  }
}

void lightSweepStep()
{
        int colorToSet[3];
        colorToSet[0] = startColor1[0] + (int)(colorDeltas1[0] * elapsedTime1/totalTime1);
        colorToSet[1] = startColor1[1] + (int)(colorDeltas1[1] * elapsedTime1/totalTime1);
        colorToSet[2] = startColor1[2] + (int)(colorDeltas1[2] * elapsedTime1/totalTime1);
        setColor(pinNums1, colorToSet);
        elapsedTime1 += delayTime1;
        delay(delayTime1);

        if(elapsedTime1 > totalTime1)
        {
            lightSweepEnd();
        }
}

void setColor(int pinNums[], int color[])
{
    analogWrite(pinNums[0], color[0]);
    analogWrite(pinNums[1], color[1]);
    analogWrite(pinNums[2], color[2]);
}

void lightSweepInit(int time)
{
    Serial.println("test");
    lightLoopRunning = true;    
    totalTime1 = time;
    elapsedTime1 = 0.0;

    pinNums1[0] = RED;
    pinNums1[1] = GREEN;
    pinNums1[2] = BLUE;

    startColor1[0] = 0;
    startColor1[1] = 0;
    startColor1[2] = 255;

    endColor1[0] = 255;
    endColor1[1] = 0;
    endColor1[2] = 0;

    colorDeltas1[0] = endColor1[0] - startColor1[0];
    colorDeltas1[1] = endColor1[1] - startColor1[1];
    colorDeltas1[2] = endColor1[2] - startColor1[2];
    int maxDelta = max(max(abs(endColor1[0] - startColor1[0]),
                           abs(endColor1[1] - startColor1[1])),
                           abs(endColor1[2] - startColor1[2]));
    // To stop an infinite loop
    maxDelta = max(maxDelta, 1); 
    
    delayTime1 = totalTime1/maxDelta;

    setColor(pinNums1, startColor1);
} 

void lightSweepEnd()
{
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);
    lightLoopRunning = false;
}
