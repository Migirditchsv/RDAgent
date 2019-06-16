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
#include "RDAgent.h"//pulls in everything else. 
#include "VectorMatrix.h"
#include "random.h"

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

// Function Declarations
double Fittness();
void GenomeLinker();
int discretize(double value, int minbound, int maxbound);

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

    // Init Genome
    //Controller traits
    int rdparamnum = Agent.Controller.GetParameterNumber();
    // Interface traits
    int inpercs = Agent.Interface.inperceptronnum;
    int outpercs = Agent.Interface.outperceptronnum;
    int maxlinks = Agent.Interface.maxlinknum;
    // add it all up
    int genomesize = rdparamnum + 2*maxlinks*( inpercs + outpercs); 
    // set size
    genome.SetBounds(1,genomesize);

    // Init Agent
    RDAgent Agent(0,0);
    Agent.Printer(0);


    // TSearch Configuration



   // Dynamics

    return(0);
}

double Fittness()
{
    double fit = abs( Agent.PositionX() );// move away from center
    return fit;
}

void GenomeLinker()
{// Configured for 1D ring with vertical symetry
    //vars
    int poscounter=1;//tracks position in search vector
    double dgene; // holder for genome values to be discritized
    int    igene; // holder for integer converted genes
    // Parameters
    int parameters = Agent.Controller.GetParameterNumber();
    int channelnum = Agent.Controller.GetChemicalNumber();

//PARAMETER LINK
    // RD Parameters
    while(poscounter<=parameters)
    {
    Agent.Controller.SetParameter( poscounter , genome(poscounter) );
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
int discretize(double value, int minbound, int maxbound)
{   
    //vars
    int index;
    double scale;

    scale = 0.5 * (maxbound - minbound);
    index =  round( scale * (value + 1.0 ) );

    return( index );
}