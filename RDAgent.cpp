// ***********************************************************
//  A class for Reaction Diffusion controlled agents with
//  simple ray vision-like sensors. Based on code by Matthew
//  Setzler.
//
//  By: Sam Migirditch
//
//  05/19| SVM: Converted from VisualAgent to RDAgent
// ************************************************************

// Std. Libs
#include <iostream>//debuging
//#include <cmath>

// Headers
#include "RDAgent.h"
#include "VectorMatrix.h"//handle vector stuff
//#include "RDControl.h"

using namespace std;

// Utility method for initializing and updating rays

void ResetRay(Ray &ray, double theta, double cx, double cy) {
  if (abs(theta) < 0.0000001) ray.m = INFINITY;  // special case, vertical ray
  else ray.m = 1/tan(theta);
  ray.b = cy - ray.m*cx;
  ray.length = MaxRayLength;

  // Set starting coordinates (i.e. on upper perimeter of agent body)
  if (ray.m == INFINITY) {
    ray.startX = cx;
    ray.startY = cy+BodySize/2;
    return;
  }
  ray.startX = cx + (BodySize/2) * sin(theta);
  ray.startY = cy + (BodySize/2) * cos(theta);
}


// *******
// Accessors
// *******

//  Prints out some status info
void RDAgent::Printer(int linenum)
{
cout<<"|||PRINT STATUS|||"<<endl;
cout<<"Position Number:"<<linenum<<endl;

cout<<"---Controller State---"<<endl;
int controllersize = Controller.GetReactorSize(); 
//TMatrix<double> controllerstate = Controller.GetReactorState();
cout<<"Controller Size:" <<controllersize<<endl;
//cout<<"Controller State:"<<controllerstate<<endl;

cout<<"---Agent State---"<<endl;
double posx = PositionX();
double posy = PositionY();
cout<<"Agent X Position:"<<posx<<endl;
cout<<"Agent Y Position:"<<posy<<endl;
}

// Writes out the data in binary dump
void RDAgent::Writer()
{

}

//// Return the ratio of ray segments formed by an intersection.
//// Return 0 if no intersection.
//double SensorState(int rayindx)
//{
//    object.RayIntersection(Rays[rayindx]);
//    double sensorvalue = (MaxRayLength - Rays[rayindx].length)/MaxRayLength;
//    return sensorvalue;
//}

void RDAgent::SetTimeResolutions(double controldt_,// time delta on controller
                                  double controllimit_,// limit on controller steps per agent step
                                  double agentdt_// time delta on agent motion
)
{
  controldt=controldt_;
  controllimit=controllimit_;
  agentdt=agentdt_;
}

// Change x-position

void RDAgent::SetPositionX(double newx) {
  cx = newx;
  ResetRays();
}

// Reset the state of the agent

void RDAgent::Reset(double ix, double iy, int randomize)
{
    // Vars
    cx = ix; cy = iy; vx = 0.0;
    // Write state
    if (randomize) Controller.RandomReactorState();
    else Controller.HomogenousReactorState();
    ResetRays();
}


void RDAgent::ResetRays() {
  double theta = -VisualAngle/2;  
  for (int i=1; i<=NumRays; i++) {
    ResetRay(Rays[i], theta, cx, cy);
    theta += VisualAngle/(NumRays-1);
  }
}

// Step the agent

void RDAgent::Step(VisualObject &object)// the object being looked for
{ 
  //Read sensors into controller
  ResetRays();// do this first
  Interface.FireInputPerceptrons(object);
  for(int controlstep = 1; controlstep<=controllimit; controlstep++)
  {
    Controller.EulerStep(controldt);
  }
  //Read controller to motors
  Interface.FireOutputPerceptrons();
  cx = cx + agentdt*vx;
  if (cx < -EnvWidth/2) {
    cx = -EnvWidth/2;
  }
  else if (cx > EnvWidth/2)
  {
    cx = EnvWidth/2;
  }
}
///////////////////////OLD BLUPRINT////////////////////////
//  int rayindx, targetindx, channelindx, sourceindx;
//  double externalinput, controlleroutput, sign;
//  //init vars
//  externalinput = 0;
//  controlleroutput = 0;
//  sign = 0;
//  // Update visual sensors and check inputs
//  ResetRays();
//  for (int p=0; p<Interface.inperceptronnum; p++)
//  {
//   // Reset pereptron state
//   Interface.inperceptron[p].state=0.0;
//   // link perc to sensor
//    rayindx = Interface.inperceptron[p].source(1);//which ray feeds this perceptron
//    // detect pointed-to object
//    object.RayIntersection(Rays[rayindx]);
//    externalinput = (MaxRayLength - Rays[rayindx].length)/MaxRayLength;
//    // inject detected signal
//    for(int target=1; target<=Interface.maxlinknum; target++)
//    {
//      targetindx = Interface.inperceptron[p].target(target);
//      //check if placeholder
//      if(targetindx<=0){goto skipin;}
//
//      channelindx = Interface.inperceptron[p].channel;
//      Controller.InjectCell(externalinput,channelindx,targetindx);
//
//      skipin:;// from placeholder target index skip condition
//    }
//  }// End input injection
//
//    // Run RD step
//  for(int controlstep = 1; controlstep<=controllimit; controlstep++)
//  {
//    Controller.EulerStep(controldt);
//  }
//  //read out to motors
//  for(int p=0; p<=Interface.outperceptronnum; p++)
//  {
//    // Clear out perceptron state
//    Interface.outperceptron[p].state=0.0;
//    //accumulate sources
//    for(int source=1; source<=Interface.maxlinknum; source++)
//    {
//      // check for real indicies
//      sourceindx  = Interface.outperceptron[p].source(source);
//      if(sourceindx<=0){goto skipout;}
//      // accumulate
//      channelindx      = Interface.outperceptron[p].channel;
//      controlleroutput = Controller.cellstate(sourceindx,channelindx);
//      Interface.outperceptron[p].state+=controlleroutput;
//
//      skipout:;// from output source skip condition
//    }// end accumulate sources
//    //index into target ( left-right for now)
//    targetindx = Interface.outperceptron[p].target(1);
//    if(targetindx == 1){sign=1.0;}
//    else if (targetindx == 2){sign=-1.0;}
//    else{ cerr<<"RDAgent.cpp:step() outperceptron target out of bounds"<<endl;exit(0);}
//    vx+=sign*Interface.outperceptron[p].state;
//  }// end output perceptron   
//  // Update agent state
//  cx = cx + agentdt*vx;
//  if (cx < -EnvWidth/2) {
//    cx = -EnvWidth/2;
//  } else if (cx > EnvWidth/2) {
//    cx = EnvWidth/2;
//  }


