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
double Fittness(TVector<double> &gene, RandomState &rs);
void InitGenome();
void TrackParticle(VisualObject particle);
void BilateralGenomeLinker(TVector<double> gene);
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
    // TEST BED--------
    TMatrix<double> tester;
    tester.SetBounds(1,10,1,2);
    tester.FillContents(0);
    cout<<"TESTBED: tester = \n"<<tester<<endl;
    cout<<"tester(5,2)=9 setting..."<<endl;
    tester(5,2)=9;
    cout<<tester<<endl;

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
    cout<<"DOUBLE CHECK INPERC(1).State="<<Agent.Interface.inperceptron(1).source<<endl;
    cout<<"DOUBLE CHECK INPERC(4).State="<<Agent.Interface.inperceptron(4).source<<endl;
    cout<<"DOUBLE CHECK agent.interface.sensor.size: "<< Agent.Interface.sensor.Size()<<endl;
    Agent.Printer(-1);
    Agent.Interface.SetLinkNum(maxlinks, initlinks);
    Agent.Printer(-2);
    Agent.Interface.SetRandomInputLinks();
    Agent.Printer(-3);
    Agent.Interface.SetRandomOutputLinks();
    Agent.Printer(0);

    //  Compute Genome Size
    InitGenome();// gene of 0's: genome for testing
    cout<<"RDTaskMain.cpp: Genome size set: "<<genomesize<<endl;

    // Init and configure TSearch 
    TSearch s(genomesize);
    s.SetRandomSeed(RANDOMSEED);
    s.SetEvaluationFunction(Fittness);
    s.SetSelectionMode(RANK_BASED);
    s.SetReproductionMode(GENETIC_ALGORITHM);
    s.SetPopulationSize(25);
    s.SetMaxGenerations(50);
    s.SetMutationVariance(0.1);
    s.SetMaxExpectedOffspring(1.1);
    s.SetElitistFraction(0.1);
    s.SetSearchConstraint(1);
    cout<<"Search Configuration: Complete"<<endl;

    // run search!
    s.ExecuteSearch();
    
    // Report on best individual
    cout<<"\n\n\n ----SEARCH COMPLETE----\n\n"<<endl;
    cout<<s.BestIndividual()<<endl;

    return(0);
}

double Fittness(TVector<double> &gene, RandomState &rs)
{
    cout<<"RDTaskMain::Fittness: Evaluating Agent"<<endl;
    // Init visual object  particle
    VisualObject particle;
    cout<<"RDTaskMain::Fittness: VisualObject Declared"<<endl;
    // Insert gene into agent
    BilateralGenomeLinker(gene);
    cout<<"RDTaskMain::Fittness: Genome Linking COMPLETE"<<endl;
    // prepare random initial state
    Agent.Controller.RandomReactorState();
    cout<<"RDTaskMain::Fittness: Randomize Reactor State COMPLETE"<<endl;
    // Main agent loop
    for( int t = 0; t<AGENTSTEPLIMIT; t++)
    {
        //place particle
        cout<<"RDTaskMain::Fittness: Calling Track Particle"<<endl;
        TrackParticle(particle);
        cout<<"RDTaskMain::Fittness: Calling Agent.step()"<<endl;
        Agent.Step(particle);
    }
    cout<<"RDTaskMain::Fittness: MAIN LOOP COMPLETE"<<endl;

    // Compute score
    double fit = abs( Agent.PositionX() );// move away from center
    cout<<"RDTaskMain::Fittness: Evaluation COMPLETE"<<endl;
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
    cout<<"Genome defined with size:"<<genomesize<<"\n"<<flush;
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
    //cout<<"BilateralGenomeLinker: localcontrolsize: "<<localcontrolsize<<endl;
    //cout<<"BilateralGenomeLinker: Parameters Read"<<endl;
//PARAMETER LINK
    // RD Parameters
    while(poscounter<=parameters)
    {
    Agent.Controller.SetParameter( poscounter , gene(poscounter) );
    poscounter++;
    }
    // Inperceptrons
    for(int p = 1; p<= m ; p++)
    {int antip = 2*m-p;// Indexes backwards into the array
    //cout<<"BilateralGenomeLinker: Input perceptron p: "<<p<<" m: "<<m<<" antip: "<<antip<<endl;
        for(int target=1; target<=maxlinks; target++)
        {
            //cout<<"BilateralGenomeLinker: target loop P:"<<p<<" T: "<<target<<"pos: "<<poscounter<<endl;
            // Target [discrete]
            dgene = gene(poscounter);
            //cout<<"BilateralGenomeLinker: target dgene: "<<dgene<<endl;
            igene = Discretize(dgene, 1, controllersize);
            //cout<<"BilateralGenomeLinker: target igene: "<<igene<<endl;
            Agent.Interface.inperceptron(p).target(target) = igene;
            int mirrortarget = localcontrolsize - igene + 1;
            //cout<<"BilateralGenomeLinker: mirrortarget: "<<mirrortarget<<endl;
            Agent.Interface.inperceptron(antip).target(target) = mirrortarget;
            poscounter++;
            //cout<<"BilateralGenomeLinker: Weight pos:"<<poscounter<<endl;
            //weight
            Agent.Interface.inperceptron(p).weight(target) = gene(poscounter);
            Agent.Interface.inperceptron(antip).weight(target) = gene(poscounter);
            poscounter++;
            //cout<<"channel pos:"<<poscounter<<endl;
            //channel [discrete]
            dgene = genome(poscounter);
            igene = Discretize(dgene,1,channelnum);
            Agent.Interface.inperceptron(p).channel= igene;
            Agent.Interface.inperceptron(p).channel= igene;
            poscounter++;
        }
    }
    //cout<<"BilateralGenomeLinker: TEST post in"<<endl;
    
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
    double scale;

    scale = 0.5 * (maxbound - minbound);
    index =  round( scale * (value + 1.0 ) );

    return( index );
}