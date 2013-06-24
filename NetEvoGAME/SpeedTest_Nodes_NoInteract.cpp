//
//  SpeedTest_Nodes_NoInteract.cpp
//  NE2_SIR
//
//  Created by Peter A. Kolski on 15.11.11.
//  Copyright (c) 2011 Peter A. Kolski.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  For the GNU General Public License see <http://www.gnu.org/licenses/>.
//
//
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
    SIRdynamic      mDyn( 20, 1 );
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
