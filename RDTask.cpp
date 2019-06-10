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
#include<cmath>// discritize()

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
// add it all up
int genomesize = rdparamnum;
//int genomesize + 0;// tally up here. 

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
    double dgene; // holder for genome values to be discritized
    int    igene; // holder for integer converted genes
    // Parameters
    int parameters = Agent.Controller.paramnum;
    int channelnum = Agent.Controller.chemnum;
// BEGIN PARAMETER LINK
    // RD Parameters
    while(poscounter<=parameters)
    {
    Agent.Controller.rdparameter(poscounter)=genome(poscounter);
    poscounter++;
    }
    for(int p = 0; p<= Agent.Interface.inperceptronnum; p++)
    {
        for(int target=1; target<=maxlinks; target++)
        {
            // Target [discrete]
            dgene = genome(poscounter);
            igene = discretize(dgene, 1, controllersize);
            Agent.Interface.inperceptron[p].target(target) = igene;
            poscounter++;
            //weight
            Agent.Interface.inperceptron[p].weight(target) = genome(poscounter);
            poscounter++;
            //channel [discrete]
            dgene = genome(poscounter);
            igene = discretize(dgene,1,channelnum);
            Agent.Interface.inperceptron[p].channel= igene;
            poscounter++;
        }
    }
    
    // out perceptrons
    for(int p = 0; p<= Agent.Interface.outperceptronnum; p++)
    {
            //channel [discrete]
            dgene = genome(poscounter);
            igene = discretize(dgene,1,channelnum);
            Agent.Interface.inperceptron[p].channel= igene;
            poscounter++;
        for(int source=1; source<=maxlinks; source++)
        {
            // Target [discrete]
            dgene = genome(poscounter);
            igene = discretize(dgene, 1, controllersize);
            Agent.Interface.outperceptron[p].source(source) = igene;
            poscounter++;
            //weight
            Agent.Interface.inperceptron[p].weight(source) = genome(poscounter);
            poscounter++;

        }
    }
}

//Takes a double on [-1,1] and 
int discretize(double value, int min, int max)
{   
    //vars
    int min, max, index;
    double scale;

    scale = 0.5 * (max - min);
    index =  round( scale * (value + 1.0 ) );

    return( index );
}