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
#include "VisualObject.h"
#include "random.h"
#include "TVector.h"

// Global constants

const double Pi = 3.1415926535897;
const double BodySize = 30.0; // diameter of agent
const double EnvWidth = 400.0;
const double MaxRayLength = 220.0;
const double InputGain = 10.0;
const double VisualAngle = Pi/6;
const double VelGain = 5;

// Evolutionary Targets

TMatrix<double> inperceptron; // NumRays X MaxLinks X 3(targetcell,chemindx,weight)
TMatrix<double> outperceptron; // NumRays X MaxLinks X 3(targetcell,chemindx,weight)

// The VisualAgent class declaration

class VisualAgent {
	public:
		// The constructor 
		VisualAgent(double ix = 0.0, double iy = 0.0, int NumRays_ = 7) {
            // Define Rays
			NumRays = NumRays_;
			Rays.SetBounds(1, NumRays);

            // Define links
            SetInputPerceptronLinks();
            SetOutputPerceptronLinks();
            // Initialize position
			Reset(ix,iy);
		};

		// The destructor
		~VisualAgent() {}

		// Accessors
		double PositionX() {return cx;};
		void SetPositionX(double newx);
		double PositionY() {return cy;};

		// Agent State Control
		void Reset(double ix, double iy, int randomize = 0);
        void Reset(RandomState &rs, double ix, double iy, int randomize);
		void Step(double StepSize, VisualObject &object);

        // Controller Interface
		RDControl Controller();
        
        void SetInputPerceptronLinks();
        void SetOutputPerceptronLinks();
        void FireInputPerceptrons();
        void FireOutputPerceptrons();

        // Output
        void SlowPrintState();
        void PrintConfiguration();


	private:
		void ResetRays();

		int NumRays;
		double cx, cy, vx;
		TVector<Ray> Rays;
};
