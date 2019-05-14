//*****************************************************************************
// RDTask: Acts on RDAgent objects and reports their behavior
//
//  By: Sam Migirditch, migirditch@gmail.com
//
// 05/2019: Created
//
//*****************************************************************************

// **************************** 
// Includes
// ****************************

// Std. Libs
#include<iostream>//debuging, realtime status update

// Headers
#include "RDAgent.h"

// **************************** 
// Control  Vars
// ****************************

const int steplimit = 1000;

int main()
{
    // Local Vars
    int x = 0;

    // Init Agent
    RDAgent agent();

    // Dynamics
    for(int step=1; step<=steplimit; step++)
    {
        x=0;

    }
    

    
    return(0);
}


