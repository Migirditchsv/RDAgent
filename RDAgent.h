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
const double       agentdt = 0.1;// agent time step size
// Controller
const int controllersize = 138;
const int controllermodel = 0;// 0:Grey-Scott
const int controllertopology = 0;// 0: 1D Near neigh. ring
const double controldt = 0.1;// controller step size
const int controllimit = 1;// controller steps per agent step

// Linker
const int maxlinks = 8;// Max # of links controller a perceptron may have
const int initlinks = 4;// number of links to controller a perceptron starts with

// motor
const int motorsize = 2;// number of motors needed to update agent state

// The RDAgent class declaration

class RDAgent {
	public:
		// The constructor 
		RDAgent(double ix = 0.0, double iy = 0.0) 
		{
			//init rays
			Rays.SetBounds(1, NumRays);

			//init controller
			Controller.SetReactorSize(controllersize);
			Controller.SetRDModel(controllermodel);
			Controller.SetReactorTopology(controllertopology);

			// init motor

			motor.SetBounds(1,motorsize);

			//init interface
			Printer(-7);// debug
			Interface.RefferenceInterface(Rays,// ref to sensor
										  Controller,//ref to controller
										  motor);//ref to motor
			Printer(-8);// debug
			Interface.SetLinkNum(maxlinks, initlinks);

            // set position
			Reset(ix,iy);

			// Update
			Printer(-1);
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
		TVector<double> motor;
		
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
