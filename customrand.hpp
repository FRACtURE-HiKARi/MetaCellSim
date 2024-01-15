#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>

#ifndef __customrand__
#define __customrand__

#define _Accuracy 1000
//#define _DebugMode

namespace TreeSim{
    class RandEngine;
}

class TreeSim::RandEngine{
    public:
        float rate;//This variable is currently useless
        int Generate();
        int Generate(float);
        RandEngine(float);
        RandEngine(unsigned int);
};

/*This overload is currently useless*/
TreeSim::RandEngine::RandEngine(float SPAWN_RATE){
    rate = SPAWN_RATE;
    srand(time(0));
    #ifdef _DebugMode
    printf("RE constructor called & Rate = %.2f & Time = %ld\n", rate, time(0));
    #endif
}

TreeSim::RandEngine::RandEngine(unsigned int seed){
    srand(seed);
    #ifdef _DebugMode
    printf("Overloaded Constructor Called\n");
    #endif
}

/*This overload is currently useless*/
int TreeSim::RandEngine::Generate(){
    float temp;
    temp = rand()%_Accuracy;
    temp /= _Accuracy;
    if(temp < rate)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*Generate 0 or 1 according to input float as rate*/
int TreeSim::RandEngine::Generate(float Rate){
    float temp;
    #ifdef _DebugMode
    printf("%.2f ", Rate);
    #endif
    temp = rand()%_Accuracy;
    temp /= _Accuracy;
    if(temp < Rate)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#endif /*__customrand__*/
