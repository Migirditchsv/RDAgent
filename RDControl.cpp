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
    if ( 0 == 0)
    {
        //Debug
        cout<<"Gray-Scott Model Initialzing"<<endl;
        cellsize = 1.0 / size;
        chemnum = 1;
        paramnum = 4;
        model = 0;

        cellstate.SetBounds(1,size,1,chemnum);

        rdparameter.SetBounds(1,paramnum);
        rdparameter(1)=0.055; //k Dale&Husbands 2010
        rdparameter(2)=0.02; //F Dale&Husbands 2010
        rdparameter(3)=2.0*pow(10.0,-5.0); //du Dale&Husbands 2010
        rdparameter(4)=pow(10.0,-5.0); //dv Dale&Husbands 2010

        //check rdpaarm
        cout<<"CONSTRUCT:"<<rdparameter<<endl;

        diffvec.SetBounds(1,chemnum);
        diffvec.FillContents(0);
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
    states.SetBounds(1,chemnum);
    states.FillContents(0.0);

    // loop over chem indx
    for (int chemindx=1; chemindx<=chemnum; chemindx++)
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
    for (int chemindx=1;chemindx<=chemnum;chemindx++)
    {
        totalchem += cellstate(cellindx,chemindx);
    }

    // Division Safety
    if (totalchem<=0)
    {
        cerr<<"ERROR! RDControl.cpp>NormalizeCellDensity: div by totalchem=0";
        exit(0);
    }

    inversetotal = 1.0/totalchem;

    // normalize each chem by total chem
    for (int chemindx=1;chemindx<=chemnum;chemindx++)
    {
       cellstate(cellindx,chemindx)*=inversetotal;
    }
}

// Sets the chemical state of a cell
void RDControl::SetCellState(TVector<double> newstate, int cellindx)
{
    for (int chemindx=1;chemindx<=chemnum;chemindx++)
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
    for (int target=1; target<=size; target++)
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
    std::uniform_real_distribution<> dis(0.0, 1.0);
    // Loop over all cells
    for (int target=1; target<=size; target++)
    {
       for ( int chemindx=1; chemindx<=chemnum; chemindx++)
       {
            holder = 0.5;//dis(gen);
            cellstate(target, chemindx) = holder;
       }    
   }
    // Normalize to mass density 1
    NormalizeReactorState();
}

// Set every cell to be 100% the chemical at index 0
void RDControl::HomogenousReactorState()
{
    for (int target=1; target<=size; target++)
    {
        for (int chemindx=1; chemindx<=chemnum; chemindx++)
        {
            if (chemindx==0){cellstate(target,chemindx)=0.5;}
            else{cellstate(target,chemindx)=0.5;}
        }
    }
}

//------------------------------
// Global Topology Controls
//------------------------------

// Resize a reactor
void RDControl::SetReactorSize( int newsize )
{
    size = newsize;
    cellstate.SetBounds(1,size,1,chemnum);
    cellstate.FillContents(0.0);
    adjacency.SetBounds(1,size,1,size);
    adjacency.FillContents(0.0);
}

