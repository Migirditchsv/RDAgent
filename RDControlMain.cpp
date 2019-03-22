// Testbed for RDControl Development

//Includes
#include <iostream>

//Headers
#include "RDControl.h"

//Identifiers

void PrettyPrint(TMatrix<double> &mtrx);

int main()
{
    using namespace std;
    int size, model;
    double timelimit = 10.0;

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

    // Fill every cell with 100% chemical u, but cell 0 has a 50-50 mix
    RD.HomogenousReactorState();
    RD.InjectCell(1.0,0,1);

    double stepsize = RD.GetStepSize();
    // Grind
    for (double time=0.0; time<timelimit; time+=stepsize)
    {
        RD.Reaction();
    }

    return(size);
}

void PrettyPrint(TMatrix<double> &mtrx)
{
    int size = mtrx.RowSize();
    for (int r = 0; r <= size; r++)
    {
        cout<<"[ "; 
        for (int c = 0; c <= size; c++)
        {
            cout<< mtrx[r][c]<<" "<<endl;
        }
        cout<<"]"<<endl;
    }
}
