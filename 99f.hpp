/*Some methods, declaration and definition of tree simulator*/

#include <cstdio>
#include <ctime>
#include <cmath>
#include "99f_bits.h"
#include "99f_colors.h"
#include "customrand.hpp"

#ifndef __99f_hpp__
#define __99f_hpp__

/*Uncomment following lines to enable corresponding modes*/

//#define _DebugMode
//#define _Diagonal
//#define _UseCustomDelay

/*if you want to make it randomly generate fire, comment this line*/
#define _ManualStart

#define Size_X 21
#define Size_Y 21

#ifdef _Diagonal
#define _NeighborSize 8
#else
#define _NeighborSize 4
#endif

/*Declaration*/
namespace TreeSim{
    class TreeCell;
    class Terrarian;
    class Wind;                         //currently useless
    int UpdateDelay = 1000;             //delay time, in ms
    int MaxiumBurnDuration = 3;
    int MinimumBurnDuration = 1;
    float SpawnRate = 0.1;              //rate of trees to respawn
    float ThunderRate = 0.01;           //rate of having thunder
    float AffectRate[_NeighborSize];    //rate of catching fire from different directions
    unsigned int seed = time(0);        //seed of random
    TreeSim::RandEngine DefaultRand(seed);
    #ifdef _ManualStart
    unsigned int Start_Pos_X = 0;       //start positions
    unsigned int Start_Pos_Y = 0;
    #endif
    #ifdef _UseCustomDelay
    void Delay(clock_t);
    #endif
};

/*Definition*/
class TreeSim::TreeCell{
    public:
        int posx, posy;                         //position in the terrarian
        int neighbor_status;                    //a varibale to determine whether neighbors are burning
        int burn_stage;                         //to save how many terms it has been burning
        int max_burn;                           //maxium burning terms
        /* See also "99f_bits.h" */
        unsigned short pos, status, testarr;    //in these unsigned short, bits with 1 are labelled.
        static int TreeCount;                   //total counter of how many cells generated
        Terrarian* father_terrarian;            //a pointer to the terrarian
        void Update();                          //method to update its status
        void GetTestArr();                      //border detection, determine which other cells' status to test
        void GetNeighborStatus();               //get neighbor's status (burning) according to testarr
        void Merge();                           //let the cell "[]" appear on screen
        void StartBurning();                    //set status to burning and randomize a maximum burning stage
        TreeCell( void );
};

class TreeSim::Terrarian{
    public:
        TreeSim::TreeCell Tarray[Size_Y][Size_X];   //array of class: TreeCell
        unsigned short PrevStatus[Size_Y][Size_X];  //to save the status in each stage
        unsigned int term, burn_count;              //to save how many terms has gone, and how many trees are burning
        void Update();                              //update terrarian
        void PrintTerra();                          //print terrarian to screen
        void Init();                                //initialize terrarian
};

/*This class is not done yet*/
class TreeSim::Wind{
    public:
        float mod, x, y, scale, normalize_factor;
        int rect;
        Wind(float, float);
        void SetField(float*);
        float distance(float, float);
};

/*set pisition according to the count*/
TreeSim::TreeCell::TreeCell(void){
    posx = TreeCount % Size_X;
    posy = TreeCount / Size_X;
    pos = 0;
    if(posx == 0)
    {
        pos |= PositionLeft;
    }
    else if (posx == Size_X - 1)
    {
        pos |= PositionRight;
    }
    if(posy == 0)
    {
        pos |= PositionTop;
    }
    else if (posy == Size_Y - 1)
    {
        pos |= PositionBottom;
    }
    #ifdef _DebugMode
    printf("ObjectNum = %d\tPosition = (%d, %d)\tPos = %d\n", TreeCount, posx, posy, pos);
    #endif
    TreeCount++;
}

/*update cell according to its status and neighbors*/
void TreeSim::TreeCell::Update(){
    GetTestArr();
    switch (status)
    {
    case Status_Green:
        GetNeighborStatus();
        if(neighbor_status){
            StartBurning();
        }
        break;
    
    case Status_Burning:
        if(burn_stage < max_burn){
            burn_stage += 1;
        }
        else
        {
            burn_stage = 0;
            status = Status_Burnt;
        }
        break;

    case Status_Burnt:
        if(DefaultRand.Generate(SpawnRate)){
            status = Status_Green;
        }else{
            status = Status_Burnt;
        }
        break;

    default:
        break;
    }
}

void TreeSim::TreeCell::GetTestArr(){
    testarr = 0;
    if(pos & PositionLeft){
        testarr |= Test_L|Test_LB|Test_LT;
    }
    if(pos & PositionTop){
        testarr |= Test_LT|Test_T|Test_RT;
    }
    if(pos & PositionRight){
        testarr |= Test_R|Test_RB|Test_RT;
    }
    if(pos & PositionBottom){
        testarr |= Test_B|Test_LB|Test_RB;
    }
    testarr = ~testarr;
}