// Set reactor topology
// 0: 1D Euclidean Nearest Neighbor Ring
// 1: 2D Euclidean Cardinal NN torus (size has gotta be a square number!!)
void RDControl::SetReactorTopology(int topologyindx)
{
    // Index 0: 1D Euclidian Nearest Neighbor Ring
    if (topologyindx==0)
    {
        for (int r = 2; r <= size-1; r++)
        {
            adjacency[r][r-1]     = 1.0;
            adjacency[r][r+1]     = 1.0;
        }
        // Fix edge cases
        adjacency[1][size]      = 1.0;
        adjacency[1][2]         = 1.0;
        adjacency[size][1]      = 1.0;
        adjacency[size][size-1] = 1.0;
    }

    // Index 1: 2D Cardinal NN torus
    if (topologyindx == 1)
    {
        // warn user if stupid
        double check = sqrt(size);
        if ( ceil(check)!=floor(check) )
        {
            cout<<"ERROR: RDControl.cpp-SetReactorTopology(indx): array size must be square number to use Index 1: 2D Cardinal NN torus"<<endl;
            exit(0);
        }

        // Vars
        int tr, tc, nr, nc; // target and neighbor row and col respectively
        int width = sqrt(size); // width of square is sqrt of size
        int value; // 1 or 0 for topology

                for( int target=1; target<=size; target++ )
        {
            tr = target/width;
            tc = target%width;

            for( int neighbor=1; neighbor<=size; neighbor++ )
            {
                nr = neighbor/width;
                nc = neighbor%width;

                // cardinal nearest neigbor conditional
                value = (tc==nc) and ( abs(tr-nr)==1 or abs(tr-nr)==width-1 );
                value += (tr==nr) and ( abs(tc-nc)==1 or abs(tc-nc)==width-1 );

                adjacency[target][neighbor]=value>0;
            }
        }
    }
}

//-----------------------------
// DYNAMICS
//----------------------------

// Reaction Model
void RDControl::EulerStep( double timestepsize )
{
    //Grey-Scott RD model
    if (1)//(model==0)
    {
        //params
        double k = rdparameter(1);
        double f = rdparameter(2);
        double gammau = rdparameter(3);// diffusion coeff of chem u
        double gammav = rdparameter(4);// diffusion coeff of chem v
        TMatrix<double> syncstate = cellstate; // for synchronous update
        double u,v; // pre-step values of u&v used in step calculation
        double du, dv; // change in u and v

        //cout<<rdparameter<<cout;
        
//        //Check sycn state == cell state
//        int flag = 1;
//        int crows = cellstate.RowSize();
//        int ccols = cellstate.ColumnSize();
//        int srows = syncstate.RowSize();
//        int scols = syncstate.ColumnSize();
//        //cout<<"crows:"<<crows<<"Ccolls:"<<ccols<<"srows:"<<srows<<"scolls:"<<scols<<endl;
// 
//        for( int r = 0; r<=crows-1; r++ )
//        {
//            for( int c = 0; c<=ccols-1; c++ )
//            {
//                flag *= syncstate(c,r) == cellstate(c,r);
//            }
//            if(flag==0)
//            {
//                cout<<"ERROR: RDCONTROL::EULERSTEP->SYNCSTATE FAILED"<<endl;
//                exit(0);
//            }
//        }

        for (int target=1; target<=size; target++)
        {
            u = cellstate(target, 1);
            v = cellstate(target, 2);

            Diffusion(target);

            // Find cell concentration changes
            du = gammau*diffvec(1)-u*pow(v,2)+f*(1.0-u);
            dv = gammav*diffvec(2)+u*pow(v,2)-(f+k)*v;
            
            //debug
            //cout<<"EULERSTEP| du:"<<du<<"dv:"<<dv<<endl;
            
            // inject changes into cell
            syncstate(target, 1)+=du*timestepsize;
            syncstate(target, 2)+=dv*timestepsize;
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
    double weight; 
    // ^^  diffusion network weight. Not the chemical species diffusion rate 
    // but the spatial diffusion rate between cell i and j.
    double neighborchem;
    double targetchem;

    
    diffvec.FillContents(0);

   for (int neighbor=1; neighbor<=size; neighbor++)
   {
        weight = adjacency(target, neighbor);
        if (weight==0.0)
        {
            continue;
        } 

        for ( int chemindx=1; chemindx<=chemnum; chemindx++)
        {
            //cout<<"DIFF| Targ:"<<target<<"Neig:"<<neighbor<<"Chem:"<<chemindx<<"Weight:"<<weight<<endl;
            neighborchem = cellstate(neighbor, chemindx);
            targetchem = cellstate(target, chemindx);
            diffvec(chemindx) += weight*(neighborchem-targetchem);
       }
   }
   // laplacian= dt*D( Sum(c_n) - N*c_t ) for each chem
   for (int chemindx=1; chemindx<=chemnum; chemindx++)
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
