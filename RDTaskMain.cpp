//*****************************************************************************
// RDTask: Acts on RDAgent objects and reports their behavior
//
//  By: Sam Migirditch, migirditch@gmail.com
//
// 05/2019: Created
//
//*****************************************************************************

// Debug
//#define DEBUGRDTASKMAIN

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
//#define DEGBUGRDTASKMAIN


// **************************** 
// Control  Vars: NEEDS SERIOUS LINKING/clean up
// ****************************

// IO
const char EVODATAPATH[100] = "evodata.csv";
const char BESTAGENTDATAPATH[100] = "bestagent.csv";

// Timing
const int EVOSTEPLIMIT = 500;// evo generations to run
const int AGENTSTEPLIMIT = 10;//agent stpes to run per each evo trial
const double AGENTSTEPSIZE = 0.1;// scales agent velocity on each agent step
const double RDSTEPLIMIT = 5.0;// step limit of one controller step
const double RDSTEPSIZE  = 0.1;// step size

// Evolution
const int EVOPOPSIZE = 100;
const double EVOVARIANCE = 1.5;

// Random
const long RANDOMSEED = 1;

// Controller
const int controllersize = 138;
const int controllermodel = 0;// 0:Grey-Scott
const int controllertopology = 0;// 0: 1D Near neigh. ring

// Linker
const int maxlinks = 8;// Max # of links controller a perceptron may have
const int initlinks = 4;// number of links to controller a perceptron starts with


// **************************** 
// Global Declarations
// ****************************

// Fucntion declarations
double Fittness(TVector<double> &gene, RandomState &rs);
void InitGenome();
void TrackParticle(VisualObject particle);
void BilateralGenomeLinker(TVector<double> gene);
int Discretize(double value, int minbound, int maxbound);
void WriteEvoSearchState(int Generation,
                        double BestPerf,
                        double AvgPerf,
                        double PerfVar);
int TerminationFunction(int Generation,
                        double BestPerf,
                        double AvgPerf,
                        double PerfVar);
bool fileexists(const char *filename);

// The agent
RDAgent Agent;

// The Genome;
//TVector<double> genome;//int values will be cast to int

// The search
TSearch s(1);// Need a place holder size, TSearch can't init witout a real size. Resized later.

// data files
std::ofstream evodatafile;

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
    #ifdef DEBUGRDTASKMAIN
    cout<<"RDTaskMain::Main BEGIN"<<endl;
    #endif    
    // Local Vars

    // check data folders and prepare datafile
    if(fileexists(EVODATAPATH)==1){remove(EVODATAPATH);}
    evodatafile.open(EVODATAPATH);
    //print in headers
    evodatafile<<"Generation,Best,Average,Variance"<<endl;
    evodatafile.close();
    #ifdef DEBUGRDTASKMAIN
    cout<<"RDTaskMain::Main Data Storage Paths Created"<<endl;
    #endif

    // Init Randomness Engine
    SetRandomSeed(RANDOMSEED);
    #ifdef DEBUGRDTASKMAIN
    cout<<"Random Seed Initializaiton: COMPLETE\n"<<flush;
    #endif

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
    Agent.Interface.SetRandomInputLinks();
    Agent.Interface.SetRandomOutputLinks();
    Agent.SetTimeResolutions(RDSTEPSIZE,RDSTEPLIMIT,AGENTSTEPSIZE);
    Agent.Printer(0);

    //  Compute Genome Size
    InitGenome();// gene of 0's: genome for testing
    #ifdef DEBUGRDTASKMAIN
    cout<<"RDTaskMain.cpp: Genome size set: "<<genomesize<<endl;
    #endif

    // Configure TSearch 
    s.SetVectorSize(genomesize);
    s.SetRandomSeed(RANDOMSEED);
    s.SetEvaluationFunction(Fittness);
    s.SetPopulationStatisticsDisplayFunction(WriteEvoSearchState);
    s.SetSearchTerminationFunction(TerminationFunction);
    s.SetSelectionMode(RANK_BASED);
    s.SetReproductionMode(GENETIC_ALGORITHM);
    s.SetPopulationSize(EVOPOPSIZE);
    s.SetMaxGenerations(EVOSTEPLIMIT);
    s.SetMutationVariance(EVOVARIANCE);
    s.SetMaxExpectedOffspring(1.1);
    s.SetElitistFraction(0.1);
    s.SetSearchConstraint(1);
    #ifdef DEBUGRDTASKMAIN
    cout<<"Search Configuration: Complete"<<endl;
    #endif

    // run search!
    s.ExecuteSearch();
    
    // Report on best individual
    cout<<"\n\n\n----SEARCH COMPLETE----\n\n"<<endl;
    cout<<s.BestIndividual()<<endl;

    return(0);
}

