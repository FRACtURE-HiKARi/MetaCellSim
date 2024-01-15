/*if you are using windows, uncomment this line*/
//#define _WindowsMode

#include "99f.hpp"
#include <cstdio>
#include <unistd.h>

#define _ManualWind

int TreeSim::TreeCell::TreeCount = 0;
TreeSim::Terrarian terrarian;
TreeSim::Wind wind(0, 1);//currently useless
unsigned int MaxTerm = 200;

void Init(){
    TreeSim::SpawnRate = 0.01;
#ifdef _ManualWind
    TreeSim::AffectRate[POS_L] = 0.95;
    TreeSim::AffectRate[POS_R] = 0.1;
    TreeSim::AffectRate[POS_T] = 0.3;
    TreeSim::AffectRate[POS_B] = 0.3;
#else
    wind.SetField(TreeSim::AffectRate);
#endif
    TreeSim::Start_Pos_X = 8;
    TreeSim::Start_Pos_Y = 0;
    TreeSim::UpdateDelay = 100;
    terrarian.Init();
}

void Update(){
    terrarian.Update();
    terrarian.PrintTerra();
#ifndef _ManualWind
    printf("(%.2f, %.2f)\n", wind.x, wind.y);
#endif
    for(int i = 0; i < sizeof(TreeSim::AffectRate)/sizeof(TreeSim::AffectRate[0]); i++){
        printf("%.2f ", TreeSim::AffectRate[i]);
    }
    printf("\n");
    printf("Seed: %u\n", TreeSim::seed);
#ifdef _UseCustomDelay
    Delay(TreeSim::UpdateDelay);
#else
    usleep(TreeSim::UpdateDelay * 1000);
#endif
}

int main(int argc, char* argv[]){
    Init();
    while(terrarian.term < MaxTerm){
        Update();
        if(terrarian.burn_count == 0)
            break;
    }
    return 0;
}