//*****************************************************************************
// PerceptronInterface: Uses perceptrons to link:
// (ENVIRONMENNT)->[IN-PERCEPTRONS]->(Controller)->[Out-Perceptrons]->(Actuators)
// 
// Where sensor and Actuator are TVector objects and Controller is a
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


// Debug controls
#define DEBUGAGENTINTERFACE

// **************************** 
// Includes
// ****************************

// Std. libs
#include <iostream>// Debug cout,
#include <vector>// in/out perceptron groups
//#include <cmath>

// Headers
#include "VectorMatrix.h"
#include "AgentInterface.h"
#include "RDAgent.h"// call to sensors (rays) should replace at some point
// ^ Theoretically unneeded refference. 
#include "random.h"

// **************************** 
// Accessors
// ****************************
void AgentInterface::SetLinkNum(int ml, int il)
{
    maxlinknum = ml;
    initlinknum = il;
    // Forcinly resize in and out perceptron target/source and weight arrays
    // in perc resize
    for(int p=1; p<= inperceptronnum; p++)
    {
        inperceptron(p).target.SetBounds(1,maxlinknum);
        inperceptron(p).weight.SetBounds(1,maxlinknum);
    }

    // out perc resize
    for(int p=1; p<= outperceptronnum; p++)
    {
        outperceptron(p).source.SetBounds(1,maxlinknum);
        outperceptron(p).weight.SetBounds(1,maxlinknum);
    }
 
    cout<<"AgentInterface:: SetLinkNum COMPLETE"<<endl;
}
// **************************** 
// Utility functions
// ****************************

void AgentInterface::RefferenceInterface(TVector<Ray>& sensor_,
                            RDControl& controller_,
                            TVector<double>& actuator_)
        {
            sensor = sensor_;
            controller = controller_;
            actuator = actuator_;

            // Compute sizes for initialization
            inperceptronnum      = sensor.Size();
            controllersize  = controller.GetReactorSize();
            controllerdimension = controller.GetChemicalNumber();
            outperceptronnum    = actuator.Size();

            // Initialize Input TVector
            inperceptron.SetBounds(1,inperceptronnum);
            for(int p=1; p<=inperceptronnum; p++)
            {
                inperceptron(p).channel = 1; // Default to channel 1
                inperceptron(p).source.SetBounds(1,1);//only one source
                inperceptron(p).source.FillContents(p); // one perceptron per sense organ
                inperceptron(p).state   = 0.0; // start clean
                inperceptron(p).target.SetBounds(1,maxlinknum); //size target list
                inperceptron(p).target.FillContents(0); // targets <=0 are skipped
                inperceptron(p).weight.SetBounds(1,maxlinknum); // size weight list
                inperceptron(p).weight.FillContents(1.0); //Index determines skip, nonzero default weights increase evolvability
            }

            // Initialize output TVector
            outperceptron.SetBounds(1,outperceptronnum);
            for(int p=1; p<=outperceptronnum; p++)
            {
                outperceptron(p).channel = 1; // Default to channel 1
                outperceptron(p).source.SetBounds(1,maxlinknum);
                outperceptron(p).source.FillContents(0);// Default off all read out links
                outperceptron(p).state   = 0.0; // start clean
                outperceptron(p).target.SetBounds(1,1); //size target list
                outperceptron(p).target.FillContents(p); // targets <=0 are skipped
                outperceptron(p).weight.SetBounds(1,maxlinknum); // size weight list
                outperceptron(p).weight.FillContents(1.0); //Index determines skip, nonzero default weights increase evolvability
            }
            cout<<"RefferenceInterface: COMPLETE"<<endl;//debug

          //// Set Initial Values 
          //SetRandomInputLinks(); 
          //cout<<"Input links randomized"<<endl;// debug
          //SetRandomOutputLinks();
          //cout<<"Output links randomied"<<endl;// debug


            };// end constructor
 
 void AgentInterface::SetRandomInputLinks()
 {
     //vars
    int linkindx;
    double weight;

    // Refresh controllersize
    controllersize = controller.GetReactorSize();
    for(int p=1; p<=inperceptronnum; p++)
    {
        for(int l=1; l<=initlinknum; l++)
        {
           linkindx = UniformRandomInteger(1,controllersize);
           inperceptron(p).target(l)=linkindx;
           weight = UniformRandom(0,1);
       }
    }
    cout<<"AgentInterface::SetRandomInputLinks COMPLETE"<<endl;
 }

 void AgentInterface::SetRandomOutputLinks()
 {
     //vars
     int linkindx;
     double weight;

     for(int p=1; p<=outperceptronnum; p++)
     {
         for(int l=1; l<=initlinknum; l++)
         {
            linkindx = UniformRandomInteger(1,controllersize);
            outperceptron(p).source(l)=linkindx;
            weight = UniformRandom(0,1);
            outperceptron(p).weight(l)=weight;
         }
     }
    cout<<"AgentInterface::SetRandomOutputLinks COMPLETE"<<endl;
 }

