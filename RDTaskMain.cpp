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
// Precompile instructions
// ****************************

// define debug level. Comment to toggle on off
#define DEGBUGRDTASKMAIN

// **************************** 
// Declarations
// ****************************

double Fittness(TVector<double> &gene, RandomState &rs);
void InitGenome();
void TrackParticle(VisualObject particle);
void BilateralGenomeLinker(TVector<double> gene);
int Discretize(double value, int minbound, int maxbound);


// **************************** 
// Control  Vars: NEEDS SERIOUS LINKING/clean up
// ****************************

// Timing
const int EVOSTEPLIMIT = 10;// evo generations to run
const int AGENTSTEPLIMIT = 30;//agent stpes to run per each evo trial
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
    // Local Vars

    // Init Randomness Engine
    SetRandomSeed(RANDOMSEED);
    #ifdef DEBUG0
    cout<<"Random Seed Initializaiton: COMPLETE\n"<<flush;
    #endif

    // Init Agent
    Agent;
    Agent.SetPositionX(0.0);
    // Agent.Controller stuff
    cout<<"RDTaskMain- Init Phase: controllersize= "<<controllersize<<endl;
    Agent.Controller.SetReactorSize(controllersize);
    Agent.Printer(-11);
    Agent.Controller.SetRDModel(controllermodel);
    Agent.Printer(-22);
    Agent.Controller.SetReactorTopology(controllertopology);
    // Agent.Interface stuff
    Agent.Interface.RefferenceInterface(Agent.Rays, Agent.Controller, Agent.motor);
    Agent.Printer(-33);
    Agent.Interface.SetLinkNum(maxlinks, initlinks);
    Agent.Interface.SetRandomInputLinks();
    Agent.Interface.SetRandomOutputLinks();
    Agent.Printer(0);

    //  Compute Genome Size
    InitGenome();// gene of 0's: genome for testing
    #ifdef DEBUG0
    cout<<"RDTaskMain.cpp: Genome size set: "<<genomesize<<endl;
    #endif

    // Init and configure TSearch 
    TSearch s(genomesize);
    s.SetRandomSeed(RANDOMSEED);
    s.SetEvaluationFunction(Fittness);
    s.SetSelectionMode(RANK_BASED);
    s.SetReproductionMode(GENETIC_ALGORITHM);
    s.SetPopulationSize(500);
    s.SetMaxGenerations(50);
    s.SetMutationVariance(0.8);
    s.SetMaxExpectedOffspring(1.1);
    s.SetElitistFraction(0.1);
    s.SetSearchConstraint(1);
    #ifdef DEBUG0
    cout<<"Search Configuration: Complete"<<endl;
    #endif

    // run search!
    s.ExecuteSearch();
    
    // Report on best individual
    cout<<"\n\n\n ----SEARCH COMPLETE----\n\n"<<endl;
    cout<<s.BestIndividual()<<endl;

    return(0);
}

double Fittness(TVector<double> &gene, RandomState &rs)
{
    #ifdef DEBUG0
    cout<<"RDTaskMain::Fittness: Begin Evaluating Agent"<<endl;
    #endif
    
    // Init visual object  particle
    VisualObject particle;
    #ifdef DEBUG0
    cout<<"RDTaskMain::Fittness: VisualObject Declared"<<endl;
    #endif

    // Insert gene into agent
    BilateralGenomeLinker(gene);
    #ifdef DEBUG0
    cout<<"RDTaskMain::Fittness: Genome Linking COMPLETE"<<endl;
    #endif
    
    // prepare random initial state
    Agent.Controller.RandomReactorState();
    Agent.SetPositionX(0.0);
    #ifdef DEBUG0
    cout<<"RDTaskMain::Fittness: Randomize Initial Agent Position and Reactor State COMPLETE"<<endl;
    #endif



    // Main agent loop
    for( int t = 0; t<AGENTSTEPLIMIT; t++)
    {
        //place particle
        //#ifdef DEBUG0
        //cout<<"RDTaskMain::Fittness: Calling Track Particle"<<endl;
        //#endif
        TrackParticle(particle);

       // #ifdef DEBUG0
       // cout<<"RDTaskMain::Fittness: Calling Agent.step()"<<endl;
       // #endif
        cout<<"RDTaskMain::Fittness prechoke"<<endl;
        Agent.Step(particle);
        cout<<"RDTaskMain::Fittness postchoke"<<endl;

    }
    #ifdef DEBUG0
    cout<<"RDTaskMain::Fittness: MAIN LOOP COMPLETE"<<endl;
    #endif

    // Compute score
    double fit = abs( Agent.PositionX() );// move away from center
    //cout<<"RDTaskMain::Fittness: Evaluation COMPLETE"<<endl;
    return fit;// does this have to be normalized?
}

