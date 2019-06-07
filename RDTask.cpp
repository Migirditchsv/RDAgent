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
//void Printer(int linenum);
//void Writer();
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

// **************************** 
// Global Objects
// ****************************

// The agent
RDAgent Agent;

// The Genome;
TVector<double> genome;//int values will be cast to int
//Controller traits
int rdparamnum = Agent.Controller.paramnum;
//int controllersize = Agent.Controller.size;
// Interface traits
int inpercs = Agent.Interface.inperceptronnum;
int outpercs = Agent.Interface.outperceptronnum;
int maxlinks = Agent.Interface.maxlinknum;

genomesize = 
genomesize+= 

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
    RDAgent Agent(0,0);
    //Printer(0);

    // TSearch Configuration


   // Dynamics

    return(0);
}

void Fittness()
{
    int x = 0;
}

void GenomeLinker()
{// Configured for 1D ring with vertical symetry
    //vars
    int poscounter=1;//tracks position in search vector
    int parameters;// number of paramteers
// BEGIN PARAMETER LINK
    // RD Parameters
    parameters = Agent.Controller.paramnum;
    while(poscounter<=parameters)
    {
    Agent.Controller.rdparameter(poscounter)=genome(poscounter);
    poscounter++;
    }
}