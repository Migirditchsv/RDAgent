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
int controllersize = Controller.size; 
TMatrix<double> controllerstate = Controller.cellstate;
cout<<"Controller Size:" <<controllersize<<endl;
cout<<"Controller State:"<<controllerstate<<endl;

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
    if (randomize) Controller.RandomReactorState;
    else Controller.HomogenousReactorState();
    ResetRays();
}

//void RDAgent::Reset(RandomState &rs, double ix, double iy, int randomize) {
//  cx = ix; cy = iy; vx = 0;
//  if (randomize) Controller.RandomizeReactorState();
//  else Controller.HomogenousCircuitState();
//  ResetRays();
//}

void RDAgent::ResetRays() {
  double theta = -VisualAngle/2;  
  for (int i=1; i<=NumRays; i++) {
    ResetRay(Rays[i], theta, cx, cy);
    theta += VisualAngle/(NumRays-1);
  }
}

// Step the agent

void RDAgent::Step(double controldt,// time delta on controller
                   double ctlimit,// limit on controller steps per agent step
                   double agentdt,// time delta on agent motion
                   VisualObject &object)// the object being looked for
{ 
    // Update visual sensors and check inputs
    ResetRays();
    for (int p=0; p<Interface.inperceptronnum; p++)
    {
      //vars
      int rayindx, targetindx, channelindx;
      double externalinput;
      vector<perceptron> localperceptron = Interface.inperceptron;
      localperceptron[0].state = 666.6;
      int sizeish = localperceptron[0].source.Size(); //OK this is how you've got to index
      // link perc to sensor
      rayindx = Interface.inperceptron;//(p).source(1);//which ray feeds this perceptron
      // detect pointed-to object
      object.RayIntersection(Rays[rayindx]);
      externalinput = (MaxRayLength - Rays[rayindx].length)/MaxRayLength;
      // inject detected signal
      for(int target=1; target<=Interface.maxlinknum; target++)
      {
        targetindx = Interface.inperceptron(p).target(t);
        //check if placeholder
        if(targetindx<=0){goto skip;}

        channelindx = Interface.inperceptron(p).channel(t);
        Controller.InjectCell(externalinput,channelindx,targetindx);

        skip:;// from placeholder index skip condition
      }
    } 

    // Read Sensors into Controller

    // Step Controller
    double ct = 0.0;
    while( ct<=ctlimit)
    {
    Controller.EulerStep(controldt);
    ct+=controldt;
    }
    // Read Out from controller
        // sum perceptron weights and find net actuator forces
        // return vx

    // Update agent state
    // replace with output reader having cell-actuator linker
    //vx = VelGain*(Controller.outputs[13] - Controller.outputs[14]);
    cx = cx + StepSize*vx;
    if (cx < -EnvWidth/2) {
      cx = -EnvWidth/2;
    } else if (cx > EnvWidth/2) {
      cx = EnvWidth/2;
  }
}
