#include "Arduino.h"
#include "SweepingLight.h"

SweepingLight::SweepingLight(int pinNums[], const bool& commonCathode)
{
    _pinNums = new int[3];
    _currentStartPlotPointIndex = 0;
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
    delete _colorDeltas;
    if(_plotPoints)
    {
        delete _plotPoints;
    }
}

void SweepingLight::init(SweepingLightPlotPoint* plotPointArray[], int plotPointArraySize, const unsigned long& startTimeMs, const unsigned long& initDelayTimeMs)
{
    if(plotPointArraySize < 2)
    {
        return;
    }

    _lightLoopRunning = true;    

    memcpy(_plotPoints, plotPointArray, plotPointArraySize * sizeof(SweepingLightPlotPoint*));
    _numPlotPoints = plotPointArraySize;
    _currentStartPlotPointIndex = 0;

    _startTimeMs = startTimeMs;
    _initDelayTimeMs = initDelayTimeMs;

    if(_isCommonCathode)
    {
        for(int i=0; i<plotPointArraySize; i++)
        {
            for(int j=0; j<3; j++)
            {
                _plotPoints[i]->_color[0] = 255 -  _plotPoints[i]->_color[0];
                _plotPoints[i]->_color[1] = 255 -  _plotPoints[i]->_color[1];
                _plotPoints[i]->_color[2] = 255 -  _plotPoints[i]->_color[2];
            }
        }
    }

    setDeltasAndPlotLineLengthFromStartPlotPointIndex(_currentStartPlotPointIndex);
}

void SweepingLight::setDeltasAndPlotLineLengthFromStartPlotPointIndex(int index)
{
    _lengthOfCurrentPlotLine = _plotPoints[index+1]->_offset - _plotPoints[index]->_offset;
    _colorDeltas[0] = _plotPoints[index+1]->_color[0] - _plotPoints[index]->_color[0];
    _colorDeltas[1] = _plotPoints[index+1]->_color[1] - _plotPoints[index]->_color[1];
    _colorDeltas[2] = _plotPoints[index+1]->_color[2] - _plotPoints[index]->_color[2];
}
 
void SweepingLight::step(const unsigned long& currentTimeMs)
{
    if(currentTimeMs < _startTimeMs + _initDelayTimeMs)
    {
        return;
    }

    unsigned long plotEndTimeMs = _startTimeMs + _initDelayTimeMs + _lengthOfCurrentPlotLine;
    if(currentTimeMs > plotEndTimeMs)
    {
        _currentStartPlotPointIndex++;
        if(_currentStartPlotPointIndex < _numPlotPoints)
        {
            setDeltasAndPlotLineLengthFromStartPlotPointIndex(_currentStartPlotPointIndex);
        }
        else
        {
            finish();
            return;
        }
    }

    unsigned long elapsedTimeOnPlotLineMs = currentTimeMs - (_startTimeMs + _plotPoints[_currentStartPlotPointIndex]->_offset);

    int colorToSet[3];
    float ratio = float(elapsedTimeOnPlotLineMs)/float(_lengthOfCurrentPlotLine);

    // Serial.print(_startColor[0]);
    // Serial.print(" --- ");
    // Serial.print("ratio: ");
    // Serial.print(ratio);
    // Serial.print(" elapsed: ");
    // Serial.print(elapsedTimeLitMs);
    // Serial.print(" timeLit: ");
    // Serial.println(_timeLitMs);

    colorToSet[0] = _plotPoints[_currentStartPlotPointIndex]->_color[0] + (int)(_colorDeltas[0] * ratio);
    colorToSet[1] = _plotPoints[_currentStartPlotPointIndex]->_color[1] + (int)(_colorDeltas[1] * ratio);
    colorToSet[2] = _plotPoints[_currentStartPlotPointIndex]->_color[2] + (int)(_colorDeltas[2] * ratio);
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