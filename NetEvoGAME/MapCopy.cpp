//
//  MapCopy.cpp
//  LemonGame
//
//  Created by BildPeter Visuals on 08.11.11.
//  Copyright (c) 2011 BildPeter Visuals. All rights reserved.
//

#include <netevo.h>
#include <iostream>
//#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>

using namespace std;
using namespace lemon;
using namespace netevo;

int main( void ){
    netevo::System      neSys;
    
    ListDigraph sourceGraph, targetGraph;
    ListDigraph::NodeMap< double >      *xCoordNodeMap;
    ListDigraph::NodeMap< double >      *targetNodeMap;
    ListDigraph::NodeMap< double >      *neNodeMap;

    // ----- NodeReference Map -----
    // Welcher Knoten kommt beim neuen Graphen heraus, wenn ich den alten Knoten/ID anspreche?
    ListDigraph::NodeMap< ListDigraph::Node >   nodeReferenceMap( sourceGraph );    
    ListDigraph::NodeMap< ListDigraph::Node >   neReferenceMap( neSys );
    
    xCoordNodeMap = new ListDigraph::NodeMap< double >( sourceGraph );
    targetNodeMap = new ListDigraph::NodeMap< double >( targetGraph );
    neNodeMap     = new ListDigraph::NodeMap< double >( neSys );
    
    
    try {
        digraphReader( sourceGraph, "/Users/sonneundasche/Desktop/euregio_Layout_Kamada_Kawai.lgf")
        .nodeMap( "xCoord" , *xCoordNodeMap)
        .run();
    } catch (lemon::Exception& error) { // check if there was any error
        std::cerr << "Error: " << error.what() << std::endl;
    }

    digraphCopy( sourceGraph, targetGraph )
    .nodeMap( *xCoordNodeMap, *targetNodeMap )
    .nodeRef( nodeReferenceMap )
    .run();
    


    
//    digraphCopy( sourceGraph, neSys )
//    .nodeMap( *xCoordNodeMap, *neNodeMap )
//    .nodeRef( neReferenceMap )
//    .run();
    
    DigraphCopy< ListDigraph, System> cg( sourceGraph, neSys );
    cg.nodeMap( *xCoordNodeMap, *neNodeMap );
    cg.nodeRef( neReferenceMap );
    cg.run();

    neSys.copyDigraph( sourceGraph );
    
    
    cout << " Source Graph | Nodes: " << countNodes( sourceGraph ) << "  -- Arcs: " << countArcs( sourceGraph ) << endl;
    cout << " Target Graph | Nodes: " << countNodes( targetGraph ) << "  -- Arcs: " << countArcs( targetGraph ) << endl;
    cout << " NetEvo Graph | Nodes: " << countNodes( neSys ) << "  -- Arcs: " << countArcs( neSys ) << endl;
    

    for (int i = 0;  i < countNodes( sourceGraph ) ; i++) {
//        cout << "ID: " << i << " \t| Source: " << (*xCoordNodeMap)[ sourceGraph.nodeFromId( i ) ] << " \t| Target: " << (*targetNodeMap)[ targetGraph.nodeFromId( i ) ] << endl;
        cout << "ID: "  << i << " \t| Src: " << (*xCoordNodeMap)[ sourceGraph.nodeFromId( i ) ] 
                        << " \t| Target: " << (*targetNodeMap)[ nodeReferenceMap[ targetGraph.nodeFromId( i ) ] ]
                        << " \t| NE sys: " << (*neNodeMap)[ neReferenceMap[ neSys.nodeFromId( i ) ] ] 
                        << endl;
    }

}