void TreeSim::TreeCell::GetNeighborStatus(){
    neighbor_status = 0;
    #ifdef _Diagonal
    if((testarr&Test_LB) && DefaultRand.Generate(AffectRate[POS_LB]))
        neighbor_status += father_terrarian->PrevStatus[posy + 1][posx - 1]  & Status_Burning;
    if((testarr&Test_RB) && DefaultRand.Generate(AffectRate[POS_RB]))
        neighbor_status += father_terrarian->PrevStatus[posy + 1][posx + 1]  & Status_Burning;    
    if((testarr&Test_LT) && DefaultRand.Generate(AffectRate[POS_LT]))
        neighbor_status += father_terrarian->PrevStatus[posy - 1][posx - 1]  & Status_Burning;
    if((testarr&Test_LT) && DefaultRand.Generate(AffectRate[POS_RT]))
        neighbor_status += father_terrarian->PrevStatus[posy - 1][posx + 1]  & Status_Burning;
    #endif
    if((testarr&Test_L) && DefaultRand.Generate(AffectRate[POS_L]))
        neighbor_status += father_terrarian->PrevStatus[posy][posx - 1]      & Status_Burning;
    if((testarr&Test_T) && DefaultRand.Generate(AffectRate[POS_T]))
        neighbor_status += father_terrarian->PrevStatus[posy - 1][posx]      & Status_Burning;
    if((testarr&Test_R) && DefaultRand.Generate(AffectRate[POS_R]))
        neighbor_status += father_terrarian->PrevStatus[posy][posx + 1]      & Status_Burning;
    if((testarr&Test_B) && DefaultRand.Generate(AffectRate[POS_B]))
        neighbor_status += father_terrarian->PrevStatus[posy + 1][posx]      & Status_Burning;
}

/*update terrarian*/
void TreeSim::Terrarian::Update(){
    int i, j;
    burn_count = 0;
    /*save previous burning status, and count burning cells*/
    for(i = 0; i < Size_Y; i++){
        for(j = 0; j < Size_X; j++){
            PrevStatus[i][j] = Tarray[i][j].status;
            if(PrevStatus[i][j] == Status_Burning)
                burn_count += 1;
        }
    }
    /*update each cell*/
    for(i = 0; i < Size_Y; i++){
        for(j = 0; j < Size_X; j++){
            Tarray[i][j].Update();
            /*randomly generate fire (thunder)*/
            #ifndef _ManualStart
            if(DefaultRand.Generate(ThunderRate) && (Tarray[i][j].status & Status_Green))
                Tarray[i][j].StartBurning();
            #endif
        }
    }
    /*record term*/
    term++;
}

/*initialize terrarian*/
void TreeSim::Terrarian::Init(){
    int i, j;
    for(i = 0; i < Size_Y; i++){
        for(j = 0; j < Size_X; j++){
            Tarray[i][j].father_terrarian = this;
            Tarray[i][j].status = Status_Green;
        }
    }
    term = 0;
    #ifdef _ManualStart
    Tarray[Start_Pos_Y][Start_Pos_X].status = Status_Burning;
    #endif
}

void TreeSim::Terrarian::PrintTerra(){
    int i, j;
    /*clear screen*/
    #ifdef _WindowsMode
    system("cls");
    #else
    system("clear");
    #endif
    /*let each cell to appear on screen*/
    for(i = 0; i < Size_Y; i++){
        for(j = 0; j < Size_X; j++){
            Tarray[i][j].Merge();
        }
        printf("\n");
    }
    printf("Term: %u\n", term);
}

/*how the cells appears. colors are defined in "99f_colors.h"*/
void TreeSim::TreeCell::Merge(){
    switch (status)
    {
    case Status_Green:
        printf(STYLE_CLEAR COLOR_GREEN "[]" STYLE_CLEAR);
        break;
    
    case Status_Burning:
        printf(STYLE_CLEAR COLOR_RED   "[]" STYLE_CLEAR);
        break;
    
    case Status_Burnt:
        printf(STYLE_CLEAR             "[]" STYLE_CLEAR);
        break;

    default:
        break;
    }
}

/*function to start a burning cell*/
void TreeSim::TreeCell::StartBurning(){
    status = Status_Burning;
    burn_stage = 0;
    max_burn = rand()%(MaxiumBurnDuration - MinimumBurnDuration + 1) + MinimumBurnDuration;
}

/*following definition of class TreeSim::Wind are not done yet*/
TreeSim::Wind::Wind(float X_COMP, float Y_COMP){
    x = X_COMP;
    y = Y_COMP;
    mod = sqrt(x*x + y*y);
    scale = abs(x)>abs(y)?abs(x):abs(y);
    normalize_factor = scale / (scale + 1);

}

float TreeSim::Wind::distance(float posx, float posy){
    return sqrt(pow((posx - x), 2) + pow(posy - y, 2));
}

void TreeSim::Wind::SetField(float* rate_field){
    float local_max = rate_field[0];
    float local_min = rate_field[0];
    float tmp_diff;
    int i;
    #ifdef _Diagonal
    #endif
    rate_field[POS_L] = distance(-1 * scale, 0);
    rate_field[POS_R] = distance( 1 * scale, 0);
    rate_field[POS_T] = distance(0, -1 * scale);
    rate_field[POS_B] = distance(0,  1 * scale);
    for(i = 0; i < _NeighborSize; i++)
    {
        if (rate_field[i] > local_max)
            local_max = rate_field[i];
        if (rate_field[i] < local_min)
            local_min = rate_field[i];
    }
    for(i = 0; i < _NeighborSize; i++)
    {
        rate_field[i] = (rate_field[i] - local_min)/(local_max - local_min);
        rate_field[i] = normalize_factor * (rate_field[i] - 0.5) + 0.5;
    }
}


#ifdef _UseCustomDelay
void TreeSim::Delay(clock_t delay){
    clock_t now = clock();
    while(clock() - now < delay);
}
#endif

#endif /*__99f_hpp__*/
