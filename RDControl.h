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
//#include <cmath>

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
        RDControl()
        {}

        // Destruct reactor
        ~RDControl()
        {
           // SetReactorSize( 0 );
            //rdparameter.SetBounds(0,0);
            //cellstate.SetBounds(0,0,0,0);
        }



        // Accessors
        int GetReactorSize();
        TMatrix<double>& GetReactorState();
        void SetReactorSize(int newsize);
        int GetParameterNumber();
        TVector<double>& GetRDParameters();
        void SetParameter( int indx, double newparameter);
        int GetChemicalNumber();//set determined by model

        TVector<double> GetCellState( int cellindx );
        double CellStateChannel( int cellndx, int channel);
  
        // Single Cell Control
        void NormalizeCellDensity( int cellindx );
        void SetCellState( TVector<double> newstate, int cellindx);
        void InjectCell( double amount, int chemindx, int cellindx);

        // Global Cell Controls
        void SetRDModel(int modelindx);
        void NormalizeReactorState();
        void RandomReactorState();
        void HomogenousReactorState();

        // Global Topology Control
        void SetReactorTopology( int topologyindex );

        // Dynamics
        void EulerStep( double stepsize );
        void Diffusion( int target );

    private:
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
};
