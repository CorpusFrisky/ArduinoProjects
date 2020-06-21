//www.elegoo.com
//2016.12.8

#include "SweepingLight.h"

// Define Pins
#define RESET 9
#define BLUE1 6
#define GREEN1 3
#define RED1 5

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
    int pinNum1[3] = {RED1, GREEN1, BLUE1};
    sweepingLight1 = new SweepingLight(pinNum1);

}

// main loop
void loop()
{
    if (digitalRead(RESET) == LOW && !sweepingLight1->_lightLoopRunning)
    {
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
        sweepingLight1->step();
    }
}
