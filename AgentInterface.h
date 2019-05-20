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
#include "VectorMatrix.h"
//#include "random.h"

#pragma once

// **************************** 
// Public Globals
// ****************************

int inputperceptrons;// Number of input perceptrons
int outputperceptrons;// Number of output perceptrons
TVector<double> perceptronstates;// The state of the perceptron 

// **************************** 
// RDControl Class Declaration
// ****************************

class PerceptronInterface
    public:
        // Constructor
        PerceptronInterface(TVector<double> * environmentptr_,
                            TMatrix<double> * controllerptr_,
                            TVector<double> * actuatorptr_,
                            int inperceptronnum_
                            int outperceptronnum_,
                            int MaxLinks_ // Number of in or out links for a perc
                            )
            {
            // Safe copy arguments
            environmentptr = environmentptr_;
            controllerptr = controllerptr_;
            actuatorptr = actuatorptr_;
            inperceptronnum = inperceptronnum_;
            outperceptronnum = outperceptronnum_;
            maxlinks = maxlinks_;

            // Pull in objects for warm up
            environment = &environmentptr;
            controller  = &controllerptr;
            actuator    = &actuatorptr;

            // Compute sizes for initialization
            environmentsize = environment.Size();
            controllersize  = controller.RowSize();
            controllerdimension = controller.CollumnSize();
            actuatorsize = actuator.Size();

            // Initialize arrays
            

            }
    private:
    // **************************** 
    // Private Global Vars
    // ****************************
    
    // Control and Safety
    int maxlinks;
    int environmentsize;
    int controllersize;
    int controllerdimension; // number of channels in the controller
    int actuatorsize;
    // Pointers for linking
    TVector<double> * environmentptr;
    TMatrix<double> * controllerptr;    
    TVector<double> * actuatorptr;
    // Local copy of external states
    TVector<double>  environment;// on [0,1]
    TMatrix<double>  controller;// weighted by [-1,1]
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
    TMatrix<int> source;// The channels a perc pulls from
    TMatrix<int> target;// The channels a perc projects onto
    TMatrix<double> weight;// The weights applied to values pooled or projected
    TMatrix<double> channel;
    // ^ The channel that an in-layer perc projects to for a given controller cell

    
    
