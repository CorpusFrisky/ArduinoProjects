#include "Arduino.h"
#include "SweepingLight.h"

SweepingLight::SweepingLight(int pinNums[], bool commonCathode)
{
    Serial.println("constucting");
    _pinNums = new int[3];
    _startColor = new int[3];
    _endColor = new int[3];
    _colorDeltas = new int[3];
    _lightLoopRunning = false;
    _isCommonCathode = commonCathode;

    memcpy(_pinNums, pinNums, 3*sizeof(int));

    pinMode(_pinNums[0], OUTPUT);
    pinMode(_pinNums[1], OUTPUT);
    pinMode(_pinNums[2], OUTPUT);
    digitalWrite(_pinNums[0], HIGH);
    digitalWrite(_pinNums[1], HIGH);
    digitalWrite(_pinNums[2], HIGH);
}

SweepingLight::~SweepingLight()
{
    delete _pinNums;
    delete _startColor;
    delete _endColor;
    delete _colorDeltas;
}

void SweepingLight::init(int startColor[], int endColor[], int startTimeMs, int initDelayTimeMs, int timeLitMs)
{
    _lightLoopRunning = true;    

    _startTimeMs = startTimeMs;
    _initDelayTimeMs = initDelayTimeMs;
    _timeLitMs = timeLitMs;

    if(_isCommonCathode)
    {
        for(int i=0; i<3; i++)
        {
            _startColor[0] = 255 - startColor[0];
            _startColor[1] = 255 - startColor[1];
            _startColor[2] = 255 - startColor[2];
            _endColor[0] = 255 - endColor[0];
            _endColor[1] = 255 - endColor[1];
            _endColor[2] = 255 - endColor[2];
        }
    }
    else
    {
        memcpy(_startColor, startColor, 3*sizeof(int));
        memcpy(_endColor, endColor, 3*sizeof(int));
    }

    _colorDeltas[0] = _endColor[0] - _startColor[0];
    _colorDeltas[1] = _endColor[1] - _startColor[1];
    _colorDeltas[2] = _endColor[2] - _startColor[2];
}
 
void SweepingLight::step(int currentTimeMs)
{
    if(currentTimeMs < _startTimeMs + _initDelayTimeMs)
    {
        return;
    }

    if(currentTimeMs > _startTimeMs + _initDelayTimeMs + _timeLitMs)
    {
        finish();
        return;
    }

    int elapsedTimeLitMs = currentTimeMs - (_startTimeMs + _initDelayTimeMs);

    int colorToSet[3];
    float ratio = elapsedTimeLitMs/_timeLitMs;
    colorToSet[0] = _startColor[0] + (int)(_colorDeltas[0] * ratio);
    colorToSet[1] = _startColor[1] + (int)(_colorDeltas[1] * ratio);
    colorToSet[2] = _startColor[2] + (int)(_colorDeltas[2] * ratio);
    setColor(colorToSet);
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