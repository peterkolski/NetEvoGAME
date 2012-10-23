//
//  NE2_Reference_LEMON_Map.cpp
//
//  Created by BildPeter Visuals on 28.11.11.
//  Copyright (c) 2011 BildPeter Visuals. All rights reserved.
//

#include <iostream>
#include <netevo.h>
#include <lemon/random.h>

using namespace netevo;
using namespace lemon;


/*
 In this example it is intended to copy the values from a node maps of a LEMON graph
 into a NetEvo system, which was copied from the original LEMON graph via .copyDigraph()
 
 To do this, a cross reference map stores the nodes of the original LEMON system, and 
 can be accessed by the current NetEvo system node.
 */


int main() {
    
    // ----- Create a LEMON graph and a node map to store values -----
    ListDigraph                     lemonSourceGraph;
    ListDigraph::NodeMap< int >     lemonSourceNodeMap( lemonSourceGraph );

    // ----- Do the same for a NetEvo system  -----
    System                          netevoSys;
    System::NodeMap< int >          netevoTargetNodeMap ( netevoSys );

    // ----- Create empty cross reference maps for the NetEvo system storing source nodes  -----
    ListDigraph::NodeMap< ListDigraph::Node >  refNodeMap( netevoSys );
    ListDigraph::ArcMap< ListDigraph::Arc >    refArcMap(  netevoSys ); 
    
    // ----- Create some values for the LEMON node map -----
    for ( ListDigraph::NodeIt lNode (lemonSourceGraph ); lNode != INVALID; ++lNode ){
        lemonSourceNodeMap[ lNode ] = rand();
    }

    // ----- Copy the LEMON graph into a NetEvo System, passing through the reference maps -----
    netevoSys.copyDigraph( lemonSourceGraph, "InteractionMap", "NoArcDynamic", refNodeMap, refArcMap);
    
    // ----- Assigning the source values to the new NetEvo system  -----
    for (System::NodeIt neNode( netevoSys ); neNode != INVALID; ++neNode ){
        netevoTargetNodeMap[ neNode ] = lemonSourceNodeMap [ refNodeMap[ neNode ] ] ;
        cout << netevoTargetNodeMap[ neNode ] << endl;
    }
    
    
    return 0;
}
