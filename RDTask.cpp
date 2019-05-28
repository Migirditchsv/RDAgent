//*****************************************************************************
// RDTask: Acts on RDAgent objects and reports their behavior
//
//  By: Sam Migirditch, migirditch@gmail.com
//
// 05/2019: Created
//
//*****************************************************************************

// **************************** 
// Includes
// ****************************

// Std. Libs
#include<iostream>//debuging, realtime status update
#include<fstream>//write to file

// Headers
#include "TSearch.h"
#include "RDAgent.h"

using namespace std;

// **************************** 
// Control  Vars
// ****************************

// Timing
const int EVOSTEPLIMIT = 10;// evo generations to run
const int AGENTSTEPLIMIT = 300;//agent stpes to run per each evo trial
const int RDSTEPLIMIT = 5;//RD cycles to run per each agent step

// Resolution
const int EVOPOPSIZE = 5;
const int RDCELLNUM = 148;

// Random
const long RANDOMSEED = 1;

int main()
{
    // Local Vars
    int x = 0;

    // Init Randomness Engine
    SetRandomSeed(RANDOMSEED);

    // Init Agent
    RDAgent agent();

    // TSearch Configuration

//**************************** 
// MAIN LOOP
//****************************
   // Dynamics
    for(int step=1; step<=steplimit; step++)
    {
        x=0;

    }
    

    
    return(0);
}

//  Prints out some status info
void Printer(int linenum)
{
int y = 0;
}

// Writes out the data
void Writer()
{

}
