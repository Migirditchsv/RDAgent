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
#include<cmath>// Discritize()

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

// Controller
const int controllersize = 138;
const int controllermodel = 0;// 0:Grey-Scott
const int controllertopology = 0;// 0: 1D Near neigh. ring
const double controldt = 0.1;// controller step size
const int controllimit = 1;// controller steps per agent step

// Linker
const int maxlinks = 8;// Max # of links controller a perceptron may have
const int initlinks = 4;// number of links to controller a perceptron starts with


// **************************** 
// Global Objects
// ****************************

// The agent
RDAgent Agent;

// The Genome;
TVector<double> genome;//int values will be cast to int

// Function Declarations
double Fittness();
void InitGenome();
void BilateralGenomeLinker();
int Discretize(double value, int minbound, int maxbound);

// Global Var Declarations
int m,j;// in and out perceptron (respectively) index for bilateral symetry
int genomesize;// length of genome

//**************************** 
// Warm Up
//****************************

//**************************** 
// MAIN LOOP
//****************************

int main()
{
    // TEST
    TMatrix<int> testm;
    testm.SetBounds(1,3,1,5);
    testm.FillContents(0);
    testm(1,5) = 9;
    cout<<"TEST\nROW:"<<testm.RowSize()<<"\nCOL:"<<testm.ColumnSize()<<"\n"<<flush;
    cout<<testm<<"\n"<<flush;
    // Local Vars

    // Init Randomness Engine
    SetRandomSeed(RANDOMSEED);
    cout<<"Random Seed Initializaiton: COMPLETE\n"<<flush;// debug

    // Init Agent
    Agent;
    Agent.SetPositionX(0.0);
    // Agent.Controller stuff
    Agent.Controller.SetReactorSize(controllersize);
    Agent.Controller.SetRDModel(controllermodel);
    Agent.Controller.SetReactorTopology(controllertopology);
    // Agent.Interface stuff
    Agent.Interface.RefferenceInterface(Agent.Rays, Agent.Controller, Agent.motor);
    Agent.Interface.SetLinkNum(maxlinks, initlinks);
    cout<<"RDTaskmain.cpp: maxlinks: "<<Agent.Interface.maxlinknum<<endl;
    Agent.Interface.SetRandomInputLinks();
    cout<<"TaskMain::outperceptron.Size= "<<Agent.Interface.outperceptron.Size()<<endl;
    Agent.Interface.SetRandomOutputLinks();
    cout<<"Agent Initialization: COMPLETE\n"<<flush;// debug
    Agent.Printer(0);

    // Init Genome
    InitGenome();

    // TSearch Configuration
    BilateralGenomeLinker();


   // Dynamics

    return(0);
}

double Fittness()
{
    double fit = abs( Agent.PositionX() );// move away from center
    return fit;
}

void InitGenome()
{
  //Controller traits
    int rdparamnum = Agent.Controller.GetParameterNumber();
    // Interface traits
    int inpercs = Agent.Interface.inperceptronnum;
    int outpercs = Agent.Interface.outperceptronnum;
    int maxlinks = Agent.Interface.maxlinknum;
    // add it all up
    m = 3*maxlinks*ceil( inpercs / 2 );// 3-> target,weight,channel
    j = 3 * maxlinks * ceil( outpercs / 2 );
    genomesize = rdparamnum + m + j;
    // set size
    genome.SetBounds(1,genomesize);
    cout<<"Genome defined with size:"<<genomesize<<"\n"<<flush;
}


void BilateralGenomeLinker()
{// Configured for 1D ring with vertical symetry
    //vars
    int poscounter=1;//tracks position in search vector
    double dgene; // holder for genome values to be Discritized
    int    igene; // holder for integer converted genes
    // Parameters
    int parameters = Agent.Controller.GetParameterNumber();
    int channelnum = Agent.Controller.GetChemicalNumber();
    int localcontrolsize = Agent.Controller.GetReactorSize();

//PARAMETER LINK
    // RD Parameters
    while(poscounter<=parameters)
    {
    Agent.Controller.SetParameter( poscounter , genome(poscounter) );
    poscounter++;
    }
    // Inperceptrons
    for(int p = 1; p<= m ; p++)
    {int antip = 2*m+1-p;// Indexes backwards into the array
        for(int target=1; target<=maxlinks; target++)
        {
            // Target [discrete]
            dgene = genome(poscounter);
            igene = Discretize(dgene, 1, controllersize);
            Agent.Interface.inperceptron(p).target(target) = igene;
            int mirrortarget = localcontrolsize - igene + 1;
            Agent.Interface.inperceptron(antip).target(target) = mirrortarget;
            poscounter++;
            //weight
            Agent.Interface.inperceptron(p).weight(target) = genome(poscounter);
            Agent.Interface.inperceptron(antip).weight(target) = genome(poscounter);
            poscounter++;
            //channel [discrete]
            dgene = genome(poscounter);
            igene = Discretize(dgene,1,channelnum);
            Agent.Interface.inperceptron(p).channel= igene;
            Agent.Interface.inperceptron(p).channel= igene;
            poscounter++;
        }
    }
    cout<<"CRASH TEST"<<endl;
    
    // out perceptrons
    for(int p = 1; p<= j; p++)
    {
            //channel [discrete]
            dgene = genome(poscounter);
            igene = Discretize(dgene,1,channelnum);
            Agent.Interface.inperceptron[p].channel= igene;
            poscounter++;
        for(int source=1; source<=maxlinks; source++)
        {
            // Target [discrete]
            dgene = genome(poscounter);
            igene = Discretize(dgene, 1, controllersize);
            Agent.Interface.outperceptron[p].source(source) = igene;
            poscounter++;
            //weight
            Agent.Interface.inperceptron[p].weight(source) = genome(poscounter);
            poscounter++;

        }
    }
    cout<<"BilateralGenomeLinker: COMPLETE:"<<endl;
}

//Takes a double on [-1,1] and 
int Discretize(double value, int minbound, int maxbound)
{   
    //vars
    int index;
    double scale;

    scale = 0.5 * (maxbound - minbound);
    index =  round( scale * (value + 1.0 ) );

    return( index );
}