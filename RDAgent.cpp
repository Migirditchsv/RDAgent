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
  cout<<"\n|||PRINT STATUS|||\n"<<flush;
  cout<<"Position Number: "<<linenum<<"\n\n"<<flush;
  
  cout<<"---Controller State---"<<"\n"<<flush;
  int controllersize = Controller.GetReactorSize(); 
  cout<<"Controller Size: " <<controllersize<<"\n"<<flush;
  int controllerchannels = Controller.GetChemicalNumber();
  cout<<"Controller Channels: "<<controllerchannels<<"\n\n"<<flush;
  //TMatrix<double> controllerstate = Controller.GetReactorState();
  //cout<<"Controller State:"<<controllerstate<<endl;
  
  cout<<"---Agent State---\n"<<flush;
  double posx = PositionX();
  double posy = PositionY();
  cout<<"Agent X Position:"<<posx<<"\n"<<flush;
  cout<<"Agent Y Position:"<<posy<<"\n\n"<<flush;
  
  cout<<"---Interface---"<<"\n"<<flush;
  cout<<"In Perceptrons: "<<Interface.inperceptron.Size()<<"\n"<<flush;
  cout<<"Out Perceptrons: "<<Interface.outperceptron.Size()<<"\n"<<flush;
  cout<<"Max Links: "<<Interface.maxlinknum<<"\n"<<flush;
  cout<<"Initial Links: "<<Interface.initlinknum<<"\n"<<flush;
  cout<<"\n-Input Perceptron Info-\n"<<flush;
  for(int p = 1; p<=Interface.inperceptronnum; p++)
  {
    cout<<"Inperceptron("<<p<<")"<<endl;
    cout<<"State: "<<Interface.inperceptron(p).state<<
    " Source: "<<Interface.inperceptron(p).source<<
    " Target: "<<Interface.inperceptron(p).target<<"\n"<<flush;
  }
  cout<<"\n-Output Perceptron Info-\n"<<flush;
  for(int p = 1; p<=Interface.outperceptronnum; p++)
  {
    cout<<"outperceptron("<<p<<")"<<endl;
    cout<<"State: "<< Interface.outperceptron(p).state<<
    " Source: "<<Interface.outperceptron(p).source<<
    " Target: "<<Interface.outperceptron(p).target<<"\n"<<flush;
  }
  cout<<"-------------------------\n\n"<<endl;
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

TVector<Ray>& RDAgent::RefferenceRays()
{
  return Rays;
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
  // Local Vars
  double controltime = 0.0;

  //Read sensors into controller
  //cout<<"RDAgent::Step(): Resetting Rays"<<endl;
  ResetRays();// do this first
  //cout<<"RDAgent::Step(): Firing Input Perceptrons"<<endl;
  Interface.FireInputPerceptrons(object);
  while(controltime<=controllimit)
  {
    #ifdef DEBUG1
    cout<<"RDAgent::Step(): Making Euler Step| controller step: "<<controltime
    <<":"<<controllimit<<endl;
    #endif

    Controller.EulerStep(controldt);
    controltime+=controldt;
  }
  //Read controller to motors
  //cout<<"RDAgent::Step(): Firing Output Perceptrons"<<endl;
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
