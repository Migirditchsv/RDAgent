// Testbed for RDControl Development

//Includes
#include <iostream> // cout, cin
#include <cmath> // fmod, 

//Headers
#include "RDControl.h"

//Identifiers

void PrettyPrint(TMatrix<double> &mtrx, int printindx);

int main()
{
    using namespace std;
    int size, model, topoindx;
    double timelimit = 100.0;
    double stepsize = 0.1;
    double printfreq = 10;

    cout<<"Enter RD Controler Size as integer:0";
    //cin >> size;
    size = 9;
    
    cout << "Enter RD Model:\n 0-> Gray-Scott \n____________\n";
    //cin >> model;
    model = 0;

    // Initialize Reaction Diffusion Controller
    RDControl RD(size, model);
    cout<<"RDControler Initialilzed"<<endl;
    cout<<"MAIN| rdparam:"<<RD.rdparameter<<endl;
    
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
    RD.InjectCell( 1.0, 0, 0 );
    cout<<"Spiked cell state:"<< RD.cellstate<<endl;


    // Grind
    for (double time=0.0; time<timelimit; time+=stepsize)
    {
        RD.EulerStep( stepsize );
        RD.InjectCell(5, 7, 1);
        RD.InjectCell(7, 0, 0);
        //output
        if(fmod(time,printfreq)<stepsize  )
        {
        cout<<"TIME:"<<time<<endl;
        cout<<"State:\n"<<RD.cellstate<<endl;
        }
    }
    return(0);
}

