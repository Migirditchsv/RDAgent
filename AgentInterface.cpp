//*****************************************************************************
// PerceptronInterface: Uses perceptrons to link:
// (ENVIRONMENNT)->[IN-PERCEPTRONS]->(Controller)->[Out-Perceptrons]->(Actuators)
// 
// Where Environment and Actuator are TVector objects and Controller is a
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
#include "random.h"

// **************************** 
// Accessors
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
            inperceptron[p].target=-1;
            inperceptron[p].weight=0;
        }// end fill non-init links
    }// end input layer 
    // output layer
    for(int p=0; p<outperceptronnum; p++ )
    {
        // Fill initilized links
        for( int s=0; t<initlinknum; t++ )
        {
        // random source
        int randomsource = UniformRandomInteger(1, controllersize);
        outperceptron[p].source(s).randomsource;
        // random weight for source 
        double randomweight = UniformRandom(0.0,1.0);
        inperceptron[p].weight(t) = randomweight;
        }//end fill init links
        // Fill uninitialized links
        for( int t=initlinkum; t<maxlinknum; t++ )
        {
            inperceptron[p].target=-1;
            inperceptron[p].weight=0;
        }// end fill non-init links
    }// end input layer 
}

// **************************** 
// Utility functions
// ****************************

