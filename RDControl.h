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
        RDControl( int size );
        // Destructor
        ~RDControl();

        // Accessors
        int ReactorSize(void);
        TVector<double> CellState( int cellindx );
  
        // Single Cell Control
        void NormalizeCellDensity( int cellindx );
        void SetCellState( TVector<double> newstate, int cellindx);
        void InjectChemical( double amount, int chemindx, int cellindx);

         // Global Cell Controls
        void RandomizeReactorState();

        // Global Topology Control
        void SetReactorSize( int newsize  );
        void SetReactorTopology();

        // Dynamics
        void RDTimeStepEuler();
        void Reaction();
        TVector<double> Diffusion();


        // Objects
        int size; // number of RD cells 
        int chemnum; // number of chemicals
        
        TMatrix<double> adjacency; // Weighted Cell adjacency mtrx
        TMatrix<double> cellstate; // reactorsize by chemicalspeciesnumber mtrx
};
