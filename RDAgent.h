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
const double BodySize = 30.0; 
const double EnvWidth = 400.0;
const double MaxRayLength = 220.0;
const double InputGain = 1.0; //Maybe remove
const double VisualAngle = Pi/6;
const double VelGain = 5;
const int 	 NumRays = 7;
const int    ActuatorNum = 2;
double       agentdt = 0.1;// agent time step size
// Controller
const int controllersize = 138;
const int controllermodel = 0;// 0:Grey-Scott
const int controllertopology = 0;// 0: 1D Near neigh. ring
double controldt = 0.1;// controller step size
int controllimit = 1;// controller steps per agent step

// Linker
int maxlinks = 8;// Max # of links controller a perceptron may have
int initlinks = 4;// number of links to controller a perceptron starts with

// actuator
int actuatorsize = 2;// number of actuators needed to update agent state

// The RDAgent class declaration

class RDAgent {
	public:
		// The constructor 
		RDAgent(double ix = 0.0, double iy = 0.0) 
		//: Interface(Interface), Control(Control)
		{
			//init rays
			Rays.SetBounds(1, NumRays);

			//init controller
			Controller.SetReactorSize(controllersize);
			Controller.SetRDModel(controllermodel);
			Controller.SetReactorTopology(controllertopology);

			// init actuator
			actuator.SetBounds(1,actuatorsize);

			//init interface
			Interface.RefferenceInterface(Rays,// point to sensor
									Controller,//point to controller
									actuator);//point to actuator
			Interface.SetLinkNum(maxlinks,// link limit
								initlinks);// initial number of links

            // set position
			Reset(ix,iy);
		};

		// The destructor
		~RDAgent() {}

		// Accessors
		void Printer(int linenum); // print state to terminal
		void Writer();// write state to file
		void SetTimeResolutions(double controldt_,// time delta on controller
		                                  double controllimit_,// limit on controller steps per agent step
		                                  double agentdt_// time delta on agent motion
		);
		double PositionX() {return cx;};
		void SetPositionX(double newx);
		double PositionY() {return cy;};
        double SensorState(int rayindx);
        //^Returns the length ratios of the ray components about the point(1)
        //of intersection.
		void Reset(double ix, double iy, int randomize = 0);
        void Reset(RandomState &rs, double ix, double iy, int randomize);
		void ResetRays();
		void Step( VisualObject &object);

        // declare Controller
		RDControl Controller;//(int controllersize, int controllermodel);

		// declare Actuator
		TVector<double> actuator;
		
		// declare Interface
		AgentInterface Interface;
        
        // Data Output
        void SlowPrintState();
        void PrintConfiguration();


	private:


		int NumRays;
		double cx, cy, vx;
		TVector<Ray> Rays;
		double controldt;// step size on controller
		double controllimit;// number of controller time steps per agent step
		double agentdt;// step size on agent
        
        // Evolutionary Targets
        
        TMatrix<double> InputTargets;//NumRaysXMaxLinks: Controller element targeted
        TMatrix<double> InputSpecies;//NumRaysXMaxLinks: chemical species to inject
        TMatrix<double> InputWeight;//NumRaysXMaxLinks: Input[0,1]scalefactor [-1,1]
        TMatrix<double> outputTargets;
        TMatrix<double> outputSpecies;//NumRaysXMaxLinks: chemical species to read
        TMatrix<double> outputWeight;//NumRaysXMaxLinks:output[0,1]scalefactor[-1,1]
        // Note: Outputs are rounded to fit into the interval [0,maxdouble]
        // after summation.


};
