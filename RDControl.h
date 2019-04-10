//*****************************************************************************
// RDController: Simulates a reaction diffusion system 
//
// Sam Migirditch, migirditch@gmail.com
//
// 03/01/2019: Created
//
//*****************************************************************************

// **************************** 
// Includes
// ****************************

// Std. libs
#include <iostream>
//#include <math.h>

// Headers
#include "VectorMatrix.h"
//#include "random.h"

#pragma once

// **************************** 
// RDControl Class Declaration
// ****************************

class RDControl {
    public:
        // Constructor
        RDControl( int size, int model );
        // Destructor
        ~RDControl();

        // Accessors
        int ReactorSize(void);
        void SetStepSize(double newsize);
        double GetStepSize();
        TVector<double> CellState( int cellindx );
  
        // Single Cell Control
        void NormalizeCellDensity( int cellindx );
        void SetCellState( TVector<double> newstate, int cellindx);
        void InjectCell( double amount, int chemindx, int cellindx);

        // Global Cell Controls
        void NormalizeReactorState();
        void RandomizeReactorState();
        void HomogenousReactorState();

        // Global Topology Control
        void SetReactorSize( int newsize  );
        void SetReactorTopology();

        // Dynamics
        void TimeStepEuler();
        void Reaction();
        void Diffusion( int target );


        // Data Objects
        int size; // number of RD cells 
        double cellsize = 1.0/size;
        int model; // model index, Grey-Scott, BZ etc. 
        int chemnum; // number of chemicals
        int paramnum; //number of parameters in RD Model
        double timestepsize = 0.1;
        
        TVector<double> rdparameter;
        TVector<double> diffvec; // a vector for passing the chem diffusion changes around

        TMatrix<double> adjacency; // Weighted Cell adjacency mtrx
        TMatrix<double> cellstate; // reactorsize by chemicalspeciesnumber mtrx
};
