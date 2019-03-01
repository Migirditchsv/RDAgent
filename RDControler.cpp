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
//#include <math.h>

// Headers
#include "RDControl.h"
//#include "VectorMatrix.h"
//#include "random.h"

// **************************** 
// Constructors and Destructors
// ****************************

// Construct reactor

RDControl::RDControl( int CellNum )
{
    SetCellNum( CellNum );
}

// Destruct reactor

RDControl::~RDControl()
{
    SetCellNum( 0 );
}
