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
    int size, model, topoindx;
    double timelimit = 10.0;
    double stepsize = 1;

    cout<<"Enter RD Controler Size as integer:0";
    //cin >> size;
    size = 9;
    
    cout << "Enter RD Model:\n 0-> Gray-Scott \n____________\n";
    //cin >> model;
    model = 0;

    // Initialize Reaction Diffusion Controller
    RDControl RD(size, model);
    cout<<"RDControler Initialilzed"<<endl;
    
    // Apply a 1D euclidean ring geometry
    cout<<"Enter topology index:\n0:1D NN ring\n1:2D Cardinal NN torus\n____________\n"<<endl;
    //cin>>topoindx;
    topoindx = 0;
    RD.SetReactorTopology(topoindx);
    cout<<"Reactor Topology:"<<endl;
    cout<<RD.adjacency<<endl;

    // Fill reactor
    RD.HomogenousReactorState();

    // Check fill
    cout<<"cellstate pre out:\n"<<RD.cellstate<<endl;
    RD.InjectCell( 10.0, 0, 0 );
    cout<<"Spiked cell state:"<< RD.cellstate<<endl;
    
    // Grind
    for (double time=0.0; time<timelimit; time+=stepsize)
    {
        cout<<"TIME:"<<time<<endl;
        RD.EulerStep( stepsize );
        RD.InjectCell(0.5, 1, 1);
        RD.InjectCell(0.7, 0, 0);

    }

    //print state
    cout<<"post out:\n"<<RD.cellstate<<endl;
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
    cout<<"]\n";
}
