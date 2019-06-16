//*****************************************************************************
// PerceptronInterface: Uses perceptrons to link:
// (ENVIRONMENNT)->[IN-PERCEPTRONS]->(Controller)->[Out-Perceptrons]->(Actuators)
// 
// Where sensor and Actuator are TVector objects and Controller is a
// TMatrix object. The Controler TMatrix is interpreted as (element,channel) as
// this package is designed to read single channel signals into and out of a
// controller object whose individual elements combine information from 
// multiple chanels and share the result in a diffusive manner. 
//
// Sam Migirditch, migirditch@gmail.com
//
// 05/16/2019: Created
//
//*****************************************************************************

// **************************** 
// Includes
// ****************************

// Std. libs
#include <iostream>// Debug cout,
#include <vector>// in/out perceptron groups
//#include <cmath>

// Headers
#include "VectorMatrix.h"
#include "AgentInterface.h"
#include "RDAgent.h"// call to sensors (rays) should replace at some point
// ^ Theoretically unneeded refference. 
#include "random.h"

// **************************** 
// Accessors
// ****************************

// **************************** 
// Utility functions
// ****************************

void AgentInterface::RefferenceInterface(TVector<Ray> &rsensor,
                            RDControl &rcontroller,
                            TVector<double> &ractuator/*,
                            int inperceptronnum = 0,
                            int outperceptronnum = 0,
                            int maxlinknum = 0, 
                            int initlinknum = 0*/)
        {
            // Compute sizes for initialization
            sensorsize      = sensor.Size();
            controllersize  = controller.GetReactorSize();
            controllerdimension = controller.GetChemicalNumber();
            actuatorsize    = actuator.Size();

            // Initialize Input TVector
            inperceptron.SetBounds(1,inperceptronnum);
            for(int p=1; p<=inperceptronnum; p++)
            {
                inperceptron(p).channel = 1; // Default to channel 1
                inperceptron(p).source.SetBounds(1,1);//only one source
                inperceptron(p).source.FillContents(p); // one perceptron per sense organ
                inperceptron(p).state   = 0.0; // start clean
                inperceptron(p).target.SetBounds(1,maxlinknum); //size target list
                inperceptron(p).target.FillContents(0); // targets <=0 are skipped
                inperceptron(p).weight.SetBounds(1,maxlinknum); // size weight list
                inperceptron(p).weight.FillContents(1.0); //Index determines skip, nonzero default weights increase evolvability
            }


            // Initialize output TVector
            outperceptron.SetBounds(1,outperceptronnum);
            for(int p=1; p<=outperceptronnum; p++)
            {
                inperceptron(p).channel = 1; // Default to channel 1
                inperceptron(p).source.SetBounds(1,maxlinknum);
                inperceptron(p).source.FillContents(0);// Default off all read out links
                inperceptron(p).state   = 0.0; // start clean
                inperceptron(p).target.SetBounds(1,1); //size target list
                inperceptron(p).target.FillContents(p); // targets <=0 are skipped
                inperceptron(p).weight.SetBounds(1,maxlinknum); // size weight list
                inperceptron(p).weight.FillContents(1.0); //Index determines skip, nonzero default weights increase evolvability
            }

            // Old input initialize, replaced with type TVectors
            //// Initialize Input Perceptrons
            //for(int i=0; i<inperceptronnum; i++)
            //{
            //    // initialize struct
            //    perceptron perc;
            //    perc.source.SetBounds(1,1);// single source inputs
            //    perc.target.SetBounds(1,maxlinknum);
            //    perc.weight.SetBounds(1,maxlinknum);
            //    //^ fill struct with default (skip/neutral) values
            //    perc.source(1)=i;// one perceptron per sense organ
            //    perc.target.FillContents(0);// targets with index =< 0 are skipped
            //    perc.channel = 1;// All perceptrons default to channel 1
            //    perc.weight.FillContents(0);// To be filled with values on (0,1)
            //    
            //    // Push struct onto inperceptron vector
            //    inperceptron.emplace_back(perc);
            //}

           // Depreciated in favor of TVector object
           // // Initialize Output Perceptrons
           // for(int i=0; i<outperceptronnum; i++)// vecs index from zero. god forgive me.
           // {
           //     // initialize struct
           //     perceptron perc;
           //     perc.source.SetBounds(1,maxlinknum);
           //     //^ possibly many controller inputs
           //     perc.target.SetBounds(1,1);
           //     //^ Controller inputs aggregated to one actuator
           //     perc.weight.SetBounds(1,maxlinknum);
           //     // fill struct with default (skip/neutral) values
           //     perc.source.FillContents(0);// Link to controller later
           //     perc.target(1)= i;// output to assigned actuator
           //     perc.channel = 1;// Read from channel 1
           //     perc.weight.FillContents(0);// To be filled with values on (0,1)
           //     
           //     // Push struct onto inperceptron vector
           //     outperceptron.emplace_back(perc);
           // }

            // Set Initial Values 
            SetRandomInputLinks(); // in prog
            SetRandomOutputLinks();
            SetRandomInputWeights();
            SetRandomOutputWeights();

            };// end constructor
 
 void AgentInterface::SetRandomInputLinks()
 {
     //vars
     int linkindx;
     double weight;

     for(int p=1; p<=inperceptronnum; p++)
     {
         for(int l=1; l<=initlinknum; l++)
         {
            linkindx = UniformRandomInteger(1,controllersize);
            inperceptron(p).target(l)=linkindx;
            weight = UniformRandom(0,1);
            inperceptron(p).weight(l)=weight;
         }
     }
 }

 void AgentInterface::SetRandomOutputLinks()
 {
     //vars
     int linkindx;
     double weight;

     for(int p=1; p<=outperceptronnum; p++)
     {
         for(int l=1; l<=initlinknum; l++)
         {
            linkindx = UniformRandomInteger(1,controllersize);
            outperceptron(p).source(l)=linkindx;
            weight = UniformRandom(0,1);
            outperceptron(p).weight(l)=weight;
         }
     }
 }

