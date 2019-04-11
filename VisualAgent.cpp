// ***********************************************************
//  Methods for a VisualAgent
//
//  Matthew Setzler 4/19/17
// ************************************************************

#include "VisualAgent.h"

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
// Control
// *******

// Change x-position

void VisualAgent::SetPositionX(double newx) {
  cx = newx;
  ResetRays();
}

// Reset the state of the agent

void VisualAgent::Reset(double ix, double iy, int randomize) {
  cx = ix; cy = iy; vx = 0.0;
  if (randomize) Controller.RandomizeReactorState();
  else Controller.HomogenousReactorState();
  ResetRays();
}

void VisualAgent::Reset(RandomState &rs, double ix, double iy, int randomize) {
  cx = ix; cy = iy; vx = 0;
  if (randomize) Controller.RandomizeReactorState();
  else Controller.HomogenousCircuitState();
  ResetRays();
}

void VisualAgent::ResetRays() {
  double theta = -VisualAngle/2;  
  for (int i=1; i<=NumRays; i++) {
    ResetRay(Rays[i], theta, cx, cy);
    theta += VisualAngle/(NumRays-1);
  }
}


// Step the agent

void VisualAgent::Step(double StepSize, VisualObject &object) { 
  // Update visual sensors and check inputs
  ResetRays();
  for (int i=1; i<=NumRays; i++) {
    object.RayIntersection(Rays[i]);
    double external_input = InputGain*(MaxRayLength - Rays[i].length)/MaxRayLength;
    //Replace following line with injector function having a sensor-cell linker.
    //Controller.SetNeuronExternalInput(i, external_input);
  }

  // Step nervous system
  Controller.EulerStep(StepSize);

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
