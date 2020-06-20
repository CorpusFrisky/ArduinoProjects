#include "Arduino.h"
#include "SweepingLight.h"

SweepingLight::SweepingLight(int pinNums[])
{
    _pinNums = new int[3];
    _startColor = new int[3];
    _endColor = new int[3];
    _colorDeltas = new int[3];
    _lightLoopRunning = false;

    _pinNums[0] = pinNums[0];
    _pinNums[1] = pinNums[1];
    _pinNums[2] = pinNums[2];
}

SweepingLight::~SweepingLight()
{
    delete _pinNums;
    delete _startColor;
    delete _endColor;
    delete _colorDeltas;
}

 void SweepingLight::init(int startColor[], int endColor[], int totalTime)
 {
     Serial.println("init");
    _lightLoopRunning = true;    
    _totalTime = totalTime;
    _elapsedTime = 0.0;

    _startColor[0] = startColor[0];
    _startColor[1] = startColor[1];
    _startColor[2] = startColor[2];
    _endColor[0] = endColor[0];
    _endColor[1] = endColor[1];
    _endColor[2] = endColor[2];

    _colorDeltas[0] = _endColor[0] - _startColor[0];
    _colorDeltas[1] = _endColor[1] - _startColor[1];
    _colorDeltas[2] = _endColor[2] - _startColor[2];
    int maxDelta = max(max(abs(_endColor[0] - _startColor[0]),
                           abs(_endColor[1] - _startColor[1])),
                           abs(_endColor[2] - _startColor[2]));
    // To stop an infinite loop
    maxDelta = max(maxDelta, 1); 
    
    _delayTime = _totalTime/maxDelta;

    setColor(_startColor);
 }
 
 void SweepingLight::step()
 {
    int colorToSet[3];
    float ratio = _elapsedTime/_totalTime;
    colorToSet[0] = _startColor[0] + (int)(_colorDeltas[0] * ratio);
    colorToSet[1] = _startColor[1] + (int)(_colorDeltas[1] * ratio);
    colorToSet[2] = _startColor[2] + (int)(_colorDeltas[2] * ratio);
    setColor(colorToSet);
    _elapsedTime += _delayTime;
    delay(_delayTime);

    if(_elapsedTime > _totalTime)
    {
        finish();
    }
 }

 void SweepingLight::finish()
 {
    digitalWrite(_pinNums[0], HIGH);
    digitalWrite(_pinNums[1], HIGH);
    digitalWrite(_pinNums[2], HIGH);
    _lightLoopRunning = false;
 }

void SweepingLight::setColor(int color[])
{
    analogWrite(_pinNums[0], color[0]);
    analogWrite(_pinNums[1], color[1]);
    analogWrite(_pinNums[2], color[2]);
}