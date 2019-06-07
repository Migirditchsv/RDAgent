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
#include "RDAgent.h"// call to sensors (rays)
#include "random.h"

// **************************** 
// Accessors
// ****************************

// **************************** 
// Utility functions
// ****************************

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
           controller->InjectCell(externalinput,channelindx,targetindx); 
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
            internalstate = controller->CellStateChannel(sourceindx, channelindx);
            outperceptron[p].state+= weight * internalstate;
            //skipout from source loop
            skipout:;
        }// end source loop

        // Write state to acutator
        actuator(targetindx)=outperceptron[p].state;

    }//end perc loop
}// End FireOutPerceptrons

