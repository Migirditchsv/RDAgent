// What is up with TMatricies randomly transposing?
// TMatrix << overload isn't causing it...
// 

#include <iostream>

#include "VectorMatrix.h"

int main()
{
    using namespace std;

    int ColumnSize=5;
    int colsize=3;

    // Init
    TMatrix<int> mtx;
    
    cout<<"All matricies have a marker at mtx(1,2)=1"<<endl;

    cout<<"init'd matrix should have no size"

    <<"\nmtx.ColumnSize()= "<<mtx.ColumnSize()
    <<"\nmtx.Colsize()= "<<mtx.RowSize()
    <<"\nPrinting mtx:\n"<<mtx<<endl;
    
    // Show that x.ColumnSize() and x.RowSize() return interchanged values
    cout<<"Setting mtx to be a "<<ColumnSize<<" x "<<colsize<<" matrix via the command:\nmtx.SetBounds( 1 , ColumnSize="
    <<ColumnSize<<" , 1 ,  colsize="<<colsize<<" )"<<endl;
    mtx.SetBounds(1,ColumnSize,1,colsize);

    int rowtruth = mtx.ColumnSize()==ColumnSize;
    int coltruth = mtx.RowSize()==colsize;
    mtx.FillContents(0);
    mtx(1,2)=1;
    cout<<"Sanity check: mtx.ColumnSize()==ColumnSize = "<<rowtruth<<endl;
    cout<<"Sanity check: mtx.RowSize()==RowSize = "<<coltruth<<endl;
    
    cout<<"\nmtx.ColumnSize()= "<<mtx.ColumnSize()
    <<"\nmtx.Colsize()= "<<mtx.RowSize()
    <<"\nPrinting mtx:\n"<<mtx<<endl;
 
    // Show that it's not just (col,row) notation, but that the x.Row/ColSize() functions are out of sync
    // with the x.SetBounds() funciton
    cout<<"\n\nSetting mtx to be a "<<ColumnSize<<" x "<<colsize<<" matrix via the command:\nmtx.SetBounds( 1 , colsize="
    <<colsize<<" , 1 ,  ColumnSize="<<ColumnSize<<" )"<<endl;
    mtx.SetBounds(1,colsize,1,ColumnSize);
    mtx.FillContents(0);
    mtx(1,2)=1;

    rowtruth = mtx.ColumnSize()==ColumnSize;
    coltruth = mtx.RowSize()==colsize;
    cout<<"Sanity check: mtx.ColumnSize()==ColumnSize = "<<rowtruth<<endl;
    cout<<"Sanity check: mtx.RowSize()==RowSize = "<<coltruth<<endl;
    
    cout<<"\nmtx.ColumnSize()= "<<mtx.ColumnSize()
    <<"\nmtx.Colsize()= "<<mtx.RowSize()
    <<"\nPrinting mtx:\n"<<mtx<<endl;
 
    // Iterate
    mtx.SetBounds(1,50,1,2);
    int tracer = 1;
    for(int r=1; r<=50; r++)
    {
        for(int c=1; c<=2; c++)
        {
            mtx(r,c)=tracer;
            tracer++;
        }
    }
    cout<<"Iterated matrix:\n"<<mtx<<endl;
}

