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
//#include "VectorMatrix.h"
//#include "random.h"

#pragma once

// **************************** 
// RDControl Class Declaration
// ****************************

class RDControl {
    public:
        // Constructor
        RDControl( int cellNum );
        // Destructor
        ~RDControl();

        // Accessors
        int     ReactorSize(void) {return size;};
        void    SetReactorSize( int newsize  );
        double  CellState( int cellindx ) { return states[ cellindx ]; };
        
}
