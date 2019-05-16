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
        PerceptronInterface(TVector<double> * environmentptr,
                            TMatrix<double> * controllerptr,
                            TVector<double> * actuatorptr,
                            int MaxLinks_ // Number of in or out links for a perc
                            )
            {
            // Pull in objects for warm up
            environment = &environmentptr;
            controller  = &controllerptr;
            actuator    = &actuatorptr;

            // Compute sizes for initialization
            environmentsize = environment.Size();
            controllersize  = controller.RowSize();
            controllerdimension = controller.CollumnSize();
            actuatorsize = actuator.Size();
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
    TVector<double>  environment;
    TMatrix<double>  controller;
    TVector<double>  actuator;
