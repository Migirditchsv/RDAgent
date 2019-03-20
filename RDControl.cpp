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
#include "RDControl.h"
#include "VectorMatrix.h"
//#include "random.h"

using namespace std;

// **************************** 
// Constructors and Destructors
// ****************************

// Construct reactor

RDControl::RDControl( int size )
{
    SetReactorSize( size );
    chemnum = 2;
}

// Destruct reactor

RDControl::~RDControl()
{
    SetReactorSize( 0 );
}

// **************************** 
// Utilities
// ****************************

// ACCESSORS

//Get reactor size
int RDControl::ReactorSize()
{
    return size;
}

//Get Tvector of the concentrations of species in a cell
TVector<double> RDControl::CellState( int cellindx )
{
    // init returnable state vector
    TVector<double> states;
    states.SetBounds(0,chemnum);
    states.FillContents(0.0);

    // loop over chem indx
    for (int chemindx=0; chemindx<chemnum; chemindx++)
    {
        states(chemindx)=cellstate(cellindx,chemindx);
    }
    return states;
}

// SINGLE CELL CONTROL

// Normalize the densities of chemicals in a cell
void RDControl::NormalizeCellDensity( int cellindx )
{
    // init vars
    double totalchem = 0.0;
    double inversetotal = 0.0;
    
    // sum for total chem
    for (int chemindx=0;chemindx<chemnum;chemindx++)
    {
        totalchem = cellstate(cellindx,chemindx);
    }

    // Division Safety
    if (totalchem<=0)
    {
        cerr<<"ERROR! RDControl.cpp>NormalizeCellDensity: div by totalchem=0";
        exit(0);
    }

    inversetotal = 1.0/totalchem;

    // normalize each chem by total chem
    for (int chemindx=0;chemindx<chemnum;chemindx++)
    {
        cellstate(cellindx,chemindx)*=inversetotal;
    }
}

// Sets the chemical state of a cell
void RDControl::SetCellState(TVector<double> newstate, int cellindx)
{
    for (int chemindx=0;chemindx<chemnum;chemindx++)
    {
        cellstate(cellindx,chemindx)=newstate(chemindx);
    }
    NormalizeCellDensity(cellindx);
}

// Resize a reactor
void RDControl::SetReactorSize( int newsize )
{
    cout<<"RDControl.cpp: Initing new reactor"<<endl;//debug
    size = newsize;
    cout<<"RDControl.cpp: newi size set"<<endl;//debug
    cellstate.SetBounds(0,size,0,chemnum);
    cout<<"RDControl.cpp: state bounds set"<<endl;//debug
    cellstate.FillContents(0.0);
    cout<<"RDControl.cpp: a and b state zero filled"<<endl;//debug
    adjacency.SetBounds(0,size,0,size);
    cout<<"RDControl.cpp: adjacency bounds set"<<endl;//debug
    adjacency.FillContents(0.0);
    cout<<"RDControl.cpp: adjacency zero filled "<<endl;//debug
}

// Set reactor topology
void RDControl::SetReactorTopology()
{
    // 1D Euclidian Nearest Neighbor Ring
    for (int r = 1; r <= size-1; r++)
    {
        adjacency[r][r-1]     = 1.0;
        adjacency[r][r+1]     = 1.0;
    }
    cout<<"RDControl.cpp: adjacency main block filled"<<endl;//debug
    // Fix edge cases
    adjacency[0][size]      = 1.0;
    adjacency[0][1]           = 1.0;
    adjacency[size][0]      = 1.0;
    adjacency[size][size-1] = 1.0;
    cout<<"RDControl.cpp: adjacency edge cases filled"<<endl;//debug

}

// DYNAMICS

// Reaction Model
void RDControl::Reaction()
{
    //Grey-Scott RD model
    //params
    double du = 2.0*10**(-5); //diffusion constant
    double dv = 10**(-5); // diffusion constant
    double f  = rdparameter[0];
    double k  = rdparameter[1];

    for (int target=0; target<size; target++)
    {
        
    }
}

// Computes Diffusion-time rate for all species for each cell
TVector<double> RDControl::Diffusion()
{
    //internal declarations
    TVector<double> dchem;
    dchem.SetBounds(0,chemnum);
    dchem.FillContents(0.0);
    int neighborcount=0;

    for (int target=0; target<size; target++)
    {
        neighborcount = 0;
        for (int neighbor=0; neighbor<size; neighbor++)
        {
            if (adjacency(target, neighbor)==0.0){continue;}
            neighborcount++;
            for ( int chemindx = 0; chemindx<chemnum; chemindx++)
            {
                dchem(chemindx)+= cellstate(neighbor, chemindx);
            }
        }
        
        // laplacian= D( Sum(c_n ) - N*c_t ) for each chem
        for (int chemindx = 0; chemindx<chemnum; chemindx++)
        {
            dchem(chemindx) *= timestepsize;
            dchem(chemindx) -= (neighborcount*timestepsize - 1.0)*cellstate(target,chemindx);
            //dchem is now the unnormalized next state of target cell
            // D = 1 should be ok?
            // dt*(D=1)(c_i+c_j-N*c_t)+c_t= (~)(-dt*N-1)c_t
            SetCellState(dchem, target);
            NormalizeCellDensity(target);
        }
    }
}

// Diffusion Time Step: Extra Crude And super not optimized Euler method
void RDControl::RDTimeStepEuler()
{
    double weight;
    
    for (int r = 0; r<=size-1; r++)
    {
        for (int c = 0; c<=size-1; c++)
        {
            if (adjacency[r][c]!=0.0)
            {
                weight = adjacency[r][c];

            }
        }
    }
}

// Topology setting function
void TopologyLibrary(int topoindx)
{
    // TOPOLOGY INDEX:
    // 0: disconnected
    // 1: fully connected
    // 2: 1D ring nearest neighbor

}