// Reset Interface: Resets links and weights to random values
void AgentInterface::ResetInterface()
{
    // refresh controller size
    controllersize = controller.GetReactorSize();

    // input layer
    for(int p=1; p<inperceptronnum; p++ )
    {
        // Fill initilized links
        for( int t=0; t<initlinknum; t++ )
        {
        // random target
        int randomtarget = UniformRandomInteger(1, controllersize);
        inperceptron(p).target(t) = randomtarget;
        // random weight for target
        double randomweight = UniformRandom(0.0,1.0);
        inperceptron(p).weight(t) = randomweight;
        }//end fill init links
        // Fill uninitialized links
        for( int t=initlinknum; t<maxlinknum; t++ )
        {
            inperceptron(p).target(t)=-1;
            inperceptron(p).weight(t)=0;
        }// end fill non-init links
    }// end input layer 

    // output layer
    for(int p=1; p<outperceptronnum; p++ )
    {
        // Fill initilized links
        for( int s=0; s<initlinknum; s++ )
        {
        // random source
        int randomsource = UniformRandomInteger(1, controllersize);
        outperceptron(p).source(s) = randomsource;
        // random weight for source 
        double randomweight = UniformRandom(0.0,1.0);
        inperceptron(p).weight(s) = randomweight;
        }//end fill init links
        // Fill uninitialized links
        for( int s=initlinknum; s<maxlinknum; s++ )
        {
            inperceptron(p).target(s)=-1;
            inperceptron(p).weight(s)=0;
        }// end fill non-init links
    }// end input layer 
}

// inject relevant weighted sensor value to target controller elements
void AgentInterface::FireInputPerceptrons(VisualObject &object)
{
    //vars
    int sourceindx, channelindx, targetindx;
    double externalinput;
    // Loop over in perceptrons
    for(int p=1; p<=inperceptronnum; p++)
    {
        // Reset perceptron state
        inperceptron(p).state=0.0;
        
        // Set index routes
        sourceindx = inperceptron(p).source(1);
        channelindx = inperceptron(p).channel;

        #ifdef DEBUGAGENTINTERFACE
        cout<<"AgentInterface::FireInputPerceptron indicies read"<<endl;
        #endif

        // measure sensor state
        object.RayIntersection(sensor(sourceindx));
        externalinput = (MaxRayLength - sensor(sourceindx).length)/MaxRayLength;      
        inperceptron(p).state = externalinput;

        #ifdef DEBUGAGENTINTERFACE
        cout<<"AgentInterface::FireInputPerceptron External Input: "<<externalinput
        <<"\n     (MaxRayLength[ "<<MaxRayLength<<" ] - sensor( sourceindx[ "<<sourceindx
        <<"] ).length["<<sensor(sourceindx).length<<"] ) / MaxRayLength"
        <<(MaxRayLength - sensor(sourceindx).length)/MaxRayLength<<endl;
        #endif

        // inject into controller targets
        for(int target=1; target<=maxlinknum; target++)
        {
           targetindx = inperceptron(p).target(target); 
           // check for placeholder indx
           if(targetindx<=0){goto skip;}

            #ifdef DEBUGAGENTINTERFACE
            cout<<"AgentInterface::FireInputPerceptron\n    targetindx"<<targetindx
            <<"\n   channelindx"<<channelindx
            <<"\n   controller.GetReactorSize(): "<<controller.GetReactorSize()
            <<"\n   controller.GetChemicalNumber(): "<<controller.GetChemicalNumber()
            <<endl;
            #endif

           // inject
           controller.InjectCell(externalinput,targetindx, channelindx);
           skip:;
        }//end target loop
    }//end perceptron loop
    #ifdef DEBUGAGENTINTERFACE
    cout<<"AgentInterface::FireInputPerceptrons Post Fire Reactor State:\n"
    <<controller.GetReactorState()<<endl;
    #endif
}// end FireInputPerceptrons

void AgentInterface::FireOutputPerceptrons()
{
    // vars
    int sourceindx, channelindx, targetindx;
    double weight, internalstate;

    // clear actuator state
    actuator.FillContents(0.0);

    // loop over out percs
    for(int p=1; p<=outperceptronnum; p++)
    {// loop over percs

        // Clear perceptronstate
        outperceptron(p).state=0.0;

        // get indicies
        channelindx = outperceptron(p).channel;
        targetindx  = outperceptron(p).target(1);

        for(int source=1;source<=maxlinknum;source++)
        {//loop over sources & accumulate
            // Get sourceindx
            sourceindx = outperceptron(p).source(source);
            // check for placeholder index
            if(sourceindx<=0){goto skipout;}

            // get link weight
            weight = outperceptron(p).weight(source);
            //read from controller
            internalstate = controller.CellStateChannel(sourceindx, channelindx);
            #ifdef DEBUGAGENTINTERFACE
            if(internalstate==0.0)
            {
                cerr<<"AgentInterface::FireOutpurPerceptrons internal state= "<<internalstate
                <<"highly unlikely.\n   manual check:\n    controller.cellstate("<<
                sourceindx<<","<<channelindx<<")="<<controller.CellStateChannel(sourceindx,channelindx)
                <<"ReactorState:\n"<<controller.GetReactorState()<<endl;
            }
            #endif
            outperceptron(p).state+= weight * internalstate;

           // #ifdef DEBUGAGENTINTERFACE
           // cout<<"AgentInterface::FireOutputPerceptrons p: "<<p<<" sourceindx: "<<sourceindx
           // <<"\n weight: "<<weight
           // <<"\n internal state (controller): "<<internalstate
           // <<"\n outperceptron.state= "<<outperceptron(p).state
           // <<endl;
           // #endif

            //skipout from source loop
            skipout:;
        }// end source loop

        // Write state to acutator
        actuator(targetindx)+=outperceptron(p).state;

    }//end perc loop

    #ifdef DEBUGAGENTINTERFACE
    cout<<"actuator: "<<actuator<<endl;
    #endif

}// End FireOutPerceptrons

