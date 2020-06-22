#ifndef SweepingLight_h
#define SweepingLight_h

#include "Arduino.h"

class SweepingLight
{
    public:
        SweepingLight(int pinNums[], bool isCommonCathode);
        ~SweepingLight();
        void init(int startColor[], int endColor[], int startTimeMs, int initDelayTimeMs, int timeLitMs);
        void step(int currentTimeMs);
        void finish();
        bool _lightLoopRunning;        
    private:
        void setColor(int color[]);

        bool _isCommonCathode;                
        int* _pinNums;
        int* _startColor;
        int* _endColor;
        int* _colorDeltas;
        
        float _startTimeMs;        
        float _initDelayTimeMs;
        float _timeLitMs;
};

#endif