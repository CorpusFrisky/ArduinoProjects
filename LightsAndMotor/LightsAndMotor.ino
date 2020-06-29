//www.elegoo.com
//2016.12.8

#include "SweepingLight.h"
#include <Servo.h>


// Define Pins
#define IR_DETECTOR A0
#define RESET 12
#define SERVO 13

#define RED1 4
#define GREEN1 3
#define BLUE1 2

#define RED2 7
#define GREEN2 6
#define BLUE2 5

#define RED3 10
#define GREEN3 9
#define BLUE3 8

SweepingLight* sweepingLight1;
SweepingLight* sweepingLight2;
SweepingLight* sweepingLight3;
Servo servo;

int _servoPos = 0;
unsigned long _servoStartTimeMs = 0;
unsigned long _servoMoveEndTimeMs = 0;
unsigned long _servoMoveTimeMs = 0;
unsigned long _servoResetTimeMs = 0;
unsigned long _servoResetEndTimeMs = 0;
bool _isServoMoving = false;
    
void setup()
{
    Serial.begin(115200);
    //RESET switch
    pinMode(RESET, INPUT_PULLUP);

    //IR_DETECTOR
    pinMode(IR_DETECTOR, INPUT);

    // led 1
    int pinNum1[3] = {RED1, GREEN1, BLUE1};
    sweepingLight1 = new SweepingLight(pinNum1, true);

    // led 2
    int pinNum2[3] = {RED2, GREEN2, BLUE2};
    sweepingLight2 = new SweepingLight(pinNum2, true);

    // led 3
    int pinNum3[3] = {RED3, GREEN3, BLUE3};
    sweepingLight3 = new SweepingLight(pinNum3, true);

    servo.attach(SERVO);
    servo.write(_servoPos);
}

// main loop
void loop()
{
    unsigned long currentTimeMs = millis();
    //Serial.println(currentTime);

    if (
        digitalRead(RESET) == LOW && 
        //digitalRead(IR_DETECTOR) == LOW &&
        !sweepingLight1->_lightLoopRunning)
    {
        Serial.println("Sweep starting");
        int color1[3] = {25, 0, 0};
        int color2[3] = {0, 0, 25};
        int color3[3] = {0, 25, 0};
        SweepingLightPlotPoint* points1[2];
        points1[0] = new SweepingLightPlotPoint(color1, 0);
        points1[1] = new SweepingLightPlotPoint(color2, 5000);
        points1[2] = new SweepingLightPlotPoint(color3, 7000);
        sweepingLight1->init(points1, 3, currentTimeMs, 0);

        servoInit(currentTimeMs, 7000, 2000);
    }

    if(sweepingLight1->_lightLoopRunning)
    {
        sweepingLight1->step(currentTimeMs);
    }

    if(sweepingLight2->_lightLoopRunning)
    {
        sweepingLight2->step(currentTimeMs);
    }

    if(sweepingLight3->_lightLoopRunning)
    {
        sweepingLight3->step(currentTimeMs);
    }

    if(_isServoMoving)
    {
        servoStep(currentTimeMs);
    }
}

void servoInit(unsigned long currentTimeMs, unsigned long timeToMove, unsigned long servoResetTimeMs)
{
    _servoStartTimeMs = currentTimeMs;
    _servoMoveEndTimeMs = currentTimeMs + timeToMove;
    _servoMoveTimeMs = timeToMove;
    _servoResetTimeMs = servoResetTimeMs;
    _servoResetEndTimeMs = _servoMoveEndTimeMs + _servoResetTimeMs;
    _isServoMoving = true;
}

void servoStep(unsigned long currentTimeMs)
{
    if(currentTimeMs > _servoResetEndTimeMs)
    {
        servo.write(0);
        _isServoMoving = false;
        return;
    }

    if(currentTimeMs < _servoMoveEndTimeMs)
    {
        _servoPos = (int)(180.0 * (float(currentTimeMs) - float(_servoStartTimeMs)) / _servoMoveTimeMs);
    }
    else
    {
        _servoPos = 180 - (int)(180.0 * (float(currentTimeMs) - float(_servoMoveEndTimeMs)) / _servoResetTimeMs);
    }

    servo.write(_servoPos);
}