// Reset Interface: Resets links and weights to random values
void AgentInterface::ResetInterface()
{
    // input layer
    for(int p=0; p<inperceptronnum; p++ )
    {
        // Fill initilized links
        for( int t=0; t<initlinknum; t++ )
        {
        // random target
        int randomtarget = UniformRandomInteger(1, controllersize);
        inperceptron[p].target(t) = randomtarget;
        // random weight for target
        double randomweight = UniformRandom(0.0,1.0);
        inperceptron[p].weight(t) = randomweight;
        }//end fill init links
        // Fill uninitialized links
        for( int t=initlinknum; t<maxlinknum; t++ )
        {
            inperceptron[p].target(t)=-1;
            inperceptron[p].weight(t)=0;
        }// end fill non-init links
    }// end input layer 

    // output layer
    for(int p=0; p<outperceptronnum; p++ )
    {
        // Fill initilized links
        for( int s=0; s<initlinknum; s++ )
        {
        // random source
        int randomsource = UniformRandomInteger(1, controllersize);
        outperceptron[p].source(s) = randomsource;
        // random weight for source 
        double randomweight = UniformRandom(0.0,1.0);
        inperceptron[p].weight(s) = randomweight;
        }//end fill init links
        // Fill uninitialized links
        for( int s=initlinknum; s<maxlinknum; s++ )
        {
            inperceptron[p].target(s)=-1;
            inperceptron[p].weight(s)=0;
        }// end fill non-init links
    }// end input layer 
}

// inject relevant weighted sensor value to target controller elements
void AgentInterface::FireInputPerceptrons(VisualObject &object)
{
    //vars
    int sourceindx, channelindx, targetindx;
    double externalinput;
    // Loop over in perceptrons
    for(int p=0; p<inperceptronnum; p++)
    {
        // Reset perceptron state
        inperceptron[p].state=0.0;
        // Set index routes
        sourceindx = inperceptron[p].source(1);
        channelindx = inperceptron[p].channel;
        // measure sensor state
        object.RayIntersection(sensor(sourceindx));
        externalinput = (MaxRayLength - sensor(sourceindx).length)/MaxRayLength;
        inperceptron[p].state = externalinput;
        // inject into controller targets
        for(int target=1; target<=maxlinknum; target++)
        {
           targetindx = inperceptron[p].target(target); 
           // check for placeholder indx
           if(targetindx<=0){goto skip;}
           // inject
           controller.InjectCell(externalinput,channelindx,targetindx); 
           skip:;
        }//end target loop
    }//end perceptron loop
}// end FireInputPerceptrons

void AgentInterface::FireOutputPerceptrons()
{
    // vars
    int sourceindx, channelindx, targetindx;
    double weight, internalstate;
    // loop over out percs
    for(int p=0; p<=outperceptronnum; p++)
    {// loop over percs
        // Clear perceptronstate
        outperceptron[p].state=0.0;
        // get indicies
        channelindx = outperceptron[p].channel;
        targetindx  = outperceptron[p].target(1);

        for(int source=1;source<=maxlinknum;source++)
        {//loop over sources & accumulate
            // Get sourceindx
            sourceindx = outperceptron[p].source(source);
            // check for placeholder index
            if(sourceindx<=0){goto skipout;}

            // get link weight
            weight = outperceptron[p].weight(sourceindx);
            //read from controller
            internalstate = controller.CellStateChannel(sourceindx, channelindx);
            outperceptron[p].state+= weight * internalstate;
            //skipout from source loop
            skipout:;
        }// end source loop

        // Write state to acutator
        actuator(targetindx)=outperceptron[p].state;

    }//end perc loop
}// End FireOutPerceptrons

