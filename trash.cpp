// sandbox

#include <iostream>
#include <vector>

#include "VectorMatrix.h"

int main()
{
    using namespace std;

    TVector<int> vec;
    TMatrix<int> mtx;


    cout<<"Initialized vector should have size zero\nvector.Size()= "<<vec.Size()
    <<"Print vector:"<<vec<<endl;
    cout<<"Initialized matrix should have dimensions 0x0\n

    testvec1.SetBounds(1,3);
    testmtrx1.SetBounds(1,3,1,5);

    cout<<"flag2 testvec1.size: "<<testvec1.Size()<<endl;
    cout<<testvec1<<endl;
    cout<<"flag2 testmtrx1.RowSize :"<<testmtrx1.RowSize()<<" testmtrx.ColumnSize: "<<testmtrx1.ColumnSize()<<endl;
    cout<<testmtrx1<<endl;

    testvec1.SetBounds(1,9);
    testmtrx1.SetBounds(1,4,1,9);

    cout<<"flag3 testvec1.size: "<<testvec1.Size()<<endl;
    cout<<testvec1<<endl;
    cout<<"flag3 testmtrx1.RowSize: "<<testmtrx1.RowSize()<<" testmtrx.ColumnSize: "<<testmtrx1.ColumnSize()<<endl;
    cout<<testmtrx1<<endl;

    testvec1.SetBounds(1,1);
    testmtrx1.SetBounds(1,9,1,4);
    testmtrx1.FillContents(0);

    cout<<"flag4 testvec1.size: "<<testvec1.Size()<<endl;
    cout<<testvec1<<endl;
    cout<<"flag4 testmtrx1.RowSize: "<<testmtrx1.RowSize()<<" testmtrx.ColumnSize: "<<testmtrx1.ColumnSize()<<endl;
    cout<<testmtrx1<<endl;


    testvec1.SetBounds(1,1);
    testmtrx1.SetBounds(1,2,1,2);

    cout<<"flag5 testvec1.size: "<<testvec1.Size()<<endl;
    cout<<testvec1<<endl;
    cout<<"flag5 testmtrx1.RowSize :"<<testmtrx1.RowSize()<<" testmtrx.ColumnSize: "<<testmtrx1.ColumnSize()<<endl;
    cout<<testmtrx1<<endl;

    int sizecheck1 = 78;
    int sizecheck2 = 43;
    testmtrx1.SetRowSize(sizecheck1);
    testmtrx1.SetColumnSize(sizecheck2);
    int truth1 = testmtrx1.RowSize() == sizecheck1 && testmtrx1.ColumnSize() == sizecheck2;
    cout<<" Truth check size increasing: "<< truth1 << endl;
    sizecheck1 = 2;
    sizecheck2 = 3;
    testmtrx1.SetRowSize(sizecheck1);
    testmtrx1.SetColumnSize(sizecheck2);
    truth1 = testmtrx1.RowSize() == sizecheck1 && testmtrx1.ColumnSize() == sizecheck2;
    cout<<" Truth check size decreasing: "<< truth1 << endl;
    truth1 = testmtrx1.RowSize() == testmtrx1.RowSize();
    cout<<" GROUND TRUTH CHECK: "<< truth1 << endl;

}

