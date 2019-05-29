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
// Declarations
// ****************************
void Printer(int linenum);
void Writer();
// pointer to agent object

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

//**************************** 
// Warm Up
//****************************

//**************************** 
// MAIN LOOP
//****************************

int main()
{
    // Local Vars

    // Init Randomness Engine
    SetRandomSeed(RANDOMSEED);

    // Init Agent
    RDAgent Agent;
    Printer(0);

    // TSearch Configuration


   // Dynamics

    return(0);
}

//  Prints out some status info
void Printer(int linenum)
{
cout<<"|||PRINT STATUS|||"<<endl;
cout<<"Position Number:"<<linenum<<endl;

cout<<"---Controller State---"<<endl;
int size = Agent.Controller.size; 
TMatrix<double> state == Agent.Controller.state;
cout<<"Controller Size:" <<<<endl;
cout<<"Controller State:"<<<<endl;

cout<<"---Agent State---"<<endl;
double posx = Agent.PositionX();
double posy = Agent.PositionY();
cout<<"Agent X Position:"<<posx<<endl;
cout<<"Agent Y Position:"<<posy<<endl;
}

// Writes out the data
void Writer()
{

}
