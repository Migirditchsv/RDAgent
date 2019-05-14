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
#include "VectorMatrix.h"

#pragma once

// Global constants

const double Pi = 3.1415926535897;
// diameter of agent
const double BodySize = 30.0; 
const double EnvWidth = 400.0;
const double MaxRayLength = 220.0;
const double InputGain = 10.0;
const double VisualAngle = Pi/6;
const double VelGain = 5;
// Max # of links to RDCells a whisker may have. Each starts with half as many random links
const double maxlinks = 8;



// The RDAgent class declaration

class RDAgent {
	public:
		// The constructor 
		RDAgent(double ix = 0.0, double iy = 0.0, int NumRays_ = 7) {
            // Define Rays
			NumRays = NumRays_;
			Rays.SetBounds(1, NumRays);

            // Define links
            // DEBUG: NEED TO DO THE POINTER SHIT FOR 3D MATRICIES.
            inperceptron.SetBounds(1,NumRays,1,maxlinks,2);//(species,weight)
            inperceptron.FillContents(-1);
            outperceptron.SetBounds(1,NumRays,1,maxlinks,2);
            outperceptron.FillContents(-1);
            SetInputPerceptronLinks();
            SetOutputPerceptronLinks();
            // Initialize position
			Reset(ix,iy);
		};

		// The destructor
		~RDAgent() {}

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
        
        // Evolutionary Targets
        
        TMatrix<double> inperceptron;//NumRaysXMaxLinksX3(targetcell,chemindx,weight)
        TMatrix<double> outperceptron;//NumRaysXMaxLinksX3(targetcell,chemindx,weight)

};