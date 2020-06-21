//www.elegoo.com
//2016.12.8

#include "SweepingLight.h"

// Define Pins
#define RESET 9

#define RED1 4
#define GREEN1 3
#define BLUE1 2

#define RED2 7
#define GREEN2 6
#define BLUE2 5

SweepingLight* sweepingLight1;
SweepingLight* sweepingLight2;
    
void setup()
{
    Serial.begin(9600);
    //RESET switch
    pinMode(RESET, INPUT_PULLUP);

    // led 1
    int pinNum1[3] = {RED1, GREEN1, BLUE1};
    sweepingLight1 = new SweepingLight(pinNum1, true);

    // led 2
    int pinNum2[3] = {RED2, GREEN2, BLUE2};
    sweepingLight2 = new SweepingLight(pinNum2, true);
}

// main loop
void loop()
{
    if (digitalRead(RESET) == LOW && 
        !sweepingLight1->_lightLoopRunning &&
        !sweepingLight2->_lightLoopRunning)
    {
        int startColor1[3] = {25, 0, 0};
        int endColor1[3] = {0, 25, 0};
        sweepingLight1->init(startColor1, endColor1, 5000);

        int startColor2[3] = {0, 25, 0};
        int endColor2[3] = {0, 0, 25};
        sweepingLight2->init(startColor2, endColor2, 5000);
    }

    if(sweepingLight1->_lightLoopRunning)
    {
        sweepingLight1->step();
    }

    if(sweepingLight2->_lightLoopRunning)
    {
        sweepingLight2->step();
    }
}
