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
//#include <cmath>

// Headers
#include "VectorMatrix.h"// Duh
#include "Perceptron.h"
//#include "random.h"

#pragma once

// **************************** 
// Public Globals
// ****************************

//int inputperceptronnum;// Number of input perceptrons
//int outputperceptronnum;// Number of output perceptrons
//TVector<double> perceptronstates;// The state of the perceptron 

// **************************** 
// Objects
// ****************************

struct perceptron
{
    double state; // perceptron state = sum of weighted input(s)
    TVector<int> source;
    // ^ Potentially weighted source(s) a perceptron integrates
    TVector<int> target;
    //^ indicies of a TVector that weighted states are projected onto
    TVector<int> channel;
    // ^ The channel that is read from or two on a given controller channel
    TVector<double> weight;
    // ^ The weights that will be applied to integrated or projected values
    // depending on if perceptron is input or output.
};

// **************************** 
// RDControl Class Declaration
// ****************************

class AgentInterface{
    public:
        // Constructor
        AgentInterface(TVector<double> * eptr,
                            TMatrix<double> * cptr,
                            TVector<double> * aptr,
                            int innum,
                            int outnum,
                            int maxnum, 
                            int initnum){
            
            // Safe copy arguments
            environmentptr = eptr;
            controllerptr = cptr;
            actuatorptr = aptr;
            inperceptronnum = innum;
            outperceptronnum = outnum;
            maxlinknum = maxnum;
            initlinknum = initnum;

            // Pull in objects for warm up
            environment = &environmentptr;
            controller  = &controllerptr;
            actuator    = &actuatorptr;

            // Compute sizes for initialization
            environmentsize = environment.Size();
            controllersize  = controller.RowSize();
            controllerdimension = controller.ColumnSize();
            actuatorsize = actuator.Size();

            // Initialize Input Perceptrons
            TVector<perceptron> inperceptrons;
            inperceptrons.SetBounds(1,inperceptronnum);
            inperceptrons.source.Setbounds(1,inperceptronnum);//single source
            inperceptrons.target.SetBounds(1,maxlinknum);
            inperceptrons.channel.SetBounds(1,maxlinknum);
            inperceptrons.weight.SetBounds(1,maxlinknum);

            // Initialize Output Perceptrons
            outperceptrons.SetBounds(1,outperceptronnum);
            };// end constructor



            
    private:
    // **************************** 
    // Private Global Vars
    // ****************************
    
    // Array size control 
    int inperceptronnum;// Number of input layer perceptrons
    int outperceptronnum;// Number of output layer perceptrons
    int maxlinknum;// Number of links to or from controller each perc starts with
    int initlinknum;// Number of links to start with
    int environmentsize;// Number of variables defining the sensable environment
    int controllersize;// Number of objects composing the controller
    int controllerdimension; // number of channels in the controller
    int actuatorsize;// Number of actuators under agent control
    
    // Perceptron arrays
    TVector<perceptron> inperceptrons;
    TVector<perceptron> outperceptrons;

    // Pointers for linking
    TVector<double> * environmentptr;
    TMatrix<double> * controllerptr;    
    TVector<double> * actuatorptr;
    // Local copy of external states
    TVector<double>  environment;// on [0,1]
    TMatrix<double>  controller;// on [0,1], to be weighted by [-1,1]
    TVector<double>  actuator;// on [0,1]
    // --------------------------The perceptrons-------------------------------
    // These arrays use the following convention for perceptron indicies:
    // indx > 0: input layer perceptron
    // indx = 0: ignored
    // indx < 0: output layer perceptron. 
    // This sign convention is used to autoselect between environment,
    // controller and actuator for all other indicies as described in the next
    // section. Don't mess it up!
    // ------------------------------------------------------------------------
    // in-layer percs only read from a single source (environmental variable)
    // and out-layer percs read from multiple sources (controller cells and
    // channels). The opposite is true for targets. To enforce this, negative 
    // environment, controller or actuator indicies are ignored. To protect
    // this convention, the in-layer perceptrons are checked to make sure there
    // is only one non-positive source index value per perceptron. The same is
    // done for out-layer perceptrons but for the target index instead.
    // Sorry, this was the cleanest way I could think to do it.
    // ------------------------------------------------------------------------
    // TMatrix<int> source;// The channels a perc pulls from
    // TMatrix<int> target;// The channels a perc projects onto
    // TMatrix<double> weight;// The weights applied to values pooled or projected
    // TMatrix<double> channel;
    // ^ The channel that an in-layer perc projects to for a given controller cell
    
    
    }// end agentinterface class
