//*****************************************************************************
// PerceptronInterface: Uses perceptrons to link:
// (ENVIRONMENNT)->[IN-PERCEPTRONS]->(Controller)->[Out-Perceptrons]->(Actuators)
// 
// Where Environment and Actuator are TVector objects and Controller is a
// TMatrix object. The Controler TMatrix is interpreted as (element,channel) as
// this package is designed to read single channel signals into and out of a
// controller object whose individual elements combine information from 
// multiple chanels and share the result in a diffusive manner. 
//
// Sam Migirditch, migirditch@gmail.com
//
// 05/16/2019: Created
//
//*****************************************************************************

// **************************** 
// Includes
// ****************************

// Std. libs
#include <iostream>// Debug cout,
//#include <cmath>

// Headers
#include "VectorMatrix.h"
#include "AgentInterface.h"
//#include "random.h"

