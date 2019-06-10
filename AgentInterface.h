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
#include <vector> // perceptrons structures are stored in vectors.
//#include <cmath>

// Headers
#include "VectorMatrix.h"// sensor, controller and actuator states
#include "RDControl.h"
#include "VisualObject.h"
//#include "random.h"

#pragma once

using namespace std;
// **************************** 
// Public Globals
// ****************************
// Any value which evolution acts on must be a public global
// Maybe accessor functions should be written to read and set these values
// instead of making them directly accessible. Yeah that's exactly what needs
// to happen.


// **************************** 
// Objects
// ****************************

struct perceptron
{
    public:
    double state; // perceptron state = sum of weighted input(s)
    TVector<int> source;
    // ^ Potentially weighted source(s) a perceptron integrates
    TVector<int> target;
    //^ indicies of a TVector that weighted states are projected onto
    int channel;
    // ^ The channel that is read from or two on a given controller channel
    TVector<double> weight;
    // ^ The weights that will be applied to integrated or projected values
    // depending on if perceptron is input or output.
};

// **************************** 
// RDControl Class Declaration
// ****************************
TVector<Ray> nullrayvec;
TVector<double> nullvector;
RDControl nullcontroller(0,0);
class AgentInterface{
    public:

// **************************** 
// Constructor & destructor
// ****************************

        // Constructor
        AgentInterface(     TVector<Ray> &rsensor,
                            RDControl &rcontroller,
                            TVector<double> &ractuator,
                            int inperceptronnum = 0,
                            int outperceptronnum = 0,
                            int maxlinknum = 0, 
                            int initlinknum = 0)
        : sensor(rsensor),controller(rcontroller),actuator(ractuator)
        {
            // Compute sizes for initialization
            sensorsize      = sensor.Size();
            controllersize  = controller.CellState.RowSize();
            controllerdimension = controller.CellState.ColumnSize();
            actuatorsize    = actuator.Size();

            // Initialize Input Perceptrons
            for(int i=0; i<inperceptronnum; i++)
            {
                // initialize struct
                perceptron perc;
                perc.source.SetBounds(1,1);// single source inputs
                perc.target.SetBounds(1,maxlinknum);
                perc.weight.SetBounds(1,maxlinknum);
                //^ fill struct with default (skip/neutral) values
                perc.source(1)=i;// one perceptron per sense organ
                perc.target.FillContents(0);// targets with index =< 0 are skipped
                perc.channel = 1;// All perceptrons default to channel 1
                perc.weight.FillContents(0);// To be filled with values on (0,1)
                
                // Push struct onto inperceptron vector
                inperceptron.emplace_back(perc);
            }

            // Initialize Output Perceptrons
            for(int i=0; i<outperceptronnum; i++)// vecs index from zero. god forgive me.
            {
                // initialize struct
                perceptron perc;
                perc.source.SetBounds(1,maxlinknum);
                //^ possibly many controller inputs
                perc.target.SetBounds(1,1);
                //^ Controller inputs aggregated to one actuator
                perc.weight.SetBounds(1,maxlinknum);
                // fill struct with default (skip/neutral) values
                perc.source.FillContents(0);// Link to controller later
                perc.target(1)= i;// output to assigned actuator
                perc.channel = 1;// Read from channel 1
                perc.weight.FillContents(0);// To be filled with values on (0,1)
                
                // Push struct onto inperceptron vector
                outperceptron.emplace_back(perc);
            }

            // Set Initial Values 
            SetRandomInputLinks(); // in prog
            SetRandomOutputLinks();
            SetRandomInputWeights();
            SetRandomOutputWeights();

            };// end constructor
        
        //Destructor
        ~AgentInterface()
        {
            inperceptron.clear();
            outperceptron.clear();
        }

    // **************************** 
    // Accessors
    // ****************************

    void ResetInterface();// Resets links and weights to random values
    void FireInputPerceptrons(VisualObject &object);
    void FireOutputPerceptrons();// Sets the state of the actuators from controller

    // **************************** 
    // Global Vars
    // ****************************
    
    // Array size control 
    int inperceptronnum;// Number of input layer perceptrons
    int outperceptronnum;// Number of output layer perceptrons
    int maxlinknum;// Number of links to or from controller each perc starts with
    int initlinknum;// Number of links to start with
    int sensorsize;// Number of variables defining the sensable sensor
    int controllersize;// Number of objects composing the controller
    int controllerdimension; // number of channels in the controller
    int actuatorsize;// Number of actuators under agent control
    
    // Perceptron arrays
    vector<perceptron> inperceptron;
    vector<perceptron> outperceptron;

    // Pointers for linking
    //VisualObject    &visobject;// the visual object being looked for
    TVector<Ray>    &sensor;
    RDControl       &controller;    
    TVector<double> &actuator;


            
    private:
    // **************************** 
    // Private Utility Functions
    // ****************************

    void SetRandomInputLinks();
    void SetRandomOutputLinks();
    void SetRandomInputWeights();
    void SetRandomOutputWeights();

    void RefreshsensorState();
    void PushToController();


    //// Local copy of external states
    //TVector<double>  sensor;// on [0,1]
    //TMatrix<double>  controller;// on [0,1], to be weighted by [-1,1]
    //TVector<double>  actuator;// on [0,1]
  
    };// end agentinterface class
