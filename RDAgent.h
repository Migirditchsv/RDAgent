// ***********************************************************
//  A class for Reaction Diffusion controlled agents with
//  simple ray vision-like sensors. Based on code by Matthew
//  Setzler.
//
//  By: Sam Migirditch
//
//  05/19| SVM: Converted from VisualAgent to RDAgent
// ************************************************************

#include "RDControl.h"
#include "AgentInterface.h"
#include "VisualObject.h"
#include "random.h"
#include "VectorMatrix.h"

#pragma once

// Global constants
const double Pi = 3.1415926535897;

//***************************
// Agent Settings
//***************************
// Agent
const double BodySize = 0.1; 
const double EnvWidth = 1.0;
const double MaxRayLength = 3.0; // should be tied to RDTaskMain::particlestartheight
const double InputGain = 1.0; //Maybe remove
const double VisualAngle = Pi/4;
const int 	 NumRays = 5;
const int    ActuatorNum = 2;
const double agentdt = 0.1;// agent time step size
const double defaultxpos = 0.0; // x position at initialization
const double defaultypos = 0.0; // y position initilization

// motor
const int motorsize = 2;// number of motors needed to update agent state

// The RDAgent class declaration

class RDAgent {
	public:
		// The constructor 
		RDAgent() 
		{
			cout<<"Num Rays: "<<NumRays<<endl;
			//init rays
			Rays.SetBounds(1, NumRays);
			cout<<"RDAgent::Constructor: Ray Size Set: "<<Rays.Size()<<endl;

			//init controller
			//Controller.SetReactorSize(controllersize);
			//Controller.SetRDModel(controllermodel);
			//Controller.SetReactorTopology(controllertopology);

			// init motor
			motor.SetBounds(1,motorsize);
			cout<<"RDAgent::Constructor: Motor Size Set: "<<motor.Size()<<endl;

		//	//init interface
		//	Interface.RefferenceInterface(Rays,// ref to sensor
		//								  Controller,//ref to controller
		//								  motor);//ref to motor
		//	Interface.SetLinkNum(maxlinks, initlinks);

            // set position
			Reset(defaultxpos,defaultypos);

			// Update
			Printer(-1);
		};

		// The destructor
		~RDAgent() {}// probably should be made more less bad.

		// Accessors
		void Printer(int linenum); // print state to terminal
		void Writer();// write state to file
		int  GetEnvWidth() {return EnvWidth;}
		void SetTimeResolutions(double controldt_,// time delta on controller
		                                  double controllimit_,// limit on controller steps per agent step
		                                  double agentdt_// time delta on agent motion
		);
		double PositionX() {return cx;};
		void SetPositionX(double newx);
		double PositionY() {return cy;};
		double GetBodySize() {return BodySize;};
		double GetRayLength() {return MaxRayLength;};
        double SensorState(int rayindx);
        //^Returns the length ratios of the ray components about the point(1)
        //of intersection.
		void Reset(double ix, double iy, int randomize = 0);
        void Reset(RandomState &rs, double ix, double iy, int randomize);
		TVector<Ray>& RefferenceRays();// returns a refference to Rays
		void ResetRays();
		void Step( VisualObject &object);

        // declare Controller
		RDControl Controller;//(int controllersize, int controllermodel);

		// declare Actuator
		TVector<double> motor;

		// declare Rays
		TVector<Ray> Rays;
		
		// declare Interface
		AgentInterface Interface;
        
        // Data Output


	private:


		double cx, cy, vx;

		double controldt = 0.1;// step size on controller
		double controllimit = 1;// number of controller time steps per agent step
		double agentdt;// step size on agent
        
};
