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
#include <cmath>
#include <random>

// Headers
#include "RDControl.h"
#include "VectorMatrix.h"
//#include "random.h"

using namespace std;

// **************************** 
// Constructors and Destructors
// ****************************

// Construct reactor

RDControl::RDControl( int size, int model )
{
    SetReactorSize( size );

    // Model 0: Grey-Scott
    if (model == 0)
    {
        cellsize = 1.0 / size;
        chemnum = 2;
        paramnum = 4;

        rdparameter.SetBounds(0,paramnum);
        rdparameter(0)=0.055; //k Dale&Husbands 2010
        rdparameter(1)=0.02; //F Dale&Husbands 2010
        rdparameter(2)=2.0*pow(10.0,-5.0); //du Dale&Husbands 2010
        rdparameter(3)=pow(10.0,-5.0); //dv Dale&Husbands 2010
    }
}

// Destruct reactor

RDControl::~RDControl()
{
    SetReactorSize( 0 );
    rdparameter.SetBounds(0,0);
    cellstate.SetBounds(0,0,0,0);
}

// **************************** 
// Utilities
// ****************************

//-----------------------------
// ACCESSORS
//----------------------------

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

//-----------------------------
// SINGLE CELL CONTROL
//-----------------------------

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

// Inject an amount of a chemical into a single cell
void RDControl::InjectCell(double amount, int cellindx, int chemindx)
{
    cellstate(cellindx,chemindx)+=amount;
    NormalizeCellDensity(cellindx);
}

//-----------------------------
// Global Cell Control
// ----------------------------

// Normalize entire controller
void RDControl::NormalizeReactorState()
{
    for (int target=0; target<size; target++)
    {
        NormalizeCellDensity(target);
    }
}

// Randomize reactor state
void RDControl::RandomReactorState()
{
    // Vars
    double holder;
    
    //Randomness
    std::random_device rd;  //seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(1.0, 2.0);
    // Loop over all cells
    for (int target=0; target<size; target++)
    {
       for ( int chemindx = 0; chemindx<chemnum; chemindx++)
       {
            holder = dis(gen);
            cellstate(target, chemindx)= holder;
       }    
       NormalizeCellDensity(target);
   }
}

// Set every cell to be 100% the chemical at index 0
void RDControl::HomogenousReactorState()
{
    for (int target=0; target<size; target++)
    {
        for (int chemindx=0; chemindx<chemnum; chemindx++)
        {
            if (chemindx==0){cellstate(target,chemindx)=1.0;}
            else{cellstate(target,chemindx)=0.0;}
        }
    }
}

//------------------------------
// Global Topology Controls
//------------------------------

// Resize a reactor
void RDControl::SetReactorSize( int newsize )
{
    //cout<<"RDControl.cpp: Initing new reactor"<<endl;//debug
    size = newsize;
    //cout<<"RDControl.cpp: newi size set"<<endl;//debug
    cellstate.SetBounds(0,size,0,chemnum);
    //cout<<"RDControl.cpp: state bounds set"<<endl;//debug
    cellstate.FillContents(0.0);
    //cout<<"RDControl.cpp: a and b state zero filled"<<endl;//debug
    adjacency.SetBounds(0,size,0,size);
    //cout<<"RDControl.cpp: adjacency bounds set"<<endl;//debug
    adjacency.FillContents(0.0);
    //cout<<"RDControl.cpp: adjacency zero filled "<<endl;//debug
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
    //cout<<"RDControl.cpp: adjacency main block filled"<<endl;//debug
    // Fix edge cases
    adjacency[0][size]      = 1.0;
    adjacency[0][1]         = 1.0;
    adjacency[size][0]      = 1.0;
    adjacency[size][size-1] = 1.0;
    //cout<<"RDControl.cpp: adjacency edge cases filled"<<endl;//debug

}

//-----------------------------
// DYNAMICS
//----------------------------

// Reaction Model
void RDControl::EulerStep( double timestepsize )
{
    //Grey-Scott RD model
    if (model==0)
    {
        //params
        double k  = rdparameter[0];
        double f  = rdparameter[1];
        double gammau = rdparameter[2];// diffusion coeff of chem u
        double gammav = rdparameter[3];// diffusion coeff of chem v
        TVector<double> diffvec;
        TMatrix<double> syncstate = cellstate; // for synchronous update
        double u,v; // pre-step values of u&v used in step calculation
        double du, dv; // change in u and v

        for (int target=0; target<size; target++)
        {
            u = cellstate(target, 0);
            v = cellstate(target, 1);

            Diffusion(target); // Why doesn't this work?? 
            // Find cell concentration changes
            du = gammau*diffvec(0)-u*pow(v,2)+f*(1.0-u);
            dv = gammav*diffvec(1)-u*pow(v,2)-(f+k)*v;

            // inject changes into cell
            syncstate(target, 0)+=du*timestepsize;
            syncstate(target, 1)+=dv*timestepsize;
        }
        // copy back to cellstate
        cellstate = syncstate;
        // Normalize controller state
        NormalizeReactorState();
    }
}

// Computes Diffusion-time rate for all species for each cell
void RDControl::Diffusion(int target)
{
    //internal declarations
    int neighborcount=0;
    double weight; 
    // ^^  diffusion network weight. Not the chemical species diffusion rate 
    // but the spatial diffusion rate between cell i and j.
    double neighborchem;
    double targetchem;

   for (int neighbor=0; neighbor<size; neighbor++)
   {
       weight = adjacency(neighbor, target);
       if (weight==0.0){continue;}
       neighborcount++;
       for ( int chemindx = 0; chemindx<chemnum; chemindx++)
       {
           neighborchem = cellstate(neighbor, chemindx);
           targetchem = cellstate(target, chemindx);
           diffvec(chemindx) += weight*(neighborchem-targetchem);
       }
   }
   // laplacian= dt*D( Sum(c_n) - N*c_t ) for each chem
   for (int chemindx = 0; chemindx<chemnum; chemindx++)
   {
       diffvec(chemindx) *= cellsize;
   }
}

//// Diffusion Time Step: Extra Crude And super not optimized Euler method
//void RDControl::TimeStepEuler()
//{
//    double weight;
//    
//    for (int r = 0; r<=size-1; r++)
//    {
//        for (int c = 0; c<=size-1; c++)
//        {
//            if (adjacency[r][c]!=0.0)
//            {
//                weight = adjacency[r][c];
//
//            }
//        }
//    }
//}

// Topology setting function
//void TopologyLibrary(int topoindx)
//{
//    // TOPOLOGY INDEX:
//    // 0: disconnected
//    // 1: fully connected
//    // 2: 1D ring nearest neighbor
//
//}
