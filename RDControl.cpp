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

// Headers
#include "RDControl.h"
#include "VectorMatrix.h"
#include "random.h"

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

//Set Time Step Size
void RDControl::SetStepSize(double newsize)
{
    timestepsize=newsize;
}

double RDControl::GetStepSize()
{
    return(timestepsize);
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

// Randomize reactor state
void RDControl::RandomizeReactorState()
{
    // Vars
    double holder, counter=0.0;
    
    // Loop over all cells
    for (int target=0; target<size; target++)
   {
       for ( int chemindx = 0; chemindx<chemnum; chemindx++)
       {
            holder = UniformRandom(0.0,1.0);
            counter += holder;
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

//-----------------------------
// DYNAMICS
//----------------------------

// Reaction Model
void RDControl::Reaction()
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
        //diffvec.SetBounds(0,2);
        double u,v; // pre-step values of u&v used in step calculation
        double du, dv; // change in u and v

        for (int target=0; target<size; target++)
        {
            u = cellstate(target, 0);
            v = cellstate(target, 1);

            diffvec = Diffusion(target); // Why doesn't this work?? 
            // Find cell concentration changes
            du = gammau*diffvec(0)-u*pow(v,2)+f*(1.0-u);
            dv = gammav*diffvec(1)-u*pow(v,2)-(f+k)*v;

            // inject changes into cell
            cellstate(target, 0)+=du*timestepsize;
            cellstate(target, 1)+=dv*timestepsize;

            // Normalize cell
            NormalizeCellDensity(target);
        }
    }
}

// Computes Diffusion-time rate for all species for each cell
TVector<double> RDControl::Diffusion(int target)
{
    //internal declarations
    TVector<double> dchem;
    dchem.SetBounds(0,chemnum);
    dchem.FillContents(0.0);
    int neighborcount=0;
    double weight;
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
           dchem(chemindx) = weight*(neighborchem-targetchem);
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
   return dchem;
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
