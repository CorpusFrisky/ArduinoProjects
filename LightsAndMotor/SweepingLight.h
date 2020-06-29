#ifndef SweepingLight_h
#define SweepingLight_h

#include "Arduino.h"

struct SweepingLightPlotPoint
{
    public:
        SweepingLightPlotPoint();    
        SweepingLightPlotPoint(int color[], unsigned long offset)
        {
            memcpy(_color, color, 3*sizeof(int));
            _offset = offset;
        }

        int _color[3];
        unsigned long _offset;
};

class SweepingLight
{
    public:
        SweepingLight(int pinNums[], const bool& isCommonCathode);
        ~SweepingLight();
        void init(SweepingLightPlotPoint* plotPointArray[], int plotPointArraySize, const unsigned long& startTimeMs, const unsigned long& initDelayTimeMs);
        void step(const unsigned long& currentTimeMs);
        void finish();
        bool _lightLoopRunning;        
    private:
        void setColor(int color[]);
        void setDeltasAndPlotLineLengthFromStartPlotPointIndex(int index);

        SweepingLightPlotPoint** _plotPoints;
        int _numPlotPoints;
        int _currentStartPlotPointIndex;

        bool _isCommonCathode;                
        int* _pinNums;
        int* _colorDeltas;
        
        unsigned long _startTimeMs;        
        unsigned long _initDelayTimeMs;
        unsigned long _lengthOfCurrentPlotLineMs;
        unsigned long _endTimeOfCurrentPlotLineMs;
};

#endif