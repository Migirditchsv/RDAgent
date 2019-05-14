//*****************************************************************************
// RDController: Simulates a reaction diffusion system 
//
// Sam Migirditch, migirditch@gmail.com
//
// 03/01/2019: Created
// 04/15/2019: V1.0 Complete!
//
//*****************************************************************************

// **************************** 
// Includes
// ****************************

// Std. libs
//#include <iostream>
//#include <math.h>

// Headers
#include "VectorMatrix.h"
#include "random.h"

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


        // Data Objects
        int size; // number of RD cells 
        double cellsize; // spatial size of cells 1/size.
        int model; // model index, Grey-Scott, BZ etc. 
        int chemnum; // number of chemicals
        int paramnum; //number of parameters in RD Model
        TVector<double> rdparameter; // reaction and diffusion parameter
        TVector<double> diffvec; // a vector for passing the chem diff rates
        TMatrix<double> adjacency; // Weighted Cell adjacency mtrx
        TMatrix<double> cellstate; // reactorsize by chemicalspeciesnumber mtrx


        // Accessors
        int ReactorSize(void);
        TVector<double> CellState( int cellindx );
  
        // Single Cell Control
        void NormalizeCellDensity( int cellindx );
        void SetCellState( TVector<double> newstate, int cellindx);
        void InjectCell( double amount, int chemindx, int cellindx);

        // Global Cell Controls
        void NormalizeReactorState();
        void RandomReactorState();
        void HomogenousReactorState();

        // Global Topology Control
        void SetReactorSize( int newsize  );
        void SetReactorTopology( int topologyindex );

        // Dynamics
        void EulerStep( double stepsize );
        void Diffusion( int target );

};
