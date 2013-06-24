//
//  MapCopyDyn.cpp
//  LemonGame
//
//  Created by Peter A. Kolski on 23.11.11.
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
//#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include "/Users/sonneundasche/Programmierung/lpz/peterchen/EXTRA/LemonGame/LemonGame/PeterDynamics.h"


using namespace std;
using namespace lemon;
using namespace netevo;

int main( void ){
    netevo::System      neSys;
    
    ListDigraph sourceGraph, targetGraph;
    ListDigraph::NodeMap< double >      xCoordNodeMap( sourceGraph );
    ListDigraph::NodeMap< double >      *targetNodeMap;
    ListDigraph::NodeMap< double >      *neNodeMap;
    
    SIRdynamics sirDyn;
    neSys.addNodeDynamic( &sirDyn );
    
    // ----- NodeReference Map -----
    // Welcher Knoten kommt beim neuen Graphen heraus, wenn ich den alten Knoten/ID anspreche?
    ListDigraph::NodeMap< ListDigraph::Node >   nodeReferenceMap( sourceGraph );    
    ListDigraph::NodeMap< ListDigraph::Node >   neReferenceMap  ( neSys );
    ListDigraph::ArcMap < ListDigraph::Arc >   neArcRefMap     ( neSys );
    
    targetNodeMap = new ListDigraph::NodeMap< double >( targetGraph );
    neNodeMap     = new ListDigraph::NodeMap< double >( neSys );
    
    
    try {
        digraphReader( sourceGraph, "/Users/sonneundasche/Desktop/euregio_Layout_Kamada_Kawai.lgf")
        .nodeMap( "xCoord" , xCoordNodeMap)
        .run();
    } catch (lemon::Exception& error) { // check if there was any error
        std::cerr << "Error: " << error.what() << std::endl;
    }
    
    digraphCopy( sourceGraph, targetGraph )
    .nodeMap( xCoordNodeMap, *targetNodeMap )
    .nodeRef( nodeReferenceMap )
    .run();
    
    
    
        neSys.copyDigraph( sourceGraph, "SIR_Dynamic", "NoArcDynamic", neReferenceMap, neArcRefMap );
    
    //    digraphCopy( sourceGraph, neSys )
    //    .nodeMap( *xCoordNodeMap, *neNodeMap )
    //    .nodeRef( neReferenceMap )
    //    .run();
    
//    DigraphCopy< ListDigraph, System> cg( sourceGraph, neSys );
//    cg.nodeMap( *xCoordNodeMap, *neNodeMap );
//    cg.nodeRef( neReferenceMap );
//    cg.run();
    
    
    cout << " Source Graph | Nodes: " << countNodes( sourceGraph ) << "  -- Arcs: " << countArcs( sourceGraph ) << endl;
    cout << " Target Graph | Nodes: " << countNodes( targetGraph ) << "  -- Arcs: " << countArcs( targetGraph ) << endl;
    cout << " NetEvo Graph | Nodes: " << countNodes( neSys ) << "  -- Arcs: " << countArcs( neSys ) << endl;
 
    // ----- Copy old Map into a new one and compare -----
    
    System::NodeMap< double > newNetevoMap ( neSys );
    for (ListDigraph::NodeIt n( neSys ); n!= INVALID; ++n){
        newNetevoMap[ n ] = xCoordNodeMap[ neReferenceMap[ n ] ];
    }
    
    // ----- Output all the Maps ------
    for (int i = 0;  i < countNodes( sourceGraph ) ; i++) {
        //        cout << "ID: " << i << " \t| Source: " << xCoordNodeMap[ sourceGraph.nodeFromId( i ) ] << " \t| Target: " << (*targetNodeMap)[ targetGraph.nodeFromId( i ) ] << endl;
        cout << "ID: "  << i << " \t| Src: " << xCoordNodeMap[ sourceGraph.nodeFromId( i ) ]     << " || " << countOutArcs( sourceGraph, sourceGraph.nodeFromId( i ) )
        << " \t| Target: " << (*targetNodeMap)[ nodeReferenceMap[ sourceGraph.nodeFromId( i ) ] ]
        << " \t| NE sys: " << newNetevoMap[ neSys.nodeFromId( i ) ]           << " || " << countOutArcs( neSys, neSys.nodeFromId( i ) )
        << endl;
    }
    

    
    // ----- RUN SIMULATION -----
/*
    SimObserverToStream obs(cout);
    ChangeLog   nullLogger;
    SimulateOdeFixed    ODE_Solver( RK_4, 0.1);
    
    State   init = State( neSys.totalStates(), 10);
    ODE_Solver.simulate( neSys, 0.1, init, obs, nullLogger );
*/
    
}