#ifndef SweepingLight_h
#define SweepingLight_h

#include "Arduino.h"

class SweepingLight
{
    public:
        SweepingLight(int pinNums[], const bool& isCommonCathode);
        ~SweepingLight();
        void init(int startColor[], int endColor[], const unsigned long& startTimeMs, const unsigned long& initDelayTimeMs, const unsigned long& timeLitMs);
        void step(const unsigned long& currentTimeMs);
        void finish();
        bool _lightLoopRunning;        
    private:
        void setColor(int color[]);

        bool _isCommonCathode;                
        int* _pinNums;
        int* _startColor;
        int* _endColor;
        int* _colorDeltas;
        
        unsigned long _startTimeMs;        
        unsigned long _initDelayTimeMs;
        unsigned long _timeLitMs;
};

#endif