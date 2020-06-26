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
        //digitalRead(RESET) == LOW && 
        digitalRead(IR_DETECTOR) == LOW &&
        !sweepingLight1->_lightLoopRunning &&
        !sweepingLight2->_lightLoopRunning)
    {
        int startColor1[3] = {25, 0, 0};
        int endColor1[3] = {0, 0, 25};
        SweepingLightPlotPoint* points1[2];
        points1[0] = new SweepingLightPlotPoint(startColor1, 0);
        points1[1] = new SweepingLightPlotPoint(endColor1, 5000);
        sweepingLight1->init(points1, 2, currentTimeMs, 0);

        int startColor2[3] = {25, 0, 0};
        int endColor2[3] = {0, 0, 25};
        SweepingLightPlotPoint* points2[2];        
        points2[0] = new SweepingLightPlotPoint(startColor2, 0);
        points2[1] = new SweepingLightPlotPoint(endColor2, 5000);
        sweepingLight1->init(points2, 2, currentTimeMs, 1000);

        int startColor3[3] = {25, 0, 0};
        int endColor3[3] = {0, 0, 25};
        SweepingLightPlotPoint* points3[2];        
        points3[0] = new SweepingLightPlotPoint(startColor3, 0);
        points3[1] = new SweepingLightPlotPoint(endColor3, 5000);
        sweepingLight1->init(points3, 2, currentTimeMs, 2000);

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
    Serial.print(" currentTimeMs: ");
    Serial.print(currentTimeMs);
    Serial.print(" _servoStartTimeMs: ");
    Serial.println(_servoStartTimeMs);
    Serial.print(" _servoMoveEndTimeMs: ");
    Serial.println(_servoMoveEndTimeMs);
     Serial.print(" _servoResetTimeMs: ");
    Serial.println(_servoResetTimeMs);
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

    Serial.println(_servoPos);
    servo.write(_servoPos);
}
