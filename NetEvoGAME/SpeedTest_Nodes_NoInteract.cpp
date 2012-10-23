//
//  SpeedTest_Nodes_NoInteract.cpp
//  NE2_SIR
//
//  Created by BildPeter Visuals on 15.11.11.
//  Copyright (c) 2011 BildPeter Visuals. All rights reserved.
//

#include <netevo.h>
#include <iostream>
#include <lemon/time_measure.h>
#include "PeterDynamics.h"

using namespace netevo;
using namespace lemon;

/* --------------------------------------------------------------------------------------------------------
 TO DO: 
 -------------------------------------------------------------------------------------------------------- */



int main(){
    System          pSys;
    SIRdynamics     mDyn;
    double  StepSize = 0.001;
    double  tMax = 1.0;
    pSys.addNodeDynamic( &mDyn );
    
    vector< int     > outpGraphSize;
    vector< double  > outpRuntime;
    
    Timer T(true);
    cout << "Start!" << endl;
    
    ChangeLog   nullLog;
    SimObserver         nullObserver;
    SimulateOdeFixed    ODE_Solver( RK_4, StepSize);
    
    // ----- Loop of Graphsize -----
    for ( int GraphSize = 10000; GraphSize <= 1000000;  ){

        ListDigraph     lemonGraph;
        for ( int i = 0; i < GraphSize; i++) lemonGraph.addNode();
        
        pSys.copyDigraph( lemonGraph, "SIR_Dynamic", "NoArcDynamic");
        cout << "Nodes: " << countNodes( pSys ) << endl;
        cout << "Arcs: "  << countArcs( pSys )  << endl;
        
        // ----- Anfangsbedingungen abhängig von dem Grad des Knotens
        // Ein Wert, damit es nicht weg-optimiert wird
        State   init = State( pSys.totalStates(), 10);
                    
        T.restart();
        ODE_Solver.simulate( pSys, tMax, init, nullObserver, nullLog );
        outpRuntime.push_back( T.realTime() );
        outpGraphSize.push_back( GraphSize );
        cout << "Run Time: " << outpRuntime.back() << " Sek -> " << outpRuntime.back() / 60 << " min"<< endl;
        
        if ( GraphSize <= 90000 ) GraphSize += 10000;
        else GraphSize += 100000;
    }
    
    ofstream outFile( "/Users/sonneundasche/Dropbox/MartinCL/SpeedTests/Test_onlyNodeDyn_1000StepsTEST.txt" );
    outFile << "Nodes" << "\t" << "Time" << endl;
    
    auto iterGrSize = outpGraphSize.begin();
    for( auto iter = outpRuntime.begin(); iter != outpRuntime.end(); iter++ ){
        outFile << *iterGrSize << "\t" << *iter << endl;
        iterGrSize++;
    }
 
    outFile.close();
    
    cout << "finito...";
    
    return 0;
}


/*--------------------------------------------------------------------------------------------------------
 ----------------------------------------------------------------------------------------------------- */