double Fittness(TVector<double> &gene, RandomState &rs)
{
    #ifdef DEBUGRDTASKMAIN
    cout<<"RDTaskMain::Fittness: Begin Evaluating Agent"<<endl;
    #endif
    
    // Init visual object  particle
    VisualObject particle;
    #ifdef DEBUGRDTASKMAIN
    cout<<"RDTaskMain::Fittness: VisualObject Declared"<<endl;
    #endif

    // Insert gene into agent
    BilateralGenomeLinker(gene);
    #ifdef DEBUGRDTASKMAIN
    cout<<"RDTaskMain::Fittness: Genome Linking COMPLETE"<<endl;
    #endif
    
    // prepare random initial state
    Agent.Controller.RandomReactorState();
    Agent.SetPositionX(0.0);
    #ifdef DEBUGRDTASKMAIN
    cout<<"RDTaskMain::Fittness: Randomize Initial Agent Position and Reactor State COMPLETE"<<endl;
    #endif



    // Main agent loop
    for( int t = 0; t<AGENTSTEPLIMIT; t++)
    {
        //place particle
        TrackParticle(particle);
        #ifdef DEBUGRDTASKMAIN
        cout<<"RDTaskMain::Fittness: Particle Tracking COMPLETE"<<endl;
        #endif

        Agent.Step(particle);
        #ifdef DEBUGRDTASKMAIN
        cout<<"RDTaskMain::Fittness: Calling Agent.step()"<<endl;
        #endif

    }
    #ifdef DEBUGRDTASKMAIN
    cout<<"RDTaskMain::Fittness: MAIN LOOP COMPLETE"<<endl;
    #endif

    // Compute score
    double fit = abs( Agent.PositionX() / Agent.GetEnvWidth() );// move away from center, normalized

    #ifdef DEBUGRDTASKMAIN
    Agent.Printer(1);
    #endif
    
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
    #ifdef DEBUGRDTASKMAIN
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
            dgene = gene(poscounter);
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
            dgene = gene(poscounter);
            igene = Discretize(dgene,1,channelnum);
            Agent.Interface.inperceptron[p].channel= igene;
            poscounter++;
        for(int source=1; source<=maxlinks; source++)
        {
            // Target [discrete]
            dgene = gene(poscounter);
            igene = Discretize(dgene, 1, controllersize);
            Agent.Interface.outperceptron[p].source(source) = igene;
            poscounter++;
            //weight
            Agent.Interface.inperceptron[p].weight(source) = gene(poscounter);
            poscounter++;

        }
    }
    //cout<<"BilateralGenomeLinker: COMPLETE:"<<endl;
}

//Takes a double on [-1,1] and 
int Discretize(double value, int minbound, int maxbound)
{   
    //vars
    double index;
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

    index = value + 1.0;// now [0,2]
    index *= 0.5;//now [0,1]
    index = round( index * scale ) + minbound;// now on int [min,max]


    return( index );
}

void WriteEvoSearchState(int Generation, double BestPerf, double AvgPerf, double PerfVar)
{
    // If file does not exist, warn
    if(fileexists(EVODATAPATH)==0)
    {
        cerr<<"Error: Evolutionary Data Storage File Not Found In RDTaskMain::WriteEvoSearch"
        <<"\n   Attempted File Path: "<<EVODATAPATH<<endl;
        exit(0);
    }

    // Open file
    evodatafile.open(EVODATAPATH, std::ios::app);
    // write stats to file
    evodatafile<<Generation<<","<<BestPerf<<","<<AvgPerf<<","<<PerfVar<<","<<flush;
    // write population to file
    for(int i=1; i<=EVOPOPSIZE-1; i++)
    {
        evodatafile<<s.Performance(i)<<","<<flush;
    }
    // Line break to end entry
    evodatafile<<s.Performance(EVOPOPSIZE)<<endl;

    // Close file
    evodatafile.close();

    // Print Status
    cout<<"RDTaskMain::WriteEvoSearchState Generation "<<Generation<<" of "
    <<EVOSTEPLIMIT<<" COMPLETE.\n"
    <<"Best: "<<BestPerf<<" Average: "<<AvgPerf<<" Variance: "<<PerfVar<<endl;
}

int TerminationFunction(int Generation, double BestPerf, double AvgPerf, double PerfVar)
{
    int truth = 0;

    // Kill if bestperf is perfect
    truth += ( BestPerf >= 1.0 );
    // Kill if evo step limit reached
    truth += ( Generation > EVOSTEPLIMIT );

    return truth;
}

bool fileexists(const char *filename) {
  std::ifstream ifile(filename);
  return (bool)ifile;
}