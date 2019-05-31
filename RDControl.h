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
        RDControl( int size, int model )
        {
            SetReactorSize( size );

            // Model 0: Grey-Scott
            if ( 0 == 0)
            {
                //Debug
                cout<<"Gray-Scott Model Initialzing"<<endl;
                cellsize = 1.0 / size;
                chemnum = 2;
                paramnum = 4;
                //model = 0;

                cellstate.SetBounds(1,size,1,chemnum);

                rdparameter.SetBounds(1,paramnum);
                rdparameter(1)=0.055; //k Dale&Husbands 2010
                rdparameter(2)=0.02; //F Dale&Husbands 2010
                rdparameter(3)=2.0*pow(10.0,-5.0); //du Dale&Husbands 2010
                rdparameter(4)=pow(10.0,-5.0); //dv Dale&Husbands 2010
            }

        diffvec.SetBounds(1,chemnum);
        diffvec.FillContents(0);
       }
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
