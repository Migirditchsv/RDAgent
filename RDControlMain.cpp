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
    int size;

    cout<<"Enter RD Controler Size as integer:";
    cin >> size;

    RDControl RD(size);
    cout<<"RDControler inited"<<endl;
    
    RD.SetReactorTopology();
    cout<<"Reactor Topology Set"<<endl;
    cout<<RD.adjacency<<endl;
    PrettyPrint(RD.adjacency);

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
