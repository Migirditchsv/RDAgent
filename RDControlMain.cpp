// Testbed for RDControl Development

//Includes
#include <iostream>

//Headers
#include "RDControl.h"

//Identifiers

void PrettyPrint(TMatrix<double> &mtrx, int printindx);

int main()
{
    using namespace std;
    int size, model;
    double timelimit = 10.0;
    double stepsize = 0.01;

    cout<<"Enter RD Controler Size as integer:";
    cin >> size;
    
    cout << "Enter RD Controler Model (0: GS euclidean ring w/ Nearest Neighbor)";
    cin >> model;

    // Initialize Reaction Diffusion Controller
    RDControl RD(size, model);
    cout<<"RDControler inited"<<endl;
    
    // Apply a 1D euclidean ring geometry
    RD.SetReactorTopology();
    cout<<"Reactor Topology Set"<<endl;
    cout<<RD.adjacency<<endl;

    // Fill reactor
    RD.RandomReactorState();

    // Check fill
    PrettyPrint( RD.cellstate, 0 );
    PrettyPrint( RD.cellstate, 1 );

    
    // Grind
    for (double time=0.0; time<timelimit; time+=stepsize)
    {
        //cout<<"TIME:"<<time<<endl;
        RD.EulerStep( stepsize );
        RD.InjectCell(100, 1, 0);
    }

    //print state
    //cout<<RD.cellstate<<endl;
    PrettyPrint( RD.cellstate, 0 );
    PrettyPrint( RD.cellstate, 1 );
    return(size);
}

void PrettyPrint(TMatrix<double> &mtrx, int printindx)
{
    int size = mtrx.ColumnSize();
    int rsize = mtrx.RowSize();
    cout<<"r="<<rsize<<"c="<<size<<endl;
    cout<<"[";
    for (int r = 0; r <= size; r++)
    {
        cout<< mtrx[printindx][r]<<" ";
    }
    cout<<"]";
}