void TrackParticle( VisualObject particle )
{
    double py = Agent.GetBodySize() + 0.2 * Agent.GetRayLength() ;
    double px = Agent.PositionX() - 0.5 * Agent.GetBodySize() ;
    particle.SetPositionY( py );
    particle.SetPositionX( px );
}


void InitGenome()// an actual TVector gene does not need to be created. TSearch handles that. Change later
{
  //Controller traits
    int rdparamnum = Agent.Controller.GetParameterNumber();
    // Interface traits
    int inpercs = Agent.Interface.inperceptronnum;
    int outpercs = Agent.Interface.outperceptronnum;
    int maxlinks = Agent.Interface.maxlinknum;
    // add it all up
    m = ceil( inpercs / 2.0 );
    j = ceil( outpercs / 2.0 );
    genomesize = rdparamnum + 3 * maxlinks * ( m + j );
    // set size
    genome.SetBounds(1,genomesize);
    #ifdef DEBUG0
    cout<<"Genome defined with size:"<<genomesize<<"\n"<<flush;
    #endif
}


void BilateralGenomeLinker(TVector<double> gene)
{// Configured for 1D ring with vertical symetry
    //vars
    int poscounter=1;//tracks position in search vector
    double dgene; // holder for genome values to be Discritized
    int    igene; // holder for integer converted genes
    // Parameters
    int parameters = Agent.Controller.GetParameterNumber();
    int channelnum = Agent.Controller.GetChemicalNumber();
    int localcontrolsize = Agent.Controller.GetReactorSize();
    #ifdef DEBUGRDTASKMAIN
    cout<<"BilateralGenomeLinker: Parameters Read"<<endl;
    #endif

    //PARAMETER LINK
    // RD Parameters
    while(poscounter<=parameters)
    {
    Agent.Controller.SetParameter( poscounter , gene(poscounter) );
    poscounter++;
    }
    // Inperceptrons
    for(int p = 1; p<= m ; p++)
    {
        int antip = 2*m-p;// Indexes backwards into the array
        #ifdef DEBUGRDTASKMAIN
        cout<<"BilateralGenomeLinker: Input perceptron p: "<<p<<" m: "<<m<<" antip: "<<antip<<endl;
        #endif
        
        for(int target=1; target<=maxlinks; target++)
        {
            // Target [discrete]
            dgene = gene(poscounter);
            #ifdef DEBUGRDTASKMAIN
            cout<<"BilateralGenomeLinker: target dgene: "<<dgene<<endl;
            #endif
            igene = Discretize(dgene, 1, localcontrolsize);

            #ifdef DEBUGRDTASKMAIN
            cout<<"BilateralGenomeLinker: target igene: "<<igene<<endl;
            #endif
            
            Agent.Interface.inperceptron(p).target(target) = igene;
            int mirrortarget = localcontrolsize - igene + 1;
            #ifdef DEBUGRDTASKMAIN
            cout<<"BilateralGenomeLinker: mirrortarget: "<<mirrortarget<<endl;
            #endif

            Agent.Interface.inperceptron(antip).target(target) = mirrortarget;
            poscounter++;
            #ifdef DEBUGRDTASKMAIN
            cout<<"BilateralGenomeLinker: Weight pos:"<<poscounter<<endl;
            #endif
            //weight
            Agent.Interface.inperceptron(p).weight(target) = gene(poscounter);
            Agent.Interface.inperceptron(antip).weight(target) = gene(poscounter);
            poscounter++;
            //channel [discrete]
            dgene = genome(poscounter);
            igene = Discretize(dgene,1,channelnum);
            Agent.Interface.inperceptron(p).channel= igene;
            Agent.Interface.inperceptron(p).channel= igene;
            poscounter++;
        }
    }
    
    // out perceptrons
    for(int p = 1; p<= 0; p++)
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
    //cout<<"BilateralGenomeLinker: COMPLETE:"<<endl;
}

//Takes a double on [-1,1] and 
int Discretize(double value, int minbound, int maxbound)
{   
    //vars
    int index;
    int truth = 1;
    double scale = maxbound - minbound;



    // if value is out of [-1,1] exit
    truth*= value>=-1;
    truth*= value<=1;
    if( truth==0 )
    {
        cerr<<"RDTaskMain::Discritize value= "<<value<<" out of bounds[-1,1]. ABORTING."<<endl;
        exit(EXIT_FAILURE);
    }

    index = value + 1;// now [0,2]
    index *= 0.5;//now [0,1]
    index *= scale;// now[min,max]

    // if value is out of [minbound,maxbound] exit
    truth*= index>=minbound;
    truth*=index<=maxbound;
    if( truth==0 )
    {
        cerr<<"RDTaskMain::Discritize index= "<<index<<" out of bounds["
        <<minbound<<","<<maxbound<<"].\n    Badly converted "<< value<<" to "<<index
        <<" ABORTING."<<endl;
        exit(EXIT_FAILURE);
    }
    
    return( index );
}