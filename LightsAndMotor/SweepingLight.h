#ifndef SweepingLight_h
#define SweepingLight_h

#include "Arduino.h"

class SweepingLight
{
    public:
        SweepingLight(int pinNums[]);
        ~SweepingLight();
        void init(int startColor[], int endColor[], int totalTime);
        void step();
        void finish();
        bool _lightLoopRunning;        
    private:
        void SweepingLight::setColor(int color[]);
        int* _pinNums;
        int* _startColor;
        int* _endColor;
        float _totalTime;
        int* _colorDeltas;
        float _delayTime;
        float _elapsedTime;
};

#endif