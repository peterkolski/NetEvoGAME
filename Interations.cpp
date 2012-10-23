//
//  Interations.cpp
//  NE2_SIR
//
//  Created by Peter A. Kolski on 11.11.11.
//  Copyright (c) 2011 BildPeter Visuals. All rights reserved.
//

#include <iostream>
#include <list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/adaptors.h>
#include <lemon/time_measure.h>

using namespace lemon;
using namespace std;

int main(){
    ListDigraph     lemonGraph;
    
    
    // ----- Time measurement
    Timer T(true);
    Timer Tlocal(true);
    cout << "Start!" << endl;
    
    // ----- Read the Graph from file to a LEMON System
    readGraph( lemonGraph, "/Users/sonneundasche/Dropbox/NetEvo/21 LEMON Graphs/super_lemon_schaf.lgf");
    cout << "Nodes: " << countNodes( pSys ) << endl;
    cout << "Arcs: "  << countArcs( pSys )  << endl;
    

    // ----- Logger & Observer
//    ChangeLog   nullLog;
//    vector<double> tOut;
//    vector<State>  xOut;
//    SimObserverToVectors vectorObserver(xOut, tOut);
//    SimObserverToStream obs( cout );
//    SimObserver         nullObserver;
//    
//    
//    // ----- Variables
//    double StepSize = 0.001;
//    bool endSim = false;
//    double tMax = 5.0;
//    int     globalSteps = 0;
//    double t = 0.0;
//    double thresholdTerminate = 0.1;
//    double sizeOfEpidemic = 0.0;
//    double lastSizeOfEpidemic = -1;
//    int    StateParamsAmount  = mDyn.getStates();
//    SimulateOdeFixed    ODE_Solver( RK_4, StepSize);
//    SimulateMap         InterActionMap;
//    SimulateOdeStep     ODE_Stepper( StepSize );
    
    cout << "Initialisation Time: " << T.realTime() << endl << endl;
    cout << "StepSize: " << StepSize << " --- Maximum time: " << tMax << " --- Steps: " << tMax / StepSize << endl << endl;
    cout << " --- Start Simulation!!! ---" << endl << endl;
    cout << "Initial Size of Epidemic: " << sizeOfEpi( init, 1, 3 ) << endl;
    T.restart();    // Start the timer
   

}
