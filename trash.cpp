// What is up with TMatricies randomly transposing?
// TMatrix << overload isn't causing it...
// 

#include <iostream>

#include "VectorMatrix.h"

int main()
{
    using namespace std;

    TMatrix<int> mtx;
    mtx.SetBounds(1,3,1,5);
    mtx.FillContents(0);
    int height = mtx.ColumnSize();
    int width  = mtx.RowSize();
    cout<<"Height = "<<height<<" Width = "<<width<<endl;
    cout<<"mtx\n"<<mtx<<endl;

    mtx.InitializeContents(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
    cout<<"Height = "<<height<<" Width = "<<width<<endl;
    cout<<"mtx\n"<<mtx<<endl;

}

